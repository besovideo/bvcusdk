#ifndef _BVCU_UserConfig_H_
#define _BVCU_UserConfig_H_

#include "AAABase.h"
#include "BVCUConst.h"

// 权限资源信息
typedef struct _BVCU_UCFG_Resource_
{
    // 设备ID号 
    char sPuID[AAA_ID_LEN];

    // 针对该设备的权限 
    AAA_PUPermissions szPermissions;

}BVCU_UCFG_Resource;

// 用户组的基本信息
typedef struct _BVCU_UCFG_UserGroup_ 
{
    // 组的id标识符 
    char sId[AAA_ID_LEN];

    // 组的名称 
    char sName[AAA_NAME_LEN];

    // 组的上级组ID, 顶层组的此值为空 
    char sParentId[AAA_ID_LEN];

    // 组的名称（简称） 
    char sAbbreviation[AAA_NAME_LEN];
}BVCU_UCFG_UserGroup;

// 用户组详细信息
typedef struct _BVCU_UCFG_UserGroupInfo_
{
    // 组的id标识符，创建时可以为空，成功回调通知ID 
    char sId[AAA_ID_LEN];

    // 组的名称 
    char sName[AAA_NAME_LEN];

    // 组的上级组 
    char sParentId[AAA_ID_LEN];

    // 组的名称（简称） 
    char sAbbreviation[AAA_NAME_LEN];

    // 描述该组的一些信息 
    char sDescription[AAA_DESCRIPTION_LEN];

    // 用户组拥有资源数  
    unsigned int   iResource;

    // 资源数组  
    BVCU_UCFG_Resource* pResource;

} BVCU_UCFG_UserGroupInfo;


// 用户的基本信息
typedef struct _BVCU_UCFG_User_
{
    // 用户的账号名,登录时使用的名称 
    char sId[AAA_ID_LEN];

    // 用户的组id 
    char sGroupId[AAA_ID_LEN];

    // 用户的姓名，可不设置 
    char sName[AAA_NAME_LEN];

}BVCU_UCFG_User;

// 用户的详细信息
typedef struct _BVCU_UCFG_UserInfo_
{
    // 用户的账号名,登录时使用的名称 
    char sId[AAA_ID_LEN];

    // 用户密码 
    char sPasswd[AAA_PASSWD_LEN];

    // 密码是否有效（0:无效，表示sPasswd不修改;其它值为有效） 
    int  bSetPasswd;

    // 系统管理权限 
    AAA_UserPermissions szSysadmin;

    // 指向分配此帐户的Server，第一版本不使用，可不设置 
    char sServerId[AAA_ID_LEN];

    // 用户的组id 
    char sGroupId[AAA_ID_LEN];

    // 用户的最大会话数，可不设置 
    int  iMaxSession;

    // 分配此用户的id 
    char sAllocateId[AAA_ID_LEN];

    // 用户的姓名，可不设置 
    char sName[AAA_NAME_LEN];

    // 用户的联系电话，可不设置 
    char sPhone[AAA_PHONE_LEN];

    // 用户的email，可不设置 
    char sEmail[AAA_EMAIL_LEN];

    // 用户的职称，可不设置 
    char sPosition[AAA_NAME_LEN];

    // 对此用户的描述，可不设置 
    char sDescription[AAA_DESCRIPTION_LEN];

    // 用户拥有资源数  
    unsigned int   iResource;

    // 资源数组  
    BVCU_UCFG_Resource* pResource;

}BVCU_UCFG_UserInfo;

// 修改用户密码结构体
typedef struct _BVCU_UCFG_Mod_passwd_
{
    // 用户的账号名, 登录时使用的名称 
    char sId[AAA_ID_LEN];

    // 用户的原密码,修改他人密码时不用填充此值，但需要管理权限 
    char sPassword[AAA_PASSWD_LEN];

    // 用户的新密码 
    char sNewPassword[AAA_PASSWD_LEN];

}BVCU_UCFG_ModPasswd;

// UKeyBase 基本信息
typedef struct _BVCU_UCFG_UKeyBase_
{
    // UKey ID 号 
    char sId[AAA_ID_LEN];

    // UKey 绑定的授权用户。可以为空，表示只添加授权UKey 
    char szUserId[AAA_ID_LEN];

}BVCU_UCFG_UKeyBase;

// UKeyInfo
typedef struct _BVCU_UCFG_UKeyInfo_
{
    // UKey ID 号 
    char sId[AAA_ID_LEN];

    // UKey 绑定的授权用户。可以为空，表示只添加授权UKey 
    char szUserId[AAA_ID_LEN];

    // 授权码 
    char code[AAA_PASSWD_LEN];

    // 创建该UKey记录的用户。  只读 
    char allocateUserId[AAA_ID_LEN];

    // 授权开始时间 
    BVCU_WallTime beginTime;

    // 授权结束时间 
    BVCU_WallTime endTime;

    // 对此UKey的描述，可不设置 
    char description[AAA_DESCRIPTION_LEN];

}BVCU_UCFG_UKeyInfo;

