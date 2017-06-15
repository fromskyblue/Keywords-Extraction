using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GDITest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics; //创建画板,这里的画板是由Form提供的.
            Pen p = new Pen(Color.Black, 2);//定义了一个蓝色,宽度为的画笔
            g.DrawLine(p, 20, 10, 20, 110);//在画板上画直线,起始坐标为(10,10),终点坐标为(100,100)
            g.DrawRectangle(p, 10, 10, 100, 100);//在画板上画矩形,起始坐标为(10,10),宽为,高为
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int[] d = { 1, 2, 1, 2, 3, 4, 5, 50, 2, 1, 3, 1, 2, 3, 2, 3, 100, 2, 1, 3, 1, 4, 3, 2, 1, 2, 1, 1, 1 };
            int max = 207;
            Graphics g = this.CreateGraphics();
            Pen p = new Pen(Color.Black, 2);
            g.DrawRectangle(p, 20, 220, 740, 100);
            int length = 0;
            foreach (int a in d)
            {
                length += a;
                float x = (float)(20 + 1.0 * length * 740 / max);
                g.DrawLine(p, x, 220, x, 320);
            }
        }
    }
}
