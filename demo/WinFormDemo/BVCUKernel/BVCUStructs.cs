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

    public struct BVCU_Event_SessionCmd
    {
        public Int32 iResult;
        public Int32 iPercent;
        public BVCU_CmdMsgContent stContent;
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
    }
}
