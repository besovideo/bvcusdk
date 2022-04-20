#include "MDataList.h"
using namespace std;

CMDataList::CMDataList(void)
{
    InitializeCriticalSection(&m_puLock);
    InitializeCriticalSection(&m_dialogLock);
}


CMDataList::~CMDataList(void)
{
    ClearPu();
    ClearDialog();
    DeleteCriticalSection(&m_puLock);
    DeleteCriticalSection(&m_dialogLock);
}

int CMDataList::GetPu(DWORD_PTR Index,MPUChannelInfo* pPuChannelInfo)
{
    if ( Index )
    {
        EnterCriticalSection(&m_puLock);
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            if ((DWORD_PTR)*iPu == Index)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            MPUChannelInfo* onePu = *iPu;
            if (pPuChannelInfo)
                *pPuChannelInfo = *onePu;
            LeaveCriticalSection(&m_puLock);
            return BVCU_RESULT_S_OK;
        }
        LeaveCriticalSection(&m_puLock);
        return BVCU_RESULT_E_NOTFOUND;
    }
    return BVCU_RESULT_E_FAILED;
}
int    CMDataList::GetPu(const char* puId,MPUChannelInfo* pPuChannelInfo)
{
    if ( puId )
    {
        EnterCriticalSection(&m_puLock);
        MPUChannelInfo* onePu;
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            onePu = *iPu;
            if (strcmp(onePu->szPUID,puId)== 0)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            if (pPuChannelInfo)
                *pPuChannelInfo = *onePu;
            LeaveCriticalSection(&m_puLock);
            return BVCU_RESULT_S_OK;
        }
        LeaveCriticalSection(&m_puLock);
        return BVCU_RESULT_E_NOTFOUND;
    }
    return BVCU_RESULT_E_FAILED;
}
DWORD_PTR CMDataList::GetPuIndex(const char* puId)
{
    if ( puId )
    {
        EnterCriticalSection(&m_puLock);
        MPUChannelInfo* onePu;
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            onePu = *iPu;
            if (strcmp(onePu->szPUID,puId)== 0)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            LeaveCriticalSection(&m_puLock);
            return (DWORD_PTR)onePu;
        }
        LeaveCriticalSection(&m_puLock);
    }
    return 0;
}
DWORD_PTR CMDataList::InsertPu(char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo)
{
    if ( puId )
    {
        if (BVCU_Result_FAILED(GetPu(puId,NULL)))
        {
            int i = 0;
            MPUChannelInfo* onePu = new MPUChannelInfo;
            if (!onePu)
                return 0;
            EnterCriticalSection(&m_puLock);
            strcpy_s(onePu->szPUID,BVCU_MAX_ID_LEN+1,puId);
            if (puName)
                strcpy_s(onePu->szPUName,BVCU_MAX_NAME_LEN+1,puName);
            else
                onePu->szPUName[0] = '\0';
            onePu->iOnlineStatus = status;
            if (channelNo > PU_CHANNEL_NUM_MAX) channelNo = PU_CHANNEL_NUM_MAX;
            onePu->iChannelCount = channelNo;
            for (i = 0; i < channelNo; ++ i)
            {
                onePu->pChannel[i] = channel[i];
            }

            m_puList.push_back(onePu);
            LeaveCriticalSection(&m_puLock);
            return (DWORD_PTR)onePu;
        }
        else
            return 0;
    }
    return 0;
}
int    CMDataList::SetPuStatus(const char* puId, int status)
{
    if ( puId )
    {
        EnterCriticalSection(&m_puLock);
        MPUChannelInfo* onePu;
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            onePu = *iPu;
            if (strcmp(onePu->szPUID,puId)== 0)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            onePu->iOnlineStatus = status;
            LeaveCriticalSection(&m_puLock);
            return BVCU_RESULT_S_OK;
        }
        LeaveCriticalSection(&m_puLock);
        return BVCU_RESULT_E_NOTFOUND;
    }
    return BVCU_RESULT_E_FAILED;
}
int    CMDataList::DeletePu(const char* puId)
{
    if ( puId )
    {
        EnterCriticalSection(&m_puLock);
        MPUChannelInfo* onePu;
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            onePu = *iPu;
            if (strcmp(onePu->szPUID,puId)== 0)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            m_puList.erase(iPu);
            delete onePu;
        }
        LeaveCriticalSection(&m_puLock);
        return BVCU_RESULT_S_OK;
    }
    return BVCU_RESULT_E_FAILED;
}
int    CMDataList::DeletePu(int Index)
{
    if ( Index )
    {
        EnterCriticalSection(&m_puLock);
        list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
        while(iPu != m_puList.end())
        {
            if ((int)*iPu == Index)
                break;
            ++ iPu;
        }
        if (iPu != m_puList.end())
        { // find 
            delete *iPu;
            m_puList.erase(iPu);
        }
        LeaveCriticalSection(&m_puLock);
        return BVCU_RESULT_S_OK;
    }
    return BVCU_RESULT_E_FAILED;
}
int    CMDataList::ClearPu()
{
    EnterCriticalSection(&m_puLock);
    MPUChannelInfo* onePu;
    list<MPUChannelInfo*>::iterator iPu = m_puList.begin();
    while(iPu != m_puList.end())
    {
        onePu = *iPu;
        delete onePu;
        ++ iPu;
    }
    m_puList.clear();
    LeaveCriticalSection(&m_puLock);
    return BVCU_RESULT_S_OK;
}

int    CMDataList::GetDialogIndex(BVCU_HDialog pDialog)
{
    if (pDialog)
    {
        EnterCriticalSection(&m_dialogLock);
        list<BVCU_HDialog>::iterator iDlg = m_dialogList.begin();
        while (iDlg != m_dialogList.end())
        {
            if (pDialog == *iDlg)
            {
                LeaveCriticalSection(&m_dialogLock);
                return (int)pDialog;
            }
            ++ iDlg;
        }
        LeaveCriticalSection(&m_dialogLock);
    }
    return 0;
}
int    CMDataList::InsertDialog(BVCU_HDialog pDialog)
{
    if (GetDialogIndex(pDialog))
        return 0;
    EnterCriticalSection(&m_dialogLock);
    m_dialogList.push_back(pDialog);
    LeaveCriticalSection(&m_dialogLock);
    return (int)pDialog;
}
int    CMDataList::DeleteDialog(BVCU_HDialog pDialog)
{
    if (pDialog)
    {
        EnterCriticalSection(&m_dialogLock);
        list<BVCU_HDialog>::iterator iDlg = m_dialogList.begin();
        while (iDlg != m_dialogList.end())
        {
            if (pDialog == *iDlg)
            {
                m_dialogList.erase(iDlg);
                LeaveCriticalSection(&m_dialogLock);
                return BVCU_RESULT_S_OK;
            }
            ++ iDlg;
        }
        LeaveCriticalSection(&m_dialogLock);
        return BVCU_RESULT_E_NOTFOUND;
    }
    return BVCU_RESULT_E_FAILED;
}