#pragma once
#include "BVCU.h"

// CBVPuWifi dialog

class CDeviceManager;

class CBVPuWifi : public CDialogEx
{
    DECLARE_DYNAMIC(CBVPuWifi)

public:
    CBVPuWifi(CWnd* pParent = NULL);   // standard constructor
    CBVPuWifi(CDeviceManager* parent);
    virtual ~CBVPuWifi();

// Dialog Data
    enum { IDD = IDD_DIALOG_PU_WIFI };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    char m_szPuID[BVCU_MAX_ID_LEN + 1];
    CDeviceManager* m_parent;
    BVCU_PUCFG_Wifi m_wifi;
public:
    void OnGetPuWifi(const char* szPuID, const BVCU_PUCFG_Wifi* pWifi);
    BVCU_Result SetPuWifi();
};
