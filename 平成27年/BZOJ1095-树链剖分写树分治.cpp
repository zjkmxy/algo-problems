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
 
//����ſ��޸�����
class Heap{
public:
    struct HNODE{
        int id;
        int val;
        bool operator<(const HNODE& rhs)const{
            return val < rhs.val;
        };
    };
    static HNODE POOL[MAXN<<3]; //Ԫ�س�
    static int TABLE[MAXN<<3], pcnt; //��Ӧ���
    int cnt;
    int *tab;
    HNODE *node;
 
    inline int LCHD(int id){return id << 1;}
    inline int RCHD(int id){return id << 1 | 1;}
    //��ʼ��
    inline void Init(){
        cnt = 0;
        node = nullptr;
        tab = nullptr;
    }
    //���ӽڵ㣬���ر��
    inline int New(){
        return (++ cnt);
    }
    //����ռ佨����
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
    //�ڲ�����������
    inline void chg(HNODE& lhs, HNODE& rhs){
        swap(tab[lhs.id], tab[rhs.id]);
        swap(lhs, rhs);
    }
    //�޸�һ����
    void Update(int id, int val){
        int cur = tab[id];
        if(node[cur].val > val){
            //�½�
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
            //�ϸ�
            node[cur].val = val;
            while(cur > 1 && node[cur >> 1] < node[cur]){
                chg(node[cur >> 1], node[cur]);
                cur >>= 1;
            }
        }
    }
    //ȡ�����ֵ(-1��ʾ������)
    inline int GetMax(){
        return cnt > 0 ? node[1].val : -1;
    }
    //ȡ�������
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
    int id;     //���
    PNODE par, top, son;    //���ף���ͷ���غ���
    PEDGE head;
    int siz, dep;
    int w, plast;   //�����ţ������յ�
 
    //ά������
    bool black; //�Ƿ��Ǻ�ɫ
    Heap hdis;  //ά��������������ľ���
    Heap hgap;  //ά��������������ľ������ֵ
    int lab;    //�ڸ��׵Ķ��еı�ţ�ֻ���Ả�ӽڵ���
    void *sgtn; //�߶�������Ӧ�ĵ��ָ�룬ֻ��top�ڵ���
}node[MAXN];
struct EDGE{
    PNODE a, b;
    PEDGE next;
}edge[MAXN<<1];
int redirecttab[MAXN];  //���е�w���Ӧ�ĵ�
int N, Q, ecnt;
int blackcnt;	//�ڵƵĸ���
 
//�������߶���
class Segtree{
public:
    struct SN{
        int len;    //���䳤�ȣ�ע���ǵ�ĸ���������ֱ�ӼӾ��Ǿ���
        int ldis, rdis, gap;    //-1��ʾ������
        SN *lchd, *rchd;
        //������NODE����߶�����ֵ
        inline void assgin(int id){
            //���е��Ả���ڲ������
            ldis = rdis = node[id].hdis.GetMax();
            gap = max(node[id].hdis.GetGap(), node[id].hgap.GetMax());
            //����㱾��
            if(node[id].black){
                ldis = rdis = max(ldis, 0);
                gap = max(gap, 0);
            }
        }
        //�ϴ�
        void pop(){
            //�˴���Ϊ������Ӱ�죬�������߶��Ҷ˵�Ϊֹ�����ӵ��Ҿ��벻һ�����Һ��ӵĴ󣬲����뵱Ȼ��
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
    //����
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
    //����һ���ڵ�
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
    //ָ��ת������sgnt��������ָ��ת��ΪSN*
    inline static SN* cvt(void* ptr){
        return reinterpret_cast<SN *>(ptr);
    }
}sgt;
 
//�ӱ�
void addedge(PNODE a, PNODE b){
    edge[ecnt].a = a;
    edge[ecnt].b = b;
    edge[ecnt].next = a->head;
    a->head = &edge[ecnt];
    ecnt ++;
}
 
//������
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
        x->w = ++tz; x->top = tp;  //������
        redirecttab[tz] = x - node;
        //���յ��ȷ��
        x->plast = tz;
        x->top->plast = tz;
        //�ݹ�⿪��
        if(x->son)
            DFS2(x->son);
        //������֧���Ả�ӣ�
        for(EDGE *j = x->head; j; j = j->next){
            if(j->b == x->son || j->b == x->par)    continue;
            tp = j->b;   DFS2(j->b);
 
            //�Ả�Ӽ����
            j->b->lab = x->hdis.New();
            j->b->lab = x->hgap.New();
        }
    }
    inline void Split(NODE *root){
        tz = 0;
        tp = root, root->dep = 0;    DFS(root);  DFS2(root);
    }
};
 
//��ʼ���ڵ�
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
 
//�����
int Game(){
    if(blackcnt == 0){
        return -1;
    }else if(blackcnt == 1){
        return 0;
    }
    Segtree::SN* qn = sgt.cvt(node[1].sgtn);
    return max(qn->gap, 0);
}
 
//�Ƶı仯��ά��˳���߶���->��->�߶���->�����������
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
    //�⿪
    BTS::Split(node + 1);
    //������
    for(i = 1; i <= N; i ++){
        node[i].hdis.Build();
        node[i].hgap.Build();
    }
    //�����߶���
    for(i = 1; i <= N; i ++){
        if(node[i].top == &node[i])
            node[i].sgtn = sgt.build(node[i].w, node[i].plast);
    }
    //��ȫ������
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