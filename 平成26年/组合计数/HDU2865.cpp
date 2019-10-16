/*
HDU2865 Birthday Toy

�м��Ԫ�ؿ���Ⱦk����ɫ�����Ǳ��ϵľ�ֻ����k-1������Ⱦ�ˡ�
�������a����ʱȾɫ��������f[a]�������׵õ���
	f[a]=(k-3)f[a-1]+(k-2)f[a-2]
���������þ�������ݼ��㣺
	�� f[a] f[a+1] �� �� f[a-1] f[a] ���� k-3 1 ��
	��  0     0    ��=��   0     0   ���� k-2 0 ��
	����f[0]=0,f[1]=k-1
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

typedef signed long long int SINT;

#define PRIMENUM 3410
long primes[PRIMENUM] = {};
bool notprime[32000];

long nmod[32];        //n����ȡ����
long fcnt[100];       //n�����������ݴ�
long faci[100];       //n������
long fval[100];       //n�����ӻ�
long nfac;            //n�����Ӹ���
SINT ans;             //���մ�

#define MAXM 2
#define MODN 1000000007
int n, k;
int mat[32][MAXM][MAXM];  //mat[i]=��ϵ�����2^i����

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

//��չŷ������㷨
SINT ExtendGcd(SINT a,SINT b,SINT &x,SINT &y)
{
	SINT d,tmp;

	if(b==0)
	{
		x = 1;
		y = 0;
		return a;
	}
	d = ExtendGcd(b,a%b,x,y);
	tmp = x;
	x = y;
	y = tmp - (a/b)*y;

	return d;
}

//x��y�˷���Ԫ
SINT Divisor(SINT x, SINT y)
{
	SINT d, ret, tmp;

	d = ExtendGcd(x,y,ret,tmp);
	//���������޽�
	if(d != 1)  
		return -1;
	//�����С��
	ret = (ret % y + y) % y;
	return ret;
}

//���پ�����Ԥ��
void mul(int mata[MAXM][MAXM], int matb[MAXM][MAXM], int prod[MAXM][MAXM])
{
	int i, j, k;
	SINT sum;
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			sum = 0;
			for(k=0;k<2;k++)
			{
				sum += (SINT)mata[i][k] * (SINT)matb[k][j];
				//����ò�Ʋ���ȡģ��ûɶ�á�
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
void mati(int ret[MAXM][MAXM])
{
	int i, j;
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			ret[i][j] = (i==j);
		}
	}
}

SINT getd(int cnt)  
{
	int temp[2][MAXM][MAXM], i, cur = 0;
	SINT ret = 0;

	memset(temp, 0, sizeof(temp));
	temp[cur][0][1] = k - 1;
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
	}
	ret = temp[cur][0][0];
	
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
		ans += (getd(fc - 1) * (SINT)phi) % MODN;
		ans %= MODN;
	}
}

int main()
{
	init_prime();
	while(scanf("%d%d",&n, &k)!=EOF)
	{
		ans = 0;
		fact(n);
		mat[0][0][0] = k - 3;
		mat[0][0][1] = 1;
		mat[0][1][0] = k - 2;
		mat[0][1][1] = 0;
		matpow_init();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		ans *= k;
		ans %= MODN;
		printf("%lld\n",ans);
	}
	return 0;
}
