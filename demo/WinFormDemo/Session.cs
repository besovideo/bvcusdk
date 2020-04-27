using System;
using System.Windows.Forms;
using System.Threading;
using System.Collections;
using System.Text;

namespace WindowsFormsTest
{
    class Session
    {
        IntPtr m_bvsdkHandle;
        EventHandler m_bvsdkEventHandler;
        Form1 m_mainForm;
        public Session(IntPtr sdkHandle, EventHandler eventHandler, Form1 mainForm)
        {
            m_bvsdkHandle = sdkHandle;
            m_bvsdkEventHandler = eventHandler;
            m_mainForm = mainForm;
            m_server = new Server();
        }

        class Server
        {
            public string ip;
            public int port;
            public string usrName;
            public string psw;
            public IntPtr sessionHandle;
            public string ServerName
            {
                get
                {
                    return ip + ":" + port;
                }
            }
            public ArrayList puList;
            public Server()
            {
                sessionHandle = IntPtr.Zero;
                puList = new ArrayList();
            }
        }

        public IntPtr Handle
        {
            get
            {
                return m_server.sessionHandle;
            }
        }

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

        Server m_server;
        const int SERVER_TIME_OUT_SECOND = 60;
        public void login(string ip, int port, string usrName, string psw)
        {
            m_server.ip = ip;
            m_server.port = port;
            m_server.usrName = usrName;
            m_server.psw = psw;
            try
            {
                BVCU.FAILED(
                    BVCU.ManagedLayer_CuLogin(m_bvsdkHandle, ref m_server.sessionHandle,
                    Encoding.UTF8.GetBytes(ip), port, Encoding.UTF8.GetBytes(usrName), Encoding.UTF8.GetBytes(psw), SERVER_TIME_OUT_SECOND,
                    m_bvsdkEventHandler.server_OnEvent, m_bvsdkEventHandler.server_ProcChannelInfo));
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

        public string Name
        {
            get
            {
                return m_server.ServerName;
            }
        }

        public class Pu
        {
            public string id;
            public string puName;
            public bool online;
            public ArrayList channelList;
            public ArrayList puPtz;
            public Pu()
            {
                channelList = new ArrayList();
                puPtz = new ArrayList();
            }
            public int getChannelNo(string channelName)
            {
                if (channelName.Contains(TSP_CHANNEL))
                {
                    return int.Parse(channelName.Substring(TSP_CHANNEL.Length));
                }
                else if (channelName.Contains(GPS_CHANNEL))
                {
                    return int.Parse(channelName.Substring(GPS_CHANNEL.Length));
                }
                else if (channelName.Contains(VIDEO_AUDIO_CHANNEL))
                {
                    return int.Parse(channelName.Substring(VIDEO_AUDIO_CHANNEL.Length));
                }
                else if (channelName.Contains(DEFAULT_CHANNEL_NAME_PREFIX))
                {
                    return int.Parse(channelName.Substring(DEFAULT_CHANNEL_NAME_PREFIX.Length));
                }
                foreach (Channel channel in channelList)
                {
                    if (channel.channelName.Equals(channelName))
                    {
                        return channel.channelNo;
                    }
                }
                return Channel.INVALID_CHANNEL_NO;
            }
        }

        const string VIDEO_AUDIO_CHANNEL = "音视频通道";
        const string GPS_CHANNEL = "GPS通道";
        const string TSP_CHANNEL = "TSP通道";
        const string DEFAULT_CHANNEL_NAME_PREFIX = "通道";
        public class Channel
        {
            public int channelNo;
            string m_channelName;
            public bool online;
            public string channelName
            {
                get
                {
                    if (m_channelName.Length != 0)
                    {

                        if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL &&
                               channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                        {
                            return m_channelName;
                        }
                        else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS &&
                                channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                        {
                            return m_channelName;
                        }
                        else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_TSP &&
                                channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                        {
                            return TSP_CHANNEL + m_channelName;
                        }
                        return m_channelName;
                    }
                    else
                    {
                        if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL &&
                               channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                        {
                            return VIDEO_AUDIO_CHANNEL + channelNo;
                        }
                        else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS &&
                                channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                        {
                            return GPS_CHANNEL + channelNo;
                        }
                        else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_TSP &&
                                channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                        {
                            return TSP_CHANNEL + channelNo;
                        }
                        return DEFAULT_CHANNEL_NAME_PREFIX + channelNo;
                    }
                }
                set
                {
                    m_channelName = value;
                }
            }
            public bool audioPlayback;
            public bool speak;
            public bool video;
            public bool beginTrans;
            public int ptzIdx;
            public const int INVALID_CHANNEL_NO = -1;
        }

        /*
         * On Session event
         */
        delegate void DelegateOnLoginOk();
        DelegateOnLoginOk delegateOnLoginOk;
        void procOnLoginOk()
        {
            int result = 0;
            for (int i = 0; i < BVCU.FAIL_RETRY_TIMES; i++)
            {
                result = BVCU.ManagedLayer_CuGetPuList(m_bvsdkHandle, m_server.sessionHandle,
                    m_bvsdkEventHandler.cmd_OnGetPuList);
                if (!BVCU.SUCCEEDED(result))
                {
                    Thread.Sleep(BVCU.FAIL_RETRY_INTERVAL);
                }
                else
                {
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

        public void OnServerDisConnect()
        {
            try
            {
                m_bvsdkEventHandler.showMessageBoxOnEvent(m_mainForm, "服务器下线");
            }
            catch
            {
                // 程序销毁时不再弹出对话框
            }
        }
    }
}
