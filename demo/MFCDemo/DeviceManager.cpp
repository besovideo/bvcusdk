// DeviceManager.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "DeviceManager.h"
#include "afxdialogex.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include <list>
using namespace std;

// CDeviceManager dialog

IMPLEMENT_DYNAMIC(CDeviceManager, CDialogEx)

CDeviceManager::CDeviceManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DEVICE_MANAGER, pParent)
{

}

CDeviceManager::CDeviceManager(CMFCDemoDlg* pMainDlg)
    : CDialogEx(IDD_DIALOG_DEVICE_MANAGER, NULL)
    , m_bTimerAlive(FALSE)
    , m_pMainDlg(pMainDlg)
    , m_dlgDeviceInfo(this)
    , m_dlgPuServers(this)
    , m_dlgPuWifi(this)
    , m_dlgRadioNetwork(this)
    , m_dlgGpsData(this)
{
}

CDeviceManager::~CDeviceManager()
{
}

void CDeviceManager::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICE_LIST, m_DeviceTree);
    DDX_Control(pDX, IDC_TAB_DEVICE_MANAGER, m_tabCtrlDeviceManager);
}


BEGIN_MESSAGE_MAP(CDeviceManager, CDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_DEVICE_LIST, &CDeviceManager::OnNMClickDeviceList)
    ON_MESSAGE(WM_DEVICE_INFO, OnDlgDeviceInfo)
    ON_MESSAGE(WM_PU_SERVERS, OnDlgServers)
    ON_MESSAGE(WM_PU_WIFI, OnDlgPuWifi)
    ON_MESSAGE(WM_PU_RADIO, OnDlgRadioNetwork)
    ON_MESSAGE(WM_PU_ONLINE_CONTROL, OnDlgOnlineControl)
    ON_MESSAGE(WM_CONTROL_NOTE, OnDlgControlCmd)
    ON_MESSAGE(WM_CHANNEL_GPS, OnDlgGpsData)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DEVICE_MANAGER, &CDeviceManager::OnTcnSelchangeTabDeviceManager)
    ON_BN_CLICKED(ID_BUTTON_APPLY, &CDeviceManager::OnBnClickedButtonApply)
    ON_WM_TIMER()
END_MESSAGE_MAP()

// CDeviceManager message handlers

BOOL CDeviceManager::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    m_dialogList.clear();
    m_dlgDeviceInfo.Create(IDD_DIALOG_DEVICEINFO, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgDeviceInfo);
    m_dlgPuServers.Create(IDD_DIALOG_PU_SERVERS, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgPuServers);
    m_dlgPuWifi.Create(IDD_DIALOG_PU_WIFI, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgPuWifi);
    m_dlgRadioNetwork.Create(IDD_DIALOG_RADIONETWORK, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgRadioNetwork);
    m_dlgOnlineControl.Create(IDD_DIALOG_ONLINE_CONTROL, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgOnlineControl);
    m_dlgGpsData.Create(IDD_DIALOG_GPSDATA, &m_tabCtrlDeviceManager);
    m_dialogList.push_back(&m_dlgGpsData);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BVCU_Result CDeviceManager::Initalize()
{
    AddRootNode();
    GetDeviceList();
    return BVCU_RESULT_S_OK;
}

