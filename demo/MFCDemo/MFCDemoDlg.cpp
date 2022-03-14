
// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDemoDlg 对话框




CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDemoDlg::IDD, pParent)
    , m_pannelNormal(this)
    , m_DeviceManager(this)
    , m_UserManager(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHANNEL_LIST, m_puList);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_CHANNEL_LIST, &CMFCDemoDlg::OnNMDblclkChannelList)
	ON_NOTIFY(NM_RCLICK, IDC_CHANNEL_LIST, &CMFCDemoDlg::OnNMRClickChannelList)
	ON_COMMAND_RANGE(PULIST_MENU_BASE, PULIST_MENU_END, OnPopupMenu)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFCDemoDlg 消息处理程序

BOOL CMFCDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_playPannel.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, 
		CRect(0,0,0,0), this, 1990);
	m_pannelSelct.Create(IDD_PANNEL_SELECT,this);
	m_pannelPtz.Create(IDD_PANNEL_PTZ,this);
	m_pannelCruise.Create(IDD_PANNEL_CRUISE,this);
	m_pannelNormal.Create(IDD_PANNEL_NORMAL,this);
	m_pannelGPS.Create(IDD_PANNEL_GPS,this);
	m_PannelTest.Create(IDD_PANNEL_TEST,this);
	m_PannelRecordPlay.Create(IDD_PANNEL_RECORDPLAY, this);
    m_pannelRecordPlayRemote.Create(CPannelRecPlayRemote::IDD, this);
    m_pannelSearchPUList.Create(IDD_PANNEL_SEARCHPU, this);
	// 初始化 界面
	UpdatePannelPosition();
	m_puList.ShowWindow(SW_SHOW);
	m_playPannel.ShowWindow(SW_SHOW);
	m_pannelSelct.ShowWindow(SW_SHOW);
	m_pannelPtz.ShowWindow(SW_SHOW);

	m_pannelCruise.SetBrother(&m_pannelPtz);

    m_DeviceManager.Create(IDD_DIALOG_DEVICE_MANAGER);
    m_UserManager.Create(IDD_DIALOG_USER_MANAGER);

	// login 
	CLoginDlg loginDlg;
	loginDlg.DoModal();

    char szRootName[BVCU_MAX_NAME_LEN + 1] = { 0 };
	if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_loginStatus))
	{
		// BVCU
        BVCU_SessionInfo ssInfo;
		HTREEITEM treeRoot = NULL;
		if (BVCU_Result_SUCCEEDED(BVCU_GetSessionInfo(CMLibBVCU::m_session,&ssInfo)))
		{
            if (strlen(ssInfo.szServerName) > 0)
            {
                utf8ToAnsi(ssInfo.szServerName, sizeof(ssInfo.szServerName), ssInfo.szServerName);
                treeRoot = m_puList.InsertItem(ssInfo.szServerName);
                strncpy_s(szRootName, ssInfo.szServerName, _TRUNCATE);
            }
            else
            {
                char szServerAddr[BVCU_MAX_HOST_NAME_LEN + 1] = { 0 };
                utf8ToAnsi(szServerAddr, BVCU_MAX_HOST_NAME_LEN + 1, ssInfo.stParam.szServerAddr);
                treeRoot = m_puList.InsertItem(szServerAddr);
                strncpy_s(szRootName, szServerAddr, _TRUNCATE);
            }
		}
        else
        {
            treeRoot = m_puList.InsertItem(CMFCDemoApp::m_config.GetServerIP());
            strncpy_s(szRootName, CMFCDemoApp::m_config.GetServerIP(), _TRUNCATE);
        }
		m_puList.SetItemData(treeRoot,0);
		//CMLibBVCU::GetPUList();
        CMLibBVCU::SearchPUList();
	}
    DeviceManagerRootNode(szRootName);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void  CMFCDemoDlg::UpdatePannelPosition()
{
	GetClientRect(&m_clientRect);

	m_playRect.top = m_clientRect.top + 2;
	m_playRect.bottom = m_clientRect.bottom - 2;
	m_playRect.left = m_clientRect.left + 204;
	m_playRect.right = m_clientRect.right;
	GetDlgItem(IDC_SCREEN_WIN)->MoveWindow(m_playRect);

	m_funcRect.top = m_clientRect.top + 2;
	m_funcRect.bottom = m_funcRect.top + 220;
	m_funcRect.right = m_clientRect.left + 200;
	m_funcRect.left  = m_clientRect.left + 2;	

	m_selectRect.left = m_funcRect.left;
	m_selectRect.right = m_funcRect.right;
	m_selectRect.top = m_funcRect.bottom + 1;
	m_selectRect.bottom = m_selectRect.top + 50 + 30;

	m_listRect.left = m_selectRect.left;
	m_listRect.right = m_selectRect.right;
	m_listRect.top = m_selectRect.bottom + 1;
	m_listRect.bottom = m_clientRect.bottom - 2;

	m_puList.MoveWindow(m_listRect);
	m_playPannel.MoveWindow(m_playRect);
	m_pannelSelct.MoveWindow(m_selectRect);
	m_pannelPtz.MoveWindow(m_funcRect);
	m_pannelCruise.MoveWindow(m_funcRect);
	m_pannelNormal.MoveWindow(m_funcRect);
	m_pannelGPS.MoveWindow(m_funcRect);
	m_PannelRecordPlay.MoveWindow(m_funcRect);
    m_pannelRecordPlayRemote.MoveWindow(m_funcRect);
    m_pannelSearchPUList.MoveWindow(m_funcRect);
}

