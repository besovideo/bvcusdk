
// MFCDemo.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "libBVCU/MLibBVCU.h"
#include "Config.h"
#include "MDataList.h"


//画面分割通道显示信息
typedef struct _SplitInfoNode
{
	BVCU_HDialog pDialog; // 浏览的Dialog
	char sPUID[BVCU_MAX_ID_LEN+1]; // PU ID
	BVCU_PUOneChannelInfo stChannel; // open channel info
	bool bSaveData;   //数据是否保存

	DWORD iPTS;  //当前帧时间戳
}SplitInfoNode;

// CMFCDemoApp:
// 有关此类的实现，请参阅 MFCDemo.cpp
//

class CMFCDemoApp : public CWinApp
{
public:
	CMFCDemoApp();
	static HANDLE m_loginEvent;
	static int    m_loginStatus;

	static SplitInfoNode m_talkDialg; // 对讲Dialog，只定义了一个Dialog，故只支持一路对讲
	static BVCU_HDialog m_SendTspDialog; // 发送透明串口数据Dialog

	static CConfig m_config;
	/* 超时时间 单位 秒 */
	static int    m_timeOut;
	static CMDataList m_data;

	static char   m_SnapshotFile[256];
	static char   m_VideoFilePath[256];
// 重写
public:
	virtual BOOL InitInstance();

	static void  WINAPI OnSessionEvent(BVCU_HSession hSession, int iEventCode, void* pParam);
	static void  WINAPI OnDialogEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, int mediaDir);
	static void  WINAPI OnStorageEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, char* fileName, int strLen, SAV_TYPE_INT64 iTimestamp);
	static void  WINAPI OnGetPuList(BVCU_HSession hSession, char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo, int finished);
    static void  WINAPI OnRecvGPSData(BVCU_HDialog hDialog, BVCU_PUCFG_GPSData* pTSPData);
    static void  WINAPI OnRecvTSPData(BVCU_HDialog hDialog, const char* pGPSData, int len);
	static void  WINAPI OnAfterRender(BVCU_HDialog hDialog, DWORD iPTS);
// 实现

	DECLARE_MESSAGE_MAP()
};

class  CMFCDemoDlg;
extern CMFCDemoApp theApp;
extern CMFCDemoDlg* mainDlg;