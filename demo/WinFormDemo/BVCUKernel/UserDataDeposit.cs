using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsTest
{
    /// <summary>
    /// 用于C#层调用C++接口时寄存C#层数据，寄存数据会返回一个索引
    /// </summary>
    public class UserDataDeposit
    {
        private int iSerialNumber;
        public UserDataDeposit() 
        {
            iSerialNumber = 0;
            m_userdata_list = new List<UserDataItem>();
        }
        private int buildASN() 
        {
            iSerialNumber--;
            return iSerialNumber;
        }
        public int DepositAData(object userData) 
        {
            int iSN = buildASN();
            UserDataItem userDataItem = new UserDataItem(iSN, userData);
            m_userdata_list.Add(userDataItem);
            return iSN;
        }

        private List<UserDataItem> m_userdata_list;
        private class UserDataItem
        {
            private int iSN;
            private object userData;
            public UserDataItem(int iSN, object userData) 
            {
                this.iSN = iSN;
                this.userData = userData;
            }

            public bool IndexEquals(int iSN) 
            {
                return this.iSN == iSN;
            }

            public object GetUserData() 
            {
                return userData;
            }
        }

        public void ReleaseAllData() 
        {
            iSerialNumber = 0;
            m_userdata_list.Clear();
        }

        /// <summary>
        /// 通过索引获取用户数据
        /// </summary>
        /// <param name="iSN">索引</param>
        /// <param name="bIsDelete">获取用户数据后,是否删除m_userdata_list中该数据</param>
        /// <returns></returns>
        public object GetUserDataByIndex(int iSN, bool bIsDelete) 
        {
            UserDataItem userDataItem = null;
            object userData = null;
            foreach (UserDataItem item in m_userdata_list) 
            {
                if (item.IndexEquals(iSN)) 
                {
                    userDataItem = item;
                }
            }

            if (null != userDataItem) 
            {
                //m_userdata_list.Remove(userDataItem);
                userData = userDataItem.GetUserData();
            }
            return userData;
        }
    }
}
