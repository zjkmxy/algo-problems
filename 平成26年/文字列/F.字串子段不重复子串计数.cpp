/*hdu4622
T������������l��r���ִ������ڲ��ظ����Ӵ�������
ÿ�ζ�make_height�Ļ��ض�TLE������Ĺؼ���������lcp�����ʣ�
��ĸ��rank���Ӵ�rank����Ӧ������µõ���Ч��lcp��
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
using namespace std;

#define MAXLEN 2005

char gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];  //����

void make_sa1(char *str, int sa[], int rank[], int len)
{
	int i;
	int cnt[256];

	//��������
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

	//ȡ����λ
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

int make_sam(char *str, int sa[], int rank[], int k, int len) //��������i+k,i
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
		rank[m]--;  //ԭ���Ǵ�1����
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
	int get,lcp,avh,avh2;  //����������Ч��׺�����lcpֵ��ǰһ��׺����Ч���ȣ���һ��׺��Ч����
	int lcp2;              //��Чlcp

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
			if((gsa[i]<l)||(gsa[i]>r)) //����Ҫ��ĺ�׺
			{
				lcp = min(lcp, gheight[i]);
			}else{                     //��Ч��׺
				lcp = min(lcp, gheight[i]);
				avh2 = r + 1 - gsa[i];
				lcp2 = lcp;
				lcp2 = min(lcp2, avh);  //lcp��Ӧ�ó���ǰһ�Ӵ�����Ч����
				lcp2 = min(lcp2, avh2); //lcp��Ӧ�ó�����һ�Ӵ�����Ч���ȣ�������56����ȥ��54��ʵ����55��
				get = avh2 - lcp2;  
				if((avh > avh2)&&(lcp >= avh2)); 
				//����˵����ǰ�Ӵ���������rank�Ŵ�ģ��ڵ�ǰ��r��Ӧ��������ν��ǰһ������ǰ�棬���Բ��ܸ�avh��lcp
				//����afafas(rank0)<afas(rank1)<afsafafas(rank2)��r���Ƶ�fas
				//ע�⵽����������˳����a(rank1)<afa(rank0)<afsafa(rank2)
				//������a���Զ���afa��afasȴ����afafas֮�������������ض���
				//�����ߵ��ظ�ǰ׺(afa)>�϶��ߵ���ʵ����(a)���ң��϶��ߵ�һ����ƥ����ַ�(s)>�ϳ��ߵ�һ����ƥ����ַ�(f)��
				//�������Դ���afas���ر��ǲ�����lcp�ļ����ܵ��䳤�ȵ�Ӱ�죬�Ϳ�������һ����Ч��׺�ļ���ʱ�õ���ʵ��lcp
				//��Ϊ ��Чlcp=min(��lcp,A��Ч����,B��Ч����)������rank��û��ϵ��������Сȡ���Ǹ������⣡
				else
				{
					avh = avh2;
					lcp = gheight[i+1]; //����ֻҪ���þͲ���Խ��
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
����������
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