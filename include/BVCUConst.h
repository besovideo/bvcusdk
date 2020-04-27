#ifndef __BVCUCONST_H__
#define __BVCUCONST_H__

#ifdef _MSC_VER
#ifdef LIBBVCU_EXPORTS
#define LIBBVCU_API __declspec(dllexport)
#else
#define LIBBVCU_API __declspec(dllimport)
#endif
#else
#define LIBBVCU_API __attribute__ ((visibility("default")))
#endif//_MSC_VER

#define BVCU_MAX_CHANNEL_COUNT 64          // PU最大通道数
#define BVCU_MAX_ID_LEN 31                 // PU/CU ID长度
#define BVCU_MAX_NAME_LEN 63               // 显示用的名字长度
#define BVCU_MAX_ID_NAME_LEN (BVCU_MAX_ID_LEN > BVCU_MAX_NAME_LEN ? BVCU_MAX_ID_LEN : BVCU_MAX_NAME_LEN)
#define BVCU_MAX_PASSWORD_LEN 63           // 密码长度
#define BVCU_MAX_FILE_NAME_LEN 255         // 文件全路径最大长度
#define BVCU_MAX_HOST_NAME_LEN 127         // IP地址/域名最大长度
#define BVCU_MAX_SEDOMAIN_NAME_LEN 1023    // SmartEye域名最大长度
#define BVCU_LAT_LNG_UNIT 10000000.0       // 经纬度单位

#define BVCU_MAX_DAYTIMESLICE_COUNT 6      // 一天划分的时间片数目
#define BVCU_MAX_LANGGUAGE_COUNT 32        // 支持的语言数目
#define BVCU_MAX_MOBILEPHONE_NUM_LEN 15    // 手机号码长度
#define BVCU_MAX_ALARMLINKACTION_COUNT 64  // 报警联动数目

//PTZ
#define BVCU_PTZ_MAX_PROTOCOL_COUNT 32 // 云台支持的协议最大数目
#define BVCU_PTZ_MAX_PRESET_COUNT 256  // 预置点数目
#define BVCU_PTZ_MAX_CRUISE_COUNT 32   // 巡航路线数目
#define BVCU_PTZ_MAX_CRUISEPOINT_COUNT 32 // 每条巡航路线最多允许的预置点数
#define BVCU_PTZ_MAX_NAME_LEN 31 // 云台相关（例如预置点、巡航路线）名字
#define BVCU_PTZ_MAX_SPEED 15    // 云台运动最大速度
#define BVCU_PTZ_MIN_SPEED 1     // 云台运动最小速度
#define BVCU_PTZ_MAX_LOCK_TIMEOUT 60 // 云台锁定最长持续时间，单位秒，超过该时间Server会自动解锁。客户端软件也应实现自动解锁。

// Decoder
#define  BVCU_MAX_SUPPORT_RESOLUTION 6  // 解码器输出口支持的最多分辨率数。 
#define  BVCU_DEC_MAX_WINDOWS  32  // 解码器每个显示通道支持的最多窗口数。

#define BVCU_MAX_TOUR_IPCHANNEL_COUNT    16 // IP通道最大轮询连接数量

// 函数返回值及事件通知状态码
typedef enum _BVCU_Result {
    BVCU_RESULT_E_FAILED  = -0x10000, // general error
    BVCU_RESULT_E_INVALIDARG,         // invalid argument
    BVCU_RESULT_E_UNSUPPORTED,        // unsupported functions
    BVCU_RESULT_E_ALLOCMEMFAILED,     // allocate memory failed 
    BVCU_RESULT_E_MEMALIGNMENT,       // memory alignment is not satisfied
    BVCU_RESULT_E_NOTFOUND,           // not found
    BVCU_RESULT_E_NOTALLOWED,         // the requested access is not allowed
    BVCU_RESULT_E_IO,                 // I/O error
    BVCU_RESULT_E_EOF,                // End of file
    BVCU_RESULT_E_INVALIDDATA,        // Invalid data found when processing input
    BVCU_RESULT_E_NOTIMPL,            // not implemented
    BVCU_RESULT_E_BUSY,               // busy.deny service now
    BVCU_RESULT_E_INUSE,              // device in use
    BVCU_RESULT_E_BADREQUEST,         // bad request
    BVCU_RESULT_E_AUTHORIZE_FAILED,   // authorize failed。登录/发送命令等的OnEvent回调中使用
    BVCU_RESULT_E_BADSTATE,           // bad internal state 
    BVCU_RESULT_E_NOTINITILIZED,      // not initialized   
    BVCU_RESULT_E_FATALERROR,         // fatal error. BVCU should be closed
    BVCU_RESULT_E_OUTOFSPACE,         // out of space
    BVCU_RESULT_E_DISCONNECTED,       // disconnected
    BVCU_RESULT_E_TIMEOUT,            // time out
    BVCU_RESULT_E_CONNECTFAILED,      // connect failed
    BVCU_RESULT_E_ABORTED,            // request aborted
    BVCU_RESULT_E_THRAEDCONTEXT,      // can not execute in the specified thread context
    BVCU_RESULT_E_UNAVAILABLE,        // unavailable, eg: initialize a dialog with an offlined PU  
    BVCU_RESULT_E_ALREADYEXIST,       // already exist  
    BVCU_RESULT_E_SEVERINTERNAL,      // Server internal error
    BVCU_RESULT_E_MAXRETRIES,         // 达到最大重试次数
    
    BVCU_RESULT_E_AAA_OBJECTNOTFOUND = -0x0F000,// AAA 用户/用户组等不存在。SESSION_CLOSE事件的OnEvent回调中使用
    BVCU_RESULT_E_VTDU_NONE, // VTDU 没有可用的VTDU转发服务器

    BVCU_RESULT_S_OK = 0,             // succeed
    BVCU_RESULT_S_IGNORE,             // succeed,but something can not handle is ignored.
    BVCU_RESULT_S_PENDING,            // operation is pending.
    BVCU_RESULT_S_CONTINUE,           // 表示后续还有FTP数据
}BVCU_Result;
#define BVCU_Result_SUCCEEDED(a) ( ((int)(a)) >= 0 )
#define BVCU_Result_FAILED(a) ( ((int)(a)) < 0 )

//编码器
#define BVCU_VIDEO_QUALITY_COUNT 6

//DialogTarget.iIndexMajor取值范围与通道类型
#define BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL  0   //音视频通道
#define BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL  0x00FFFF
#define BVCU_SUBDEV_INDEXMAJOR_MIN_GPS      0x010000 //GPS设备数据
#define BVCU_SUBDEV_INDEXMAJOR_MAX_GPS      0x0100FF
#define BVCU_SUBDEV_INDEXMAJOR_MIN_TSP      0x010100 //透明串口设备数据
#define BVCU_SUBDEV_INDEXMAJOR_MAX_TSP      0x0101FF
#define BVCU_SUBDEV_INDEXMAJOR_PLAYBACK     0x010200 //远程回放通道
#define BVCU_SUBDEV_INDEXMAJOR_DOWNLOAD     0x010210 //文件传输通道（上传/下载）
#define BVCU_SUBDEV_INDEXMAJOR_MIN_CUSTOM   0xF00000 //自定义设备数据
#define BVCU_SUBDEV_INDEXMAJOR_MAX_CUSTOM   0xF000FF

