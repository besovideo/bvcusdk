#include "MLibBVCU.h"
#include "DbgConsoleOut.h"
#include <stdio.h>

#ifdef _WIN64
#pragma comment(lib, "libBVCU_x64.lib")
//#pragma comment(lib, "bvdisplay_x64.lib")
#else
#pragma comment(lib, "libBVCU.lib")
//#pragma comment(lib, "bvdisplay.lib")
#endif // _WIN64


BVCU_HSession CMLibBVCU::m_session = NULL;
int CMLibBVCU::m_iSearchPUIndex = 0;
int CMLibBVCU::m_iLoadPUCount = 0;
int CMLibBVCU::m_iPUCount = 0;

BVCU_Cmd_OnGetPuList CMLibBVCU::m_OnGetPuList = NULL;
BVCU_Server_OnEvent  CMLibBVCU::m_OnServerEvent = NULL;
BVCU_Dialog_OnDialogEvent CMLibBVCU::m_OnDialogEvent = NULL;
BVCU_Dialog_OnStorageEvent CMLibBVCU::m_OnStorageEvent = NULL;
BVCU_Dlg_AfterRender CMLibBVCU::m_OnAfterRender = NULL;
BVCU_Dlg_OnRecvGPSData CMLibBVCU::m_OnRecvGPSData = NULL;
BVCU_Dlg_OnRecvTSPData CMLibBVCU::m_OnRecvTSPData = NULL;


