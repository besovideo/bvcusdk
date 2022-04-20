#pragma once


// CPannelPTZ dialog

class CPannelPTZ : public CDialogEx
{
    DECLARE_DYNAMIC(CPannelPTZ)

public:
    CPannelPTZ(CWnd* pParent = NULL);   // standard constructor
    virtual ~CPannelPTZ();

// Dialog Data
    enum { IDD = IDD_PANNEL_PTZ };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
    // 设置 PTZ 控制板 按钮是否可用
    void  SetPUChannelInfo(const SplitInfoNode* pSplitInfo);
    void  EnablePtz(BOOL bEnable);
    void  SendPtzCmd(int iCmd, int bStop, int iParam1, int iParam2, int iParam3);
    DECLARE_MESSAGE_MAP()
public:
    SplitInfoNode  m_splitInfo;
    int m_ptzData;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedPtzset();
};
