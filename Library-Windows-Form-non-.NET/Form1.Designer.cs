namespace LibraryProject
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.searchbutton = new System.Windows.Forms.Button();
            this.clearall = new System.Windows.Forms.Button();
            this.textbookname = new System.Windows.Forms.TextBox();
            this.textbookauthor = new System.Windows.Forms.TextBox();
            this.textbooksubject = new System.Windows.Forms.TextBox();
            this.combobookyear = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.yearradio = new System.Windows.Forms.RadioButton();
            this.subjectradio = new System.Windows.Forms.RadioButton();
            this.authorradio = new System.Windows.Forms.RadioButton();
            this.bookradio = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button2 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel3 = new System.Windows.Forms.Panel();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.SystemColors.HotTrack;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label1.Size = new System.Drawing.Size(800, 23);
            this.label1.TabIndex = 0;
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Red;
            this.button1.ForeColor = System.Drawing.Color.White;
            this.button1.Location = new System.Drawing.Point(0, 0);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(35, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "x";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Controls.Add(this.tabPage6);
            this.tabControl1.Controls.Add(this.tabPage7);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 23);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tabControl1.RightToLeftLayout = true;
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(800, 427);
            this.tabControl1.TabIndex = 2;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panel2);
            this.tabPage1.Controls.Add(this.groupBox2);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 24);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(792, 399);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "جستجو";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Click += new System.EventHandler(this.tabPage1_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.searchbutton);
            this.panel2.Controls.Add(this.clearall);
            this.panel2.Controls.Add(this.textbookname);
            this.panel2.Controls.Add(this.textbookauthor);
            this.panel2.Controls.Add(this.textbooksubject);
            this.panel2.Controls.Add(this.combobookyear);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.label4);
            this.panel2.Location = new System.Drawing.Point(3, 136);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(783, 257);
            this.panel2.TabIndex = 4;
            this.panel2.Paint += new System.Windows.Forms.PaintEventHandler(this.panel2_Paint);
            // 
            // searchbutton
            // 
            this.searchbutton.Location = new System.Drawing.Point(238, 180);
            this.searchbutton.Name = "searchbutton";
            this.searchbutton.Size = new System.Drawing.Size(75, 23);
            this.searchbutton.TabIndex = 10;
            this.searchbutton.Text = "جستجو کن";
            this.searchbutton.UseVisualStyleBackColor = true;
            this.searchbutton.Click += new System.EventHandler(this.searchbutton_Click);
            // 
            // clearall
            // 
            this.clearall.Location = new System.Drawing.Point(115, 179);
            this.clearall.Name = "clearall";
            this.clearall.Size = new System.Drawing.Size(75, 23);
            this.clearall.TabIndex = 9;
            this.clearall.Text = "پاک";
            this.clearall.UseVisualStyleBackColor = true;
            this.clearall.Click += new System.EventHandler(this.clearall_Click);
            // 
            // textbookname
            // 
            this.textbookname.Location = new System.Drawing.Point(277, 20);
            this.textbookname.Name = "textbookname";
            this.textbookname.Size = new System.Drawing.Size(405, 23);
            this.textbookname.TabIndex = 8;
            this.textbookname.TextChanged += new System.EventHandler(this.textbookname_TextChanged);
            this.textbookname.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textbookname_KeyDown);
            this.textbookname.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.textbookname_PreviewKeyDown);
            // 
            // textbookauthor
            // 
            this.textbookauthor.Location = new System.Drawing.Point(277, 71);
            this.textbookauthor.Name = "textbookauthor";
            this.textbookauthor.Size = new System.Drawing.Size(405, 23);
            this.textbookauthor.TabIndex = 7;
            this.textbookauthor.TextChanged += new System.EventHandler(this.textbookauthor_TextChanged);
            // 
            // textbooksubject
            // 
            this.textbooksubject.Location = new System.Drawing.Point(396, 126);
            this.textbooksubject.Name = "textbooksubject";
            this.textbooksubject.Size = new System.Drawing.Size(286, 23);
            this.textbooksubject.TabIndex = 6;
            this.textbooksubject.TextChanged += new System.EventHandler(this.textbooksubject_TextChanged);
            // 
            // combobookyear
            // 
            this.combobookyear.FormattingEnabled = true;
            this.combobookyear.Location = new System.Drawing.Point(479, 180);
            this.combobookyear.Name = "combobookyear";
            this.combobookyear.Size = new System.Drawing.Size(203, 23);
            this.combobookyear.TabIndex = 5;
            this.combobookyear.SelectedIndexChanged += new System.EventHandler(this.combobookyear_SelectedIndexChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(704, 183);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(52, 15);
            this.label7.TabIndex = 4;
            this.label7.Text = "سال چاپ";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(711, 126);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(45, 15);
            this.label6.TabIndex = 3;
            this.label6.Text = "موضوع";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(694, 74);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 15);
            this.label5.TabIndex = 2;
            this.label5.Text = "نام نویسنده";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(711, 28);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(48, 15);
            this.label4.TabIndex = 1;
            this.label4.Text = "نام کتاب";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.yearradio);
            this.groupBox2.Controls.Add(this.subjectradio);
            this.groupBox2.Controls.Add(this.authorradio);
            this.groupBox2.Controls.Add(this.bookradio);
            this.groupBox2.Location = new System.Drawing.Point(3, 61);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(783, 69);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "جستجو بر اساس";
            this.groupBox2.Enter += new System.EventHandler(this.groupBox2_Enter);
            // 
            // yearradio
            // 
            this.yearradio.AutoSize = true;
            this.yearradio.Location = new System.Drawing.Point(277, 34);
            this.yearradio.Name = "yearradio";
            this.yearradio.Size = new System.Drawing.Size(70, 19);
            this.yearradio.TabIndex = 3;
            this.yearradio.TabStop = true;
            this.yearradio.Text = "سال چاپ";
            this.yearradio.UseVisualStyleBackColor = true;
            this.yearradio.CheckedChanged += new System.EventHandler(this.yearradio_CheckedChanged);
            // 
            // subjectradio
            // 
            this.subjectradio.AutoSize = true;
            this.subjectradio.Location = new System.Drawing.Point(447, 34);
            this.subjectradio.Name = "subjectradio";
            this.subjectradio.Size = new System.Drawing.Size(63, 19);
            this.subjectradio.TabIndex = 2;
            this.subjectradio.TabStop = true;
            this.subjectradio.Text = "موضوع";
            this.subjectradio.UseVisualStyleBackColor = true;
            this.subjectradio.CheckedChanged += new System.EventHandler(this.subjectradio_CheckedChanged);
            // 
            // authorradio
            // 
            this.authorradio.AutoSize = true;
            this.authorradio.Location = new System.Drawing.Point(561, 34);
            this.authorradio.Name = "authorradio";
            this.authorradio.Size = new System.Drawing.Size(71, 19);
            this.authorradio.TabIndex = 1;
            this.authorradio.TabStop = true;
            this.authorradio.Text = "نام مولف";
            this.authorradio.UseVisualStyleBackColor = true;
            this.authorradio.CheckedChanged += new System.EventHandler(this.authorradio_CheckedChanged);
            // 
            // bookradio
            // 
            this.bookradio.AutoSize = true;
            this.bookradio.Location = new System.Drawing.Point(673, 34);
            this.bookradio.Name = "bookradio";
            this.bookradio.Size = new System.Drawing.Size(66, 19);
            this.bookradio.TabIndex = 0;
            this.bookradio.TabStop = true;
            this.bookradio.Text = "نام کتاب";
            this.bookradio.UseVisualStyleBackColor = true;
            this.bookradio.CheckedChanged += new System.EventHandler(this.bookradio_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(786, 52);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new System.Drawing.Point(3, 58);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(783, 100);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "جستجو بر اساس";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(5, 9);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "ورود";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.Red;
            this.label3.Location = new System.Drawing.Point(690, 13);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 15);
            this.label3.TabIndex = 1;
            this.label3.Text = "مهمان";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(734, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 15);
            this.label2.TabIndex = 0;
            this.label2.Text = "نام کاربر";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.panel3);
            this.tabPage2.Location = new System.Drawing.Point(4, 24);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(792, 399);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "نمایش کتاب";
            this.tabPage2.UseVisualStyleBackColor = true;
            this.tabPage2.Click += new System.EventHandler(this.tabPage2_Click);
            // 
            // panel3
            // 
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(3, 3);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(786, 393);
            this.panel3.TabIndex = 0;
            this.panel3.Paint += new System.Windows.Forms.PaintEventHandler(this.panel3_Paint);
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 24);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(792, 399);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "نمایش اعضا";
            this.tabPage3.UseVisualStyleBackColor = true;
            this.tabPage3.Click += new System.EventHandler(this.tabPage3_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Location = new System.Drawing.Point(4, 24);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(792, 399);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "ورود کتاب";
            this.tabPage4.UseVisualStyleBackColor = true;
            this.tabPage4.Click += new System.EventHandler(this.tabPage4_Click);
            // 
            // tabPage5
            // 
            this.tabPage5.Location = new System.Drawing.Point(4, 24);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Size = new System.Drawing.Size(792, 399);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "ورود اعضا";
            this.tabPage5.UseVisualStyleBackColor = true;
            this.tabPage5.Click += new System.EventHandler(this.tabPage5_Click);
            // 
            // tabPage6
            // 
            this.tabPage6.Location = new System.Drawing.Point(4, 24);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Size = new System.Drawing.Size(792, 399);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "امانت";
            this.tabPage6.UseVisualStyleBackColor = true;
            this.tabPage6.Click += new System.EventHandler(this.tabPage6_Click);
            // 
            // tabPage7
            // 
            this.tabPage7.Location = new System.Drawing.Point(4, 24);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Size = new System.Drawing.Size(792, 399);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "برگشت امانت";
            this.tabPage7.UseVisualStyleBackColor = true;
            this.tabPage7.Click += new System.EventHandler(this.tabPage7_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Label label1;
        private Button button1;
        private TabControl tabControl1;
        private TabPage tabPage1;
        private Panel panel1;
        private TabPage tabPage2;
        private TabPage tabPage3;
        private TabPage tabPage4;
        private TabPage tabPage5;
        private TabPage tabPage6;
        private TabPage tabPage7;
        private Label label3;
        private Label label2;
        private GroupBox groupBox1;
        private Button button2;
        private GroupBox groupBox2;
        private RadioButton yearradio;
        private RadioButton subjectradio;
        private RadioButton authorradio;
        private RadioButton bookradio;
        private Panel panel2;
        private Button searchbutton;
        private Button clearall;
        private TextBox textbookname;
        private TextBox textbookauthor;
        private TextBox textbooksubject;
        private ComboBox combobookyear;
        private Label label7;
        private Label label6;
        private Label label5;
        private Label label4;
        private Panel panel3;
    }
}