BVCU_Result CMLibBVCU::server_OnNotify(BVCU_HSession hSession, BVCU_NotifyMsgContent* pData)
{
	if (pData)
	{
		if (pData->iSubMethod == BVCU_SUBMETHOD_PU_CHANNELINFO && pData->stMsgContent.iDataCount > 0)
		{
			BVCU_PUChannelInfo* puChannelInfo = (BVCU_PUChannelInfo*)(pData->stMsgContent.pData);
			int j = pData->stMsgContent.iDataCount - 1;
			for (int i = 0; i < j; i ++)
			{
				m_OnGetPuList(hSession,puChannelInfo[i].szPUID,puChannelInfo[i].szPUName,puChannelInfo[i].iOnlineStatus,puChannelInfo[i].pChannel,puChannelInfo[i].iChannelCount,0);
			}
            if (j >= 0)
                m_OnGetPuList(hSession,puChannelInfo[j].szPUID,puChannelInfo[j].szPUName,puChannelInfo[j].iOnlineStatus,puChannelInfo[j].pChannel,puChannelInfo[j].iChannelCount,1);
			return BVCU_RESULT_S_OK;
		}
	}
	return BVCU_RESULT_E_FAILED;
}
void CMLibBVCU::server_OnEvent(BVCU_HSession hSession, int iEventCode, void* pParam)
{
	m_OnServerEvent(hSession,iEventCode,pParam);
	if(iEventCode == BVCU_EVENT_SESSION_CLOSE)
		m_session = NULL;
}
void CMLibBVCU::dialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam)
{
	BVCU_DialogInfo dlgInfo;
	BVCU_Result result = BVCU_RESULT_S_OK;
	switch(iEventCode)
	{
	case BVCU_EVENT_DIALOG_OPEN:
	case BVCU_EVENT_DIALOG_UPDATE:
	case BVCU_EVENT_DIALOG_CLOSE:
    {
        result = BVCU_GetDialogInfo(hDialog, &dlgInfo);
        if (BVCU_Result_SUCCEEDED(result)) {
            m_OnDialogEvent(hDialog, iEventCode, ((BVCU_Event_Common*)pParam)->iResult, dlgInfo.stParam.iAVStreamDir);
        }
    } break;
	case BVCU_EVENT_STORAGE_FILE_REQUIRENAME:
	case BVCU_EVENT_STORAGE_FILE_OPEN:
	case BVCU_EVENT_STORAGE_FILE_CLOSE:
	case BVCU_EVENT_STORAGE_ERROR:
    {
        BVCU_Event_Storage* storage = (BVCU_Event_Storage*)pParam;
        m_OnStorageEvent(hDialog, iEventCode, storage->iResult, storage->szFileName, strlen(storage->szFileName), storage->iTimestamp);
    } break;
	default:
		break;
	}
}
void CMLibBVCU::cmd_OnEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    BVCU_Event_SessionCmd* pEvent = (BVCU_Event_SessionCmd*)pParam;
    char szPuName[BVCU_MAX_NAME_LEN + 1] = { 0 };
	switch(pCommand->iMethod){
	case BVCU_METHOD_QUERY:
		if(pCommand->iSubMethod == BVCU_SUBMETHOD_PU_LIST){
            int iPercent = pEvent->iPercent;
			if (pEvent->stContent.iDataCount > 0)
			{
                int iFinished = iPercent == 100 ? 1 : 0;
				BVCU_PUChannelInfo* puChannelInfo = (BVCU_PUChannelInfo*)(pEvent->stContent.pData);
				int j = pEvent->stContent.iDataCount - 1;
				for (int i = 0; i < j; i ++)
		 		{
                    memset(szPuName, 0, sizeof(szPuName));
                    utf8ToAnsi(szPuName, BVCU_MAX_NAME_LEN + 1, puChannelInfo[i].szPUName);
					m_OnGetPuList(hSession,puChannelInfo[i].szPUID, 
                        szPuName,puChannelInfo[i].iOnlineStatus | PU_STATUS_GETLIST,
						puChannelInfo[i].pChannel,puChannelInfo[i].iChannelCount, iFinished);
				}

                if (j >= 0) {
                    memset(szPuName, 0, sizeof(szPuName));
                    utf8ToAnsi(szPuName, BVCU_MAX_NAME_LEN + 1, puChannelInfo[j].szPUName);
                    m_OnGetPuList(hSession, puChannelInfo[j].szPUID,
                        szPuName, puChannelInfo[j].iOnlineStatus | PU_STATUS_GETLIST,
                        puChannelInfo[j].pChannel, puChannelInfo[j].iChannelCount, iFinished);
                }
			}
            if (pEvent->iPercent == 100)
            {
                TRACE("加载设备完成\n");
                m_OnGetPuList(hSession, NULL, NULL, 0, NULL, 0, 1);
            }
        }
        else if (pCommand->iSubMethod == BVCU_SUBMETHOD_SEARCH_LIST) {
            if (pEvent->stContent.iDataCount > 0)
            {
                BVCU_Search_Response* pSearchRes = (BVCU_Search_Response*)(pEvent->stContent.pData);
                if (pSearchRes->stSearchInfo.iType == BVCU_SEARCH_TYPE_PULIST)
                {
                    m_iPUCount = pSearchRes->stSearchInfo.iTotalCount;
                    m_iSearchPUIndex += pSearchRes->iCount;
                    if (m_iSearchPUIndex >= m_iPUCount)
                        m_iSearchPUIndex = 0;
                    BVCU_PUChannelInfo* puChannelInfo = pSearchRes->pData.pPUList;
                    int j = pSearchRes->iCount - 1;
                    for (int i = 0; i < j; i++)
                    {
                        memset(szPuName, 0, sizeof(szPuName));
                        utf8ToAnsi(szPuName, BVCU_MAX_NAME_LEN + 1, puChannelInfo[i].szPUName);
                        m_OnGetPuList(hSession, puChannelInfo[i].szPUID,
                            szPuName, puChannelInfo[i].iOnlineStatus | PU_STATUS_GETLIST,
                            puChannelInfo[i].pChannel, puChannelInfo[i].iChannelCount, 0);
                    }
                    if (j >= 0) {
                        memset(szPuName, 0, sizeof(szPuName));
                        utf8ToAnsi(szPuName, BVCU_MAX_NAME_LEN + 1, puChannelInfo[j].szPUName);
                        m_OnGetPuList(hSession, puChannelInfo[j].szPUID,
                            szPuName, puChannelInfo[j].iOnlineStatus | PU_STATUS_GETLIST,
                            puChannelInfo[j].pChannel, puChannelInfo[j].iChannelCount, 1);
                    }
                }
            }
            else CMLibBVCU::GetPUList();
        }
        else if(pCommand->iSubMethod == BVCU_SUBMETHOD_PU_DEVICEINFO){
			if (BVCU_Result_SUCCEEDED(pEvent->iResult) && pEvent->stContent.iDataCount > 0)
			{
				BVCU_PUCFG_DeviceInfo* lpDevInfo = (BVCU_PUCFG_DeviceInfo*)pEvent->stContent.pData;
			}
		}
        else if(pCommand->iSubMethod == BVCU_SUBMETHOD_PU_PTZATTR){
			if (pEvent->stContent.iDataCount > 0)
			{
				BVCU_PUCFG_PTZAttr* ptzAttr = (BVCU_PUCFG_PTZAttr*)pEvent->stContent.pData;
			}
		}
		break;
	case BVCU_METHOD_CONTROL:
        if (pCommand->iSubMethod == BVCU_SUBMETHOD_USER_MODPASSWD)
        {
            if (BVCU_Result_SUCCEEDED(pEvent->iResult))
                TRACE("修改当前用户密码成功\n");
            else
                TRACE("修改当前用户密码失败\n");
        }
		break;
	case BVCU_METHOD_SUBSCRIBE:
		break;
	}
}
BVCU_Result CMLibBVCU::afterDecode_GPS(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame)
{
	if (hDialog && pFrame)
	{
		if (pFrame->ppData[0]  && pFrame->iDataSize[0] == sizeof(BVCU_PUCFG_GPSData))
		{
			if(m_OnRecvGPSData)
				m_OnRecvGPSData(hDialog, (BVCU_PUCFG_GPSData*)pFrame->ppData[0]);
		}
	}
	return BVCU_RESULT_S_OK;
}
BVCU_Result CMLibBVCU::afterDecode_TSP(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame)
{
    if (hDialog && pFrame)
    {
        if (pFrame->ppData[0])
        {
            if(m_OnRecvTSPData)
                m_OnRecvTSPData(hDialog, (char*)pFrame->ppData[0], pFrame->iDataSize[0]);
        }
    }
    return BVCU_RESULT_S_OK;
}
BVCU_Result CMLibBVCU::afterRender(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame)
{
	m_OnAfterRender(hDialog,pFrame->iPTS/SAV_TIME_BASE);
	return BVCU_RESULT_E_FAILED;
}

