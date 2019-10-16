#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char code[210];
char str[110][210];
int N;
char set[256];

int check(int x){
	char *s, *t, p;

	for(p = 'a'; p <= 'z'; p ++)
		set[p] = 0;
	set[' '] = ' ';
	for(s = code, t = str[x]; *s && *t; ++ s, ++ t){
		if(set[*t] == 0){
			set[*t] = *s;
		} else{
			if(set[*t] != *s)
				return 0;
		}
	}
	return *s == 0 && *t == 0;
}

void decode(int x){
	char *t;
	for(t = str[x]; *t; ++ t)
		putchar(set[*t]);
	putchar('\n');
}

void domain(){
	int i;

	//取得输入
	N = 0;
	do{
		if(!gets(str[++ N]))
			break;
	} while(str[N][0]);
	-- N;

	//逐一尝试
	for(i = 1; i <= N; i ++){
		if(check(i))
			break;
	}
	if(i > N){
		puts("No solution.");
	} else{
		for(i = 1; i <= N; i ++)
			decode(i);
	}
}

int main(){
	int T;
	scanf("%d\n", &T);
	gets(code);
	gets(str[0]);
	while(T --){
		domain();
		if(T)
			putchar('\n');
	}
	return 0;
}