void CDeviceManager::AddRootNode(char* rootName)
{
    if (!rootName || !rootName[0])
        return;
    HTREEITEM treeRoot = m_DeviceTree.GetRootItem();
    if (treeRoot) return;
    treeRoot = m_DeviceTree.InsertItem(rootName);
    m_DeviceTree.SetItemData(treeRoot, 0);
}
void CDeviceManager::OnGetDeviceList(char* puId, char* puName, int status,
    BVCU_PUOneChannelInfo* channel, int channelNo, int finished)
{
    HTREEITEM onePU = FindPuItem(puId);
    if ((status & PU_STATUS_ONLINE) || (status & PU_STATUS_GETLIST))
    {
        if (!onePU)
        {
            int d_index = CMFCDemoApp::m_data.GetPuIndex(puId);
            if (d_index)
            {
                char displayname[128] = { 0 };
                sprintf_s(displayname, sizeof(displayname), "%s(%s)", puName, puId);
                onePU = m_DeviceTree.GetRootItem();
                onePU = m_DeviceTree.InsertItem(displayname, onePU);
                m_DeviceTree.SetItemData(onePU, d_index);
                if (channelNo > 0)
                {
                    int i;
                    char channelName[32];
                    if (*channel[0].szName == '\0')
                        sprintf_s(channelName, 32, "通道%d", channel[0].iChannelIndex);
                    else
                        strcpy_s(channelName, 32, channel[0].szName);
                    HTREEITEM preChannel = m_DeviceTree.InsertItem(channelName, onePU);
                    m_DeviceTree.SetItemData(preChannel, 0);
                    for (i = 1; i < channelNo; ++i)
                    {
                        if (*channel[i].szName == '\0')
                        {
                            if (BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                                sprintf_s(channelName, 32, "音视通道%d", channel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_GPS <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                                sprintf_s(channelName, 32, "GPS通道%d", channel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_TSP <= channel[i].iChannelIndex &&
                                channel[i].iChannelIndex <= BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                                sprintf_s(channelName, 32, "TSP通道%d", channel[i].iChannelIndex);
                        }
                        else
                            strcpy_s(channelName, 32, channel[i].szName);
                        preChannel = m_DeviceTree.InsertItem(channelName, onePU, preChannel);
                        m_DeviceTree.SetItemData(preChannel, i);
                    }
                }
            }
        }
        if (onePU && (status & PU_STATUS_ONLINE))
            m_DeviceTree.SetItemColor(onePU, 0x00ff0000);
        mainDlg->SortDeviceTree();
        if (finished)
        {
            TRACE("结束\n");
        }
    }
    if (status == PU_STATUS_OFFLINE)
    { // offline
        if (onePU)
            m_DeviceTree.SetItemColor(onePU, 0x00000000);
    }

    SortDeviceTree();
}
void CDeviceManager::GetDeviceList()
{
    list<MPUChannelInfo*>* pDeviceList = CMFCDemoApp::m_data.GetPuList();
    if (!pDeviceList) return;
    for (list<MPUChannelInfo*>::iterator iter = pDeviceList->begin();
        iter != pDeviceList->end(); ++iter)
    {
        AddDeviceTreeNode(*iter);
    }
}
void CDeviceManager::AddRootNode()
{
    BVCU_SessionInfo ssInfo;
    HTREEITEM treeRoot = NULL;
    if (BVCU_Result_SUCCEEDED(BVCU_GetSessionInfo(CMLibBVCU::m_session, &ssInfo)))
    {
        utf8ToAnsi(ssInfo.szServerName, sizeof(ssInfo.szServerName), ssInfo.szServerName);
        treeRoot = m_DeviceTree.InsertItem(ssInfo.szServerName);
    }
    else
        treeRoot = m_DeviceTree.InsertItem(CMFCDemoApp::m_config.GetServerIP());
    m_DeviceTree.SetItemData(treeRoot, 0);
}
void CDeviceManager::AddDeviceTreeNode(MPUChannelInfo* pPuChannelInfo)
{
    if (!pPuChannelInfo) return;
    int status = pPuChannelInfo->iOnlineStatus | PU_STATUS_GETLIST;
    HTREEITEM onePU = FindPuItem(pPuChannelInfo->szPUID);
    if ((status & PU_STATUS_ONLINE)
        || (status & PU_STATUS_GETLIST))
    {
        if (!onePU)
        {
            int d_index = CMFCDemoApp::m_data.GetPuIndex(pPuChannelInfo->szPUID);
            if (d_index)
            {
                onePU = m_DeviceTree.GetRootItem();
                if (pPuChannelInfo->szPUName[0])
                    onePU = m_DeviceTree.InsertItem(pPuChannelInfo->szPUName, onePU);
                else
                    onePU = m_DeviceTree.InsertItem(pPuChannelInfo->szPUID, onePU);
                m_DeviceTree.SetItemData(onePU, d_index);
                if (pPuChannelInfo->iChannelCount)
                {
                    int i;
                    char channelName[32];
                    if (pPuChannelInfo->pChannel[0].szName == '\0')
                        sprintf_s(channelName, 32, "通道%d", 
                            pPuChannelInfo->pChannel[0].iChannelIndex);
                    else
                        strcpy_s(channelName, 32, pPuChannelInfo->pChannel[0].szName);
                    HTREEITEM preChannel = m_DeviceTree.InsertItem(channelName, onePU);
                    m_DeviceTree.SetItemData(preChannel, 0);
                    for (i = 1; i < pPuChannelInfo->iChannelCount; ++i)
                    {
                        if (pPuChannelInfo->pChannel[0].szName == '\0')
                        {
                            if (BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL <= 
                                pPuChannelInfo->pChannel[i].iChannelIndex &&
                                pPuChannelInfo->pChannel[i].iChannelIndex <= 
                                BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                                sprintf_s(channelName, 32, "音视通道%d", 
                                    pPuChannelInfo->pChannel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_GPS <= 
                                pPuChannelInfo->pChannel[i].iChannelIndex &&
                                pPuChannelInfo->pChannel[i].iChannelIndex <=
                                BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                                sprintf_s(channelName, 32, "GPS通道%d", 
                                    pPuChannelInfo->pChannel[i].iChannelIndex);
                            else if (BVCU_SUBDEV_INDEXMAJOR_MIN_TSP <= 
                                pPuChannelInfo->pChannel[i].iChannelIndex &&
                                pPuChannelInfo->pChannel[i].iChannelIndex <= 
                                BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                                sprintf_s(channelName, 32, "TSP通道%d", 
                                    pPuChannelInfo->pChannel[i].iChannelIndex);
                        }
                        else
                            strcpy_s(channelName, 32, pPuChannelInfo->pChannel[i].szName);
                        preChannel = m_DeviceTree.InsertItem(channelName, onePU, preChannel);
                        m_DeviceTree.SetItemData(preChannel, i);
                    }
                }
            }
        }
        if (onePU && (status & PU_STATUS_ONLINE))
            m_DeviceTree.SetItemColor(onePU, 0x00ff0000);
    }
    if (status == PU_STATUS_OFFLINE)
    { // offline
        if (onePU)
            m_DeviceTree.SetItemColor(onePU, 0x00000000);
    }
}

HTREEITEM CDeviceManager::FindPuItem(const char* puId)
{
    HTREEITEM onePU = NULL;
    int puHead = CMFCDemoApp::m_data.GetPuIndex(puId);
    if (puHead)
    { // find the pu in data list; hava to find it in listView.
        onePU = m_DeviceTree.GetRootItem();
        onePU = m_DeviceTree.GetNextVisibleItem(onePU);
        while (onePU)
        {
            if ((unsigned int)m_DeviceTree.GetItemData(onePU)
                == (unsigned int)puHead)
                break;
            onePU = m_DeviceTree.GetNextSiblingItem(onePU);
        }
    }
    return onePU;
}
HTREEITEM CDeviceManager::FindChannelItem(const char* puId, int channelIndex)
{
    HTREEITEM onePU = FindPuItem(puId);
    HTREEITEM oneChannel = NULL;
    if (onePU)
    {
        int pu_index = m_DeviceTree.GetItemData(onePU);
        MPUChannelInfo puInfo;
        ZeroMemory(&puInfo, sizeof(puInfo));
        if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pu_index, &puInfo)))
        {
            oneChannel = m_DeviceTree.GetNextVisibleItem(onePU);
            while (oneChannel)
            {
                unsigned int ch = (unsigned int)m_DeviceTree.GetItemData(oneChannel);
                if (0 <= ch && ch < puInfo.iChannelCount)
                {
                    if (puInfo.pChannel[ch].iChannelIndex == channelIndex)
                        break;
                }
                oneChannel = m_DeviceTree.GetNextSiblingItem(oneChannel);
            }
        }
    }
    return oneChannel;
}

BVCU_Result CDeviceManager::Finish()
{
    return BVCU_RESULT_S_OK;
}

void CDeviceManager::SortDeviceTree()
{
    memset(&m_tvSortCB, 0, sizeof(m_tvSortCB));
    m_tvSortCB.hParent = m_DeviceTree.GetRootItem();
    if (!m_tvSortCB.hParent) return;
    m_tvSortCB.lParam = (LPARAM)&m_DeviceTree;
    m_tvSortCB.lpfnCompare = CMFCDemoDlg::DeviceSort_Proc;
    m_DeviceTree.SortChildrenCB(&m_tvSortCB);
}
void CDeviceManager::SortTree(HTREEITEM hTreeParent)      // 对hTreeParent子节点进行排序
{
    if (!hTreeParent) return;
    memset(&m_tvSortCB, 0, sizeof(m_tvSortCB));
    m_tvSortCB.hParent = m_DeviceTree.GetRootItem();
    if (!m_tvSortCB.hParent) return;
    m_tvSortCB.lParam = (LPARAM)&m_DeviceTree;
    m_tvSortCB.lpfnCompare = CMFCDemoDlg::DeviceSort_Proc;
    m_DeviceTree.SortChildrenCB(&m_tvSortCB);
}
int CDeviceManager::DeviceSort_Proc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

void CDeviceManager::OnNMClickDeviceList(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    CPoint point;
    UINT uFlag;
    BOOL bCheck;
    GetCursorPos(&point);
    m_DeviceTree.ScreenToClient(&point);
    HTREEITEM hTree = m_DeviceTree.HitTest(point, &uFlag);
    do 
    {
        if (hTree && (TVHT_ONITEM & uFlag))
        {
            if (hTree == m_DeviceTree.GetRootItem())
                break;
            m_DeviceTree.SelectItem(hTree);
            int iTreeData = m_DeviceTree.GetItemData(hTree);
            MPUChannelInfo puInfo;
            memset(&puInfo, 0, sizeof(puInfo));
            if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(iTreeData, &puInfo)))
            {
                // 点击的是设备节点
                // m_tabCtrlDeviceManager.DeleteAllItems();
                ClearTabAllItems();
                GetDeviceInfo(&puInfo);
            }
            else
            {
                // 点击的是通道节点
                // m_tabCtrlDeviceManager.DeleteAllItems();
                ClearTabAllItems();
                HTREEITEM hDeviceTree = m_DeviceTree.GetParentItem(hTree);
                if (!hDeviceTree) break;
                int pu_index = m_DeviceTree.GetItemData(hDeviceTree);
                if (BVCU_Result_FAILED(CMFCDemoApp::m_data.GetPu(pu_index, &puInfo)))
                    break;
                int iChannelNo = iTreeData;
                if (iChannelNo > puInfo.iChannelCount)
                    break;
                BVCU_PUOneChannelInfo* pChannelInfo = &(puInfo.pChannel[iChannelNo]);
                if (!pChannelInfo) break;
                if (pChannelInfo->iChannelIndex < BVCU_SUBDEV_INDEXMAJOR_MIN_GPS ||
                    pChannelInfo->iChannelIndex > BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                    break;
                GetGpsData(&puInfo, pChannelInfo->iChannelIndex);
            }
        }
    } while (0);
    *pResult = 0;
}

BVCU_Result CDeviceManager::GetDeviceInfo(const MPUChannelInfo* pDevice)
{
    if (!pDevice)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_DEVICEINFO;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetDeviceInfo(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent || BVCU_Result_FAILED(pEvent->iResult)) return;
    BVCU_PUCFG_DeviceInfo* pDeviceInfo = (BVCU_PUCFG_DeviceInfo*)pEvent->stContent.pData;
    if (!pDeviceInfo) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_DEVICE_INFO, (WPARAM)&m_CurrentCmd, (LPARAM)pDeviceInfo);
}
LRESULT CDeviceManager::OnDlgDeviceInfo(WPARAM wParam, LPARAM lParam)
{
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    BVCU_PUCFG_DeviceInfo* pDeviceInfo = (BVCU_PUCFG_DeviceInfo*)lParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(0, "设备信息");
    m_dlgDeviceInfo.MoveWindow(&rc);
    m_dlgDeviceInfo.OnGetDeviceInfo(pDeviceInfo);
    m_dlgDeviceInfo.ShowWindow(TRUE);
    MPUChannelInfo puInfo;
    memset(&puInfo, 0, sizeof(puInfo));
    if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pDeviceInfo->szID, &puInfo)))
    {
        GetPUServer(&puInfo);
    }
    SetTabPage(0);
    return 0;
}

