#ifndef __PU_CONFIG_H__
#define __PU_CONFIG_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
#include "SAVCodec.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!
//

// 设备PU ID范围
enum{
    // 注意：PU ID为32位，ID范围规定其高4位值。
    BVCU_PUID_NORMAL = 0x00, // 板卡设备
    BVCU_PUID_ANDROID = 0x10,// android移动设备
    BVCU_PUID_IPHONE = 0x20, // iphone移动设备
    BVCU_PUID_GATEWAY = 0x30,// 网关代理设备
};

//设备信息，除非注明可写，否则其成员只读
typedef struct _BVCU_PUCFG_DeviceInfo{
    char szID[BVCU_MAX_ID_LEN+1];                // 设备ID
    char szManufacturer[BVCU_MAX_NAME_LEN+1];    // 制造商名字
    char szProductName[BVCU_MAX_NAME_LEN+1];     // 产品名
    char szSoftwareVersion[BVCU_MAX_NAME_LEN+1]; // 软件版本
    char szHardwareVersion[BVCU_MAX_NAME_LEN+1]; // 硬件版本
    int  iPUType;          // BVCU_PUTYPE_*
    int  iLanguage[BVCU_MAX_LANGGUAGE_COUNT];    // 支持的语言列表。BVCU_LANGUAGE_*
    int  iLanguageIndex;  // 当前使用的语言索引。可写
    char szName[BVCU_MAX_NAME_LEN+1];// 名字。可写
    int  iWIFICount;      // WIFI数目
    int  iRadioCount;     // 无线模块数目
    int  iChannelCount;   // 音视频通道数
    int  iVideoInCount;   // 视频输入数
    int  iAudioInCount;   // 音频输入数
    int  iAudioOutCount;  // 音频输出数
    int  iPTZCount;       // PTZ数
    int  iSerialPortCount;// 串口数 
    int  iAlertInCount;   // 报警输入数
    int  iAlertOutCount;  // 报警输出数
    int  iStorageCount;   // 存储设备数
    int  iGPSCount;       // GPS设备数
    int  bSupportSMS;     // 是否支持手机短信功能。0-不支持，1-支持

    int  iPresetCount; // 支持的PTZ预置点数目
    int  iCruiseCount; // 支持的PTZ巡航点数目
    int  iAlarmLinkActionCount; //支持的报警联动数目

    // PU位置
    int  iLongitude; // 经度，东经是正值，西经负值，单位1/10000000度。大于180度或小于-180度表示无效值
    int  iLatitude;  // 纬度，北纬是正值，南纬是负值，单位1/10000000度。大于180度或小于-180度表示无效值

    char szBluetoothAddr[BVCU_MAX_ID_LEN + 1];  // 设备蓝牙地址，空字符表示设备没有蓝牙模块（不支持蓝牙），格式：ff:ff:ff:ff:ff:ff

    int  iReserved[8];
}BVCU_PUCFG_DeviceInfo;

/*
描述PU的通道媒体信息
约定：每个通道，通道号与相关子设备号关系为： iDevIndex = iChannelIndex - BVCU_SUBDEV_INDEXMAJOR_MIN_*
如：音视频通道3 对应的 videoIn/audioIn/audioOut的子设备号都为 (3-0) = 3
如：串口通道0x010100 对应的串口设备号为：(0x010100 - 0x010100) = 0
*/
typedef struct _BVCU_PUCFG_ChannelInfo{
    char   szName[BVCU_MAX_NAME_LEN + 1]; // Channel Name  可写
    int    iChannelIndex;  // PU通道号
    char   iMediaDir;      // PU每个通道的流体流方向。参见BVCU_MEDIADIR_* 
    char   iPTZIndex;      // PU每个通道的云台索引。-1表示无云台

    char   cReserved[2];// 对齐
}BVCU_PUCFG_ChannelInfo;

// 通道信息：被 BVCU_MSGCONTENT_TYPE_PU_CHANNELINFO（上下线通知）/BVCU_SUBMETHOD_GETPULIST（获取PU列表）使用
typedef struct _BVCU_PUCFG_PUChannelInfo {
    char  szPUID[BVCU_MAX_ID_LEN + 1];    // PU ID
    char  szPUName[BVCU_MAX_NAME_LEN + 1];// PU Name
    int   iChannelCount; // pChannel数组成员个数(PU通道数)
    BVCU_PUCFG_ChannelInfo* pChannel;  //PU通道信息数组。

    int   iOnlineStatus;  // 在线状态，参见BVCU_ONLINE_STATUS_*。
    int   iOnlineThrough; // 上线途径,BVCU_PU_ONLINE_THROUGH_*的某个值，iOnlineStatus=BVCU_ONLINE_STATUS_ONLINE时有效。
    BVCU_WallTime stOnlineTime;// 上线时间，iOnlineStatus在线时有意义（平台填写)  UTC时间 
    BVCU_WallTime stBootTime;  // 设备开机时间，平台填写，设备上线时通过iBootDuration上报开机时长（秒），  UTC时间
    int   iBootDuration;  // 设备开机运行了多长时间，设备上报，设备上线时上报设备已经开机多长时间， 单位 （秒）
    int   iStreamPathWay; // 数据流传输路径，见BVCU_STREAM_PATHWAY_*，值来自BVCU_PUCFG_RegisterServer.iStreamPathWay
    BVCU_ENCFG_Candidate stIPAddr; // 设备上线地址信息（注意网络序的值)。

    // 下面的信息来自于BVCU_PUCFG_DeviceInfo,PU注册上报时以BVCU_PUCFG_DeviceInfo为准。
    unsigned char iAVCount;        // 音视频通道数
    unsigned char iVideoInCount;   // 视频输入数
    unsigned char iAudioInCount;   // 音频输入数
    unsigned char iAudioOutCount;  // 音频输出数
    unsigned char iPTZCount;       // PTZ数
    unsigned char iSerialPortCount;// 串口数 
    unsigned char iAlertInCount;   // 报警输入数
    unsigned char iAlertOutCount;  // 报警输出数
    unsigned char iStorageCount;   // 存储设备数
    unsigned char iGPSCount;       // GPS设备数
    unsigned char iReserve[2];     // 保留对齐
    //PU位置，GPS坐标
    int  iLongitude; // 经度，东经是正值，西经负值，单位1/10000000度。大于180度或小于-180度表示无效值
    int  iLatitude;  // 纬度，北纬是正值，南纬是负值，单位1/10000000度。大于180度或小于-180度表示无效值
}BVCU_PUCFG_PUChannelInfo;

// 通道当前传输状态（针对通道）
typedef struct _BVCU_PUCFG_ChannelTransmit{
    int  iChannelIndex;  // PU通道号
    int  iDialogCount;   // 创建的会话数
    struct TransmitDialog{
        int iStreamType; // 流类型。BVCU_ENCODERSTREAMTYPE_*。 -1：表示自动
        int iTransmitPathWay;  // 数据流传输路径，见BVCU_STREAM_PATHWAY_*
        int iTransmitMediaDir; // 打开的媒体流方向。参见BVCU_MEDIADIR_* 
        int iAudienceCount; // 观看者个数。P2P传输时为1，VTDU转发时为具体分发数。
        // 同BVCU_UCFG_User_Online.iApplierID,P2P时通过iAudienceApplierID可以查到用户详细信息。见BVCU_SUBMETHOD_USER_ONLINEINFO
        int iAudienceApplierID; // CMS分配的观看者登录标识。P2P：观看者ApplierID。VTDU：VTDU的ApplierID。
    }* pDialogList;      // 创建的会话信息
}BVCU_PUCFG_ChannelTransmit;

// 升级固件状态
typedef struct _BVCU_PUCFG_UpdateStatus{
    int  iPercent;// 升级百分比，0～100,-1表示不在升级状态
    int  iSpeed;  // 下载速度，单位KB/s。 < 0 表示升级出错，见BVCU_RESULT_E_* 。
    char szSoftwareVersion[BVCU_MAX_NAME_LEN+1]; // 当前软件版本
    BVCU_WallTime stUpdateFinishTime;// 当前软件版本升级完成时刻    
}BVCU_PUCFG_UpdateStatus;

typedef struct _BVCU_PUCFG_UPGRADE_{
    char szFTPID[BVCU_MAX_ID_LEN+1]; // 指定的升级ftp服务器
    char szFilePath[BVCU_MAX_FILE_NAME_LEN+1]; // 指定的文件路径
    char szFileName[BVCU_MAX_NAME_LEN+1]; // 指定的文件名称
    int  bPromptly; // 是否立即升级，0-下次启动时再升级  1-立即升级
}BVCU_PUCFG_Upgrade;

// 设备组下的PU描述信息
typedef struct _BVCU_PUCFG_GroupPU{
    char  szPUID[BVCU_MAX_ID_LEN+1]; // PU ID
}BVCU_PUCFG_GroupPU;

// 设备组列表
typedef struct _BVCU_PUCFG_GroupItem{
    char szID[BVCU_MAX_ID_LEN+1]; // 组的id标识符

    char szName[BVCU_MAX_NAME_LEN+1]; // 组的名称 

    char szParentID[BVCU_MAX_ID_LEN+1]; // 组的上级组, 顶层组的此值为空
}BVCU_PUCFG_GroupItem;

// 设备组信息
typedef struct _BVCU_PUCFG_GroupInfo{
    char szID[BVCU_MAX_ID_LEN+1]; // 组的id标识符，创建时可以为空，成功回调通知ID

    char szName[BVCU_MAX_NAME_LEN+1]; // 组的名称 

    char szParentID[BVCU_MAX_ID_LEN+1]; // 组的上级组, 顶层组的此值为空
    
    char szDescription[BVCU_MAX_SEDOMAIN_NAME_LEN+1]; //  描述该组的一些信息

    int  iPUCount; // PU组下PU数目

    BVCU_PUCFG_GroupPU* pPU; // PU组下PU数组
}BVCU_PUCFG_GroupInfo;

// 电源参数
typedef struct _BVCU_PUCFG_Power{
    int iTurnOffDelay;// 定时关机延时。单位秒
    int bEnableTimer; // 是否允许定时开关机。0-不允许，1-允许
    BVCU_DayTimeSlice stTurnOn[7][BVCU_MAX_DAYTIMESLICE_COUNT];// 每周7天中的每天开机时刻，该时刻之外的时间认为是关机。每个时间片时长不能低于10分钟！！！
    // 电池相关信息 
    int iBatteryPower;  // 电池当前电量，单位 1/1000百分比。例如：电量100%时iBatteryPower=100*1000. 只读
    int iBatteryStatus; // 电池当前状态，0：放电 1：充电。只读
    int bLowPowerAlarm; // 是否启用电池低电量报警。0-不报警，1-报警
    int iMinPowerAlarm; // 电池低电量报警阎值。单位 1/1000百分比（同iBatteryPower）。

    int iReserved[6];   // 保留内存对齐，填0。
}BVCU_PUCFG_Power;

