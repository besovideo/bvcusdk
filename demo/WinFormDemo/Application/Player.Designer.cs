namespace WindowsFormsTest
{
    partial class Player
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
            this.tbPlay = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // tbPlay
            // 
            this.tbPlay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbPlay.Location = new System.Drawing.Point(0, 0);
            this.tbPlay.Multiline = true;
            this.tbPlay.Name = "tbPlay";
            this.tbPlay.Size = new System.Drawing.Size(259, 260);
            this.tbPlay.TabIndex = 2;
            // 
            // Player
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(259, 260);
            this.Controls.Add(this.tbPlay);
            this.Name = "Player";
            this.Text = "Player";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.TextBox tbPlay;


    }
}