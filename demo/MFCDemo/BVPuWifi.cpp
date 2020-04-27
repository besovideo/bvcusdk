// BVPuWifi.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVPuWifi.h"
#include "afxdialogex.h"
#include "DeviceManager.h"

// CBVPuWifi dialog

IMPLEMENT_DYNAMIC(CBVPuWifi, CDialogEx)

CBVPuWifi::CBVPuWifi(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PU_WIFI, pParent)
{

}

CBVPuWifi::CBVPuWifi(CDeviceManager* parent)
    : CDialogEx(IDD_DIALOG_PU_WIFI, NULL)
    , m_parent(parent)
{
}

CBVPuWifi::~CBVPuWifi()
{
}

void CBVPuWifi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBVPuWifi, CDialogEx)
END_MESSAGE_MAP()


// CBVPuWifi message handlers

BOOL CBVPuWifi::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVPuWifi::OnGetPuWifi(const char* szPuID, const BVCU_PUCFG_Wifi* pWifi)
{
    if (!szPuID || !szPuID[0] || !pWifi)
        return;
    strncpy_s(m_szPuID, szPuID, _TRUNCATE);
    m_wifi = *pWifi;
    // 是否启用
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_WIFI))->SetCheck(m_wifi.bEnable);
    // 信号强度
    if (m_wifi.iSignalLevel >= 0 && m_wifi.iSignalLevel <= 100)
        ((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_SIGNAL_LEVEL))->SetPos(m_wifi.iSignalLevel);
    // SSID
    char szAnsiSSID[32] = { 0 };
    utf8ToAnsi(szAnsiSSID, 32, m_wifi.stGeneral.szSSID);
    if (strlen(szAnsiSSID) <= 0) return;
    ((CEdit*)GetDlgItem(IDC_EDIT_SSID))->SetWindowTextA(szAnsiSSID);
    // 密码
    char szAnsiPassword[64] = { 0 };
    utf8ToAnsi(szAnsiPassword, 64, m_wifi.stGeneral.szWPAKey);
    if (strlen(szAnsiPassword) <= 0) return;
    ((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetWindowTextA(szAnsiPassword);
}

BVCU_Result CBVPuWifi::SetPuWifi()
{
    UpdateData(TRUE);
    if (!m_parent) return BVCU_RESULT_E_INVALIDDATA;
    CString szMessage("");
    szMessage.Format("是否设置设备: %s的Wifi信息", m_szPuID);
    if (IDYES != ::AfxMessageBox(szMessage, MB_YESNO))
        return BVCU_RESULT_E_NOTALLOWED;
    CString szSSID;
    CString szPassword;
    szSSID.Empty();
    szPassword.Empty();
    ((CEdit*)GetDlgItem(IDC_EDIT_SSID))->GetWindowTextA(szSSID);
    if (szSSID.IsEmpty() || szSSID.GetLength() > 32)
    {
        ::AfxMessageBox(_T("Wifi SSID不可为空且不可超过32个字符"));
        return BVCU_RESULT_E_INVALIDDATA;
    }
    ((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowTextA(szPassword);
    if (szPassword.IsEmpty() || szSSID.GetLength() > 64)
    {
        ::AfxMessageBox(_T("Wifi密码不可为空且不可超过64个字符"));
        return BVCU_RESULT_E_INVALIDDATA;
    }
    char szUtf8SSID[32] = { 0 };
    ansiToUtf8(szUtf8SSID, 32, szSSID);
    strncpy_s(m_wifi.stGeneral.szSSID , szUtf8SSID, _TRUNCATE);
    char szUtf8WPAKey[64];
    ansiToUtf8(szUtf8WPAKey, 64, szPassword);
    strncpy_s(m_wifi.stGeneral.szWPAKey, szUtf8WPAKey, _TRUNCATE);
    return m_parent->SetPuWifi(m_szPuID, &m_wifi);
}