BVCU_Result CDeviceManager::GetPUServer(const MPUChannelInfo* pDevice)
{
    if (!pDevice)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_SERVERS;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetPuServer(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    BVCU_PUCFG_Servers* pServers = (BVCU_PUCFG_Servers*)pEvent->stContent.pData;
    if (!pServers) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_PU_SERVERS, (WPARAM)&m_CurrentCmd, (LPARAM)pServers);
}
LRESULT CDeviceManager::OnDlgServers(WPARAM wParam, LPARAM lParam)
{
    BVCU_PUCFG_Servers* pServers = (BVCU_PUCFG_Servers*)lParam;
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(m_tabCtrlDeviceManager.GetItemCount(), "服务器设置");
    m_dlgPuServers.OnGetPuServers(pCommand->szTargetID, pServers);
    m_dlgPuServers.MoveWindow(&rc);
    m_dlgPuServers.ShowWindow(TRUE);
    SetTabPage(0);
    MPUChannelInfo puInfo;
    memset(&puInfo, 0, sizeof(puInfo));
    if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pCommand->szTargetID, &puInfo)))
    {
        GetPuWifi(&puInfo);
    }
    return 0;
}

BVCU_Result CDeviceManager::GetPuWifi(const MPUChannelInfo* pDevice)
{
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_WIFI;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetPuWifi(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    BVCU_PUCFG_Wifi* pWifi = (BVCU_PUCFG_Wifi*)pEvent->stContent.pData;
    if (!pWifi) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_PU_WIFI, (WPARAM)&m_CurrentCmd, (LPARAM)pWifi);
}
LRESULT CDeviceManager::OnDlgPuWifi(WPARAM wParam, LPARAM lParam)
{
    BVCU_PUCFG_Wifi* pWifi = (BVCU_PUCFG_Wifi*)lParam;
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(m_tabCtrlDeviceManager.GetItemCount(), "Wifi设置");
    m_dlgPuWifi.OnGetPuWifi(pCommand->szTargetID, pWifi);
    m_dlgPuWifi.MoveWindow(&rc);
    m_dlgPuWifi.ShowWindow(TRUE);
    SetTabPage(0);
    MPUChannelInfo puInfo;
    memset(&puInfo, 0, sizeof(puInfo));
    if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pCommand->szTargetID, &puInfo)))
    {
        GetRadioNetwork(&puInfo);
    }
    return 0;
}

