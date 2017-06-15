using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace NewlineConversion
{
    class Program
    {
        static void Main(string[] args)
        {
            Encoding encode = Encoding.GetEncoding("GB2312");
            string path_read = @"E:\云盘\程序\基于熵差的关键词提取\TheOriginofSpeciesv6.txt";
            string path_write = @"E:\云盘\程序\基于熵差的关键词提取\TheOriginofSpeciesv6_New.txt";
            string TheOriSpe = "";

            TheOriSpe = File.ReadAllText(path_read, encode);

            Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
            TheOriSpe = replaceSpace.Replace(TheOriSpe, "\n\n").Trim();

            Console.Write(TheOriSpe);

            File.WriteAllText(path_write, TheOriSpe, encode);

            Console.ReadKey();
        }
    }
}
