/**************************************************************
    Problem: 1095
    User: zjkmxy
    Language: C++
    Result: Accepted
    Time:1924 ms
    Memory:31792 kb
****************************************************************/
 
#define nullptr NULL
#include <cstdio>
#include <cmath>
#include <set>
#include <algorithm>
using namespace std;
 
const int MAXN = 100005;
 
//带标号可修改最大堆
class Heap{
public:
    struct HNODE{
        int id;
        int val;
        bool operator<(const HNODE& rhs)const{
            return val < rhs.val;
        };
    };
    static HNODE POOL[MAXN<<3]; //元素池
    static int TABLE[MAXN<<3], pcnt; //对应表池
    int cnt;
    int *tab;
    HNODE *node;
 
    inline int LCHD(int id){return id << 1;}
    inline int RCHD(int id){return id << 1 | 1;}
    //初始化
    inline void Init(){
        cnt = 0;
        node = nullptr;
        tab = nullptr;
    }
    //增加节点，返回标号
    inline int New(){
        return (++ cnt);
    }
    //分配空间建立堆
    inline void Build(){
        node = POOL + pcnt - 1;
        tab = TABLE + pcnt - 1;
        pcnt += cnt;
        for(int i = 1; i <= cnt; i ++){
            node[i].id = i;
            node[i].val = -1;
            tab[i] = i;
        }
    }
    //内部操作：交换
    inline void chg(HNODE& lhs, HNODE& rhs){
        swap(tab[lhs.id], tab[rhs.id]);
        swap(lhs, rhs);
    }
    //修改一个点
    void Update(int id, int val){
        int cur = tab[id];
        if(node[cur].val > val){
            //下降
            node[cur].val = val;
            while(true){
                int large = cur;
                if(LCHD(cur) <= cnt && node[large] < node[LCHD(cur)])
                    large = LCHD(cur);
                if(RCHD(cur) <= cnt && node[large] < node[RCHD(cur)])
                    large = RCHD(cur);
                if(large != cur){
                    chg(node[cur], node[large]);
                    cur = large;
                }else
                    break;
            }
        }else{
            //上浮
            node[cur].val = val;
            while(cur > 1 && node[cur >> 1] < node[cur]){
                chg(node[cur >> 1], node[cur]);
                cur >>= 1;
            }
        }
    }
    //取得最大值(-1表示不存在)
    inline int GetMax(){
        return cnt > 0 ? node[1].val : -1;
    }
    //取得最大间隔
    int GetGap(){
        if(cnt < 2)
            return -1;
        int mv = node[2].val;
        if(cnt >= 3)
            mv = max(mv, node[3].val);
        return node[1].val + mv;
    }
};
Heap::HNODE Heap::POOL[MAXN<<3];
int Heap::TABLE[MAXN<<3];
int Heap::pcnt = 0;
 
struct NODE;
struct EDGE;
typedef NODE *PNODE;
typedef EDGE *PEDGE;
struct NODE{
    int id;     //编号
    PNODE par, top, son;    //父亲，链头，重孩子
    PEDGE head;
    int siz, dep;
    int w, plast;   //分配点号，链的终点
 
    //维护数据
    bool black; //是否是黑色
    Heap hdis;  //维护所有子链到其的距离
    Heap hgap;  //维护所有子链本身的距离最大值
    int lab;    //在父亲的堆中的编号，只有轻孩子节点有
    void *sgtn; //线段树所对应的点的指针，只有top节点有
}node[MAXN];
struct EDGE{
    PNODE a, b;
    PEDGE next;
}edge[MAXN<<1];
int redirecttab[MAXN];  //所有的w其对应的点
int N, Q, ecnt;
int blackcnt;	//黑灯的个数
 
//复合型线段树
class Segtree{
public:
    struct SN{
        int len;    //区间长度，注意是点的个数，所以直接加就是距离
        int ldis, rdis, gap;    //-1表示不存在
        SN *lchd, *rchd;
        //从树的NODE获得线段树的值
        inline void assgin(int id){
            //所有的轻孩子内部的情况
            ldis = rdis = node[id].hdis.GetMax();
            gap = max(node[id].hdis.GetGap(), node[id].hgap.GetMax());
            //这个点本身
            if(node[id].black){
                ldis = rdis = max(ldis, 0);
                gap = max(gap, 0);
            }
        }
        //上传
        void pop(){
            //此处因为有子树影响，到整个线段右端点为止，左孩子的右距离不一定比右孩子的大，不能想当然。
            if(lchd->rdis >= 0)
                rdis = lchd->rdis + rchd->len;
            else
                rdis = -1;
            rdis = max(rdis, rchd->rdis);
            if(rchd->ldis >= 0)
                ldis = rchd->ldis + lchd->len;
            else
                ldis = -1;
            ldis = max(ldis, lchd->ldis);
            gap = max(lchd->gap, rchd->gap);
            if(lchd->rdis >= 0 && rchd->ldis >= 0)
                gap = max(gap, lchd->rdis + rchd->ldis + 1);
        }
    }sn[MAXN<<2];
    int ncnt;
    //建树
    SN* build(int L, int R){
        SN* ret = &sn[ncnt ++];
        ret->len = (R - L) + 1;
        if(L == R){
            ret->lchd = ret->rchd = nullptr;
            ret->assgin(redirecttab[L]);
        }else{
            int mid = (L + R) >> 1;
            ret->lchd = build(L, mid);
            ret->rchd = build(mid + 1, R);
            ret->pop();
        }
        return ret;
    }
    //更新一个节点
    void update(SN* cur, int pos, int L, int R){
        if(L == R){
            cur->assgin(redirecttab[L]);
            return ;
        }
        int mid = (L + R) >> 1;
        if(pos <= mid){
            update(cur->lchd, pos, L, mid);
        }else{
            update(cur->rchd, pos, mid + 1, R);
        }
        cur->pop();
    }
    //指针转换，把sgnt从无类型指针转化为SN*
    inline static SN* cvt(void* ptr){
        return reinterpret_cast<SN *>(ptr);
    }
}sgt;
 
