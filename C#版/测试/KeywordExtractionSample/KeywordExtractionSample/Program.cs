using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using KeywordExtractionAPI;
using System.Text.RegularExpressions;

namespace KeywordExtractionSample
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime dt1 = DateTime.Now;

            string path = "Document.txt";
            Encoding encoder = Encoding.GetEncoding("gb2312");
            WORDSFRE[] WordsFre;

            string TheDocument = File.ReadAllText(path, encoder);

            //Console.WriteLine("Start Statistics Document Information!");
            //TheDocument = KeywordExtractionAPI.MyFun.DocStandardization(TheDocument);

            //Console.WriteLine("Start Remove Stops!");
            //TheDocument = KeywordExtractionAPI.MyFun.RemoveStop(TheDocument);

            Console.WriteLine("Start Statistics Words!");
            WordsFre = KeywordExtractionAPI.MyFun.StatisticsWords(TheDocument);

            Console.WriteLine("Start Calculating The Entropy Difference!");
            foreach (WORDSFRE wf in WordsFre)
            {
                wf.EntropyDifference_Max();
            }

            Console.WriteLine("Start Sorting!");
            KeywordExtractionAPI.MyFun.QuickSort(WordsFre, 0, WordsFre.Length - 1);

            Console.WriteLine("\nResult：");
            for (int i = 0; i < 20 /*WordsFre.Length*/; i++)
            {
                if (WordsFre[i].ED == 0)
                {
                    break;
                }
                Console.WriteLine(WordsFre[i].Word + ":\t" + WordsFre[i].ED);
            }

            DateTime dt2 = DateTime.Now;
            Console.WriteLine("\nProgram run is complete, total elapsed time " + (dt2 - dt1) + ".");
            Console.ReadKey();
        }
    }
}
