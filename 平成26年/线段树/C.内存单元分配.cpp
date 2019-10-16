//�������Delete���ﲻ�ԣ����Ǽ�ʹ�Ĺ���ҲĪ��WA�ˡ���
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

#define MAXN 60000
#define LCHD(id) (id << 1)
#define RCHD(id) ((id << 1) + 1)

enum COLOR {cRed, cBlack};
typedef unsigned long int ULONG;
typedef struct _MEMORY_AREA{
	ULONG st;
	ULONG ed;
	COLOR col;
	ULONG size;
	struct _MEMORY_AREA *parent;
	struct _MEMORY_AREA *left;
	struct _MEMORY_AREA *right;
}MEMORY_AREA, *PMEMORY_AREA;
typedef struct _MADDRESS_SPACE{
	ULONG low;
	ULONG size;
	MEMORY_AREA Nil;
}MADDRESS_SPACE, *PMADDRESS_SPACE;
typedef struct _SEG_TREE{
	ULONG left ,right;
	ULONG lval,rval,maxval; //����ʣ��ռ䣬�м����ʣ��ռ�
	int   cover;            //��ǣ�0ȫ�գ�1ȫ����-1���������´���
}SEG_TREE, *PSEG_TREE;

MEMORY_AREA ma_arr[MAXN*10];
SEG_TREE    tree[MAXN*10];
ULONG ma_cnt;
MADDRESS_SPACE space;
ULONG n, m;
vector<MEMORY_AREA> vec;

bool cmp(const MEMORY_AREA &a, const MEMORY_AREA &b)  
{  
	return a.st < b.st;  
}  

//����Ϊ�����ά������
void Rotate(PMEMORY_AREA pNil, PMEMORY_AREA x, bool left_rotate)
{
	PMEMORY_AREA y;

	//�����ӽڵ�
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

	//�����ڵ�
	y->parent = x->parent;
	if(x->parent == pNil) //���ڵ�
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

PMEMORY_AREA Locate(PMADDRESS_SPACE space, ULONG addr) 
{
	PMEMORY_AREA pNil = &space->Nil;
	PMEMORY_AREA node = space->Nil.left;
	while(node != pNil)
	{
		if(addr < node->st)
			node = node->left;
		else if(addr >= node->ed)
			node = node->right;
		else
			return node;
	}
	return NULL;
}

PMEMORY_AREA Minimum(PMEMORY_AREA pNil, PMEMORY_AREA node)
{
	while(node->left != pNil)
		node = node->left;
	return node;
}

PMEMORY_AREA Next(PMEMORY_AREA pNil, PMEMORY_AREA node)
{
	PMEMORY_AREA tmp;

	if(node == pNil)
		return pNil;
	if(node->right != pNil)
		return Minimum(pNil, node->right);
	tmp = node->parent;
	while((tmp != pNil) && (node == tmp->right))
	{
		node = tmp;
		tmp = tmp->parent;
	}
	return tmp;
}

void Transplant(PMEMORY_AREA pNil, PMEMORY_AREA u, PMEMORY_AREA v)
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
	v->parent = u->parent; //Ϊ��֮��Ĳ��������NILһ����ʱ�ĵĸ��ڵ㡣
	u = u->parent;
	while(u != pNil)
	{
		u->size = u->left->size + u->right->size + 1;
		u = u->parent;
	}
}

void Insert(PMEMORY_AREA pNil, ULONG st, ULONG ed)
{
	PMEMORY_AREA x,y,z;

	z = &ma_arr[ma_cnt];
	ma_cnt++;
	z->st = st;
	z->ed = ed;
	z->left = pNil;
	z->right = pNil;
	z->col = cRed;
	z->size = 1;

	//Ѱ�Ҳ���㲢����
	y = pNil;
	x = pNil->left;
	while(x != pNil)
	{
		y = x;
		x->size++;
		if(ed < y->st)
		{
			x = y->left;
		}else{
			x = y->right;
		}
	}
	z->parent = y;
	if(y == pNil) //���ڵ�
	{
		pNil->left = z;
		pNil->right = z;
	}else if(ed < y->st)
	{
		y->left = z;
	}else{
		y->right = z;
	}

	//���������ɫ�޲�����
	while(z->parent->col == cRed)
	{
		if(z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right; //��ڵ�
			if(y->col == cRed)  //���1����ɫ��ڵ�������ɫ
			{
				z->parent->col = cBlack;
				y->col = cBlack;
				z->parent->parent->col = cRed;
				z = z->parent->parent;
			}else{              //��ɫ��ڵ�
				if(z == z->parent->right) //���2�����������ң�����ֱ����ת
				{
					z = z->parent;
					Rotate(pNil, z, true);
				}
				//���2��������������ת��ɫ���
				z->parent->col = cBlack;
				z->parent->parent->col = cRed;
				Rotate(pNil, z->parent->parent, false);
				//Ȼ��ѭ���ͽ�����
			}
		}else{
			y = z->parent->parent->left;
			if(y->col == cRed)
			{
				z->parent->col = cBlack;
				y->col = cBlack;
				z->parent->parent->col = cRed;
				z = z->parent->parent;
			}else{
				if(z == z->parent->left)
				{
					z = z->parent;
					Rotate(pNil, z, false);
				}
				z->parent->col = cBlack;
				z->parent->parent->col = cRed;
				Rotate(pNil, z->parent->parent, true);
			}
		}
	}
	if(pNil->left->col != cBlack)
	{
		pNil->left->col = cBlack; //���ڵ��Ǻ�ɫ��
	}
}

