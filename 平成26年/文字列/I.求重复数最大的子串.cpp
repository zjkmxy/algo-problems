/*
POJ3693
题目大意:

一个字符串的重复数x的含义是, 它可以由至多x个相同的字符串连接而成. 现在给出一个长度为N的字符串, N不超过100000, 求出它的重复数最大且字典序最小的子串.

简要分析:

论文题, 思路很有意思. 我们定义重复的字符串为循环节, 那么我们就枚举循环节长度L, 那么如果答案存在, 则必定包含s[0], s[L], s[2 * L]等中相邻的连个. 我们再枚举j, 则相当于我们假定s[j * L]和s[(j + 1) * L]在答案中, 且s[j * L]在第一个循环节内. 这里我们需要求后缀j * L和后缀(j + 1) * L的最长公共前缀, 这个可以用后缀数组和ST算法做好预处理后O(1)回答. 设LCP长度为M, 则答案显然为M / L + 1, 但这不一定是最好的, 因为答案的首尾不一定再我们枚举的位置上. 我的解决方法是, 我们考虑M % L的值的意义, 我们可以认为是后面多了M % L个字符, 但是我们更可以想成前面少了(L - M % L)个字符! 所以我们求后缀j * L - (L - M % L)与后缀(j + 1) * L - (L - M % L)的最长公共前缀, 看是否不小于L. 对于字典序的保证, 我的算法没有保证复杂度, 因为我是从每个j * L位置暴力往前for答案的初始位置, 答案的比较用后缀数组就好, 中间加上各种剪枝跑得还凑合. 在忽略那个暴力的情况下, 算法复杂度为O(N * (1 / 1 + 1 / 2 + ... + 1 / N)) = O(NlogN).
==================================================================
用例的话注意：
dacbacbacbaa
dbacbacbacbb
结果都是acbacbacb不要错成cbacbacba或者bacbacbac
而
sdafg
结果是一个a

*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXLEN 200000
#define LOGMLN 18

//以下全局公用
char gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];
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

	m = (int)(floor(log((double)len)/lg2));
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
	m = (int)(floor(log((double)(b-a))/lg2));
	return min(lcp[a+1][m], lcp[b-(1<<m)+1][m]);
}

void domain(int len)
{
	int l, i, m, k;                //枚举长度，所在位置，lcp，前移lcp
	int rep, ans, pos, anslen;     //当前重复次数，最小重复次数，答案位置，答案长度
	int t;

	ans = 1;
	anslen = 1;
	pos = 0;
	for(l=1;l<len;l++)
	{
		if(gstr[l]<gstr[pos])
			pos = l;
	}
	for(l=1;l<len;l++)
	{
		for(i=0;i+l<len;i+=l)
		{
			m = get_lcp(grank[i], grank[i + l]);
			if((m < l)||(m / l + 2 < ans))
				continue;
			//这里可以继续剪枝。也可以不剪。反正有一半以上的时间空着……
			for(t=max(i-l+1,0);t<=i;t++)
			{
				k = get_lcp(grank[t], grank[t + l]);
				rep = k / l + 1;
				if(rep > ans)
				{
					ans = rep;
					pos = t;
					anslen = l;
				}else if(rep == ans) //字典序比较
				{
					if(grank[t] < grank[pos])
					{
						ans = rep;
						pos = t;
						anslen = l;
					}else if(grank[t] == grank[pos])
					{
						//事实上不应该存在这种可能性
						if(anslen > l)
						{
							ans = rep;
							pos = t;
							anslen = l;
						}
					}
				}
			}

		}
	}

	for(i=0;i<anslen*ans;i++)
	{
		printf("%c",gstr[pos+i]);
	}
	printf("\n");
}

int main()
{
	int i = 0,len;

	lg2 = log(2.00);
	while(scanf("%s",gstr)!=EOF)
	{
		if(gstr[0]=='#')
			break;
		len = strlen(gstr);
		make_sa(gstr, gsa, grank, len);
		calc_ht(gstr, gsa, grank, gheight, len);
		rmqst_init(gheight, len);
		printf("Case %d: ",++i);
		domain(len);
	}

	return 0;
}