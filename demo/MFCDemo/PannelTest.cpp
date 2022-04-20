// PannelTest.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "PannelTest.h"
#include "afxdialogex.h"

#define STREAM_COUNT 4
#define TIME_SLICE_COUNT 7

// CPannelTest dialog
static BVCU_PUCFG_VideoIn  VideoInData;

static BVCU_PUCFG_EncoderChannel EcoderData;

// Ŀǰһ������£� �豸���������������ÿ���������7��ʱ��Ƭ����
static BVCU_PUCFG_EncoderStreamParam EcoderStraamParam[STREAM_COUNT];
static BVCU_PUCFG_EncoderParam TimeSlice[TIME_SLICE_COUNT];

CButton* pButtonSetVideoIn = NULL;
CButton* pButtonSetEcoder = NULL;
char *szData = "123456";
BOOL bRunVideoIn = FALSE;
BOOL bRunEcoder = FALSE;

IMPLEMENT_DYNAMIC(CPannelTest, CDialogEx)

CPannelTest::CPannelTest(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPannelTest::IDD, pParent)
{
    ZeroMemory(&m_splitInfo, sizeof(m_splitInfo));
}

CPannelTest::~CPannelTest()
{
}

void CPannelTest::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


static void OnQueryVideoInCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_VIDEOIN)
    {
        BVCU_Event_SessionCmd* pCmdEvent = (BVCU_Event_SessionCmd*)pParam;
        if (BVCU_Result_SUCCEEDED(pCmdEvent->iResult))
        {
            pButtonSetVideoIn->EnableWindow(TRUE);
            
            // ���յ��� ��Ƶ�������� ��������
            VideoInData = *(BVCU_PUCFG_VideoIn*)(pCmdEvent->stContent.pData);
        }
        else
        {
            ::AfxMessageBox("VideoIn��ѯʧ��");
            return;
        }
    }
}

static void OnControlVideoInCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_VIDEOIN)
    {
        BVCU_Event_SessionCmd* pCmdEvent = (BVCU_Event_SessionCmd*)pParam;
        if (BVCU_Result_SUCCEEDED(pCmdEvent->iResult))
        {
            ::AfxMessageBox("VideoIn���óɹ�");
        }
        else
        {
            ::AfxMessageBox("VideoIn��������ʧ��");
            return;
        }
    }
}

static void OnQueryEcoderCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_ENCODERCHANNEL)
    {
        BVCU_Event_SessionCmd* pCmdEvent = (BVCU_Event_SessionCmd*)pParam;
        if (BVCU_Result_SUCCEEDED(pCmdEvent->iResult))
        {
            pButtonSetEcoder->EnableWindow(TRUE);


            // ���յ��� �������� ��������
            EcoderData = *(BVCU_PUCFG_EncoderChannel*)(pCmdEvent->stContent.pData);
            // ָ����Ҫ���
            int i;
            for (i = 0; i < EcoderData.iCount && i < STREAM_COUNT; ++ i)
            {
                EcoderStraamParam[i] = (EcoderData.pParams[i]);
                int j;
                for (j = 0; j < EcoderStraamParam[i].iCount && j < TIME_SLICE_COUNT; ++ j)
                {
                    TimeSlice[j] = EcoderStraamParam[i].pstParams[j];
                }
                EcoderStraamParam[i].iCount = j;
                EcoderStraamParam[i].pstParams = TimeSlice;
            }
            EcoderData.iCount = i;
            EcoderData.pParams = EcoderStraamParam;
        }
        else
        {
            ::AfxMessageBox("���������ѯʧ��");
            return;
        }
    }
}

static void OnControlEcoderCmdEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    if (pCommand->iSubMethod == BVCU_SUBMETHOD_PU_ENCODERCHANNEL)
    {
        BVCU_Event_SessionCmd* pCmdEvent = (BVCU_Event_SessionCmd*)pParam;
        if (BVCU_Result_SUCCEEDED(pCmdEvent->iResult))
        {
            ::AfxMessageBox("����������óɹ�");
        }
        else
        {
            ::AfxMessageBox("���������������ʧ��");
            return;
        }
    }
}

BEGIN_MESSAGE_MAP(CPannelTest, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOIN, &CPannelTest::OnBnClickedButtonVideoin)
    ON_BN_CLICKED(IDC_BUTTON_ENCODER, &CPannelTest::OnBnClickedButtonEncoder)
    ON_BN_CLICKED(IDC_BUTTON_SETVIDEOIN, &CPannelTest::OnBnClickedButtonSetvideoin)
    ON_BN_CLICKED(IDC_BUTTON_SETENCODER, &CPannelTest::OnBnClickedButtonSetencoder)
