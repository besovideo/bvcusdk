/*
 * 本类用于Dialog相关操作
 * 包括：Dialog创建、关闭、截图、录像及对应的事件处理
 */

using System;
using System.Collections;
using System.Windows.Forms;
using System.Text;

namespace WindowsFormsTest
{
    class Dialog
    {
        /*
         * 用于本地存储一个Dialog对话
         */
        public class OneDialog
        {
            public Panel panel;         // 用于显示预览的窗口
            public IntPtr dialogHandle; // BVCU SDK返回的对话句柄
            public Session.Pu pu;       // 对话所属的Pu设备
            public int channelNo;       // 对话所在的通道
            public DateTime timeStamp;  // 对话当前显示帧的时间戳
            public bool recording;      // 是否正在录像
            public OneDialog()
            {
                dialogHandle = IntPtr.Zero;
            }
        }

        /*
         * 创建的所有Dialog列表
         */
        ArrayList m_dialogs;

        /*
         * 用于在打开的Dialog的预览画面中渲染字体
         */
        AfterRender m_afterRender;

        /*
         * 全局变量
         */
        IntPtr m_bvsdkHandle;
        EventHandler m_bvsdkEventHandler;
        Form1 m_mainForm;
        Session m_session;

        public Dialog(IntPtr sdkHandle, EventHandler eventHandler, Form1 mainForm, Session session)
        {
            m_bvsdkHandle = sdkHandle;
            m_bvsdkEventHandler = eventHandler;
            m_mainForm = mainForm;
            m_session = session;
            m_dialogs = new ArrayList();
            m_afterRender = new AfterRender(this);
            openDialogFailed = new DialogHandleInDel(procOpenDialogFailed);
            m_gpsDialogs = new ArrayList();
            m_tspDialogs = new ArrayList();
        }

        /*
         * 创建的总对话(Dialog)数
         */
        public int Count
        {
            get
            {
                return m_dialogs.Count;
            }
        }

        /*
         * 查询函数
         */
        public OneDialog getDialog(IntPtr handle)
        {
            foreach (OneDialog dialog in m_dialogs)
            {
                if (dialog.dialogHandle == handle)
                {
                    return dialog;
                }
            }
            return null;
        }

        public OneDialog getDialog(Panel panel)
        {
            foreach (OneDialog dialog in m_dialogs)
            {
                if (dialog.panel.Equals(panel))
                {
                    return dialog;
                }
            }
            return null;
        }

        /*
         * 打开对话(Dialog)
         */
        public void openBrowse(Session.Pu pu, int channelNo, Panel panel)
        {
            try
            {
                BVCU_DialogControlParam_Network net = new BVCU_DialogControlParam_Network(0, 5, 1, 3);
                BVRect dispRect = new BVRect(0, 0, panel.ClientRectangle.Height, panel.ClientRectangle.Width);
                OneDialog dlg = new OneDialog();
                foreach (Session.Channel channel in pu.channelList)
                {
                    int volume = -1;
                    if (channel.channelNo == channelNo)
                    {
                        if (channel.audioPlayback)
                        {
                            volume = 0;
                        }
                        BVCU.FAILED(
                            BVCU.ManagedLayer_CuBrowsePu(m_bvsdkHandle, ref dlg.dialogHandle,
                            m_session.Handle, Encoding.UTF8.GetBytes(pu.id), channelNo,
                            panel.Handle, ref dispRect,
                            volume, 0, Encoding.UTF8.GetBytes(""), true, ref net,
                            m_bvsdkEventHandler.dialog_OnDialogEvent,
                            m_bvsdkEventHandler.dialog_OnStorageEvent));
                        break;
                    }
                }
                dlg.panel = panel;
                dlg.pu = pu;
                dlg.channelNo = channelNo;
                m_dialogs.Add(dlg);
            }
            catch
            {
                panel.ContextMenuStrip = null;
                MessageBox.Show("打开预览失败");
            }
        }

