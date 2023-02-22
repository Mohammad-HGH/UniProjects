using Librarydotnet.library_dbDataSetTableAdapters;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Librarydotnet
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        string path = "", path1="";
        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'library_dbDataSet.user' table. You can move, or remove it, as needed.
            this.userTableAdapter.Fill(this.library_dbDataSet.user);
            // TODO: This line of code loads data into the 'library_dbDataSet.amanat' table. You can move, or remove it, as needed.
            this.amanatTableAdapter.Fill(this.library_dbDataSet.amanat);
            // TODO: This line of code loads data into the 'library_dbDataSet.member' table. You can move, or remove it, as needed.
            this.memberTableAdapter.Fill(this.library_dbDataSet.member);
            // TODO: This line of code loads data into the 'library_dbDataSet.book' table. You can move, or remove it, as needed.
            this.bookTableAdapter.Fill(this.library_dbDataSet.book);
            label1.Text = "برنامه جامع کتابخانه" + "    "+ DateTime.Now.ToString();
            for(int i =1353;i<1401;i++)
                yearCombo.Items.Add(i);
            searchBtn.Enabled = false;
            bookRadio.Checked = true;
            bookInput.Enabled = true;
            yearCombo.Enabled= false;
            panel3.Enabled = false;
            panel4.Visible = true;
           
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void bookRadio_CheckedChanged(object sender, EventArgs e)
        {
  
            bookInput.Enabled = true;
            authorInput.Enabled = false;
            subjectInput.Enabled = false;
            yearCombo.Enabled = false;
        }
        private void authorRadio_CheckedChanged(object sender, EventArgs e)
        {
            authorInput.Enabled = true;
            bookInput.Enabled = false;
            subjectInput.Enabled = false;
            yearCombo.Enabled = false;
        }

        private void subjectRadio_CheckedChanged(object sender, EventArgs e)
        {
            subjectInput.Enabled = true;
            authorInput.Enabled = false;
            bookInput.Enabled = false;
            yearCombo.Enabled = false;
        }

        private void yearRadio_CheckedChanged(object sender, EventArgs e)
        {
            yearCombo.Enabled=true;
            authorInput.Enabled = false;
            bookInput.Enabled = false;
            subjectInput.Enabled = false;
        }
        private void authorInput_TextChanged(object sender, EventArgs e)
        {
            if (authorInput.TextLength > 0)
            {
                searchBtn.Enabled = true;
            }
        }

        private void bookInput_TextChanged(object sender, EventArgs e)
        {
            if (bookInput.TextLength > 0)
            {
                searchBtn.Enabled = true;
            }
        }

        private void subjectInput_TextChanged(object sender, EventArgs e)
        {
            if (subjectInput.TextLength > 0)
            {
                searchBtn.Enabled = true;
            }
        }

        private void yearCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (yearCombo.SelectedIndex>= 0)
            {
                searchBtn.Enabled = true;
            }
        }

        private void authorInput_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(char.IsLetter((char)e.KeyCode) || e.KeyCode == Keys.Back || e.KeyCode == Keys.Delete || e.KeyCode == Keys.Right || e.KeyCode == Keys.Left))
                e.SuppressKeyPress = true;
        }

        private void clearBtn_Click(object sender, EventArgs e)
        {
            bookInput.Clear();
            subjectInput.Clear();
            authorInput.Clear();
            
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            bookBindingSource.MoveLast();
            int index = dataGridView1.CurrentCell.RowIndex;
            pictureBox1.ImageLocation = dataGridView1.Rows[index].Cells[8].Value.ToString();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            bookBindingSource.MoveNext();
            int index = dataGridView1.CurrentCell.RowIndex;
            pictureBox1.ImageLocation = dataGridView1.Rows[index].Cells[8].Value.ToString();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            bookBindingSource.MovePrevious();
            int index = dataGridView1.CurrentCell.RowIndex;
            pictureBox1.ImageLocation = dataGridView1.Rows[index].Cells[8].Value.ToString();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            bookBindingSource.MoveFirst();
            int index = dataGridView1.CurrentCell.RowIndex;
            pictureBox1.ImageLocation = dataGridView1.Rows[index].Cells[8].Value.ToString();
        }

        private void searchBtn_Click(object sender, EventArgs e)
        {
            tabControl1.SelectedTab = tabPage2;
            panel3.Enabled = true;
            if (bookRadio.Checked) {
                if (bookInput.TextLength>0)
                {
                    bookTableAdapter.FillBybookname(library_dbDataSet.book,bookInput.Text);
                    bookTableAdapter.Fill(library_dbDataSet.book);
                }
                else
                {
                    MessageBox.Show("نام کتاب ذکر نشده است!");
                }
            }
            if (authorRadio.Checked) {
                if (authorInput.TextLength > 0)
                {
                    bookTableAdapter.FillByauthor(library_dbDataSet.book, authorInput.Text);
                    bookTableAdapter.Fill(library_dbDataSet.book);
                }
                else
                {
                    MessageBox.Show("نام نویسنده ذکر نشده است!");
                }

            }
            if (subjectRadio.Checked) {
                if (subjectInput.TextLength > 0)
                {
                    bookTableAdapter.FillBysubject(library_dbDataSet.book, subjectInput.Text);
                    bookTableAdapter.Fill(library_dbDataSet.book);
                }
                else
                {
                    MessageBox.Show("موضوع ذکر نشده است!");
                }
            }
            
            if (yearRadio.Checked) {
                if (yearCombo.SelectedIndex>=0)
                {
                    bookTableAdapter.FillBychapyear(library_dbDataSet.book, yearCombo.Text);
                }
                else
                {
                    MessageBox.Show("سال چاپ انتخاب نشده است!");
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            loginForm frmLogin = new loginForm();
            frmLogin.ShowDialog();
            string[] info;
            info = File.ReadAllLines("log.txt");
            string fName=info[info.Length-1];
            int n = fName.Length;
            int m = fName.IndexOf("\t",0);
            label3.Text= fName.Remove(m, n - m);
            if (label3.Text == "admin")
            {
                // tabControl1.SelectedTab = tabPage8;
                pBoss.Visible = true;
            }
            else
            {
                pBoss.Visible = false;
            }
            panel4.Visible = true;
            panel5.Visible = true;
            panel6.Visible = true;
            panel7.Visible = true;

            for (int i = 1353; i < 1401; i++)
                yearCombo1.Items.Add(i);

            bookTableAdapter.FillByall(library_dbDataSet.book);
            int c = library_dbDataSet.book.Rows.Count;

            if (c == 0)
                codeInput.Text = "1";
            else
                codeInput.Text = (int.Parse(library_dbDataSet.book.Rows[c-1]["bookid"].ToString()) + 1).ToString();



            memberTableAdapter.FillByall(library_dbDataSet.member);
            int count = library_dbDataSet.member.Rows.Count;
            if (c > 0)
            {
                userCode.Text = (int.Parse(library_dbDataSet.member.Rows[count - 1]["memberid"].ToString()) + 1).ToString();
            }


            amanatTableAdapter.FillByall(library_dbDataSet.amanat);
            count=library_dbDataSet.amanat.Rows.Count;
            if (count > 0)
            {
                label31.Text = (int.Parse(library_dbDataSet.amanat.Rows[count - 1]["amanatid"].ToString()) + 1).ToString();
            }

            label32.Text=DateTime.Now.ToLongDateString();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            string pathname="";
            if(folderBrowserDialog1.ShowDialog()==DialogResult.OK)
            {
                pathname=folderBrowserDialog1.SelectedPath+"\\";
                File.WriteAllText("pathname.txt",pathname);
            }
        }

        private void panel4_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tabPage7_Click(object sender, EventArgs e)
        {

        }

        private void panel6_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void tabPage2_Click(object sender, EventArgs e)
        {

        }

        private void panel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void bookBindingSource1_CurrentChanged(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label14_Click(object sender, EventArgs e)
        {

        }

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void tabPage3_Click(object sender, EventArgs e)
        {

        }

        private void panel8_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tabPage4_Click(object sender, EventArgs e)
        {

        }   

        private void submitBtn_Click(object sender, EventArgs e)
        {
            if (codeInput.TextLength>0&&nameInput1.TextLength>0&&authorInput1.TextLength>0&& motarjemInput1.TextLength>0&&entesharatInput1.TextLength>0&& subjectInput1.TextLength>0&& isbnInput1.TextLength>0&&yearCombo1.Text!="")
            {
                bookTableAdapter.InsertQuery(int.Parse(codeInput.Text),nameInput1.Text,authorInput1.Text,motarjemInput1.Text,entesharatInput1.Text,yearCombo1.Text,subjectInput1.Text,isbnInput1.Text,path);
                bookTableAdapter.Fill(library_dbDataSet.book);
                MessageBox.Show("ثبت با موفقیت انجام شد!");
                codeInput.Text = int.Parse(codeInput.Text).ToString();
                nameInput1.Clear();
                authorInput1.Clear();
                motarjemInput1.Clear();
                entesharatInput1.Clear();
                subjectInput1.Clear();
                isbnInput1.Clear();
                path = "";
            }
            else
            {
                MessageBox.Show("یکی از ورودی ها پر نشده است!");
            }
        }

        private void clearBtn1_Click(object sender, EventArgs e)
        {

        }

        private void button7_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "image files(*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png";
            if(openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                bookPicture.ImageLocation = openFileDialog1.FileName;
                path = openFileDialog1.FileName;
            }
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void yearCombo1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label22_Click(object sender, EventArgs e)
        {

        }

        private void isbnInput1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label21_Click(object sender, EventArgs e)
        {

        }

        private void subjectInput1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label20_Click(object sender, EventArgs e)
        {

        }

        private void entesharatInput1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label19_Click(object sender, EventArgs e)
        {

        }

        private void motarjemInput1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label18_Click(object sender, EventArgs e)
        {

        }

        private void authorInput1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label17_Click(object sender, EventArgs e)
        {

        }

        private void nameInput_TextChanged(object sender, EventArgs e)
        {

        }

        private void label16_Click(object sender, EventArgs e)
        {

        }

        private void codeInput_TextChanged(object sender, EventArgs e)
        {

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void dataGridView2_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void bookBindingSource2_CurrentChanged(object sender, EventArgs e)
        {

        }

        private void tabPage5_Click(object sender, EventArgs e)
        {

        }

        private void panel5_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tabPage6_Click(object sender, EventArgs e)
        {

        }

        private void panel7_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel4_Paint_1(object sender, PaintEventArgs e)
        {

        }

        private void tabPage8_Click(object sender, EventArgs e)
        {

        }

        private void pBoss_Paint(object sender, PaintEventArgs e)
        {

        }

        private void bookBindingSource_CurrentChanged(object sender, EventArgs e)
        {

        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }

        private void button10_Click(object sender, EventArgs e)
        {
            openFileDialog2.Filter = "image files(*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png";
            if (openFileDialog2.ShowDialog() == DialogResult.OK)
            {
                pictureBox2.ImageLocation = openFileDialog2.FileName;
                path1 = openFileDialog2.FileName;
            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (textBox8.TextLength > 0) {
                memberTableAdapter.FillBycode(library_dbDataSet.member, int.Parse(textBox8.Text));
                if(library_dbDataSet.member.Rows.Count > 0)
                {
                    label35.Text = library_dbDataSet.member.Rows[0]["membername"].ToString() + " " + library_dbDataSet.member.Rows[0]["memberfamily"].ToString();
                    label36.Text = library_dbDataSet.member.Rows[0]["membergrade"].ToString();
                    label38.Text = library_dbDataSet.member.Rows[0]["memberreshte"].ToString();
                    textBox9.Focus();
                }
                else
                {
                    MessageBox.Show("عضو با این کد وجود نداره!");
                }
            } else
            {
                MessageBox.Show("کد عضو مشخص نیست!");
                textBox9.Focus();
            }
        }

        private void textBox8_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode==Keys.Enter)
            {
                if (textBox8.TextLength > 0)
                {
                    memberTableAdapter.FillBycode(library_dbDataSet.member, int.Parse(textBox8.Text));
                    if (library_dbDataSet.member.Rows.Count > 0)
                    {
                        label35.Text = library_dbDataSet.member.Rows[0]["membername"].ToString() + " " + library_dbDataSet.member.Rows[0]["memberfamily"].ToString();
                        label36.Text = library_dbDataSet.member.Rows[0]["membergrade"].ToString();
                        label38.Text = library_dbDataSet.member.Rows[0]["memberreshte"].ToString();
                        textBox9.Focus();
                    }
                    else
                    {
                        MessageBox.Show("عضو با این کد وجود نداره!");
                    }
                }
                else
                {
                    MessageBox.Show("کد عضو مشخص نیست!");
                    textBox9.Focus();
                }
            }
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (textBox9.TextLength > 0)
            {
                bookTableAdapter.FillBybookcode(library_dbDataSet.book, int.Parse(textBox9.Text));
                if (library_dbDataSet.book.Rows.Count>0)
                {
                    label41.Text = library_dbDataSet.book.Rows[0]["bookname"].ToString();
                    label42.Text = library_dbDataSet.book.Rows[0]["author"].ToString();
                    label44.Text = library_dbDataSet.book.Rows[0]["subject"].ToString();
                }
                else
                {
                    MessageBox.Show("کتاب با این کد وجود نداره!");
                }
            }
            else
            {
                MessageBox.Show("کد کتاب مشخص نیست!");
            }
        }

        private void textBox9_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (textBox9.TextLength > 0)
                {
                    bookTableAdapter.FillBybookcode(library_dbDataSet.book, int.Parse(textBox9.Text));
                    if (library_dbDataSet.book.Rows.Count > 0)
                    {
                        label41.Text = library_dbDataSet.book.Rows[0]["bookname"].ToString();
                        label42.Text = library_dbDataSet.book.Rows[0]["author"].ToString();
                        label44.Text = library_dbDataSet.book.Rows[0]["subject"].ToString();
                    }
                    else
                    {
                        MessageBox.Show("کتاب با این کد وجود نداره!");
                    }
                }
                else
                {
                    MessageBox.Show("کد کتاب مشخص نیست!");
                }
            }
        }

        private void button13_Click(object sender, EventArgs e)
        {
                int n = dataGridView4.Rows.Count - 1;
                bool flag = false;
                for(int i = 0; i < n; i++)
                {
                    if (dataGridView4.Rows[i].Cells[0].Value.ToString()== textBox9.Text) {
                        flag = true;
                        break;
                    }
                }


                if (!flag)
                {
                    dataGridView4.Rows.Add();
                    dataGridView4.Rows[n].Cells[0].Value = textBox9.Text;
                    dataGridView4.Rows[n].Cells[1].Value = label41.Text;
                    dataGridView4.Rows[n].Cells[2].Value = label42.Text;
                    dataGridView4.Rows[n].Cells[3].Value = label44.Text;
                    label47.Text = (n + 1).ToString();
                    label35.Text = "";
                    label36.Text = "";
                    label38.Text = "";
                    label41.Text = "";
                    label42.Text = "";
                    label44.Text = "";
                    textBox9.Clear();
                }
                else
                {
                    MessageBox.Show("کتاب قبلا وارد شده است!");
                    textBox9.Clear();
                    textBox8.Clear();
                }
          



        }

        private void button14_Click(object sender, EventArgs e)
        {
            if (textBox8.TextLength>0&&dataGridView4.Rows.Count>0)
            {
                userTableAdapter.FillByusername(library_dbDataSet.user, label3.Text);
                int Id = int.Parse(library_dbDataSet.user.Rows[0]["userid"].ToString());
                int n = dataGridView4.Rows.Count - 1;
                int c = int.Parse(label31.Text);
                for (int i= 0; i < n; i++)
                {
                    amanatTableAdapter.InsertQuerywithoutreturn(c, Id, int.Parse(textBox8.Text), int.Parse(dataGridView4.Rows[i].Cells[0].Value.ToString()),DateTime.Now);
                    c++;
                }
                MessageBox.Show("ثبت انجام شد!");
                for(int i=0; i < n; i++)
                    dataGridView4.Rows.RemoveAt(0);

                textBox8.Clear();
                label31.Text = c.ToString();
                textBox9.Focus();
            }
            else
            {
                MessageBox.Show("کتاب یا عضو مشخص نشده است!");
            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if(userCode.TextLength>0&&familyName.TextLength>0&&reshtehCombo.SelectedIndex>=0&& payeCombo.SelectedIndex>=0&& userAddress.TextLength > 0&&path1!="")
            {
                memberTableAdapter.InsertQuery(int.Parse(userCode.Text), userName.Text,familyName.Text,payeCombo.Text,reshtehCombo.Text,userAddress.Text,path1);
                memberTableAdapter.Fill(library_dbDataSet.member);
                MessageBox.Show("اطلاعات عضو جدید ثبت شد!");
                userCode.Text = (int.Parse(userCode.Text)+1).ToString();
                userName.Clear();
                familyName.Clear();
                userAddress.Clear();
                path1 = "";

            }
            else
            {
                MessageBox.Show("یکی از موارد خواسته شده وارد نشده است!");
            }
        }
    }
}