int  CMLibBVCU::Initialize(const BVCU_GlobalParam* pParam, MBVCU_InitParam mParam)
{
	m_session = NULL;
	m_OnGetPuList = mParam.onGetPuList;
	m_OnRecvGPSData = mParam.onRecvGPSData;
    m_OnRecvTSPData = mParam.onRecvTSPData;
	m_OnAfterRender = mParam.onAfterRender;
	m_OnServerEvent = mParam.onServerEvent;
	m_OnDialogEvent = mParam.onDialogEvent;
	m_OnStorageEvent= mParam.onStorageEvent;

#ifdef _DEBUG
	CDbgConsoleOut::RedirectIOToConsole();
#endif
	return BVCU_Initialize(pParam);
}
int  CMLibBVCU::Finish()
{
	Logout();
	return BVCU_Finish();
}

int  CMLibBVCU::Login(char* IP, int port, char* name, char* password, int timeOut)
{
	Logout();
	BVCU_ServerParam svrParam;
	memset(&svrParam,0,sizeof(svrParam));
	svrParam.iSize = sizeof(svrParam);
	strcpy_s(svrParam.szServerAddr,sizeof(svrParam.szServerAddr),IP);
	strcpy_s(svrParam.szUserName,sizeof(svrParam.szUserName),name);
	strcpy_s(svrParam.szPassword,sizeof(svrParam.szPassword),password);
	svrParam.iServerPort  = port;
	svrParam.iCmdProtoType = BVCU_PROTOTYPE_TCP;
	svrParam.iTimeOut = timeOut;
	strcat_s(svrParam.szUserAgent, "mfc demo");
	strcat_s(svrParam.szClientID, "CU_mfcDemo");
	svrParam.OnEvent = server_OnEvent;
	svrParam.OnNotify = server_OnNotify;
	return BVCU_Login(&m_session,&svrParam);
}
int  CMLibBVCU::Logout()
{
    m_iSearchPUIndex = 0;
    m_iLoadPUCount = 0;
    m_iPUCount = 0;
	if (m_session)
        return BVCU_Logout(m_session);
	return BVCU_RESULT_E_FAILED;
}
int  CMLibBVCU::GetPUList()
{
	if (m_session)
	{
		BVCU_Command cmdInfo;
		memset(&cmdInfo,0,sizeof(cmdInfo));
		cmdInfo.iSize = sizeof(cmdInfo);
		cmdInfo.iMethod = BVCU_METHOD_QUERY;
		cmdInfo.iSubMethod = BVCU_SUBMETHOD_PU_LIST;
		cmdInfo.OnEvent = cmd_OnEvent;
		return BVCU_SendCmd(m_session,&cmdInfo);
	}
	return BVCU_RESULT_E_FAILED;
}
int CMLibBVCU::SearchPUList()
{
    if (m_session)
    {
        BVCU_Search_Request stSearchReq;
        memset(&stSearchReq, 0x00, sizeof(stSearchReq));
        stSearchReq.stSearchInfo.iType = BVCU_SEARCH_TYPE_PULIST;
        stSearchReq.stSearchInfo.iPostition = m_iSearchPUIndex;
        stSearchReq.stSearchInfo.iCount = 128;
        BVCU_Command cmdInfo;
        memset(&cmdInfo, 0, sizeof(cmdInfo));
        cmdInfo.iSize = sizeof(cmdInfo);
        cmdInfo.iMethod = BVCU_METHOD_QUERY;
        cmdInfo.iSubMethod = BVCU_SUBMETHOD_SEARCH_LIST;
        cmdInfo.OnEvent = cmd_OnEvent;
        cmdInfo.stMsgContent.pData = &stSearchReq;
        cmdInfo.stMsgContent.iDataCount = 1;
        return BVCU_SendCmd(m_session, &cmdInfo);
    }
    return BVCU_RESULT_E_FAILED;
}
int  CMLibBVCU::OpenGPS_TSP(BVCU_HDialog* phDialog,const char* puId,int channel)
{
	if ( m_session && phDialog && puId )
	{
		BVCU_DialogParam dlgParam;
		BVCU_DialogControlParam dlgCtrlParam;
		memset(&dlgParam,0x00,sizeof(dlgParam));
		memset(&dlgCtrlParam,0x00,sizeof(dlgCtrlParam));
		dlgParam.iSize = sizeof(dlgParam);
		dlgCtrlParam.iSize = sizeof(dlgCtrlParam);

		dlgParam.hSession = m_session;
		BVCU_DialogTarget  PU;
		strncpy_s(PU.szID,BVCU_MAX_ID_LEN+1,puId,_TRUNCATE);
		PU.iIndexMajor = channel;
		PU.iIndexMinor = -1;
		dlgParam.iTargetCount = 1;
		dlgParam.pTarget = &PU;
		dlgParam.iAVStreamDir = BVCU_MEDIADIR_DATARECV | BVCU_MEDIADIR_DATASEND;
		dlgParam.OnEvent = dialog_OnEvent;
        if (BVCU_SUBDEV_INDEXMAJOR_MIN_GPS <= channel && channel <= BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
            dlgParam.afterDecode = afterDecode_GPS;
        else
            dlgParam.afterDecode = afterDecode_TSP;

		dlgCtrlParam.stNetwork.iDelayMax = 5000;
		dlgCtrlParam.stNetwork.iDelayMin = 1000;
		dlgCtrlParam.stNetwork.iDelayVsSmooth = 3;
		dlgCtrlParam.stRender.iPlackbackVolume = 50;

		return BVCU_Dialog_Open(phDialog,&dlgParam,&dlgCtrlParam);
	}
	return BVCU_RESULT_E_FAILED;
}
int  CMLibBVCU::Browse(BVCU_HDialog* phDialog,const char* puId,int channel,int avdir,
	 HWND hwnd,BVCU_Display_Rect* rect)
{
	if ( m_session && phDialog && puId && rect)
	{
		BVCU_DialogParam dlgParam;
		BVCU_DialogControlParam dlgCtrlParam;
		memset(&dlgParam,0x00,sizeof(dlgParam));
		memset(&dlgCtrlParam,0x00,sizeof(dlgCtrlParam));
		dlgParam.iSize = sizeof(dlgParam);
		dlgCtrlParam.iSize = sizeof(dlgCtrlParam);

		dlgParam.hSession = m_session;
		BVCU_DialogTarget  PU;
		strncpy_s(PU.szID,BVCU_MAX_ID_LEN+1,puId,_TRUNCATE);
		PU.iIndexMajor = channel;
		PU.iIndexMinor = -1;
		dlgParam.iTargetCount = 1;
		dlgParam.pTarget = &PU;
		dlgParam.iAVStreamDir = avdir;
		dlgParam.OnEvent = dialog_OnEvent;
		dlgParam.afterRender = afterRender;

		dlgCtrlParam.stNetwork.iDelayMax = 5000;
		dlgCtrlParam.stNetwork.iDelayMin = 1000;
		dlgCtrlParam.stNetwork.iDelayVsSmooth = 3;
		dlgCtrlParam.stRender.hWnd = hwnd;
		dlgCtrlParam.stRender.rcDisplay = *rect;
		dlgCtrlParam.stRender.iPlackbackVolume = 50;

		return BVCU_Dialog_Open(phDialog,&dlgParam,&dlgCtrlParam);
	}
	return BVCU_RESULT_E_FAILED;
}

int CMLibBVCU::RecordPlayRemote(BVCU_HDialog* phDialog, const char* filename, HWND hwnd, BVCU_Display_Rect* rect, int openAtTime, BOOL openVideo, BOOL openAudio)
{
    if (m_session && phDialog && rect)
    {
        BVCU_DialogTarget target;
        {
            memset(&target, 0, sizeof(BVCU_DialogTarget));
            target.iIndexMajor = BVCU_SUBDEV_INDEXMAJOR_PLAYBACK;//BVCU_SUBDEV_INDEXMAJOR_PLAYBACK
            target.iIndexMinor = 0;
            memcpy(target.szID, "NRU_", sizeof(target.szID));
        }

        BVCU_DialogParam dlgParam;
        {
            memset(&dlgParam, 0x00, sizeof(dlgParam));
            dlgParam.iSize = sizeof(dlgParam);

            dlgParam.OnEvent = dialog_OnEvent;
            dlgParam.afterRender = afterRender;
            dlgParam.hSession = m_session;
            // dlgParam.iAVStreamDir = BVCU_MEDIADIR_VIDEORECV; //avdir; //BVCU_MEDIADIR_VIDEORECV | BVCU_MEDIADIR_AUDIORECV;
            dlgParam.bOverTCP = 0;
            dlgParam.iTargetCount = 1;
            dlgParam.pTarget = &target;
            dlgParam.stFileTarget.pPathFileName = (char*)filename;
            dlgParam.stFileTarget.iStartTime = openAtTime; // 起始时间 s
            dlgParam.stFileTarget.iEndTime = -1;

            if (openVideo) {
                dlgParam.iAVStreamDir |= BVCU_MEDIADIR_VIDEORECV;
            }
            if (openAudio) {
                dlgParam.iAVStreamDir |= BVCU_MEDIADIR_AUDIORECV;
            }
        }

        BVCU_DialogControlParam dlgCtrlParam;
        {
            memset(&dlgCtrlParam, 0x00, sizeof(dlgCtrlParam));
            dlgCtrlParam.iSize = sizeof(dlgCtrlParam);

            dlgCtrlParam.stNetwork.iDelayMax = 5000;
            dlgCtrlParam.stNetwork.iDelayMin = 1000;
            dlgCtrlParam.stNetwork.iDelayVsSmooth = 3;
            dlgCtrlParam.stRender.hWnd = hwnd;
            dlgCtrlParam.stRender.rcDisplay = *rect;
            dlgCtrlParam.stRender.iPlackbackVolume = 50;

            //sprintf(dlgCtrlParam.stStorage.szFilePath, "G:/abc.mkv");
            //dlgCtrlParam.stStorage.iFileLenInSeconds = BVCU_STORAGE_MAX_FILELENINSEC;
        }

        return BVCU_Dialog_Open(phDialog, &dlgParam, &dlgCtrlParam);
    }

    return BVCU_RESULT_E_FAILED;
}

static void RecBackPlayOnEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    if (BVCU_EVENT_SESSION_CMD_COMPLETE == iEventCode) {
        BVCU_Event_SessionCmd* scmd = (BVCU_Event_SessionCmd*)pParam;
        if (scmd) {
            printf("RecBackPlayOnEvent: %d\n", scmd->iResult);
        }
    }
}