        /*
         * 关闭对话(Dialog)
         */
        public void closeBrowse(Panel panel)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.panel == panel)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    m_dialogs.Remove(dlg);
                    break;
                }
            }
        }

        /*
         * 截图
         */
        public void capture(Panel panel)
        {
            if (m_mainForm.CapturePath.Length == 0)
            {
                MessageBox.Show("未设置截图路径");
                return;
            }
            try
            {
                foreach (OneDialog dlg in m_dialogs)
                {
                    if (dlg.panel == panel)
                    {
                        string path = m_mainForm.CapturePath + "\\"
                            + dlg.timeStamp.Year + dlg.timeStamp.Month.ToString().PadLeft(2, '0')
                            + dlg.timeStamp.Day.ToString().PadLeft(2, '0') + dlg.timeStamp.Hour.ToString().PadLeft(2, '0')
                            + dlg.timeStamp.Minute.ToString().PadLeft(2, '0') + dlg.timeStamp.Second.ToString().PadLeft(2, '0')
                            + dlg.timeStamp.Millisecond.ToString().PadLeft(3, '0') + ".jpg";
                        BVCU.FAILED(
                            BVCU.ManagedLayer_CuSnapshot(m_bvsdkHandle, dlg.dialogHandle,
                            Encoding.UTF8.GetBytes(path), 85));
                        break;
                    }
                }
            }
            catch
            {
                MessageBox.Show("截图失败");
            }
        }

        /*
         * 录像
         */
        public void record(Panel panel)
        {
            if (m_mainForm.RecordPath.Length == 0)
            {
                MessageBox.Show("未设置录像路径");
                return;
            }
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.panel == panel)
                {
                    dlg.recording = !dlg.recording;
                    if (dlg.recording)
                    {
                        try
                        {
                            BVCU.FAILED(
                                BVCU.ManagedLayer_CuSetRecordStorageParam(m_bvsdkHandle, dlg.dialogHandle,
                                Encoding.UTF8.GetBytes(m_mainForm.RecordPath), 3600));
                        }
                        catch
                        {
                            MessageBox.Show("保存文件失败");
                            dlg.recording = false;
                        }
                    }
                    else
                    {
                        BVCU.ManagedLayer_CuSetRecordStorageParam(m_bvsdkHandle, dlg.dialogHandle, Encoding.UTF8.GetBytes(""), 0);
                    }
                    break;
                }
            }
        }

        /*
         * 判断当前窗口(panel)是否正在录像
         */
        public bool Recording(Panel panel)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.panel == panel)
                {
                    return dlg.recording;
                }
            }
            return false;
        }

        /*
         * 设置当前窗口(panel)的播放音量
         */
        public void setVolume(Panel panel, int volume)
        {
            foreach (OneDialog dialog in m_dialogs)
            {
                if (dialog.panel == panel)
                {
                    BVCU.ManagedLayer_CuPlayVolume(m_bvsdkHandle, dialog.dialogHandle, volume);
                    break;
                }
            }
        }

        /*
         * 对话打开成功，创建字体、在视频帧上叠加字幕显示时间戳
         */
        public void OnOpenDialog(IntPtr dialogHandle)
        {
            m_afterRender.createTimeStampFont(getDialog(dialogHandle));
        }

        /*
         * 通过主窗口(Form1)的Invoke在回调中刷新界面处理对话打开失败事件
         */
        delegate void DialogHandleInDel(IntPtr dialogHandle);
        DialogHandleInDel openDialogFailed;
        void procOpenDialogFailed(IntPtr dialogHandle)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.dialogHandle == dialogHandle)
                {
                    dlg.panel.ContextMenuStrip = null;
                    dlg.panel = null;
                    m_dialogs.Remove(dlg);
                    break;
                }
            }
        }

        public void OnOpenDialogFailed(IntPtr dialogHandle)
        {
            m_bvsdkEventHandler.showMessageBoxOnEvent(m_mainForm, "打开预览失败");
            m_mainForm.BeginInvoke(openDialogFailed, new object[] { dialogHandle });
        }

        /*
         * 存储失败消息回调
         */
        public void OnRecordFailed(IntPtr dialogHandle)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.dialogHandle == dialogHandle)
                {
                    dlg.recording = false;
                    m_bvsdkEventHandler.showMessageBoxOnEvent(m_mainForm, "录像失败");
                    break;
                }
            }
        }

        public void OnResizeDialog()
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                BVRect rect = new BVRect();
                rect.left = dlg.panel.DisplayRectangle.Left;
                rect.top = dlg.panel.DisplayRectangle.Top;
                rect.right = dlg.panel.DisplayRectangle.Right;
                rect.bottom = dlg.panel.DisplayRectangle.Bottom;
                BVCU.ManagedLayer_CuResizeDialogWindow(m_bvsdkHandle, dlg.dialogHandle, ref rect);
            }
        }

        /*
         * 打开GPS流
         */
        ArrayList m_gpsDialogs;
        public int openGpsDialog(Session.Pu pu, int channelNo)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.pu.id.Equals(pu.id, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    return -1;
                }
            }
            IntPtr dialog = IntPtr.Zero;
            if (BVCU.ManagedLayer_CuOpenGpsDialog(m_bvsdkHandle, ref dialog, m_session.Handle, Encoding.UTF8.GetBytes(pu.id), 
                    channelNo, m_bvsdkEventHandler.gpsDialog_OnEvent, m_bvsdkEventHandler.gpsDialog_OnData)
                == BVCU.BVCU_RESULT_S_OK)
            {
                OneDialog dlg = new OneDialog();
                dlg.dialogHandle = dialog;
                dlg.channelNo = channelNo;
                dlg.pu = pu;
                m_gpsDialogs.Add(dlg);
                return 0;
            }
            return -1;
        }

        public void onGpsDialogOpenFailed(IntPtr dialog)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.dialogHandle == dialog)
                {
                    m_gpsDialogs.Remove(dlg);
                    return;
                }
            }
        }

        //打开Tsp
        ArrayList m_tspDialogs;
        public int openTspDialog(Session.Pu pu, int channelNo)
        {
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.pu.id.Equals(pu.id, StringComparison.CurrentCultureIgnoreCase) &&
                    dlg.channelNo == channelNo)
                {
                    return -1;
                }
            }
            IntPtr dialog = IntPtr.Zero;
            if (BVCU.ManagedLayer_CuOpenTspDialog(m_bvsdkHandle, ref dialog, m_session.Handle, Encoding.UTF8.GetBytes(pu.id),
                channelNo, m_bvsdkEventHandler.tspDialog_OnEvent, m_bvsdkEventHandler.tspDialog_OnData)
                == BVCU.BVCU_RESULT_S_OK)
            {
                OneDialog dlg = new OneDialog();
                dlg.dialogHandle = dialog;
                dlg.channelNo = channelNo;
                dlg.pu = pu;
                m_tspDialogs.Add(dlg);
                return 0;
            }
            return -1;
        }
        public void onTspDialogOpenFailed(IntPtr dialog)
        {
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.dialogHandle == dialog)
                {
                    m_tspDialogs.Remove(dlg);
                    return;
                }
            }
        }

        delegate void OnGetGpsData(IntPtr dialog, float lat, float lng);
        OnGetGpsData deleGetGpsData;
        public void onGpsData(IntPtr dialog, float lat, float lng)
        {
            if (null == deleGetGpsData)
            {
                deleGetGpsData = new OnGetGpsData(procGetGpsData);
            }
            m_mainForm.BeginInvoke(deleGetGpsData, new object[] { dialog, lat, lng });
        }

        void procGetGpsData(IntPtr dialog, float lat, float lng)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.dialogHandle == dialog)
                {
                    m_mainForm.onGetGpsData(dlg.pu.id, lat, lng);
                    return;
                }
            }
        }

        public void closeGpsDialog(string puId, int channelNo)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    return;
                }
            }
        }

        delegate void OnGetTspData(IntPtr dialog, string pTspData, int len);
        OnGetTspData deleGetTspData;
        public void onTspData(IntPtr dialog, string pTspData, int len)
        {
            if (null == deleGetTspData)
            {
                deleGetTspData = new OnGetTspData(procGetTspData);
            }
            m_mainForm.BeginInvoke(deleGetTspData, new object[] {dialog, pTspData, len});
        }
        public void procGetTspData(IntPtr dialog, string pTspData, int len)
        {
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.dialogHandle == dialog)
                {
                    m_mainForm.onGetTspData(dlg.pu.id, dlg.channelNo, pTspData, len);
                    return;
                }
            }
        }
        public void closeTspDialog(string puId, int channelNo)
        {
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    return;
                }
            }
        }
    }
}
