using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;

namespace WindowsFormsTest
{
    public partial class Form1 : Form
    {
        ArrayList m_videoPanels;
        BVCUSDKOperator m_sdkOperator;
        public Form1()
        {
            m_videoPanels = new ArrayList();
            InitializeComponent();
            devideScreen();
            m_sdkOperator = new BVCUSDKOperator(this);
            m_getPuList = new GetPuListDel(procGetPuList);
            m_capturePath = "";
            m_recordPath = "";
            m_activePanelBorder = new RectBorder(panelVideo, Color.Red);
            Panel panel = m_videoPanels[0] as Panel;
            m_activePanelBorder.show(panel.Location, panel.Width, panel.Height, ACTIVE_PANEL_BORDER_WIDTH);
            m_activePanel = panel;
            m_sdkOperator.PtzSpeed = (int)numericUpDownPTZSpeed.Value;
        }

        /*
         * 划分窗口为2X2的4窗口结构
         */
        const int VIDEO_PANEL_COUNT = 4;
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

        /*
         * 绘制鼠标选中的当前窗口边框
         */
        RectBorder m_activePanelBorder;
        const int ACTIVE_PANEL_BORDER_WIDTH = 1;
        Panel m_activePanel;
        void panel_Click(object sender, EventArgs e)
        {
            Panel panel = sender as Panel;
            m_activePanelBorder.show(panel.Location, panel.Width, panel.Height, ACTIVE_PANEL_BORDER_WIDTH);
            m_sdkOperator.Dialog.setVolume(m_activePanel, 0);
            m_activePanel = panel;
            m_sdkOperator.Dialog.setVolume(m_activePanel, 50);
        }

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