BVCU_Result CDeviceManager::GetRadioNetwork(const MPUChannelInfo* pDevice)
{
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_RADIONETWORK;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetRadioNetwork(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    if (pEvent->stContent.iDataCount <= 0 || !pEvent->stContent.pData) return;
    BVCU_Event_SessionCmd* pEventData = new BVCU_Event_SessionCmd();
    pEventData->iPercent = pEvent->iPercent;
    pEventData->iResult = pEvent->iResult;
    pEventData->stContent.pNext = NULL;
    pEventData->stContent.iDataCount = pEvent->stContent.iDataCount;
    pEventData->stContent.pData = pEvent->stContent.pData;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_PU_RADIO, (WPARAM)&m_CurrentCmd, (LPARAM)pEventData);
}
LRESULT CDeviceManager::OnDlgRadioNetwork(WPARAM wParam, LPARAM lParam)
{
    BVCU_Event_SessionCmd* pEventData = (BVCU_Event_SessionCmd*)lParam;
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(m_tabCtrlDeviceManager.GetItemCount(), "无线网设置");
    m_dlgRadioNetwork.OnGetRadioNetwork(pCommand->szTargetID, 
        (BVCU_PUCFG_RadioNetwork*)pEventData->stContent.pData, pEventData->stContent.iDataCount);
    m_dlgRadioNetwork.MoveWindow(&rc);
    m_dlgRadioNetwork.ShowWindow(TRUE);
    delete pEventData;
    pEventData = NULL;
    SetTabPage(0);
    MPUChannelInfo puInfo;
    memset(&puInfo, 0, sizeof(puInfo));
    if (BVCU_Result_SUCCEEDED(CMFCDemoApp::m_data.GetPu(pCommand->szTargetID, &puInfo)))
    {
        GetOnlineControl(&puInfo);
    }
    return 0;
}

