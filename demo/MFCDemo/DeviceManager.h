#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

class CMFCDemoDlg;
// CDeviceManager dialog

#include "TreeCtrlX.h"
#include "BVCU.h"
#include "afxcmn.h"
#include "BVDeviceInfo.h"
#include "BVPuServers.h"
#include "BVPuWifi.h"
#include "BVRadioNetwork.h"
#include "BVPuOnlineControl.h"
#include "BVGpsData.h"
#include <list>

#define WM_DEVICE_INFO WM_USER + 2017
#define WM_PU_SERVERS WM_DEVICE_INFO + 1
#define WM_PU_WIFI  WM_PU_SERVERS + 1
#define WM_PU_RADIO WM_PU_WIFI + 1
#define WM_PU_ONLINE_CONTROL WM_PU_RADIO + 1
#define WM_CHANNEL_GPS WM_PU_ONLINE_CONTROL + 1
#define WM_CONTROL_NOTE WM_CHANNEL_GPS + 1

#define ID_MESSAGE_TIMER 0x1010              // 定时器ID

class CDeviceManager : public CDialogEx
{
    DECLARE_DYNAMIC(CDeviceManager)

public:
    CDeviceManager(CWnd* pParent = NULL);   // standard constructor
    CDeviceManager(CMFCDemoDlg* pMainDlg);
    virtual ~CDeviceManager();

    // Dialog Data
    enum { IDD = IDD_DIALOG_DEVICE_MANAGER };
    virtual BOOL OnInitDialog();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
private:
    CMFCDemoDlg* m_pMainDlg;
    CTreeCtrlX m_DeviceTree;
    CBVDeviceInfo m_dlgDeviceInfo;
    CBVPuServers m_dlgPuServers;
    CBVPuWifi m_dlgPuWifi;
    CBVRadioNetwork m_dlgRadioNetwork;
    CBVPuOnlineControl m_dlgOnlineControl;
    CBVGpsData m_dlgGpsData;
    std::list<CDialogEx*> m_dialogList;
    BVCU_Command m_CurrentCmd;
    BOOL m_bTimerAlive;
    HTREEITEM FindPuItem(const char* puId);
    HTREEITEM FindChannelItem(const char* puId, int channelIndex);
    void GetDeviceList();
    void AddRootNode();
    void AddDeviceTreeNode(MPUChannelInfo* pPuChannelInfo);
    BVCU_Result Initalize();
    BVCU_Result Finish();
    TVSORTCB m_tvSortCB;
    void SortDeviceTree();
    void SortTree(HTREEITEM hTreeParent);       // 对hTreeParent子节点进行排序
    static int CALLBACK DeviceSort_Proc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    void SetTabPage(int iPageIndex);
    void ClearTabAllItems();
    void UpdateCurrentCmd(const BVCU_Command* pCommand);
    /* ============================== query command ============================== */
    // 获取device info
    BVCU_Result GetDeviceInfo(const MPUChannelInfo* pDevice);
    void OnGetDeviceInfo(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgDeviceInfo(WPARAM wParam, LPARAM lParam);

    // 获取服务器设置
    BVCU_Result GetPUServer(const MPUChannelInfo* pDevice);
    void OnGetPuServer(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgServers(WPARAM wParam, LPARAM lParam);

    // 获取wifi信息
    BVCU_Result GetPuWifi(const MPUChannelInfo* pDevice);
    void OnGetPuWifi(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgPuWifi(WPARAM wParam, LPARAM lParam);

    // 获取无线网信息
    BVCU_Result GetRadioNetwork(const MPUChannelInfo* pDevice);
    void OnGetRadioNetwork(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgRadioNetwork(WPARAM wParam, LPARAM lParam);

    // 获取上线控制
    BVCU_Result GetOnlineControl(const MPUChannelInfo* pDevice);
    void OnGetOnlineControl(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgOnlineControl(WPARAM wParam, LPARAM lParam);

    // 获取GPS数据
    BVCU_Result GetGpsData(const MPUChannelInfo* pDevice, int iGpsChannelNo);
    void OnGetGpsData(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
    afx_msg LRESULT OnDlgGpsData(WPARAM wParam, LPARAM lParam);

    // call back
    void OnCmdEvent_In(BVCU_HSession hSession, BVCU_Command* pCommand,
        int iEventCode, void* pParam);
    static void OnCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, 
        int iEventCode, void* pParam);

public:
    void AddRootNode(char* rootName);
    void OnGetDeviceList(char* puId, char* puName, int status,
        BVCU_PUOneChannelInfo* channel, int channelNo, int finished);
    CTabCtrl m_tabCtrlDeviceManager;
    afx_msg void OnNMClickDeviceList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTcnSelchangeTabDeviceManager(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonApply();
    afx_msg LRESULT OnDlgControlCmd(WPARAM wParam, LPARAM lParam);

    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void StartTimer(int iTime);
    void StopTimer();
    void OnTimerProcess();
    void ShowMessageNote(CString message);

    /* ============================== control command ============================== */
    // 重启设备
    BVCU_Result RebootDevice(const char* szPuID);
    void OnRebootCmd(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);

    // 设置设备服务器信息
    BVCU_Result SetPuServers(const char* szPuID, const BVCU_PUCFG_Servers* pServers);
    void OnSetPuServers(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);

    // 设置设备wifi信息
    BVCU_Result SetPuWifi(const char* szPuID, const BVCU_PUCFG_Wifi* pWifi);
    void OnSetPuWifi(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);

    // 设置设备上线方式
    BVCU_Result SetPuOnlineControl(const char* szPuID, const BVCU_PUCFG_OnlineControl* pOnlineConrol);
    void OnSetPuOnlineControl(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
};

#endif // !__DEVICE_MANAGER_H__

