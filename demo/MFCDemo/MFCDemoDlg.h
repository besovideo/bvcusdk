
// MFCDemoDlg.h : ͷ�ļ�
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

// CMFCDemoDlg �Ի���
class CMFCDemoDlg : public CDialogEx
{
// ����
public:
    CMFCDemoDlg(CWnd* pParent = NULL);    // ��׼���캯��

// �Ի�������
    enum { IDD = IDD_MFCDemo_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
    // ���� ���� ���
    CRect        m_clientRect;   //�����������λ��
    CRect        m_selectRect;    //��ѡ��ҳ�桱����λ��
    CRect        m_playRect;        //"��Ƶ����"�����λ��
    CRect        m_funcRect;        //"���ܰ���"�����λ��
    CRect        m_listRect;        //"�豸�б�"�����λ��

    CPlayContainer    m_playPannel;        //������Ļ�װ壭�Ӵ���
    
    CPannelSelect   m_pannelSelct;      //����ѡ�� �˵�
    CPannelPTZ      m_pannelPtz;        //��̨���� �˵�
    CPannelCruise   m_pannelCruise;     //Ѳ������ �˵�
    CPannelNormal   m_pannelNormal;     //����˵�
    CPannelGPS      m_pannelGPS;        //GPS���� ��ʾ��
    CPannelRecPlayRemote m_pannelRecordPlayRemote;//Զ�̻طŽ���
    SearchPUList m_pannelSearchPUList;  //��ҳ�����豸����

    CPannelTest m_PannelTest;            // ���Ա�������ͱ�������Ĳ�ѯ������

    CDeviceManager m_DeviceManager;     // �豸�������
    CUserManager m_UserManager;         // �û�������棬Ŀǰֻ֧���޸���������

    void  UpdatePannelPosition();

protected:
    void mOnPannelClick(CWnd& wnd);

public:
    // ������� �� ���_
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
    // ����ʵ��
    int   SnapShot(BVCU_HDialog pDialog, DWORD _tim);
    int   SaveVideo(BVCU_HDialog pDialog, BOOL bSave);
    int   CloseDialog(const SplitInfoNode* pSplitInfo);
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
    void SortTree(HTREEITEM hTreeParent);       // ��hTreeParent�ӽڵ��������
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