//视频码率控制
typedef enum _BVCU_RATECONTROL{
    BVCU_RATECONTROL_INVALID = -1,
    BVCU_RATECONTROL_CBR = 0,
    BVCU_RATECONTROL_VBR,
    BVCU_RATECONTROL_CVBR
}BVCU_RATECONTROL;

//视频制式
typedef enum _BVCU_VIDEOFORMAT{
    BVCU_VIDEOFORMAT_UNKNOWN = 0,
    BVCU_VIDEOFORMAT_PAL,
    BVCU_VIDEOFORMAT_NTSC,
    BVCU_VIDEOFORMAT_P, // progressive scanning
    BVCU_VIDEOFORMAT_I, // interlaced scanning

    BVCU_VIDEOFORMAT_CUSTOM = 0x8000,//自定义
}BVCU_VIDEOFORMAT;

//存储类型
typedef enum _BVCU_STORAGE_RECORDTYPE{
    BVCU_STORAGE_RECORDTYPE_NONE =        0,//不存储
    BVCU_STORAGE_RECORDTYPE_MANUAL =  (1<<0),//手动存储
    BVCU_STORAGE_RECORDTYPE_ONTIME =  (1<<1),//定时存储
    BVCU_STORAGE_RECORDTYPE_ONALARM = (1 << 2),//报警存储（根据报警联动的配置）
    BVCU_STORAGE_RECORDTYPE_AUTOSAVE= (1 << 3),//自动存储（根据自动存储配置）
    BVCU_STORAGE_RECORDTYPE_UPLOAD  = (1 << 4),//上传的文件（终端上传到平台的文件）
    BVCU_STORAGE_RECORDTYPE_DOWNLOAD= (1 << 5),//下载的文件（平台从终端下载的文件）
    BVCU_STORAGE_RECORDTYPE_AVCALL = (1 << 6),//音视频通话文件
}BVCU_STORAGE_RECORDTYPE;

typedef enum _BVCU_STORAGE_FILE_TYPE{
    BVCU_STORAGE_FILE_TYPE_ALL = 0,      // 不限定文件类型
    BVCU_STORAGE_FILE_TYPE_RECORD = 1 << 0,   // 录像文件
    BVCU_STORAGE_FILE_TYPE_CAPTURE = 1 << 1,  // 图片文件
    BVCU_STORAGE_FILE_TYPE_GPS = 1 << 2,      // GPS文件
    BVCU_STORAGE_FILE_TYPE_AUDIO = 1 << 3,    // 音频文件
    BVCU_STORAGE_FILE_TYPE_LOG = 1 << 4,      // 日志文件
    BVCU_STORAGE_FILE_TYPE_FIRMWARE = 1 << 8, // 固件文件
}BVCU_STORAGE_FILE_TYPE;

// 媒体方向
typedef enum _BVCU_MEDIADIR{
    BVCU_MEDIADIR_VIDEOSEND = (1<<0),
    BVCU_MEDIADIR_VIDEORECV = (1<<1),
    BVCU_MEDIADIR_AUDIOSEND = (1<<2),
    BVCU_MEDIADIR_AUDIORECV = (1<<3),
    BVCU_MEDIADIR_TALKONLY  = (BVCU_MEDIADIR_AUDIOSEND | BVCU_MEDIADIR_AUDIORECV),
    BVCU_MEDIADIR_DATASEND  = (1<<4),
    BVCU_MEDIADIR_DATARECV  = (1<<5),
}BVCU_MEDIADIR;


// 在线状态
typedef enum _BVCU_ONLINE_STATUS{
    BVCU_ONLINE_STATUS_OFFLINE = 0,
    BVCU_ONLINE_STATUS_ONLINE,
}BVCU_ONLINE_STATUS;

// 会话通道当前状态， 按位表达当前会话存在的异常，异常位为零表示正常
typedef enum _BVCU_DIALOG_STATUS {
    BVCU_DIALOG_STATUS_OFFLINE = 0,
    BVCU_DIALOG_STATUS_ONLINE = (1 << 0), //会话在连通中
    BVCU_DIALOG_STATUS_E_A_PTS = (1 << 1), //音频时间戳错误
    BVCU_DIALOG_STATUS_E_V_PTS = (1 << 2), //视频时间戳错误
    BVCU_DIALOG_STATUS_E_A_DEC = (1 << 3), //音频解码有错误
    BVCU_DIALOG_STATUS_E_V_DEC = (1 << 4), //视频解码有错误（成功解码关键帧后重新统计）
    BVCU_DIALOG_STATUS_E_A_RENDER = (1 << 5), //音频播放有错误
    BVCU_DIALOG_STATUS_E_V_RENDER = (1 << 6), //视频播放有错误

    BVCU_DIALOG_STATUS_E_V_KEY = (1 << 7), //视频没有I帧（丢帧后重新统计）
    BVCU_DIALOG_STATUS_E_V_LOAST = (1 << 8), //视频有丢包（收到关键帧后重新统计）
    BVCU_DIALOG_STATUS_E_V_SPS = (1 << 9), //视频SPS\PPS发生变化或有错

    BVCU_DIALOG_STATUS_E_AUDIO = BVCU_DIALOG_STATUS_E_A_PTS | BVCU_DIALOG_STATUS_E_A_DEC | BVCU_DIALOG_STATUS_E_A_RENDER,
    BVCU_DIALOG_STATUS_E_VIDEO = BVCU_DIALOG_STATUS_E_V_PTS | BVCU_DIALOG_STATUS_E_V_DEC | BVCU_DIALOG_STATUS_E_V_RENDER
    | BVCU_DIALOG_STATUS_E_V_KEY | BVCU_DIALOG_STATUS_E_V_LOAST | BVCU_DIALOG_STATUS_E_V_SPS,
    BVCU_DIALOG_STATUS_E_ALL = BVCU_DIALOG_STATUS_E_AUDIO | BVCU_DIALOG_STATUS_E_VIDEO,
}BVCU_DIALOG_STATUS;

// subscribe状态
typedef enum _BVCU_SUBSCRIBE_STATUS{
    BVCU_SUBSCRIBE_OFF = 0,
    BVCU_SUBSCRIBE_ON,
}BVCU_SUBSCRIBE_STATUS;

// 传输层协议类型
typedef enum _BVCU_PROTOTYPE{
    BVCU_PROTOTYPE_TCP = 0,
    BVCU_PROTOTYPE_UDP,
    BVCU_PROTOTYPE_TLS,

    BVCU_PROTOTYPE_FTP,
    BVCU_PROTOTYPE_HTTP,
    BVCU_PROTOTYPE_SIP,
    BVCU_PROTOTYPE_RTSP,
}BVCU_PROTOTYPE;

// 数据流 传输途径
enum {
    BVCU_STREAM_PATHWAY_AUTO = 0, // 服务器决定传输途径
    BVCU_STREAM_PATHWAY_VTDU,// 通过 VTDU转发
    BVCU_STREAM_PATHWAY_P2P, // CU P2P 直连设备
};

