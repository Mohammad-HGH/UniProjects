using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Librarydotnet
{
    public partial class loginForm : Form
    {
        public loginForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode==Keys.Enter)
                txtPassword.Focus();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
                txtPassword.PasswordChar = '\0';
            else
                txtPassword.PasswordChar = '*';
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if(txtUsername.TextLength>0 && txtPassword.TextLength > 0)
            {
                userTableAdapter.FillByname(library_dbDataSet.user, txtUsername.Text, txtPassword.Text);
                if (library_dbDataSet.user.Rows.Count > 0)
                {
                    string fName = library_dbDataSet.user.Rows[0]["username"].ToString();
                    string level = library_dbDataSet.user.Rows[0]["userlevel"].ToString();
                    string pathname="";
                    if(File.Exists("pathname.txt"))
                    {
                        pathname = File.ReadAllText("pathname.txt");
                    }

                   
                    if (File.Exists(pathname+"\\"+"log.txt"))
                        File.AppendAllText(pathname + "\\" + "log.txt", "\r \n" + fName + "\t" + level + "\t" + DateTime.Now);
                    else
                        File.WriteAllText(pathname + "\\" + "log.txt", "\r \n" + fName + "\t" + level + "\t" + DateTime.Now);
                    this.Close();
                }
                else
                {
                    MessageBox.Show("کاربر یافت نشد!");
                    txtUsername.Clear();
                    txtPassword.Clear();
                    txtUsername.Focus();
                }
            }
            else
            {
                MessageBox.Show("نام کاربری یا گذرواژه وارد نشده!");
                txtUsername.Focus();
            }
        }

        private void loginForm_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'library_dbDataSet.user' table. You can move, or remove it, as needed.
            this.userTableAdapter.Fill(this.library_dbDataSet.user);

        }
    }
}
