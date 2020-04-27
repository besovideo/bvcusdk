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
        public bool online;
        /// <summary>
        /// 设备通道集合
        /// </summary>
        public ArrayList channelList;
        /// <summary>
        /// 设备云台集合
        /// </summary>
        public ArrayList puPtz;

        /// <summary>
        /// 构造函数
        /// </summary>
        public Pu()
        {
            channelList = new ArrayList();
            puPtz = new ArrayList();
        }

        /// <summary>
        /// 获得指定通道channelName的通道编号
        /// </summary>
        /// <param name="channelName">通道名：{如："音视频通道"，"GPS通道"，"TSP通道"}</param>
        /// <returns>通道编号</returns>
        public int getChannelNo(string channelName)
        {
            if (channelName.Contains(BVCU.TSP_CHANNEL))
            {
                return int.Parse(channelName.Substring(BVCU.TSP_CHANNEL.Length));
            }
            else if (channelName.Contains(BVCU.GPS_CHANNEL))
            {
                return int.Parse(channelName.Substring(BVCU.GPS_CHANNEL.Length));
            }
            else if (channelName.Contains(BVCU.VIDEO_AUDIO_CHANNEL))
            {
                return int.Parse(channelName.Substring(BVCU.VIDEO_AUDIO_CHANNEL.Length));
            }
            else if (channelName.Contains(BVCU.DEFAULT_CHANNEL_NAME_PREFIX))
            {
                return int.Parse(channelName.Substring(BVCU.DEFAULT_CHANNEL_NAME_PREFIX.Length));
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

}