// 通道硬件连接方式
typedef enum {
    BVCU_CHANNEL_DEV_CONNECT_UNKNOWN = 0,
    BVCU_CHANNEL_DEV_CONNECT_ANALOG,
    BVCU_CHANNEL_DEV_CONNECT_HDMI,
    BVCU_CHANNEL_DEV_CONNECT_IP,
    BVCU_CHANNEL_DEV_CONNECT_CMOS,
    BVCU_CHANNEL_DEV_CONNECT_SDI,
    BVCU_CHANNEL_DEV_CONNECT_USB,
    BVCU_CHANNEL_DEV_CONNECT_AHD, // 模拟高清
} BVCU_CHANNEL_DEV_CONNECT_TYPE;

typedef enum _BVCU_IPC_NET_PROTO_TYPE{
    BVCU_IPC_NET_PROTO_TYPE_UNKNOWN = 0,
    BVCU_IPC_NET_PROTO_TYPE_ONVIF,
    BVCU_IPC_NET_PROTO_TYPE_SMARTEYE, // SmartEye平台
    BVCU_IPC_NET_PROTO_TYPE_HIK, // 暂时没有使用到
}BVCU_IPC_NET_PROTO_TYPE;

// CU发出/接收的命令。在BVCU_SendCmd中用到
typedef enum _BVCU_METHOD_TYPE{
    BVCU_METHOD_UNKNOWN  = 0,
    BVCU_METHOD_QUERY,
    BVCU_METHOD_CONTROL,
    BVCU_METHOD_SUBSCRIBE,

    BVCU_METHOD_RESERVED = 0x80000,//保留供内部使用
}BVCU_METHOD_TYPE;