// 摄像头采集参数
typedef struct  _BVCU_PUCFG_VideoColorCtl{
    BVCU_DayTimeSlice stTime;   // 时间片
    unsigned char cBrightness;  // 亮度,取值范围[0,255]
    unsigned char cContrast;    // 对比度,取值范围[0,255]
    unsigned char cSaturation;  // 饱和度,取值范围[0,255]
    unsigned char cHue;         // 色调,取值范围[0,255]
}BVCU_PUCFG_VideoColorCtl;

// 图像曝光配置
typedef struct _BVCU_PUCFG_ImageExposure {
    int iMode;           // 球机的曝光模式,0-手动模式，1-自动曝光，2-光圈优先，3-快门优先，4-增益优先
    int iShutter;        // 快门时间，单位秒 N表示1/N秒 [1,100000] (手动模式和快门优先模式下有效) 
    int iGain;           // 增益倍数，为实际取值*100的值 [0,100000] (手动模式和增益优先模式下有效)
    int iIris;           // 光圈大小，0-关 1000-全开 [0,1000] (手动模式和光圈优先模式下有效)
    int iMaxShutter;     // 最大快门值, [1,100000], (手动模式和快门优先模式下无效 其它模式下有效)
    int iMinShutter;     // 最小快门值, [1,100000], (手动模式和快门优先模式下无效 其它模式下有效)
    int iMaxGain;        // 最大增益限制，曝光补偿，为实际取值*100的值，[0,100000], (手动模式和增益优先模式下无效 其它模式下有效)
    int iMinGain;        // 最小增益限制，曝光补偿，为实际取值*100的值，[0,100000], (手动模式和增益优先模式下无效 其它模式下有效)
    int iMaxIris;        // 最大光圈限制值 [0,1000], (手动模式和光圈优先模式下无效 其它模式下有效)
    int iMinIris;        // 最小光圈限制值 [0,1000], (手动模式和光圈优先模式下无效 其它模式下有效)
    int iAllowReduceFrame; // 曝光不足时允许降帧 [0,1]
    int iAntiFlickerFreq;  // 抗闪频率 0-不抗闪 1~255-抗闪频率 [0,255]
} BVCU_PUCFG_ImageExposure;

// 运动检测
#define BVCU_PUCFG_MAX_MD_COUNT 16
typedef struct _BVCU_PUCFG_MotionDetect{
    int bSupport;// 0-不支持，1-支持。只读
    BVCU_ImageRect rcROI[BVCU_PUCFG_MAX_MD_COUNT];// 区域，以16x16为单位，
    int iSensitivity;// 灵敏度，范围0~10,0表示不检测，越大的值越灵敏
    int iInterval;   // 每次检测时间间隔，单位毫秒。大的间隔可以节省计算时间，对使用电池供电的设备，还可以延长电池使用寿命
    // 但有可能漏检运动事件。设为0则每帧视频都做检测。
}BVCU_PUCFG_MotionDetect;

// 视频区域遮盖
#define BVCU_PUCFG_MAX_SHELTER_COUNT 4
typedef struct _BVCU_PUCFG_VideoShelter{ //
    int bSupport;// 0-不支持，1-支持。只读
    BVCU_ImageRect rcROI[BVCU_PUCFG_MAX_SHELTER_COUNT];// 区域，以16x16为单位，
}BVCU_PUCFG_VideoShelter;

// 镜头遮挡检测
#define BVCU_PUCFG_MAX_VIDEOOCCLUSION_COUNT 4
typedef struct _BVCU_PUCFG_VideoOcclusionDetect{ //
    int bSupport;// 0-不支持，1-支持。只读
    BVCU_ImageRect rcROI[BVCU_PUCFG_MAX_VIDEOOCCLUSION_COUNT];// 区域，以16x16为单位，
}BVCU_PUCFG_VideoOcclusionDetect;

// 视频输入
enum{
    // 注意：格式1/2/9/10/11/12年月日之间的分隔符由BVCU_PUCFG_VideoIn.cOSDTimeSplitChar决定
    BVCU_OSD_TIMEFORMAT_INVALID = 0,// 不叠加时间
    BVCU_OSD_TIMEFORMAT_1,// YYYY-MM-DD hh:mm:ss
    BVCU_OSD_TIMEFORMAT_2,// YYYY-MM-DD 星期W hh:mm:ss
    BVCU_OSD_TIMEFORMAT_3,// DD日MM月YYYY年 hh:mm:ss
    BVCU_OSD_TIMEFORMAT_4,// DD日MM月YYYY年 星期W hh:mm:ss
    BVCU_OSD_TIMEFORMAT_5,// MM月DD日YYYY年 hh:mm:ss
    BVCU_OSD_TIMEFORMAT_6,// MM月DD日YYYY年 星期W hh:mm:ss
    BVCU_OSD_TIMEFORMAT_7,// YYYY年MM月DD日 hh:mm:ss
    BVCU_OSD_TIMEFORMAT_8,// YYYY年MM月DD日 星期W hh:mm:ss
    BVCU_OSD_TIMEFORMAT_9,// DD-MM-YYYY hh:mm:ss
    BVCU_OSD_TIMEFORMAT_10,// DD-MM-YYYY 星期W hh:mm:ss
    BVCU_OSD_TIMEFORMAT_11,// MM-DD-YYYY hh:mm:ss
    BVCU_OSD_TIMEFORMAT_12,// MM-DD-YYYY 星期W hh:mm:ss
};

// IP通道配置
typedef struct _BVCU_PUCFG_OnvifChannelInfo {
    char szIPConfigName[BVCU_MAX_NAME_LEN + 1]; // 该ipchannel配置的名称,即标记该BVCU_CFG_IPChannelInfo作用。
    char szIPDevName[BVCU_MAX_NAME_LEN + 1]; // 通过网络协议获取的IP设备名称，只读 （如果这个IP设备没有设备名称可为空）
    char szIPDevID[BVCU_MAX_NAME_LEN + 1];// 通过网络协议获取的IP设备ID，只读 （如果这个IP设备没有设备ID可为空）
    char szAddr[BVCU_MAX_HOST_NAME_LEN + 1];
    int  iPort;
    char szUserName[BVCU_MAX_NAME_LEN + 1];
    char szPassword[BVCU_MAX_PASSWORD_LEN + 1];
    int iRemoteChannelList[BVCU_MAX_CHANNEL_COUNT]; // 远端IP设备支持的通道号 ,数组以 -1 结束
    int iRemoteChannelIndex; // 远端IP设备中的通道号
    int iTransportProtocolList[4]; // 传输协议类型列表 见iTransportProtocol, 数组以 -1 结束
    int iTransportProtocol; // 传输协议类型：  0 - tcp 1---udp  2-- 多播， -1--auto(自动)
    int iTransStreamModeList[4];   // 传输码流类型： 0-- 自动 1 -- 主码流  2 -- 子码流 ,数组以 -1 结束
    int iTransStreamModeForLocalMainStream;   // NVR本地的主码流使用远端IP设备的码流类型， 默认是local的主码流对应remote的主码流
    int iTransStreamModeForLocalSubStream; // NVR本地的子码流使用远端IP设备的码流类型， 默认是local的子码流对应remote的子码流
}BVCU_PUCFG_OnvifChannelInfo;

typedef struct _BVCU_PUCFG_IPChannelInfo {
    int bEnable;
    int iStatus; // IP 通道是否在线  0 -- 不在线  1--- 在线
    int iIntervalTime;  // 轮询时间 ，单位秒 ， （0--- 代表永久连接）
    int iNetProtocol;   // 网络通信协议： BVCU_IPC_NET_PROTO_TYPE_xx
    union {
        BVCU_PUCFG_OnvifChannelInfo stOnvifChannelInfo;
    }stIpChParam;
} BVCU_PUCFG_IPChannelInfo;

typedef struct _BVCU_PUCFG_IPChannelConfig {
    int bEnable; // 是否 0-不使能  1-使能 （才开启网络IPC发现/连接等功能，下次查询可获得发现设备列表）
    int iConnectType; // 0 -- 单连接   1---多路轮询连接
    int iIPChannelConnectCount; // 添加到连接列表的IP通道数量；  最大值为BVCU_MAX_TOUR_IPCHANNEL_COUNT
    BVCU_PUCFG_IPChannelInfo *pstIPChannelConnectList; // 连接列表顺序为多路轮询的顺序。 
    int iIPChannelFindCount;    // 被发现但没有添加到连接列表的IP通道数量；
    BVCU_PUCFG_IPChannelInfo *pstIPChannelFindList; // 还没添加到连接列表中的IP 通道。  
} BVCU_PUCFG_IPChannelConfig;

// CMOS摄像头配置
typedef struct _BVCU_PUCFG_CMOSChannelInfo {
    int  iCMOSIndex;                    // CMOS设备编号，从0开始累加。
    char stName[BVCU_MAX_NAME_LEN + 1]; // CMOS设备名称，用于给用户辨别CMOS设备。
} BVCU_PUCFG_CMOSChannelInfo;
typedef struct _BVCU_PUCFG_CMOSChannelConfig {
    int iConnectIndex;          // 当前使用的CMOS设备编号。
    int iCMOSCount;             // pstCMOSChannelList数组大小；  最大值为BVCU_MAX_TOUR_IPCHANNEL_COUNT
    BVCU_PUCFG_CMOSChannelInfo *pstCMOSChannelList; // CMOS设备列表，按设备编号排序。 
} BVCU_PUCFG_CMOSChannelConfig;

// 通道输入硬件连接
typedef struct  _BVCU_PUCFG_ChannelDevConnectCfg {
    int szChannelConnectTypeOption[4]; // 设备支持的视频输入源类型。每个类型最多有1个。 BVCU_CHANNEL_DEV_CONNECT_TYPE
    int iChannelConnectType; // 当前使用的视频输入源类型。
    int iCurrentConnectTag;  // 当前获取到配置的tag，数据源端生成，配置端必须原样返回。// 0~7位表示子序号，8~15位表示视频输入源类型。16~31位设备端使用。

    union {
        BVCU_PUCFG_CMOSChannelConfig stCMOSChannelConfig;
        BVCU_PUCFG_IPChannelConfig stIPChannelConfig;
    } stParam[4];
} BVCU_PUCFG_ChannelDevConnectCfg;

