#ifndef _BVCU_CONFERENCE_H_
#define _BVCU_CONFERENCE_H_

#include "BVCUConst.h"
#include "PUConfig.h"

/*=================================集群语音会议=================================*/
//会议类型
enum {
    //语音会议发言类型,占用4bit
    BVCU_CONF_MODE_SPEAK_DISCUSSIONGROUP = (0<<0),//讨论组模式。自由发言，无需申请。可以多个人同时发言。
    BVCU_CONF_MODE_SPEAK_CHAIRMAN = (1<<0), //演讲培训模式。发言者需申请，或者由chairman点名发言。同一时刻只能有一个人发言
    BVCU_CONF_MODE_SPEAK_PUBLIC = (2 << 0), //公众号模式。管理员发言是广播给所有人，普通发言者发言只有管理员能收到。

    //集群加入类型,占用3bit
    BVCU_CONF_MODE_JOIN_INVITE = (0<<4),//不能主动加入。Opener拉人进来，被拉者可以接受或拒绝。
    BVCU_CONF_MODE_JOIN_PASSWORD = (1<<4),//主动加入，但需要输入密码
    BVCU_CONF_MODE_JOIN_FREE = (2<<4),//主动加入，不提示密码

    //是否由服务器自动同意发言申请,占用1bit
    BVCU_CONF_MODE_APPLY_NEEDAGREE = (0 << 7),//需要管理员同意。演讲等模式时，会议成员申请发言权，需要会议管理员同意。
    BVCU_CONF_MODE_APPLY_AUTOAGREE = (1 << 7),//服务器自动同意。演讲等模式时，服务器自动同意会议成员的申请发言权请求。

    //会议是否永远开始（自动开始，不准停止）,占用1bit
    BVCU_CONF_MODE_START_STOPADMIN = (0 << 8),//由会议管理员来开始，停止会议。
    BVCU_CONF_MODE_START_FOREVER   = (1 << 8),//会议自动开始，并不允许停止。 只对 演讲模式生效。

    //会议是否自动开始平台录音（自动开始，不准停止）, 占用1bit
    BVCU_CONF_MODE_RECORD_STOPADMIN = (0 << 9),//由会议管理员来开始，停止会议。
    BVCU_CONF_MODE_RECORD_FOREVER = (1 << 9),//会议自动开始，并不允许停止。 只对 演讲模式生效。
};

#define BVCU_CONF_GetModeSpeak(ConfMode) (((unsigned int)(ConfMode)) & (0x0F))
#define BVCU_CONF_GetModeJoin(ConfMode) (((unsigned int)(ConfMode)) & (0x070))
#define BVCU_CONF_GetModeApply(ConfMode) (((unsigned int)(ConfMode)) & (0x080))
#define BVCU_CONF_GetModeStart(ConfMode) (((unsigned int)(ConfMode)) & (0x0100))
#define BVCU_CONF_GetModeRecord(ConfMode) (((unsigned int)(ConfMode)) & (0x0200))
#define BVCU_CONF_SetModeSpeak(ConfMode,newMode) ((((unsigned int)(ConfMode)) & (~0x0F)) | newMode)
#define BVCU_CONF_SetModeJoin(ConfMode,newMode) ((((unsigned int)(ConfMode)) & (~0x070)) | newMode)
#define BVCU_CONF_SetModeApply(ConfMode,newMode) ((((unsigned int)(ConfMode)) & (~0x080)) | newMode)
#define BVCU_CONF_SetModeStart(ConfMode,newMode) ((((unsigned int)(ConfMode)) & (~0x0100)) | newMode)
#define BVCU_CONF_SetModeRecord(ConfMode,newMode) ((((unsigned int)(ConfMode)) & (~0x0200)) | newMode)


//会议状态
enum{
    BVCU_CONF_STATUS_STOPPED = 0,
    BVCU_CONF_STATUS_STARTED,
};

//participator的集群权限。集群创建者creator拥有所有权限
enum{
    // 一个集群必须至少有一个参与者有ADMIN权限。如果最后一个ADMIN退出集群，集群被自动删除。
    BVCU_CONF_PARTICIPATOR_POWER_ADMIN = (1<<0),//集群管理。可打开/关闭会议、添加/删除集群成员等。
    BVCU_CONF_PARTICIPATOR_POWER_MODETATOR = (1<<1),//发言管理。可允许/禁止某个成员发言
};

