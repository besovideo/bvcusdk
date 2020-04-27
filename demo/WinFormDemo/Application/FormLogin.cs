using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsTest
{
    public partial class FormLogin : Form
    {
        public FormLogin()
        {
            InitializeComponent();
        }

        int port;
        private void buttonOK_Click(object sender, EventArgs e)
        {
            try
            {
                port = int.Parse(textBoxPort.Text);
            }
            catch
            {
                MessageBox.Show("非法输入");
            }
        }

        public string IP
        {
            get
            {
                return textBoxIp.Text;
            }
        }

        public int Port
        {
            get
            {
                return port;
            }
        }

        public string UserName
        {
            get
            {
                return textBoxUsrName.Text;
            }
        }

        public string Password
        {
            get
            {
                return textBoxPsw.Text;
            }
        }
    }
}
