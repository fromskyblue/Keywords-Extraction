using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace ictclas_seg
{
    enum eCodeType
    {
        CODE_TYPE_UNKNOWN,//type unknown
        CODE_TYPE_ASCII,//ASCII
        CODE_TYPE_GB,//GB2312,GBK,GB10380
        CODE_TYPE_UTF8,//UTF-8
        CODE_TYPE_BIG5//BIG5
    }

    [StructLayout(LayoutKind.Explicit)]
    //处理结果类型定义
    public struct result_t
    {
        [FieldOffset(0)]
        public int start;
        [FieldOffset(4)]
        public int length;
        [FieldOffset(8)]
        public int sPos;
        [FieldOffset(12)]
        public int sPosLow;
        [FieldOffset(16)]
        public int POS_id;
        [FieldOffset(20)]
        public int word_ID;
        [FieldOffset(24)]
        public int word_type;
        [FieldOffset(28)]
        public int weight;
    }

    class Program
    {
        const string path = @"ICTCLAS50.dll";

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_Init",CallingConvention=CallingConvention.Cdecl)]
        public static extern bool ICTCLAS_Init(String sInitDirPath);

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_Exit", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ICTCLAS_Exit();

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_ParagraphProcessAW", CallingConvention = CallingConvention.Cdecl)]
        private static extern int ICTCLAS_ParagraphProcessAW(String sParagraph, [Out, MarshalAs(UnmanagedType.LPArray)]result_t[] result, eCodeType eCT, int bPOSTagged);

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_ParagraphProcess", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ICTCLAS_ParagraphProcess(String sParagraph, int nPaLen, String sResult, eCodeType eCt, int bPOStagged);

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_ImportUserDict", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ICTCLAS_ImportUserDict(String pszDictBuffer, int nLength, eCodeType eCT);

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_SaveTheUsrDic", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ICTCLAS_SaveTheUsrDic();
        //文件分词
        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_FileProcess", CallingConvention = CallingConvention.Cdecl)]
        public static extern double ICTCLAS_FileProcess(String sSrcFilename, String sDsnFilename, eCodeType eCt, int bPOStagged);

        static void Main(string[] args)
        {
            // 系统初始化
            if (!ICTCLAS_Init(null))
            {
                System.Console.WriteLine("Init ICTCLAS failed!");
                System.Console.Read();
            }
            //导入用户字典
            string path = @"userdic.txt";
            Encoding encode = Encoding.GetEncoding("GB2312");
            string userdic = File.ReadAllText(path, encode);
            ICTCLAS_ImportUserDict(userdic, userdic.Length, eCodeType.CODE_TYPE_UNKNOWN);

            String s = "abc他说的确 实在理";
            int count = s.Length;//先得到结果的词数
            result_t[] result = new result_t[count];//在客户端申请资源
            int i = 0;

            //对字符串进行分词处理
            int nWrdCnt = ICTCLAS_ParagraphProcessAW(s, result, eCodeType.CODE_TYPE_GB, 1);
            result_t r;

            //取字符串真实长度
            byte[] mybyte = System.Text.Encoding.Default.GetBytes(s);
            byte[] byteWord;
            Console.WriteLine("No  start, length,POS_ID,Word_ID, UserDefine, Word,wordtype,weight\n");
            for (i = 0; i < nWrdCnt; i++)
            {
                r = result[i];
                String sWhichDic = "";

                switch (r.word_type)
                {
                    case 0:
                        sWhichDic = "核心词典";
                        break;
                    case 1:
                        sWhichDic = "用户词典";
                        break;
                    case 2:
                        sWhichDic = "专业词典";
                        break;
                    default:
                        break;
                }

                byteWord = new byte[r.length];

                //取字符串一部分
                Array.Copy(mybyte, r.start, byteWord, 0, r.length);
                string wrd = System.Text.Encoding.Default.GetString(byteWord);

                if (wrd==" ")
                {
                    continue;
                }

                Console.WriteLine("{0},{1},{2},{3},{4},{5},{6}\n", wrd, r.start, r.length, r.POS_id, r.word_ID, sWhichDic, r.weight);

            }

            //文本文件分词处理
            //ICTCLAS_FileProcess("test.txt", "Input_result.txt", eCodeType.CODE_TYPE_GB, 1);
            //释放资源退出
            ICTCLAS_Exit();

            Console.ReadKey();
        }
    }
}