END_MESSAGE_MAP()


// CPannelTest message handlers

// ��ѯVideoIn����
void CPannelTest::OnBnClickedButtonVideoin()
{
    // TODO: Add your control notification handler code here
    pButtonSetVideoIn = (CButton*)GetDlgItem(IDC_BUTTON_SETVIDEOIN);
    pButtonSetVideoIn->EnableWindow(FALSE);

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnQueryVideoInCmdEvent;  // �ص�

        cmd.iMethod = BVCU_METHOD_QUERY;  //��������
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_VIDEOIN; // ��������

        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex; // ����ͨ����
        strncpy_s(cmd.szTargetID,m_splitInfo.sPUID,_TRUNCATE);  // �豸ID��
        CMLibBVCU::SendCmd(&cmd);
        return;
    }
    return;
}

// ����VideoIn�� VideoInֻ��Ҫ����һ�Σ��������Ҫ�޸ĵĻ�������Ҫÿ�ζ�����
void CPannelTest::OnBnClickedButtonSetvideoin()
{
    // TODO: Add your control notification handler code here
    strncpy_s(VideoInData.szOSDTitle, szData, _TRUNCATE); // ������Ҫ���ӵ����֣�UTF-8���롣�� �ṹ��˵��
    // ����λ�����ã���λ����
    VideoInData.stOSDTitlePos.iLeft = 300;
    VideoInData.stOSDTitlePos.iTop = 50;
    // �����С����
    VideoInData.iOSDTimeFontSize = 12;

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnControlVideoInCmdEvent; // �ص�

        cmd.iMethod = BVCU_METHOD_CONTROL;
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_VIDEOIN;

        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex; // ͨ����
        strncpy_s(cmd.szTargetID, m_splitInfo.sPUID, _TRUNCATE); // �豸ID��

        // ���ݸ���
        cmd.stMsgContent.iDataCount = 1;
        cmd.stMsgContent.pData = &VideoInData;
        CMLibBVCU::SendCmd(&cmd);
        bRunVideoIn = FALSE;
    }
}

// ��ѯ�������
void CPannelTest::OnBnClickedButtonEncoder()
{
    // TODO: Add your control notification handler code here

    pButtonSetEcoder = (CButton*)GetDlgItem(IDC_BUTTON_SETENCODER);

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnQueryEcoderCmdEvent;

        cmd.iMethod = BVCU_METHOD_QUERY;
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_ENCODERCHANNEL;

        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex;
        strncpy_s(cmd.szTargetID,m_splitInfo.sPUID,_TRUNCATE);

        CMLibBVCU::SendCmd(&cmd);
        return;
    }
    return;
}

// ���ñ������   ֻ��Ҫ����һ�Σ��������Ҫ�޸ĵĻ�������Ҫÿ�ζ�����
void CPannelTest::OnBnClickedButtonSetencoder()
{
    // TODO: Add your control notification handler code here
    EcoderData.pParams->pstParams->iOverlay |= BVCU_PUCFG_OVERLAY_TEXT ;  // ��� �Զ������� ���ӡ��� �ṹ��˵��

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnControlEcoderCmdEvent;

        cmd.iMethod = BVCU_METHOD_CONTROL;
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_ENCODERCHANNEL;


        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex;
        strncpy_s(cmd.szTargetID, m_splitInfo.sPUID, _TRUNCATE);

        cmd.stMsgContent.iDataCount = 1;
        cmd.stMsgContent.pData = &EcoderData;
        CMLibBVCU::SendCmd(&cmd);
        bRunVideoIn = FALSE;
    }
}

void CPannelTest::SetSplitInfo(const SplitInfoNode* pSplitInfo)
{
    (CButton*)GetDlgItem(IDC_BUTTON_VIDEOIN)->EnableWindow(TRUE);
    (CButton*)GetDlgItem(IDC_BUTTON_ENCODER)->EnableWindow(TRUE);

    // �򵥲����豸��һЩ��ѯ�������Ϊ��ֹ��һ���豸�����ò������ø�����һ���豸
    // ����ֻ����ָ��һ���豸 �� ͨ���ţ� pDialog�����ж��Ƿ��Ѿ�ָ�����豸
    if (!m_splitInfo.pDialog)
    {
        m_splitInfo = *pSplitInfo;
    }
}
