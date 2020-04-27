using System;
using System.Windows.Forms;
using System.Threading;
using System.Collections;
using System.Text;

namespace WindowsFormsTest
{
    /// <summary>
    /// session/一个登录/连接
    /// </summary>
    public class Session
    {
        /// <summary>
        /// 一个BVCU实例，类库句柄，初始化库时初始化m_bvsdkHandle
        /// </summary>
        IntPtr m_bvsdkHandle;

        /// <summary>
        /// 事件处理类
        /// </summary>
        EventHandler m_bvsdkEventHandler;

        /// <summary>
        /// 主窗体
        /// </summary>
        MainWinForm m_mainForm;
        
        Server m_server;
        const int SERVER_TIME_OUT_SECOND = 60;
        const int SERVER_RETRY_TIMES = 10;      // 重连次数为10
        const int SERVER_RETRY_INTERVAL = 10;   // 每隔10秒执行一次断线重连

        bool m_bServerRetry = false;        // 是否断线重连
        int m_ServerInterval = 0;           // 定时器间隔
        int m_iLoginRetryCount = 0;         // demo断线重连计数
        int m_ServerRetryTimes = 0;         // demo断线重连次数

        delegate void OnLoginRetry();
        private System.Threading.Timer m_loginRetryTimer = null;
        OnLoginRetry m_delegateLoginRetry = null;


        BVCUSdkOperator m_globalBVCUSdk = null;

        /// <summary>
        /// L一个连接
        /// </summary>
        /// <param name="sdkHandle">
        ///     SDKHandle: 初始化库，传递IntPtr类型m_bvcuSdkHandle，BVCU.ManagedLayer_CuInit(ref  m_bvcuSdkHandle)
        /// </param>
        /// <param name="eventHandler">事件响应类</param>
        /// <param name="mainForm">主窗体</param>
        public Session(IntPtr sdkHandle, EventHandler eventHandler, MainWinForm mainForm)
        {
            m_bvsdkHandle = sdkHandle;
            m_bvsdkEventHandler = eventHandler;
            m_mainForm = mainForm;
            m_server = new Server();
            m_ServerInterval = SERVER_RETRY_INTERVAL;
            m_bServerRetry = false;
            m_iLoginRetryCount = 0;
            m_ServerRetryTimes = SERVER_RETRY_TIMES;
        }

        public void SetBVCUSdk(BVCUSdkOperator globalBVCUSdk)
        {
            m_globalBVCUSdk = globalBVCUSdk;
        }


        /// <summary>
        /// 获得server的sessionHandle，只读属性
        /// </summary>
        public IntPtr Handle
        {
            get
            {
                return m_server.sessionHandle;
            }
        }

        /// <summary>
        /// 获得指定设备puId对应的设备
        /// </summary>
        /// <param name="puId"></param>
        /// <returns></returns>
        public Pu getPu(string puId)
        {
            foreach (Pu pu in m_server.puList)
            {
                if (pu.id.Equals(puId))
                {
                    return pu;
                }
            }
            return null;
        }

        public Channel getChannel(string puId, int channelNo)
        {
            Pu pu = getPu(puId);
            if (pu != null)
            {
                foreach (Channel channel in pu.channelList)
                {
                    if (channel.channelNo == channelNo)
                    {
                        return channel;
                    }
                }
            }
            return null;
        }

        public ArrayList PuList
        {
            get
            {
                return m_server.puList;
            }
        }

        public string Name
        {
            get
            {
                return m_server.ServerName;
            }
        }

        public void login(string ip, int port, string usrName, string psw)
        {
            m_server.ip = ip;
            m_server.port = port;
            m_server.usrName = usrName;
            m_server.psw = psw;
            m_server.ukeyid = "";
            m_server.ukeycode = "";
            try
            {
                //封装
                int ret = BVCU.ManagedLayer_CuLogin(m_bvsdkHandle, 
                    ref m_server.sessionHandle, Encoding.UTF8.GetBytes(ip), 
                    port, Encoding.UTF8.GetBytes(usrName),
                    Encoding.UTF8.GetBytes(psw), SERVER_TIME_OUT_SECOND,
                    m_bvsdkEventHandler.server_OnEvent, m_bvsdkEventHandler.server_ProcChannelInfo);

                BVCU.FAILED(ret);
            }
            catch
            {
                MessageBox.Show("登录失败");
            }
        }

