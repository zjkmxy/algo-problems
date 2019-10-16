/*
代码略过，提示思路。
设F(a,b)表示有a条链和b个独立点时的成环期望数。那么F(a,b)加上原来的环个数就是答案。
F(a,0)的链是可以绕到自己的每个链有1/a的概率绕到自己成环，其他情况下合并到别的链，所以答案是1+F(a-1,0)
而F(a,b)计算的时候，因为b个点不能绕道自己成为环，所以比F(a,b)小。用逐步淘汰原理，考虑有至少i个独点：
出现情况C(b, i)
出现概率1 / (a + b) * 1 / (a + b - 1)  + ... + 1 / (a + b - i + 1)
情况下的环期望F(a + b - i, 0)
全部乘起来然后循环i容斥即可。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
#include<cassert>
#include<unordered_map>
#include<iomanip>
using namespace std;

const int MAXN = 4000+10;

bool mark[MAXN];
int cost[MAXN];
int to[MAXN],from[MAXN];

double calc(int a,int b)
{
	if (b==0)
	{
		double res=0;
		for (int i=1;i<=a;i++)
			res+=1./i;
		return res;
	}
	double res=calc(a+b,0),p=1;
	double pn=1;
	for (int i=1;i<=b;i++)
	{
		pn*=(double(b-i+1)/(i));
		pn*=1./(a+b-i+1);
		double now=calc(a+b-i,0)+i;
		double sgn=(i%2==1?-1:1);
		p+=sgn*pn;
		res+=now*sgn*pn;
	}
	return res/p;
}
int main()
{
	int tt; cin >> tt;
	for (int o=1; o<=tt; o++){
		memset(to, -1, sizeof to);
		memset(from, -1, sizeof from);
		memset(mark, 0, sizeof mark);
		int n,cost; cin >> n >> cost;
		for (int i=0; i<n; i++){
			cin >> to[i]; to[i]--;
			if (to[i] != -1)
				from[to[i]] = i;
		}
		int a = 0;
		for (int i=0; i<n; i++) if (to[i]!=-1 && from[i]==-1){
			int cur = i;
			while (cur != -1){
				mark[cur] = true;
				cur = to[cur];
			}
			a++;
		}
		int res = 0;
		for (int i=0; i<n; i++) if (!mark[i] && to[i]!=-1 && from[i]!=-1){
			int cur = i;
			while (!mark[cur]){
				mark[cur] = true;
				cur = to[cur];
			}
			res++;
		}
		int b = 0;
		for (int i=0; i<n; i++) if (!mark[i]) b++;
		cout<<fixed<<setprecision(10)<< cost * (res + calc(a,b)) << endl;
	}
	return 0;
}