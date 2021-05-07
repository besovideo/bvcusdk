// PannelRecordPlay.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PannelRecordPlay.h"
#include "afxdialogex.h"
#include <io.h>


// CPannelRecordPlay 对话框

IMPLEMENT_DYNAMIC(CPannelRecordPlay, CDialogEx)

CPannelRecordPlay::CPannelRecordPlay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPannelRecordPlay::IDD, pParent)
{

	EnableAutomation();
}

CPannelRecordPlay::~CPannelRecordPlay()
{
}

void CPannelRecordPlay::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void CPannelRecordPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPannelRecordPlay, CDialogEx)
	ON_BN_CLICKED(IDC_RP_OPEN, &CPannelRecordPlay::OnBnClickedRpOpen)
	ON_BN_CLICKED(IDC_RP_PLAY, &CPannelRecordPlay::OnBnClickedRpPlay)
	ON_BN_CLICKED(IDC_RP_CLOSE, &CPannelRecordPlay::OnBnClickedRpClose)
	ON_BN_CLICKED(IDC_RP_SLOW, &CPannelRecordPlay::OnBnClickedRpSlow)
	ON_BN_CLICKED(IDC_RP_FAST, &CPannelRecordPlay::OnBnClickedRpFast)
	ON_BN_CLICKED(IDC_RP_JUMPTO, &CPannelRecordPlay::OnBnClickedRpJumpto)
	ON_BN_CLICKED(IDC_RP_VOLDOWN, &CPannelRecordPlay::OnBnClickedRpVoldown)
	ON_BN_CLICKED(IDC_RP_VOLUP, &CPannelRecordPlay::OnBnClickedRpVolup)
	ON_BN_CLICKED(IDC_RP_STEPFORWARD, &CPannelRecordPlay::OnBnClickedRpStepforward)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPannelRecordPlay, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IPannelRecordPlay 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {03F573FA-DA80-4DC1-B4D1-90C3DA86A133}
static const IID IID_IPannelRecordPlay =
{ 0x3F573FA, 0xDA80, 0x4DC1, { 0xB4, 0xD1, 0x90, 0xC3, 0xDA, 0x86, 0xA1, 0x33 } };

BEGIN_INTERFACE_MAP(CPannelRecordPlay, CDialogEx)
	INTERFACE_PART(CPannelRecordPlay, IID_IPannelRecordPlay, Dispatch)
END_INTERFACE_MAP()


// CPannelRecordPlay 消息处理程序


void CPannelRecordPlay::OnBnClickedRpOpen()
{
	// TODO: 在此添加控件通知处理程序代码
#ifdef __RECORDPLAY_H__
	int pHandler = 0;
	BVCU_RecordPlay_FileParam rpFileParam;
	BVCU_RecordPlay_FileInfo rpFileInfo;
	BVCU_RecordPlay_SizeCtrl rpSizeCtrl;
	memset(&rpFileParam, NULL, sizeof(BVCU_RecordPlay_FileParam));
	memset(&rpFileInfo, NULL, sizeof(BVCU_RecordPlay_FileInfo));
	memset(&rpSizeCtrl, NULL, sizeof(BVCU_RecordPlay_SizeCtrl));

	CPlayContainer* pPlayContainer = ((CMFCDemoDlg*)GetParent())->GetPlayContainer();
	CPlayWnd* nowWnd = (CPlayWnd*)pPlayContainer->GetNextFreePage();
	if (NULL == nowWnd)
	{
		return;
	}
	CRect rect;
	::GetClientRect(nowWnd->GetSafeHwnd(), &rect);
	rpSizeCtrl.rcDisplay.iLeft = rect.left;
	rpSizeCtrl.rcDisplay.iRight = rect.right;
	rpSizeCtrl.rcDisplay.iTop = rect.top;
	rpSizeCtrl.rcDisplay.iBottom = rect.bottom;
	rpSizeCtrl.hWnd = (BVCU_HWND)nowWnd->GetSafeHwnd();

	char szTmp[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, szTmp, _countof(szTmp));
	char* p = _tcsrchr(szTmp, '\\');

	char szPath[MAX_PATH] = {0};
	_sntprintf_s(szPath, (MAX_PATH > (p - szTmp) ? (p - szTmp + 1): (MAX_PATH - 1)) , _T("%s"), szTmp);
	StrCat(szPath, "PU_EEAA0001_01_20130515_155928.mkv");
	if (-1 != _taccess(szPath, 00))
	{
		rpFileParam.szMediaFileName = szPath;
	}
	else
	{
		CString s;
		s.Format("%s 文件不存在！", szPath);
		MessageBox(s);
		return;
	}

	char szGps[MAX_PATH] = {0};
	_sntprintf_s(szGps, (MAX_PATH > (p - szTmp) ? (p - szTmp + 1): (MAX_PATH - 1)) , _T("%s"), szTmp);
	StrCat(szGps, "PU_EEAA0001_01_20130515_155928.xml");
	if (-1 != _taccess(szGps, 00))
	{
		rpFileParam.szGPSFileName = szGps;
		rpFileInfo.bGPS = TRUE;
	}

	BVCU_Result result = BVCU_RecordPlay_Open((BVCU_RecordPlay_Handler*)&pHandler, &rpFileParam, &rpFileInfo);
	if (BVCU_Result_FAILED(result))
	{
		return;
	}
	SplitInfoNode splitInfo;
	splitInfo.pDialog = (BVCU_HDialog)pHandler;
	nowWnd->SetSplitInfo(&splitInfo);

	result = BVCU_RecordPlay_Control((BVCU_RecordPlay_Handler)pHandler, BVCU_RECORDPLAY_CTRL_RESIZE, &rpSizeCtrl);
	if (BVCU_Result_FAILED(result))
	{
		return;
	}

	result = BVCU_RecordPlay_Control((BVCU_RecordPlay_Handler)pHandler, BVCU_RECORDPLAY_CTRL_CALLBACK, gps_OnData);

	int iSyncGroupID = 1;
	result = BVCU_RecordPlay_Control((BVCU_RecordPlay_Handler)pHandler, BVCU_RECORDPLAY_CTRL_SYNCGROUPID, &iSyncGroupID);

	this->OnBnClickedRpPlay();
#endif
}


