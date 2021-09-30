/// <summary>
/// 常量
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsTest
{
    public partial class BVCU
    {
        /// <summary>
        /// 常量属性
        /// </summary>
        #region 属性
        public const int BVCU_MAX_ID_LEN = 31;
        public const int BVCU_MAX_NAME_LEN = 63;
        public const int BVCU_MAX_ID_NAME_LEN = BVCU_MAX_NAME_LEN;
        public const int BVCU_RESULT_S_OK = 0;
        public const int FAIL_RETRY_TIMES = 10;//失败重试次数
        public const int FAIL_RETRY_INTERVAL = 100;//重试下一次的时间间隔(millisecond)
        public const int INVALID_CHANNEL_NO = -1;

        public const int BVCU_EVENT_SESSION_OPEN = 1;
        public const int BVCU_EVENT_SESSION_CLOSE = 2;

        public const int BVCU_EVENT_DIALOG_OPEN = 1;
        public const int BVCU_EVENT_DIALOG_UPDATE = 2;
        public const int BVCU_EVENT_DIALOG_CLOSE = 3;

        public const int BVCU_ONLINE_STATUS_OFFLINE = 0;
        public const int BVCU_ONLINE_STATUS_ONLINE = 1;

        public const int SAV_TIME_BASE = 1000000;

        public const int BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL = 0;
        public const int BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL = 0x0FFFF;
        public const int BVCU_SUBDEV_INDEXMAJOR_MIN_GPS = 0x010000; //GPS设备数据
        public const int BVCU_SUBDEV_INDEXMAJOR_MAX_GPS = 0x0100FF;
        public const int BVCU_SUBDEV_INDEXMAJOR_MIN_TSP = 0x010100; //透明串口设备数据
        public const int BVCU_SUBDEV_INDEXMAJOR_MAX_TSP = 0x0101FF;
        public const float BVCU_LAT_LNG_UNIT = 10000000.0f;


        /// <summary>
        /// BVCU_Result
        /// </summary>
        public enum BVCU_Result
        {
            BVCU_RESULT_E_FAILED = -0x10000, /*general error*/
            BVCU_RESULT_E_INVALIDARG,         /*invalid argument*/
            BVCU_RESULT_E_UNSUPPORTED,        /*unsupported functions*/
            BVCU_RESULT_E_ALLOCMEMFAILED,    /*allocate memory failed*/
            BVCU_RESULT_E_MEMALIGNMENT,      /*memory alignment is not satisfied*/
            BVCU_RESULT_E_NOTFOUND,          /*not found*/
            BVCU_RESULT_E_NOTALLOWED,        /*the requested access is not allowed*/
            BVCU_RESULT_E_IO,                /*I/O error*/
            BVCU_RESULT_E_EOF,               /*End of file*/
            BVCU_RESULT_E_INVALIDDATA,       /*Invalid data found when processing input*/
            BVCU_RESULT_E_NOTIMPL,           /*not implemented*/
            BVCU_RESULT_E_BUSY,              /*busy.deny service now*/
            BVCU_RESULT_E_INUSE,              /*device in use*/
            BVCU_RESULT_E_BADREQUEST,        /*bad request*/
            BVCU_RESULT_E_AUTHORIZE_FAILED,    /*authorize failed。登录/发送命令等的OnEvent回调中使用*/
            BVCU_RESULT_E_BADSTATE,          /*bad internal state*/
            BVCU_RESULT_E_NOTINITILIZED,      /*not initialized*/
            BVCU_RESULT_E_FATALERROR,        /*fatal error. BVCU should be closed*/
            BVCU_RESULT_E_OUTOFSPACE,        /*out of space*/
            BVCU_RESULT_E_DISCONNECTED,      /*disconnected*/
            BVCU_RESULT_E_TIMEOUT,            /*time out*/
            BVCU_RESULT_E_CONNECTFAILED,      /*connect failed*/
            BVCU_RESULT_E_ABORTED,            /*request aborted*/
            BVCU_RESULT_E_THRAEDCONTEXT,      /*can not execute in the specified thread context*/
            BVCU_RESULT_E_UNAVAILABLE,        /*unavailable, eg: initialize a dialog with an offlined PU*/
            BVCU_RESULT_E_ALREADYEXIST,       /*already exist*/
            BVCU_RESULT_E_SEVERINTERNAL,      /*Server internal error*/
            BVCU_RESULT_E_MAXRETRIES,         /*达到最大重试次数*/
            BVCU_RESULT_E_PASSWDFORMAT,       // 密码格式错误
            BVCU_RESULT_E_PASSWDEXPIRE,       // 密码过期
            BVCU_RESULT_E_PASSWD_RECENTUSED,  // 密码最近使用过

            BVCU_RESULT_E_AAA_OBJECTNOTFOUND = -0x0F000,/*AAA 用户/用户组等不存在。SESSION_CLOSE事件的OnEvent回调中使用*/

            BVCU_RESULT_S_OK = 0,            /*succeed*/
            BVCU_RESULT_S_IGNORE,            /*succeed,but something can not handle is ignored.*/
            BVCU_RESULT_S_PENDING,           /*operation is pending.*/
        };

        public const string VIDEO_AUDIO_CHANNEL = "音视频通道";
        public const string GPS_CHANNEL = "GPS通道";
        public const string TSP_CHANNEL = "TSP通道";
        public const string DEFAULT_CHANNEL_NAME_PREFIX = "通道";

        /// <summary>
        /// 云台控制命令
        /// </summary>
        public class BVCU_PTZ_COMMAND
        {
            public const int UP = 0;
            public const int DOWN = 1;
            public const int LEFT = 2;
            public const int RIGHT = 3;

            public const int LEFTTOP = 4;
            public const int RIGHTTOP = 5;
            public const int LEFTDOWN = 6;
            public const int RIGHTDOWN = 7;

            public const int ZOOM_INC = 8;
            public const int ZOOM_DEC = 9;

            public const int FOCUS_INC = 10;
            public const int FOCUS_DEC = 11;

            public const int APERTURE_INC = 12;
            public const int APERTURE_DEC = 13;

            public const int PRESET_GO = 14;
            public const int PRESET_SET = 15;
            public const int PRESET_SETNAME = 16;
            public const int PRESET_DEL = 17;

            public const int CRUISE_GO = 18;
            public const int CRUISE_STOP = 19;
            public const int CRUISE_SET = 20;
            public const int CRUISE_DEL = 21;

            public const int AUX = 22;
        }

        /// <summary>
        /// ???
        /// </summary>
        public class BVCU_RESULT_E
        {
            public const int FAILED = -0x10000;
            public const int INVALIDARG = FAILED + 1;
            public const int UNSUPPORTED = FAILED + 2;
            public const int ALLOCMEMFAILED = FAILED + 3;
            public const int MEMALIGNMENT = FAILED + 4;
            public const int NOTFOUND = FAILED + 5;
            public const int NOTALLOWED = FAILED + 6;
            public const int IO = FAILED + 7;
            public const int EOF = FAILED + 8;
            public const int INVALIDDATA = FAILED + 9;
            public const int NOTIMPL = FAILED + 10;
            public const int BUSY = FAILED + 11;
            public const int INUSE = FAILED + 12;
            public const int BADREQUEST = FAILED + 13;
            public const int AUTHORIZE_FAILED = FAILED + 14;
            public const int BADSTATE = FAILED + 15;
            public const int NOTINITILIZED = FAILED + 16;
            public const int FATALERROR = FAILED + 17;
            public const int OUTOFSPACE = FAILED + 18;
            public const int DISCONNECTED = FAILED + 19;
            public const int TIMEOUT = FAILED + 20;
            public const int CONNECTFAILED = FAILED + 21;
        }

        public class BVCU_SUBMETHOD
        {
            public const int SET = -1;
            public const int GET = -2;
            public const int PU_LIST = 0x00010;
            public const int PU_BROADCASTSTATUS = PU_LIST + 1;
            public const int PU_GPSDATA = PU_LIST + 2;
            public const int PU_STORAGE_MEDIA = PU_LIST + 3;//存储器信息。输入类型无; 输出类型：BVCU_PUCFG_Storage_Media数组
            public const int PU_GROUPLIST = PU_LIST + 4;
            public const int PU_GROUPINFO = PU_LIST + 5;
            public const int PU_UPDATESTATUS = PU_LIST + 6;
            public const int PU_RECORDSTATUS = PU_LIST + 12;

            public const int PU_DEVICEINFO = 0x10000;
            public const int PU_CHANNELINFO = PU_DEVICEINFO + 1;
            public const int PU_DEVICETIME = PU_DEVICEINFO + 2;
            public const int PU_SERVERS = PU_DEVICEINFO + 3;
            public const int PU_ETHERNET = PU_DEVICEINFO + 4;
            public const int PU_WIFI = PU_DEVICEINFO + 5;
            public const int PU_RADIONETWORK = PU_DEVICEINFO + 6;
            public const int PU_PTZATTR = PU_DEVICEINFO + 7;
            public const int PU_ENCODERCHANNEL = PU_DEVICEINFO + 8;
            public const int PU_VIDEOIN = PU_DEVICEINFO + 9;
            public const int PU_AUDIOIN = PU_DEVICEINFO + 10;
            public const int PU_AUDIOOUT = PU_DEVICEINFO + 11;
            public const int PU_ALERTIN = PU_DEVICEINFO + 12;
            public const int PU_ALERTOUT = PU_DEVICEINFO + 13;
            public const int PU_SERIALPORT = PU_DEVICEINFO + 14;
            public const int PU_GPS = PU_DEVICEINFO + 15;
            public const int PU_STORAGE_SCHEDULE = PU_DEVICEINFO + 16;//存储计划。输入类型：BVCU_PUCFG_Storage_Schdule;输出类型：无
            public const int PU_STORAGE_RULE = PU_DEVICEINFO + 17;//存储属性。输入类型：BVCU_PUCFG_Storage_Rule;输出类型：无
            public const int PU_STORAGE_FORMAT = PU_DEVICEINFO + 18;//格式化存储器。输入类型：BVCU_PUCFG_Storage_Format;输出类型：无
            public const int PU_ONLINECONTROL = PU_DEVICEINFO + 19;
            public const int PU_SNAPSHOTPARAM = PU_DEVICEINFO + 20;
            public const int PU_POWER = PU_DEVICEINFO + 21;
            public const int PU_GPSSPEEDLIMIT = PU_DEVICEINFO + 22;
            public const int PU_CARINFO = PU_DEVICEINFO + 23;
            public const int PU_BLUETOOTH = PU_DEVICEINFO + 24;
            public const int PU_VIDEOIN_CMOS = PU_DEVICEINFO + 25;
            public const int PU_ZFYINFO = PU_DEVICEINFO + 26;
            public const int PU_MISCHARDWARE = PU_DEVICEINFO + 27;

            public const int USER_GROUPLIST = 0x01000;
            public const int USER_GROUPINFO = USER_GROUPLIST + 1;
            public const int USER_USERLIST = USER_GROUPLIST + 2;
            public const int USER_USERINFO = USER_GROUPLIST + 3;
            public const int USER_ONLINE = USER_GROUPLIST + 4;
            public const int USER_ONLINEINFO = USER_GROUPLIST + 5;//获得在线用户信息 lcl 2014年01月15日 17:50:05
            public const int USER_CMP = USER_GROUPLIST + 6; //比较两个用户(组）/设备的权限关系。输入类型：BVCU_UCFG_User_Cmp； 输出类型：BVCU_UCFG_User_CmpResult

            public const int PU_REBOOT = 0x20000;
            public const int PU_DELETE = PU_REBOOT + 1;
            public const int PU_SHUTDOWN = PU_REBOOT + 2;
            public const int PU_SAVECONFIG = PU_REBOOT + 3;
            public const int PU_PTZCONTROL = PU_REBOOT + 4;
            public const int PU_PUTOFFLINE = PU_REBOOT + 5;
            public const int PU_MANUALRECORD = PU_REBOOT + 6;
            public const int PU_SNAPSHOT = PU_REBOOT + 7;
            public const int PU_UPGRADE = PU_REBOOT + 8;
            public const int PU_ADDGROUP = PU_REBOOT + 9;
            public const int PU_MODGROUP = PU_REBOOT + 10;
            public const int PU_DELGROUP = PU_REBOOT + 11;
            public const int PU_OPENDIALOG = PU_REBOOT + 12; //请求创建会话。输入类型：BVCU_PUCFG_OpenDialog； 输出类型：BVCU_PUCFG_OpenDialog
            public const int PU_SYNCHRONIZATION = PU_REBOOT + 13;

            public const int USER_ADDGROUP = 0x22000;
            public const int USER_MODGROUP = USER_ADDGROUP + 1;
            public const int USER_DELGROUP = USER_ADDGROUP + 2;
            public const int USER_ADDUSER = USER_ADDGROUP + 3;
            public const int USER_MODUSER = USER_ADDGROUP + 4;
            public const int USER_DELUSER = USER_ADDGROUP + 5;
            public const int USER_MODPASSWD = USER_ADDGROUP + 6;
            public const int USER_KICKOUT = USER_ADDGROUP + 7;

            public const int CONF_LIST = 0x01600;
            public const int CONF_INFO = CONF_LIST + 1;
            public const int CONF_DEL_LIST = CONF_LIST + 2;

            public const int CONF_PARTICIPATOR_VOLUME = 0x11200;
            public const int CONF_BASEINFO = CONF_PARTICIPATOR_VOLUME + 1;
            public const int CONF_DEFAULT = CONF_PARTICIPATOR_VOLUME + 2;// 获取/设置默认会议列表，BVCU_Command.szTargetID设置为 设备ID（或用户账号）
                                                                         // 输入类型：BVCU_Conf_BaseInfo数组；输出类型：无；触发类型：同名Notify

            public const int CONF_CREATE = 0x22600;
            public const int CONF_DELETE = CONF_CREATE + 1;
            public const int CONF_START = CONF_CREATE + 2;
            public const int CONF_STOP = CONF_CREATE + 3;
            public const int CONF_PARTICIPATOR_ADD = CONF_CREATE + 4;
            public const int CONF_PARTICIPATOR_REMOVE = CONF_CREATE + 5;
            public const int CONF_PARTICIPATOR_MODIFY = CONF_CREATE + 6;
            public const int CONF_PARTICIPATOR_INVITE_SPEAK = CONF_CREATE + 7;
            public const int CONF_PARTICIPATOR_TERMINATE_SPEAK = CONF_CREATE + 8;
            public const int CONF_PARTICIPATOR_JOIN = CONF_CREATE + 9;
            public const int CONF_PARTICIPATOR_EXIT = CONF_CREATE + 10;
            public const int CONF_PARTICIPATOR_APPLYFOR_STARTSPEAK = CONF_CREATE + 11;
            public const int CONF_PARTICIPATOR_APPLYFOR_ENDSPEAK = CONF_CREATE + 12;
            public const int CONF_PARTICIPATOR_LEAVE = CONF_CREATE + 13;
            public const int CONF_PARTICIPATOR_RETURN = CONF_CREATE + 14;
            public const int CONF_PARTICIPATOR_INFO = CONF_CREATE + 15;
            public const int CONF_VISITOR_JOIN = CONF_CREATE + 16;   //请求旁听会议。输入类型：无；输出类型：BVCU_Conf_Participator_Info
            public const int CONF_PARTICIPATOR_INVITE_JOIN = CONF_CREATE + 17;   //邀请加入会议，输入类型：BVCU_Conf_Participator_Invite；输出类型：无，BVCU_Command.iTargetIndex==1表示多组同时监听。
            public const int CONF_PARTICIPATOR_KICKOUT = CONF_CREATE + 18;       //踢出会议（区别删除会议参与者，这里是离开正在进行的会议），输入类型：BVCU_Conf_Participator_Invite；输出类型：无
            public const int IM_MSG = 0x22620;// 发送即时消息。输入类型：BVCU_IM_Msg数组 输出类型：BVCU_IM_Msg数组，用于获取各消息ID。消息接收方收到同名Notify。

            public const int LINKACTION_ADD = 0x22700;
            public const int LINKACTION_SET = LINKACTION_ADD + 1;
            public const int LINKACTION_DEL = LINKACTION_ADD + 2;
            public const int EVENT_PROCESS = LINKACTION_ADD + 3;

            public const int BVCU_SUBMETHOD_VTDU_LIST = 0x01300;

            public const int BVCU_SUBMETHOD_VTDU_INFO = 0x10F00;

            public const int BVCU_SUBMETHOD_CMS_BASEINFO = 0x2100;
            public const int BVCU_SUBMETHOD_CMS_DIALOGINFO = BVCU_SUBMETHOD_CMS_BASEINFO + 1;
            public const int BVCU_SUBMETHOD_CMS_TIME = BVCU_SUBMETHOD_CMS_BASEINFO + 2;

            public const int BVCU_SUBMETHOD_NRU_LIST = 0x1400;
            public const int BVCU_SUBMETHOD_NRU_SCHEDULE_LIST = BVCU_SUBMETHOD_NRU_LIST + 1;
            public const int BVCU_SUBMETHOD_NRU_SCHEDULE_GET = BVCU_SUBMETHOD_NRU_LIST + 2;

            public const int BVCU_SUBMETHOD_NRU_INFO = 0x11000;

            public const int BVCU_SUBMETHOD_NRU_DELETE = 0x22400;
            public const int BVCU_SUBMETHOD_NRU_SCHEDULE_SET = BVCU_SUBMETHOD_NRU_DELETE + 1;
            public const int BVCU_SUBMETHOD_NRU_SCHEDULE_DEL = BVCU_SUBMETHOD_NRU_DELETE + 2;
            public const int BVCU_SUBMETHOD_NRU_MANUALRECORD = BVCU_SUBMETHOD_NRU_DELETE + 3;

            public const int LINKACTION_LIST = 0x01700;
            public const int LINKACTION_GET = LINKACTION_LIST + 1;

            public const int BVCU_SUBMETHOD_SEARCH_LIST = 0x01800;

            public const int REGION_LIST = 0x1900;
            public const int REGION_GET = REGION_LIST + 1;
            public const int REGION_ROUTELIST = REGION_LIST + 2;
            public const int ROUTEPATH_GET = REGION_LIST + 3;
            public const int ROUTEPLAN_GET = REGION_LIST + 4;

            public const int REGION_ADD = 0x22900;
            public const int REGION_SET = REGION_ADD + 1;
            public const int REGION_DEL = REGION_ADD + 2;

            public const int ROUTEPATH_ADD = REGION_ADD + 3;
            public const int ROUTEPATH_SET = REGION_ADD + 4;
            public const int ROUTEPATH_DEL = REGION_ADD + 5;
            public const int ROUTEPLAN_ADD = REGION_ADD + 6;
            public const int ROUTEPLAN_SET = REGION_ADD + 7;
            public const int ROUTEPLAN_DEL = REGION_ADD + 8;


            public const int BVCU_SUBMETHOD_LINKACTION_NOTIFY = 0x30000; //报警联动执行通知。负载BVCU_Event_LinkAction_Notify
            public const int BVCU_SUBMETHOD_EVENT_NOTIFY = BVCU_SUBMETHOD_LINKACTION_NOTIFY + 1; //发生报警事件通知。负载BVCU_Event_Source

            // 解码器相关命令
            public const int BVCU_SUBMETHOD_DEC_LIST = 0x01500;
            public const int BVCU_SUBMETHOD_DEC_DEVICEINFO = 0x11100;
            public const int BVCU_SUBMETHOD_DEC_OPTIONS = BVCU_SUBMETHOD_DEC_DEVICEINFO + 1;
            public const int BVCU_SUBMETHOD_DEC_DECODER = BVCU_SUBMETHOD_DEC_DEVICEINFO + 2;
            public const int BVCU_SUBMETHOD_DEC_DISPLAY = BVCU_SUBMETHOD_DEC_DEVICEINFO + 3;
        }

        public class BVCU_EVENT_TYPE
        {
            public const int NONE = 0;

            //通用
            public const int DISKERROR = 0x0001;
            public const int OUTOFDISKSPACE = DISKERROR + 1;       //磁盘空间不足。源：PU/NRU的Storage
            public const int FACE = DISKERROR + 2;                 //人脸识别报警。源：PU

            //PU相关
            public const int VIDEOLOST = 0x1000;
            public const int VIDEOMD = VIDEOLOST + 1;
            public const int VIDEOOCCLUSION = VIDEOLOST + 2;
            public const int ALERTIN = VIDEOLOST + 3;
            public const int PERIOD = VIDEOLOST + 4;
            public const int PUONLINE = VIDEOLOST + 5;
            public const int PUOFFLINE = VIDEOLOST + 6;
            public const int LOWTEMPERATURE = VIDEOLOST + 7;        //低温报警。源：PU 的 TemperatureIn
            public const int HIGHTEMPERATURE = VIDEOLOST + 8;       //高温报警。源：PU 的 TemperatureIn
            public const int SLOWSPEED = VIDEOLOST + 9;             //低速报警。源：PU 的 GPS
            public const int OVERSPEED = VIDEOLOST + 10;             //超速报警。源：PU 的 GPS
            public const int INTOREGION = VIDEOLOST + 11;            //进入区域报警。源：PU 的 GPS
            public const int OUTREGION = VIDEOLOST + 12;             //出区域报警。源：PU 的 GPS
            public const int LOWVOLTAGE = VIDEOLOST + 13;            //低电压报警。源：PU 的 VoltageIn
            public const int HIGHVOLTAGE = VIDEOLOST + 14;           //高电压报警。源：PU 的 VoltageIn
            public const int DEVICESTILL = VIDEOLOST + 15;           //设备静止不动报警。源：PU 的 GPS
            public const int DEVICEINVERTED = VIDEOLOST + 16;        //设备被倒立放置报警。源：PU
            public const int REMBATTERY = VIDEOLOST + 17;            //设备电池被拆。源：PU 
            public const int DISASSEMBLE = VIDEOLOST + 18;           //设备被拆卸。源：PU 
            public const int LOWPOWER = VIDEOLOST + 19;           //设备低电量报警。源：PU 
            public const int INTROUTE = VIDEOLOST + 20;           //进入线路报警。源：PU 的 GPS 
            public const int OUTROUTE = VIDEOLOST + 21;           //偏移线路报警。源：PU 的 GPS 

            //NRU相关
            public const int NRUONLINE = 0x2000;
            public const int NRUOFFLINE = NRUONLINE + 1;

            //VTDU相关
            public const int VTDUONLINE = 0x3000;
            public const int VTDUOFFLINE = VTDUONLINE + 1;

            //CMS相关
            public const int CMSONLINE = 0x4000;
            public const int CMSOFFLINE = CMSONLINE + 1;

            //用户(User)相关
            public const int USERLOGIN = 0x5000;
            public const int USERLOGOUT = USERLOGIN + 1;

            //decorder(解码器）相关
            public const int DECONLINE = 0x6000;
            public const int DECOFFLINE = DECONLINE + 1;

            //该值及往后的值为自定义类型
            public const int CUSTOM = 0x10000000;
        }

        public static string ConvertEventType2String(int type)
        {
            switch (type)
            {
                case BVCU_EVENT_TYPE.DISKERROR://磁盘错误
                    return "磁盘错误";
                case BVCU_EVENT_TYPE.LOWPOWER://低电量报警
                    return "低电量报警";
                case BVCU_EVENT_TYPE.FACE://人脸识别报警
                    return "人脸识别报警";
                case BVCU_EVENT_TYPE.VIDEOLOST://视频丢失
                    return "视频丢失";
                case BVCU_EVENT_TYPE.VIDEOMD://运动检测
                    return "运动检测";
                case BVCU_EVENT_TYPE.VIDEOOCCLUSION://视频遮挡
                    return "视频遮挡";
                case BVCU_EVENT_TYPE.ALERTIN://报警输入
                    return "报警输入";
                case BVCU_EVENT_TYPE.PERIOD://周期报警
                    return "周期报警";
                case BVCU_EVENT_TYPE.PUONLINE://设备上线
                    return "设备上线";
                case BVCU_EVENT_TYPE.PUOFFLINE://设备下线
                    return "设备下线";
                case BVCU_EVENT_TYPE.LOWTEMPERATURE://低温报警
                    return "低温报警";
                case BVCU_EVENT_TYPE.HIGHTEMPERATURE://高温报警
                    return "高温报警";
                case BVCU_EVENT_TYPE.SLOWSPEED://低速报警
                    return "低速报警";
                case BVCU_EVENT_TYPE.OVERSPEED://超速报警
                    return "超速报警";
                case BVCU_EVENT_TYPE.INTOREGION://进入区域报警
                    return "进入区域报警";
                case BVCU_EVENT_TYPE.OUTREGION://出区域报警
                    return "出区域报警";
                case BVCU_EVENT_TYPE.LOWVOLTAGE://低电压报警
                    return "低电压报警";
                case BVCU_EVENT_TYPE.HIGHVOLTAGE://高电压报警
                    return "高电压报警";
                case BVCU_EVENT_TYPE.DEVICESTILL://设备静止不动报警
                    return "设备静止不动报警";
                case BVCU_EVENT_TYPE.DEVICEINVERTED://设备被倒立放置报警
                    return "设备被倒立放置报警";
                case BVCU_EVENT_TYPE.REMBATTERY://设备电池被拆
                    return "设备电池被拆";
                case BVCU_EVENT_TYPE.DISASSEMBLE://设备被拆卸
                    return "设备被拆卸";
                case BVCU_EVENT_TYPE.INTROUTE://进入线路
                    return "进入线路";
                case BVCU_EVENT_TYPE.OUTROUTE://离开线路
                    return "离开线路";
                case BVCU_EVENT_TYPE.NRUONLINE://存储服务器上线
                    return "存储服务器上线";
                case BVCU_EVENT_TYPE.NRUOFFLINE://存储服务器下线
                    return "存储服务器下线";
                case BVCU_EVENT_TYPE.VTDUONLINE://VTDU上线
                    return "VTDU上线";
                case BVCU_EVENT_TYPE.VTDUOFFLINE://VTDU下线
                    return "VTDU下线";
                case BVCU_EVENT_TYPE.CMSONLINE://服务器上线
                    return "服务器上线";
                case BVCU_EVENT_TYPE.CMSOFFLINE://服务器下线
                    return "服务器下线";
                case BVCU_EVENT_TYPE.USERLOGIN://用户登录
                    return "用户登录";
                case BVCU_EVENT_TYPE.USERLOGOUT://用户退出
                    return "用户退出";
                case BVCU_EVENT_TYPE.CUSTOM://用户自定义事件
                    return "用户自定义事件";
                default:// "未知类型";
                    return "";//MultiLanguage.ConvertString("EVENT_UNKNOWN");
            }
        }

        #endregion 属性
    }
}
