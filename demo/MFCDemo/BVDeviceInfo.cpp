// BVDeviceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVDeviceInfo.h"
#include "afxdialogex.h"
#include "DeviceManager.h"

// CBVDeviceInfo dialog

IMPLEMENT_DYNAMIC(CBVDeviceInfo, CDialogEx)

CBVDeviceInfo::CBVDeviceInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DEVICEINFO, pParent)
{

}

CBVDeviceInfo::CBVDeviceInfo(CDeviceManager* parent)
    :CDialogEx(IDD_DIALOG_DEVICEINFO, NULL)
    , m_Parent(parent)
{

}

CBVDeviceInfo::~CBVDeviceInfo()
{
}

void CBVDeviceInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBVDeviceInfo, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_REBOOT, &CBVDeviceInfo::OnBnClickedButtonReboot)
END_MESSAGE_MAP()


// CBVDeviceInfo message handlers


BOOL CBVDeviceInfo::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVDeviceInfo::OnGetDeviceInfo(const BVCU_PUCFG_DeviceInfo* pDeviceInfo)
{
    if (!pDeviceInfo) return;
    memset(&m_deviceInfo, 0, sizeof(m_deviceInfo));
    m_deviceInfo = *pDeviceInfo;
    ((CEdit*)GetDlgItem(IDC_EDIT_DEVICEID))->SetWindowTextA(pDeviceInfo->szID);
}

void CBVDeviceInfo::OnBnClickedButtonReboot()
{
    // TODO: Add your control notification handler code here
    if (!m_Parent) return;
    CString szMessage("");
    szMessage.Format("是否重启设备: %s", m_deviceInfo.szID);
    if (IDYES != ::AfxMessageBox(szMessage, MB_YESNO))
        return;
    m_Parent->RebootDevice(m_deviceInfo.szID);
}
