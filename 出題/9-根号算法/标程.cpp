#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
using namespace std;

const int MAXN = 10001;
const int GRP = 180;
const int MAXT = GRP * MAXN;
typedef long long int LLI;

map<int, int> item[MAXN];
int A, B, P, Q, K;

inline int getVal(int id){
  if (id > MAXN)
    id = id;

  auto ita = item[id].find(A);
  auto itb = item[id].find(B);
  int ret = 0;
  if (ita != item[id].end())
    ret += P * ita->second;
  if (itb != item[id].end())
    ret += Q * itb->second;
  return ret;
}

bool comp(const int& l, const int& r){
  int vl = getVal(l), vr = getVal(r);
  if (vl != vr)
    return vl > vr;
  else
    return l < r;
}

//SB树
struct NODE{
  int key;
  NODE *chd[2]; //0->l, 1->r
  int sz;
}node[1];
int ncnt;
typedef NODE* PNODE;

PNODE rotate(PNODE rt, int dir){
  PNODE y = rt->chd[dir ^ 1];
  rt->chd[dir ^ 1] = y->chd[dir];
  y->chd[dir] = rt;
  y->sz = rt->sz;
  rt->sz = rt->chd[0]->sz + rt->chd[1]->sz + 1;
  return y;
}

void maintain(PNODE &rt, int dir){
  if (rt == node)
    return;
  if (rt->chd[dir]->chd[dir]->sz <= rt->chd[dir ^ 1]->sz){
    if (rt->chd[dir]->chd[dir ^ 1]->sz > rt->chd[dir ^ 1]->sz){
      rt->chd[dir] = rotate(rt->chd[dir], dir);
    }
    else
      return;
  }
  rt = rotate(rt, dir ^ 1);

  maintain(rt->chd[0], 0);
  maintain(rt->chd[1], 1);
  maintain(rt, 0);
  maintain(rt, 1);
}

//bakpos:如果是之前创建过的节点，就不再创建新的
void insert(PNODE& rt, int key, int bakpos = 0){
  if (rt == node){
    //if (bakpos == 0)
    //  rt = &node[ncnt++];
    //else
    //  rt = &node[bakpos];
    rt = new NODE();
    rt->chd[0] = rt->chd[1] = node;
    rt->key = key;
    rt->sz = 1;
  }
  else{
    rt->sz++;
    int dir = comp(key, rt->key) ? 0 : 1;
    insert(rt->chd[dir], key, bakpos);
    maintain(rt, dir);
  }
}

PNODE remove(PNODE& rt, int key){
  int dir = -1;
  rt->sz--;
  if (rt->key != key){
    //向下递归
    dir = comp(key, rt->key) ? 0 : 1;
    return remove(rt->chd[dir], key);
  }
  if (rt->chd[0] == node)
    dir = 1;
  else if (rt->chd[1] == node)
    dir = 0;
  if (dir >= 0){
    //直接移除
    PNODE tmp = rt;
    rt = rt->chd[dir];
    delete tmp;
    return tmp;
  }
  //查找孩子
  if (rt->chd[0]->sz >= rt->chd[1]->sz)
    dir = 1;
  else
    dir = 0;
  PNODE* ptr = &rt->chd[dir ^ 1];
  while ((*ptr)->chd[dir] != node){
    (*ptr)->sz--;
    ptr = &(*ptr)->chd[dir];
  }
  //交换键，删除ptr
  rt->key = (*ptr)->key;
  PNODE tmp = *ptr;
  *ptr = (*ptr)->chd[dir ^ 1];
  delete tmp;
  return tmp;
}

int select(PNODE rt, int K){
  while (rt != node && rt->chd[0]->sz != K - 1){
    if (rt->chd[0]->sz < K - 1){
      K -= rt->chd[0]->sz + 1;
      rt = rt->chd[1];
    }
    else{
      rt = rt->chd[0];
    }
  }
  return rt->key;
}

void dfsGetCommon(PNODE rt, int x, vector<int>& v){
  if (rt == node)
    return;
  if (item[rt->key].find(x) != item[rt->key].end())
    v.push_back(rt->key);
  dfsGetCommon(rt->chd[0], x, v);
  dfsGetCommon(rt->chd[1], x, v);
}