int CMLibBVCU::RecordPlayRemoteJummp(BVCU_HDialog phDialog, int32_t jump)
{
    if (m_session && phDialog)
    {
        BVCU_CTRL_RTSP rtsp;
        {
            memset(&rtsp, 0, sizeof(BVCU_CTRL_RTSP));
            rtsp.hDialog = phDialog;
            memcpy(rtsp.szCmdType, "PLAY", sizeof(rtsp.szCmdType)); // PLAY指定位置跳转
            rtsp.iRange_npt_begin = jump;
            rtsp.iRange_npt_end = -1;
            rtsp.iScale = -1;
        }

        BVCU_Command cmd;
        {
            memset(&cmd, 0, sizeof(BVCU_Command));
            cmd.iSize = sizeof(BVCU_Command);

            cmd.iMethod = BVCU_METHOD_CONTROL;
            cmd.iSubMethod = BVCU_SUBMETHOD_RTSP;
            cmd.stMsgContent.iDataCount = 1;
            cmd.stMsgContent.pData = &rtsp;
            cmd.OnEvent = RecBackPlayOnEvent;
        }

        return BVCU_SendCmd(m_session, &cmd);
    }

    return BVCU_RESULT_E_FAILED;
}


int CMLibBVCU::RecordPlayRemoteStep(BVCU_HDialog phDialog)
{
    if (m_session && phDialog)
    {
        BVCU_CTRL_RTSP rtsp;
        {
            memset(&rtsp, 0, sizeof(BVCU_CTRL_RTSP));
            rtsp.hDialog = phDialog;
            memcpy(rtsp.szCmdType, "PLAY", sizeof(rtsp.szCmdType)); // PLAY指定位置跳转
            rtsp.iRange_npt_begin = -2;
            rtsp.iRange_npt_end = -1;
            rtsp.iScale = -1;
        }

        BVCU_Command cmd;
        {
            memset(&cmd, 0, sizeof(BVCU_Command));
            cmd.iSize = sizeof(BVCU_Command);

            cmd.iMethod = BVCU_METHOD_CONTROL;
            cmd.iSubMethod = BVCU_SUBMETHOD_RTSP;
            cmd.stMsgContent.iDataCount = 1;
            cmd.stMsgContent.pData = &rtsp;
            cmd.OnEvent = RecBackPlayOnEvent;
        }

        return BVCU_SendCmd(m_session, &cmd);
    }

    return BVCU_RESULT_E_FAILED;
}

