#ifndef __BVCU_COMMON_H__
#define __BVCU_COMMON_H__
#include <BVCUConst.h>
typedef struct _BVCU_ImageSize{
    int iWidth;
    int iHeight;
}BVCU_ImageSize;

typedef struct _BVCU_RelativeSize {
    float fWidth;       //相对宽度，0.0001 ~ 1
    float fHeight;      //相对高度，0.0001 ~ 1
}BVCU_RelativeSize;

typedef struct _BVCU_ImagePos{
    int iLeft;
    int iTop;
}BVCU_ImagePos;

typedef struct _BVCU_ImageRect{
    int iLeft;
    int iTop;
    int iWidth;
    int iHeight;
}BVCU_ImageRect;

typedef struct _BVCU_RelativeRect {
    float fLeft;        //边界框左上角点的X轴相对坐标，0.0001 ~ 1
    float fTop;         //边界框左上角点的Y轴相对坐标，0.0001 ~ 1
    float fWidth;       //边界框的相对宽度，0.0001 ~ 1
    float fHeight;      //边界框的相对高度，0.0001 ~ 1
}BVCU_RelativeRect;

//墙上时间
typedef struct _BVCU_WallDate {
    int   iYear;
    char  iMonth;
    char  iDay;
    char  cReserved[2];//对齐
}BVCU_WallDate;

//一天中的一个时间片
typedef struct _BVCU_DayTimeSlice{
    char cHourBegin, cMinuteBegin, cSecondBegin;
    char cHourEnd, cMinuteEnd, cSecondEnd;
    char cReserved[2];//对齐
}BVCU_DayTimeSlice;
//注意：所有BVCU_DayTimeSlice stWeek[7][...]等表示一周中的时间片，stWeek[0]表示周日，stWeek[1]表示周一,...stWeek[6]表示周六

//墙上时间
typedef struct _BVCU_WallTime{
    short iYear; 
    char  iMonth; 
    char  iDay; 
    char  iHour;
    char  iMinute;
    char  iSecond;
    char  cReserved[1];//对齐
}BVCU_WallTime;

//视频制式
typedef struct _BVCU_VideoFormat{
    int iVideoFormat;//BVCU_VIDEOFORMAT_*。只读
    BVCU_ImageSize stImageSize[8];//该制式支持的图像分辨率。只读
    int iFPSMax[8];//不同分辨率对应的最大采样帧率。单位1/1000帧。例如25fps，需要设置为25*1000。只读
    int iIndex;//当前使用的分辨率索引。
}BVCU_VideoFormat;

//显示参数
#define BVCU_HWND void* 

typedef struct _BVCU_Display_Rect
{
    int iLeft;
    int iTop;
    int iRight;
    int iBottom;
}BVCU_Display_Rect;

