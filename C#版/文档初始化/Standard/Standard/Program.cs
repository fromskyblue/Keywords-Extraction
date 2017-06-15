using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections;


namespace Standard
{
    class Program
    {
        static void Main(string[] args)
        {
            Encoding encode = Encoding.GetEncoding("GB2312");
            string infile = args[0];
            string outfile = args[1];
            string resultfile = args[2];
//             string infile = @"./Origin.txt";
//             string outfile = @"./rvPun.txt";
//             string resultfile = @"./sResult.json";
            string TheDoc = "";
            string result = "{\"Punctuation\":[";

            TheDoc = File.ReadAllText(infile, encode);
            
            Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
            TheDoc = replaceSpace.Replace(TheDoc, " ").Trim();
            
            string pun = "(),.:\"';&#?";
            string[] pun_chs = { "Brace", "Anti Brackets", "Comma", "Period", "Colon", "Double Quotes", "Single Quotes", "Semicolon", "And", "Wells", "Question Mark" };
            for (int i=0;i<pun.Length;i++)
            {
                //Console.WriteLine(pun[i]);
                string r = "[" + pun[i] + "]";
                MatchCollection mc = Regex.Matches(TheDoc, r);
                //Console.WriteLine(pun[i] + "\t" + mc.Count.ToString());
                if (i!=0)
                {
                result += ",";
                }
                result += "{\"punc\":\"" + pun_chs[i] + "\",\"count\":\"" + mc.Count.ToString() + "\"}";
            }
            result += "]}";
            //Console.WriteLine(result);

            TheDoc = Regex.Replace(TheDoc, @"[，。、：“”（）【】《》(),.:""'';&#?]", "");
            TheDoc = Regex.Replace(TheDoc, @"[—]", " ");
            TheDoc = Regex.Replace(TheDoc, "\n", " ");
            TheDoc = replaceSpace.Replace(TheDoc, " ").Trim();

            TheDoc = TheDoc.ToLower();
            
            File.WriteAllText(outfile, TheDoc, encode);
            File.WriteAllText(resultfile, result, encode);
        }
    }
}
