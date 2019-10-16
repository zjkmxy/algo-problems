/*
维护X的自乘和N次自乘
因为X的次数不超过10次
所以用10次多项式可以免TLE
*/
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXP = 11;
const int MODN = 1000000000;
const int MAXN = 100005;
typedef unsigned long ul;
typedef long long ll;
typedef unsigned long long ull;

inline void assert(const bool exp){
	if(!exp) throw 0;
}

struct uint32{
	ul n;
	uint32(ul n = 0):n(n){}
	void assign(int x){
		for(x %= MODN; x < 0; x += MODN);
		n = (ul)x;
	}
	inline uint32 operator-() const{
		return uint32(MODN - n);
	};
	friend inline uint32 operator*(const uint32& a, const uint32& b){
		ull A(a.n), B(b.n);
		return uint32(A * B % MODN);
	}
	friend inline uint32 operator+(const uint32& a, const uint32& b){
		ul n = a.n + b.n;
		return uint32((n < a.n || n >= MODN) ? (n - MODN) : n);
	}
	friend inline uint32 operator-(const uint32& a, const uint32& b){
		return (a.n < b.n) ? (a.n + MODN - b.n) : (a.n - b.n);
	}
};
class Polynominal{
public:
	static uint32 x[MAXN];
	static uint32 xp[MAXP];
	static int n;
	uint32 v[MAXP];
	Polynominal(int val = 0){
		fill(v + 1, v + MAXP, uint32(0));
		v[0] = val;
	}
	friend inline Polynominal operator+(const Polynominal& a, const Polynominal& b){
		Polynominal ret;
		for(int i = 0; i < MAXP; i ++)
			ret.v[i] = a.v[i] + b.v[i];
		return ret;
	}
	friend inline Polynominal operator-(const Polynominal& a, const Polynominal& b){
		Polynominal ret;
		for(int i = 0; i < MAXP; i ++)
			ret.v[i] = a.v[i] - b.v[i];
		return ret;
	}
	friend inline Polynominal operator*(const Polynominal& a, const Polynominal& b){
		Polynominal ret;
		for(int i = 0; i < MAXP; i ++){
			for(int j = MAXP - i - 1; j >= 0; j --){
				ret.v[i + j] = ret.v[i + j] + a.v[i] * b.v[j];
			}
		}
		return ret;
	}
	inline Polynominal minus()const{
		Polynominal ret;
		for(int i = 0; i < MAXP; i ++)
			ret.v[i] = - v[i];
		return ret;
	}
	inline Polynominal sqr()const{
		return (*this) * (*this);
	}
	inline Polynominal fold()const{
		Polynominal ret;
		for(int i = 0; i < MAXP; i ++)
			ret.v[0] = ret.v[0] + this->v[i] * xp[i];
		return ret;
	};
	static inline Polynominal N(){
		return Polynominal(n);
	}
	static inline Polynominal X(){
		Polynominal ret;
		ret.v[1] = 1;
		return ret;
	}
	static void Prepare(){
		static uint32 tmp[MAXN];
		int i, j;
		scanf("%d", &n);
		for(i = 0; i < n; i ++)
			scanf("%d", &x[i]);
		for(i = 0; i < n; i ++)
			tmp[i] = 1;
		for(j = 0; j < MAXP; j ++){
			xp[j] = 0;
			for(i = 0; i < n; i ++)
				xp[j] = xp[j] + tmp[i];
			for(i = 0; i < n; i ++)
				tmp[i] = tmp[i] * x[i];
		}
	}
};
uint32 Polynominal::x[MAXN];
uint32 Polynominal::xp[MAXP];
int Polynominal::n;
namespace Calc{
	char buf[MAXN<<2];
	int pos, len;
	void Prepare(){
		char ch;
		pos = len = 0;
		do{
			do{
				ch = getchar();
			}while(ch == ' ' || ch == '\n' || ch == '\t');
			buf[len ++] = ch;
		}while(ch != EOF);
		len --;
		buf[len] = 0;
	}
	inline char Token(){
		return buf[pos ++];
	}
	Polynominal Expression();
	Polynominal Term(){
		char ch = Token();
		Polynominal ret;
		switch(ch){
		case '+':
			//+/
			assert(Token() == '/');
			return Expression().fold();
			break;
		case '*':
			//*:
			assert(Token() == ':');
			return Expression().sqr();
			break;
		case '-':
			return Expression().minus();
			break;
		case 'X':
			return Polynominal::X();
			break;
		case 'N':
			return Polynominal::N();
			break;
		case '(':
			ret = Expression();
			assert(Token() == ')');
			return ret;
			break;
		default:
			//数字
			assert(isdigit(ch));
			int val = 0;
			do{
				val = val * 10 + ch - '0';
				ch = Token();
			}while(isdigit(ch));
			pos --;
			return Polynominal(val);
			break;
		};
	}
	Polynominal Expression(){
		Polynominal ret = Term();
		switch(Token()){
		case '+':
			return ret + Expression();
			break;
		case '-':
			return ret - Expression();
			break;
		case '*':
			return ret * Expression();
			break;
		default:
			pos --;
			return ret;
			break;
		}
	}
}

int main(){
	freopen("j.in", "r", stdin);
	freopen("j.out", "w", stdout);
	Polynominal::Prepare();
	Calc::Prepare();
	Polynominal ret = Calc::Expression();
	printf("%lu\n", ret.v[0]);
	return 0;
}