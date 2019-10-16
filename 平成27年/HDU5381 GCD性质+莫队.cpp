#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cctype>
using namespace std;

const int MAXN = 10005;
const int MAX_LOG = 17;
typedef long long ll;
typedef pair<int, int> pii;

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

inline char getc(){
    static const int BUFLEN = 1 << 15;
    static char B[BUFLEN], *S = B, *T = B;
    if(S == T){
        S = B;
        T = S + fread(B, 1, BUFLEN, stdin);
    }
    return (S == T) ? 0 : *(S ++);
}
int ReadInt(){
    char ch;
    do ch = getc(); while(!isdigit(ch));
    int aa = ch - '0';
    for(ch = getc(); isdigit(ch); ch = getc())
        aa = aa * 10 + ch - '0';
    return aa;
}

struct QUERY{
    int id, l, r, pos;
    ll ans;
    bool operator<(const QUERY& rhs)const{
        if(pos != rhs.pos)
            return pos < rhs.pos;
        else
            return r < rhs.r;
    }
}que[MAXN];
int comp(const void* A, const void* B){
    return ((QUERY*)A)->id - ((QUERY*)B)->id;
}

int N, M;
int arr[MAXN];

namespace rgq{
    int pre_log[MAXN];
    int st[MAX_LOG][MAXN];

    void init(){
        pre_log[1] = 0;
        for(int i = 2; i < MAXN; i ++)
            pre_log[i] = pre_log[(i >> 1)] + 1;
    }
    inline void pre(){
        int i, j, m = pre_log[N], p;
        copy(arr + 1, arr + 1 + N, st[0] + 1);
        for(j = 1; j <= m; j ++){
            p = 1 << (j - 1);
            for(i = N - (p << 1) + 1; i >= 1; i --)
                st[j][i] = gcd(st[j-1][i], st[j-1][i + p]);
        }
    }
    inline int query(int l, int r){
        int t = pre_log[r - l + 1];
        return gcd(st[t][l], st[t][r - (1 << t) + 1]);
    }
};

namespace sol{
    pii val[MAXN][30], val2[MAXN][30];    //gcd的下降阶梯
    ll preSum[MAXN][30], preSum2[MAXN][30];
    int sz[MAXN], sz2[MAXN];

    void presub(int i, pii val[30], ll ps[30], int& sz){
        int low, high, pos, mid;
        int tar = arr[i], cur = i;    //目标值，位置

        while (cur <= N) {
            low = cur, high = N;
            pos = -1;
            while (low <= high) {
                mid = (low + high) >> 1;
                if (rgq::query(i, mid) == tar) {
                    pos = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            val[sz] = pii(pos, tar);
            ps[sz] = 1ll * tar * (pos - cur + 1);
            sz ++;
            cur = pos + 1;
            tar = rgq::query(i, cur);
        }
        for(pos = 1; pos < sz; ++ pos)
            ps[pos] += ps[pos - 1];
    }

    inline void pre(){
        int i;

        for(i = 1; i <= N; ++ i)
            sz[i] = sz2[i] = 0;
        rgq::pre();
        for(i = 1; i <= N; ++ i)
            presub(i, val[i], preSum[i], sz[i]);
        reverse(arr + 1, arr + N + 1);
        rgq::pre();
        for(i = 1; i <= N; ++ i)
            presub(i, val2[i], preSum2[i], sz2[i]);
    }

    //sum[i=l..r]{gcd(l..i)}
    ll fixLeft(int l, int r){
        ll ret = 0;
        pii tt = pii(r, -1);
        int id = lower_bound(val[l], val[l] + sz[l], tt) - val[l];
        if(id > 0){
            ret += preSum[l][id - 1];
            ret += 1ll * (r - val[l][id - 1].first) * val[l][id].second;
        }else{
            ret += 1ll * (r - l + 1) * val[l][id].second;
        }
        return ret;
    }

    //sum[i=l..r]{gcd(i..r)}
    ll fixRight(int l, int r){
        swap(l, r);
        l = N + 1 - l, r = N + 1 - r;

        ll ret = 0;
        pii tt = pii(r, -1);
        int id = lower_bound(val2[l], val2[l] + sz2[l], tt) - val2[l];
        if(id > 0){
            ret += preSum2[l][id - 1];
            ret += 1ll * (r - val2[l][id - 1].first) * val2[l][id].second;
        }else{
            ret += 1ll * (r - l + 1) * val2[l][id].second;
        }
        return ret;
    }
};

void input(){
    int i, W;

    N = ReadInt();
    for(i = 1; i <= N; i ++)
        arr[i] = ReadInt();
    M = ReadInt();
    W = static_cast<int>(sqrt(N));
    for(i = 0; i < M; i ++){
        que[i].id = i;
        que[i].l = ReadInt(), que[i].r = ReadInt();
        que[i].pos = que[i].l / W;
    }
    sol::pre();
}

void deal(){
    int i, L = 1, R = 0;
    ll ans = 0;
    for(i = 0; i < M; i ++){
        QUERY& q = que[i];
        for(; R < q.r; R ++)
            ans = ans + sol::fixRight(L, R + 1);
        for(; R > q.r; R --)
            ans = ans - sol::fixRight(L, R);
        for(; L < q.l; L ++)
            ans = ans - sol::fixLeft(L, R);
        for(; L > q.l; L --)
            ans = ans + sol::fixLeft(L - 1, R);
        q.ans = ans;
    }
}

int main(){
    int i, T;
    rgq::init();
    T = ReadInt();
    while(T --){
        input();
        sort(que, que + M);
        deal();
        qsort(que, M, sizeof(QUERY), comp);
        for(i = 0; i < M; i ++)
            printf("%I64d\n", que[i].ans);
    }
    return 0;
}