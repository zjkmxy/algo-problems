#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define MAXN 10005
#define MAXT 105

typedef struct _TEAM
{
	int no;
	int scnt;			//总题数
	int stime;			//总时间
}TEAM, *PTEAM;
TEAM arr[MAXT];
typedef struct _SUBMIT
{
	int tim, no, qno, res;
}SUBMIT, *PSUBMIT;
SUBMIT srr[MAXN];
int solvet[MAXN];
int submits[MAXN];
int rank[MAXT];
int n, q;

void submit(int tim, int no, int qno, int res)
{
	if(solvet[qno] > 0)  //已经AC的提交不处理
		return ;
	if(res == 0)				//错误提交
	{
		submits[qno]++;
	}else{
		solvet[qno] = tim;
		arr[no].scnt++;
		arr[no].stime += submits[qno] * 20 + tim;
	}
}

int comp(const void*A, const void *B)
{
	PTEAM a = &arr[*(int*)A], b = &arr[*(int*)B];
	if(a->scnt != b->scnt)
	{
		return b->scnt - a->scnt;
	}else if(a->stime != b->stime)
	{
		return a->stime - b->stime;
	}else{
		return a->no - b->no;
	}
}

int equalt(int a, int b)
{
	return (arr[a].scnt == arr[b].scnt && arr[a].stime == arr[b].stime);
}

int comps(const void*A, const void *B)
{
	PSUBMIT a = (PSUBMIT)A, b = (PSUBMIT)B;
	return a->tim - b->tim;
}

int domain(int cas)
{
	int i, j;
	int t, no, qno, res;
	char str[10];
	scanf("%d",&n);
	if(n == 0)
		return 0;
	for(i=1;i<=n;i++)
	{
		rank[i-1] = i;
		arr[i].no = i;
		arr[i].stime = 0;
		arr[i].scnt = 0;
	}
	q = 0;
	while(1)
	{
		scanf("%d",&t);
		if(t == 0)
			break;
		scanf("%d%d%s",&no,&qno,str);
		res = (str[0] == 'y');
		//submit(t, no, qno, res);
		srr[q].no = no;
		srr[q].qno = qno;
		srr[q].tim = t;
		srr[q].res = res;
		q++;
	}
	qsort(srr, q, sizeof(SUBMIT), comps);
	for(i=1;i<=n;i++)
	{
		for(j=0;j<MAXN;j++)
		{
			solvet[j] = 0;
			submits[j] = 0;
		}
		for(j=0;j<q;j++)
		{
			if(srr[j].no == i)
				submit(srr[j].tim, srr[j].no, srr[j].qno, srr[j].res);
		}
	}
	qsort(rank, n, sizeof(int), comp);
	printf("case %d:\n",cas);
	i = 0;
	while(i < n)
	{
		for(j=i;j<n;j++)
		{
			if(!equalt(rank[i], rank[j]))
				break;
			printf("%d %d %d %d\n",rank[j],arr[rank[j]].scnt,arr[rank[j]].stime,i+1);
		}
		if(j > 2)
			return 1;
		i = j;
	}

	return 1;
}

int main()
{
	int cas = 1;
	while(domain(cas))
		cas++;
	return 0;
}