        /*
         * 显示Login窗口
         */
        private void Form1_Shown(object sender, EventArgs e)
        {
            using (FormLogin login = new FormLogin())
            {
                if (login.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                {
                    this.Close();
                    return;
                }
                m_sdkOperator.Session.login(login.IP, login.Port, login.UserName, login.Password);
            }
        }

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

        /*
         * 获取Pu列表后在消息回调中刷新界面上的资源树结构
         */
        delegate void GetPuListDel();
        GetPuListDel m_getPuList;
        void procGetPuList()
        {
            treeViewResList.Nodes.Clear();
            TreeNode session = new TreeNode(m_sdkOperator.Session.Name);
            treeViewResList.Nodes.Add(session);
            for (int i = 0; i < m_sdkOperator.Session.PuList.Count; i++)
            {
                Session.Pu pu = (Session.Pu)m_sdkOperator.Session.PuList[i];
                TreeNode puNode = new TreeNode();
                puNode.Name = pu.id;
                if (pu.puName.Length == 0)
                    puNode.Text = pu.id;
                else
                    puNode.Text = pu.puName;

                session.Nodes.Add(puNode);
                bool online = false;
                foreach (Session.Channel channl in pu.channelList)
                {
                    TreeNode channelNode = new TreeNode(channl.channelName);
                    if (channl.online)
                    {
                        channelNode.ForeColor = Color.Blue;
                        online = true;
                    }
                    else
                    {
                        channelNode.ForeColor = Color.Gray;
                    }
                    puNode.Nodes.Add(channelNode);
                }
                if (online)
                {
                    puNode.ForeColor = Color.Blue;
                }
                else
                {
                    puNode.ForeColor = Color.Gray;
                }
            }
        }

        public void getPuList()
        {
            treeViewResList.BeginInvoke(m_getPuList);
        }

        /*
         * 双击界面上的资源树打开对话
         */
        const int TREE_LEVEL_SESSION = 0;
        const int TREE_LEVEL_PU = 1;
        const int TREE_LEVEL_CHANNEL = 2;

        private void treeViewResList_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node.Level == TREE_LEVEL_CHANNEL)
            {
                Session.Pu pu = m_sdkOperator.Session.getPu(e.Node.Parent.Name);
                int channelNo = pu.getChannelNo(e.Node.Text);
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
                            m_sdkOperator.Dialog.openBrowse(pu, channelNo, panel);
                            break;
                        }
                    }
                }
                else if (channelNo >= BVCU.BVCU_SUBDEV_INDEXMAJOR_MIN_GPS && channelNo < BVCU.BVCU_SUBDEV_INDEXMAJOR_MAX_GPS)
                {
                    if (m_sdkOperator.Dialog.openGpsDialog(pu, channelNo) == 0)
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
            }
        }

        /*
         * 右键关闭对话菜单
         */
        private void toolStripMenuItemClosePreview_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;
            panel.ContextMenuStrip = null;
            m_sdkOperator.Dialog.closeBrowse(panel);
        }

        /*
         * 右键截图菜单
         */
        private void toolStripMenuItemSnapshot_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;
            m_sdkOperator.Dialog.capture(panel);
        }

        /*
         * 右键录像菜单
         */
        private void toolStripMenuItemRecord_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            Panel panel = (menu.GetCurrentParent() as ContextMenuStrip).SourceControl as Panel;
            m_sdkOperator.Dialog.record(panel);
        }

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

        /*
         * 配置截图及录像保存路径
         */
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
        private void buttonCapturePath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                CapturePath = folderBrowserDialog.SelectedPath;
            }
        }

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
        private void buttonRecordPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                RecordPath = folderBrowserDialog.SelectedPath;
            }
        }

        /*
         * 窗口关闭前释放所有资源
         */
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_sdkOperator.Session.logout();
            m_sdkOperator.Dispose();
        }

        void buttonPtzControl(Button button, BVCU_PTZ_MOVE move)
        {
            int cmd = BVCU_PTZ_COMMAND.UP;
            if (button.Name.Equals("buttonPTZLeftUp"))
            {
                cmd = BVCU_PTZ_COMMAND.LEFTTOP;
            }
            else if (button.Name.Equals("buttonPTZUp"))
            {
                cmd = BVCU_PTZ_COMMAND.UP;
            }
            else if (button.Name.Equals("buttonPTZRightUp"))
            {
                cmd = BVCU_PTZ_COMMAND.RIGHTTOP;
            }
            else if (button.Name.Equals("buttonPTZRight"))
            {
                cmd = BVCU_PTZ_COMMAND.RIGHT;
            }
            else if (button.Name.Equals("buttonPTZRightDown"))
            {
                cmd = BVCU_PTZ_COMMAND.RIGHTDOWN;
            }
            else if (button.Name.Equals("buttonPTZDown"))
            {
                cmd = BVCU_PTZ_COMMAND.DOWN;
            }
            else if (button.Name.Equals("buttonPTZLeftDown"))
            {
                cmd = BVCU_PTZ_COMMAND.LEFTDOWN;
            }
            else if (button.Name.Equals("buttonPTZLeft"))
            {
                cmd = BVCU_PTZ_COMMAND.LEFT;
            }
            else if (button.Name.Equals("buttonLensUp"))
            {
                cmd = BVCU_PTZ_COMMAND.ZOOM_DEC;
            }
            else if (button.Name.Equals("buttonLensDown"))
            {
                cmd = BVCU_PTZ_COMMAND.ZOOM_INC;
            }
            else if (button.Name.Equals("buttonFocusUp"))
            {
                cmd = BVCU_PTZ_COMMAND.FOCUS_DEC;
            }
            else if (button.Name.Equals("buttonFocusDown"))
            {
                cmd = BVCU_PTZ_COMMAND.FOCUS_INC;
            }
            else if (button.Name.Equals("buttonApertureUp"))
            {
                cmd = BVCU_PTZ_COMMAND.APERTURE_DEC;
            }
            else if (button.Name.Equals("buttonApertureDown"))
            {
                cmd = BVCU_PTZ_COMMAND.APERTURE_INC;
            }
            else
            {
                return;
            }
            Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
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
            Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
            if (dlg != null)
            {
                m_sdkOperator.getPtzAttr(dlg.pu.id, dlg.channelNo);
            }
        }

        private void comboBoxPresetPoint_DropDown(object sender, EventArgs e)
        {
            comboBoxPresetPoint.Items.Clear();
            Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
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
            Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
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
                Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
                if (dlg == null)
                {
                    return;
                }
                m_sdkOperator.ptzControl(BVCU_PTZ_COMMAND.PRESET_GO, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
            }
            catch
            { }
        }

        bool m_cruiseBegin;
        private void buttonCruise_Click(object sender, EventArgs e)
        {
            try
            {
                int index = int.Parse(comboBoxCruiseLine.Text.Substring(0, comboBoxCruiseLine.Text.IndexOf(" | ")));
                m_cruiseBegin = !m_cruiseBegin;
                Dialog.OneDialog dlg = m_sdkOperator.Dialog.getDialog(m_activePanel);
                if (dlg == null)
                {
                    return;
                }
                if (m_cruiseBegin)
                {
                    m_sdkOperator.ptzControl(BVCU_PTZ_COMMAND.CRUISE_GO, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
                }
                else
                {
                    m_sdkOperator.ptzControl(BVCU_PTZ_COMMAND.CRUISE_STOP, dlg.pu.id, dlg.channelNo, BVCU_PTZ_MOVE.BEGIN, index);
                }
            }
            catch
            { }
        }

        public void onGetGpsData(string puId, float lat, float lng)
        {
            foreach (ListViewItem item in listViewGPSData.Items)
            {
                if (item.Text.Equals(puId, StringComparison.CurrentCultureIgnoreCase))
                {
                    ListViewItem.ListViewSubItem latItem = new ListViewItem.ListViewSubItem();
                    latItem.Text = lat.ToString("0.00000");
                    item.SubItems[1] = latItem;
                    ListViewItem.ListViewSubItem lngItem = new ListViewItem.ListViewSubItem();
                    lngItem.Text = lng.ToString("0.00000");
                    item.SubItems[2] = lngItem;
                    return;
                }
            }
        }
        public void onGetTspData(string puId, int iChannelNum, string pTspData, int len)
        {
            foreach (ListViewItem item in listViewGPSData.Items)
            {
                if (item.Text == puId)
                {
                    Console.WriteLine("item.Tag = {0}, iChannelNum = {1}.", item.Tag.ToString(), iChannelNum.ToString());
                    if (item.Tag.ToString() == iChannelNum.ToString())
                    {
                        ListViewItem.ListViewSubItem TspData = new ListViewItem.ListViewSubItem();
                        TspData.Text = pTspData;
                        item.SubItems[1] = TspData;
                        ListViewItem.ListViewSubItem length = new ListViewItem.ListViewSubItem();
                        length.Text = len.ToString();
                        item.SubItems[2] = length;
                        break;
                    }
                }
            }
        }

        private void toolStripMenuItemCloseGpsDialog_Click(object sender, EventArgs e)
        {
            if (listViewGPSData.SelectedItems.Count > 0)
            {
                foreach (ListViewItem item in listViewGPSData.SelectedItems)
                {
                    m_sdkOperator.Dialog.closeGpsDialog(item.Text, (int)item.Tag);
                    listViewGPSData.Items.Remove(item);
                }
            }
        }
    }
}
