#pragma once
#include "afxcmn.h"
#include <afxtempl.h>

struct Color_Font
{
    COLORREF color;
    LOGFONT logfont;
};

class CTreeCtrlX :
    public CTreeCtrl
{
private:
    CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;
public:
    CTreeCtrlX(void);
    ~CTreeCtrlX(void);

    void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
    void SetItemBold(HTREEITEM hItem, BOOL bBold);
    void SetItemColor(HTREEITEM hItem, COLORREF color);
    BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
    BOOL GetItemBold(HTREEITEM hItem);
    COLORREF GetItemColor(HTREEITEM hItem);

    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

