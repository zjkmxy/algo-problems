#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXLEN 200000
#define LOGMLN 18

char gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //公用
double lg2;
int lcp[MAXLEN][LOGMLN];

void make_sa1(char *str, int sa[], int rank[], int len)
{
	int i;
	int cnt[256];

	//计数排序
	memset(cnt, 0, sizeof(cnt));
	for(i=0;i<len;i++)
	{
		cnt[str[i]]++;
	}
	for(i=1;i<255;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[str[i]]--;
		sa[cnt[str[i]]] = i;
	}

	//取得排位
	rank[sa[0]] = 1;
	for(i=1;i<len;i++)
	{
		if(str[sa[i]] == str[sa[i-1]])
			rank[sa[i]] = rank[sa[i-1]];
		else
			rank[sa[i]] = rank[sa[i-1]] + 1;
	}
}

int getrank(int len, int rank[], int i)
{
	if(i < len)
		return rank[i];
	else
		return 0;
}

int make_sam(char *str, int sa[], int rank[], int k, int len) //基数排序i+k,i
{
	int i,*cnt,*temp;

	cnt = new int[len+1];
	temp = new int[len];
	//排序尾位
	memset(cnt, 0, sizeof(int) * (len + 1));
	for(i=0;i<len;i++)
	{
		cnt[getrank(len, rank, i+k)]++;
	}
	for(i=1;i<=len;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[getrank(len, rank, i+k)]--;
		temp[cnt[getrank(len, rank, i+k)]] = i;
	}

	//排序前位
	memset(cnt, 0, sizeof(int) * (len + 1));
	for(i=0;i<len;i++)
	{
		cnt[getrank(len, rank, temp[i])]++;
	}
	for(i=1;i<=len;i++)
	{
		cnt[i]+=cnt[i-1];
	}
	for(i=len-1;i>=0;i--)
	{
		cnt[getrank(len, rank, temp[i])]--;
		sa[cnt[getrank(len, rank, temp[i])]] = temp[i];
	}

	//取得排位
	temp[sa[0]] = 1;
	for(i=1;i<len;i++)
	{
		if((getrank(len, rank, sa[i])==getrank(len, rank, sa[i-1]))&&(getrank(len, rank, sa[i]+k)==getrank(len, rank, sa[i-1]+k)))
			temp[sa[i]] = temp[sa[i-1]];
		else
			temp[sa[i]] = temp[sa[i-1]] + 1;
	}
	memcpy(rank, temp, sizeof(int) * len);

	if(temp[sa[len-1]]==len)
		k = 0; //可以停止
	else
		k = k * 2;

	delete [] temp;
	delete [] cnt;
	return k;
}

void make_sa(char *str, int sa[], int rank[], int len)
{
	int m;

	make_sa1(str, sa, rank, len);
	for(m=1;m<len;m<<=1)
	{
		if(make_sam(str, sa, rank, m, len) == 0)
			break;
	}
	for(m=0;m<len;m++)
	{
		rank[m]--;  //原来是从1数的
	}
}

int getlcp(char *a, char *b)
{
	int i = 0;
	while(a[i] && b[i] && (a[i] == b[i]))
		i++;
	return i;
}

void calc_ht(char *str, int sa[], int rank[], int height[], int len)
{
	int i,l;

	height[0] = 0;
	if(rank[0])
	{
		height[rank[0]] = getlcp(str, str + sa[rank[0]-1]);
	}
	for(i=1;i<len;i++)
	{
		if(!rank[i])
			continue;
		if(height[rank[i-1]] <= 1)
		{
			height[rank[i]] = getlcp(str + i, str + sa[rank[i]-1]);
		}else{
			l = height[rank[i-1]] - 1;
			height[rank[i]] = l + getlcp(str + i + l, str + sa[rank[i]-1] + l);
		}
	}
}

void rmqst_init(int height[], int len)  //初始化lcp数组，二维数组不便传入
{
	int i,j,m;

	m = (int)(floor(log(len)/lg2));
	for(i=0;i<len;i++)
	{
		lcp[i][0] = height[i];
	}
	for(j=1;j<=m;j++)
	{
		for(i=len-1;i>=0;i--)
		{
			lcp[i][j] = lcp[i][j-1];
			if(i + (1<<(j-1)) < len)
				lcp[i][j]=min(lcp[i][j],lcp[i + (1<<(j-1))][j-1]);
		}
	}
}

int get_lcp(int a, int b)
{
	int m, tmp;

	if(b < a)
	{
		tmp = a;
		a = b;
		b = tmp;
	}
	m = (int)(floor(log(b-a)/lg2));
	return min(lcp[a+1][m], lcp[b-(1<<m)+1][m]);
}

int main()
{
	int i, j, len, len1;
	int maxl1, maxi1, maxl2, maxi2, curl;
	bool flag;

	lg2 = log(2.00);
	while(scanf("%s",gstr)!=EOF)
	{
		len1 = strlen(gstr);
		len = len1 * 2 + 1;
		gstr[len1] = '#';
		gstr[len] = 0;
		for(i=len1-1,j=len1+1;i>=0;i--,j++)
		{
			gstr[j] = gstr[i];
		}

		make_sa(gstr, gsa, grank, len);
		calc_ht(gstr, gsa, grank, gheight, len);
		rmqst_init(gheight, len);
		
		//奇数
		maxl1 = 0;
		for(i=0;i<len1;i++)
		{
			curl = get_lcp(grank[i], grank[len-1-i]);
			if(curl > maxl1)
			{
				maxl1 = curl;
				maxi1 = i;
			}
		}
		maxi1 -= (maxl1 - 1);
		maxl1 = maxl1 * 2 - 1;

		//偶数
		maxl2 = 0;
		maxi2 = 0;
		for(i=1;i<len1;i++)
		{
			curl = get_lcp(grank[i-1], grank[len-1-i]);
			if(curl > maxl2)
			{
				maxl2 = curl;
				maxi2 = i;
			}
		}
		maxi2 -= (maxl2 - 1);
		maxl2 = (maxl2 - 1) * 2;

		if(maxl1 < maxl2)
		{
			maxi1 = maxi2;
			maxl1 = maxl2;
		}
		for(i=0;i<maxl1;i++)
		{
			printf("%c",gstr[maxi1+i]);
		}
		printf("\n");
	}

	return 0;
}
