/*====================================================
 * CBVCUSndCmd.cpp
 *
 *  Created on: 2012-3-22
 *      Author: corgan
 ====================================================*/

#include "CBVCUSndCmd.h"

BVCU_Cmd_OnGetPuList CBVCUSndCmd::m_procGetPuList = NULL;

int CBVCUSndCmd::getPuList(BVCU_HSession hSession, BVCU_Cmd_OnGetPuList onEvent)
{
    BVCU_Result result = queryCmd(hSession, NULL, NULL, 0, INVALID_DEVICE, BVCU_SUBMETHOD_PU_LIST);
    if (BVCU_Result_SUCCEEDED(result))
    {
        m_procGetPuList = onEvent;
    }
    return result;
}

BVCU_Cmd_OnGetPuDeviceInfo CBVCUSndCmd::m_procGetPuDeviceInfo = NULL;

int CBVCUSndCmd::setPuDeviceInfo(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_DeviceInfo* cmdData)
{
    return controlCmd(hSession, puId, cmdData, sizeof(BVCU_PUCFG_DeviceInfo), device, BVCU_SUBMETHOD_PU_DEVICEINFO);
}

int CBVCUSndCmd::getPuDeviceInfo(BVCU_HSession hSession, char* puId, int device, BVCU_Cmd_OnGetPuDeviceInfo onEvent)
{
    BVCU_Result result = queryCmd(hSession, puId, NULL, sizeof(BVCU_PUCFG_DeviceInfo), device,
            BVCU_SUBMETHOD_PU_DEVICEINFO);
    if (BVCU_Result_SUCCEEDED(result))
    {
        m_procGetPuDeviceInfo = onEvent;
    }
    return result;
}

BVCU_Cmd_OnGetPuPtzAttr CBVCUSndCmd::m_procGetPuPtzAttr = NULL;

int CBVCUSndCmd::setPuPtzAttr(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_PTZAttr* cmdData)
{
    return controlCmd(hSession, puId, cmdData, sizeof(BVCU_PUCFG_DeviceInfo), device, BVCU_SUBMETHOD_PU_PTZATTR);
}

int CBVCUSndCmd::getPuPtzAttr(BVCU_HSession hSession, char* puId, int device, BVCU_Cmd_OnGetPuPtzAttr onEvent)
{
    BVCU_Result result = queryCmd(hSession, puId, NULL, sizeof(BVCU_PUCFG_DeviceInfo), device,
            BVCU_SUBMETHOD_PU_PTZATTR);
    if (BVCU_Result_SUCCEEDED(result))
    {
        m_procGetPuPtzAttr = onEvent;
    }
    return result;
}

int CBVCUSndCmd::setPuPtzControl(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_PTZControl* cmdData)
{
    return controlCmd(hSession, puId, cmdData, sizeof(BVCU_PUCFG_PTZControl), device, BVCU_SUBMETHOD_PU_PTZCONTROL);
}

int CBVCUSndCmd::setPuManualRemoteRecord(BVCU_HSession hSession, char* puId, int device, BVCU_PUCFG_ManualRecord* cmdData)
{
	return controlCmd(hSession, puId, cmdData, sizeof(BVCU_PUCFG_ManualRecord), device, BVCU_SUBMETHOD_PU_MANUALRECORD);
}

BVCU_Cmd_ControlResult CBVCUSndCmd::m_procControlResult = NULL;
BVCU_Cmd_QueryResult CBVCUSndCmd::m_procQueryResult = NULL;

