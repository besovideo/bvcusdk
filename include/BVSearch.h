#ifndef __BV_SEARCH_H__
#define __BV_SEARCH_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
#include "PUConfig.h"
#include "BVEvent.h"
#include "UserConfig.h"
#include "BVCUConference.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!

//==============索引命令相关负载定义=====================

// 索引目标类型
enum 
{
    BVCU_SEARCH_TYPE_UNKNOWN = 0, // 未知  请求过滤条件 / 回复结果负载
    BVCU_SEARCH_TYPE_FILE,  // 文件索引  BVCU_Search_FileFilter / BVCU_Search_FileInfo
    BVCU_SEARCH_TYPE_LOG_CU_LOGIN, // CU上下线记录 BVCU_Search_CULoginLog / BVCU_Search_CULoginLog
    BVCU_SEARCH_TYPE_LOG_PU_LOGIN, // PU上下线记录 BVCU_Search_PULoginLog / BVCU_Search_PULoginLog
    BVCU_SEARCH_TYPE_LOG_OPERATE,  // 操作日志（记录命令操作） BVCU_Search_OperateLog / BVCU_Search_OperateLog
    BVCU_SEARCH_TYPE_LOG_DIALOG,   // 通道日志（记录通道操作） BVCU_Search_DialogLog / BVCU_Search_DialogLog
    BVCU_SEARCH_TYPE_PULIST,       // 设备列表  BVCU_Search_PUListFiler / BVCU_PUChannelInfo
    BVCU_SEARCH_TYPE_EVENT,        // 事件记录  BVCU_Search_EventFilter / BVCU_Event_SourceSaved
    BVCU_SEARCH_TYPE_USERLIST,     // 用户列表  BVCU_Search_UserFilter / BVCU_UCFG_User
    BVCU_SEARCH_TYPE_IM_MSG,       // 历史消息  BVCU_Search_IMMsgFilter / BVCU_Search_IMMsg
    BVCU_SEARCH_TYPE_UALIST,       // 登陆用户的UA设备列表  BVCU_Search_UAListFilter / BVCU_Search_UAInfo
};

// 离线消息索引选项
enum
{
    //发送者接收者条件,占用4bit
    BVCU_SEARCH_OP_IMMSG_SR_STRICT = (0 << 0),  // 严格按照发送者接收者索引条件
    BVCU_SEARCH_OP_IMMSG_SR_EACH = (1 << 0),    // 发送者和接收者互发消息

    //会议条件,占用2bit
    BVCU_SEARCH_OP_IMMSG_CONF_P2P = (0 << 4),   // 索引所有会议和单聊消息
    BVCU_SEARCH_OP_IMMSG_P2P = (1 << 4),        // 索引单聊消息
    BVCU_SEARCH_OP_IMMSG_CONF = (2 << 4),       // 索引会议消息

    //按照时间搜索还是消息ID搜索, 占用1bit
    BVCU_SEARCH_OP_IMMSG_TIME = (0 << 6),       // 根据时间索引
    BVCU_SEARCH_OP_IMMSG_ID = (1 << 6),         // 根据消息ID索引
};
#define BVCU_SEARCH_OP_GetIMMsgSR(OPMode) (((unsigned int)(OPMode)) & (0x0F))
#define BVCU_SEARCH_OP_GetIMMsgConf(OPMode) (((unsigned int)(OPMode)) & (0x030))
#define BVCU_SEARCH_OP_GetIMMSGTimeOrID(OPMode) (((unsigned int)(OPMode)) & (0x040))
#define BVCU_SEARCH_OP_SetIMMsgSR(OPMode,newMode) ((((unsigned int)(OPMode)) & (~0x0F)) | newMode)
#define BVCU_SEARCH_OP_SetIMMsgConf(OPMode,newMode) ((((unsigned int)(OPMode)) & (~0x030)) | newMode)
#define BVCU_SEARCH_OP_SetIMMSGTimeOrID(OPMode) ((((unsigned int)(OPMode)) & (~0x040)) | newMode)

