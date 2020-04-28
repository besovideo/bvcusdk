/*
 * 本类用于处理消息回调
 */

using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WindowsFormsTest
{
    /// <summary>
    /// 事件处理
    /// </summary>
    public class EventHandler
    {
        Session m_session;
        Dialog m_dialog;
        
        /// <summary>
        /// 初始化全局变量
        /// </summary>
        /// <param name="bvcuSdkHandle"></param>
        /// <param name="session"></param>
        /// <param name="dialog"></param>
        public void init(IntPtr bvcuSdkHandle, Session session, Dialog dialog)
        {
            m_session = session;
            m_dialog = dialog;
            BVCU.ManagedLayer_CuSetPuControlResultProcFunc(bvcuSdkHandle, onControlResult);
        }

        public EventHandler()
        {
            dialog_OnDialogEvent = new BVCU_Dialog_OnDialogEvent(Dialog_OnDialogEvent);
            dialog_OnStorageEvent = new BVCU_Dialog_OnStorageEvent(Dialog_OnStorageEvent);
            gpsDialog_OnEvent = new BVCU_GpsDialog_OnEvent(GpsDialog_OnEvent);
            gpsDialog_OnData = new BVCU_GpsDialog_OnData(GpsDialog_OnData);
            tspDialog_OnEvent = new BVCU_TspDialog_OnEvent(TspDialog_OnEvent);
            tspDialog_OnData = new BVCU_TspDialog_OnData(TspDialog_onData);

            server_OnEvent = new BVCU_Server_OnEvent(Server_OnEvent);
            server_ProcChannelInfo = new BVCU_Server_ProcChannelInfo(Server_ProcNotifyChannelInfo);
            cmd_OnGetPuList = new BVCU_Cmd_OnGetPuList(Cmd_OnGetPuList);

            onControlResult = new BVCU_Cmd_ControlResult(OnControlResult);
            m_showMessageBoxOnEvent = new ShowMessageDel(procShowMessageBoxOnEvent);
            onGetPtzAttr = new BVCU_Cmd_OnGetPuPtzAttr(OnGetPuPtzAttr);
        }

        /// <summary>
        /// 用控件Invoke在消息回调中弹出消息提示框
        /// </summary>
        /// <param name="message"></param>
        delegate void ShowMessageDel(string message);
        ShowMessageDel m_showMessageBoxOnEvent;
        void procShowMessageBoxOnEvent(string message)
        {
            MessageBox.Show(message);
        }
        public void showMessageBoxOnEvent(Control ctrl, string message)
        {
            ctrl.BeginInvoke(m_showMessageBoxOnEvent, new object[] { message });
        }


        #region Dialog Event
        /// <summary>
        /// Dialog Event
        /// </summary>
        /// <param name="dialog"></param>
        /// <param name="eventCode"></param>
        /// <param name="errorCode"></param>
        /// <param name="mediaDir"></param>
        public delegate void BVCU_Dialog_OnDialogEvent(IntPtr dialog, int eventCode, int errorCode, int mediaDir);
        public delegate void BVCU_Dialog_OnStorageEvent(IntPtr dialog, int eventCode, int errorCode, IntPtr fileName, int strLen, Int64 timeStamp);

        public BVCU_Dialog_OnDialogEvent dialog_OnDialogEvent;
        //成功打开会话，响应（回调）
        void Dialog_OnDialogEvent(IntPtr dialog, int eventCode, int errorCode, int mediaDir)
        {
            try
            {
                switch (eventCode)
                {
                    case BVCU.BVCU_EVENT_DIALOG_OPEN:
                        if (errorCode == BVCU.BVCU_RESULT_S_OK)
                        {
                            m_dialog.OnOpenDialog(dialog);
                            LogHelper.LogHelper.RecordLog(3, "打开通道成功");
                        }
                        else
                        {
                            m_dialog.OnOpenDialogFailed(dialog);
                            LogHelper.LogHelper.RecordLog(51, "打开通道失败");
                        }
                        break;
                    case BVCU.BVCU_EVENT_DIALOG_CLOSE:
                        break;
                    case BVCU.BVCU_EVENT_DIALOG_UPDATE:
                        break;
                    default:
                        break;
                }
            }
            catch
            {
                // Not found dlg
                return;
            }
        }

        public BVCU_Dialog_OnStorageEvent dialog_OnStorageEvent;
        void Dialog_OnStorageEvent(IntPtr dialog, int eventCode, int errorCode, IntPtr fileName, int strLen, Int64 timeStamp)
        {
            string file = Marshal.PtrToStringAnsi(fileName, strLen);
            if (errorCode != BVCU.BVCU_RESULT_S_OK)
            {
                m_dialog.OnRecordFailed(dialog);
            }
        }

        #endregion Dialog Event



        public delegate void BVCU_GpsDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode);
        public delegate void BVCU_GpsDialog_OnData(IntPtr dialog, IntPtr pGpsData, Int32 len);
        public BVCU_GpsDialog_OnData gpsDialog_OnData;
        public BVCU_GpsDialog_OnEvent gpsDialog_OnEvent;
        void GpsDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode)
        {
            if (errorCode != BVCU.BVCU_RESULT_S_OK)
            {
                m_dialog.onGpsDialogOpenFailed(dialog);
                LogHelper.LogHelper.RecordLog(69, "打开Gps通道失败， 错误码:" + errorCode);
            }
            else
            {
                LogHelper.LogHelper.RecordLog(0, "打开Gps通道成功");
            }
        }

        void GpsDialog_OnData(IntPtr dialog, IntPtr pGpsData, Int32 len)
        {
            m_dialog.onGpsData(dialog, pGpsData, len);
        }
        public delegate void BVCU_TspDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode);
        public delegate void BVCU_TspDialog_OnData(IntPtr dialog, IntPtr pTspData, int len);
        public BVCU_TspDialog_OnData tspDialog_OnData;
        public BVCU_TspDialog_OnEvent tspDialog_OnEvent;
        void TspDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode)
        {
            if (errorCode != BVCU.BVCU_RESULT_S_OK)
            {
                m_dialog.onTspDialogOpenFailed(dialog);
                LogHelper.LogHelper.RecordLog(56, "打开串口通道失败, 错误码:" + errorCode);
            }
            else
            {
                this.m_dialog.TspStatusChange_OnEvent();
                LogHelper.LogHelper.RecordLog(0, "打开串口通道成功");
            }
        }
        void TspDialog_onData(IntPtr dialog, IntPtr pTspData, int len)
        {
            if (pTspData == IntPtr.Zero || len <= 0)
            {
                return;
            }
            byte[] byteTspData = new byte[len];
            Marshal.Copy(pTspData, byteTspData, 0, byteTspData.Length);
            m_dialog.onTspData(dialog, byteTspData);
        }



        #region Session Event
        /// <summary>
        /// Session Event
        /// </summary>
        public const int BVCU_ONLINE_STATUS_OFFLINE = 0;
        public const int BVCU_ONLINE_STATUS_ONLINE = 1;
        public delegate void BVCU_Server_ProcChannelInfo(IntPtr session, IntPtr puId, IntPtr puName, int status, ref BVCU_PUOneChannelInfo channel, int finished);
        public delegate void BVCU_Server_OnEvent(IntPtr session, int eventCode, ref BVCU_Event_Common eventCommon);
        public delegate void BVCU_Cmd_OnGetPuList(IntPtr session, IntPtr puId, IntPtr puName, int status, ref BVCU_PUOneChannelInfo channel, int finished);

        public BVCU_Server_OnEvent server_OnEvent;
        /// <summary>
        /// 服务器事件（session_open、session_close）
        /// </summary>
        void Server_OnEvent(IntPtr session, int eventCode, ref BVCU_Event_Common eventCommon)
        {
            switch (eventCode)
            {
                case BVCU.BVCU_EVENT_SESSION_OPEN://建立连接事件
                    if (eventCommon.errorCode == BVCU.BVCU_RESULT_S_OK)
                    {
                        m_session.OnLoginOk();//连接成功， 更新设备列表...
                        return;
                    }
                    else if (eventCommon.errorCode == BVCU.BVCU_RESULT_E.TIMEOUT)
                    {
                        Console.WriteLine("连接服务器超时");
                    }
                    else if (eventCommon.errorCode == BVCU.BVCU_RESULT_E.CONNECTFAILED)
                    {
                        Console.WriteLine("连接服务器失败");
                    }
                    m_session.OnLoginFailed();
                    break;
                case BVCU.BVCU_EVENT_SESSION_CLOSE://关闭连接事件
                    if (eventCommon.errorCode == BVCU.BVCU_RESULT_S_OK)//成功关闭连接
                    {
                    }
                    else
                    {
                        if (eventCommon.errorCode == BVCU.BVCU_RESULT_E.DISCONNECTED)
                        {
                            m_session.OnServerDisConnect();//服务器下线
                            return;
                        }
                        return;
                    }
                    break;
                default:
                    break;
            }
        }

        #endregion Session Event

        /// <summary>
        /// Pu status change
        /// </summary>
        public BVCU_Server_ProcChannelInfo server_ProcChannelInfo;
        void Server_ProcNotifyChannelInfo(IntPtr session, IntPtr ptPuId, IntPtr ptPuName, int iStatus, ref BVCU_PUOneChannelInfo channel, int iFinished)
        {
            LogHelper.LogHelper.RecordLog(100, "==========================================");
            string puId = Marshal.PtrToStringAnsi(ptPuId, BVCU.BVCU_MAX_ID_LEN + 1).Split('\0')[0];
            string puName = Marshal.PtrToStringAnsi(ptPuName, BVCU.BVCU_MAX_NAME_LEN + 1).Split('\0')[0];
            bool bNeedClearGps = false;
            bool bNeedClearTsp = false;
            if (iStatus == BVCU.BVCU_ONLINE_STATUS_OFFLINE)
            {
                m_dialog.clearDialog(puId, channel.iChannelIndex);
                bNeedClearGps = m_dialog.clearGpsDialog(puId, channel.iChannelIndex);
                bNeedClearTsp = m_dialog.clearTspDialog(puId, channel.iChannelIndex);
                m_session.clearGpsDataList(puId, channel.iChannelIndex, bNeedClearGps | bNeedClearTsp);
                m_session.OnRemovePu(puId);
            }
            Channel chnl = new Channel();
            getChannel(chnl, channel, iStatus);
            m_session.OnGetPu(puName, puId, chnl);
            if (BVCU.TRUE(iFinished))
            {
                m_session.OnGetPuListFinished();
            }
            return;
        }

        /// <summary>
        /// 获得设备列表的响应函数
        /// </summary>
        public BVCU_Cmd_OnGetPuList cmd_OnGetPuList;
        void Cmd_OnGetPuList(IntPtr session, IntPtr ptPuId, IntPtr ptPuName, int iStatus, ref BVCU_PUOneChannelInfo channel, int iFinished)
        {
            string puId = Marshal.PtrToStringAnsi(ptPuId, BVCU.BVCU_MAX_ID_LEN + 1).Split('\0')[0];
            string puName = Marshal.PtrToStringAnsi(ptPuName, BVCU.BVCU_MAX_NAME_LEN + 1).Split('\0')[0];
            Byte[] bpuid = new Byte[BVCU.BVCU_MAX_ID_LEN + 1];
            Byte[] bpuname = new Byte[BVCU.BVCU_MAX_NAME_LEN + 1];
            Marshal.Copy(ptPuId, bpuid, 0, BVCU.BVCU_MAX_ID_LEN + 1);
            Marshal.Copy(ptPuName, bpuname, 0, BVCU.BVCU_MAX_ID_LEN + 1);
            string spuid = System.Text.Encoding.UTF8.GetString(bpuid).Split('\0')[0];
            string spuname = System.Text.Encoding.UTF8.GetString(bpuname).Split('\0')[0];
            if (BVCU.TRUE(iFinished))
            {
                m_session.OnGetPuListFinished();
            }
            Channel chnl = new Channel();
            if (iStatus == BVCU.BVCU_ONLINE_STATUS_OFFLINE)
            {
                chnl.online = false;
            }
            else
            {
                chnl.online = true;
            }
            getChannel(chnl, channel, iStatus);
            m_session.OnGetPu(spuname, spuid, chnl);
            /*if (channel.szName.Equals("gps"))
            {
                chnl = new Session.Channel();
                channel.szName = "TSP";
                channel.iPTZIndex = 15;
                channel.iMediaDir = 32;
                channel.iChannelIndex = 65792;
                if (iStatus == BVCU.BVCU_ONLINE_STATUS_OFFLINE)
                {
                    chnl.online = false;
                }
                else
                {
                    chnl.online = true;
                }
                getChannel(chnl, channel);
                m_session.OnGetPu(puName, puId, chnl);
            }*/
        }

        void getChannel(Channel chnl, BVCU_PUOneChannelInfo channel)
        {
            chnl.channelName = channel.szName;
            
            BVCU.AVDirection avDir = BVCU.GetAVDirection(channel.iMediaDir);

            chnl.audioPlayback = avDir.audioRecv;
            chnl.speak = avDir.audioSnd;
            chnl.video = avDir.videoRecv;
            chnl.ptzIdx = channel.iPTZIndex;
            chnl.channelNo = channel.iChannelIndex;
        }

        void getChannel(Channel chnl, BVCU_PUOneChannelInfo channel, int iStatus)
        {
            chnl.channelName = channel.szName;

            BVCU.AVDirection avDir = BVCU.GetAVDirection(channel.iMediaDir);

            chnl.audioPlayback = avDir.audioRecv;
            chnl.speak = avDir.audioSnd;
            chnl.video = avDir.videoRecv;
            chnl.ptzIdx = channel.iPTZIndex;
            chnl.channelNo = channel.iChannelIndex;

            chnl.online = iStatus == 1 ? true : false;
        }

        /// <summary>
        /// 控制消息
        /// </summary>
        public delegate void BVCU_Cmd_ControlResult(IntPtr session, IntPtr puId, Int32 device, Int32 subMethod, Int32 result);

        BVCU_Cmd_ControlResult onControlResult;
        void OnControlResult(IntPtr session, IntPtr ptPuId, Int32 device, Int32 subMethod, Int32 result)
        {
        }

        public delegate void BVCU_Cmd_OnGetPuPtzAttr(IntPtr session, IntPtr puId, int ptzIndex, IntPtr ptzAttr);
        public BVCU_Cmd_OnGetPuPtzAttr onGetPtzAttr;
        void OnGetPuPtzAttr(IntPtr session, IntPtr puIdPtr, int ptzIndex, IntPtr ptzAttrPtr)
        {
            string puId = Marshal.PtrToStringAnsi(puIdPtr);
            BVCU_PUCFG_PTZAttr ptzAttr = (BVCU_PUCFG_PTZAttr)Marshal.PtrToStructure(ptzAttrPtr, typeof(BVCU_PUCFG_PTZAttr));
            Pu pu = m_session.getPu(puId);
            if (pu != null)
            {
                pu.puPtz.Add(ptzAttr);
            }
        }
    }
}
