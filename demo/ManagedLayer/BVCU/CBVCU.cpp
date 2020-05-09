/*====================================================
 * CBVCU.cpp
 *
 *  Created on: 2012-2-1
 *      Author: bv-corgan
 =====================================================*/
#include "CBVCU.h"
#include "../Public.h"

#pragma comment(lib, "libBVCU.lib")

int CBVCU::init()
{
    BVCU_GlobalParam globalParam;
    memset(&globalParam, 0, sizeof(globalParam));
    globalParam.iSize = sizeof(globalParam);
    return BVCU_Initialize(&globalParam);
}

int CBVCU::login(BVCU_HSession* session, char* serverIP, int serverPort, char* userName, char* psw, int timeOutSec,
        BVCU_Server_OnEvent onEvent, BVCU_Cmd_OnGetPuList proc)
{
    BVCU_ServerParam svrParam;
    memset(&svrParam, 0, sizeof(svrParam));
    svrParam.iSize = sizeof(svrParam);
    strcpy_s(svrParam.szServerAddr,sizeof(svrParam.szServerAddr),serverIP);
    svrParam.iServerPort = serverPort;
    svrParam.iCmdProtoType = BVCU_PROTOTYPE_TCP;
    strcpy_s(svrParam.szUserName,sizeof(svrParam.szUserName),userName);
    strcpy_s(svrParam.szPassword,sizeof(svrParam.szPassword),psw);
    svrParam.iTimeOut = timeOutSec * 1000;
    strcat_s(svrParam.szUserAgent, "cu client");
    strcat_s(svrParam.szClientID, "CU_123456789");
	strcpy(svrParam.szUKeyID, "");
	strcpy(svrParam.szUkeyCode, "");
    svrParam.OnEvent = server_OnEvent;
    svrParam.OnNotify = server_OnNotify;
    BVCU_Result result = BVCU_Login(session, &svrParam);
    if (BVCU_Result_SUCCEEDED(result))
    {
        m_procChannelInfo = proc;
        m_procServerEvent = onEvent;
    }
    return result;
}

BVCU_Cmd_OnGetPuList CBVCU::m_procChannelInfo = NULL;

int CBVCU::procMsgContent(BVCU_CmdMsgContent* msgContent)
{
    int result = BVCU_RESULT_S_OK;
    /*
    switch (msgContent->iMsgContentType)
    {
    case BVCU_MSGCONTENT_TYPE_PUINFO:
        while(msgContent)
        {
            BVCU_PUChannelInfo* channelInfo = (BVCU_PUChannelInfo*)msgContent->pData;
            for(int i = 0; i < msgContent->iDataCount; i++)
            {
                result = m_procChannelInfo(&channelInfo[i]);
                if (BVCU_Result_FAILED(result))
                {
                    return result;
                }
            }
            msgContent = msgContent->pNext;
        }
        break;
    default:
        break;
    }*/
    return result;
}

BVCU_Server_OnEvent CBVCU::m_procServerEvent = NULL;

void CBVCU::server_OnEvent(BVCU_HSession hSession, int iEventCode, void* pParam)
{
    return m_procServerEvent(hSession, iEventCode, pParam);
}

BVCU_Result CBVCU::server_OnNotify(BVCU_HSession hSession, BVCU_NotifyMsgContent* pData)
{
    BVCU_PUChannelInfo* puChannelInfo = NULL;
    BVCU_PUOneChannelInfo* channel = NULL;
    if (NULL == pData || !m_procChannelInfo)
    {
        return BVCU_RESULT_S_OK;
    }
    if (pData->iSubMethod == BVCU_SUBMETHOD_PU_CHANNELINFO && pData->stMsgContent.iDataCount > 0)
    {
        puChannelInfo = (BVCU_PUChannelInfo*)(pData->stMsgContent.pData);
        if (NULL == puChannelInfo)
        {
            return BVCU_RESULT_S_OK;
        }
        for (int i = 0; i < pData->stMsgContent.iDataCount; i++)
        {
            channel = puChannelInfo->pChannel;
            for (int j = 0; j < puChannelInfo->iChannelCount; j++)
            {
                m_procChannelInfo(hSession, puChannelInfo->szPUID, puChannelInfo->szPUName, puChannelInfo->iOnlineStatus, channel, 0);
                channel++;
            }
            puChannelInfo++;
        }
        channel--;
        puChannelInfo--;
        m_procChannelInfo(hSession, puChannelInfo->szPUID, puChannelInfo->szPUName, puChannelInfo->iOnlineStatus, channel, 1);
    }
    return BVCU_RESULT_S_OK;
}