void CMFCDemoDlg::mOnPannelClick(CWnd& wnd)
{
    m_PannelRecordPlay.ShowWindow(SW_HIDE);
    m_pannelGPS.ShowWindow(SW_HIDE);
    m_pannelNormal.ShowWindow(SW_HIDE);
    m_pannelCruise.ShowWindow(SW_HIDE);
    m_PannelTest.ShowWindow(SW_HIDE);
    m_pannelPtz.ShowWindow(SW_HIDE);
    m_pannelRecordPlayRemote.ShowWindow(SW_HIDE);
    m_pannelSearchPUList.ShowWindow(SW_HIDE);

    if (wnd) {
        wnd.ShowWindow(SW_SHOW);
    }
}

void  CMFCDemoDlg::OnPannelPtz()
{
    mOnPannelClick(m_pannelPtz);
}
void  CMFCDemoDlg::OnPannelCruise()
{
    mOnPannelClick(m_pannelCruise);
}
void  CMFCDemoDlg::OnPannelNormal()
{
    mOnPannelClick(m_pannelNormal);
}
void  CMFCDemoDlg::OnPannelSet()
{
    mOnPannelClick(m_PannelTest);
}
void  CMFCDemoDlg::OnPannelGPS()
{
    mOnPannelClick(m_pannelGPS);
}
void  CMFCDemoDlg::OnPannelTest()
{
    mOnPannelClick(m_PannelTest);
}
void  CMFCDemoDlg::OnPannelRecordPlay()
{
    mOnPannelClick(m_PannelRecordPlay);
}
void  CMFCDemoDlg::OnPannelRecordPlayRemote()
{
    mOnPannelClick(m_pannelRecordPlayRemote);
}
void CMFCDemoDlg::OnPannelSearchPU()
{
    mOnPannelClick(m_pannelSearchPUList);
}
void  CMFCDemoDlg::OnPlayWnd(CPlayWnd* pSelect)
{
	SplitInfoNode splitInfo;
	pSelect->GetSplitInfo(&splitInfo);
	m_pannelPtz.SetPUChannelInfo(&splitInfo);
	if (splitInfo.pDialog)
	{
		HTREEITEM pPUItem = FindChannelItem(splitInfo.sPUID,
			splitInfo.stChannel.iChannelIndex);
		if (pPUItem)
			m_puList.SelectItem(pPUItem);
	}
}

void CMFCDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//窗口最小化与窗口大小无变化不处理
	if (GetDlgItem(IDC_SCREEN_WIN))
	{
		if ((cx ==0 || cy == 0) || 
			(cx == m_clientRect.Width() && cy == m_clientRect.Height())) 
		{
			return;
		}
		else
		{
			UpdatePannelPosition();
			//		UpdateScreen(m_normalBtnPannel.GetSplit());
			Invalidate();
		}
	}
}

void CMFCDemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_playPannel.DestroyWindow();
}

CPlayWnd* CMFCDemoDlg::FindPlayWnd(BVCU_HDialog pDialog)
{
	return (CPlayWnd*)m_playPannel.FindPage((int)pDialog);
}
CPlayWnd* CMFCDemoDlg::FindPlayWnd(const char* puId, int iChannelIndex)
{
	return (CPlayWnd*)m_playPannel.FindPage(puId, iChannelIndex);
}

HTREEITEM CMFCDemoDlg::FindPuItem(const char* puId)
{
	HTREEITEM onePU = NULL;
	int puHead = CMFCDemoApp::m_data.GetPuIndex(puId);
	if (puHead)
	{ // find the pu in data list; hava to find it in listView.
		onePU = m_puList.GetRootItem();
		onePU = m_puList.GetNextVisibleItem(onePU);
		while (onePU)
		{
			if ((unsigned int)m_puList.GetItemData(onePU)
				== (unsigned int)puHead)
				break;
			onePU = m_puList.GetNextSiblingItem(onePU);
		}
	}
	return onePU;
}
HTREEITEM CMFCDemoDlg::FindChannelItem(const char* puId, int channelIndex)
{
	HTREEITEM onePU = FindPuItem(puId);
	HTREEITEM oneChannel = NULL;
	if (onePU)
	{
		DWORD_PTR pu_index = m_puList.GetItemData(onePU);
		MPUChannelInfo puInfo;
		ZeroMemory(&puInfo,sizeof(puInfo));
		if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pu_index,&puInfo)))
		{
			oneChannel = m_puList.GetNextVisibleItem(onePU);
			while (oneChannel)
			{
				int ch = (int)m_puList.GetItemData(oneChannel);
				if (0 <= ch && ch < puInfo.iChannelCount)
				{
					if ( puInfo.pChannel[ch].iChannelIndex == channelIndex)
						break;
				}
				oneChannel = m_puList.GetNextSiblingItem(oneChannel);
			}
		}
	}
	return oneChannel;
}

