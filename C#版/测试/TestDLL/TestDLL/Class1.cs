using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestDLL
{
    public class Class1
    {
        public int a;
        public void ShowMessage(int b)
        {
            Console.WriteLine("你以成功调用了动态连接！" + (a + b).ToString());
            Console.ReadLine();
        }
    }
}
