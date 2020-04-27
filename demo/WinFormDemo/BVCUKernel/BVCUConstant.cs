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
            BVCU_RESULT_E_MAXRETRIES,              /*达到最大重试次数*/

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

    



        #endregion 属性


    }




}
