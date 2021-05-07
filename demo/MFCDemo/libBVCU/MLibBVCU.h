#pragma once

// #include "Windows.h"
#include <afx.h>
extern "C"
{
#include "BVCU.h"
#include "BVCUConst.h"
#include "PUConfig.h"
#include "BVSearch.h"
}

typedef void (WINAPI *BVCU_Server_OnEvent)(BVCU_HSession hSession, int iEventCode, void* pParam);
typedef void (WINAPI *BVCU_Dialog_OnDialogEvent)(BVCU_HDialog hDialog, int eventCode, int errorCode, int mediaDir);
typedef void (WINAPI *BVCU_Dialog_OnStorageEvent)(BVCU_HDialog hDialog, int eventCode, int errorCode, char* fileName, int strLen, SAV_TYPE_INT64 iTimestamp);

typedef void (WINAPI *BVCU_Cmd_OnGetPuList)(BVCU_HSession hSession, char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo, int finished);
typedef void (WINAPI *BVCU_Dlg_OnRecvGPSData)(BVCU_HDialog hDialog, BVCU_PUCFG_GPSData* pGPSData);
typedef void (WINAPI *BVCU_Dlg_OnRecvTSPData)(BVCU_HDialog hDialog, const char* pTSPData, int len);
typedef void (WINAPI *BVCU_Dlg_AfterRender)(BVCU_HDialog hDialog,DWORD ipts);

typedef struct m_bvcu_lib_param
{
    BVCU_Cmd_OnGetPuList  onGetPuList;
    BVCU_Dlg_OnRecvGPSData onRecvGPSData;
    BVCU_Dlg_OnRecvTSPData onRecvTSPData;
    BVCU_Dlg_AfterRender  onAfterRender;

    BVCU_Server_OnEvent   onServerEvent;
    BVCU_Dialog_OnDialogEvent onDialogEvent;
    BVCU_Dialog_OnStorageEvent onStorageEvent;
}MBVCU_InitParam;

enum {
    PU_STATUS_OFFLINE = 0,
    PU_STATUS_ONLINE = 1 << 0,
    PU_STATUS_GETLIST = 1 << 1,
};

BOOL utf8ToAnsi(char* _dir, int _len, const char* _src);
BOOL ansiToUtf8(char* _dir, int _len, const char* _src);

int DataToHexString(char* _dir, int _dLen, const char* _src, int _sLen);
int HexStringToData(char* _dir, int _dLen, const char* _src);

class CMLibBVCU
{
public:
    static BVCU_HSession m_session;
    static int m_iSearchPUIndex;
    static int m_iLoadPUCount;
    static int m_iPUCount;

    static BVCU_Cmd_OnGetPuList m_OnGetPuList;
    static BVCU_Dlg_AfterRender m_OnAfterRender;
    static BVCU_Server_OnEvent  m_OnServerEvent;
    static BVCU_Dlg_OnRecvGPSData    m_OnRecvGPSData;
    static BVCU_Dlg_OnRecvTSPData    m_OnRecvTSPData;
    static BVCU_Dialog_OnDialogEvent m_OnDialogEvent;
    static BVCU_Dialog_OnStorageEvent m_OnStorageEvent;

    static BVCU_Result server_OnNotify(BVCU_HSession hSession, BVCU_NotifyMsgContent* pData);
    static void server_OnEvent(BVCU_HSession hSession, int iEventCode, void* pParam);
    static void dialog_OnEvent(BVCU_HDialog hDialog, int iEventCode, void* pParam);
    static void cmd_OnEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam);
    /* 获得 GPS 数据*/
    static BVCU_Result afterDecode_GPS(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
    static BVCU_Result afterDecode_TSP(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
    /* 在 afterRender 中叠加时间显示*/
    static BVCU_Result afterRender(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
public:

    static int  Initialize(const BVCU_GlobalParam* pParam, MBVCU_InitParam mParam);
    static int  Finish();

    static BVCU_HSession GetSession(){ return m_session; }

    static int  Login(char* IP, int port, char* name, char* password, int timeOut);
    static int  Logout();
    static int  GetPUList();
    static int  SearchPUList();
    static int  OpenGPS_TSP(BVCU_HDialog* phDialog,const char* puId,int channel);
    static int  Browse(BVCU_HDialog* phDialog,const char* puId,int channel,int avdir,
        HWND hwnd,BVCU_Display_Rect* rect);
    static int  RecordPlayRemote(BVCU_HDialog* phDialog, const char* filename, HWND hwnd, BVCU_Display_Rect* rect, int openAtTime, BOOL openVideo, BOOL openAudio); // 远程录像回放
    static int  RecordPlayRemoteJummp(BVCU_HDialog phDialog, int32_t jump); // 远程录像回放
    static int  RecordPlayRemoteStep(BVCU_HDialog phDialog); // 远程录像回放
    static int  RecordPlayRemotePause(BVCU_HDialog phDialog); // 远程录像回放
    static int  RecordPlayRemotePlay(BVCU_HDialog phDialog); // 远程录像回放
    static int  RecordPlayRemotePlaySpeed(BVCU_HDialog phDialog, int speed); // 远程录像
	static int TalkOnly(BVCU_HDialog* phDialog, const char* puId, int channel);
    static int  SaveVideo(BVCU_HDialog hDialog,const char* folder,int iFileLenInSeconds);
    static int  ResizeDialogWindow(BVCU_HDialog hDialog,RECT* newRect);

    static BVCU_Result SendCmd(const BVCU_Command* pCmd);

	static int CMLibBVCU::SendTspData(BVCU_HDialog hDialog, SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen);
};

