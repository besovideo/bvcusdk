/*====================================================
 * ManagedLayer.cpp
 *
 *  Created on: 2012-1-4
 *      Author: bv-corgan
 *  update on : 2013-12-17 20:22
 *      content: 增加串口发送数据： LIB_MANAGED_LAYER_API int SendTspData(BVCU_HDialog hDialog, 
										SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen)
 *      redactor: l c l
 =====================================================*/

#include "ManagedLayer.h"

#ifdef _DEBUG
#include "DbgConsoleOut.h"
#endif

LIB_MANAGED_LAYER_API int ManagedLayer_CuInit(const int** handle)
{
    CBVCU* bvcu = new CBVCU();
    *handle = (int*)bvcu;
    return bvcu->init();
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuLogin(const int* handle, BVCU_HSession* session, char* serverIp,
        int serverPort, char* usrName, char* usrPsw, int timeOutSec, BVCU_Server_OnEvent onEvent,
	BVCU_Server_Notify onNotify)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->login(session, serverIp, serverPort, usrName, usrPsw, timeOutSec, onEvent, onNotify);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuLogout(const int* handle, BVCU_HSession session)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->logout(session);
}

LIB_MANAGED_LAYER_API void ManagedLayer_CuRelease(const int* handle)
{
    CBVCU* bvcu = (CBVCU*)handle;
    bvcu->release();
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuCloseDialog(const int* handle, BVCU_HDialog* dlg)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->closeDlg(dlg);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuBrowsePu(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
        char* puId, int channelNo, HWND hWnd, BVDisplay_RECT* dispRect, int volume, int singleRecFileSec,
        char* recFileDir, bool videoTrans,  BVCU_DialogControlParam_Network* netWork,
        BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent)
{
    CBVCU* bvcu = (CBVCU*)handle;
    RECT rect;
    rect.top = dispRect->iTop;
    rect.bottom = dispRect->iBottom;
    rect.left = dispRect->iLeft;
    rect.right = dispRect->iRight;
    return bvcu->openBrowseDlg(dlg, session, puId, channelNo, hWnd, &rect, volume, singleRecFileSec, recFileDir,
            videoTrans, netWork, onDlgEvent, onStoreEvent);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuPlayVolume(const int* handle, BVCU_HDialog dlg, int volume)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->playVolumeSet(dlg, volume);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSpeakVolume(const int* handle, BVCU_HDialog dlg, int volume)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->speakVolumeSet(dlg, volume);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuChangeWindow(const int* handle, BVCU_HDialog dlg, HWND hWnd, BVDisplay_RECT* dispRect)
{
    CBVCU* bvcu = (CBVCU*)handle;
    RECT rect;
    rect.top = dispRect->iTop;
    rect.left = dispRect->iLeft;
    rect.right = dispRect->iRight;
    rect.bottom = dispRect->iBottom;
    return bvcu->changeDisplayWindow(dlg, hWnd, &rect);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSetRecordStorageParam(const int* handle, BVCU_HDialog dlg, char* fileDir,
        int fileSec)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->setRecordStorageParam(dlg, fileDir, fileSec);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSnapshot(const int* handle, BVCU_HDialog dlg, char* fileDir, int quality)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->snapshot(dlg, fileDir, quality);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuAVStop(const int* handle, BVCU_HDialog dlg, bool stopA, bool stopV,
        bool stopSpeak, bool releaseHWND)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->stopAudioOrVideo(dlg, stopA, stopV, stopSpeak, releaseHWND);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuNewTalk(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
        char* puId, int channelNo, int captureVolume, int audioVolume, BVCU_DialogControlParam_Network* netWork,
        BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->newTalkOnly(dlg, session, puId, channelNo, captureVolume, audioVolume, netWork, onDlgEvent, onStoreEvent);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuResizeDialogWindow(const int* handle, BVCU_HDialog dlg, BVDisplay_RECT* newRect)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->resizeDialogWindow(dlg, newRect);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuApplyControlParam(const int* handle, BVCU_HDialog dlg,
            int speakVolume, BVCU_DialogControlParam_Network* netWork)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->applyControlParam(dlg, speakVolume, netWork);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenGpsDialog(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, 
    BVCU_GpsDialog_OnEvent onDlgEvent, BVCU_GpsDialog_OnData onDlgData)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->openGpsDialog(dlg, session, puId, channelNo, onDlgEvent, onDlgData);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenTspDialog(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo,
	BVCU_TspDialog_OnEvent onDlgEvent, BVCU_TspDialog_OnData onDlgData)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->openTspDialog(dlg, session, puId, channelNo, onDlgEvent, onDlgData);
}

/*
 * Send command
 */
LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuControlResultProcFunc(const int* handle, BVCU_Cmd_ControlResult onCtrlRes)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->setControlResultProcFunc(onCtrlRes);
}

LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuQueryResultProcFunc(const int* handle, BVCU_Cmd_QueryResult onQueryRes)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->getBVCUSndCmd()->setQueryResultProcFunc(onQueryRes);
}


LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuList(const int* handle, BVCU_HSession session, BVCU_Cmd_OnGetPuList onEvent)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->getPuList(session, onEvent);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuDeviceInfo(const int* handle, BVCU_HSession session, char* puId,
            int device, BVCU_PUCFG_DeviceInfo* cmdData)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->setPuDeviceInfo(session, puId, device, cmdData);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuDeviceInfo(const int* handle, BVCU_HSession session, char* puId,
            int device, BVCU_Cmd_OnGetPuDeviceInfo onEvent)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->getPuDeviceInfo(session, puId, device, onEvent);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
        int device, BVCU_PUCFG_PTZAttr* cmdData)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->setPuPtzAttr(session, puId, device, cmdData);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
        int device, BVCU_Cmd_OnGetPuPtzAttr onEvent)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->getPuPtzAttr(session, puId, device, onEvent);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzControl(const int* handle, BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_PTZControl* cmdData)
{
    CBVCU* bvcu = (CBVCU*)handle;
    return bvcu->getBVCUSndCmd()->setPuPtzControl(hSession, puId, device, cmdData);
}


