/*
POJ4436
题目大意给出n个字符串，求出所有字符串中出现的不同的整数和mod2012的值（即出现多次算一次）
首先当然合并求height
计算的时候用维护部分和然后减掉的方法可以优化，见下。
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXLEN 200000
#define LOGMLN 18
#define MAXN   10001
#define LOGMLN 18
#define RANGE  256
#define MODNUM 2012
typedef char CHAR; //注意getlcp中默认以0收尾

CHAR gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //公用
int gpos[MAXLEN];                               //某点所在子串末尾位置
int N, glen, mlen;                              //个数，总长度，最大长度
double lg2;
int lcp[MAXLEN][LOGMLN];
int gT[MAXLEN], gV[MAXLEN], gP[MAXLEN];          //某大牛的优化算法
//gV[k]=子串头到k位的值，gT[k]=子串头到k位的累加值
/*
譬如说，1234,V[0,1,2,3]={1,12,123,1234},T[0,1,2,3]={1,13,136,1470}
get(2,3)=3+34=T[3]-T[1]-V[1]*P[1]
*/

void make_sa1(CHAR *str, int sa[], int rank[], int len)
{
	int i;
	int *cnt;

	cnt = new int[RANGE+1];
	//计数排序
	memset(cnt, 0, sizeof(int) * (RANGE + 1));
	for(i=0;i<len;i++)
	{
		cnt[str[i]]++;
	}
	for(i=1;i<RANGE;i++)
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

	delete cnt;
}

int getrank(int len, int rank[], int i)
{
	if(i < len)
		return rank[i];
	else
		return 0;
}

int make_sam(CHAR *str, int sa[], int rank[], int k, int len) //基数排序i+k,i
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

void make_sa(CHAR *str, int sa[], int rank[], int len)
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

int getlcp(CHAR *a, CHAR *b)
{
	int i = 0;
	while((a[i] != '#') && (b[i] != '#') && (a[i] == b[i]))  //以#字收尾
		i++;
	return i;
}

void calc_ht(CHAR *str, int sa[], int rank[], int height[], int len)
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

bool input()
{
	int i,j,pos,clen;

	if(scanf("%d",&N)==EOF)
		return false;
	if(N == 0)
		return false;

	mlen = 0;  //取最大
	pos = 0;
	for(i=0;i<N;i++)
	{
		scanf("%s",gstr + pos);
		clen = strlen(gstr + pos);
		gpos[pos] = pos+clen-1;
		gV[pos] = gstr[pos] - '0';
		gT[pos] = gV[pos];
		for(j=pos+1;j<pos+clen;j++)
		{
			gpos[j] = pos+clen-1;
			gV[j] = gV[j-1] * 10 + gstr[j] - '0';
			gV[j] %= MODNUM;
			gT[j] = gT[j-1] + gV[j];
			gT[j] %= MODNUM;
		}
		if(clen > mlen)
			mlen = clen;
		pos += clen;
		gstr[pos] = '#';
		gV[pos] = 0;
		gT[pos] = 0;
		pos++;
		gstr[pos] = 0;
	}
	glen = pos;

	return true;
}

int calc(int st, int ed, int cal)  //传入开始点，结束点（含），计数点。此计算必定TLE
	//calc(st,ed,cal)=calc(st,ed,0)-calc(st,st+cal-1,0)
{
	int *T,i,ret = 0;

	if(ed < st)
		return 0;
	T = new int[mlen+1];

	T[0] = gstr[st]-'0';
	for(i=1;i<=ed-st;i++)
	{
		T[i] = T[i-1]*10+(gstr[st+i]-'0');
		T[i] %= MODNUM;
	}
	for(i=cal;i<=ed-st;i++)
	{
		ret += T[i];
		ret %= MODNUM;
	}

	delete [] T;

	return ret;
}

int get(int st, int ed) //calc(st,ed,0)的简化计算方法
{
	int ret;
	long long int tmp;

	if(ed < st)
		return 0;

	if(st == 0)
	{
		ret = gT[ed];
	}else{
		ret = gT[ed] - gT[st-1];
		ret %= MODNUM;
		ret -= gV[st-1]*gP[ed-st];
		ret %= MODNUM;
	}
	if(ret < 0)
		ret += MODNUM;

	return ret;
}

void domain()
{
	int start; //1开始的后缀起点
	int ret = 0;
	int i;

	//寻找第一个不是0的后缀
	for(start=N;start<glen;start++)
	{
		if(gstr[gsa[start]]!='0')
			break;
	}

	for(i=start;i<glen;i++)
	{
		//ret += calc(gsa[i],gpos[gsa[i]],gheight[i]);
		//ret += calc(gsa[i],gpos[gsa[i]],0) - calc(gsa[i],gsa[i]+gheight[i]-1,0);
		if(gsa[i] + gheight[i] <= gpos[gsa[i]])
		{
			ret += get(gsa[i],gpos[gsa[i]]) - get(gsa[i],gsa[i]+gheight[i]-1);
			ret %= MODNUM;
			if(ret < 0)
				ret += MODNUM;
		}
	}

	printf("%d\n",ret);
}

int main()
{
	int i = 0;

	gP[0] = 10;
	for(i=1;i<MAXLEN;i++)
		gP[i]=(gP[i-1]+1)*10%MODNUM;

	while(input())
	{
		make_sa(gstr, gsa, grank, glen);
		calc_ht(gstr, gsa, grank, gheight, glen);
		domain();
	}

	return 0;
}
