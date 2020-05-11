/*====================================================
 * CBVCU.h
 *
 *  Created on: 2012-2-1
 *      Author: bv-corgan
 =====================================================*/

#ifndef __CBVCU_H___
#define __CBVCU_H___

#include "Windows.h"
#include "locale.h"
extern "C"
{
#include "BVCU.h"
}
#include "../BVDisplay/CBVDisplay.h"
#include "CBVCUSndCmd.h"

using namespace std;

#define BVCU_WCHAR wchar_t
#define BVCU_IP_STRING_LEN 16
#define WINDOWS_MAX_FILE_PATH_LEN MAX_PATH
#define UNKNOW_DIALOG_EVENT_CODE -1
#define FALSE 0
#define TRUE 1

typedef void (WINAPI *BVCU_Server_OnEvent)(BVCU_HSession hSession, int iEventCode, void* pParam);
typedef void (WINAPI *BVCU_Dialog_OnDialogEvent)(BVCU_HDialog hDialog, int eventCode, int errorCode, int mediaDir);
typedef void (WINAPI *BVCU_Dialog_OnStorageEvent)(BVCU_HDialog hDialog, int eventCode, int errorCode, char* fileName, int strLen, SAV_TYPE_INT64 iTimestamp);
typedef void (WINAPI *BVCU_GpsDialog_OnEvent)(BVCU_HDialog hDialog, int iEventCode, int errorCode);
typedef void (WINAPI *BVCU_GpsDialog_OnData)(BVCU_HDialog hDialog, BVCU_PUCFG_GPSData* pGspData, int len);
typedef void (WINAPI *BVCU_TspDialog_OnEvent)(BVCU_HDialog hDialog, int iEventCode, int errorCode);
typedef void (WINAPI *BVCU_TspDialog_OnData)(BVCU_HDialog hDialog, const char* pTspData, int len);
typedef void (WINAPI *BVCU_Server_Notify)(BVCU_HSession hSession, BVCU_NotifyMsgContent* msgContent);

class CBVCU
{
    static BVCU_Cmd_OnGetPuList m_procChannelInfo;
    static BVCU_Server_OnEvent m_procServerEvent;
    static BVCU_Dialog_OnDialogEvent m_procDialogEvent;
    static BVCU_Dialog_OnStorageEvent m_procStorageEvent;
    static BVCU_GpsDialog_OnEvent m_procGpsDialogEvent;
    static BVCU_GpsDialog_OnData m_procGpsDialogOnData;
	static BVCU_TspDialog_OnEvent m_procTspDialogEvent;
	static BVCU_TspDialog_OnData m_procTspDialogOnData;
    static BVCU_Result server_OnNotify(BVCU_HSession hSession, BVCU_NotifyMsgContent* pData);
    static void server_OnEvent(BVCU_HSession hSession, int iEventCode, void* pParam);
    static int procMsgContent(BVCU_CmdMsgContent* msgContent);
    static void dialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam);
    static void gpsDialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam);
    static BVCU_Result gpsDialog_OnData(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
	static void tspDialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam);
	static BVCU_Result tspDialog_OnData(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);

    CBVCUSndCmd m_cBVCUSndCmd;
private:
	static BVCU_Server_Notify m_procServerNotify;

public:
    int init();
    void release();
    int login(BVCU_HSession* session, char* serverIP, int serverPort, char* userName, char* psw, int timeOutSec,
            BVCU_Server_OnEvent onEvent, BVCU_Server_Notify onNotify);
    int logout(BVCU_HSession session);

    int openBrowseDlg(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, HWND hWnd, RECT* dispRect,
            int volume, int singleRecFileSec, char* recFileDir, bool videoTrans, BVCU_DialogControlParam_Network* netWork,
            BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
    int closeDlg(BVCU_HDialog dlg);
    int playVolumeSet(BVCU_HDialog dlg, int volume);
    int speakVolumeSet(BVCU_HDialog dlg, int volume);
    int changeDisplayWindow(BVCU_HDialog dlg, HWND hWnd, RECT* dispRect);
    int setRecordStorageParam(BVCU_HDialog dlg, char* dir, int fileSec);
    int snapshot(BVCU_HDialog hDialog, const char* szFileName, int iQuality);
    int stopAudioOrVideo(BVCU_HDialog hDialog, bool stopAudio, bool stopVideo, bool stopSpeak, bool releaseHWND);
    int newTalkOnly(BVCU_HDialog* hDialog, BVCU_HSession session, char* puId, int channelNo, int captureVolume,
            int audioVolume, BVCU_DialogControlParam_Network* netWork,
            BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
    int resizeDialogWindow(BVCU_HDialog hDialog, BVDisplay_RECT* newRect);
    CBVCUSndCmd* getBVCUSndCmd();
    int applyControlParam(BVCU_HDialog dlg, int speakVolume, BVCU_DialogControlParam_Network* netWork);
    int openGpsDialog(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, 
            BVCU_GpsDialog_OnEvent onDlgEvent, BVCU_GpsDialog_OnData onDlgData);
	int openTspDialog(BVCU_HDialog* dlg, BVCU_HSession session, char* puId, int channelNo, 
		BVCU_TspDialog_OnEvent onDlgEvent, BVCU_TspDialog_OnData onDlgData);


    CBVCU()
    {
    }
    ~CBVCU()
    {
    }
};

#endif /* End of __CBVCU_H___ */
