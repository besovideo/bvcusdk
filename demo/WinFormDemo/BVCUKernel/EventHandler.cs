/*
 * 本类用于处理消息回调
 */

using System;
using System.Collections.Generic;
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
            BVCU.ManagedLayer_CuSetPuQueryResultProcFunc(bvcuSdkHandle, onQueryResult);
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
            server_onNotify = new BVCU_Server_Notify(Server_OnNotify);
            cmd_OnGetPuList = new BVCU_Cmd_OnGetPuList(Cmd_OnGetPuList);

            onControlResult = new BVCU_Cmd_ControlResult(OnControlResult);
            m_showMessageBoxOnEvent = new ShowMessageDel(procShowMessageBoxOnEvent);
            onGetPtzAttr = new BVCU_Cmd_OnGetPuPtzAttr(OnGetPuPtzAttr);

            onQueryResult = new BVCU_Cmd_QueryResult(OnQueryResult);
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
            if (IntPtr.Zero == pGpsData) return;

            BVCU_PUCFG_GPSData gpsData = (BVCU_PUCFG_GPSData)Marshal.PtrToStructure(pGpsData, typeof(BVCU_PUCFG_GPSData));
            m_dialog.onGpsData(dialog, gpsData, len);
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
        public delegate void BVCU_Server_OnEvent(IntPtr session, int eventCode, ref BVCU_Event_Common eventCommon);
        public delegate void BVCU_Cmd_OnGetPuList(IntPtr session, IntPtr puId, IntPtr puName, int status, IntPtr ptPUOneChannelInfo, int finished);

        public BVCU_Server_OnEvent server_OnEvent;
        /// <summary>
        /// 服务器 Session 事件（session_open、session_close）
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

        #region On Server Notify

        public delegate void BVCU_Server_Notify(IntPtr session, IntPtr msgContent);
        public BVCU_Server_Notify server_onNotify;

        /// <summary>
        /// 服务器通知
        /// </summary>
        /// <param name="session"></param>
        /// <param name="msgContent"></param>
        void Server_OnNotify(IntPtr session, IntPtr msgContent)
        {
            BVCU_NotifyMsgContent content = (BVCU_NotifyMsgContent)Marshal.PtrToStructure(msgContent, typeof(BVCU_NotifyMsgContent));

            switch (content.iSubMethod)
            {
                case BVCU.BVCU_SUBMETHOD.PU_CHANNELINFO:
                    {
                        Server_ProcNotifyChannelInfo(session, content.stMsgContent.pData, content.stMsgContent.iDataCount);
                        break;
                    }
                case BVCU.BVCU_SUBMETHOD.BVCU_SUBMETHOD_EVENT_NOTIFY:
                    if (content.stMsgContent.pData != IntPtr.Zero)
                    {
                        BVCU_Event_Source source = (BVCU_Event_Source)Marshal.PtrToStructure(content.stMsgContent.pData, typeof(BVCU_Event_Source));
                        OnEventNotify(source);
                    }
                    break;
            }
        }

        #endregion

        /// <summary>
        /// 事件报警
        /// </summary>
        /// <param name="source"></param>
        private void OnEventNotify(BVCU_Event_Source source)
        {
            if(null != m_session)
            {
                m_session.onShowAlarmEventMessage(source);
            }
        }

        public class PuInfo
        {
            public BVCU_PUChannelInfo puChannelInfo;
            public BVCU_PUOneChannelInfo[] channels;
        }

        /// <summary>
        /// Pu status change
        /// </summary>
        private void onNotifyChannelInfo(BVCU_PUChannelInfo puChannelInfo,BVCU_PUOneChannelInfo puOneChannelInfo)
        {
            bool bNeedClearGps = false;
            bool bNeedClearTsp = false;
            if (puChannelInfo.iOnlineStatus == BVCU.BVCU_ONLINE_STATUS_OFFLINE)
            {
                m_dialog.clearDialog(puChannelInfo.szPUID, puOneChannelInfo.iChannelIndex);
                bNeedClearGps = m_dialog.clearGpsDialog(puChannelInfo.szPUID, puOneChannelInfo.iChannelIndex);
                bNeedClearTsp = m_dialog.clearTspDialog(puChannelInfo.szPUID, puOneChannelInfo.iChannelIndex);
                m_session.clearGpsDataList(puChannelInfo.szPUID, puOneChannelInfo.iChannelIndex, bNeedClearGps | bNeedClearTsp);
                //m_session.OnRemovePu(puChannelInfo.szPUID);
            }

            Channel chnl = m_session.getChannel(puChannelInfo.szPUID, puOneChannelInfo.iChannelIndex);
            if (null == chnl)
            {
                chnl = new Channel();
            }

            getChannel(chnl, puOneChannelInfo);
            m_session.OnGetPu(puChannelInfo.szPUName, puChannelInfo.szPUID, puChannelInfo.iOnlineStatus, chnl);
        }

        void Server_ProcNotifyChannelInfo(IntPtr session, IntPtr puChannelInfo, int puChannelCnt)
        {
            PuInfo[] puChannels = new PuInfo[puChannelCnt];
            IntPtr puChannelInfoPt = puChannelInfo;
            for (int i = 0; i < puChannels.Length; i++)
            {
                puChannels[i] = new PuInfo();
                puChannels[i].puChannelInfo = (BVCU_PUChannelInfo)Marshal.PtrToStructure(puChannelInfoPt, typeof(BVCU_PUChannelInfo));
                puChannels[i].channels = new BVCU_PUOneChannelInfo[puChannels[i].puChannelInfo.iChannelCount];
                IntPtr channelPt = puChannels[i].puChannelInfo.pChannel;
                for (int j = 0; j < puChannels[i].channels.Length; j++)
                {
                    puChannels[i].channels[j] = (BVCU_PUOneChannelInfo)Marshal.PtrToStructure(channelPt, typeof(BVCU_PUOneChannelInfo));
                    onNotifyChannelInfo(puChannels[i].puChannelInfo, puChannels[i].channels[j]);
                    channelPt += Marshal.SizeOf(typeof(BVCU_PUOneChannelInfo));
                }
                puChannelInfoPt += Marshal.SizeOf(typeof(BVCU_PUChannelInfo));
            }

            m_session.OnGetPuListFinished();
        }

        /// <summary>
        /// 获得设备列表的响应函数
        /// </summary>
        public BVCU_Cmd_OnGetPuList cmd_OnGetPuList;
        void Cmd_OnGetPuList(IntPtr session, IntPtr ptPuId, IntPtr ptPuName, int iOnlineStatus, IntPtr ptPUOneChannelInfo, int iFinished)
        {
            BVCU_PUOneChannelInfo puOneChannelInfo = (BVCU_PUOneChannelInfo)Marshal.PtrToStructure(ptPUOneChannelInfo, typeof(BVCU_PUOneChannelInfo));

            Byte[] bpuid = new Byte[BVCU.BVCU_MAX_ID_LEN + 1];
            Byte[] bpuname = new Byte[BVCU.BVCU_MAX_NAME_LEN + 1];
            Marshal.Copy(ptPuId, bpuid, 0, BVCU.BVCU_MAX_ID_LEN + 1);
            Marshal.Copy(ptPuName, bpuname, 0, BVCU.BVCU_MAX_ID_LEN + 1);
            string spuid = System.Text.Encoding.UTF8.GetString(bpuid).Split('\0')[0];
            string spuname = System.Text.Encoding.UTF8.GetString(bpuname).Split('\0')[0];

            if (BVCU.TRUE(iFinished))
            {
                m_session.OnGetPuListFinished();
                return;
            }
            Channel chnl = new Channel();
            getChannel(chnl, puOneChannelInfo);
            m_session.OnGetPu(spuname, spuid,iOnlineStatus, chnl);
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

        /// <summary>
        /// 控制消息
        /// </summary>
        public delegate void BVCU_Cmd_ControlResult(IntPtr session, IntPtr puId, Int32 device, Int32 subMethod, Int32 result);

        BVCU_Cmd_ControlResult onControlResult;
        void OnControlResult(IntPtr session, IntPtr ptPuId, Int32 device, Int32 subMethod, Int32 result)
        {
        }


        public class BVSearchResponse
        {
            public BVCU_SearchInfo searchInfo;
            public List<BVCU_Search_FileInfo> fileInfo;
            public List<BVCU_PUChannelInfo> pulist;

            public BVSearchResponse(BVCU_Search_Response searchResponse)
            {
                searchInfo = searchResponse.stSearchInfo;
                IntPtr ptr = searchResponse.pData;
                switch (searchResponse.stSearchInfo.iType)
                {
                    case (int)BVSDKAdapter.BVCU_SEARCH_TYPE.BVCU_SEARCH_TYPE_FILE:
                        fileInfo = new List<BVCU_Search_FileInfo>();
                        int size = Marshal.SizeOf(typeof(BVCU_Search_FileInfo));
                        for (int i = 0; i < searchResponse.iCount; i++)
                        {
                            BVCU_Search_FileInfo file = (BVCU_Search_FileInfo)Marshal.PtrToStructure(ptr, typeof(BVCU_Search_FileInfo));
                            fileInfo.Add(file);
                            ptr += Marshal.SizeOf(typeof(BVCU_Search_FileInfo));
                        }
                        break;
                    case (int)BVSDKAdapter.BVCU_SEARCH_TYPE.BVCU_SEARCH_TYPE_PULIST:
                        pulist = new List<BVCU_PUChannelInfo>();
                        for (int i = 0; i < searchResponse.iCount; i++)
                        {
                            pulist.Add((BVCU_PUChannelInfo)Marshal.PtrToStructure(ptr, typeof(BVCU_PUChannelInfo)));
                            ptr += Marshal.SizeOf(typeof(BVCU_PUChannelInfo));
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        public delegate void BVCU_Cmd_QueryResult(IntPtr session, IntPtr pCommand, IntPtr pEvent);
        public BVCU_Cmd_QueryResult onQueryResult;
        public void OnQueryResult(IntPtr session, IntPtr pCommand, IntPtr pEvent)
        {
            BVCU_Command command = (BVCU_Command)Marshal.PtrToStructure(pCommand, typeof(BVCU_Command));
            BVCU_Event_SessionCmd sessionCmd = (BVCU_Event_SessionCmd)Marshal.PtrToStructure(pEvent, typeof(BVCU_Event_SessionCmd));
            if (command.stMsgContent.pData == IntPtr.Zero || sessionCmd.stContent.pData == IntPtr.Zero)
                return;
            if (command.iSubMethod== 18)
            {
                Test_Struct s1 = (Test_Struct)Marshal.PtrToStructure(command.pUserData, typeof(Test_Struct));
                BVCU_PUCFG_GPSData gpsdata = (BVCU_PUCFG_GPSData)Marshal.PtrToStructure(sessionCmd.stContent.pData, typeof(BVCU_PUCFG_GPSData));
            }
            else if (command.iSubMethod == BVSDKAdapter.BVCU_SUBMETHOD.BVCU_SUBMETHOD_SEARCH_LIST)
            {
                BVCU_Search_Response searchResponse = (BVCU_Search_Response)Marshal.PtrToStructure(sessionCmd.stContent.pData, typeof(BVCU_Search_Response));
                if (searchResponse.stSearchInfo.iType == (Int32)BVSDKAdapter.BVCU_SEARCH_TYPE.BVCU_SEARCH_TYPE_FILE)
                {
                    BVCU_SearchInfo stSearchInfo = searchResponse.stSearchInfo;
                    BVSearchResponse searchRes = new BVSearchResponse(searchResponse);//读取文件列表
                    m_session.showSearchRecordFiles(searchRes, command.szTargetID);
                }

            }
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
