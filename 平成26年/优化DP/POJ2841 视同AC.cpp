#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<cmath>
#include<queue>
using namespace std;

#define MAXN 2005
#define MAXI 0x3fffffffffffffff
typedef long long int LLI;

const int dirs[3][2] = {{0, -1}, {-1, 0}, {1, 0}};

char map[MAXN][MAXN];
LLI DP[MAXN][MAXN][2]; //到达[i][j][f]状态至少需要多少步
LLI ori[MAXN][2];  //原初的计算
LLI SB[MAXN], SS[MAXN], NF[MAXN], NB[MAXN], NS[MAXN];
int que[MAXN], back, front;
int N, M;

void input()
{
	int i, j;

	cin >> N >> M;
	for(i=1;i<=N;i++)
	{
		for(j=1;j<=M;j++)
		{
			cin >> map[i][j];
		}
	}
}

inline LLI gety1(int x, int f)
{
	return ori[x][f^NF[x]] + (x * x - x) / 2 + SB[x] - x * NB[x] - SS[x] + x * NS[x];
}

inline LLI gety2(int x, int f)
{
	return ori[x][f^NF[x]] + (x * x + x) / 2 - SB[x] + x * NB[x] + SS[x] - x * NS[x];
}

inline LLI getx1(int x)
{
	return x;
}

inline LLI getk1(int j)
{
	return j - NB[j] + NS[j];
}

inline LLI getk2(int j)
{
	return j + NB[j] - NS[j];
}

LLI dp_calc()
{
	int i, j, lf, rf;  //注意循环的f变量和实际更新的f变量不是一个
	int x, y, z;
	LLI ans;

	for(j=0;j<=M+1;j++)
	{
		DP[N][j][1] = MAXI;
		DP[N][j][0] = (map[N][j]=='H' ? 0 : MAXI);
	}
	for(i=N-1;i>1;i--)
	{
		//初始化
		for(j=0;j<=M+1;j++)
		{
			DP[i][j][1] = DP[i][j][0] = MAXI;
			ori[j][1] = ori[j][0] = MAXI;
		}

		//下方更新
		for(j=1;j<=M;j++)
		{
			for(lf=0;lf<=1;lf++)
			{
				if(map[i][j] == 'O')
					continue;
				if(map[i][j] == 'F')
					rf = lf ^ 1;
				else
					rf = lf;
				if(map[i][j] == 'B')
					x = 0;
				else if(map[i][j] == 'S')
					x = 2;
				else
					x = 1;
				DP[i][j][rf] = min(DP[i+1][j][lf] + x, MAXI);
				ori[j][rf] = DP[i][j][rf];
			}
		}

		//左右更新
		for(lf=0;lf<=1;lf++)
		{
			//顺向扫描
			NS[0] = NB[0] = NF[0] = SS[0] = SB[0] = 0;
			for(j=1;j<=M;j++)
			{
				NS[j] = NS[j-1] + (map[i][j]=='S' ? 1 : 0);
				NB[j] = NB[j-1] + (map[i][j]=='B' ? 1 : 0);
				NF[j] = NF[j-1] ^ (map[i][j]=='F' ? 1 : 0);
				SS[j] = SS[j-1] + (map[i][j]=='S' ? j : 0);
				SB[j] = SB[j-1] + (map[i][j]=='B' ? j : 0);
			}
			front = back = 0;
			que[0] = 0;
			rf = lf;
			for(j=1;j<=M;j++)
			{
				if(map[i][j] == 'F')
					rf ^= 1;
				if(map[i][j] == 'O')
				{
					front = back = 0;
					que[0] = 0;
				}

				while(front < back && (gety1(que[front], lf) - gety1(que[front+1], lf)) >= getk1(j) * (getx1(que[front]) - getx1(que[front+1])))
				{
					front++;
				}
				x = que[front];
				DP[i][j][rf] = min(DP[i][j][rf], ori[x][lf^NF[x]] + ((j-x)*(j-x)+(j-x))/2 - (SB[j]-SB[x]) + (NB[j]-NB[x])*x + (SS[j]-SS[x]) - (NS[j]-NS[x])*x);
				z = j;
				while(front < back)
				{
					x = que[back-1];
					y = que[back];
					if((gety1(x, lf) - gety1(y, lf)) * (getx1(y) - getx1(z)) >= (gety1(y, lf) - gety1(z, lf)) * (getx1(x) - getx1(y)))
						back--;
					else
						break;
				}
				back++;
				que[back] = z;
			}

			//反向扫描
			NS[M+1] = NB[M+1] = NF[M+1] = SS[M+1] = SB[M+1] = 0;
			for(j=M;j>=1;j--)
			{
				NS[j] = NS[j+1] + (map[i][j]=='S' ? 1 : 0);
				NB[j] = NB[j+1] + (map[i][j]=='B' ? 1 : 0);
				NF[j] = NF[j+1] ^ (map[i][j]=='F' ? 1 : 0);
				SS[j] = SS[j+1] + (map[i][j]=='S' ? j : 0);
				SB[j] = SB[j+1] + (map[i][j]=='B' ? j : 0);
			}
			front = back = 0;
			que[0] = M+1;
			rf = lf;
			for(j=M;j>=1;j--)
			{
				if(map[i][j] == 'F')
					rf ^= 1;
				if(map[i][j] == 'O')
				{
					front = back = 0;
					que[0] = M+1;
				}

				while(front < back && (gety2(que[front], lf) - gety2(que[front+1], lf)) >= getk2(j) * (getx1(que[front]) - getx1(que[front+1])))
				{
					front++;
				}
				x = que[front];
				DP[i][j][rf] = min(DP[i][j][rf], ori[x][lf^NF[x]] + ((x-j)*(x-j)+(x-j))/2 + (SB[j]-SB[x]) - (NB[j]-NB[x])*x - (SS[j]-SS[x]) + (NS[j]-NS[x])*x);
				z = j;
				while(front < back)
				{
					x = que[back-1];
					y = que[back];
					if((gety2(x, lf) - gety2(y, lf)) * (getx1(y) - getx1(z)) >= (gety2(y, lf) - gety2(z, lf)) * (getx1(x) - getx1(y)))
						back--;
					else
						break;
				}
				back++;
				que[back] = z;
			}
		}
	}

	//得出结果
	ans = MAXI;
	for(j=1;j<=M;j++)
	{
		if(map[1][j] != 'H')
			continue;
		ans = min(ans, DP[2][j][1] + 1);
	}

	return ans;
}

