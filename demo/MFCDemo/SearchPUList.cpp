// SearchPUList.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "SearchPUList.h"
#include "afxdialogex.h"


// SearchPUList 对话框

IMPLEMENT_DYNAMIC(SearchPUList, CDialogEx)

SearchPUList::SearchPUList(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PANNEL_SEARCHPU, pParent)
{
    m_iPUCount = 0;
    m_iLoad = 0;
    m_iIndex = 0;
}

SearchPUList::~SearchPUList()
{
}

void SearchPUList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_COUNT, m_iPUCount);
    DDX_Text(pDX, IDC_EDIT_INDEX, m_iIndex);
    DDX_Text(pDX, IDC_EDIT_LOAD, m_iLoad);
}


BEGIN_MESSAGE_MAP(SearchPUList, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SEARCHPU, &SearchPUList::OnBnClickedButtonSearchpu)
    ON_MESSAGE(WM_SEARCH_PU_LIST, OnMsgUpdateCount)
END_MESSAGE_MAP()


// SearchPUList 消息处理程序


void SearchPUList::UpdateSearchInfo()
{
    UpdateData();
    m_iLoad = CMLibBVCU::m_iLoadPUCount;
    m_iIndex = CMLibBVCU::m_iSearchPUIndex;
    m_iPUCount = CMLibBVCU::m_iPUCount;
    UpdateData(FALSE);
}

void SearchPUList::OnBnClickedButtonSearchpu()
{
    CMLibBVCU::SearchPUList();
    UpdateSearchInfo();
    // TODO: 在此添加控件通知处理程序代码
}

LRESULT SearchPUList::OnMsgUpdateCount(WPARAM wParam, LPARAM lParam)
{
    UpdateSearchInfo();
    return 0;
}