/* CU发出/接收的子命令。 下面的注释中，
输入类型：指BVCU_Command的pData成员的数据类型，由BVCU_Command.iSubMethod决定
输出类型：对query/control等Command,BVCU_Command.OnEvent的pParam指向的((BVCU_Event_SessionCmd*)pParam)->stContent.pData
      对subscribe等Command,接收到的数据通过BVCU_ServerParam.OnNotify通知CU,pData->pData
      对notify，由BVCU_NotifyMsgContent.iSubMethod确定数据类型
触发类型：有些Control命令在CMS执行后，CMS会发送一个通知(notify)或命令(command)给BVCU。
          注释中“同名Notify/Command”表示会产生与原命令相同SUBMETHOD类型的Notify/Command。
          无改注释的命令不会触发Notify/Command。Query命令不会触发。

*/
typedef enum _BVCU_SUBMETHOD_TYPE{
    BVCU_SUBMETHOD_UNKNOWN = 0,

    BVCU_SUBMETHOD_CUSTOM = 0x00001, //自定义命令，服务器透明转发该命令给目标。输入类型：自定义；输出类型: 自定义。
                                     //注意，BVCU_CmdMsgContent.pData被认为是字节类型，BVCU_CmdMsgContent.iDataCount表示字节长度
    
    //=============query====================    
    //PU部分-----------------------------
    BVCU_SUBMETHOD_PU_LIST = 0x00010,//CU从Server获取设备列表。输入类型：无；输出类型: BVCU_PUCFG_PUChannelInfo数组
    BVCU_SUBMETHOD_PU_BROADCASTSTATUS,//获取广播的设备列表状态(一个Session只能同时有一个广播)。输入类型：无；输出类型：BVCU_BroadcastStatus数组
    BVCU_SUBMETHOD_PU_GPSDATA,//获取PU的GPS通道数据。输入类型：无；输出类型: BVCU_PUCFG_GPSData
    BVCU_SUBMETHOD_PU_STORAGE_MEDIA, //获取某个存储器的分区信息。输入类型无; 输出类型：BVCU_PUCFG_Storage_Media数组
    BVCU_SUBMETHOD_PU_GROUPLIST, // 获取设备分组列表。输入类型：无；输出类型：BVCU_PUCFG_GroupItem数组
    BVCU_SUBMETHOD_PU_GROUPINFO, // 获取设备分组信息。输入类型：BVCU_PUCFG_GroupItem；输出类型：BVCU_PUCFG_GroupInfo
    BVCU_SUBMETHOD_PU_UPDATESTATUS, //升级固件的状态。输入类型：无；输出类型：BVCU_PUCFG_UpdateStatus
    BVCU_SUBMETHOD_PU_POSITION, //基站、wifi定位信息。输入类型：无；输出类型：BVCU_PUCFG_Position
    BVCU_SUBMETHOD_PU_UserList, //获取平台设备使用者列表。输入类型：无；输出类型：BVCU_PUCFG_User数组
    BVCU_SUBMETHOD_PU_UserInfo, //获取平台设备使用者信息。输入类型：BVCU_PUCFG_User；输出类型：BVCU_PUCFG_UserInfo
    BVCU_SUBMETHOD_PU_UserGroupList, //获取平台设备使用者组列表。输入类型：无；输出类型：BVCU_PUCFG_UserGroup数组
    BVCU_SUBMETHOD_PU_UserGroupInfo, //获取平台设备使用者组信息。输入类型：BVCU_PUCFG_UserGroup；输出类型：BVCU_PUCFG_UserGroupInfo
    BVCU_SUBMETHOD_PU_RECORDSTATUS,  //获取设备存储状态（录像、录音、GPS..）。输入类型：无；输出类型：BVCU_PUCFG_RecordStatus
    
    //User部分---------------------------
    BVCU_SUBMETHOD_USER_GROUPLIST = 0x01000,//获取用户组列表。输入类型：无；输出类型：BVCU_UCFG_UserGroup数组
    BVCU_SUBMETHOD_USER_GROUPINFO,//获取用户组信息。输入类型：BVCU_UCFG_UserGroup；输出类型：BVCU_UCFG_UserGroupInfo
    BVCU_SUBMETHOD_USER_USERLIST, //获取用户列表。输入类型：无；输出类型：BVCU_UCFG_User数组
    BVCU_SUBMETHOD_USER_USERINFO, //获取用户信息。输入类型：BVCU_UCFG_User； 输出类型：BVCU_UCFG_UserInfo
    BVCU_SUBMETHOD_USER_ONLINE, //获取在线用户列表。输入类型：无； 输出类型：BVCU_UCFG_User_Online数组
    BVCU_SUBMETHOD_USER_ONLINEINFO, //获取在线用户信息。输入类型：BVCU_UCFG_User_Online； 输出类型：BVCU_UCFG_User_OnlineInfo
    BVCU_SUBMETHOD_USER_CMP,    //比较两个用户(组）/设备的权限关系。输入类型：BVCU_UCFG_User_Cmp； 输出类型：BVCU_UCFG_User_CmpResult

    //VTDU部分----------------------------
    BVCU_SUBMETHOD_VTDU_LIST = 0x01300,//获取VTDU列表。输入类型：无；输出类型：BVCU_VTDUCFG_VTDUItem数组

    //NRU部分----------------------------
    BVCU_SUBMETHOD_NRU_LIST = 0x01400,//获取NRU列表。输入类型：无；输出类型：BVCU_NRUCFG_NRUItem数组
    BVCU_SUBMETHOD_NRU_SCHEDULE_LIST, //某个NRU存储计划列表。输入类型：无；输出类型：BVCU_NRUCFG_Storage_Schedule_ListItem数组
    BVCU_SUBMETHOD_NRU_SCHEDULE_GET,//NRU的一条存储计划。输入类型：BVCU_NRUCFG_Storage_Schedule_ListItem, 输出类型：BVCU_NRUCFG_Storage_Schedule

    //Decoder部分------------------------
    BVCU_SUBMETHOD_DEC_LIST = 0x01500,//获取解码器列表。输入类型：无；输出类型：BVCU_DECCFG_BaseInfo数组

    //CONF(集群）部分---------------------------
    BVCU_SUBMETHOD_CONF_LIST = 0x01600,//获取集群列表。输入类型：无；输出类型：BVCU_Conf_BaseInfo数组
                                       //BVCU_Command.iTargetIndex：0-查询所有集群列表；1-查询自己所在集群列表；2-查询自己不在的集群列表；3-查询有管理权限的集群列表。
    BVCU_SUBMETHOD_CONF_INFO,//获取某个集群详细信息，BVCU_Command.szTargetID设置为CONF_ID。输入类型：无；输出类型：BVCU_Conf_Info
    BVCU_SUBMETHOD_CONF_DEL_LIST,//获取已经被删除的集群列表。输入类型：无；输出类型：BVCU_Conf_BaseInfo数组

    //ALARM部分--------------------------
    BVCU_SUBMETHOD_LINKACTION_LIST = 0x01700,//获取报警联动列表。输入类型：无；输出类型：BVCU_Event_LinkAction_Base数组
    BVCU_SUBMETHOD_LINKACTION_GET,//获取某个报警联动详细信息。输入类型：BVCU_Event_LinkAction_Base;输出类型：BVCU_Event_LinkAction

    //Search部分---------------------------
    // BVCU_Command.szTargetID设置为CMS/NRU/VTDU ID。
    BVCU_SUBMETHOD_SEARCH_LIST = 0x01800, // 索引文件（记录等）。输入类型：BVCU_Search_Request 输出类型:BVCU_Search_Response 
    BVCU_SUBMETHOD_SYSTEM_INFO, // 系统及状态信息。输入类型：无  输出类型:BVCU_ENCFG_SystemInfo
    
    //Region部分-------------------------
    BVCU_SUBMETHOD_REGION_LIST = 0x1900, //获取CMS区域列表。输入类型：无；输出类型：BVCU_Region_Base数组
    BVCU_SUBMETHOD_REGION_GET, //获取某个CMS区域详细信息。输入类型：BVCU_Region_Base;输出类型：BVCU_Region_Info
    BVCU_SUBMETHOD_ROUTEPATH_LIST, //获取所有线路/区域详细信息。输入类型：无;输出类型：BVCU_Route_Path数组
    BVCU_SUBMETHOD_ROUTEPATH_GET, //获取某个线路/区域详细信息。输入类型：BVCU_Route_Path;输出类型：BVCU_Route_Path
    BVCU_SUBMETHOD_ROUTEPLAN_GET, //获取某些线路/区域规划信息。输入类型：BVCU_Route_Plan;输出类型：BVCU_Route_Plan数组

    //UKey部分---------------------------
    BVCU_SUBMETHOD_UKEY_LIST = 0x2000, //获取授权UKey列表。输出类型：BVCU_UCFG_UKeyBase数组
    BVCU_SUBMETHOD_UKEY_GET, //获取某个授权UKey详细信息。输入类型：BVCU_UCFG_UKeyBase;输出类型：BVCU_UCFG_UKeyInfo

    //CMS部分----------------------------
    BVCU_SUBMETHOD_CMS_BASEINFO = 0x2100, //获取登录CMS基本信息。输入类型：无;输出类型：BVCU_CMSCFG_BaseInfo
    BVCU_SUBMETHOD_CMS_DIALOGINFO, //获取登录CMS通道信息。输入类型：BVCU_CMSCFG_DialogInfo;输出类型：BVCU_CMSCFG_DialogInfo数组
    BVCU_SUBMETHOD_CMS_TIME, //获取登录CMS时间。输入类型：BVCU_CMSCFG_WallTime; 输出类型：BVCU_CMSCFG_WallTime

    //=============query/control=============    
    /* 注意：query/control类型的命令，通常query和control的输入类型和输出类型是对称的，例如
    BVCU_SUBMETHOD_PTZATTR命令做Query时，输入类型无，输出类型是BVCU_PUCFG_PTZAttr，做Control
    时，输入类型BVCU_PUCFG_PTZAttr，输出类型无。以下注释中，只表明Control的参数，如果没有特别说明，     
    Query的参数输入/输出类型就是Control的输出/输入类型
    */
    //PU部分
    BVCU_SUBMETHOD_PU_DEVICEINFO = 0x10000, //设备信息。输入类型：BVCU_PUCFG_DeviceInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_CHANNELINFO,//某个PU的通道信息。输入类型：BVCU_PUCFG_PUChannelInfo；输出类型: 无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_DEVICETIME,//设备时间。输入类型：BVCU_WallTime；输出类型：无
    BVCU_SUBMETHOD_PU_SERVERS, //注册服务器。输入类型：BVCU_PUCFG_Servers；输出类型：无
    BVCU_SUBMETHOD_PU_ETHERNET, //以太网。输入类型：BVCU_PUCFG_Ethernet；输出类型：无
    BVCU_SUBMETHOD_PU_WIFI, //WIFI。输入类型：BVCU_PUCFG_Wifi；输出类型：无
    BVCU_SUBMETHOD_PU_RADIONETWORK, //无线网络。输入类型：BVCU_PUCFG_RadioNetwork数组；输出类型：无
    BVCU_SUBMETHOD_PU_PTZATTR,  //云台属性。输入类型：BVCU_PUCFG_PTZAttr；输出类型：无；
    BVCU_SUBMETHOD_PU_ENCODERCHANNEL,//编码通道属性。输入类型：BVCU_PUCFG_EncoderChannel；输出类型：无；
    BVCU_SUBMETHOD_PU_VIDEOIN,//视频输入属性。输入类型：BVCU_PUCFG_VideoIn；输出类型：无
    BVCU_SUBMETHOD_PU_AUDIOIN,//音频输入属性。输入类型：BVCU_PUCFG_AudioIn；输出类型：无
    BVCU_SUBMETHOD_PU_AUDIOOUT,//音频输出属性。输入类型：BVCU_PUCFG_AudioOut；输出类型：无
    BVCU_SUBMETHOD_PU_ALERTIN,//报警输入属性。输入类型：BVCU_PUCFG_AlertIn；输出类型：无
    BVCU_SUBMETHOD_PU_ALERTOUT,//报警输出属性。输入类型：BVCU_PUCFG_AlertOut；输出类型：无
    BVCU_SUBMETHOD_PU_SERIALPORT,//串口属性。输入类型：BVCU_PUCFG_SerialPort;输出类型：无
    BVCU_SUBMETHOD_PU_GPS,//GPS属性。输入类型：BVCU_PUCFG_GPSParam;输出类型：无
    BVCU_SUBMETHOD_PU_STORAGE_SCHEDULE, //存储计划。输入类型：BVCU_PUCFG_Storage_Schedule;输出类型：无
    BVCU_SUBMETHOD_PU_STORAGE_RULE, //存储属性。输入类型：BVCU_PUCFG_Storage_Rule;输出类型：无    
    BVCU_SUBMETHOD_PU_STORAGE_FORMAT,//初始化某个存储器。输入类型：BVCU_PUCFG_Storage_Format;输出类型：无  
    BVCU_SUBMETHOD_PU_ONLINECONTROL,//上下线控制。输入类型：BVCU_PUCFG_OnlineControl；输出类型：无
    BVCU_SUBMETHOD_PU_SNAPSHOTPARAM,//配置自动抓拍参数。输入类型：BVCU_PUCFG_SnapshotParam;输出类型：无
    BVCU_SUBMETHOD_PU_POWER,//配置电源参数。输入类型：BVCU_PUCFG_Power;输出类型：无
    BVCU_SUBMETHOD_PU_GPSSPEEDLIMIT,//配置设备全局限速。输入类型：BVCU_PUCFG_GPSSpeedLimit;输出类型：无
    BVCU_SUBMETHOD_PU_CARINFO,//设备相关的车辆信息（如：车载设备）。输入类型：BVCU_PUCFG_CarInfo;输出类型：无
    BVCU_SUBMETHOD_PU_BLUETOOTH,//设备蓝牙信息。 输入类型：BVCU_PUCFG_Bluetooth;输出类型：无
    BVCU_SUBMETHOD_PU_VIDEOIN_CMOS,//视频输入CMOS属性。输入类型：BVCU_PUCFG_ImageExposure；输出类型：无
    BVCU_SUBMETHOD_PU_ZFYINFO,//设备相关的执法记录仪信息。输入类型：BVCU_PUCFG_ZFYInfo;输出类型：无
    BVCU_SUBMETHOD_PU_MISCHARDWARE,//设备外围硬件信息。输入类型：BVCU_PUCFG_MiscHardware数组;输出类型：无。查询时回复的是设备支持的所有硬件信息数组，设置时可以只是修改的硬件信息。

    //VTDU部分
    BVCU_SUBMETHOD_VTDU_INFO = 0x10F00,//VTDU信息。输入类型：BVCU_VTDUCFG_VTDUInfo；输出类型：无

    //NRU部分
    BVCU_SUBMETHOD_NRU_INFO = 0x11000,//NRU信息。输入类型：BVCU_NRUCFG_NRUInfo；输出类型：无

    //Decoder部分------------------------
    //BVCU_Command.szTargetID设置为DEC_ID
    BVCU_SUBMETHOD_DEC_DEVICEINFO = 0x11100, // 解码器详细信息。输入类型：BVCU_DECCFG_DeviceInfo；输出类型：无
    BVCU_SUBMETHOD_DEC_OPTIONS, //解码器设置选项。输入类型：BVCU_DECCFG_Options；输出类型：无
    BVCU_SUBMETHOD_DEC_DECODER, // 解码器解码通道配置。输入类型：BVCU_DECCFG_DecoderParam；输出类型：无
    BVCU_SUBMETHOD_DEC_DISPLAY, // 解码器显示通道配置。输入类型：BVCU_DECCFG_DisplayParam；输出类型：无

    //CONF（集群）部分
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_VOLUME = 0x11200,//调整某个参与者的音量。注意只影响本地听到的音量，BVCU_Command.szTargetID设置为CONF_ID。
                                                      //输入类型：BVCU_Conf_Participator_Info，输出类型：无
    BVCU_SUBMETHOD_CONF_BASEINFO,//获取/设置集群语音会议的信息，BVCU_Command.szTargetID设置为CONF_ID。
                        //输入类型：BVCU_Conf_BaseInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_CONF_DEFAULT,//获取/设置默认语音会议列表，BVCU_Command.szTargetID设置为 设备ID（或用户账号）
                        //输入类型：BVCU_Conf_BaseInfo数组；输出类型：无；触发类型：同名Notify

    //=============control=============    
    //PU部分---------------------------
    BVCU_SUBMETHOD_PU_REBOOT = 0x20000,   //重启设备。输入类型：无；输出类型：无
    BVCU_SUBMETHOD_PU_DELETE,       //从数据库中删除PU。输入类型：无；输出类型: 无
    BVCU_SUBMETHOD_PU_SHUTDOWN, //关闭设备。输入类型：无；输出类型：无
    BVCU_SUBMETHOD_PU_SAVECONFIG, //通知设备保存配置。输入类型：无；输出类型：无
    BVCU_SUBMETHOD_PU_PTZCONTROL,//操作云台。输入类型：BVCU_PUCFG_PTZControl；输出类型：无    
    BVCU_SUBMETHOD_PU_PUTOFFLINE,//手工使PU下线。输入类型：无；输出类型：无
    BVCU_SUBMETHOD_PU_MANUALRECORD,//手工启动/停止PU录像。输入类型：BVCU_PUCFG_ManualRecord；输出类型：无
    BVCU_SUBMETHOD_PU_SNAPSHOT, //手工远程抓拍。输入类型：BVCU_PUCFG_Snapshot；输出类型：无
    BVCU_SUBMETHOD_PU_UPGRADE, //升级固件。输入类型：BVCU_PUCFG_Upgrade；输出类型：无
    BVCU_SUBMETHOD_PU_ADDGROUP, //添加设备分组。输入类型：BVCU_PUCFG_GroupInfo；输出类型：BVCU_PUCFG_GroupItem；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_MODGROUP, //修改设备分组。输入类型：BVCU_PUCFG_GroupInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_DELGROUP, //删除设备分组。输入类型：BVCU_PUCFG_GroupItem；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_OPENDIALOG, //请求创建会话。输入类型：BVCU_PUCFG_OpenDialog； 输出类型：BVCU_PUCFG_OpenDialog
    BVCU_SUBMETHOD_PU_SYNCHRONIZATION, //同步图片（上传未上传的抓图，下载CU上传的图片）。输入类型：BVCU_PUCFG_Picture
    BVCU_SUBMETHOD_PU_USER_ADDGROUP, //添加设备使用者组。输入类型：BVCU_PUCFG_UserGroupInfo；输出类型：BVCU_PUCFG_UserGroup；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_MODGROUP, //修改设备使用者组。输入类型：BVCU_PUCFG_UserGroupInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_DELGROUP, //删除设备使用者组。输入类型：BVCU_PUCFG_UserGroup；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_ADDUSER,  //添加设备使用者。输入类型：BVCU_PUCFG_UserInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_MODUSER,  //修改设备使用者。输入类型：BVCU_PUCFG_UserInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_DELUSER,  //删除设备使用者。输入类型：BVCU_PUCFG_User；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_PU_USER_OPERATE,  //绑定或解绑设备。输入类型：BVCU_PUCFG_UserOperate；输出类型：无；触发类型：同名Notify
    
    //User部分---------------------------
    BVCU_SUBMETHOD_USER_ADDGROUP = 0x22000, //添加用户组。输入类型：BVCU_UCFG_UserGroupInfo；输出类型：BVCU_UCFG_UserGroup；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_MODGROUP, //修改用户组。输入类型：BVCU_UCFG_UserGroupInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_DELGROUP, //删除用户组。输入类型：BVCU_UCFG_UserGroup；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_ADDUSER,  //添加用户。输入类型：BVCU_UCFG_UserInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_MODUSER,  //修改用户。输入类型：BVCU_UCFG_UserInfo；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_DELUSER,  //删除用户。输入类型：BVCU_UCFG_User；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_USER_MODPASSWD,//修改用户密码。输入类型：BVCU_UCFG_ModPasswd；输出类型：无
    BVCU_SUBMETHOD_USER_KICKOUT,  //踢出在线用户。输入类型：BVCU_UCFG_Kickout；输出类型：无

    //NRU部分---------------------------
    BVCU_SUBMETHOD_NRU_DELETE = 0x22400, //从数据库中删除NRU。输入类型：无；输出类型: 无
    BVCU_SUBMETHOD_NRU_SCHEDULE_SET,//配置NRU的一条存储计划。输入类型：BVCU_NRUCFG_Storage_Schedule, 输出类型:无；触发类型：同名Notify
    BVCU_SUBMETHOD_NRU_SCHEDULE_DEL,//删除NRU的一条存储计划。输入类型：BVCU_NRUCFG_Storage_Schedule_ListItem, 输出类型:无；触发类型：同名Notify
    BVCU_SUBMETHOD_NRU_MANUALRECORD,//手工启动/停止某个PU录像到NRU。输入类型：BVCU_NRUCFG_ManualRecord；输出类型：无
    BVCU_SUBMETHOD_NRU_SNAPSHOT,    //手工抓拍到NRU。输入类型：BVCU_NRUCFG_Snapshot；输出类型：无
    BVCU_SUBMETHOD_NRU_BACKUPDB,    //手工备份数据库。BVCU_Command.iTargetIndex：备份表信息，见BVCU_DBTABLE_TYPE。需要是超级管理员权限。
        // 输入类型：无；输出类型：BVCU_NRUCFG_BackupDB

    //CONF（集群）部分---------------------------
    //以下由具有BVCU_CONF_PARTICIPATOR_POWER_ADMIN权限者调用
    BVCU_SUBMETHOD_CONF_CREATE = 0x22600,//创建集群。 输入类型：BVCU_Conf_BaseInfo，
                                         // 输出类型：BVCU_Conf_BaseInfo + BVCU_Conf_Participator_Info 同名notify

    //以下所有BVCU_SUBMETHOD_CONF_*命令除非特别说明，否则BVCU_Command.szTargetID设置为CONF_ID
    BVCU_SUBMETHOD_CONF_DELETE,//删除集群。输入类型：无;输出类型:无；触发类型：同名Notify
    BVCU_SUBMETHOD_CONF_START,//开始语音会议。BVCU_Command.iTargetIndex==1表示多组同时监听。输入类型：无;输出类型:无；触发类型：BVCU_SUBMETHOD_CONF_START_CMD
    BVCU_SUBMETHOD_CONF_STOP,//停止语音会议。输入类型：无;输出类型:无；触发类型：同名Notify

    BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD, //添加集群成员。
                                          //输入类型：BVCU_Conf_Participator_Info数组，一次调用最多允许BVCU_CONF_MAX_PARTICIPATOR_ONETIME个。
                                          //输出类型：BVCU_Conf_Participator_AddResult
                                          //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD_CMD（被加入者收到）和BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD_NOTIFY（所有参与者收到）

    BVCU_SUBMETHOD_CONF_PARTICIPATOR_REMOVE,//删除集群成员。输入类型：BVCU_Conf_Participator_Info数组；输出类型：无；触发类型：同名Notify
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_MODIFY,//修改集群成员。输入类型：BVCU_Conf_Participator_Info数组；输出类型：无；触发类型：同名Notify

    //以下由BVCU_CONF_PARTICIPATOR_POWER_MODETATOR权限者调用
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_INVITE_SPEAK,//点名发言。仅对BVCU_CONF_MODE_SPEAK_CHAIRMAN模式的语音会议有意义。
                                                  //输入类型：BVCU_Conf_Participator_Info；输出类型：无
                                                  //触发类型：同名Notify(所有参与者收到)
                                                  //  BVCU_SUBMETHOD_CONF_PARTICIPATOR_INVITE_SPEAK_CMD(仅被点名者收到)
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_TERMINATE_SPEAK,//结束发言。输入类型：BVCU_Conf_Participator_Info；输出类型：无;触发类型：同名Notify
    //以下由participator调用
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN,//请求加入集群。输入类型：BVCU_Conf_Participator_Join；输出类型：无；
                                        //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN_NOTIFY（所有参与者收到）
                                        //            BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN_CMD(仅集群ADMIN收到）
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_EXIT,//请求退出集群。输入类型：无；输出类型：无；触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_EXIT_NOTIFY    
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK,//申请发言，仅对BVCU_CONF_MODE_SPEAK_CHAIRMAN模式的语音会议有意义。输入类型：无;输出类型：无；
                                                        //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK_NOTIFY    （所有参与者收到）
                                                        //          BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK_CMD(仅集群ADMIN收到）
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_ENDSPEAK,//结束发言，仅对BVCU_CONF_MODE_SPEAK_CHAIRMAN模式的语音会议有意义。输入类型：无;输出类型：无；
                                                       //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_ENDSPEAK_NOTIFY    
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_LEAVE,//暂时离开语音会议。输入类型：无；输出类型：无；
                                           //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_LEAVE_NOTIFY
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_RETURN,//与LEAVE相对，回到语音会议。BVCU_Command.iTargetIndex==1表示多组同时监听。输入类型：BVCU_Conf_Participator_Info;输出类型：无：
                                            //触发类型：BVCU_SUBMETHOD_CONF_PARTICIPATOR_RETURN_NOTIFY
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_INFO,//更改自己的成员信息(目前仅szAliasName有意义)，通知CMS。输入类型：BVCU_Conf_Participator_Info；输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_CONF_VISITOR_JOIN,   //请求旁听语音会议。输入类型：无；输出类型：BVCU_Conf_Participator_Info。BVCU_Command.iTargetIndex==1表示多组同时监听。
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_INVITE_JOIN,   //邀请参加语音会议，输入类型：BVCU_Conf_Participator_Invite；输出类型：无，BVCU_Command.iTargetIndex==1表示多组同时监听。
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_KICKOUT,       //踢出语音会议（区别删除集群成员，这里是离开正在进行的语音会议），输入类型：BVCU_Conf_Participator_Invite；输出类型：无
    BVCU_SUBMETHOD_IM_MSG = 0x22620, // 发送即时消息。输入类型：BVCU_IM_Msg数组 输出类型：BVCU_IM_Msg数组，用于获取各消息ID。消息接收方收到同名Notify。

    //存储文件部分---------------------------
    // BVCU_Search_FileFilter 中szDesc1，szDesc2作为修改字段，其余都作为过滤条件
    BVCU_SUBMETHOD_RECORDFILE_DESC_SET = 0x22630,  //设置文件记录的描述。输入类型：BVCU_Search_FileFilter，输出类型:无
    BVCU_SUBMETHOD_RECORDFILE_DELETE,          //删除指定条件的文件记录以及文件。输入类型：BVCU_NRUCFG_DeleteFile，输出类型:BVCU_NRUCFG_DeleteFileResult

    //事件联动部分---------------------------
    BVCU_SUBMETHOD_LINKACTION_ADD = 0x22700,//添加报警联动。输入类型：BVCU_Event_LinkAction;输出类型：无;
    BVCU_SUBMETHOD_LINKACTION_SET,//修改报警联动。输入类型：BVCU_Event_LinkAction;输出类型：无;
    BVCU_SUBMETHOD_LINKACTION_DEL,//删除报警联动。输入类型：BVCU_Event_LinkAction_Base;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_EVENT_PROCESS,//处警。输入类型：BVCU_Event_SourceSaved;输出类型：无

    //0x22800部分-------------------------------
    BVCU_SUBMETHOD_RTSP = 0x22800,//回放会话RTSP控制命令。输入类型：BVCU_CTRL_RTSP;输出类型：BVCU_CTRL_RTSP;接收者先回命令，再发新数据。

    //电子围栏&线路规划部分------------------
    BVCU_SUBMETHOD_REGION_ADD = 0x22900, //添加CMS区域。输入类型：BVCU_Region_Info;输出类型：无;
    BVCU_SUBMETHOD_REGION_SET, //修改CMS区域。输入类型：BVCU_Region_Info;输出类型：无;
    BVCU_SUBMETHOD_REGION_DEL, //删除CMS区域。输入类型：BVCU_Region_Base;输出类型：无;触发类型：同名Notify

    BVCU_SUBMETHOD_ROUTEPATH_ADD, //添加CMS区域。输入类型：BVCU_Route_Path;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_ROUTEPATH_SET, //修改CMS区域。输入类型：BVCU_Route_Path;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_ROUTEPATH_DEL, //删除CMS区域。输入类型：BVCU_Route_Path;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_ROUTEPLAN_ADD, //添加CMS区域。输入类型：BVCU_Route_Plan;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_ROUTEPLAN_SET, //修改CMS区域。输入类型：BVCU_Route_Plan;输出类型：无;触发类型：同名Notify
    BVCU_SUBMETHOD_ROUTEPLAN_DEL, //删除CMS区域。输入类型：BVCU_Route_Plan;输出类型：无;触发类型：同名Notify

    //UKey管理------------------
    BVCU_SUBMETHOD_UKEY_ADD = 0x23000, //添加授权UKey。输入类型：BVCU_UCFG_UKeyInfo;输出类型：无;
    BVCU_SUBMETHOD_UKEY_SET, //修改授权Ukey。输入类型：BVCU_UCFG_UKeyInfo;输出类型：无;
    BVCU_SUBMETHOD_UKEY_DEL, //删除授权UKey。输入类型：BVCU_UCFG_UKeyBase;输出类型：无;

    //=============subscribe/notify=============
    //调用者应检查BVCU_NotifyMsgContent.szSourceID，确定信息的发出者
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_EXIT_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_EXIT,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_ENDSPEAK_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_ENDSPEAK,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_LEAVE_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_LEAVE,//负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_RETURN_NOTIFY = BVCU_SUBMETHOD_CONF_PARTICIPATOR_RETURN,//负载BVCU_Conf_Participator_Info        

    BVCU_SUBMETHOD_SYNCHRONIZATION_NOTIFY = BVCU_SUBMETHOD_PU_SYNCHRONIZATION, //同步图片（下载PU上传的图片）。负载：BVCU_PUCFG_Picture
    BVCU_SUBMETHOD_PU_RECORDSTATUS_NOTIFY = BVCU_SUBMETHOD_PU_RECORDSTATUS,    //通知设备存储状态变化（录像、录音、GPS..）。负载：BVCU_PUCFG_RecordStatus
    
    BVCU_SUBMETHOD_LINKACTION_NOTIFY = 0x30000,//报警联动执行通知。负载BVCU_Event_LinkAction_Notify
    BVCU_SUBMETHOD_EVENT_NOTIFY,//发生报警事件通知。负载BVCU_Event_Source
    
    //=============收到的command=======================
    //应用程序在Session的OnCmmand回调中收到命令，处理后配置BVCU_Event_SessionCmd参数并执行命令的OnEvent回调，BVCU_Event_SessionCmd.iResult表示命令执行结果。
    //注意：不可以在OnCommand回调中执行OnEvent回调。
    //注释说明，输入负载表示接收的命令负载，回响负载表示OnEvent的BVCU_Event_SessionCmd.stContent的负载类型
    BVCU_SUBMETHOD_CONF_START_CMD = BVCU_SUBMETHOD_CONF_START, //语音会议开始，询问是否参加。
                                                               //输入负载：BVCU_Conf_BaseInfo；回响负载：无
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD_CMD = BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD,//被邀请加入集群。
                                                                                    //输入负载：BVCU_Conf_BaseInfo+本参与者的BVCU_Conf_Participator_Info
                                                                                    //回响负载：如果同意加入，负载BVCU_Conf_Participator_Info（应用程序应填写szAliasName），不同意为空
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN_CMD = BVCU_SUBMETHOD_CONF_PARTICIPATOR_JOIN,//用户请求加入集群，仅集群Admin收到。
                                                                                        //输入负载：BVCU_Conf_Participator_Info；回响负载：无
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK_CMD = BVCU_SUBMETHOD_CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK,//用户请求发言，集群Admin通过返回成功与否决定是否同意。负载BVCU_Conf_Participator_Info
    BVCU_SUBMETHOD_CONF_PARTICIPATOR_INVITE_SPEAK_CMD = BVCU_SUBMETHOD_CONF_PARTICIPATOR_INVITE_SPEAK,//被点名发言。输入负载：BVCU_Conf_Participator_Info；回响负载：无

    BVCU_SUBMETHOD_PU_OPENDIALOG_CMD = BVCU_SUBMETHOD_PU_OPENDIALOG, // PU请求建立会话。应用层回同意后，由应用层调用BVCU_Dialog_Open()创建会话。
                                                                    // 输入负载：BVCU_PUCFG_OpenDialog； 回响负载：BVCU_PUCFG_OpenDialog
    
    BVCU_SUBMETHOD_USER_KICKOUT_CMD = BVCU_SUBMETHOD_USER_KICKOUT,  // 当前用户被踢出。应用层只需提示用户被踢出，回复该命令后，库内部做下线处理。
                                                                    // 输入负载：BVCU_UCFG_Kickout； 回响负载：无
    
    //=============reserved=============
    BVCU_SUBMETHOD_RESERVED = 0x80000//之后的值内部使用
}BVCU_SUBMETHOD_TYPE;