int main()
{
	LLI ans;

	input();
	ans = dp_calc();
	if(ans >= MAXI || N <= 2)
	{
		cout << "No solution" << endl;
	}else{
		cout << ans << endl;
	}
	return 0;
}
/////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;
 
// For Ideone
typedef long long __int64;
 
// Please replace all %lld with %I64d manmally.
 
char map[1010][1010];
int sum_s[1010], sum_b[1010], s[1010];
int sum_sr[1010], sum_br[1010], sr[1010];
int mask_slr[1010], mask_blr[1010], lr[1010];
int mask_srl[1010], mask_brl[1010], rl[1010];
 
int n, m;
__int64 dyna[2][1010][2], (*x)[2] = dyna[0], (*y)[2] = dyna[1];
void prepare (int r)
{
sum_s[0] = sum_b[0] = s[0] = 0;
mask_slr[0] = mask_blr[0] = lr[0] = 0;
for (int i = 1; i < m; i++)
{
sum_s[i] = sum_s[i - 1], sum_b[i] = sum_b[i - 1];
mask_slr[i] = mask_slr[i - 1], mask_blr[i] = mask_blr[i - 1];
if (map[r][i] == 'S') ++sum_s[i], mask_slr[i] += i;
else if (map[r][i] == 'B') ++sum_b[i], mask_blr[i] += i;
s[i] = sum_s[i] - sum_b[i];
lr[i] = mask_slr[i] - mask_blr[i];
}
sum_sr[m - 1] = sum_br[m - 1] = sr[0] = 0;
mask_srl[m - 1] = mask_brl[m - 1] = rl[m - 1] = 0;
for (int i = m - 2; i >= 0; i--)
{
sum_sr[i] = sum_sr[i + 1], sum_br[i] = sum_br[i + 1];
mask_srl[i] = mask_srl[i + 1], mask_brl[i] = mask_brl[i + 1];
if (map[r][i] == 'S') ++sum_sr[i], mask_srl[i] += m - 1 - i;
else if (map[r][i] == 'B') ++sum_br[i], mask_brl[i] += m - 1 - i;
sr[i] = sum_sr[i] - sum_br[i];
rl[i] = mask_srl[i] - mask_brl[i];
}
}
int head[2], tail[2], even, odd;
struct rec
{
int pl, odd;
rec (void) {}
rec (int p0, int odd0) : pl(p0), odd(odd0) {}
} queue[2][1010];
void qclear (void)
{
head[0] = head[1] = tail[0] = tail[1] = 0;
even = 0, odd = 1;
}
inline int ylr (int k) { return k * k - k - 2 * lr[k] + 2 * k * s[k]; }
inline int yrl (int k) { return k * k + k - 2 * rl[k] + 2 * (m - 1 - k) * sr[k]; }
const int mi = 1519931117;
int main ()
{
scanf("%d %d", &n, &m);
if (n <= 2)
{
printf("No solution\n");
return 0;
}
for (int i = 1; i <= n; i++) scanf("%s", map[n - i]);
for (int j = 0; j < m; j++)
{
if (map[0][j] == 'H')
{
if (map[1][j] == 'S') y[j][0] = 2;
else if (map[1][j] == 'B') y[j][0] = 0;
else y[j][0] = 1;
} else y[j][0] = mi;
y[j][1] = mi;
}
for (int i = 2; i < n; i++)
{
prepare(i - 1);
// L --> R
qclear(); for (int j = 0; j < m; j++)
{
if (map[i - 1][j] == 'O') qclear();
else
{
if (y[j][0] < mi)
{
while (tail[even] - head[even] >= 2)
{
int tl = queue[even][tail[even] - 1].pl, tlo = queue[even][tail[even] - 1].odd;
int tlr = queue[even][tail[even] - 2].pl, tlro = queue[even][tail[even] - 2].odd;
if (((ylr(j) + y[j][0] * 2) - (ylr(tl) + y[tl][tlo] * 2)) * (tl - tlr) <=
((ylr(tl) + y[tl][tlo] * 2) - (ylr(tlr) + y[tlr][tlro] * 2)) * (j - tl)) --tail[even];
else break;
} queue[even][tail[even]++] = rec(j, 0);
}
if (y[j][1] < mi)
{
while (tail[odd] - head[odd] >= 2)
{
int tl = queue[odd][tail[odd] - 1].pl, tlo = queue[odd][tail[odd] - 1].odd;
int tlr = queue[odd][tail[odd] - 2].pl, tlro = queue[odd][tail[odd] - 2].odd;
if (((ylr(j) + y[j][1] * 2) - (ylr(tl) + y[tl][tlo] * 2)) * (tl - tlr) <=
((ylr(tl) + y[tl][tlo] * 2) - (ylr(tlr) + y[tlr][tlro] * 2)) * (j - tl)) --tail[odd];
else break;
} queue[odd][tail[odd]++] = rec(j, 1);
}
int tt = j + s[j];
while (tail[even] - head[even] >= 2)
{
int hd = queue[even][head[even]].pl, hdo = queue[even][head[even]].odd;
int hdr = queue[even][head[even] + 1].pl, hdro = queue[even][head[even] + 1].odd;
if ((ylr(hdr) + y[hdr][hdro] * 2) - (ylr(hd) + y[hd][hdo] * 2) <= tt * 2 * (hdr - hd)) ++head[even];
else break;
}
while (tail[odd] - head[odd] >= 2)
{
int hd = queue[odd][head[odd]].pl, hdo = queue[odd][head[odd]].odd;
int hdr = queue[odd][head[odd] + 1].pl, hdro = queue[odd][head[odd] + 1].odd;
if ((ylr(hdr) + y[hdr][hdro] * 2) - (ylr(hd) + y[hd][hdo] * 2) <= tt * 2 * (hdr - hd)) ++head[odd];
else break;
}
if (map[i - 1][j] == 'F') swap(odd, even);
}
if (tail[even] > head[even])
{
int hd = queue[even][head[even]].pl, hdo = queue[even][head[even]].odd;
x[j][0] = y[hd][hdo] +
(j - hd) * (j - hd + 1) / 2 +
lr[j] - lr[hd] -
hd * (s[j] - s[hd]);
} else x[j][0] = mi;
if (tail[odd] > head[odd])
{
int hd = queue[odd][head[odd]].pl, hdo = queue[odd][head[odd]].odd;
x[j][1] = y[hd][hdo] +
(j - hd) * (j - hd + 1) / 2 +
lr[j] - lr[hd] -
hd * (s[j] - s[hd]);
} else x[j][1] = mi;
}
// R --> L
qclear(); for (int j = m - 1; j >= 0; j--)
{
if (map[i - 1][j] == 'O') qclear();
else
{
if (y[j][0] < mi)
{
while (tail[even] - head[even] >= 2)
{
int tl = queue[even][tail[even] - 1].pl, tlo = queue[even][tail[even] - 1].odd;
int tlr = queue[even][tail[even] - 2].pl, tlro = queue[even][tail[even] - 2].odd;
if (((yrl(tl) + y[tl][tlo] * 2) - (yrl(j) + y[j][0] * 2)) * (tlr - tl) >=
((yrl(tlr) + y[tlr][tlro] * 2) - (yrl(tl) + y[tl][tlo] * 2)) * (tl - j)) --tail[even];
else break;
} queue[even][tail[even]++] = rec(j, 0);
}
if (y[j][1] < mi)
{
while (tail[odd] - head[odd] >= 2)
{
int tl = queue[odd][tail[odd] - 1].pl, tlo = queue[odd][tail[odd] - 1].odd;
int tlr = queue[odd][tail[odd] - 2].pl, tlro = queue[odd][tail[odd] - 2].odd;
if (((yrl(tl) + y[tl][tlo] * 2) - (yrl(j) + y[j][1] * 2)) * (tlr - tl) >=
((yrl(tlr) + y[tlr][tlro] * 2) - (yrl(tl) + y[tl][tlo] * 2)) * (tl - j)) --tail[odd];
else break;
} queue[odd][tail[odd]++] = rec(j, 1);
}
int tt = j - sr[j];
while (tail[even] - head[even] >= 2)
{
int hd = queue[even][head[even]].pl, hdo = queue[even][head[even]].odd;
int hdr = queue[even][head[even] + 1].pl, hdro = queue[even][head[even] + 1].odd;
if ((yrl(hd) + y[hd][hdo] * 2) - (yrl(hdr) + y[hdr][hdro] * 2) >= tt * 2 * (hd - hdr)) ++head[even];
else break;
}
while (tail[odd] - head[odd] >= 2)
{
int hd = queue[odd][head[odd]].pl, hdo = queue[odd][head[odd]].odd;
int hdr = queue[odd][head[odd] + 1].pl, hdro = queue[odd][head[odd] + 1].odd;
if ((yrl(hd) + y[hd][hdo] * 2) - (yrl(hdr) + y[hdr][hdro] * 2) >= tt * 2 * (hd - hdr)) ++head[odd];
else break;
}
if (map[i - 1][j] == 'F') swap(odd, even);
}
if (tail[even] > head[even])
{
int hd = queue[even][head[even]].pl, hdo = queue[even][head[even]].odd;
x[j][0] = min(x[j][0], y[hd][hdo] +
(hd - j) * (hd - j + 1) / 2 +
rl[j] - rl[hd] -
(m - 1 - hd) * (sr[j] - sr[hd]));
}
if (tail[odd] > head[odd])
{
int hd = queue[odd][head[odd]].pl, hdo = queue[odd][head[odd]].odd;
x[j][1] = min(x[j][1], y[hd][hdo] +
(hd - j) * (hd - j + 1) / 2 +
rl[j] - rl[hd] -
(m - 1 - hd) * (sr[j] - sr[hd]));
}
}
// Global
for (int j = 0; j < m; j++)
{
if (map[i][j] == 'S') x[j][1] += 2, x[j][0] += 2;
else if (map[i][j] != 'B') x[j][1] += 1, x[j][0] += 1;
}
// Finally...
swap(x, y);
}
__int64 ans = mi;
for (int j = 0; j < m; j++) if (map[n - 1][j] == 'H') ans = min(ans, y[j][1]);
if (ans < mi) printf("%lld\n", ans);
else printf("No solution\n");
return 0;
}
