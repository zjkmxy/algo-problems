/*
HDU2481 Toy

�ǳ��ѵ�һ���⣬��Burnsideԭ��+���ظ�������
getd�����ݼ����ġ�����ͼ��������������������W[n]=4W[n-1]-4W[n-2]+W[n-3]
��Ϊm��ȷ����������ǰ�˺�n�����ֱ�ӳ�n���ɡ�
ans / n % m = (ans % (m * n) / n) % m
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

typedef signed long long int SINT;
typedef SINT MATRIX[3][3];
typedef SINT VECTOR[3];

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n����ȡ����
long fcnt[100];       //n�����������ݴ�
long faci[100];       //n������
long fval[100];       //n�����ӻ�
long nfac;            //n�����Ӹ���
SINT ans;             //���մ�

int n, m;
SINT MODN;
MATRIX mat[32];
//const VECTOR W0 = {0, 1, 5};  //W0,W1,W2

//�˷�ȡģ����Խλ��
SINT mult_mod(SINT a, SINT b)
{
	SINT ret = 0, tmp = a;

	if(b<0)
		b += MODN;
	if(tmp<0)
		tmp += MODN;
	while(b)
	{
		if(b&0x1)
		{
			ret += tmp;
			if(ret >= MODN)
				ret -= MODN;
		}
		tmp <<= 1;
		if(tmp >= MODN)
			tmp -= MODN;
		b>>=1;
	}
	return ret;
}

//������
void mul(MATRIX mata, MATRIX matb, MATRIX prod)
{
	int i, j, k;
	SINT sum;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			sum = 0;
			for(k=0;k<3;k++)
			{
				sum += mult_mod(mata[i][k], matb[k][j]);
			}
			prod[i][j] = (sum % MODN);
		}
	}
}
void matpow_init()
{
	int i;
	for(i=1;i<32;i++)
	{
		mul(mat[i-1], mat[i-1], mat[i]);
	}
}
void mul_vec(VECTOR a, MATRIX b, VECTOR prod)
{
	int i, j;
	SINT sum;
	for(i=0;i<3;i++)
	{
		sum = 0;
		for(j=0;j<3;j++)
		{
			sum += mult_mod(a[j], b[j][i]);
		}
		prod[i] = sum % MODN;
	}
}


//ɸ��Ԥ��������
void init_prime()
{
	int i, j = 0, k = 0;
	for(i=2;k<PRIMENUM;i++)
	{
		if(notprime[i])
			continue;

		for(j=i+i; j < 32000; j+=i)
		{
			notprime[j] = true;
		}
		primes[k] = i;
		k++;
	}
}

//���ӷֽ�
void fact(int x)
{
	int i;

	nfac = 0;
	for(i=0;(i<PRIMENUM)&&(x>1);i++)
	{
		if(x % primes[i] == 0)
		{
			faci[nfac] = primes[i];
			fcnt[nfac] = 0;
			fval[nfac] = 1;
			do{
				x /= primes[i];
				fval[nfac] *= primes[i];
				fcnt[nfac]++;
			}while(x % primes[i] == 0);
			nfac++;
		}
	}
	if(x > 1)  //xʣ������Ϊ�������������������
	{
		faci[nfac] = x;
		fcnt[nfac] = 1;
		fval[nfac] = x;
		nfac++;
	}
}

SINT getd(int cnt)  
{
	VECTOR temp[2];
	int i, cur = 0;
	SINT ret = 0;

	memset(temp, 0, sizeof(temp));
	temp[cur][0] = 0;
	temp[cur][1] = 1;
	temp[cur][2] = 5;
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul_vec(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
	}
	ret = temp[cur][0];

	return ret;
}

void DFS(int pfi, int fc, int phi)  //��������ţ���ǰ���ӣ�n/i��ŷ������ֵ
{
	int i, p;

	if(pfi < nfac)
	{
		for(i=0,p=fval[pfi];i<=fcnt[pfi];i++,p/=faci[pfi])
		{
			DFS(pfi + 1, fc * (fval[pfi] / p), phi * (p - p / faci[pfi]));
		}
	}else{
		ans += mult_mod(getd(fc), phi);
		ans %= MODN;
	}
}

int main()
{
	init_prime();
	while(scanf("%d%d",&n, &m)!=EOF)
	{
		MODN = (SINT)n * (SINT)m;  //Ԥ�ȳ˺��ˣ���ʱ��ֱ�ӳ�

		memset(mat[0], 0, sizeof(mat[0]));
		mat[0][0][2] = 1;
		mat[0][1][0] = 1;
		mat[0][1][2] = -4;
		mat[0][2][1] = 1;
		mat[0][2][2] = 4;
		matpow_init();

		ans = 0;
		fact(n);
		DFS(0, 1, 1);
		ans = (ans / n % m + m) % m;
		printf("%lld\n",ans);
	}
	return 0;
}
