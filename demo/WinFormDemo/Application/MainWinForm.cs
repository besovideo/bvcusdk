using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text;

namespace WindowsFormsTest
{
    public partial class MainWinForm : Form
    {
        #region 属性[部分]
        /// <summary>
        /// 视频浏览panel
        /// </summary>
        ArrayList m_videoPanels;
        /// <summary>
        /// SDK
        /// </summary>
        public BVCUSdkOperator m_sdkOperator;
        #endregion 属性[部分]


        /// <summary>
        /// 构造函数
        /// </summary>
        public MainWinForm()
        {
            m_videoPanels = new ArrayList();

            InitializeComponent();

            devideScreen();

            m_sdkOperator = new BVCUSdkOperator(this);

            m_getPuList = new GetPuListDel(procGetPuList);//设置获得设备列表后的响应

            m_capturePath = "";
            m_recordPath = "";

            m_activePanelBorder = new RectBorder(panelVideo, Color.Red);
            Panel panel = m_videoPanels[0] as Panel;
            m_activePanelBorder.show(panel.Location, panel.Width, panel.Height, ACTIVE_PANEL_BORDER_WIDTH);
            m_activePanel = panel;

            m_sdkOperator.PtzSpeed = (int)numericUpDownPTZSpeed.Value;
        }


        #region 浏览窗口绘制相关

        /// <summary>
        /// 划分窗口为2X2的4窗口结构
        /// </summary>
        const int VIDEO_PANEL_COUNT = 4;

        /// <summary>
        /// 划分窗口为2X2的4窗口结构
        /// </summary>
        void devideScreen()
        {
            for (int i = 0; i < VIDEO_PANEL_COUNT; i++)
            {
                Panel panel = new Panel();
                panel.BorderStyle = BorderStyle.Fixed3D;
                panel.Click += new System.EventHandler(panel_Click);
                panel.MouseUp += new MouseEventHandler(panel_MouseUp);
                m_videoPanels.Add(panel);
                panelVideo.Controls.Add(panel);
            }
            locateResizeVideoPanel();
        }

        /// <summary>
        /// 确定位置
        /// </summary>
        void locateResizeVideoPanel()
        {
            for (int i = 0; i < VIDEO_PANEL_COUNT; i++)
            {
                Panel panel = m_videoPanels[i] as Panel;
                panel.Width = panelVideo.Width / 2;
                panel.Height = panelVideo.Height / 2;
                switch (i)
                {
                    case 0:
                        panel.Location = new Point(0, 0);
                        break;
                    case 1:
                        panel.Location = new Point(panelVideo.Width / 2, 0);
                        break;
                    case 2:
                        panel.Location = new Point(0, panelVideo.Height / 2);
                        break;
                    case 3:
                        panel.Location = new Point(panelVideo.Width / 2, panelVideo.Height / 2);
                        break;
                }
            }
        }

        /// <summary>
        /// 绘制鼠标选中的当前窗口边框
        /// </summary>
        RectBorder m_activePanelBorder;

        const int ACTIVE_PANEL_BORDER_WIDTH = 3;//边框的宽度
        Panel m_activePanel;

        /// <summary>
        /// 响应小视屏窗口点击事件, 引用位置：this.devideScreen()
        /// </summary>
        void panel_Click(object sender, EventArgs e)
        {
            Panel panel = sender as Panel;
            m_activePanelBorder.show(panel.Location, panel.Width, panel.Height, ACTIVE_PANEL_BORDER_WIDTH);
            m_sdkOperator.Dialog.setVolume(m_activePanel, 0);
            m_activePanel = panel;
            m_sdkOperator.Dialog.setVolume(m_activePanel, 50);
        }

