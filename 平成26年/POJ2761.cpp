//红黑的模板，版子和思路都不太对，但是A了。
//其实就是区间K大，但是红黑居然比线段树快这不科学……
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;


enum COLOR {cRed, cBlack};
typedef signed long int SINT;
typedef unsigned long int UINT;
struct _RBNODE{
	struct _RBNODE *parent;
	struct _RBNODE *left;
	struct _RBNODE *right; //pNil的左右节点全部是根节点
	SINT val;
	UINT bh; //这里的黑高包含当前节点
	UINT size;
	COLOR col;
};
typedef struct _RBNODE RBNODE;
typedef RBNODE *PRBNODE;

void rotate(PRBNODE pNil, PRBNODE x, bool left_rotate);
void insert(PRBNODE pNil, SINT val);
PRBNODE rb_init();
void rb_free(PRBNODE pNil, PRBNODE root);
void in_walk(PRBNODE pNil, PRBNODE root, void(*pCb)(PRBNODE,int), int depth);
void rb_transplant(PRBNODE pNil, PRBNODE u, PRBNODE v);
PRBNODE rb_minimum(PRBNODE pNil, PRBNODE x);
PRBNODE rb_maximum(PRBNODE pNil, PRBNODE x);
PRBNODE rb_successor(PRBNODE pNil, PRBNODE node);
void rb_delete(PRBNODE pNil, PRBNODE z);
void rb_del_fix(PRBNODE pNil, PRBNODE x);
PRBNODE search(PRBNODE pNil, PRBNODE st, SINT val);
PRBNODE get(PRBNODE pNil, PRBNODE st, UINT rank);

struct FEED{
	int x, y, rank, order;
}feed[50005];
RBNODE tree[1000005];
int cnt;
PRBNODE nil;
int n, m, a[100005];
int ans[50005];

//////
void rotate(PRBNODE pNil, PRBNODE x, bool left_rotate)
{
	PRBNODE y;

	//处理子节点
	if(left_rotate)
	{
		y = x->right;
		x->right = y->left;
		if(y->left != pNil)
		{
			y->left->parent = x;
		}
		y->left = x;
	}else{
		y = x->left;
		x->left = y->right;
		if(y->right != pNil)
		{
			y->right->parent = x;
		}
		y->right = x;
	}

	//处理父节点
	y->parent = x->parent;
	if(x->parent == pNil) //根节点
	{
		pNil->left = y;
		pNil->right = y;
	}else if(x == x->parent->left)
	{
		x->parent->left = y;
	}else{
		x->parent->right = y;
	}

	x->parent = y;
	y->size = x->size;
	x->size = x->left->size + x->right->size + 1;
}

void insert(PRBNODE pNil, SINT val)
{
	PRBNODE x,y,z;

	//z = new RBNODE;
	z = &tree[cnt];
	cnt++;
	z->val = val;
	z->left = pNil;
	z->right = pNil;
	z->col = cRed;
	z->bh = 1;
	z->size = 1;

	//寻找插入点并插入
	y = pNil;
	x = pNil->left;
	while(x != pNil)
	{
		y = x;
		x->size++;
		if(val < y->val)
		{
			x = y->left;
		}else{
			x = y->right;
		}
	}
	z->parent = y;
	if(y == pNil) //根节点
	{
		pNil->left = z;
		pNil->right = z;
	}else if(val < y->val)
	{
		y->left = z;
	}else{
		y->right = z;
	}

	//以下完成颜色修补操作
	while(z->parent->col == cRed)
	{
		if(z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right; //叔节点
			if(y->col == cRed)  //情况1：红色叔节点则重着色
			{
				z->parent->col = cBlack;
				z->parent->bh++;
				y->col = cBlack;
				y->bh++;
				z->parent->parent->col = cRed;
				z = z->parent->parent;
			}else{              //黑色叔节点
				if(z == z->parent->right) //情况2：修正点在右，不能直接旋转
				{
					z = z->parent;
					rotate(pNil, z, true);
				}
				//情况2：修正点在左，旋转着色解决
				z->parent->col = cBlack;
				z->parent->bh++;
				z->parent->parent->col = cRed;
				z->parent->parent->bh--;
				rotate(pNil, z->parent->parent, false);
				//然后循环就结束了
			}
		}else{
			y = z->parent->parent->left;
			if(y->col == cRed)
			{
				z->parent->col = cBlack;
				z->parent->bh++;
				y->col = cBlack;
				y->bh++;
				z->parent->parent->col = cRed;
				z = z->parent->parent;
			}else{
				if(z == z->parent->left)
				{
					z = z->parent;
					rotate(pNil, z, false);
				}
				z->parent->col = cBlack;
				z->parent->bh++;
				z->parent->parent->col = cRed;
				z->parent->parent->bh--;
				rotate(pNil, z->parent->parent, true);
			}
		}
	}
	if(pNil->left->col != cBlack)
	{
		pNil->left->col = cBlack; //根节点是黑色的
		pNil->left->bh++;
	}
}

