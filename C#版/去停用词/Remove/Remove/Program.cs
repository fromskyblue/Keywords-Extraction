using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections;

namespace Remove
{
    class Program
    {
        static void Main(string[] args)
        {
            Encoding encode = Encoding.GetEncoding("GB2312");
            string infile = args[0];
            string stopfile = args[1];
            string outfile = args[2];
            string resultfile = args[3];
//             string infile = @"./rvPun.txt";
//             string stopfile = @"./StopList.txt";
//             string outfile = @"./dltStop.txt";
//             string resultfile = @"./rResult.json";
            string TheDoc = "";
            string TheDoc_New = "";
            string[] words;
            string[] stops;
            bool isStop = false;
            int WordNum = 0;
            int StopNum = 0;
            Hashtable ha = new Hashtable();

            TheDoc = File.ReadAllText(infile, encode);

            stops = File.ReadAllLines(stopfile, encode);

            words = TheDoc.Split(' ');

            foreach (string wd in words)
            {
                foreach (string sp in stops)
                {
                    if (wd == sp)
                    {
                        isStop = true;
                        //Console.WriteLine(WordNum + "已经被删除");
                        StopNum++;
                        break;
                    }
                }
                if (!isStop)
                {
                    if (TheDoc_New == "")
                    {
                        TheDoc_New += wd;
                    }
                    else
                    {
                        TheDoc_New += " ";
                        TheDoc_New += wd;
                    }
                    if (ha.Contains(wd))
                    {
                        ha[wd] = (int)ha[wd] + 1;
                    }
                    else
                    {
                        ha.Add(wd, 1);
                    }
                }
                isStop = false;
                WordNum++;
            }

            string result = "{\"WordNum\":\"" + WordNum.ToString() + "\",\"StopNum\":\"" + StopNum.ToString() + "\"}";
            //Console.WriteLine(result);

            File.WriteAllText(outfile, TheDoc_New, encode);
            File.WriteAllText(resultfile, result, encode);
        }
    }
}
