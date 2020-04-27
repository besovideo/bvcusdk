#pragma once


// CPannelTest dialog

class CPannelTest : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelTest)

public:
	CPannelTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPannelTest();

// Dialog Data
	enum { IDD = IDD_PANNEL_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRITICAL_SECTION m_cs;
	SplitInfoNode m_splitInfo;
public:
	afx_msg void SetSplitInfo(const SplitInfoNode* pSplitInfo);
	afx_msg void OnBnClickedButtonVideoin();
	afx_msg void OnBnClickedButtonEncoder();
	afx_msg void OnBnClickedButtonSetvideoin();
	afx_msg void OnBnClickedButtonSetencoder();
};
