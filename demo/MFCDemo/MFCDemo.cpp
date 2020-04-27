
// MFCDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PlayContainer.h"
#include "PlayWnd.h"
#include "PannelGPS.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HANDLE CMFCDemoApp::m_loginEvent = NULL;
int    CMFCDemoApp::m_loginStatus = BVCU_RESULT_E_FAILED;
SplitInfoNode CMFCDemoApp::m_talkDialg;
BVCU_HDialog CMFCDemoApp::m_SendTspDialog = NULL;

CConfig CMFCDemoApp::m_config(".\\config");
int    CMFCDemoApp::m_timeOut = 10;
CMDataList CMFCDemoApp::m_data;

char   CMFCDemoApp::m_SnapshotFile[256] = {0};
char   CMFCDemoApp::m_VideoFilePath[256]= {0};
// CMFCDemoApp

BEGIN_MESSAGE_MAP(CMFCDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCDemoApp 构造

CMFCDemoApp::CMFCDemoApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	ZeroMemory(&m_talkDialg, sizeof(m_talkDialg));
}


// 唯一的一个 CMFCDemoApp 对象

CMFCDemoApp theApp;

CMFCDemoDlg* mainDlg = NULL;

// CMFCDemoApp 初始化

BOOL CMFCDemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// Initialize lib
	BVCU_GlobalParam bParam;
	MBVCU_InitParam  mParam;
	bParam.iSize = sizeof(bParam);
	bParam.OnEvent = NULL;
	mParam.onGetPuList = OnGetPuList;
    mParam.onRecvGPSData = OnRecvGPSData;
    mParam.onRecvTSPData = OnRecvTSPData;
	mParam.onAfterRender = OnAfterRender;
	mParam.onServerEvent = OnSessionEvent;
	mParam.onDialogEvent = OnDialogEvent;
	mParam.onStorageEvent= OnStorageEvent;
	if (BVCU_Result_FAILED( CMLibBVCU::Initialize(&bParam,mParam) ))
	{
		MessageBox(NULL,_T("Initialize lib is failed!"),_T("Error"),MB_OK);
		return FALSE;
	}
	m_loginEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	// login seccess
	CMFCDemoDlg dlg;
	m_pMainWnd = &dlg;
	mainDlg = & dlg;
	dlg.DoModal();
	mainDlg = NULL;
	if (BVCU_Result_SUCCEEDED(m_loginStatus))
		CMLibBVCU::Logout();

	// Release lib
	CMLibBVCU::Finish();
	CloseHandle(m_loginEvent);

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
    // 开启命令行窗口 
    int hCrt;
    FILE *hf;
    AllocConsole();
    hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
    hf = _fdopen( hCrt, "w" );
    *stdout = *hf;
    int i = setvbuf( stdout, NULL, _IONBF, 0 );
	return FALSE;
}


