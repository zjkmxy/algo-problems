/*
考虑每一个点可以被它和他的所有直接和间接的父亲删除，假设这些点一共有k个，那么它就有1/k的概率被炸开。
加和即可。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 1005
#define MAXM 1000005
#define LOGN 32
typedef unsigned long DWORD;
typedef DWORD MATRIX[MAXN][32];
MATRIX maparr;
int N, lbn;
int cnt[MAXN];

inline void setp(int p, int q)
{
    maparr[p][q>>5] |= (1 << (q&0x1F));
}
inline bool getp(int p, int q)
{
    return (maparr[p][q>>5] & (1 << (q&0x1F)));
}
int bitcnt(DWORD x)
{
    int res = 0, t, j;
    static int b[5] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff, 0x0000ffff};

    for(t=1,j=0;j<5;j++,t<<=1)
        x = (x & b[j]) + ((x >> t) & b[j]);
    return x;
}

void Warshall()
{
    int k, i, j;

    lbn = N >> 5;
    for(k=1;k<=N;k++)
    {
        for(i=1;i<=N;i++)
        {
            if(!getp(i, k))
                continue;
            for(j=0;j<=lbn;j++)
                maparr[i][j] |= maparr[k][j];
        }
    }
    for(i=1;i<=N;i++)
    {
        for(j=1;j<=N;j++)
        {
            if(getp(i, j))
                cnt[j]++;
        }
    }
}

double domain()
{
    int i, j, n, a;
    double ans = 0.0;

    scanf("%d",&N);
    memset(maparr, 0, sizeof(maparr));
    for(i=1;i<=N;i++)
    {
        setp(i, i);
        cnt[i] = 0;
        scanf("%d",&n);
        while(n--)
        {
            scanf("%d",&a);
            setp(i, a);
        }
    }
    Warshall();
    for(i=1;i<=N;i++)
    {
        ans += 1.0 / cnt[i];
    }
    return ans;
}

int main()
{
    int t, cas;
    scanf("%d",&t);
    for(cas=1;cas<=t;cas++)
    {
        printf("Case #%d: %.5lf\n",cas,domain());
    }
    return 0;
}
