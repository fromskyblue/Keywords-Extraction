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
            string path_stops = args[1];
            string path_dltStop = args[2];
            string TheOriSpe = "";
            string TheOriSpe_New = "";
            string[] words;
            string[] stops;
            bool isStop = false;
            int WordNum = 0;
            int StopNum = 0;
            Hashtable ha = new Hashtable();

            TheOriSpe = File.ReadAllText(path, encode);

            //Console.WriteLine(path);
            //Console.WriteLine(path_rvPun);
            //Console.WriteLine(path_dltStop);

            //Console.WriteLine("开始进行停用词删除处理。");
            stops = File.ReadAllLines(path_stops, encode);

            words = TheOriSpe.Split(' ');

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
                    if (TheOriSpe_New=="")
                    {
                        TheOriSpe_New += wd;
                    } 
                    else
                    {
                        TheOriSpe_New += " ";
                        TheOriSpe_New += wd;
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

            File.WriteAllText(path_dltStop, TheOriSpe_New, encode);
            //Console.WriteLine("处理完毕！共有词数" + WordNum + "个，其中停用词" + StopNum + "个，余下共" + (WordNum - StopNum) + "个词");

            string reword = WordNum.ToString() + "," + StopNum.ToString();
            Console.WriteLine(reword);
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
