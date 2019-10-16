#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<cctype>
using namespace std;

char str[10000];
int rad[10000];		//表示以i为轴心的最长len
int len;

void calc_rad(char str[], int len)
{
	int i, j, k;

	//预处理成为奇数串
	for(i=len-1;i>=0;i--){
		str[(i<<1)+2] = str[i];
		str[(i<<1)+1] = '#';
	}
	len = (len<<1)+2;
	str[0] = '^';
	str[len-1] = '#';
	str[len] = '$';
	for(i=1,j=0;i<len;){
		//扫描法得出rad[i]
		while(str[i-j-1]==str[i+j+1])
			j++;
		rad[i] = j;
		//确定接下来j个数的rad
		for(k=1;k<=j&&rad[i-k]!=rad[i]-k;k++)
			rad[i+k] = min(rad[i-k], rad[i] - k);
		i += k;
		j = max(j - k, 0);
	}
}

int main()
{
	int i, pos = 1;
	gets(str);
	len = strlen(str);
	calc_rad(str, len);
	for(i=1;i<=(len<<1)+2;i++){
		if(rad[i] > rad[pos]){
			pos = i;
		}
	}
	for(i=pos-rad[pos];i<=pos+rad[pos];i++)
		if(isalpha(str[i]) || isdigit(str[i]))
			putchar(str[i]);
	putchar('\n');
	return 0;
}
