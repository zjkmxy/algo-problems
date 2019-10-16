/*

��Ŀ������

    PQ�ռ����˺ܶ�ħ��ʯ��

    ħ��ʯһ���к����������ɫ��ÿ��ħ��ʯ����һ��ħ��ֵ��PQÿ����ɫ��ħ��ʯ����N�顣

    ��˵��ֻҪ����ħ��ֵ�ĺ�ǡ�õ���M������ħ��ʯ��һ�����һ��ʱ���Ϳ����ٻ����᳤���ˣ�������PQ������Ը����

    ���ʣ�PQ�ж����ַ��������ٻ����᳤����? 

���룺

    ��һ��,��������N,M. (0 < N <= 10000 , 0 < M <= 3 * 10 ^ 17)

    ����������,��������ħ��ʯ,ÿ��N������,���δ���ħ��ʯ��ħ��ֵ. ����ħ��ֵ��ȡֵ��Χ����[0 , 10 ^ 17).

�����

    ���������ķ�����. ��֤�����int��Χ��.

*/
/*
������R���������Ӽ�A,B�͵㼯P=AxB���Լ����⸺б��ֱ��l,���ǿ��Բ������²��Ա���l��P��ȫ���㣺
  1.ȡa=max(A)��b=min(B)����Q0=(a,b)��Ϊ��㡣pred(x)��succ(x)�ֱ����ǰ���ͺ�̡�
  2.�ظ����²���ֱ���޷��ظ�����Qi��l�����ϲ࣬��Qi+1=(pred(a),b)��������Qi+1=(a,succ(b))��
֤����
  1.����Q(a,b)��P�еĵ㣬��Ҫ������õ���һ�δ������غ�l�ĵ�ΪQ'(a',b')�����ǿ���֤��������
[a,a']*[b,b']��������һ�����l��P�����������ô�����һ����Q'
  2.�����Ĳ���֤�����Զ��׼���
*/

#include<stdio.h>
#include<stdlib.h>
#include<new>
using namespace std;

typedef unsigned long int  UINT;
typedef unsigned long long int  LINT;
struct _DATA{
	LINT val;
	UINT num;
};
typedef struct _DATA DATA;
typedef struct _DATA *PDATA;

PDATA msa,msb,msc,mdesc;
UINT  nsa,nsb,nsc,ndesc;

//�Ƚϲ���
int comp(const void *a, const void *b)
{
	return (((PDATA)a)->val - ((PDATA)b)->val);
}

//���ϵ������ص����������ؼ���Ԫ�ظ���
UINT overlapify(PDATA dta, UINT n)
{
	UINT i,pos;

	qsort(dta, n, sizeof(DATA), comp);
	pos = 0;
	for(i=1;i<n;i++)
	{
		if(dta[i].val == dta[pos].val)
		{
			dta[pos].num += dta[i].num;
		}else{
			pos++;
			dta[pos].val = dta[i].val;
			dta[pos].num = dta[i].num;
		}
	}
	return pos+1;
}

//���ϵĵѿ���������B*C->DESC ���Ӷ�O(n^2)  ���������������
int product(LINT m)
{
	UINT i,j,k;

	ndesc = nsb * nsc;
	mdesc = new(std::nothrow) DATA [ndesc];
	if(mdesc == NULL)
		return 0;
	k = 0;
	for(i=0;i<nsb;i++)
	{
		for(j=0;j<nsc;j++)
		{
			mdesc[k].val = msb[i].val + msc[j].val;
			if(mdesc[k].val < m)
			{
				mdesc[k].num = msb[i].num * msc[j].num;
				k++;
			}
		}
	}
	ndesc = overlapify(mdesc, k);
	return 1;
}

//���ֲ��Ҳ���
UINT BiSearch(LINT res)
{
	UINT begin, end, mid;

	begin = 0;
	end = ndesc - 1;
	while(end > begin)
	{
		mid = (begin + end) / 2;
		if(mdesc[mid].val == res)
		{
			break;
		}
		if(mdesc[mid].val > res)
		{
			end = mid - 1;
		}else{
			begin = mid + 1;
		}
	}

	if(end < begin)
	{
		return 0;
	}
	if(end == begin)
	{
		mid = begin;
	}

	if(mdesc[mid].val != res)
	{
		return 0;
	}else{
		return mdesc[mid].num;
	}
}

//ֱ�����ֲ��ҷ�   O(nlgn)��
UINT search2(LINT m)
{
	UINT i,ret;

	ret = 0;
	for(i=0;i<nsa;i++)
	{
		if(msa[i].val + mdesc[0].val > m)
			continue;
		if(msa[i].val + mdesc[ndesc-1].val < m)
			continue;
		ret += msa[i].num * BiSearch(m - msa[i].val);
	}

	return ret;
}

//�α���ҷ�   O(n^2)��
UINT search(LINT m)
{
	UINT i,j,ret;
	signed long int k;
	LINT judge;

	ret = 0;
	for(i=0;i<nsa;i++)
	{
		if(msa[i].val + msb[0].val + msc[0].val > m)
			continue;
		if(msa[i].val + msb[nsb-1].val + msc[nsc-1].val < m)
			continue;
		j = 0;
		k = nsc - 1;
		while((j < nsb)&&(k >= 0))
		{
			judge = msa[i].val + msb[j].val + msc[k].val;
			if(judge > m)
			{
				k -= 1;
			}else if(judge < m)
			{
				j += 1;
			}else{
				ret += msa[i].num * msb[j].num * msc[k].num;
				k -= 1;
				j += 1;
			}
		}
	}

	return ret;
}

//��������
UINT doinput(PDATA dta,UINT n)
{
	UINT i,pos;

	for(i=0;i<n;i++)
	{
		scanf("%llu",&dta[i].val);
		dta[i].num = 1;
	}
	return overlapify(dta, n);
}

int main()
{
	UINT n;
	LINT m;

	scanf("%lu%llu",&n,&m);
	msa = new DATA [n];
	msb = new DATA [n];
	msc = new DATA [n];
	mdesc = NULL;
	nsa = doinput(msa, n);
	nsb = doinput(msb, n);
	nsc = doinput(msc, n);

	if(!product(m))
	{
		printf("%lu\n",search(m));
	}else{
		printf("%lu\n",search2(m));
		if(mdesc != NULL)
		{
			delete [] mdesc;
		}
	}

	delete [] msa;
	delete [] msb;
	delete [] msc;
	return 0;
}
