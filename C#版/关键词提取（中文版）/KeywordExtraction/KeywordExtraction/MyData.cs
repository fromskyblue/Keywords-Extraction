using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using KeywordExtractionAPI;
using System.Runtime.InteropServices;

namespace KeywordExtraction
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

    class MyFun
    {
        const string path = @"ICTCLAS50.dll";

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_Init", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ICTCLAS_Init(String sInitDirPath);

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_Exit", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ICTCLAS_Exit();

        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_ParagraphProcessAW", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ICTCLAS_ParagraphProcessAW(String sParagraph, [Out, MarshalAs(UnmanagedType.LPArray)]result_t[] result, eCodeType eCT, int bPOSTagged);
        
        [DllImport(path, CharSet = CharSet.Ansi, EntryPoint = "ICTCLAS_ImportUserDict", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ICTCLAS_ImportUserDict(String pszDictBuffer, int nLength, eCodeType eCT);
    }

    class MyData
    {
        public static string TheDoc;
        public static KeywordExtractionAPI.WORDSFRE[] WordsFre;
    }
}
