#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXC 105
#define MAXL 11
#define MAXN 10
#define ALB  4
#define INF  0x3fffffff

const int MODN = 100000;
typedef long long LLI;
typedef LLI matrix2[MAXC][MAXC];
typedef LLI vector2[MAXC];

matrix2 temp;
matrix2 ret;
vector2 tmp2;
void matmul(matrix2 A, matrix2 B, int N)
{
	int i, j, k;
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			temp[i][j] = 0;
			for(k=0;k<N;k++)
			{
				temp[i][j] += A[i][k] * B[k][j];
			}
			temp[i][j] %= MODN;
		}
	memcpy(A, temp, sizeof(matrix2));
}
void matpow(matrix2 A, int N, int k)
{
	int i, j;
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			ret[i][j] = (i == j);
		}
	while(k > 0)
	{
		if(k & 1)
			matmul(ret, A, N);
		matmul(A, A, N);
		k>>=1;
	}
	memcpy(A, ret, sizeof(matrix2));
}

class ACA
{
public:
	struct NODE
	{
		char ch;		//µ±Ç°×Ö·û
		int fail;		//Ê§°ÜÖ¸Õë
		int next[ALB];	//×ªÒÆÖ¸Õë
		int ed;			//ÖÕ½á×´Ì¬
		inline int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	int cnt;
	matrix2 dpmat;
	void init()
	{
		cnt = 1;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = 0;
	}
	void insert(char *str, int len)
	{
		int i, cur = 0;
		for(i=0;i<len;i++){
			if(node[cur][str[i]] == -1){
				node[cur][str[i]] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				node[cnt].ed = 0;
				cnt++;
			}
			cur = node[cur][str[i]];
		}
		node[cur].ed = true;
	}
	void build()
	{
		queue<int> Q;
		int cur, j, p;
		for(j=0;j<ALB;j++){
			if(node[0][j] >= 0){
				Q.push(node[0][j]);
				node[node[0][j]].fail = 0;
			}else
				node[0][j] = 0;
		}
		while(!Q.empty()){
			cur = Q.front();
			Q.pop();
			p = node[cur].fail;
			for(j=0;j<ALB;j++){
				if(node[cur][j] >= 0){
					node[node[cur][j]].fail = node[p][j];
					node[node[cur][j]].ed |= node[node[p][j]].ed;
					Q.push(node[cur][j]);
				}else
					node[cur][j] = node[p][j];
			}
		}
	}
	LLI dp_calc(int L)
	{
		int i, j;
		LLI ret = 0;
		memset(dpmat, 0, sizeof(dpmat));
		for(i=0;i<cnt;i++)
		{
			if(node[i].ed)
				continue;
			for(j=0;j<4;j++)
			{
				if(node[node[i][j]].ed)
					continue;
				dpmat[i][node[i][j]]++;
			}
		}
		matpow(dpmat, cnt, L);
		for(i=0;i<cnt;i++)
			ret = (ret + dpmat[0][i]) % MODN;
		return ret;
	}
}aca;

char str[MAXL];
int len;
void deal()
{
	int i;
	len = strlen(str);
	for(i=0;i<len;i++)
	{
		switch(str[i])
		{
		case 'A':
			str[i] = 0;
			break;
		case 'G':
			str[i] = 1;
			break;
		case 'C':
			str[i] = 2;
			break;
		case 'T':
			str[i] = 3;
			break;
		}
	}
}

int main()
{
	int M, N;
	scanf("%d%d",&M,&N);
	aca.init();
	while(M--)
	{
		scanf("%s",str);
		deal();
		aca.insert(str, len);
	}
	aca.build();
	N = aca.dp_calc(N);
	printf("%d\n",N);
	return 0;
}
