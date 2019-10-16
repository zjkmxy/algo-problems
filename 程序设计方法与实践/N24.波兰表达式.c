#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char str[100];

int GetVal(){
	int ret, a, b;
	char op;

	scanf("%s", str);
	if(isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]))){
		sscanf(str, "%d", &ret);
	} else{
		op = str[0];
		a = GetVal();
		b = GetVal();
		switch(op){
		case '+':
			ret = a + b;
			break;
		case '-':
			ret = a - b;
			break;
		case '*':
			ret = a * b;
			break;
		default:
			ret = *(int*)NULL;
			break;
		}
	}
	return ret;
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --){
		printf("%d\n", GetVal());
	}
	return 0;
}