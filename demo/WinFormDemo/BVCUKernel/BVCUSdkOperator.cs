/*
 * 本类用于主窗口调用BVCU SDK函数操作
 * 包括创建Session，打开Dialog及二者对应的事件回调函数的处理
 */
using System;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsTest
{
    public class BVCUSdkOperator : IDisposable
    {
        #region 字段(m_bvcuSdkHandle, m_session, m_dialog, m_eventHandler, m_ptzSpeed, m_mainForm)

        /// <summary>
        /// BVCU SDK 全局Handle
        /// </summary>
        public IntPtr m_bvcuSdkHandle;

        /// <summary>
        /// 登录服务器生成一个Session
        /// </summary>
        public Session m_session;

        /// <summary>
        /// 打开预览生成一个Dialog
        /// </summary>
        public Dialog m_dialog;

        /// <summary>
        /// 事件处理类
        /// </summary>
        public EventHandler m_eventHandler;
      
        /// <summary>
        /// PTZ控制
        /// </summary>
        public int m_ptzSpeed;

        /// <summary>
        /// 主窗口类
        /// </summary>
        public MainWinForm m_mainForm;

        #endregion 字段


        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="mainform">主窗口</param>
        public BVCUSdkOperator(MainWinForm mainform)
        {
            m_mainForm = mainform;
            
            BVCU.FAILED(BVCU.ManagedLayer_CuInit(ref m_bvcuSdkHandle));//初始化库
            m_eventHandler = new EventHandler();
            m_session = new Session(m_bvcuSdkHandle, m_eventHandler, m_mainForm);
            m_dialog = new Dialog(m_bvcuSdkHandle, m_eventHandler, m_mainForm, m_session);
            m_eventHandler.init(m_bvcuSdkHandle, m_session, m_dialog);
            m_session.SetBVCUSdk(this);
        }


        #region 属性(Handle, Session, Dialog, PtzSpeed)

        /// <summary>
        /// BVCU SDK 全局Handle
        /// </summary>
        public IntPtr Handle
        {
            get
            {
                return m_bvcuSdkHandle;
            }
        }

        public Session Session
        {
            get
            {
                return m_session;
            }
        }

        public Dialog Dialog
        {
            get
            {
                return m_dialog;
            }
        }

        public int PtzSpeed
        {
            set
            {
                m_ptzSpeed = value;
            }
        }

        #endregion 属性


        #region 行为(PTZ, Dispose)

        public void ptzControl(int cmd, string puId, int channelNo, BVCU_PTZ_MOVE move, int presetNo = 0)
        {
            Channel channel = m_session.getChannel(puId, channelNo);
            if (channel == null)
            {
                return;
            }
            BVCU_PUCFG_PTZControl ptzCtrl = new BVCU_PUCFG_PTZControl();
            ptzCtrl.bStop = (int)move;
            ptzCtrl.iPTZCommand = cmd;
            ptzCtrl.iParam3 = ptzCtrl.iParam2 = ptzCtrl.iParam1 = 0;
            switch (cmd)
            {
                case BVCU.BVCU_PTZ_COMMAND.LEFT:
                case BVCU.BVCU_PTZ_COMMAND.RIGHT:
                case BVCU.BVCU_PTZ_COMMAND.UP:
                case BVCU.BVCU_PTZ_COMMAND.DOWN:
                    ptzCtrl.iParam2 = m_ptzSpeed;
                    break;
                case BVCU.BVCU_PTZ_COMMAND.LEFTDOWN:
                case BVCU.BVCU_PTZ_COMMAND.LEFTTOP:
                case BVCU.BVCU_PTZ_COMMAND.RIGHTDOWN:
                case BVCU.BVCU_PTZ_COMMAND.RIGHTTOP:
                    ptzCtrl.iParam1 = ptzCtrl.iParam2 = m_ptzSpeed;
                    break;
                case BVCU.BVCU_PTZ_COMMAND.PRESET_GO:
                    ptzCtrl.iParam1 = presetNo;
                    ptzCtrl.iParam2 = ptzCtrl.iParam3 = m_ptzSpeed;
                    break;
                case BVCU.BVCU_PTZ_COMMAND.CRUISE_GO:
                    ptzCtrl.iParam1 = presetNo;
                    break;
                default:
                    break;
            }
            IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(BVCU_PUCFG_PTZControl)));
            Marshal.StructureToPtr(ptzCtrl, pt, true);
            BVCU.ManagedLayer_CuSetPuPtzControl(m_bvcuSdkHandle, m_session.Handle, Encoding.UTF8.GetBytes(puId), channel.ptzIdx, pt);
        }

        public void getPtzAttr(string puId, int channelNo)
        { 
            Channel channel = m_session.getChannel(puId, channelNo);
            if (channel == null)
            {
                return;
            }
            BVCU.ManagedLayer_CuGetPuPtzAttr(m_bvcuSdkHandle, m_session.Handle, Encoding.UTF8.GetBytes(puId),
                channel.ptzIdx, m_eventHandler.onGetPtzAttr);
        }


        public void GetGpsData(string puId,int channelNo, Test_Struct s)
        {
            //int i = BVCU.ManagedLayer_CuGetPuGpsInfo(m_bvcuSdkHandle, m_session.Handle, Encoding.UTF8.GetBytes(puId),
            //   channelNo - BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS);

            IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Test_Struct)));
            Marshal.StructureToPtr(s, pt, false);
            int j = BVCU.ManagedLayer_CuGetPuGpsInfoV2(m_bvcuSdkHandle, m_session.Handle, Encoding.UTF8.GetBytes(puId),
               channelNo - BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS,pt);
        }

        /// <summary>
        /// 停止使用库
        /// </summary>
        public void Dispose()
        {
            BVCU.ManagedLayer_CuRelease(m_bvcuSdkHandle);
        }

        #endregion 行为
    }
}
