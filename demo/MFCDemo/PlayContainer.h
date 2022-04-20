#pragma once

#include "afxwin.h"
#include "PlayWnd.h"

#define  BV_MAX_WND_NUM 4

class CPlayContainer :
    public CWnd
{
public:
    CPlayContainer(void);
    ~CPlayContainer(void);

protected:
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

public:
    CWnd* GetPage(int nIndex);

    CWnd* FindPage(int DateKey);
    CWnd* FindPage(const char* Id, int index);

    // 创建窗口
    BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

    // 设置激活窗口
    void SetActivePage(CWnd *pWnd, BOOL bRepaint = TRUE, BOOL bCreate = FALSE);

    // 得到激活窗口
    CWnd *GetActivePage();

    // 得到下一个可用窗口
    CWnd *GetNextFreePage(CWnd *pWnd = NULL);

    // 更新窗口
    virtual void UpdateWnd();

    // 激活窗口边框
    void SetDrawActivePage(BOOL bFlag,COLORREF clrTopLeft=RGB(255, 0, 0), COLORREF clrBottomRight=RGB(255, 0, 0));
    BOOL GetDrawActivePage();

private:
    CPlayWnd m_playWnd[BV_MAX_WND_NUM];
    // 激活窗口指针
    CWnd *m_pActivePage;
    // 边框标记
    BOOL m_bDrawActive;
    // 窗口边框颜色
    COLORREF m_clrTopLeft;
    COLORREF m_clrBottomRight;

    // 根据子窗口的序号 和 显示范围 ,得到窗口的位置
    void CalcPageRect(LPRECT lpRect,int nIndex,int nPageCount);

    // 画激活窗口边框
    void DrawActivePage(BOOL bFlag);
};

