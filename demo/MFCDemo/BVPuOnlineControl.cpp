// BVPuOnlineControl.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVPuOnlineControl.h"
#include "afxdialogex.h"
#include "DeviceManager.h"

// CBVPuOnlineControl dialog

IMPLEMENT_DYNAMIC(CBVPuOnlineControl, CDialogEx)

CBVPuOnlineControl::CBVPuOnlineControl(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_ONLINE_CONTROL, pParent)
{

}

CBVPuOnlineControl::CBVPuOnlineControl(CDeviceManager* parent)
    : CDialogEx(IDD_DIALOG_ONLINE_CONTROL, NULL)
    , m_parent(parent)
{
    memset(m_szPuID, 0, BVCU_MAX_ID_LEN + 1);
}

CBVPuOnlineControl::~CBVPuOnlineControl()
{
}

void CBVPuOnlineControl::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBVPuOnlineControl, CDialogEx)
END_MESSAGE_MAP()

// CBVPuOnlineControl message handlers

BOOL CBVPuOnlineControl::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVPuOnlineControl::OnGetOnlineControl(const char* szPuID, const BVCU_PUCFG_OnlineControl* pOnlineControl)
{
    if (!szPuID || !szPuID[0] || !pOnlineControl)
        return;
    strncpy_s(m_szPuID, szPuID, _TRUNCATE);
    memset(&m_OnlineControl, 0, sizeof(m_OnlineControl));
    m_OnlineControl = *pOnlineControl;
    if (m_OnlineControl.stRCO[0][0].iThrough == BVCU_PU_ONLINE_THROUGH_WIFI)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_WIFI))->SetCheck(TRUE);
    }
    else if (m_OnlineControl.stRCO[0][0].iThrough == BVCU_PU_ONLINE_THROUGH_RADIO)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_RADIO))->SetCheck(TRUE);
    }
}

BVCU_Result CBVPuOnlineControl::SetOnlineControl()
{
    UpdateData(TRUE);
    if (!m_parent) return BVCU_RESULT_E_INVALIDDATA;
    CString szMessage("");
    szMessage.Format("是否设置设备: %s的上线控制信息", m_szPuID);
    if (IDYES != ::AfxMessageBox(szMessage, MB_YESNO))
        return BVCU_RESULT_E_NOTALLOWED;
    int iThrough = BVCU_PU_ONLINE_THROUGH_INVALID;
    if (((CButton*)GetDlgItem(IDC_RADIO_WIFI))->GetCheck())
    {
        iThrough = BVCU_PU_ONLINE_THROUGH_WIFI;
    }
    else if (((CButton*)GetDlgItem(IDC_RADIO_RADIO))->GetCheck())
    {
        iThrough = BVCU_PU_ONLINE_THROUGH_RADIO;
    }
    else
    {
        ::AfxMessageBox(_T("请选择一种上传方式"));
        return BVCU_RESULT_E_NOTFOUND;
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < BVCU_MAX_DAYTIMESLICE_COUNT; j++)
        {
            m_OnlineControl.stRCO[i][j].iThrough = iThrough;
        }
    }
    return m_parent->SetPuOnlineControl(m_szPuID, &m_OnlineControl);
}