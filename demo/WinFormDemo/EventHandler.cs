/*
 * 本类用于处理消息回调
 */

using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WindowsFormsTest
{
    class EventHandler
    {
        Session m_session;
        Dialog m_dialog;
        /*
         * 初始化全局变量
         */
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
        
        /*
         * 用控件Invoke在消息回调中弹出消息提示框
         */
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


        /*
         * Dialog Event
         */
        public delegate void BVCU_Dialog_OnDialogEvent(IntPtr dialog, int eventCode, int errorCode, int mediaDir);
        public delegate void BVCU_Dialog_OnStorageEvent(IntPtr dialog, int eventCode, int errorCode, IntPtr fileName, int strLen, Int64 timeStamp);

        public BVCU_Dialog_OnDialogEvent dialog_OnDialogEvent;
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
                            Console.WriteLine("打开通道成功");
                        }
                        else
                        {
                            m_dialog.OnOpenDialogFailed(dialog);
                            Console.WriteLine("打开通道失败");
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

        public delegate void BVCU_GpsDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode);
        public delegate void BVCU_GpsDialog_OnData(IntPtr dialog, Int32 lat, Int32 lng);
        public BVCU_GpsDialog_OnData gpsDialog_OnData;
        public BVCU_GpsDialog_OnEvent gpsDialog_OnEvent;
        void GpsDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode)
        {
            if (errorCode != BVCU.BVCU_RESULT_S_OK)
            {
                m_dialog.onGpsDialogOpenFailed(dialog);
            }
        }

        void GpsDialog_OnData(IntPtr dialog, Int32 lat, Int32 lng)
        {
            m_dialog.onGpsData(dialog, (float)lat / BVCU.BVCU_LAT_LNG_UNIT, (float)lng / BVCU.BVCU_LAT_LNG_UNIT);
        }
        public delegate void BVCU_TspDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode);
        public delegate void BVCU_TspDialog_OnData(IntPtr dialog, string pTspData, int len);
        public BVCU_TspDialog_OnData tspDialog_OnData;
        public BVCU_TspDialog_OnEvent tspDialog_OnEvent;
        void TspDialog_OnEvent(IntPtr dialog, int eventCode, Int32 errorCode)
        {
            if (errorCode != BVCU.BVCU_RESULT_S_OK)
            {
                m_dialog.onTspDialogOpenFailed(dialog);
            }
        }
        void TspDialog_onData(IntPtr dialog, string pTspData, int len)
        {
            m_dialog.onTspData(dialog, pTspData, len);
        }


        /*
         * Session Event
         */
        public const int BVCU_ONLINE_STATUS_OFFLINE = 0;
        public const int BVCU_ONLINE_STATUS_ONLINE = 1;
        public delegate void BVCU_Server_ProcChannelInfo(IntPtr session, IntPtr puId, IntPtr puName, int status, ref BVCU_PUOneChannelInfo channel, int finished);
        public delegate void BVCU_Server_OnEvent(IntPtr session, int eventCode, ref BVCU_Event_Common eventCommon);
        public delegate void BVCU_Cmd_OnGetPuList(IntPtr session, IntPtr puId, IntPtr puName, int status, ref BVCU_PUOneChannelInfo channel, int finished);

        public BVCU_Server_OnEvent server_OnEvent;
        void Server_OnEvent(IntPtr session, int eventCode, ref BVCU_Event_Common eventCommon)
        {
            switch (eventCode)
            {
                case BVCU.BVCU_EVENT_SESSION_OPEN:
                    if (eventCommon.errorCode == BVCU.BVCU_RESULT_S_OK)
                    {
                        m_session.OnLoginOk();
                        return;
                    }
                    else if (eventCommon.errorCode == BVCU_RESULT_E.TIMEOUT)
                    {
                    }
                    else if (eventCommon.errorCode == BVCU_RESULT_E.CONNECTFAILED)
                    {
                    }
                    break;
                case BVCU.BVCU_EVENT_SESSION_CLOSE:
                    if (eventCommon.errorCode == BVCU.BVCU_RESULT_S_OK)
                    {
                    }
                    else
                    {
                        if (eventCommon.errorCode == BVCU_RESULT_E.DISCONNECTED)
                        {
                            m_session.OnServerDisConnect();
                            return;
                        }
                        return;
                    }
                    break;
                default:
                    break;
            }
        }

        /*
         * Pu status change
         */
        public BVCU_Server_ProcChannelInfo server_ProcChannelInfo;
        void Server_ProcNotifyChannelInfo(IntPtr session, IntPtr ptPuId, IntPtr ptPuName, int iStatus, ref BVCU_PUOneChannelInfo channel, int iFinished)
        {
            string puId = Marshal.PtrToStringAnsi(ptPuId, BVCU.BVCU_MAX_ID_LEN + 1).Split('\0')[0];
            string puName = Marshal.PtrToStringAnsi(ptPuName, BVCU.BVCU_MAX_NAME_LEN + 1).Split('\0')[0];
            if (iStatus == BVCU.BVCU_ONLINE_STATUS_OFFLINE)
            {
                m_session.OnRemovePu(puId);
            }
            if (BVCU.TRUE(iFinished))
            {
                m_session.OnGetPuListFinished();
            }
            Session.Channel chnl = new Session.Channel();
            getChannel(chnl, channel);
            m_session.OnGetPu(puName, puId, chnl);
            return;
        }

        /*
         * Get Pu List
         */
        public BVCU_Cmd_OnGetPuList cmd_OnGetPuList;
        void Cmd_OnGetPuList(IntPtr session, IntPtr ptPuId, IntPtr ptPuName, int iStatus, ref BVCU_PUOneChannelInfo channel, int iFinished)
        {
            string puId = Marshal.PtrToStringAnsi(ptPuId, BVCU.BVCU_MAX_ID_LEN + 1).Split('\0')[0];
            string puName = Marshal.PtrToStringAnsi(ptPuName, BVCU.BVCU_MAX_NAME_LEN + 1).Split('\0')[0];
            if (BVCU.TRUE(iFinished))
            {
                m_session.OnGetPuListFinished();
            }
            Session.Channel chnl = new Session.Channel();
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

        void getChannel(Session.Channel chnl, BVCU_PUOneChannelInfo channel)
        {
            chnl.channelName = channel.szName;
            BVCU.AVDirection avDir = BVCU.GetAVDirection(channel.iMediaDir);
            chnl.audioPlayback = avDir.audioRecv;
            chnl.speak = avDir.audioSnd;
            chnl.video = avDir.videoRecv;
            chnl.ptzIdx = channel.iPTZIndex;
            chnl.channelNo = channel.iChannelIndex;
        }

        /*
         * 控制消息
         */
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
            Session.Pu pu = m_session.getPu(puId);
            if (pu != null)
            {
                pu.puPtz.Add(ptzAttr);
            }
        }
    }
}