// 候选地址(IDE)信息
enum
{
    BVCU_ICE_CAND_TYPE_HOST,
    BVCU_ICE_CAND_TYPE_SRFLX,
    //BVCU_ICE_CAND_TYPE_PRFLX,
    //BVCU_ICE_CAND_TYPE_RELAYED,
    BVCU_ICE_CAND_TYPE_MAX
};
typedef struct _BVCU_ENCFG_Candidate_
{
    unsigned short   iType; // 地址类型 BVCU_ICE_CAND_TYPE_*
    unsigned short   iPort; // 端口，网络序 同 sockaddr_in.sin_port
    unsigned int     s_ip;  // IP地址信息，网络序, 同 sockaddr_in.sin_addr
}BVCU_ENCFG_Candidate;
typedef struct _BVCU_ENCFG_ICEAgent_
{
    unsigned int iID; // ICE Session ID号，本地唯一标示
    unsigned int iCandCount;    // 候选人个数
    BVCU_ENCFG_Candidate pstCands[8];  // 候选人列表
}BVCU_ENCFG_ICEAgent;
//系统及运行状态信息
typedef struct _BVCU_ENCFG_NetAdapter_
{
    char szAdapterName[BVCU_MAX_NAME_LEN + 1]; // 适配器名称。空：表示无效
    char szConnectName[BVCU_MAX_NAME_LEN + 1]; // 连接名称。如："本地连接1".
    int  iIPv4Addr[8]; // IPv4地址配置，网络序。遇到零结束。
    int  iIPv4Mask[8]; // IPv4子网掩码配置,对应iIPv4Addr
    int  iIPv4Gateway[8]; // IPv4网关。
    int  iUploadRate;  // 当前上传速度。单位：KBit/s
    int  iDownloadRate;  // 当前下载速度。单位：KBit/s
}BVCU_ENCFG_NetAdapter;
typedef struct _BVCU_ENCFG_Progress_
{
    char szName[BVCU_MAX_NAME_LEN + 1]; // 进程名称.空：表示无效。
    int  iCPUUsage;       // CPU占用率，单位：1/1000
    int  iMemorySet;      // 专用物理内存占用，单位：KByte.
    int  iUploadRate;     // 当前上传速度。单位：KBit/s
    int  iDownloadRate;   // 当前下载速度。单位：KBit/s
}BVCU_ENCFG_Progress;
typedef struct _BVCU_ENCFG_SystemInfo_ 
{
    char szSystemName[BVCU_MAX_NAME_LEN + 1]; // 带有位数、版本信息的系统名称。如："windows 10 教育版 64位"
    char szCPUInfo[BVCU_MAX_NAME_LEN + 1]; // CPU信息。
    int  iMemoryRAM;       // 内存(RAM）总大小，单位： MByte。
    int  iCPUUsage;        // 当前CPU占用率， 单位： 1/1000
    int  iMemorySet;       // 当前物理内存占用，单位：KByte.
    BVCU_ENCFG_NetAdapter stNetAdapter[4];  // 当前网络情况，遇到szAdapterName为空结束。
    BVCU_ENCFG_Progress   stProgress[16];   // 当前进程情况，只获取占用CPU（内存，带宽）高的部分进程。，遇到szName为空结束。
}BVCU_ENCFG_SystemInfo;
// 回放控制参数
typedef struct _BVCU_CTRL_RTSP_
{
    // 命令回复时，以下参数要根据实际执行结果正确填写。
    void* hDialog; //会话的句柄。如：BVCU_HDialog
    char  szCmdType[16]; // "PLAY"、"PAUSE"、"TEARDOWN"。注意字母大写。
    int   iRange_npt_begin; // 指定播放开始位置。相对于文件开始位置的毫秒。-1：表示当前播放位置。
    int   iRange_npt_end;// 指定播放结束位置。相对于文件开始位置的毫秒。-1：表示播放到文件结束。
    int   iScale;        // 指定播放速度。单位1/100倍。例如：25,50,100,200,400分别对应0.25，0.5，1，2，4倍数。-1：不改变速度。

    int   iReserved[3];  // 保留，对齐。内部有使用，不要更改。
}BVCU_CTRL_RTSP;

enum {
    BVCU_LOG_LEVEL_UNKNOWN = -1,
    BVCU_LOG_LEVEL_DEBUG,
    BVCU_LOG_LEVEL_INFO,
    BVCU_LOG_LEVEL_NOTICE,
    BVCU_LOG_LEVEL_WARN,
    BVCU_LOG_LEVEL_ERROR,
    BVCU_LOG_LEVEL_CRIT,
    BVCU_LOG_LEVEL_ALERT,
    BVCU_LOG_LEVEL_FATAL,
    BVCU_LOG_LEVEL_MAX_COUNT
};

enum {
    BVCU_LOG_DEVICE_UNKNOWN = -1,
    BVCU_LOG_DEVICE_CONSOLE,
    BVCU_LOG_DEVICE_FILE,
    BVCU_LOG_DEVICE_MAX_COUNT,
};

typedef void(*BVCU_Log_Callback)(int level, const char* log);

#endif
