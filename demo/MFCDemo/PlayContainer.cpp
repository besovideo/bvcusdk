#include "stdafx.h"
#include "MFCDemo.h"
#include "PlayContainer.h"
#include "MFCDemoDlg.h"


CPlayContainer::CPlayContainer(void)
{
	m_pActivePage = NULL;

	m_bDrawActive = FALSE;
}


CPlayContainer::~CPlayContainer(void)
{
}

BEGIN_MESSAGE_MAP(CPlayContainer, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CPlayContainer::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	dwStyle|=WS_EX_TOOLWINDOW;
	return CWnd::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext );
}

void CPlayContainer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	UpdateWnd();
}

BOOL CPlayContainer::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(RGB(192,192,192));
	pDC->FillRect(&rt,&br);

	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

int  CPlayContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	for(int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		m_playWnd[i].Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
			CRect(0, 0, 0, 0), this, 1990, NULL);
	}

	SetDrawActivePage(TRUE, RGB(248,5,182), RGB(248,5,182));
	SetActivePage(&m_playWnd[0], TRUE, TRUE);

	return 0;
}

void CPlayContainer::OnDestroy() 
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	for(int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		m_playWnd[i].DestroyWindow();
	}
}

CWnd* CPlayContainer::GetPage(int nIndex)
{
	if (nIndex >= 0 && nIndex < BV_MAX_WND_NUM)
	{
		return &m_playWnd[nIndex];
	}

	return NULL;
}

CWnd* CPlayContainer::FindPage(int DateKey)
{
	for (int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		if (m_playWnd[i].GetDataKey() == DateKey)
			return &m_playWnd[i];
	}
	return NULL;
}
CWnd* CPlayContainer::FindPage(const char* Id, int index)
{
	SplitInfoNode  puChannelInfo;
	for (int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		m_playWnd[i].GetSplitInfo(&puChannelInfo);
		if ((puChannelInfo.stChannel.iChannelIndex == index)
			&& (_strcmpi(Id,puChannelInfo.sPUID) ==0))
			return &m_playWnd[i];
	}
	return NULL;
}

void  CPlayContainer::SetActivePage(CWnd *pWnd, BOOL bRepaint, BOOL bCreate /* = FALSE*/)
{
	// check parameter
	if(	!pWnd || !IsWindow(pWnd->m_hWnd) )	return;

	// if pWnd is the Active Page, return 
	if( m_pActivePage == pWnd ) return;

	// check list
	int i = 0;
	for (; i < BV_MAX_WND_NUM; ++ i)
	{
		if ( &m_playWnd[i] == pWnd )
			break;
	}
	if(i >= BV_MAX_WND_NUM)
	{
		TRACE("__This Window is not a member of container, the operation will terminate.\n");
		return;
	}


	if(bRepaint) UpdateWnd();

	if( m_bDrawActive ) DrawActivePage(FALSE);

	m_pActivePage = pWnd;
	if( m_bDrawActive ) DrawActivePage(TRUE);

	CMFCDemoDlg* pParent = (CMFCDemoDlg*)GetParent();
	if (!bCreate)
		pParent->OnPlayWnd((CPlayWnd*)pWnd);
}

CWnd* CPlayContainer::GetActivePage()
{
	return m_pActivePage;
}

CWnd* CPlayContainer::GetNextFreePage(CWnd *pWnd)
{
	CWnd* playWnd;
	int p = 0;
	if (pWnd)
		playWnd = pWnd;
	else
		playWnd = m_pActivePage;
	if (playWnd)
	{
		for (int i = 0; i < BV_MAX_WND_NUM; ++ i)
		{
			if (playWnd == &m_playWnd[i])
			{
				p = i;
				break;
			}
		}
	}
	for (int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		if (!m_playWnd[p].BPlay())
		{
			return &m_playWnd[p];
		}
		p = (p+1)%BV_MAX_WND_NUM;
	}
	return NULL;
}

void CPlayContainer::UpdateWnd()
{
	if(!IsWindowVisible()||IsIconic()) return;

	//得到窗口的坐标
	CRect rtContainer;
	GetClientRect(&rtContainer);
	rtContainer.DeflateRect(1,1);

	//默认状态
	CRect rt;
	
	for(int i = 0; i < BV_MAX_WND_NUM; ++ i)
	{
		rt=rtContainer;
		CalcPageRect(&rt,i,BV_MAX_WND_NUM);
		rt.DeflateRect(1,1,1,1); // 1 为 窗口之间的间隔
		m_playWnd[i].MoveWindow(&rt);
		m_playWnd[i].ShowWindow(SW_SHOW);
	}
	if( m_bDrawActive ) DrawActivePage(TRUE);
}

void CPlayContainer::SetDrawActivePage(BOOL bFlag,COLORREF clrTopLeft,COLORREF clrBottomRight)
{
	if(m_bDrawActive==bFlag) return;
	if(bFlag)
	{
		m_clrTopLeft=clrTopLeft;
		m_clrBottomRight=clrBottomRight;
	}
	m_bDrawActive=bFlag;
	DrawActivePage(bFlag);
}
BOOL CPlayContainer::GetDrawActivePage()
{
	return m_bDrawActive;
}

void CPlayContainer::CalcPageRect(LPRECT lpRect,int nIndex,int nPageCount)
{
	if((nPageCount<=0)||(nIndex>=BV_MAX_WND_NUM))
	{
		lpRect->left=lpRect->right=lpRect->top=lpRect->bottom=0;
		return;
	}
	//get row count
	int nRow=0;
	while((nRow)*(nRow)<nPageCount) nRow++;

	//get singledlg width and height
	int nWidth=(lpRect->right-lpRect->left)/nRow;
	int nHeight=(lpRect->bottom-lpRect->top)/nRow;

	//get top-left point
	CPoint pt;
	pt.x=lpRect->left+nWidth*(nIndex%nRow);
	pt.y=lpRect->top+nHeight*(nIndex/nRow);

	//set rect return back
	lpRect->left=pt.x;
	lpRect->top=pt.y;
	lpRect->right=lpRect->left+nWidth;
	lpRect->bottom=lpRect->top+nHeight;
}

void CPlayContainer::DrawActivePage(BOOL bFlag)
{
	if( !m_pActivePage ) return;

	CRect rt;
	m_pActivePage->GetWindowRect(&rt);
	ScreenToClient(&rt);
	rt.InflateRect(1,1);

	if(bFlag)
	{
		CDC *pDC=GetDC();
		if(!pDC) return;

		//		pDC->Draw3dRect(&rt,m_clrTopLeft, m_clrBottomRight);	
		pDC->Draw3dRect(&rt,RGB(255,0,0), RGB(255,0,0));

		ReleaseDC(pDC);
	}
	else
		InvalidateRect(&rt);
}