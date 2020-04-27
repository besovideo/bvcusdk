/// <summary>
/// 功能：形式转换
///     1. C语言中true，false与C#中的bool之间的变换
///     2. C语言中的字符串与C#中的String间的转换
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsTest
{
    public partial class BVCU
    {
        /// <summary>
        /// 判断fail、success、true
        /// </summary>
        #region Failed_Successed_True

        /// <summary>
        /// 判断结果是否失败
        /// </summary>
        /// <param name="result">需要判断的值</param>
        /// <returns>result: 非0抛出异常</returns>
        public static void FAILED(int result)
        {
            if (result != 0)
            {
                throw new Exception(Marshal.GetExceptionForHR(result).Message);
            }
        }

        /// <summary>
        /// 判断结果是true、false. C、C++与C#的转换
        /// </summary>
        /// <param name="result">需要判断的值</param>
        /// <returns>result: 0为true， 其它为false</returns>
        public static bool SUCCEEDED(int result)
        {
            if (result == 0)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// 判断结果是true、false. C、C++与C#的转换
        /// </summary>
        /// <param name="result">需要判断的值</param>
        /// <returns>result:0为false， 非0为ture</returns>
        public static bool TRUE(int result)
        {
            if (result != 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        #endregion Failed_Successed_True




        /// <summary>
        /// 辅助功能模块, 包括SessionName， GetUtfByte
        /// </summary>
        #region 辅助功能模块

        /// <summary>
        /// 转换格式
        /// </summary>
        /// <param name="utf8">字节数组</param>
        /// <returns>字节数组对应的字符串</returns>
        public static string GetUtf8Byte(Byte[] utf8)
        {
            return Encoding.UTF8.GetString(utf8).Split('\0')[0];
        }
        #endregion 辅助功能模块

    }
}
