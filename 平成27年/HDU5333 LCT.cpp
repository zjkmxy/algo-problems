#include <cstdio>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int MAXM = 200005;
const int INF = 0x3fffffff;
struct EDGE{
    int a, b;    //确保a比b小
    bool used;
}edge[MAXM];
int N, M, Q;
struct OPT{
    int b;        //进入时间
    bool add;    //加边？查询？
    int id;
    OPT(){};
    OPT(int b, bool add, int id):b(b),add(add),id(id){}
    bool operator<(const OPT& rhs)const{
        if(b != rhs.b)
            return b < rhs.b;
        else if(add != rhs.add)
            return add;
        else
            return id < rhs.id;
    }
}opt[MAXM<<1];
struct QUERY{
    int l, r, ans;
}query[MAXM];

int ReadInt(){
    int ret = 0;
    char c;
    while(!isdigit(c = getchar()));
    ret = c - '0';
    while(isdigit(c = getchar())){
        ret = ret * 10 + (c - '0');
    }
    return ret; 
}

class BIT{
public:
    int a[MAXN], cnt;
    inline static int lowbit(int x){
        return x & (- x);
    }
    inline void init(){
        fill(a, a + N + 1, 0);
        cnt = 0;
    }
    inline void add(int p, int v){
        cnt += v;
        for(; p <= N; p += lowbit(p))
            a[p] += v;
    }
    inline int sum(int p){
        int sum = 0;
        for(; p > 0; p -= lowbit(p))
            sum += a[p];
        return sum;

    }
}bit;

class LCT{
private:
    typedef struct NODE{
        NODE *l, *r, *par;  //左，右，父亲
        bool root;          //是否为根（可以用函数查）
        int fix, size;

        //补充标记
        bool rev;    //反转
        int val;    //本值
        NODE* mx;    //最小值点
        NODE(int val = INF):fix(rand()),size(1),val(val){
            par = l = r = nullptr;
            root = true;

            mx = this;
            rev = false;
        };
    }*PNODE;
    PNODE root;
    NODE node[MAXN], edl[MAXM];    //代表顶点和边的点
    typedef pair<PNODE, PNODE> Droot;

    inline int size(PNODE cur){  //取得大小
        return cur ? cur->size : 0;
    }
    inline void do_rev(PNODE cur){
        if(cur){
            cur->rev = !cur->rev;
            swap(cur->l, cur->r);
        }
    }
    inline void pushdown(PNODE cur){
        if(cur->rev){
            do_rev(cur->l);
            do_rev(cur->r);
            cur->rev = false;
        }
    }
    inline void popup(PNODE cur){
        cur->size = 1;
        cur->mx = cur;
        if(cur->l){
            cur->l->root = false;
            cur->l->par = cur;
            cur->size += cur->l->size;
            if(cur->l->mx->val < cur->mx->val)
                cur->mx = cur->l->mx;
        }
        if(cur->r){
            cur->r->root = false;
            cur->r->par = cur;
            cur->size += cur->r->size;
            if(cur->r->mx->val < cur->mx->val)
                cur->mx = cur->r->mx;
        }
    }

