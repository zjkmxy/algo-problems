/*

题目描述：

    PQ收集到了很多魔法石。

    魔法石一共有红黄蓝三种颜色，每块魔法石都有一个魔力值。PQ每种颜色的魔法石都有N块。

    传说，只要三块魔力值的和恰好等于M的三种魔法石各一块聚在一起时，就可以召唤出会长大人，并满足PQ的三个愿望。

    请问，PQ有多少种方法可以召唤出会长大人? 

输入：

    第一行,两个整数N,M. (0 < N <= 10000 , 0 < M <= 3 * 10 ^ 17)

    接下来三行,代表三种魔法石,每行N个整数,依次代表魔法石的魔力值. 所有魔力值的取值范围都在[0 , 10 ^ 17).

输出：

    符合条件的方案数. 保证结果在int范围内.

*/
/*
定理：对R的两良序子集A,B和点集P=AxB，以及任意负斜率直线l,我们可以采用如下策略遍历l∩P的全部点：
  1.取a=max(A)和b=min(B)，令Q0=(a,b)作为起点。pred(x)和succ(x)分别代表前驱和后继。
  2.重复以下操作直到无法重复：若Qi在l的右上侧，令Qi+1=(pred(a),b)；否则令Qi+1=(a,succ(b))。
证明：
  1.假设Q(a,b)是P中的点，按要求操作得到第一次穿过或重合l的点为Q'(a',b')，我们可以证明长方形
[a,a']*[b,b']中至多有一个点∈l∩P，且如果有那么这个点一定是Q'
  2.其他的不再证明，显而易见。
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

//比较操作
int comp(const void *a, const void *b)
{
	return (((PDATA)a)->val - ((PDATA)b)->val);
}

//集合的排序重叠操作，返回集合元素个数
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

//集合的笛卡尔积操作B*C->DESC 复杂度O(n^2)  不用这个反而更快
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

//二分查找操作
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

//直积二分查找法   O(nlgn)的
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

//游标查找法   O(n^2)的
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

//进行输入
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
