using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpICTCLAS;
using System.IO;

namespace DLLExample
{
    class Program
    {
        static void Main(string[] args)
        {

            string DictPath = Path.Combine(Environment.CurrentDirectory, "Data") + Path.DirectorySeparatorChar;
            Console.WriteLine("正在初始化字典库，请稍候...");
            WordSegmentSample sample = new WordSegmentSample(DictPath, 2);

            string path = @"E:\云盘\程序\基于熵差的关键词提取_杨老师论文\C#版\测试\文档\中文示例.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            string Doc;
            DateTime d1, d2;
            Doc = File.ReadAllText(path, encode);
            d1 = DateTime.Now;

            List<WordResult[]> result = sample.Segment(Doc);

            d2 = DateTime.Now;
            Console.WriteLine("分词完成，用时" + (d2 - d1).ToString() + "。");
            
            Console.Write("\n按下回车键退出......");
            Console.ReadKey();
        }
    }
}
