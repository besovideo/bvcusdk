#pragma once


// CBVDeviceInfo dialog
#include "BVCU.h"

class CDeviceManager;

class CBVDeviceInfo : public CDialogEx
{
    DECLARE_DYNAMIC(CBVDeviceInfo)

public:
    CBVDeviceInfo(CWnd* pParent = NULL);   // standard constructor
    CBVDeviceInfo(CDeviceManager* parent);
    virtual ~CBVDeviceInfo();

// Dialog Data
    enum { IDD = IDD_DIALOG_DEVICEINFO };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
private:
    CDeviceManager* m_Parent;
    BVCU_PUCFG_DeviceInfo m_deviceInfo;
public:
    virtual BOOL OnInitDialog();
    void OnGetDeviceInfo(const BVCU_PUCFG_DeviceInfo* pDeviceInfo);
    afx_msg void OnBnClickedButtonReboot();
};
