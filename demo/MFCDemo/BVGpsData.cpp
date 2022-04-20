// BVGpsData.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "BVGpsData.h"
#include "afxdialogex.h"


// CBVGpsData dialog

IMPLEMENT_DYNAMIC(CBVGpsData, CDialogEx)

CBVGpsData::CBVGpsData(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_GPSDATA, pParent)
{

}

CBVGpsData::CBVGpsData(CDeviceManager* parent)
    : CDialogEx(IDD_DIALOG_GPSDATA, NULL)
    , m_parent(parent)
{
    memset(m_szPuID, 0, BVCU_MAX_ID_LEN + 1);
    m_iGpsChannelNo = -1;
    memset(&m_GpsData, 0, sizeof(m_GpsData));
}

CBVGpsData::~CBVGpsData()
{
}

void CBVGpsData::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBVGpsData, CDialogEx)
END_MESSAGE_MAP()

// CBVGpsData message handlers

BOOL CBVGpsData::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBVGpsData::OnGetGpsData(const char* szPuID, int iGpsChannel, const BVCU_PUCFG_GPSData* pGpsData)
{
    if (!szPuID || !szPuID[0] || !pGpsData)
        return;
    if (iGpsChannel < BVCU_SUBDEV_INDEXMAJOR_MIN_GPS ||
        iGpsChannel > BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
        return;
    m_GpsData = *pGpsData;
    // ��λ״̬
    CString szLocatedStatus = !m_GpsData.bOrientationState ? "δ��λ" : "��λ";
    ((CEdit*)GetDlgItem(IDC_EDIT_LOCATE_STATUS))->SetWindowText(szLocatedStatus);
    // ����״̬
    CString szOrientationState = !m_GpsData.bAntennaState ? "��" : "��";
    ((CEdit*)GetDlgItem(IDC_EDIT_ANTENNA_STATUS))->SetWindowText(szOrientationState);
    // ����
    CString szDate("");
    szDate.Format("%d-%d-%d", m_GpsData.stTime.iYear, m_GpsData.stTime.iMonth,
        m_GpsData.stTime.iDay);
    ((CEdit*)GetDlgItem(IDC_EDIT_DATE))->SetWindowText(szDate);
    // ʱ��
    CString szTime("");
    szTime.Format("%d:%d:%d", m_GpsData.stTime.iHour, m_GpsData.stTime.iMinute,
        m_GpsData.stTime.iSecond);
    ((CEdit*)GetDlgItem(IDC_EDIT_TIME))->SetWindowText(szTime);
    // ����
    CString szHeight("");
    szHeight.Format("%f", m_GpsData.iHeight / 100.0);
    ((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->SetWindowText(szHeight);
    // �ٶ�
    CString szSpeed("");
    szSpeed.Format("%f", m_GpsData.iSpeed / 1000.0);
    ((CEdit*)GetDlgItem(IDC_EDIT_SPEED))->SetWindowText(szSpeed);
    // �����
    CString szAngle("");
    szAngle.Format("%f", m_GpsData.iAngle / 1000.0);
    ((CEdit*)GetDlgItem(IDC_EDIT_ANGLE))->SetWindowText(szAngle);
    // ��λ����
    CString szStarCount("");
    szStarCount.Format("%d", m_GpsData.iStarCount);
    ((CEdit*)GetDlgItem(IDC_EDIT_STARCOUNT))->SetWindowText(szStarCount);
    // ����
    CString szLongitude("");
    szLongitude.Format("%f", m_GpsData.iLongitude / BVCU_LAT_LNG_UNIT);
    ((CEdit*)GetDlgItem(IDC_EDIT_LONGITUDE))->SetWindowText(szLongitude);
    // γ��
    CString szLatitude("");
    szLatitude.Format("%f", m_GpsData.iLatitude / BVCU_LAT_LNG_UNIT);
    ((CEdit*)GetDlgItem(IDC_EDIT_LATITUDE))->SetWindowText(szLatitude);
    // �����ź���Դ
    ((CButton*)GetDlgItem(IDC_CHECK_GPS))->SetCheck(m_GpsData.iSatelliteSignal &
        BVCU_PUCFG_SATELLITE_GPS);
    ((CButton*)GetDlgItem(IDC_CHECK_BDS))->SetCheck(m_GpsData.iSatelliteSignal &
        BVCU_PUCFG_SATELLITE_BDS);
    ((CButton*)GetDlgItem(IDC_CHECK_GLONASS))->SetCheck(m_GpsData.iSatelliteSignal &
        BVCU_PUCFG_SATELLITE_GLONASS);
    ((CButton*)GetDlgItem(IDC_CHECK_GALILEO))->SetCheck(m_GpsData.iSatelliteSignal &
        BVCU_PUCFG_SATELLITE_GALILEO);
    ((CButton*)GetDlgItem(IDC_CHECK_QZSS))->SetCheck(m_GpsData.iSatelliteSignal &
        BVCU_PUCFG_SATELLITE_QZSS);
}