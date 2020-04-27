#ifndef __BVPU_ONLINE_CONTROL_H__
#define __BVPU_ONLINE_CONTROL_H__

// CBVPuOnlineControl dialog

#include "BVCU.h"

class CDeviceManager;

class CBVPuOnlineControl : public CDialogEx
{
    DECLARE_DYNAMIC(CBVPuOnlineControl)

public:
    CBVPuOnlineControl(CWnd* pParent = NULL);   // standard constructor
    CBVPuOnlineControl(CDeviceManager* parent);
    virtual ~CBVPuOnlineControl();

    // Dialog Data
    enum { IDD = IDD_DIALOG_ONLINE_CONTROL };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    CDeviceManager* m_parent;
    char m_szPuID[BVCU_MAX_ID_LEN + 1];
    BVCU_PUCFG_OnlineControl m_OnlineControl;
public:
    void OnGetOnlineControl(const char* szPuID, const BVCU_PUCFG_OnlineControl* pOnlineControl);
    BVCU_Result SetOnlineControl();
};

#endif // !__BVPU_ONLINE_CONTROL_H__

