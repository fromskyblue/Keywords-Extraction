using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using KeywordExtractionAPI;
using System.Text.RegularExpressions;

namespace KeywordExtraction
{
    public partial class KeywordExtractionForm : Form
    {
        public KeywordExtractionForm()
        {
            InitializeComponent();
        }

        private void KeywordExtractionForm_Load(object sender, EventArgs e)
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            SaveButton.Enabled = false;
            StandardizationButton.Enabled = false;
            ParticipleButton.Enabled = false;
            RemoveStopButton.Enabled = false;
            KeywordExtractionMaxButton.Enabled = false;
            KeywordExtractionNormalButton.Enabled = false;
            StatusLabel.Text = "欢迎使用基于熵差的关键词提取软件。";
            MessageBox.Show("对文档关键词的提取过程：\n1.打开文档: 点击\"打开文件\"按钮，选择目标文档。（文件应为\"*.txt\"文件）\n2.文档标准化: 点击\"文档标准化\"按钮，去除文档中的标点符号、换行符等无用的符号，并将全文的字母替换为小写字母。（如果上述操作均已完成，可以跳过此步。）\n3.中文分词: 点击\"中文分词\"按钮，对中文文本进行分词。（如果为非中文文本，可以跳过此步。）\n4.去除停用词: 点击\"去除停用词\"按钮，按照停用词列表，去除文档中的停用词。（此步骤可选，去除停用词后，可能会提高提取关键词的准确度。）\n5(1).关键词提取（一般熵）: 点击\"关键词提取（一般熵）\"按钮，提取关键词。\n5(2).关键词提取（最大熵）: 点击\"关键词提取（最大熵）\"按钮，按照最大熵的方法，提取关键词。", "帮助");
        }

