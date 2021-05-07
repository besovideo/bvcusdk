// RadioNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVRadioNetwork.h"
#include "afxdialogex.h"
#include "DeviceManager.h"

// CBVRadioNetwork dialog

IMPLEMENT_DYNAMIC(CBVRadioNetwork, CDialogEx)

CBVRadioNetwork::CBVRadioNetwork(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RADIONETWORK, pParent)
{

}

CBVRadioNetwork::CBVRadioNetwork(CDeviceManager* parent)
    : CDialogEx(IDD_DIALOG_RADIONETWORK, NULL)
    , m_parent(parent)
{

}

CBVRadioNetwork::~CBVRadioNetwork()
{
}

void CBVRadioNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBVRadioNetwork, CDialogEx)
END_MESSAGE_MAP()


// CBVRadioNetwork message handlers


BOOL CBVRadioNetwork::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVRadioNetwork::OnGetRadioNetwork(const char* szPuID, 
    const BVCU_PUCFG_RadioNetwork* pRadioNetwork, int iRadioCount)
{
    if (!szPuID || !szPuID[0] || !pRadioNetwork || iRadioCount <= 0)
        return;
    strncpy_s(m_szPuID, szPuID, _TRUNCATE);
    memset(&m_radioNetwork, 0, sizeof(BVCU_PUCFG_RadioNetwork) * 16);
    if (iRadioCount <= 0) return;
    m_iRadioCount = iRadioCount;
    for (int i = 0; i < iRadioCount; ++i)
        m_radioNetwork[i] = *(pRadioNetwork + i);
    // 这里只显示第一个模块，配置第一个模块作为示范
    // 使能
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE))->SetCheck(m_radioNetwork[0].bEnable);
    // 信号强度
    char szSignalLevel[1024] = { 0 };
    char szSignalLevelType[4][MAX_PATH] = { 0 };
    for (int i = 0; i < 4; i++)
    {
        CString szTmp = GetRadioNetwordType(m_radioNetwork[0].iTypeAll[i]);
        if (szTmp.Compare("UNKNOW"))
        {
            sprintf_s(szSignalLevelType[i], "%s:%d    ", szTmp, m_radioNetwork[0].iSignalLevel[i]);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (strlen(szSignalLevelType[i]) > 0)
            strcat_s(szSignalLevel, szSignalLevelType[i]);
    }
    if (strlen(szSignalLevel) > 0)
        ((CEdit*)GetDlgItem(IDC_EDIT_SIGNAL_LEVEL))->SetWindowTextA(szSignalLevel);
}

CString CBVRadioNetwork::GetRadioNetwordType(int type)
{
    CString szRadioNetworkType("");
    switch (type)
    {
    case BVCU_RADIONETWORK_TYPE_AUTO:
        szRadioNetworkType = "AUTO";
        break;
    case BVCU_RADIONETWORK_TYPE_GPRS:
        szRadioNetworkType = "GPRS";
        break;
    case BVCU_RADIONETWORK_TYPE_CDMA:
        szRadioNetworkType = "CDMA";
        break;
    case BVCU_RADIONETWORK_TYPE_EDGE:
        szRadioNetworkType = "EDGE";
        break;
    case BVCU_RADIONETWORK_TYPE_3GWCDMA:
        szRadioNetworkType = "3GWCDMA";
        break;
    case BVCU_RADIONETWORK_TYPE_3GTDSCDMA:
        szRadioNetworkType = "3GTDSCDMA";
        break;
    case BVCU_RADIONETWORK_TYPE_3GCDMA2K:
        szRadioNetworkType = "3GCDMA2K";
        break;
    case BVCU_RADIONETWORK_TYPE_4GTDLTE:
        szRadioNetworkType = "4GTDLTE";
        break;
    case BVCU_RADIONETWORK_TYPE_4GFDLTE:
        szRadioNetworkType = "4GFDLTE";
        break;
    case BVCU_RADIONETWORK_TYPE_4GLTE:
        szRadioNetworkType = "4GLTE";
        break;
    case BVCU_RADIONETWORK_TYPE_GSM:
        szRadioNetworkType = "GSM";
        break;
    case BVCU_RADIONETWORK_TYPE_EDGEPLUS:
        szRadioNetworkType = "EDGEPLUS";
        break;
    case BVCU_RADIONETWORK_TYPE_3GHSPA:
        szRadioNetworkType = "3GHSPA";
        break;
    case BVCU_RADIONETWORK_TYPE_3GHSPAPLUS:
        szRadioNetworkType = "3GHSPAPLUS";
        break;
    case BVCU_RADIONETWORK_TYPE_3GCDMA1X:
        szRadioNetworkType = "3GCDMA1X";
        break;
    default:
        szRadioNetworkType = "UNKNOW";
        break;
    }
    return szRadioNetworkType;
}