void CBVCUSndCmd::cmd_OnEvent(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam)
{
    BVCU_Event_SessionCmd* pEvent = (BVCU_Event_SessionCmd*)pParam;
    BVCU_PUChannelInfo* puInfo = NULL;
    BVCU_PUOneChannelInfo* channel = NULL;
    switch (pCommand->iMethod)
    {
    case BVCU_METHOD_QUERY:
		m_procQueryResult(hSession, pCommand->szTargetID, pCommand->iTargetIndex, pCommand->iSubMethod, pEvent);
        if (BVCU_Result_FAILED(pEvent->iResult))
        {
            return;
        }
        switch (pCommand->iSubMethod)
        {
        case BVCU_SUBMETHOD_PU_LIST:
            puInfo = (BVCU_PUChannelInfo*)(pEvent->stContent.pData);
            if (NULL == puInfo)
            {
                return;
            }
            for (int j = 0; j < pEvent->stContent.iDataCount; j++)
            {
                channel = puInfo->pChannel;
                for (int i = 0; i < puInfo->iChannelCount; i++)
                {
                    m_procGetPuList(hSession, puInfo->szPUID, puInfo->szPUName, puInfo->iOnlineStatus, channel, 0);
                    channel++;
                }
                puInfo++;
            }
            puInfo--;
            channel--;
			//if (pEvent->iPercent >= 100)
				m_procGetPuList(hSession, puInfo->szPUID, puInfo->szPUName, puInfo->iOnlineStatus, channel, 1);
            break;
        case BVCU_SUBMETHOD_PU_DEVICEINFO:
			if (0 < pEvent->stContent.iDataCount)
			{
				m_procGetPuDeviceInfo(hSession, (BVCU_PUCFG_DeviceInfo*)pEvent->stContent.pData);
			}
            break;

        case BVCU_SUBMETHOD_PU_PTZATTR:
            m_procGetPuPtzAttr(hSession, pCommand->szTargetID, pCommand->iTargetIndex, (BVCU_PUCFG_PTZAttr*)pEvent->stContent.pData);
            break;

        default:
            break;
        }
        break;
    case BVCU_METHOD_CONTROL:
        m_procControlResult(hSession, pCommand->szTargetID, pCommand->iTargetIndex, pCommand->iSubMethod, pEvent->iResult);
        break;
    default:
        break;
    }
}

void CBVCUSndCmd::setControlResultProcFunc(BVCU_Cmd_ControlResult onCtrlRes)
{
    m_procControlResult = onCtrlRes;
}

void CBVCUSndCmd::setQueryResultProcFunc(BVCU_Cmd_QueryResult onQueryRes)
{
	m_procQueryResult = onQueryRes;
}

BVCU_Result CBVCUSndCmd::queryCmd(BVCU_HSession hSession, char* puId, void* cmdData, int dataLen, int device,
        int subMethod)
{
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(BVCU_Command));
    cmd.iSize = sizeof(BVCU_Command);
    if (NULL != puId)
    {
        strcpy_s(cmd.szTargetID, puId);
    }
    cmd.OnEvent = cmd_OnEvent;
    if (NULL != cmdData)
    {
        memcpy(&cmd.stMsgContent, cmdData, sizeof(BVCU_CmdMsgContent));
    }
    cmd.iTargetIndex = device;
    cmd.iMethod = BVCU_METHOD_QUERY;
    cmd.iSubMethod = subMethod;
    return BVCU_SendCmd(hSession, &cmd);
}

BVCU_Result CBVCUSndCmd::queryCmd(BVCU_HSession hSession, char * puId, int device, void * cmdData, int subMethod, void * usrData)
{
	BVCU_Command cmd;
	memset(&cmd, 0, sizeof(BVCU_Command));
	cmd.iSize = sizeof(BVCU_Command);
	if (NULL != puId)
	{
		strncpy_s(cmd.szTargetID, puId, _TRUNCATE);
	}
	cmd.OnEvent = cmd_OnEvent;
	cmd.iTargetIndex = device;
	if (NULL != cmdData)
	{
		cmd.stMsgContent.iDataCount = 1;
		cmd.stMsgContent.pNext = NULL;
		cmd.stMsgContent.pData = cmdData;
	}
	cmd.iMethod = BVCU_METHOD_QUERY;
	cmd.iSubMethod = subMethod;
	cmd.pUserData = usrData;

	BVCU_Result ret = BVCU_SendCmd(hSession, &cmd);

	return ret;
}

BVCU_Result CBVCUSndCmd::controlCmd(BVCU_HSession hSession, char* puId, void* cmdData, int dataLen, int device,
        int subMethod)
{
    BVCU_Command cmd;
    memset(&cmd, 0, sizeof(BVCU_Command));
    cmd.iSize = sizeof(BVCU_Command);
    if (NULL != puId)
    {
        strcpy_s(cmd.szTargetID, puId);
    }
    cmd.OnEvent = cmd_OnEvent;
    if (NULL != cmdData)
    {
        cmd.stMsgContent.iDataCount = 1;
        cmd.stMsgContent.pNext = NULL;
        cmd.stMsgContent.pData = cmdData;
    }
    cmd.iTargetIndex = device;
    cmd.iMethod = BVCU_METHOD_CONTROL;
    cmd.iSubMethod = subMethod;
    return BVCU_SendCmd(hSession, &cmd);
}

int CBVCUSndCmd::getPuGpsInfo(BVCU_HSession hSession, char* puId, int device)
{
	return queryCmd(hSession, puId, device, NULL, BVCU_SUBMETHOD_PU_GPSDATA, NULL);
}
