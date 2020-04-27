
// MFCDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "libBVCU/MLibBVCU.h"
#include "Config.h"
#include "MDataList.h"


//����ָ�ͨ����ʾ��Ϣ
typedef struct _SplitInfoNode
{
	BVCU_HDialog pDialog; // �����Dialog
	char sPUID[BVCU_MAX_ID_LEN+1]; // PU ID
	BVCU_PUOneChannelInfo stChannel; // open channel info
	bool bSaveData;   //�����Ƿ񱣴�

	DWORD iPTS;  //��ǰ֡ʱ���
}SplitInfoNode;

// CMFCDemoApp:
// �йش����ʵ�֣������ MFCDemo.cpp
//

class CMFCDemoApp : public CWinApp
{
public:
	CMFCDemoApp();
	static HANDLE m_loginEvent;
	static int    m_loginStatus;

	static SplitInfoNode m_talkDialg; // �Խ�Dialog��ֻ������һ��Dialog����ֻ֧��һ·�Խ�
	static BVCU_HDialog m_SendTspDialog; // ����͸����������Dialog

	static CConfig m_config;
	/* ��ʱʱ�� ��λ �� */
	static int    m_timeOut;
	static CMDataList m_data;

	static char   m_SnapshotFile[256];
	static char   m_VideoFilePath[256];
// ��д
public:
	virtual BOOL InitInstance();

	static void  WINAPI OnSessionEvent(BVCU_HSession hSession, int iEventCode, void* pParam);
	static void  WINAPI OnDialogEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, int mediaDir);
	static void  WINAPI OnStorageEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, char* fileName, int strLen, SAV_TYPE_INT64 iTimestamp);
	static void  WINAPI OnGetPuList(BVCU_HSession hSession, char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo, int finished);
    static void  WINAPI OnRecvGPSData(BVCU_HDialog hDialog, BVCU_PUCFG_GPSData* pTSPData);
    static void  WINAPI OnRecvTSPData(BVCU_HDialog hDialog, const char* pGPSData, int len);
	static void  WINAPI OnAfterRender(BVCU_HDialog hDialog, DWORD iPTS);
// ʵ��

	DECLARE_MESSAGE_MAP()
};

class  CMFCDemoDlg;
extern CMFCDemoApp theApp;
extern CMFCDemoDlg* mainDlg;