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
            RemoveStopButton.Enabled = false;
            KeywordExtractionMaxButton.Enabled = false;
            KeywordExtractionNormalButton.Enabled = false;
            StatusLabel.Text = "Welcome to keyword extraction software, which is based on the entropy difference.";
            MessageBox.Show("The process of document keyword extraction:\n\nStep 1: Open the document: Click \"Open\" button, select the target document. (File should be \"*. Txt \" file)\n\nStep 2: Document standardization: Click \"Document Standardization\" button to remove the document punctuation, line breaks, and other useless symbols, and text replaces lowercase letters. (If the above operations have been completed, you can skip this step.)\n\nStep 3: Stop word removal: Click \"Remove Stop\" button and follow the list of stop words, removing stop words in the document. (This step is optional, remove stop words, it may improve the accuracy of keyword extraction.)\n\nStep 4: Here are two method to extract the keyword, you can choose one of them to finish the Keyword extraction work.\n    4.1: Keyword extraction (normal entropy): Click \"Keyword Extraction (Entropy)\" button to extract keywords.\n    4.2: Keyword extraction (maximum entropy): Click \"Keyword Extraction (Maximum Entropy)\" button and follow the maximum entropy method to extract keywords.", "Help");
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
                RemoveStopButton.Enabled = true;
                KeywordExtractionMaxButton.Enabled = true;
                KeywordExtractionNormalButton.Enabled = true;
                StatusLabel.Text = ofd.FileName + " file open finish.";
            }
        }

        private void StandardizationButton_Click(object sender, EventArgs e)
        {
            ClossButton();
            MyData.TheDoc = MyFun.DocStandardization(MyData.TheDoc);
            StatusLabel.Text = "Document standardization process has been completed.";
            OpenButton();
        }

        private void RemoveStopButton_Click(object sender, EventArgs e)
        {
            ClossButton();
            MessageBox.Show("Tip: If the document is longer, it may take you few minutes, please be patient ...", "Prompt", 0, MessageBoxIcon.Asterisk);
            MyData.TheDoc = MyFun.RemoveStop(MyData.TheDoc);
            StatusLabel.Text = "Remove stop-words has been completed.";
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

                StatusLabel.Text = "Keyword extraction process is ongoing:0%";

                ResultListView.Items.Clear();

                MyData.WordsFre = MyFun.StatisticsWords(MyData.TheDoc);

                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    MyData.WordsFre[i].EntropyDifference_Max();
                    progressBar1.Value = i * 100 / MyData.WordsFre.Length;
                    StatusLabel.Text = "Keyword extraction process is ongoing: " + progressBar1.Value + "%";
                }

                MyFun.QuickSort(MyData.WordsFre, 0, MyData.WordsFre.Length - 1);

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

                StatusLabel.Text = "Keyword extraction has been completed. The extraction spend " + (dt2 - dt1).ToString() + ".";

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
                save[i] = (i + 1).ToString() + "\t" + MyData.WordsFre[i].Word.ToString() + "\t" + MyData.WordsFre[i].ED.ToString();
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
            RemoveStopButton.Enabled = true;
            KeywordExtractionMaxButton.Enabled = true;
            KeywordExtractionNormalButton.Enabled = true;
        }

        void ClossButton()
        {
            SelectFileButton.Enabled = false;
            SaveButton.Enabled = false;
            StandardizationButton.Enabled = false;
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
                RemoveStopButton.Enabled = true;
                KeywordExtractionMaxButton.Enabled = true;
                KeywordExtractionNormalButton.Enabled = true;
                StatusLabel.Text = path + " file open finish.";
            }
            else
            {
                MessageBox.Show("Sorry, your documents do not meet format, please select \"txt\" file.");
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
                SolidBrush b1 = new SolidBrush(Color.Black);
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

                StatusLabel.Text = "Keyword extraction process is ongoing: 0%";

                ResultListView.Items.Clear();

                MyData.WordsFre = MyFun.StatisticsWords(MyData.TheDoc);

                for (int i = 0; i < MyData.WordsFre.Length; i++)
                {
                    MyData.WordsFre[i].EntropyDifference_Normal();
                    progressBar1.Value = i * 100 / MyData.WordsFre.Length;
                    StatusLabel.Text = "Keyword extraction process is ongoing: " + progressBar1.Value + "%";
                }

                MyFun.QuickSort(MyData.WordsFre, 0, MyData.WordsFre.Length - 1);

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

                StatusLabel.Text = "Keyword extraction has been completed. The extraction spend " + (dt2 - dt1).ToString() + ".";

                OpenButton();
            }
        }

        private void HelpButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show("The process of document keyword extraction:\n\nStep 1: Open the document: Click \"Open\" button, select the target document. (File should be \"*. Txt \" file)\n\nStep 2: Document standardization: Click \"Document Standardization\" button to remove the document punctuation, line breaks, and other useless symbols, and text replaces lowercase letters. (If the above operations have been completed, you can skip this step.)\n\nStep 3: Stop word removal: Click \"Remove Stop\" button and follow the list of stop words, removing stop words in the document. (This step is optional, remove stop words, it may improve the accuracy of keyword extraction.)\n\nStep 4: Here are two method to extract the keyword, you can choose one of them to finish the Keyword extraction work.\n    4.1: Keyword extraction (normal entropy): Click \"Keyword Extraction (Entropy)\" button to extract keywords.\n    4.2: Keyword extraction (maximum entropy): Click \"Keyword Extraction (Maximum Entropy)\" button and follow the maximum entropy method to extract keywords.", "Help");
        }

        private void TextRichTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }
    }
}
