/*
POJ2888 Magic Bracelet

Polya������ʹ�ˣ�ֱ����Burnside������
��ϸ��˵��μ�getd��ע��
*/
#include<cstdio>
#include<cstdlib>
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

#define MAXM 11
#define MODN 9973
int n, m, k;
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
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		{
			prod[i][j] = 0;
			for(k=0;k<m;k++)
			{
				prod[i][j] += mata[i][k] * matb[k][j];
				//prod[i][j] %= MODN;  ���ﲻע�͵��Ļ����ܵú�������
			}
			prod[i][j] %= MODN;
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
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		{
			ret[i][j] = (i==j);
		}
	}
}

/*
���ĺ���getd�������ڱ任f��cnt��ѭ���ڵ�ʱ�����ǵõ��Ĳ�����ɫ������D(f)
Ҳ���ǡ�����cnt����Ʒ������������������κα任�������ܵõ��Ŀ��з�����
������ai��ͷ������ai-b1-b2-...-bq-ai������������ô�ض��й�ϵ����
g[ai,b1]*g[b1,b2]*...*g[bq,ai]=1
������ai��ͷ�Ŀ��������ĸ������ǹ�ϵ����g��k�����жԽ�Ԫ��gk[ai,ai]��ֵ
���������þ�������ݼ���
*/
int getd(int cnt)  
{
	int temp[2][MAXM][MAXM], i, cur = 0, ret = 0;

	mati(temp[cur]);
	for(i=0;cnt>0;i++,cnt>>=1)
	{
		if(cnt & 1)
		{
			mul(temp[cur], mat[i], temp[cur^1]);
			cur ^= 1;
		}
	}
	for(i=0;i<m;i++)
	{
		ret += temp[cur][i][i];
		//ret %= MODN;  //����Ҳ��Ч������
	}
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
		ans += (getd(fc) * (SINT)phi) % MODN;
		ans %= MODN;
	}
}

int main()
{
	int t, i, j, a, b;

	scanf("%d",&t);
	init_prime();
	while(t--)
	{
		scanf("%d%d%d",&n, &m, &k);
		ans = 0;
		fact(n);
		for(i=0;i<m;i++)
		{
			for(j=0;j<m;j++)
			{
				mat[0][i][j] = 1;
			}
		}
		for(i=0;i<k;i++)
		{
			scanf("%d%d",&a,&b);
			a--;
			b--;
			mat[0][a][b] = mat[0][b][a] = 0;
		}
		matpow_init();
		DFS(0, 1, 1);
		ans *= Divisor(n, MODN);
		ans %= MODN;
		printf("%lld\n",ans);
	}
	return 0;
}
