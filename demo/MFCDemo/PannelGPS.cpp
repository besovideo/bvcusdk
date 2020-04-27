// PannelGPS.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PannelGPS.h"
#include "afxdialogex.h"

BOOL bSend = TRUE; //global value
// CPannelGPS dialog

IMPLEMENT_DYNAMIC(CPannelGPS, CDialogEx)

CPannelGPS::CPannelGPS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPannelGPS::IDD, pParent)
	, m_ShowGPSStr(_T(""))
	, m_szTspData(_T(""))
    , m_bHexString(FALSE)
{
	InitializeCriticalSection(&m_cs);
	memset(&m_splitInfo, 0, sizeof(m_splitInfo));
	/*m_ShowGPSStr.Format("\r\n设备: PU_00000000\r\n时间: 2012.12.25  16:32:05\r\n"
		"\r\n经度: 117.11111\r\n纬度: 31.22222\r\n"
		"\r\n高度: 11111\r\n方向: 111.111\r\n速度: 120000\r\n"
		"\r\n定位星数: 20\r\n天线状态: 好\r\n定位状态: 定位");*/
}

CPannelGPS::~CPannelGPS()
{
	DeleteCriticalSection(&m_cs);
}

void CPannelGPS::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SHOWGPS, m_ShowGPSStr);
    DDX_Text(pDX, IDC_TSPDATA, m_szTspData);
    DDX_Check(pDX, IDC_CHECK1, m_bHexString);
}


BEGIN_MESSAGE_MAP(CPannelGPS, CDialogEx)
    ON_MESSAGE(WM_GPS_RECVDATA,OnRecvGPSData)
    ON_MESSAGE(WM_TSP_RECVDATA,OnRecvTSPData)
	ON_MESSAGE(WM_GPS_CLOSE, OnCloseDialog)
	ON_BN_CLICKED(IDC_SENDTSP, &CPannelGPS::OnBnClickedSendtsp)
	ON_MESSAGE(WM_TSP_GPS_BSHOW, OnBShowGPSTSP)
ON_BN_CLICKED(IDC_CHECK1, &CPannelGPS::OnClickedHex)
END_MESSAGE_MAP()

LRESULT CPannelGPS::OnCloseDialog(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	CPlayContainer *pContainer = (CPlayContainer *)GetParent();
	EnterCriticalSection(&m_cs);
	if (m_splitInfo.pDialog)
	{
		mainDlg->CloseDialog(&m_splitInfo);
	}
    OnBShowGPSTSP(FALSE, 0);
	memset(&m_splitInfo,0x00,sizeof(m_splitInfo));
	LeaveCriticalSection(&m_cs);
	UpdateData(FALSE);
	return 0;
}

BOOL CPannelGPS::BPlay()
{
	if (m_splitInfo.pDialog)
		return TRUE;
	return FALSE;
}

BOOL CPannelGPS::GetSplitInfo(SplitInfoNode* info)
{
	if (!info)
	{
		return FALSE;
	}

	EnterCriticalSection(&m_cs);
	memcpy(info, &m_splitInfo, sizeof(SplitInfoNode));
	LeaveCriticalSection(&m_cs);

	return TRUE;
}

BOOL CPannelGPS::SetSplitInfo(SplitInfoNode* info)
{
	if (!info)
	{
		return FALSE;
	}

	UpdateData(TRUE);
	EnterCriticalSection(&m_cs);
	memcpy(&m_splitInfo, info, sizeof(SplitInfoNode));
	LeaveCriticalSection(&m_cs);
	m_ShowGPSStr.Format("\r\n设备: %s\r\n", m_splitInfo.sPUID);
	UpdateData(FALSE);
	return TRUE;
}

static char* g_gpsFormat = "\r\n设备: %s\r\n时间: %4d.%2d.%2d  %2d:%2d:%2d\r\n"
	"\r\n经度: %.7f\r\n纬度: %.7f\r\n"
	"\r\n高度: %.7f米\r\n方向: %.7f\r\n速度: %d米/小时\r\n"
	"\r\n定位星数: %d\r\n天线状态: %s\r\n定位状态: %s";
