#ifndef __NRU_CONFIG_H__
#define __NRU_CONFIG_H__

#include "BVCUConst.h"
#include "BVCUCommon.h"
#include "BVSearch.h"
//##!!!!!!!注意：所有结构体的Reserved成员必须初始化为0!!!!!!!!!!!!!
//============NRU信息=======================
//存储介质
typedef struct _BVCU_NRUCFG_StorageMedia{
    char szName[BVCU_MAX_NAME_LEN+1];//名字。只读
    unsigned int  iTotalSpace;//总空间，单位MB。只读
    unsigned int  iFreeSpace;//剩余空间，单位MB。只读
    int  bInUse;//是否使用该存储器。可写
}BVCU_NRUCFG_StorageMedia;

typedef struct _BVCU_NRUCFG_NRUItem{
    char szID[BVCU_MAX_ID_LEN+1];      //ID。只读
    char szName[BVCU_MAX_NAME_LEN+1];  //名字。可写
    int  iStorageMediaCount;//存储器个数。只读
    int  iOnlineStatus; //在线状态，参见BVCU_ONLINE_STATUS_*。在上下线通知中，根据iOnlineStatus判断是上线通知还是下线通知
    int  iReserved[2];//保留，必须为0
}BVCU_NRUCFG_NRUItem;

typedef struct _BVCU_NRUCFG_NRUInfo{
    char szID[BVCU_MAX_ID_LEN+1];             //ID。只读
    char szManufacturer[BVCU_MAX_NAME_LEN+1]; //制造商名字。只读
    char szSoftwareVersion[BVCU_MAX_NAME_LEN+1]; //软件版本。只读
    char szHardwareVersion[BVCU_MAX_NAME_LEN+1]; //硬件版本。只读
    char szName[BVCU_MAX_NAME_LEN+1];//名字。可写
    int  iNoSpaceRule; //磁盘空间不足处理规则。0-停止录像，1-覆盖旧录像。可写
    int  iReserveRecordDays;//录像文件保留天数。可写
    int  iRecordFileLength;//录像文件时间长度。单位秒。。可写
    int  iReservePicDays;//抓图文件保留天数。可写
    int  iReserveGPSDays; // GPS数据保留天数。可写
    int  iStorageMediaCount;//存储器个数。只读
    BVCU_NRUCFG_StorageMedia* pStorageMedia;//存储器数组
    int  iOnlineStatus; //在线状态，参见BVCU_ONLINE_STATUS_*。在上下线通知中，根据iOnlineStatus判断是上线通知还是下线通知
    int  iMinSpaceLimit; // 最小磁盘空间需求，当剩余磁盘空间小于该值时，停止（覆盖）录像(iNoSpaceRule)，单位：Mbyte。不小于100.
    int  bAlwaysSaveAllGPS; // 是否自动保存所有在线设备的GPS数据。0-否，1-是
    int  bAutoSaveAVStream; // 是否自动保存打开的视频流。见 BVCU_NRU_AUTOSAVE_*
    int  iMaxTransferCount; // 最大文件传输并发数，所有的文件传输在一起的总数。0-不限制.
    int  iBandwidthLimit;   // 文件传输总限速，所有的文件传输在一起的限速，单位Kbytes。0-不限制.
    int  iMaxDownloadCount; // 最大下载计划下载文件并发数，要小于iMaxTransferCount限制，给其它文件传输留一定的资源。
    int  iFileFormat;       // 录像文件格式，目前只支持BVCU_FILE_STORAGE_MKV 和 BVCU_FILE_STORAGE_MP4，默认BVCU_FILE_STORAGE_MP4
    int  iReserved[2];//保留，必须为0
}BVCU_NRUCFG_NRUInfo;
enum 
{
    BVCU_NRU_AUTOSAVE_NONE = 0,            // 不自动存储
    BVCU_NRU_AUTOSAVE_PUREQ = 1 << 0,      // 自动保存设备主动上传视频（设备一键请求平台实时音视频录像）
    BVCU_NRU_AUTOSAVE_OPENVIDEO = 1 << 1,  // 自动保存所有打开的视频（只打开音频不存储）
    BVCU_NRU_AUTOSAVE_OPENAUDIO = 1 << 2,  // 自动保存所有打开的音频（只打开视频不存储）
    BVCU_NRU_AUTOSAVE_AVCALL = 1 << 3,     // 自动保存音视频通话媒体数据。
};

//============存储计划 与 下载计划=====================

typedef struct _BVCU_NRUCFG_Storage_Schedule_ListItem{
    /**  计划的名称，唯一。 由标示头 和 用户设置名称 组成，标示头不用显示给用户。
     *   存储计划以"SAVE_"开头，后面为用户设置的计划名称。
     *   下载计划以"DOWN_"开头，后面为用户设置的计划名称。
     *   没有标示头 或 不为以上已知标示时，默认为 存储计划。
    */
    char szName[BVCU_MAX_NAME_LEN+1];//计划的名称，唯一。
    BVCU_WallTime stBegin;//开始时间。全0表示立即生效。
    BVCU_WallTime stEnd;//结束时间。全FF表示永远不结束。在开始/结束时间这段范围内，计划有效。
}BVCU_NRUCFG_Storage_Schedule_ListItem;