//集群成员的会议状态
enum {
    BVCU_CONF_PARTICIPATOR_STATUS_UNKNOWN = -1,
    BVCU_CONF_PARTICIPATOR_STATUS_OFFLINE = BVCU_ONLINE_STATUS_OFFLINE,//下线
    BVCU_CONF_PARTICIPATOR_STATUS_ONLINE_LEAVED,//上线，但临时离开会议。这种情况下participator不可发言，也听不到其他人的发言
    BVCU_CONF_PARTICIPATOR_STATUS_ONLINE_INSEAT,//上线，并且正在参与会议
    BVCU_CONF_PARTICIPATOR_STATUS_ONLINE_SPEAKING,//上线，并且正在发言。只对BVCU_CONF_MODE_SPEAK_CHAIRMAN有效
};
//集群成员的会议标签
enum {
    BVCU_CONF_PARTICIPATOR_TAG_INVALID = 0,
    BVCU_CONF_PARTICIPATOR_TAG_RELOGIN = (1 << 0), // 是重登录会议成员。
    BVCU_CONF_PARTICIPATOR_TAG_DEFAULT = (1 << 1), // 该成员默认语音进入该会议。
};

//集群基本信息
typedef struct _BVCU_Conf_BaseInfo{
    char szName[BVCU_MAX_NAME_LEN+1];//名字。必须设置为非空。
    char szID[BVCU_MAX_ID_LEN+1];//集群ID。Create时保持为空，返回CMS设置的ID；其他命令必须设置
    int  iMode;//BVCU_CONF_MODE_*。必须设置
    char szPassword[BVCU_MAX_PASSWORD_LEN+1];//密码，仅对BVCU_CONF_NODE_JOIN_PASSWORD和BVCU_SUBMETHOD_CONF_CREATE命令有意义，其他情况设置为空
    int  iConfStatus;//语音会议状态,BVCU_CONF_STATUS_*，只读
    int  iTimeOut;   //多长时间没有发言者，服务器自动删除集群。单位：秒。<= 0表示没有超时限制。创建集群时填写后不允许修改。
    int  iRecordStatus;//平台对会议录音状态，0：不在录音；1：在录音。只读
    char szRecordNRUID[BVCU_MAX_ID_LEN + 1]; // 对会议录音的NRU ID，需要录音时设置，空：表示取消平台录音。
}BVCU_Conf_BaseInfo;


//集群成员
typedef struct _BVCU_Conf_Participator_Info{
    char szID[BVCU_MAX_ID_LEN+1];//participator ID。通常是PU/CU ID
    char szUserName[BVCU_MAX_NAME_LEN+1];//登录用户名，目前仅对CU有意义。PU为PU的Name
    char szAddr[BVCU_MAX_HOST_NAME_LEN+1];//地址信息，CU必须设置。PU可选
    char szAliasName[BVCU_MAX_NAME_LEN+1];//集群中使用的别名。可以为空。仅在BVCU_SUBMETHOD_CONF_PARTICIPATOR_INFO和CMS返回中填充
    int  iApplierID;//CMS分配的用户标识。添加CU参与者时(BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD),必须设置该值。其他情况下设置为无效值，应设置为0
    int  iPID;//用户加入或被加入集群时，CMS分配给该成员的成员ID，仅对该集群有效。-1表示无效值。
                //CU participator应保存该值，在断线重连等异常情况下，继续会议使用RETURN命令时，CMS根据登录用户名和iPID的组合来确定该用户继续会议。
    int  iAllowedMediaDir;//BVCU_MEDIADIR_*。仅对CONF_PARTICIPATOR_ADD/MODIFY命令有意义，其他命令无意义
    int  iStatus;//当前语音会议状态。BVCU_CONF_PARTICIPATOR_STATUS_*。由CMS填充为有意义的值。CU必须填写为BVCU_CONF_PARTICIPATOR_STATUS_UNKNOWN
    int  iPower;//权限，BVCU_CONF_PARTICIPATOR_POWER_*。作为control命令输入时，只有BVCU_CONF_PARTICIPATOR_POWER_ADMIN权限者的设置值起作用。
    int  iVolume;//获取信息的成员看到的szID成员会议音量。取值范围0~128，128表示原始音量。默认值应设置为128。
    //仅对BVCU_SUBMETHOD_CONF_PARTICIPATOR_VOLUME/BVCU_SUBMETHOD_CONF_INFO命令有意义
    int  iTag;  //此集群成员在语音会议中的一些标签。见BVCU_CONF_PARTICIPATOR_TAG_*。 只读。
    int  iReserved;//保留，必须设置为0
}BVCU_Conf_Participator_Info;

