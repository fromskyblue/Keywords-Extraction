using System;
using System.Collections.Generic;
using System.Text;
using SharpICTCLAS;

public class WordSegmentSample
{
   private int nKind = 1;  //在NShortPath方法中用来决定初步切分时分成几种结果
   private WordSegment wordSegment;

   //=======================================================
   // 构造函数，在没有指明nKind的情况下，nKind 取 1
   //=======================================================
   public WordSegmentSample(string dictPath) : this(dictPath, 1) { }

   //=======================================================
   // 构造函数
   //=======================================================
   public WordSegmentSample(string dictPath, int nKind)
   {
      this.nKind = nKind;
      this.wordSegment = new WordSegment();
      //wordSegment.PersonRecognition = false;
      //wordSegment.PlaceRecognition = false;
      //wordSegment.TransPersonRecognition = false;

      //---------- 订阅分词过程中的事件 ----------
      wordSegment.OnSegmentEvent += new SegmentEventHandler(this.OnSegmentEventHandler);
      wordSegment.InitWordSegment(dictPath);
   }

   //=======================================================
   // 开始分词
   //=======================================================
   public List<WordResult[]> Segment(string sentence)
   {
      return wordSegment.Segment(sentence, nKind);
   }
   
   //=======================================================
   // 输出分词过程中每一步的中间结果
   //=======================================================
   private void OnSegmentEventHandler(object sender, SegmentEventArgs e)
   {
      switch (e.Stage)
      {
         case SegmentStage.BeginSegment:
            Console.WriteLine("\r\n==== 原始句子：\r\n");
            //Console.WriteLine(e.Info + "\r\n");
            break;
         case SegmentStage.AtomSegment:
            Console.WriteLine("\r\n==== 原子切分：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.GenSegGraph:
            Console.WriteLine("\r\n==== 生成 segGraph：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.GenBiSegGraph:
            Console.WriteLine("\r\n==== 生成 biSegGraph：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.NShortPath:
            Console.WriteLine("\r\n==== NShortPath 初步切分的到的 N 个结果：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.BeforeOptimize:
            Console.WriteLine("\r\n==== 经过数字、日期合并等策略处理后的 N 个结果：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.OptimumSegment:
            Console.WriteLine("\r\n==== 将 N 个结果归并入OptimumSegment：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.PersonAndPlaceRecognition:
            Console.WriteLine("\r\n==== 加入对姓名、翻译人名以及地名的识别：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.BiOptimumSegment:
            Console.WriteLine("\r\n==== 对加入对姓名、地名的OptimumSegment生成BiOptimumSegment：\r\n");
            //Console.WriteLine(e.Info);
            break;
         case SegmentStage.FinishSegment:
            Console.WriteLine("\r\n==== 最终识别结果：\r\n");
            //Console.WriteLine(e.Info);
            break;
      }
   }
}
