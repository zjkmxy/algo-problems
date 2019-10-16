#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
using namespace std;
const int MAXN = 300005;

struct SplayTree{
  struct NODE{
    NODE *ch[2], *par;
    int size;
    /*其他数据*/
    int val, add, mv;
    bool rev;
    NODE(int val = 0) :val(val), size(1){
      ch[0] = ch[1] = par = NULL;
      add = 0;
      rev = false;
      mv = val;
    }
    inline int siz(){
      return this ? size : 0;
    }
    inline void push(){
      /*标记下传*/
      if (rev){
        if (ch[0]){
          swap(ch[0]->ch[0], ch[0]->ch[1]);
          ch[0]->rev = !ch[0]->rev;
        }
        if (ch[1]){
          swap(ch[1]->ch[0], ch[1]->ch[1]);
          ch[1]->rev = !ch[1]->rev;
        }
        rev = false;
      }
      if (add != 0){
        if (ch[0]){
          ch[0]->mv += add;
          ch[0]->add += add;
          ch[0]->val += add;
        }
        if (ch[1]){
          ch[1]->mv += add;
          ch[1]->add += add;
          ch[1]->val += add;
        }
        add = 0;
      }
    }
    inline void pop(){
      size = ch[0]->siz() + ch[1]->siz() + 1;
      /*统计结果上传更新*/
      mv = val;
      if (ch[0])
        mv = min(mv, ch[0]->mv);
      if (ch[1])
        mv = min(mv, ch[1]->mv);
    }
    NODE* rot(int dir){
      NODE *p = ch[dir];
      if (p->ch[dir ^ 1])
        p->ch[dir ^ 1]->par = this;
      ch[dir] = p->ch[dir ^ 1];
      p->ch[dir ^ 1] = this;
      p->par = par;
      par = p;
      pop();
      p->pop();
      return p;
    }
    NODE* GetK(int k){ /*取得子树中第k的节点*/
      NODE* p = this;
      while (true){
        p->push();
        int leftSiz = p->ch[0]->siz();
        if (leftSiz == k - 1)
          break;
        if (k > leftSiz){
          p = p->ch[1];
          k = k - leftSiz - 1;
        }
        else{
          p = p->ch[0];
        }
      }
      return p;
    }
    inline void Rotate(){ /*有的写法可能需要特判f==root*/
      NODE *f = par;
      NODE*&p = f->par->ch[f->pos()];
      p = f->rot(pos());
    }
    NODE* Splay(NODE *p){ /*把p提为子节点，返回p*/
      while (p->par != this){
        p->par->par->push();
        p->par->push();
        p->push();
        p->Rotate();
        if (p->par != this)
          p->Rotate();
      }
      return p;
    }
    inline NODE* SplayK(int k){ /*用子树中第k大的替代自己并返回*/
      return par->Splay(GetK(k));
    }
    inline int pos(){ //是否是右节点
      return this == par->ch[1];
    }
  }node[MAXN], *root; //TODO:假设rt.ch[0]是真根，内化另外两个函数
  int cnt;

  inline NODE* GetRange(int l, int r){
    return root->ch[0]->SplayK(l)->ch[1]->SplayK(r - l + 2)->ch[0];
  }
  
  //预制节点root,0号和N号

  void Add(int l, int r, int v){
    NODE* p = root->ch[0]->SplayK(l)->ch[1]->SplayK(r - l + 2)->ch[0];
    p->val += v;
    p->mv += v;
    p->add += v;
  }
  void Reverse(int l, int r){
    NODE* p = root->ch[0]->SplayK(l)->ch[1]->SplayK(r - l + 2)->ch[0];
    swap(p->ch[0], p->ch[1]);
    p->rev = !p->rev;
  }
  int Min(int l, int r){
    NODE* p = root->ch[0]->SplayK(l)->ch[1]->SplayK(r - l + 2)->ch[0];
    return p->mv;
  }
  void Insert(int k, int val){
    NODE *p = root->ch[0]->SplayK(k);
    p->push();
    NODE* cur = &node[cnt++];
    *cur = NODE(val);
    cur->ch[1] = p->ch[1];
    if (p->ch[1])
      p->ch[1]->par = cur;
    p->ch[1] = cur;
    cur->par = p;
    cur->pop();
    p->pop();
  }
  void Delete(int k){
    root->ch[0]->SplayK(k + 1)->ch[1]->SplayK(1);
    NODE *p = root->ch[0];
    p->push();
    p->ch[1]->ch[0] = p->ch[0];
    p->ch[0]->par = p->ch[1];
    root->ch[0] = p->ch[1];
    p->ch[1]->par = root;
    p->ch[1]->pop();
  }
  void Revolve(int l, int r, int t){
    Reverse(l, r - t);
    Reverse(r - t + 1, r);
    Reverse(l, r);
  }

  void Init(){
    cnt = 3;
    node[0] = node[1] = node[2] = NODE();
    root = node;
    node[0].ch[0] = node + 1;
    node[1].par = node;
    node[1].ch[1] = node + 2;
    node[2].par = node + 1;
    node[1].pop();
    node[0].pop();
  }

  int spct;
  void DFS(NODE* x){
    spct++;
    x->push();
    if (x->par && x->par->ch[x->pos()] != x)
      x = x;
    if (x->ch[0]){
      DFS(x->ch[0]);
    }
    spct--;
    for (int i = 0; i < spct; i++)
      printf(" ");
    printf("%d\n", x - node);
    spct++;
    if (x->ch[1]){
      DFS(x->ch[1]);
    }
    spct--;
  }

  void Show(){
    DFS(root);
  }
}tree;

int arr[MAXN], N, M;
int main(){
  int i, x, y, t;
  char op[20];
  scanf("%d", &N);
  for (i = 1; i <= N; i++){
    scanf("%d", &arr[i]);
  }
  tree.Init();
  for (i = 1; i <= N; i++){
    tree.Insert(i, arr[i]);
  }
  scanf("%d", &M);
  while (M--){
    scanf("%s%d", op, &x);
    if (op[0] == 'A'){
      scanf("%d%d", &y, &t);
      tree.Add(x, y, t);
    }
    else if (op[0] == 'I'){
      scanf("%d", &t);
      tree.Insert(x + 1, t);
    }
    else if (op[0] == 'D'){
      tree.Delete(x);
    }
    else if (op[0] == 'M'){
      scanf("%d", &y);
      printf("%d\n", tree.Min(x, y));
    }
    else if (op[3] == 'E'){
      scanf("%d", &y);
      tree.Reverse(x, y);
    }
    else{
      scanf("%d%d", &y, &t);
      t %= (y - x + 1);
      t += (y - x + 1);
      t %= (y - x + 1);
      if (t)
        tree.Revolve(x, y, t);
    }
  }
  return 0;
}