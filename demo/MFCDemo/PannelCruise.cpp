// PannelCruise.cpp : implementation file
//

#include "stdafx.h"
#include "libBVCU//MLibBVCU.h"
#include "MFCDemo.h"
#include "PannelPTZ.h"
#include "PannelCruise.h"
#include "afxdialogex.h"


// CPannelCruise dialog

IMPLEMENT_DYNAMIC(CPannelCruise, CDialogEx)

CPannelCruise::CPannelCruise(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPannelCruise::IDD, pParent)
{
    m_brother = NULL;
//    m_bCruise_go = FALSE;
    m_cruise_now = -1;
}

CPannelCruise::~CPannelCruise()
{
}

void CPannelCruise::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PRESET_LIST, m_PresetList);
    DDX_Control(pDX, IDC_CRUISE_LIST, m_cruiseList);
}


BEGIN_MESSAGE_MAP(CPannelCruise, CDialogEx)
    ON_BN_CLICKED(IDC_PRESET_GO, &CPannelCruise::OnBnClickedPresetGo)
    ON_BN_CLICKED(IDC_CRUISE_GO, &CPannelCruise::OnBnClickedCruiseGo)
ON_CBN_SELCHANGE(IDC_CRUISE_LIST, &CPannelCruise::OnSelchangeCruiseList)
END_MESSAGE_MAP()


// CPannelCruise message handlers
void  CPannelCruise::ClearStatus()
{
//    m_bCruise_go = FALSE;
    m_cruise_now = -1;
    m_cruiseList.Clear();
    m_cruiseList.ResetContent();
    m_PresetList.Clear();
    m_PresetList.ResetContent();
    GetDlgItem(IDC_CRUISE_GO)->SetWindowText("¿ªÊ¼Ñ²º½");
}
void  CPannelCruise::SetBrother(CPannelPTZ* brother)
{
    m_brother = brother;
}
void  CPannelCruise::SetPtzAttr(BVCU_PUCFG_PTZAttr* ptzAttr)
{
    ClearStatus();
    if (ptzAttr)
    {
        char tempStr[128];
        BVCU_PUCFG_Preset* pPreset = ptzAttr->stPreset;
        for (int i = 0; i < BVCU_PTZ_MAX_PRESET_COUNT; ++i)
        {
            if (pPreset[i].iID >= 0)
            {
                utf8ToAnsi(tempStr,128,pPreset[i].szPreset);
                int iItem = m_PresetList.AddString(tempStr);
                m_PresetList.SetItemData(iItem, pPreset[i].iID);
            }
            else
                break;
        }
        BVCU_PUCFG_Cruise* pCruise = ptzAttr->stCruise;
        for (int i = 0; i < BVCU_PTZ_MAX_CRUISE_COUNT; ++i)
        {
            if (pCruise[i].iID >= 0)
            {
                utf8ToAnsi(tempStr,128, pCruise[i].szName);
                int iItem = m_cruiseList.AddString(tempStr);
                m_cruiseList.SetItemData(iItem, pCruise[i].iID);

                if (ptzAttr->iActiveCruiseID == pCruise[i].iID)
                { // ÉèÖÃµ±Ç°Ö´ÐÐµÄÑ²º½Â·Ïß×´Ì¬
                    m_cruiseList.SetCurSel(iItem);
//                    m_bCruise_go = TRUE;
                    m_cruise_now = pCruise[i].iID;
                    GetDlgItem(IDC_CRUISE_GO)->SetWindowText("Í£Ö¹Ñ²º½");
                }
            }
            else
                break;
        }
    }
}

void CPannelCruise::OnBnClickedPresetGo()
{
    // TODO: Add your control notification handler code here
    int iItem = m_PresetList.GetCurSel();
    if (iItem >= 0)
    {
        int iID = m_PresetList.GetItemData(iItem);
        m_brother->SendPtzCmd(BVCU_PTZ_COMMAND_PRESET_GO,0,iID,8,8);
    }
}

void CPannelCruise::OnBnClickedCruiseGo()
{
    // TODO: Add your control notification handler code here
    int iItem = m_cruiseList.GetCurSel();
    if (iItem >= 0)
    {
        int iID = m_cruiseList.GetItemData(iItem);
        if (iID == m_cruise_now)
        {
            m_brother->SendPtzCmd(BVCU_PTZ_COMMAND_CRUISE_STOP,0,iID,0,0);
            GetDlgItem(IDC_CRUISE_GO)->SetWindowText("¿ªÊ¼Ñ²º½");
            m_cruise_now = -1;
        }
        else
        {
            m_brother->SendPtzCmd(BVCU_PTZ_COMMAND_CRUISE_GO,0,iID,0,0);
            GetDlgItem(IDC_CRUISE_GO)->SetWindowText("Í£Ö¹Ñ²º½");
            m_cruise_now = iID;
        }
    }
}

void CPannelCruise::OnSelchangeCruiseList()
{
    // TODO: Add your control notification handler code here
    int iItem = m_cruiseList.GetCurSel();
    if (iItem >= 0)
    {
        int iID = m_cruiseList.GetItemData(iItem);
        if (iID == m_cruise_now)
        {
            GetDlgItem(IDC_CRUISE_GO)->SetWindowText("Í£Ö¹Ñ²º½");
        }
        else
        {
            GetDlgItem(IDC_CRUISE_GO)->SetWindowText("¿ªÊ¼Ñ²º½");
        }
    }
}