//PTZ类型
typedef enum _BVCU_PTZ_TYPE{
    BVCU_PTZ_TYPE_INVALID = 0,
    BVCU_PTZ_TYPE_MACHINE,  // 机械云台
    BVCU_PTZ_TYPE_ELECTRON, // 电子云台
}BVCU_PTZ_TYPE;
//PTZ协议
typedef enum _BVCU_PTZ_PROTO_TYPE{
    BVCU_PTZ_PROTO_INVALID = 0,
    BVCU_PTZ_PROTO_USERDEFINED,
    BVCU_PTZ_PROTO_PELCO_D,
    BVCU_PTZ_PROTO_PELCO_P,
    BVCU_PTZ_PROTO_SAMSUNG,
    BVCU_PTZ_PROTO_VISCA,
    BVCU_PTZ_PROTO_YAAN,
}BVCU_PTZ_PROTO_TYPE;

//PU功能类型
typedef enum _BVCU_PUTYPE{
    BVCU_PUTYPE_ENCODER = 0,//编码器
    BVCU_PUTYPE_DECODER, //解码器
    BVCU_PUTYPE_STORAGE, //存储器
}BVCU_PUTYPE;

//语言
typedef enum _BVCU_LANGUAGE_TYPE{
    BVCU_LANGUAGE_INVALID = 0,
    BVCU_LANGUAGE_ENGLISH,// 英文 
    BVCU_LANGUAGE_CHINESE_SIMPLIFIED,// 简体中文 
    BVCU_LANGUAGE_CHINESE_TRADITIONAL,// 繁体中文
}BVCU_LANGUAGE_TYPE;