        public void logout()
        {
            BVCU.ManagedLayer_CuLogout(m_bvsdkHandle, m_server.sessionHandle);
        }


        /// <summary>
        /// On Session event
        /// </summary>
        delegate void DelegateOnLoginOk();

        DelegateOnLoginOk delegateOnLoginOk;

        /// <summary>
        /// 登录成功后调用；获取设备列表
        /// </summary>
        void procOnLoginOk()
        {
            Console.WriteLine("登录服务器 {0} 成功", m_server.ip);
            releaseReloginTimer();
            m_iLoginRetryCount = 0;
            m_bServerRetry = true;
            int result = 0;
            for (int i = 0; i < BVCU.FAIL_RETRY_TIMES; i++)//试BVCU.FAIL_RETRY_TIMES(=10)次-L
            {
                result = BVCU.ManagedLayer_CuGetPuList(m_bvsdkHandle, m_server.sessionHandle,
                    m_bvsdkEventHandler.cmd_OnGetPuList);

                if (!BVCU.SUCCEEDED(result))
                {
                    Thread.Sleep(BVCU.FAIL_RETRY_INTERVAL);
                }
                else
                {
                    Console.WriteLine("获取设备列表成功");
                    return;
                }
            }

            MessageBox.Show("获取设备列表失败");
        }

        public void OnLoginOk()
        {
            if (delegateOnLoginOk == null)
            {
                delegateOnLoginOk = new DelegateOnLoginOk(procOnLoginOk);
            }

            // 回调函数内部不能直接发送命令，否则会引起回调死锁
            m_mainForm.BeginInvoke(delegateOnLoginOk);
        }

        public void OnLogoutOK()
        {
            MessageBox.Show("成功退出服务器");
        }

        public void OnGetPu(string puName, string puId, Channel channel)
        {
            foreach (Pu pu in m_server.puList)
            {
                if (pu.id.Equals(puId))
                {
                    foreach (Channel chnl in pu.channelList)
                    {
                        if (chnl.channelNo == channel.channelNo)
                        {
                            chnl.online = channel.online;
                            return;
                        }
                    }
                    pu.channelList.Add(channel);
                    return;
                }
            }
            Pu newPu = new Pu();
            newPu.id = puId;
            newPu.puName = puName;
            newPu.channelList.Add(channel);
            m_server.puList.Add(newPu);
        }

        public void OnGetPuListFinished()
        {
            m_mainForm.getPuList();
        }

        public void OnRemovePu(string puId)
        {
            ArrayList toDel = new ArrayList();
            foreach (Pu pu in m_server.puList)
            {
                if (pu.id.Equals(puId))
                {
                    toDel.Add(pu);
                    pu.channelList.Clear();
                }
            }
            foreach (Pu pu in toDel)
            {
                m_server.puList.Remove(pu);
            }
            toDel.Clear();
        }

        delegate void ClearGpsDataList(string puid, int channelNo);
        ClearGpsDataList delegateClearGpsDataList;

        public void clearGpsDataList(string puid, int channelNo, bool bClear)
        {
            if (!bClear) return;
            if (m_mainForm != null && m_mainForm.IsHandleCreated)
            {
                if (delegateClearGpsDataList == null)
                {
                    delegateClearGpsDataList = new ClearGpsDataList(procClearGpsDataList);
                }
                m_mainForm.BeginInvoke(delegateClearGpsDataList, new object[]{puid, channelNo});
            }
        }

        void procClearGpsDataList(string puid, int channelNo)
        {
            m_mainForm.ClearlistViewGPSData(puid, channelNo);
        }

