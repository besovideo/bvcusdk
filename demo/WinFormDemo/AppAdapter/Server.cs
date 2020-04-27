using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace WindowsFormsTest
{

    /// <summary>
    /// 服务器
    /// </summary>
    public class Server
    {
        public string ip;
        public int port;
        public string usrName;
        public string psw;
        public IntPtr sessionHandle;
        public string ukeyid;
        public string ukeycode;
        public ArrayList puList;


        public Server()
        {
            sessionHandle = IntPtr.Zero;
            puList = new ArrayList();
        }


        public string ServerName
        {
            get
            {
                return ip + ":" + port;
            }
        }
    }
}
