#include <iostream>
#include <cmath>
#include <cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>
#include<vector>
using namespace std;

#define MID(l, r) (((l) + (r)) >> 1)
#define MAXN 100005
#define MAXK 100005
typedef long long LLI;

struct NODE
{
    int left;
    int right;
    int lchd;  //左右孩子指针，-1不存在，-2待决（0节点）
    int rchd;
    LLI sum;   //距离的和
    int cnt;   //个数的和
}tree[MAXN*60];

int tcnt;
int N, M, X, P; //X限制不用离散化横坐标
int root[MAXK];

struct SEG
{
    int L, D;
    int dno;
    bool add;
    bool operator<(const SEG other)const
    {
        if(L != other.L)
            return L < other.L;
        return add > other.add;
    };
}segs[MAXN<<1];
int dy[MAXN], ny, nseg;

//二分
int BiSearch(int x)
{
    int begin,end,mid;

    begin = 0;
    end = ny - 1;

    while(end > begin + 1)
    {
        mid = (begin + end) >> 1;
        if(dy[mid] == x)
            return mid;
        if(dy[mid] < x)
        {
            begin = mid;
        }else{
            end = mid;
        }
    }

    if(dy[end] > x)
    {
        return begin;
    }else{
        return end;
    }
}

int compi(const void *A, const void *B)
{
    return *(int*)A - *(int*)B;
}

//去重
void uniq(int n)
{
    int i, j;

    qsort(dy, n, sizeof(int), compi);
    i = 0;
    j = 0;
    while(j < n)
    {
        dy[i] = dy[j];
        do{
            j++;
        }while((dy[i]==dy[j])&&(j < n));
        i++;
    }
    ny = i;
}

void init()
{
    tcnt = 0;
    ny = 0;
    nseg = 0;
}

int build_tree(int l, int r)
{
    int mid;
    int ret = tcnt;

    tcnt++;
    tree[ret].left = l;
    tree[ret].right = r;
    tree[ret].sum = 0;
    tree[ret].cnt = 0;
    if(l == r)
    {
        tree[ret].lchd = -1;
        tree[ret].rchd = -1;
    }else{
        mid = MID(l, r);
        tree[ret].lchd = build_tree(l, mid);
        tree[ret].rchd = build_tree(mid+1, r);
    }
    return ret;
}

inline void pop_up(int id)
{
    tree[id].sum = tree[tree[id].lchd].sum + tree[tree[id].rchd].sum;
    tree[id].cnt = tree[tree[id].lchd].cnt + tree[tree[id].rchd].cnt;
}

//修改
int modify(int id, int dno, int D, int cnt)
{
    int ret = tcnt;

    tcnt++;
    if(tree[id].left == dno && tree[id].right == dno)
    {
        tree[ret].lchd = tree[ret].rchd = -1;
        tree[ret].left = tree[ret].right = dno;
        tree[ret].sum = tree[id].sum + D;
        tree[ret].cnt = tree[id].cnt + cnt;
        return ret;
    }
    tree[ret] = tree[id];
    if(dno <= MID(tree[id].left, tree[id].right))
    {
        tree[ret].lchd = modify(tree[id].lchd, dno, D, cnt);
    }else{
        tree[ret].rchd = modify(tree[id].rchd, dno, D, cnt);
    }
    pop_up(ret);
    return ret;
}

void query(int id, int l, int r, LLI &sum, int &cnt)
{
    int tmp;
    LLI mid;
    if(tree[id].left >= l && tree[id].right <= r)
    {
        sum = tree[id].sum;
        cnt = tree[id].cnt;
        return;
    }
    mid = MID(tree[id].left, tree[id].right);
    if(r <= mid)
    {
        query(tree[id].lchd, l, r, sum, cnt);
    }else if(l > mid){
        query(tree[id].rchd, l, r, sum, cnt);
    }else{
        query(tree[id].lchd, l, mid, sum, cnt);
        query(tree[id].rchd, mid+1, r, mid, tmp);
        sum += mid;
        cnt += tmp;
    }
}

LLI query(int id, int k)
{
    int tmp;
    LLI mid;
    
    if(tree[id].cnt <= k)
        return tree[id].sum;
    if(tree[id].lchd == -1)
    {
        return dy[tree[id].left] * k;
    }
    if(tree[tree[id].lchd].cnt > k)
    {
        return query(tree[id].lchd, k);
    }else{
        return query(tree[id].lchd, tree[tree[id].lchd].cnt) + query(tree[id].rchd, k-tree[tree[id].lchd].cnt);
    }
}

bool input()
{
    int i, j;
    int l, r, d;

    init();
    if(!(cin >> N >> M >> X >> P))
        return false;
    for(i=0;i<N;i++)
    {
        cin >> l >> r >> d;
        segs[nseg].L = l;
        segs[nseg].D = d;
        segs[nseg].add = true;
        nseg++;
        segs[nseg].L = r;
        segs[nseg].D = d;
        segs[nseg].add = false;
        nseg++;
        dy[ny] = d;
        ny++;
    }
    sort(segs, segs+nseg);
    uniq(ny);
    for(i=0;i<nseg;i++)
    {
        segs[i].dno = BiSearch(segs[i].D);
    }
    root[0] = build_tree(0, ny-1);
    root[1] = root[0];
    for(i=1, j=0;i<=X;i++)
    {
        while(j<nseg&&(segs[j].L < i || (segs[j].L == i && segs[j].add)))
        {
            root[i] = modify(root[i], segs[j].dno, segs[j].add?segs[j].D:-segs[j].D, segs[j].add?1:-1);
            j++;
        }
        root[i+1] = root[i];
        while(j<nseg&&(segs[j].L == i && !segs[j].add))
        {
            root[i+1] = modify(root[i+1], segs[j].dno, segs[j].add?segs[j].D:-segs[j].D, segs[j].add?1:-1);
            j++;
        }
    }

    return true;
}

void domain()
{
    int i, x, a, b, c;
    LLI pre = 1;

    for(i=0;i<M;i++)
    {
        cin >> x >> a >> b >> c;
        if(pre > P)
            pre = 2 * query(root[x], (LLI)(a * pre + b) % c);
        else
            pre = query(root[x], (LLI)(a * pre + b) % c);
        cout << pre << endl;
    }
}

int main()
{
    while(input())
    {
        domain();
    }
    return 0;
}