BVCU_Result CDeviceManager::GetOnlineControl(const MPUChannelInfo* pDevice)
{
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_ONLINECONTROL;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetOnlineControl(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    BVCU_PUCFG_OnlineControl* pOnlineControl = (BVCU_PUCFG_OnlineControl*)pEvent->stContent.pData;
    if (!pOnlineControl) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_PU_ONLINE_CONTROL, (WPARAM)&m_CurrentCmd, (LPARAM)pOnlineControl);
}
LRESULT CDeviceManager::OnDlgOnlineControl(WPARAM wParam, LPARAM lParam)
{
    BVCU_PUCFG_OnlineControl* pOnlineControl = (BVCU_PUCFG_OnlineControl*)lParam;
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(m_tabCtrlDeviceManager.GetItemCount(), "上线控制");
    m_dlgOnlineControl.OnGetOnlineControl(pCommand->szTargetID, pOnlineControl);
    m_dlgOnlineControl.MoveWindow(&rc);
    m_dlgOnlineControl.ShowWindow(TRUE);
    SetTabPage(0);
    return 0;
}

BVCU_Result CDeviceManager::GetGpsData(const MPUChannelInfo* pDevice, int iGpsChannelNo)
{
    if (!pDevice ||
        iGpsChannelNo < BVCU_SUBDEV_INDEXMAJOR_MIN_GPS ||
        iGpsChannelNo > BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_GPSDATA;
    strncpy_s(cmd.szTargetID, pDevice->szPUID, _TRUNCATE);
    cmd.iTargetIndex = iGpsChannelNo;
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnGetGpsData(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    BVCU_PUCFG_GPSData* pGpsData = (BVCU_PUCFG_GPSData*)pEvent->stContent.pData;
    if (!pGpsData) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_CHANNEL_GPS, (WPARAM)&m_CurrentCmd, (LPARAM)pGpsData);
}
LRESULT CDeviceManager::OnDlgGpsData(WPARAM wParam, LPARAM lParam)
{
    BVCU_PUCFG_GPSData* pGpsData = (BVCU_PUCFG_GPSData*)lParam;
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    CRect rc;
    m_tabCtrlDeviceManager.GetClientRect(rc);
    rc.top += 20;
    m_tabCtrlDeviceManager.InsertItem(m_tabCtrlDeviceManager.GetItemCount(), "GPS数据");
    m_dlgGpsData.OnGetGpsData(pCommand->szTargetID, pCommand->iTargetIndex, pGpsData);
    m_dlgGpsData.MoveWindow(&rc);
    m_dlgGpsData.ShowWindow(TRUE);
    return 0;
}

void CDeviceManager::OnCmdEvent_In(BVCU_HSession hSession, BVCU_Command* pCommand,
    int iEventCode, void* pParam)
{
    BVCU_Event_SessionCmd* pEvent = (BVCU_Event_SessionCmd*)pParam;
    if (!pEvent) return;
    switch (pCommand->iMethod)
    {
    case BVCU_METHOD_QUERY:
        if (BVCU_SUBMETHOD_PU_DEVICEINFO == pCommand->iSubMethod)
        {
            OnGetDeviceInfo(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_SERVERS == pCommand->iSubMethod)
        {
            OnGetPuServer(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_WIFI == pCommand->iSubMethod)
        {
            OnGetPuWifi(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_RADIONETWORK == pCommand->iSubMethod)
        {
            OnGetRadioNetwork(pCommand, pEvent); 
        }
        else if (BVCU_SUBMETHOD_PU_ONLINECONTROL == pCommand->iSubMethod)
        {
            OnGetOnlineControl(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_GPSDATA == pCommand->iSubMethod)
        {
            OnGetGpsData(pCommand, pEvent);
        }
        break;
    case BVCU_METHOD_CONTROL:
        if (BVCU_SUBMETHOD_PU_REBOOT == pCommand->iSubMethod)
        {
            OnRebootCmd(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_SERVERS == pCommand->iSubMethod)
        {
            OnSetPuServers(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_WIFI == pCommand->iSubMethod)
        {
            OnSetPuWifi(pCommand, pEvent);
        }
        else if (BVCU_SUBMETHOD_PU_ONLINECONTROL == pCommand->iSubMethod)
        {
            OnSetPuOnlineControl(pCommand, pEvent);
        }
        break;
    default:
        break;
    }
}
void CDeviceManager::OnCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, 
    int iEventCode, void* pParam)
{
    mainDlg->GetDeviceManager()->OnCmdEvent_In(hSession, pCommand, iEventCode, pParam);
}

void CDeviceManager::OnTcnSelchangeTabDeviceManager(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    int iCurSel = m_tabCtrlDeviceManager.GetCurSel();
    SetTabPage(iCurSel);
    *pResult = 0;
}
void CDeviceManager::SetTabPage(int iPageIndex)
{
    if (iPageIndex < 0 || iPageIndex >= m_tabCtrlDeviceManager.GetItemCount())
        return;
    switch (iPageIndex)
    {
    case 0:
        m_dlgDeviceInfo.ShowWindow(TRUE);
        m_dlgPuServers.ShowWindow(FALSE);
        m_dlgPuWifi.ShowWindow(FALSE);
        m_dlgRadioNetwork.ShowWindow(FALSE);
        m_dlgOnlineControl.ShowWindow(FALSE);
        break;
    case 1:
        m_dlgDeviceInfo.ShowWindow(FALSE);
        m_dlgPuServers.ShowWindow(TRUE);
        m_dlgPuWifi.ShowWindow(FALSE);
        m_dlgRadioNetwork.ShowWindow(FALSE);
        m_dlgOnlineControl.ShowWindow(FALSE);
        break;
    case 2:
        m_dlgDeviceInfo.ShowWindow(FALSE);
        m_dlgPuServers.ShowWindow(FALSE);
        m_dlgPuWifi.ShowWindow(TRUE);
        m_dlgRadioNetwork.ShowWindow(FALSE);
        m_dlgOnlineControl.ShowWindow(FALSE);
        break;
    case 3:
        m_dlgDeviceInfo.ShowWindow(FALSE);
        m_dlgPuServers.ShowWindow(FALSE);
        m_dlgPuWifi.ShowWindow(FALSE);
        m_dlgRadioNetwork.ShowWindow(TRUE);
        m_dlgOnlineControl.ShowWindow(FALSE);
        break;
    case 4:
        m_dlgDeviceInfo.ShowWindow(FALSE);
        m_dlgPuServers.ShowWindow(FALSE);
        m_dlgPuWifi.ShowWindow(FALSE);
        m_dlgRadioNetwork.ShowWindow(FALSE);
        m_dlgOnlineControl.ShowWindow(TRUE);
        break;
    default:
        break;
    }
}
void CDeviceManager::ClearTabAllItems()
{
    m_tabCtrlDeviceManager.DeleteAllItems();
    for (list<CDialogEx*>::iterator iter = m_dialogList.begin();
        iter != m_dialogList.end(); ++iter)
    {
        (*iter)->ShowWindow(FALSE);
    }
}
void CDeviceManager::UpdateCurrentCmd(const BVCU_Command* pCommand)
{
    if (!pCommand) return;
    memset(&m_CurrentCmd, 0, sizeof(m_CurrentCmd));
    m_CurrentCmd = *pCommand;
}

void CDeviceManager::OnBnClickedButtonApply()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    int iCurTabpage = m_tabCtrlDeviceManager.GetCurSel();
    if (iCurTabpage < 0 || iCurTabpage > m_tabCtrlDeviceManager.GetItemCount())
        return;
    switch (iCurTabpage)
    {
    case 0:  // 设备信息设置
        break;
    case 1:  // 设备服务器设置
        m_dlgPuServers.SetPuServers();
        break;
    case 2:  // 设备wifi设置
        m_dlgPuWifi.SetPuWifi();
        break;
    case 3:
        break;
    case 4:  // 设置上线方式 wifi or 4G
        m_dlgOnlineControl.SetOnlineControl();
        break;
    default:
        break;
    }
}
LRESULT CDeviceManager::OnDlgControlCmd(WPARAM wParam, LPARAM lParam)
{
    BVCU_Command* pCommand = (BVCU_Command*)wParam;
    BVCU_Event_SessionCmd* pEvent = (BVCU_Event_SessionCmd*)lParam;
    if (!pCommand || !pEvent) return 0;
    CString szControlResult("");
    switch (pCommand->iMethod)
    {
    case BVCU_METHOD_QUERY:
        break;
    case BVCU_METHOD_CONTROL:
        if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_REBOOT)
        {
            if (BVCU_Result_SUCCEEDED(pEvent->iResult))
                szControlResult = "重启设备成功";
            else
                szControlResult = "重启设备失败";
        }
        else if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_SERVERS)
        {
            if (BVCU_Result_SUCCEEDED(pEvent->iResult))
                szControlResult = "设置设备服务器成功";
            else
                szControlResult = "设置设备服务器失败";
        }
        else if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_WIFI)
        {
            if (BVCU_Result_SUCCEEDED(pEvent->iResult))
                szControlResult = "设置设备Wifi成功";
            else
                szControlResult = "设置设备Wifi失败";
        }
        else if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_ONLINECONTROL)
        {
            if (BVCU_Result_SUCCEEDED(pEvent->iResult))
                szControlResult = "设置上线控制成功";
            else
                szControlResult = "设置上线控制失败";
        }
        break;
    default:
        break;
    }
    if (szControlResult.GetLength() > 0)
        ShowMessageNote(szControlResult);
    return 0;
}

void CDeviceManager::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch (nIDEvent)
    {
    case ID_MESSAGE_TIMER:
        OnTimerProcess();
        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}
void CDeviceManager::StartTimer(int iTime)
{
    if (!m_bTimerAlive)
    {
        SetTimer(ID_MESSAGE_TIMER, iTime, NULL);
        m_bTimerAlive = TRUE;
    }
}
void CDeviceManager::StopTimer()
{
    if (m_bTimerAlive)
    {
        KillTimer(ID_MESSAGE_TIMER);
        m_bTimerAlive = FALSE;
    }
}
void CDeviceManager::OnTimerProcess()
{
    CString szMessage("");
    ((CEdit*)GetDlgItem(IDC_EDIT_NOTE_MESSAGE))->GetWindowTextA(szMessage);
    if (szMessage.GetLength() > 0)
        ((CEdit*)GetDlgItem(IDC_EDIT_NOTE_MESSAGE))->SetWindowTextA("");
    StopTimer();
}
void CDeviceManager::ShowMessageNote(CString message)
{
    if (message.GetLength() <= 0) return;
    StopTimer();
    ((CEdit*)GetDlgItem(IDC_EDIT_NOTE_MESSAGE))->SetWindowTextA(message);
    UpdateData(FALSE);
    StartTimer(3000);
}

BVCU_Result CDeviceManager::RebootDevice(const char* szPuID)
{
    if (!szPuID || !szPuID[0])
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_REBOOT;
    strncpy_s(cmd.szTargetID, szPuID, _TRUNCATE);
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnRebootCmd(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent) return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_CONTROL_NOTE, (WPARAM)&m_CurrentCmd, (LPARAM)pEvent);
}