// 索引信息
// 注意事项：每次索引的结果是动态的，iTotalCount(总数）可能会变，导致索引的结果有重叠
typedef struct _BVCU_Search_Info_
{
    int iType; // 见 BVCU_SEARCH_TYPE_*
    int iPostition; // 起始结果下标位置, 从 0 开始。
    // 请求的最大结果数 / 本次返回的结果数。
    // 建议不要超过1024，否则命令可能会失败。建议值128。
    // 例如上层界面一个页面显示50条结果，可以提前计算出总页数，用户操作页面时，每次请求两个页面的数据（100条，缓冲一个页面）
    int iCount; 
    int iTotalCount; // 索引到的总结果数。请求时无意义。
}BVCU_SearchInfo;

// 文件索引
typedef struct _BVCU_Search_File_Filter_
{
    int iChannelIndex; //Channel号 -1：不作为索引条件  
    unsigned int iFileSizeMin;  //文件大小的下限,单位字节  // 0：不作为索引条件  
    unsigned int iFileSizeMax;  //文件大小的上限,单位字节  // 0：不作为索引条件  
    int iRecordType;   //录像原因，BVCU_STORAGE_RECORDTYPE_*  // 0：不作为索引条件  
    int iFileType;     //文件类型, BVCU_STORAGE_FILE_TYPE_*   // 0：不作为索引条件   
    int iReserves[2];  //对其保留字段，填 0
    int iTimeCondition;//时间条件对应的时间字段类型。0，1：录像开始时间。2：录像结束时间。3：录像开始or结束时间。4：文件索引入库时间。
    long long iTimeBegin; // 时间条件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 必须填写有效值 
    long long iTimeEnd;   // 时间条件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 必须大于iTimeEnd
    char szPUID[BVCU_MAX_ID_LEN + 1];//PU/Conf ID 精确查找。空：不作为索引条件
    char szFileName[BVCU_MAX_FILE_NAME_LEN + 1]; // 文件名称，模糊匹配。 空：不作为索引条件
    char szDesc1[64];  //自定义描述1，精确查找   空：不作为索引条件
    char szDesc2[64];  //自定义描述2，模糊匹配   空：不作为索引条件
    char szUserID[BVCU_MAX_ID_LEN + 1];//设备使用者账号， 精确查找。空：不作为索引条件
}BVCU_Search_FileFilter;
typedef struct _BVCU_Search_File_Info_
{
    int iRecordType; //录像原因，BVCU_STORAGE_RECORDTYPE_*    
    int iFileType;   //BVCU_STORAGE_FILE_TYPE_*
    unsigned int iFileSize;   //文件大小，单位字节
    unsigned int iRecordID;//存储索引条目ID，用于快速区分每个文件。例如数据库自增ID，可以不提供(填 0)
    long long iTimeBegin; // 录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数
    long long iTimeEnd;   // 录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数
    long long iTimeRecord;// 文件索引入库时间，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 //对上传下载的文件有意义。
    //文件路径 + 文件名。录像（图片）文件名严格要求格式为："PU_%08X_%02d_xxx.xxx",PUID,channelIndex 或Conf_ID_0_xxx会议录音文件。
    char szFilePath[BVCU_MAX_FILE_NAME_LEN + 1];
    char szDesc1[64];  //自定义描述1   空：不作为索引条件
    char szDesc2[64];  //自定义描述2   空：不作为索引条件
    char szSourceID[BVCU_MAX_ID_LEN + 1]; //文件所在源ID，NRU/PU
}BVCU_Search_FileInfo;