void rb_del_fix(PMEMORY_AREA pNil, PMEMORY_AREA x)
{
	PMEMORY_AREA w; //w��x���ֵܽڵ�

	while((x != pNil->left)&&(x->col == cBlack))
	{
		if(x == x->parent->left)
		{
			w = x->parent->right;
			if(w->col == cRed) //���1���ֵܺ죬��ֶ�ڣ�ת��Ϊ2��3��4
			{
				w->col = cBlack;
				x->parent->col = cRed;
				Rotate(pNil, x->parent, true);
				w = x->parent->right;
			}
			if((w->left->col == cBlack)&&(w->right->col == cBlack)) //���2���ֵܼ���ֶ�Ժڣ�������ɫ���ơ�
			{
				w->col = cRed;
				x = x->parent;
			}else{
				if(w->right->col == cBlack){ //���3���ֵܼ���ֶ�Ժڣ���������4��
					w->left->col = cBlack;
					w->col = cRed;
					Rotate(pNil, w, false);
					w = x->parent->right;
				}
				//���4���ֵܺڣ���ֶ�죬���������
				w->col = x->parent->col;
				if(x->parent->col != cBlack)
				{
					x->parent->col = cBlack;
				}
				w->right->col = cBlack;
				Rotate(pNil, x->parent, true);
				break;
			}
		}else{
			w = x->parent->left;
			if(w->col == cRed)
			{
				w->col = cBlack;
				x->parent->col = cRed;
				Rotate(pNil, x->parent, false);
				w = x->parent->left;
			}
			if((w->right->col == cBlack)&&(w->left->col == cBlack))
			{
				w->col = cRed;
				x = x->parent;
			}else{
				if(w->left->col == cBlack){
					w->right->col = cBlack;
					w->col = cRed;
					Rotate(pNil, w, true);
					w = x->parent->left;
				}
				w->col = x->parent->col;
				if(x->parent->col != cBlack)
				{
					x->parent->col = cBlack;
				}
				w->left->col = cBlack;
				Rotate(pNil, x->parent, false);
				break;
			}
		}
	}
	if((pNil->left != x)&&(pNil->left->col == cRed))
	{
		pNil->col = cBlack;
	}
	x->col = cBlack;
}

