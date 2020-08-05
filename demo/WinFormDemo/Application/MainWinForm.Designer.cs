namespace WindowsFormsTest
{
    partial class MainWinForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.buttonCapturePath = new System.Windows.Forms.Button();
            this.buttonRecordPath = new System.Windows.Forms.Button();
            this.labelCapturePath = new System.Windows.Forms.Label();
            this.labelRecordPath = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.buttonApertureDown = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonApertureUp = new System.Windows.Forms.Button();
            this.buttonFocusDown = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonFocusUp = new System.Windows.Forms.Button();
            this.buttonLensDown = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonLensUp = new System.Windows.Forms.Button();
            this.buttonPTZRightDown = new System.Windows.Forms.Button();
            this.buttonPTZDown = new System.Windows.Forms.Button();
            this.buttonPTZLeftDown = new System.Windows.Forms.Button();
            this.buttonPTZRight = new System.Windows.Forms.Button();
            this.buttonPTZLeft = new System.Windows.Forms.Button();
            this.buttonPTZRightUp = new System.Windows.Forms.Button();
            this.buttonPTZUp = new System.Windows.Forms.Button();
            this.buttonPTZLeftUp = new System.Windows.Forms.Button();
            this.numericUpDownPTZSpeed = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.buttonGetPtzAttr = new System.Windows.Forms.Button();
            this.buttonCruise = new System.Windows.Forms.Button();
            this.comboBoxCruiseLine = new System.Windows.Forms.ComboBox();
            this.buttonGotoPresetPoint = new System.Windows.Forms.Button();
            this.comboBoxPresetPoint = new System.Windows.Forms.ComboBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.btnSendMsg = new System.Windows.Forms.Button();
            this.listViewGPSData = new System.Windows.Forms.ListView();
            this.columnHeaderPuName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderLat = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderLng = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuStripGps = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItemCloseGpsDialog = new System.Windows.Forms.ToolStripMenuItem();
            this.tbInputMsg = new System.Windows.Forms.TextBox();
            this.tabPageGetGps = new System.Windows.Forms.TabPage();
            this.buttonGetGps = new System.Windows.Forms.Button();
            this.tabPageDocRetrieval = new System.Windows.Forms.TabPage();
            this.buttonSearchFile = new System.Windows.Forms.Button();
            this.dateTimePickerBegin = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerEnd = new System.Windows.Forms.DateTimePicker();
            this.dataGridViewRecord = new System.Windows.Forms.DataGridView();
            this.num = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.puId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.channelNo = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.reason = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.beginTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.endTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.fileType = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.fileSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.downloadPercent = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.fileName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.filePath = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.description1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.description2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.treeViewResList = new System.Windows.Forms.TreeView();
            this.panelVideo = new System.Windows.Forms.Panel();
            this.contextMenuStripVideo = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItemClosePreview = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSnapshot = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemRecord = new System.Windows.Forms.ToolStripMenuItem();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.contextMenuStripTalkOnly = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ToolStripMenuItemTalkOnly = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripDocRetrieval = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ToolStripMenuItemDocRetrieval = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripFileDown = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ToolStripMenuItemFileDown = new System.Windows.Forms.ToolStripMenuItem();
            this.panelLeft.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPTZSpeed)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.contextMenuStripGps.SuspendLayout();
            this.tabPageGetGps.SuspendLayout();
            this.tabPageDocRetrieval.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewRecord)).BeginInit();
            this.contextMenuStripVideo.SuspendLayout();
            this.contextMenuStripTalkOnly.SuspendLayout();
            this.contextMenuStripDocRetrieval.SuspendLayout();
            this.contextMenuStripFileDown.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelLeft
            // 
            this.panelLeft.Controls.Add(this.tabControl);
            this.panelLeft.Controls.Add(this.treeViewResList);
            this.panelLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLeft.Location = new System.Drawing.Point(0, 0);
            this.panelLeft.Name = "panelLeft";
            this.panelLeft.Size = new System.Drawing.Size(235, 509);
            this.panelLeft.TabIndex = 0;
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabPage1);
            this.tabControl.Controls.Add(this.tabPage2);
            this.tabControl.Controls.Add(this.tabPage3);
            this.tabControl.Controls.Add(this.tabPage4);
            this.tabControl.Controls.Add(this.tabPageGetGps);
            this.tabControl.Controls.Add(this.tabPageDocRetrieval);
            this.tabControl.Location = new System.Drawing.Point(12, 12);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(217, 228);
            this.tabControl.TabIndex = 5;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.buttonCapturePath);
            this.tabPage1.Controls.Add(this.buttonRecordPath);
            this.tabPage1.Controls.Add(this.labelCapturePath);
            this.tabPage1.Controls.Add(this.labelRecordPath);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(209, 202);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "常规";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // buttonCapturePath
            // 
            this.buttonCapturePath.Location = new System.Drawing.Point(85, 31);
            this.buttonCapturePath.Name = "buttonCapturePath";
            this.buttonCapturePath.Size = new System.Drawing.Size(83, 23);
            this.buttonCapturePath.TabIndex = 2;
            this.buttonCapturePath.Text = "截图路径...";
            this.buttonCapturePath.UseVisualStyleBackColor = true;
            this.buttonCapturePath.Click += new System.EventHandler(this.buttonCapturePath_Click);
            // 
            // buttonRecordPath
            // 
            this.buttonRecordPath.Location = new System.Drawing.Point(85, 77);
            this.buttonRecordPath.Name = "buttonRecordPath";
            this.buttonRecordPath.Size = new System.Drawing.Size(83, 23);
            this.buttonRecordPath.TabIndex = 1;
            this.buttonRecordPath.Text = "录像路径...";
            this.buttonRecordPath.UseVisualStyleBackColor = true;
            this.buttonRecordPath.Click += new System.EventHandler(this.buttonRecordPath_Click);
            // 
            // labelCapturePath
            // 
            this.labelCapturePath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelCapturePath.Location = new System.Drawing.Point(6, 10);
            this.labelCapturePath.Name = "labelCapturePath";
            this.labelCapturePath.Size = new System.Drawing.Size(162, 18);
            this.labelCapturePath.TabIndex = 3;
            // 
            // labelRecordPath
            // 
            this.labelRecordPath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelRecordPath.Location = new System.Drawing.Point(6, 57);
            this.labelRecordPath.Name = "labelRecordPath";
            this.labelRecordPath.Size = new System.Drawing.Size(162, 17);
            this.labelRecordPath.TabIndex = 4;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.buttonApertureDown);
            this.tabPage2.Controls.Add(this.label4);
            this.tabPage2.Controls.Add(this.buttonApertureUp);
            this.tabPage2.Controls.Add(this.buttonFocusDown);
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Controls.Add(this.buttonFocusUp);
            this.tabPage2.Controls.Add(this.buttonLensDown);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.buttonLensUp);
            this.tabPage2.Controls.Add(this.buttonPTZRightDown);
            this.tabPage2.Controls.Add(this.buttonPTZDown);
            this.tabPage2.Controls.Add(this.buttonPTZLeftDown);
            this.tabPage2.Controls.Add(this.buttonPTZRight);
            this.tabPage2.Controls.Add(this.buttonPTZLeft);
            this.tabPage2.Controls.Add(this.buttonPTZRightUp);
            this.tabPage2.Controls.Add(this.buttonPTZUp);
            this.tabPage2.Controls.Add(this.buttonPTZLeftUp);
            this.tabPage2.Controls.Add(this.numericUpDownPTZSpeed);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(209, 202);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "云台";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // buttonApertureDown
            // 
            this.buttonApertureDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApertureDown.Location = new System.Drawing.Point(96, 134);
            this.buttonApertureDown.Name = "buttonApertureDown";
            this.buttonApertureDown.Size = new System.Drawing.Size(39, 23);
            this.buttonApertureDown.TabIndex = 18;
            this.buttonApertureDown.Text = "大";
            this.buttonApertureDown.UseVisualStyleBackColor = true;
            this.buttonApertureDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonApertureDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonApertureDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(96, 119);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 12);
            this.label4.TabIndex = 17;
            this.label4.Text = "光圈";
            // 
            // buttonApertureUp
            // 
            this.buttonApertureUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApertureUp.Location = new System.Drawing.Point(96, 93);
            this.buttonApertureUp.Name = "buttonApertureUp";
            this.buttonApertureUp.Size = new System.Drawing.Size(39, 23);
            this.buttonApertureUp.TabIndex = 16;
            this.buttonApertureUp.Text = "小";
            this.buttonApertureUp.UseVisualStyleBackColor = true;
            this.buttonApertureUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonApertureUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonApertureUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonFocusDown
            // 
            this.buttonFocusDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonFocusDown.Location = new System.Drawing.Point(51, 134);
            this.buttonFocusDown.Name = "buttonFocusDown";
            this.buttonFocusDown.Size = new System.Drawing.Size(39, 23);
            this.buttonFocusDown.TabIndex = 15;
            this.buttonFocusDown.Text = "远";
            this.buttonFocusDown.UseVisualStyleBackColor = true;
            this.buttonFocusDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonFocusDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonFocusDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(51, 119);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 14;
            this.label3.Text = "调焦";
            // 
            // buttonFocusUp
            // 
            this.buttonFocusUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonFocusUp.Location = new System.Drawing.Point(51, 93);
            this.buttonFocusUp.Name = "buttonFocusUp";
            this.buttonFocusUp.Size = new System.Drawing.Size(39, 23);
            this.buttonFocusUp.TabIndex = 13;
            this.buttonFocusUp.Text = "近";
            this.buttonFocusUp.UseVisualStyleBackColor = true;
            this.buttonFocusUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonFocusUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonFocusUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonLensDown
            // 
            this.buttonLensDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonLensDown.Location = new System.Drawing.Point(6, 134);
            this.buttonLensDown.Name = "buttonLensDown";
            this.buttonLensDown.Size = new System.Drawing.Size(39, 23);
            this.buttonLensDown.TabIndex = 12;
            this.buttonLensDown.Text = "放";
            this.buttonLensDown.UseVisualStyleBackColor = true;
            this.buttonLensDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonLensDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonLensDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 119);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 11;
            this.label2.Text = "镜头";
            // 
            // buttonLensUp
            // 
            this.buttonLensUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonLensUp.Location = new System.Drawing.Point(6, 93);
            this.buttonLensUp.Name = "buttonLensUp";
            this.buttonLensUp.Size = new System.Drawing.Size(39, 23);
            this.buttonLensUp.TabIndex = 10;
            this.buttonLensUp.Text = "缩";
            this.buttonLensUp.UseVisualStyleBackColor = true;
            this.buttonLensUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonLensUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonLensUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZRightDown
            // 
            this.buttonPTZRightDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZRightDown.Location = new System.Drawing.Point(96, 64);
            this.buttonPTZRightDown.Name = "buttonPTZRightDown";
            this.buttonPTZRightDown.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZRightDown.TabIndex = 9;
            this.buttonPTZRightDown.Text = "右下";
            this.buttonPTZRightDown.UseVisualStyleBackColor = true;
            this.buttonPTZRightDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZRightDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZRightDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZDown
            // 
            this.buttonPTZDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZDown.Location = new System.Drawing.Point(51, 64);
            this.buttonPTZDown.Name = "buttonPTZDown";
            this.buttonPTZDown.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZDown.TabIndex = 8;
            this.buttonPTZDown.Text = "下";
            this.buttonPTZDown.UseVisualStyleBackColor = true;
            this.buttonPTZDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZLeftDown
            // 
            this.buttonPTZLeftDown.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZLeftDown.Location = new System.Drawing.Point(6, 64);
            this.buttonPTZLeftDown.Name = "buttonPTZLeftDown";
            this.buttonPTZLeftDown.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZLeftDown.TabIndex = 7;
            this.buttonPTZLeftDown.Text = "左下";
            this.buttonPTZLeftDown.UseVisualStyleBackColor = true;
            this.buttonPTZLeftDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZLeftDown.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZLeftDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZRight
            // 
            this.buttonPTZRight.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZRight.Location = new System.Drawing.Point(96, 35);
            this.buttonPTZRight.Name = "buttonPTZRight";
            this.buttonPTZRight.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZRight.TabIndex = 6;
            this.buttonPTZRight.Text = "右";
            this.buttonPTZRight.UseVisualStyleBackColor = true;
            this.buttonPTZRight.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZRight.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZRight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZLeft
            // 
            this.buttonPTZLeft.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZLeft.Location = new System.Drawing.Point(6, 35);
            this.buttonPTZLeft.Name = "buttonPTZLeft";
            this.buttonPTZLeft.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZLeft.TabIndex = 5;
            this.buttonPTZLeft.Text = "左";
            this.buttonPTZLeft.UseVisualStyleBackColor = true;
            this.buttonPTZLeft.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZLeft.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZLeft.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZRightUp
            // 
            this.buttonPTZRightUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZRightUp.Location = new System.Drawing.Point(96, 6);
            this.buttonPTZRightUp.Name = "buttonPTZRightUp";
            this.buttonPTZRightUp.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZRightUp.TabIndex = 4;
            this.buttonPTZRightUp.Text = "右上";
            this.buttonPTZRightUp.UseVisualStyleBackColor = true;
            this.buttonPTZRightUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZRightUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZRightUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZUp
            // 
            this.buttonPTZUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZUp.Location = new System.Drawing.Point(51, 6);
            this.buttonPTZUp.Name = "buttonPTZUp";
            this.buttonPTZUp.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZUp.TabIndex = 3;
            this.buttonPTZUp.Text = "上";
            this.buttonPTZUp.UseVisualStyleBackColor = true;
            this.buttonPTZUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // buttonPTZLeftUp
            // 
            this.buttonPTZLeftUp.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPTZLeftUp.Location = new System.Drawing.Point(6, 6);
            this.buttonPTZLeftUp.Name = "buttonPTZLeftUp";
            this.buttonPTZLeftUp.Size = new System.Drawing.Size(39, 23);
            this.buttonPTZLeftUp.TabIndex = 2;
            this.buttonPTZLeftUp.Text = "左上";
            this.buttonPTZLeftUp.UseVisualStyleBackColor = true;
            this.buttonPTZLeftUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_Begin);
            this.buttonPTZLeftUp.MouseLeave += new System.EventHandler(this.buttonPTZControl_MouseLeave);
            this.buttonPTZLeftUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPTZControl_End);
            // 
            // numericUpDownPTZSpeed
            // 
            this.numericUpDownPTZSpeed.Location = new System.Drawing.Point(62, 175);
            this.numericUpDownPTZSpeed.Maximum = new decimal(new int[] {
            15,
            0,
            0,
            0});
            this.numericUpDownPTZSpeed.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownPTZSpeed.Name = "numericUpDownPTZSpeed";
            this.numericUpDownPTZSpeed.Size = new System.Drawing.Size(106, 21);
            this.numericUpDownPTZSpeed.TabIndex = 1;
            this.numericUpDownPTZSpeed.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numericUpDownPTZSpeed.ValueChanged += new System.EventHandler(this.numericUpDownPTZSpeed_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 177);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "云台速度";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.buttonGetPtzAttr);
            this.tabPage3.Controls.Add(this.buttonCruise);
            this.tabPage3.Controls.Add(this.comboBoxCruiseLine);
            this.tabPage3.Controls.Add(this.buttonGotoPresetPoint);
            this.tabPage3.Controls.Add(this.comboBoxPresetPoint);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(209, 202);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "巡航";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // buttonGetPtzAttr
            // 
            this.buttonGetPtzAttr.Location = new System.Drawing.Point(3, 3);
            this.buttonGetPtzAttr.Name = "buttonGetPtzAttr";
            this.buttonGetPtzAttr.Size = new System.Drawing.Size(74, 23);
            this.buttonGetPtzAttr.TabIndex = 4;
            this.buttonGetPtzAttr.Text = "获取信息";
            this.buttonGetPtzAttr.UseVisualStyleBackColor = true;
            this.buttonGetPtzAttr.Click += new System.EventHandler(this.buttonGetPtzAttr_Click);
            // 
            // buttonCruise
            // 
            this.buttonCruise.Location = new System.Drawing.Point(96, 113);
            this.buttonCruise.Name = "buttonCruise";
            this.buttonCruise.Size = new System.Drawing.Size(75, 23);
            this.buttonCruise.TabIndex = 3;
            this.buttonCruise.Text = "巡航";
            this.buttonCruise.UseVisualStyleBackColor = true;
            this.buttonCruise.Click += new System.EventHandler(this.buttonCruise_Click);
            // 
            // comboBoxCruiseLine
            // 
            this.comboBoxCruiseLine.FormattingEnabled = true;
            this.comboBoxCruiseLine.Location = new System.Drawing.Point(3, 87);
            this.comboBoxCruiseLine.Name = "comboBoxCruiseLine";
            this.comboBoxCruiseLine.Size = new System.Drawing.Size(168, 20);
            this.comboBoxCruiseLine.TabIndex = 2;
            this.comboBoxCruiseLine.DropDown += new System.EventHandler(this.comboBoxCruiseLine_DropDown);
            // 
            // buttonGotoPresetPoint
            // 
            this.buttonGotoPresetPoint.Location = new System.Drawing.Point(96, 58);
            this.buttonGotoPresetPoint.Name = "buttonGotoPresetPoint";
            this.buttonGotoPresetPoint.Size = new System.Drawing.Size(75, 23);
            this.buttonGotoPresetPoint.TabIndex = 1;
            this.buttonGotoPresetPoint.Text = "前往";
            this.buttonGotoPresetPoint.UseVisualStyleBackColor = true;
            this.buttonGotoPresetPoint.Click += new System.EventHandler(this.buttonGotoPresetPoint_Click);
            // 
            // comboBoxPresetPoint
            // 
            this.comboBoxPresetPoint.FormattingEnabled = true;
            this.comboBoxPresetPoint.Location = new System.Drawing.Point(3, 32);
            this.comboBoxPresetPoint.Name = "comboBoxPresetPoint";
            this.comboBoxPresetPoint.Size = new System.Drawing.Size(168, 20);
            this.comboBoxPresetPoint.TabIndex = 0;
            this.comboBoxPresetPoint.DropDown += new System.EventHandler(this.comboBoxPresetPoint_DropDown);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.btnSendMsg);
            this.tabPage4.Controls.Add(this.listViewGPSData);
            this.tabPage4.Controls.Add(this.tbInputMsg);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(209, 202);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "GPS/串口";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // btnSendMsg
            // 
            this.btnSendMsg.Enabled = false;
            this.btnSendMsg.Location = new System.Drawing.Point(148, 176);
            this.btnSendMsg.Name = "btnSendMsg";
            this.btnSendMsg.Size = new System.Drawing.Size(52, 23);
            this.btnSendMsg.TabIndex = 0;
            this.btnSendMsg.Text = "send";
            this.btnSendMsg.UseVisualStyleBackColor = true;
            this.btnSendMsg.Click += new System.EventHandler(this.btnSendMsg_Click);
            // 
            // listViewGPSData
            // 
            this.listViewGPSData.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewGPSData.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderPuName,
            this.columnHeaderLat,
            this.columnHeaderLng});
            this.listViewGPSData.ContextMenuStrip = this.contextMenuStripGps;
            this.listViewGPSData.FullRowSelect = true;
            this.listViewGPSData.GridLines = true;
            this.listViewGPSData.Location = new System.Drawing.Point(0, 0);
            this.listViewGPSData.Name = "listViewGPSData";
            this.listViewGPSData.Size = new System.Drawing.Size(209, 170);
            this.listViewGPSData.TabIndex = 0;
            this.listViewGPSData.UseCompatibleStateImageBehavior = false;
            this.listViewGPSData.View = System.Windows.Forms.View.Details;
            // 
            // columnHeaderPuName
            // 
            this.columnHeaderPuName.Text = "设备名";
            this.columnHeaderPuName.Width = 68;
            // 
            // columnHeaderLat
            // 
            this.columnHeaderLat.Text = "纬度/Tsp";
            this.columnHeaderLat.Width = 69;
            // 
            // columnHeaderLng
            // 
            this.columnHeaderLng.Text = "经度/Len";
            this.columnHeaderLng.Width = 67;
            // 
            // contextMenuStripGps
            // 
            this.contextMenuStripGps.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemCloseGpsDialog});
            this.contextMenuStripGps.Name = "contextMenuStripGps";
            this.contextMenuStripGps.Size = new System.Drawing.Size(136, 26);
            // 
            // toolStripMenuItemCloseGpsDialog
            // 
            this.toolStripMenuItemCloseGpsDialog.Name = "toolStripMenuItemCloseGpsDialog";
            this.toolStripMenuItemCloseGpsDialog.Size = new System.Drawing.Size(135, 22);
            this.toolStripMenuItemCloseGpsDialog.Text = "关闭Gps流";
            this.toolStripMenuItemCloseGpsDialog.Click += new System.EventHandler(this.toolStripMenuItemCloseGpsDialog_Click);
            // 
            // tbInputMsg
            // 
            this.tbInputMsg.Location = new System.Drawing.Point(3, 178);
            this.tbInputMsg.Name = "tbInputMsg";
            this.tbInputMsg.Size = new System.Drawing.Size(139, 21);
            this.tbInputMsg.TabIndex = 0;
            // 
            // tabPageGetGps
            // 
            this.tabPageGetGps.Controls.Add(this.buttonGetGps);
            this.tabPageGetGps.Location = new System.Drawing.Point(4, 22);
            this.tabPageGetGps.Name = "tabPageGetGps";
            this.tabPageGetGps.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageGetGps.Size = new System.Drawing.Size(209, 202);
            this.tabPageGetGps.TabIndex = 4;
            this.tabPageGetGps.Text = "获取GPS";
            this.tabPageGetGps.UseVisualStyleBackColor = true;
            // 
            // buttonGetGps
            // 
            this.buttonGetGps.Location = new System.Drawing.Point(6, 6);
            this.buttonGetGps.Name = "buttonGetGps";
            this.buttonGetGps.Size = new System.Drawing.Size(93, 23);
            this.buttonGetGps.TabIndex = 0;
            this.buttonGetGps.Text = "获取";
            this.buttonGetGps.UseVisualStyleBackColor = true;
            this.buttonGetGps.Click += new System.EventHandler(this.buttonGetGps_Click);
            // 
            // tabPageDocRetrieval
            // 
            this.tabPageDocRetrieval.Controls.Add(this.buttonSearchFile);
            this.tabPageDocRetrieval.Controls.Add(this.dateTimePickerBegin);
            this.tabPageDocRetrieval.Controls.Add(this.dateTimePickerEnd);
            this.tabPageDocRetrieval.Controls.Add(this.dataGridViewRecord);
            this.tabPageDocRetrieval.Location = new System.Drawing.Point(4, 22);
            this.tabPageDocRetrieval.Name = "tabPageDocRetrieval";
            this.tabPageDocRetrieval.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageDocRetrieval.Size = new System.Drawing.Size(209, 202);
            this.tabPageDocRetrieval.TabIndex = 5;
            this.tabPageDocRetrieval.Text = "文件检索";
            this.tabPageDocRetrieval.UseVisualStyleBackColor = true;
            // 
            // buttonSearchFile
            // 
            this.buttonSearchFile.Location = new System.Drawing.Point(167, 6);
            this.buttonSearchFile.Name = "buttonSearchFile";
            this.buttonSearchFile.Size = new System.Drawing.Size(36, 34);
            this.buttonSearchFile.TabIndex = 9;
            this.buttonSearchFile.Text = "检索";
            this.buttonSearchFile.UseVisualStyleBackColor = true;
            this.buttonSearchFile.Visible = false;
            // 
            // dateTimePickerBegin
            // 
            this.dateTimePickerBegin.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.dateTimePickerBegin.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerBegin.Location = new System.Drawing.Point(3, 3);
            this.dateTimePickerBegin.MinDate = new System.DateTime(2000, 1, 1, 0, 0, 0, 0);
            this.dateTimePickerBegin.Name = "dateTimePickerBegin";
            this.dateTimePickerBegin.Size = new System.Drawing.Size(157, 21);
            this.dateTimePickerBegin.TabIndex = 7;
            // 
            // dateTimePickerEnd
            // 
            this.dateTimePickerEnd.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.dateTimePickerEnd.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerEnd.Location = new System.Drawing.Point(3, 30);
            this.dateTimePickerEnd.MinDate = new System.DateTime(2000, 1, 1, 0, 0, 0, 0);
            this.dateTimePickerEnd.Name = "dateTimePickerEnd";
            this.dateTimePickerEnd.Size = new System.Drawing.Size(157, 21);
            this.dateTimePickerEnd.TabIndex = 8;
            // 
            // dataGridViewRecord
            // 
            this.dataGridViewRecord.AllowUserToAddRows = false;
            this.dataGridViewRecord.AllowUserToDeleteRows = false;
            this.dataGridViewRecord.BackgroundColor = System.Drawing.SystemColors.Control;
            this.dataGridViewRecord.BorderStyle = System.Windows.Forms.BorderStyle.None;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle4.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle4.Padding = new System.Windows.Forms.Padding(3, 0, 3, 0);
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewRecord.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this.dataGridViewRecord.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            this.dataGridViewRecord.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.num,
            this.puId,
            this.channelNo,
            this.reason,
            this.beginTime,
            this.endTime,
            this.fileType,
            this.fileSize,
            this.downloadPercent,
            this.fileName,
            this.filePath,
            this.description1,
            this.description2});
            this.dataGridViewRecord.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.dataGridViewRecord.Location = new System.Drawing.Point(3, 57);
            this.dataGridViewRecord.Name = "dataGridViewRecord";
            this.dataGridViewRecord.ReadOnly = true;
            this.dataGridViewRecord.RowHeadersVisible = false;
            this.dataGridViewRecord.RowHeadersWidth = 22;
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle6.Padding = new System.Windows.Forms.Padding(3);
            this.dataGridViewRecord.RowsDefaultCellStyle = dataGridViewCellStyle6;
            this.dataGridViewRecord.RowTemplate.Height = 23;
            this.dataGridViewRecord.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewRecord.Size = new System.Drawing.Size(203, 142);
            this.dataGridViewRecord.TabIndex = 6;
            // 
            // num
            // 
            this.num.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.num.HeaderText = "序号";
            this.num.Name = "num";
            this.num.ReadOnly = true;
            this.num.Width = 50;
            // 
            // puId
            // 
            this.puId.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.puId.HeaderText = "设备ID";
            this.puId.Name = "puId";
            this.puId.ReadOnly = true;
            this.puId.Width = 72;
            // 
            // channelNo
            // 
            this.channelNo.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.channelNo.HeaderText = "通道号";
            this.channelNo.Name = "channelNo";
            this.channelNo.ReadOnly = true;
            this.channelNo.Width = 72;
            // 
            // reason
            // 
            this.reason.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.reason.HeaderText = "录像原因";
            this.reason.Name = "reason";
            this.reason.ReadOnly = true;
            this.reason.Width = 84;
            // 
            // beginTime
            // 
            this.beginTime.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.beginTime.HeaderText = "开始时间";
            this.beginTime.Name = "beginTime";
            this.beginTime.ReadOnly = true;
            this.beginTime.Width = 84;
            // 
            // endTime
            // 
            this.endTime.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.endTime.HeaderText = "结束时间";
            this.endTime.Name = "endTime";
            this.endTime.ReadOnly = true;
            this.endTime.Width = 84;
            // 
            // fileType
            // 
            this.fileType.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.fileType.HeaderText = "文件类型";
            this.fileType.Name = "fileType";
            this.fileType.ReadOnly = true;
            this.fileType.Width = 84;
            // 
            // fileSize
            // 
            this.fileSize.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.fileSize.HeaderText = "文件大小(MB)";
            this.fileSize.Name = "fileSize";
            this.fileSize.ReadOnly = true;
            this.fileSize.Width = 108;
            // 
            // downloadPercent
            // 
            this.downloadPercent.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            dataGridViewCellStyle5.Format = "N2";
            dataGridViewCellStyle5.NullValue = null;
            this.downloadPercent.DefaultCellStyle = dataGridViewCellStyle5;
            this.downloadPercent.HeaderText = "下载进度(%)";
            this.downloadPercent.Name = "downloadPercent";
            this.downloadPercent.ReadOnly = true;
            this.downloadPercent.Width = 102;
            // 
            // fileName
            // 
            this.fileName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.fileName.HeaderText = "文件名";
            this.fileName.Name = "fileName";
            this.fileName.ReadOnly = true;
            this.fileName.Width = 160;
            // 
            // filePath
            // 
            this.filePath.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.filePath.HeaderText = "存储路径";
            this.filePath.MinimumWidth = 140;
            this.filePath.Name = "filePath";
            this.filePath.ReadOnly = true;
            // 
            // description1
            // 
            this.description1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.description1.HeaderText = "描述1";
            this.description1.MinimumWidth = 80;
            this.description1.Name = "description1";
            this.description1.ReadOnly = true;
            this.description1.Width = 80;
            // 
            // description2
            // 
            this.description2.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.description2.HeaderText = "描述2";
            this.description2.MinimumWidth = 80;
            this.description2.Name = "description2";
            this.description2.ReadOnly = true;
            this.description2.Width = 80;
            // 
            // treeViewResList
            // 
            this.treeViewResList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.treeViewResList.ItemHeight = 16;
            this.treeViewResList.Location = new System.Drawing.Point(3, 246);
            this.treeViewResList.Name = "treeViewResList";
            this.treeViewResList.Size = new System.Drawing.Size(229, 260);
            this.treeViewResList.TabIndex = 0;
            this.treeViewResList.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeViewResList_NodeMouseClick);
            this.treeViewResList.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeViewResList_NodeMouseDoubleClick);
            // 
            // panelVideo
            // 
            this.panelVideo.BackColor = System.Drawing.Color.Black;
            this.panelVideo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelVideo.Location = new System.Drawing.Point(235, 0);
            this.panelVideo.Name = "panelVideo";
            this.panelVideo.Size = new System.Drawing.Size(556, 509);
            this.panelVideo.TabIndex = 1;
            // 
            // contextMenuStripVideo
            // 
            this.contextMenuStripVideo.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemClosePreview,
            this.toolStripMenuItemSnapshot,
            this.toolStripMenuItemRecord});
            this.contextMenuStripVideo.Name = "contextMenuStripVideo";
            this.contextMenuStripVideo.Size = new System.Drawing.Size(125, 70);
            // 
            // toolStripMenuItemClosePreview
            // 
            this.toolStripMenuItemClosePreview.Name = "toolStripMenuItemClosePreview";
            this.toolStripMenuItemClosePreview.Size = new System.Drawing.Size(124, 22);
            this.toolStripMenuItemClosePreview.Text = "关闭预览";
            this.toolStripMenuItemClosePreview.Click += new System.EventHandler(this.toolStripMenuItemClosePreview_Click);
            // 
            // toolStripMenuItemSnapshot
            // 
            this.toolStripMenuItemSnapshot.Name = "toolStripMenuItemSnapshot";
            this.toolStripMenuItemSnapshot.Size = new System.Drawing.Size(124, 22);
            this.toolStripMenuItemSnapshot.Text = "截图";
            this.toolStripMenuItemSnapshot.Click += new System.EventHandler(this.toolStripMenuItemSnapshot_Click);
            // 
            // toolStripMenuItemRecord
            // 
            this.toolStripMenuItemRecord.Name = "toolStripMenuItemRecord";
            this.toolStripMenuItemRecord.Size = new System.Drawing.Size(124, 22);
            this.toolStripMenuItemRecord.Text = "录像";
            this.toolStripMenuItemRecord.Click += new System.EventHandler(this.toolStripMenuItemRecord_Click);
            // 
            // contextMenuStripTalkOnly
            // 
            this.contextMenuStripTalkOnly.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemTalkOnly});
            this.contextMenuStripTalkOnly.Name = "contextMenuStripTalkOnly";
            this.contextMenuStripTalkOnly.Size = new System.Drawing.Size(125, 26);
            // 
            // ToolStripMenuItemTalkOnly
            // 
            this.ToolStripMenuItemTalkOnly.Name = "ToolStripMenuItemTalkOnly";
            this.ToolStripMenuItemTalkOnly.Size = new System.Drawing.Size(124, 22);
            this.ToolStripMenuItemTalkOnly.Text = "打开对讲";
            this.ToolStripMenuItemTalkOnly.Click += new System.EventHandler(this.ToolStripMenuItemTalkOnly_Click);
            // 
            // contextMenuStripDocRetrieval
            // 
            this.contextMenuStripDocRetrieval.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemDocRetrieval});
            this.contextMenuStripDocRetrieval.Name = "contextMenuStripDocRetrieval";
            this.contextMenuStripDocRetrieval.Size = new System.Drawing.Size(125, 26);
            // 
            // ToolStripMenuItemDocRetrieval
            // 
            this.ToolStripMenuItemDocRetrieval.Name = "ToolStripMenuItemDocRetrieval";
            this.ToolStripMenuItemDocRetrieval.Size = new System.Drawing.Size(124, 22);
            this.ToolStripMenuItemDocRetrieval.Text = "文件检索";
            this.ToolStripMenuItemDocRetrieval.Click += new System.EventHandler(this.ToolStripMenuItemDocRetrieval_Click);
            // 
            // contextMenuStripFileDown
            // 
            this.contextMenuStripFileDown.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemFileDown});
            this.contextMenuStripFileDown.Name = "contextMenuStripFileDown";
            this.contextMenuStripFileDown.Size = new System.Drawing.Size(153, 48);
            // 
            // ToolStripMenuItemFileDown
            // 
            this.ToolStripMenuItemFileDown.Name = "ToolStripMenuItemFileDown";
            this.ToolStripMenuItemFileDown.Size = new System.Drawing.Size(152, 22);
            this.ToolStripMenuItemFileDown.Text = "下载";
            this.ToolStripMenuItemFileDown.Click += new System.EventHandler(this.ToolStripMenuItemFileDown_Click);
            // 
            // MainWinForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(791, 509);
            this.Controls.Add(this.panelVideo);
            this.Controls.Add(this.panelLeft);
            this.Name = "MainWinForm";
            this.Text = "WinFormDemo";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Shown += new System.EventHandler(this.FormLogin_Show);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.panelLeft.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPTZSpeed)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.contextMenuStripGps.ResumeLayout(false);
            this.tabPageGetGps.ResumeLayout(false);
            this.tabPageDocRetrieval.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewRecord)).EndInit();
            this.contextMenuStripVideo.ResumeLayout(false);
            this.contextMenuStripTalkOnly.ResumeLayout(false);
            this.contextMenuStripDocRetrieval.ResumeLayout(false);
            this.contextMenuStripFileDown.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelLeft;
        private System.Windows.Forms.Panel panelVideo;
        private System.Windows.Forms.TreeView treeViewResList;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripVideo;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemClosePreview;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSnapshot;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemRecord;
        private System.Windows.Forms.Label labelRecordPath;
        private System.Windows.Forms.Label labelCapturePath;
        private System.Windows.Forms.Button buttonCapturePath;
        private System.Windows.Forms.Button buttonRecordPath;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button buttonPTZRightDown;
        private System.Windows.Forms.Button buttonPTZDown;
        private System.Windows.Forms.Button buttonPTZLeftDown;
        private System.Windows.Forms.Button buttonPTZRight;
        private System.Windows.Forms.Button buttonPTZLeft;
        private System.Windows.Forms.Button buttonPTZRightUp;
        private System.Windows.Forms.Button buttonPTZUp;
        private System.Windows.Forms.Button buttonPTZLeftUp;
        private System.Windows.Forms.NumericUpDown numericUpDownPTZSpeed;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonApertureDown;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonApertureUp;
        private System.Windows.Forms.Button buttonFocusDown;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonFocusUp;
        private System.Windows.Forms.Button buttonLensDown;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonLensUp;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Button buttonCruise;
        private System.Windows.Forms.ComboBox comboBoxCruiseLine;
        private System.Windows.Forms.Button buttonGotoPresetPoint;
        private System.Windows.Forms.ComboBox comboBoxPresetPoint;
        private System.Windows.Forms.Button buttonGetPtzAttr;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.ListView listViewGPSData;
        private System.Windows.Forms.ColumnHeader columnHeaderPuName;
        private System.Windows.Forms.ColumnHeader columnHeaderLat;
        private System.Windows.Forms.ColumnHeader columnHeaderLng;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripGps;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemCloseGpsDialog;
        private System.Windows.Forms.TextBox tbInputMsg;
        private System.Windows.Forms.Button btnSendMsg;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripTalkOnly;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemTalkOnly;
        private System.Windows.Forms.TabPage tabPageGetGps;
        private System.Windows.Forms.Button buttonGetGps;
        private System.Windows.Forms.TabPage tabPageDocRetrieval;
        public System.Windows.Forms.DataGridView dataGridViewRecord;
        private System.Windows.Forms.DataGridViewTextBoxColumn num;
        private System.Windows.Forms.DataGridViewTextBoxColumn puId;
        private System.Windows.Forms.DataGridViewTextBoxColumn channelNo;
        private System.Windows.Forms.DataGridViewTextBoxColumn reason;
        private System.Windows.Forms.DataGridViewTextBoxColumn beginTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn endTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn fileType;
        private System.Windows.Forms.DataGridViewTextBoxColumn fileSize;
        private System.Windows.Forms.DataGridViewTextBoxColumn downloadPercent;
        private System.Windows.Forms.DataGridViewTextBoxColumn fileName;
        private System.Windows.Forms.DataGridViewTextBoxColumn filePath;
        private System.Windows.Forms.DataGridViewTextBoxColumn description1;
        private System.Windows.Forms.DataGridViewTextBoxColumn description2;
        private System.Windows.Forms.DateTimePicker dateTimePickerBegin;
        private System.Windows.Forms.DateTimePicker dateTimePickerEnd;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripDocRetrieval;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemDocRetrieval;
        private System.Windows.Forms.Button buttonSearchFile;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripFileDown;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemFileDown;
    }
}