int CBVCU::logout(BVCU_HSession session)
{
    return BVCU_Logout(session);
}

void CBVCU::release()
{
    BVCU_Result result = BVCU_Finish();
    if (BVCU_Result_FAILED(result))
    {
        BV_DBG_CONSOLE_OUT("BVCU release failed, error code %d\n", result);
    }
}

BVCU_Dialog_OnDialogEvent CBVCU::m_procDialogEvent = NULL;

BVCU_Dialog_OnStorageEvent CBVCU::m_procStorageEvent = NULL;

int CBVCU::openBrowseDlg(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, HWND hWnd, RECT* dispRect,
        int volume, int singleRecFileSec, char* recFileDir, bool videoTrans, BVCU_DialogControlParam_Network* netWork,
        BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent)
{
    BVCU_DialogParam dlgParam;
    BVCU_DialogControlParam dlgCtlParam;
    memset(&dlgParam, 0, sizeof(dlgParam));
    memset(&dlgCtlParam, 0, sizeof(dlgCtlParam));
    dlgParam.iSize = sizeof(dlgParam);
    dlgCtlParam.iSize = sizeof(dlgCtlParam);

    if (volume >= 0 && videoTrans)
    {
        dlgParam.iAVStreamDir = BVCU_MEDIADIR_VIDEORECV | BVCU_MEDIADIR_AUDIORECV;
    }
    else if (volume >= 0)
    {
        dlgParam.iAVStreamDir = BVCU_MEDIADIR_AUDIORECV;
    }
    else if (videoTrans)
    {
        dlgParam.iAVStreamDir = BVCU_MEDIADIR_VIDEORECV;
    }
    else
    {
        return BVCU_RESULT_E_INVALIDARG;
    }
    dlgParam.hSession = session;
    dlgParam.OnEvent = dialog_OnEvent;
    BVCU_DialogTarget target;
    memset(&target, 0, sizeof(BVCU_DialogTarget));
    target.iIndexMajor = channelNo;
    dlgParam.afterRender = CBVDisplay::afterRender;
    strcpy_s(target.szID, BVCU_MAX_ID_LEN + 1, puId);
    target.iIndexMinor = -1;
    dlgParam.pTarget = &target;
    dlgParam.iTargetCount = 1;

    dlgCtlParam.stRender.hWnd = hWnd;
    dlgCtlParam.stRender.iPlackbackVolume = volume;
    dlgCtlParam.stRender.rcDisplay.iBottom = dispRect->bottom;
    dlgCtlParam.stRender.rcDisplay.iLeft = dispRect->left;
    dlgCtlParam.stRender.rcDisplay.iRight = dispRect->right;
    dlgCtlParam.stRender.rcDisplay.iTop = dispRect->top;
    dlgCtlParam.stStorage.iFileLenInSeconds = singleRecFileSec;
    if (recFileDir && recFileDir[0] != 0)
    {
        strcpy_s(dlgCtlParam.stStorage.szFilePath,sizeof(dlgCtlParam.stStorage.szFilePath),recFileDir);
    }
    else
    {
        dlgCtlParam.stStorage.szFilePath[0] = 0;
    }
    dlgCtlParam.stNetwork.iDelayMax = 10 * 1000;
    dlgCtlParam.stNetwork.iDelayMin = 1 * 1000;
    dlgCtlParam.stNetwork.iDelayVsSmooth = 3;
    BVCU_Result result = BVCU_Dialog_Open(dlg, &dlgParam, &dlgCtlParam);
    if (BVCU_Result_SUCCEEDED(result))
    {
        if (NULL == m_procDialogEvent)
        {
            m_procDialogEvent = onDlgEvent;
        }
        if (NULL == m_procStorageEvent)
        {
            m_procStorageEvent = onStoreEvent;
        }
    }
    return result;
}

