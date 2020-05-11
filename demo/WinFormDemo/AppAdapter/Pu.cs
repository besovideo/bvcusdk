using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace WindowsFormsTest
{
    /// <summary>
    /// 设备
    /// </summary>
    public class Pu
    {
        private Session m_parent;
        private bool m_bOnlineStatus;

        /// <summary>
        /// 设备ID
        /// </summary>
        public string id;
        /// <summary>
        /// 设备名
        /// </summary>
        public string puName;
        /// <summary>
        /// 设备在线状态
        /// </summary>
        public bool OnlineStatus
        {
            get
            {
                return m_bOnlineStatus;
            }
            set
            {
                m_bOnlineStatus = value;
            }
        }
        /// <summary>
        /// 设备通道集合
        /// </summary>
        public ArrayList channelList;
        /// <summary>
        /// 设备云台集合
        /// </summary>
        public ArrayList puPtz;
        /// <summary>
        /// Pu 所属的 Session
        /// </summary>
        public Session Parent
        {
            get
            {
                return m_parent;
            }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public Pu( Session session)
        {
            m_parent = session;
            channelList = new ArrayList();
            puPtz = new ArrayList();
        }
    }

}
