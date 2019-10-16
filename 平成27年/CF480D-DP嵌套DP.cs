using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_001
{
    class Program
    {
        static int[] parseInts()
        {
            return Console.ReadLine().Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).
                Select(x => int.Parse(x.Trim())).ToArray();
        }
        class Parcel : IComparable
        {
            public int inT, outT, w, s, v;
            public Parcel(int inT = 0, int outT = 0, int w = 0, int s = 0, int v = 0)
            {
                this.inT = inT;
                this.outT = outT;
                this.w = w;
                this.s = s;
                this.v = v;
            }
            public void read()
            {
                int[] temp = parseInts();
                inT = temp[0];
                outT = temp[1];
                w = temp[2];
                s = temp[3];
                v = temp[4];
            }
            public int CompareTo(object rhs)
            {
                if (outT != ((Parcel)rhs).outT)
                {
                    return outT.CompareTo(((Parcel)rhs).outT);
                }
                else
                {
                    return -inT.CompareTo(((Parcel)rhs).inT);
                }
            }
        }
        static void Main(string[] args)
        {
            int[] temp = parseInts();
            int n = temp[0], s = temp[1];
            Parcel[] parcel = new Parcel[n + 1];
            int[,] dp = new int[n + 1, s + 1];
            for (int i = 0; i < n; i++)
            {
                parcel[i] = new Parcel();
                parcel[i].read();
            }
            parcel[n] = new Parcel(0, 2 * n, 0, s, 0);
            Array.Sort(parcel);
            for (int i = 0; i <= n; i++)
            {
                int st = parcel[i].inT;
                for (int k = parcel[i].w; k <= s; k++)
                {
                    int cur = st;
                    int[] g = new int[2 * n + 1];   //不同时间下的最优值
                    int kk = Math.Min(k - parcel[i].w, parcel[i].s);    //残余承载量
                    g[cur] = 0;
                    //枚举时间进行DP
                    for (int j = 0; j < i; j++)
                    {
                        if (parcel[j].inT >= parcel[i].inT)
                        {
                            while (cur < parcel[j].outT)
                            {
                                g[cur + 1] = g[cur];
                                cur++;
                            }
                            g[cur] = Math.Max(g[cur], g[parcel[j].inT] + dp[j, kk]);
                        }
                    }
                    //最后得出当前承载量下的答案
                    dp[i, k] = g[cur] + parcel[i].v;
                }
            }
            Console.WriteLine(dp[n, s]);
            return;
        }
    }
}
