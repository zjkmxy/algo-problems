/*
用例：
in:
3
abcdefg
bcdefgh
cdefghi
3
xxx
yyy
zzz
4
aabcd
baacd
abaac
bddaa
5
aabcd
baacd
abaac
bddaa
aacadb
4
abbaa
aggaa
ajjaa
ahhaa
0

out:
bcdefg
cdefgh

?

aa

aac

aa
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 1005000
#define MAXN   105
#define RANGE  256
typedef char CHAR; //注意getlcp中默认以0收尾

CHAR gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //公用
int gpos[MAXLEN];                               //某点所在子串位置
int N, glen, mlen;

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
	while((a[i] != '#') && (b[i] != '#') && (a[i] == b[i]))
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

bool exist(CHAR *str, int sa[], int height[], int len, int sublen)
{
	int i,cnt;
	bool exs[MAXN];

	for(i=N;i<len;i++) //前几个元素都是以#开头的
	{
		if(height[i] < sublen)
			continue;

		memset(exs, 0, N);
		exs[gpos[sa[i-1]]] = true;
		cnt = 1;
		while((height[i] >= sublen)&&(i < len))
		{
			if(!exs[gpos[sa[i]]])
			{
				exs[gpos[sa[i]]] = true;
				cnt++;
				if(cnt > N / 2)
					return true;
			}
			i++;
		}
	}
	return false;
}

int bisearch(CHAR *str, int sa[], int height[])
{
	int start,end,mid;

	start = 0;
	end = mlen;

	while(end > start + 1)
	{
		mid = (start + end) / 2;
		if(exist(str, sa, height, glen, mid))
		{
			start = mid;
		}else{
			end = mid;
		}
	}

	if(exist(str, sa, height, glen, end))
		return end;
	else
		return start;
}

bool input()
{
	int i,j,pos,clen;

	scanf("%d",&N);
	if(N == 0)
		return false;

	mlen = MAXLEN;  //取最大
	pos = 0;
	for(i=0;i<N;i++)
	{
		scanf("%s",gstr + pos);
		clen = strlen(gstr + pos);
		for(j=pos;j<pos+clen;j++)
		{
			gpos[j] = i;
		}
		if(clen < mlen)
			mlen = clen;
		pos += clen;
		gstr[pos] = '#';
		pos++;
		gstr[pos] = 0;
	}
	glen = pos;

	return true;
}

void output(CHAR *str, int sa[], int height[], int len, int sublen)
{
	int i,cnt;
	bool exs[MAXN];

	for(i=N;i<len;i++) //前几个元素都是以#开头的
	{
		if(height[i] < sublen)
			continue;

		memset(exs, 0, N);
		exs[gpos[sa[i-1]]] = true;
		cnt = 1;
		while((height[i] >= sublen)&&(i < len))
		{
			if(!exs[gpos[sa[i]]])
			{
				exs[gpos[sa[i]]] = true;
				cnt++;
			}
			i++;
		}
		if(cnt > N / 2)  //可行解，输出sa[i-1]的sublen位
		{
			for(cnt=0;cnt<sublen;cnt++)
			{
				printf("%c",str[sa[i-1]+cnt]);
			}
			printf("\n");
		}
	}
}

int main()
{
	int n = 0;
	bool flag = false;

	while(input())
	{
		if(flag)
			printf("\n");
		make_sa(gstr, gsa, grank, glen);
		calc_ht(gstr, gsa, grank, gheight, glen);
		n = bisearch(gstr, gsa, gheight);
		if(n > 0)
			output(gstr, gsa, gheight, glen, n);
		else
			printf("?\n");
		flag = true;
	}

	return 0;
}