#define BVCU_CONF_MAX_PARTICIPATOR_ONETIME 1024 //一次CONF_PARTICIPATOR_ADD/REMOVE命令允许添加的集群成员最大数目
//CMS对"添加集群成员”命令处理结果
typedef struct _BVCU_Conf_Participator_AddResult{
    int iResultBits[BVCU_CONF_MAX_PARTICIPATOR_ONETIME/32];//按照BVCU_SUBMETHOD_CONF_PARTICIPATOR_ADD命令中数组顺序，
    //成功添加的位设置为1，失败的位设置为0。数组顺序i=>bit位映射关系：第[i/32]个int的[31-(i&31)]位
}BVCU_Conf_Participator_AddResult;

//集群信息
typedef struct _BVCU_Conf_Info{
    BVCU_Conf_BaseInfo baseInfo;//集群基本信息
    int iParticipatorCount;//集群成员个数
    BVCU_Conf_Participator_Info* pParticipators;//集群成员列表
    BVCU_Conf_Participator_Info* pCreator;//集群创建者，必须出现在成员列表中
    int  iReserved[2];//保留，必须设置为0
}BVCU_Conf_Info;

//申请加入集群
typedef struct _BVCU_Conf_Participator_Join{
    char szPassword[BVCU_MAX_PASSWORD_LEN+1];//密码，仅对BVCU_CONF_NODE_JOIN_PASSWORD和BVCU_SUBMETHOD_CONF_CREATE命令有意义，其他情况设置为空
    char szAliasName[BVCU_MAX_NAME_LEN+1];//集群中使用的别名。可以为空。
}BVCU_Conf_Participator_Join;

//邀请加入集群
typedef struct _BVCU_Conf_Participator_Invite {
    char szID[BVCU_MAX_ID_LEN + 1];//participator ID。通常是PU/CU ID
    char szUserName[BVCU_MAX_NAME_LEN + 1];//登录用户名，目前仅对CU有意义。PU为PU的Name
    int  iPID;//用户加入或被加入集群时，CMS分配给该成员的成员ID，仅对该集群有效。-1表示无效值。
              //应保存该值，在断线重连等异常情况下，继续参加语音会议使用RETURN命令时，CMS根据登录用户名和iPID的组合来确定该用户继续会议。
}BVCU_Conf_Participator_Invite;
/*================================================================*/


/*==============================即时通讯==========================*/
enum
{
    BVCU_IM_TYPE_UNKNOWN = 0,  // 未知
    BVCU_IM_TYPE_TEXT,         // 文字      szTextMsg  utf-8编码
    BVCU_IM_TYPE_FACE,         // 表情      szTextMsg  ascii
    BVCU_IM_TYPE_GPS,          // 地理位置  BVCU_PUCFG_GPSData
    BVCU_IM_TYPE_FILE,         // 文件      BVCU_IM_File
    BVCU_IM_TYPE_PICTURE,      // 图片      BVCU_IM_File
    BVCU_IM_TYPE_AUDIO,        // 语音      BVCU_IM_File
    BVCU_IM_TYPE_VIDEO,        // 视频      BVCU_IM_File
    BVCU_IM_TYPE_CONF_AUDIO,   // 会议实时语音片段      BVCU_IM_File
    BVCU_IM_TYPE_CALL_AUDIO,   // 语音通话  BVCU_IM_AV_CALL
    BVCU_IM_TYPE_CALL_VIDEO,   // 视频通话  BVCU_IM_AV_CALL
    BVCU_IM_TYPE_NOTIFY,       // 通知消息  BVCU_IM_Notify
    BVCU_IM_TYPE_CTRL,         // 操作消息  BVCU_IM_IMCtrl
    BVCU_IM_TYPE_CUSTOM = 31,  // 自定义    szTextMsg
};

enum
{
    BVCU_AVCALL_STATE_ALREADY_ANSWER = 0,   //已接听
    BVCU_AVCALL_STATE_NO_ANSWER,            //无人接听
    BVCU_AVCALL_STATE_CANCEL,               //取消通话
    BVCU_AVCALL_STATE_BUSY,                 //忙线
    BVCU_AVCALL_STATE_REFUSE                //拒绝接听
};