void CBVCU::dialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam)
{
    BVCU_Event_Storage* storage = (BVCU_Event_Storage*)pParam;
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_RESULT_S_OK;
    switch(iEventCode)
    {
    case BVCU_EVENT_DIALOG_OPEN:
    case BVCU_EVENT_DIALOG_UPDATE:
    case BVCU_EVENT_DIALOG_CLOSE:
        result = BVCU_GetDialogInfo(hDialog, &dlgInfo);
        if (BVCU_Result_SUCCEEDED(result))
        {
            m_procDialogEvent(hDialog, iEventCode, ((BVCU_Event_Common*)pParam)->iResult, dlgInfo.stParam.iAVStreamDir);
        }
        break;
    case BVCU_EVENT_STORAGE_FILE_OPEN:
    case BVCU_EVENT_STORAGE_FILE_CLOSE:
    case BVCU_EVENT_STORAGE_ERROR:
        m_procStorageEvent(hDialog, iEventCode, storage->iResult, storage->szFileName, strlen(storage->szFileName), storage->iTimestamp);
        break;
    default:
        break;
    }
}

int CBVCU::setRecordStorageParam(BVCU_HDialog dlg, char* dir, int fileSec)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(dlg, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
	if(dir)
		strcpy_s(dlgInfo.stControlParam.stStorage.szFilePath,sizeof(dlgInfo.stControlParam.stStorage.szFilePath),dir);
	else
		dlgInfo.stControlParam.stStorage.szFilePath[0] = 0;
    dlgInfo.stControlParam.stStorage.iFileLenInSeconds = fileSec;
    return BVCU_Dialog_Control(dlg, &dlgInfo.stControlParam);
}

int CBVCU::closeDlg(BVCU_HDialog dlg)
{
    return BVCU_Dialog_Close(dlg);
}

int CBVCU::playVolumeSet(BVCU_HDialog dlg, int volume)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(dlg, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    dlgInfo.stControlParam.stRender.iPlackbackVolume = volume;
    return BVCU_Dialog_Control(dlg, &dlgInfo.stControlParam);
}

int CBVCU::speakVolumeSet(BVCU_HDialog dlg, int volume)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(dlg, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    dlgInfo.stControlParam.stRender.iCaptureVolume = volume;
    return BVCU_Dialog_Control(dlg, &dlgInfo.stControlParam);
}

int CBVCU::changeDisplayWindow(BVCU_HDialog dlg, HWND hWnd, RECT* dispRect)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(dlg, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    dlgInfo.stControlParam.stRender.hWnd = hWnd;
    dlgInfo.stControlParam.stRender.rcDisplay.iBottom = dispRect->bottom;
    dlgInfo.stControlParam.stRender.rcDisplay.iLeft = dispRect->left;
    dlgInfo.stControlParam.stRender.rcDisplay.iRight = dispRect->right;
    dlgInfo.stControlParam.stRender.rcDisplay.iTop = dispRect->top;
    return BVCU_Dialog_Control(dlg, &dlgInfo.stControlParam);
}

int CBVCU::snapshot(BVCU_HDialog hDialog, const char* szFileName, int iQuality)
{
    return BVCU_Dialog_Snapshot(hDialog, szFileName, iQuality);
}

int CBVCU::stopAudioOrVideo(BVCU_HDialog hDialog, bool stopAudio, bool stopVideo, bool stopSpeak, bool releaseHWND)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(hDialog, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    if (stopAudio)
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir & ~BVCU_MEDIADIR_AUDIORECV;
    }
    else
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir | BVCU_MEDIADIR_AUDIORECV;
    }
    if (stopVideo)
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir & ~BVCU_MEDIADIR_VIDEORECV;
    }
    else
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir | BVCU_MEDIADIR_VIDEORECV;
    }
    if (stopSpeak)
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir & ~BVCU_MEDIADIR_AUDIOSEND;
    }
    else
    {
        dlgInfo.stParam.iAVStreamDir = dlgInfo.stParam.iAVStreamDir | BVCU_MEDIADIR_AUDIOSEND;
    }
    if (releaseHWND)
    {
        dlgInfo.stControlParam.stRender.hWnd = 0;
        dlgInfo.stControlParam.stRender.rcDisplay.iBottom = 0;
        dlgInfo.stControlParam.stRender.rcDisplay.iLeft = 0;
        dlgInfo.stControlParam.stRender.rcDisplay.iRight = 0;
        dlgInfo.stControlParam.stRender.rcDisplay.iTop = 0;
        result = BVCU_Dialog_Control(hDialog, &dlgInfo.stControlParam);
        if (BVCU_Result_FAILED(result))
        {
            return result;
        }
    }
    return BVCU_Dialog_Update(hDialog, &dlgInfo.stParam);
}

