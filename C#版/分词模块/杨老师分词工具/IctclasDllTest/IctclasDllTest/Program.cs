using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Text.RegularExpressions;

namespace IctclasDllTest
{
    class Program
    {
        [DllImport(@"IctclasDll.dll", EntryPoint = "IctclasProcess", CallingConvention = CallingConvention.Cdecl)]
        public static extern void IctclasProcess(string s, byte[] t);

        static void Main(string[] args)
        {
            string path = @"E:\云盘\程序\基于熵差的关键词提取_杨老师论文\C#版\测试\文档\123.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            string Doc;
            DateTime d1, d2;

            Doc = File.ReadAllText(path, encode);

            byte[] t = new byte[Doc.Length * 3];
            Console.WriteLine("开始分词。");
            d1 = DateTime.Now;
            IctclasProcess(Doc, t);
            d2 = DateTime.Now;
            Console.WriteLine("分词完成，用时" + (d2 - d1).ToString() + "。");
            Console.ReadKey();
            string DocIctclas = encode.GetString(t);

            Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
            DocIctclas = Regex.Replace(DocIctclas, "\0", "");

            Console.WriteLine("原始文档：");
            Console.WriteLine(Doc);
            Console.WriteLine("\n分词后文档：");
            Console.WriteLine(DocIctclas);

            Console.ReadKey();
        }
    }
}
