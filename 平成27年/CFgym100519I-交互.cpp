#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

bool vis[300005];
int main(){
	int i, n, j;
	char op[3];
	long long int l;

	scanf("%d", &n);
	l = 0;
	for(i = 0; i < 40; i ++){
		printf("2\n");
		fflush(stdout);
		scanf("%s", op);
		if(op[0] == 'O'){
			exit(0);
		}
		l = l << 1;
		if(op[0] == '>')
			l ++;
	}
	//��ʱ��x0֮����pӦ��ǡ����l����2^41��l+1����2^41֮��
	//ע��p���������༴���ѡ�񲻵���p������q��������ĳ��xʹ��x / p �������޽�֮��
	for(i = 2; i < 300005; i ++){
		if(!vis[i]){
			if(  (((l + 1) * i) >> 40) - ((l * i) >> 40) >= 1 ){
				printf("%d\n", i);
				exit(0);
			}
			for(j = i + i; j < 300005; j += i){
				vis[j] = true;
			}
		}
	}
	throw 0;
	return 0;
}
