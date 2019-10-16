#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

vector<pair<ll, ll>> divs;

ll gcd(ll a, ll b){
	return b ? gcd(b, a % b) : a;
}

//(ax+b)/(cx+d)
struct DAT{
	ll a, b, c, d;


	static DAT op(const DAT& lhs, const DAT& rhs){
		DAT ret;
		int g;

		if(rhs.a == 0 && rhs.c == 0){
			ret.a = lhs.a * rhs.d + rhs.b * lhs.c;
			ret.b = lhs.b * rhs.d + rhs.b * lhs.d;
			ret.c = lhs.c * rhs.d;
			ret.d = lhs.d * rhs.d;
		} else if(lhs.a == 0 && lhs.c == 0){
			ret.a = rhs.a * lhs.d + lhs.b * rhs.c;
			ret.b = rhs.b * lhs.d + lhs.b * rhs.d;
			ret.c = rhs.c * lhs.d;
			ret.d = rhs.d * lhs.d;
		} else{
			ret.a = *(int*)NULL;
		}
		g = gcd(gcd(ret.a, ret.b), gcd(ret.c, ret.d));
		if(g != 0){
			ret.a /= g, ret.b /= g, ret.c /= g, ret.d /= g;
		}
		return ret;
	}

	DAT operator-() const{
		DAT ret = *this;
		ret.a = -a, ret.b = -b;
		return ret;
	}

	inline DAT operator+(const DAT& rhs) const{
		return op(*this, rhs);
	};
	inline DAT operator-(const DAT& rhs) const{
		return op(*this, - rhs);
	};
	DAT operator*(const DAT& rhs)const{
		DAT ret;
		ll g;
		ret.a = a * rhs.b + b * rhs.a;
		ret.b = b * rhs.b;
		ret.c = c * rhs.d + d * rhs.c;
		ret.d = d * rhs.d;
		g = gcd(gcd(ret.a, ret.b), gcd(ret.c, ret.d));
		if(g != 0){
			ret.a /= g, ret.b /= g, ret.c /= g, ret.d /= g;
		} else{
			g = *(int*)NULL;
		}
		return ret;
	};
	DAT operator/(const DAT& rhs)const{
		DAT ret;
		ll g;
		ret.a = a * rhs.d + b * rhs.c;
		ret.b = b * rhs.d;
		ret.c = c * rhs.b + d * rhs.a;
		ret.d = d * rhs.b;
		g = gcd(gcd(ret.a, ret.b), gcd(ret.c, ret.d));
		if(g != 0){
			ret.a /= g, ret.b /= g, ret.c /= g, ret.d /= g;

			//Record the divisor
			g = gcd(ret.c, ret.d);
			divs.push_back(make_pair(ret.c / g, ret.d / g));
		} else{
			g = *(int*)NULL;
		}
		return ret;
	};
};
DAT stk[100];
ll sp;

void Read(){
	char str[10];

	sp = 0;
	while(scanf("%s", str) != EOF){
		switch(str[0]){
		case '+':
			stk[sp - 2] = stk[sp - 2] + stk[sp - 1];
			sp --;
			break;
		case '-':
			stk[sp - 2] = stk[sp - 2] - stk[sp - 1];
			sp --;
			break;
		case '*':
			stk[sp - 2] = stk[sp - 2] * stk[sp - 1];
			sp --;
			break;
		case '/':
			stk[sp - 2] = stk[sp - 2] / stk[sp - 1];
			sp --;
			break;
		case 'X':
			stk[sp].a = 1;
			stk[sp].b = stk[sp].c = 0;
			stk[sp].d = 1;
			sp ++;
			break;
		default:
			sscanf(str, "%I64d", &stk[sp].b);
			stk[sp].a = stk[sp].c = 0;
			stk[sp].d = 1;
			sp ++;
			break;
		}
	}

	if(sp != 1){
		sp = *(int*)NULL;
		printf("%d\n", sp);
	}

}

void Solve(){
	DAT &res = stk[0];
	ll p, q, g;

	if(res.a == 0 && res.b == 0){
		puts("MULTIPLE");
	} else if(res.a == 0){
		puts("NONE");
	} else if(res.a * res.d == res.b * res.c){
		puts("NONE");
	} else{
		p = - res.b;
		q = res.a;
		g = gcd(p, q);
		p /= g, q /= g;
		if(q < 0){
			q = - q;
			p = - p;
		}

		bool flag = false;
		for(auto it : divs){
			if(p * it.first + q * it.second == 0){
				flag = true;
				break;
			}
		}

		if(flag){
			puts("NONE");
		} else{
			printf("X = %I64d/%I64d\n", p, q);
		}
	}
}

int main(){
#ifndef _DEBUG
	freopen("equation.in", "r", stdin);
	freopen("equation.out", "w", stdout);
#endif

	Read();
	Solve();
	return 0;
}