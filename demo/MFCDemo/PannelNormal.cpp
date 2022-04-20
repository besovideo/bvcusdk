// PannelNormal.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "PannelNormal.h"
#include "afxdialogex.h"
#include "MFCDemoDlg.h"


// CPannelNormal dialog

IMPLEMENT_DYNAMIC(CPannelNormal, CDialogEx)

CPannelNormal::CPannelNormal(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPannelNormal::IDD, pParent)
{
    m_snapshotFile = _T("");
    m_videoFile = _T("");
}

CPannelNormal::CPannelNormal(CMFCDemoDlg* pMain)
    : CDialogEx(CPannelNormal::IDD, NULL /*=NULL*/)
    , m_pMain(pMain)
{

}

CPannelNormal::~CPannelNormal()
{
    m_pMain = NULL;
}

void CPannelNormal::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SNAPSHOT_FILE, m_snapshotFile);
    DDX_Text(pDX, IDC_VIDEOFILE, m_videoFile);
}


BEGIN_MESSAGE_MAP(CPannelNormal, CDialogEx)
    ON_BN_CLICKED(IDC_SNAPSHOT_CH, &CPannelNormal::OnBnClickedSnapshotCh)
    ON_BN_CLICKED(IDC_VIDEO_CH, &CPannelNormal::OnBnClickedVideoCh)
    ON_BN_CLICKED(IDC_BUTTON_DEVICE_MANAGER, &CPannelNormal::OnBnClickedButtonDeviceManager)
    ON_BN_CLICKED(IDC_BUTTON_USER_MANAGER, &CPannelNormal::OnBnClickedButtonUserManager)
END_MESSAGE_MAP()


// CPannelNormal message handlers


void CPannelNormal::OnBnClickedSnapshotCh()
{
    // TODO: Add your control notification handler code here

    // The BROWSEINFO struct tells the shell  
    // how it should display the dialog.  
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));
    bi.ulFlags   = BIF_USENEWUI;
    bi.hwndOwner = NULL;
    bi.lpszTitle = "选择抓图路径";

    // must call this if using BIF_USENEWUI
    ::OleInitialize(NULL);

    // Show the dialog and get the itemIDList for the selected folder. 
    LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
    if(pIDL != NULL)
    {
        // Create a buffer to store the path, then get the path.
        char buffer[_MAX_PATH] = {'\0'};
        if(::SHGetPathFromIDList(pIDL, buffer) != 0)
        {
            // Set the string value.
            m_snapshotFile.Format(buffer);
        }
        // free the item id list
        CoTaskMemFree(pIDL);
    }
    ::OleUninitialize();

    UpdateData(FALSE);
}

void CPannelNormal::OnBnClickedVideoCh()
{
    // TODO: Add your control notification handler code here

    // The BROWSEINFO struct tells the shell  
    // how it should display the dialog.  
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));
    bi.ulFlags   = BIF_USENEWUI;
    bi.hwndOwner = NULL;
    bi.lpszTitle = "选择抓图路径";

    // must call this if using BIF_USENEWUI
    ::OleInitialize(NULL);

    // Show the dialog and get the itemIDList for the selected folder. 
    LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
    if(pIDL != NULL)
    {
        // Create a buffer to store the path, then get the path.
        char buffer[_MAX_PATH] = {'\0'};
        if(::SHGetPathFromIDList(pIDL, buffer) != 0)
        {
            // Set the string value.
            m_videoFile.Format(buffer);
        }
        // free the item id list
        CoTaskMemFree(pIDL);
    }
    ::OleUninitialize();

    UpdateData(FALSE);
}

void CPannelNormal::OnBnClickedButtonDeviceManager()
{
    // TODO: Add your control notification handler code here
    if (m_pMain)
        m_pMain->ShowDeviceManager();
}
void CPannelNormal::OnBnClickedButtonUserManager()
{
    // TODO: Add your control notification handler code here
    if (m_pMain)
        m_pMain->ShowUserManager();
}
