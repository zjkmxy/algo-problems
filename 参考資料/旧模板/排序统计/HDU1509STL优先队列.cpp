//HDU1509STL优先队列
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

struct MSG
{
	char name[100];
	int par;
	int pri;
	int index;
};
priority_queue<MSG> que;

bool operator<(const MSG &x,const MSG &y)
{
	if(x.pri != y.pri)
		return (x.pri > y.pri);
	else
		return (x.index > y.index);
}

int main()  
{  
	int i;
	char op[5];
	MSG msg;

	i = 0;
	while(scanf("%s",op)!=EOF)
	{
		if(op[0]=='G')
		{
			if(que.empty())
				printf("EMPTY QUEUE!\n");
			else
			{
				msg = que.top();
				que.pop();
				cout << msg.name << ' ' << msg.par << endl;
			}
		}
		else
		{
			cin >> msg.name >> msg.par >> msg.pri;
			msg.index = i;
			i++;
			que.push(msg);
		}
	}
	return 0;
}