/*====================================================
* ManagedLayer.h
*
*  Created on: 2012-1-4
*      Author: bv-corgan
*  update on: 2013-12-17 20:17
*      content: add annotate， function SendTspData() (at line 107~108)
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
 *  给C#的接口
 */
extern "C"
{
	/*初始化库*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuInit(const int** handle);	
	/*停止使用库*/
	LIB_MANAGED_LAYER_API void ManagedLayer_CuRelease(const int* handle);



	/*登录Server*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuLogin(const int* handle, BVCU_HSession* session, char* serverIp,
		int serverPort, char* usrName, char* usrPsw, int timeOutSec, BVCU_Server_OnEvent onEvent,
		BVCU_Server_Notify onNotify);
	/*登出*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuLogout(const int* handle, BVCU_HSession session);




	/*创建会话*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuBrowsePu(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
		char* puId, int channelNo, HWND hWnd, BVDisplay_RECT* dispRect, int volume, int singleRecFileSec,
		char* recFileDir, bool videoTrans, BVCU_DialogControlParam_Network* netWork,
		BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
	/*创建new Talk Only 会话*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuNewTalk(const int* handle, BVCU_HDialog* dlg, BVCU_HSession session,
		char* puId, int channelNo, int captureVolume, int audioVolume, BVCU_DialogControlParam_Network* netWork,
		BVCU_Dialog_OnDialogEvent onDlgEvent, BVCU_Dialog_OnStorageEvent onStoreEvent);
	/*创建Gps会话*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenGpsDialog(const int* handle, BVCU_HDialog* dlg, 
		BVCU_HSession session, char* puId, int channelNo,  BVCU_GpsDialog_OnEvent onDlgEvent, 
		BVCU_GpsDialog_OnData onDlgData);
	/*创建tsp会话*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuOpenTspDialog(const int* handle, BVCU_HDialog* dlg, 
		BVCU_HSession session, char* puId, int channelNo, BVCU_TspDialog_OnEvent onDlgEvent, 
		BVCU_TspDialog_OnData onDlgData);

	/*关闭会话*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuCloseDialog(const int* handle, BVCU_HDialog* dlg);




	/*更改会话的本地设置，包括接收/存储/回放等, （这个是更改播放的声音大小？？）*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuPlayVolume(const int* handle, BVCU_HDialog dlg, int volume);

	/*更改会话的本地设置，包括接收/存储/回放等, （这个是更改说话的声音大小？？）*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSpeakVolume(const int* handle, BVCU_HDialog dlg, int volume);

	/*更改本地设置：*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuChangeWindow(const int* handle, BVCU_HDialog dlg, HWND hWnd,
		BVDisplay_RECT* dispRect);
	
	/*更改会话的本地设置:录像的存储位置，长度[秒]*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetRecordStorageParam(const int* handle, BVCU_HDialog dlg,
		char* fileDir, int fileSec);

	/*更改本地设置*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuResizeDialogWindow(const int* handle, BVCU_HDialog dlg, BVDisplay_RECT* newRect);
	
	/*更改本地设置*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuApplyControlParam(const int* handle, BVCU_HDialog dlg,
		int speakVolume, BVCU_DialogControlParam_Network* netWork);






	/*本地抓取会话中接收到的一帧视频，存为图像文件。目前仅支持JPG格式*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSnapshot(const int* handle, BVCU_HDialog dlg, char* fileDir, int quality);

	/*更改已建立的会话，需要与Server通讯*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuAVStop(const int* handle, BVCU_HDialog dlg, bool stopA, bool stopV,
		bool stopSpeak, bool releaseHWND);




	/*发送串口数据*/
	LIB_MANAGED_LAYER_API int SendTspData(BVCU_HDialog hDialog, SAV_TYPE_UINT8 * SavData, SAV_TYPE_INT32 iSavDataLen);





	/*===========  发送命令，完成命令/功能  ===========*/
	/*L?设置云台控制结果回调函数*/
	LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuControlResultProcFunc(const int* handle, BVCU_Cmd_ControlResult onCtrlRes);

	LIB_MANAGED_LAYER_API void ManagedLayer_CuSetPuQueryResultProcFunc(const int * handle, BVCU_Cmd_QueryResult onQueryRes);

	/*获得设备列表*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuList(const int* handle, BVCU_HSession session, BVCU_Cmd_OnGetPuList onEvent);
	
	/*设置设备信息*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuDeviceInfo(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_PUCFG_DeviceInfo* cmdData);
	
	/*获得设备信息*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuDeviceInfo(const int* handle, BVCU_HSession hSession, char* puId,
		int device, BVCU_Cmd_OnGetPuDeviceInfo onEvent);
	
	/*设置设备云台属性*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_PUCFG_PTZAttr* cmdData);
	
	/*获得设备云台属性*/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuPtzAttr(const int* handle, BVCU_HSession session, char* puId,
		int device, BVCU_Cmd_OnGetPuPtzAttr onEvent);

	/**/
	LIB_MANAGED_LAYER_API int ManagedLayer_CuSetPuPtzControl(const int* handle, BVCU_HSession hSession, char* puId,
		int device, BVCU_PUCFG_PTZControl* cmdData);




	/*Display after render:解码后显示*/
	LIB_MANAGED_LAYER_API void ManagedLayer_DispSetDisplayFontFunc(DisplayFont func);

	/*手动停止/启动PU录像*/
	LIB_MANAGED_LAYER_API int ManagedLayer_PuManualRemoteRecord(const int* handle, BVCU_HSession hSession, char *puId, 
		int channelIndex, BVCU_PUCFG_ManualRecord* recordParam);

	LIB_MANAGED_LAYER_API int ManagedLayer_CuGetPuGpsInfo(const int* handle, BVCU_HSession session, char* puId, int device);
}

#endif /* End of __MANAGEDLAYER_H___ */