int CMLibBVCU::RecordPlayRemotePause(BVCU_HDialog phDialog)
{
    if (m_session && phDialog)
    {
        BVCU_CTRL_RTSP rtsp;
        {
            memset(&rtsp, 0, sizeof(BVCU_CTRL_RTSP));
            rtsp.hDialog = phDialog;
            memcpy(rtsp.szCmdType, "PAUSE", sizeof(rtsp.szCmdType)); // PLAY指定位置跳转
            rtsp.iRange_npt_begin = -1;
            rtsp.iRange_npt_end = -1;
            rtsp.iScale = -1;
        }

        BVCU_Command cmd;
        {
            memset(&cmd, 0, sizeof(BVCU_Command));
            cmd.iSize = sizeof(BVCU_Command);

            cmd.iMethod = BVCU_METHOD_CONTROL;
            cmd.iSubMethod = BVCU_SUBMETHOD_RTSP;
            cmd.stMsgContent.iDataCount = 1;
            cmd.stMsgContent.pData = &rtsp;
            cmd.OnEvent = RecBackPlayOnEvent;
        }

        return BVCU_SendCmd(m_session, &cmd);
    }

    return BVCU_RESULT_E_FAILED;
}

int CMLibBVCU::RecordPlayRemotePlay(BVCU_HDialog phDialog)
{
    if (m_session && phDialog)
    {
        BVCU_CTRL_RTSP rtsp;
        {
            memset(&rtsp, 0, sizeof(BVCU_CTRL_RTSP));
            rtsp.hDialog = phDialog;
            memcpy(rtsp.szCmdType, "PLAY", sizeof(rtsp.szCmdType)); // PLAY指定位置跳转
            rtsp.iRange_npt_begin = -1;
            rtsp.iRange_npt_end = -1;
            rtsp.iScale = -1;
        }

        BVCU_Command cmd;
        {
            memset(&cmd, 0, sizeof(BVCU_Command));
            cmd.iSize = sizeof(BVCU_Command);

            cmd.iMethod = BVCU_METHOD_CONTROL;
            cmd.iSubMethod = BVCU_SUBMETHOD_RTSP;
            cmd.stMsgContent.iDataCount = 1;
            cmd.stMsgContent.pData = &rtsp;
            cmd.OnEvent = RecBackPlayOnEvent;
        }

        return BVCU_SendCmd(m_session, &cmd);
    }

    return BVCU_RESULT_E_FAILED;
}