void CMFCDemoDlg::OnNMDblclkChannelList(NMHDR *pNMHDR, LRESULT *pResult)
{ // 双击预览视频
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM oneChannel = m_puList.GetSelectedItem();
	HTREEITEM onePU = m_puList.GetParentItem(oneChannel);
	if (onePU)
	{
		DWORD_PTR pu_index = m_puList.GetItemData(onePU);
		MPUChannelInfo puInfo;
		ZeroMemory(&puInfo,sizeof(puInfo));
		if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pu_index,&puInfo)))
		{
			// be online
			if (!(puInfo.iOnlineStatus & PU_STATUS_ONLINE))
			{ // offline
				return ;
			}
			// find channel
			if (puInfo.iChannelCount > 0)
			{
				int ch = m_puList.GetItemData(oneChannel);
				if (ch >= puInfo.iChannelCount)
					return ;
				int puChannelIndex = puInfo.pChannel[ch].iChannelIndex;
				if (puChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
				{ // 音视频通道
					CPlayWnd* nowWnd = FindPlayWnd(puInfo.szPUID,puChannelIndex);
					if (nowWnd)
					{ // be open dialog, close dialog now.
						::PostMessage(nowWnd->GetSafeHwnd(),VIDEO_MENU_CLOSE,0,0);
						return ;
					}
					// open dialog;
					nowWnd = (CPlayWnd*)m_playPannel.GetNextFreePage();
					if (nowWnd)
					{
						SplitInfoNode splitInfo;
						RECT play_rect;
						nowWnd->GetSplitInfo(&splitInfo);
						nowWnd->GetClientRect(&play_rect);
                        BVCU_Display_Rect dispRect;
                        dispRect.iBottom = play_rect.bottom;
                        dispRect.iLeft = play_rect.left;
                        dispRect.iRight = play_rect.right;
                        dispRect.iTop = play_rect.top;
						int result = CMLibBVCU::Browse(&splitInfo.pDialog,puInfo.szPUID,
							puChannelIndex,puInfo.pChannel[ch].iMediaDir&(~BVCU_MEDIADIR_AUDIOSEND)&
							(~BVCU_MEDIADIR_VIDEOSEND),nowWnd->GetSafeHwnd(),&dispRect);
						if (BVCU_Result_SUCCEEDED(result))
						{
							strncpy_s(splitInfo.sPUID,BVCU_MAX_ID_LEN+1,puInfo.szPUID,_TRUNCATE);
							memcpy(&splitInfo.stChannel,&puInfo.pChannel[ch],sizeof(splitInfo.stChannel));
							nowWnd->SetSplitInfo(&splitInfo);
							m_PannelTest.SetSplitInfo(&splitInfo);
							if (nowWnd == (CPlayWnd*)m_playPannel.GetActivePage())
								m_pannelPtz.SetPUChannelInfo(&splitInfo); // 设置当前ptz属性
							m_puList.SetItemColor(oneChannel, 0x0000ff00);
						}
						else
							MessageBox("打开音视频通道失败！");
					}
					else
						MessageBox("窗口已经全部被占用！");
				}
				else if (BVCU_SUBDEV_INDEXMAJOR_MIN_GPS <= puChannelIndex && 
					puChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_TSP )
				{ // GPS 数据通道
					SplitInfoNode splitInfo;
					m_pannelGPS.GetSplitInfo(&splitInfo);
					if (m_pannelGPS.BPlay())
					{ // be open dialog, close dialog now.
						if (_strcmpi(splitInfo.sPUID, puInfo.szPUID) == 0 && 
                            puChannelIndex == splitInfo.stChannel.iChannelIndex)
						{
							::PostMessage(m_pannelGPS.GetSafeHwnd(),WM_GPS_CLOSE,0,0);
						}
						else
						{
							MessageBox("GPS信息窗口已被占用，请先关闭打开的GPS通道！");
						}
						return ;
					}
					else
					{
						int result = CMLibBVCU::OpenGPS_TSP(&splitInfo.pDialog, puInfo.szPUID, puChannelIndex);
						if (BVCU_Result_SUCCEEDED(result))
						{
							strncpy_s(splitInfo.sPUID,BVCU_MAX_ID_LEN+1,puInfo.szPUID,_TRUNCATE);
							memcpy(&splitInfo.stChannel,&puInfo.pChannel[ch],sizeof(splitInfo.stChannel));
							m_pannelGPS.SetSplitInfo(&splitInfo);
							m_puList.SetItemColor(oneChannel, 0x0000ff00);
							CMFCDemoApp::m_SendTspDialog = splitInfo.pDialog;
							if (BVCU_SUBDEV_INDEXMAJOR_MIN_TSP <= puChannelIndex && puChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
								::PostMessage(m_pannelGPS.GetSafeHwnd(), WM_TSP_GPS_BSHOW, 1, 0);
						}
						else
							MessageBox("打开GPS/TSP 通道失败！");
					}
				}
			}
		}
	}
}

