using System;
using System.Text;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;

namespace LogHelper
{
    public class LogHelper
    {
        /// <summary>
        /// 日志
        /// </summary>
        /// <param name="strErrorNum">信息级别</param>
        /// <param name="description">信息内容</param>
        /// <returns>void</returns>
        public static void RecordLog(int level, string description)
        {
            string logFilePath; //日志文件的路径
            DateTime dt = DateTime.Now;
            try
            {
                //创建日志的目录
                {
                    logFilePath = Directory.GetCurrentDirectory() + "\\RunLogDirectory";
                    if (Directory.Exists(logFilePath) == false)
                    {
                        Directory.CreateDirectory(logFilePath); //建立目录
                    }

                    logFilePath = logFilePath + "\\" + dt.ToString("yyyy-MM");
                    if (Directory.Exists(logFilePath) == false)
                    {
                        Directory.CreateDirectory(logFilePath); //建立目录
                    }
                }

                logFilePath = logFilePath + "\\" + dt.ToString("dd") + ".txt";//日志文件名

                //日志信息
                string logInfo = string.Format("[Time:{0}] [Level:{1}] [Information:{2}.]", dt.ToString("yyyy-MM-dd HH:mm:ss"), level, description);

                //控制台显示日志信息（如果控制台可用）
                {
                    if (level < 50)
                    {
                        Console.WriteLine(logInfo);
                    }
                    else
                    {
                        //如果日志信息级别高于50， 以红色字体显示日志信息
                        logInfo = string.Format("{0} [{1}]", logInfo, LogHelper.GetCodeLocationOfLine());

                        ConsoleColor consoleColor = Console.ForegroundColor;
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine(logInfo);
                        Console.ForegroundColor = consoleColor;
                    }
                }

                StreamWriter fileWriterStream = new StreamWriter(logFilePath, true); //创建日志文件
                fileWriterStream.WriteLine(logInfo);
                fileWriterStream.Close(); //关闭fileWriterStream对象
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
            }
        }

        /// <summary>
        /// 获得当前行信息
        /// </summary>
        /// <returns>行信息</returns>
        private static string GetCodeLocationOfLine()
        {
            string[] lines = System.Environment.StackTrace.Split('\n');

            if (null != lines && lines.Length >= 5)
            {
                return lines[4];
            }
            else
            {
                return "don't find!!!";
            }

        }
    }
}