int CMLibBVCU::RecordPlayRemotePlaySpeed(BVCU_HDialog phDialog, int speed)
{
    if (m_session && phDialog)
    {
        BVCU_CTRL_RTSP rtsp;
        {
            memset(&rtsp, 0, sizeof(BVCU_CTRL_RTSP));
            rtsp.hDialog = phDialog;
            memcpy(rtsp.szCmdType, "PLAY", sizeof(rtsp.szCmdType)); // PLAY指定位置跳转
            rtsp.iRange_npt_begin = -1;
            rtsp.iRange_npt_end = -1;
            rtsp.iScale = speed;
        }

        BVCU_Command cmd;
        {
            memset(&cmd, 0, sizeof(BVCU_Command));
            cmd.iSize = sizeof(BVCU_Command);

            cmd.iMethod = BVCU_METHOD_CONTROL;
            cmd.iSubMethod = BVCU_SUBMETHOD_RTSP;
            cmd.stMsgContent.iDataCount = 1;
            cmd.stMsgContent.pData = &rtsp;
            cmd.OnEvent = RecBackPlayOnEvent;
        }

        return BVCU_SendCmd(m_session, &cmd);
    }

    return BVCU_RESULT_E_FAILED;
}

int CMLibBVCU::TalkOnly(BVCU_HDialog* phDialog, const char* puId, int channel)
{
	if ( m_session && phDialog && puId)
	{
		BVCU_DialogParam dlgParam;
		BVCU_DialogControlParam dlgCtrlParam;
		memset(&dlgParam,0x00,sizeof(dlgParam));
		memset(&dlgCtrlParam,0x00,sizeof(dlgCtrlParam));
		dlgParam.iSize = sizeof(dlgParam);
		dlgCtrlParam.iSize = sizeof(dlgCtrlParam);

		dlgParam.hSession = m_session;
		BVCU_DialogTarget  PU;
		strncpy_s(PU.szID,BVCU_MAX_ID_LEN+1,puId,_TRUNCATE);
		PU.iIndexMajor = channel;
		PU.iIndexMinor = -1;
		dlgParam.iTargetCount = 1;
		dlgParam.pTarget = &PU;
		dlgParam.iAVStreamDir = BVCU_MEDIADIR_TALKONLY;
		dlgParam.OnEvent = dialog_OnEvent;
		dlgParam.afterRender = afterRender;

		dlgCtrlParam.stNetwork.iDelayMax = 5000;
		dlgCtrlParam.stNetwork.iDelayMin = 1000;
		dlgCtrlParam.stNetwork.iDelayVsSmooth = 3;
		dlgCtrlParam.stRender.iPlackbackVolume = 100;
		dlgCtrlParam.stRender.iCaptureVolume = 100;

		return BVCU_Dialog_Open(phDialog,&dlgParam,&dlgCtrlParam);
	}
	return BVCU_RESULT_E_FAILED;
}
int  CMLibBVCU::SaveVideo(BVCU_HDialog hDialog,const char* folder,int iFileLenInSeconds)
{
	BVCU_DialogInfo dlgInfo;
	BVCU_DialogControlParam ctrlParam;
	int result = BVCU_RESULT_E_FAILED;
	if (hDialog)
	{
		result = BVCU_GetDialogInfo(hDialog,&dlgInfo);
		if (BVCU_Result_SUCCEEDED(result))
		{
			memcpy(&ctrlParam,&dlgInfo.stControlParam,sizeof(ctrlParam));
			if (folder)
				strncpy_s(ctrlParam.stStorage.szFilePath,BVCU_MAX_FILE_NAME_LEN+1,folder,_TRUNCATE);
			else
				ctrlParam.stStorage.szFilePath[0] = '\0';
			ctrlParam.stStorage.iFileLenInSeconds = iFileLenInSeconds;
			result = BVCU_Dialog_Control(hDialog,&ctrlParam);
		}
	}
	return result;
}
int  CMLibBVCU::ResizeDialogWindow(BVCU_HDialog hDialog,RECT* newRect)
{
	BVCU_DialogInfo dlgInfo;
	BVCU_Result result = BVCU_GetDialogInfo(hDialog, &dlgInfo);
	if (BVCU_Result_FAILED(result))
	{
		return result;
	}
	dlgInfo.stControlParam.stRender.rcDisplay.iBottom = newRect->bottom;
	dlgInfo.stControlParam.stRender.rcDisplay.iLeft = newRect->left;
	dlgInfo.stControlParam.stRender.rcDisplay.iRight = newRect->right;
	dlgInfo.stControlParam.stRender.rcDisplay.iTop = newRect->top;
	return BVCU_Dialog_Control(hDialog, &dlgInfo.stControlParam);
}