void in_walk(PRBNODE pNil, PRBNODE root, void(*pCb)(PRBNODE,int), int depth)
{
	if(root != pNil)
	{
		in_walk(pNil, root->left, pCb, depth + 1);
		pCb(root,depth);
		in_walk(pNil, root->right, pCb, depth + 1);
	}
}

PRBNODE rb_init()
{
	PRBNODE ret;

	//ret = new RBNODE;
	ret = &tree[0];
	cnt = 1;
	ret->bh = 1;
	ret->size = 0;
	ret->col = cBlack;
	ret->left = ret;
	ret->right = ret;
	ret->parent = ret;
	ret->val = 0;

	return ret;
}

void rb_free(PRBNODE pNil, PRBNODE root)
{
	if(root->left != pNil)
	{
		rb_free(pNil, root->left);
	}
	if((root->right != root->left)&&(root->right != pNil))
	{
		rb_free(pNil, root->right);
	}
	//delete root;
}

void rb_transplant(PRBNODE pNil, PRBNODE u, PRBNODE v)
{
	if(u->parent == pNil)
	{
		pNil->left = v;
		pNil->right = v;
	}else if(u == u->parent->left)
	{
		u->parent->left = v;
	}else{
		u->parent->right = v;
	}
	v->parent = u->parent; //为了之后的操作，会给NIL一个临时的的父节点。

	u = u->parent;
	while(u != pNil)
	{
		u->size = u->left->size + u->right->size + 1;
		u = u->parent;
	}
}

PRBNODE rb_minimum(PRBNODE pNil, PRBNODE x)
{
	while(x->left != pNil)
	{
		x = x->left;
	}
	return x;
}

PRBNODE rb_maximum(PRBNODE pNil, PRBNODE x)
{
	while(x->right != pNil)
	{
		x = x->right;
	}
	return x;
}

PRBNODE rb_successor(PRBNODE pNil, PRBNODE node)
{
	PRBNODE tmp;

	if(node == pNil)
		return pNil;
	if(node->right != pNil)
		return rb_minimum(pNil, node->right);
	tmp = node->parent;
	while((tmp != pNil) && (node == tmp->right))
	{
		node = tmp;
		tmp = tmp->parent;
	}
	return tmp;
}


