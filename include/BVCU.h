#ifdef  __cplusplus
extern "C" {
#endif
#ifndef __LIBBVCU_H__
#define __LIBBVCU_H__
/*================================================================*/
#include "SAVCodec.h"
#include "BVCUConst.h"
#include "BVCUCommon.h"

// NOTE: 所有字符串必须是UTF-8编码
// 事件码
// 全局事件
enum{
    
    BVCU_EVENT_AUDIOINPUT_DISCONNECTED = 1, // 没有插入音频输入设备。事件参数：无
    BVCU_EVENT_AUDIOINPUT_CONNECTED, // 插入音频输入设备。事件参数：无
};
// Session事件
enum{
    BVCU_EVENT_SESSION_OPEN = 1,  // 创建Session。事件参数：BVCU_Event_Common
    BVCU_EVENT_SESSION_CLOSE,     // 关闭Session。事件参数：BVCU_Event_Common
    BVCU_EVENT_SESSION_CMD_COMPLETE, // 命令完成。事件参数：BVCU_Event_SessionCmd
};
// Dialog事件
enum{
    BVCU_EVENT_DIALOG_OPEN = 1,    // 创建Dialog。事件参数：BVCU_Event_Common
    BVCU_EVENT_DIALOG_UPDATE,      // 更新Dialog。事件参数：BVCU_Event_Common
    BVCU_EVENT_DIALOG_CLOSE,       // 关闭Dialog。事件参数：BVCU_Event_Common
    BVCU_EVENT_STORAGE_FILE_REQUIRENAME,  // 获取文件名，该事件提供了自定义文件名机制。事件参数: BVCU_Event_Storage
    BVCU_EVENT_STORAGE_FILE_OPEN,  // 创建文件。事件参数: BVCU_Event_Storage
    BVCU_EVENT_STORAGE_FILE_CLOSE, // 关闭文件。事件参数：BVCU_Event_Storage
    BVCU_EVENT_STORAGE_ERROR,      // 存储出错。事件参数：BVCU_Event_Storage
};

//Subscribe
typedef struct _BVCU_Subscribe_Status{
    int iSubscribeStatus;//订阅状态。BVCU_SUBSCRIBE_*
    int iReserved[4];
}BVCU_Subscribe_Status;

// CU发送的通知包和收到的命令回响内容
typedef struct _BVCU_CmdMsgContent BVCU_CmdMsgContent;
struct _BVCU_CmdMsgContent{
    /* 一个通知/回响可能包含多条信息。pNext指向下一条信息。最后一条信息的pNext应指向NULL 
        每个通知/回响的信息类型和顺序是固定的。大多数通知/回响只支持一种数据类型(pNext是NULL) **/
    BVCU_CmdMsgContent* pNext;
    
    // 信息数目
    int iDataCount;

    /* 信息数组，数组元素个数等于iDataCount，pData[0]表示第一个成员，pData[1]表示第2个成员。
    类型由具体命令决定 **/
    void* pData;
};

typedef struct _BVCU_NotifyMsgContent BVCU_NotifyMsgContent;
struct _BVCU_NotifyMsgContent{
    // 一个通知可能包含多条信息。pNext指向下一条信息。最后一条信息的pNext应指向NULL。
    BVCU_NotifyMsgContent* pNext;

    // 通知内容的类型，BVCU_SUBMETHOD_*
    int iSubMethod;

    // 信息源（系统中的网络实体)ID。为空表示是当前登录的Server
    char szSourceID[BVCU_MAX_ID_LEN+1];

    // 信息源的附属设备的索引，从0开始，例如PU的云台/通道/音视频IO等。设为-1表示无意义
    int iSourceIndex;
    
    // 目标ID。为空表示命令目标是当前登录的Server
    char szTargetID[BVCU_MAX_ID_LEN+1];

    // 从0开始的目标附属设备的索引，例如PU的云台/通道/音视频IO等。设为-1表示无意义
    int iTargetIndex;

    // 通知负载
    BVCU_CmdMsgContent stMsgContent;
};

// 所有事件参数中，涉及到指针的部分，调用者都应意识到库可能是在栈上分配内存。OnEvent函数返回后指针可能失效
typedef struct _BVCU_Event_Common
{
    BVCU_Result iResult;// 错误码
}BVCU_Event_Common;

typedef struct _BVCU_Event_SessionCmd
{
    BVCU_Result iResult;// 错误码
    int iPercent;//命令完成百分比，取值范围0~100。一个命令的返回可能很长，BVCU通过多次调用OnEvent来通知应用程序，
    //每次OnEvent的iPercent成员会递增，100表示彻底完成，只有最后一个OnEvent才能设置为100。
    //如果出错，iResult会被设置成错误码，iPercent设置成出错时完成的百分比。
    BVCU_CmdMsgContent stContent; // 命令回响的数据
}BVCU_Event_SessionCmd;

typedef struct _BVCU_Event_Storage
{
    BVCU_Result iResult;/* 错误码。
        BVCU_EVENT_STORAGE_FILE_REQUIRENAME：只有BVCU_RESULT_S_OK一种
        BVCU_EVENT_STORAGE_ERROR：
        （1）如果iResult==BVCU_RESULT_E_OUTOFSPACE,库内部会立即关闭当前文件并发出BVCU_EVENT_STORAGE_FILE_CLOSE，然后持续监测硬盘空间，
        如果硬盘空间足够大，则自动再次开始录像。
        （2）如果iResult==BVCU_RESULT_E_FAILED，表示未知原因错误，库保证在一个文件关闭之前，无论遇到几次错误，都只发送一次该事件，
        发出事件后，库仍然正常录像 **/

    char* szFileName;/* 文件名，要求（1）必须是绝对路径（2）以'\0'结尾。该字符串指向库内部的长度为(BVCU_MAX_FILE_NAME_LEN+1)字节的字符数组
    应用程序可以在FILE_REQUIRENAME事件中修改成任何合法的文件名，
    以实现文件管理策略。其他事件中禁止修改 **/
    SAV_TYPE_INT64 iTimestamp;// REQUIRENAME/FILE_OPEN:文件开始时间戳，FILE_CLOSE:文件结束时间戳。从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数*/
}BVCU_Event_Storage;

typedef  void* BVCU_HSession;
typedef  void* BVCU_HDialog;

typedef struct _BVCU_GlobalParam{
    // 本结构体的大小，分配者应初始化为sizeof(BVCU_GlobalParam)
    int iSize;

    /* 库把库内部检测到的全局事件通知应用程序
     iEventCode:事件码，参见全局事件
     pParam: 每个事件对应的参数，具体类型参考各个事件码的说明。如果pParam是NULL，表示无参数。
    */
    void (*OnEvent)(int iEventCode, void* pParam);

    // 保留
    int iReserved[4];
}BVCU_GlobalParam;


// CU发出的命令
typedef struct _BVCU_Command BVCU_Command;
struct _BVCU_Command{
    // 本结构体的大小，分配者应初始化为sizeof(BVCU_Command)
    int iSize;
    
    // 用户自定义数据。通常用于回调通知。应用程序/库可以用该成员来区分不同的命令
    void* pUserData;

    // 命令类型，BVCU_METHOD_* 
    int iMethod;
    
    // 子命令类型，BVCU_SUBMETHOD_*，决定了BVCU_CmdMsgContent.pData类型
    int iSubMethod;

    // 命令源（系统中的网络实体)ID。SendCmd()时不填，库内部自动填写。
    char szSourceID[BVCU_MAX_ID_LEN + 1];

    // 命令源的登录标识。 0表示无意义。SendCmd()时不填，库内部自动填写。
    int iSourceID;

    // 系统中的网络实体目标ID。设置为空表示命令目标是当前登录的Server
    char szTargetID[BVCU_MAX_ID_LEN+1];

    // 从0开始的目标附属设备的索引，例如PU的云台/通道/音视频IO等。设为-1表示无意义
    int iTargetIndex;
    
    // 命令超过iTimeOut未收到回响则认为失败，单位毫秒。如果设置为0，则采用BVCU_ServerParam.iTimeout
    int iTimeOut;

    // 命令负载
    BVCU_CmdMsgContent stMsgContent;

    /*事件。
    pCommand:本命令指针。注意该指针指向的是SDK内部维护的一个BVCU_Command浅拷贝
    iEventCode:事件码，参见BVCU_EVENT_SESSION_CMD_*。目前iEventCode总是等于BVCU_EVENT_SESSION_CMD_COMPLETE
     pParam: 每个事件对应的参数，具体类型参考各个事件码的说明
    */
    void (*OnEvent)(BVCU_HSession hSession, BVCU_Command* pCommand, int iEventCode, void* pParam);

    // 保留
    int iReserved[2];
};

// Server信息
typedef struct _BVCU_ServerParam{
    // 本结构体的大小，分配者应初始化为sizeof(BVCU_ServerParam)
    int iSize;
    
    // 用户自定义数据。通常用于回调通知
    void* pUserData;

    // Server地址，域名或者IP
    char szServerAddr[BVCU_MAX_HOST_NAME_LEN+1];
    
    // Server端口号
    int  iServerPort;
    
    // Client ID。必须以应用程序类型（"CU_"/"NRU_"/"PU_"等）开始（之后的部分禁止包含'_')，调用者应选择一种ID分配方式，尽量使每台计算机上的ID不同。如果为空，则由库内部生成ID
    char szClientID[BVCU_MAX_ID_LEN+1];

    // 应用程序名称。该名称被Server端记录到Log中
    char szUserAgent[BVCU_MAX_NAME_LEN+1];

    // 登录用户名
    char szUserName[BVCU_MAX_NAME_LEN+1];
    
    // 登录密码
    char szPassword[BVCU_MAX_PASSWORD_LEN + 1];

    // Ukey ID。从UKey中获得的UKeyID。登录验证中使用。如果为空，表示没有UKey
    char szUKeyID[BVCU_MAX_ID_LEN + 1];

    // Ukey 授权码。 从UKey中获得的验证授权码。如果为空，表示没有UKey授权码
    char szUkeyCode[BVCU_MAX_PASSWORD_LEN + 1];
    
    // CU与Server之间命令通道使用的传输层协议类型，参见BVCU_PROTOTYPE_*。目前仅支持TCP/UDP
    int iCmdProtoType;

    // 命令超过iTimeOut未收到回响则认为失败，单位毫秒。必须>0
    int iTimeOut;

    /* CU/NRU收到Server的Notify通知后的回调函数
    返回：库会根据返回值构造给Server的回响包
    */
    BVCU_Result (*OnNotify)(BVCU_HSession hSession, BVCU_NotifyMsgContent* pData);

    /* 该Session相关的事件。函数BVCU_GetSessionInfo可以用来获得BVCU_ServerParam参数。
    iEventCode:事件码，参见Session事件
     pParam: 每个事件对应的参数，具体类型参考各个事件码的说明 */
    void (*OnEvent)(BVCU_HSession hSession, int iEventCode, void* pParam);

    /*
    CU/NRU收到的Control/Query命令 
    pCommand：库内部的一个BVCU_Command对象指针。应用程序完成命令处理后，应调用pCommand->OnEvent 
    返回：BVCU_RESULT_S_OK表示应用程序要处理本命令，其他值表示应用程序忽略该命令，由库决定如何处理。
    */
    BVCU_Result (*OnCommand)(BVCU_HSession hSession, BVCU_Command* pCommand);

    // 保留，必须初始化为0
    int iReserved[4];
}BVCU_ServerParam;


// Session信息。一次登录创建一个Session。
typedef struct _BVCU_SessionInfo{
    // 创建该Session的Param
    BVCU_ServerParam stParam;

    // 服务器ID
    char szServerID[BVCU_MAX_ID_LEN+1];

    // SmartEye域名
    char szDomain[BVCU_MAX_SEDOMAIN_NAME_LEN+1];

    // 服务器名
    char szServerName[BVCU_MAX_NAME_LEN+1];

    // Server 代码名称
    char szServerCodeName[BVCU_MAX_NAME_LEN+1];

    // Server版本号
    char szServerVersion[BVCU_MAX_NAME_LEN+1];

    // CU本地IP
    char szLocalIP[BVCU_MAX_HOST_NAME_LEN+1];
    
    // CU本地命令端口
    int iLocalPort;
    
    // CU是否在线
    int iOnlineStatus;
    
    // 登录时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的毫秒数
    SAV_TYPE_INT64 iLoginTime;
    
    // 本次登录持续时间，单位毫秒
    SAV_TYPE_INT64 iOnlineTime;
    
    // CMS分配的用户标识
    int iApplierID; 
    
    // 保留，必须初始化为0
    int iReserved[3];
}BVCU_SessionInfo;


// 设备通道信息， 具体定义见 PUConfig.h
typedef struct _BVCU_PUCFG_ChannelInfo BVCU_PUOneChannelInfo;
//通道信息：被 BVCU_MSGCONTENT_TYPE_PU_CHANNELINFO（上下线通知）/BVCU_SUBMETHOD_GETPULIST（获取PU列表）使用
typedef struct _BVCU_PUCFG_PUChannelInfo BVCU_PUChannelInfo;


// 会话相关。一个会话是指浏览一个设备的一个通道音视频内容或者对讲

typedef struct _BVCU_DialogTarget{
    // 目标ID，例如PU ID,NRU ID
    char szID[BVCU_MAX_ID_LEN+1];
    
    // 目标下属的子设备的主要号，BVCU_SUBDEV_INDEXMAJOR_* 
    int iIndexMajor;

    /* 目标下属的子设备的次要号，例如PU通道下属的流号。设置为-1表示由Server决定传输哪个流。
      bit 0～5：BVCU_ENCODERSTREAMTYPE_* 
      bit 6~31：由BVCU_ENCODERSTREAMTYPE_决定的参数，默认为0
          对BVCU_ENCODERSTREAMTYPE_STORAGE/BVCU_ENCODERSTREAMTYPE_PREVIEW，设置为0，表示未使用
          对BVCU_ENCODERSTREAMTYPE_PICTURE，bit 6~9：连拍张数-1（即设置为0表示连拍1张）；bit 10~15: 抓拍间隔，单位秒，最大允许值为60秒
    */
    int iIndexMinor;
    
}BVCU_DialogTarget;

typedef struct _BVCU_FileTarget {
    // 目标文件相对路径+文件名, 有意义时库内部会深拷贝
    char* pPathFileName;

    // 回放时：回放开始时间， 从文件头开始的秒数 
    int iStartTime;

    // 回放时：回放结束时间， 从文件头开始的秒数， 需要大于iStartTime (-1: 文件结束) 
    int iEndTime;
}BVCU_FileTarget;

typedef struct _BVCU_DialogParam
{
    // 本结构体的大小，分配者应初始化为sizeof(BVCU_DialogParam)
    int iSize;

    // 用户自定义数据。通常用于回调通知
    void* pUserData;

    // 登录Session
    BVCU_HSession hSession;
    
    // 会话目标个数
    int iTargetCount;

    /* 会话目标数组。pTarget内存由调用者分配/释放。调用BVCU_Dialog_Open/BVCU_Dialog_Update时， 
        SDK会保留pTarget的拷贝。调用BVCU_GetDialogInfo时，pTarget会指向SDK内部的拷贝，调用者
        不可以对拷贝做任何修改
    */
    const BVCU_DialogTarget* pTarget;

    // 会话文件目标，回放和文件传输通道时有意义，实时通道时设置为零
    BVCU_FileTarget stFileTarget;

    // 会话的数据流方向, BVCU_MEDIADIR_* 
    int iAVStreamDir;

    // 会话的流传输途径, BVCU_STREAM_PATHWAY_* 
    int iAVStreamWay;

    // 会话数据通道是否走TCP连接，0：否，1：是。over TCP时iAVStreamWay无意义 
    int bOverTCP;

    //视频通话请求返回的CallID
    char szCallID[BVCU_MAX_ID_LEN + 1];

    //打开会话密码
    char szPassword[BVCU_MAX_ID_LEN + 1];

    /*NetWorkThread：对音视频，收到帧数据后调用。调用者不可以在回调数据中对收到的数据进行处理，可以拷贝到自己内存中处理。
          对纯数据(pCodec == NULL)，从缓冲区得到数据后立即回调
    pCodec: Codec信息
    pPacket：音视频数据：收到的原始媒体数据；纯数据：组好帧后的数据
    返回：对纯数据无意义。对音视频数据：
       BVCU_RESULT_S_OK：pPacket被解码显示/播放。
       BVCU_RESULT_E_FAILED：pPacket不被解码显示/播放。
    */
    BVCU_Result(*afterRecv)(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Packet* pPacket);

    /*CodecThread：对音视频数据，解码完成后调用。调用者可以对解码后的数据进行各种处理。对于视频数据，必须做备份后修改备份，
                                 并把pFrame中ppData数据指针指向修改后的备份。
            对纯数据(SAVCodec_Context.eMediaType==SAV_MEDIATYPE_DATA)，从缓冲区得到数据后立即回调
    pCodec: Codec信息
    pFrame：音视频数据：解码得到的原始媒体数据；纯数据：组好帧后的数据
        返回：对纯数据无意义。对音视频数据，
                BVCU_RESULT_S_OK：pFrame被显示/播放。
        BVCU_RESULT_E_FAILED：pFrame不被显示/播放。
    */
    BVCU_Result (*afterDecode)(BVCU_HDialog hDialog, SAVCodec_Context* pCodec, SAV_Frame* pFrame);
    
    /*VideoRenderThread/AudioRenderThread：显示/播放完一个音频或视频帧后调用。用户可以在此处叠加其他效果如文字等。
    pCodec: Codec信息
    pFrame：解码得到的原始媒体数据
    返回：目前库会忽略返回值
    */
    BVCU_Result (*afterRender)(BVCU_HDialog hDialog, SAVCodec_Context* pCodec,SAV_Frame* pFrame);

    /* 
    事件回调。函数BVCU_GetDialogInfo可以用来获得BVCU_DialogParam
    iEventCode:事件码，参见Dialog事件
    pParam: 每个事件对应的参数，具体类型参考各个事件码的说明。如果pParam是NULL，表示无参数。
    BVCU_RESULT_E_UNSUPPORTED: 设备只支持P2P。
    BVCU_RESULT_E_VTDU_NONE：没有可用流媒体服务器。
    BVCU_RESULT_E_MAXRETRIES： 设备该通道已经达到最大P2P打开数。
    BVCU_RESULT_E_ALREADYEXIST：对讲或串口被占用。
    BVCU_RESULT_E_BUSY: 通道状态繁忙，不能影响操作。
    BVCU_RESULT_E_IO： 建立P2P连接失败。
    */
    void (*OnEvent)(BVCU_HDialog hDialog, int iEventCode, void* pParam);

    // 保留
    int iReserved[4];
}BVCU_DialogParam;


// 控制会话的网络参数
#define BVCU_NETWORK_DVSS_MIN 1
#define BVCU_NETWORK_DVSS_MAX 7
#define BVCU_NETWORK_DELAY_MAX 10000
typedef struct _BVCU_DialogControl_Network{
    // 会话命令超过iTimeOut未收到回响则认为失败，单位毫秒。如果设置为0，则采用BVCU_ServerParam.iTimeout
    int iTimeOut;

    // A/V数据从接收到输出，最大允许的延迟，不能超过BVCU_NETWORK_DELAY_MAX。单位：毫秒（参考值：5000）
    int iDelayMax;
    
    // A/V数据从接收到输出，最小允许的延迟，不能超过BVCU_NETWORK_DELAY_MAX。单位：毫秒（参考值：1000）
    int iDelayMin;
    
    // 播放延迟与平滑选择。取值范围BVCU_NETWORK_DVSS_MIN～BVCU_NETWORK_DVSS_MAX，越小则播放延迟越小，但平滑变差，越大则播放越平滑，但延迟变大（参考值：3）。
    int iDelayVsSmooth;
}BVCU_DialogControlParam_Network;

// 控制会话的视频显示/音频回放
#define BVCU_RENDER_NO_VIDEO NULL
#define BVCU_RENDER_NO_AUDIO -1

// 视频显示模式
enum {
    BVCU_DISPLAY_MODE_FILL = 0, // 填充
    BVCU_DISPLAY_MODE_AUTO_ZOOM,// 自动等比例缩放
    BVCU_DISPLAY_MODE_ZOOM      // 等比例缩放
};

typedef struct _BVCU_Display_Param {
    // 视频显示模式(BVCU_DISPLAY_MODE_*)
    int iDisplayMode;

    // 缩放比例, fMulZoom = 放大倍数*10  当iDisplayMode=BVCU_DISPLAY_MODE_ZOOM时有效
    int fMulZoom;

    // 视频旋转角度, 仅支持90的整倍数
    int iAngle;
}BVCU_Display_Param;

typedef struct _BVCU_DialogControl_Render{
    // 显示窗口句柄, BVCU_RENDER_NO_VIDEO表示不显示，并且不要执行视频解码。
    BVCU_HWND hWnd;
    
    // 显示矩形，设置成(0,0,0,0)表示不显示，但执行视频解码
    BVCU_Display_Rect rcDisplay;
    
    // 播放音量，正常范围0～100.如果设为BVCU_RENDER_NO_AUDIO 表示不播放音频，并且不执行音频解码
    int  iPlackbackVolume;
    
    // 采集音量，正常范围0～100.如果设为BVCU_RENDER_NO_AUDIO 表示不采集音频
    int  iCaptureVolume;
    
    // 使能或禁止音视频同步。0：使能；1：禁止
    int bDisableAVSync;

    // 视频显示参数
    BVCU_Display_Param stDisplayParam;
}BVCU_DialogControlParam_Render;

// 控制会话的录像
#define BVCU_STORAGE_MAX_FILELENINSEC 7200
typedef struct _BVCU_DialogControl_Storage{
    // 录像路径
    char szFilePath[BVCU_MAX_FILE_NAME_LEN+1];
    
    // 每个录像文件的时间长度，单位秒。设置为<=0表示停止存储，不能超过BVCU_STORAGE_MAX_FILELENINSEC
    int   iFileLenInSeconds;

    int iFileFormat;   //文件存储格式，BVCU_FILE_STORAGE_*，默认mkv
    // 保留
    int iReserved[1];
}BVCU_DialogControlParam_Storage;

// 控制会话
typedef struct _BVCU_DialogControlParam{
    // 本结构体的大小，分配者应初始化为sizeof(BVCU_DialogControlParam)
    int iSize;

    BVCU_DialogControlParam_Network stNetwork;
    BVCU_DialogControlParam_Render  stRender;
    BVCU_DialogControlParam_Storage stStorage;
}BVCU_DialogControlParam;

// 会话信息
typedef struct _BVCU_DialogInfo{
    // Dialog参数
    BVCU_DialogParam stParam;
    BVCU_DialogControlParam stControlParam;
    
    // 媒体信息
    SAVCodec_Context stVideoCodecRemote;
    SAVCodec_Context stAudioCodecRemote;
    SAVCodec_Context stAudioCodecLocal;
    
    // 会话开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iCreateTime;
    
    // 会话持续时间，单位微秒
    SAV_TYPE_INT64 iOnlineTime;
    
    // 会话当前状态，见 BVCU_DIALOG_STATUS_* 
    int iStatus;

    // 统计信息
    
    // 收到的总包数
    SAV_TYPE_INT64 iVideoTotalPackets;
    SAV_TYPE_INT64 iAudioTotalPackets;
    
    // 收到的总帧数
    SAV_TYPE_INT64 iVideoTotalFrames;
    SAV_TYPE_INT64 iAudioTotalFrames;
    
    // 网络部分长时间统计数据
    int iVideoLostRateLongTerm;// 丢包(或帧)率，单位1/10000
    int iAudioLostRateLongTerm;// 丢包(或帧)率，单位1/10000
    int iVideoRecvFPSLongTerm; // 网络接收帧率，单位1/10000帧每秒
    int iVideoKbpsLongTerm;// 视频数据码率，单位 Kbits/second
    int iAudioKbpsLongTerm;// 音频数据码率，单位 Kbits/second
    

    // 网络部分短时间时间统计数据
    int iVideoLostRateShortTerm;
    int iAudioLostRateShortTerm;
    int iVideoRecvFPSShortTerm;
    int iVideoKbpsShortTerm;
    int iAudioKbpsShortTerm;

    // VideoRender显示帧率
    int iVideoRenderFPS;// 帧率，单位1/10000帧每秒
}BVCU_DialogInfo;

/*==========================initialize=======================================*/
/**
*初始化BVCU库，只能在应用程序启动时调用一次。任何其他BVCU库函数只有在 
 BVCU_Initialize成功后才可以调用
*/
LIBBVCU_API BVCU_Result BVCU_Initialize(const BVCU_GlobalParam* pParam);

/**
*停止使用BVCU库
*/
LIBBVCU_API BVCU_Result BVCU_Finish();


/**
 * 
 * 设置日志输出级别
 * @author lcj (2012/1/17)
 * 
 * @param iLogDevice    BVCU_LOG_DEVICE_*
 * @param iLevel    BVCU_LOG_LEVEL*
 * 
 * @return LIBBVCU_API BVCU_Result 
 */
LIBBVCU_API BVCU_Result BVCU_SetLogLevel(int iLogDevice, int iLevel);

/**
 * 
 * 
 * @author lcj (2012/1/17)
 * 
 * @param iLogDevice BVCU_LOG_DEVICE_*
 * @param iLevel 
 * 
 * @return LIBBVCU_API int  BVCU_LOG_LEVEL*
 */
LIBBVCU_API int BVCU_GetLogLevel(int iLogDevice);
/*=======================login/logout=========================================*/
/*
注意：登录完成后，应用程序随时可能收到BVCU_EVENT_SESSION_CLOSE事件回调，
回调之后，Session被SDK摧毁，BVCU_HSession变成无效值
*/

/**
*登录Server。该函数是异步的。如果登录成功，在返回前或者返回后会产生OnEvent回调。
* @param[out] phSession: 返回登录Session
* @param[in] pParam: Server信息
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Login(BVCU_HSession* phSession, BVCU_ServerParam* pParam);

/**
*获得登录Session相关信息
*@param[in] hSession: BVCU_Login返回的登录Session.
*@param[out] pInfo: BVCU_SessionInfo 
*@return: BVCU_Result
*/
LIBBVCU_API BVCU_Result BVCU_GetSessionInfo(BVCU_HSession hSession, BVCU_SessionInfo* pInfo);

/**
* 退出登录。该函数是异步的，在返回前或者返回后会产生OnEvent回调。
* 注意：(1)该函数必须在BVCU_Login登录成功且BVCU_Login的OnEvent回调函数被调用之后才可以调用 
*  (2)不能在任何OnEvent/OnNotify中调用BVCU_Logout
* @param[in] hSession: BVCU_Login返回的登录Session.
* @return: 常见返回值
*    BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。
*    BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Logout(BVCU_HSession hSession);

/*============================command=========================================*/
/**
* CU发送命令。该函数是异步的，命令完成后触发BVCU_Command.OnEvent回调通知。
* @param[in] hSession: BVCU_Login返回的登录Session.
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 调用正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_NOTEXIST: 登录Session不存在，即未登录
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_SendCmd(BVCU_HSession hSession, BVCU_Command* pCommand);

/*=============================dialog=========================================*/
/*
注意：会话过程中，随时可能收到BVCU_EVENT_DIALOG_CLOSE事件回调，
回调之后，会话被SDK摧毁，BVCU_HDialog变成无效值
*/

/**
* 创建会话。该函数是异步的。如果创建会话成功，在返回前或者返回后会产生OnEvent回调函数，
* 事件码是BVCU_EVENT_DIALOG_OPEN，如果事件参数的iResult是失败代码，则会话创建失败，调用者不必调用BVCU_Dialog_Close
* @param[out] phDialog: 返回会话句柄.
* @param[in] pParam: 会话参数。
* @param[in] pControl: 控制参数。
* @return: 常见返回值 
*        BVCU_RESULT_S_OK: 调用正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_UNSUPPORTED: 不支持的操作，例如在不支持对讲的通道上要求对讲
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Open(BVCU_HDialog* phDialog, BVCU_DialogParam* pParam, BVCU_DialogControlParam* pControl);

/**
*获得会话相关信息
*@param[in] hDialog: BVCU_Dialog_Open返回的hDialog. 
*@param[out] pInfo: BVCU_DialogInfo 
*@return: BVCU_Result
*/
LIBBVCU_API BVCU_Result BVCU_GetDialogInfo(BVCU_HDialog hDialog, BVCU_DialogInfo* pInfo);

/**
* 更改已建立的会话，需要与Server通讯。对已建立的会话，允许修改pParam->iAVStreamDir。 对只有一个Target的Dialog，
* 还允许修改Target的iMajorIndex，用于实现通道切换。 
* 该函数是异步的。 
* 该函数可能发出异步BVCU_EVENT_DIALOG_UPDATE事件，携带结果状态码。如果结果状态码失败，可能会出现两种情况： 
* (1)Dialog仍然处于Update之前的打开状态。例如只传音/视频=>音视频同传失败
* (2)Dialog关闭，接着会发送BVCU_EVENT_DIALOG_CLOSE事件。例如只传音频=>只传视频或相反；更改iMajorIndex等
* @param[in] hDialog: BVCU_Dialog_Open返回的hDialog.
* @param[in] pParam: 会话参数。
* @return: 常见返回值 
*        BVCU_RESULT_S_OK:调用正确完成。结果通过OnEvent通知调用者，事件码是BVCU_EVENT_DIALOG_UPDATE/BVCU_EVENT_DIALOG_CLOSE。
*        BVCU_RESULT_E_NOTEXIST: 会话不存在 
*        BVCU_RESULT_E_BUSY:上一次的会话操作还未完成 
*        BVCU_RESULT_E_UNSUPPORTED:不支持的操作，例如在不支持对讲的通道上要求对讲 BVCU_RESULT_E_FAILED： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Update(BVCU_HDialog hDialog, BVCU_DialogParam* pParam);

/**
*本地抓取会话中接收到的一帧视频，存为图像文件。目前仅支持JPG格式
* @param[in] hDialog: BVCU_Dialog_Open返回的hDialog.
* @param[in] szFileName: 抓取文件名。
* @param[in] iQuality:
*       JPG压缩质量，取值范围1～100，数值越大压缩后的图像质量越好，但文件会越大。建议质量不小于80
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 成功
*        BVCU_RESULT_E_FAILED： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Snapshot(BVCU_HDialog hDialog, const char* szFileName, int iQuality);

/**
* 更改会话的本地设置，包括接收/存储/回放等。此函数不需要与Server通讯。
* @param[in] hDialog: BVCU_Dialog_Open返回的Dialog句柄.
* @param[in] pParam: 控制参数。
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 成功
*        BVCU_RESULT_E_NOTEXIST: 会话不存在
*        BVCU_RESULT_E_UNSUPPORTED: 不支持的操作
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Control(BVCU_HDialog hDialog, BVCU_DialogControlParam* pParam);

/**
* 向会话通道中写数据。目前只支持含BVCU_MEDIADIR_DATASEND方向的会话通道。
* @param[in] hDialog: BVCU_Dialog_Open返回的Dialog句柄.
* @param[in] pData: 写入的数据。
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 成功
*        BVCU_RESULT_E_NOTEXIST: 会话不存在
*        BVCU_RESULT_E_UNSUPPORTED: 不支持的操作
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Write(BVCU_HDialog hDialog, SAV_Packet* pData);

/**
* 关闭会话。该函数是异步的，在返回前或者返回后会产生OnEvent回调函数，
* 注意：(1)该函数必须在BVCU_Dialog_Open成功且BVCU_Dialog_Open的OnEvent回调函数被调用之后才可以调用 
* (2)不能在任何OnEvent/OnNotify中调用BVCU_Dialog_Close 
* @param[in] hDialog: BVCU_Dialog_Open返回的Dialog句柄.
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。 
*        BVCU_RESULT_S_IGNORE:  会话不存在
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_Dialog_Close(BVCU_HDialog hDialog);

/*=============================file transfer====================================*/

typedef  void* BVCU_File_HTransfer;

// 文件传输参数设置。收到传输请求并同意时：szLocalFileName，OnEvent需要填写。
typedef struct _BVCU_File_TransferParam {
    int iSize; // 本结构体的大小，分配者应初始化为sizeof(BVCU_File_TransferParam)
    void* pUserData;          // 用户自定义数据。BVCU_File_GlobalParam.OnFileRequest中可以填写。
    BVCU_HSession hSession;   // BVCU_Login返回的句柄。
    char  szTargetID[BVCU_MAX_ID_LEN+1]; // 文件传输目标对象ID。PU/NRU。"NRU_"时库内部会在回调前填写具体NRU ID。
    char* pRemoteFilePathName;// 远端路径+文件名。
    char* pLocalFilePathName; // 本地路径+文件名。BVCU_File_GlobalParam.OnFileRequest中要填写。
    unsigned int iFileStartOffset;     // 文件开始传输偏移。0：重新传输。-1(0xffffffff)：库内自动处理续传。
    int iTimeOut;             // 连接超时间。单位 毫秒。
    int bUpload;              // 0-下载，1-上传。

    /* 文件传输句柄事件回调函数。 BVCU_File_GlobalParam.OnFileRequest中要填写。
    hTransfer:传输句柄。
    pUserData：本结构体中的pUserData。
    iEventCode: 事件码，参见BVCU_EVENT_DIALOG_*。Open/Close。
    iResult: 事件对应的结果码。
    */
    void(*OnEvent)(BVCU_File_HTransfer hTransfer, void* pUserData, int iEventCode, BVCU_Result iResult);

}BVCU_File_TransferParam;

// 文件传输信息
typedef struct _BVCU_File_TransferInfo {
    // 传输参数
    BVCU_File_TransferParam stParam;

    // Transfer开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数
    SAV_TYPE_INT64 iCreateTime;

    // Transfer持续时间，单位微秒
    SAV_TYPE_INT64 iOnlineTime;

    // 已经传输的字节数，包含BVCU_File_TransferParam.iFileStartOffset。和iTotalBytes一起计算出当前传输进度。
    unsigned int iTransferBytes;
    // 总字节数
    unsigned int iTotalBytes;

    int iSpeedKBpsLongTerm;// 长时间传输速率，单位 KBytes/second
    int iSpeedKBpsShortTerm;// 短时间传输速率，单位 KBytes/second

}BVCU_File_TransferInfo;

typedef struct _BVCU_File_GlobalParam {
    int iSize; // 本结构体的大小，分配者应初始化为sizeof(BVCU_File_GlobalParam)
    int iTCPListenPort; // 被动下载时TCP监听端口。
    int iMaxTransfer;   // 支持的并发传输最大数目。0表示无限制
    int iBandwidthLimit;// 带宽限制。单位kbps，0表示无限制，建议限制文件传输的带宽。

    /* 收到文件传输请求回调
    hTransfer：文件传输的句柄。
    pParam: 每个事件对应的参数，具体类型参考各个事件码的说明。如果pParam是NULL，表示无参数。
    返回：BVCU_RESULT_S_OK表示应用程序接受该传输请求，其他值表示拒绝。同情请求时注意填写pParam中需要填写的值。
    */
    BVCU_Result (*OnFileRequest)(BVCU_File_HTransfer hTransfer, BVCU_File_TransferParam* pParam);

}BVCU_File_GlobalParam;


/**
* 设置文件传输相关的全局参数。请在调用BVCU_Initialize()前，调用BVCU_FileTransfer_SetGlobalParam().
* @param[in] pParam: 文件传输相关的全局参数
* @return: 常见返回值。BVCU_RESULT_S_OK: 正确完成。
*/
LIBBVCU_API BVCU_Result BVCU_FileTransfer_SetGlobalParam(BVCU_File_GlobalParam* pParam);

/**
* 创建文件传输。该函数是异步的。如果创建会话成功，在返回前或者返回后会产生OnEvent回调函数，
* 事件码是BVCU_EVENT_DIALOG_OPEN，如果事件参数的iResult是失败代码，则会话创建失败。
* @param[out] phTransfer: 返回传输句柄.
* @param[in] pParam: 传输参数。
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 调用正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_E_UNSUPPORTED: 不支持的操作，服务器版本过低。
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FileTransfer_Open(BVCU_File_HTransfer* phTransfer, BVCU_File_TransferParam* pParam);

/**
*获得会话相关信息
*@param[in] hTransfer: BVCU_FileTransfer_Open返回的hTransfer，或BVCU_File_GlobalParam.OnFileRequest通知的hTransfer
*@param[out] pInfo: BVCU_File_TransferInfo
*@return: BVCU_Result
*/
LIBBVCU_API BVCU_Result BVCU_FileTransfer_GetInfo(BVCU_File_HTransfer hTransfer, BVCU_File_TransferInfo* pInfo);

/**
* 关闭文件传输。
* @param[in] hTransfer: 同BVCU_FileTransfer_GetInfo.
* @return: 常见返回值
*        BVCU_RESULT_S_OK: 正确完成。结果通过OnEvent通知调用者。
*        BVCU_RESULT_S_IGNORE:  会话不存在
*        BVCU_RESULT_E_FAILED或其他： 其他错误导致失败
*/
LIBBVCU_API BVCU_Result BVCU_FileTransfer_Close(BVCU_File_HTransfer hTransfer);


/*================================================================*/
#endif
#ifdef  __cplusplus
};
#endif