	//这里其实是可以自底向上写的，比较烦没写
    inline void getrank(PNODE A, PNODE& rt, int& rk){
        static PNODE stk[MAXN];
        int sp = 0;
        //先强制更新
        rt = A;
        while(!rt->root){
            rt = rt->par;
            stk[sp ++] = rt;
        }
        while(sp --){
            pushdown(stk[sp]);
        }
        //再处理操作
        rt = A;
        rk = size(rt->l) + 1;
        while(!rt->root){
            if(rt == rt->par->r)
                rk += rt->par->size - rt->size;
            rt = rt->par;
        }
    }
    PNODE concat_it(PNODE A, PNODE B){ //保序合并
        if(!A) return B;
        if(!B) return A;
        if(A->fix < B->fix){
            pushdown(A);
            A->r = concat_it(A->r, B);
            popup(A);
            return A;
        }else{
            pushdown(B);
            B->l = concat_it(A, B->l);
            popup(B);
            return B;
        }
    }
    inline PNODE concat(PNODE A, PNODE B){
        PNODE ret = concat_it(A, B);
        ret->root = true;
        return ret;
    }
    Droot split_it(PNODE A, int k){
        if(!A) return Droot(nullptr, nullptr);
        Droot ret;
        pushdown(A);
        A->root = true;
        if(size(A->l) >= k){
            ret = split_it(A->l, k);
            A->l = ret.second;
            popup(A);
            ret.second = A;
        }else{
            ret = split_it(A->r, k - size(A->l) - 1);
            A->r = ret.first;
            popup(A);
            ret.first = A;
        }
        return ret;
    }
    inline Droot split(PNODE rt, int k){  //分裂点，排位
        NODE *tmp = rt->par;
        Droot ret = split_it(rt, k);
        if(ret.first)
            ret.first->par = tmp;
        if(ret.second){
            for(tmp = ret.first; tmp->r; tmp = tmp->r);
            ret.second->par = tmp;
        }
        return ret;
    }
    PNODE access(PNODE A){
        PNODE rt, pchd = nullptr;
        int rk;
        while(A){
            getrank(A, rt, rk);
            A = rt->par;
            pchd = concat(split(rt, rk).first, pchd);
            pchd->par = A;
        }
        return pchd;
    }
private:
    inline NODE* GetRt(PNODE p){
        while(p->par)
            p = p->par;
        return p;
    }
    inline void Link(int j){
        edl[j].par = node + edge[j].a;
        PNODE rt = access(edl + j);
        do_rev(rt);
        rt->par = node + edge[j].b;
        edge[j].used = true;
    }
    inline void DoCut(PNODE u){
        PNODE rt;
        int rk;
        access(u);
        getrank(u, rt, rk);
        rt = split(rt, rk - 1).second;
        rt->par = nullptr;
    }
    inline void Cut(int j){
        ChangeRoot(edl + j);
        DoCut(node + edge[j].a);
        DoCut(node + edge[j].b);
        edge[j].used = false;
    }
    inline void ChangeRoot(PNODE u){
        do_rev(access(u));
    }

public:
    //题目中操作：加边换边
    void AddEdge(int j){
        PNODE u = node + edge[j].a, v = node + edge[j].b, z;
        //是否直接链接？
        if(GetRt(u) != GetRt(v)){
            bit.add(edge[j].a, 1);
            Link(j);
            return;
        }
        //访问路径最小
        ChangeRoot(u);
        v = access(v);
        z = v->mx;
        //换边？
        if(edge[j].a > z->val){
            int lab = z - edl;
            bit.add(edge[lab].a, - 1);
            Cut(lab);
            bit.add(edge[j].a, 1);
            Link(j);
        }
    }
    //初始化
    void Init(int N, int M, int Q){
        int i;

        bit.init();
        for(i = 0; i <= N; i ++)
            node[i] = NODE();
        for(i = 0; i < M; i ++){
            edge[i].a = ReadInt();
            edge[i].b = ReadInt();
            if(edge[i].a > edge[i].b)
                swap(edge[i].a, edge[i].b);
            edge[i].used = false;
            edl[i] = NODE(edge[i].a);
            opt[i] = OPT(edge[i].b, true, i);
        }
        for(i = 0; i < Q; i ++){
            query[i].l = ReadInt();
            query[i].r = ReadInt();
            opt[i + M] = OPT(query[i].r, false, i);
        }
        sort(opt, opt + M + Q);
    }
    inline int Query(int L){
        return N - bit.cnt + bit.sum(L - 1);
    }
}tree;

//被卡常数的特判
inline void case1(){
    int a, b, i;
    for(i = 0; i < M + Q; i ++){
        a = ReadInt();
        b = ReadInt();
    }
    for(i = 0; i < Q; i ++){
        printf("%d\n", N);
    }
}

int main(){
    int i, cas = 0;
    while(scanf("%d%d%d", &N, &M, &Q) != EOF){
        ++ cas;
        if(M == 0){
            case1();
            continue;
        }
        tree.Init(N, M, Q);
        for(i = 0; i < M + Q; i ++){
            if(opt[i].add){
                tree.AddEdge(opt[i].id);
            }else{
                query[opt[i].id].ans = tree.Query(query[opt[i].id].l);
            }
        }
        for(i = 0; i < Q; i ++){
            printf("%d\n", query[i].ans);
        }
    }
    return 0;
}