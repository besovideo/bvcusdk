#pragma once


// CPannelGPS dialog
#define WM_GPS_RECVDATA    WM_USER + 2012
#define WM_GPS_CLOSE       WM_USER + 2013
#define WM_TSP_RECVDATA    WM_USER + 2014
#define WM_TSP_GPS_BSHOW    WM_USER + 2015
#include "stdio.h"

class CPannelGPS : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelGPS)

public:
	CPannelGPS(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPannelGPS();

// Dialog Data
	enum { IDD = IDD_PANNEL_GPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRITICAL_SECTION m_cs;
	CString m_ShowGPSStr;
	SplitInfoNode m_splitInfo;
	FILE* m_gpsoutfile;
public:
	BOOL BPlay();
	//get split info 
	BOOL			GetSplitInfo(SplitInfoNode* info);
	//set split info -- copy whole structure
	BOOL			SetSplitInfo(SplitInfoNode* info);

	BVCU_HDialog GetDialog(){ return m_splitInfo.pDialog; }

    CString m_szTspData;
    BOOL m_bHexString;
    afx_msg LRESULT OnRecvGPSData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRecvTSPData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBShowGPSTSP(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSendtsp();
    afx_msg void OnClickedHex();
};