// CPannelGPS message handlers
LRESULT CPannelGPS::OnRecvGPSData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	BVCU_HDialog pDialog = (BVCU_HDialog)wParam;
	BVCU_PUCFG_GPSData* pGPSData = (BVCU_PUCFG_GPSData*)lParam;
	if (pDialog == m_splitInfo.pDialog && pGPSData)
	{
		BVCU_WallTime* pTime = &(pGPSData->stTime);
		m_ShowGPSStr.Format(g_gpsFormat, m_splitInfo.sPUID,
			pTime->iYear,pTime->iMonth,pTime->iDay,pTime->iHour,pTime->iMinute,pTime->iSecond,
			((double)pGPSData->iLongitude)/10000000.0, ((double)pGPSData->iLatitude)/10000000.0,
			((double)pGPSData->iHeight)/100.0, ((double)pGPSData->iAngle)/1000.0, pGPSData->iSpeed,
			pGPSData->iStarCount, pGPSData->bAntennaState ? "好" : "坏",
			pGPSData->bOrientationState ? "定位" : "未定位"
			);
	}
	UpdateData(FALSE);
	return 0;
}

LRESULT CPannelGPS::OnRecvTSPData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
    BVCU_HDialog pDialog = (BVCU_HDialog)wParam;
    char* pTSPData = (char*)lParam + sizeof(int);
    int len = *((int*)lParam);
    if (pDialog == m_splitInfo.pDialog && pTSPData)
    {
        if (m_bHexString)
        {
            char buf[1024] = { 0 };
            DataToHexString(buf,sizeof(buf), pTSPData, len);
            m_ShowGPSStr.Format("%.*s", strlen(buf), buf);
        }
        else
            m_ShowGPSStr.Format("%.*s", len,pTSPData);
    }
    UpdateData(FALSE);
    return 0;
}

LRESULT CPannelGPS::OnBShowGPSTSP(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)(wParam);
    GetDlgItem(IDC_TSPDATA)->EnableWindow(bShow);
    GetDlgItem(IDC_CHECK1)->EnableWindow(bShow);
	GetDlgItem(IDC_SENDTSP)->EnableWindow(bShow);
	return 0;
}

void CPannelGPS::OnBnClickedSendtsp()
{
	// TODO: Add your control notification handler code here
	if (NULL == CMFCDemoApp::m_SendTspDialog)
	{
		::AfxMessageBox(_T("请打开设备TSP通道"));
        GetDlgItem(IDC_TSPDATA)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_SENDTSP)->EnableWindow(FALSE);
		return;
	}
	UpdateData(TRUE);
	if (0 == m_szTspData.GetLength())
	{
		::AfxMessageBox(_T("请输入透明串口数据"));
		return;
	}
	SAV_TYPE_UINT8 * pSendData =  (SAV_TYPE_UINT8*)m_szTspData.GetBuffer();
    SAV_TYPE_INT32 iSendLen = strlen((char*)pSendData);
    int result = BVCU_RESULT_S_OK;
    if (m_bHexString)
    {
        char buf[1024] = { 0 };
        iSendLen = HexStringToData(buf, sizeof(buf), (char*)pSendData);

        result = CMLibBVCU::SendTspData(CMFCDemoApp::m_SendTspDialog, (SAV_TYPE_UINT8*)buf, iSendLen);
    }
    else
        result = CMLibBVCU::SendTspData(CMFCDemoApp::m_SendTspDialog, pSendData, iSendLen);
	if (!BVCU_Result_SUCCEEDED(result))
	{
		::AfxMessageBox(_T("发送失败"));
		return;
	}
	UpdateData(FALSE);
}

void CPannelGPS::OnClickedHex()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if (m_splitInfo.pDialog && 
        BVCU_SUBDEV_INDEXMAJOR_MIN_TSP <= m_splitInfo.stChannel.iChannelIndex && 
        BVCU_SUBDEV_INDEXMAJOR_MAX_TSP >= m_splitInfo.stChannel.iChannelIndex)
    {
        char * pSendData = (char*)m_szTspData.GetBuffer();
        SAV_TYPE_INT32 iSendLen = strlen((char*)pSendData);
        char* pSendData2 = (char*)m_ShowGPSStr.GetBuffer();
        SAV_TYPE_INT32 iSendLen2 = strlen((char*)pSendData2);
        char buf[1024] = { 0 };
        int  iLen = 0;
        if (m_bHexString)
        {
            iLen = DataToHexString(buf, sizeof(buf), pSendData, iSendLen);
            m_szTspData.Format("%.*s",iLen, buf);
            iLen = DataToHexString(buf, sizeof(buf), pSendData2, iSendLen2);
            m_ShowGPSStr.Format("%.*s",iLen, buf);
        }
        else
        {
            iLen = HexStringToData(buf, sizeof(buf), pSendData);
            buf[iLen] = '\0';
            m_szTspData.Format("%.*s",iLen, buf);
            iLen = HexStringToData(buf, sizeof(buf), pSendData2);
            buf[iLen] = '\0';
            m_ShowGPSStr.Format("%.*s",iLen, buf);
        }
    }

    UpdateData(FALSE);
}
