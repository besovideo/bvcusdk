// PannelRecPlayRemote.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "PannelRecPlayRemote.h"
#include "afxdialogex.h"
#include "libBVCU/MLibBVCU.h"
#include "MFCDemoDlg.h"
#include "Resource.h"

static const int PLAY_WND_PAGE_INDEX = 0; //播放使用的窗口

// CPannelRecPlayRemote 对话框

IMPLEMENT_DYNAMIC(CPannelRecPlayRemote, CDialogEx)

CPannelRecPlayRemote::CPannelRecPlayRemote(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPannelRecPlayRemote::IDD, pParent)
{
}

CPannelRecPlayRemote::~CPannelRecPlayRemote()
{
}

void CPannelRecPlayRemote::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    SetDlgItemInt(IDC_EDIT_REMOTE_PLAY_SPEED, 100);
    SetDlgItemInt(IDC_EDIT_REMOTE_PLAY_OPEN, 0);
    SetDlgItemInt(IDC_EDIT_REMOTE_PLAY_JUMP, 0);

    ((CButton *)GetDlgItem(IDC_CHECK_VIDEO))->SetCheck(TRUE);
}


BEGIN_MESSAGE_MAP(CPannelRecPlayRemote, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_REC_OPEN, &CPannelRecPlayRemote::OnBnClickedBtnRecOpen)
    ON_BN_CLICKED(IDC_BTN_REC_CLOSE, &CPannelRecPlayRemote::OnBnClickedBtnRecClose)
    ON_BN_CLICKED(IDC_BTN_REC_JUMP, &CPannelRecPlayRemote::OnBnClickedBtnRecJump)
    ON_BN_CLICKED(IDC_BTN_REC_STEP, &CPannelRecPlayRemote::OnBnClickedBtnRecStep)
    ON_BN_CLICKED(IDC_BTN_REC_PAUSE, &CPannelRecPlayRemote::OnBnClickedBtnRecPause)
    ON_BN_CLICKED(IDC_BTN_REC_PLAY, &CPannelRecPlayRemote::OnBnClickedBtnRecPlay)
    ON_BN_CLICKED(IDC_BTN_REC_PLAY_SPEED, &CPannelRecPlayRemote::OnBnClickedBtnRecPlaySpeed)
END_MESSAGE_MAP()


// CPannelRecPlayRemote 消息处理程序

void CPannelRecPlayRemote::OnBnClickedBtnRecOpen()
{
    // TODO:  在此添加控件通知处理程序代码

    CMFCDemoDlg* dlg = (CMFCDemoDlg*)GetParent();
    if (NULL == dlg) {
        return;
    }

    CPlayWnd* nowWnd = (CPlayWnd*)dlg->GetPlayContainer()->GetPage(PLAY_WND_PAGE_INDEX);
    if (NULL == nowWnd) {
        MessageBox("窗口已经被占用!");
        return;
    }

    SplitInfoNode splitInfo;
    nowWnd->GetSplitInfo(&splitInfo);

    if (nowWnd->BPlay()) {
        MessageBox("已经播放!");
        return;
    }

    RECT play_rect;
    nowWnd->GetClientRect(&play_rect);

    BVCU_Display_Rect dispRect;
    dispRect.iBottom = play_rect.bottom;
    dispRect.iLeft = play_rect.left;
    dispRect.iRight = play_rect.right;
    dispRect.iTop = play_rect.top;


    UINT opens = GetDlgItemInt(IDC_EDIT_REMOTE_PLAY_OPEN);

    BOOL bCheckVideo = ((CButton *)GetDlgItem(IDC_CHECK_VIDEO))->GetCheck(); //返回1表示选上，0表示没选上
    BOOL bCheckAudio = ((CButton *)GetDlgItem(IDC_CHECK_AUDIO))->GetCheck();

    if (FALSE == bCheckVideo && FALSE == bCheckAudio) {
        MessageBox("请选择 音频/视频!");
        return;
    }

    // 播放
    int result = CMLibBVCU::RecordPlayRemote(&splitInfo.pDialog, "G:/PU_123456_00_20180103_100242.mkv", nowWnd->GetSafeHwnd(), &dispRect, opens, bCheckVideo, bCheckAudio);
    if (BVCU_Result_SUCCEEDED(result)) {
        nowWnd->SetSplitInfo(&splitInfo); // 更新信息, 关闭时会用到
    }
    else {
        MessageBox("打开音视频通道失败!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecClose()
{
    // TODO:  在此添加控件通知处理程序代码

    CMFCDemoDlg* dlg = (CMFCDemoDlg*)GetParent();
    if (dlg)
    {
        CPlayWnd* nowWnd = (CPlayWnd*)dlg->GetPlayContainer()->GetPage(PLAY_WND_PAGE_INDEX);
        if (nowWnd) {
            if (FALSE == nowWnd->BPlay()) {
                MessageBox("已经关闭!");
                return;
            }

            SplitInfoNode splitInfo;
            nowWnd->GetSplitInfo(&splitInfo);// 获取播放信息

            dlg->CloseDialog(&splitInfo); // 关闭

            memset(&splitInfo, 0, sizeof(splitInfo));
            nowWnd->SetSplitInfo(&splitInfo); // 更新信息
        }
    }
}


BOOL CPannelRecPlayRemote::mGetDialog(CMFCDemoDlg** pDlg, SplitInfoNode* pSplitInfo)
{
    CMFCDemoDlg* dlg = (CMFCDemoDlg*)GetParent();
    if (NULL == dlg) {
        return FALSE;
    }

    CPlayWnd* nowWnd = (CPlayWnd*)dlg->GetPlayContainer()->GetPage(PLAY_WND_PAGE_INDEX);
    if (nowWnd) {
        if (FALSE == nowWnd->BPlay()) {
            MessageBox("已经关闭!");
            return FALSE;
        }

        SplitInfoNode splitInfo;
        nowWnd->GetSplitInfo(&splitInfo);// 获取播放信息

        if (pDlg) {
            *pDlg = dlg;
        }
        if (pSplitInfo) {
            *pSplitInfo = splitInfo;
        }
        return TRUE;
    }
}

void CPannelRecPlayRemote::OnBnClickedBtnRecJump()
{
    // TODO: 在此添加控件通知处理程序代码
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }

    UINT jumpMs = GetDlgItemInt(IDC_EDIT_REMOTE_PLAY_JUMP);
    int result = CMLibBVCU::RecordPlayRemoteJummp(splitInfo.pDialog, jumpMs);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("音视频跳转失败!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecStep()
{
    // TODO: 在此添加控件通知处理程序代码
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemoteStep(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("音视频单步失败!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPause()
{
    // TODO: 在此添加控件通知处理程序代码
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemotePause(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("音视频暂停失败!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPlay()
{
    // TODO: 在此添加控件通知处理程序代码
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemotePlay(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("音视频播放失败!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPlaySpeed()
{
    // TODO: 在此添加控件通知处理程序代码
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }

    UINT playSpeed = GetDlgItemInt(IDC_EDIT_REMOTE_PLAY_SPEED);
    if (playSpeed < 25 || playSpeed > 400) {
        ::MessageBoxA(NULL, "播放速度单位(1/100), 取值范围[25 - 400]", "警告", MB_OK);
        return;
    }

    int result = CMLibBVCU::RecordPlayRemotePlaySpeed(splitInfo.pDialog, playSpeed);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("音视频更改速度失败!");
    }
}
