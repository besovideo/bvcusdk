using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsTest
{
    public partial class BVCU
    {
        /// <summary>
        /// 显示字体信息相关
        /// </summary>
        #region 显示字体


        public const int BVDISPLAY_TEXT_EFFECT_OUTLINE = 1;
        public const int FONT_DISPLAY_MODE_SIZE = 32;

        public const int BVDISPLAY_INVALID_INDEX = -1;

        public struct Color
        {
            public Int32 format;    // Default value 0 stands for rgba color
            public UInt32 RGBA;
            public Color(UInt32 color)
            {
                format = 0;
                RGBA = color;
            }
        }

        public struct FontDisplayMode
        {
            public Int32 size;
            public Int32 rate;
            public Int32 effect;
            public Color fontColor;
            public Color backColor;
            public Int32 effectSize;
        }

        public delegate void DisplayFont(IntPtr dialog, Int64 timeStamp);

        #endregion 显示字体

    }
}
