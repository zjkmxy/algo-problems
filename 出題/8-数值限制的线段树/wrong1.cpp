/*����һ������İ汾
ʧ��������
10 5
1 1 8 98
2 3 5 68
2 2 3 57
1 1 7 22
3 1 4
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

    //�ͣ��ӱ�־
    int sum, add;
    //�����е��Ͻ���½�
    int sup, inf;
    //����ת�����
    int tag;
    //���½���Թ�Ͻ�Ľڵ����
    int slen, ilen;
    //���Լ��⣬���������sup����Сinf
    int mxs, mni;
    int mns, mxi;
    //��sum��add��tag�⣬����ֵ�������add����ȷ

    inline void PushAdd(int t){
      int len = R - L + 1;
      if (sup == DUMMY)
        len -= slen;
      if (inf == DUMMY)
        len -= ilen;
      add += t;
      sum += t * len;
    }

    //Ҷ�ӽڵ���һ����Ӱ����һ��
    inline void FillSup(int s){
      if (sup != DUMMY)
        sum -= (sup + add) * slen;
      if (s != DUMMY){
        sum += s * slen;
        s -= add;
      }
      sup = s;
    }

    inline void FillInf(int i){
      if (inf != DUMMY)
        sum -= (inf + add) * ilen;
      if (i != DUMMY){
        sum += i * ilen;
        i -= add;
      }
      inf = i;
    }

    void Pushdown(){
      if (tag != 0){
        l->FillTag(tag, DUMMY);
        r->FillTag(tag, DUMMY);
        tag = 0;
      }

      if (add){
        l->PushAdd(add);
        r->PushAdd(add);

        if (sup != DUMMY)
          sup += add;
        if (inf != DUMMY)
          inf += add;
        if (mxs > -1)
          mxs += add;
        if (mni < FULL + 1)
          mni += add;
        if (mxi > -1)
          mxi += add;
        if (mns < FULL + 1)
          mns += add;
        //����û��

        add = 0;
      }

      if (sup != DUMMY){
        if (l->sup == DUMMY)
          l->FillSup(sup);
        if (r->sup == DUMMY)
          r->FillSup(sup);
      }
      if (inf != DUMMY){
        if (l->inf == DUMMY)
          l->FillInf(inf);
        if (r->inf == DUMMY)
          r->FillInf(inf);
      }
    }

    void Popup(){
      //��ʱ�Լ������ܴ���add

      //ժ��inf
      if (inf != DUMMY){
        if (l->inf != DUMMY && l->inf + l->add == inf)
          l->FillInf(DUMMY);
        if (r->inf != DUMMY && r->inf + r->add == inf)
          r->FillInf(DUMMY);
      }
      //ժ��sup
      if (sup != DUMMY){
        if (l->sup != DUMMY && l->sup + l->add == sup)
          l->FillSup(DUMMY);
        if (r->sup != DUMMY && r->sup + r->add == sup)
          r->FillSup(DUMMY);
      }
      //pushdown��ժ����Ҫ��

      //����mni
      mni = min(l->mni + l->add, r->mni + r->add);
      if (l->inf != DUMMY)
        mni = min(mni, l->inf + l->add);
      if (r->inf != DUMMY)
        mni = min(mni, r->inf + r->add);
      //����mxs
      mxs = max(l->mxs + l->add, r->mxs + r->add);
      if (l->sup != DUMMY)
        mxs = max(mxs, l->sup + l->add);
      if (r->sup != DUMMY)
        mxs = max(mxs, r->sup + r->add);

      //����mns
      mns = min(l->mns + l->add, r->mns + r->add);
      if (l->sup != DUMMY)
        mns = min(mns, l->sup + l->add);
      if (r->sup != DUMMY)
        mns = min(mns, r->sup + r->add);
      //����mxi
      mxi = max(l->mxi + l->add, r->mxi + r->add);
      if (l->inf != DUMMY)
        mxi = max(mxi, l->inf + l->add);
      if (r->inf != DUMMY)
        mxi = max(mxi, r->inf + r->add);

      //����slen��ilen
      slen = 0;
      if (l->sup == DUMMY)
        slen += l->slen;
      if (r->sup == DUMMY)
        slen += r->slen;
      ilen = 0;
      if (l->inf == DUMMY)
        ilen += l->ilen;
      if (r->inf == DUMMY)
        ilen += r->ilen;

      //����sum
      sum = l->sum + r->sum;
      if (sup != DUMMY)
        sum += sup * slen;
      if (inf != DUMMY)
        sum += inf * ilen;
    }

    //t = -1��ʾinf��t = 1��ʾsup
    //������븽����ǣ���ΪCleanʱ����x�ĺ���FillTag����x������sup/inf
    //����x��sup/inf�����޸ģ�������������ƶ�ʧЧ
    inline void FillTag(int t, int val){
      FillSup(DUMMY);
      FillInf(DUMMY);
      tag = t;
      if (t > 0){
        ilen = 0;
        slen = R - L + 1;
        sup = val;
        inf = DUMMY;
      }
      else{
        slen = 0;
        ilen = R - L + 1;
        inf = val;
        sup = DUMMY;
      }

      mni = mns = 0x3fffffff;
      mxs = mxi = -0x3fffffff;
      if (val != DUMMY)
        sum = val * (R - L + 1);
      else
        sum = 0;
      add = 0;
    }

    //������д���val��sup��inf���������ǵĳ���ת����slen
    void CleanAbove(int val){
      if (inf != DUMMY && inf + add >= val){
        FillTag(1, val);
        return;
      }
      if (sup != DUMMY && sup + add >= val)
        FillSup(DUMMY);
      if (mxs + add < val && mxi + add < val)
        return;
      Pushdown();
      l->CleanAbove(val);
      r->CleanAbove(val);
      Popup();
    }

    //�������С��val��sup��inf���������ǵĳ���ת����ilen
    void CleanBelow(int val){
      if (sup != DUMMY && sup + add <= val){
        FillTag(-1, val);
        return;
      }
      if (inf != DUMMY && inf + add <= val)
        FillInf(DUMMY);
      if (mns + add > val && mni + add > val)
        return;
      Pushdown();
      l->CleanBelow(val);
      r->CleanBelow(val);
      Popup();
    }

  }node[MAXN << 1];
  int ncnt;

  NODE* Build(int l, int r){
    NODE* ret = &node[ncnt++];
    ret->L = l;
    ret->R = r;
    ret->inf = ret->sup = DUMMY;
    ret->sum = 0;
    ret->add = 0;
    ret->mni = ret->mns = 0x3fffffff;
    ret->mxs = ret->mxi = -0x3fffffff;
    ret->slen = 0;
    ret->tag = 0;
    if (l == r){
      ret->ilen = 1;
    }
    else{
      int mid = (l + r) >> 1;
      ret->l = Build(l, mid);
      ret->r = Build(mid + 1, r);
      ret->ilen = ret->l->ilen + ret->r->ilen;
    }
    if (ret == node)
      ret->inf = 0;
    return ret;
  }

  void Plus(NODE* rt, int l, int r, int t){
    if (l <= rt->L && rt->R <= r){
      rt->CleanAbove(FULL - t);
      if (rt->sup == DUMMY)
        rt->FillSup(FULL - t);
      rt->PushAdd(t);
      return;
    }
    rt->Pushdown();
    if (l <= rt->l->R)
      Plus(rt->l, l, min(rt->l->R, r), t);
    if (r >= rt->r->L)
      Plus(rt->r, max(rt->r->L, l), r, t);
    rt->Popup();
  }

  void Minus(NODE* rt, int l, int r, int t){
    if (l <= rt->L && rt->R <= r){
      rt->CleanBelow(t);
      if (rt->inf == DUMMY)
        rt->FillInf(t);
      rt->PushAdd(-t);
      return;
    }
    rt->Pushdown();
    if (l <= rt->l->R)
      Minus(rt->l, l, min(rt->l->R, r), t);
    if (r >= rt->r->L)
      Minus(rt->r, max(rt->r->L, l), r, t);
    rt->Popup();
  }

  int GetSum(NODE* rt, int l, int r){
    if (l <= rt->L && rt->R <= r){
      return rt->sum;
    }
    else{
      int ret = 0;
      rt->Pushdown();
      if (l <= rt->l->R)
        ret += GetSum(rt->l, l, min(rt->l->R, r));
      if (r >= rt->r->L)
        ret += GetSum(rt->r, max(rt->r->L, l), r);
      rt->Popup();
      return ret;
    }
  }
}segt;


int main(){
  //freopen("tmp.in", "r", stdin);
  //freopen("tmp.out", "w", stdout);
  //FILE* answer = fopen("tmp.ans", "r");

  int N, Q, c, x, y, z;
  while (scanf("%d%d", &N, &Q) != EOF){
    segt.ncnt = 0;
    segt.Build(1, N);

    while (Q--){
      scanf("%d%d%d", &c, &x, &y);
      if (x == 1 && y == 6)
        x = x;
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
        segt.Minus(segt.node, x, y, z);
      }
      else{
        //int myans = segt.GetSum(segt.node, x, y);
        //int ans;
        //fscanf(answer, "%d", &ans);
        //if (myans != ans)
        //  ans = ans;

        printf("%d\n", segt.GetSum(segt.node, x, y));
      }
    }
  }
  return 0;
}
