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

#endif

