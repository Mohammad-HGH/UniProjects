namespace LibraryProject
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            label1.Text = "برنامه کتابخانه" + DateTime.Now.ToString();
            for(int i=1353;i<=1401;i+=5)
                combobookyear.Items.Add(i);
            searchbutton.Enabled = false;
            bookradio.Checked = true;
            textbookauthor.Enabled = false;
            textbooksubject.Enabled = false;
            combobookyear.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void bookradio_CheckedChanged(object sender, EventArgs e)
        {
            textbookname.Enabled = true;
            textbookauthor.Enabled = false;
            textbooksubject.Enabled = false;
            combobookyear.Enabled = false;
        }

        private void authorradio_CheckedChanged(object sender, EventArgs e)
        {
            textbookauthor.Enabled = true;
            textbookname.Enabled = false;
            textbooksubject.Enabled = false;
            combobookyear.Enabled = false;
        }

        private void subjectradio_CheckedChanged(object sender, EventArgs e)
        {
            textbooksubject.Enabled=true;
            textbookauthor.Enabled=false;
            textbookname.Enabled=false;
            combobookyear.Enabled = false;
        }

        private void yearradio_CheckedChanged(object sender, EventArgs e)
        {
            combobookyear.Enabled = true;
            textbookauthor.Enabled = false;
            textbookname.Enabled = false;
            textbooksubject.Enabled = false;
        }

        private void textbookname_TextChanged(object sender, EventArgs e)
        {
            if(textbookname.TextLength>0)
                searchbutton.Enabled=true;
        }

        private void textbookauthor_TextChanged(object sender, EventArgs e)
        {
            if (textbookauthor.TextLength > 0)
                searchbutton.Enabled = true;
        }

        private void textbooksubject_TextChanged(object sender, EventArgs e)
        {
            if (textbooksubject.TextLength > 0)
                searchbutton.Enabled = true;
        }

        private void combobookyear_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (combobookyear.SelectedIndex >= 0)
                searchbutton.Enabled = true;
        }

        private void clearall_Click(object sender, EventArgs e)
        {
            textbookauthor.Clear();
            textbookname.Clear();
            textbooksubject.Clear();
        }

        private void textbookname_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {

        }

        private void textbookname_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(char.IsLetter((char)e.KeyCode) || e.KeyCode==Keys.Back || e.KeyCode==Keys.Delete|| e.KeyCode == Keys.Right|| e.KeyCode == Keys.Left))
            {
                e.SuppressKeyPress = true;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void searchbutton_Click(object sender, EventArgs e)
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

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
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

        private void tabPage3_Click(object sender, EventArgs e)
        {

        }

        private void tabPage4_Click(object sender, EventArgs e)
        {

        }

        private void tabPage5_Click(object sender, EventArgs e)
        {

        }

        private void tabPage6_Click(object sender, EventArgs e)
        {

        }

        private void tabPage7_Click(object sender, EventArgs e)
        {

        }
    }
}