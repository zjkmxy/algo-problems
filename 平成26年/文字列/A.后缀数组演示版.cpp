#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 300000

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

void make_sa(char *str, int sa[], int rank[])
{
	int m ,len;

	len = strlen(str);
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

void calc_ht(char *str, int sa[], int rank[], int height[])
{
	int i,len,l;

	len = strlen(str);
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

void calheight(char *str,int *sa,int *rank,int *height,int len)  //基本不是人看的……
{
	int i,j,k=0;
	for(i=0;i<len;height[rank[i++]]=k)
		for(k?k--:0,j=sa[rank[i]-1];str[i+k]==str[j+k];k++) ;
	return;
}

int main()
{
	int i;

	while(scanf("%s",gstr)!=EOF)
	{
		make_sa(gstr, gsa, grank);
		calc_ht(gstr, gsa, grank, gheight);
		for(i=0;i<strlen(gstr);i++)
			printf("%s : %d\n",gstr + gsa[i], gheight[i]);
	}

	return 0;
}