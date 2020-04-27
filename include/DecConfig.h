#ifndef __DEC_CONFIG_H__
#define __DEC_CONFIG_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
#include "PUConfig.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!
//
// 解码器基本信息，获取解码器列表时使用
typedef struct _BVCU_DECCFG_BaseInfo {
    char  szDeviceID[BVCU_MAX_ID_LEN + 1];    // Decoder ID
    char  szName[BVCU_MAX_NAME_LEN + 1];// Decoder Name
    int   iDecChanCount;  //解码通道数 
    int   iDispChanCount; //显示通道数

    int   iOnlineStatus;  // 在线状态，参见BVCU_ONLINE_STATUS_*。
    int   iOnlineThrough; // 上线途径,BVCU_PU_ONLINE_THROUGH_*的某个值，iOnlineStatus=BVCU_ONLINE_STATUS_ONLINE时有效。
    BVCU_WallTime stOnlineTime;// 上线时间，iOnlineStatus在线时有意义（平台填写)  UTC时间 
    BVCU_WallTime stBootTime;  // 设备开机时间，平台填写，设备上线时通过iBootDuration上报开机时长，UTC时间
}BVCU_DECCFG_BaseInfo;

//设备信息，除非注明可写，否则其成员只读
typedef struct _BVCU_DECCFG_DeviceInfo{
    char szDeviceID[BVCU_MAX_ID_LEN+1];       //设备ID
    char szManufacturer[BVCU_MAX_NAME_LEN+1]; //制造商名字
    char szProductName[BVCU_MAX_NAME_LEN+1];     //产品名
    char szSoftwareVersion[BVCU_MAX_NAME_LEN+1]; //软件版本
    char szHardwareVersion[BVCU_MAX_NAME_LEN+1]; //硬件版本    
    int  iPUType;          //BVCU_PUTYPE_*
    int  iLanguage[BVCU_MAX_LANGGUAGE_COUNT];    //支持的语言列表。BVCU_LANGUAGE_*
    int  iLanguageIndex;  //当前使用的语言索引。可写
    char szName[BVCU_MAX_NAME_LEN+1];//名字。可写

    int   iDecChanCount;  //解码通道数 
    int   iDispChanCount; //显示通道数

    BVCU_ImageSize stVgaSupportResolution[BVCU_MAX_SUPPORT_RESOLUTION];   //VGA  支持的分辨率 遇到宽或高<=0的分辨率时结束数组。
    BVCU_ImageSize stHdmiSupportResolution[BVCU_MAX_SUPPORT_RESOLUTION];  //HDMI 支持的分辨率 遇到宽或高<=0的分辨率时结束数组。
    BVCU_ImageSize stDviSupportResolution[BVCU_MAX_SUPPORT_RESOLUTION];   //DVI  支持的分辨率 遇到宽或高<=0的分辨率时结束数组。
    BVCU_ImageSize stYpbprSupportResolution[BVCU_MAX_SUPPORT_RESOLUTION]; //Ypbpr支持的分辨率 遇到宽或高<=0的分辨率时结束数组。

    int   iOnlineThrough; // 上线途径,BVCU_PU_ONLINE_THROUGH_*的某个值，iOnlineStatus=BVCU_ONLINE_STATUS_ONLINE时有效。
    int   iBootDuration;  // 设备开机运行了多长时间，设备上报，设备上线时上报设备已经开机多长时间， 单位 （秒）
}BVCU_DECCFG_DeviceInfo;

// SmartEye 数据源信息
typedef struct _BVCU_DECCFG_BVDataChanInfo{
    BVCU_PUCFG_RegisterServer stBVServer; // SmartEye 服务器登录信息。
    char szPUID[BVCU_MAX_ID_LEN + 1]; // 源设备ID号
    int  iChannelIndex; // 源设备通道号
    int  iStreamMode;   // 码流类型，-1：自动，暂时不使用，需要填-1。
} BVCU_DECCFG_BVDataChanInfo;
// 数据源通道状态
typedef struct _BVCU_DECCFG_DataChanStatus{
    int  bOnline;   // 数据通道是否在线。-1：未知，0-不在线，1-在线。
    // 音频数据信息
    int  iCodeIDA;  // 音频编码格式  见 SAVCODEC_ID_*(SAVCodec.h)
    int  iFpsDecA;  // 音频解码帧率  单位1/10000帧每秒
    int  iDecodedA; // 已经解码的音频帧数
    int  iLostRateA;// 音频丢包(或帧)率，单位1/10000
    int  iKbpsA;    // 音频数据码率，单位 Kbits/second
    // 视频数据信息
    int  iCodeIDV;  // 视频编码格式  见 SAVCODEC_ID_*(SAVCodec.h)
    int  iFpsDecV;  // 视频解码帧率  单位1/10000帧每秒
    int  iDecodedV; // 已经解码的视频帧数
    int  iLostRateV;// 视频丢包(或帧)率，单位1/10000
    int  iKbpsV;    // 视频数据码率，单位 Kbits/second
    int  iImgW; // 解码通道当前的图像大小,宽
    int  iImgH; // 高
}BVCU_DECCFG_DataChanStatus;
// 解码通道数据源
typedef struct _BVCU_DECCFG_DataChanInfo {
    int  bEnable;  // 是否使能， 0-不使能， 1-使能。 
    int  iDataChanType; // 通道协议类型， 见 BVCU_IPC_NET_PROTO_TYPE_*， 目前只支持BVCU_IPC_NET_PROTO_TYPE_SMARTEYE
    union {
        BVCU_DECCFG_BVDataChanInfo stBVChannelInfo;
    }stChannelParam;
    BVCU_DECCFG_DataChanStatus stStatus; // 通道当前状态
} BVCU_DECCFG_DataChanInfo;

 //解码通道信息，除非注明可写，否则其成员只读
