/*
HDU1527 ȡʯ����Ϸ

�����������ȡ�
�ٶȼ��ɡ�
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
using namespace std;

double ratio;

bool is_p(int a, int b)
{
	int j;

	if(a > b)
		swap(a, b);
	j = floor(a * ratio);
	if(j * (1 + ratio) != a)  //������a[j],b[j]��
		j++;                  //������a[j+1],b[j+1]��
	if(b == a + j)            //b[j] = a[j] + j
		return true;          //a[j]��δ���ڴ������г��ֵ���С����
	else
		return false;
}

int main()
{
	int a, b;
	ratio = (sqrt(5.0) - 1.0) / 2.0;
	while(scanf("%d%d",&a,&b)!=EOF)
	{
		if(is_p(a, b))
		{
			printf("0\n");
		}else{
			printf("1\n");
		}
	}
	return 0;
}