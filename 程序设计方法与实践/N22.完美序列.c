#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100005
char str[MAXN];
int val[MAXN];
int N, cnt;

int domain(){
	int i, sum = 0;

	cnt = 0;
	for(i = N - 1; i >= 0; i --){
		switch(str[i]){
		case ')':
			sum ++;
			break;
		case '(':
			sum --;
			if(sum < 0){
				if(cnt <= 0)
					return 0;
				val[cnt - 1] ++;
				sum ++;
			}
			break;
		case '#':
			sum ++;
			val[cnt ++] = 1;
			break;;
		}
	}
	return sum == 0;
}

int main(){
	int i;

	while(gets(str)){
		N = strlen(str);
		if(domain()){
			for(i = cnt - 1; i >= 0; i --)
				printf("%d\n", val[i]);
		} else
			puts("-1");
	}
	return 0;
}