#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<algorithm>

#define MAXL 10
#define MAXC 1000
#define MAXN 10005
typedef long long int LLI;

int follow[MAXN], next[MAXN], height[MAXN], value[MAXN];
double prob[MAXL][MAXC];  //每点概率
double prob2[MAXL][MAXC]; //二次概率
double scnt, scnt2;  //情况个数
int val[MAXL][MAXC];
int len[MAXL], cnt, mflr;  //链数目和长度，最大深度
int n, m;

void calc_prob()
{
	int i, j;
	double tmp;

	scnt = 0;
	scnt2 = 1;
	memset(prob, 0, sizeof(prob));
	memset(prob2, 0, sizeof(prob2));
	for(i=0;i<cnt;i++)
	{
		scnt = scnt + len[i] + len[i] * scnt;
		scnt2 *= len[i] + 1;
	}
	for(i=0;i<cnt;i++)
	{
		tmp = (scnt - len[i]) / (len[i] + 1.0);
		for(j=0;j<len[i];j++)
		{
			prob[i][j] = (len[i]-j) * (1.0 + tmp) / scnt;
			prob2[i][j] = (len[i]-j) / double(len[i] + 1);
		}
	}
}

double calc_exp()
{
	int i, j;
	int cnt1, cnt2;  //总个数，取个数
	int stat, sm;
	double ans = 0, sit;  //总答案，情况概率
	double sib;

	mflr = 0;
	for(i=0;i<cnt;i++)
		mflr = std::max(mflr, len[i]);
	for(i=0;i<mflr;i++)
	{
		cnt1 = 0;
		for(j=0;j<cnt;j++)
		{
			if(len[j] > i)
				cnt1++;
		}

		for(stat=0;stat<(1<<cnt);stat++)
		{
			cnt2 = 0;
			sm = 0;
			sit = sib = 1.0;
			for(j=0;j<cnt;j++)
			{
				if((stat & (1<<j))==0)
				{
					sit *= 1.0 - prob[j][i];
					sib *= 1.0 - prob2[j][i];
					continue;
				}
				sit *= prob[j][i];  //不需要特判是否存在
				sib *= prob2[j][i];
				sm += val[j][i];
				cnt2++;
			}
			sit *= sm;
			if(cnt2 > 1)
				sit = sit + sm * double(cnt2) / double(cnt1) * sib * scnt2 / scnt;
			ans += sit;
		}
	}
	return ans;
}

void init()
{
	int i, cur, ch;

	cnt = 0;
	for(i=0;i<n;i++)
	{
		if(next[i] >= 0)
			continue;
		
		cur = i;
		ch = 0;
		while(follow[cur] >= 0)
		{
			ch++;
			height[cur] = ch;
			cur = follow[cur];
		}
		ch++;
		height[cur] = ch;
		len[cnt] = ch;
		for(ch=0;next[cur]>=0;ch++,cur=next[cur])
		{
			val[cnt][ch] = value[cur];
		}
		val[cnt][ch] = value[cur];
		cnt++;
	}
}

void input()
{
	int i, a, b;
	scanf("%d%d",&n,&m);

	cnt = 0;
	memset(follow, -1, sizeof(follow));
	memset(next, -1, sizeof(next));
	for(i=0;i<n;i++)
	{
		scanf("%d",&value[i]);
	}
	for(i=0;i<m;i++)
	{
		scanf("%d%d",&a,&b);
		follow[b] = a;
		next[a] = b;
	}
	init();
	calc_prob();
}

int main()
{
	int T;
	scanf("%d",&T);
	while(T--)
	{
		input();
		printf("%.3lf\n",calc_exp());
	}
	return 0;
}
