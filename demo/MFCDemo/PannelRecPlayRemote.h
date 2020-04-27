#pragma once

// CPannelRecPlayRemote 对话框

class CPannelRecPlayRemote : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelRecPlayRemote)

public:
	CPannelRecPlayRemote(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPannelRecPlayRemote();

// 对话框数据
	enum { IDD = IDD_PANNEL_RECPLAY_REMOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnRecOpen();
    afx_msg void OnBnClickedBtnRecClose();

protected:
    BOOL mGetDialog(CMFCDemoDlg** pDlg, SplitInfoNode* pSplitInfo);
public:
    afx_msg void OnBnClickedBtnRecJump();
    afx_msg void OnBnClickedBtnRecStep();
    afx_msg void OnBnClickedBtnRecPause();
    afx_msg void OnBnClickedBtnRecPlay();
    afx_msg void OnBnClickedBtnRecPlaySpeed();
};
