/*
HDU3404 Nim积游戏
高维的翻硬币问题，用Tartan定理解。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

const int mpow[2][2] = {{0, 0}, {0, 1}};

int Nim_Mul_Pow(int x, int y)  //计算x=2^a且y<=x时的积
{
	int ta, tm, p, s, t, d1, d2;

	if(x < 2)
		return mpow[x][y];
	for(ta=0,tm=1;(1<<tm)<=x;ta++,tm<<=1);
	tm>>=1;
	tm = (1<<tm);
	ta--;
	p = x / tm;
	s = y / tm;
	t = y % tm;
	d1 = Nim_Mul_Pow(p, s);
	d2 = Nim_Mul_Pow(p, t);

	return (tm*(d1^d2))^Nim_Mul_Pow(tm>>1, d1);
}

int Nim_Mul(int x, int y)
{
	int ta, tm, p, q, s, t, c1, c2, c3;

	if(x < y)
		swap(x, y);
	if(x < 2)
		return mpow[x][y];
	for(ta=0,tm=1;(1<<tm)<=x;ta++,tm<<=1);
	tm>>=1;
	tm = (1<<tm);
	ta--;
	p = x / tm;
	q = x % tm;
	s = y / tm;
	t = y % tm;
	c1 = Nim_Mul(p, s);
	c2 = Nim_Mul(p, t) ^ Nim_Mul(q, s);
	c3 = Nim_Mul(q, t);
	return (tm*(c1^c2))^c3^Nim_Mul_Pow(tm>>1, c1);
}

int main()
{
	int t, n, ret, x, y;

	scanf("%d",&t);
	while(t--)
	{
		ret = 0;
		scanf("%d",&n);
		while(n--)
		{
			scanf("%d%d",&x,&y);
			ret ^= Nim_Mul(x, y);
		}
		if(ret)
			printf("Have a try, lxhgww.\n");
		else
			printf("Don't waste your time.\n");
	}
	return 0;
}
