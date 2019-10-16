/*
不平等博弈
用surreal数巧妙地得以解决。
*/
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

#define MAXN 100
bool arr[MAXN];

void read(int n)
{
	int i;
	char ch;

	for(i=0;i<n;i++)
	{
		//scanf("%c%*c",&ch);  //因为空格个数多于一个，用scanf读入会出问题。
		cin >> ch;
		if(ch == 'W')
			arr[i] = true;
		else if(ch == 'B')
			arr[i] = false;
		else
			throw 0;
	}
}

double getrn(int n)
{
	int i;
	double a = 1073741824, ret = 0;  //这里是防止精度误差的。

	for(i=0;i<n&&arr[i]==arr[0];i++)
	{
		if(arr[i])
			ret += a;
		else
			ret -= a;
	}
	for(a/=2.0;i<n;i++,a/=2.0)
	{
		if(arr[i])
			ret += a;
		else
			ret -= a;
	}
	return ret;
}

void domain()
{
	int n1, n2, n3;
	double a1 = 0.0, a2 = 0.0;

	scanf("%*s%*d%*c");
	scanf("%d%d%d%*c",&n1,&n2,&n3);
	read(n1);
	a1 += getrn(n1);
	read(n2);
	a1 += getrn(n2);
	read(n3);
	a1 += getrn(n3);

	scanf("%d%d%d%*c",&n1,&n2,&n3);
	read(n1);
	a2 += getrn(n1);
	read(n2);
	a2 += getrn(n2);
	read(n3);
	a2 += getrn(n3);

	if(a1 >= a2)
		printf("Yes\n");
	else
		printf("No\n");
}

int main()
{
	int i, t;
	scanf("%d",&t);
	try{
	for(i=1;i<=t;i++)
	{
		printf("Test %d: ",i);
		domain();
	}
	}catch(int)
	{
	}
	return 0;
}
