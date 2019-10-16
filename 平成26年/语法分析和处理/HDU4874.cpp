#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <list>
using namespace std;

#define MAXK 1048580
#define MAXI 0x3fffffff
#define MAXN 100005

#define LCHD(x) (((x)<<1))
#define RCHD(x) (((x)<<1)+1)
#define MID(l,r)  (((l)+(r))>>1)

struct NODE
{
	int left, right;
	bool thru;
	int lsum, rsum, msum;
}tree[MAXN<<3];

int arr[MAXN], uparr[MAXN];  //下界，上界
bool vis[MAXK];
int n, k;
char token[100], lastch;
int tklen;

int read_const()
{
	char ch;
	int ret;

	if(lastch > 0)
	{
		if(lastch == EOF)
			return -1;
		ret = lastch - '0';
		lastch = 0;
	}else
		ret = 0;
	ch = getchar();
	if(ch == EOF)
		return -1;
	while(ch == ' ')
		ch = getchar();
	while(isdigit(ch))
	{
		ret = ret * 10 + ch - '0';
		ch = getchar();
	}
	if(ch != ' ' && ch != '\n' && ch != '\r')
		lastch = ch;
	return ret;
}

bool read_token()
{
	char ch;
	tklen = 0;
	if(lastch > 0)
	{
		if(lastch == EOF)
			return false;
		token[0] = lastch;
		lastch = 0;
		tklen++;
	}
	while(true)
	{
		if(tklen == 1)
		{
			if(token[0] == '{' || token[0] == '}')
				return true;
			if(token[0] == '[' || token[0] == ']')
				return true;
			if(token[0] == '(' || token[0] == ')')
				return true;
			if(token[0] == ',' || token[0] == ';')
				return true;
		}
		if(tklen == 2)
		{
			if(token[0] == '>' && token[1] == '=')
				return true;
		}
		ch = getchar();
		if(ch == ' ' || ch == '\n' || ch == '\r')
		{
			if(tklen > 0)
				return true;
			else
				continue;
		}
		if(ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ',' || ch == ';')
		{
			if(tklen > 0)
			{
				lastch = ch;
				return true;
			}
		}
		token[tklen] = ch;
		tklen++;
	}
}

void push_up(int id)
{
	tree[id].lsum = tree[LCHD(id)].lsum + (tree[LCHD(id)].thru?tree[RCHD(id)].lsum:0);
	tree[id].rsum = tree[RCHD(id)].rsum + (tree[RCHD(id)].thru?tree[LCHD(id)].rsum:0);
	tree[id].thru = tree[LCHD(id)].thru && tree[RCHD(id)].thru;
	tree[id].msum = tree[LCHD(id)].rsum + tree[RCHD(id)].lsum;
	if(tree[LCHD(id)].msum > tree[id].msum)
		tree[id].msum = tree[LCHD(id)].msum;
	if(tree[RCHD(id)].msum > tree[id].msum)
		tree[id].msum = tree[RCHD(id)].msum;
}

void build_tree(int id, int left, int right)
{
	tree[id].left = left;
	tree[id].right = right;
	tree[id].thru = false;
	if(left == right)
	{
		tree[id].lsum = tree[id].rsum = tree[id].msum = arr[left];
	}else{
		build_tree(LCHD(id), left, MID(left, right));
		build_tree(RCHD(id), MID(left, right)+1, right);
	}
	push_up(id);
}

void update(int id, int pos)
{
	int mid;
	if(tree[id].left == tree[id].right)
	{
		tree[id].lsum = tree[id].rsum = tree[id].msum = arr[pos];
		tree[id].thru = arr[pos] > 0;
		return ;
	}
	mid = MID(tree[id].left, tree[id].right);
	if(pos <= mid)
	{
		update(LCHD(id), pos);
	}else{
		update(RCHD(id), pos);
	}
	push_up(id);
}

void Do_Body(bool arriable);
void Do_State(bool arriable);

int ID[MAXN], TMP[MAXN], CST[MAXN];
//int DBG_MSUM[MAXN];
bool ARRI[MAXN];
int RET[MAXN];
int flptr = 0;

