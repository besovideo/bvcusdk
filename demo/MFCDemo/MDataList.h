#pragma once
#include "libBVCU/MLibBVCU.h"
#include <list>

#define PU_CHANNEL_NUM_MAX 32

typedef struct MBVCU_ChannelInfo 
{
	/*PU ID*/
	char  szPUID[BVCU_MAX_ID_LEN+1];

	/*PU Name*/
	char  szPUName[BVCU_MAX_NAME_LEN+1];

	/*PU通道数*/
	int   iChannelCount;

	/*PU通道信息数组*/
	BVCU_PUOneChannelInfo pChannel[PU_CHANNEL_NUM_MAX];

	/*在线状态，参见BVCU_ONLINE_STATUS_* */
	int   iOnlineStatus;
}MPUChannelInfo;

class CMDataList
{
private:
	CRITICAL_SECTION m_puLock;
	CRITICAL_SECTION m_dialogLock;

	std::list<MPUChannelInfo*> m_puList;
    std::list<BVCU_HDialog> m_dialogList;

public:
	CMDataList(void);
	~CMDataList(void);

	/* return BVCU_RESULT_* */
	int    GetPu(DWORD_PTR Index,MPUChannelInfo* pPuChannelInfo);
	/* return BVCU_RESULT_* */
	int    GetPu(const char* puId,MPUChannelInfo* pPuChannelInfo);
	/* return Index or 0:not found */
	DWORD_PTR GetPuIndex(const char* puId);
	/* return Index (0:failed) */
	DWORD_PTR InsertPu(char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int channelNo);
	/* return BVCU_RESULT_* */
	int    SetPuStatus(const char* puId, int status);
	/* return BVCU_RESULT_* */
	int    DeletePu(const char* puId);
	/* return BVCU_RESULT_* */
	int    DeletePu(int Index);
	/* return BVCU_RESULT_* */
	int    ClearPu();

	/* return Index (0:failed) */
	int    GetDialogIndex(BVCU_HDialog pDialog);
	/* return Index (0:failed) */
	int    InsertDialog(BVCU_HDialog pDialog);
	/* return BVCU_RESULT_* */
	int    DeleteDialog(BVCU_HDialog pDialog);
	void    ClearDialog()
	{
		EnterCriticalSection(&m_dialogLock);
		m_dialogList.clear();
		LeaveCriticalSection(&m_dialogLock);
	}

    std::list<MPUChannelInfo*>* GetPuList() { return &m_puList; }
};