//k个单关键字的树根
PNODE root[MAXN];
//所有的查询
struct QUERY{
  int A, B, P, Q, K, id;
  bool operator<(const QUERY& rhs)const{
    if (A != rhs.A)
      return A < rhs.A;
    if (B != rhs.B)
      return B < rhs.B;
    return id < rhs.id;
  }
}query[100001];
int ans[100001];

//读入外挂
inline char getc(){
  static const int BUFLEN = 1 << 15;
  static char B[BUFLEN], *S = B, *T = B;
  if (S == T) S = B, T = S + fread(B, 1, BUFLEN, stdin);
  return (S == T) ? 0 : *(S++);
}
int ReadInt(){
  char ch; int aa = 0;
  do ch = getc(); while (!isdigit(ch) && ch);
  if (!ch)
    return -1;
  do aa = aa * 10 + ch - '0', ch = getc();
  while (isdigit(ch));
  return aa;
}
int ReadFloat(){
  char ch; int aa = 0;
  do ch = getc(); while (!isdigit(ch));
  char tmp = getc();
  if (tmp != '.')
    return (ch - '0') * 1000;
  int z = 4;
  do aa = aa * 10 + ch - '0', ch = getc(), z--;
  while (isdigit(ch));
  while (z--) aa *= 10;
  return aa;
}

bool domain(){
  int N, M, c, x, y;
  //vector<pair<int, int>> del;
  vector<int> del;
  vector<int> ins;
  vector<int> tmp;

  N = ReadInt(), M = ReadInt();
  if (N <= 0)
    return false;

  ncnt = 1;
  for (int i = 0; i <= N; i++){
    root[i] = node;
  }

  P = 1000;
  B = 0;
  Q = 0;
  for (int i = 0; i < N; i++){
    c = ReadInt();
    item[i].clear();
    while (c--){
      x = ReadInt();
      y = ReadFloat();
      item[i][x] = y;

      A = x;
      insert(root[x], i);
    }
  }

  int base = ncnt;
  for (int i = 0; i < M; i++){
    query[i].A = ReadInt();
    query[i].P = ReadFloat();
    query[i].B = ReadInt();
    query[i].Q = ReadFloat();
    query[i].K = ReadInt();
    query[i].id = i;
  }
  sort(query, query + M);

  A = 0;
  B = 0;
  int prim = 0, snd = 0; //上次插入的组
  for (int i = 0; i < M; i++){
    K = query[i].K;

    if (query[i].A == A && query[i].B == B){
      //直接查
      ans[query[i].id] = select(root[prim], K);
      continue;
    }

    //否则，先处理旧的
    for (int j = 0, sz = ins.size(); j < sz; j++){
      remove(root[prim], ins[j]);
    }
    ncnt = base;
    A = prim;
    P = 1000;
    B = 0;
    Q = 0;
    for (int j = 0, sz = del.size(); j < sz; j++){
      //insert(root[prim], del[j].first, del[j].second);
      insert(root[prim], del[j]);
    }
    ins.clear();
    del.clear();

    if (query[i].B == 0){
      //直接查
      A = query[i].A;
      prim = A;
      ans[query[i].id] = select(root[prim], K);
      continue;
    }

    //确定主副组
    if (root[query[i].A]->sz >= root[query[i].B]->sz){
      prim = query[i].A;
      snd = query[i].B;
    }
    else{
      prim = query[i].B;
      snd = query[i].A;
    }
    //删除公共元素
    A = prim;
    //dfsGetCommon(root[snd], prim, tmp);
    //for (int j = 0, sz = tmp.size(); j < sz; j++){
    //  int pos = remove(root[prim], tmp[j]) - node;
    //  del.push_back(make_pair(tmp[j], pos));
    //}
    //tmp.clear();
    dfsGetCommon(root[snd], prim, del);
    for (int j = 0, sz = del.size(); j < sz; j++){
      remove(root[prim], del[j]);
    }
    //插入新元素
    A = snd;
    dfsGetCommon(root[snd], snd, ins);
    A = query[i].A;
    B = query[i].B;
    P = query[i].P;
    Q = query[i].Q;
    for (int j = 0, sz = ins.size(); j < sz; j++){
      insert(root[prim], ins[j]);
    }

    //查询结果
    ans[query[i].id] = select(root[prim], K);
  }

  //输出答案
  for (int i = 0; i < M; i++){
    printf("%d\n", ans[i] + 1);
  }

  return true;
}

int main(){
  node->key = -1;
  node->sz = 0;
  node->chd[0] = node->chd[1] = node;

  while (domain());
  return 0;
}