//WIFI安全类型
typedef enum _BVCU_WIFI_SECURITY_TYPE{
    BVCU_WIFI_SECURITY_TYPE_OPEN = 0,
    BVCU_WIFI_SECURITY_TYPE_SHARED,
    BVCU_WIFI_SECURITY_TYPE_WPA_PSK,
    BVCU_WIFI_SECURITY_TYPE_WPA2_PSK,
}BVCU_WIFI_SECURITY_TYPE;

//WIFI加密类型
typedef enum _BVCU_WIFI_CRYPT_TYPE{
    BVCU_WIFI_CRYPT_TYPE_NONE = 0,
    BVCU_WIFI_CRYPT_TYPE_WEP_40,//64
    BVCU_WIFI_CRYPT_TYPE_WEP_104,//128
    BVCU_WIFI_CRYPT_TYPE_TKIP,
    BVCU_WIFI_CRYPT_TYPE_AES,//CCMP
}BVCU_WIFI_CRYPT_TYPE;

//无线网络类型
typedef enum _BVCU_RADIONETWORK_TYPE{
    BVCU_RADIONETWORK_TYPE_INVALID = 0,
    BVCU_RADIONETWORK_TYPE_AUTO,
    BVCU_RADIONETWORK_TYPE_GPRS,
    BVCU_RADIONETWORK_TYPE_CDMA,
    BVCU_RADIONETWORK_TYPE_EDGE,
    BVCU_RADIONETWORK_TYPE_3GWCDMA,
    BVCU_RADIONETWORK_TYPE_3GTDSCDMA,
    BVCU_RADIONETWORK_TYPE_3GCDMA2K,
    BVCU_RADIONETWORK_TYPE_4GTDLTE,
    BVCU_RADIONETWORK_TYPE_4GFDLTE,
    BVCU_RADIONETWORK_TYPE_4GLTE,

    BVCU_RADIONETWORK_TYPE_GSM,
    BVCU_RADIONETWORK_TYPE_EDGEPLUS,
    BVCU_RADIONETWORK_TYPE_3GHSPA,
    BVCU_RADIONETWORK_TYPE_3GHSPAPLUS,
    BVCU_RADIONETWORK_TYPE_3GCDMA1X,
}BVCU_RADIONETWORK_TYPE;

