/*====================================================
 * CBVCUSndCmd.h
 *
 *  Created on: 2012-3-22
 *      Author: corgan
 ====================================================*/

#ifndef __CBVCUSNDCMD_H__
#define __CBVCUSNDCMD_H__

#include "Windows.h"
extern "C"
{
#include "BVCU.h"
#include "BVCUConst.h"
#include "PUConfig.h"
}

typedef void (WINAPI *BVCU_Cmd_ControlResult)(BVCU_HSession hSession, char* puId, int device, int subMethod, int result);
typedef void (WINAPI *BVCU_Cmd_QueryResult)(BVCU_HSession hSession, BVCU_Command* pCommand, BVCU_Event_SessionCmd* pEvent);
typedef void (WINAPI *BVCU_Cmd_OnGetPuList)(BVCU_HSession hSession, char* puId, char* puName, int status, BVCU_PUOneChannelInfo* channel, int finished);
typedef void (WINAPI *BVCU_Cmd_OnGetPuDeviceInfo)(BVCU_HSession hSession, BVCU_PUCFG_DeviceInfo* cmdData);
typedef void (WINAPI *BVCU_Cmd_OnGetPuPtzAttr)(BVCU_HSession hSession, char* puId, int ptzIndex, BVCU_PUCFG_PTZAttr* cmdData);

#define INVALID_DEVICE -1

class CBVCUSndCmd
{
    static BVCU_Cmd_ControlResult m_procControlResult;
	static BVCU_Cmd_QueryResult m_procQueryResult;
    static BVCU_Cmd_OnGetPuList m_procGetPuList;
    static BVCU_Cmd_OnGetPuDeviceInfo m_procGetPuDeviceInfo;
    static BVCU_Cmd_OnGetPuPtzAttr m_procGetPuPtzAttr;

    static void cmd_OnEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam);

    BVCU_Result queryCmd(BVCU_HSession hSession, char* puId, void* cmdData, int dataLen, int device, int subMethod);
	BVCU_Result queryCmd(BVCU_HSession hSession, char* puId, int device, void* cmdData, int subMethod, void* usrData);
    BVCU_Result controlCmd(BVCU_HSession hSession, char* puId, void* cmdData, int dataLen, int device, int subMethod);

public:
    void setControlResultProcFunc(BVCU_Cmd_ControlResult onCtrlRes);
	void setQueryResultProcFunc(BVCU_Cmd_QueryResult onQueryRes);
    int getPuList(BVCU_HSession hSession, BVCU_Cmd_OnGetPuList onEvent);
    int setPuDeviceInfo(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_DeviceInfo* cmdData);
    int getPuDeviceInfo(BVCU_HSession hSession, char* puId, int device, BVCU_Cmd_OnGetPuDeviceInfo onEvent);
    int setPuPtzAttr(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_PTZAttr* cmdData);
    int getPuPtzAttr(BVCU_HSession hSession, char* puId, int device, BVCU_Cmd_OnGetPuPtzAttr onEvent);
    int setPuPtzControl(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_PTZControl* cmdData);
	int setPuManualRemoteRecord(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_ManualRecord* cmdData);

	int getPuGpsInfo(BVCU_HSession hSession, char* puId, int device);
	int getPuGpsInfoV2(BVCU_HSession hSession, char* puId, int device,void* userData);
};


#endif /* __CBVCUSNDCMD_H__ */