int  CMFCDemoDlg::CloseDialog(const SplitInfoNode* pSplitInfo)
{
	if (pSplitInfo)
	{
		BVCU_Dialog_Close(pSplitInfo->pDialog);
		HTREEITEM oneChannel = FindChannelItem(pSplitInfo->sPUID,pSplitInfo->stChannel.iChannelIndex);
		if (oneChannel)
			m_puList.SetItemColor(oneChannel, 0x00000000);
		return TRUE;
	}
	return FALSE;
}

int  CMFCDemoDlg::SnapShot(BVCU_HDialog pDialog, DWORD _tim)
{
	if ( pDialog )
	{
		if (m_pannelNormal.m_snapshotFile.GetString()[0])
		{
			time_t _t = _tim;
			tm tim;
			localtime_s(&tim,&_t);
			char fileName[256];
			sprintf_s(fileName,256,"%s\\%04d%02d%02d%02d%02d%02d000.jpg",
				m_pannelNormal.m_snapshotFile.GetString(),
				tim.tm_year+1900,tim.tm_mon+1,tim.tm_mday,tim.tm_hour,tim.tm_min,tim.tm_sec);
			ansiToUtf8(fileName, 256, fileName);
			return BVCU_Dialog_Snapshot(pDialog,fileName,100);
		}
		else
			MessageBox("请先配置抓图路径!");
	}
	return BVCU_RESULT_E_FAILED;
}

int  CMFCDemoDlg::SaveVideo(BVCU_HDialog pDialog, BOOL bSave)
{
	if ( pDialog )
	{
		if (bSave)
		{
			if (m_pannelNormal.m_videoFile.GetString()[0])
			{
				char filePath[256];
				ansiToUtf8(filePath, 256, m_pannelNormal.m_videoFile.GetString());
				return CMLibBVCU::SaveVideo(pDialog,filePath,3600);
			}else
				MessageBox("请先配置视频保存路径!");
		}
		else
			return CMLibBVCU::SaveVideo(pDialog,NULL,0);
	}
	return BVCU_RESULT_E_FAILED;
}

void CMFCDemoDlg::OnNMRClickChannelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CPoint pt(0,0);
	CPoint ptM(0,0);
	GetCursorPos(&pt);
	GetCursorPos(&ptM);
	m_puList.ScreenToClient(&pt);
	HTREEITEM hSelect = NULL;
	hSelect = m_puList.HitTest(pt);
	if(hSelect) m_puList.Select(hSelect,TVGN_CARET);
	HTREEITEM oneChannel = m_puList.GetSelectedItem();
	HTREEITEM hParent = m_puList.GetParentItem(oneChannel);
	HTREEITEM hChild = m_puList.GetNextSiblingItem(oneChannel);
	if (hParent && hChild)
	{
		DWORD_PTR iPuIndex = m_puList.GetItemData(hParent);
		MPUChannelInfo puInfo;
		ZeroMemory(&puInfo, sizeof(puInfo));
		if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(iPuIndex, &puInfo)))
		{
			CMenu menu;
			menu.CreatePopupMenu();
			if (CMFCDemoApp::m_talkDialg.pDialog)
			{
				int ch = m_puList.GetItemData(oneChannel);
				if (ch >= puInfo.iChannelCount)
					return;
				int puChannelIndex = puInfo.pChannel[ch].iChannelIndex;
				if (_strcmpi(puInfo.szPUID, CMFCDemoApp::m_talkDialg.sPUID) == 0 &&
					puChannelIndex == CMFCDemoApp::m_talkDialg.stChannel.iChannelIndex)
					menu.AppendMenu(MF_STRING, PULIST_MENU_TALK, "关闭对讲");
				else
					menu.AppendMenu(MF_STRING, PULIST_MENU_TALK, "打开新对讲");
			}
			else
				menu.AppendMenu(MF_STRING, PULIST_MENU_TALK, "打开对讲");
			TrackPopupMenu(
				menu.m_hMenu,
				TPM_LEFTALIGN,
				ptM.x,
				ptM.y,
				0,
				m_hWnd,
				NULL);
		}
	}
}

