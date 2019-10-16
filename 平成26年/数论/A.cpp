/*
HDU3306  Another kind of Fibonacci

A(0) = 1 , A(1) = 1 , A(N) = X * A(N - 1) + Y * A(N - 2) (N >= 2).
我们希望计算 S(N) , S(N) = A(0)^2 +A(1)^2+……+A(n)^2.

解法：矩阵快速幂
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

#define MODN 10007

typedef signed long int SINT;
typedef SINT MATRIX[4][4];
typedef SINT VECTOR[4];  //S_n,A_n^2,A_n*A_{n-1},A_{n-1}^2 这个是关键，自己推

MATRIX mat[33];
int n, x, y;

//矩阵处理
void mul(MATRIX mata, MATRIX matb, MATRIX prod)
{
	int i, j, k;
	SINT sum;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			sum = 0;
			for(k=0;k<4;k++)
			{
				sum += (mata[i][k] * matb[k][j]) % MODN;
			}
			prod[i][j] = (sum % MODN);
		}
	}
}

void mul_vec(VECTOR a, MATRIX b, VECTOR prod)
{
	int i, j;
	SINT sum;
	for(i=0;i<4;i++)
	{
		sum = 0;
		for(j=0;j<4;j++)
		{
			sum += (a[j] * b[j][i]) % MODN;
		}
		prod[i] = sum % MODN;
	}
}

SINT calc(int cnt)  
{
	VECTOR temp[2];
	int i, cur = 0;
	SINT ret = 0;

	memset(temp, 0, sizeof(temp));
	temp[cur][0] = 1;
	temp[cur][1] = 1;
	temp[cur][2] = 1;
	temp[cur][3] = 1;
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul_vec(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
		mul(mat[i], mat[i], mat[i+1]);
	}
	ret = temp[cur][0];

	return ret;
}

int main()
{
	while(scanf("%d%d%d",&n,&x,&y)!=EOF)
	{
		x %= MODN;
		y %= MODN;
		memset(mat[0], 0, sizeof(mat[0]));
		mat[0][0][0] = 1;
		mat[0][1][0] = 1;
		mat[0][1][1] = (x * x) % MODN;
		mat[0][2][1] = (2 * x * y) % MODN;
		mat[0][3][1] = (y * y) % MODN;
		mat[0][1][2] = x;
		mat[0][2][2] = y;
		mat[0][1][3] = 1;
		printf("%d\n",calc(n));
	}
	return 0;
}
