#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cctype>
using namespace std;

typedef long long ll;
typedef unsigned  long ul;
typedef unsigned  long long ull;
const ul MODN = 1000000007;
const int MAXN = 2005;

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
    while(ch=getc(), !isdigit(ch));
    int aa = ch - '0';
    while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
    return aa;
}

inline ul ml(ul a,ul b){
    ul ret;
    __asm__ __volatile__ ("\tmull %%ebx\n\tdivl %%ecx\n" :"=d"(ret):"a"(a),"b"(b),"c"(MODN));
    return ret;
}

struct uint32{
    ul n;
    uint32(ul n = 0):n(n){}
    void assign(int x){    for(x %= MODN; x < 0; x += MODN);    n = (ul)x;    }
    inline uint32 operator-() const{    return uint32(MODN - n);    };
    friend inline uint32 operator*(const uint32& a, const uint32& b){
        return ml(a.n, b.n);
    }
    friend inline uint32 operator+(const uint32& a, const uint32& b){
        ul n = a.n + b.n;
        return uint32((n < a.n || n >= MODN) ? (n - MODN) : n);
    }
    friend inline uint32 operator-(const uint32& a, const uint32& b){
        return (a.n < b.n) ? (a.n + MODN - b.n) : (a.n - b.n);
    }
    uint32 pow(ul p) const{
        uint32 ret(1), a(n);
        for(; p >= 1; p >>= 1, a = a * a)    if(p & 1) ret = ret * a;
        return ret;
    }
    uint32 inv() const{
        int x1 = 1, x2 = 0, x3 = MODN, y1 = 0, y2 = 1, y3 = n, k;
        while (y3 != 1){
            k = x3 / y3, x1 -= y1 * k, x2 -= y2 * k, x3 -= y3 * k;
            swap(x1, y1), swap(x2, y2), swap(x3, y3);
        }
        return uint32(y2 >= 0 ? y2 : y2 + MODN);
    }
    friend inline uint32 operator/(const uint32& a, const uint32& b){    return a * b.inv();    }
    inline uint32 div2() const{    return (*this) * uint32((MODN + 1) >> 1);    };
    inline uint32 sqrt()const{    return pow(MODN - 2);     }
};
uint32 P[MAXN][MAXN], mpow[MAXN];
int N, M;

//P表示：选i个元素，恰好有j种元素是奇数个的方案
void calcP(){
    int i, j;

    mpow[0] = 1;
    for(j = 1; j <= N; j ++)
        mpow[j] = mpow[j - 1] * M;
    for(i = 0; i <= N; i ++){
        for(j = 0; j <= M; j ++){
            P[i][j] = 0;
        }
    }
    P[0][0] = 1;
    for(i = 0; i <= N; ++ i){
        for(j = i & 1; j <= M; j += 2){
            if(j > 0)
                P[i + 1][j - 1] = P[i + 1][j - 1] + P[i][j] * j;
            if(j < M)
                P[i + 1][j + 1] = P[i + 1][j + 1] + P[i][j] * (M - j);
        }
    }
}

void domain(){
    int i, j, len;
    uint32 ans = 0;

    N = ReadInt(), M = ReadInt();
    calcP();
    for(len = 1; len <= N; len ++){
        ans = ans + P[len][len & 1] * mpow[N - len] * (N - len + 1);
    }
    printf("%lu\n", ans.n);
}

int main(){
    int T;
    T = ReadInt();
    while(T --)
        domain();
    return 0;
}