        public void OnServerDisConnect()
        {
            if (!m_mainForm.IsHandleCreated)
                return;
            ClearDialogs();
            clearResource();
            if (m_bServerRetry && m_ServerRetryTimes >= m_iLoginRetryCount)
            {
                loginRetry();
            }
        }

        public void ClearDialogs()
        {
            for (int i = 0; i < m_globalBVCUSdk.m_dialog.m_dialogs.Count; ++i)
            {
                OneDialog oneDialog = (OneDialog)m_globalBVCUSdk.m_dialog.m_dialogs[i];
                if (oneDialog.dialogHandle != null)
                {
                    m_globalBVCUSdk.m_dialog.closeDialog(oneDialog);
                }
            }
            for (int i = 0; i < m_globalBVCUSdk.m_dialog.m_gpsDialogs.Count; ++i)
            {
                OneDialog oneDialog = (OneDialog)m_globalBVCUSdk.m_dialog.m_gpsDialogs[i];
                if (oneDialog.dialogHandle != null)
                {
                    m_globalBVCUSdk.m_dialog.closeGpsDialog(oneDialog);
                }
            }
            for (int i = 0; i < m_globalBVCUSdk.m_dialog.m_tspDialogs.Count; ++i)
            {
                OneDialog oneDialog = (OneDialog)m_globalBVCUSdk.m_dialog.m_tspDialogs[i];
                if (oneDialog.dialogHandle != null)
                {
                    m_globalBVCUSdk.m_dialog.closeTspDialog(oneDialog);
                }
            }
        }

        delegate void ClearResource();
        ClearResource delegateClearRecource;

        public void clearResource()
        {
            if (m_mainForm != null && m_mainForm.IsHandleCreated)
            {
                if (delegateClearRecource == null)
                {
                    delegateClearRecource = new ClearResource(procoClearResource);
                }
                m_mainForm.BeginInvoke(delegateClearRecource);
            }
        }

        public void procoClearResource()
        {
            m_mainForm.clearTreeNodes();
        }

        public void OnLoginFailed()
        {
            Console.WriteLine("登录服务器 {0} 失败", m_server.ip);
        }

        public void loginRetry()
        {
            if (m_loginRetryTimer == null)
                m_loginRetryTimer = new System.Threading.Timer(new TimerCallback(reloginTimer));
            m_loginRetryTimer.Change(m_ServerInterval * 1000, m_ServerInterval * 1000);
        }

        private void reloginTimer(Object state)
        {
            if (m_mainForm != null && m_mainForm.IsHandleCreated)
            {
                if (m_delegateLoginRetry == null)
                {
                    m_delegateLoginRetry = new OnLoginRetry(procRelogin);
                }
                m_mainForm.BeginInvoke(m_delegateLoginRetry);
            }
        }

        private void procRelogin()
        {
            try
            {
                //封装
                int ret = BVCU.ManagedLayer_CuLogin(m_bvsdkHandle,
                    ref m_server.sessionHandle, Encoding.UTF8.GetBytes(m_server.ip),
                    m_server.port, Encoding.UTF8.GetBytes(m_server.usrName),
                    Encoding.UTF8.GetBytes(m_server.psw), SERVER_TIME_OUT_SECOND,
                    m_bvsdkEventHandler.server_OnEvent, m_bvsdkEventHandler.server_ProcChannelInfo);
                m_iLoginRetryCount++;
                Console.WriteLine("服务器断线重连执行第{0}次", m_iLoginRetryCount);
                if (m_iLoginRetryCount >= m_ServerRetryTimes)
                {
                    Console.WriteLine("注销断线重连timer");
                    releaseReloginTimer();
                    return;
                }
            }
            catch
            {
                MessageBox.Show("登录失败");
            }
        }

        private void releaseReloginTimer()
        {
            if (m_loginRetryTimer != null)
            {
                m_loginRetryTimer.Dispose();
                m_loginRetryTimer = null;
            }
        }
    }
}
