#pragma once
#include "BVCU.h"

// CBVRadioNetwork dialog

class CDeviceManager;

class CBVRadioNetwork : public CDialogEx
{
	DECLARE_DYNAMIC(CBVRadioNetwork)

public:
    CBVRadioNetwork(CWnd* pParent = NULL);   // standard constructor
    CBVRadioNetwork(CDeviceManager* parent);
	virtual ~CBVRadioNetwork();

// Dialog Data
	enum { IDD = IDD_DIALOG_RADIONETWORK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    CDeviceManager* m_parent;
    char m_szPuID[BVCU_MAX_ID_LEN + 1];
    int m_iRadioCount;
    BVCU_PUCFG_RadioNetwork m_radioNetwork[16];
    CString GetRadioNetwordType(int type);
public:
    void OnGetRadioNetwork(const char* szPuID, const BVCU_PUCFG_RadioNetwork* pRadioNetwork, 
        int iRadioCount);
};