void CPannelRecordPlay::OnBnClickedRpPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	BVCU_RecordPlay_PlayInfo playInfo;
	BVCU_Result result = BVCU_RecordPlay_Query(handler, &playInfo);
	switch (playInfo.iPlayState)
	{
	case BVCU_RECORDPLAY_STATE_PLAYING:
		result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_PAUSE, NULL);
		if (BVCU_Result_FAILED(result))
		{
			return;
		}
		GetDlgItem(IDC_RP_PLAY)->SetWindowText(_T("Play"));
		break;

	case BVCU_RECORDPLAY_STATE_OPENED:
	case BVCU_RECORDPLAY_STATE_PAUSE:
		result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_PLAY, NULL);
		if (BVCU_Result_FAILED(result))
		{
			return;
		}
		GetDlgItem(IDC_RP_PLAY)->SetWindowText(_T("Pause"));
		break;
	}
}


void CPannelRecordPlay::OnBnClickedRpClose()
{
	// TODO: 在此添加控件通知处理程序代码
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	BVCU_Result result = BVCU_RecordPlay_Close(handler);
	SetHandler(NULL);
}


void CPannelRecordPlay::OnBnClickedRpSlow()
{
	// TODO: 在此添加控件通知处理程序代码
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	BVCU_RecordPlay_PlayInfo playInfo;
	BVCU_Result result = BVCU_RecordPlay_Query(handler, &playInfo);
	int iSpeed = playInfo.iSpeed / 2;
	result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_SPEED, (void*)&iSpeed);
}


void CPannelRecordPlay::OnBnClickedRpFast()
{
	// TODO: 在此添加控件通知处理程序代码
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	BVCU_RecordPlay_PlayInfo playInfo;
	BVCU_Result result = BVCU_RecordPlay_Query(handler, &playInfo);
	int iSpeed = playInfo.iSpeed * 2;
	result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_SPEED, (void*)&iSpeed);
}

BVCU_RecordPlay_Handler CPannelRecordPlay::GetHandler()
{
	CPlayContainer* pPlayContainer = ((CMFCDemoDlg*)GetParent())->GetPlayContainer();
	CPlayWnd* pPlayWnd = (CPlayWnd*)pPlayContainer->GetActivePage();
	SplitInfoNode infoNode;
	pPlayWnd->GetSplitInfo(&infoNode);
	return (BVCU_RecordPlay_Handler)infoNode.pDialog;
}

void CPannelRecordPlay::SetHandler(BVCU_RecordPlay_Handler handler)
{
	CPlayContainer* pPlayContainer = ((CMFCDemoDlg*)GetParent())->GetPlayContainer();
	CPlayWnd* pPlayWnd = (CPlayWnd*)pPlayContainer->GetActivePage();
	SplitInfoNode infoNode;
	pPlayWnd->GetSplitInfo(&infoNode);
	infoNode.pDialog = (BVCU_HDialog)handler;
	pPlayWnd->SetSplitInfo(&infoNode);
}

void CPannelRecordPlay::OnBnClickedRpJumpto()
{
	// TODO: 在此添加控件通知处理程序代码
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	CString csTmp;
	GetDlgItem(IDC_RP_TIME)->GetWindowText(csTmp);
	int iTime = atoi(csTmp);
	BVCU_Result result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_JUMP, (void*)&iTime);
}


void CPannelRecordPlay::OnBnClickedRpVoldown()
{
	// TODO: Add your control notification handler code here
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	CString csTmp;
	GetDlgItem(IDC_RP_VOL)->GetWindowText(csTmp);
	int iVol = atoi(csTmp);
	iVol -= 5;
	if (iVol < 0)
	{
		iVol = 0;
	}
	BVCU_Result result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_VOLUME, (void*)&iVol);
	if (BVCU_Result_SUCCEEDED(result))
	{
		csTmp.Format("%d", iVol);
		GetDlgItem(IDC_RP_VOL)->SetWindowText(csTmp);
	}
}


void CPannelRecordPlay::OnBnClickedRpVolup()
{
	// TODO: Add your control notification handler code here
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	CString csTmp;
	GetDlgItem(IDC_RP_VOL)->GetWindowText(csTmp);
	int iVol = atoi(csTmp);
	iVol += 5;
	if (iVol > 100)
	{
		iVol = 100;
	}
	BVCU_Result result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_VOLUME, (void*)&iVol);
	if (BVCU_Result_SUCCEEDED(result))
	{
		csTmp.Format("%d", iVol);
		GetDlgItem(IDC_RP_VOL)->SetWindowText(csTmp);
	}
}


void CPannelRecordPlay::OnBnClickedRpStepforward()
{
	// TODO: Add your control notification handler code here
	BVCU_RecordPlay_Handler handler = GetHandler();
	if (!handler)
	{
		return;
	}
	BVCU_Result result = BVCU_RecordPlay_Control(handler, BVCU_RECORDPLAY_CTRL_STEP, NULL);
}

void gps_OnData(BVCU_RecordPlay_Handler hHandler, BVCU_RecordPlay_CallbackParam* pParam)
{
	if (pParam && pParam->pGPSData)
	{
		TRACE("%d  %d\n", pParam->pGPSData->stData.iLongitude, pParam->pGPSData->stData.iLatitude);
	}
}