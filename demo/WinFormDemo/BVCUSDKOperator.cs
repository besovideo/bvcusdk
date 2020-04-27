/*
 * 本类用于主窗口调用BVCU SDK函数操作
 * 包括创建Session，打开Dialog及二者对应的事件回调函数的处理
 */
using System;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsTest
{
    class BVCUSDKOperator : IDisposable
    {
        /*
         * 事件处理类
         */
        EventHandler m_eventHandler;
        /*
         * 主窗口类
         */
        Form1 m_mainForm;

        public BVCUSDKOperator(Form1 mainform)
        {
            m_mainForm = mainform;
            BVCU.FAILED(BVCU.ManagedLayer_CuInit(ref m_bvcuSdkHandle));
            m_eventHandler = new EventHandler();
            m_session = new Session(m_bvcuSdkHandle, m_eventHandler, m_mainForm);
            m_dialog = new Dialog(m_bvcuSdkHandle, m_eventHandler, m_mainForm, m_session);
            m_eventHandler.init(m_bvcuSdkHandle, m_session, m_dialog);
        }

        /*
         * BVCU SDK 全局Handle
         */
        IntPtr m_bvcuSdkHandle;
        public IntPtr Handle
        {
            get
            {
                return m_bvcuSdkHandle;
            }
        }

        /*
         * 释放资源
         */
        public void Dispose()
        {
            BVCU.ManagedLayer_CuRelease(m_bvcuSdkHandle);
        }

        /*
         * 登录服务器生成一个Session
         */
        Session m_session;
        public Session Session
        {
            get
            {
                return m_session;
            }
        }

        /*
         * 打开预览生成一个Dialog
         */
        Dialog m_dialog;
        public Dialog Dialog
        {
            get
            {
                return m_dialog;
            }
        }

        /*
         * PTZ控制
         */
        int m_ptzSpeed;
        public int PtzSpeed
        {
            set
            {
                m_ptzSpeed = value;
            }
        }

        public void ptzControl(int cmd, string puId, int channelNo, BVCU_PTZ_MOVE move, int presetNo = 0)
        {
            Session.Channel channel = m_session.getChannel(puId, channelNo);
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
                case BVCU_PTZ_COMMAND.LEFT:
                case BVCU_PTZ_COMMAND.RIGHT:
                case BVCU_PTZ_COMMAND.UP:
                case BVCU_PTZ_COMMAND.DOWN:
                    ptzCtrl.iParam2 = m_ptzSpeed;
                    break;
                case BVCU_PTZ_COMMAND.LEFTDOWN:
                case BVCU_PTZ_COMMAND.LEFTTOP:
                case BVCU_PTZ_COMMAND.RIGHTDOWN:
                case BVCU_PTZ_COMMAND.RIGHTTOP:
                    ptzCtrl.iParam1 = ptzCtrl.iParam2 = m_ptzSpeed;
                    break;
                case BVCU_PTZ_COMMAND.PRESET_GO:
                    ptzCtrl.iParam1 = presetNo;
                    ptzCtrl.iParam2 = ptzCtrl.iParam3 = m_ptzSpeed;
                    break;
                case BVCU_PTZ_COMMAND.CRUISE_GO:
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
            Session.Channel channel = m_session.getChannel(puId, channelNo);
            if (channel == null)
            {
                return;
            }
            BVCU.ManagedLayer_CuGetPuPtzAttr(m_bvcuSdkHandle, m_session.Handle, Encoding.UTF8.GetBytes(puId),
                channel.ptzIdx, m_eventHandler.onGetPtzAttr);
        }
    }
}