enum
{
    BVCU_IM_NOTIFY_SHAKE = (0 << 0),   // 震动（抖动）提醒。
    BVCU_IM_NOTIFY_BELL = (1 << BVCU_IM_TYPE_CUSTOM),     // 响铃提醒。使用自身默认的响铃语音。
    BVCU_IM_NOTIFY_TEXT = (1 << BVCU_IM_TYPE_TEXT),       // 文字提醒。需要组合文字消息。
    BVCU_IM_NOTIFY_PICTURE = (1 << BVCU_IM_TYPE_PICTURE), // 图片提醒。需要组合图片消息。
    BVCU_IM_NOTIFY_AUDIO = (1 << BVCU_IM_TYPE_AUDIO),     // 语音提醒。需要组合语音消息。
};
enum
{
    BVCU_IM_NOTIFY_ATTR_SYSTEM = (1 << 0),   // 系统提醒。
    BVCU_IM_NOTIFY_ATTR_CONF   = (1 << 1),   // 只提醒在参加语音会议的成员；群消息有效。
    BVCU_IM_NOTIFY_ATTR_SHOW   = (1 << 2),   // 在聊天框中显示。
    BVCU_IM_NOTIFY_ATTR_LOOP   = (1 << 3),   // 循环播放提醒音。
};
enum
{
    BVCU_IM_CTRL_REPLY   = (1 << 0),    // 回复消息。CTRL消息和回复的消息组合发送。
    BVCU_IM_CTRL_RETRACT = (1 << 1),    // 撤回消息。
};

typedef struct _BVCU_IM_AV_CALL
{
    char szCallID[BVCU_MAX_ID_LEN + 1];// 音视频通话ID标识，通过BVCU_Search_FileFilter.szDesc1可以检索通话相关音视频记录。
    int  iState;                       // 音视频通话结果状态。见BVCU_AVCALL_STATE_*，BVCU_AVCALL_STATE_ALREADY_ANSWER时通话时长有意义。
    unsigned int iDuration;            // 通话时长，单位，秒
    char szApplyerUserID[BVCU_MAX_ID_LEN + 1]; // 通话请求发起者账号。
}BVCU_IM_AV_CALL;

typedef struct _BVCU_IM_FILE_ {
    char szID[BVCU_MAX_ID_LEN + 1]; // 指定的文件服务器ID
    char szFilePath[BVCU_MAX_FILE_NAME_LEN + 1]; // 指定的文件路径+文件名
    unsigned int  iFileSize;   //文件大小，单位字节
    unsigned int  iDuration;   //音视频文件时长，单位毫秒。
}BVCU_IM_File;

typedef struct _BVCU_IM_NOTIFY_ {
    int iContent;       // 消息提醒的内容BVCU_IM_NOTIFY_* 组合。某些内容后面需要跟组合消息，例如文字提醒，需要跟文字组合消息。
    int iAttribute;     // 提醒消息的一些属性。BVCU_IM_NOTIFY_ATTR_* 组合。
}BVCU_IM_Notify;

typedef struct _BVCU_IM_IMCTRL_ {
    int iOperate;      // 消息操作动作类型；见BVCU_IM_CTRL_*。
    int iReserved;     // 64位对齐，保留，填0
    long long iMsgID;  // 消息操作针对的消息ID号。
}BVCU_IM_IMCtrl;

typedef struct _BVCU_IM_Msg_ {
    int  iType;        // 消息类型，见 BVCU_IM_TYPE_*
    int  iReserved;    // 64位对齐，保留，填0
    /* 消息来源和目标的ID信息 放在 命令（通知）的头中。
    int  iSourceID; //消息源ApplierID。CMS分配的用户标识。同BVCU_Conf_Participator_Info.iApplierID
    int  iTargetID; //消息目标ApplierID。CMS分配的用户标识。同BVCU_Conf_Participator_Info.iApplierID
    char szSourceID[BVCU_MAX_ID_LEN + 1]; //消息源 ID。通常是PU/CU ID
    char szTargetID[BVCU_MAX_ID_LEN + 1]; //消息目标 ID。通常是PU/CU/PU Group/Conference ID
    */
    long long iMsgID; //消息id标识。消息发送者不需要填写（清零），服务器生成ID，并回复发送者。
    // 消息内容， 有iType 确定类型， 见 BVCU_IM_TYPE_*
    union {
        char szTextMsg[512];//文字、表情、自定义
        BVCU_PUCFG_GPSData stGPSData; // 位置消息
        BVCU_IM_File    stFile;       // 文件、图片、语音、视频
        BVCU_IM_AV_CALL stCall;       // 音视频通话信息
        BVCU_IM_Notify  stNotify;     // 提醒消息
        BVCU_IM_IMCtrl  stCtrl;       // 操作消息。
    }stMsg;
    BVCU_WallTime stSendTime; //消息发送时间,UTC时间。消息发送者不需要填写（清零），以服务器接收到消息时间算。
} BVCU_IM_Msg;
/*================================================================*/

#endif