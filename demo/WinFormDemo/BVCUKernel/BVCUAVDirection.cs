using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsTest
{
    public partial class BVCU
    {

        /// <summary>
        /// GetAVDirection
        /// </summary>
        #region GetAVDirection

        const Int32 BVCU_MEDIADIR_VIDEOSEND = 1;
        const Int32 BVCU_MEDIADIR_VIDEORECV = 2;
        const Int32 BVCU_MEDIADIR_AUDIOSEND = 4;
        const Int32 BVCU_MEDIADIR_AUDIORECV = 8;

        public struct AVDirection
        {
            public bool audioSnd;
            public bool audioRecv;
            public bool videoSnd;
            public bool videoRecv;
        }

        /// <summary>
        /// 获得AV Direction
        /// </summary>
        /// <param name="dir"></param>
        /// <returns></returns>
        public static AVDirection GetAVDirection(int dir)
        {
            AVDirection result = new AVDirection();

            result.audioRecv = ((dir & BVCU_MEDIADIR_AUDIORECV) != 0);
            result.audioSnd = ((dir & BVCU_MEDIADIR_AUDIOSEND) != 0);
            result.videoRecv = ((dir & BVCU_MEDIADIR_VIDEORECV) != 0);
            result.videoSnd = ((dir & BVCU_MEDIADIR_VIDEOSEND) != 0);

            return result;
        }

        #endregion GetAVDirection


    }
}