typedef struct _BVCU_NRUCFG_Storage_Channel{
    int  iChannelIndex; //通道号, BVCU_SUBDEV_INDEXMAJOR_*
    union 
    { // 分别在录像计划和下载计划时使用不同有意义值
        struct  
        { // 下载计划有意义
            /* BVCU_PU_ONLINE_THROUGH_*组合，当设备上线方式包含在iNetThrough内才下载其录像。
              例如iNetThrough==BVCU_PU_ONLINE_THROUGH_ETHERNET，即只下载计划内以太网上线设备的录像
            */
            unsigned short  iOnlineThrough;
            unsigned short  iDownFileType; // 下载文件类型，BVCU_STORAGE_FILE_TYPE_*组合，不可为零。
        }stDownParam;
        int bRecordAudio;  //录像时是否录制音频 0-不录 1-录
    }stParam; // 通道存储（下载）计划参数
}BVCU_NRUCFG_Storage_Channel;

typedef struct _BVCU_NRUCFG_Storage_PU{
    char szID[BVCU_MAX_ID_LEN+1];  //设备ID
    int  iChannelCount; //pChannel数组成员个数
    BVCU_NRUCFG_Storage_Channel* pChannel; //通道数组
}BVCU_NRUCFG_Storage_PU;

typedef struct _BVCU_NRUCFG_Storage_Schedule{
    char szName[BVCU_MAX_NAME_LEN+1];//计划名称  同BVCU_NRUCFG_Storage_Schedule_ListItem.szName[]
    BVCU_DayTimeSlice stWeek[7][BVCU_MAX_DAYTIMESLICE_COUNT];//一周的时间片划分，每天BVCU_MAX_DAYTIMESLICE_COUNT个时间片
    BVCU_WallTime stBegin;//开始时间。全0表示立即生效。
    BVCU_WallTime stEnd;//结束时间。全FF表示永远不结束。在开始/结束时间这段范围内，计划有效。
    int  iPUCount; //pStoragePU数组成员个数
    BVCU_NRUCFG_Storage_PU* pStoragePU; //该计划涉及到的PU列表
}BVCU_NRUCFG_Storage_Schedule;

//手工远程录像到NRU
typedef struct _BVCU_NRUCFG_ManualRecord{
    char szID[BVCU_MAX_ID_LEN+1];  //设备ID
    int iChannelIndex;//通道号, BVCU_SUBDEV_INDEXMAJOR_*
    int bStart;//1-开始录像，0-停止录像
    int iLength;//存储时间长度，单位秒
    int iMediaDir;  //媒体方向, BVCU_MEDIADIR_*
}BVCU_NRUCFG_ManualRecord;

//手工远程抓拍到NRU
typedef struct _BVCU_NRUCFG_Snapshot{
    char szID[BVCU_MAX_ID_LEN+1];  //设备ID
    int iChannelIndex;//通道号, BVCU_SUBDEV_INDEXMAJOR_*
    int iCount;//抓拍张数，最大允许值为15
    int iInterval;//抓拍间隔，单位秒。最大允许值为60秒    
}BVCU_NRUCFG_Snapshot;

//手工备份数据库
typedef struct _BVCU_NRUCFG_BackupDB
{
    int iTables;  // 当前备份的表信息。见BVCU_DBTABLE_TYPE.组合。
    int iPercent; // 当前备份状态.0～100,-1表示不在备份状态。100表示已完成
    char szBackupFilePathName[BVCU_MAX_FILE_NAME_LEN + 1]; //当前备份文件完整路径+文件名。（可通过BVCU下载接口下载）。
    BVCU_WallTime stFinishTime;//当前备份完成时刻，UTC时间。
}BVCU_NRUCFG_BackupDB;

//手动删除文件
enum
{
    BVCU_NRU_DELFILE_TYPE_UNKNOWN = 0,
    BVCU_NRU_DELFILE_TYPE_FILES,    //特定文件条件删除
    BVCU_NRU_DELFILE_TYPE_FILEIDS   //特定文件ID删除
};
typedef struct _BVCU_NRUCFG_FileID_
{
    int iCount; //iRecordID 数目
    unsigned int* iRecordID;//存储索引条目ID数组，用于快速区分每个文件。
    char szSourceID[BVCU_MAX_ID_LEN + 1]; //文件所在源ID，NRU/PU
}BVCU_NRUCFG_FileID;

typedef struct _BVCU_NRUCFG_DeleteFile_Request_
{
    int iType;   //过滤类型,BVCU_NRU_DELFILE_TYPE_
    int iCount;  //过滤条件的个数
    union
    {
        BVCU_Search_FileFilter* stFileFilters;
        BVCU_NRUCFG_FileID* stFileIDFilters;
    };
}BVCU_NRUCFG_DeleteFileReq;

typedef struct _BVCU_NRUCFG_DeleteFile_Result_
{
    int iCount; //删除文件个数
}BVCU_NRUCFG_DeleteFileResult;
#endif
