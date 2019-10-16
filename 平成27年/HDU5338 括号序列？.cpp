#include <cstdio>
#include <set>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
int arr[MAXN], go[MAXN], pos[MAXN];
int N;

class Segtree{
public:
    int mv[262144], n; //最大值
    inline void Init(int N){
        int i, tn = N;
        for(n = 1, N ++; N > 0; N >>= 1, n <<= 1);
        fill(mv + n + tn + 1, mv + (n << 1), 0);
		copy(arr, arr + tn + 1, mv + n);
        for(i = n - 1; i > 0; i --)
            mv[i] = max(mv[i<<1], mv[i<<1|1]);
    }
    inline void Update(int p, int v){
        p += n;
        mv[p] = v;
        for(p >>= 1; p > 0; p >>= 1)
            mv[p] = max(mv[p<<1], mv[p<<1|1]);
    }
    inline int Query(int l, int r){
        int ret = -1;
        for(l += n - 1, r += n + 1; l ^ r ^ 1; l >>= 1, r >>= 1){
            if(~l & 1)
                ret = max(ret, mv[l ^ 1]);
            if(r & 1)
                ret = max(ret, mv[r ^ 1]);
        }
        return ret;
    }
}segt;
set<int> blk;    //阻断节点

//计算左临界
inline int lbound(const int ps){
	return *(blk.lower_bound(ps).operator--()) + 1;
}
//计算单点
void deal(int ps){
    int lv = segt.Query(lbound(ps), ps), rv = arr[ps + 1];
    if(go[rv] >= 0 || rv < lv){
        //连左边
        go[arr[ps]] = lv;
        for(int i = pos[lv]; i < ps; i ++)
            go[arr[i]] = arr[i + 1];
        blk.insert(ps);
    }else{
        //连右边
        segt.Update(ps + 1, 0);
    }
}

void domain(){
    int i;
    scanf("%d", &N);
    for(i = 1; i <= N; i ++){
        scanf("%d", &arr[i]);
        pos[arr[i]] = i;
        go[i] = -1;
    }
    arr[N + 1] = 0;
    blk.clear();
    blk.insert(0);
    segt.Init(N);
    for(i = 1; i <= N; i ++){
        if(go[i] == -1)
            deal(pos[i]);
    }
    for(i = 1; i <= N; i ++){
        printf("%d", go[i]);
        putchar(i < N ? ' ' : '\n');
    }
}
int main(){
	int T;
	scanf("%d", &T);
	while(T --)
		domain();
	return 0;
}