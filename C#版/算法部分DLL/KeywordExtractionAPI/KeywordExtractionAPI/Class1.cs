using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.IO;
using System.Text.RegularExpressions;

namespace KeywordExtractionAPI
{
    public class MyFun
    {
        public static string DocStandardization(string TheDoc)
        {
            try
            {
                string TheDoc_New = "";

                //删除换行和多个空格
                Regex replaceSpace = new Regex(@"\s{2,}", RegexOptions.IgnoreCase);
                TheDoc = Regex.Replace(TheDoc, "\n", " ");
                TheDoc = replaceSpace.Replace(TheDoc, " ").Trim();

                //删除标点
                TheDoc = Regex.Replace(TheDoc, @"[，。、：；！？…—“”（）【】《》(),.:""'';&#?!]", " ");
                TheDoc = replaceSpace.Replace(TheDoc, " ").Trim();

                //全文转换为小写
                TheDoc_New = TheDoc.ToLower();

                return TheDoc_New;
            }
            catch (System.Exception ex)
            {
                return TheDoc;
            }
        }

        public static string RemoveStop(string TheDoc)
        {

            string path_stops = @".\StopList.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            string TheDoc_New = "";

            string[] words;
            string[] stops;
            bool isStop = false;
            int WordNum = 0;
            int StopNum = 0;

            words = TheDoc.Split(' ');

            try
            {
                stops = File.ReadAllLines(path_stops, encode);
            }
            catch (System.Exception ex)
            {
                return "ERROR: The file \"StopList.txt\" can not be found.";
            }

            try
            {
                foreach (string wd in words)
                {
                    foreach (string sp in stops)
                    {
                        if (wd == sp)
                        {
                            isStop = true;
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
                    }
                    isStop = false;
                    WordNum++;
                }

                return TheDoc_New;
            }
            catch (System.Exception ex)
            {
                return TheDoc;
            }
        }

        public static WORDSFRE[] StatisticsWords(string TheDoc)
        {
            try
            {
                WORDSFRE[] WordsFre;
                string[] words;
                Hashtable ha = new Hashtable();

                //获取全部词
                words = TheDoc.Split(' ');

                //统计词频
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

                //统计位置信息
                WordsFre = new WORDSFRE[ha.Count];
                int WordsFreID = 0;
                int ExistID = 0;
                bool isExist = false;
                for (int i = 0; i < words.Length; i++)
                {
                    for (int j = 0; j < WordsFreID; j++)
                    {
                        if (words[i] == WordsFre[j].Word)
                        {
                            isExist = true;
                            ExistID = j;
                            break;
                        }
                    }
                    if (!isExist)
                    {
                        WordsFre[WordsFreID] = new WORDSFRE();
                        WordsFre[WordsFreID].Position = new int[(int)ha[words[i]]];
                        WordsFre[WordsFreID].Distance = new int[(int)ha[words[i]]];
                        WordsFre[WordsFreID].Word = words[i];
                        WordsFre[WordsFreID].Frequency = 1;
                        WordsFre[WordsFreID].Position[0] = i;
                        WordsFreID++;
                    }
                    else
                    {
                        WordsFre[ExistID].Position[WordsFre[ExistID].Frequency] = i;
                        WordsFre[ExistID].Frequency++;
                        isExist = false;
                    }
                }

                for (int i = 0; i < WordsFre.Length; i++)
                {
                    for (int j = 0; j < WordsFre[i].Position.Length; j++)
                    {
                        if (j == 0)
                        {
                            WordsFre[i].Distance[j] = WordsFre[i].Position[j] + words.Length - WordsFre[i].Position[WordsFre[i].Position.Length - 1];
                        }
                        else
                        {
                            WordsFre[i].Distance[j] = WordsFre[i].Position[j] - WordsFre[i].Position[j - 1];
                        }
                    }
                }

                return WordsFre;
            }
            catch (System.Exception ex)
            {
                return null;
            }
        }

        public static bool QuickSort(WORDSFRE[] array, int left, int right)
        {
            try
            {
                if (left < right)
                {
                    int middle = GetMiddleFroQuickSort(array, left, right);
                    if (middle == -1)
                    {
                        return false;
                    }
                    QuickSort(array, left, middle - 1);
                    QuickSort(array, middle + 1, right);
                }
                return true;
            }
            catch (System.Exception ex)
            {
                return false;
            }
        }

        private static int GetMiddleFroQuickSort(WORDSFRE[] array, int left, int right)
        {
            try
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
            catch (System.Exception ex)
            {
                return -1;
            }
        }
    }