BVCU_Result CDeviceManager::SetPuServers(const char* szPuID, 
    const BVCU_PUCFG_Servers* pServers)
{
    if (!szPuID || !szPuID[0] || !pServers)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_ONLINECONTROL;
    strncpy_s(cmd.szTargetID, szPuID, _TRUNCATE);
    cmd.stMsgContent.iDataCount = 1;
    cmd.stMsgContent.pNext = NULL;
    cmd.stMsgContent.pData = (void*)pServers;
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnSetPuServers(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent)
        return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_CONTROL_NOTE, (WPARAM)&m_CurrentCmd, (LPARAM)pEvent);
}

BVCU_Result CDeviceManager::SetPuWifi(const char* szPuID, const BVCU_PUCFG_Wifi* pWifi)
{
    if (!szPuID || !szPuID[0] || !pWifi)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_WIFI;
    strncpy_s(cmd.szTargetID, szPuID, _TRUNCATE);
    cmd.stMsgContent.iDataCount = 1;
    cmd.stMsgContent.pNext = NULL;
    cmd.stMsgContent.pData = (void*)pWifi;
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnSetPuWifi(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent)
        return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_CONTROL_NOTE, (WPARAM)&m_CurrentCmd, (LPARAM)pEvent);
}

BVCU_Result CDeviceManager::SetPuOnlineControl(const char* szPuID,
    const BVCU_PUCFG_OnlineControl* pOnlineConrol)
{
    if (!szPuID || !szPuID[0] || !pOnlineConrol)
        return BVCU_RESULT_E_INVALIDARG;
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.iSize = sizeof(cmd);
    cmd.OnEvent = CDeviceManager::OnCmdEvent;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = BVCU_SUBMETHOD_PU_ONLINECONTROL;
    strncpy_s(cmd.szTargetID, szPuID, _TRUNCATE);
    cmd.stMsgContent.iDataCount = 1;
    cmd.stMsgContent.pNext = NULL;
    cmd.stMsgContent.pData = (void*)pOnlineConrol;
    return BVCU_SendCmd(CMLibBVCU::m_session, &cmd);
}
void CDeviceManager::OnSetPuOnlineControl(BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent)
{
    if (!pCommand || !pEvent)
        return;
    UpdateCurrentCmd(pCommand);
    PostMessage(WM_CONTROL_NOTE, (WPARAM)&m_CurrentCmd, (LPARAM)pEvent);
}
