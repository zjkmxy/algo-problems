/*
����������HDU5306�ǲ�ͬ�ģ����˼·��HDU5306��TLE

HDU5306�����ȥ��¼����tag���ֵ��ֱ�ӱ����ǻᱻ����N^2��,�������£�
0 N 0 N 0 N 0 N ...
Ȼ��ÿ�δ�N���εݼ���[1,N]������min����

���������һ��ֻ��100�����������ݱ���100��֮���������ֶ�һ����
���������O(N^2)�ı����㷨�������ͱ����O(N(log(N)+RANGE))
RANGE=100���ǿ��Թ��ġ�
�ο�������������µ�ƽ�����Ӷ�Ҳû��O(N^2)
*/
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

int main(){
  int N, Q, c, x, y, z;
  while (scanf("%d%d", &N, &Q) != EOF){
    segt.ncnt = 0;
    segt.Build(1, N);

    while (Q--){
      scanf("%d%d%d", &c, &x, &y);
      if (c == 1){
        scanf("%d", &z);
        if (z == 0)
          continue;
        segt.Plus(segt.node, x, y, z);
      }
      else if (c == 2){
        scanf("%d", &z);
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