// 视频输入配置
typedef struct  _BVCU_PUCFG_VideoIn{
    BVCU_PUCFG_VideoColorCtl stVCC[2];// 色彩控制。0表示白天的配置，1表示晚上的配置

    BVCU_PUCFG_MotionDetect stMD;// 运动检测
    BVCU_PUCFG_VideoShelter stShelter;// 区域遮盖
    BVCU_PUCFG_VideoOcclusionDetect stOcclusion; // 镜头遮挡检测

    BVCU_VideoFormat stVideoFormat[4];    
    int iVideoFormatIndex;// 当前视频制式索引。 -1 表示由前端自动检测选择视频制式和分辨率。

    BVCU_PUCFG_ChannelDevConnectCfg stChannelDevConnectCfg; // video in 硬件连接配置

    // 叠加图标。
    int  iOSDIcon;// -1表示设备不支持叠加图标，==0表示支持但目前没有叠加，==1表示支持并且已叠加
    char szOSDIcon[BVCU_MAX_FILE_NAME_LEN + 1];// 图片的本地路径,查询时无意义。为空表示不更新图片，否则把本地图片上传到PU
    BVCU_ImagePos stOSDIconPos; // 位置。
                                // 叠加文字
    char szOSDTitle[BVCU_MAX_NAME_LEN + 1];// 文字内容
    BVCU_ImagePos stOSDTitlePos; // 位置
    char  iOSDTitleFontSize; // 字体大小
                             // 叠加时间
    char  cOSDTime;// 叠加时间格式，BVCU_OSD_TIMEFORMAT_*
    char  cOSDTimeSplitChar;// YYYY-MM-DD 之间的分隔符。常见的是'.' '-'和'/'。
    char  iOSDTimeFontSize; // 字体大小
    BVCU_ImagePos stOSDTimePos; // 叠加时间 位置
    BVCU_ImagePos stOSDGPSPos;  // 叠加GPS 位置
    BVCU_ImagePos stOSDNamePos; // 叠加通道名 位置
    char  iOSDGPSFontSize; // 叠加GPS 字体大小
    char  iOSDNameFontSize; // 叠加通道名 字体大小
    int   iReserved[4];
}BVCU_PUCFG_VideoIn;

typedef struct _BVCU_PUCFG_AudioVqe {
    int  iEnableNoise;     // 0 -- 关闭降噪功能， 1 -- 打开降噪功能
    char iNoiseLevel[4];   // 可选降噪等级，-1 表示 无效值， 只读
    int  iNoiseLevelIndex; // 当前使用的降噪等级索引
} BVCU_PUCFG_AudioVqe;
typedef struct  _BVCU_PUCFG_AudioIn{
    BVCU_PUCFG_ChannelDevConnectCfg stChannelDevConnectCfg;// audio in 硬件连接配置

    int iInput;// BVCU_AUDIOIN_INPUT_*
    int iChannelCount; // 声道数
    int iSamplesPerSec;// 采样率
    int iBitsPerSample;// 采样精度
    int iVolume;// 音量

    BVCU_PUCFG_AudioVqe stVqe;  // 音频处理
    int iEnableHalfDuplex;      // 是否开启半双工，默认关闭，0：关闭，1：开启
}BVCU_PUCFG_AudioIn;

// 音频输出
typedef struct _BVCU_PUCFG_AudioDecoderParam{
    int bEnable;// 0-禁止，1-使能
    SAVCodec_ID iAudioCodecAll[4];// 支持的音频解码器ID。只读
    int iAudioCodecIndex;// 当前使用的音频解码器
}BVCU_PUCFG_AudioDecoderParam;

typedef struct  _BVCU_PUCFG_AudioOut{
    BVCU_PUCFG_AudioDecoderParam stADParam;// 音频解码参数。
    int iChannelCount; // 声道数
    int iSamplesPerSec;// 采样率
    int iBitsPerSample;// 采样精度
    int iVolume;// 音量
    int iReserved[4];
}BVCU_PUCFG_AudioOut;

// RTMP推流输出配置
typedef struct _BVCU_PUCFG_RtmpParam {
    int bEnable;      // 是否启用RTMP推流 0-否 1-是
    int iOnlineTime;  // 本次已推送时长 单位：s(秒)， 只读
    int iRealTimeRate;// 实时速率  单位：bps, 只读
    SAV_TYPE_INT64 iTotalData;// 本次已推送总数据量  单位：byte（字节）， 只读
    char szServerAddr[512];   // RTMP推流服务器地址（URL地址，默认为空）
    int bActive;              // 是否正在推流
    int iStatus;  // 当前状态 BVCU_Result. 
                  // 常见： BVCU_RESULT_S_OK：正常，BVCU_RESULT_E_DISCONNECTED:网络断开，
                  // BVCU_RESULT_E_IO：网络拥塞，BVCU_RESULT_E_CONNECTFAILED:无法连接RTMP服务器
} BVCU_PUCFG_RtmpParam;

// 通道音视频协议扩展输出（其它协议支持） 针对通道
typedef struct  _BVCU_PUCFG_ChannelNetProtoOut {
    BVCU_PUCFG_RtmpParam stRtmpParam;  // Rtmp协议输出音视频配置。
}BVCU_PUCFG_ChannelNetProtoOut;

// 报警输入
enum {
    BVCU_ALERTIN_TYPE_INVALID = -1,
    BVCU_ALERTIN_TYPE_NORMAL = 0, // 常规继电器报警输入  负载 stNormal
    BVCU_ALERTIN_TYPE_TEMPERATURE = 1, // 温度报警输入  负载 stTemperature
    BVCU_ALERTIN_TYPE_VOLTAGE = 2,     // 电压报警输入  负载 stVoltage
};
enum {
    BVCU_ALERTIN_HARDWARE_NORMAL = 0, // 常规硬件连接  负载 NULL
    BVCU_ALERTIN_HARDWARE_485,        // 485串口连接   负载 BVCU_PUCFG_SerialPort  .iType BVCU_PTZ_PROTO_
    BVCU_ALERTIN_HARDWARE_232,        // 232串口连接   负载 BVCU_PUCFG_RS232
};

typedef struct _BVCU_PUCFG_AlertIn_Normal{
    int bType;// 报警触发模式：0-闭路报警 1-开路报警
    int iInterval;// 检测间隔，单位秒。检测到报警输入后，经过iInterval秒再次检测
}BVCU_PUCFG_AlertIn_Normal;
typedef struct _BVCU_PUCFG_AlertIn_Temperature
{
    int bEnable;  // 1 启用， 0 禁用
    int iCurTemperature;  // 当前温度，单位 1/1000 摄氏度（oC)，(只读）
    int iMinTemperature;  // 低温报警阎值，单位 1/1000 摄氏度
    int iMaxTemperature;  // 高温报警阎值， 单位 1/1000 摄氏度
    int iDelay;  // 报警延续时间，单位毫秒
}BVCU_PUCFG_AlertIn_Temperature;
typedef struct _BVCU_PUCFG_AlertIn_Voltage{
    int bEnable;  // 1 启用， 0 禁用
    int iCurVoltage;  // 当前电压，单位 1/1000 伏(V)，(只读）
    int iMinVoltage;  // 低电压报警阎值， 单位 1/1000 伏
    int iMaxVoltage;  // 高电压报警阎值， 单位 1/1000 伏
    int iDelay;  // 报警延续时间，单位毫秒
}BVCU_PUCFG_AlertIn_Voltage;

typedef struct  _BVCU_PUCFG_AlertIn{
    int iMode;  // 工作模式：1-报警模式 （目前只有报警模式，后期会有枚举）
    int iStatus;// 当前报警状态：0-无报警 1-在报警 （只读）
    int iAlertCount; // 报警计数，从stBeginCount时刻开始（设置为-1表示清空计数，PU重置开始时间）
    char szName[BVCU_MAX_NAME_LEN + 1]; // 报警输入名称
    BVCU_WallTime stBeginCount;// 开始统计报警时间
    BVCU_DayTimeSlice stWeek[7][BVCU_MAX_DAYTIMESLICE_COUNT];// 一周中的布防时间片段

    int iAlertInType; // 见 BVCU_ALERTIN_TYPE_* (只读）
    void* pAlertData; // 报警输入的参数负载，见 BVCU_ALERTIN_TYPE_* 说明

    int iHardwareType; // 硬件连接方式 见 BVCU_ALERTIN_HARDWARE_* 说明 (只读)
    void* pHardwareParam; // 硬件对应的配置参数， 见 BVCU_ALERTIN_HARDWARE_* 说明

}BVCU_PUCFG_AlertIn;

// 报警输出
typedef struct  _BVCU_PUCFG_AlertOut{
    int iDefaultStatus; // 默认状态：0-通，1-断，2-闪烁
    int iStatus;   // 报警输出当前状态：0-通，1-断，2-闪烁
    int iDuration; // 报警输出持续时间，单位:秒，0-表示持续到永远
    int iOnTime;   // 闪烁时，接通时间，单位:毫秒
    int iOffTime;  // 闪烁时，断开时间，单位:毫秒
    char szOnName[BVCU_MAX_NAME_LEN+1];  // 接通动作别名
    char szOffName[BVCU_MAX_NAME_LEN+1]; // 断开动作别名
    char szFlicker[BVCU_MAX_NAME_LEN+1]; // 闪烁动作别名
}BVCU_PUCFG_AlertOut;

// Positioning Satellite signal type
enum {
    BVCU_PUCFG_SATELLITE_GPS = (1 << 0), // 美国GPS卫星
    BVCU_PUCFG_SATELLITE_BDS = (1 << 1), // 中国北斗卫星
    BVCU_PUCFG_SATELLITE_GLONASS = (1 << 2), // 俄罗斯格洛纳斯卫星
    BVCU_PUCFG_SATELLITE_GALILEO = (1 << 3), // 欧洲伽利略卫星
    BVCU_PUCFG_SATELLITE_QZSS = (1 << 4), // 日本准天顶卫星
};
// GPS
typedef struct  _BVCU_PUCFG_GPSParam{
    char szName[BVCU_MAX_NAME_LEN + 1]; // 模块名称  (只读）
    int  iSupportSatelliteSignal;       // 支持的卫星信号 （只读） BVCU_PUCFG_SATELLITE_*组合
    int  iSetupSatelliteSignal;         // 使用的卫星信号 BVCU_PUCFG_SATELLITE_* 组合
    int bEnable;        // 是否使能
    int iReportInterval;// 采样时间间隔，单位秒
    int iReportDistance;// 采样距离间隔，单位米
    int iReserved[2];// 保留，设置为0
}BVCU_PUCFG_GPSParam;