//加边
void addedge(PNODE a, PNODE b){
    edge[ecnt].a = a;
    edge[ecnt].b = b;
    edge[ecnt].next = a->head;
    a->head = &edge[ecnt];
    ecnt ++;
}
 
//大树锯
namespace BTS{
    void DFS(NODE *x){
        x->siz = 1;      x->son = nullptr;
        for(EDGE *j = x->head; j; j = j->next){
            NODE *u = j->b;
            if(u == x->par)  continue;
            u->par = x;  u->dep = x->dep + 1;
            DFS(u);
            if(!x->son || u->siz > x->son->siz)  x->son = u;
            x->siz += u->siz;
        }
    }
    int tz; NODE *tp;
    void DFS2(NODE *x){
        x->w = ++tz; x->top = tp;  //分配编号
        redirecttab[tz] = x - node;
        //链终点的确定
        x->plast = tz;
        x->top->plast = tz;
        //递归锯开链
        if(x->son)
            DFS2(x->son);
        //分治旁支（轻孩子）
        for(EDGE *j = x->head; j; j = j->next){
            if(j->b == x->son || j->b == x->par)    continue;
            tp = j->b;   DFS2(j->b);
 
            //轻孩子加入堆
            j->b->lab = x->hdis.New();
            j->b->lab = x->hgap.New();
        }
    }
    inline void Split(NODE *root){
        tz = 0;
        tp = root, root->dep = 0;    DFS(root);  DFS2(root);
    }
};
 
//初始化节点
void InitNode(int id){
    node[id].id = id;
    node[id].black = false;
    node[id].hdis.Init();
    node[id].hgap.Init();
    node[id].par = node[id].son = node[id].top = nullptr;
    node[id].head = nullptr;
    node[id].sgtn = nullptr;
    node[id].lab = 0;
}
 
//计算答案
int Game(){
    if(blackcnt == 0){
        return -1;
    }else if(blackcnt == 1){
        return 0;
    }
    Segtree::SN* qn = sgt.cvt(node[1].sgtn);
    return max(qn->gap, 0);
}
 
//灯的变化：维护顺序按线段树->堆->线段树->……交替进行
void Change(int id){
    NODE *tp, *cur = &node[id];
    cur->black = !cur->black;
    if(cur->black)
        blackcnt ++;
    else
        blackcnt --;
    while(cur){
        tp = cur->top;
        sgt.update(sgt.cvt(tp->sgtn), cur->w, tp->w, tp->plast);
        cur = tp->par;
        if(cur){
            int disval = sgt.cvt(tp->sgtn)->ldis;
            int gapval = sgt.cvt(tp->sgtn)->gap;
            if(disval >= 0)
                disval += 1;
            cur->hdis.Update(tp->lab, disval);
            cur->hgap.Update(tp->lab, gapval);
        }
    }
}
 
int main(){
    int i, a, b;
    char op[3];
 
    scanf("%d", &N);
    blackcnt = 0;
    ecnt = 0;
    for(i = 1; i <= N; i ++){
        InitNode(i);
    }
    for(i = 1; i < N; i ++){
        scanf("%d%d", &a, &b);
        addedge(node + a, node + b);
        addedge(node + b, node + a);
    }
    //锯开
    BTS::Split(node + 1);
    //建立堆
    for(i = 1; i <= N; i ++){
        node[i].hdis.Build();
        node[i].hgap.Build();
    }
    //建立线段树
    for(i = 1; i <= N; i ++){
        if(node[i].top == &node[i])
            node[i].sgtn = sgt.build(node[i].w, node[i].plast);
    }
    //先全部开灯
    for(i = 1; i <= N; i ++){
        Change(i);
    }
    scanf("%d", &Q);
    while(Q --){
        scanf("%s", op);
        if(op[0] == 'C'){
            scanf("%d", &a);
            Change(a);
        }else{
            printf("%d\n", Game());
        }
    }
    return 0;
}