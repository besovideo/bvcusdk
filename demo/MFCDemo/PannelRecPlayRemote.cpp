// PannelRecPlayRemote.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "PannelRecPlayRemote.h"
#include "afxdialogex.h"
#include "libBVCU/MLibBVCU.h"
#include "MFCDemoDlg.h"
#include "Resource.h"

static const int PLAY_WND_PAGE_INDEX = 0; //����ʹ�õĴ���

// CPannelRecPlayRemote �Ի���

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


// CPannelRecPlayRemote ��Ϣ�������

void CPannelRecPlayRemote::OnBnClickedBtnRecOpen()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    CMFCDemoDlg* dlg = (CMFCDemoDlg*)GetParent();
    if (NULL == dlg) {
        return;
    }

    CPlayWnd* nowWnd = (CPlayWnd*)dlg->GetPlayContainer()->GetPage(PLAY_WND_PAGE_INDEX);
    if (NULL == nowWnd) {
        MessageBox("�����Ѿ���ռ��!");
        return;
    }

    SplitInfoNode splitInfo;
    nowWnd->GetSplitInfo(&splitInfo);

    if (nowWnd->BPlay()) {
        MessageBox("�Ѿ�����!");
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

    BOOL bCheckVideo = ((CButton *)GetDlgItem(IDC_CHECK_VIDEO))->GetCheck(); //����1��ʾѡ�ϣ�0��ʾûѡ��
    BOOL bCheckAudio = ((CButton *)GetDlgItem(IDC_CHECK_AUDIO))->GetCheck();

    if (FALSE == bCheckVideo && FALSE == bCheckAudio) {
        MessageBox("��ѡ�� ��Ƶ/��Ƶ!");
        return;
    }

    // ����
    int result = CMLibBVCU::RecordPlayRemote(&splitInfo.pDialog, "G:/PU_123456_00_20180103_100242.mkv", nowWnd->GetSafeHwnd(), &dispRect, opens, bCheckVideo, bCheckAudio);
    if (BVCU_Result_SUCCEEDED(result)) {
        nowWnd->SetSplitInfo(&splitInfo); // ������Ϣ, �ر�ʱ���õ�
    }
    else {
        MessageBox("������Ƶͨ��ʧ��!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecClose()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    CMFCDemoDlg* dlg = (CMFCDemoDlg*)GetParent();
    if (dlg)
    {
        CPlayWnd* nowWnd = (CPlayWnd*)dlg->GetPlayContainer()->GetPage(PLAY_WND_PAGE_INDEX);
        if (nowWnd) {
            if (FALSE == nowWnd->BPlay()) {
                MessageBox("�Ѿ��ر�!");
                return;
            }

            SplitInfoNode splitInfo;
            nowWnd->GetSplitInfo(&splitInfo);// ��ȡ������Ϣ

            dlg->CloseDialog(&splitInfo); // �ر�

            memset(&splitInfo, 0, sizeof(splitInfo));
            nowWnd->SetSplitInfo(&splitInfo); // ������Ϣ
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
            MessageBox("�Ѿ��ر�!");
            return FALSE;
        }

        SplitInfoNode splitInfo;
        nowWnd->GetSplitInfo(&splitInfo);// ��ȡ������Ϣ

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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }

    UINT jumpMs = GetDlgItemInt(IDC_EDIT_REMOTE_PLAY_JUMP);
    int result = CMLibBVCU::RecordPlayRemoteJummp(splitInfo.pDialog, jumpMs);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("����Ƶ��תʧ��!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecStep()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemoteStep(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("����Ƶ����ʧ��!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPause()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemotePause(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("����Ƶ��ͣʧ��!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPlay()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }


    int result = CMLibBVCU::RecordPlayRemotePlay(splitInfo.pDialog);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("����Ƶ����ʧ��!");
    }
}


void CPannelRecPlayRemote::OnBnClickedBtnRecPlaySpeed()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SplitInfoNode splitInfo;
    if (!mGetDialog(NULL, &splitInfo)) {
        return;
    }

    UINT playSpeed = GetDlgItemInt(IDC_EDIT_REMOTE_PLAY_SPEED);
    if (playSpeed < 25 || playSpeed > 400) {
        ::MessageBoxA(NULL, "�����ٶȵ�λ(1/100), ȡֵ��Χ[25 - 400]", "����", MB_OK);
        return;
    }

    int result = CMLibBVCU::RecordPlayRemotePlaySpeed(splitInfo.pDialog, playSpeed);
    if (BVCU_Result_FAILED(result)) {
        MessageBox("����Ƶ�����ٶ�ʧ��!");
    }
}
