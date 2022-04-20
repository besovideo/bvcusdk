#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PlayWnd.h"
#include "PlayContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPlayWnd

CPlayWnd::CPlayWnd()
{
    InitializeCriticalSection(&m_cs);
    memset(&m_splitInfo, 0, sizeof(m_splitInfo));
//    m_textColor.bAlpha = 1;
    /*m_textColor.eColorFormat = BVDISPLAY_COLOR_FMT_RGB;
    m_textColor.bRed = 255;
    m_textColor.bGreen = 255;
    m_textColor.bBlue= 255;*/
    //{{AFX_DATA_INIT(CPlayWnd)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CPlayWnd, CWnd)
    //{{AFX_MSG_MAP(CPlayWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_MESSAGE(VIDEO_MENU_CLOSE, OnCloseDialog)
    //}}AFX_MSG_MAP
    ON_COMMAND_RANGE(VIDEO_MENU_BASE, VIDEO_MENU_END, OnPopupMenu)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CPlayWnd message handlers
BOOL CPlayWnd::OnEraseBkgnd(CDC* pDC) 
{
    // TODO: Add your message handler code here and/or call default
    CRect rt;
    GetClientRect(&rt);
    CBrush br;
    if (m_splitInfo.pDialog)
        br.CreateSolidBrush(VIDEO_BACK_COLOR_WAIT);
    else
        br.CreateSolidBrush(VIDEO_BACK_COLOR_NULL);
    pDC->FillRect(&rt,&br);

    return TRUE;
    return CWnd::OnEraseBkgnd(pDC);
}

LRESULT CPlayWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    // TODO: Add your specialized code here and/or call the base class
    CPlayContainer *pContainer = (CPlayContainer *)GetParent();
    if(pContainer)
    {
        switch(message)
        {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            pContainer->SetActivePage(this);
            break;
        default:
            break;
        }
    }
    return CWnd::DefWindowProc(message, wParam, lParam);
}

BOOL CPlayWnd::DestroyWindow() 
{
    DeleteCriticalSection(&m_cs);
    return CWnd::DestroyWindow();
}

void CPlayWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    // TODO: Add your message handler code here
    CPlayContainer *pContainer = (CPlayContainer *)GetParent();
    pContainer->SetActivePage(this);

    //    CContainerWnd *pContainer = (CContainerWnd *)GetParent();
    CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING , VIDEO_MENU_CLOSE, NAME_MENU_CLOSE);
    menu.AppendMenu(MF_STRING , VIDEO_MENU_SNAPSHOT, NAME_MENU_SNAPSHOT);
    menu.AppendMenu(MF_STRING | m_splitInfo.bSaveData ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_SAVEVIDEO, NAME_MENU_SAVEVIDEO);

    TrackPopupMenu( 
        menu.m_hMenu, 
        TPM_LEFTALIGN, 
        point.x, 
        point.y,
        0,
        m_hWnd,
        NULL);
}

void CPlayWnd::OnPopupMenu(UINT nID)
{
    CPlayContainer *pContainer = (CPlayContainer *)GetParent();
    switch(nID)
    {
    case VIDEO_MENU_CLOSE:
        EnterCriticalSection(&m_cs);
        if (m_splitInfo.pDialog)
        {
            mainDlg->CloseDialog(&m_splitInfo);
            //BVDisplay_DeleteFont((BVDisplay_HWND)GetSafeHwnd(),m_textFont);
        }
        memset(&m_splitInfo,0x00,sizeof(m_splitInfo));
        LeaveCriticalSection(&m_cs);
        break;
    case VIDEO_MENU_SNAPSHOT:
        EnterCriticalSection(&m_cs);
        if (m_splitInfo.pDialog && mainDlg)
        {
            mainDlg->SnapShot(m_splitInfo.pDialog,m_splitInfo.iPTS);
        }
        LeaveCriticalSection(&m_cs);
        break;
    case VIDEO_MENU_SAVEVIDEO:
        EnterCriticalSection(&m_cs);
        if (m_splitInfo.pDialog && mainDlg)
        {
            int result = mainDlg->SaveVideo(m_splitInfo.pDialog,!m_splitInfo.bSaveData);
            if (BVCU_Result_SUCCEEDED(result))
                m_splitInfo.bSaveData = !m_splitInfo.bSaveData;
        }
        LeaveCriticalSection(&m_cs);
        break;
    default:
        break;
    }
}
LRESULT CPlayWnd::OnCloseDialog(WPARAM wParam, LPARAM lParam)
{
    OnPopupMenu(VIDEO_MENU_CLOSE);
    return 0;
}

BOOL CPlayWnd::BPlay()
{
    if (m_splitInfo.pDialog)
        return TRUE;
    return FALSE;
}

BOOL CPlayWnd::GetSplitInfo(SplitInfoNode* info)
{
    if (!info)
    {
        return FALSE;
    }

    EnterCriticalSection(&m_cs);
    memcpy(info, &m_splitInfo, sizeof(SplitInfoNode));
    LeaveCriticalSection(&m_cs);

    return TRUE;
}

BOOL CPlayWnd::SetSplitInfo(SplitInfoNode* info)
{
    if (!info)
    {
        return FALSE;
    }

    EnterCriticalSection(&m_cs);
    memcpy(&m_splitInfo, info, sizeof(SplitInfoNode));
    LeaveCriticalSection(&m_cs);

    OnEraseBkgnd(GetDC());
    return TRUE;
}

void CPlayWnd::BeginRender(BVCU_HDialog pDialog)
{
    if (pDialog == m_splitInfo.pDialog)
    {
        //BVDisplay_CreateFont((BVDisplay_HWND)GetSafeHwnd(),&m_textFont,10,L"Times New Roman");
    }
}
void CPlayWnd::AfterRender(DWORD iPTS)
{
    /*
    if (m_splitInfo.pDialog)
    {
        m_splitInfo.iPTS = iPTS;
        time_t tim = iPTS;
        tm localtim;
        localtime_s(&localtim,&tim);
        wchar_t  sPTS[128];
        swprintf_s(sPTS,128,L"%04d/%02d/%02d %02d%02d%02d",
            localtim.tm_year+1900, localtim.tm_mon+1, localtim.tm_mday,
            localtim.tm_hour,localtim.tm_min,localtim.tm_sec);
        BVDisplay_RECT rect;
        rect.iTop = rect.iLeft = 5;
        rect.iBottom = 45;
        rect.iRight  = 300;
        BVDisplay_DisplayFont((BVDisplay_HWND)GetSafeHwnd(),m_textFont,&rect,&m_textColor,sPTS);
    }
    */
//    InvalidateRect(&rect);
}

void CPlayWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_splitInfo.pDialog)
    {
        RECT rect;
        GetClientRect(&rect);
#ifdef __RECORDPLAY_H__
        BVCU_RecordPlay_SizeCtrl rpSizeCtrl;
        rpSizeCtrl.rcDisplay.iLeft = rect.left;
        rpSizeCtrl.rcDisplay.iRight = rect.right;
        rpSizeCtrl.rcDisplay.iTop = rect.top;
        rpSizeCtrl.rcDisplay.iBottom = rect.bottom;
        if (BVCU_RESULT_E_INVALIDARG != BVCU_RecordPlay_Control(m_splitInfo.pDialog, BVCU_RECORDPLAY_CTRL_RESIZE, &rpSizeCtrl))
        {
            return;
        }
#endif
        CMLibBVCU::ResizeDialogWindow(m_splitInfo.pDialog,&rect);
    }
}
