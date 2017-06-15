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
            //string path_rvPun = args[1];
            string path_stops = args[1];
            string path_dltStop = args[2];
            //string path = @".\Origin.txt";
            //string path_rvPun = @".\rvPun.txt";
            //string path_stops = @".\StopList.txt";
            //string path_dltStop = @".\dltStop.txt";
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

            //Console.WriteLine("开始删除标点。");
//             Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
//             TheOriSpe = replaceSpace.Replace(TheOriSpe, " ").Trim();
// 
//             TheOriSpe = Regex.Replace(TheOriSpe, @"[，。、：“”（）【】《》(),.:""'';&#?]", "");
//             TheOriSpe = Regex.Replace(TheOriSpe, @"[—]", " ");
//             TheOriSpe = Regex.Replace(TheOriSpe, "\n", " ");
//             TheOriSpe = replaceSpace.Replace(TheOriSpe, " ").Trim();
            //Console.WriteLine("删除标点完毕。");

            //Console.WriteLine("开始将文档转换为小写。");
            //TheOriSpe = TheOriSpe.ToLower();
            //Console.WriteLine("转换完毕。");

            //File.WriteAllText(path_rvPun, TheOriSpe, encode);

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

//             Console.WriteLine("开始统计词频信息。");
//             WORDSFRE[] WordsFre;
//             string[] NewWords;
//             int WordsFreID = 0;
//             int ExistID = 0;
//             bool isExist = false;
//             NewWords = TheOriSpe_New.Split(' ');
//             WordsFre = new WORDSFRE[ha.Count];
//             string path_Line = @".\Line.txt";
//             File.WriteAllLines(path_Line, NewWords);
// 
//             for (int i = 0; i < NewWords.Length; i++)
//             {
//                 for (int j = 0; j < WordsFreID; j++)
//                 {
//                     if (NewWords[i] == WordsFre[j].Word)
//                     {
//                         isExist = true;
//                         ExistID = j;
//                         break;
//                     }
//                 }
//                 if (!isExist)
//                 {
//                     WordsFre[WordsFreID] = new WORDSFRE();
//                     WordsFre[WordsFreID].Position = new int[(int)ha[NewWords[i]]];
//                     WordsFre[WordsFreID].Distance = new int[(int)ha[NewWords[i]]];
//                     WordsFre[WordsFreID].Word = NewWords[i];
//                     WordsFre[WordsFreID].Frequency = (int)ha[NewWords[i]];
//                     WordsFre[WordsFreID].Position[0] = i;
//                     WordsFre[WordsFreID].PositionID = 1;
//                     WordsFreID++;
//                 }
//                 else
//                 {
//                     WordsFre[ExistID].Position[WordsFre[ExistID].PositionID] = i;
//                     WordsFre[ExistID].PositionID++;
//                     isExist = false;
//                 }
//             }
// 
//             for (int i = 0; i < WordsFre.Length; i++)
//             {
//                 for (int j = 0; j < WordsFre[i].Position.Length; j++)
//                 {
//                     if (j == 0)
//                     {
//                         WordsFre[i].Distance[j] = WordsFre[i].Position[j] + NewWords.Length - WordsFre[i].Position[WordsFre[i].Position.Length - 1];
//                     } 
//                     else
//                     {
//                         WordsFre[i].Distance[j] = WordsFre[i].Position[j] - WordsFre[i].Position[j - 1];
//                     }
//                 }
//             }
//             Console.WriteLine("统计词频信息完成。");
// 
//             Console.ReadKey();
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
