/*hdu4622
T个用例，计算l到r的字串区间内不重复的子串个数。
每次都make_height的话必定TLE，解决的关键在于利用lcp的性质，
在母串rank和子串rank不对应的情况下得到有效的lcp。
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
using namespace std;

#define MAXLEN 2005

char gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //公用

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

void domain()
{
	int i,j,ret,l,r,Q;
	int len;
	int get,lcp,avh,avh2;  //贡献数，有效后缀间的满lcp值，前一后缀的有效长度，这一后缀有效长度
	int lcp2;              //有效lcp

	scanf("%s %d",gstr,&Q);
	len = strlen(gstr);
	make_sa(gstr, gsa, grank, len);
	calc_ht(gstr, gsa, grank, gheight, len);
	for(j=0;j<Q;j++)
	{
		scanf("%d%d",&l,&r);
		l--;
		r--;
		
		ret = 0;
		lcp = 0;  //gheight[0]==0
		avh = 0;
		for(i=0;i<len;i++)
		{
			if((gsa[i]<l)||(gsa[i]>r)) //不合要求的后缀
			{
				lcp = min(lcp, gheight[i]);
			}else{                     //有效后缀
				lcp = min(lcp, gheight[i]);
				avh2 = r + 1 - gsa[i];
				lcp2 = lcp;
				lcp2 = min(lcp2, avh);  //lcp不应该超过前一子串的有效长度
				lcp2 = min(lcp2, avh2); //lcp不应该超过这一子串的有效长度（加上是56，减去是54，实际是55）
				get = avh2 - lcp2;  
				if((avh > avh2)&&(lcp >= avh2)); 
				//！！说明当前子串根本就是rank排错的，在当前的r下应该排在所谓“前一个”的前面，所以不能改avh和lcp
				//例如afafas(rank0)<afas(rank1)<afsafafas(rank2)而r限制到fas
				//注意到了吗？真正的顺序是a(rank1)<afa(rank0)<afsafa(rank2)
				//所以在a明显短于afa而afas却排在afafas之后的特殊情况，必定有
				//（二者的重复前缀(afa)>较短者的真实长度(a)）且（较短者第一个不匹配的字符(s)>较长者第一个不匹配的字符(f)）
				//这样忽略处理afas，特别是不能让lcp的计算受到其长度的影响，就可以在下一个有效后缀的计算时得到真实的lcp
				//因为 有效lcp=min(满lcp,A有效长度,B有效长度)，所以rank错没关系，长度最小取错是根本问题！
				else
				{
					avh = avh2;
					lcp = gheight[i+1]; //这里只要不用就不怕越界
				}
				ret += get;
			}
		}

		printf("%d\n",ret);
	}
}

int main()
{
	int T,i;

	scanf("%d",&T);
	for(i=0;i<T;i++)
	{
		domain();
	}

	return 0;
}
/*
测试用例：
in:
4
afsafa
1
1 6
afsafafas
1
1 6
bbaba
5
3 4
2 2
2 5
2 4
1 4
baaba
5
3 3
3 4
1 4
3 5
5 5

out:
17
17
3
1
7
5
8
1
3
8
5
1
*/