typedef struct  _BVCU_PUCFG_GPSData{
    BVCU_WallTime stTime;// 数据对应的时间
    int  iLongitude; // 经度，东经是正值，西经负值，单位1/10000000度
    int  iLatitude;  // 纬度，北纬是正值，南纬是负值，单位1/10000000度
    int  iHeight; // 高度，单位1/100米
    int  iAngle;  // 方向角(正北方向为原点，顺时针为正),单位1/1000度
    int  iSpeed;  // 速度(米/小时)
    int  iStarCount;    // 定位星数, <= 0时非卫星定位。
    int  bAntennaState; // 天线状态(1-好，0-坏，2-差分定位，4-RTK固定解定位，5-RTK浮点解定位，6-推算定位) 
    int  bOrientationState; // 定位状态(1-定位，0-不定位) 
    int  iSatelliteSignal;  // 卫星信号来源 BVCU_PUCFG_SATELLITE_* 组合
    int  iReserved[4];
}BVCU_PUCFG_GPSData;

typedef struct  _BVCU_PUCFG_GPSSpeedLimit{
    int  bEnable;        // 是否启用，0:不启用 1：启用
    int  bStopDetection; // 启用车辆启停检测，0：不启用 1：启用
    int  iMaxSpeed;      // 最高速度(1/1000 m/s) 零表示不限速
    int  iMinSpeed;      // 最低速度(1/1000 m/s) 零表示不限速
    int  iSpeedDelay;    // 速度持续时间（ms) 避免瞬间速度造成的误报
    int  iAlarmInterval; // 报警间隔(ms)
    int  iReserved[4];
}BVCU_PUCFG_GPSSpeedLimit;

// 叠加选项枚举 每项对应叠加配置见 BVCU_PUCFG_VideoIn
enum {
    BVCU_PUCFG_OVERLAY_TIME = (1<<0),  // 叠加 时间
    BVCU_PUCFG_OVERLAY_TEXT = (1<<1),  // 叠加 自定义文字
    BVCU_PUCFG_OVERLAY_GPS =  (1<<2),  // 叠加 GPS信息
    BVCU_PUCFG_OVERLAY_ALARM = (1<<3), // 叠加 事件信息
    BVCU_PUCFG_OVERLAY_NAME = (1<<4),  // 叠加 通道名
};

// 自动抓拍参数
typedef struct  _BVCU_PUCFG_SnapshotParam{
    BVCU_ImageSize iImageSizeAll[8];// 支持的抓拍图像分辨率。只读
    int iImageSizeIndex; // 当前使用的抓拍图像分辨率
    int iQuality;// 抓拍JPG压缩质量，取值范围1～100    
    int iSequencePicCount; // 单次连拍图片数。
    int iSequenceInterval; // 连拍的每张图片时间间隔。单位毫秒
    int iSequenceDelay;    // 一次连拍周期结束后延时时间。单位毫秒。
    int iMaxSequenceCount; // 一次开始手动抓拍命令，连续抓拍的最大周期数，然后自动停止抓拍。 0：表示不限制最大周期数。
    int iOverlay;// 叠加信息标志。BVCU_PUCFG_OVERLAY_*的组合
    int iReserved[2];// 保留，必须设置为0
}BVCU_PUCFG_SnapshotParam;

// 音频编码器参数
typedef struct _BVCU_PUCFG_AudioEncoderParam{
    SAVCodec_ID iCodecID;  // 编码器ID
    char iChannelCount[4]; // 可选声道数，0表示无效值。只读
    char iBitsPerSample[4];// 可选采样精度，0表示无效值。只读
    int iSamplesPerSec[8]; // 可选采样率，0表示无效值。只读
    int iBitRate[8];// 可选的码率，0表示无效的码率。单位bits/s。只读
    char iChannelCountIndex; // 当前使用的声道索引
    char iBitsPerSampleIndex;// 当前使用的采样精度索引
    char iSamplesPerSecIndex;// 当前使用的采样率索引
    char iBitRateIndex;// 当前使用的码率索引
}BVCU_PUCFG_AudioEncoderParam;

typedef struct _BVCU_PUCFG_EncodeRectParam {
    int bEncodeRectEnable; // 是否使能编码区域。 0-禁止，1-使能
    BVCU_RelativeSize iMinRectSize;//  编码区域最小相对分辨率，相对VideoIn原始图像坐标。只读。
    BVCU_RelativeRect iEncodeRect; //  视频编码区域，相对VideoIn原始图像坐标。里面的相对宽（高）不能小于iMinRectSize.
    BVCU_ImageSize    iEncodeSize; //  视频编码区域编码分辨率，将编码区域对应的VideoIn原始图像编码为这个分辨率。0，0表示使用原始分辨率。
}BVCU_PUCFG_EncodeRectParam;

// 编码器压缩参数
typedef struct _BVCU_PUCFG_EncoderParam{
    BVCU_DayTimeSlice stTime; // 时间片。不同的时间可以采用不同的编码参数

    // 视频编码属性
    int bVideoEnable;// 0-禁止，1-使能
    SAVCodec_ID iVideoCodecAll[4];// 支持的视频编码器ID。只读
    int iVideoCodecIndex;// 当前使用的视频编码器
    BVCU_RATECONTROL iRateControl;// 当前使用的码率控制类型，参考BVCUConst.h中 enum BVCU_RATECONTROL。
    BVCU_RATECONTROL iRateControlAll[4];// 支持的码率控制类型，参考BVCUConst.h中 enum BVCU_RATECONTROL。只读
    BVCU_ImageSize   iImageSizeAll[8];  // 可选的视频编码分辨率，全0表示无效的编码分辨率。只读
    int iImageSizeIndex;                // 当前使用的视频编码分辨率
    int iFramesPerSec;// 单位1/1000帧。例如25fps，需要设置为25*1000。该帧率不能超过BVCU_PUCFG_VideoIn.iFPSMax
    int iKeyFrameInterval;// 关键帧间隔
    int iImageQuality;// 视频编码质量，取值范围1~BVCU_VIDEO_QUALITY_COUNT
    int iKbpsLimitMin[BVCU_VIDEO_QUALITY_COUNT];// 每个质量等级对应的码率限制最小值。只读
    int iKbpsLimitMax;// 码率限制最大值，由视频分辨率/帧率决定。只读
    int iKbpsLimitCurrent;// 码率限制当前值。
    int iKbpsCustom; // 自定义码率，当自定义码率有效时( > 0)，使用自定义码率，不使用iImageQuality。
    int iOverlay;// 叠加信息标志。BVCU_PUCFG_OVERLAY_*的组合
    BVCU_PUCFG_EncodeRectParam stEncodeRect;  // 编码区域设置

    // 音频编码属性
    int bAudioEnable;// 0-禁止，1-使能
    BVCU_PUCFG_AudioEncoderParam iAudioCodecAll[4];// 支持的音频编码器ID。只读
    int iAudioCodecIndex;// 当前使用的音频编码器
    int iReserved2[3];
}BVCU_PUCFG_EncoderParam;

typedef struct _BVCU_PUCFG_EncoderStreamParam{
    int iCount;// 编码器配置的时间片个数
    BVCU_PUCFG_EncoderParam* pstParams;// 编码器配置，每个成员对应一个时间片段的设置
    int iStreamType;// 流类型。BVCU_ENCODERSTREAMTYPE_*
    int bEnableTransfer;// 是否允许传输。0-不允许，1-允许
    int iReserved[4];
}BVCU_PUCFG_EncoderStreamParam;

// 编码器通道
typedef struct  _BVCU_PUCFG_EncoderChannel{
    char szName[BVCU_MAX_NAME_LEN+1];
    int  iCount;// 支持的码流个数。只读
    BVCU_PUCFG_EncoderStreamParam* pParams;
    char iVideoInIndex; // 摄像头索引，-1表示不支持。只读
    char iAudioInIndex; // 音频输入索引，-1表示不支持。只读
    char iAudioOutIndex;// 音频输出索引，-1表示不支持。只读
    char iPTZIndex; // 云台索引。-1表示不支持。只读
    char cReserved[16];
}BVCU_PUCFG_EncoderChannel;

// 解码器通道
/*
typedef struct  _BVCU_PUCFG_DecoderChannel{

}BVCU_PUCFG_DecoderChannel;
*/

// RS232串口
typedef struct _BVCU_PUCFG_RS232{
    int   iDataBit;    // 数据位。5/6/7/8
    int   iStopBit;    // 停止位。0:1位，1：1.5位，2：2位
    int   iParity;     // 奇偶校验位。0:无，1：奇校验，2：偶校验
    int   iBaudRate;   // 波特率.常见的包括1200，2400，4800，9600，19200，38400，57600，115200等
    int   iFlowControl;// 流控。0:无，1：软流控，2：硬流控
}BVCU_PUCFG_RS232;

// 串口
typedef struct _BVCU_PUCFG_SerialPort{
    BVCU_PUCFG_RS232 stRS232;
    int iAddress;// RS485地址，如果为-1，表明不是RS485串口
    int iType;   // 0-数据传输（例如PPP拨号）;1-控制台;2-透明串口;3-外接GPS;
}BVCU_PUCFG_SerialPort;

// =======================云台相关============================
// 预置点
typedef struct _BVCU_PUCFG_Preset{
    int  iID;// 预置点号。-1表示无效，有效值从0开始
    char szPreset[BVCU_PTZ_MAX_NAME_LEN+1]; // 预置点名
}BVCU_PUCFG_Preset;
// 巡航点
typedef struct _BVCU_PUCFG_CruisePoint{
    short iPreset;// 预置点号。-1表示无效值
    short iSpeed; // 转到下一巡航点的云台速度
    int   iDuration;// 在本预置点停留时间，单位秒
}BVCU_PUCFG_CruisePoint;

// 巡航路线
typedef struct _BVCU_PUCFG_Cruise{
    int  iID;// 巡航路线号。-1表示无效，有效值从0开始
    char szName[BVCU_PTZ_MAX_NAME_LEN+1];// 巡航路线名字。未设置的巡航路线名字为空

    // 巡航路线的巡航点。约定：有效的巡航点放在数组前面，数组中第一个无效巡航点之后的点都被认为是无效点
    BVCU_PUCFG_CruisePoint stPoints[BVCU_PTZ_MAX_CRUISEPOINT_COUNT];
}BVCU_PUCFG_Cruise;

