#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
using namespace std;

#define S_S 1
#define S_H 2
#define S_D 3
#define S_C 4

long double prob[14][14][14][14][5][5];   //ÅÆ¿â×´Ì¬
int as, ah, ad, ac;

void DFS(int S, int H, int D, int C, int J1, int J2)
{
	long double ans, tmp;
	int res, i;
	int tab[5];

	if(prob[S][H][D][C][J1][J2] > -1.0)
		return;

	ans = 0.0;
	res = S + H + D + C + (J1 == 0) + (J2 == 0);
	tab[1] = 13 - S;
	tab[2] = 13 - H;
	tab[3] = 13 - D;
	tab[4] = 13 - C;
	tab[J1]++;
	tab[J2]++;
	if(tab[1] >= as && tab[2] >= ah && tab[3] >= ad && tab[4] >= ac)
	{
		prob[S][H][D][C][J1][J2] = 0.0;
		return ;
	}else if(res == 0)
	{
		prob[S][H][D][C][J1][J2] = 100.0;
		return ;
	}

	if(S > 0)
	{
		DFS(S-1, H, D, C, J1, J2);
		ans += 1.0 * S / res * prob[S-1][H][D][C][J1][J2];
	}
	if(H > 0)
	{
		DFS(S, H-1, D, C, J1, J2);
		ans += 1.0 * H / res * prob[S][H-1][D][C][J1][J2];
	}
	if(D > 0)
	{
		DFS(S, H, D-1, C, J1, J2);
		ans += 1.0 * D / res * prob[S][H][D-1][C][J1][J2];
	}
	if(C > 0)
	{
		DFS(S, H, D, C-1, J1, J2);
		ans += 1.0 * C / res * prob[S][H][D][C-1][J1][J2];
	}
	if(J1 == 0)
	{
		tmp = 100;
		for(i=1;i<=4;i++)
		{
			DFS(S, H, D, C, i, J2);
			tmp = min(tmp, prob[S][H][D][C][i][J2]);
		}
		ans += 1.0 / res * tmp;
	}
	if(J2 == 0)
	{
		tmp = 100;
		for(i=1;i<=4;i++)
		{
			DFS(S, H, D, C, J1, i);
			tmp = min(tmp, prob[S][H][D][C][J1][i]);
		}
		ans += 1.0 / res * tmp;
	}
	
	prob[S][H][D][C][J1][J2] = ans + 1.0;
}

void init()
{
	int i, j, k, l, m, n;

	for(i=0;i<14;i++)
	{
		for(j=0;j<14;j++)
		{
			for(k=0;k<14;k++)
			{
				for(l=0;l<14;l++)
				{
					for(m=0;m<5;m++)
					{
						for(n=0;n<5;n++)
						{
							prob[i][j][k][l][m][n] = -100.0;
						}
					}
				}
			}
		}
	}
}

int main()
{
	int t, i, x;

	scanf("%d",&t);
	for(i=1;i<=t;i++)
	{
		printf("Case %d: ", i);
		init();
		scanf("%d%d%d%d",&ac,&ad,&ah,&as);
		x = 0;
		if(ac > 13)
			x += ac - 13;
		if(ad > 13)
			x += ad - 13;
		if(ah > 13)
			x += ah - 13;
		if(as > 13)
			x += as - 13;
		if(x > 2)
		{
			printf("-1\n");
			continue;
		}else{
			DFS(13, 13, 13, 13, 0, 0);
			printf("%.7Lf\n", prob[13][13][13][13][0][0]);
		}
	}
	return 0;
}
