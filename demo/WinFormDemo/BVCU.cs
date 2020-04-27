/*
 * 本类为静态类，用于映射C++到托管C#
 * 结构体注释参考BVCU.h
 */

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace WindowsFormsTest
{
    public struct BVRect
    {
        public Int32 top;
        public Int32 left;
        public Int32 bottom;
        public Int32 right;
        public BVRect(Int32 t, Int32 l, Int32 b, Int32 r)
        {
            top = t;
            left = l;
            bottom = b;
            right = r;
        }
    }

    public struct BVCU_PUOneChannelInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = BVCU.BVCU_MAX_NAME_LEN + 1)]
        public string szName;
        public Int32 iChannelIndex;
        public Byte iMediaDir;
        public Byte iPTZIndex;
        Byte cReserved;
        Byte cReserved0;
    }

    public struct BVCU_DialogControlParam_Network
    {
        public Int32 iTimeOut;
        public Int32 iDelayMax;
        public Int32 iDelayMin;
        public Int32 iDelayVsSmooth;
        public BVCU_DialogControlParam_Network(Int32 timeOut, Int32 max, Int32 min, Int32 vs)
        {
            iTimeOut = timeOut;
            iDelayMax = max;
            iDelayMin = min;
            iDelayVsSmooth = vs;
        }
    }

    public struct BVCU_Event_Common
    {
        public int errorCode;
    }

    public struct BVCU_PUCFG_PTZControl
    {
        public Int32 iPTZCommand;
        public Int32 bStop;
        public Int32 iParam1;
        public Int32 iParam2;
        public Int32 iParam3;
    }

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

    public enum BVCU_PTZ_MOVE
    {
        BEGIN, STOP
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_RS232
    {
        public Int32 iDataBit;
        public Int32 iStopBit;
        public Int32 iParity;
        public Int32 iBaudRate;
        public Int32 iFlowControl;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_Preset
    {
        public Int32 iIndex;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        Byte[] m_szPreset;
        public string szPreset
        {
            get
            {
                return BVCU.GetUtf8Byte(m_szPreset);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_CruisePoint
    {
        public Int16 iPreset;
        public Int16 iSpeed;
        public Int32 iDuration;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_Cruise
    {
        public Int32 iIndex;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        Byte[] m_szName;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public BVCU_PUCFG_CruisePoint[] stPoints;
        public string szName
        {
            get
            {
                return BVCU.GetUtf8Byte(m_szName);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_PTZAttr
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public Int32[] iPTZProtocolAll;
        public Int32 iPTZProtocolIndex;
        public Int32 iAddress;
        public BVCU_PUCFG_RS232 stRS232;
        public Int32 bChangePreset;
        public Int32 bChangeCruise;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
        public BVCU_PUCFG_Preset[] stPreset;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public BVCU_PUCFG_Cruise[] stCruise;
        public Int32 iActiveCruiseID;
    }

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

    class BVCU
    {
        public const int BVCU_MAX_ID_LEN = 31;
        public const int BVCU_MAX_NAME_LEN = 63;
        public const int BVCU_RESULT_S_OK = 0;
        public const int FAIL_RETRY_TIMES = 10;
        public const int FAIL_RETRY_INTERVAL = 100;     // in millisecond
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

        public static string SessionName(string ip, int port)
        {
            return ip + ":" + port;
        }

#if DEBUG
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_DebugConsoleInit();
#endif


        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuInit(ref IntPtr handle);
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_CuRelease(IntPtr handle);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuLogin(IntPtr handle, ref IntPtr session, Byte[] serverIp, Int32 serverPort, Byte[] usrName, Byte[] usrPsw,
            int timeOutSec, EventHandler.BVCU_Server_OnEvent onEvent, EventHandler.BVCU_Server_ProcChannelInfo procChannelInfo);
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuLogout(IntPtr handle, IntPtr session);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuList(IntPtr handle, IntPtr session, EventHandler.BVCU_Cmd_OnGetPuList getPuList);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuBrowsePu(IntPtr handle, ref IntPtr dialog, IntPtr session, Byte[] puId, int channelNo,
            IntPtr hWnd, ref BVRect dispRect, int volume, int singleRecFileSec, Byte[] recFileDir, bool videoTrans, ref BVCU_DialogControlParam_Network netWork,
            EventHandler.BVCU_Dialog_OnDialogEvent onDlgEvent, EventHandler.BVCU_Dialog_OnStorageEvent onStoreEvent);
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuCloseDialog(IntPtr handle, IntPtr dialog);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuPlayVolume(IntPtr handle, IntPtr dialog, int volume);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSetRecordStorageParam(IntPtr handle, IntPtr dialog, byte[] fileDir, int fileSec);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSnapshot(IntPtr handle, IntPtr dialog, byte[] fileDir, int fileSec);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuResizeDialogWindow(IntPtr handle, IntPtr dialog, ref BVRect dispRect);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_CuSetPuControlResultProcFunc(IntPtr handle, EventHandler.BVCU_Cmd_ControlResult onCtrlRes);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSetPuPtzControl(IntPtr handle, IntPtr session, Byte[] puId, int device, IntPtr ptzCtrl);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuOpenTspDialog(IntPtr handle, ref IntPtr dialog, IntPtr session, Byte[] puId, int channelNo,
            EventHandler.BVCU_TspDialog_OnEvent onDlgEvent, EventHandler.BVCU_TspDialog_OnData onDlgData);

        //获取云台的属性
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuPtzAttr(IntPtr handle, IntPtr session, Byte[] puId, int device, EventHandler.BVCU_Cmd_OnGetPuPtzAttr onEvent);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuOpenGpsDialog(IntPtr handle, ref IntPtr dialog, IntPtr session, Byte[] puId, int channelNo,
            EventHandler.BVCU_GpsDialog_OnEvent onDlgEvent, EventHandler.BVCU_GpsDialog_OnData onDlgData);

        public static string GetUtf8Byte(Byte[] utf8)
        {
            return Encoding.UTF8.GetString(utf8).Split('\0')[0];
        }

        public static void FAILED(int result)
        {
            if (result != 0)
            {
                throw new Exception(Marshal.GetExceptionForHR(result).Message);
            }
        }

        public static bool SUCCEEDED(int result)
        {
            if (result == 0)
            {
                return true;
            }
            return false;
        }

        public static bool TRUE(int result)
        {
            if (result != 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        const Int32 BVCU_MEDIADIR_VIDEOSEND = 1;
        const Int32 BVCU_MEDIADIR_VIDEORECV = 2;
        const Int32 BVCU_MEDIADIR_AUDIOSEND = 4;
        const Int32 BVCU_MEDIADIR_AUDIORECV = 8;
        public struct AVDirection
        {
            public bool audioSnd;
            public bool audioRecv;
            public bool videoSnd;
            public bool videoRecv;
        }
        public static AVDirection GetAVDirection(int dir)
        {
            AVDirection result = new AVDirection();
            result.audioRecv = ((dir & BVCU_MEDIADIR_AUDIORECV) != 0);
            result.audioSnd = ((dir & BVCU_MEDIADIR_AUDIOSEND) != 0);
            result.videoRecv = ((dir & BVCU_MEDIADIR_VIDEORECV) != 0);
            result.videoSnd = ((dir & BVCU_MEDIADIR_VIDEOSEND) != 0);
            return result;
        }



        public struct Color
        {
            public Int32 format;    // Default value 0 stands for rgba color
            public UInt32 RGBA;
            public Color(UInt32 color)
            {
                format = 0;
                RGBA = color;
            }
        }

        public const int BVDISPLAY_TEXT_EFFECT_OUTLINE = 1;
        public const int FONT_DISPLAY_MODE_SIZE = 32;
        public struct FontDisplayMode
        {
            public Int32 size;
            public Int32 rate;
            public Int32 effect;
            public Color fontColor;
            public Color backColor;
            public Int32 effectSize;
        }

        public const int BVDISPLAY_INVALID_INDEX = -1;

        public delegate void DisplayFont(IntPtr dialog, Int64 timeStamp);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_DispSetDisplayFontFunc(DisplayFont dispFontFunc);

        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_CreateFont(IntPtr hwnd, ref int fontIdx, int fontSize, [MarshalAs(UnmanagedType.LPWStr)]String fontName);

        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_DisplayFont(IntPtr hwnd, int fontIdx, ref BVRect dispRect, ref Color color, [MarshalAs(UnmanagedType.LPWStr)]String text);

        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_SetFontDisplayMode(IntPtr hwnd, int fontIdx, ref FontDisplayMode effect);
    }
}
