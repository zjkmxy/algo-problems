//�������Ⱳ�ӳ������ѵ��⡣���ܾ�����鿴����
#include<stdio.h>

#define MAXN 20
#define MODN 1000000009
typedef signed long long int LL;
LL comb[MAXN+1][MAXN+1];
LL fac[MAXN+1];

void init()
{
	int i, j;

	fac[0] = 1;
	comb[0][0] = 1;
	for(i=1;i<=MAXN;i++)
		fac[i] = i * fac[i-1] % MODN;
	for(i=1;i<=MAXN;i++){
		comb[i][0] = 1;
		for(j=1;j<=i;j++){
			comb[i][j] = comb[i-1][j-1] + comb[i-1][j];
			if(comb[i][j] > MODN)
				comb[i][j] -= MODN;
		}
	}
}

int main()
{
	int n;
	LL a1, a2, a3;
	init();
	while(scanf("%d",&n)!=EOF){
		printf("%d\n",n + (n >> 1) + 1);
		a1 = (n == 2) ? 1 : (fac[n >> 1] >> 1);	//n/2+1��ѭ���ģ�ȥ�Գ�Բ���еĸ�����ע��2��ʱ����1
		a2 = (n == 2) ? 1 : fac[(n >> 1) + 1];	//n/2+1��ѭ���������Ÿ���
		a3 = comb[n][(n >> 1) - 1];
		a3 = a3 * a3 % MODN;					//��ѡ���з��ö���Ԫ��
		printf("%lld\n", a1 * a2 % MODN * a3 % MODN);
	}
	return 0;
}