// CU上下线记录
typedef struct _BVCU_Search_CULoginLog_
{
    char szCUID[BVCU_MAX_ID_LEN + 1]; // CU ID   空：不作为索引条件
    char szUserName[BVCU_MAX_NAME_LEN + 1]; // 登录用户名  空：不作为索引条件
    char szIP[BVCU_MAX_ID_LEN + 1]; // 登录IP地址  空：不作为索引条件
    int  iPort; // 登录端口号  0：不作为索引条件
    int  iApplierId;   // 登录时分配的登录标示  0：不作为索引条件
    int  iDuration;    // 本次上线在线时长，单位秒。  不作为索引条件
    int  iTotalDuration; // 以szCUID为标示统计的总上线时长，单位秒。 不作为索引条件
    long long iOnlineTime;  // 上线时间  0：不作为索引条件  作为索引条件时：表示iOnlineTime到iOfflineTime时间段内上线或下线的记录
    long long iOfflineTime; // 下线时间  0：不作为索引条件 从1970-01-01 00:00:00 +0000 (UTC)开始的秒数
}BVCU_Search_CULoginLog;

// PU上下线记录
typedef struct _BVCU_Search_PULoginLog_
{
    char szPUID[BVCU_MAX_ID_LEN + 1]; // PU ID   空：不作为索引条件
    char szIP[BVCU_MAX_ID_LEN + 1]; // 登录IP地址  空：不作为索引条件
    int  iPort; // 登录端口号  0：不作为索引条件
    int  iApplierId;  // 登录时分配的登录标示  0：不作为索引条件
    int  iDuration;   // 本次上线在线时长，单位秒。  不作为索引条件
    int  iTotalDuration; // 以szPUID为标示统计的总上线时长，单位秒。 不作为索引条件
    long long iOnlineTime; // 上线时间  0：不作为索引条件  作为索引条件时：表示iOnlineTime到iOfflineTime时间段内上线或下线的记录
    long long iOfflineTime;// 下线时间  0：不作为索引条件 从1970-01-01 00:00:00 +0000 (UTC)开始的秒数
}BVCU_Search_PULoginLog;

// 操作日志
typedef struct _BVCU_Search_OperateLog_
{
    char szSourceID[BVCU_MAX_ID_LEN + 1];//命令源ID号   空：不作为索引条件
    char szUserName[BVCU_MAX_NAME_LEN + 1]; // 命令源登录用户名(PU时为空)  空：不作为索引条件
    char szTargetID[BVCU_MAX_ID_LEN + 1];//命令目标ID号   空：不作为索引条件
    int  iMethod; //命令主类型 BVCU_METHOD_*  // 0：不作为索引条件  
    int  iSubMethod; //命令子类型 BVCU_SUBMETHOD_*  // 0：不作为索引条件 
    int  iApplierId;   //命令源登录标示  // 0：不作为索引条件  
    int  iTargetIndex; //目标子设备号 -1：不作为索引条件
    long long iOperateTime; // 操作发生时间，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件  
    union
    {
        long long iTimeEnd;   // 操作索引结束时间，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件  
        int iResult; // 操作命令执行结果 BVCU_Result
    }u; // 请求索引时 iTimeEnd，返回索引结果时 iResult
    char szDescription[128]; // 操作描述  不作为索引条件
}BVCU_Search_OperateLog;

// 通道日志
typedef struct _BVCU_Search_DialogLog_
{
    char szPUID[BVCU_MAX_ID_LEN + 1];//PU ID   空：不作为索引条件
    char szSourceID[BVCU_MAX_ID_LEN + 1];//命令源ID号   空：不作为索引条件
    char szUserName[BVCU_MAX_NAME_LEN + 1]; // 命令源登录用户名(PU时为空)  空：不作为索引条件
    int  iApplierId; //命令源登录标示  // 0：不作为索引条件  
    int  iChannelIndex; //Channel号 -1：不作为索引条件  
    int  iMediaDir; //打开的媒体方向   不作为索引条件  
    int  iDuration;  // 本次打开通道时长，单位秒。  不作为索引条件
    int  iCUPUDuration; // szSourceID打开该设备本通道的总时长，单位秒。 不作为索引条件
    int  iReserves[3];  //对其保留字段，填 0
    long long iOpenTime;  // 打开通道时间  0：不作为索引条件  作为索引条件时：表示iOpenTime到iCloseTime时间段内打开或关闭的记录
    long long iCloseTime; // 关闭通道时间  0：不作为索引条件  从1970-01-01 00:00:00 +0000 (UTC)开始的秒数
}BVCU_Search_DialogLog;

