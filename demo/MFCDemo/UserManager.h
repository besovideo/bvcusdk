#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__

class CMFCDemoDlg;

#include "BVCU.h"
#include "UserConfig.h"

// CUserManager dialog

class CUserManager : public CDialogEx
{
    DECLARE_DYNAMIC(CUserManager)

public:
    CUserManager(CWnd* pParent = NULL);   // standard constructor
    CUserManager(CMFCDemoDlg* parent);
    virtual ~CUserManager();
    enum { IDD = IDD_DIALOG_USER_MANAGER };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    CMFCDemoDlg* m_parent;
    char m_szUserID[AAA_ID_LEN];
    BVCU_UCFG_ModPasswd m_modPassword;
public:
    void SetCurrentUserID(const char* szUserID);
    char* GetCurrentUserID();
    afx_msg void OnBnClickedOk();
};

#endif // !__USER_MANAGER_H__
