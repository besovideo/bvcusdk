#ifndef __GPS_DATA_H__
#define __GPS_DATA_H__

#include "BVCU.h"

// CBVGpsData dialog

class CDeviceManager;

class CBVGpsData : public CDialogEx
{
    DECLARE_DYNAMIC(CBVGpsData)

public:
    CBVGpsData(CWnd* pParent = NULL);   // standard constructor
    CBVGpsData(CDeviceManager* parent);
    virtual ~CBVGpsData();

    // Dialog Data
    enum { IDD = IDD_DIALOG_GPSDATA };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    CDeviceManager* m_parent;
    char m_szPuID[BVCU_MAX_ID_LEN + 1];
    int m_iGpsChannelNo;
    BVCU_PUCFG_GPSData m_GpsData;
public:
    void OnGetGpsData(const char* szPuID, int iGpsChannel, const BVCU_PUCFG_GPSData* pGpsData);
};

#endif // !__GPS_DATA_H__