void  CMFCDemoApp::OnSessionEvent(BVCU_HSession hSession, int iEventCode, void* pParam)
{
	BVCU_Event_Common* pEvent = (BVCU_Event_Common*)pParam;

	if(iEventCode == BVCU_EVENT_SESSION_CLOSE)
	{
		m_loginStatus = BVCU_RESULT_E_DISCONNECTED;
		if (pEvent->iResult == BVCU_RESULT_E_DISCONNECTED)
		{
			MessageBox(NULL,_T("与服务器断开了连接！"),_T("error"),MB_OK);
		}
		SendMessage(mainDlg->GetSafeHwnd(),WM_CLOSE,NULL,NULL);
	}
	else if(iEventCode == BVCU_EVENT_SESSION_OPEN){
		if(SAV_Result_SUCCEEDED(pEvent->iResult)){
			m_loginStatus = BVCU_RESULT_S_OK;
		}else{
			if(pEvent->iResult == BVCU_RESULT_E_TIMEOUT)
				m_loginStatus = BVCU_RESULT_E_TIMEOUT;
			else if (pEvent->iResult == BVCU_RESULT_E_AUTHORIZE_FAILED)
				m_loginStatus = BVCU_RESULT_E_AUTHORIZE_FAILED;
		}
		SetEvent(m_loginEvent);
	}
}
void  CMFCDemoApp::OnDialogEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, int mediaDir)
{
	switch(eventCode){
	case BVCU_EVENT_DIALOG_OPEN:
		if (mainDlg)
		{
			CPlayWnd* nPlayWnd = mainDlg->FindPlayWnd(hDialog);
			if (nPlayWnd)
			{
				if(BVCU_Result_SUCCEEDED(errorCode)){
					nPlayWnd->BeginRender(hDialog);
				}else{
					::PostMessage(nPlayWnd->m_hWnd,VIDEO_MENU_CLOSE,0,0);
				}
			}
            else
            {
                CPannelGPS* pWnd = mainDlg->GetPannelGPS();
                if (pWnd->GetDialog() == hDialog)
                {
                    if(BVCU_Result_FAILED(errorCode)){
						CMFCDemoApp::m_SendTspDialog = NULL;
						::PostMessage(pWnd->m_hWnd, WM_GPS_CLOSE, 0, 0);
						::PostMessage(pWnd->GetSafeHwnd(), WM_TSP_GPS_BSHOW, 0, 0);
                    }
                }
            }
		}
        if(BVCU_Result_FAILED(errorCode)){
            char tmsg[1024];
            BVCU_DialogInfo dlgInfo;
            BVCU_Result bvResult = BVCU_GetDialogInfo(hDialog, &dlgInfo);
            if (BVCU_Result_SUCCEEDED(bvResult))
            {
                sprintf_s(tmsg,"打开通道失败!PUID: %s 通道号: %d  错误码: %d",
                    dlgInfo.stParam.pTarget->szID, dlgInfo.stParam.pTarget->iIndexMajor, errorCode);
                MessageBox(NULL,tmsg,"通知",MB_OK);
            }
			if (m_talkDialg.pDialog == hDialog)
				ZeroMemory(&CMFCDemoApp::m_talkDialg, sizeof(CMFCDemoApp::m_talkDialg));
        }
		break;
	case BVCU_EVENT_DIALOG_CLOSE:
		break;
	}
}
void  CMFCDemoApp::OnStorageEvent(BVCU_HDialog hDialog, int eventCode, int errorCode, char* fileName, int strLen, SAV_TYPE_INT64 iTimestamp)
{

}
void  CMFCDemoApp::OnGetPuList(BVCU_HSession hSession, char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo, int finished)
{
    if (!finished)
        mainDlg->DisableCloseBtn();
	CTreeCtrlX* pPuListView = &mainDlg->m_puList;
	HTREEITEM onePU = mainDlg->FindPuItem(puId);
	if( (status & PU_STATUS_ONLINE) || (status & PU_STATUS_GETLIST))
	{
		if (!onePU)
		{
            ++CMLibBVCU::m_iLoadPUCount;
			int d_index = m_data.InsertPu(puId,puName,status,channel,channelNo);
			if (d_index)
			{
				onePU = pPuListView->GetRootItem();
				if (puName[0])
					onePU = pPuListView->InsertItem(puName,onePU);
				else
					onePU = pPuListView->InsertItem(puId,onePU);
				pPuListView->SetItemData(onePU,d_index);
				if (channelNo > 0)
				{
                    int i;
					char channelName[32];
					if (*channel[0].szName == '\0')
						sprintf_s(channelName,32,"通道%d",channel[0].iChannelIndex);
					else
						strcpy_s(channelName,32,channel[0].szName);
					HTREEITEM preChannel = pPuListView->InsertItem(channelName,onePU);
					pPuListView->SetItemData(preChannel,0);
					for (i = 1; i < channelNo; ++ i)
					{
						if (*channel[i].szName == '\0')
                        {
                            if (BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                                sprintf_s(channelName,32,"音视通道%d",channel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_GPS <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                                sprintf_s(channelName,32,"GPS通道%d",channel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_TSP <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                                sprintf_s(channelName,32,"TSP通道%d",channel[i].iChannelIndex);
                        }
						else
							strcpy_s(channelName,32,channel[i].szName);
						preChannel = pPuListView->InsertItem(channelName,onePU,preChannel);
						pPuListView->SetItemData(preChannel,i);
					}
				}
			}
		}
        else
        {
            m_data.SetPuStatus(puId, status);
        }
		if (onePU && (status & PU_STATUS_ONLINE))
			pPuListView->SetItemColor(onePU, 0x00ff0000);
        mainDlg->SortDeviceTree();
	}
	if ( status == PU_STATUS_OFFLINE)
	{ // offline
		if (onePU)
			pPuListView->SetItemColor(onePU, 0x00000000);
		m_data.SetPuStatus(puId,status);
	}
    mainDlg->DeviceManagerDeviceNode(puId, puName, status, channel, channelNo, finished);
    if (finished)
        mainDlg->EnableCloseBtn();
}
void  CMFCDemoApp::OnRecvGPSData(BVCU_HDialog hDialog, BVCU_PUCFG_GPSData* pGPSData)
{
	static BVCU_PUCFG_GPSData g_gpsData;
	if (mainDlg)
	{
		CPannelGPS* pPannelGPS = mainDlg->GetPannelGPS();
		if (hDialog == pPannelGPS->GetDialog())
		{
			g_gpsData = *pGPSData;
			::PostMessage(pPannelGPS->m_hWnd,WM_GPS_RECVDATA,(WPARAM)hDialog,(LPARAM)&g_gpsData);
		}
	}
}
void  CMFCDemoApp::OnRecvTSPData(BVCU_HDialog hDialog, const char* pTSPData, int len)
{
    static char TSPData[1024];
    if (mainDlg)
    {
        CPannelGPS* pPannelGPS = mainDlg->GetPannelGPS();
        if (hDialog == pPannelGPS->GetDialog())
        {
            char * pData = TSPData + sizeof(int);
            *((int*)TSPData) = len;
            strncpy_s(pData, 1020, pTSPData, _TRUNCATE);
            ::PostMessage(pPannelGPS->m_hWnd,WM_TSP_RECVDATA,(WPARAM)hDialog,(LPARAM)TSPData);
        }
    }
}
void  CMFCDemoApp::OnAfterRender(BVCU_HDialog hDialog, DWORD iPTS)
{
	if (mainDlg)
	{
		CPlayWnd* nPlayWnd = mainDlg->FindPlayWnd(hDialog);
		if (nPlayWnd)
			nPlayWnd->AfterRender(iPTS);
	}
}