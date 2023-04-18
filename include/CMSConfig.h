#ifndef __CMS_CONFIG_H__
#define __CMS_CONFIG_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!

//============CMS信息=======================

//CMS墙上时间
typedef struct _BVCU_CMSCFG_WallTime
{
    BVCU_WallTime stRequsterTime; // 发送获取cms时间时，请求者的UTC时间值
    BVCU_WallTime stCMSTime; // 收到命令时，CMS的UTC时间值
}BVCU_CMSCFG_WallTime;

// 后台http接口地址，serverE版本开始支持
typedef struct _BVCU_CMSCFG_HttpApi
{
    char szHttpUrl[128];  // http接口地址
    char szHttpsUrl[128]; // https接口地址
    char szToken[512]; // 当前登录实例token，用于发http请求
}BVCU_CMSCFG_HttpApi;

//CMS基本信息
typedef struct _BVCU_CMSCFG_BaseInfo
{
    unsigned short iCUPort; // cu监听端口，TCP
    unsigned short iPUPort; // pu监听端口，UDP
    unsigned short iDataPort; // 数据端口，UDP
    unsigned short iReserve;

    char cms_id[BVCU_MAX_ID_LEN + 1];  // CMS ID号
    char cms_version[BVCU_MAX_ID_LEN + 1];    // CMS 版本号
    char cms_displayname[BVCU_MAX_NAME_LEN + 1];// CMS 显示用的名称
    char cms_domain[BVCU_MAX_NAME_LEN + 1];  // CMS 域名
}BVCU_CMSCFG_BaseInfo;

//CMS通道信息
typedef struct _BVCU_CMSCFG_ChannelInfo
{
    char  szID[BVCU_MAX_ID_LEN + 1];  // 对象ID号。空：不作为索引条件
    int   iApplierID;    // CMS分配的登录标识，（请求者）。0：不作为索引条件
    int   iMediaDir;     // 流媒体方向 相对请求者。见 BVCU_MEDIADIR_*。不作为索引条件
    int   bOverTCP;      // 是否是TCP, 1:是 0：否 2:不支持TCP。不作为索引条件
}BVCU_CMSCFG_ChannelInfo;

typedef struct _BVCU_CMSCFG_DialogInfo
{
    BVCU_CMSCFG_ChannelInfo stRequestor;  // 请求者通道信息
    BVCU_CMSCFG_ChannelInfo stTarget;     // 被请求者通道信息
    char  szVTDUID[BVCU_MAX_ID_LEN + 1];  // 转发VTDU ID号。空：不作为索引条件
    int   iChannelIndex; // 通道号。-1：不作为索引条件
    int   iPathWay;      // 见 BVCU_STREAM_PATHWAY_* 。不作为索引条件
}BVCU_CMSCFG_DialogInfo;

#define BVCU_CMSCFG_MAX_BWLIST_COUNT 128    // 一次操作最大的黑白名单数

enum {
    BVCU_CMSCFG_BWLIST_OBJTYPE_PU = 1,  // PU类型
    BVCU_CMSCFG_BWLIST_OBJTYPE_CU,      // CU类型
};

typedef struct _BVCU_CMSCFG_BWList_Info
{
    int bEnableBW;      // 是否启用黑白名单，0：关闭黑白名单，1：黑名单，2：白名单
}BVCU_CMSCFG_BWList_Info;

typedef struct _BVCU_CMSCFG_BWList_Item
{
    long long iBWID;        // 黑白名单唯一ID
    int bBlack;             // 是否是黑名单，1：黑名单，0：白名单
    int iObjType;           // 目标对象类型，BVCU_CMSCFG_BWLIST_OBJTYPE_*
    char szObjID[BVCU_MAX_ID_LEN + 1]; // 目标ID，目标若是设备填设备ID，目标若是用户就填用户ID
}BVCU_CMSCFG_BWList_Item;

typedef struct _BVCU_CMSCFG_BWList_Filter
{
    int iPosition;      // 下标位置，从0开始
    int iCount;         // 最大BVCU_CMSCFG_MAX_BWLIST_COUNT个
    int iBWType;        // 黑白名单类型，0：不过滤类型，1：黑名单，2：白名单
}BVCU_CMSCFG_BWList_Filter;

typedef struct _BVCU_CMSCFG_BWList
{
    BVCU_CMSCFG_BWList_Filter filter;   // 过滤条件，BVCU_SUBMETHOD_BWLIST_GET用到
    int iTotalCount;    // 列表总数，BVCU_SUBMETHOD_BWLIST_GET用到
    int iCount;         // BVCU_CMSCFG_BWList_Item 个数
    BVCU_CMSCFG_BWList_Item* pList; // 黑白名单列表
}BVCU_CMSCFG_BWList;

#endif

