#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <list>
using namespace std;

#define MAXB 400
#define MAXN 400

struct DATA
{
	int num[MAXB];
	int got[MAXB];
	int cnt[MAXB];
}dta[MAXN];
int N, M;

bool tag[MAXB];
void change(int nb, int pos, int num)
{
	int i, j;

	dta[nb].num[pos] = num;
	j = pos + num;
	if(pos + num >= MAXB || nb * MAXB + pos + num >= N)
	{
		dta[nb].cnt[pos] = 1;
		dta[nb].got[pos] = nb * MAXB + pos + num;
	}else{
		dta[nb].cnt[pos] = dta[nb].cnt[j] + 1;
		dta[nb].got[pos] = dta[nb].got[j];
	}
	memset(tag, 0, sizeof(tag));
	tag[pos] = true;
	for(i=pos-1;i>=0;i--)
	{
		if(!tag[dta[nb].num[i] + i])
			continue;
		
		dta[nb].cnt[i] = dta[nb].cnt[dta[nb].num[i] + i] + 1;
		dta[nb].got[i] = dta[nb].got[dta[nb].num[i] + i];
		tag[i] = true;
	}
}

void init()
{
	int i, j;
	int nb, pos;

	cin >> N >> M;
	nb = pos = 0;
	for(i=0;i<N;i++,pos++)
	{
		if(pos==MAXB)
		{
			nb++;
			pos = 0;
		}

		cin >> dta[nb].num[pos];
	}
	for(;nb>=0;nb--,pos=MAXB)
	{
		for(pos--;pos>=0;pos--)
		{
			j = pos + dta[nb].num[pos];
			if(j >= MAXB || nb * MAXB + j >= N)
			{
				dta[nb].got[pos] = nb * MAXB + j;
				dta[nb].cnt[pos] = 1;
			}else{
				dta[nb].cnt[pos] = dta[nb].cnt[j] + 1;
				dta[nb].got[pos] = dta[nb].got[j];
			}
		}
	}
}

void query(int nb, int pos)
{
	int cnt = 0;
	while(dta[nb].got[pos] < N)
	{
		cnt += dta[nb].cnt[pos];
		pos = dta[nb].got[pos];
		nb = pos / MAXB;
		pos = pos % MAXB;
	}
	while(nb * MAXB + pos + dta[nb].num[pos] < N)
	{
		pos += dta[nb].num[pos];
		cnt++;
	}
	printf("%d %d\n", nb * MAXB + pos + 1, cnt + 1);
}

int main()
{
	int op, a, b;
	init();
	while(M--)
	{
		cin >> op;
		if(op == 0)
		{
			cin >> a >> b;
			a--;
			change(a / MAXB, a % MAXB, b);
		}else{
			cin >> a;
			a--;
			query(a / MAXB, a % MAXB);
		}
	}
	return 0;
}
