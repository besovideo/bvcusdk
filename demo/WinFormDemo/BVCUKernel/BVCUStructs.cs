///<summary>
/// 文件名: BVCUHelper.cs
/// 时间：2013-12-12 08：44
/// 创建者：lcl
/// 说明：
///     该文件是从原BVCU.cs中提取出的，代码部分未做任何修改
/// 
/// 更新：更新文件名为BVCUStructs.cs
/// 时间：2013-12-18 09:05
/// 更新者：lcl
/// 说明:
///     代码中的BVCU_RESULT_E，只更新文件名
///</summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace WindowsFormsTest
{
    /// <summary>
    /// 矩形，上下左右， 用于控制视频显示区域大小
    /// </summary>
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

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_ENCFG_Candidate
    {
        public UInt16 iType;    // 地址类型 BVCU_ICE_CAND_TYPE_*
        public UInt16 iPort;    // 端口，网络序 同 sockaddr_in.sin_port
        public UInt32 s_ip;     // IP地址信息，网络序, 同 sockaddr_in.sin_addr
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUChannelInfo
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVCU.BVCU_MAX_ID_LEN + 1)]
        Byte[] m_szPUID;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVCU.BVCU_MAX_NAME_LEN + 1)]
        Byte[] m_szPUName;
        public Int32 iChannelCount;
        public IntPtr pChannel;
        public Int32 iOnlineStatus;
        public Int32 iOnlineThrough;
        public BVCU_WallTime stOnlineTime;
        public BVCU_WallTime stBootTime;
        public Int32 iBootDuration;
        public Int32 iStreamPathWay;
        public BVCU_ENCFG_Candidate stIPAddr; // 设备上线地址信息（注意网络序的值)。
        public byte iAVCount;   //音视频通道数
        public byte iVideoInCount;   //视频输入数
        public byte iAudioInCount;   //音频输入数
        public byte iAudioOutCount;  //音频输出数
        public byte iPTZCount;       //PTZ数
        public byte iSerialPortCount;//串口数 
        public byte iAlertInCount;   //报警输入数
        public byte iAlertOutCount;  //报警输出数
        public byte iStorageCount;   //存储设备数
        public byte iGPSCount;       //GPS设备数
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] iReserve;     //保留对齐
        public Int32 iLongitude;
        public Int32 iLatitude;

        public string szPUID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szPUID);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szPUID, BVCU.BVCU_MAX_ID_LEN + 1);
            }
        }
        public string szPUName
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szPUName);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szPUName, BVCU.BVCU_MAX_NAME_LEN + 1);
            }
        }
    }

    /// <summary>
    /// Pu channel information
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUOneChannelInfo
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVCU.BVCU_MAX_NAME_LEN + 1)]
        Byte[] m_szName;
        public Int32 iChannelIndex;
        public Byte iMediaDir;
        public Byte iPTZIndex;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        Byte[] cReserved;

        public string szName
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szName);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szName, BVCU.BVCU_MAX_NAME_LEN + 1);
            }
        }
    }

    /// <summary>
    /// 网络 会话控制信息？
    /// 超时，最大延时，最小延时，？？延时
    /// </summary>
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

    /// <summary>
    /// 事件错误代码ErrorCode
    /// </summary>
    public struct BVCU_Event_Common
    {
        public int errorCode;
    }

    /// <summary>
    /// 云台控制
    /// </summary>
    public struct BVCU_PUCFG_PTZControl
    {
        public Int32 iPTZCommand;
        public Int32 bStop;
        public Int32 iParam1;
        public Int32 iParam2;
        public Int32 iParam3;
    }

    /// <summary>
    /// 云台控制，位置移动【开始、停止】
    /// </summary>
    public enum BVCU_PTZ_MOVE
    {
        BEGIN, STOP
    }

    /// <summary>
    /// 前端设备 串口 配置信息, 数据位、停止位、校验位、波特率...
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_RS232
    {
        /// <summary>
        /// 数据位
        /// </summary>
        public Int32 iDataBit;

        /// <summary>
        /// 停止位
        /// </summary>
        public Int32 iStopBit;

        public Int32 iParity;
        public Int32 iBaudRate;
        public Int32 iFlowControl;
    }

    /// <summary>
    /// 预设值配置
    /// </summary>
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

    /// <summary>
    /// 巡航的一点？，(预设置)？，速度，持续时间
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_CruisePoint
    {
        public Int16 iPreset;
        public Int16 iSpeed;
        public Int32 iDuration;
    }

    /// <summary>
    /// 巡游
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_Cruise
    {
        public Int32 iIndex;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        Byte[] m_szName;

        /// <summary>
        /// 巡游点集合
        /// </summary>
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

    /// <summary>
    /// 云台属性参数
    /// </summary>
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


    /// <summary><para>手工远程录像</para></summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_ManualRecord
    {
        /// <summary><para>1-开始录像，0-停止录像</para></summary>
        public Int32 bStart;
        /// <summary><para>存储时间长度，单位秒,</para>
        /// <para> -1表示使用 BVCU_PUCFG_Storage_Rule.iRecordFileLength</para>
        /// </summary>
        public Int32 iLength;
    }

    //GPS数据结构体 wfy on 20150417
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_PUCFG_GPSData
    {
        public BVCU_WallTime stTime;
        public Int32 iLongitude;
        public Int32 iLatitude;
        public Int32 iHeight;
        public Int32 iAngle;
        public Int32 iSpeed;
        public Int32 iStarCount;
        public Int32 bAntennaState;
        public Int32 bOrientationState;
        /// <summary>卫星信号来源 BVCU_PUCFG_SATELLITE_* 组合</summary>
        public int iSatelliteSignal;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        Int32[] iReserved;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Event_SessionCmd
    {
        public Int32 iResult;
        public Int32 iPercent;
        public BVCU_CmdMsgContent stContent;
    }


    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Command
    {
        public Int32 iSize;
        public IntPtr pUserData;
        public Int32 iMethod;
        public Int32 iSubMethod;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (BVSDKAdapter.BVCU_MAX_ID_LEN + 1))]
        Byte[] m_szSourceID;
        public Int32 iSourceID;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (BVSDKAdapter.BVCU_MAX_ID_LEN + 1))]
        Byte[] m_szTargetID;
        public Int32 iTargetIndex;
        public Int32 iTimeOut;
        public BVCU_CmdMsgContent stMsgContent;
        IntPtr OnEvent;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public Int32[] iReserved;
        public string szTargetID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szTargetID);
            }
            set
            {
                if (value == null) { value = ""; }
                BVSDKAdapter.String2Utf8Byte(value, ref m_szTargetID, BVSDKAdapter.BVCU_MAX_ID_LEN + 1);
            }
        }
        public string szSourceID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szSourceID);
            }
            set
            {
                if (value == null) { value = ""; }
                BVSDKAdapter.String2Utf8Byte(value, ref m_szSourceID, BVSDKAdapter.BVCU_MAX_ID_LEN + 1);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_WallTime
    {
        public Int16 iYear;
        public Byte iMonth;
        public Byte iDay;
        public Byte iHour;
        public Byte iMinute;
        public Byte iSecond;
        Byte cReserved;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_DayTimeSlice
    {
        public Byte cHourBegin;
        public Byte cMinuteBegin;
        public Byte cSecondBegin;
        public Byte cHourEnd;
        public Byte cMinuteEnd;
        public Byte cSecondEnd;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        Byte[] cReserved;

        public DateTime begin
        {
            get
            {
                return new DateTime(1970, 1, 1, cHourBegin, cMinuteBegin, cSecondBegin);
            }
        }

        public DateTime end
        {
            get
            {
                return new DateTime(1970, 1, 1, cHourEnd, cMinuteEnd, cSecondEnd);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_CmdMsgContent
    {
        public IntPtr pNext;
        public Int32 iDataCount;
        public IntPtr pData;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_NotifyMsgContent
    {
        IntPtr pNext;
        public Int32 iSubMethod;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (BVCU.BVCU_MAX_ID_LEN + 1))]
        Byte[] m_szSourceID;
        public Int32 iSourceIndex;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (BVCU.BVCU_MAX_ID_LEN + 1))]
        Byte[] m_szTargetID;
        public Int32 iTargetIndex;
        public BVCU_CmdMsgContent stMsgContent;
        public string szSourceID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szSourceID);
            }
        }
        public string szTargetID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szTargetID);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Event_Source
    {
        public Int32 iEventType;  // 事件类型
        public BVCU_WallTime stTime;  // 事件发生时刻
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVCU.BVCU_MAX_ID_NAME_LEN + 1)]
        Byte[] m_szID; // 源ID  
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVCU.BVCU_MAX_ID_NAME_LEN + 1)]
        Byte[] m_szDevID; //设备ID
        public Int32 iSubDevIdx; // 子设备索引号
        //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        //Int32[] iReserved;
        public Int32 iValue;  //保留给PU用,CU上下线时是CU.iApplierID
        public Int32 bEnd;  //是否是结束报警。0：开始报警。1：结束报警
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        Byte[] m_szEventDesc; // 报警描述

        public string szEventDesc
        {
            get
            {
                if (m_szEventDesc == null)
                    return " ";
                else
                    return BVSDKAdapter.GetUtf8Byte(m_szEventDesc);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szEventDesc, 128);
            }
        }
        public string szID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szID);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szID, BVCU.BVCU_MAX_ID_NAME_LEN + 1);
            }
        }

        public string szDevID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szDevID);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szDevID, BVCU.BVCU_MAX_ID_NAME_LEN + 1);
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct Test_Struct
    {
        public Int32 i;
    }

    public partial class BVSDKAdapter
    {
        public static T DeepClone<T>(T obj)
        {
            using (var ms = new System.IO.MemoryStream())
            {
                var formatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                formatter.Serialize(ms, obj);
                ms.Position = 0;

                return (T)formatter.Deserialize(ms);
            }
        }

        public static string GetUtf8Byte(Byte[] utf8)
        {
            return System.Text.Encoding.UTF8.GetString(utf8).Split('\0')[0];
        }

        public static void String2Utf8Byte(string str, ref Byte[] utf8, int byteLenth)
        {
            Byte[] source = System.Text.Encoding.UTF8.GetBytes(str);
            utf8 = new Byte[byteLenth];
            for (int i = 0; i <= source.Length; i++)
            {
                if (i == (byteLenth - 1))
                {
                    utf8[i] = 0;
                    break;
                }
                if (i == source.Length)
                {
                    utf8[i] = 0;
                }
                else
                {
                    utf8[i] = source[i];
                }
            }
        }

        public static IntPtr String2Utf8Intptr(string str)
        {
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(str);
            IntPtr ptr = Marshal.AllocHGlobal(buffer.Length + 1);
            Marshal.Copy(buffer, 0, ptr, buffer.Length);
            Marshal.WriteByte(ptr, buffer.Length, 0);
            return ptr;
        }

        /// <summary>
        /// GB2312转换成UTF8
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public static string gb2312_utf8(string text)
        {
            //声明字符集   
            System.Text.Encoding utf8, gb2312;
            //gb2312   
            gb2312 = System.Text.Encoding.GetEncoding("gb2312");
            //utf8   
            utf8 = System.Text.Encoding.GetEncoding("utf-8");
            byte[] gb;
            gb = gb2312.GetBytes(text);
            gb = System.Text.Encoding.Convert(gb2312, utf8, gb);
            //返回转换后的字符   
            return gb2312.GetString(gb);
        }

        /// <summary>
        /// UTF8转换成GB2312
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public static string utf8_gb2312(string text)
        {
            //声明字符集   
            System.Text.Encoding utf8, gb2312;
            //utf8   
            utf8 = System.Text.Encoding.GetEncoding("utf-8");
            //gb2312   
            gb2312 = System.Text.Encoding.GetEncoding("gb2312");
            byte[] utf;
            utf = utf8.GetBytes(text);
            utf = System.Text.Encoding.Convert(utf8, gb2312, utf);
            //返回转换后的字符   
            return utf8.GetString(utf);
        }

        public static bool WallTimeValidCheck(ref BVCU_WallTime walltime)
        {
            bool bValid = true;
            if (walltime.iYear < 1 || walltime.iYear > 9999)
            {
                walltime.iYear = 2000;
                bValid = false;
            }
            if (walltime.iMonth > 12 || walltime.iMonth <= 0)
            {
                walltime.iMonth = 12;
                bValid = false;
            }
            if (walltime.iDay < 1 || walltime.iDay > 31)
            {
                walltime.iDay = 10;
                bValid = false;
            }
            if (walltime.iHour >= 24)
            {
                walltime.iHour %= 24;
                bValid = false;
            }
            if (walltime.iMinute >= 60)
            {
                walltime.iMinute %= 60;
                bValid = false;
            }
            if (walltime.iSecond >= 60)
            {
                walltime.iSecond %= 60;
                bValid = false;
            }
            return bValid;
        }

        public static bool DayTimeSliceValidCheck(ref BVCU_DayTimeSlice oneDay)
        {
            bool bValid = true;
            if (oneDay.cHourBegin > 23)
            {
                oneDay.cHourBegin %= 24;
                bValid = false;
            }
            if (oneDay.cMinuteBegin > 59)
            {
                oneDay.cMinuteBegin %= 60;
                bValid = false;
            }
            if (oneDay.cSecondBegin > 59)
            {
                oneDay.cSecondBegin %= 60;
                bValid = false;
            }
            if (oneDay.cHourEnd > 23)
            {
                oneDay.cHourEnd %= 24;
                bValid = false;
            }
            if (oneDay.cMinuteEnd > 59)
            {
                oneDay.cMinuteEnd %= 60;
                bValid = false;
            }
            if (oneDay.cSecondEnd > 59)
            {
                oneDay.cSecondEnd %= 60;
                bValid = false;
            }
            if (oneDay.cHourEnd < oneDay.cHourBegin)
            {
                bValid = false;
            }
            if ((oneDay.cHourEnd == oneDay.cHourBegin && oneDay.cMinuteEnd < oneDay.cMinuteBegin))
            {
                bValid = false;
            }
            if ((oneDay.cHourEnd == oneDay.cHourBegin && oneDay.cMinuteEnd == oneDay.cMinuteBegin && oneDay.cSecondEnd < oneDay.cSecondBegin))
            {
                bValid = false;
            }
            return bValid;
        }

        private const double EARTH_RADIUS = 6378.137;//地球半径
        private static double rad(double d)
        {
            return d * Math.PI / 180.0;
        }

        /// <summary>获得两个经纬度的距离(单位: m)</summary>
        public static double GetTwoLatLngDistance(double lat1, double lng1, double lat2, double lng2)
        {
            double radLat1 = rad(lat1);
            double radLat2 = rad(lat2);
            double a = radLat1 - radLat2;
            double b = rad(lng1) - rad(lng2);

            double s = 2 * Math.Asin(Math.Sqrt(Math.Pow(Math.Sin(a / 2), 2) + Math.Cos(radLat1) * Math.Cos(radLat2) * Math.Pow(Math.Sin(b / 2), 2)));
            s = s * EARTH_RADIUS;
            s = Math.Round(s * 10000) / 10;
            return s;
        }


        public static string ParsePathGetFileName(string strFilePath, int iEndIndex = -1)
        {
            string szFileName = "";
            int iIndexFlagBackslash = strFilePath.LastIndexOf('\\');
            int iIndexFlagSlash = strFilePath.LastIndexOf('/');
            int iIndex = iIndexFlagBackslash > iIndexFlagSlash ? iIndexFlagBackslash : iIndexFlagSlash;
            if (iEndIndex == -1)
            {
                szFileName = strFilePath.Substring(iIndex + 1);
            }
            else
            {
                szFileName = strFilePath.Substring(iIndex + 1, iEndIndex);
            }
            return szFileName;
        }

        public const int BVCU_STORAGE_RECORDTYPE_NONE = 0;
        public const int BVCU_STORAGE_RECORDTYPE_MANUAL = 1;
        public const int BVCU_STORAGE_RECORDTYPE_ONTIME = 1 << 1;
        public const int BVCU_STORAGE_RECORDTYPE_ONALARM = 1 << 2;
        public const int BVCU_STORAGE_RECORDTYPE_AUTOSAVE = (1 << 3);//自动存储（根据自动存储配置）
        public const int BVCU_STORAGE_RECORDTYPE_UPLOAD = (1 << 4);//上传的文件（终端上传到平台的文件）
        public const int BVCU_STORAGE_RECORDTYPE_DOWNLOAD = (1 << 5);//下载的文件（平台从终端下载的文件）
        public const int BVCU_STORAGE_RECORDTYPE_VIDEOCALL = (1 << 6);//视频对讲
        public static string GetRecordReason(int iType)
        {
            if (iType == BVCU_STORAGE_RECORDTYPE_NONE)
            {
                return "不存储"; // "不存储";
            }
            string result = "";
            if ((iType & BVCU_STORAGE_RECORDTYPE_MANUAL) != 0)
            {
                result += "手动存储"; // "手动存储";
            }
            if ((iType & BVCU_STORAGE_RECORDTYPE_ONTIME) != 0)
            {
                result += "定时存储"; // "定时存储";
            }
            if ((iType & BVCU_STORAGE_RECORDTYPE_ONALARM) != 0)
            {
                result += "报警联动存储"; // "报警联动存储";
            }
            if ((iType & BVCU_STORAGE_RECORDTYPE_AUTOSAVE) != 0)
            {
                result += "自动存储"; // "自动存储";
            }
            if ((iType & BVCU_STORAGE_RECORDTYPE_UPLOAD) != 0)
            {
                result += "上传的文件"; // "上传的文件";
            }
            if ((iType & BVCU_STORAGE_RECORDTYPE_DOWNLOAD) != 0)
            {
                result += "下载的文件"; // "下载的文件";
            }
            return result;
        }

        public static DateTime getDateTimeFromMicroSecond(long tick)
        {
            DateTime result = new DateTime(1970, 1, 1, 0, 0, 0);
            TimeSpan timeSpan = new TimeSpan(tick * 10);
            result += timeSpan;
            TimeZone tz = TimeZone.CurrentTimeZone;
            result = tz.ToLocalTime(result);
            return result;
        }

        public const int BVCU_MAX_ID_LEN = 31;
        public const int BVCU_MAX_FILE_NAME_LEN = 255;

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
            public const int ABORTED = FAILED + 22;
            public const int THRAEDCONTEXT = FAILED + 23;
            public const int UNAVAILABLE = FAILED + 24;
            public const int ALREADYEXIST = FAILED + 25;
            public const int SEVERINTERNAL = FAILED + 26;
            public const int MAXRETRIES = FAILED + 27;

            public const int AAA_OBJECTNOTFOUND = -0x0F000;
            public const int VTDU_NONE = AAA_OBJECTNOTFOUND + 1;
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

            #region 【黑白名单部分】
            /// <summary>
            /// 获取黑白名单配置信息，输入类型: 无;输出类型: BVCU_CMSCFG_BWList_Info;
            /// </summary>
            public const int BVCU_SUBMETHOD_BWLIST_INFO_GET = 0x2200;
            /// <summary>
            /// 获取指定条件的黑白名单。输入类型：BVCU_CMSCFG_BWList_Filter，输出类型：BVCU_CMSCFG_BWList
            /// </summary>
            public const int BVCU_SUBMETHOD_BWLIST_GET = BVCU_SUBMETHOD_BWLIST_INFO_GET + 1;

            #region 【黑白名单管理】
            /// <summary>
            /// 设置黑白名单配置信息。输入类型：BVCU_CMSCFG_BWList_Info，输出类型：无
            /// </summary>
            public const int BVCU_SUBMETHOD_BWLIST_INFO_SET = 0x23100;
            /// <summary>
            /// 添加黑白名单。输入类型：BVCU_CMSCFG_BWList， 输出类型：BVCU_CMSCFG_BWList
            /// </summary>
            public const int BVCU_SUBMETHOD_BWLIST_ADD = BVCU_SUBMETHOD_BWLIST_INFO_SET + 1;
            /// <summary>
            /// 删除指定黑白名单。输入类型：BVCU_CMSCFG_BWList，输出类型：无
            /// </summary>
            public const int BVCU_SUBMETHOD_BWLIST_DEL = BVCU_SUBMETHOD_BWLIST_INFO_SET + 2;

            #endregion

            #endregion

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


        #region 文件检索
        // 索引目标类型
        public enum BVCU_SEARCH_TYPE
        {
            BVCU_SEARCH_TYPE_UNKNOWN = 0,  // 未知
            BVCU_SEARCH_TYPE_FILE,         // 文件索引  BVCU_Search_FileFilter / BVCU_Search_FileInfo
            BVCU_SEARCH_TYPE_LOG_CU_LOGIN, // CU上下线记录 BVCU_Search_CULoginLog / BVCU_Search_CULoginLog
            BVCU_SEARCH_TYPE_LOG_PU_LOGIN, // PU上下线记录 BVCU_Search_PULoginLog / BVCU_Search_PULoginLog
            BVCU_SEARCH_TYPE_LOG_OPERATE,  // 操作日志（记录命令操作） BVCU_Search_OperateLog / BVCU_Search_OperateLog
            BVCU_SEARCH_TYPE_LOG_DIALOG,   // 通道日志（记录通道操作） BVCU_Search_DialogLog / BVCU_Search_DialogLog
            BVCU_SEARCH_TYPE_PULIST,       // 设备列表  BVCU_Search_PUListFiler / BVCU_PUChannelInfo
            BVCU_SEARCH_TYPE_EVENT,        // 事件记录  BVCU_Search_EventFilter / BVCU_Event_SourceSaved
            BVCU_SEARCH_TYPE_USERLIST,     // 用户列表  BVCU_Search_UserFilter / BVCU_UCFG_User
            BVCU_SEARCH_TYPE_IM_MSG,       // 历史消息  BVCU_Search_IMMsgFilter / BVCU_Search_IMMsg
            BVCU_SEARCH_TYPE_UALIST,       // 登陆用户的UA设备列表  BVCU_Search_UAListFilter / BVCU_Search_UAInfo
        }

        public class BVCU_STORAGE_FILE_TYPE
        {
            public const int ALL = 0;
            public const int RECORD = 1 << 0;
            public const int CAPTURE = 1 << 1;
            public const int GPS = 1 << 2;
            public const int AUDIO = 1 << 3;// 音频文件
            public const int LOG = 1 << 4;//日志文件
            public const int FIRMWARE = 1 << 8;
        }

        public static long getSec(DateTime time)
        {
            DateTime origin = new DateTime(1970, 1, 1, 0, 0, 0, 0);
            TimeZone tz = TimeZone.CurrentTimeZone;
            time = tz.ToUniversalTime(time);
            TimeSpan diff = time - origin;
            return (long)diff.TotalSeconds;
        }


        #endregion

    }

    #region 文件检索
    // 索引信息
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_SearchInfo
    {
        public Int32 iType; // 见 BVCU_SEARCH_TYPE_*
        public Int32 iPostition; // 起始结果下标位置, 从 0 开始。
                                 // 请求的最大结果数 / 本次返回的结果数。
                                 // 建议不要超过1024，否则命令可能会失败。建议值128。
                                 // 例如上层界面一个页面显示50条结果，可以提前计算出总页数，用户操作页面时，每次请求两个页面的数据（100条，缓冲一个页面）
        public Int32 iCount;
        public Int32 iTotalCount; // 索引到的总结果数。请求时无意义。
    }


    // 文件索引

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Search_FileFilter
    {
        public Int32 iChannelIndex; //Channel号 -1：不作为索引条件  
        public UInt32 iFileSizeMin; //文件大小的下限,单位字节  // 0：不作为索引条件  
        public UInt32 iFileSizeMax; //文件大小的上限,单位字节  // 0：不作为索引条件  
        public Int32 iRecordType;//录像原因，BVCU_STORAGE_RECORDTYPE_*  // 0：不作为索引条件  
        public Int32 iFileType;// 文件类型, BVCU_STORAGE_FILE_TYPE_*  // 0：不作为索引条件  
        private Int32 iReserve1;
        private Int32 iReserve2;
        private Int32 iTimeCondition;//时间条件对应的时间字段类型。0，1：录像开始时间。2：录像结束时间。3：录像开始or结束时间。4：文件索引入库时间。
        public Int64 iTimeBegin; /*录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数*/ // 0：不作为索引条件  
        public Int64 iTimeEnd;   /*录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的微秒数*/ // 0：不作为索引条件  
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_ID_LEN + 1)]
        Byte[] szPUID;//PU ID   空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_FILE_NAME_LEN + 1)]
        Byte[] szFileName; // 文件名称，模糊匹配。  空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        Byte[] szDesc1; // 自定义描述1  空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        Byte[] szDesc2; // 自定义描述2  空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_ID_LEN + 1)]
        Byte[] szUserID;//设备使用者账号， 精确查找。空：不作为索引条件

        public string puID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szPUID);
            }
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szPUID, BVSDKAdapter.BVCU_MAX_ID_LEN + 1);
                initDesc();
            }
        }

        public string fileName
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szFileName, BVSDKAdapter.BVCU_MAX_FILE_NAME_LEN + 1);
            }
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szFileName);
            }
        }

        public string userID
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szUserID, BVSDKAdapter.BVCU_MAX_ID_LEN + 1);
            }
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szUserID);
            }
        }

        public string desc1
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szDesc1, 64);
            }
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szDesc1);
            }
        }

        public string desc2
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szDesc2, 64);
            }
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szDesc2);
            }
        }

        private void initDesc()
        {
            BVSDKAdapter.String2Utf8Byte("", ref szDesc1, 64);
            BVSDKAdapter.String2Utf8Byte("", ref szDesc2, 64);
        }
    }


    // 索引请求负载
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Search_Request
    {
        public BVCU_SearchInfo stSearchInfo;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 640)]
        public byte[] pData;
        public static byte[] CreateDataArray()
        {
            return new byte[640];
        }
    }

    // 索引回复负载
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Search_Response
    {
        public BVCU_SearchInfo stSearchInfo;
        public Int32 iCount; //负载中结果个数，stSearchInfo.iCount为索引端填写的返回结果数，理论上需和该值相同。
        public IntPtr pData;
        //}pData; // 索引结果列表。具体类型见 stSearchInfo.iType
    }

    public class SearchCommon
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public struct SearchUserData
        {
            public BVCU_SearchInfo stSearchInfo;
            public IntPtr pData;
            public Object oData;
            public object userData;
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_Search_FileInfo
    {
        public Int32 iRecordType; //录像原因，BVCU_STORAGE_RECORDTYPE_*    
        public Int32 iFileType;   // BVCU_STORAGE_FILE_TYPE_*
        public UInt32 iFileSize;   //文件大小，单位字节
        public Int32 iRecordID;   //存储索引条目ID，用于快速区分每个文件。例如数据库自增ID，可以不提供，填 0
        public Int64 iTimeBegin; /*录像文件开始时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数*/
        public Int64 iTimeEnd;   /*录像文件结束时刻，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数*/
        public Int64 iTimeRecord;/*文件索引入库时间，从1970-01-01 00:00:00 +0000 (UTC)开始的秒数*/ //对上传下载的文件有意义。
                                                                                    //文件路径 + 文件名。录像（图片）文件名严格要求格式为："PU_%08d_%02d_xxx.xxx",PUID,channelIndex
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_FILE_NAME_LEN + 1)]
        Byte[] szFilePath;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        Byte[] szDesc1; // 自定义描述1  空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        Byte[] szDesc2; // 自定义描述2  空：不作为索引条件
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_ID_LEN + 1)]
        Byte[] szSourceID; //文件所在源ID，NRU/PU
        public string filePath
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref szFilePath, BVSDKAdapter.BVCU_MAX_FILE_NAME_LEN + 1);
            }
            get
            {
                string strfilePath = BVSDKAdapter.GetUtf8Byte(szFilePath);
                if (strfilePath.EndsWith(".gps") && (strfilePath.Contains(@"\\") || strfilePath.Contains(@"//")))
                {
                    strfilePath = strfilePath.Replace(@"\\", "\\").Replace(@"//", "/");
                }
                return strfilePath;
            }
        }

        public string desc1
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szDesc1);
            }
        }

        public string desc2
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szDesc2);
            }
        }

        public string sourceID
        {
            get
            {
                return BVSDKAdapter.GetUtf8Byte(szSourceID);
            }
        }
    }

    public delegate void DeleOnTransferOpenEvent(IntPtr hTransfer, IntPtr pUserData, int iEventCode, int iResult);
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_File_TransferParam
    {
        Int32 iSize;
        public IntPtr pUserData;
        public IntPtr hSession;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BVSDKAdapter.BVCU_MAX_ID_LEN + 1)]
        Byte[] m_szTargetID;
        IntPtr pRemoteFilePathName;
        IntPtr pLocalFilePathName;
        Int32 iFileStartOffset;
        Int32 iTimeOut;
        Int32 bUpload;
        private DeleOnTransferOpenEvent OnEvent; //C#层不使用

        public bool bIsUpload
        {
            get
            {
                return bUpload == 1;
            }
            set
            {
                bUpload = value ? 1 : 0;
            }
        }

        public string szTargetID
        {
            set
            {
                BVSDKAdapter.String2Utf8Byte(value, ref m_szTargetID, BVSDKAdapter.BVCU_MAX_ID_LEN + 1);
            }
            get
            {
                return BVSDKAdapter.GetUtf8Byte(m_szTargetID);
            }
        }

        /// <summary>
        /// 可能为空
        /// </summary>
        public string szRemoteFilePathName
        {
            get
            {
                if (pRemoteFilePathName != IntPtr.Zero)
                {
                    return Marshal.PtrToStringAuto(pRemoteFilePathName);
                }
                return null;
            }
            set
            {
                if (pRemoteFilePathName != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(pRemoteFilePathName);
                    pRemoteFilePathName = IntPtr.Zero;
                }
                pRemoteFilePathName = BVSDKAdapter.String2Utf8Intptr(value);
            }
        }

        public IntPtr ptrRemoteFilePathName
        {
            set
            {
                pRemoteFilePathName = value;
            }
            get
            {
                return pRemoteFilePathName;
            }
        }

        public string szLocalFilePathName
        {
            get
            {
                if (pLocalFilePathName != IntPtr.Zero)
                {
                    return Marshal.PtrToStringAuto(pLocalFilePathName);
                }
                return null;
            }
            set
            {
                if (pLocalFilePathName != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(pLocalFilePathName);
                    pLocalFilePathName = IntPtr.Zero;
                }
                pLocalFilePathName = BVSDKAdapter.String2Utf8Intptr(value);
            }
        }

        public IntPtr ptLocalFilePathName
        {
            set
            {
                pLocalFilePathName = value;
            }
            get
            {
                return pLocalFilePathName;
            }
        }

        /// <summary>
        /// 文件开始传输偏移
        /// </summary>
        public ReTransferType reTransferType
        {
            get
            {
                return (iFileStartOffset == 0) ? ReTransferType.RETRANSMISSION : ReTransferType.RESUME;
            }
            set
            {
                iFileStartOffset = (value == ReTransferType.RETRANSMISSION) ? 0 : -1;
            }
        }

        public void Release()
        {
            if (pRemoteFilePathName != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pRemoteFilePathName);
                pRemoteFilePathName = IntPtr.Zero;
            }
            if (pLocalFilePathName != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pLocalFilePathName);
                pLocalFilePathName = IntPtr.Zero;
            }
        }
    }

    /// <summary>
    /// 重新传输类型
    /// </summary>
    public enum ReTransferType
    {
        RETRANSMISSION, //重新传输
        RESUME          //续传
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct BVCU_File_TransferInfo
    {
        public BVCU_File_TransferParam stParam;
        Int32 ihelper;
        Int64 iCreateTime;
        Int64 iOnlineTime;
        public UInt32 iTransferBytes;
        public UInt32 iTotalBytes;
        Int32 iSpeedKBpsLongTerm;
        Int32 iSpeedKBpsShortTerm;

        public float Percentage
        {
            get
            {
                if (iTotalBytes == 0)
                {
                    return 0;
                }
                return (float)(((int)(((iTransferBytes) / (iTotalBytes * 1.0)) * 10000)) / 10000.0);
            }
        }
    }


    #endregion

}
