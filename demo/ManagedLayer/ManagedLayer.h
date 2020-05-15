/*====================================================
* ManagedLayer.h
*
*  Created on: 2012-1-4
*      Author: bv-corgan
*  update on: 2013-12-17 20:17
*      content: add annotate�� function SendTspData() (at line 107~108)
*      redactor: l c l
=====================================================*/

#ifndef __MANAGEDLAYER_H___
#define __MANAGEDLAYER_H___

#define LIB_MANAGED_LAYER_EXPORTS
#ifdef _MSC_VER
#ifdef LIB_MANAGED_LAYER_EXPORTS
#define LIB_MANAGED_LAYER_API __declspec(dllexport)
#else
#define LIB_MANAGED_LAYER_API __declspec(dllimport)
#endif
#else
#define LIB_MANAGED_LAYER_API
#endif//_MSC_VER
#include "BVCU/CBVCU.h"
#include "BVDisplay/CBVDisplay.h"

/*
 *  ��C#�Ľӿ�
 */
extern "C"
{
	/*��ʼ����*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuInit(const int** handle);	
	/*ֹͣʹ�ÿ�*/
	LIB_MANAGED_LAYER_API void ManagedLayer_CuRelease(const int* handle);



	/*��¼Server*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuLogin(const int* handle, BVCU_HSession* session, char* serverIp,
		int serverPort, char* usrName, char* usrPsw, int timeOutSec, BVCU_Server_OnEvent onEvent,
		BVCU_Server_Notify onNotify);
	/*�ǳ�*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuLogout(const int* handle, BVCU_HSession session);




	/*�����Ự*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuBrowsePu(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
		char* puId, int channelNo, HWND hWnd, BVDisplay_RECT* dispRect, int volume, int singleRecFileSec,
		char* recFileDir, bool videoTrans, BVCU_DialogControlParam_Network* netWork,
		BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
	/*����new Talk Only �Ự*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuNewTalk(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
		char* puId, int channelNo, int captureVolume, int audioVolume, BVCU_DialogControlParam_Network* netWork,
		BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
	/*����Gps�Ự*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenGpsDialog(const int* handle, BVCU_HDialog* dlg, 
		BVCU_HSession session, char* puId, int channelNo,  BVCU_GpsDialog_OnEvent onDlgEvent, 
		BVCU_GpsDialog_OnData onDlgData);
	/*����tsp�Ự*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenTspDialog(const int* handle, BVCU_HDialog* dlg, 
		BVCU_HSession session, char* puId, int channelNo, BVCU_TspDialog_OnEvent onDlgEvent, 
		BVCU_TspDialog_OnData onDlgData);

	/*�رջỰ*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuCloseDialog(const int* handle, BVCU_HDialog* dlg);




	/*���ĻỰ�ı������ã���������/�洢/�طŵ�, ������Ǹ��Ĳ��ŵ�������С������*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuPlayVolume(const int* handle, BVCU_HDialog dlg, int volume);

	/*���ĻỰ�ı������ã���������/�洢/�طŵ�, ������Ǹ���˵����������С������*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSpeakVolume(const int* handle, BVCU_HDialog dlg, int volume);

	/*���ı������ã�*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuChangeWindow(const int* handle, BVCU_HDialog dlg, HWND hWnd,
		BVDisplay_RECT* dispRect);
	
	/*���ĻỰ�ı�������:¼��Ĵ洢λ�ã�����[��]*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetRecordStorageParam(const int* handle, BVCU_HDialog dlg,
		char* fileDir, int fileSec);

	/*���ı�������*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuResizeDialogWindow(const int* handle, BVCU_HDialog dlg, BVDisplay_RECT* newRect);
	
	/*���ı�������*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuApplyControlParam(const int* handle, BVCU_HDialog dlg,
		int speakVolume, BVCU_DialogControlParam_Network* netWork);






	/*����ץȡ�Ự�н��յ���һ֡��Ƶ����Ϊͼ���ļ���Ŀǰ��֧��JPG��ʽ*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSnapshot(const int* handle, BVCU_HDialog dlg, char* fileDir, int quality);

	/*�����ѽ����ĻỰ����Ҫ��ServerͨѶ*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuAVStop(const int* handle, BVCU_HDialog dlg, bool stopA, bool stopV,
		bool stopSpeak, bool releaseHWND);




	/*���ʹ�������*/
	LIB_MANAGED_LAYER_API int SendTspData(BVCU_HDialog hDialog, SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen);





	/*===========  ��������������/����  ===========*/
	/*L?������̨���ƽ���ص�����*/
	LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuControlResultProcFunc(const int* handle, BVCU_Cmd_ControlResult onCtrlRes);

	LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuQueryResultProcFunc(const int * handle, BVCU_Cmd_QueryResult onQueryRes);

	/*����豸�б�*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuList(const int* handle, BVCU_HSession session, BVCU_Cmd_OnGetPuList onEvent);
	
	/*�����豸��Ϣ*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuDeviceInfo(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_PUCFG_DeviceInfo* cmdData);
	
	/*����豸��Ϣ*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuDeviceInfo(const int* handle, BVCU_HSession hSession, char* puId,
		int device, BVCU_Cmd_OnGetPuDeviceInfo onEvent);
	
	/*�����豸��̨����*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_PUCFG_PTZAttr* cmdData);
	
	/*����豸��̨����*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_Cmd_OnGetPuPtzAttr onEvent);

	/**/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzControl(const int* handle, BVCU_HSession hSession, char* puId,
		int device, BVCU_PUCFG_PTZControl* cmdData);




	/*Display after render:�������ʾ*/
	LIB_MANAGED_LAYER_API void ManagedLayer_DispSetDisplayFontFunc(DisplayFont func);

	/*�ֶ�ֹͣ/����PU¼��*/
	LIB_MANAGED_LAYER_API int ManagedLayer_PuManualRemoteRecord(const int* handle, BVCU_HSession hSession, char *puId, 
		int channelIndex, BVCU_PUCFG_ManualRecord* recordParam);

	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuGpsInfo(const int* handle, BVCU_HSession session, char* puId, int device);
}

#endif /* End of __MANAGEDLAYER_H___ */