        /// <summary>
        /// 猜测：小视屏窗口的红线绘制，引用位置：m_activePanelBorder = new RectBorder(panelVideo, Color.Red);
        /// </summary>
        class RectBorder
        {
            PictureBox[] lines;
            public RectBorder(Control parent, Color color)
            {
                lines = new PictureBox[4];
                for (int i = 0; i < 4; i++)
                {
                    lines[i] = new PictureBox();
                    lines[i].BackColor = color;
                    parent.Controls.Add(lines[i]);
                    lines[i].BringToFront();
                }
            }
            public void show(Point location, int width, int height, int lineWidth)
            {
                lines[0].Height = lineWidth;
                lines[0].Width = width;
                lines[0].Location = location;
                lines[1].Height = height;
                lines[1].Width = lineWidth;
                lines[1].Location = new Point(location.X + width - lineWidth, location.Y);
                lines[2].Height = lineWidth;
                lines[2].Width = width;
                lines[2].Location = new Point(location.X, location.Y + height - lineWidth);
                lines[3].Height = height;
                lines[3].Width = lineWidth;
                lines[3].Location = location;
            }
        }


        /// <summary>
        /// 窗口大小改变时发生
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e3"></param>
        private void Form1_Resize(object sender, EventArgs e)
        {
            try
            {
                locateResizeVideoPanel();
                m_activePanelBorder.show(m_activePanel.Location, m_activePanel.Width, m_activePanel.Height, ACTIVE_PANEL_BORDER_WIDTH);
                m_sdkOperator.Dialog.OnResizeDialog();
            }
            catch
            { }
        }


        #endregion 浏览窗口绘制相关


        #region 登录

