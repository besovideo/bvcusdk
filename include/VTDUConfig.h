#ifndef __VTDU_CONFIG_H__
#define __VTDU_CONFIG_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!
//============VTDU信息=======================
// 网络基本信息
typedef struct _BVCU_VTDUCFG_NetInfo_
{
    int iTotalUPBandwidth; // 上行总带宽， 单位：KBps。 可写
    int iTotalDWBandwidth; // 下行总带宽， 单位：KBps。 可写
    int iUsedUPBandwidth;  // 已占用上行带宽，单位：KBps。只读
    int iUsedDWBandwidth;  // 已占用下行带宽，单位：KBps。只读
    int iDialogCount;      // 在传会话个数。只读
}BVCU_VTDUCFG_NetInfo;

// 基本信息
typedef struct _BVCU_VTDUCFG_VTDUItem{
    char szID[BVCU_MAX_ID_LEN+1];      //ID。只读
    char szName[BVCU_MAX_NAME_LEN+1];  //名字。可写
    int  iDialogCount;  //在传会话个数。 只读
    int  iOnlineStatus; //在线状态，参见BVCU_ONLINE_STATUS_*。在上下线通知中，根据iOnlineStatus判断是上线通知还是下线通知
}BVCU_VTDUCFG_VTDUItem;

// 详细信息
typedef struct _BVCU_VTDUCFG_VTDUInfo_
{
    char szID[BVCU_MAX_ID_LEN+1];             //ID。只读
    char szSoftwareVersion[BVCU_MAX_NAME_LEN+1]; //软件版本。只读
    char szName[BVCU_MAX_NAME_LEN+1];         //名字。可写
    BVCU_VTDUCFG_NetInfo  stNetInfo;  // 网络基本信息。 可写
    BVCU_ENCFG_ICEAgent   stICEInfo;  // UDP数据端口地址信息。ICE Info。只读
	int  iTCPDataPort;                // TCP数据端口。注意不是网络序。只读
    int  iOnlineStatus; //在线状态，参见BVCU_ONLINE_STATUS_*。在上下线通知中，根据iOnlineStatus判断是上线通知还是下线通知
    BVCU_ENCFG_Candidate  stRegisterServer; // 上线服务器地址。可写
}BVCU_VTDUCFG_VTDUInfo;


#endif
