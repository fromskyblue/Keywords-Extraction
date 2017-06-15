using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace EntropyDifference
{
    class Program
    {
        static void Main(string[] args)
        {
            int[] d = { 1, 2, 1, 2, 3, 4, 5, 50, 2, 1, 3, 1, 2, 3, 2, 3, 100, 2, 1, 3, 1, 4, 3, 2, 1, 2, 1, 1, 1 };

            //计算mean值
            Double mean = 0;
            Console.Write("原始序列为：");
            foreach (int a in d)
            {
                Console.Write(a + ",");
                mean += a;
            }
            mean /= d.Length;
            Console.Write("\n\nmean值为：" + mean);

            //intrinsic mode 和 extrinsic mode
            int[] intrinsic;
            int[] extrinsic;
            int in_num = 0, ex_num = 0;
            foreach (int a in d)
            {
                if (a>mean)
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
            foreach (int a in d)
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
            Console.Write("\n\nintrinsic序列为：");
            foreach (int a in intrinsic)
            {
                Console.Write(a + ",");
                if (ha_intrinsic.Contains(a))
                {
                    ha_intrinsic[a] = (int)ha_intrinsic[a] + 1;
                }
                else
                {
                    ha_intrinsic.Add(a, 1);
                }
            }
            Console.Write("\n\nextrinsic序列为：");
            foreach (int a in extrinsic)
            {
                Console.Write(a + ",");
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
            Console.Write("\n\nintrinsic序列的熵 H(dI)=" + H_dI);
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
            Console.Write("\n\nextrinsic序列的熵 H(dE)=" + H_dE);

            //计算EDq(d)
            Double EDq_d=0.0;
            int q = 2;
            EDq_d = Math.Pow(H_dI, q) - Math.Pow(H_dE, q);
            Console.Write("\n\n熵差 EDq(d)=" + EDq_d);

            //计算EDgeoq(d)
            Double EDgeoq_d = 0.0;
            Double Hgeo_dI = 0.0;
            Double pI = 0.0;
            Double Hgeo_dE = 0.0;
            Double pE = 0.0;
            for (int i = 0; i < intrinsic.Length; i++)
            {
                pI += (1 / mean) * Math.Pow(1 - 1 / mean, intrinsic[i] - 1);
            }
            for (int i = 0; i < intrinsic.Length; i++)
            {
                Hgeo_dI += -((1 / mean) * Math.Pow(1 - 1 / mean, intrinsic[i] - 1)) / pI * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, intrinsic[i] - 1)) / pI, 2);
            }
            for (int i = 0; i < extrinsic.Length; i++)
            {
                pE += (1 / mean) * Math.Pow(1 - 1 / mean, extrinsic[i] - 1);
            }
            for (int i = 0; i < extrinsic.Length; i++)
            {
                Hgeo_dE += -((1 / mean) * Math.Pow(1 - 1 / mean, extrinsic[i] - 1)) / pE * Math.Log(((1 / mean) * Math.Pow(1 - 1 / mean, extrinsic[i] - 1)) / pE, 2);
            }
            EDgeoq_d = Math.Pow(Hgeo_dI, q) - Math.Pow(Hgeo_dE, q);
            Console.Write("\n\n熵差 EDgeoq(d)=" + EDgeoq_d);

            //计算EDnorq(d)
            Double EDnorq_d = 0.0;
            EDnorq_d = EDq_d / Math.Abs(EDgeoq_d);
            Console.Write("\n\n熵差 EDnorq(d)=" + EDnorq_d);

            Console.ReadKey();
        }
    }
}
