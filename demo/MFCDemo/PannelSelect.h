#pragma once


// CPannelSelect dialog

class CPannelSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelSelect)

public:
	CPannelSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPannelSelect();

// Dialog Data
	enum { IDD = IDD_PANNEL_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelPtz();
	afx_msg void OnBnClickedSelNormal();
	afx_msg void OnBnClickedSelSet();
	afx_msg void OnBnClickedSelGPS();
    afx_msg void OnBnClickedButton5();
	afx_msg void OnClickedSelRecordplay();
    afx_msg void OnBnClickedSelRecplayRemote();
};
