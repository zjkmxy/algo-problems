/**************************************************************
    Problem: 4066
    User: zjkmxy
    Language: C++
    Result: Accepted
    Time:32172 ms
    Memory:10188 kb
****************************************************************/

/*
KD-Tree模板题
插入的时候，当成每次换维度比较的替罪羊，额外维护子树覆盖矩形区域。（然而不能段改）
删除的时候，先用mn和mx找在被删除结点比较维度下的前驱或者后继（和二叉树不同），换过来直接删
*/

#define nullptr NULL
#include <cstdio>
#include <set>
#include <list>
#include <algorithm>
#include <cctype>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
 
const int MAXN = 200005;
const double ALPHA = 0.8;
 
class Tree{
public:
    static int cpk; //正在比较的维度
    struct NODE{
        NODE *lchd, *rchd;
        int d[2];   //坐标
        int val;    //本值
 
        int mn[2], mx[2];//最小最大坐标
        int sum, cnt;   //和值
        inline bool operator<(const NODE& rhs)const{
            return d[cpk] < rhs.d[cpk];
        }
        inline bool operator==(const NODE& rhs)const{
            return d[0] == rhs.d[0] && d[1] == rhs.d[1];
        };
        inline bool In(const NODE& rhs)const{
            return rhs.mn[0] <= mn[0] && mx[0] <= rhs.mx[0] && rhs.mn[1] <= mn[1] && mx[1] <= rhs.mx[1];
        }
        inline bool Out(const NODE& rhs){
            return mn[0] > rhs.mx[0] || mx[0] < rhs.mn[0] || mn[1] > rhs.mx[1] || mx[1] < rhs.mn[1];
        }
        inline int size()const{
            return this ? cnt : 0;
        }
        inline void popup(){
            mn[0] = mx[0] = d[0];
            mn[1] = mx[1] = d[1];
            sum = val;
            cnt = 1;
            if(lchd){
                mn[0] = min(mn[0], lchd->mn[0]);
                mx[0] = max(mx[0], lchd->mx[0]);
                mn[1] = min(mn[1], lchd->mn[1]);
                mx[1] = max(mx[1], lchd->mx[1]);
                sum += lchd->sum;
                cnt += lchd->cnt;
            }
            if(rchd){
                mn[0] = min(mn[0], rchd->mn[0]);
                mx[0] = max(mx[0], rchd->mx[0]);
                mn[1] = min(mn[1], rchd->mn[1]);
                mx[1] = max(mx[1], rchd->mx[1]);
                sum += rchd->sum;
                cnt += rchd->cnt;
            }
        }
    }node[MAXN];
    typedef NODE *PNODE;
    inline static bool comp(NODE* l, NODE* r){
        return (*l) < (*r);
    }
    PNODE dfn[MAXN], root;
    int ncnt;
 
    PNODE Build(int L, int R, int k){
        PNODE ret = nullptr;
        if(L > R)
            return nullptr;
        int mid = (L + R) >> 1;
        cpk = k;
        nth_element(dfn + L, dfn + mid, dfn + R + 1, comp);
        ret = dfn[mid];
        ret->lchd = Build(L, mid - 1, k ^ 1);
        ret->rchd = Build(mid + 1, R, k ^ 1);
        ret->popup();
        return ret;
    }
    int Remove(PNODE &rt, PNODE lst[]){
        int cnt = 0;
        if(!rt)
            return 0;
        cnt += Remove(rt->lchd, lst);
        lst[cnt ++] = rt;
        cnt += Remove(rt->rchd, lst + cnt);
        rt = nullptr;
        return cnt;
    }
    inline void Rebuild(PNODE& rt, int k){
        rt = Build(0, Remove(rt, dfn) - 1, k);
    }
    PNODE Insert(PNODE& rt, const NODE& now, int k){
        if(!rt){
            rt = &node[ncnt ++];
            rt->mn[0] = rt->mx[0] = rt->d[0] = now.d[0];
            rt->mn[1] = rt->mx[1] = rt->d[1] = now.d[1];
            rt->val = rt->sum = 0;
            rt->cnt = 1;
            rt->lchd = rt->rchd = nullptr;
        }
        if(*rt == now){
            rt->val += now.val;
            rt->sum += now.val;
            return nullptr;
        }
        PNODE scape;
        if(now.d[k] < rt->d[k])
            scape = Insert(rt->lchd, now, k ^ 1);
        else
            scape = Insert(rt->rchd, now, k ^ 1);
        rt->popup();
        if(rt->cnt * ALPHA > max(rt->lchd->size(), rt->rchd->size())){
            if(scape != nullptr){
                if(rt->lchd == scape)
                    Rebuild(rt->lchd, k ^ 1);
                else
                    Rebuild(rt->rchd, k ^ 1);
            }
            return nullptr;
        }else
            return rt;
    }
    int Query(PNODE& rt, const NODE& now){
        if(!rt)
            return 0;
        int sum = 0;
        if(rt->In(now)){
            return rt->sum;
        }else if(rt->Out(now)){
            return 0;
        }else{
            if(rt->d[0] >= now.mn[0] && rt->d[0] <= now.mx[0] && rt->d[1] >= now.mn[1] && rt->d[1] <= now.mx[1])
                sum += rt->val;
        }
        sum += Query(rt->lchd, now) + Query(rt->rchd, now);
        return sum;
    }
    inline void Init(){
        ncnt = 0;
        root = nullptr;
    }
}tree;
int Tree::cpk = 0;
 
int N;
int lastans = 0;
int main(){
    Tree::NODE now;
    int op = 0;
    scanf("%d", &N);
    tree.Init();
    while(true){
        scanf("%d", &op);
        if(op == 3)
            break;
        if(op == 1){
            scanf("%d%d%d", &now.d[0], &now.d[1], &now.val);
            now.d[0] ^= lastans, now.d[1] ^= lastans, now.val ^= lastans;
            tree.Insert(tree.root, now, 0);
        }else{
            scanf("%d%d%d%d", &now.mn[0], &now.mn[1], &now.mx[0], &now.mx[1]);
            now.mn[0] ^= lastans, now.mn[1] ^= lastans, now.mx[0] ^= lastans, now.mx[1] ^= lastans;
            lastans = tree.Query(tree.root, now);
            printf("%d\n", lastans);
        }
    }
    return 0;
}