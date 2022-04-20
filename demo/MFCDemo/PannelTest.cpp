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

// 目前一般情况下： 设备最多有三个码流，每个码流最多7个时间片配置
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
            
            // 将收到的 视频输入配置 保存下来
            VideoInData = *(BVCU_PUCFG_VideoIn*)(pCmdEvent->stContent.pData);
        }
        else
        {
            ::AfxMessageBox("VideoIn查询失败");
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
            ::AfxMessageBox("VideoIn设置成功");
        }
        else
        {
            ::AfxMessageBox("VideoIn设置命令失败");
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


            // 将收到的 编码配置 保存下来
            EcoderData = *(BVCU_PUCFG_EncoderChannel*)(pCmdEvent->stContent.pData);
            // 指针需要深拷贝
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
            ::AfxMessageBox("编码参数查询失败");
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
            ::AfxMessageBox("编码参数设置成功");
        }
        else
        {
            ::AfxMessageBox("编码参数设置命令失败");
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

// 查询VideoIn参数
void CPannelTest::OnBnClickedButtonVideoin()
{
    // TODO: Add your control notification handler code here
    pButtonSetVideoIn = (CButton*)GetDlgItem(IDC_BUTTON_SETVIDEOIN);
    pButtonSetVideoIn->EnableWindow(FALSE);

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnQueryVideoInCmdEvent;  // 回调

        cmd.iMethod = BVCU_METHOD_QUERY;  //命令类型
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_VIDEOIN; // 具体命令

        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex; // 具体通道号
        strncpy_s(cmd.szTargetID,m_splitInfo.sPUID,_TRUNCATE);  // 设备ID号
        CMLibBVCU::SendCmd(&cmd);
        return;
    }
    return;
}

// 设置VideoIn， VideoIn只需要设置一次，如果不需要修改的话，不需要每次都设置
void CPannelTest::OnBnClickedButtonSetvideoin()
{
    // TODO: Add your control notification handler code here
    strncpy_s(VideoInData.szOSDTitle, szData, _TRUNCATE); // 设置需要叠加的文字，UTF-8编码。见 结构体说明
    // 叠加位置设置，单位像素
    VideoInData.stOSDTitlePos.iLeft = 300;
    VideoInData.stOSDTitlePos.iTop = 50;
    // 字体大小设置
    VideoInData.iOSDTimeFontSize = 12;

    {
        BVCU_Command cmd;
        ZeroMemory(&cmd,sizeof(cmd));
        cmd.iSize = sizeof(cmd);
        cmd.OnEvent = OnControlVideoInCmdEvent; // 回调

        cmd.iMethod = BVCU_METHOD_CONTROL;
        cmd.iSubMethod = BVCU_SUBMETHOD_PU_VIDEOIN;

        cmd.iTargetIndex = m_splitInfo.stChannel.iChannelIndex; // 通道号
        strncpy_s(cmd.szTargetID, m_splitInfo.sPUID, _TRUNCATE); // 设备ID号

        // 数据负载
        cmd.stMsgContent.iDataCount = 1;
        cmd.stMsgContent.pData = &VideoInData;
        CMLibBVCU::SendCmd(&cmd);
        bRunVideoIn = FALSE;
    }
}

// 查询编码参数
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

// 设置编码参数   只需要设置一次，如果不需要修改的话，不需要每次都设置
void CPannelTest::OnBnClickedButtonSetencoder()
{
    // TODO: Add your control notification handler code here
    EcoderData.pParams->pstParams->iOverlay |= BVCU_PUCFG_OVERLAY_TEXT ;  // 添加 自定义文字 叠加。见 结构体说明

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

    // 简单测试设备的一些查询配置命令，为防止将一个设备的配置参数设置给了另一个设备
    // 这里只允许指定一次设备 和 通道号， pDialog用来判断是否已经指定过设备
    if (!m_splitInfo.pDialog)
    {
        m_splitInfo = *pSplitInfo;
    }
}