void Do_State(bool arriable)
{
	int id = 0, cst, tmp = 0;
start:
	flptr++;
	if(flptr >= MAXN)
		throw 0;
	read_token();
	if(token[0] == 'c' && token[1] == 'g')
	{
		read_token(); //(
		cst = read_const();
		if(arriable)
		{
			vis[cst] = true;
			//printf("cg :%d\n",cst);
		}
		read_token(); //)
		read_token(); //;
		goto ret ;
	}
	if(token[0] == '{')
	{
		//Do_Body(arriable);
		goto dobody;

dobody_ret:
		read_token();  //}
		goto ret ;
	}
	if(token[0] == 'i' && token[1] == 'f')
	{
		read_token(); //(
		read_token(); //a
		read_token(); //[
		id = read_const();
		read_token(); //]
		read_token(); //>=
		cst = read_const();
		read_token(); //)

		tmp = arr[id];
		if(tmp < cst)
		{
			arr[id] = cst;
			if(arriable)
				update(1, id);
		}

		TMP[flptr] = tmp;
		ID[flptr] = id;
		CST[flptr] = cst;
		ARRI[flptr] = arriable;
		RET[flptr] = 1;
		//DBG_MSUM[flptr] = tree[1].msum;
		arriable = arriable && tree[1].msum <=k && arr[id]<uparr[id];
		if(arr[id] >= uparr[id])
		{
			//printf("Conf %d %d~%d\n",id,arr[id],uparr[id]);
		}
		goto start;
		//Do_State(arriable && tree[1].msum <=k && arr[id]<uparr[id]);
ret1:
		arr[id] = tmp;
		if(arriable)
			update(1, id);
		//if(lastch > 0)
		//    throw 0;  //; 后的读入lastch
		while(lastch == 0 || lastch == ' ' || lastch == '\n' || lastch == '\r')
		{
			lastch = getchar();
		}
		if(lastch != 'e')
		{
			goto ret ;
		}
		read_token(); // else
		tmp = uparr[id];
		uparr[id] = min(tmp, cst);

		TMP[flptr] = tmp;
		ID[flptr] = id;
		CST[flptr] = cst;
		ARRI[flptr] = arriable;
		RET[flptr] = 2;
		//DBG_MSUM[flptr] = tree[1].msum;
		arriable = arriable && tree[1].msum <=k && arr[id]<uparr[id];
		if(arr[id] >= uparr[id])
		{
			//printf("Conf %d %d~%d\n",id,arr[id],uparr[id]);
		}
		goto start;
		//Do_State(arriable && arr[id]<uparr[id]);
ret2:
		uparr[id] = tmp;
		goto ret ;
	}

ret :
	flptr--;
	if(RET[flptr] == 0)
		return ;
	id = ID[flptr];
	tmp = TMP[flptr];
	cst = CST[flptr];
	arriable = ARRI[flptr];
	//if(tree[1].msum != DBG_MSUM[flptr])
	//    flptr = flptr;
	if(RET[flptr] == 1)
		goto ret1;
	else if(RET[flptr] == 2)
		goto ret2;
	else if(RET[flptr] == 3)
		goto ret3;
	else
		throw 2;

dobody:
	while(lastch == 0 || lastch == ' ' || lastch == '\n' || lastch == '\r')
	{
		lastch = getchar();
	}
	while(lastch == 'i' || lastch == 'c' || lastch == '{')
	{
		//Do_State(arriable);
		TMP[flptr] = tmp;
		ID[flptr] = id;
		cst = CST[flptr];
		ARRI[flptr] = arriable;
		RET[flptr] = 3;
		//DBG_MSUM[flptr] = tree[1].msum;
		goto start;

ret3:
		while(lastch == 0 || lastch == ' ' || lastch == '\n' || lastch == '\r')
		{
			lastch = getchar();
		}
	}
	if(lastch != '}')
		throw 0;
	goto dobody_ret;
}

void Do_Body(bool arriable)
{
	while(lastch == 0 || lastch == ' ' || lastch == '\n' || lastch == '\r')
	{
		lastch = getchar();
	}
	while(lastch == 'i' || lastch == 'c' || lastch == '{')
	{
		Do_State(arriable);
		while(lastch == 0 || lastch == ' ' || lastch == '\n' || lastch == '\r')
		{
			lastch = getchar();
		}
	}
	if(lastch != '}')
		throw 0;
}

int main()
{
	int i, ans = 0;

	for(i=0;i<MAXN;i++)
	{
		uparr[i] = MAXI;
		arr[i] = 0;
	}
	read_token(); // game
	read_token(); // (
	n = read_const();
	read_token(); // ,
	k = read_const();
	read_token(); // )
	read_token(); // {
	build_tree(1, 1, n);
	Do_Body(true);

	for(i=0;i<MAXK;i++)
	{
		if(vis[i])
		{
			//printf("%d\n",i);
			ans++;
		}
	}
	printf("%d\n",ans);

	return 0;
}
