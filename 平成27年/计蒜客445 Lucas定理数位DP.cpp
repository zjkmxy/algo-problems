//http://blog.jisuanke.com/?p=146  Ã‚Ω‚
#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXP = 12;
const int MODN = 47;
int A[3][MAXP+3], L[3][MAXP+3], R[3][MAXP+3], N[MAXP+3];
LL DP[3][MAXP+3][1<<6][MODN*3+1];
LL comb[50][50];

void input(int i){
	int j;
	LL a, l, r;
	scanf("%lld%lld%lld", &a, &l, &r);
	for(j = 1; j <= MAXP; j ++){
		A[i][j] = a % MODN;
		a /= MODN;
		L[i][j] = l % MODN;
		l /= MODN;
		R[i][j] = r % MODN;
		r /= MODN;
	}
}

void pre(){
	int i, j;
	comb[0][0] = 1;
	for(i = 1; i < 50; i ++){
		comb[i][0] = 1;
		for(j = 1; j <= i; j ++){
			comb[i][j] = (comb[i-1][j] + comb[i-1][j-1]) % MODN;
		}
	}
}

void dptrans(int j, int pos, int mask, int res, int val){
	LL cur = DP[j][pos][mask][res];
	LL *next;
	mask = mask | ((val > L[j][pos] ? 1 : 0) << (j << 1));
	mask = mask | ((val < R[j][pos] ? 1 : 0) << (j << 1 | 1));
	res -= val;
	if(j == 2){
		res = res * MODN + N[pos - 1];
		res = min(res, MODN * 3);
		next = &DP[0][pos-1][mask][res];
	}else{
		next = &DP[j+1][pos][mask][res];
	}
	(*next) = ((*next) + cur * comb[A[j][pos]][val]) % MODN;
}

void dpcalc(){
	int pos, j, mask, res, val, st, ed;
	LL ans = 0;

	memset(DP, 0, sizeof(DP));
	DP[2][MAXP+1][0][0] = 1;
	for(pos = MAXP+1; pos > 0; pos --){
		for(j = 0; j <= 2; j ++){
			for(mask = 0; mask < (1 << 6); mask ++){
				for(res = 0; res <= MODN * 3; res ++){
					if(DP[j][pos][mask][res] == 0)
						continue;
					st = (mask & (1 << (j << 1))) ? 0 : L[j][pos];
					ed = (mask & (1 << (j << 1 | 1))) ? MODN - 1 : R[j][pos];
					ed = min(ed, res);
					for(val = st; val <= ed; val ++)
						dptrans(j, pos, mask, res, val);
				}
			}
		}
	}
	for(mask = 0; mask < (1 << 6); mask ++){
		for(res = 0; res <= MODN * 3; res ++){
			ans = (ans + DP[0][0][mask][res]) % MODN;
		}
	}
	printf("%ld\n", ans);
}

void domain(){
	LL n;
	int j;

	input(0);
	input(1);
	input(2);
	scanf("%lld", &n);
	for(j = 1; j <= MAXP; j ++){
		N[j] = n % MODN;
		n /= MODN;
	}
	dpcalc();
}

int main(){
	int T;

	pre();
	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}