// 在线用户的基本信息
typedef struct _BVCU_UCFG_User_Online_
{
    // 用户的账号名,登录时使用的名称 
    char sUserId[AAA_ID_LEN];

    // 登录所用设备ID。通常是CU/UA ID 
    char szDevID[BVCU_MAX_ID_LEN + 1];

    // 用户名 
    char szUserName[BVCU_MAX_NAME_LEN + 1];

    // 地址信息 
    char szAddr[BVCU_MAX_HOST_NAME_LEN + 1];

    // 在线状态 
    int iStatus;//BVCU_ONLINE_STATUS_*

    // CMS分配的登录标识 
    int iApplierID;

    // 登录时间 
    BVCU_WallTime stLoginTime;

}BVCU_UCFG_User_Online;

typedef struct _BVCU_UCFG_Dialog_Channel_
{
    int iChannelIndex; // 会话通道号
    int iStreamIndex;  // 通道下属的流号。设置为-1表示由Server决定传输哪个流。见 BVCU_ENCODERSTREAMTYPE_*
    int iAVStreamDir;  // 会话的数据流方向，已CU为方向参考
}BVCU_UCFG_Dialog_Channel;

typedef struct _BVCU_UCFG_Dialog_PU_
{
    char szID[BVCU_MAX_ID_LEN + 1]; // 会话目标ID (PU ID)
    int  iDialogCount;  // 会话个数。
    BVCU_UCFG_Dialog_Channel* pDialog; // 会话列表
}BVCU_UCFG_Dialog_PU;

// 在线用户的基本信息
typedef struct _BVCU_UCFG_User_Online_Info_
{
    // 用户基本信息 
    BVCU_UCFG_User_Online stBase;

    // 正在观看的设备数 
    int iPUDialog;

    // 正在观看的设备列表 
    BVCU_UCFG_Dialog_PU* pPUDialog;
}BVCU_UCFG_User_OnlineInfo;

// 用户间权限查询,查询用户A针对B的权限。
typedef struct _BVCU_UCFG_User_Comparison_
{
    // 用户A的账号名, 与下面的iApplierIDA只需要填其中一个
    char sUserIdA[AAA_ID_LEN];
    // 用户A的登录标识， 0：无效值 
    int  iApplierIDA;

    // 用户A针对什么资源的权限 
    int iBType; // 0:用户  1:用户组  2:设备
    union
    {
        char  byUnionLen[64];
        struct
        {   // 用户B
            char sUserIdB[AAA_ID_LEN];
            int  iApplierIDB;
        };
        struct
        {   // 用户组B
            char sUserGroupId[AAA_ID_LEN];
        };
        struct
        {   // 设备B
            char sPUId[AAA_ID_LEN];
        };
    };
}BVCU_UCFG_User_Cmp;
typedef struct _BVCU_UCFG_User_Cmp_Result_
{
    // 管理权 
    AAA_Permissions config;

    // 是否可见(是否可以看到该用户/设备) 
    AAA_Permissions cansee;

    // 保留,内存对齐 
    AAA_Permissions reserve[2];

    // 等级关系， 用于用户（组）
    int iLevel;  // >0:A高于B, ==0:同级, <0:A低于B.

    // 通道权限（下标为通道索引号）,用于设备 
    AAA_Permissions channel[AAA_PU_CHANNEL_MAX + 1];

}BVCU_UCFG_User_CmpResult;

enum
{
    BVCU_UCFG_KICKOUT_REASON_NORMAL = 0, // 常规
    BVCU_UCFG_KICKOUT_REASON_OVERLOAD, // 服务器超载
    BVCU_UCFG_KICKOUT_REASON_DELUSER, // 用户被删除
    BVCU_UCFG_KICKOUT_REASON_KICKOUT, // 被在线用户踢出
    BVCU_UCFG_KICKOUT_REASON_UNWELL, // 看你不爽
    BVCU_UCFG_KICKOUT_REASON_RELOGIN, // 用户重登录
};

// 使在线用户下线
typedef struct _BVCU_UCFG_Kickout_
{
    // 使下线原因, BVCU_UCFG_KICKOUT_REASON_*  
    int  iReason; 

    // 目标用户的账号名  
    char szTargetId[AAA_ID_LEN];

    // 目标用户登录所用设备ID。通常是CU ID 
    char szDevID[BVCU_MAX_ID_LEN + 1];

    // 目标用户CMS分配的登录标识  
    int  iTargetApplierID;

    // 下线命令来源用户账户名，如果不是来源于在线用户，则为空 
    char szSourceId[AAA_ID_LEN]; // 只读，由服务器设置

    // 下线命令来源IP地址  
    char szAddr[BVCU_MAX_HOST_NAME_LEN + 1]; // 只读，由服务器设置

}BVCU_UCFG_Kickout;

#endif