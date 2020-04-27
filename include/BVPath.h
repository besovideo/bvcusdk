#ifndef __BVPATH_H__
#define __BVPATH_H__

#include "PUConfig.h"

enum
{ // 
    BVREGION_TYPE_UNKNOWN = 0,

    /* GPS位置限制（驶入/驶出 区域/路线） 和 速度限制
       iActionType有效时，GPS位置限制有效； iMaxSpeed/iMinSpeed 不为零时，速度限制有效
    */
    BVREGION_TYPE_ROUTE,  // 路线规划， iWidth 意义为道路宽度
    BVREGION_TYPE_REGION, // 区域围栏， iWidth 无意义（填零）
    BVREGION_TYPE_POINT,  // 圆形围栏， iWidth 意义为半径,  只能包含一个坐标点（pCoord只有一个成员）
    BVREGION_TYPE_RECT,   // 矩形围栏， iWidth 无意义（填零）,  只能包含两个坐标点（左上和右下）

    // 信息叠加（当GPS位置满足条件时，视频通道中叠加指定的信息）
    BVREGION_TYPE_OVERLAY_ROUTE = 0x100 ,  // 路段 信息叠加， iMaxSpeed/iMinSpeed 无效（填零）
    BVREGION_TYPE_OVERLAY_REGION, // 区域 信息叠加， iWidth/iMaxSpeed/iMinSpeed 无效（填零）
    BVREGION_TYPE_OVERLAY_POINT,  // 地点 信息叠加， iMaxSpeed/iMinSpeed 无效（填零）   iWidth意义为半径, 只能包含一个坐标点（pCoord只有一个成员）

    // 布防区域（只有GPS位置满足条件时，才上报报警）
    BVREGION_TYPE_ALARM_ROUTE = 0x200,  // 路段 报警布防， iMaxSpeed/iMinSpeed 无效（填零）
    BVREGION_TYPE_ALARM_REGION, // 区域 报警布防， iWidth/iMaxSpeed/iMinSpeed 无效（填零）
    BVREGION_TYPE_ALARM_POINT,  // 地点 报警布防， iMaxSpeed/iMinSpeed 无效（填零）   iWidth意义为半径, 只能包含一个坐标点（pCoord只有一个成员）
};

enum
{
    PATH_ACTION_TYPE_NONE = 0, // 不限制
    PATH_ACTION_TYPE_LEAVE = 1<<0, // 禁止离开
    PATH_ACTION_TYPE_ENTRY = 1<<1, // 禁止驶入
    PATH_ACTION_TYPE_ALL = PATH_ACTION_TYPE_LEAVE | PATH_ACTION_TYPE_ENTRY, // 禁止驶入和离开 （平台不使用)
};

typedef struct _BVCU_Path_Coord {
    int  iLongitude; //经度，东经是正值，西经负值，单位1/10000000度。大于180度或小于-180度表示无效值
    int  iLatitude; //纬度，北纬是正值，南纬是负值，单位1/10000000度。大于180度或小于-180度表示无效值
} BVCU_Path_Coord;

typedef struct _BVCU_Path_Base {
    char szID[BVCU_MAX_ID_LEN+1];    // 线路的ID
    char szPathName[BVCU_MAX_NAME_LEN+1]; //线路的可视化名称，（信息叠加时，改值为叠加信息）
    int  bEnable;    // 该条线路是否使能 0:否 1:是
    int  iActionType;// 禁止离开/禁止驶入 见 PATH_ACTION_TYPE_*  参考 BVREGION_TYPE_*
    int  iWidth;     // 宽度/半径(1/1000 m)  参考 BVREGION_TYPE_*
    int  iMaxSpeed;  // 最高速度(1/1000 m/s) 零表示不限速 参考 BVREGION_TYPE_*
    int  iMinSpeed;  // 最低速度(1/1000 m/s) 零表示不限速 参考 BVREGION_TYPE_*
    int  iBufferTime;// 违规缓冲时长(单位：s) 零表示违规立即报警。其它值表示违规时长达到iBufferTime秒后开始报警。
} BVCU_Path_Base;

typedef struct _BVCU_Path_Info {
    BVCU_Path_Base szBase;    //线路基本信息
    int iCoordSize;    // 线路中坐标的数目
    BVCU_Path_Coord* pCoord; //坐标集
} BVCU_Path_Info;

typedef struct _BVCU_Region_Base {
    char szID[BVCU_MAX_ID_LEN+1];     // 区域ID
    char szName[BVCU_MAX_NAME_LEN+1]; // 区域的可视化名称
    int  iVersion;      // 版本, 目前为 1
    int  iRegionType;   // 区域类型 见 BVREGION_TYPE_*
} BVCU_Region_Base;

typedef struct _BVCU_Region_Info {
    BVCU_Region_Base stBase; //区域的基本信息
    int iPathSize;
    BVCU_Path_Info* pPathInfo; //线路集
    BVCU_DayTimeSlice stWeek[7][BVCU_MAX_DAYTIMESLICE_COUNT];//一周的有效时间片划分，只有满足时间片时，限制才有效。
} BVCU_Region_Info;

typedef struct _BVCU_Region_Alarm {
    char szPUID[BVCU_MAX_ID_LEN+1];    //发生报警的PU
    char szPathID[BVCU_MAX_ID_LEN+1];  //报警所在哪条线路
    BVCU_PUCFG_GPSData szGPSData;      //PU当时的GPS数据
} BVCU_Region_Alarm;


//==========================================新线路/区域规划=======================================

// 线路/区域 信息
typedef struct _BVCU_Route_Path {
    char szID[BVCU_MAX_ID_LEN + 1];        // 线路/区域 ID号，创建者生成，需要唯一。（可做查询条件）
    char szName[BVCU_MAX_NAME_LEN + 1];    // 线路/区域 名称
    int  iRouteType;    // 线路/区域类型 见 BVREGION_TYPE_*
    int  iWidth;        // 宽度/半径(1/1000 m)  参考 BVREGION_TYPE_*
    int  iCoordSize;    // 线路/区域中坐标的数目，BVCU_SUBMETHOD_ROUTEPATH_LIST 命令时，坐标集为空
    BVCU_Path_Coord* pCoord; // 坐标集
} BVCU_Route_Path;

// 线路/区域规划，同一天内同一个用户/设备不超过256个规划。
typedef struct _BVCU_Route_Plan {
    char szID[BVCU_MAX_ID_LEN + 1];        // 规划ID号，创建者生成，需要唯一。（可做查询条件）
    char szName[BVCU_MAX_NAME_LEN + 1];    // 规划名称
    BVCU_WallTime  stBeginTime;    // 规划生效-开始时间（可做查询条件）
    BVCU_WallTime  stEndTime;      // 规划生效-结束时间（可做查询条件）
    int  bEnable;    // 该规划是否使能 0:否 1:是
    int  iActionType;// 禁止离开/禁止驶入 见 PATH_ACTION_TYPE_*  参考 BVREGION_TYPE_*
    int  iBufferTime;// 违规缓冲时长(单位：s) 零表示违规立即报警。其它值表示违规时长达到iBufferTime秒后开始报警。
    int  iTargetType;// 规划限制对象类型：0-设备 1-用户。（可做查询条件）
    char szTargetID[BVCU_MAX_ID_LEN + 1];        // 目前对象（用户/设备）ID号。（可做查询条件）
    char szRoutePathID[BVCU_MAX_ID_LEN + 1];     // 规划使用的线路/区域信息。对应 BVCU_Route_Path.
}BVCU_Route_Plan;


#endif