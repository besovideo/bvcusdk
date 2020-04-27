#pragma once

class CMFCDemoDlg;
// CPannelNormal dialog

class CPannelNormal : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelNormal)

public:
	CPannelNormal(CWnd* pParent = NULL);   // standard constructor
    CPannelNormal(CMFCDemoDlg* pMain);
	virtual ~CPannelNormal();

// Dialog Data
	enum { IDD = IDD_PANNEL_NORMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_snapshotFile;
	CString m_videoFile;
	afx_msg void OnBnClickedSnapshotCh();
	afx_msg void OnBnClickedVideoCh();
    CMFCDemoDlg* m_pMain;
    afx_msg void OnBnClickedButtonDeviceManager();
    afx_msg void OnBnClickedButtonUserManager();
};
