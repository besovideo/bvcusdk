/*======================================================
    本头文件内所有接口deprecated 。
========================================================*/
extern "C" {
#ifndef __RECORDPLAY_H__
#define __RECORDPLAY_H__

/*================================================================*/
#include "SAVCodec.h"
#include "BVCUConst.h"
#include "BVEvent.h"
#include "PUConfig.h"
#include "BVCU.h"

//==========================FTP Client相关接口===============================
typedef void* BVCU_FTP_HSession;
typedef void* BVCU_FTP_HTransfer;

/*
登录服务器信息。可以使用FTP Server ID或者IP/Port登录，如果BVCU_FTP_ServerParam中设置了ID，则忽略IP/Port
*/
typedef struct _BVCU_FTP_ServerParam {
    int iSize;// 本结构体的大小，分配者应初始化为sizeof(BVCU_FTP_ServerParam)
    BVCU_HSession hSession;/* 登录CMS的Session句柄。如果设置为0，表示使用szServerAddr/iServerPort直接连接FTP Server，
                            如果非0，表示使用szID连接FTP Server **/
    char szID[BVCU_MAX_ID_LEN + 1];//FTP Server ID, "VTDU_"(或"NRU_")表示由服务器分配VTDU（或NRU），登录成功后此处为实际登录的FTP ID

    char szServerAddr[BVCU_MAX_HOST_NAME_LEN + 1];//FTP Server地址，域名或者IP
    int  iServerPort;    //FTP Server端口号        
    char szUserName[BVCU_MAX_NAME_LEN + 1];//登录用户名，目前没有使用，必须设置为空
    char szPassword[BVCU_MAX_PASSWORD_LEN + 1];//登录密码，目前没有使用，必须设置为空

    int iTimeOut;//超过iTimeOut未收到回响则认为失败，单位毫秒。必须>0
    int iKeepAliveInterval;//发送KeepAlive包的间隔，单位毫秒。设置<=0表示不发送
                            /* 事件。
                            iEventCode:事件码，参见Session事件BVCU_EVENT_SESSION_*
                            pParam: 每个事件对应的参数，具体类型参考各个事件码的说明 **/
    void(*OnEvent)(BVCU_FTP_HSession hSession, int iEventCode, void* pParam);

    int iReserved[2];//保留，必须为0
}BVCU_FTP_ServerParam;


/*
FTP_Session信息。一次登录创建一个FTP Session
*/
enum {
    BVCU_FTP_SERVER_CAP_RESUMEBREAKTRANSFER = (1 << 0),//断点续传
    BVCU_FTP_SERVER_CAP_UPLOAD = (1 << 1),//允许上传
    BVCU_FTP_SERVER_CAP_OVERWRITE = (1 << 2),//上传允许覆盖旧文件
};
typedef struct _BVCU_FTP_SessionInfo {
    BVCU_FTP_ServerParam stParam;//创建该Session的Param
    unsigned int bCapability;//支持的功能。是BVCU_FTP_SERVER_CAP_*的组合
    unsigned int iMaxSession;//该Server支持的并发Session最大数目。0表示无限制
    unsigned int iMaxTransfer;//该Server支持的并发传输最大数目。0表示无限制
    unsigned int iMaxTransferPerSession;//每个Session支持的并发传输最大数目。0表示无限制
    unsigned int iBandwidthLimit;//带宽限制。单位Kbytes，0表示无限制
    int iTimeIdle;//允许的Idle时间长度，超过该长度且没有收到Client的任何命令则server主动断开连接。单位秒。-1表示无限制

    int iReserved[4]; // 保留，必须初始化为0
}BVCU_FTP_SessionInfo;

//传输参数
enum {
    BVCU_FTP_RULE_SAMEFILENAME_OVERWRITE = 0,
    BVCU_FTP_RULE_SAMEFILENAME_SKIP,
    BVCU_FTP_RULE_SAMEFILENAME_RESUME,//如果已有的同名文件长度较小则续传
};

enum {
    BVCU_FTP_TRANSFER_EVENT_CLOSE = 0,   // Transfer 关闭。事件参数：BVCU_Event_Common
    BVCU_FTP_TRANSFER_EVENT_PROGRESS = 1,// Transfer 进度。事件参数：整形数组 int[2]，分别为已传输KB、总KB
};

typedef struct _BVCU_FTP_TransferParam {
    int iSize;//本结构体的大小，分配者应初始化为sizeof(BVCU_FTP_TransferParam)
    BVCU_FTP_HSession hSession;//BVCU_FTP_Login返回的句柄
    char szRemoteFileName[BVCU_MAX_FILE_NAME_LEN + 1];//远端文件名
    char* szLocalFileName;//本地文件。如果OnData设置!=NULL，则忽略该值，否则库内部把数据保存到szLocalFileName命名的本地文件中
    int iRuleSameFileName;//遇到同名文件的处理规则，BVCU_FTP_RULE_SAMEFILENAME_*
    int iTimeout;//连接超时
    int bUpload;//0-下载，1-上传
    void* pUserData;//自定义数据

                    /*回调函数。
                    hTransfer:传输句柄，
                    pUserData：本结构体中的pUserData
                    pBuffer：库内部缓冲区。
                    upload：应用程序应在pBuffer写入不超过iSizeBytes字节的数据。
                    download：应用程序应把pBuffer中的数据保存，长度iSizeBytes
                    iSizeBytes:
                    iSizeBytes==0,则表示下载完毕
                    iSizeBytes > 0，对upload表示应用程序需提供的数据长度，对download表示应用程序需消费的数据长度

                    返回值：upload：实际写入pBuffer的数据长度，如果返回值<iSizeBytes，则认为是最后一个上传数据包，停止传输
                    download：实际读取pBuffer的数据长度，如果返回值<iSizeBytes，则认为出错，停止传输
                    */
    int(*OnData)(BVCU_FTP_HTransfer hTransfer, void* pUserData, void* pBuffer, int iSizeBytes);
    /* iEventCode: 事件码，参见BVCU_FTP_TRANSFER_EVENT_*
    * pParam: 每个事件对应的参数，具体类型参考各个事件码的说明
    */
    void(*OnEvent)(BVCU_FTP_HTransfer hTransfer, int iEventCode, void* pParam, void* pUserData);
    int iReserved[2];
}BVCU_FTP_TransferParam;

//传输控制参数
typedef struct _BVCU_FTP_TransferControlParam {
    int iSize;//本结构体的大小，分配者应初始化为sizeof(BVCU_FTP_TransferControlParam)
    int iBufferSize;//库内部使用的传输缓冲区大小，单位byte。建议不小于16K
    int iBufferCount;//库内部使用的传输缓冲区个数，必须>=2
    unsigned int iBandwidthLimit;//带宽限制。单位Kbytes，0表示无限制
    int iResumeLimit;//断线重连次数限制，超过设定次数停止传输。-1表示无限制。
    int iReserved[2];
}BVCU_FTP_TransferControlParam;

/*
传输信息
*/
typedef struct _BVCU_FTP_TransferInfo {
    //传输参数
    BVCU_FTP_TransferParam stParam;
    BVCU_FTP_TransferControlParam stControlParam;

    //Transfer开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iCreateTime;

    //Transfer持续时间，单位微秒
    SAV_TYPE_INT64 iOnlineTime;

    //已经收到的字节数
    int iReceivedKb;
    //总字节数
    int iTotalKb;

    int iSpeedKBpsLongTerm; //长时间传输速率，单位 KBytes/second
    int iSpeedKBpsShortTerm;//短时间传输速率，单位 KBytes/second

    int iResumeCount;//已进行的断线重连次数
    int iReseved;
}BVCU_FTP_TransferInfo;

//FTP命令
enum {
    BVCU_FTP_METHOD_SEARCH_RECORDFILE = 1,//查找录像文件。输入类型:BVCU_FTP_RecordFileFilter; 输出类型:BVCU_FTP_RecordFileInfo
    BVCU_FTP_METHOD_SEARCH_FILE,//查找普通文件。输入类型:BVCU_FTP_FileFilter; 输出类型:BVCU_FTP_FileInfo
    BVCU_FTP_METHOD_DEL_RECORDFILE,//删除录像文件。输入类型:BVCU_FTP_RecordFileFilter; 输出类型:无
    BVCU_FTP_METHOD_DEL_FILE,//删除普通文件或目录。输入类型:BVCU_FTP_FileFilter; 输出类型:无
    BVCU_FTP_METHOD_CD,//切换目录。输入类型:变长字符数组; 输出类型:无
    BVCU_FTP_METHOD_PWD,//获取当前目录。输入类型:无; 输出类型:变长字符数组
    BVCU_FTP_METHOD_MKD,//创建目录。输入类型:变长字符数组; 输出类型:无
};

struct RemoteFileListData
{
    char* data;
    int length;
    RemoteFileListData* next;
};
typedef struct _BVCU_FTP_Command BVCU_FTP_Command;
struct _BVCU_FTP_Command {
    int iSize;//本结构体的大小，分配者应初始化为sizeof(BVCU_Command)

    //用户自定义数据。通常用于回调通知。应用程序/库可以用该成员来区分不同的命令
    void* pUserData;

    //命令类型，BVCU_FTP_METHOD_*，决定了BVCU_CmdMsgContent.pData类型 
    int iMethod;

    //命令超过iTimeOut未收到回响则认为失败，单位毫秒。如果设置为0，则采用BVCU_ServerParam.iTimeout
    int iTimeOut;

    //命令负载
    BVCU_CmdMsgContent stMsgContent;

    RemoteFileListData* pFileList;
    /* 事件。
    pCommand:本命令指针。注意该指针指向的是SDK内部维护的一个BVCU_FTP_Command浅拷贝
    iEventCode:事件码，参见BVCU_EVENT_SESSION_CMD_*。目前iEventCode总是等于BVCU_EVENT_SESSION_CMD_COMPLETE
    pParam: 每个事件对应的参数，具体类型参考各个事件码的说明
    */
    void(*OnEvent)(BVCU_FTP_HSession hSession, BVCU_FTP_Command* pCommand, int iEventCode, void* pParam);

    //保留
    int iReserved[1];
};

enum {
    BVCU_FTP_FILTER_INDEX_PUID = (1 << 0),
    BVCU_FTP_FILTER_INDEX_PUNAME = (1 << 1),
    BVCU_FTP_FILTER_INDEX_CHANNELINDEX = (1 << 2),
    BVCU_FTP_FILTER_INDEX_CHANNELNAME = (1 << 3),
    BVCU_FTP_FILTER_INDEX_RECORDTYPE = (1 << 4),
    BVCU_FTP_FILTER_INDEX_TIMEBEGIN = (1 << 5),
    BVCU_FTP_FILTER_INDEX_TIMEEND = (1 << 6),
    BVCU_FTP_FILTER_INDEX_FILESIZEMIN = (1 << 7),
    BVCU_FTP_FILTER_INDEX_FILESIZEMAX = (1 << 8),
    BVCU_FTP_FILTER_INDEX_PATTERN = (1 << 9),
    BVCU_FTP_FILTER_INDEX_PATTERNTYPE = (1 << 10),
};
//录像文件查找
typedef struct _BVCU_FTP_RecordFileFilter {
    int iValidIndex;//BVCU_FTP_FILTER_INDEX_的组合。表示对应的有效成员
    int iReserved1;
    char szPUID[BVCU_MAX_ID_LEN + 1];//PU ID
    char szPUName[BVCU_MAX_NAME_LEN + 1];//PU Name
    int  iChannelIndex;//Channel号
    int iReserved2;
    char szChannelName[BVCU_MAX_NAME_LEN + 1];//Channel Name    
    SAV_TYPE_INT64 iTimeBegin; //录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iTimeEnd;   //录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iFileSizeMin; //文件大小的下限
    SAV_TYPE_INT64 iFileSizeMax; //文件大小的上限
    int  iRecordType;//录像原因，BVCU_STORAGE_RECORDTYPE_*
    int iFileType;// 文件类型, BVCU_STORAGE_FILE_TYPE_*
}BVCU_FTP_RecordFileFilter;

//录像文件信息
typedef struct _BVCU_FTP_RecordFileInfo {
    char szFilePath[BVCU_MAX_FILE_NAME_LEN + 1];//文件路径
    char szFileName[BVCU_MAX_FILE_NAME_LEN + 1];//文件名
    char szPUID[BVCU_MAX_ID_LEN + 1];//PU ID
    char szPUName[BVCU_MAX_NAME_LEN + 1];//PU Name
    int  iChannelIndex;//Channel号
    int  iReserved1;
    char szChannelName[BVCU_MAX_NAME_LEN + 1];//Channel Name
    int  iRecordType;//录像原因，BVCU_STORAGE_RECORDTYPE_*    
    int iFileType;   // BVCU_STORAGE_FILE_TYPE_*
    SAV_TYPE_INT64 iTimeBegin; //录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iTimeEnd;   //录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iFileSize; //文件大小，单位字节
    BVCU_ImageSize stImageSize; // 录像的图像分辨率 单位：像素。 保留
    int iFPS; // 录像文件采样频率；单位：1/1000帧。 保留
    int iReserved2;//保留，必须为0
}BVCU_FTP_RecordFileInfo;

//普通文件查找
enum {
    BVCU_FTP_PATTERNTYPE_FILE = (1 << 0),//文件
    BVCU_FTP_PATTERNTYPE_DIRECTORY = (1 << 1),//目录
};
/*
* 查找文件时通过iPatternType指定类型，可以同时查找符合条件的文件和目录，如果不设置
* 过滤条件，则返回当前目录下所有文件和目录列表
*/
typedef struct _BVCU_FTP_FileFilter {
    int iValidIndex;//BVCU_FTP_FILTER_INDEX_的组合。表示对应的有效成员
    char* szPattern;//文件名模板
    int iPatternType;//文件类型
    SAV_TYPE_INT64 iTimeBegin; //录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iTimeEnd;   //录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iFileSizeMin; //文件大小的下限
    SAV_TYPE_INT64 iFileSizeMax; //文件大小的上限
    int iReserved[2];//保留，必须为0
}BVCU_FTP_FileFilter;

//普通文件信息
typedef struct _BVCU_FTP_FileInfo {
    char szFilePath[BVCU_MAX_FILE_NAME_LEN + 1];//文件路径
    char szFileName[BVCU_MAX_FILE_NAME_LEN + 1];//文件名    
    SAV_TYPE_INT64 iTime; //文件修改时间，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iFileSize; //文件大小，单位字节
    int iReserved[2];//保留，必须为0
}BVCU_FTP_FileInfo;

/*
注意：登录完成后，应用程序随时可能收到BVCU_EVENT_SESSION_CLOSE事件回调，回调之后，Session被SDK摧毁，BVCU_FTP_HSession变成无效值
*/

/**
*登录Server。该函数是异步的。如果登录成功，在返回前或者返回后会产生OnEvent回调。
* @param[out] phSession: 返回登录Session
* @param[in] pParam: Server信息
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FTP_Login(BVCU_FTP_HSession* phSession, BVCU_FTP_ServerParam* pParam);

/**
*获得登录Session相关信息
*@param[in] hSession: BVCU_FTP_Login返回的登录Session.
*@param[out] pInfo: BVCU_FTP_SessionInfo
*@return: BVCU_Result
*/
LIBBVCU_API BVCU_Result BVCU_FTP_GetSessionInfo(BVCU_FTP_HSession hSession, BVCU_FTP_SessionInfo* pInfo);

/**
* 退出登录。该函数是异步的，在返回前或者返回后会产生OnEvent回调。
* 注意：(1)该函数必须在BVCU_FTP_Login登录成功且BVCU_FTP_Login的OnEvent回调函数被调用之后才可以调用
*  (2)不能在任何OnEvent/OnNotify中调用BVCU_FTP_Logout
* @param[in] hSession: BVCU_FTP_Login返回的登录Session.
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FTP_Logout(BVCU_FTP_HSession hSession);

/**
* CU发送命令。该函数是异步的，命令完成后触发BVCU_FTP_Command.OnEvent回调通知。
* @param[in] hSession: BVCU_FTP_Login返回的登录Session.
* @return: 常见返回值
*       BVCU_RESULT_S_OK: 调用正确完成。结果通过OnEvent通知调用者。
*      BVCU_RESULT_E_NOTEXIST: 登录Session不存在，即未登录
*      BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FTP_SendCommand(BVCU_FTP_HSession hSession, BVCU_FTP_Command* pCommand);

/**
* 创建传输。该函数是异步的。如果创建成功，在返回前或者返回后会产生OnData回调函数，
* @param[out] phTransfer: 返回句柄.
* @param[in] pParam: 传输参数。
* @param[in] pControlParam: 控制参数。
* @return: 常见返回值
*     BVCU_RESULT_S_OK: 调用正确完成。
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
/*
注意：如果下载/上传正常完成或者发生异常，库会在最后一个OnData回调后自动销毁Transfer对象，这种情况下应用程序无需调用BVCU_FTP_Transer_Close
*/
LIBBVCU_API BVCU_Result BVCU_FTP_Transer_Open(BVCU_FTP_HTransfer* phTransfer, BVCU_FTP_TransferParam* pParam, BVCU_FTP_TransferControlParam* pControlParam);

/**
*获得会话相关信息。必须在Transfer生存期内调用
* @param[in] hTransfer: BVCU_FTP_Transer_Open返回的hTransfer.
* @param[out] pInfo: BVCU_FTP_TransferInfo
* @return: BVCU_Result
*/
LIBBVCU_API BVCU_Result BVCU_FTP_GetTransferInfo(BVCU_FTP_HTransfer hTransfer, BVCU_FTP_TransferInfo* pInfo);

/**
* 更改Transfer的本地设置，此函数不需要与Server通讯。
* @param[in] hTransfer: BVCU_FTP_Transer_Open返回的hTransfer.
* @param[in] pParam: 控制参数。
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 成功
*        BVCU_RESULT_E_NOTEXIST: Transfer不存在
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FTP_Transer_Control(BVCU_FTP_HTransfer hTransfer, BVCU_FTP_TransferControlParam* pControlParam);

/**
* 关闭传输。该函数是异步的，在返回前或者返回后会产生OnData回调函数，
* 注意： (1)不能在OnData回调中调用BVCU_FTP_Transer_Close
* @param[in] hTransfer: BVCU_FTP_Login返回的登录Session.
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnData通知调用者。
*        BVCU_RESULT_S_IGNORE:  会话不存在
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FTP_Transfer_Close(BVCU_FTP_HTransfer hTransfer);

/*================================================================*/

//==========================录像文件回放相关接口===============================

// NOTE: 所有字符串必须是UTF-8编码
typedef struct _BVCU_RecordPlay_FileParam{
    char* szMediaFileName;
    char* szGPSFileName;
    char* szEventFileName;  
}BVCU_RecordPlay_FileParam;


typedef struct _BVCU_RecordPlay_FileInfo{
    //开始时间，单位 1/SAV_TIME_BASE。从 Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始经过的时间
    SAV_TYPE_INT64 iCreationTime;   
    int iDuration;//持续时间，单位毫秒      

    //视频宽度和高度
    int iVideoWidth;
    int iVideoHeight;

    //录像原因,BVCU_STORAGE_RECORDTYPE_*
    int iReason;

    //音频通道数,0-没有音频，1-单声道道，2-双声道,...
    int iAudioChannel;  

    //是否有GPS,0-没有，1-有
    int bGPS;   

    //是否有事件,0-没有，1-有
    int bEvent;
}BVCU_RecordPlay_FileInfo;

//播放状态
enum
{
    BVCU_RECORDPLAY_STATE_IDLE = 0,//未打开任何文件
    BVCU_RECORDPLAY_STATE_OPENED,//打开了文件
    BVCU_RECORDPLAY_STATE_PLAYING,//正在播放
    BVCU_RECORDPLAY_STATE_PAUSE,//暂停
    BVCU_RECORDPLAY_STATE_STEP,//单帧步进
    BVCU_RECORDPLAY_STATE_CLOSEING,//正在关闭
    BVCU_RECORDPLAY_STATE_CLOSEED,//关闭了文件，可能是手动关闭或播放完毕后自动关闭
};

typedef struct _BVCU_RecordPlay_SizeCtrl{     
    BVCU_HWND hWnd;//显示窗口句柄
    BVCU_Display_Rect rcSource;//视频源矩形，大小不能超过BVCU_RecordPlay_FileInfo中的视频宽高
    BVCU_Display_Rect rcDisplay;//显示矩形
}BVCU_RecordPlay_SizeCtrl;

typedef struct _BVCU_RecordPlay_ColorCtrl{
    int iBrightness;//亮度，取值范围0-100
    int iContrast;//对比度，取值范围0-100
    int iSaturation;//饱和度，取值范围0-100
}BVCU_RecordPlay_ColorCtrl;

#define SPEEDBASE 64
//播放信息
typedef struct _BVCU_RecordPlay_PlayInfo{   
    int iTimeOffset;//当前播放时刻，从0开始，单位毫秒
    BVCU_RecordPlay_SizeCtrl stSizeCtrl;//视频显示    
    BVCU_RecordPlay_ColorCtrl stColorCtrl;//色彩控制

    //音频回放设备的状态。0-音频设备未打开，1-打开音频回放设备成功，2-尝试过打开音频设备但失败
    int iAudioDevice;

    //正在播放的音频声道，以bit位表示每个声道的播放状态，0-不播放，1-播放。bit 0-左声道（单声道），bit 1-右声道
    int iPlaybackChannel;

    //播放音量，取值范围0-100
    int  iPlaybackVolume;

    //播放速度，单位1/64   
    int iSpeed;

    //播放状态，BVCU_RECORDPLAY_STATE_*
    int iPlayState;

    //同步组ID。>=1表示同步组ID，<=0表示不加入同步组。默认值为0。加入同一组的文件播放时同步
    int  iSyncGroupID;

    //边放边存的文件名。用于远程播放
    char* szSaveFileName;

    //下载速度，单位KB/s。用于远程播放
    int iKBps;

    //播放缓冲区的满度百分比。取值范围0-100。用于远程播放时衡量是否卡顿
    int  iFullness;
}BVCU_RecordPlay_PlayInfo;


typedef void* BVCU_RecordPlay_Handler;

typedef struct _BVCU_RecordPlay_GPSData{
    SAV_TYPE_INT64 iTimeOffset;//相对时间，以BVCU_RecordPlay_FileInfo.iCreationTime为开始
    BVCU_PUCFG_GPSData stData;//GPS数据
}BVCU_RecordPlay_GPSData;

//录像关联的事件
typedef struct _BVCU_RecordPlay_EventData{  
    SAV_TYPE_INT64 iTimeOffset;//相对时间，以FileInfo.iCreationTime为开始
    BVCU_WallTime stTime;//事件发生时间
    char szID[BVCU_MAX_ID_LEN+1];//事件源ID
//    BVCU_AlarmContent stAlarm; //事件内容
}BVCU_RecordPlay_EventData;

typedef struct _BVCU_RecordPlay_CallbackParam{
    int bStateChanged; //状态是否发生改变
    BVCU_Result iErrorCode; //状态码
    BVCU_RecordPlay_GPSData* pGPSData; //GPS数据
    BVCU_RecordPlay_EventData* pEventData; //Event数据
    SAV_Packet* pVideoData; //视频数据
    SAV_Packet* pAudioData; //音频数据
}BVCU_RecordPlay_CallbackParam;

//通知应用层的回调，注意不能在该回调中执行阻塞或耗时的操作。
typedef void (*BVCU_RecordPlay_Callback)(BVCU_RecordPlay_Handler hHandler, BVCU_RecordPlay_CallbackParam* pParam);

//播放控制
enum {
    BVCU_RECORDPLAY_CTRL_RESIZE = 1,//缩放，如全屏、跟随窗口大小、保持宽高比、视频局部放大等，应用层根据窗口大小变化消息调用此功能，参数：BVCU_RecordPlay_SizeCtrl指针
    BVCU_RECORDPLAY_CTRL_VOLUME,//音量，参数: int iVolume
    BVCU_RECORDPLAY_CTRL_AUDIOCHANNEL,//声道，参数:int iPlaybackChannel
    BVCU_RECORDPLAY_CTRL_SPEED,//速度,参数: int iSpeed
    BVCU_RECORDPLAY_CTRL_COLOR,//色彩,参数：BVCU_RECORDPLAY_ColorCtrl指针
    BVCU_RECORDPLAY_CTRL_JUMP,//跳转，参数：int iTimeOffset
    BVCU_RECORDPLAY_CTRL_PAUSE,//暂停，参数: 无
    BVCU_RECORDPLAY_CTRL_PLAY,//播放，参数：无
    BVCU_RECORDPLAY_CTRL_STEP,//单帧播放，每播放完一帧后处于PAUSE状态，参数：无
    BVCU_RECORDPLAY_CTRL_CALLBACK,//GPS/Event回调，参数BVCU_RecordPlay_Callback函数指针
    BVCU_RECORDPLAY_CTRL_SYNCGROUPID,//同步组ID,参数：int  iSyncGroupID
    BVCU_RECORDPLAY_CTRL_SAVEFILE,//边放边存文件，用于播放远端录像文件，参数: const char* 文件名指针，设为NULL表示不使用边放边存。
};

//打开文件
LIBBVCU_API BVCU_Result BVCU_RecordPlay_Open(BVCU_RecordPlay_Handler* pHandler, BVCU_RecordPlay_FileParam* pParam, BVCU_RecordPlay_FileInfo* pInfo, BVCU_HSession hSession = NULL);

//执行播放控制
LIBBVCU_API BVCU_Result BVCU_RecordPlay_Control(BVCU_RecordPlay_Handler hHandler, int iCtrlCode, void* pParam);

//查询播放信息
LIBBVCU_API BVCU_Result BVCU_RecordPlay_Query(BVCU_RecordPlay_Handler hHandler, BVCU_RecordPlay_PlayInfo* pInfo);

//关闭文件
LIBBVCU_API BVCU_Result BVCU_RecordPlay_Close(BVCU_RecordPlay_Handler hHandler);
#endif
};