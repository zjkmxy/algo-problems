/*
HDU1809  A New Tetris Game(2)

1760的加强版
必须算出每一种情况的SG值再异或，不可以偷懒的。
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<map>
#include<algorithm>
using namespace std;

#define MAXN 55
int block[MAXN][MAXN];
int n, m, t;

map<string, int> sg_map;  //SG值只和当前地图有关，所以不用每次清空

string encode()
{
	int i, j;
	string ret = "";

	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			ret += (block[i][j] + '0');
		}
	}
	return ret;
}

int calc_sg()
{
	int i, j, ret, k;
	bool gto[100];   //能走到的sg
	map<string, int>::iterator it;

	memset(gto, 0, sizeof(gto));
	for(i=1;i<n;i++)
	{
		for(j=1;j<m;j++)
		{
			if(block[i-1][j-1] + block[i-1][j] + block[i][j-1] + block[i][j] > 0)
				continue;

			block[i-1][j-1] = block[i-1][j] = block[i][j-1] = block[i][j] = 1;
			it = sg_map.find(encode());
			if(it == sg_map.end())
			{
				k = calc_sg();
			}else{
				k = it->second;
			}
			block[i-1][j-1] = block[i-1][j] = block[i][j-1] = block[i][j] = 0;
			gto[k] = true;
		}
	}
	ret = -1;
	for(i=0;i<100;i++)
	{
		if(!gto[i])
		{
			ret = i;
			break;
		}
	}
	sg_map.insert(make_pair(encode(), ret));
	return ret;
}

int main()
{
	int i, j;
	int sum;

	while(scanf("%d",&t)!=EOF)
	{
		sum = 0;
		while(t--)
		{
			scanf("%d%d",&n,&m);
			for(i=0;i<n;i++)
			{
				for(j=0;j<m;j++)
				{
					scanf("%1d",&block[i][j]);
				}
			}
			sum ^= calc_sg();
		}
		if(sum)
			printf("Yes\n");
		else
			printf("No\n");
	}
	return 0;
}
