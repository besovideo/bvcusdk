#pragma once

#include "MFCDemo.h"

// menu
#define	VIDEO_MENU_BASE         WM_USER + 1990
#define VIDEO_MENU_CLOSE        WM_USER + 1991
#define	VIDEO_MENU_SNAPSHOT     WM_USER + 1992
#define VIDEO_MENU_SAVEVIDEO    WM_USER + 1993
#define	VIDEO_MENU_END			WM_USER + 2000

#define NAME_MENU_CLOSE         "¹Ø±ÕÊÓÆµÔ¤ÀÀ"
#define	NAME_MENU_SNAPSHOT      "×¥Í¼"
#define NAME_MENU_SAVEVIDEO     "±£´æÊÓÆµÂ¼Ïñ"

// Back Color
#define	VIDEO_BACK_COLOR_NULL	RGB(0,0,0)
#define VIDEO_BACK_COLOR_WAIT   RGB(125,132,255)

// CPlayWnd dialog

class CPlayWnd : public CWnd
{
public:
	CPlayWnd(); 

	virtual BOOL DestroyWindow();

protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd*,CPoint);
	afx_msg void OnPopupMenu(UINT nID);
	afx_msg LRESULT OnCloseDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int  GetDataKey(){return (int)m_splitInfo.pDialog;}

	BOOL BPlay();
	//get split info 
	BOOL			GetSplitInfo(SplitInfoNode* info);
	//set split info -- copy whole structure
	BOOL			SetSplitInfo(SplitInfoNode* info);
	
	void BeginRender(BVCU_HDialog pDialog);
	void AfterRender(DWORD iPTS);
private:
	CRITICAL_SECTION m_cs;
	SplitInfoNode m_splitInfo;

	int  m_textFont;
};
