/*
POJ3693
��Ŀ����:

һ���ַ������ظ���x�ĺ�����, ������������x����ͬ���ַ������Ӷ���. ���ڸ���һ������ΪN���ַ���, N������100000, ��������ظ���������ֵ�����С���Ӵ�.

��Ҫ����:

������, ˼·������˼. ���Ƕ����ظ����ַ���Ϊѭ����, ��ô���Ǿ�ö��ѭ���ڳ���L, ��ô����𰸴���, ��ض�����s[0], s[L], s[2 * L]�������ڵ�����. ������ö��j, ���൱�����Ǽٶ�s[j * L]��s[(j + 1) * L]�ڴ���, ��s[j * L]�ڵ�һ��ѭ������. ����������Ҫ���׺j * L�ͺ�׺(j + 1) * L�������ǰ׺, ��������ú�׺�����ST�㷨����Ԥ�����O(1)�ش�. ��LCP����ΪM, �����ȻΪM / L + 1, ���ⲻһ������õ�, ��Ϊ�𰸵���β��һ��������ö�ٵ�λ����. �ҵĽ��������, ���ǿ���M % L��ֵ������, ���ǿ�����Ϊ�Ǻ������M % L���ַ�, �������Ǹ��������ǰ������(L - M % L)���ַ�! �����������׺j * L - (L - M % L)���׺(j + 1) * L - (L - M % L)�������ǰ׺, ���Ƿ�С��L. �����ֵ���ı�֤, �ҵ��㷨û�б�֤���Ӷ�, ��Ϊ���Ǵ�ÿ��j * Lλ�ñ�����ǰfor�𰸵ĳ�ʼλ��, �𰸵ıȽ��ú�׺����ͺ�, �м���ϸ��ּ�֦�ܵû��պ�. �ں����Ǹ������������, �㷨���Ӷ�ΪO(N * (1 / 1 + 1 / 2 + ... + 1 / N)) = O(NlogN).
==================================================================
�����Ļ�ע�⣺
dacbacbacbaa
dbacbacbacbb
�������acbacbacb��Ҫ���cbacbacba����bacbacbac
��
sdafg
�����һ��a

*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;

#define MAXLEN 200000
#define LOGMLN 18

//����ȫ�ֹ���
char gstr[MAXLEN];
int gsa[MAXLEN],grank[MAXLEN],gheight[MAXLEN];
double lg2;
int lcp[MAXLEN][LOGMLN];

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

void domain(int len)
{
	int l, i, m, k;                //ö�ٳ��ȣ�����λ�ã�lcp��ǰ��lcp
	int rep, ans, pos, anslen;     //��ǰ�ظ���������С�ظ���������λ�ã��𰸳���
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
			//������Լ�����֦��Ҳ���Բ�����������һ�����ϵ�ʱ����š���
			for(t=max(i-l+1,0);t<=i;t++)
			{
				k = get_lcp(grank[t], grank[t + l]);
				rep = k / l + 1;
				if(rep > ans)
				{
					ans = rep;
					pos = t;
					anslen = l;
				}else if(rep == ans) //�ֵ���Ƚ�
				{
					if(grank[t] < grank[pos])
					{
						ans = rep;
						pos = t;
						anslen = l;
					}else if(grank[t] == grank[pos])
					{
						//��ʵ�ϲ�Ӧ�ô������ֿ�����
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