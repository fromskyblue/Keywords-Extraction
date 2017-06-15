using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections;

namespace Pretreatment
{
    class Program
    {
        static int Main(string[] args)
        {
            Encoding encode = Encoding.GetEncoding("GB2312");
            string path = args[0];
            string path_rvPun = args[1];
            //string path = @"./Origin.txt";
            //string path_rvPun = @"./rvPun.txt";
            string TheOriSpe = "";

            TheOriSpe = File.ReadAllText(path, encode);

            //Console.WriteLine(path);
            //Console.WriteLine(path_rvPun);

            //Console.WriteLine("开始删除标点。");
            Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
            TheOriSpe = replaceSpace.Replace(TheOriSpe, " ").Trim();

            MatchCollection mc = Regex.Matches(TheOriSpe, @"[，]");
            Console.Write(" ， " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[。]");
            Console.Write(" 。 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[、]");
            Console.Write(" 、 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[：]");
            Console.Write(" ： " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[“]");
            Console.Write(" “ " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[”]");
            Console.Write(" ” " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[（]");
            Console.Write(" （ " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[）]");
            Console.Write(" ） " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[【]");
            Console.Write(" 【 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[】]");
            Console.Write(" 】 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[《]");
            Console.Write(" 《 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[》]");
            Console.Write(" 》 " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[(]");
            Console.Write(" ( " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[)]");
            Console.Write(" ) " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[,]");
            Console.Write(" , " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[.]");
            Console.Write(" . " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[:]");
            Console.Write(" : " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[""]");
            Console.Write(" \" " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"['']");
            Console.Write(" ' " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[;]");
            Console.Write(" ; " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[&]");
            Console.Write(" & " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[#]");
            Console.Write(" # " + mc.Count.ToString());
            mc = Regex.Matches(TheOriSpe, @"[?]");
            Console.Write(" ? " + mc.Count.ToString());

            TheOriSpe = Regex.Replace(TheOriSpe, @"[，。、：“”（）【】《》(),.:""'';&#?]", "");
            TheOriSpe = Regex.Replace(TheOriSpe, @"[—]", " ");
            TheOriSpe = Regex.Replace(TheOriSpe, "\n", " ");
            TheOriSpe = replaceSpace.Replace(TheOriSpe, " ").Trim();
            //Console.WriteLine("删除标点完毕。");

            //Console.WriteLine("开始将文档转换为小写。");
            TheOriSpe = TheOriSpe.ToLower();
            //Console.WriteLine("转换完毕。");

            File.WriteAllText(path_rvPun, TheOriSpe, encode);

            //Console.ReadKey();

            return 0;
        }
    }

    class WORDSFRE
    {
        public string Word;
        public int Frequency;
        public int[] Position;
        public int[] Distance;
        public int PositionID;
        public Double ED;
    }
}
