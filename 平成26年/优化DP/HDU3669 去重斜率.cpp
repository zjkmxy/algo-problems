#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

typedef __int64 LL;
#define MAXN 50005
#define MAXK 105
struct DATA
{
    LL w, h;
}data[MAXN];
LL dp[MAXN][2];
int N, K, que[MAXN], front, back;

int comp(const void *A, const void *B)
{
    DATA *a, *b;
    a = (DATA*)A;
    b = (DATA*)B;
    if(a->h != b->h)
        return b->h - a->h;
    else
        return a->w - b->w;
}

bool input()
{
    int i, j;
    if(scanf("%d%d",&N,&K)==EOF)
        return false;

    for(i=1;i<=N;i++)
    {
        scanf("%I64d%I64d",&data[i].h,&data[i].w);
    }
    qsort(data+1, N, sizeof(DATA), comp);
    i = 1;
    j = 1;
    while(j <= N)
    {
        data[i] = data[j];
        do{
            j++;
        //}while((data[i].w>=data[j].w || data[i].h==data[j].h)&&(j<=N));
        }while((data[i].w>=data[j].w)&&(j<=N));
        i++;
    }
    N = i - 1;

    return true;
}

LL dp_calc()
{
    int i, j, x, y;
    int cur = 0;
    LL ans;

    for(i=1;i<=N;i++)
    {
        dp[i][cur] = data[1].h * data[i].w;
    }
    ans = dp[N][cur];
    for(j=2;j<=K;j++)
    {
        front = back = 0;
        que[0] = 0;
        for(i=1;i<=N;i++)
        {
            while(front < back && (dp[que[front]][cur] - dp[que[front+1]][cur]) >= -data[i].w * (data[que[front]+1].h - data[que[front+1]+1].h))
            {
                front++;
            }
            dp[i][cur^1] = dp[que[front]][cur] + data[i].w * data[que[front]+1].h;
            while(front < back)
            {
                x = que[back-1];
                y = que[back];
                if((dp[x][cur] - dp[y][cur]) * (data[y+1].h - data[i+1].h) <= (dp[y][cur] - dp[i][cur]) * (data[x+1].h - data[y+1].h))
                    back--;
                else
                    break;
            }
            back++;
            que[back] = i;
        }

        //if(dp[N][j] == dp[N][j-1])
        //    return dp[N][j];
        cur^=1;
        ans = min(ans, dp[N][cur]);
    }

    return ans;
}

int main()
{
    while(input())
    {
        printf("%I64d\n",dp_calc());
    }
    return 0;
}