        /// <summary>
        /// 显示Login窗口, 在主窗体首次显示时就显示登录窗口
        /// </summary>
        private void FormLogin_Show(object sender, EventArgs e)
        {
            this.Hide();//131211增加
            using (FormLogin login = new FormLogin())
            {
                if (login.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                {
                    this.Close();
                    return;
                }

                m_sdkOperator.Session.login(login.IP, login.Port, login.UserName, login.Password);
            }

            this.Show();//131211增加
        }

        #endregion 登录


        #region 刷新设备列表

        /// <summary>
        /// 获取Pu列表后在消息回调中刷新界面上的资源树结构
        /// </summary>
        delegate void GetPuListDel();
        GetPuListDel m_getPuList;


        /// <summary>
        /// 获得设备列表，更新Form中设备TreeView的内容，被this.getPuList()调用
        /// </summary>
        void procGetPuList()
        {
            treeViewResList.Nodes.Clear();
            TreeNode session = new TreeNode(m_sdkOperator.Session.Name);
            treeViewResList.Nodes.Add(session);

            for (int i = 0; i < m_sdkOperator.Session.PuList.Count; i++)
            {
                Pu pu = (Pu)m_sdkOperator.Session.PuList[i];
                TreeNode puNode = new TreeNode();
                puNode.Name = pu.id;
                if (pu.puName.Length == 0)
                    puNode.Text = pu.id;
                else
                    puNode.Text = pu.puName;

                session.Nodes.Add(puNode);

                for (int chIndex = 0; chIndex < pu.channelList.Count; chIndex++)
                {
                    Channel channl = (Channel)pu.channelList[chIndex];
                    TreeNode channelNode = new TreeNode(channl.channelName);
                    if (channl.OnlineStatus)
                    {
                        channelNode.ForeColor = Color.Blue;
                    }
                    else
                    {
                        channelNode.ForeColor = Color.Gray;
                    }
                    channelNode.Tag = channl.channelNo;
                    puNode.Nodes.Add(channelNode);
                }

                if (pu.OnlineStatus)
                {
                    puNode.ForeColor = Color.Blue;
                }
                else
                {
                    puNode.ForeColor = Color.Gray;
                }
            }
        }

        /// <summary>
        /// 获得设备列表
        /// </summary>
        public void getPuList()
        {
            if (treeViewResList.IsHandleCreated)
            {
                treeViewResList.BeginInvoke(m_getPuList);
            }
        }

        #endregion 刷新设备列表


        #region 节点浏览有关事件
        /// <summary>
        /// 双击界面上的资源树打开对话
        /// </summary>
        const int TREE_LEVEL_SESSION = 0;
        const int TREE_LEVEL_PU = 1;
        const int TREE_LEVEL_CHANNEL = 2;

        /// <summary>
        /// 点击设备列表中的节点时发生
        /// </summary>
        private void treeViewResList_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (null == e || null == e.Node) return;

            //点击通道时发生，视频通道
            if (e.Node.Level == TREE_LEVEL_CHANNEL)
            {
                Pu pu = m_sdkOperator.Session.getPu(e.Node.Parent.Name);
                int channelNo = (int)e.Node.Tag;
                if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL && channelNo < BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                {
                    if (m_sdkOperator.Dialog.Count == VIDEO_PANEL_COUNT)
                    {
                        MessageBox.Show("窗口已全部占用");
                        return;
                    }
                    foreach (Panel panel in m_videoPanels)
                    {
                        if (panel.ContextMenuStrip == null)
                        {
                            panel.ContextMenuStrip = contextMenuStripVideo;
                            Console.WriteLine("Open dialog pu " + pu.id + " channel " + channelNo);

                            //打开浏览*******************************************************************
                            m_sdkOperator.Dialog.openBrowse(pu, channelNo, panel);

                            /*
                            Player p = new Player();
                            p.Show();

                            m_sdkOperator.Dialog.PreviewVideo(pu, channelNo, p.tbPlay);
                            */
                            break;
                        }
                    }
                }
                else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS && channelNo < BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                {
                    //打开Gps通道

                    if(m_sdkOperator.Dialog.openGpsDialog(pu, channelNo) == 0)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Text = pu.id;
                        item.Tag = channelNo;
                        ListViewItem.ListViewSubItem latItem = new ListViewItem.ListViewSubItem();
                        ListViewItem.ListViewSubItem lngItem = new ListViewItem.ListViewSubItem();
                        item.SubItems.AddRange(new ListViewItem.ListViewSubItem[] { latItem, lngItem });
                        listViewGPSData.Items.Add(item);
                    }
                }
                else
                {
                    //打开Tsp通道
                    if (m_sdkOperator.Dialog.openTspDialog(pu, channelNo) == 0)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Text = pu.id;
                        item.Tag = channelNo;
                        ListViewItem.ListViewSubItem TspData = new ListViewItem.ListViewSubItem();
                        ListViewItem.ListViewSubItem len = new ListViewItem.ListViewSubItem();
                        item.SubItems.AddRange(new ListViewItem.ListViewSubItem[] { TspData, len });

                        listViewGPSData.Items.Add(item);
                    }
                }
            }//点击通道时发生 end
        }

        /// <summary>
        /// 右键关闭对话菜单
        /// </summary>
        private void toolStripMenuItemClosePreview_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;
            panel.ContextMenuStrip = null;

            m_sdkOperator.Dialog.closeBrowse(panel);
        }

        /// <summary>
        /// 右键截图菜单
        /// </summary>
        private void toolStripMenuItemSnapshot_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;

            m_sdkOperator.Dialog.capture(panel);
        }

        /// <summary>
        /// 右键录像菜单
        /// </summary>
        private void toolStripMenuItemRecord_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;

            m_sdkOperator.Dialog.record(panel);
        }

        /// <summary>
        /// 视频浏览框 右键弹起事件响应
        /// </summary>
        void panel_MouseUp(object sender, MouseEventArgs e)
        {
            Panel panel = sender as Panel;
            if (m_sdkOperator.Dialog.Recording(panel))
            {
                toolStripMenuItemRecord.Checked = true;
            }
            else
            {
                toolStripMenuItemRecord.Checked = false;
            }
        }

        #endregion 浏览有关事件


        #region 串口发送数据

        /// <summary>
        ///  串口发送数据
        /// </summary>
        private void btnSendMsg_Click(object sender, EventArgs e)
        {
            this.m_sdkOperator.Dialog.SendTspData(this.tbInputMsg.Text);
            //this.tbInputMsg.Text = "";
        }

        #endregion 串口发送数据


        #region 其他事件

        private void toolStripMenuItemCloseGpsDialog_Click(object sender, EventArgs e)
        {
            if (listViewGPSData.SelectedItems.Count > 0)
            {
                foreach (ListViewItem item in listViewGPSData.SelectedItems)
                {
                    m_sdkOperator.Dialog.closeGpsDialog(item.Text, (int)item.Tag);                    
                    listViewGPSData.Items.Remove(item);

                    //新增
                    this.m_sdkOperator.Dialog.closeTspDialog(item.Text, (int)item.Tag);
                }
            }
        }

        /// <summary>
        /// 窗口关闭前释放所有资源
        /// </summary>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_sdkOperator.Session.logout();
            m_sdkOperator.Dispose();
        }

        #endregion 其他事件


        #region 配置截图录像的存储路径


        /// <summary>
        /// 配置截图及录像保存路径
        /// </summary>
        string m_capturePath;
        public string CapturePath
        {
            get
            {
                return m_capturePath;
            }
            private set
            {
                m_capturePath = value;
                labelCapturePath.Text = value;
            }
        }

        /// <summary>
        /// 配置截图及录像保存路径
        /// </summary>
        private void buttonCapturePath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                CapturePath = folderBrowserDialog.SelectedPath;
            }
        }

        /// <summary>
        /// 配置截图及录像保存路径
        /// </summary>
        string m_recordPath;
        public string RecordPath
        {
            get
            {
                return m_recordPath;
            }
            private set
            {
                m_recordPath = value;
                labelRecordPath.Text = value;
            }
        }

        /// <summary>
        /// 配置截图及录像保存路径
        /// </summary>
        private void buttonRecordPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                RecordPath = folderBrowserDialog.SelectedPath;
            }
        }

        #endregion 配置截图录像的存储路径


        #region 云台控制

        /// <summary>
        /// 云台控制响应
        /// </summary>
        /// <param name="button"></param>
        /// <param name="move"></param>
        void buttonPtzControl(Button button, BVCU_PTZ_MOVE move)
        {
            int cmd = BVCU.BVCU_PTZ_COMMAND.UP;
            if (button.Name.Equals("buttonPTZLeftUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.LEFTTOP;
            }
            else if (button.Name.Equals("buttonPTZUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.UP;
            }
            else if (button.Name.Equals("buttonPTZRightUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.RIGHTTOP;
            }
            else if (button.Name.Equals("buttonPTZRight"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.RIGHT;
            }
            else if (button.Name.Equals("buttonPTZRightDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.RIGHTDOWN;
            }
            else if (button.Name.Equals("buttonPTZDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.DOWN;
            }
            else if (button.Name.Equals("buttonPTZLeftDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.LEFTDOWN;
            }
            else if (button.Name.Equals("buttonPTZLeft"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.LEFT;
            }
            else if (button.Name.Equals("buttonLensUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.ZOOM_DEC;
            }
            else if (button.Name.Equals("buttonLensDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.ZOOM_INC;
            }
            else if (button.Name.Equals("buttonFocusUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.FOCUS_DEC;
            }
            else if (button.Name.Equals("buttonFocusDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.FOCUS_INC;
            }
            else if (button.Name.Equals("buttonApertureUp"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.APERTURE_DEC;
            }
            else if (button.Name.Equals("buttonApertureDown"))
            {
                cmd = BVCU.BVCU_PTZ_COMMAND.APERTURE_INC;
            }
            else
            {
                return;
            }
            OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);

            if (dlg != null)
            {
                m_sdkOperator.ptzControl(cmd, dlg.pu.id, dlg.channelNo, move);
            }
        }

        private void buttonPTZControl_Begin(object sender, MouseEventArgs e)
        {
            Button button = sender as Button;
            buttonPtzControl(button, BVCU_PTZ_MOVE.BEGIN);
        }

        private void buttonPTZControl_End(object sender, MouseEventArgs e)
        {
            Button button = sender as Button;
            buttonPtzControl(button, BVCU_PTZ_MOVE.STOP);
        }

        private void buttonPTZControl_MouseLeave(object sender, EventArgs e)
        {
            Button button = sender as Button;
            buttonPtzControl(button, BVCU_PTZ_MOVE.STOP);
        }

        private void numericUpDownPTZSpeed_ValueChanged(object sender, EventArgs e)
        {
            m_sdkOperator.PtzSpeed = (int)numericUpDownPTZSpeed.Value;
        }

        private void buttonGetPtzAttr_Click(object sender, EventArgs e)
        {
            OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
            if (dlg != null)
            {
                m_sdkOperator.getPtzAttr(dlg.pu.id, dlg.channelNo);
            }
        }

        private void comboBoxPresetPoint_DropDown(object sender, EventArgs e)
        {
            comboBoxPresetPoint.Items.Clear();
            OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
            if (dlg != null && dlg.pu.puPtz.Count > 0)
            {
                BVCU_PUCFG_PTZAttr ptzAttr = (BVCU_PUCFG_PTZAttr)dlg.pu.puPtz[0];
                foreach (BVCU_PUCFG_Preset preset in ptzAttr.stPreset)
                {
                    if (preset.iIndex != -1)
                    {
                        comboBoxPresetPoint.Items.Add(preset.iIndex + " | " + preset.szPreset);
                    }
                }
            }
        }

        private void comboBoxCruiseLine_DropDown(object sender, EventArgs e)
        {
            comboBoxCruiseLine.Items.Clear();
            OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
            if (dlg != null && dlg.pu.puPtz.Count > 0)
            {
                BVCU_PUCFG_PTZAttr ptzAttr = (BVCU_PUCFG_PTZAttr)dlg.pu.puPtz[0];
                foreach (BVCU_PUCFG_Cruise cruise in ptzAttr.stCruise)
                {
                    if (cruise.iIndex != -1)
                    {
                        comboBoxCruiseLine.Items.Add(cruise.iIndex + " | " + cruise.szName);
                    }
                }
            }
        }

        private void buttonGotoPresetPoint_Click(object sender, EventArgs e)
        {
            try
            {
                int index = int.Parse(comboBoxPresetPoint.Text.Substring(0, comboBoxPresetPoint.Text.IndexOf(" | ")));
                OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
                if (dlg == null)
                {
                    return;
                }
                m_sdkOperator.ptzControl(BVCU.BVCU_PTZ_COMMAND.PRESET_GO, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
            }
            catch
            { }
        }

        #endregion 云台控制


        #region  巡游

        /// <summary>
        /// 巡游事件响应
        /// </summary>
        bool m_cruiseBegin;
        /// <summary>
        /// 巡游事件响应
        /// </summary>
        private void buttonCruise_Click(object sender, EventArgs e)
        {
            try
            {
                int index = int.Parse(comboBoxCruiseLine.Text.Substring(0, comboBoxCruiseLine.Text.IndexOf(" | ")));
                m_cruiseBegin = !m_cruiseBegin;
                OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
                if (dlg == null)
                {
                    return;
                }
                if (m_cruiseBegin)
                {
                    m_sdkOperator.ptzControl(BVCU.BVCU_PTZ_COMMAND.CRUISE_GO, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
                }
                else
                {
                    m_sdkOperator.ptzControl(BVCU.BVCU_PTZ_COMMAND.CRUISE_STOP, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
                }
            }
            catch
            { }
        }

        #endregion 巡游


        #region GpsAndTsp

        /// <summary>
        /// 获得Gps数据
        /// </summary>
        public void onGetGpsData(string puId, BVCU_PUCFG_GPSData gpsData, Int32 len)
        {
            foreach (ListViewItem item in listViewGPSData.Items)
            {
                if (item.Text.Equals(puId, StringComparison.CurrentCultureIgnoreCase))
                {
                    float lat = 0;
                    float lng = 0;
                    ListViewItem.ListViewSubItem latItem = new ListViewItem.ListViewSubItem();
                    lat = (float)gpsData.iLatitude / BVCU.BVCU_LAT_LNG_UNIT;
                    lng = (float)gpsData.iLongitude / BVCU.BVCU_LAT_LNG_UNIT;
                    latItem.Text = lat.ToString("0.00000");
                    item.SubItems[1] = latItem;
                    ListViewItem.ListViewSubItem lngItem = new ListViewItem.ListViewSubItem();
                    lngItem.Text = lng.ToString("0.00000");
                    item.SubItems[2] = lngItem;
                    LogHelper.LogHelper.RecordLog(8, string.Format("({0}, {1})", lat.ToString("0.00000"), lng.ToString("0.00000")));
                    return;
                }
            }
        }

        /// <summary>将字节数组转换为字符串</summary>
        /// <param name="input"></param>
        /// <param name="hex_UI_string">true: 16进制字符串, false: UI字符串</param>
        /// <returns></returns>
        public string BytesToString_HexUI(byte[] input, bool hex_UI_string = false)
        {
            if (hex_UI_string)
            {
                return BytesToHexString(input);
            }
            else
            {
                return BytesToUIString(input);
            }
        }

        /// <summary>字节数组转换为16进制字符串</summary>
        public string BytesToHexString(byte[] data)
        {
            try
            {
                return BitConverter.ToString(data).Replace('-', ' ');
            }
            catch
            {
                return string.Empty;
            }
        }

        /// <summary>字节数组转换为界面字符串</summary>
        public string BytesToUIString(byte[] data)
        {
            try
            {
                return Encoding.UTF8.GetString(data);
            }
            catch
            {
                return string.Empty;
            }
        }
        /// <summary>
        /// 获得Tsp数据
        /// </summary>
        public void onGetTspData(string puId, int iChannelNum, byte[] byteTspData)
        {
            string strTspData = BytesToString_HexUI(byteTspData);
            foreach (ListViewItem item in listViewGPSData.Items)
            {
                if (item.Text == puId)
                {
                    //串口数据显示
                    {
                        DateTime dataNow = DateTime.Now;
                        if ( null == strTspData || strTspData.Length <= 0)
                            return;
                        string msg = string.Format("{0}-{1} MSG:{2} [{3}:{4}:{5}]\r\n", puId, iChannelNum, strTspData, dataNow.Hour, dataNow.Minute, dataNow.Second);
                        //this.tbTSPData.AppendText(msg);
                        //this.tbTSPData.ScrollToCaret();

                        LogHelper.LogHelper.RecordLog(2, msg);
                    }

                    if (item.Tag.ToString() == iChannelNum.ToString())
                    {
                        ListViewItem.ListViewSubItem TspData = new ListViewItem.ListViewSubItem();
                        TspData.Text = strTspData;
                        item.SubItems[1] = TspData;
                        ListViewItem.ListViewSubItem length = new ListViewItem.ListViewSubItem();
                        length.Text = strTspData.Length.ToString();
                        item.SubItems[2] = length;
                        break;
                    }
                }
            }
        }

        #endregion Gps

        delegate void SendTspMsg(bool isCanSendMsg);
        private SendTspMsg senTspMsg = null;
        public void TspStatusChange_OnEvent(bool isHaveOpen)
        {
            if (null == this.senTspMsg)
            {
                this.senTspMsg = new SendTspMsg(this.SenTspMsg_OnEvent);
            }

            this.BeginInvoke(this.senTspMsg, new object[] { isHaveOpen });
        }


        private void SenTspMsg_OnEvent(bool isCanSendMsg)
        {
            this.btnSendMsg.Enabled = isCanSendMsg;
        }

        int g_channelNo;
        Pu g_pu = null;
        private void treeViewResList_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (null == e || null == e.Node) return;

            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                if (e.Node.Level == TREE_LEVEL_CHANNEL)
                {
                    Pu pu = m_sdkOperator.Session.getPu(e.Node.Parent.Name);
                    int ichannelNo = (int)e.Node.Tag;
                    g_pu = pu;
                    g_channelNo = ichannelNo;
                    if (ichannelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_CHANNEL && ichannelNo < BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_CHANNEL)
                    {
                        contextMenuStripTalkOnly.Show(Control.MousePosition.X,
                            Control.MousePosition.Y);
                    }
                }
            }
        }

        // 打开对讲
        private void ToolStripMenuItemTalkOnly_Click(object sender, EventArgs e)
        {
            if (m_sdkOperator.Dialog.Count == VIDEO_PANEL_COUNT)
            {
                MessageBox.Show("窗口已全部占用");
                return;
            }
            foreach (Panel panel in m_videoPanels)
            {
                if (panel.ContextMenuStrip == null)
                {
                    panel.ContextMenuStrip = contextMenuStripVideo;
                    Console.WriteLine("Open dialog pu for talk" + g_pu.id + " channel " + g_channelNo);
                    m_sdkOperator.Dialog.openTalkOnly(g_pu, g_channelNo, panel);
                    break;
                }
            }
        }

        public void clearTreeNodes()
        {
            treeViewResList.Nodes.Clear();
            if (listViewGPSData.Items.Count > 0)
                listViewGPSData.Items.Clear();
        }

        public void ClearlistViewGPSData(string puid, int channelNo)
        {
            for (int i = 0; i < listViewGPSData.Items.Count; ++i)
            {
                ListViewItem item = listViewGPSData.Items[i];
                if (puid.Equals(item.Text, StringComparison.CurrentCultureIgnoreCase) && 
                    (int)item.Tag == channelNo)
                {
                    listViewGPSData.Items.Remove(item);
                }
            }
        }

        delegate void OnShowMessage(BVCU_Event_Source source);
        OnShowMessage delegateOnShowMessage;

        public void procShowAlarmEventMessage(BVCU_Event_Source source)
        {
            string strEvent = source.szID;
            string strEventType = BVCU.ConvertEventType2String(source.iEventType);
            if (strEvent.Length == 0)
            {
                strEvent = "null" + " " + strEventType + " " + source.szEventDesc;
            }
            else if (source.iEventType >= BVCU.BVCU_EVENT_TYPE.VIDEOLOST
                && source.iEventType <= BVCU.BVCU_EVENT_TYPE.OUTROUTE)
            {
                if (null != m_sdkOperator && null != m_sdkOperator.m_session)
                {
                    string puName = "";
                    Pu pu = m_sdkOperator.m_session.getPu( source.szID);
                    if (pu != null)
                    {
                        puName = pu.puName;
                    }

                    strEvent = puName + "(" + source.szID + ")" + " " + strEventType + " " + source.szEventDesc;
                    if (source.iEventType != BVCU.BVCU_EVENT_TYPE.PUONLINE &&
                        source.iEventType != BVCU.BVCU_EVENT_TYPE.PUOFFLINE)
                    {
                        if (source.bEnd == 1)
                        {
                            strEvent += " " + "结束";
                        }
                    }
                }
            }

            MessageBox.Show(strEvent,"WinformDemo");
        }

        public void onShowAlarmEventMessage(BVCU_Event_Source source)
        {
            if(this.IsHandleCreated)
            {
                if(null == delegateOnShowMessage )
                {
                    delegateOnShowMessage = new OnShowMessage(procShowAlarmEventMessage);
                }
                this.BeginInvoke(delegateOnShowMessage, new object[] { source });
            }
        }

        private void buttonGetGps_Click(object sender, EventArgs e)
        {
            if (g_channelNo < BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS &&
                g_channelNo > BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS || g_pu == null)
                return;
            Test_Struct s = new Test_Struct();
            s.i = 2;
            m_sdkOperator.GetGpsData(g_pu.id, g_channelNo, s);
        }

    }
}