void CMFCDemoDlg::OnPopupMenu(UINT nID)
{
	// TODO: Add your command handler code here
	HTREEITEM oneChannel = m_puList.GetSelectedItem();
	HTREEITEM onePU = m_puList.GetParentItem(oneChannel);
	if (onePU)
	{
		DWORD_PTR pu_index = m_puList.GetItemData(onePU);
		MPUChannelInfo puInfo;
		ZeroMemory(&puInfo,sizeof(puInfo));
		if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pu_index, &puInfo)))
		{
			// be online
			if (!(puInfo.iOnlineStatus & PU_STATUS_ONLINE))
			{ // offline
				::AfxMessageBox(_T("当前设备不在线"));
				return ;
			}
			// find channel
			if (puInfo.iChannelCount > 0)
			{
				int ch = m_puList.GetItemData(oneChannel);
				if (ch >= puInfo.iChannelCount)
					return ;
				int puChannelIndex = puInfo.pChannel[ch].iChannelIndex;

				if (puChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
				{ // 音视频通道
					BOOL bNowDialog = FALSE;
					if (_strcmpi(puInfo.szPUID, CMFCDemoApp::m_talkDialg.sPUID) == 0 &&
						puChannelIndex == CMFCDemoApp::m_talkDialg.stChannel.iChannelIndex)
						bNowDialog = TRUE;
					if (CMFCDemoApp::m_talkDialg.pDialog)
					{
						BVCU_Dialog_Close(CMFCDemoApp::m_talkDialg.pDialog);
						ZeroMemory(&CMFCDemoApp::m_talkDialg, sizeof(CMFCDemoApp::m_talkDialg));
					}
					if (!bNowDialog){
						int result = CMLibBVCU::TalkOnly(&CMFCDemoApp::m_talkDialg.pDialog, puInfo.szPUID, puChannelIndex);
						if (BVCU_Result_SUCCEEDED(result))
						{
							strncpy_s(CMFCDemoApp::m_talkDialg.sPUID, BVCU_MAX_ID_LEN + 1, puInfo.szPUID, _TRUNCATE);
							memcpy(&CMFCDemoApp::m_talkDialg.stChannel, &puInfo.pChannel[ch], sizeof(CMFCDemoApp::m_talkDialg.stChannel));
						}
						TRACE("\n返回结果result = %d\n", result);
					}
				}
				else
				{
					::AfxMessageBox(_T("打开的不是语音通道"));
					return;
				}
			}
		}
	}
}

BOOL CMFCDemoDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_INPUTLANGCHANGEREQUEST)
    {
        HKL hkl = (HKL)pMsg->lParam;
        ActivateKeyboardLayout(hkl, KLF_REPLACELANG);//激活对应输入法  

        // 直接返回，不要调用默认的输入法处理；否则会导致死机  
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CMFCDemoDlg::SortDeviceTree()
{
    memset(&m_tvSortCB, 0, sizeof(m_tvSortCB));
    m_tvSortCB.hParent = m_puList.GetRootItem();
    if (!m_tvSortCB.hParent) return;
    m_tvSortCB.lParam = (LPARAM)&m_puList;
    m_tvSortCB.lpfnCompare = CMFCDemoDlg::DeviceSort_Proc;
    m_puList.SortChildrenCB(&m_tvSortCB);
}

void CMFCDemoDlg::SortTree(HTREEITEM hTreeParent)
{
    if (!hTreeParent) return;
    memset(&m_tvSortCB, 0, sizeof(m_tvSortCB));
    m_tvSortCB.hParent = m_puList.GetRootItem();
    if (!m_tvSortCB.hParent) return;
    m_tvSortCB.lParam = (LPARAM)&m_puList;
    m_tvSortCB.lpfnCompare = CMFCDemoDlg::DeviceSort_Proc;
    m_puList.SortChildrenCB(&m_tvSortCB);
}

int CALLBACK CMFCDemoDlg::DeviceSort_Proc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CTreeCtrlX* pTreeCtrl = (CTreeCtrlX*)lParamSort;
    if (!pTreeCtrl) return 0;
    MPUChannelInfo* NodeX = (MPUChannelInfo*)lParam1;
    MPUChannelInfo* NodeY = (MPUChannelInfo*)lParam2;
    if (NodeX && NodeY)
    {
        if ((NodeX->iOnlineStatus & PU_STATUS_ONLINE) &&
            !(NodeY->iOnlineStatus & PU_STATUS_ONLINE))
        {
            return -1;
        }
        else if (!(NodeX->iOnlineStatus & PU_STATUS_ONLINE) &&
            (NodeY->iOnlineStatus & PU_STATUS_ONLINE))
        {
            return 1;
        }
        else
        {
            CString szNodeNameX, szNodeNameY;
            if (strlen(NodeX->szPUName) > 0)
                szNodeNameX.Format("%s", NodeX->szPUName);
            else
                szNodeNameX.Format("%s", NodeX->szPUID);
            if (strlen(NodeY->szPUName) > 0)
                szNodeNameY.Format("%s", NodeY->szPUName);
            else
                szNodeNameY.Format("%s", NodeY->szPUID);
            return szNodeNameX.Compare(szNodeNameY);
        }

    }
    else
    {
        return 0;
    }
}

