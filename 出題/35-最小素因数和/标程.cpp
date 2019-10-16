//全体的の流れは：オイラーの線形篩、包除原理、（素数和）動的計画法、（合成数和）二重篩
//計算量は約O(R^0.8)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
//#include <omp.h>

const long long L = 1000;
const long long R = 20000000;
const long long MAXN = 1000000001ll;
const int QSIZ = 2000000;
const int PRIMESIZE = 100000;
const int K = 100;
int minfac[PRIMESIZE + 1], prime[PRIMESIZE / 10], tot;
#define __USE_INCEXC_ONLY 0

long long ans;

double getrusage_sec(){
    
    struct rusage t;
    struct timeval tv;
    getrusage(RUSAGE_SELF, &t);
    tv = t.ru_utime;
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
    
}

//包除原理で合数の最小素因数の和を計算す
//全部合数の和(そして素数の和)も同じ方法で計算出来るが、掛け算があるので今しない
//並列化して2倍速計算可能
void incExc(int maxp){
    long long curans;
    static long long q[3][QSIZ];
    int qsiz, p, i, k, l, m;
    
    qsiz = 1;
    p = 0;
    q[0][0] = 1;
    for(i = 0; i < tot && prime[i] <= maxp; i ++, p ^= 1){
        curans = 0;
        //ここブレーク条件を事前に計算
        for(int j = 0; j < qsiz; j ++){
            long long cur = q[p][j] * prime[i];
            if(cur > R || cur < -R){
                qsiz = j;
                break;
            }
            curans += R / cur - (L - 1) / cur;
            q[2][j] = - cur;
        }
        
        //ここ両端から中央へ
        q[p][qsiz] = q[2][qsiz] = R + 1;
        qsiz <<= 1;
        for(k = l = m = 0; m < qsiz; m++){
            if(llabs(q[p][k]) < llabs(q[2][l])){
                q[p^1][m] = q[p][k ++];
            }else{
                q[p^1][m] = q[2][l ++];
            }
        }
        
        //値 = N / 今までの積 * 今の素数
        ans += curans * prime[i];
    }
}

//N以下全部素数の和
//並列化出来るがあまりに効果なし
long long primeSum(long long N){
    int i, vs, t, p, r;
    static long long v[PRIMESIZE * 2 + 2];
    // S[i] = SumOfPrime[v[i]], v[i]以下の全ての素数の和
    static long long S[PRIMESIZE * 2 + 2];
    long long sp, p2;
    int bound1, bound2;
    
    r = (int)sqrt(N);
    for(i = 1; i <= r; i ++){
        v[i] = N / i;
    }
    for(; v[i - 1] > 1; i ++){
        v[i] = v[i - 1] - 1;
    }
    vs = i;
    // 初期値:S[i]=v[i]以下全部数字の和-1
    for(i = 1; i < vs; i ++){
        S[i] = v[i] * (v[i] + 1) / 2 - 1;
    }
    // sqrt(N)以下の素数に対し篩に掛ける
    // pに対し: SumOfPrime[x] -= p * (SumOfPrime[x / p] - SumOfPrime[p - 1]);
    bound2 = vs - 1;
    for(t = 0; t < tot; t ++){
        p = prime[t];
        sp = S[vs - p + 1];
        p2 = p * p;
        while(v[bound2] < p2) bound2 --;
        bound1 = r / p;
        bound1 = bound1 < bound2 ? bound1 : bound2;
        for(i = 1; i <= bound1; i ++){
            S[i] -= p * (S[i * p] - sp);
        }
        for(i = bound1 + 1; i <= bound2; i ++){
            S[i] -= p * (S[vs - (int)(v[i] / p)] - sp);
        }
    }
    return S[1];
}

void dblSieve(){
    static int res[MAXN / K + 1];
    int cnt, i, rbound, cur;
    int cnt2, lbound;
    int newbndr, j, newbndl;
    
    rbound = (int)(R / K);
    lbound = (int)((L - 1) / K);
    for(i = 1; i <= rbound; i ++){
        res[i] = 1;
    }
    cnt = cnt2 = 0;
    for(i = 0; i < tot; i ++){
        cur = prime[i];
        if(cur > K){
            //上界を更新
            newbndr = (int)(R / cur);
            newbndl = (int)((L - 1) / cur);
            for(; rbound > newbndr; -- rbound){
                cnt -= (res[rbound] ^ 1);
            }
            for(; lbound > newbndl; -- lbound){
                cnt2 -= (res[lbound] ^ 1);
            }
            //答えの更新
            ans += (long long)((newbndr - cnt) - (newbndl - cnt2)) * cur;
        }
        //篩にかけ
        for(j = cur; j <= lbound; j += cur){
            cnt2 += res[j];
            cnt += res[j];
            res[j] = 0;
        }
        for(; j <= rbound; j += cur){
            cnt += res[j];
            res[j] = 0;
        }
    }
}

int main(){
    int sqrtr, i, j;
    double t1, t2;
    
    t1 = getrusage_sec();
    
    sqrtr = sqrt(R);
    // minfac[i]==m => iの最小素因数==prime[m-1]
    for(i = 2; i <= sqrtr; i ++){
        if(minfac[i] == 0) {
            prime[tot] = i;
            ++ tot;
            minfac[i] = tot;
        }
        for(j = 0; j < minfac[i]; j ++){
            if(i * prime[j] > sqrtr) break;
            minfac[i * prime[j]] = j + 1;
        }
    }
    
    ans = 0;
    if(__USE_INCEXC_ONLY){
        incExc(sqrtr);
    }else{
        incExc(K);
        dblSieve();
    }
    
    for(int i = 0; i < tot; i ++){
        //Lより小さい素数はprimeSumから引く
        //Lより大きい素数はincExcより引く
        ans -= prime[i];
    }
    ans += primeSum(R);
    
    t2 = getrusage_sec();
    
    printf("answer %lld\n", ans);
    printf("time %f\n", t2 - t1);
    
    return 0;
}
