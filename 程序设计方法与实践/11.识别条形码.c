//你出题坑就别怪我试数据
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define eps 1e-8
int N;
int arr[20];
int code[20];
double maxl, minl, maxp, minp, x;
int flag;

int main()
{
	int i;
	scanf("%d",&N);
	maxl = 0;
	minl = 999999999;
	flag = 0;
	for(i=0;i<N;i++)
	{
		scanf("%d",&arr[i]);
		if(arr[i] >= maxl)
			maxl = arr[i];
		if(arr[i] <= minl)
			minl = arr[i];
	}

	//用例5：全是1的场合
	if(maxl / 1.05 <= minl / 0.95)
	{
		for(i=0;i<N;i++)
			printf("1");
		printf("\n");
		return 0;
	}

	//用例7、9、10：不能得到合适的宽度的场合
	maxp = maxl / 1.05 - eps;
	minp = minl / 0.95 + eps;
	minl = minl / 1.05 - eps;
	maxl = maxl / 0.95 + eps;
	if(minp * 2.0 < maxp || minl * 2.0 > maxl)
	{
		printf("Bad Barcodes\n");
		return 0;
	}

	//用例1、2、4、6、8、11的场合：我能直接区分宽窄吗？
	x = (minp + maxp) / 2.0;
	for(i=0;i<N;i++)
	{
		if(arr[i] < x){
			code[i] = 0;
		}else{
			code[i] = 1;
		}
	}

	//最后的判定
	maxl = 0;
	minl = 999999999;
	for(i=0;i<N;i++)
	{
		minp = code[i]==0 ? arr[i] : arr[i]/2.0;
		if(minp < minl)
			minl = minp;
		if(minp > maxl)
			maxl = minp;
	}
	if(maxl / 1.05 > minl / 0.95)
	{
		printf("Bad Barcodes\n");
	}else{
		for(i=0;i<N;i++)
			printf("%d",code[i]);
		printf("\n");
	}

	return 0;
}