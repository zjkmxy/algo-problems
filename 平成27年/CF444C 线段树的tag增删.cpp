#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 100005;
int N, Q;

struct Segtree{
  struct NODE{
    int tag; //统一化标记(0表示脏)
    LL add;  //单位加权
    LL sum;  //区间和
    int len; //区间长
    NODE *l, *r;

    void Pushdown(){
      if (add > 0){
        l->add += add;
        l->sum += add * l->len;
        r->add += add;
        r->sum += add * r->len;
        add = 0;
      }
      if (tag > 0){
        l->tag = r->tag = tag;
        tag = 0;
      }
    }

    void Popup(){
      sum = l->sum + r->sum;
      if (l->tag == r->tag && l->tag > 0){
        tag = l->tag;
        l->tag = r->tag = 0;
      }
    }

    void Paint(int col){
      if (tag > 0 || l == nullptr){
        //纯节点，直接修改
        add += abs(col - tag);
        sum += 1ll * len * abs(col - tag);
        tag = col;
      }
      else{
        //否则进入
        Pushdown();
        l->Paint(col);
        r->Paint(col);
        Popup();
      }
    }
  }node[MAXN << 2];
  int ncnt, maxn;

  NODE* BuildSub(int l, int r){
    NODE* ret = &node[ncnt++];
    ret->add = 0;
    if (l == r)
      ret->tag = l;
    else
      ret->tag = 0;
    ret->sum = 0;
    ret->len = r - l + 1;
    if (l == r){
      ret->l = ret->r = nullptr;
    }
    else{
      int m = (l + r) >> 1;
      ret->l = BuildSub(l, m);
      ret->r = BuildSub(m + 1, r);
    }
    return ret;
  }

  void PaintSub(NODE* rt, int l, int r, int col, int L, int R){
    if (l <= L && R <= r){
      rt->Paint(col);
      return;
    }
    int m = (L + R) >> 1;
    rt->Pushdown();
    if (r <= m){
      PaintSub(rt->l, l, r, col, L, m);
    }
    else if (l > m){
      PaintSub(rt->r, l, r, col, m + 1, R);
    }
    else{
      PaintSub(rt->l, l, m, col, L, m);
      PaintSub(rt->r, m + 1, r, col, m + 1, R);
    }
    rt->Popup();
  }

  LL QuerySub(NODE* rt, int l, int r, int L, int R){
    if (l <= L && R <= r){
      return rt->sum;
    }
    int m = (L + R) >> 1;
    LL ret = 0;
    rt->Pushdown();
    if (r <= m){
      ret += QuerySub(rt->l, l, r, L, m);
    }
    else if (l > m){
      ret += QuerySub(rt->r, l, r, m + 1, R);
    }
    else{
      ret += QuerySub(rt->l, l, m, L, m);
      ret += QuerySub(rt->r, m + 1, r, m + 1, R);
    }
    rt->Popup();
    return ret;
  }

  inline void Build(int N){
    ncnt = 0;
    maxn = N;
    BuildSub(1, N);
  }
  inline void Paint(int l, int r, int col){
    PaintSub(node, l, r, col, 1, maxn);
  }
  inline LL Query(int l, int r){
    return QuerySub(node, l, r, 1, maxn);
  }
}segt;

//读入挂
int ReadInt(){
  int ret = 0;
  char c;
  while (!isdigit(c = getchar()));
  ret = c - '0';
  while (isdigit(c = getchar())){
    ret = ret * 10 + (c - '0');
  }
  return ret;
}

int main(){
  int N = ReadInt(), M = ReadInt();
  int l, r, cmd, a;
  segt.Build(N);
  while (M--){
    cmd = ReadInt();
    l = ReadInt();
    r = ReadInt();
    if (cmd == 1){
      a = ReadInt();
      segt.Paint(l, r, a);
    }
    else{
      printf("%I64d\n", segt.Query(l, r));
    }
  }
  return 0;
}