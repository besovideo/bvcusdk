using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsTest
{

    /// <summary>
    /// 通道
    /// </summary>
    public class Channel
    {
        /// <summary>
        /// 通道号
        /// </summary>
        public int channelNo;

        /// <summary>
        /// 通道名称
        /// </summary>
        string m_channelName;

        /// <summary>
        /// 通道是否在线
        /// </summary>
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
                        return BVCU.TSP_CHANNEL + m_channelName;
                    }
                    return m_channelName;
                }
                else
                {
                    if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL &&
                           channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                    {
                        return BVCU.VIDEO_AUDIO_CHANNEL + channelNo;
                    }
                    else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS &&
                            channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                    {
                        return BVCU.GPS_CHANNEL + channelNo;
                    }
                    else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_TSP &&
                            channelNo <= BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_TSP)
                    {
                        return BVCU.TSP_CHANNEL + channelNo;
                    }
                    return BVCU.DEFAULT_CHANNEL_NAME_PREFIX + channelNo;
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

}