// 历史消息
typedef struct _BVCU_Search_IMMsg_
{
    char* szSendDevID;    // 信息发送者，设备ID
    char* szSendUserID;   // 信息发送者，登录用户ID
    char* szConfID;       // 信息目标群组。NULL：点对点消息
    char* szRecvDevID;    // 信息接收者，设备ID
    char* szRecvUserID;   // 信息接收者，登录用户ID
    char* szReserved;     // 64位对齐，保留，填NULL
    int  iHasBeenRecv;    // 消息是否已经接收者接收(0: 未接收, 1: 接收)
    int  iCombMsgOrder;   // 组合消息顺序序号(0: 表示非组合消息，1,2,3...，表示组合消息, -1表示结尾)
    BVCU_IM_Msg stMsg;
}BVCU_Search_IMMsg;

// 登陆用户的UA设备
typedef struct _BVCU_Search_UAInfo_
{
    char  szDevID[BVCU_MAX_ID_LEN + 1];      // 设备 ID "PU_xxxx"
    char  szDevName[BVCU_MAX_NAME_LEN + 1];  // 设备名称 Name
    char  szUserID[BVCU_MAX_ID_LEN + 1];     // 用户账号 ID
    char  szUserName[BVCU_MAX_NAME_LEN + 1]; // 用户名称 Name
    int iApplierID;  // CMS分配的登录标识 
    int iReserved[15]; // 保留，用于添加设备能力信息。
}BVCU_Search_UAInfo;


// 设备列表索引
// 注意：设备总列表会根据在线情况排序，为防止漏加载设备，当收到设备下线通知时，请将当前分页加载索引下标减1。
typedef struct _BVCU_Search_PUList_Filter_
{
    int  iOnlineStatus;   //设备在线状态 0：全部设备 1：在线设备 2：不在线设备
    int  iReserved;       //64位对齐
    long long iTimeBegin; // 设备最后上线时间范围，开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件
    long long iTimeEnd;   // 设备最后上线时间范围，结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件
    char szIDOrName[BVCU_MAX_NAME_LEN + 1];  // 设备名称(或PU ID）  空：不作为索引条件 （名称或设备ID，支持模糊查询）
    char szGroupID[BVCU_MAX_ID_LEN + 1]; // 设备所在组的id标识符  空：不作为索引条件
}BVCU_Search_PUListFilter;

// 事件索引
typedef struct _BVCU_Search_EventFilter_
{
    int  iEventType;  // 事件类型，BVCU_EVENT_TYPE_*，0：不作为索引条件
    int  iSubDevIdx;  // 子设备索引，如PU的视频输入等。-1: 不作为索引条件
    int  bProcessed;  // 是否处警。0-未处警，1-已处警。-1: 不作为索引条件
    int  iReserved;       //64位对齐
    long long iTimeBegin; // 事件发生时间范围，开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件
    long long iTimeEnd;   // 事件发生时间范围，结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数 // 0：不作为索引条件
    char szID[BVCU_MAX_NAME_LEN + 1];  // 源ID，例如PU ID/用户名等。空：不作为索引条件
}BVCU_Search_EventFilter;

// 用户列表索引
typedef struct _BVCU_Search_UserFilter_
{
    char szGroupId[AAA_ID_LEN]; // 用户所在组id。空：不作为索引条件
}BVCU_Search_UserFilter;

