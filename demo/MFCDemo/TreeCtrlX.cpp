#include "StdAfx.h"
#include "TreeCtrlX.h"


CTreeCtrlX::CTreeCtrlX(void)
{
}


CTreeCtrlX::~CTreeCtrlX(void)
{
}

BEGIN_MESSAGE_MAP(CTreeCtrlX, CTreeCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CTreeCtrlX::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
    Color_Font cf;
    if( !m_mapColorFont.Lookup( hItem, cf ) )
        cf.color = (COLORREF)-1;
    cf.logfont = logfont;
    m_mapColorFont[hItem] = cf;
}

void CTreeCtrlX::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
    SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

void CTreeCtrlX::SetItemColor(HTREEITEM hItem, COLORREF color)
{
    Color_Font cf;
    if( !m_mapColorFont.Lookup( hItem, cf ) )
        cf.logfont.lfFaceName[0] = '\0';
    cf.color = color;
    m_mapColorFont[hItem] = cf;
    RedrawWindow(NULL);
}

BOOL CTreeCtrlX::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
    Color_Font cf;
    if( !m_mapColorFont.Lookup( hItem, cf ) )
        return FALSE;
    if( cf.logfont.lfFaceName[0] == '\0' ) 
        return FALSE;
    *plogfont = cf.logfont;
    return TRUE;

}

BOOL CTreeCtrlX::GetItemBold(HTREEITEM hItem)
{
    return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CTreeCtrlX::GetItemColor(HTREEITEM hItem)
{
    // Returns (COLORREF)-1 if color was not set
    Color_Font cf;
    if( !m_mapColorFont.Lookup( hItem, cf ) )
        return (COLORREF)-1;
    return cf.color;

}

//添加WM_PAINT 信息监控，重载OnPaint()函数来实现绘制

afx_msg void CTreeCtrlX::OnPaint()
{
    //获取当前绘制对象的DC
    CPaintDC dc(this);

    // 使用将要绘制的对象的DC创建一个memory DC
    //memory device context的概念：是在内存中创建一个结构来反映一个显示（屏幕区域、窗口、
    //打印机等）的表面。可以用来先在内存中准备好要显示的图像，从而实现双缓存，提高刷新
    //速度减少刷新时产生的闪烁。
    CDC memDC;
    //从当前DC创建内存对象
    memDC.CreateCompatibleDC( &dc );

    //定义CRect对象，用来确定区域
    CRect rcClip, rcClient;
    //获取当前对象的边界区域
    dc.GetClipBox( &rcClip );
    //获取当前对象的用户区域
    GetClientRect(&rcClient);

    // Select a compatible bitmap into the memory DC
    //创建一个bmp文件，作为memDC的内容
    //该文件的大小与用于区域相同
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
    memDC.SelectObject( &bitmap );

    // Set clip region to be same as that in paint DC
    //通过对象的边界区域创建CRgn对象
    CRgn rgn;
    rgn.CreateRectRgnIndirect( &rcClip );
    memDC.SelectClipRgn(&rgn);
    rgn.DeleteObject();


    // First let the control do its default drawing.
    //首先让控件自己进行默认的绘制，绘制到内存中
    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

    //获取树状控件的第一个节点
    HTREEITEM hItem = GetFirstVisibleItem();

    //遍历这棵树
    int n = GetVisibleCount()+1;
    while( hItem && n--)
    {
        CRect rect;

        // Do not meddle with selected items or drop highlighted items
        //不对选中的节点和实行拖放功能的节点进行操作
        UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;

        //定义字体、颜色
        Color_Font cf;


        //设置字体
        if ( !(GetItemState( hItem, selflag ) & selflag )
            && m_mapColorFont.Lookup( hItem, cf ))
        {
            CFont *pFontDC;
            CFont fontDC;
            LOGFONT logfont;

            if( cf.logfont.lfFaceName[0] != '\0' )
            {
                //用户定义了字体
                logfont = cf.logfont;
            }
            else
            {
                // 用户没有定义，使用系统字体
                CFont *pFont = GetFont();
                pFont->GetLogFont( &logfont );
            }

            //用户是否设定节点为加粗
            if( GetItemBold( hItem ) )
                logfont.lfWeight = 700;
            //创建字体
            fontDC.CreateFontIndirect( &logfont );
            pFontDC = memDC.SelectObject( &fontDC );

            //设置字体颜色
            if( cf.color != (COLORREF)-1 )
                memDC.SetTextColor( cf.color );

            //获取节点文字
            CString sItem = GetItemText( hItem );

            //获取节点区域
            GetItemRect( hItem, &rect, TRUE );
            //设置背景色为系统色
            memDC.SetBkColor( GetSysColor( COLOR_WINDOW ) );
            //向内存中的图片写入内容,为该节点的内容
            memDC.TextOut( rect.left+2, rect.top+1, sItem );

            memDC.SelectObject( pFontDC );
        }
        hItem = GetNextVisibleItem( hItem );
    }


    dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
        rcClip.left, rcClip.top, SRCCOPY );

}
