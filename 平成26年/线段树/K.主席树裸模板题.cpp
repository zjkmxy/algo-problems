//ZOJ2112  30M 140ms
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define MAXN   50002
#define MAXM   10002
#define MAXS   60002
#define MAX2 2500000
using namespace std;

#define lowbit(x) (x&(-x))

struct Tree{
	int lchd, rchd;
	int num;
}tree[MAX2];
int cnt;             //tree���˶���

int n, m;            //����������ѯ��
int arr[MAXN];       //ԭ����
int num[MAXS];       //��ɢ����
int nn;              //��ɢ�����������ã�
int root[MAXN];      //[1..n]�洢ԭʼ���ڵ㣨�޸������߶�����ע�����Ҫ��һ����������״�����еĻ�ʱ����������ò�ƿռ�Ҳ������
int chgr[MAXN];      //[1..n]�洢�仯�ĸ��ڵ㣨��״�������޸������߶���=>��ϯ����

struct Oper{
	int i, j, k;     //k==0��ΪCij��������Qijk
}oper[MAXM];         //���߲�ѯ

int compi(const void* A, const void* B)
{
	int *a, *b;
	a = (int*)A;
	b = (int*)B;
	return (*a - *b);
}

int BiSearch(int x)
{
	int begin,end,mid;

	begin = 0;
	end = nn - 1;

	while(end > begin + 1)
	{
		mid = (begin + end) >> 1;
		if(num[mid] == x)
			return mid;
		if(num[mid] < x)
		{
			begin = mid;
		}else{
			end = mid;
		}
	}

	if(num[end] > x)
	{
		return begin;
	}else{
		return end;
	}
}

int build_tree(int l,int r)
{
	int id = cnt, mid = (l + r) >> 1;

	cnt++;
	tree[id].num = 0;
	if (l==r){
		tree[id].lchd = 0;
		tree[id].rchd = 0;
		return id;
	}
	tree[id].lchd = build_tree(l,mid);
	tree[id].rchd = build_tree(mid+1,r);
	return id;
}

int operate(int id,int l,int r, int val, bool ins)  //�ڵ�id�������޽磬��ֵ������/ɾ��
{
	int ret = cnt, mid = (l + r)>>1;
	cnt++;
	memcpy(&tree[ret], &tree[id], sizeof(Tree));
	if(ins)
		tree[ret].num++;
	else
		tree[ret].num--;
	if (l == r)
		return ret;

	if (val<=mid){
		tree[ret].lchd = operate(tree[id].lchd,l,mid,val, ins);
	}
	else{
		tree[ret].rchd = operate(tree[id].rchd,mid+1,r,val, ins);
	}

	return ret;
}

void change(int j, int t)
{
	int i, val;

	val = BiSearch(arr[j - 1]);
	arr[j - 1] = t;
	i = j;
	while(i <= n)
	{
		chgr[i] = operate(chgr[i], 0, nn-1, val, false);
		i += lowbit(i);
	}
	val = BiSearch(t);
	i = j;
	while(i <= n)
	{
		chgr[i] = operate(chgr[i], 0, nn-1, val, true);
		i += lowbit(i);
	}
}

int query(int p, int q, int k) //i-1,j==p,qΪ�˱����ظ�
{
	int tcnt, i;
	int l = 0, r = nn - 1, mid;
	int idl[100], idr[100], ndl = 1, ndr = 1;  //���Ҹ�����Ҫ��ѯ���߶������޸���+��ϯ����

	idl[0] = root[p];
	idr[0] = root[q];
	for(i=p;i>0;i-=lowbit(i))
	{
		idl[ndl] = chgr[i];
		ndl++;
	}
	for(i=q;i>0;i-=lowbit(i))
	{
		idr[ndr] = chgr[i];
		ndr++;
	}

	while(l != r)
	{
		tcnt = 0;
		//�ұ�ȫ����ȥ���ȫ��
		for(i=0;i<ndr;i++)
		{
			if(idr[i])
				tcnt += tree[tree[idr[i]].lchd].num;
		}
		for(i=0;i<ndl;i++)
		{
			if(idl[i])
				tcnt -= tree[tree[idl[i]].lchd].num;
		}
		mid = (l + r) >> 1;
		if(tcnt >= k)
		{
			//�������
			for(i=0;i<ndr;i++)
			{
				if(idr[i])
					idr[i] = tree[idr[i]].lchd;
			}
			for(i=0;i<ndl;i++)
			{
				if(idl[i])
					idl[i] = tree[idl[i]].lchd;
			}
			r = mid;
		}else{
			//����Һ���
			for(i=0;i<ndr;i++)
			{
				if(idr[i])
					idr[i] = tree[idr[i]].rchd;
			}
			for(i=0;i<ndl;i++)
			{
				if(idl[i])
					idl[i] = tree[idl[i]].rchd;
			}
			l = mid + 1;
			k -= tcnt;
		}
	}

	return l;
}

void input()
{
	int i, j;
	char op[3];

	scanf("%d%d",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
		num[i] = arr[i];
	}
	for(j=0;j<m;j++)
	{
		scanf("%s",op);
		if(op[0]=='Q')
		{
			scanf("%d%d%d",&oper[j].i,&oper[j].j,&oper[j].k);
		}else{
			scanf("%d%d",&oper[j].i,&oper[j].j);
			oper[j].k = 0;
			num[i] = oper[j].j;
			i++;
		}
	}
	nn = i;
	qsort(num, nn, sizeof(int), compi);
	i = 0;
	j = 0;
	while(j < nn)
	{
		num[i] = num[j];
		do{
			j++;
		}while((num[i]==num[j])&&(j < nn));
		i++;
	}
	nn = i;
	cnt = 0;
	root[0] = build_tree(0, nn - 1); //����ֵӦ����0������һ�ſ���
	for(i=1;i<=n;i++)
	{
		j = BiSearch(arr[i-1]);
		root[i] = operate(root[i-1], 0, nn-1, j, true);
	}
	for(i=0;i<=n;i++)
	{
		chgr[i] = root[0];   //�ޱ仯��ȫ������
	}
}

int main()
{
	int i, t, j;

	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		input();
		for(j=0;j<m;j++)
		{
			if(oper[j].k == 0)
			{
				change(oper[j].i, oper[j].j);
			}else{
				printf("%d\n",num[query(oper[j].i - 1, oper[j].j, oper[j].k)]);
			}
		}
	}

	return 0;
}