// 历史消息索引
typedef struct _BVCU_Search_IMMsg_Filter_
{
    char szSendDevID[BVCU_MAX_ID_LEN + 1];    // 信息发送者，设备ID, 空：不作为索引条件
    char szSendUserID[BVCU_MAX_ID_LEN + 1];   // 信息发送者，登录用户ID, 空：不作为索引条件
    char szConfID[BVCU_MAX_ID_LEN + 1];       // 信息目标群组
    char szRecvDevID[BVCU_MAX_ID_LEN + 1];    // 信息接收者，设备ID, 空：不作为索引条件
    char szRecvUserID[BVCU_MAX_ID_LEN + 1];   // 信息接收者，登录用户ID, 空：不作为索引条件
    char szCallID[BVCU_MAX_ID_LEN + 1];       // 视频通话CallID, 空：不作为检索条件
    long long iStartTime;        // 开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数；或者消息起始ID（包括该ID）, 必须填写
    long long iEndTime;          // 结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数；或者消息结束ID（包括该ID） // 0：表示到当前时刻或者当前最大的ID
    int  iHasBeenRecv;           // 消息是否已经接收者接收(0: 未接收, 1: 接收, -1: 筛选时不作为条件)
    int  iMsgType;               // 消息类型，见 BVCU_IM_TYPE_*, 0 筛选全部类型
    int  iOption;                // 搜索选项， BVCU_SEARCH_OP_IMMSG_*
    int  iReserved;              // 64位对齐
    char stMsgContent[512];      // 消息内容。模糊匹配   空：不作为索引条件
}BVCU_Search_IMMsgFilter;

// 登陆用户设备列表索引
typedef struct _BVCU_Search_UAList_Filter_
{
    char szDevIDOrName[BVCU_MAX_NAME_LEN + 1];   // 设备名称(或PU ID）  空：不作为索引条件 （名称或设备ID，支持模糊查询）
    char szUserIDOrName[BVCU_MAX_NAME_LEN + 1];  // 用户账号/名称  空：不作为索引条件 （名称或设备ID，支持模糊查询）
}BVCU_Search_UAListFilter;

// 索引请求负载
typedef struct _BVCU_Search_Request_
{
    BVCU_SearchInfo stSearchInfo;
    union
    {
        char    byUnionLen[640]; // 枚举最大内存，用于对齐。
        BVCU_Search_FileFilter stFileFilter;
        BVCU_Search_CULoginLog stCULoginLogFilter;
        BVCU_Search_PULoginLog stPULoginLogFilter;
        BVCU_Search_OperateLog stOperateFilter;
        BVCU_Search_DialogLog stDialogLogFilter;
        BVCU_Search_PUListFilter stPUListFilter;
        BVCU_Search_EventFilter stEventFilter;
        BVCU_Search_UserFilter stUserFilter;
        BVCU_Search_IMMsgFilter stIMMsgFilter;
        BVCU_Search_UAListFilter stUAListFilter;
    }stFilter; // 使用的具体类型见 stSearchInfo.iType
}BVCU_Search_Request;

// 索引回复负载
typedef struct _BVCU_Search_Response_
{
    BVCU_SearchInfo stSearchInfo;

    int iCount; //负载中结果个数，stSearchInfo.iCount为索引端填写的返回结果数，理论上和该值相同（考虑缓存区不够）。
    union
    {
        BVCU_Search_FileInfo* pFileInfo;
        BVCU_Search_CULoginLog* pCULoginLog;
        BVCU_Search_PULoginLog* pPULoginLog;
        BVCU_Search_OperateLog* pOperateLog;
        BVCU_Search_DialogLog* pDialogLog;
        BVCU_PUCFG_PUChannelInfo* pPUList;
        BVCU_Event_SourceSaved* pEventLog;
        BVCU_UCFG_User* pUserList;
        BVCU_Search_IMMsg* pIMMsgList;
        BVCU_Search_UAInfo* pUAList;
    }pData; // 索引结果列表。具体类型见 stSearchInfo.iType
}BVCU_Search_Response;

#endif

