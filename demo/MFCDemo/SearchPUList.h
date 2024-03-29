#pragma once


// SearchPUList 对话框

#define WM_SEARCH_PU_LIST WM_USER + 2018

class SearchPUList : public CDialogEx
{
    DECLARE_DYNAMIC(SearchPUList)

public:
    SearchPUList(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~SearchPUList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PANNEL_SEARCHPU };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    int m_iPUCount;
    int m_iIndex;
    int m_iLoad;
    void UpdateSearchInfo();
    afx_msg void OnBnClickedButtonSearchpu();
    afx_msg LRESULT OnMsgUpdateCount(WPARAM wParam, LPARAM lParam);
};
