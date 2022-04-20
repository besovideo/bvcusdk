
// MFCDemoDlg.h : 头文件
//

#pragma once
#include "PlayContainer.h"
#include "PannelSelect.h"
#include "PannelPTZ.h"
#include "PannelCruise.h"
#include "PannelNormal.h"
#include "PannelGPS.h"
#include "PannelTest.h"
#include "PannelRecPlayRemote.h"
#include "TreeCtrlX.h"
#include "DeviceManager.h"
#include "UserManager.h"
#include "SearchPUList.h"

// menu
#define    PULIST_MENU_BASE         WM_USER + 1990
#define PULIST_MENU_TALK        WM_USER + 1991
#define    PULIST_MENU_END            WM_USER + 2000

// CMFCDemoDlg 对话框
class CMFCDemoDlg : public CDialogEx
{
// 构造
public:
    CMFCDemoDlg(CWnd* pParent = NULL);    // 标准构造函数

// 对话框数据
    enum { IDD = IDD_MFCDemo_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
    // 界面 及其 相关
    CRect        m_clientRect;   //程序界面区域位置
    CRect        m_selectRect;    //“选择页面”面板的位置
    CRect        m_playRect;        //"视频播放"区域的位置
    CRect        m_funcRect;        //"功能按键"区域的位置
    CRect        m_listRect;        //"设备列表"区域的位置

    CPlayContainer    m_playPannel;        //播放屏幕底板－子窗口
    
    CPannelSelect   m_pannelSelct;      //界面选择 菜单
    CPannelPTZ      m_pannelPtz;        //云台控制 菜单
    CPannelCruise   m_pannelCruise;     //巡航控制 菜单
    CPannelNormal   m_pannelNormal;     //常规菜单
    CPannelGPS      m_pannelGPS;        //GPS数据 显示框
    CPannelRecPlayRemote m_pannelRecordPlayRemote;//远程回放界面
    SearchPUList m_pannelSearchPUList;  //分页加载设备界面

    CPannelTest m_PannelTest;            // 测试报警输入和编码参数的查询和设置

    CDeviceManager m_DeviceManager;     // 设备管理界面
    CUserManager m_UserManager;         // 用户管理界面，目前只支持修改自身密码

    void  UpdatePannelPosition();

protected:
    void mOnPannelClick(CWnd& wnd);

public:
    // 界面操作 及 相关_
    void  OnPannelPtz();
    void  OnPannelCruise();
    void  OnPannelNormal();
    void  OnPannelSet();
    void  OnPannelGPS();
    void  OnPannelTest();
    void  OnPannelRecordPlayRemote();
    void  OnPannelSearchPU();
    void  OnPlayWnd(CPlayWnd* pSelect);

    CPlayContainer* GetPlayContainer(){ return &m_playPannel; }
    CPannelCruise*  GetPannelCruise(){ return &m_pannelCruise;}
    CPannelGPS*     GetPannelGPS(){ return &m_pannelGPS; }
    CPannelTest*    GetPannelTest(){ return &m_PannelTest; }

    CPlayWnd* FindPlayWnd(BVCU_HDialog pDialog);
    CPlayWnd* FindPlayWnd(const char* puId, int iChannelIndex);
    HTREEITEM FindPuItem(const char* puId);
    HTREEITEM FindChannelItem(const char* puId, int channelIndex);
    // 功能实现
    int   SnapShot(BVCU_HDialog pDialog, DWORD _tim);
    int   SaveVideo(BVCU_HDialog pDialog, BOOL bSave);
    int   CloseDialog(const SplitInfoNode* pSplitInfo);
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPopupMenu(UINT nID);

    DECLARE_MESSAGE_MAP()
public:
    CTreeCtrlX m_puList;
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    afx_msg void OnNMDblclkChannelList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickChannelList(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    TVSORTCB m_tvSortCB;
    void SortDeviceTree();
    void SortTree(HTREEITEM hTreeParent);       // 对hTreeParent子节点进行排序
    static int CALLBACK DeviceSort_Proc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    void ShowDeviceManager();
    void DeviceManagerRootNode(char* rootName);
    void DeviceManagerDeviceNode(char* puId, char* puName, int status,
        BVCU_PUOneChannelInfo* channel, int channelNo, int finished);
    CDeviceManager* GetDeviceManager();

    BVCU_Result ModCurrentUserPass(BVCU_UCFG_ModPasswd* pModPasswd);
    void ShowUserManager();
    void SetCurrentUserID_In(const char* szUserID);
    static void SetCurrentUserID(const char* szUserID);
    afx_msg void OnClose();
    void DisableCloseBtn();
    void EnableCloseBtn();
};

extern CMFCDemoDlg* mainDlg;