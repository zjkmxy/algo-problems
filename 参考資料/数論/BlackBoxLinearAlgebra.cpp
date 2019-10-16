#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

const int MAXN = 10005;
const int MODN = 1000000007;
typedef long long int LLI;

template<typename T>
struct Polynomial {
  T v[MAXN];
  int sz;
  static int mod;
  Polynomial() : sz(0) {}
  Polynomial(int n, const T &x = 0) : sz(n) { fill(v, v + n, x); }
  T &operator [](int k) {
    return v[k];
  }
  const T &operator [](int k) const{
    return v[k];
  }
  Polynomial operator *(const Polynomial &o) const {
    Polynomial ret(sz + o.sz - 1);
    int i, j;
    for (i = 0; i < sz; ++i) {
      if (!v[i]) continue;
      for (j = 0; j < o.sz; ++j) {
        ret.v[i + j] += v[i] * o.v[j];
        ret.v[i + j] %= mod;
      }
    }
    return ret;
  }

  Polynomial operator %(const Polynomial &o) const {
    Polynomial ret = *this;
    int i, j, n = o.sz - 1;
    for (i = sz - 1; i >= n; --i) {
      if (!ret.v[i])
        continue;
      for (j = 1; j <= n; ++j) {
        ret.v[i - j] -= ret.v[i] * o.v[n - j];
        if ((ret.v[i - j] %= mod) < 0)
          ret.v[i - j] += mod;
      }
    }
    ret.sz = min(ret.sz, n);
    return ret;
  }

  Polynomial pow(LLI n, const Polynomial &p) const {
    Polynomial ret(1, 1), a = *this;
    for (; n; n >>= 1) {
      if (n & 1)
        ret = (ret * a) % p;
      a = (a * a) % p;
    }
    return ret;
  }
};
template<typename X>
int Polynomial<X>::mod = MODN;

LLI powm(LLI x, LLI n, LLI mod){
  LLI ret = 1;
  for (; n > 0; n >>= 1){
    if (n & 1)
      ret = ret * x % mod;
    x = x * x % mod;
  }
  return ret;
}

inline LLI invm(LLI x, LLI mod){
  return powm(x, mod - 2, mod);
}

//Berlekamp–Massey算法
//arr[0..N]是数列，ret是递推式：a[i]=sum{ret[j]*a[i-j]}
void BM(int arr[], int N, Polynomial<LLI>& ret){
  int i, j;
  LLI dn, coe;
  bool flg;
  //失败多项式、位置、误差
  Polynomial<LLI> tmp(1), bak;
  int pos = 0;
  LLI dm = arr[0];

  //初始多项式为a[i]=0，在i=0时失败，被修正为a[i]=arr[1]/arr[0]*a[i-1]
  tmp[0] = 0;
  ret.sz = 2;
  ret[0] = 0;
  ret[1] = arr[1] * invm(arr[0], ret.mod) % ret.mod;
  for (i = 2; i < N; i++){
    //计算误差dn
    //后面为了直观期间用了很多减法比较烦，想简化代码的话，
    //修改这些多项式的含义，把所有的减法和取模变成加法取模即可。
    dn = arr[i];
    for (j = 1; j < ret.sz; j++)
      dn -= arr[i - j] * ret[j] % ret.mod;
    dn = (dn % ret.mod + ret.mod) % ret.mod;
    if (dn == 0)
      continue;

    //备份原多项式，开辟空间
    flg = (tmp.sz + i - pos > ret.sz);
    if (flg){
      bak = ret;
      for (j = ret.sz; j < tmp.sz + i - pos; j++)
        ret[j] = 0;
    }
    //计算系数
    coe = invm(dm, ret.mod) * dn % ret.mod;
    //修正多项式
    for (j = 1; j < tmp.sz; j++)
      ret[j + i - pos] -= tmp[j] * coe % ret.mod;
    ret[i - pos] += coe;
    for (j = 0; j < tmp.sz; j++)
      ret[j + i - pos] = (ret[j + i - pos] % ret.mod + ret.mod) % ret.mod;
    //修正失败多项式
    if (flg){
      ret.sz = tmp.sz + i - pos;
      dm = dn;
      tmp = bak;
      pos = i;
    }
#ifdef _DEBUG
    //本机调试时演算结果
    dn = arr[i];
    for (j = 1; j < ret.sz; j++)
      dn -= arr[i - j] * ret[j] % ret.mod;
    dn = (dn % ret.mod + ret.mod) % ret.mod;
    if (dn != 0)
      throw 0;
#endif
  }
}

//Hamilton-Cayley定理
//B是递推式：a[i]=sum{B[j]*a[i-j]}
//ret是a[N]的依赖：a[N]=sum{ret[j]*a[j+1]}
//假定a从1开始而ret从0开始
Polynomial<LLI> HC(const Polynomial<LLI>& B, int N){
  Polynomial<LLI> u(B.sz), w(2);
  int i;

  N--;
  //u是递推矩阵的特征多项式
  //这里是一个非常明显的零化多项式
  u[B.sz - 1] = 1;
  for (i = 1; i < B.sz; i ++)
    u[B.sz - 1 - i] = -B[i];
  //x^N % u即答案
  w[1] = 1;
  w = w.pow(N, u);
  return w;
}

int main()
{
  int N, M, i;
  int arr[20];

  //本程序为：给定N阶递推数列的前2N项（a[1]..a[2N]），求第M项
  //稀疏矩阵乘法那里懒得写了。
  //其实就是认为A^i*b的向量列是一个具有N阶递推关系的列
  //然后用随机数把它压缩成数列，用BM求出这个递推关系
  //再用MC把递推式转化成第M项对前N项的依赖式
  //然后把b,A^1*b,...,A^(N-1)*b套进依赖式里算出A^M*b
  while (scanf("%d%d", &N, &M) != EOF){
    for (i = 0; i < N * 2; i++)
      scanf("%d", &arr[i]);
    Polynomial<LLI> iter;
    BM(arr, N * 2, iter);
    if (M > N){
      iter = HC(iter, M);
      LLI ret = 0;
      for (i = 0; i < N; i++){
        ret += arr[i] * iter[i] % MODN;
      }
      ret = ret % MODN;
      printf("%d\n", ret);
    }
    else{
      printf("%d\n", arr[M]);
    }
  }
  return 0;
}