// 云台属性
typedef struct _BVCU_PUCFG_PTZAttr{
    int iPTZProtocolAll[BVCU_PTZ_MAX_PROTOCOL_COUNT];  // 支持的所有协议列表。BVCU_PTZ_PROTO_*。只读
    int iPTZProtocolIndex;   // 当前使用的PTZ协议索引

    int iAddress;      // 485地址，范围0～255。可写。
    BVCU_PUCFG_RS232 stRS232;  // 232串口属性。可写

    // 是否批量更改预置点。0-不更改，1-更改。不更改时szPreset的内容被忽略。 仅在配置命令时有意义，查询时无意义
    int bChangePreset;

    // 是否批量更改巡航路线。0-不更改，1-更改。不更改时stCruise的内容被忽略。仅在配置命令时有意义，查询时无意义
    int bChangeCruise;

    // 预置列表。
    // 查询时返回所有预置点，设置时的作用是批量改预置点的名字和删除预置点
    // 注意：预置点的位置只能用BVCU_PTZ_COMMAND_PRESET_SET命令设置
    // 约定：所有有效的预置点放在数组最前面，如果总数目不到BVCU_PTZ_MAX_PRESET_COUNT，则第一个无效的Preset的iIndex为-1
    BVCU_PUCFG_Preset stPreset[BVCU_PTZ_MAX_PRESET_COUNT]; 

    // 巡航路线。
    // 查询时返回巡航路线。设置时批量更改巡航路线
    // 约定：所有有效的巡航路线放在数组最前面，如果总数目不到BVCU_PTZ_MAX_CRUISE_COUNT，第一个无效的Cruise的iIndex为-1
    BVCU_PUCFG_Cruise stCruise[BVCU_PTZ_MAX_CRUISE_COUNT];
    
    // 当前正在使用的巡航路线ID。-1表示没有活跃的巡航路线。
    int iActiveCruiseID;

    // 云台的类型，见 BVCU_PTZ_TYPE_*。只读
    // 电子云台：以上所有参数都没有意义，并只读。
    int iPTZType;
}BVCU_PUCFG_PTZAttr;

// 云台操作

// PTZ操作命令

// 注意：所有unused参数必须设置为0。
// 阈值点/巡航路线的序号从0开始，-1表示无效序号
enum {
    // 方向操作
    BVCU_PTZ_COMMAND_UP,       // 向上。iParam1：unused;iParam2: 速度;iParam3:unused
    BVCU_PTZ_COMMAND_DOWN,     // 向下。iParam1：unused;iParam2: 速度;iParam3:unused
    BVCU_PTZ_COMMAND_LEFT,     // 向左。iParam1：unused;iParam2: 速度;iParam3:unused
    BVCU_PTZ_COMMAND_RIGHT,    // 向右。iParam1：unused;iParam2: 速度;iParam3:unused
    BVCU_PTZ_COMMAND_LEFTTOP,  // 左上。iParam1：垂直速度;iParam2: 水平速度;iParam3:unused
    BVCU_PTZ_COMMAND_RIGHTTOP, // 右上。iParam1：垂直速度;iParam2: 水平速度;iParam3:unused
    BVCU_PTZ_COMMAND_LEFTDOWN, // 左下。iParam1：垂直速度;iParam2: 水平速度;iParam3:unused
    BVCU_PTZ_COMMAND_RIGHTDOWN,// 右下。iParam1：垂直速度;iParam2: 水平速度;iParam3:unused

    // 镜头操作
    BVCU_PTZ_COMMAND_ZOOM_INC,  // 增加放大倍数。iParam1：unused;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_ZOOM_DEC,  // 减小放大倍数。iParam1：unused;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_FOCUS_INC, // 焦距调远。iParam1：unused;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_FOCUS_DEC, // 焦距调近。iParam1：unused;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_APERTURE_INC, // 光圈放大。iParam1：unused;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_APERTURE_DEC, // 光圈缩小。iParam1：unused;iParam2: unused;iParam3:unused

    // 预置点操作
    BVCU_PTZ_COMMAND_PRESET_GO,  // 转到预置点。iParam1：预置点号;iParam2: 垂直速度;iParam3:水平速度
    BVCU_PTZ_COMMAND_PRESET_SET, // 把当前位置设置为预置点。iParam1：预置点号;iParam2: 预置点名;iParam3:unused
    BVCU_PTZ_COMMAND_PRESET_SETNAME, // 更改预置点名字。iParam1：预置点号;iParam2: 预置点名;iParam3:unused
    BVCU_PTZ_COMMAND_PRESET_DEL, // 删除预置点。iParam1：预置点号;iParam2: unused;iParam3:unused

    // 巡航路线操作
    BVCU_PTZ_COMMAND_CRUISE_GO,  // 启动巡航。iParam1：巡航路线号;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_CRUISE_STOP,// 停止巡航。iParam1：巡航路线号;iParam2: unused;iParam3:unused
    BVCU_PTZ_COMMAND_CRUISE_SET, // 设置整个巡航路线。iParam1：巡航路线号;iParam2: BVCU_PUCFG_CRUISE指针;iParam3:unused
    BVCU_PTZ_COMMAND_CRUISE_DEL, // 删除巡航路线。iParam1：巡航路线号;iParam2: unused;iParam3:unused

    // 辅助功能操作
    BVCU_PTZ_COMMAND_AUX,// 打开/关闭辅助功能开关，Param1：辅助号;iParam2: 0-关闭,1-开启;iParam3:unused

    // 锁操作
    // 如果锁定超过60秒后，用户没有手工解除锁定，Server会自动解除锁定。
    BVCU_PTZ_COMMAND_LOCK,// 锁定/解锁云台。iParam1：unused;iParam2: unused;iParam3:unused
};

typedef struct _BVCU_PUCFG_PTZControl{
    int iPTZCommand;    // BVCU_PTZ_COMMAND_*
    int bStop;// 0-动作开始，1-动作停止。仅对方向操作/镜头操作/锁操作有效，其他操作应该设置为0。锁操作：0-开始锁定，1-停止锁定
    int iParam1,iParam2,iParam3;// 参考BVCU_PTZ_COMMAND_*说明
    // 注意：BVCU_PTZ_COMMAND_CRUISE_SET的iParam2是个指针，网络发送/接收时应发送/接收整个BVCU_PTZ_COMMAND_CRUISE_SET结构体
}BVCU_PUCFG_PTZControl;

// =======================外围硬件==========================
enum  BVCU_Hardware_Type
{
    // 灯相关
    BVCU_HARDWARE_TYPE_INFRAREDLIGHT = 0, // 红外灯  BVCU_PUCFG_MiscLight
    BVCU_HARDWARE_TYPE_LEDLIGHT, // LED灯  BVCU_PUCFG_MiscLight
};
// 红外灯 LED灯
typedef struct _BVCU_PUCFG_Miscellaneous_Light {
    int iOpenMode;    // 开启方式：0-关闭  1-常开  2-自动
    int iPower;       // 功率：0 ~ 100
}BVCU_PUCFG_MiscLight;

// 外围硬件
typedef struct _BVCU_PUCFG_Miscellaneous_Hardware {
    int iType;    // 硬件类型，见BVCU_HARDWARE_TYPE_*
    union
    {
        char  byUnionLen[256];
        BVCU_PUCFG_MiscLight stMiscLight;
    };
}BVCU_PUCFG_MiscHardware;

// =======================网络相关============================

typedef struct _BVCU_PUCFG_Ethernet{
    int bDHCP;   // 是否使用DHCP。0-不使用；1-使用;-1-设备不支持
    int bPPPoE;  // 是否使用PPPoE。0-不使用；1-使用；-1-设备不支持。
    int bAutoDNS;// 自动获取DNS。0-不使用；1-使用。只有bDHCP=1或者bPPPoE=1才有意义

    char szIP[16];// ip地址。
    char szNetMask[16];// 子网掩码。
    char szGateway[16];// 默认网关。
    char szDNS[2][BVCU_MAX_HOST_NAME_LEN+1];// 域名服务器。

    char szPPPoEUserName[BVCU_MAX_NAME_LEN+1];// PPPoE用户名，只有bPPPoE=1才有意义
    char szPPPoEPassword[BVCU_MAX_PASSWORD_LEN+1];// PPPoE密码，只有bPPPoE=1才有意义

    int iStatus; // 当前状态： 0-未知 1-已正常连接 2-未插入网线 3-ip地址异常
    int iReserved[3];
}BVCU_PUCFG_Ethernet;

typedef struct _BVCU_PUCFG_WifiHotSpot{
    char szProviderAll[8][BVCU_MAX_NAME_LEN+1];// 支持的提供商列表。ChinaNet/ChinaMobile/ChinaUniccom
    int  iProviderIndex;// 当前使用的提供商
    char szAreaAll[64][16];// 开户地列表.通常用代码表示，例如bj-北京,ah-安徽,...
    int  iAreaIndex;// 开户地
    char szUserName[BVCU_MAX_NAME_LEN+1];// 用户名
    char szPassword[BVCU_MAX_PASSWORD_LEN+1];// 密码
}BVCU_PUCFG_WifiHotSpot;

typedef struct _BVCU_PUCFG_WifiGeneral{
    char szSSID[32];  // SSID 
    int iSecurityType;// 安全类型.BVCU_WIFI_SECURITY_TYPE_*
    int iCryptType;   // 加密类型.BVCU_WIFI_CRYPT_TYPE_*
    char szWEPKey[4][16];// WEP密钥
    char szWPAKey[64];// WPA密钥

    int bDHCP; // 是否使用DHCP。0-不使用；1-使用;-1-设备不支持
    int bPPPoE;// 是否使用PPPoE。0-不使用；1-使用；-1-设备不支持。PPPoE的用户名/密码使用BVCU_PUCFG_Ethernet中的
    int bAutoDNS;// 自动获取DNS。0-不使用；1-使用。只有bDHCP=1或者bPPPoE=1才有意义

    char szIP[16];// ip地址。
    char szNetMask[16];// 子网掩码。
    char szGateway[16];// 默认网关。
    char szDNS[2][BVCU_MAX_HOST_NAME_LEN + 1];// 域名服务器。

    int  iStatus; // 当前状态： 0-未知 1-已正常连接 2-wifi连接失败
    int  iReserved[3];
}BVCU_PUCFG_WifiGeneral;

