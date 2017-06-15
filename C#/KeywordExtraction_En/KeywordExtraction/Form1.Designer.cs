namespace KeywordExtraction
{
    partial class KeywordExtractionForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.PathLabel = new System.Windows.Forms.Label();
            this.SelectFileButton = new System.Windows.Forms.Button();
            this.PathTextBox = new System.Windows.Forms.TextBox();
            this.TextRichTextBox = new System.Windows.Forms.RichTextBox();
            this.ResultListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.StandardizationButton = new System.Windows.Forms.Button();
            this.RemoveStopButton = new System.Windows.Forms.Button();
            this.KeywordExtractionMaxButton = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.SaveButton = new System.Windows.Forms.Button();
            this.StatusLabel = new System.Windows.Forms.Label();
            this.KeywordExtractionNormalButton = new System.Windows.Forms.Button();
            this.HelpButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // PathLabel
            // 
            this.PathLabel.AutoSize = true;
            this.PathLabel.Location = new System.Drawing.Point(22, 30);
            this.PathLabel.Name = "PathLabel";
            this.PathLabel.Size = new System.Drawing.Size(155, 12);
            this.PathLabel.TabIndex = 0;
            this.PathLabel.Text = "Please Enter A File Path:";
            // 
            // SelectFileButton
            // 
            this.SelectFileButton.Location = new System.Drawing.Point(626, 25);
            this.SelectFileButton.Name = "SelectFileButton";
            this.SelectFileButton.Size = new System.Drawing.Size(75, 23);
            this.SelectFileButton.TabIndex = 2;
            this.SelectFileButton.Text = "Open";
            this.SelectFileButton.UseVisualStyleBackColor = true;
            this.SelectFileButton.Click += new System.EventHandler(this.SelectFileButton_Click);
            // 
            // PathTextBox
            // 
            this.PathTextBox.Location = new System.Drawing.Point(183, 27);
            this.PathTextBox.Name = "PathTextBox";
            this.PathTextBox.Size = new System.Drawing.Size(437, 21);
            this.PathTextBox.TabIndex = 1;
            // 
            // TextRichTextBox
            // 
            this.TextRichTextBox.Location = new System.Drawing.Point(38, 72);
            this.TextRichTextBox.Name = "TextRichTextBox";
            this.TextRichTextBox.Size = new System.Drawing.Size(355, 383);
            this.TextRichTextBox.TabIndex = 8;
            this.TextRichTextBox.Text = "";
            this.TextRichTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextRichTextBox_KeyPress);
            // 
            // ResultListView
            // 
            this.ResultListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.ResultListView.FullRowSelect = true;
            this.ResultListView.GridLines = true;
            this.ResultListView.Location = new System.Drawing.Point(399, 72);
            this.ResultListView.Name = "ResultListView";
            this.ResultListView.Size = new System.Drawing.Size(312, 383);
            this.ResultListView.TabIndex = 9;
            this.ResultListView.UseCompatibleStateImageBehavior = false;
            this.ResultListView.View = System.Windows.Forms.View.Details;
            this.ResultListView.SelectedIndexChanged += new System.EventHandler(this.ResultListView_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Rank";
            this.columnHeader1.Width = 45;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Word";
            this.columnHeader2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.columnHeader2.Width = 80;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Entropy Difference";
            this.columnHeader3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.columnHeader3.Width = 100;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Frequency";
            // 
            // StandardizationButton
            // 
            this.StandardizationButton.Location = new System.Drawing.Point(732, 88);
            this.StandardizationButton.Name = "StandardizationButton";
            this.StandardizationButton.Size = new System.Drawing.Size(115, 49);
            this.StandardizationButton.TabIndex = 4;
            this.StandardizationButton.Text = "Document Standardization";
            this.StandardizationButton.UseVisualStyleBackColor = true;
            this.StandardizationButton.Click += new System.EventHandler(this.StandardizationButton_Click);
            // 
            // RemoveStopButton
            // 
            this.RemoveStopButton.Location = new System.Drawing.Point(732, 188);
            this.RemoveStopButton.Name = "RemoveStopButton";
            this.RemoveStopButton.Size = new System.Drawing.Size(115, 49);
            this.RemoveStopButton.TabIndex = 6;
            this.RemoveStopButton.Text = "Remove Stop";
            this.RemoveStopButton.UseVisualStyleBackColor = true;
            this.RemoveStopButton.Click += new System.EventHandler(this.RemoveStopButton_Click);
            // 
            // KeywordExtractionMaxButton
            // 
            this.KeywordExtractionMaxButton.Location = new System.Drawing.Point(732, 388);
            this.KeywordExtractionMaxButton.Name = "KeywordExtractionMaxButton";
            this.KeywordExtractionMaxButton.Size = new System.Drawing.Size(115, 49);
            this.KeywordExtractionMaxButton.TabIndex = 7;
            this.KeywordExtractionMaxButton.Text = "Keyword Extraction (Maximum Entropy)";
            this.KeywordExtractionMaxButton.UseVisualStyleBackColor = true;
            this.KeywordExtractionMaxButton.Click += new System.EventHandler(this.KeywordExtractionButton_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(38, 492);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(809, 23);
            this.progressBar1.TabIndex = 9;
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(707, 25);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(75, 23);
            this.SaveButton.TabIndex = 3;
            this.SaveButton.Text = "Save";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // StatusLabel
            // 
            this.StatusLabel.AutoSize = true;
            this.StatusLabel.Location = new System.Drawing.Point(36, 467);
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(41, 12);
            this.StatusLabel.TabIndex = 10;
            this.StatusLabel.Text = "label1";
            // 
            // KeywordExtractionNormalButton
            // 
            this.KeywordExtractionNormalButton.Location = new System.Drawing.Point(732, 288);
            this.KeywordExtractionNormalButton.Name = "KeywordExtractionNormalButton";
            this.KeywordExtractionNormalButton.Size = new System.Drawing.Size(115, 49);
            this.KeywordExtractionNormalButton.TabIndex = 11;
            this.KeywordExtractionNormalButton.Text = "Keyword Extraction (Entropy)";
            this.KeywordExtractionNormalButton.UseVisualStyleBackColor = true;
            this.KeywordExtractionNormalButton.Click += new System.EventHandler(this.KeywordExtractionNormalButton_Click);
            // 
            // HelpButton
            // 
            this.HelpButton.Location = new System.Drawing.Point(788, 25);
            this.HelpButton.Name = "HelpButton";
            this.HelpButton.Size = new System.Drawing.Size(75, 23);
            this.HelpButton.TabIndex = 12;
            this.HelpButton.Text = "Help";
            this.HelpButton.UseVisualStyleBackColor = true;
            this.HelpButton.Click += new System.EventHandler(this.HelpButton_Click);
            // 
            // KeywordExtractionForm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(884, 637);
            this.Controls.Add(this.HelpButton);
            this.Controls.Add(this.KeywordExtractionNormalButton);
            this.Controls.Add(this.StatusLabel);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.KeywordExtractionMaxButton);
            this.Controls.Add(this.RemoveStopButton);
            this.Controls.Add(this.StandardizationButton);
            this.Controls.Add(this.ResultListView);
            this.Controls.Add(this.TextRichTextBox);
            this.Controls.Add(this.SelectFileButton);
            this.Controls.Add(this.PathTextBox);
            this.Controls.Add(this.PathLabel);
            this.MaximizeBox = false;
            this.Name = "KeywordExtractionForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Keyword Extraction Based On Entropy Difference";
            this.Load += new System.EventHandler(this.KeywordExtractionForm_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.KeywordExtractionForm_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.KeywordExtractionForm_DragEnter);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label PathLabel;
        private System.Windows.Forms.Button SelectFileButton;
        private System.Windows.Forms.TextBox PathTextBox;
        private System.Windows.Forms.RichTextBox TextRichTextBox;
        private System.Windows.Forms.ListView ResultListView;
        private System.Windows.Forms.Button StandardizationButton;
        private System.Windows.Forms.Button RemoveStopButton;
        private System.Windows.Forms.Button KeywordExtractionMaxButton;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Label StatusLabel;
        private System.Windows.Forms.Button KeywordExtractionNormalButton;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Button HelpButton;

    }
}

