// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MFCDemoDlg.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_name = CMFCDemoApp::m_config.GetUserName();
	m_password = CMFCDemoApp::m_config.GetPassword();
	m_port = CMFCDemoApp::m_config.GetPort();
	m_serverIP = CMFCDemoApp::m_config.GetServerIP();
	m_save = TRUE;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 32);
	DDX_Text(pDX, IDC_PASSWD, m_password);
	DDV_MaxChars(pDX, m_password, 32);
	DDX_Check(pDX, IDC_SAVE, m_save);
	DDX_Text(pDX, IDC_SERVERIP, m_serverIP);
	DDV_MaxChars(pDX, m_serverIP, 128);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 0, 65535);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN, &CLoginDlg::OnBnClickedLogin)
END_MESSAGE_MAP()


// CLoginDlg message handlers


void CLoginDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	CMFCDemoApp::m_loginStatus = BVCU_RESULT_E_FAILED;
	ResetEvent(CMFCDemoApp::m_loginEvent);
	UpdateData();
	if (m_serverIP == "")
	{
		MessageBox("服务器IP不能为空！");
		return;
	}
	if (m_name == "")
	{
		MessageBox("用户名不能为空！");
		return;
	}
	if (m_password == "")
	{
		MessageBox("密码不能为空！");
		return;
	}
	int status = CMLibBVCU::Login((char*)m_serverIP.GetString(),m_port,(char*)m_name.GetString(),(char*)m_password.GetString(),CMFCDemoApp::m_timeOut*1000);
	if (BVCU_Result_SUCCEEDED(status))
	{
		SetDlgItemText(IDC_STATUS,_T("登录中……"));
		m_status.ShowWindow(SW_SHOW);
		WaitForSingleObject(CMFCDemoApp::m_loginEvent,2000*CMFCDemoApp::m_timeOut);
		if (CMFCDemoApp::m_loginStatus == BVCU_RESULT_S_OK)
		{
			if (m_save)
				CMFCDemoApp::m_config.SaveServer((char*)m_serverIP.GetString(),m_port,(char*)m_name.GetString(),(char*)m_password.GetString());
            CMFCDemoDlg::SetCurrentUserID(m_name.GetString());
			OnOK();
			return;
		}
		else if (CMFCDemoApp::m_loginStatus == BVCU_RESULT_E_TIMEOUT)
			MessageBox("登录超时！");
		else if (CMFCDemoApp::m_loginStatus == BVCU_RESULT_E_AUTHORIZE_FAILED)
			MessageBox("用户名或密码错误！");
		else if (CMFCDemoApp::m_loginStatus == BVCU_RESULT_E_FAILED)
			MessageBox("请重试！");
		m_status.ShowWindow(SW_HIDE);
	}
	else
		MessageBox("请重试！");
}