        private void SelectFileButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                PathTextBox.Text = ofd.FileName;
                Encoding encode = Encoding.GetEncoding("GB2312");
                MyData.TheDoc = File.ReadAllText(ofd.FileName, encode);
                TextRichTextBox.Text = MyData.TheDoc;
                SaveButton.Enabled = false;
                ResultListView.Items.Clear();
                StandardizationButton.Enabled = true;
                ParticipleButton.Enabled = true;
                RemoveStopButton.Enabled = true;
                KeywordExtractionMaxButton.Enabled = true;
                KeywordExtractionNormalButton.Enabled = true;
                StatusLabel.Text = ofd.FileName + "文件打开完成。";
            }
        }

        private void StandardizationButton_Click(object sender, EventArgs e)
        {
            ClossButton();
            MyData.TheDoc = KeywordExtractionAPI.MyFun.DocStandardization(MyData.TheDoc);
            StatusLabel.Text = "文档标准化处理工作已经完成。";
            OpenButton();
        }

        private void ParticipleButton_Click(object sender, EventArgs e)
        {
            ClossButton();
            MessageBox.Show("提示：如果文档较长，可能需要一些时间，请耐心等待...", "提示", 0, MessageBoxIcon.Asterisk);

            // 系统初始化
            if (!MyFun.ICTCLAS_Init(null))
            {
                System.Console.WriteLine("Init ICTCLAS failed!");
                System.Console.Read();
            }
            //导入用户字典
            string path = @"userdic.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            string userdic = File.ReadAllText(path, encode);
            MyFun.ICTCLAS_ImportUserDict(userdic, userdic.Length, eCodeType.CODE_TYPE_UNKNOWN);

            int count = MyData.TheDoc.Length;//先得到结果的词数
            result_t[] result = new result_t[count];//在客户端申请资源
            int i = 0;

            //对字符串进行分词处理
            int nWrdCnt = MyFun.ICTCLAS_ParagraphProcessAW(MyData.TheDoc, result, eCodeType.CODE_TYPE_GB, 1);
            result_t r;

            //取字符串真实长度
            byte[] mybyte = System.Text.Encoding.Default.GetBytes(MyData.TheDoc);
            byte[] byteWord;
            string TheDoc = "";
            for (i = 0; i < nWrdCnt; i++)
            {
                r = result[i];

                byteWord = new byte[r.length];

                //取字符串一部分
                Array.Copy(mybyte, r.start, byteWord, 0, r.length);
                string wrd = System.Text.Encoding.Default.GetString(byteWord);

                if (wrd == " ")
                {
                    continue;
                }

                if (i==0)
                {
                    TheDoc += wrd;
                } 
                else
                {
                    TheDoc += " " + wrd;
                }
            }

            //释放资源退出
            MyFun.ICTCLAS_Exit();

            MyData.TheDoc = TheDoc;

            StatusLabel.Text = "中文分词已经完成。";

            OpenButton();
        }

        private void RemoveStopButton_Click(object sender, EventArgs e)
        {
            ClossButton();
            MessageBox.Show("提示：如果文档较长，可能需要一些时间，请耐心等待...", "提示", 0, MessageBoxIcon.Asterisk);
            MyData.TheDoc = KeywordExtractionAPI.MyFun.RemoveStop(MyData.TheDoc);
            StatusLabel.Text = "删除停用词工作已经完成。";
            OpenButton();
        }

        private void KeywordExtractionButton_Click(object sender, EventArgs e)
        {
            Thread thread = new Thread(new ThreadStart(DoWork));
            thread.IsBackground = true;
            thread.Start();
        }

        private void DoWork()
        {
            lock (this)
            {
                ClossButton();

                DateTime dt1 = DateTime.Now;

                StatusLabel.Text = "正在进行关键词提取：0%";

                ResultListView.Items.Clear();

                //统计
                MyData.WordsFre = KeywordExtractionAPI.MyFun.StatisticsWords(MyData.TheDoc);

                //计算熵差
                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    MyData.WordsFre[i].EntropyDifference_Max();
                    progressBar1.Value = i * 100 / MyData.WordsFre.Length;
                    StatusLabel.Text = "正在进行关键词提取：" + progressBar1.Value + "%";
                }

                //排序
                KeywordExtractionAPI.MyFun.QuickSort(MyData.WordsFre, 0, MyData.WordsFre.Length - 1);

                //显示在ListView中
                int WordsNum = 0;
                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    if (MyData.WordsFre[i].ED > 0)
                    {
                        WordsNum++;
                    }
                    else
                    {
                        break;
                    }
                }
                ListViewItem[] lvi = new ListViewItem[WordsNum];
                for (int i = 0; i < WordsNum; i++)
                {
                    lvi[i] = new ListViewItem();
                    lvi[i].SubItems[0].Text = (i + 1).ToString();
                    lvi[i].SubItems.Add(MyData.WordsFre[i].Word.ToString());
                    lvi[i].SubItems.Add(MyData.WordsFre[i].ED.ToString());
                    lvi[i].SubItems.Add(MyData.WordsFre[i].Frequency.ToString());
                }
                ResultListView.Items.AddRange(lvi);

                DateTime dt2 = DateTime.Now;
                SaveButton.Enabled = true;
                progressBar1.Value = 100;

                StatusLabel.Text = "关键词提取已经完成。本次提取用时" + (dt2 - dt1).ToString() + "。";

                OpenButton();
            }
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            string[] save;
            int WordsNum = 0;
            for (int i = 0; i < MyData.WordsFre.Length; i++)
            {
                if (MyData.WordsFre[i].ED > 0)
                {
                    WordsNum++;
                }
                else
                {
                    break;
                }
            }
            save = new string[WordsNum];
            for (int i = 0; i < WordsNum; i++)
            {
                save[i] = (i + 1).ToString() + "\t" + MyData.WordsFre[i].Word.ToString() + "\t" + MyData.WordsFre[i].ED.ToString() + "\t" + MyData.WordsFre[i].Frequency.ToString();
            }
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Encoding encode = Encoding.GetEncoding("GB2312");
                File.WriteAllLines(sfd.FileName, save);
            }
        }

        void OpenButton()
        {
            SelectFileButton.Enabled = true;
            SaveButton.Enabled = true;
            StandardizationButton.Enabled = true;
            ParticipleButton.Enabled = true;
            RemoveStopButton.Enabled = true;
            KeywordExtractionMaxButton.Enabled = true;
            KeywordExtractionNormalButton.Enabled = true;
        }

        void ClossButton()
        {
            SelectFileButton.Enabled = false;
            SaveButton.Enabled = false;
            StandardizationButton.Enabled = false;
            ParticipleButton.Enabled = false;
            RemoveStopButton.Enabled = false;
            KeywordExtractionMaxButton.Enabled = false;
            KeywordExtractionNormalButton.Enabled = false;
        }

        private void KeywordExtractionForm_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            Regex regex = new Regex("^*.txt$");
            Match ma = regex.Match(path);
            if (ma.Success)
            {
                PathTextBox.Text = path;
                Encoding encode = Encoding.GetEncoding("GB2312");
                MyData.TheDoc = File.ReadAllText(path, encode);
                TextRichTextBox.Text = MyData.TheDoc;
                SaveButton.Enabled = false;
                ResultListView.Items.Clear();
                StandardizationButton.Enabled = true;
                ParticipleButton.Enabled = true;
                RemoveStopButton.Enabled = true;
                KeywordExtractionMaxButton.Enabled = true;
                KeywordExtractionNormalButton.Enabled = true;
                StatusLabel.Text = path + "文件打开完成。";
            }
            else
            {
                MessageBox.Show("抱歉，您的文件不符合格式，请选择txt文件。");
            }
        }

        private void KeywordExtractionForm_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Link;
            else e.Effect = DragDropEffects.None;
        }

        private void ResultListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ResultListView.SelectedIndices != null && ResultListView.SelectedIndices.Count > 0)
            {
                Graphics g = this.CreateGraphics();
                g.Clear(this.BackColor);
                string word = "";
                ListView.SelectedIndexCollection c = ResultListView.SelectedIndices;
                word = ResultListView.Items[c[0]].SubItems[1].Text;

                int WordIndex = 0;
                for (WordIndex = 0; WordIndex < MyData.WordsFre.Length; WordIndex++)
                {
                    if (word == MyData.WordsFre[WordIndex].Word)
                    {
                        break;
                    }
                }
                int max = 0;
                foreach (int length in MyData.WordsFre[WordIndex].Distance)
                {
                    max += length;
                }
                Pen p = new Pen(Color.Black, 2);
                g.DrawRectangle(p, 38, 530, 809, 60);
                SolidBrush b1 = new SolidBrush(Color.Black);//定义单色画刷
                g.DrawString("0", new Font("宋体", 10), b1, new PointF(33, 600));
                g.DrawString((max / 10).ToString(), new Font("宋体", 10), b1, new PointF(113, 600));
                g.DrawString((max * 2 / 10).ToString(), new Font("宋体", 10), b1, new PointF(193, 600));
                g.DrawString((max * 3 / 10).ToString(), new Font("宋体", 10), b1, new PointF(273, 600));
                g.DrawString((max * 4 / 10).ToString(), new Font("宋体", 10), b1, new PointF(353, 600));
                g.DrawString((max * 5 / 10).ToString(), new Font("宋体", 10), b1, new PointF(433, 600));
                g.DrawString((max * 6 / 10).ToString(), new Font("宋体", 10), b1, new PointF(513, 600));
                g.DrawString((max * 7 / 10).ToString(), new Font("宋体", 10), b1, new PointF(593, 600));
                g.DrawString((max * 8 / 10).ToString(), new Font("宋体", 10), b1, new PointF(673, 600));
                g.DrawString((max * 9 / 10).ToString(), new Font("宋体", 10), b1, new PointF(753, 600));
                g.DrawString(max.ToString(), new Font("宋体", 10), b1, new PointF(833, 600));
                foreach (int po in MyData.WordsFre[WordIndex].Position)
                {
                    float x = (float)(38 + 1.0 * po * 809 / max);
                    g.DrawLine(p, x, 530, x, 590);
                }
            }
        }

        private void KeywordExtractionNormalButton_Click(object sender, EventArgs e)
        {
            Thread thread = new Thread(new ThreadStart(DoWork_Normal));
            thread.IsBackground = true;
            thread.Start();
        }

        private void DoWork_Normal()
        {
            lock (this)
            {
                ClossButton();

                DateTime dt1 = DateTime.Now;

                StatusLabel.Text = "正在进行关键词提取：0%";

                ResultListView.Items.Clear();

                //统计
                MyData.WordsFre = KeywordExtractionAPI.MyFun.StatisticsWords(MyData.TheDoc);

                //计算熵差
                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    MyData.WordsFre[i].EntropyDifference_Normal();
                    progressBar1.Value = i * 100 / MyData.WordsFre.Length;
                    StatusLabel.Text = "正在进行关键词提取：" + progressBar1.Value + "%";
                }

                //排序
                KeywordExtractionAPI.MyFun.QuickSort(MyData.WordsFre, 0, MyData.WordsFre.Length - 1);

                //显示在ListView中
                int WordsNum = 0;
                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    if (MyData.WordsFre[i].ED > 0)
                    {
                        WordsNum++;
                    }
                    else
                    {
                        break;
                    }
                }
                ListViewItem[] lvi = new ListViewItem[WordsNum];
                for (int i = 0; i < WordsNum; i++)
                {
                    lvi[i] = new ListViewItem();
                    lvi[i].SubItems[0].Text = (i + 1).ToString();
                    lvi[i].SubItems.Add(MyData.WordsFre[i].Word.ToString());
                    lvi[i].SubItems.Add(MyData.WordsFre[i].ED.ToString());
                    lvi[i].SubItems.Add(MyData.WordsFre[i].Frequency.ToString());
                }
                ResultListView.Items.AddRange(lvi);

                DateTime dt2 = DateTime.Now;
                SaveButton.Enabled = true;
                progressBar1.Value = 100;

                StatusLabel.Text = "关键词提取已经完成。本次提取用时" + (dt2 - dt1).ToString() + "。";

                OpenButton();
            }
        }

        private void HelpButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show("对文档关键词的提取过程：\n1.打开文档: 点击\"打开文件\"按钮，选择目标文档。（文件应为\"*.txt\"文件）\n2.文档标准化: 点击\"文档标准化\"按钮，去除文档中的标点符号、换行符等无用的符号，并将全文的字母替换为小写字母。（如果上述操作均已完成，可以跳过此步。）\n3.中文分词: 点击\"中文分词\"按钮，对中文文本进行分词。（如果为非中文文本，可以跳过此步。）\n4.去除停用词: 点击\"去除停用词\"按钮，按照停用词列表，去除文档中的停用词。（此步骤可选，去除停用词后，可能会提高提取关键词的准确度。）\n5(1).关键词提取（一般熵）: 点击\"关键词提取（一般熵）\"按钮，提取关键词。\n5(2).关键词提取（最大熵）: 点击\"关键词提取（最大熵）\"按钮，按照最大熵的方法，提取关键词。", "帮助");
        }
    }
}
