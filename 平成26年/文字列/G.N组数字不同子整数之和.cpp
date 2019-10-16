/*
POJ4436
��Ŀ�������n���ַ�������������ַ����г��ֵĲ�ͬ��������mod2012��ֵ�������ֶ����һ�Σ�
���ȵ�Ȼ�ϲ���height
�����ʱ����ά�����ֺ�Ȼ������ķ��������Ż������¡�
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
typedef char CHAR; //ע��getlcp��Ĭ����0��β

CHAR gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //����
int gpos[MAXLEN];                               //ĳ�������Ӵ�ĩβλ��
int N, glen, mlen;                              //�������ܳ��ȣ���󳤶�
double lg2;
int lcp[MAXLEN][LOGMLN];
int gT[MAXLEN], gV[MAXLEN], gP[MAXLEN];          //ĳ��ţ���Ż��㷨
//gV[k]=�Ӵ�ͷ��kλ��ֵ��gT[k]=�Ӵ�ͷ��kλ���ۼ�ֵ
/*
Ʃ��˵��1234,V[0,1,2,3]={1,12,123,1234},T[0,1,2,3]={1,13,136,1470}
get(2,3)=3+34=T[3]-T[1]-V[1]*P[1]
*/

void make_sa1(CHAR *str, int sa[], int rank[], int len)
{
	int i;
	int *cnt;

	cnt = new int[RANGE+1];
	//��������
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

	//ȡ����λ
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

int make_sam(CHAR *str, int sa[], int rank[], int k, int len) //��������i+k,i
{
	int i,*cnt,*temp;

	cnt = new int[len+1];
	temp = new int[len];
	//����βλ
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

	//����ǰλ
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

	//ȡ����λ
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
		k = 0; //����ֹͣ
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
		rank[m]--;  //ԭ���Ǵ�1����
	}
}

int getlcp(CHAR *a, CHAR *b)
{
	int i = 0;
	while((a[i] != '#') && (b[i] != '#') && (a[i] == b[i]))  //��#����β
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

void rmqst_init(int height[], int len)  //��ʼ��lcp���飬��ά���鲻�㴫��
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

	mlen = 0;  //ȡ���
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

int calc(int st, int ed, int cal)  //���뿪ʼ�㣬�����㣨�����������㡣�˼���ض�TLE
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

int get(int st, int ed) //calc(st,ed,0)�ļ򻯼��㷽��
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
	int start; //1��ʼ�ĺ�׺���
	int ret = 0;
	int i;

	//Ѱ�ҵ�һ������0�ĺ�׺
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