//音频输入
typedef enum _BVCU_AUDIOIN_INPUT_TYPE{
    BVCU_AUDIOIN_INPUT_MIC = 0,
    BVCU_AUDIOIN_INPUT_LINEIN,
}BVCU_AUDIOIN_INPUT_TYPE;

//存储媒体类型
typedef enum _BVCU_STORAGEMEDIATYPE{
    BVCU_STORAGEMEDIATYPE_INVALID = 0,
    BVCU_STORAGEMEDIATYPE_SDCARD, //SD卡
    BVCU_STORAGEMEDIATYPE_HDD,//硬盘
}BVCU_STORAGEMEDIATYPE;

//encoder设备支持的流类型
typedef enum _BVCU_ENCODERSTREAMTYPE{
    BVCU_ENCODERSTREAMTYPE_INVALID = -1,
    BVCU_ENCODERSTREAMTYPE_STORAGE,//存储流
    BVCU_ENCODERSTREAMTYPE_PREVIEW,//传输流
    BVCU_ENCODERSTREAMTYPE_PICTURE,//图片流
}BVCU_ENCODERSTREAMTYPE;
//decoder设备支持的播放接口类型
typedef enum _BVCU_DISPLAY_FORMAT_TYPE{
    BVCU_DISPLAY_FORMAT_INVALID = 0,
    BVCU_DISPLAY_FORMAT_BNC = (1 << 0),
    BVCU_DISPLAY_FORMAT_DVI = (1 << 1),
    BVCU_DISPLAY_FORMAT_VGA = (1 << 2),
    BVCU_DISPLAY_FORMAT_HDMI = (1 << 3),
    BVCU_DISPLAY_FORMAT_YPbPr = (1 << 4),
}BVCU_DISPLAY_FORMAT_TYPE;
// 数据库表信息类型
typedef enum _BVCU_DBTABLE_TYPE
{
    BVCU_DBTABLE_TYPE_UNKNOWN = 0, // 空  描述（存储位置）
    BVCU_DBTABLE_TYPE_USERINFO = (1 << 0),   // 用户信息（CMS）
    BVCU_DBTABLE_TYPE_USERGROUP = (1 << 1),  // 用户组信息（CMS）
    BVCU_DBTABLE_TYPE_PUINFO = (1 << 2),     // 设备信息（CMS）
    BVCU_DBTABLE_TYPE_PUGROUP = (1 << 3),    // 设备组信息（CMS）
    BVCU_DBTABLE_TYPE_NRUINFO = (1 << 4),    // NRU信息（CMS）
    BVCU_DBTABLE_TYPE_VTDUINFO = (1 << 5),   // VTDU信息（CMS）
    BVCU_DBTABLE_TYPE_STRATEGY = (1 << 6),   // 存储（下载）计划信息（NRU）
    BVCU_DBTABLE_TYPE_CONFERENCE = (1 << 7), // 会议群组信息（CMS）
    BVCU_DBTABLE_TYPE_LINKACTION = (1 << 8), // 报警联动信息（CMS）
    BVCU_DBTABLE_TYPE_GPSDATA = (1 << 9),    // GPS数据（NRU）
    BVCU_DBTABLE_TYPE_FILEINDEX = (1 << 10), // 文件索引信息（NRU）
    BVCU_DBTABLE_TYPE_CULOGINLOG = (1 << 11),// CU上下线记录（CMS）
    BVCU_DBTABLE_TYPE_PULOGINLOG = (1 << 12),// PU上下线记录（CMS）
    BVCU_DBTABLE_TYPE_OPERATELOG = (1 << 13),// 操作日志记录（CMS）
    BVCU_DBTABLE_TYPE_DIALOGLOG = (1 << 14), // 通道操作记录（CMS）
    BVCU_DBTABLE_TYPE_EVENTSAVED = (1 << 15),// 平台事件记录（CMS）
}BVCU_DBTABLE_TYPE;
// 文件格式
typedef enum _BVCU_FILE_STORAGE_TYPE {
    // 可以同时指定多个文件格式，库内部自动做出调整
    BVCU_FILE_STORAGE_UNKOWN = 0,   // 空
    BVCU_FILE_STORAGE_MKV = (1 << 0),      // MKV格式
    BVCU_FILE_STORAGE_MP4 = (1 << 1),      // MP4格式
    BVCU_FILE_STORAGE_AAC = (1 << 2),      // AAC格式，如果媒体方向有视频，库内部会自动选择MKV
}BVCU_FILE_STORAGE_TYPE;
#endif