enum {
    BVCU_WIFIAP_TYPE_UNKNOWN = 0,
    BVCU_WIFIAP_TYPE_UNENCRYPTED,// 不加密
    BVCU_WIFIAP_TYPE_WEP,        // WEP模式
    BVCU_WIFIAP_TYPE_WPA_PSK,    // WPA-PSK模式
    BVCU_WIFIAP_TYPE_WPA2_PSK,   // WPA2-PSK模式
};
typedef struct _BVCU_PUCFG_WifiAP{
    int  bEnableSetting;                      // 是否可以修改以下配置，只读选项, 1--可以修改(包括bEnable的修改)，0--不可以修改
    int  bEnable;                             // 是否使能 0-否 1：是
    int  bHideSSID;                           // 是否隐藏SSID  0-否 1：是
    char szSSID[BVCU_MAX_ID_LEN+1];           // SSID名称
    int  iSafeType;                           // 鉴权模式: 1-不加密; 2-WEP模式; 3-WPA-PSK模式; 4-WPA2-PSK模式 WEP模式我们不支持仅定义
    char szPassWord[BVCU_MAX_PASSWORD_LEN+1]; // 连接密钥
    int  iChannel;                            // 信道【1，11】默认 1
    int  bAutoChannelSelect;                  // 是否自动选择信道  
    char szIP[16];                            // IP
    char szSubMark[16];                       // 子网掩码
    int  bDHCP;                               // 是否开启DHCP。0-不使用；1-使用;-1-设备不支持
    char szDHCPStartAddr[16];                 // dhcp开始IP地址  bDHCP=1时有意义
    char szDHCPEndAddr[16];                   // dhcp结束IP地址  bDHCP=1时有意义
}BVCU_PUCFG_WifiAP;

typedef struct _BVCU_PUCFG_Wifi{
    int bEnable;// 连接AP热点是否使能：0-不使用；1-使用
    int iMode;  // 连接AP热点方式：0-普通方式，1-运营商热点方式
    int iSignalLevel;// 信号强度。0~100，0最差，100最好

    BVCU_PUCFG_WifiGeneral stGeneral; // 普通方式。只有iMode=0才有意义
    BVCU_PUCFG_WifiHotSpot stHostSpot;// 热点方式。只有iMode=1才有意义
    BVCU_PUCFG_WifiAP stWifiAP; // wifi AP设置。

    char szMacAddr[BVCU_MAX_ID_LEN + 1]; // wifi mac 地址，无效值为空字符串，格式：ff:ff:ff:ff:ff:ff
}BVCU_PUCFG_Wifi;

typedef struct _BVCU_PUCFG_RadioNetwork{
    int bEnable;// 是否使用该模块。0-不使用；1-使用
    int iTypeAll[4];// 模块支持的所有网络类型。BVCU_RADIONETWORK_TYPE_*
    int iTypeIndex; // 当前使用的网络类型
    
    char szModuleName[BVCU_MAX_NAME_LEN+1];// 模块名
    char szUserName[BVCU_MAX_NAME_LEN+1];  // 用户名。设为空表示采用默认
    char szPassword[BVCU_MAX_PASSWORD_LEN+1];// 密码

    char szAPN[BVCU_MAX_NAME_LEN+1];// APN名。设为空表示采用默认
    char szAccessNum[BVCU_MAX_NAME_LEN+1];// 接入号

    char szCardNum[BVCU_MAX_MOBILEPHONE_NUM_LEN+1];// 卡号

    int bOnline;// 是否在线。0-不在线，1-在线
    int iSignalLevel[4];// 每种网络类型对应的信号强度。0~100，0最差，100最好
    int iOnlineTime;// 上线时间，单位秒.设置为-1表示重置上线时间
    int iTrafficDownload;// 下载的网络流量总计，单位MB字节.设置为-1表示重置该网络流量
    int iTrafficUpload;// 上传的网络流量总计，单位MB字节.设置为-1表示重置该网络流量
    int iSpeedDownload;// 当前下载速度，单位KB/s
    int iSpeedUpload;  // 当前上载速度，单位KB/s

    // 流量管理相关 
    int iTrafficYear; // 本年消耗总流量，单位MB字节.设置为-1表示重置该网络流量
    int iTrafficMonth;// 本月消耗总流量，单位MB字节.设置为-1表示重置该网络流量
    int iTrafficDay;  // 本日目前消耗流量，单位MB字节.设置为-1表示重置该网络流量
    int iTrafficLastTime; // 上次上线日消耗的总流量，单位MB字节
    BVCU_WallTime stLastTime; // 上次上线日时间（UTC）
    int iMonthDay;    // 每月计费日(月流量起始日)（1~31）

    int bRebootAfterDialingFatalErr; // 拨号发生严重错误后是否重启系统，1-重启， 0-不重启
    int iReserved[4];

    char szIMEI_MEID[BVCU_MAX_ID_LEN + 1];// 设备IMEI/MEID号.  只读
    char szICCID[BVCU_MAX_ID_LEN + 1];    // 卡的ICCID号.  只读
}BVCU_PUCFG_RadioNetwork;

// 搜索 或 配对的蓝牙对象
typedef struct _BVCU_PUCFG_BluetoothObj {
    char szName[BVCU_MAX_NAME_LEN + 1];  // 蓝牙名称， 只读
    char szMacAddr[BVCU_MAX_ID_LEN + 1]; // 设备蓝牙地址，空字符表示设备没有蓝牙模块（不支持蓝牙），只读，格式：ff:ff:ff:ff:ff:ff
}BVCU_PUCFG_BluetoothObj;
// 蓝牙通信 设置
typedef struct _BVCU_PUCFG_Bluetooth{
    char szName[BVCU_MAX_NAME_LEN + 1];   // 蓝牙名称， 读写
    char szMacAddr[BVCU_MAX_ID_LEN + 1];  // 设备蓝牙地址，空字符表示设备没有蓝牙模块（不支持蓝牙），只读，格式：ff:ff:ff:ff:ff:ff

    BVCU_PUCFG_BluetoothObj stPairObj[8]; // 已配对的蓝牙对象，读写（可删除），最多支持记录8个配对对象。 遇到空蓝牙地址结束
    BVCU_PUCFG_BluetoothObj stScanObj[8]; // 当前扫描到的蓝牙对象，只读，最多支持上报8个对象。 遇到空蓝牙地址结束
    BVCU_PUCFG_BluetoothObj stConnectedObj;  // 当前连接的蓝牙对象， 读写（可以设置连接以上连个列表中的蓝牙对象）。
}BVCU_PUCFG_Bluetooth;

// 注册服务器
typedef struct  _BVCU_PUCFG_RegisterServer{
    char szAddr[BVCU_MAX_HOST_NAME_LEN+1];     // 服务器IP或域名
    int  iPort;          // 服务器端口
    int  iProto;         // 使用的协议类型，TCP/UDP
    int  iKeepAliveTimeout; // 设备心跳超时时间，单位秒。超过该时间设备收不到心跳包回复，认为服务器下线。范围： 45-200
    char szUserName[BVCU_MAX_ID_LEN + 1];   // 登录用户名
    char szPassword[BVCU_MAX_NAME_LEN + 1]; // 登录密码
    int  iStreamPathWay; // 数据流传输路径，见BVCU_STREAM_PATHWAY_*
    char cReserved[12];
}BVCU_PUCFG_RegisterServer;

// 设备升级服务器
typedef struct _BVCU_PUCFG_UpdateServer{
    char szAddr[BVCU_MAX_HOST_NAME_LEN+1];    // 服务器IP或域名
    int  iPort;                               // 端口    
    char szUserName[BVCU_MAX_NAME_LEN+1];     // 用户名
    char szPassword[BVCU_MAX_PASSWORD_LEN+1]; // 密码
    int  iProto;                              // 协议
    char szPath[BVCU_MAX_FILE_NAME_LEN+1];    // 设备固件存放路径
}BVCU_PUCFG_UpdateServer;

// Email服务器
typedef struct _BVCU_PUCFG_EmailServer{
    char szServerAddr[BVCU_MAX_HOST_NAME_LEN+1];// Email SMTP服务器地址。
    int  iServerPort;// Email服务器端口
    char szUserName[BVCU_MAX_NAME_LEN+1];// 帐号名
    char szPassword[BVCU_MAX_PASSWORD_LEN+1];// 密码
    char szSenderAddr[BVCU_MAX_HOST_NAME_LEN+1];// 发送者邮件地址
    int  bSSLEnable; // 是否使能SSL。0-不使能，1-使能    
}BVCU_PUCFG_EmailServer;

//  NTP配置
#define BVCU_MAX_NTP_SERVER_COUNT 3
typedef struct  _BVCU_PUCFG_NTPServer{
    char szAddr[BVCU_MAX_NTP_SERVER_COUNT][BVCU_MAX_HOST_NAME_LEN+1];     // NTP服务器IP或域名
    int  iPort[BVCU_MAX_NTP_SERVER_COUNT];           // NTP服务器默认端口为123
    int  iUpdateInterval;   // 更新时间间隔，单位：分钟
    int  bUpdateImmediately;// 立即同步
    char cReserved[16];
}BVCU_PUCFG_NTPServer;

// 较时设备。
typedef struct _BVCU_PUCFG_TimeSource{    
    int     iTimeZone; // 本地时区
    int  bDST;// 是否使用夏令时。0-不使用，1-使用
    BVCU_PUCFG_NTPServer stNTPServer;
    
    // 优先级大的设备会被优先使用
    char iNTP; // NTP优先级1-100, <=0表示不使用
    char iGPS; // GPS优先级1-100, <=0表示不使用。只有支持GPS的设备才有意义
    char iReserved[10];
}BVCU_PUCFG_TimeSource;

// DDNS
typedef struct _BVCU_PUCFG_DDNS{
    int bDDNS;// 动态域名。0-不使用；1-使用；-1-设备不支持
    char szDDNSProvider[BVCU_MAX_NAME_LEN+1]; // DDNS提供商。只有bDDNS=1才有意义
    char szDDNSAddr[BVCU_MAX_HOST_NAME_LEN+1];// DDNS服务器地址。只有bDDNS=1才有意义
    char szDDNSUserName[BVCU_MAX_NAME_LEN+1]; // DDNS用户名。只有bDDNS=1才有意义
    char szDDNSPassword[BVCU_MAX_PASSWORD_LEN+1];// DDNS密码。只有bDDNS=1才有意义
    char szDynamicName[BVCU_MAX_HOST_NAME_LEN+1];// 申请的动态域名。只有bDDNS=1才有意义
}BVCU_PUCFG_DDNS;

// 各种Server相关配置
typedef struct _BVCU_PUCFG_Servers{
    BVCU_PUCFG_RegisterServer stRegisterServer;
    BVCU_PUCFG_UpdateServer stUpdateServer;
    BVCU_PUCFG_TimeSource stTimeSource;
    BVCU_PUCFG_DDNS stDDNS;
    BVCU_PUCFG_EmailServer stEmailServer;
    BVCU_PUCFG_RegisterServer stReserveServer; // 备用 注册服务器
}BVCU_PUCFG_Servers;

