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

    // ��������
    BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

    // ���ü����
    void SetActivePage(CWnd *pWnd, BOOL bRepaint = TRUE, BOOL bCreate = FALSE);

    // �õ������
    CWnd *GetActivePage();

    // �õ���һ�����ô���
    CWnd *GetNextFreePage(CWnd *pWnd = NULL);

    // ���´���
    virtual void UpdateWnd();

    // ����ڱ߿�
    void SetDrawActivePage(BOOL bFlag,COLORREF clrTopLeft=RGB(255, 0, 0), COLORREF clrBottomRight=RGB(255, 0, 0));
    BOOL GetDrawActivePage();

private:
    CPlayWnd m_playWnd[BV_MAX_WND_NUM];
    // �����ָ��
    CWnd *m_pActivePage;
    // �߿���
    BOOL m_bDrawActive;
    // ���ڱ߿���ɫ
    COLORREF m_clrTopLeft;
    COLORREF m_clrBottomRight;

    // �����Ӵ��ڵ���� �� ��ʾ��Χ ,�õ����ڵ�λ��
    void CalcPageRect(LPRECT lpRect,int nIndex,int nPageCount);

    // ������ڱ߿�
    void DrawActivePage(BOOL bFlag);
};