BVCU_Result CMLibBVCU::SendCmd(const BVCU_Command* pCmd)
{
	return BVCU_SendCmd(m_session, (BVCU_Command*)pCmd);
}

BOOL utf8ToAnsi(char* _dir, int _len, const char* _src)
{
	int len = MultiByteToWideChar(CP_UTF8,0,_src,-1,0,0);
	wchar_t* wchar = new wchar_t[len+1];
	if (!wchar)
		return FALSE;
	MultiByteToWideChar(CP_UTF8,0,_src,-1,wchar,len);
	WideCharToMultiByte(CP_ACP,0,wchar,-1,_dir,_len,NULL,NULL);
	delete []wchar;
	return TRUE;
}
BOOL ansiToUtf8(char* _dir, int _len, const char* _src)
{
	int len = MultiByteToWideChar(CP_ACP,0,_src,-1,0,0);
	wchar_t* wchar = new wchar_t[len+1];
	if (!wchar)
		return FALSE;
	MultiByteToWideChar(CP_ACP,0,_src,-1,wchar,len);
	WideCharToMultiByte(CP_UTF8,0,wchar,-1,_dir,_len,NULL,NULL);
	delete []wchar;
	return TRUE;
}

int DataToHexString(char* _dir, int _dLen, const char* _src, int _sLen)
{
    static char dtohs[2048];
    if (_src && _dir && _dLen > (2 * _sLen) && _sLen < 1024)
    {
        register int i;
        register int j = 0;
        register char c;
        for (i = _sLen - 1; i >= 0; --i) // 过滤数据后方的零
        {
            if (_src[i] != '\0')
                break;
        }
        _sLen = i + 1;
        i = 0;
        while (1)
        {
            // first 4 bit
            c = _src[i];
            c >>= 4;
            c &= 0x0f;
            c |= 0x30;
            if (c > 0x39)
                c += 7;
            dtohs[j++] = c;
            // last 4 bit
            c = _src[i++];
            c &= 0x0f;
            c |= 0x30;
            if (c > 0x39)
                c += 7;
            dtohs[j++] = c;

            if (i >= _sLen)
                break;
        }
        dtohs[j] = '\0';
        strcpy_s(_dir, _dLen, dtohs);
        return j;
    }
    return 0;
}
int HexStringToData(char* _dir, int _dLen, const char* _src)
{
    int _sLen = strlen(_src);
    if (_src && _dir && _dLen >= (_sLen / 2))
    {
        register int i = 0;
        register int j = 0;
        register char c;
        while (1)
        {
            if (_src[i] < '0' || _src[i] > 'f')
                break;
            // first 4 bit
            c = _src[i++];
            if (c > 0x39)
                c -= 7;
            c &= 0x0f;
            c <<= 4;
            _dir[j] = c;
            // last 4 bit
            c = _src[i++];
            if (c > 0x39)
                c -= 7;
            c &= 0x0f;
            _dir[j] |= c;
            j++;
            if (i >= _sLen)
                break;
        }
        i = j;
        while (j < _dLen)
        {
            _dir[j] = 0;
            ++j;
        }
        return i;
    }
    return 0;
}

int CMLibBVCU::SendTspData(BVCU_HDialog hDialog, SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen)
{
	if (hDialog)
	{
		SAV_Packet SavPacket;
		memset(&SavPacket, 0x00, sizeof(SavPacket));
		SavPacket.iSize = sizeof(SavPacket);
		SavPacket.iPTS = SAV_PTS_VALUE_NOSET;
		SavPacket.iDuration = 0;
		SavPacket.pData = SavData;
		SavPacket.iDataSize = iSavDataLen;
		SavPacket.iStreamIndex = 0;
		SavPacket.iFlags = 0;
		SavPacket.ePictType = SAV_PICTURE_TYPE_NONE;
		return BVCU_Dialog_Write(hDialog, &SavPacket);
	}
	return BVCU_RESULT_E_FAILED;
}