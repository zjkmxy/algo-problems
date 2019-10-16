///MXYTEST
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

typedef long long ll;
const int MAXN = 2000005;
const int Dummy = -1;
int arr[MAXN];
struct Segtree{
  struct NODE{
    int L, R;
    NODE *l, *r;
    int mv;
    ll sum;
    int avlLen;  //被覆盖长度
    int tag;

    inline void popup(){
      mv = max(l->mv, r->mv);         //dirtyMax现在其实就是mv
      sum = l->sum + r->sum;
      avlLen = l->avlLen + r->avlLen; //需要pop的时候自己肯定没tag
    }

    inline void clean(int t){
      if (mv < t)
        return;
      if (tag >= t){
        tag = Dummy;
      }
      if (!l){
        if (tag != Dummy){
          avlLen = 1;
          sum = mv = tag;
        }
        else{
          sum = mv = 0;
          avlLen = 0;
        }
      }
      else{
        pushdown();
        l->clean(t);
        r->clean(t);
        popup();
      }
    }

    inline void overwrite(int t){
      int len = (R - L + 1);
      if ((tag >= 0 && tag <= t) || avlLen == len)
        return;
      sum += 1ll * t * (len - avlLen);
      avlLen = len;
      tag = mv = t;
    }

    inline void pushdown(){
      if (tag != Dummy){
        l->overwrite(tag);
        r->overwrite(tag);
      }
    }
  }node[MAXN << 1];
  int ncnt;

  NODE* Build(int l, int r){
    NODE* ret = &node[ncnt++];
    ret->L = l;
    ret->R = r;
    if (l == r){
      ret->sum = ret->tag = ret->mv = arr[l];
      ret->avlLen = 1;
      ret->l = ret->r = nullptr;
    }
    else{
      ret->tag = Dummy;
      int mid = (l + r) >> 1;
      ret->l = Build(l, mid);
      ret->r = Build(mid + 1, r);
      ret->popup();
    }
    return ret;
  }

  void Change(NODE* rt, int l, int r, int t){
    if (rt->mv <= t)
      return;
    if (l <= rt->L && rt->R <= r){
      rt->clean(t);
      if (rt->l)
        rt->popup();
      else{
        rt->sum = 0;
        rt->avlLen = 0;
      }
      rt->overwrite(t);
    }
    else{
      rt->pushdown();
      if (l <= rt->l->R)
        Change(rt->l, l, min(rt->l->R, r), t);
      if (r >= rt->r->L)
        Change(rt->r, max(rt->r->L, l), r, t);
      rt->popup();
    }
  }

  int GetMax(NODE* rt, int l, int r){
    if (l <= rt->L && rt->R <= r){
      return rt->mv;
    }
    else{
      int ret = 0;
      rt->pushdown();
      if (l <= rt->l->R)
        ret = max(ret, GetMax(rt->l, l, min(rt->l->R, r)));
      if (r >= rt->r->L)
        ret = max(ret, GetMax(rt->r, max(rt->r->L, l), r));
      return ret;
    }
  }

  ll GetSum(NODE* rt, int l, int r){
    if (l <= rt->L && rt->R <= r){
      return rt->sum;
    }
    else{
      ll ret = 0;
      rt->pushdown();
      if (l <= rt->l->R)
        ret += GetSum(rt->l, l, min(rt->l->R, r));
      if (r >= rt->r->L)
        ret += GetSum(rt->r, max(rt->r->L, l), r);
      return ret;
    }
  }
}segt;

inline char getc(){
  static const int BUFLEN = 1 << 15;
  static char B[BUFLEN], *S = B, *T = B;
  if (S == T) S = B, T = S + fread(B, 1, BUFLEN, stdin);
  return (S == T) ? 0 : *(S++);
}
int ReadInt(){
  char ch; int aa = 0;
  do ch = getc(); while (!isdigit(ch));
  do aa = aa * 10 + ch - '0', ch = getc();
  while (isdigit(ch));
  return aa;
}

int N, Q;
int main(){
  int T, i, op, l, r;
  T = ReadInt();
  while (T--){
    N = ReadInt(), Q = ReadInt();
    for (i = 1; i <= N; i++){
      arr[i] = ReadInt();
    }
    segt.ncnt = 0;
    segt.Build(1, N);
    while (Q--){
      op = ReadInt(), l = ReadInt(), r = ReadInt();
      if (op == 0){
        i = ReadInt();
        segt.Change(segt.node, l, r, i);
      }
      else{
        if (op == 1){
          int ret = segt.GetMax(segt.node, l, r);
          printf("%d\n", ret);
        }
        else{
          ll ret = segt.GetSum(segt.node, l, r);
          printf("%lld\n", ret);
        }
      }
    }
  }
  return 0;
}