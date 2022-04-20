// BVPuServers.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVPuServers.h"
#include "afxdialogex.h"
#include "DeviceManager.h"


// CBVPuServers dialog

IMPLEMENT_DYNAMIC(CBVPuServers, CDialogEx)

CBVPuServers::CBVPuServers(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_PU_SERVERS, pParent)
{

}

CBVPuServers::CBVPuServers(CDeviceManager* parent)
    : CDialogEx(IDD_DIALOG_PU_SERVERS, NULL)
    , m_parent(parent)
{
    memset(m_szPuID, 0, sizeof(m_szPuID));
}

CBVPuServers::~CBVPuServers()
{
}

void CBVPuServers::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBVPuServers, CDialogEx)
END_MESSAGE_MAP()


// CBVPuServers message handlers


BOOL CBVPuServers::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVPuServers::OnGetPuServers(const char* szPuID, const BVCU_PUCFG_Servers* pServers)
{
    if (!szPuID || !szPuID[0] || !pServers) return;
    strncpy_s(m_szPuID, szPuID, _TRUNCATE);
    memset(&m_puServers, 0, sizeof(m_puServers));
    m_puServers = *pServers;
    char szAnsiAddr[BVCU_MAX_HOST_NAME_LEN + 1] = { 0 };
    utf8ToAnsi(szAnsiAddr, BVCU_MAX_HOST_NAME_LEN + 1, m_puServers.stRegisterServer.szAddr);
    if (strlen(szAnsiAddr) <= 0) return;
    ((CEdit*)GetDlgItem(IDC_EDIT_SERVER_ADDR))->SetWindowTextA(szAnsiAddr);
    CString szServerPort;
    szServerPort.Format(_T("%d"), m_puServers.stRegisterServer.iPort);
    ((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT))->SetWindowTextA(szServerPort);
}

BVCU_Result CBVPuServers::SetPuServers()
{
    UpdateData(TRUE);
    if (!m_parent) return BVCU_RESULT_E_INVALIDDATA;
    CString szMessage("");
    szMessage.Format("是否设置设备: %s服务器信息", m_szPuID);
    if (IDYES != ::AfxMessageBox(szMessage, MB_YESNO))
        return BVCU_RESULT_E_NOTALLOWED;
    CString szAddr;
    CString szPort;
    szAddr.Empty();
    szPort.Empty();
    ((CEdit*)GetDlgItem(IDC_EDIT_SERVER_ADDR))->GetWindowTextA(szAddr);
    if (szAddr.IsEmpty())
    {
        ::AfxMessageBox(_T("设备IP不可为空"));
        return BVCU_RESULT_E_INVALIDDATA;
    }
    ((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT))->GetWindowTextA(szPort);
    if (szPort.IsEmpty())
    {
        ::AfxMessageBox(_T("设备端口不可为空"));
        return BVCU_RESULT_E_INVALIDDATA;
    }
    char szUtf8Addr[BVCU_MAX_HOST_NAME_LEN + 1] = { 0 };
    ansiToUtf8(szUtf8Addr, BVCU_MAX_HOST_NAME_LEN + 1, szAddr);
    strncpy_s(m_puServers.stRegisterServer.szAddr, szAddr, _TRUNCATE);
    m_puServers.stRegisterServer.iPort = _ttoi(szPort);
    return m_parent->SetPuServers(m_szPuID, &m_puServers);
}