// =======================存储============================
// 存储计划 针对通道
typedef struct _BVCU_PUCFG_Storage_Schedule{
    BVCU_DayTimeSlice stWeekSnapshot[7][BVCU_MAX_DAYTIMESLICE_COUNT];// 一周的抓拍时间片划分，每天BVCU_MAX_DAYTIMESLICE_COUNT个时间片
    BVCU_DayTimeSlice stWeekRecord[7][BVCU_MAX_DAYTIMESLICE_COUNT];  // 一周的录像时间片划分，每天BVCU_MAX_DAYTIMESLICE_COUNT个时间片
    int   bRecordAudio;// 是否录音频。0-不存储，1-存储。
    BVCU_WallTime stBegin;// 开始时间
    BVCU_WallTime stEnd;  // 结束时间。在开始/结束时间这段范围内，计划有效。
}BVCU_PUCFG_Storage_Schedule;

// 分区信息 针对存储器
typedef struct _BVCU_PUCFG_Storage_Media{
    char szMediaName[BVCU_MAX_FILE_NAME_LEN + 1];// 唯一标识该分区的名字或者路径
    char szFSName[BVCU_MAX_ID_LEN + 1]; // 文件系统格式
    int iStorageMediaType;   // 存储媒体类型。BVCU_STORAGEMEDIATYPE_*
    unsigned int iTotalSpace;// 总空间。单位MB 
    unsigned int iFreeSpace; // 剩余空间。单位MB    
    int bFormated;// 是否已格式化，0-未格式化，1-已格式化
    int iReserved[2];
}BVCU_PUCFG_Storage_Media;

// 存储规则 针对存储器
typedef struct _BVCU_PUCFG_Storage_Rule{
    int iNoSpaceRule;// 存储器满时处理规则。0-停止录像，1-覆盖旧录像
    int iAlarmSpace; // 空间不足报警门限。单位MB。设为0表示不报警
    int iReserveDays;// 录像文件保留天数
    int iRecordFileLength;// 录像文件时间长度。单位秒。
    int bRecordGPS;// 是否存储GPS信息。0-不存储，1-存储。
    int iReserved[2];
}BVCU_PUCFG_Storage_Rule;

// 初始化存储器 针对存储器。管理器应定时查询格式化进度
typedef struct _BVCU_PUCFG_Storage_Format{
    int iAction; // 0-未进行格式化，1-正在格式化；control命令时该值填1。
    int iPercent;// 查询返回的格式化进度（iAction==1时有意义）。正常值0～100。100表示格式化完毕，特殊值-1表示格式化失败
}BVCU_PUCFG_Storage_Format;

// 手工远程录像
typedef struct _BVCU_PUCFG_ManualRecord{
    int bStart; // 1-开始录像，0-停止录像
    int iLength;// 存储时间长度，单位秒， -1表示一直录像
    char szFileName[128]; // 录像文件名称。
    int iChannelIndex;//通道号, BVCU_SUBDEV_INDEXMAJOR_*
    int iFileType;    //录像文件类型，BVCU_STORAGE_FILE_TYPE_*组合
}BVCU_PUCFG_ManualRecord;

// 手工抓拍，保存到PU
typedef struct _BVCU_PUCFG_Snapshot{
    int  bStart;// 1-开始抓拍，0-停止抓拍 2-按指定周期数抓拍
    int  iCycleCount; // 指定的抓拍周期数，当bStart==2 时有意义。
    char szFTPID[BVCU_MAX_ID_LEN + 1]; // 抓拍图片上传服务器ID，如果ID为空，表示不需要上传。
}BVCU_PUCFG_Snapshot;

// 图片同步
typedef struct _BVCU_PUCFG_Picture{
    char szFTPID[BVCU_MAX_ID_LEN + 1]; // 图片上传下载  服务器ID
    int  iChannelIndex; // 同步图片的通道号
    BVCU_WallTime stBeginTime; // 同步图片的时间起点
}BVCU_PUCFG_Picture;

// 设备当前存储状态（前端存储）
typedef struct _BVCU_PUCFG_RecordStatus {
    int bRecordVideo; // 是否正在存储视频
    int bRecordAudio; // 是否正在存储音频
    int bRecordGPS;   // 是否正在存储GPS
    int bRecordImage; // 是否正在存储抓拍图像
    char* pFileNames; // 正在存储的文件路径+文件名称。多个文件间用','号隔开。
}BVCU_PUCFG_RecordStatus;
// ==========================上下线（注册）控制===========================
// 上线触发方式
enum{
    BVCU_PU_ONLINE_TRIGGER_INVALID = 0,
    BVCU_PU_ONLINE_TRIGGER_MANUAL = (1<<0),    // 手动 不再使用
    BVCU_PU_ONLINE_TRIGGER_ONTIME = (1 << 1),  // 定时
    BVCU_PU_ONLINE_TRIGGER_ONEVENT = (1 << 2), // 事件 暂时只处理 短信 和 来电
};

// 上线事件。各种事件可以组合
enum{
    BVCU_PU_ONLINE_EVENT_ALERTIN = 1<<0,// 报警输入  不再使用（移到前端报警联动中）
    BVCU_PU_ONLINE_EVENT_VIDEOMD = 1<<1,// 视频运动检测  不再使用（移到前端报警联动中）
    BVCU_PU_ONLINE_EVENT_SMS = 1 << 2,  // 短信输入
    BVCU_PU_ONLINE_EVENT_PHONE = 1 << 3,// 来电输入
};

// 上线途径
enum{
    BVCU_PU_ONLINE_THROUGH_INVALID  = 1<<0,    
    BVCU_PU_ONLINE_THROUGH_ETHERNET = 1<<1, // 以太网
    BVCU_PU_ONLINE_THROUGH_WIFI     = 1<<2, // WIFI
    BVCU_PU_ONLINE_THROUGH_RADIO    = 1<<3, // 无线
};

// 触发上线的短信配置
typedef struct _BVCU_PUCFG_OnlineEventSMS{
    char szCardNum[16][BVCU_MAX_MOBILEPHONE_NUM_LEN+1];// 允许的卡号。最多允许16个卡号。只有在列表中的卡号才会允许触发上线
    char szContent[128];// 触发内容。只有szCardNum中匹配szContent的短信才会触发
    int  bReply;// PU是否回复短信
}BVCU_PUCFG_OnlineEventSMS;

typedef struct _BVCU_PUCFG_OnlineControlOne{
    int iTrigger;// 触发方式
    int iEvent;  // 事件。仅在iTrigger == BVCU_PU_ONLINE_TRIGGER_ONEVENT时有意义
    int iOnlineTime;// 事件触发后，至少保持iOnlineTime秒在线,-1表示一直保持在线。仅在iEvent == BVCU_PU_ONLINE_TRIGGER_ONEVENT时有意义
    int iThrough;// 上线途径,BVCU_PU_ONLINE_THROUGH_*的组合
}BVCU_PUCFG_OnlineControlOne;

typedef struct _BVCU_PUCFG_OnlineControl{
    int bPowerOnOnline; // 上电即上线。1：上电即上线，设备将在上电的时候就上线（一直保持上线状态）；0： 设备将根据下面的配置判断是否上线。
    BVCU_DayTimeSlice stWeek[7][BVCU_MAX_DAYTIMESLICE_COUNT];// 一周中的时间片段
    BVCU_PUCFG_OnlineControlOne stRCO[7][BVCU_MAX_DAYTIMESLICE_COUNT];// 每个时间片端对应的上线方式
    BVCU_PUCFG_OnlineEventSMS stRESMS; // 短信触发的配置
}BVCU_PUCFG_OnlineControl;

enum {
    BVCU_OPENDIALOG_MODE_NORMAL = 0,    // 普通的打开通道请求，如果请求的被打开对象是自己，则为主动上传请求。
    BVCU_OPENDIALOG_MODE_AV_CALL        // 音视频通话
};

// PU 主动命令负载
typedef struct _BVCU_PUCFG_OpenDialog{ 
    char szID[BVCU_MAX_ID_LEN + 1]; // 会话目标ID (PU ID)
    int  iType;      // 会话类型, BVCU_OPENDIALOG_MODE_*
    char szApplyerUserID[BVCU_MAX_ID_LEN + 1];  // 请求者
    char szApplyeeUserID[BVCU_MAX_ID_LEN + 1];  // 被请求者
    char szCallID[BVCU_MAX_ID_LEN + 1];    // Call ID，由服务器填写
    int iChannelIndex; // 会话通道号
    int iStreamIndex;  // 通道下属的流号。设置为-1表示由Server决定传输哪个流。见 BVCU_ENCODERSTREAMTYPE_*
    int iAVStreamDir;  // 会话的数据流方向，已CU为方向参考。停止存储时，填 0
    int bRecord; // 客户端是否存储录像。 0-不存储 1-存储

    int iApplierID; // 命令目标用户的CMS分配登录标识， -1表示所有用户都接收该命令。
}BVCU_PUCFG_OpenDialog;

/*TODO: 
   1、设备手动升级
   2、上传报警声音/OSD图片
   3、远程抓拍
   4、下载录像
 
   1~4涉及到上传/下载协议，需要统一考虑
*/

// ========================== 车载车辆信息 ===========================
enum {
    BVPU_CAR_COLOR_YELLOW_BLACK = 0x01,  // 黄底黑字
    BVPU_CAR_COLOR_BLACK_WHITE,          // 黑底白字
    BVPU_CAR_COLOR_BLUE_WHITE,           // 蓝底白字
    BVPU_CAR_COLOR_WHITE_BLACK,          // 白底黑字
    BVPU_CAR_COLOR_WHITE_RED,            // 白底红字
};

// 车牌属性信息
typedef struct _BVCU_PUCFG_LicencePlateInfo {
    int     iCityID; // 车牌所在省份市县域简写 按照GB/T 2260中规定的6位编号为10进制数据
    int     iColor;  // BVPU_SDK_LICENCE_PLATE_COLOR_* 车牌颜色 JT/T 415-2006 中5.4.12的规定
    int     iType;   // 车牌类型 按照 JT/T 415-2006 中 5.4.9的规定
    char    szNumber[16];  // The car number 车牌号码 （原则上不包括省市部分，由用户配置，不强制）
    char    szVAO[16];    // vehicle administration office  车管所编号 一般12位
} BVCU_PUCFG_LicencePlateInfo;

typedef struct _BVCU_PUCFG_PersonInfo {
    char    szName[32];  // 姓名
    short   cGender;     // 0 男， 1 女
    short   sAge;        // 年龄
} BVCU_PUCFG_PersonInfo;

// 驾驶员信息
typedef struct _BVCU_PUCFG_DriverInfo {
    BVCU_PUCFG_PersonInfo   stPersonInfo;  // 个人基本信息
    char    szCardID[32];      // 驾驶员身份证编号
    char    szCertificateID[64];   // 驾驶员从业资格证编号
    char    szIssuingAgency[256];  // The name of issuing agency. 发证机构名称
} BVCU_PUCFG_DriverInfo;

