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

    /// <summary>
    /// Pu channel information
    /// </summary>
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
}
