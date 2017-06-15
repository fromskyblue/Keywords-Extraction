using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections;

namespace KeywordExtractionTest
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime dt1 = DateTime.Now;

            string path = @"E:\360云盘\程序\基于熵差的关键词提取_杨老师论文\C#版\测试\文档\TheOriginofSpeciesv6_rvPun.txt";
            //string path = @"E:\云盘\程序\基于熵差的关键词提取\123.txt";
            WORDSFRE[] WordsFre;

            //统计
            Console.WriteLine("统计文档信息 开始！");
            WordsFre = StatisticsWords(path);
            Console.WriteLine("统计文档信息 完成！");

            //计算熵差
            Console.WriteLine("计算熵差 开始！");
            foreach (WORDSFRE wf in WordsFre)
            {
                wf.EntropyDifference();
            }
            Console.WriteLine("计算熵差 完成！");

            //排序
            Console.WriteLine("熵差排序 开始！");
            QuickSort(WordsFre, 0, WordsFre.Length - 1);
            Console.WriteLine("熵差排序 完成！");

            //显示结果
            Console.WriteLine("\n结果为：");
            for (int i = 0; i < 20; i++)
            {
                Console.WriteLine(WordsFre[i].Word + "：" + WordsFre[i].ED);
            }

            DateTime dt2 = DateTime.Now;
            Console.WriteLine("\n程序运行完成，共用时" + (dt2 - dt1) + "。");
            Console.ReadKey();
        }

        static WORDSFRE[] StatisticsWords(string path)
        {
            string path_stops = @"E:\云盘\程序\基于熵差的关键词提取\EngStopList.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            WORDSFRE[] WordsFre;
            string TheDoc = "";
            string[] words;
            Hashtable ha = new Hashtable();

            //以下部分不去除停用词时省去
            //string TheDoc_New = "";
            //string[] stops;
            //bool isStop = false;
            //int WordNum = 0;
            //int StopNum = 0;

            //获取文档
            TheDoc = File.ReadAllText(path, encode);
            Console.WriteLine("　　获取文档 完成！");

            //全文转换为小写
            TheDoc = TheDoc.ToLower();
            Console.WriteLine("　　全文转换为小写 完成！");

            //获取全部词
            words = TheDoc.Split(' ');
            Console.WriteLine("　　获取全部词 完成！");

            //统计词频
            //去停用词版
            //stops = File.ReadAllLines(path_stops, encode);
            //foreach (string wd in words)
            //{
            //    foreach (string sp in stops)
            //    {
            //        if (wd == sp)
            //        {
            //            isStop = true;
            //            //Console.WriteLine(WordNum + "已经被删除");
            //            StopNum++;
            //            break;
            //        }
            //    }
            //    if (!isStop)
            //    {
            //        if (TheDoc_New == "")
            //        {
            //            TheDoc_New += wd;
            //        }
            //        else
            //        {
            //            TheDoc_New += " ";
            //            TheDoc_New += wd;
            //        }
            //        if (ha.Contains(wd))
            //        {
            //            ha[wd] = (int)ha[wd] + 1;
            //        }
            //        else
            //        {
            //            ha.Add(wd, 1);
            //        }
            //    }
            //    isStop = false;
            //    WordNum++;
            //}

            //不去停用词版
            foreach (string wd in words)
            {
                if (ha.Contains(wd))
                {
                    ha[wd] = (int)ha[wd] + 1;
                }
                else
                {
                    ha.Add(wd, 1);
                }
            }
            Console.WriteLine("　　统计词频 完成！");

            //统计位置信息
            string[] NewWords;
            NewWords = TheDoc.Split(' ');   //如果去除停用词，这里为TheDoc_New
            WordsFre = new WORDSFRE[ha.Count];
            int WordsFreID = 0;
            int ExistID = 0;
            bool isExist = false;
            for (int i = 0; i < NewWords.Length; i++)
            {
                for (int j = 0; j < WordsFreID; j++)
                {
                    if (NewWords[i] == WordsFre[j].Word)
                    {
                        isExist = true;
                        ExistID = j;
                        break;
                    }
                }
                if (!isExist)
                {
                    WordsFre[WordsFreID] = new WORDSFRE();
                    WordsFre[WordsFreID].Position = new int[(int)ha[NewWords[i]]];
                    WordsFre[WordsFreID].Distance = new int[(int)ha[NewWords[i]]];
                    WordsFre[WordsFreID].Word = NewWords[i];
                    WordsFre[WordsFreID].Frequency = (int)ha[NewWords[i]];
                    WordsFre[WordsFreID].Position[0] = i;
                    WordsFre[WordsFreID].PositionID = 1;
                    WordsFreID++;
                }
                else
                {
                    WordsFre[ExistID].Position[WordsFre[ExistID].PositionID] = i;
                    WordsFre[ExistID].PositionID++;
                    isExist = false;
                }
            }

            for (int i = 0; i < WordsFre.Length; i++)
            {
                for (int j = 0; j < WordsFre[i].Position.Length; j++)
                {
                    if (j == 0)
                    {
                        WordsFre[i].Distance[j] = WordsFre[i].Position[j] + NewWords.Length - WordsFre[i].Position[WordsFre[i].Position.Length - 1];
                    }
                    else
                    {
                        WordsFre[i].Distance[j] = WordsFre[i].Position[j] - WordsFre[i].Position[j - 1];
                    }
                }
            }
            Console.WriteLine("　　统计位置信息 完成！");

            return WordsFre;
        }

        static void QuickSort(WORDSFRE[] array, int left, int right)
        {
            if (left < right)
            {
                int middle = GetMiddleFroQuickSort(array, left, right);
                QuickSort(array, left, middle - 1);
                QuickSort(array, middle + 1, right);
            }
        }

        private static int GetMiddleFroQuickSort(WORDSFRE[] array, int left, int right)
        {
            WORDSFRE key = array[left];
            while (left < right)
            {
                while (left < right && key.ED.CompareTo(array[right].ED) > 0)
                {
                    right--;
                }
                if (left < right)
                {
                    WORDSFRE temp = array[left];
                    array[left] = array[right];
                    left++;
                }

                while (left < right && key.ED.CompareTo(array[left].ED) < 0)
                {
                    left++;
                }
                if (left < right)
                {
                    WORDSFRE temp = array[right];
                    array[right] = array[left];
                    right--;
                }
                array[left] = key;
            }
            return left;
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

        public void EntropyDifference()
        {
            //计算mean值
            int DocLength = 0;
            Double mean = 0;
            //Console.Write("词" + Word + "的原始序列为：");
            foreach (int a in Distance)
            {
                //Console.Write(a + ",");
                DocLength += a;
            }
            mean = 1.0 * DocLength / Frequency;
            //Console.Write("\n\nmean值为：" + mean);

            //intrinsic mode 和 extrinsic mode
            int[] intrinsic;
            int[] extrinsic;
            int in_num = 0, ex_num = 0;
            foreach (int a in Distance)
            {
                if (a > mean)
                {
                    ex_num++;
                }
                else
                {
                    in_num++;
                }
            }
            intrinsic = new int[in_num];
            extrinsic = new int[ex_num];
            in_num = 0;
            ex_num = 0;
            foreach (int a in Distance)
            {
                if (a > mean)
                {
                    extrinsic[ex_num] = a;
                    ex_num++;
                }
                else
                {
                    intrinsic[in_num] = a;
                    in_num++;
                }
            }

            Hashtable ha_intrinsic = new Hashtable();
            Hashtable ha_extrinsic = new Hashtable();
            //Console.Write("\n\nintrinsic序列为：");
            foreach (int a in intrinsic)
            {
                //Console.Write(a + ",");
                if (ha_intrinsic.Contains(a))
                {
                    ha_intrinsic[a] = (int)ha_intrinsic[a] + 1;
                }
                else
                {
                    ha_intrinsic.Add(a, 1);
                }
            }
            //Console.Write("\n\nextrinsic序列为：");
            foreach (int a in extrinsic)
            {
                //Console.Write(a + ",");
                if (ha_extrinsic.Contains(a))
                {
                    ha_extrinsic[a] = (int)ha_extrinsic[a] + 1;
                }
                else
                {
                    ha_extrinsic.Add(a, 1);
                }
            }

            //计算H(dI)和H(dE)
            Double H_dI = 0.0;
            Double H_dE = 0.0;
//             H_dI = Math.Log(ha_intrinsic.Count, 2);
//             H_dE = Math.Log(ha_extrinsic.Count, 2);
            int[] Complete_intrinsic = new int[ha_intrinsic.Count];
            int[] Complete_extrinsic = new int[ha_extrinsic.Count];
            int Complete_intrinsic_Num = 0;
            int Complete_extrinsic_Num = 0;
            bool isComplete = false;
            foreach (int a in intrinsic)
            {
                for (int i = 0; i < Complete_intrinsic_Num; i++)
                {
                    if (a == Complete_intrinsic[i])
                    {
                        isComplete = true;
                        break;
                    }
                }
                if (!isComplete)
                {
                    Double Pd = 1.0 * ((int)ha_intrinsic[a]) / intrinsic.Length;
                    H_dI += -Pd * Math.Log(Pd, 2);
                    Complete_intrinsic[Complete_intrinsic_Num] = a;
                    Complete_intrinsic_Num++;
                }
                isComplete = false;
            }
            //Console.Write("\n\nintrinsic序列的熵 H(dI)=" + H_dI);
            foreach (int a in extrinsic)
            {
                for (int i = 0; i < Complete_extrinsic_Num; i++)
                {
                    if (a == Complete_extrinsic[i])
                    {
                        isComplete = true;
                        break;
                    }
                }
                if (!isComplete)
                {
                    Double Pd = 1.0 * ((int)ha_extrinsic[a]) / extrinsic.Length;
                    H_dE += -Pd * Math.Log(Pd, 2);
                    Complete_extrinsic[Complete_extrinsic_Num] = a;
                    Complete_extrinsic_Num++;
                }
                isComplete = false;
            }
            //Console.Write("\n\nextrinsic序列的熵 H(dE)=" + H_dE);

            //计算EDq(d)
            Double EDq_d = 0.0;
            int q = 2;
            EDq_d = Math.Pow(H_dI, q) - Math.Pow(H_dE, q);
            //Console.Write("\n\n熵差 EDq(d)=" + EDq_d);

            //计算EDgeoq(d)
            Double EDgeoq_d = 0.0;
            Double Hgeo_dI = 0.0;
            Double pI = 0.0;
            Double Hgeo_dE = 0.0;
            Double pE = 0.0;
            for (int i = 1; i <= (int)mean; i++)
            {
                pI += (1 / mean) * Math.Pow(1 - 1 / mean, i - 1);
            }
            for (int i = 1; i <= (int)mean; i++)
            {
                Hgeo_dI += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI, 2);
            }
            pE = 1 - pI;
            for (int i = (int)mean + 1; i < DocLength; i++)
            {
                Hgeo_dE += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE, 2);
            }
            EDgeoq_d = Math.Pow(Hgeo_dI, q) - Math.Pow(Hgeo_dE, q);
            //Console.Write("\n\n熵差 EDgeoq(d)=" + EDgeoq_d);

            //计算EDnorq(d)
            Double EDnorq_d = 0.0;
            if (Math.Abs(EDgeoq_d) != 0)
            {
                EDnorq_d = EDq_d / Math.Abs(EDgeoq_d);
            }
            else
            {
                EDnorq_d = -1000;
            }
            //Console.Write("\n\n熵差 EDnorq(d)=" + EDnorq_d);

            ED = EDnorq_d;
            //Console.WriteLine(Word + " 计算完成！");
            //if (Word == "crossed")
            //{
            //    Console.WriteLine(Word + " 计算完成！" + EDq_d + "," + EDgeoq_d);
            //}
        }
    }
}
