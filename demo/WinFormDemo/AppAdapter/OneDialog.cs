using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsTest
{
    /// <summary>
    /// 用于本地存储一个Dialog对话
    /// </summary>
    public class OneDialog
    {
        public Panel panel;         // 用于显示预览的窗口
        public IntPtr dialogHandle; // BVCU SDK返回的对话句柄
        public Pu pu;               // 对话所属的Pu设备
        public int channelNo;       // 对话所在的通道
        public DateTime timeStamp;  // 对话当前显示帧的时间戳
        public bool recording;      // 是否正在录像
    
        public OneDialog()
        {
            dialogHandle = IntPtr.Zero;
        }
    }
}