void Delete(PMEMORY_AREA pNil, PMEMORY_AREA z)
{
	PMEMORY_AREA y,x;  //y�ǵ�ǰɾ���Ľڵ���������ڵĽڵ㡢x���ƶ���y��ԭʼλ�õĽڵ� ��Ϊy��Ψһ�ӽڵ��ΪNIL
	COLOR y_orig;

	y = z;
	y_orig = y->col;
	if(z->left == pNil)
	{
		x = z->right;
		Transplant(pNil, z, x);
	}else if(z->right == pNil)
	{
		x = z->left;
		Transplant(pNil, z, x);
	}else{
		y = Minimum(pNil, z->right); //Ѱ�Һ��
		y_orig = y->col;
		x = y->right;
		if(y->parent == z)
		{
			x->parent = y; //y��z���������ĳ��ϣ����ܸ�NILһ����ʱ�ĸ��ڵ㡣
		}else{
			Transplant(pNil, y, x);
			y->right = z->right;
			y->right->parent = y;
		}
		Transplant(pNil, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->size = y->left->size + y->right->size + 1;
		y->col = z->col;
	}
	if(y_orig == cBlack)  //�޲���������x��Ϊ��ɫ+ԭɫ
	{
		rb_del_fix(pNil, x);
	}
}

PMEMORY_AREA Get(PMEMORY_AREA pNil, PMEMORY_AREA st, ULONG rank)
{
	if((st->size < rank)||(rank == 0))
		return pNil;
	while(st != pNil)
	{
		if(st->left->size == rank - 1)
		{
			return st;
		}else if(st->left->size > rank - 1)
		{
			st = st->left;
		}else{
			rank -= (st->left->size + 1);
			st = st->right;
		}
	}
	return pNil;
}

//����Ϊ�߶���ά������
ULONG Length(int id)
{
	return (tree[id].right - tree[id].left + 1);
}

void build_tree(int id,ULONG l,ULONG r)
{
	int mid = (l + r) >> 1;

	tree[id].left = l;
	tree[id].right = r;
	tree[id].maxval = r - l + 1;
	tree[id].lval = tree[id].maxval;
	tree[id].rval = tree[id].maxval;
	tree[id].cover = 0;
	if (l==r){
		return;
	}

	build_tree(LCHD(id),l,mid);
	build_tree(RCHD(id),mid+1,r);
}

void update(int id,ULONG l,ULONG r, int cover)  //1��ʾռ�ã�0��ʾ��ռ��
{
	int mid = (tree[id].left + tree[id].right) >> 1;
	if (tree[id].left==l&&tree[id].right==r){
		tree[id].cover = cover;
		if(cover)
		{
			tree[id].lval = tree[id].rval = tree[id].maxval = 0;
		}else{
			tree[id].lval = tree[id].rval = tree[id].maxval = r - l + 1;
		}
		return;
	}
	if(tree[id].cover != -1)
	{
		tree[LCHD(id)].cover = tree[id].cover;
		if(tree[id].cover)
		{
			tree[LCHD(id)].lval = tree[LCHD(id)].rval = tree[LCHD(id)].maxval = 0;
		}else{
			tree[LCHD(id)].lval = tree[LCHD(id)].rval = tree[LCHD(id)].maxval = Length(LCHD(id));
		}
		tree[RCHD(id)].cover = tree[id].cover;
		if(tree[id].cover)
		{
			tree[RCHD(id)].lval = tree[RCHD(id)].rval = tree[RCHD(id)].maxval = 0;
		}else{
			tree[RCHD(id)].lval = tree[RCHD(id)].rval = tree[RCHD(id)].maxval = Length(RCHD(id));
		}
		tree[id].cover = -1;
	}
	if (r<=mid){
		update(LCHD(id),l,r,cover);
	}
	else if(l>mid){
		update(RCHD(id),l,r,cover);
	}
	else{
		update(LCHD(id),l,mid,cover);
		update(RCHD(id),mid+1,r,cover);
	}
	tree[id].lval = tree[LCHD(id)].lval;
	if(tree[id].lval == Length(LCHD(id)))
		tree[id].lval += tree[RCHD(id)].lval;
	tree[id].rval = tree[RCHD(id)].rval;
	if(tree[id].rval == Length(RCHD(id)))
		tree[id].rval += tree[LCHD(id)].rval;
	tree[id].maxval = max(tree[LCHD(id)].rval + tree[RCHD(id)].lval, max(tree[LCHD(id)].maxval, tree[RCHD(id)].maxval));
}

ULONG query(int id, ULONG len)
{
	while(tree[id].left != tree[id].right)
	{
		if(tree[id].maxval < len)
			return 0;
		if(tree[id].cover != -1)
		{
			tree[LCHD(id)].cover = tree[id].cover;
			if(tree[id].cover)
			{
				tree[LCHD(id)].lval = tree[LCHD(id)].rval = tree[LCHD(id)].maxval = 0;
			}else{
				tree[LCHD(id)].lval = tree[LCHD(id)].rval = tree[LCHD(id)].maxval = Length(LCHD(id));
			}
			tree[RCHD(id)].cover = tree[id].cover;
			if(tree[id].cover)
			{
				tree[RCHD(id)].lval = tree[RCHD(id)].rval = tree[RCHD(id)].maxval = 0;
			}else{
				tree[RCHD(id)].lval = tree[RCHD(id)].rval = tree[RCHD(id)].maxval = Length(RCHD(id));
			}
			tree[id].cover = -1;
		}

		if(tree[LCHD(id)].maxval >= len)
		{
			id = LCHD(id);
		}else if(tree[LCHD(id)].rval + tree[RCHD(id)].lval >= len)
		{
			return (tree[LCHD(id)].right - tree[LCHD(id)].rval + 1);
		}else{
			id = RCHD(id);
		}
	}
	return tree[id].left;
}

//����Ϊͨ�ú�������
ULONG FindGap(PMADDRESS_SPACE space, ULONG len)
{
	ULONG high = space->low + space->size - 1;
	PMEMORY_AREA pNil = &space->Nil;
	PMEMORY_AREA prev, node;

	//ȫ��
	if(space->Nil.left == pNil)
	{
		if(space->size >= len)
			return space->low;
		return 0;
	}

	prev = node = Minimum(pNil, space->Nil.left);
	//��ͷ����
	if(node->st - space->low >= len)
		return space->low;
	while(true)
	{
		node = Next(pNil, node);
		if(node == pNil)
			break;
		if(node->st - prev->ed > len)
			return (prev->ed + 1);
		prev = node;
	}
	//������
	if(high - prev->ed >= len)
		return (prev->ed + 1);
	return 0;
}

ULONG DoNew(PMADDRESS_SPACE space, ULONG len)
{
	ULONG ret = query(1, len);
	MEMORY_AREA tmp;
	vector<MEMORY_AREA>::iterator it;
	if(ret == 0)
		return 0;
	/*Insert(&space->Nil, ret, ret + len - 1);*/
	update(1, ret, ret + len - 1, 1);
	tmp.st = ret;
	tmp.ed = ret + len - 1;
	it = upper_bound(vec.begin(), vec.end(), tmp, cmp); 
	vec.insert(it, tmp);
	return ret;
}

ULONG DoFree(PMADDRESS_SPACE space, ULONG addr, ULONG *ed)
{
	PMEMORY_AREA node;
	ULONG ret;
	MEMORY_AREA tmp;
	vector<MEMORY_AREA>::iterator it;
	int tmp2;

	/*node = Locate(space, addr);
	if(node == NULL)
		return 0;
	ret = node->st;
	(*ed) = node->ed;
	Delete(&space->Nil, node);
	update(1, ret, *ed, 0);*/ 
	tmp.st = addr;
	tmp.ed = addr;
	it = upper_bound(vec.begin(), vec.end(), tmp, cmp); 
	tmp2 = it - vec.begin() - 1;
	if (tmp2 == - 1 || vec[tmp2].ed < addr)
		return 0;
	ret = vec[tmp2].st;
	(*ed) = vec[tmp2].ed;
	update(1, ret, *ed, 0);
	vec.erase(vec.begin() + tmp2); 
	return ret;
}

void DoReset(PMADDRESS_SPACE space)
{
	ma_cnt = 0;
	space->Nil.col = cBlack;
	space->Nil.size = 0;
	space->Nil.st = 0;
	space->Nil.ed = 0;
	space->Nil.left = &space->Nil;
	space->Nil.right = &space->Nil;
	space->Nil.parent = &space->Nil;
	space->low = 1;
	space->size = n;
	vec.clear();
}

ULONG DoGet(PMADDRESS_SPACE space, ULONG cnt)
{
	PMEMORY_AREA pNil = &space->Nil;
	PMEMORY_AREA node;

	/*node = Get(pNil, pNil->left, cnt);
	if(node == pNil)
		return 0;
	return node->st;*/
	if(cnt > vec.size())
		return 0;
	return vec[cnt-1].st;
}

void Operate()
{
	char op[10];
	ULONG arg, ret, ret2;

	scanf("%s",op);
	switch(op[0])
	{
	case 'N':
		scanf("%u",&arg);
		ret = DoNew(&space, arg);
		if(ret == 0)
		{
			printf("Reject New\n");
		}else{
			printf("New at %u\n",ret);
		}
		break;
	case 'F':
		scanf("%u",&arg);
		ret = DoFree(&space, arg, &ret2);
		if(ret == 0)
		{
			printf("Reject Free\n");
		}else{
			printf("Free from %u to %u\n",ret,ret2);
		}
		break;
	case 'G':
		scanf("%u",&arg);
		ret = DoGet(&space, arg);
		if(ret == 0)
		{
			printf("Reject Get\n");
		}else{
			printf("Get at %u\n",ret);
		}
		break;
	case 'R':
		DoReset(&space);
		update(1, 1, n, 0);
		printf("Reset Now\n");
		break;
	}
}

int main()
{
	ULONG i;

	while(scanf("%u%u",&n,&m)!=EOF)
	{
		DoReset(&space);
		build_tree(1, 1, n);
		for(i=0;i<m;i++)
		{
			Operate();
		}
		printf("\n");
	}

	return 0;
}