typedef struct _BVCU_DECCFG_DecoderParam {
    int  iDecChanIndex; // 解码通道号 从0开始，BVCU_DECCFG_DeviceInfo.iDecNums-1
    int  bDecodeEnable; // 解码起停标志，0-停止，1-主动解码，2-远程回放（暂不实现），可写
    int  iPoolTime;     // 轮询间隔 单位：毫秒  可写

    int  iDataChanMaxCount; // 当前解码通道支持的数据通道数 最大值为BVCU_MAX_TOUR_IPCHANNEL_COUNT
    int  iDataChanCount;    // 添加到列表的数据通道数量；即pDataChanInfo数组大小。  最大值为iDataChanMaxCount，可写
    BVCU_DECCFG_DataChanInfo* pDataChanInfo; //配置的数据通道列表，数组列表顺序为多路轮询的顺序。可写

    // 解码通道状态
    int  iDecodeStatus;   //当前状态:0:未启动，1-正在主动解码，2-正在远程回放（暂不实现）
}BVCU_DECCFG_DecoderParam;


// 显示通道状态
typedef struct _BVCU_DECCFG_DisplayStatus
{
    int  iDispStatus;     // 显示状态：0：未显示，1：启动显示
    int  iDispFormat;     // 显示接口类型： 见 BVCU_DISPLAY_FORMAT_*
    int  iVideoFormat;    // 视频制式: 见 BVCU_VIDEOFORMAT_*
    int  iWindowMode;     // 画面模式: 参考BVCU_DECCFG_DisplayParam.iWindowModeSup
    int  iFpsDisp[BVCU_DEC_MAX_WINDOWS];  // 每个子画面的显示帧率 单位1/10000帧每秒
    int  bFullScreenMode; // 屏幕模式  0-普通 1-全屏模式
}BVCU_DECCFG_DisplayStatus;

//显示通道信息，除非注明可写，否则其成员只读
typedef struct _BVCU_DECCFG_DisplayParam{
    int  iDispChanIndex; // 显示通道号 从0开始，BVCU_DECCFG_DeviceInfo.iDispNums-1
    int  iAudioWindowIdx;// 音频开启子窗口,对应 iDecChanIndex下标, -1: 关闭音频。 可写
    int  iPlaybackVolume;//播放音量，正常范围0～100 可写
    int  iFullWindowIdx;// 全屏某个子窗口,对应 iDecChanIndex下标, -1: 不全屏子窗口。 可写
    int  iDispFormatSup; // 支持的输出模式, 见 BVCU_DISPLAY_FORMAT_*
    BVCU_ImageSize stResolution; // 分辨率，根据iDispFormatSup从BVCU_DECCFG_DeviceInfo能力集获取。 可写
    int  iVideoFormat;      // 见 BVCU_VIDEOFORMAT_*
    char iWindowModeSup[12];// 支持的画面模式。1,2,4,6,8,9,10,13,14,16,17,19,22,25，请参考各厂家客户端窗口布局。0: 无效值。
    int  iWindowMode;       // 当前使用的画面模式。值意义同iWindewModeSup。从iWindewModeSup中取。可写
    int  iDecChanIndex[BVCU_DEC_MAX_WINDOWS]; // 各个子窗口关联的解码通道号。-1：无效值，没有关联解码通道的子窗口填-1。可写
                            // 不可重复关联解码通道号，各终端都需要检查。

    BVCU_DECCFG_DisplayStatus stStatus; // 解码通道当前状态
}BVCU_DECCFG_DisplayParam;

// 解码器 设置选项参数
typedef struct _BVCU_DECCFG_Options {
    int iTimeOut;// 会话命令等超时时间，单位毫秒。
    int iSMSRetryCount; // 服务器断线重连次数： -1：不断线重连。 0：断线一直尝试重连。或 >0（尝试次数）
    int iChanRetryCount;// 数据通道断线重连次数： 值意义同 iSMSRetryCount
    int iRetryDelay;    // 断线重连时间间隔。 单位:毫秒 参考值 60*1000
    int iCaptureVolume; // 音频采集音量，正常范围0～100
    int bOSD;      // 是否在显示中叠加信息。0-否 1-是

    int iDelayMax; // A/V数据从接收到输出，最大允许的延迟，不要超过10 000。单位：毫秒（参考值：5000）
    int iDelayMin; // A/V数据从接收到输出，最小允许的延迟，不要超过10 000。单位：毫秒（参考值：1000）
    int iDelayVsSmooth; // 播放延迟与平滑选择。取值范围1～10，越大则播放越平滑，但延迟变大（参考值：3）
}BVCU_DECCFG_Options;


#endif