    public class WORDSFRE
    {
        public string Word;
        public int Frequency;
        public int[] Position;
        public int[] Distance;
        public Double ED;

        public bool EntropyDifference_Max()
        {
            try
            {
                //计算mean值
                int DocLength = 0;
                Double mean = 0;
                foreach (int a in Distance)
                {
                    DocLength += a;
                }
                mean = 1.0 * DocLength / Frequency;

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
                foreach (int a in intrinsic)
                {
                    if (ha_intrinsic.Contains(a))
                    {
                        ha_intrinsic[a] = (int)ha_intrinsic[a] + 1;
                    }
                    else
                    {
                        ha_intrinsic.Add(a, 1);
                    }
                }
                foreach (int a in extrinsic)
                {
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
                H_dI = Math.Log(ha_intrinsic.Count, 2);
                H_dE = Math.Log(ha_extrinsic.Count, 2);

                //计算EDq(d)
                Double EDq_d = 0.0;
                int q = 2;
                EDq_d = Math.Pow(H_dI, q) - Math.Pow(H_dE, q);
                if (EDq_d<=0)
                {
                    ED = -1000;

                    return true;
                }

                //计算EDgeoq(d)
                Double EDgeoq_d = 0.0;
                Double Hgeo_dI = 0.0;
                Double pI = 0.0;
                Double Hgeo_dE = 0.0;
                Double pE = 0.0;
                for (int i = 1; i < (int)mean; i++)
                {
                    pI += (1 / mean) * Math.Pow(1 - 1 / mean, i - 1);
                }
                for (int i = 1; i < (int)mean; i++)
                {
                    Hgeo_dI += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI, 2);
                }
                pE = 1 - pI;
                for (int i = (int)mean; i < DocLength; i++)
                {
                    Hgeo_dE += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE, 2);
                }
                EDgeoq_d = Math.Pow(Hgeo_dI, q) - Math.Pow(Hgeo_dE, q);

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

                ED = EDnorq_d;

                return true;
            }
            catch (System.Exception ex)
            {
                return false;
            }
        }

        public bool EntropyDifference_Normal()
        {
            try
            {
                //计算mean值
                int DocLength = 0;
                Double mean = 0;

                foreach (int a in Distance)
                {
                    DocLength += a;
                }
                mean = 1.0 * DocLength / Frequency;

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

                foreach (int a in intrinsic)
                {
                    if (ha_intrinsic.Contains(a))
                    {
                        ha_intrinsic[a] = (int)ha_intrinsic[a] + 1;
                    }
                    else
                    {
                        ha_intrinsic.Add(a, 1);
                    }
                }
                foreach (int a in extrinsic)
                {
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

                //计算EDq(d)
                Double EDq_d = 0.0;
                int q = 2;
                EDq_d = Math.Pow(H_dI, q) - Math.Pow(H_dE, q);
                EDq_d = Math.Pow(H_dI, q) - Math.Pow(H_dE, q);
                if (EDq_d <= 0)
                {
                    ED = -1000;

                    return true;
                }

                //计算EDgeoq(d)
                Double EDgeoq_d = 0.0;
                Double Hgeo_dI = 0.0;
                Double pI = 0.0;
                Double Hgeo_dE = 0.0;
                Double pE = 0.0;
                for (int i = 1; i < (int)mean; i++)
                {
                    pI += (1 / mean) * Math.Pow(1 - 1 / mean, i - 1);
                }
                for (int i = 1; i < (int)mean; i++)
                {
                    Hgeo_dI += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pI, 2);
                }
                pE = 1 - pI;
                for (int i = (int)mean; i < DocLength; i++)
                {
                    Hgeo_dE += -((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, i - 1)) / pE, 2);
                }
                EDgeoq_d = Math.Pow(Hgeo_dI, q) - Math.Pow(Hgeo_dE, q);

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

                ED = EDnorq_d;

                return true;
            }
            catch (System.Exception ex)
            {
                return false;
            }
        }
    }
}
