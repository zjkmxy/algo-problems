#include <stdio.h>
#include <string.h>
#include <ctype.h>

char B[1 << 15], *S = B, *T = B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
	int aa;
	char ch;
	do ch = getc(); while(!isdigit(ch));
	aa = ch - '0';
	for(ch = getc(); isdigit(ch); ch = getc())
		aa = aa * 10 + ch - '0';
	return aa;
}

int a1i[131173], ai1[131173];
int mat[105][105];

int get(int n, int m){
	n = n + m - 2;
	m = m - 1;
	return (n & m) == m;
}

int calc(int n, int m){
	int i, ans = 0;

	for(i = 2; i <= m; i ++){
		if(get(n - 1, m - i + 1))
			ans ^= a1i[i];
	}
	for(i = 2; i <= n; i ++){
		if(get(n - i + 1, m - 1))
			ans ^= ai1[i];
	}
	return ans;
}

int main(){
	int i, j, Q, N, M, ans;

	for(i = 2; i <= 131172; i ++){
		a1i[i] = ReadInt();
		if(i <= 100)
			mat[1][i] = a1i[i];
	}
	for(i = 2; i <= 131172; i ++){
		ai1[i] = ReadInt();
		if(i <= 100)
			mat[i][1] = ai1[i];
	}

	for(i = 1; i <= 100; i ++){
		mat[1][i] = calc(131072 + 1, 131072 + i);
		mat[i][1] = calc(131072 + i, 131072 + 1);
	}
	for(i = 2; i <= 100; i ++){
		for(j = 2; j <= 100; j ++){
			mat[i][j] = mat[i - 1][j] ^ mat[i][j - 1];
		}
	}

	Q = ReadInt();
	while(Q --){
		N = ReadInt(), M = ReadInt();
		printf("%d\n", mat[N][M]);
	}

	return 0;
}