void CMFCDemoDlg::ShowDeviceManager()
{
    m_DeviceManager.CenterWindow(NULL);
    m_DeviceManager.ShowWindow(TRUE);
}
void CMFCDemoDlg::DeviceManagerRootNode(char* rootName)
{
    m_DeviceManager.AddRootNode(rootName);
}
void CMFCDemoDlg::DeviceManagerDeviceNode(char* puId, char* puName, int status,
    BVCU_PUOneChannelInfo* channel, int channelNo, int finished)
{
    m_DeviceManager.OnGetDeviceList(puId, puName, status, channel, channelNo, finished);
}
CDeviceManager* CMFCDemoDlg::GetDeviceManager()
{
    return &m_DeviceManager;
}

BVCU_Result CMFCDemoDlg::ModCurrentUserPass(BVCU_UCFG_ModPasswd* pModPasswd)
{
    if (!pModPasswd || !pModPasswd->sId[0] ||
        !pModPasswd->sPassword[0] || !pModPasswd->sNewPassword[0])
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(BVCU_Command);
    cmd.stMsgContent.iDataCount = 1;
    cmd.stMsgContent.pData = pModPasswd;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = BVCU_SUBMETHOD_USER_MODPASSWD;
    cmd.OnEvent = CMLibBVCU::cmd_OnEvent;
    return CMLibBVCU::SendCmd(&cmd);
}
void CMFCDemoDlg::ShowUserManager()
{
    if (strlen(m_UserManager.GetCurrentUserID()) <= 0)
    {
        ::AfxMessageBox(_T("尚未登录成功或用户ID为空"));
        return;
    }
    m_UserManager.CenterWindow(NULL);
    m_UserManager.ShowWindow(TRUE);
}
void CMFCDemoDlg::SetCurrentUserID_In(const char* szUserID)
{
    m_UserManager.SetCurrentUserID(szUserID);
}
void CMFCDemoDlg::SetCurrentUserID(const char* szUserID)
{
    if (mainDlg)
        mainDlg->SetCurrentUserID_In(szUserID);
}


void CMFCDemoDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    CDialogEx::OnClose();
}
void CMFCDemoDlg::DisableCloseBtn()
{
    CMenu* pMenu = this->GetSystemMenu(FALSE);//系统菜单
    // pMenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);//禁用关闭按钮
    pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
}
void CMFCDemoDlg::EnableCloseBtn()
{
    CMenu* pMenu = this->GetSystemMenu(FALSE);//系统菜单
    pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
    m_pannelSearchPUList.PostMessage(WM_SEARCH_PU_LIST, NULL, NULL);
}