/*
 * Display after render
 */
LIB_MANAGED_LAYER_API void ManagedLayer_DispSetDisplayFontFunc(DisplayFont func)
{
    CBVDisplay::setDisplayFontFunc(func);
}


//lcl 2013-12-17 20:29, 来源MFCDemo
//串口发送数据
LIB_MANAGED_LAYER_API int SendTspData(BVCU_HDialog hDialog, SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen)
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
		int ret =BVCU_Dialog_Write(hDialog, &SavPacket);

		return ret;
	}
	return BVCU_RESULT_E_FAILED;
}


LIB_MANAGED_LAYER_API int ManagedLayer_PuManualRemoteRecord(const int* handle, BVCU_HSession hSession, char *puId, int channelIndex, BVCU_PUCFG_ManualRecord* recordParam)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->getBVCUSndCmd()->setPuManualRemoteRecord(hSession, puId, channelIndex, recordParam);
}

LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuGpsInfo(const int * handle, BVCU_HSession session, char * puId, int device)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->getBVCUSndCmd()->getPuGpsInfo(session, puId, device);
}


LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuGpsInfoV2(const int * handle, BVCU_HSession session, char * puId, int device,void* userData)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->getBVCUSndCmd()->getPuGpsInfoV2(session, puId, device, userData);
}

LIB_MANAGED_LAYER_API int ManagedLayer_BVFileTransferOpen(const int* handle, BVCU_File_HTransfer* phTransfer, BVCU_File_TransferParam* pParam)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->fileTransferOpen(phTransfer, pParam);
}

LIB_MANAGED_LAYER_API int ManagedLayer_Search_File(const int* handle, BVCU_HSession hSession, char* szTargetId, BVCU_Search_Request* require, void* pUserData)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return (BVCU_Result)bvcu->getBVCUSndCmd()->getSearchFile(hSession, szTargetId, require, pUserData);
}

LIB_MANAGED_LAYER_API int ManagedLayer_BVFileTransferGetInfo(const int* handle, BVCU_File_HTransfer hTransfer, BVCU_File_TransferInfo* pInfo)
{
	CBVCU* bvcu = (CBVCU*)handle;
	return bvcu->getFileTransferInfo(hTransfer, pInfo);
}
