#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

typedef long long ll;
const int MAXN = 100005;
const int DUMMY = -1;
const int FULL = 100;
struct Segtree{
  struct NODE{
    int L, R;
    NODE *l, *r;

    int sum, add;
    int tag;      //��д���
    int mn, mx;

    void popup(){
      mn = min(l->mn, r->mn);
      mx = max(l->mx, r->mx);
      sum = l->sum + r->sum;
      tag = DUMMY;
      if (l->tag == r->tag && l->tag != DUMMY){
        tag = l->tag;
      }
    }

    //��val�����������
    void Fill(int val){
      tag = val;
      add = 0;
      sum = val * (R - L + 1);
      mx = mn = val;
    }

    //Ԥ����Խ���Ƶ�ֵ
    //���������Խ�͸ĳ�FULL - val
    //����Ǹ���Խ�͸ĳ�- val
    //Ȼ��ͨ��Popup������
    void Fix(int val){
      if (mx + val < FULL && mn + val > 0)
        return;
      if (tag == DUMMY){
        pushdown();
        l->Fix(val);
        r->Fix(val);
        popup();
      }
      else
        Fill(max(-val, min(FULL - val, tag)));
    }

    //�������һ����val
    void Plus(int val){
      Fix(val);
      //�������䶼һ�����޸�tag�������޸�add
      if (tag == DUMMY)
        add += val;
      else
        tag += val;
      sum += val * (R - L + 1);
      mx += val;
      mn += val;
    }

    //�´����
    void pushdown(){
      if (tag != DUMMY){
        //���串�Ǵ���ʱ��addһ����0
        l->Fill(tag);
        r->Fill(tag);
      }
      else if (add != 0){
        //����ִ�ж��Լӷ�����
        //ע�ⷴ�����渺���������������������Ҫ���Fix��
        //��Ȼ���ܻ����
        //һ������������ǣ������и��������������0Ȼ�󲻹�����
        //�����Ļ������統ǰ�ڵ���-FULL��Ȼ���24���ӽڵ���ȷ����+24
        //���ǵ�ǰ�ڵ��add��24-FULL<0������ӽڵ㱻������0����ô���޷����+24
        //����ӽڵ������-FULLһ����������FULL
        l->Plus(add);
        r->Plus(add);
        add = 0;
      }
    }
  }node[MAXN << 1];
  int ncnt;

  NODE* Build(int l, int r){
    NODE* ret = &node[ncnt++];
    ret->L = l;
    ret->R = r;
    if (l == r){
      ret->l = ret->r = nullptr;
    }
    else{
      int mid = (l + r) >> 1;
      ret->l = Build(l, mid);
      ret->r = Build(mid + 1, r);
    }
    ret->add = ret->mn = ret->mx = ret->sum = 0;
    ret->tag = 0;
    return ret;
  }

  void Plus(NODE* rt, int l, int r, int val){
    if (l <= rt->L && rt->R <= r){
      rt->Plus(val);
    }
    else{
      rt->pushdown();
      if (l <= rt->l->R)
        Plus(rt->l, l, min(rt->l->R, r), val);
      if (r >= rt->r->L)
        Plus(rt->r, max(rt->r->L, l), r, val);
      rt->popup();
    }
  }

  int GetSum(NODE* rt, int l, int r){
    if (l <= rt->L && rt->R <= r){
      return rt->sum;
    }
    else{
      int ret = 0;
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
  do ch = getc(); while (ch && !isdigit(ch));
  if (!ch)
    return 0;
  do aa = aa * 10 + ch - '0', ch = getc();
  while (isdigit(ch));
  return aa;
}

int main(){
  int N, Q, c, x, y, z;
  for (N = ReadInt(), Q = ReadInt(); N > 0; N = ReadInt(), Q = ReadInt()){
    segt.ncnt = 0;
    segt.Build(1, N);

    while (Q--){
      c = ReadInt(), x = ReadInt(), y = ReadInt();
      if (c == 1){
        z = ReadInt();
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, z);
      }
      else if (c == 2){
        z = ReadInt();
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, -z);
      }
      else{
        printf("%d\n", segt.GetSum(segt.node, x, y));
      }
    }
  }
  return 0;
}