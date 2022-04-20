#pragma once


// CPannelCruise dialog

class CPannelCruise : public CDialogEx
{
    DECLARE_DYNAMIC(CPannelCruise)

public:
    CPannelCruise(CWnd* pParent = NULL);   // standard constructor
    virtual ~CPannelCruise();

// Dialog Data
    enum { IDD = IDD_PANNEL_CRUISE };
protected:
    CPannelPTZ* m_brother;
//    BOOL        m_bCruise_go;
    int         m_cruise_now;
public:
    void  ClearStatus();
    void  SetBrother(CPannelPTZ* brother);
    void  SetPtzAttr(BVCU_PUCFG_PTZAttr* ptzAttr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CComboBox m_PresetList;
    CComboBox m_cruiseList;
    afx_msg void OnBnClickedPresetGo();
    afx_msg void OnBnClickedCruiseGo();
    afx_msg void OnSelchangeCruiseList();
};