int CBVCU::newTalkOnly(BVCU_HDialog* hDialog, BVCU_HSession session, char* puId, int channelNo, int captureVolume,
        int audioVolume, BVCU_DialogControlParam_Network* netWork, BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent)
{
    BVCU_DialogParam dlgParam;
    BVCU_DialogControlParam dlgCtlParam;
    memset(&dlgParam, 0, sizeof(dlgParam));
    memset(&dlgCtlParam, 0, sizeof(dlgCtlParam));
    dlgParam.iSize = sizeof(dlgParam);
    dlgCtlParam.iSize = sizeof(dlgCtlParam);

    dlgParam.iAVStreamDir = BVCU_MEDIADIR_TALKONLY;
    dlgParam.hSession = session;
    dlgParam.OnEvent = dialog_OnEvent;
    BVCU_DialogTarget target;
    memset(&target, 0, sizeof(BVCU_DialogTarget));
    target.iIndexMajor = channelNo;
    dlgParam.afterRender = CBVDisplay::afterRender;
    strcpy_s(target.szID, BVCU_MAX_ID_LEN + 1, puId);
    target.iIndexMinor = -1;
    dlgParam.pTarget = &target;
    dlgParam.iTargetCount = 1;
    dlgCtlParam.stRender.iCaptureVolume = captureVolume;
    dlgCtlParam.stRender.iPlackbackVolume = audioVolume;
    
    dlgCtlParam.stNetwork.iDelayMax = 10 * 1000;
    dlgCtlParam.stNetwork.iDelayMin = 1 * 1000;
    dlgCtlParam.stNetwork.iDelayVsSmooth = 3;
    BVCU_Result result = BVCU_Dialog_Open(hDialog, &dlgParam, &dlgCtlParam);
    if (BVCU_Result_SUCCEEDED(result))
    {
        if (NULL == m_procDialogEvent)
        {
            m_procDialogEvent = onDlgEvent;
        }
        if (NULL == m_procStorageEvent)
        {
            m_procStorageEvent = onStoreEvent;
        }
    }
    return result;
}

int CBVCU::resizeDialogWindow(BVCU_HDialog hDialog, BVDisplay_RECT* newRect)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(hDialog, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    dlgInfo.stControlParam.stRender.rcDisplay.iBottom = newRect->iBottom;
    dlgInfo.stControlParam.stRender.rcDisplay.iLeft = newRect->iLeft;
    dlgInfo.stControlParam.stRender.rcDisplay.iRight = newRect->iRight;
    dlgInfo.stControlParam.stRender.rcDisplay.iTop = newRect->iTop;
    return BVCU_Dialog_Control(hDialog, &dlgInfo.stControlParam);
}

CBVCUSndCmd* CBVCU::getBVCUSndCmd()
{
    return &m_cBVCUSndCmd;
}

int CBVCU::applyControlParam(BVCU_HDialog dlg, int speakVolume, BVCU_DialogControlParam_Network* netWork)
{
    BVCU_DialogInfo dlgInfo;
    BVCU_Result result = BVCU_GetDialogInfo(dlg, &dlgInfo);
    if (BVCU_Result_FAILED(result))
    {
        return result;
    }
    dlgInfo.stControlParam.stRender.iCaptureVolume = speakVolume;
    dlgInfo.stControlParam.stNetwork.iDelayMax = netWork->iDelayMax;
    dlgInfo.stControlParam.stNetwork.iDelayMin = netWork->iDelayMin;
    dlgInfo.stControlParam.stNetwork.iDelayVsSmooth = netWork->iDelayVsSmooth;
    return BVCU_Dialog_Control(dlg, &dlgInfo.stControlParam);
}

BVCU_GpsDialog_OnEvent CBVCU::m_procGpsDialogEvent = NULL;
BVCU_GpsDialog_OnData CBVCU::m_procGpsDialogOnData = NULL;
BVCU_TspDialog_OnEvent CBVCU::m_procTspDialogEvent = NULL;
BVCU_TspDialog_OnData CBVCU::m_procTspDialogOnData = NULL;

