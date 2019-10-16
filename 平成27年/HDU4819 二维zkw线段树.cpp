#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAXN = 1024;
int N, Q;
int mat[MAXN][MAXN];

struct D{
	int mx, mn;
};
class Segtree{
public:
	D node[MAXN<<1][MAXN<<1];
	int n;

	void Build(int N){
		int i, j, tmp = N;

		for(n = 1, N ++; N > 0; N >>= 1, n <<= 1);
		N = tmp;

		for(i = (n << 1) - 1; i > 0; i --){
			for(j = (n << 1) - 1; j > 0; j --){
				node[i][j].mx = 0;
				node[i][j].mn = 0x3fffffff;
			}
		}

		for(i = N + n; i > n; i --){
			for(j = N + n; j > n; j --)
				node[i][j].mn = node[i][j].mx = mat[i-n][j-n];
			for(; j > 0; j --){
				node[i][j].mn = min(node[i][j<<1].mn, node[i][j<<1|1].mn);
				node[i][j].mx = max(node[i][j<<1].mx, node[i][j<<1|1].mx);
			}
		}

		for(i = n - 1; i > 0; i --){
			for(j = (n << 1) - 1; j > 0; j --){
				node[i][j].mn = min(node[i<<1][j].mn, node[i<<1|1][j].mn);
				node[i][j].mx = max(node[i<<1][j].mx, node[i<<1|1][j].mx);
			}
		}
	}

	void Update(const int x, const int y, const int v){
		int i = x + n, j = y + n;

		node[i][j].mx = node[i][j].mn = v;
		//外层的叶子
		for(j >>= 1; j > 0; j >>= 1){
			node[i][j].mn = min(node[i][j<<1].mn, node[i][j<<1|1].mn);
			node[i][j].mx = max(node[i][j<<1].mx, node[i][j<<1|1].mx);
		}
		//外层的中间
		for(i >>= 1; i > 0; i >>= 1){
			for(j = y + n; j > 0; j >>= 1){
				node[i][j].mn = min(node[i<<1][j].mn, node[i<<1|1][j].mn);
				node[i][j].mx = max(node[i<<1][j].mx, node[i<<1|1][j].mx);
			}
		}
	}

	void Qsub(int i, int l, int r, D& q){
		for(l += n - 1, r += n + 1; l ^ r ^ 1; l >>= 1, r >>= 1){
			if(~l & 1){
				q.mn = min(q.mn, node[i][l^1].mn);
				q.mx = max(q.mx, node[i][l^1].mx);
			}
			if(r & 1){
				q.mn = min(q.mn, node[i][r^1].mn);
				q.mx = max(q.mx, node[i][r^1].mx);
			}
		}
	}

	void Query(int xl, int xr, int yl, int yr, D& q){
		xl = max(xl, 1);
		yl = max(yl, 1);
		xr = min(xr, N);
		yr = min(yr, N);

		for(xl += n - 1, xr += n + 1; xl ^ xr ^ 1; xl >>= 1, xr >>= 1){
			if(~xl & 1){
				Qsub(xl ^ 1, yl, yr, q);
			}
			if(xr & 1){
				Qsub(xr ^ 1, yl, yr, q);
			}
		}
	}
}segt;

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
    while(ch=getc(), !isdigit(ch));
    int aa = ch - '0';
    while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
    return aa;
}

void domain(){
	int i, j, x, y, l, v;
	D ans;

	N = ReadInt();
	for(i = 1; i <= N; i ++){
		for(j = 1; j <= N; j ++){
			mat[i][j] = ReadInt();
		}
	}
	segt.Build(N);
	Q = ReadInt();
	while(Q --){
		x = ReadInt(), y = ReadInt(), l = ReadInt();
		l /= 2;
		ans.mn = 0x3fffffff;
		ans.mx = 0;
		segt.Query(x - l, x + l, y - l, y + l, ans);
		v = (ans.mx + ans.mn) / 2;
		segt.Update(x, y, v);
		printf("%d\n", v);
	}
}

int main(){
	int T, cas;

	T = ReadInt();
	for(cas = 1; cas <= T; cas ++){
		printf("Case #%d:\n", cas);
		domain();
	}
	return 0;
}