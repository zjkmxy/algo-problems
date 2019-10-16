#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXI 0xffff
typedef unsigned short int USHORT;
typedef int INTARR[];

int stra[30005],strb[30005];
USHORT lena, lenb;
USHORT Li[2][30005]; //Li[k][i]=Li[k]=min{j|LCS(A[1..i],B[1..j]=k)}，不存在为INF
/*
Li矩阵显然有如下的性质：
Li[1]<Li[2]<...<Li[m]
Li+1[k]<=Li[k]
Li[k]<Li+1[k+1]
Li+1[k]=min{j,Li[k]},j=min{j|A[i+1]=B[j]且j>Li[k-1]}
*/

//快速计算Li矩阵的方法，时间复杂度上界O(pm)，p=LCS(A[1..m],B[1..n])
USHORT LCS(INTARR A, INTARR B, USHORT m, USHORT n)
{
    int i, j, k, cur, tmp, limit, start = 0;

    cur = 0;
    memset(Li, 0, sizeof(Li));
    limit = MAXI;
    for(k=1;k<=m;k++)
    {
        i = start;
        Li[cur][i] = MAXI;
        for(;i<m;i++)
        {
            Li[cur][i+1] = Li[cur][i];
            if(i >= limit)
                Li[cur^1][i] = k - 1;
            tmp = (Li[cur][i] > n ? n + 1 : Li[cur][i]);
            for(j=Li[cur^1][i]+1;j<tmp;j++)
            {
                if(A[i] == B[j-1])
                {
                    Li[cur][i+1] = j;
                    break;
                }
            }
            if(Li[cur][i+1] > n)
                start++;
            else if(Li[cur][i+1] == k)
            {
                limit = i + 1;
                break;
            }
            
        }
        if(i >= m)
            limit = MAXI;
        if(Li[cur][m] > n)  //这里不需要特判limit
            break;

        cur ^= 1;
    }

    return (k - 1);
}

int input()
{
	int i;
    if(scanf("%d%d",&lena,&lenb)==EOF)
        return 0;
    if(lena == 0 && lenb == 0)
		return 0;
	for(i=0;i<lena;i++)
		scanf("%d",&stra[i]);
	for(i=0;i<lenb;i++)
		scanf("%d",&strb[i]);
    return 1;
}

int main()
{
	int cas = 0, ans;
    while(input())
    {
		cas++;
        if(lena >= lenb)
            ans = LCS(stra, strb, lena, lenb);
        else
           ans = LCS(strb, stra, lenb, lena);
		printf("Twin Towers #%d\nNumber of Tiles : %d\n",cas,ans);
    }
    return 0;
}