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
    /// <summary>
    /// 会话
    /// </summary>
    public class Dialog
    {
        /// <summary>
        /// 创建的所有Dialog列表
        /// </summary>
        public ArrayList m_dialogs;

        /// <summary>
        /// 用于在打开的Dialog的预览画面中渲染字体
        /// </summary>
        AfterRender m_afterRender;

        /// <summary>
        /// 全局变量, Dialog句柄
        /// </summary>
        IntPtr m_bvsdkHandle;
        EventHandler m_bvsdkEventHandler;
        MainWinForm m_mainForm;
        Session m_session;

        public Dialog(IntPtr sdkHandle, EventHandler eventHandler, MainWinForm mainForm, Session session)
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

        /// <summary>
        /// 打开对话(Dialog)
        /// </summary>
        /// <param name="pu"></param>
        /// <param name="channelNo"></param>
        /// <param name="panel"></param>
        public void openBrowse(Pu pu, int channelNo, Panel panel)
        {
            try
            {
                BVCU_DialogControlParam_Network net = new BVCU_DialogControlParam_Network(0, 5, 1, 3);
                BVRect dispRect = new BVRect(0, 0, panel.ClientRectangle.Height, panel.ClientRectangle.Width);
                OneDialog dlg = new OneDialog();

                foreach (Channel channel in pu.channelList)
                {
                    int volume = -1;
                    if (channel.channelNo == channelNo)
                    {
                        if (channel.audioPlayback)
                        {
                            volume = 0;
                        }
                        //打开视屏浏览, 主要参数：sdkHandle、sessionHandle、puID、channelNo、显示区域的大小
                        int ret = BVCU.ManagedLayer_CuBrowsePu(m_bvsdkHandle,//sdk handle
                            ref dlg.dialogHandle,//dialog handle
                            m_session.Handle,//session handle
                            Encoding.UTF8.GetBytes(pu.id),//pu id
                            channelNo, //channel no                            
                            panel.Handle, //pannel handle
                            ref dispRect,//上下左右，矩形
                            volume,//音量
                            0,//single Rec File Sec
                            Encoding.UTF8.GetBytes(""), //rec File Dir
                            true,//videoTrans
                            ref net,//network 时延
                            m_bvsdkEventHandler.dialog_OnDialogEvent,
                            m_bvsdkEventHandler.dialog_OnStorageEvent);

                        BVCU.FAILED(ret);
                        LogHelper.LogHelper.RecordLog(4, string.Format("打开[{0}, {1}]的视屏预览通道", pu.puName, channelNo));
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

        public void openTalkOnly(Pu pu, int channelNo, Panel panel)
        {
            try
            {
                BVCU_DialogControlParam_Network net = new BVCU_DialogControlParam_Network(0, 5, 1, 3);
                BVRect dispRect = new BVRect(0, 0, panel.ClientRectangle.Height, panel.ClientRectangle.Width);
                OneDialog dlg = new OneDialog();

                foreach (Channel channel in pu.channelList)
                {
                    int captureVolume = 50;
                    int audioVolume = 50;
                    if (channel.channelNo == channelNo)
                    {
                        int ret = BVCU.ManagedLayer_CuNewTalk(m_bvsdkHandle,
                            ref dlg.dialogHandle,
                            m_session.Handle,
                            Encoding.UTF8.GetBytes(pu.id),
                            channelNo,
                            captureVolume,
                            audioVolume,
                            ref net,
                            m_bvsdkEventHandler.dialog_OnDialogEvent,
                            m_bvsdkEventHandler.dialog_OnStorageEvent);
                        BVCU.FAILED(ret);
                        LogHelper.LogHelper.RecordLog(4, string.Format("打开[{0}]对讲", pu.puName));
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
                MessageBox.Show("打开对讲失败");
            }
        }

        /// <summary>
        /// 视频预览
        /// </summary>
        /// <example>adf</example>
        /// <param name="pu">要预览的设备</param>
        /// <param name="channelNo">通道号</param>
        /// <param name="control">在哪个控件上预览</param>
        public void PreviewVideo(Pu pu, int channelNo, Control control)
        {
            BVCU_DialogControlParam_Network net = new BVCU_DialogControlParam_Network(0, 5, 1, 3);
            OneDialog dlg = new OneDialog();
            BVRect dispRect = new BVRect(0, 0, control.ClientRectangle.Height, control.ClientRectangle.Width);

            //打开视屏浏览
            int ret = BVCU.ManagedLayer_CuBrowsePu(m_bvsdkHandle,//sdk handle
                ref dlg.dialogHandle,//dialog handle
                m_session.Handle,//session handle
                Encoding.UTF8.GetBytes(pu.id),//pu id
                channelNo, //channel no                            
                control.Handle, //pannel handle
                ref dispRect,//上下左右，矩形
                0,//音量
                0,//single Rec File Sec
                Encoding.UTF8.GetBytes(""), //rec File Dir
                true,//videoTrans
                ref net,//network 时延
                m_bvsdkEventHandler.dialog_OnDialogEvent,
                m_bvsdkEventHandler.dialog_OnStorageEvent);
        }


        /// <summary>
        /// 创建的总对话(Dialog)数
        /// </summary>
        public int Count
        {
            get
            {
                return m_dialogs.Count;
            }
        }

        /// <summary>
        /// 查询函数
        /// </summary>
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

        /// <summary>
        /// 关闭浏览对话框(Diglog)
        /// </summary>
        /// <param name="panel">需要关闭的浏览框对应的Panel</param>
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

        public void closeDialog(OneDialog dialog)
        {
            if (null == dialog)
                return;
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg == dialog && dialog.dialogHandle != null)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    m_dialogs.Remove(dlg);
                    break;
                }
            }
            m_dialogs.Clear();
        }

        public void closeGpsDialog(OneDialog gpsDialog)
        {
            if (null == gpsDialog)
                return;
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg == gpsDialog && dlg.dialogHandle != null)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    m_gpsDialogs.Remove(dlg);
                    break;
                }
            }
            m_gpsDialogs.Clear();
        }

        public void closeTspDialog(OneDialog tspDialog)
        {
            if (null == tspDialog)
                return;
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg == tspDialog && dlg.dialogHandle != null)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    m_tspDialogs.Remove(dlg);
                    break;
                }
            }
            m_tspDialogs.Clear();
        }

        /// <summary>
        /// 截图
        /// </summary>
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


                        //截图
                        int ret = BVCU.ManagedLayer_CuSnapshot(m_bvsdkHandle, dlg.dialogHandle, Encoding.UTF8.GetBytes(path), 85);

                        BVCU.FAILED(ret);
                        break;
                    }
                }
            }
            catch
            {
                MessageBox.Show("截图失败");
            }
        }

        /// <summary>
        /// 录像
        /// </summary>
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
                            //录像
                            int ret = BVCU.ManagedLayer_CuSetRecordStorageParam(m_bvsdkHandle, dlg.dialogHandle, Encoding.UTF8.GetBytes(m_mainForm.RecordPath), 3600);

                            BVCU.FAILED(ret);
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

        /// <summary>
        /// 判断当前窗口(panel)是否正在录像
        /// </summary>
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

        /// <summary>
        /// 设置当前窗口(panel)的播放音量
        /// </summary>
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

        /// <summary>
        /// 对话打开成功，创建字体、在视频帧上叠加字幕显示时间戳
        /// </summary>
        public void OnOpenDialog(IntPtr dialogHandle)
        {
            m_afterRender.createTimeStampFont(getDialog(dialogHandle));
        }

        /// <summary>
        /// 通过主窗口(Form1)的Invoke在回调中刷新界面处理对话打开失败事件
        /// </summary>
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

        /// <summary>
        /// 存储失败消息回调
        /// </summary>
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

        /// <summary>
        /// 打开GPS流
        /// </summary>
        public ArrayList m_gpsDialogs;
        public int openGpsDialog(Pu pu, int channelNo)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.pu.id.Equals(pu.id, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    LogHelper.LogHelper.RecordLog(51, string.Format("已经发送打开[{0}, {1}]Gps通道的命令", pu.puName, channelNo));
                    return -1;
                }
            }
            IntPtr dialog = IntPtr.Zero;
            int ret = BVCU.ManagedLayer_CuOpenGpsDialog(m_bvsdkHandle, ref dialog, m_session.Handle, Encoding.UTF8.GetBytes(pu.id), channelNo, m_bvsdkEventHandler.gpsDialog_OnEvent, m_bvsdkEventHandler.gpsDialog_OnData);
            if (BVCU.BVCU_RESULT_S_OK == ret)
            {
                OneDialog dlg = new OneDialog();
                dlg.dialogHandle = dialog;
                dlg.channelNo = channelNo;
                dlg.pu = pu;
                m_gpsDialogs.Add(dlg);

                LogHelper.LogHelper.RecordLog(3, string.Format("正在打开{0}, {1}的Gps通道...", pu.puName, channelNo));
                return 0;
            }
            else
            {
                LogHelper.LogHelper.RecordLog(3, string.Format("打开{0}, {1}的Gps通道失败， 错误码:{2}", pu.puName, channelNo, ret));
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
        public ArrayList m_tspDialogs;
        public int openTspDialog(Pu pu, int channelNo)
        {
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.pu.id.Equals(pu.id, StringComparison.CurrentCultureIgnoreCase) &&
                    dlg.channelNo == channelNo)
                {
                    LogHelper.LogHelper.RecordLog(51, string.Format("已经发送打开[{0}, {1}]串口通道的命令", pu.puName, channelNo));
                    return -1;
                }
            }
            IntPtr dialog = IntPtr.Zero;

            int ret = BVCU.ManagedLayer_CuOpenTspDialog(m_bvsdkHandle, ref dialog, m_session.Handle, Encoding.UTF8.GetBytes(pu.id), channelNo, m_bvsdkEventHandler.tspDialog_OnEvent, m_bvsdkEventHandler.tspDialog_OnData);
            if (BVCU.BVCU_RESULT_S_OK == ret)
            {
                OneDialog dlg = new OneDialog();
                dlg.dialogHandle = dialog;
                dlg.channelNo = channelNo;
                dlg.pu = pu;
                m_tspDialogs.Add(dlg);

                LogHelper.LogHelper.RecordLog(0, string.Format("正在打开设备:{0}, 通道:{1}的串口", pu.puName, channelNo));
                return 0;
            }
            else
            {
                string errorMsg = string.Format("打开{0}的{1}串口失败(错误码:{2})", pu.puName, channelNo, ret);
                MessageBox.Show(errorMsg);
                LogHelper.LogHelper.RecordLog(100, errorMsg);
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

        /// <summary>
        /// 发送串口数据
        /// </summary>
        public int SendTspData(string sendData)
        {
            if (0 == this.m_tspDialogs.Count)
            {
                LogHelper.LogHelper.RecordLog(58, "没有已打开的串口通道");
                return -1;
            }

            foreach (OneDialog tspDialog in this.m_tspDialogs)
            {
                //BVCU_RESULT_S_OK: 成功
                //BVCU_RESULT_E_NOTEXIST: 会话不存在
                //BVCU_RESULT_E_UNSUPPORTED: 不支持的操作
                //BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
                //发送串口数据
                int ret = BVCU.SendTspData(tspDialog.dialogHandle, Encoding.UTF8.GetBytes(sendData), sendData.Length + 1);

                if (BVCU.BVCU_RESULT_S_OK == ret)
                {
                    LogHelper.LogHelper.RecordLog(0, "发送成功, 内容: " + sendData);
                }
                else if ((int)BVCU.BVCU_Result.BVCU_RESULT_E_NOTFOUND == ret)
                {
                    LogHelper.LogHelper.RecordLog(100, "发送失败，会话不存在");
                }
                else if ((int)BVCU.BVCU_Result.BVCU_RESULT_E_ALLOCMEMFAILED == ret)
                {
                    LogHelper.LogHelper.RecordLog(100, string.Format("发送失败，错误码:{0}, 可能原因: 您打开的会话(Dialog)模式为只读, 请增加可写功能", ret));
                }
                else
                {
                    LogHelper.LogHelper.RecordLog(100, "其他错误, 错误码: " + ret);
                }

            }


            return 0;
        }

        delegate void OnGetGpsData(IntPtr dialog, IntPtr pGpsData, Int32 len);
        OnGetGpsData deleGetGpsData;

        public void onGpsData(IntPtr dialog, IntPtr pGpsData, Int32 len)
        {
            if (null == deleGetGpsData)
            {
                deleGetGpsData = new OnGetGpsData(procGetGpsData);
            }
            m_mainForm.BeginInvoke(deleGetGpsData, new object[] { dialog, pGpsData, len });
        }

        void procGetGpsData(IntPtr dialog, IntPtr pGpsData, int len)
        {
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.dialogHandle == dialog)
                {
                    m_mainForm.onGetGpsData(dlg.pu.id, pGpsData, len);
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

            m_mainForm.BeginInvoke(deleGetTspData, new object[] { dialog, pTspData, len });
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

        public void closeDialog(string puId, int channelNo)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    int iresult = BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    if (0 == iresult)
                    {
                        m_dialogs.Remove(dlg);
                    }
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
                    int iresult = BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    if (0 == iresult)
                    {
                        m_gpsDialogs.Remove(dlg);
                    }
                    return;
                }
            }
        }
        public void closeTspDialog(string puId, int channelNo)
        {
            //新增
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    BVCU.ManagedLayer_CuCloseDialog(m_bvsdkHandle, dlg.dialogHandle);
                    for (int i = 0; i < this.m_tspDialogs.Count; i++)
                    {
                        OneDialog oneDialog = this.m_tspDialogs[i] as OneDialog;
                        if (oneDialog.pu.id.Equals(puId) && oneDialog.channelNo.Equals(channelNo))
                        {
                            this.m_tspDialogs.RemoveAt(i);
                            this.TspStatusChange_OnEvent();
                            break;
                        }
                    }
                    return;
                }
            }
        }

        public void clearDialog(string puId, int channelNo)
        {
            foreach (OneDialog dlg in m_dialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    m_dialogs.Remove(dlg);
                    return;
                }
            }
        }

        public bool clearGpsDialog(string puId, int channelNo)
        {
            bool bRemove = false;
            foreach (OneDialog dlg in m_gpsDialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    m_gpsDialogs.Remove(dlg);
                    bRemove = true;
                    break;
                }
            }
            return bRemove;
        }

        public bool clearTspDialog(string puId, int channelNo)
        {
            bool bRemove = false;
            foreach (OneDialog dlg in m_tspDialogs)
            {
                if (dlg.pu.id.Equals(puId, StringComparison.CurrentCultureIgnoreCase)
                    && dlg.channelNo == channelNo)
                {
                    m_tspDialogs.Remove(dlg);
                    bRemove = true;
                    break;
                }
            }
            return bRemove;
        }

        public void TspStatusChange_OnEvent()
        {
            if (0 != this.m_tspDialogs.Count)
            {
                this.m_mainForm.TspStatusChange_OnEvent(true);
            }
            else
            {
                this.m_mainForm.TspStatusChange_OnEvent(false);
            }
        }
    }
}