int CBVCU::openGpsDialog(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, 
    BVCU_GpsDialog_OnEvent onDlgEvent, BVCU_GpsDialog_OnData onDlgData)
{
    BVCU_DialogParam dlgParam;
    BVCU_DialogControlParam dlgCtlParam;
    memset(&dlgParam, 0, sizeof(dlgParam));
    memset(&dlgCtlParam, 0, sizeof(dlgCtlParam));
    dlgParam.iSize = sizeof(dlgParam);
    dlgCtlParam.iSize = sizeof(dlgCtlParam);
    BVCU_DialogTarget target;
    memset(&target, 0, sizeof(BVCU_DialogTarget));
    target.iIndexMajor = channelNo;
    strcpy_s(target.szID, BVCU_MAX_ID_LEN + 1, puId);
    target.iIndexMinor = -1;
    dlgParam.pTarget = &target;
    dlgParam.iTargetCount = 1;
    dlgParam.iAVStreamDir = BVCU_MEDIADIR_DATARECV;
	//dlgParam.bOverTCP = bOverTCP;
    dlgParam.hSession = session;
    dlgParam.OnEvent = gpsDialog_OnEvent;
    dlgParam.afterDecode = gpsDialog_OnData;
    if (NULL == m_procGpsDialogEvent)
    {
        m_procGpsDialogEvent = onDlgEvent;
    }
    if (NULL == m_procGpsDialogOnData)
    {
        m_procGpsDialogOnData = onDlgData;
    }

    return BVCU_Dialog_Open(dlg, &dlgParam, &dlgCtlParam);
}

BVCU_Result CBVCU::gpsDialog_OnData(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame)
{
    if (pFrame->ppData[0] != NULL)
    {
		//m_procGpsDialogOnData(hDialog, ((BVCU_PUCFG_GPSData*)(pFrame->ppData[0]))->iLatitude, ((BVCU_PUCFG_GPSData*)(pFrame->ppData[0]))->iLongitude);
		m_procGpsDialogOnData(hDialog, (BVCU_PUCFG_GPSData*)pFrame->ppData[0], pFrame->iDataSize[0]);
    }
    return BVCU_RESULT_S_OK;
}

void CBVCU::gpsDialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam)
{
    if (m_procGpsDialogOnData != NULL)
    {
        m_procGpsDialogEvent(hDialog, iEventCode, ((BVCU_Event_Common*)pParam)->iResult);
    }
}

int CBVCU::openTspDialog(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo,
	BVCU_TspDialog_OnEvent onDlgEvent, BVCU_TspDialog_OnData onDlgData)
{
	BVCU_DialogParam dlgParam;
	BVCU_DialogControlParam dlgCtlParam;
	memset(&dlgParam, 0, sizeof(dlgParam));
	memset(&dlgCtlParam, 0, sizeof(dlgCtlParam));
	dlgParam.iSize = sizeof(dlgParam);
	dlgCtlParam.iSize = sizeof(dlgCtlParam);

	dlgParam.hSession = session;
	BVCU_DialogTarget target;
	strncpy_s(target.szID,BVCU_MAX_ID_LEN+1,puId,_TRUNCATE);
//	memset(&target, 0, sizeof(BVCU_DialogTarget));
	target.iIndexMajor = channelNo;
//	strcpy_s(target.szID, BVCU_MAX_ID_LEN + 1, puId);
	target.iIndexMinor = -1;
	dlgParam.iTargetCount = 1;
	dlgParam.pTarget = &target;
	dlgParam.iAVStreamDir = BVCU_MEDIADIR_DATARECV | BVCU_MEDIADIR_DATASEND;
	dlgParam.OnEvent = tspDialog_OnEvent;
	dlgParam.afterDecode = tspDialog_OnData;
	if (NULL == m_procGpsDialogEvent)
	{
		m_procTspDialogEvent = onDlgEvent;
	}
	if (NULL == m_procGpsDialogOnData)
	{
		m_procTspDialogOnData = onDlgData;
	}

	// <TODO>: ²Î¿¼MLibBVCU.cpp
	return BVCU_Dialog_Open(dlg, &dlgParam, &dlgCtlParam);
}

BVCU_Result CBVCU::tspDialog_OnData(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame)
{
	if (pFrame->ppData[0] != NULL)
	{
		pFrame->ppData[0][pFrame->iDataSize[0]] = '\0';
		m_procTspDialogOnData(hDialog, (char*)pFrame->ppData[0], pFrame->iDataSize[0]);
	}
	return BVCU_RESULT_S_OK;
}

void CBVCU::tspDialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam)
{
	if (m_procTspDialogOnData != NULL)
	{
		m_procTspDialogEvent(hDialog, iEventCode, ((BVCU_Event_Common*)pParam)->iResult);
	}
}