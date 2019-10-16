#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#define MAX 100005
using namespace std;

typedef signed long long int SLINT;

class TTree{
private:
	TTree *lchd, *rchd; //左右子树
	int ref;            //引用计数
public:
	int left, right, mid;  //左右限界，中间分界
	SLINT sum, inc;        //和，标记

	TTree(int l, int r, SLINT arr[]);
	TTree(const TTree* other);
	~TTree();

	TTree *update(int l, int r, SLINT val);  //无副作用更新
	TTree *updatef(int l, int r, SLINT val); //带副作用更新
	SLINT query(int l, int r);               //无副作用查询
	SLINT queryf(int l, int r);              //带副作用查询
};
typedef TTree* PTree;

TTree::TTree(int l, int r, SLINT arr[])
{
	left = l;
	right = r;
	mid = (l + r) >> 1;
	inc = 0;
	ref = 1;
	if(l == r)
	{
		sum = arr[l];
		lchd = NULL;
		rchd = NULL;
	}else{
		lchd = new TTree(l, mid, arr);
		rchd = new TTree(mid + 1, r, arr);
		sum = lchd->sum + rchd->sum;
	}
}

TTree::TTree(const TTree *other)
{
	left = other->left;
	right = other->right;
	mid = other->mid;
	sum = other->sum;
	inc = other->inc;
	ref = 1;
	lchd = other->lchd;
	rchd = other->rchd;
	if(lchd != NULL)
		lchd->ref++;
	if(rchd != NULL)
		rchd->ref++;
}

TTree::~TTree()
{
	if(lchd != NULL)
	{
		lchd->ref--;
		if(lchd->ref == 0)
			delete lchd;
	}
	if(rchd != NULL)
	{
		rchd->ref--;
		if(rchd->ref == 0)
			delete rchd;
	}
}

PTree TTree::updatef(int l, int r, SLINT val)
{
	if (left==l&&right==r){
		inc += val;
		return this;
	}
	sum += ((r - l + 1) * val);

	if (r<=mid){
		lchd->updatef(l,r,val);
	}
	else if(l>mid){
		rchd->updatef(l,r,val);
	}
	else{
		lchd->updatef(l,mid,val);
		rchd->updatef(mid + 1,r,val);
	}

	return this;
}

PTree TTree::update(int l, int r, SLINT val)
{
	PTree ret;

	ret = new TTree(this);
	if (left==l&&right==r){
		ret->inc += val;
		return ret;
	}
	ret->sum += ((r - l + 1) * val);

	if (r<=mid){
		lchd->ref--;  //去掉ret对它的引用
		ret->lchd = lchd->update(l,r,val);
	}
	else if(l>mid){
		rchd->ref--;
		ret->rchd = rchd->update(l,r,val);
	}
	else{
		lchd->ref--;
		ret->lchd = lchd->update(l,mid,val);
		rchd->ref--;
		ret->rchd = rchd->update(mid+1,r,val);
	}

	return ret;
}

SLINT TTree::query(int l, int r)
{
	PTree tmp;

	if (left==l&&right==r){
		return sum + (r - l + 1) * inc;//询问总和
	}
	else{
		if(inc > 0)
		{
			if(lchd->ref == 1)
			{
				//特殊情况：如果只有自己在引用这个子节点（无历史），那么没有必要建立新的孩子
				lchd->inc += inc;  
			}else{
				lchd->ref--;
				tmp = new TTree(lchd);
				tmp->inc += inc;
				lchd = tmp;
			}
			if(rchd->ref == 1)
			{
				rchd->inc += inc;
			}else{
				rchd->ref--;
				tmp = new TTree(rchd);
				tmp->inc += inc;
				rchd = tmp;
			}
			sum += (inc * (right - left + 1));
			inc = 0;
		}
	}
	if (r<=mid){
		return lchd->query(l,r);
	}
	else if(l>mid){
		return rchd->query(l,r);
	}
	else{
		return lchd->query(l,mid) + rchd->query(mid+1,r);
	}
}

SLINT TTree::queryf(int l, int r)
{
	if (left==l&&right==r){
		return sum + (r - l + 1) * inc;//询问总和
	}
	else{
		lchd->inc += inc;
		rchd->inc += inc;
		sum += (inc * (right - left + 1));
		inc = 0;
	}
	if (r<=mid){
		return lchd->queryf(l,r);
	}
	else if(l>mid){
		return rchd->queryf(l,r);
	}
	else{
		return lchd->queryf(l,mid) + rchd->queryf(mid+1,r);
	}
}

SLINT num[100005];
int n, q, cnt;
PTree tree[100005];

bool input()
{
	int i, tmp;

	if(scanf("%d%d",&n,&q)==EOF)
	{
		return false;
	}
	for(i=1;i<=n;i++)
	{
		scanf("%d",&tmp);
		num[i] = tmp;
	}
	tree[0] = new TTree(1, n, num);
	cnt = 1;

	return true;
}

void calc()
{
	char op[8];
	int a, b, c;

	scanf("%s",op);
	if(op[0] == 'Q')  //查询[a,b]之间的和
	{
		scanf("%d%d",&a,&b);
		printf("%lld\n",tree[cnt-1]->query(a, b));
	}else if(op[0] == 'C') //[a,b]+c
	{
		scanf("%d%d%d",&a,&b,&c);
		tree[cnt] = tree[cnt-1]->update(a, b, (SLINT)c);
		cnt++;
	}else if(op[0] == 'F')  //回滚a次修改操作
	{
		scanf("%d",&a);
		for(b=cnt-1;b>cnt-1-a;b--)
		{
			delete tree[b];
		}
		cnt -= a;
	}
}

int main()
{
	int i;

	while(input())
	{
		for(i=1;i<=q;i++)
		{
			calc();
		}
		for(i=0;i<cnt;i++)
		{
			delete tree[i];
		}
	}

	return 0;
}