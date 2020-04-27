// CPannelPTZ.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PannelPTZ.h"
#include "afxdialogex.h"
#include "libBVCU/MLibBVCU.h"


// CPannelPTZ dialog

IMPLEMENT_DYNAMIC(CPannelPTZ, CDialogEx)

	CPannelPTZ::CPannelPTZ(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPannelPTZ::IDD, pParent)
{

	m_ptzData = 5;
}

CPannelPTZ::~CPannelPTZ()
{
}

void CPannelPTZ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PTZ_DATA, m_ptzData);
	DDV_MinMaxInt(pDX, m_ptzData, 0, 15);
}


BEGIN_MESSAGE_MAP(CPannelPTZ, CDialogEx)

	ON_BN_CLICKED(IDC_PTZSET, &CPannelPTZ::OnBnClickedPtzset)
END_MESSAGE_MAP()


static void OnPtzCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
	if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_PTZATTR)
	{
		BVCU_Event_SessionCmd* pCmdEvent = (BVCU_Event_SessionCmd*)pParam;
		if (BVCU_Result_SUCCEEDED(pCmdEvent->iResult))
		{
			mainDlg->GetPannelCruise()->SetPtzAttr(
				(BVCU_PUCFG_PTZAttr*)(pCmdEvent->stContent.pData));
		}
	}
}

#define PTZ_CONTROL(icmdcode, p1, p2, p3, pstop) { \
	if (!(m_splitInfo.pDialog)) \
	break ; \
	BVCU_Command cmd; \
	BVCU_PUCFG_PTZControl ctrl; \
	ZeroMemory(&cmd,sizeof(cmd)); \
	ZeroMemory(&ctrl, sizeof(ctrl)); \
	cmd.iSize = sizeof(cmd); \
	cmd.OnEvent = OnPtzCmdEvent; \
	cmd.iMethod = BVCU_METHOD_CONTROL; \
	cmd.iSubMethod = BVCU_SUBMETHOD_PU_PTZCONTROL; \
	cmd.iTargetIndex = m_splitInfo.stChannel.iPTZIndex; \
	strncpy_s(cmd.szTargetID,m_splitInfo.sPUID,_TRUNCATE); \
	cmd.stMsgContent.iDataCount = 1; \
	cmd.stMsgContent.pData = &ctrl; \
	ctrl.bStop = pstop; \
	ctrl.iPTZCommand = icmdcode; \
	ctrl.iParam1 = p1; \
	ctrl.iParam2 = p2; \
	ctrl.iParam3 = p3; \
	CMLibBVCU::SendCmd(&cmd); \
}

// CPannelPTZ message handlers
void  CPannelPTZ::SetPUChannelInfo(const SplitInfoNode* pSplitInfo)
{
	m_splitInfo = *pSplitInfo;
	
	if (m_splitInfo.pDialog)
	{
		if (m_splitInfo.stChannel.iPTZIndex >=0)
		{
			EnablePtz(TRUE);
			BVCU_Command cmd;
			ZeroMemory(&cmd,sizeof(cmd));
			cmd.iSize = sizeof(cmd);
			cmd.OnEvent = OnPtzCmdEvent;
			cmd.iMethod = BVCU_METHOD_QUERY;
			cmd.iSubMethod = BVCU_SUBMETHOD_PU_PTZATTR;
			cmd.iTargetIndex = m_splitInfo.stChannel.iPTZIndex;
			strncpy_s(cmd.szTargetID,m_splitInfo.sPUID,_TRUNCATE);
			CMLibBVCU::SendCmd(&cmd);
			return ;
		}
	}
	EnablePtz(FALSE);
	mainDlg->GetPannelCruise()->ClearStatus();
}
void  CPannelPTZ::EnablePtz(BOOL bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_PTZ_DATA);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_EXTPTZFUNC);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZSET);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_LEFTUP);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_UP);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_RIGHTUP);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_LEFT);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_RIGHT);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_LEFTDOWN);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_DOWN);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_PTZ_RIGHTDOWN);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_ZOOM_WIDE);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_ZOOM_TELE);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_FOCUS_NEAR);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_FOCUS_FAR);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_IRIS_CLOSE);
	if (pWnd)
		pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_IRIS_OPEN);
	if (pWnd)
		pWnd->EnableWindow(bEnable);
}

void  CPannelPTZ::SendPtzCmd(int iCmd, int bStop, int iParam1, int iParam2, int iParam3)
{
	do 
	{
		PTZ_CONTROL(iCmd,iParam1,iParam2,iParam3,bStop);
	} while (0);
}
BOOL CPannelPTZ::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bResult = CDialogEx::PreTranslateMessage(pMsg);
	if ((pMsg->message != WM_LBUTTONDOWN) && (pMsg->message != WM_LBUTTONUP))
		return bResult;

	UpdateData();
	CWnd* pFocus = GetFocus();
	int   iFocus = 0;
	if (pFocus)
		iFocus = pFocus->GetDlgCtrlID();
	int   bStop= 0;
	if (pMsg->message == WM_LBUTTONUP)
		bStop = 1;

	switch (iFocus)
	{
	case IDC_PTZ_LEFTUP:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_LEFTTOP,m_ptzData,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_UP:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_UP,0,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_RIGHTUP:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_RIGHTTOP,m_ptzData,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_RIGHT:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_RIGHT,0,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_RIGHTDOWN:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_RIGHTDOWN,m_ptzData,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_DOWN:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_DOWN,0,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_LEFTDOWN:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_LEFTDOWN,m_ptzData,m_ptzData,0,bStop);
		break;
	case IDC_PTZ_LEFT:
		m_ptzData = m_ptzData%BVCU_PTZ_MAX_SPEED;
		PTZ_CONTROL(BVCU_PTZ_COMMAND_LEFT,0,m_ptzData,0,bStop);
		break;
	case IDC_ZOOM_WIDE:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_ZOOM_DEC,0,0,0,bStop);
		break;
	case IDC_ZOOM_TELE:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_ZOOM_INC,0,0,0,bStop);
		break;
	case IDC_FOCUS_NEAR:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_FOCUS_DEC,0,0,0,bStop);
		break;
	case IDC_FOCUS_FAR:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_FOCUS_INC,0,0,0,bStop);
		break;
	case IDC_IRIS_CLOSE:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_APERTURE_DEC,0,0,0,bStop);
		break;
	case IDC_IRIS_OPEN:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_APERTURE_INC,0,0,0,bStop);
		break;
	case IDC_EXTPTZFUNC:
		PTZ_CONTROL(BVCU_PTZ_COMMAND_AUX,m_ptzData,1,0,bStop);
		break;
	case IDC_PTZSET:
		break;
	}
	return bResult;
}

void CPannelPTZ::OnBnClickedPtzset()
{
	// TODO: Add your control notification handler code here
	CMFCDemoDlg* mainDlg = (CMFCDemoDlg*)GetParent();
	mainDlg->OnPannelCruise();
}
