#pragma once

// CBVPuServers dialog

class CDeviceManager;

class CBVPuServers : public CDialogEx
{
    DECLARE_DYNAMIC(CBVPuServers)

public:
    CBVPuServers(CWnd* pParent = NULL);   // standard constructor
    CBVPuServers(CDeviceManager* parent);
    virtual ~CBVPuServers();
    // Dialog Data 
    enum { IDD = IDD_DIALOG_PU_SERVERS };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    char m_szPuID[BVCU_MAX_ID_LEN + 1];
    CDeviceManager* m_parent;
    BVCU_PUCFG_Servers m_puServers;
public:
    void OnGetPuServers(const char* szPuID, const BVCU_PUCFG_Servers* pServers);
    BVCU_Result SetPuServers();
};