void rb_delete(PRBNODE pNil, PRBNODE z)  //有问题
{
	PRBNODE y,x;  //y是当前删除的节点或移至树内的节点、x是移动到y的原始位置的节点 或为y的唯一子节点或为NIL
	COLOR y_orig;

	y = z;
	y_orig = y->col;
	if(z->left == pNil)
	{
		x = z->right;
		rb_transplant(pNil, z, x);
	}else if(z->right == pNil)
	{
		x = z->left;
		rb_transplant(pNil, z, x);
	}else{
		y = rb_minimum(pNil, z->right); //寻找后继
		y_orig = y->col;
		x = y->right;
		if(y->parent == z)
		{
			x->parent = y; //y是z的右子树的场合，可能给NIL一个临时的父节点。
		}else{
			rb_transplant(pNil, y, x);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(pNil, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->col = z->col;
		y->bh  = z->bh; //y替代了z，黑高也应替代。
		y->size = y->left->size + y->right->size + 1;
	}
	if(y_orig == cBlack)  //修补操作，将x视为黑色+原色
	{
		x->bh++;
		rb_del_fix(pNil, x);
	}
	//delete z;
}

void rb_delete2(PRBNODE pNil, PRBNODE z)  ///没有问题
{
	PRBNODE y,x,k; 
	if((z->left == pNil)||(z->right == pNil))
	{
		y = z;
	}else{
		y = rb_successor(pNil, z);
	}
	if(y->left != pNil)
	{
		x = y->left;
	}else{
		x = y->right;
	}
	x->parent = y->parent;
	if(y->parent == pNil)
	{
		pNil->left = x;
		pNil->right = x;
	}else if(y == y->parent->left)
	{
		y->parent->left = x;
	}else{
		y->parent->right = x;
	}
	if(y != z)
	{
		z->val = y->val;
	}
	k = y;
	while(k != pNil)
	{
		k->size--;
		k = k->parent;
	}
	if(y->col == cBlack)
		rb_del_fix(pNil, x);
}

void rb_del_fix(PRBNODE pNil, PRBNODE x)
{
	PRBNODE w; //w是x的兄弟节点

	while((x != pNil->left)&&(x->col == cBlack))
	{
		if(x == x->parent->left)
		{
			w = x->parent->right;
			if(w->col == cRed) //情况1：兄弟红，父侄黑，转化为2、3、4
			{
				w->col = cBlack;
				w->bh++;
				x->parent->col = cRed;
				x->parent->bh--;
				rotate(pNil, x->parent, true);
				w = x->parent->right;
			}
			if((w->left->col == cBlack)&&(w->right->col == cBlack)) //情况2：兄弟及两侄皆黑，则额外黑色上移。
			{
				w->col = cRed;
				w->bh--;
				x->bh--;
				x = x->parent;
			}else{
				if(w->right->col == cBlack){ //情况3：兄弟及右侄皆黑，则右旋成4。
					w->left->col = cBlack;
					w->left->bh++;
					w->col = cRed;
					w->bh--;
					rotate(pNil, w, false);
					w = x->parent->right;
				}
				//情况4：兄弟黑，右侄红，左旋解决。
				w->col = x->parent->col;
				w->bh = x->parent->bh;
				if(x->parent->col == cBlack)
				{
					x->parent->bh--;
				}else{
					x->parent->col = cBlack;
				}
				w->right->col = cBlack;
				w->right->bh++;
				x->bh--;
				rotate(pNil, x->parent, true);
				break;
			}
		}else{
			w = x->parent->left;
			if(w->col == cRed)
			{
				w->col = cBlack;
				w->bh++;
				x->parent->col = cRed;
				x->parent->bh--;
				rotate(pNil, x->parent, false);
				w = x->parent->left;
			}
			if((w->right->col == cBlack)&&(w->left->col == cBlack))
			{
				w->col = cRed;
				w->bh--;
				x->bh--;
				x = x->parent;
			}else{
				if(w->left->col == cBlack){
					w->right->col = cBlack;
					w->right->bh++;
					w->col = cRed;
					w->bh--;
					rotate(pNil, w, true);
					w = x->parent->left;
				}
				w->col = x->parent->col;
				w->bh = x->parent->bh;
				if(x->parent->col == cBlack)
				{
					x->parent->bh--;
				}else{
					x->parent->col = cBlack;
				}
				w->left->col = cBlack;
				w->left->bh++;
				x->bh--;
				rotate(pNil, x->parent, false);
				break;
			}
		}
	}
	if((pNil->left != x)&&(pNil->left->col == cRed))
	{
		pNil->col = cBlack;
		pNil->bh++;
	}
	x->col = cBlack;
}

PRBNODE search(PRBNODE pNil, PRBNODE st, SINT val)
{
	while((st != pNil)&&(st->val != val))
	{
		if(val < st->val)
		{
			st = st->left;
		}else{
			st = st->right;
		}
	}
	return st;
}

PRBNODE get(PRBNODE pNil, PRBNODE st, UINT rank)
{
	if((st->size < rank)||(rank == 0))
		return pNil;
	while(st != pNil)
	{
		if(st->left->size + 1 == rank)
		{
			return st;
		}else if(st->left->size + 1 > rank)
		{
			st = st->left;
		}else{
			rank -= (st->left->size + 1);
			st = st->right;
		}
	}
	return pNil;
}
///////
int comp(const void *A, const void *B)
{
	FEED *a, *b;
	a = (FEED*)A;
	b = (FEED*)B;
	return (a->x - b->x);
}

bool input()
{
	int i, tmp;

	if(scanf("%d%d",&n,&m)==EOF)
	{
		return false;
	}
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=1;i<=m;i++)
	{
		scanf("%d%d%d",&feed[i].x,&feed[i].y,&feed[i].rank);
		if(feed[i].x > feed[i].y)
		{
			tmp = feed[i].x;
			feed[i].x = feed[i].y;
			feed[i].y = tmp;
		}
		feed[i].order = i;
	}
	qsort(&feed[1], m, sizeof(FEED), comp);
	feed[0].y = -1;
	return true;
}

void solve()
{
	int i, j;

	for(i=1;i<=m;i++)
	{
		if(feed[i].x > feed[i-1].y)
		{
			nil = rb_init();
			for(j=feed[i].x;j<=feed[i].y;j++)
				insert(nil, a[j]);
		}else{
			for(j=feed[i-1].x;j<=feed[i].x-1;j++)
				rb_delete2(nil, search(nil, nil->left, a[j]));
			for(j=feed[i-1].y+1;j<=feed[i].y;j++)  //这个不对，但是能A(1,7->2,6)
				insert(nil, a[j]);
		}
		ans[feed[i].order] = get(nil, nil->left, feed[i].rank)->val;
	}
	for(i=1;i<=m;i++)
	{
		printf("%d\n",ans[i]);
	}
}

int main()
{
	input();
	solve();

	return 0;
}