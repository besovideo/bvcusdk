/*
 * 本类为静态类，用于映射C++到托管C#
 * 结构体注释参考BVCU.h
 */

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace WindowsFormsTest
{

    /// <summary>
    /// 主要功能是对ManagedLayer.dll, bvdisplay.dll中提供功能的提取
    /// </summary>
    public partial class BVCU
    {
        /// <summary>
        /// 动态库ManagedLayer.dll，包括ManagedLayer.h中提供给C#的所有接口【注：现未添加完全(2013-12-12 08:58)】
        /// </summary>
        #region ManagedLayer动态库

        /// <summary>
        /// 普通功能
        /// </summary>
        #region 功能模块
#if DEBUG
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_DebugConsoleInit();
#endif

        /// <summary>
        /// 初始化库
        /// </summary>
        /// <param name="handle">IntPtr类型</param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuInit(ref IntPtr handle);

        /// <summary>
        /// 停止使用库
        ///     使用示例public void Dispose() { BVCU.ManagedLayer_CuRelease(m_bvcuSdkHandle); }
        /// </summary>
        /// <param name="handle"></param>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_CuRelease(IntPtr handle);

        /// <summary>
        /// Cu 登录
        ///     使用示例：
        ///        int ret = BVCU.ManagedLayer_CuLogin(m_bvsdkHandle, 
        ///            ref m_server.sessionHandle, Encoding.UTF8.GetBytes(ip), 
        ///            port, Encoding.UTF8.GetBytes(usrName),
        ///            Encoding.UTF8.GetBytes(psw), SERVER_TIME_OUT_SECOND,
        ///            m_bvsdkEventHandler.server_OnEvent, m_bvsdkEventHandler.server_ProcChannelInfo);
        ///
        ///        BVCU.FAILED(ret);
        /// 两个事件回调处理函数EventHandler.Server_ProcNotifyChannelInfo(...)、
        ///                 EventHandler.Server_OnEvent(...)
        /// </summary>
        /// <param name="handle">IntPtr</param>
        /// <param name="session">out 登录/连接</param>
        /// <param name="serverIp">服务器IP</param>
        /// <param name="serverPort">服务器端口号</param>
        /// <param name="usrName">用户名</param>
        /// <param name="usrPsw">用户密码</param>
        /// <param name="timeOutSec">超时时间，单位秒</param>
        /// <param name="onEvent">见EventHandler.Server_OnEvent(...)，响应BVCU_EVENT_SESSION_OPEN、BVCU_EVENT_SESSION_CLOSE</param>
        /// <param name="procChannelInfo">见EventHandler.Server_ProcNotifyChannelInfo(...)</param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuLogin(IntPtr handle, ref IntPtr session, Byte[] serverIp, Int32 serverPort,
            Byte[] usrName, Byte[] usrPsw, int timeOutSec,
            EventHandler.BVCU_Server_OnEvent onEvent,
            EventHandler.BVCU_Server_Notify onNotify);

        /// <summary>
        /// Cu 退出登录
        ///     示例：
        ///         BVCU.ManagedLayer_CuLogout(m_bvsdkHandle, m_server.sessionHandle);
        /// </summary>
        /// <param name="handle">IntPtr</param>
        /// <param name="session">IntPtr</param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuLogout(IntPtr handle, IntPtr session);

        /// <summary>
        /// 获得前端设备(Pu)列表
        /// </summary>
        /// <param name="handle">CBVCU的实例</param>
        /// <param name="session">session</param>
        /// <param name="getPuList"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuList(IntPtr handle, IntPtr session,
            EventHandler.BVCU_Cmd_OnGetPuList getPuList);

        /// <summary>
        /// 功能：视频预览
        /// 在Dialog.openBrowse(...)中调用
        ///     BVCU.ManagedLayer_CuBrowsePu(m_bvsdkHandle, ref dlg.dialogHandle, 
        ///                        m_session.Handle, Encoding.UTF8.GetBytes(pu.id), channelNo,
        ///                        panel.Handle, ref dispRect,
        ///                        volume, 0, Encoding.UTF8.GetBytes(""), true, ref net,
        ///                        m_bvsdkEventHandler.dialog_OnDialogEvent,
        ///                        m_bvsdkEventHandler.dialog_OnStorageEvent));       
        /// </summary>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuBrowsePu(IntPtr handle, ref IntPtr dialog,
            IntPtr session, Byte[] puId, int channelNo, IntPtr hWnd, ref BVRect dispRect,
            int volume, int singleRecFileSec, Byte[] recFileDir, bool videoTrans,
            ref BVCU_DialogControlParam_Network netWork,
            EventHandler.BVCU_Dialog_OnDialogEvent onDlgEvent,
            EventHandler.BVCU_Dialog_OnStorageEvent onStoreEvent);

        /// <summary>
        /// 语音对讲
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuNewTalk(IntPtr handle, ref IntPtr dialog,
            IntPtr session, Byte[] puId, int channelNo, int captureVolume, int audioVolume,
            ref BVCU_DialogControlParam_Network netWork,
            EventHandler.BVCU_Dialog_OnDialogEvent onDlgEvent,
            EventHandler.BVCU_Dialog_OnStorageEvent onStoreEvent);


        /// <summary>
        /// 关闭Dialog
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuCloseDialog(IntPtr handle, IntPtr dialog);

        /// <summary>
        /// 设置播放音量
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <param name="volume">播放音量大小</param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuPlayVolume(IntPtr handle, IntPtr dialog, int volume);

        /// <summary>
        /// 设置记录（录像...）存储路径
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <param name="fileDir"></param>
        /// <param name="fileSec"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSetRecordStorageParam(IntPtr handle, IntPtr dialog, byte[] fileDir, int fileSec);

        /// <summary>
        /// 本地抓取会话中接收到的一帧视频，存为图像文件。目前仅支持JPG格式
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <param name="fileDir"></param>
        /// <param name="fileSec"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSnapshot(IntPtr handle, IntPtr dialog, byte[] fileDir, int fileSec);

        /// <summary>
        /// 重新设置Dialog
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <param name="dispRect"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuResizeDialogWindow(IntPtr handle, IntPtr dialog, ref BVRect dispRect);


        /// <summary>
        /// 设置Pu控制的响应事件
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="onCtrlRes">功能暂时未完成EventHandler.OnControlResult</param>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_CuSetPuControlResultProcFunc(IntPtr handle, EventHandler.BVCU_Cmd_ControlResult onCtrlRes);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_CuSetPuQueryResultProcFunc(IntPtr handle, EventHandler.BVCU_Cmd_QueryResult onQueryRes);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuSetPuPtzControl(IntPtr handle, IntPtr session, Byte[] puId, int device, IntPtr ptzCtrl);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuGpsInfo(IntPtr handle, IntPtr session, Byte[] puId, int device);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuGpsInfoV2(IntPtr handle, IntPtr session, Byte[] puId, int device,IntPtr userData);

        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuOpenTspDialog(IntPtr handle, ref IntPtr dialog, IntPtr session, Byte[] puId, int channelNo, EventHandler.BVCU_TspDialog_OnEvent onDlgEvent, EventHandler.BVCU_TspDialog_OnData onDlgData);

        /// <summary>
        /// 串口发送数据
        /// </summary>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl, CharSet=CharSet.Auto)]
        public static extern int SendTspData(IntPtr hDialog, Byte[] sendData, int sendDataLen);

        /// <summary>
        /// 获取云台的属性
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="session"></param>
        /// <param name="puId"></param>
        /// <param name="device"></param>
        /// <param name="onEvent"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuGetPuPtzAttr(IntPtr handle, IntPtr session, Byte[] puId, int device, EventHandler.BVCU_Cmd_OnGetPuPtzAttr onEvent);

        /// <summary>
        /// Open Gps Dialog, 在Dialog中调用
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="dialog"></param>
        /// <param name="session"></param>
        /// <param name="puId"></param>
        /// <param name="channelNo"></param>
        /// <param name="onDlgEvent"></param>
        /// <param name="onDlgData"></param>
        /// <returns></returns>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_CuOpenGpsDialog(IntPtr handle, ref IntPtr dialog, IntPtr session,
            Byte[] puId, int channelNo,
            EventHandler.BVCU_GpsDialog_OnEvent onDlgEvent,
            EventHandler.BVCU_GpsDialog_OnData onDlgData);

        // 手动设置PU停止/开始录像
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ManagedLayer_PuManualRemoteRecord(IntPtr handle, IntPtr hSession, Byte[] puId, int deviceIdx, ref BVCU_PUCFG_ManualRecord recordParam);

        #endregion 功能模块


        /// <summary>
        /// 显示相关，主要使用bvdisplay.dll库【也使用了Managedlayer.dll库】，更多详情请参考bvdisplay.h, bvcu.h
        /// </summary>
        #region 显示

        /// <summary>
        /// Display after render:解码后显示
        /// </summary>
        /// <param name="dispFontFunc"></param>
        [DllImport("ManagedLayer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ManagedLayer_DispSetDisplayFontFunc(DisplayFont dispFontFunc);

        /// <summary>
        /// 创建字体用于用户定义信息的显示.
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="fontIdx"></param>
        /// <param name="fontSize"></param>
        /// <param name="fontName"></param>
        /// <returns></returns>
        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_CreateFont(IntPtr hwnd, ref int fontIdx, int fontSize, [MarshalAs(UnmanagedType.LPWStr)]String fontName);

        /// <summary>
        /// 在指定区域内显示字体, 字体在视频上的位置由dispRect的top, left, bottom, right参数确定,  颜色由color确定.
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="fontIdx"></param>
        /// <param name="dispRect"></param>
        /// <param name="color"></param>
        /// <param name="text"></param>
        /// <returns></returns>
        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_DisplayFont(IntPtr hwnd, int fontIdx, ref BVRect dispRect, ref Color color, [MarshalAs(UnmanagedType.LPWStr)]String text);

        /// <summary>
        /// 设置字体显示效果, 设置当显示文字内容过多, 给定区域无法完全显示时的, 字体的显示方式, 包括多行显示和滚动显示等.
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="fontIdx"></param>
        /// <param name="effect"></param>
        /// <returns></returns>
        [DllImport("bvdisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BVDisplay_SetFontDisplayMode(IntPtr hwnd, int fontIdx, ref FontDisplayMode effect);

        #endregion 显示

        #endregion ManagedLayer动态库
    }
}
