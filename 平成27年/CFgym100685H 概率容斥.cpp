/*
�����Թ�����ʾ˼·��
��F(a,b)��ʾ��a������b��������ʱ�ĳɻ�����������ôF(a,b)����ԭ���Ļ��������Ǵ𰸡�
F(a,0)�����ǿ����Ƶ��Լ���ÿ������1/a�ĸ����Ƶ��Լ��ɻ�����������ºϲ�������������Դ���1+F(a-1,0)
��F(a,b)�����ʱ����Ϊb���㲻���Ƶ��Լ���Ϊ�������Ա�F(a,b)С��������̭ԭ������������i�����㣺
�������C(b, i)
���ָ���1 / (a + b) * 1 / (a + b - 1)  + ... + 1 / (a + b - i + 1)
����µĻ�����F(a + b - i, 0)
ȫ��������Ȼ��ѭ��i�ݳ⼴�ɡ�
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