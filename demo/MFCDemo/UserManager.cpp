// UserManager.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "UserManager.h"
#include "afxdialogex.h"
#include "MFCDemoDlg.h"

// CUserManager dialog

IMPLEMENT_DYNAMIC(CUserManager, CDialogEx)

CUserManager::CUserManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USER_MANAGER, pParent)
{

}

CUserManager::CUserManager(CMFCDemoDlg* parent)
    : CDialogEx(IDD_DIALOG_USER_MANAGER, NULL)
    , m_parent(parent)
{
    memset(m_szUserID, 0, AAA_ID_LEN);
}

CUserManager::~CUserManager()
{
}

void CUserManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserManager, CDialogEx)
    ON_BN_CLICKED(IDOK, &CUserManager::OnBnClickedOk)
END_MESSAGE_MAP()

// CUserManager message handlers

BOOL CUserManager::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    memset(m_szUserID, 0, AAA_ID_LEN);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserManager::SetCurrentUserID(const char* szUserID)
{
    if (!szUserID || strlen(szUserID) <= 0)
    {
        memset(m_szUserID, 0, AAA_ID_LEN);
        return;
    }
    strncpy_s(m_szUserID, szUserID, _TRUNCATE);
    char szAnsiUserID[AAA_ID_LEN] = { 0 };
    utf8ToAnsi(szAnsiUserID, AAA_ID_LEN, m_szUserID);
    ((CEdit*)GetDlgItem(IDC_EDIT_CURRENT_USERID))->SetWindowText(szAnsiUserID);
}

char* CUserManager::GetCurrentUserID()
{
    return m_szUserID;
}

void CUserManager::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    CString szOldPassword;
    CString szNewPassword;
    CString szRepeatPassword;
    ((CEdit*)GetDlgItem(IDC_EDIT_OLD_PASSWORD))->GetWindowText(szOldPassword);
    ((CEdit*)GetDlgItem(IDC_EDIT_NEW_PASSWORD))->GetWindowText(szNewPassword);
    ((CEdit*)GetDlgItem(IDC_EDIT_REPEAT_PASSWORD))->GetWindowText(szRepeatPassword);
    if (strlen(m_szUserID) <= 0)
    {
        ::AfxMessageBox(_T("当前用户名为空"));
        return;
    }
    else if (szOldPassword.GetLength() <= 0)
    {
        ::AfxMessageBox(_T("请输入旧密码"));
        return;
    }
    else if (szNewPassword.GetLength() <= 0)
    {
        ::AfxMessageBox(_T("请输入新密码"));
        return;
    }
    else if (szRepeatPassword.GetLength() <= 0)
    {
        ::AfxMessageBox(_T("请再次输入新密码"));
        return;
    }
    if (szRepeatPassword.Compare(szNewPassword))
    {
        ::AfxMessageBox(_T("两次输入的新密码不一致"));
        return;
    }
    memset(&m_modPassword, 0, sizeof(m_modPassword));
    strncpy_s(m_modPassword.sId, m_szUserID, _TRUNCATE);
    ansiToUtf8(m_modPassword.sPassword, AAA_ID_LEN, szOldPassword);
    ansiToUtf8(m_modPassword.sNewPassword, AAA_ID_LEN, szNewPassword);
    if (m_parent)
        m_parent->ModCurrentUserPass(&m_modPassword);
    CDialogEx::OnOK();
}