// 车辆相关信息
typedef struct _BVCU_PUCFG_CarInfo {
    BVCU_PUCFG_LicencePlateInfo stLicencePlate; // 车辆牌照信息
    BVCU_PUCFG_DriverInfo stDriver[8]; // 驾驶员信息， 一辆车最多可以有 8 个驾驶员
    int     iDriverCount; // 驾驶员个数
    char    szVehicleType[16]; // 车辆类型， 如： 小型家用车
    char    szVIN_EngineNo[32]; // 车架号_发动机号， 如：LJ16AA3C497021629_94007558
    char    szOwner[64];  // 所有人 或 公司 名称
}BVCU_PUCFG_CarInfo;

//========================== 执法记录仪信息 ===========================
enum
{
    BVCU_POLICETYPE_UNKNOWN = 0,    // 未知
    BVCU_POLICETYPE_TRAFFIC,        // 交警
    BVCU_POLICETYPE_TOURIST,        // 旅游警察
    BVCU_POLICETYPE_PATROL,         // 巡逻警察
};
// 执法记录仪相关信息
typedef struct _BVCU_PUCFG_ZFYInfo
{
    char szDevModel[BVCU_MAX_ID_LEN + 1];     // 设备型号.  只读
    char szIMEI_MEID[BVCU_MAX_ID_LEN + 1];    // 设备IMEI/MEID号.  只读
    char szSerial[BVCU_MAX_ID_LEN + 1];       // 产品序号，不可为空. 可写
    char szUserNo[BVCU_MAX_ID_LEN + 1];       // 使用者警号，不可为空。可写
    char szUserName[BVCU_MAX_ID_LEN + 1];     // 使用者姓名。可写
    char szUserDescribe[BVCU_MAX_ID_LEN + 1]; // 使用者描述。 可写
    char szUnitNo[BVCU_MAX_ID_LEN + 1];       // 使用者单位编号。 可写
    char szUnitName[BVCU_MAX_ID_LEN + 1];     // 使用者单位名称。 可写
    char szPostName[BVCU_MAX_ID_LEN + 1];     // 使用者岗位/职位名称。 可写
    int  iPoliceType;  // 警员类型，见  BVCU_POLICETYPE_*。可写
    char szDefaultConference[BVCU_MAX_ID_LEN + 1];// 默认发言会议ID。注意可以通过BVCU_SUBMETHOD_CONF_LIST命令获取会议名称，并显示选择会议列表。 可写
    char szCurrentConference[BVCU_MAX_ID_LEN + 1];// 当前发言会议ID。 只读
}BVCU_PUCFG_ZFYInfo;

// ========================== 基站、WiFi、IP等定位信息 ===========================
// 基站信息。  只读
typedef struct _BVCU_PUCFG_BaseStation
{
    union
    {
        char  byUnionLen[32];
        // 是否是CDMA模式，由BVCU_PUCFG_BaseStation使用者判断，例如BVCU_PUCFG_Position.bCDMA。
        // CDMA模式时，使用stCDMAbts，否则使用stOtherbts。
        struct
        {
            int iSID;   // cdma 系统标识码。    必填
            int iNID;   // cdma 网络识别码。    必填
            int iBID;   // cdma 小区唯一识别码。    必填
            int iSignal;// 信号强度，取值范围-113到0dbm。如获得信号强度为正数n，转换公式：n*2 - 113。    必填
        }stCDMAbts;
        struct
        {
            int iMCC;   // 移动用户所属国家代码，默认值为 460。    必填
            int iMNC;   // 移动网号，中国移动：0；中国联通：1；。    必填
            int iLAC;   // 位置区域码，取值范围：0~65535。    必填
            int iCellID;// 基站小区编号，取值范围：0~65535，0~268435455，其中0，65535，268435455不使用，大于65535表示为3G基站。    必填
            int iSignal;// 信号强度，取值范围-113到0dbm。如获得信号强度为正数n，转换公式：n*2 - 113。。    必填
        }stOtherbts;
    };
}BVCU_PUCFG_BaseStation;
// wifi mac信息。  只读
typedef struct _BVCU_PUCFG_WifiMac
{
    char szSSID[BVCU_MAX_ID_LEN + 1];  // SSID名称。    必填
    char szMAC[BVCU_MAX_ID_LEN + 1];   // MAC地址。    必填
    int  iSignal;                      // 信号强度。0~100。    必填
    int  iReserved[3];
}BVCU_PUCFG_WifiMac;
// 定位信息。  只读
typedef struct _BVCU_PUCFG_Position
{
    int  iAccessType; // 移动端接入网络方式,BVCU_PU_ONLINE_THROUGH_*的某个值。    必填
    char szIMEI_MEID[BVCU_MAX_ID_LEN + 1];    // 设备IMEI/MEID号。    必填
    char szspflxIP[BVCU_MAX_ID_LEN + 1];      // 设备公网出口IP地址。  设备请求端填空。非必填
    char szNetServerIP[BVCU_MAX_ID_LEN + 1];  // 设备接入基站时对应的网关IP。    非必填
    char szPhoneNum[BVCU_MAX_MOBILEPHONE_NUM_LEN+1]; // 本机号码。    非必填
    // 基站信息
    char szIMSI[BVCU_MAX_ID_LEN + 1];         // 移动用户识别码。iAccessType=BVCU_PU_ONLINE_THROUGH_RADIO时必填
    int  iRadioNetworkType;                   // 无线网络类型。见：BVCU_RADIONETWORK_TYPE。iAccessType=BVCU_PU_ONLINE_THROUGH_RADIO时必填
    int  bCDMA;          //  是否是cdma。 非cdma：0；是cdma：1。iAccessType=BVCU_PU_ONLINE_THROUGH_RADIO时必填
    BVCU_PUCFG_BaseStation stConnectBts;      // 接入基站信息。iAccessType=BVCU_PU_ONLINE_THROUGH_RADIO时必填
    BVCU_PUCFG_BaseStation stNearBts[8];      // 周边基站信息（不包含接入基站）。    非必填
    //  wifi信息
    BVCU_PUCFG_WifiMac     stConnectWifi;     // 已连接wifi热点mac信息。iAccessType=BVCU_PU_ONLINE_THROUGH_WIFI时必填
    BVCU_PUCFG_WifiMac     stNearWifi[20];    // 周边wifi热点mac信息（不包含接入，不要包含移动wifi）。    非必填
}BVCU_PUCFG_Position;

// ============================== 设备使用者 ===============================
// 使用者组的基本信息
typedef struct _BVCU_PUCFG_UserGroup_
{
    char szId[BVCU_MAX_ID_LEN + 1];      // 组的id标识符 
    char szParentId[BVCU_MAX_ID_LEN + 1];// 组的上级组ID, 顶层组的此值为空 
    char szName[BVCU_MAX_NAME_LEN + 1];  // 组的名称 
}BVCU_PUCFG_UserGroup;
// 使用者组详细信息
typedef struct _BVCU_PUCFG_UserGroupInfo_
{
    char szId[BVCU_MAX_ID_LEN + 1];      // 组的id标识符，创建时可以为空，成功回调通知ID 
    char szParentId[BVCU_MAX_ID_LEN + 1];// 组的上级组 
    char szName[BVCU_MAX_NAME_LEN + 1];  // 组的名称 
    char szDescription[128];             // 描述该组的一些信息 
} BVCU_PUCFG_UserGroupInfo;

// 使用者的基本信息
typedef struct _BVCU_PUCFG_User_
{
    char szId[BVCU_MAX_ID_LEN + 1];     // 使用者的账号名，唯一辨识
    char szGroupId[BVCU_MAX_ID_LEN + 1];// 使用者所属的组id 
    char szName[BVCU_MAX_NAME_LEN + 1]; // 使用者的姓名，可不设置 
    char szPUId[BVCU_MAX_ID_LEN + 1];   // 使用者当前使用的设备，一对一绑定关系 
    int  iOnlineStatus;      // 当前使用的设备在线状态，参见BVCU_ONLINE_STATUS_*。
}BVCU_PUCFG_User;

// 使用者的详细信息
typedef struct _BVCU_PUCFG_UserInfo_
{
    char szId[BVCU_MAX_ID_LEN + 1];        // 使用者的账号名，唯一辨识
    char szGroupId[BVCU_MAX_ID_LEN + 1];   // 使用者所属的组id 
    char szName[BVCU_MAX_NAME_LEN + 1];    // 使用者的姓名，可不设置 
    char szIDCard[BVCU_MAX_ID_LEN + 1];    // 使用者的身份证号码，可不设置 
    char szPosition[BVCU_MAX_NAME_LEN + 1];// 使用者的职称，可不设置 
    char szPhone[BVCU_MAX_MOBILEPHONE_NUM_LEN + 1];// 使用者的联系电话，可不设置 
    char szEmail[BVCU_MAX_NAME_LEN + 1];   // 使用者的email，可不设置 
    char szDescription[128]; // 对此使用者的描述，可不设置 

    // 绑定的设备，添加和修改使用者信息的时候，无需填写，查询使用者信息的时候，会返会绑定的设备
    char szPUId[BVCU_MAX_ID_LEN + 1];      // 使用者当前使用的设备，一对一绑定关系，修改用户信息时无需填写 
    int  iOnlineStatus;      // 当前使用的设备在线状态，参见BVCU_ONLINE_STATUS_*。
    char szReason[128];      // 描述当前绑定 
 
    char szCreateUserId[BVCU_MAX_ID_LEN + 1];  // 分配此使用者的用户id，对应于BVCU_UCFG_User，终端不需要填写 
    BVCU_WallTime stCreateTime;    //  创建该使用者的时间，终端不需要填写
}BVCU_PUCFG_UserInfo;

// 针对设备使用者的操作
typedef struct _BVCU_PUCFG_UserOperate
{
    char szId[BVCU_MAX_ID_LEN + 1];         // 使用者的账号名，唯一辨识 
    char szPUId[BVCU_MAX_ID_LEN + 1];       // 使用者当前使用的设备，一对一绑定关系 
    int  iOperate;                          // 0:代表解绑操作，1:代表绑定操作 
    char szReason[128];                     // 描述当前绑定 
}BVCU_PUCFG_UserOperate;

typedef struct _BVCU_PUCFG_UploadConfig
{
    int snapshotAutoUpload;         // 拍照自动上传，默认上传，0：不上传，1：上传
    int imptFileAutoUpload;         // 重点文件自动上传，默认上传，0：不上传，1：上传
    int delUploadedFile;            // 上传后删除本地文件，默认不删除，0：不删除，1：删除
}BVCU_PUCFG_UploadConfig;
#endif

