// PannelSelect.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "PannelSelect.h"
#include "afxdialogex.h"

// CPannelSelect dialog

IMPLEMENT_DYNAMIC(CPannelSelect, CDialogEx)

CPannelSelect::CPannelSelect(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPannelSelect::IDD, pParent)
{

}

CPannelSelect::~CPannelSelect()
{
}

void CPannelSelect::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPannelSelect, CDialogEx)
    ON_BN_CLICKED(IDC_SEL_PTZ, &CPannelSelect::OnBnClickedSelPtz)
    ON_BN_CLICKED(IDC_SEL_NORMAL, &CPannelSelect::OnBnClickedSelNormal)
    ON_BN_CLICKED(IDC_SEL_SET, &CPannelSelect::OnBnClickedSelSet)
    ON_BN_CLICKED(IDC_SEL_PULIST, &CPannelSelect::OnBnClickedSelGPS)
    ON_BN_CLICKED(IDC_BUTTON5, &CPannelSelect::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_SEL_RECPLAY_REMOTE, &CPannelSelect::OnBnClickedSelRecplayRemote)
END_MESSAGE_MAP()


// CPannelSelect message handlers


void CPannelSelect::OnBnClickedSelPtz()
{
    // TODO: Add your control notification handler code here
    ((CMFCDemoDlg*)GetParent())->OnPannelPtz();
}


void CPannelSelect::OnBnClickedSelNormal()
{
    // TODO: Add your control notification handler code here
    ((CMFCDemoDlg*)GetParent())->OnPannelNormal();
}


void CPannelSelect::OnBnClickedSelSet()
{
    // TODO: Add your control notification handler code here
    //((CMFCDemoDlg*)GetParent())->OnPannelSet();
    ((CMFCDemoDlg*)GetParent())->OnPannelTest();
}


void CPannelSelect::OnBnClickedSelGPS()
{
    // TODO: Add your control notification handler code here
    ((CMFCDemoDlg*)GetParent())->OnPannelGPS();
}

/*void Query_Ftp_Record();
void Login_Ftp();
BVCU_FTP_HSession m_FtpSessionHandle = NULL;*/
void CPannelSelect::OnBnClickedButton5()
{
    ((CMFCDemoDlg*)GetParent())->OnPannelSearchPU();
    /*if (NULL == m_FtpSessionHandle) 
    {
        Login_Ftp();
    }
    else
    {
        Query_Ftp_Record();
    }*/

}

void CPannelSelect::OnBnClickedSelRecplayRemote()
{
    // TODO:  在此添加控件通知处理程序代码
    ((CMFCDemoDlg*)GetParent())->OnPannelRecordPlayRemote();
}

/*void ms_On_Ftp_Event(BVCU_FTP_HSession hSession, int iEventCode, void* pParam)
{
    BVCU_Result result = ((BVCU_Event_Common*)pParam)->iResult;
    printf_s("\n\n=========ms_On_Ftp_Event: hSession:%d, iEventCode:%d, result:%d.\n\n", hSession, iEventCode, result);
}
void ms_On_Ftp_Cmd(BVCU_FTP_HSession hSession, BVCU_FTP_Command* pCommand, int iEventCode, void* pParam)
{
    BVCU_Event_SessionCmd* sessionCmd = (BVCU_Event_SessionCmd*)pParam;
    int dataCount = sessionCmd->stContent.iDataCount;

    string * fileNames = new string[dataCount];
    BVCU_FTP_RecordFileInfo* infos = (BVCU_FTP_RecordFileInfo*)(sessionCmd->stContent.pData);
    for (int i = 0;i < dataCount;i++)
    {
        fileNames[i] = string(infos[i].szFileName);
    }

    printf_s("\n\n---------ms_On_Ftp_Cmd: hSession:%d, iResult:%d, dataCount:%d, iEventCode:%d.\n", 
        hSession, sessionCmd->iResult, dataCount, iEventCode);
    for (int i = 0;i < dataCount; i++)
    {
        printf_s("fileName: %s\n", fileNames[i]);
    }
}

void Login_Ftp()
{
    BVCU_FTP_ServerParam pParam;//= new BVCU_FTP_ServerParam();

    memset(&pParam, 0, sizeof(pParam));
    pParam.iSize = sizeof(pParam);
    pParam.hSession = CMLibBVCU::m_session;
    strcpy_s(pParam.szID, sizeof(pParam.szID), "PU_273A");
    memset(pParam.szUserName, 0, sizeof(pParam.szUserName));
    memset(pParam.szPassword, 0, sizeof(pParam.szPassword));
    memset(pParam.szServerAddr, 0, sizeof(pParam.szServerAddr));
    //strcpy_s(pParam.szUserName, sizeof(pParam.szUserName), "");
    //strcpy_s(pParam.szPassword, sizeof(pParam.szPassword), "");
    //strcpy_s(pParam.szServerAddr, sizeof(pParam.szServerAddr), "");
    pParam.iServerPort = 0;
    pParam.iTimeOut = 10000;// loginTimeOut <= 0 ? 300000 : loginTimeOut;
    pParam.iKeepAliveInterval = 0;

    pParam.OnEvent = ms_On_Ftp_Event;

    BVCU_Result ret = BVCU_FTP_Login(&m_FtpSessionHandle, &pParam);
    printf_s("\n\n+++++++++++++BVCU_FTP_Login:%d\n\n", ret);
}

void Query_Ftp_Record()
{
    BVCU_FTP_Command cmd;
    memset(&cmd, 0, sizeof(BVCU_FTP_Command));
    cmd.iSize = sizeof(BVCU_FTP_Command);
    cmd.iMethod = BVCU_FTP_METHOD_SEARCH_RECORDFILE;// BVCU_FTP_METHOD_SEARCH_RECORDFILE;
    cmd.OnEvent = ms_On_Ftp_Cmd;
    cmd.iTimeOut = 10000;

    cmd.stMsgContent.iDataCount = 1;

    BVCU_FTP_RecordFileFilter recordFilter;// = new BVCU_FTP_RecordFileFilter();
    memset(&recordFilter, 0, sizeof(recordFilter));
    strcpy_s(recordFilter.szPUID, sizeof(recordFilter.szPUID), "PU_273A");
    //strcpy_s(recordFilter->szPUName, sizeof(recordFilter->szPUName), "CYYHDMDVR2737");
    recordFilter.iChannelIndex = -1;// channelIndex < 0 ? -1 : channelIndex;//如果小于0，则设置为-1，查询所有
    recordFilter.iFileType = 0;
    recordFilter.iTimeBegin = 1407132000000000;// mktime(t_startTime) * 1000000;
    recordFilter.iTimeEnd = 1407218400000000;// mktime(t_endTime) * 1000000;

    cmd.stMsgContent.iDataCount = 1;
    cmd.stMsgContent.pData = &recordFilter;

    BVCU_Result ret = BVCU_FTP_SendCommand(m_FtpSessionHandle, &cmd);
    printf_s("\n\n+++++++++++++BVCU_FTP_SendCommand:%d\n\n", ret);
}*/

