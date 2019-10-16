#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int vis[10005];
int ge[20], N, M;

int gcd(int a, int b){
	return b ? gcd(b, a % b) : a;
}

int cmp(const void *A, const void *B){
	return *(int*)A - *(int*)B;
}

int main(){
	int i, j, a, b, d, p, q;
	int able;

	scanf("%d", &N);
	for(i = 0; i < N; i ++)
		scanf("%d", &ge[i]);
	qsort(ge, N, sizeof(int), cmp);
	for(i = 1; i < N; i ++)
		ge[i] /= ge[0];
	ge[0] = 1;

	//³Ë·¨
	vis[1] = 1;
	for(i = 1; i < N; i ++){
		for(j = ge[i]; j <= 10000; j += ge[i]){
			vis[j] |= vis[j / ge[i]];
		}
	}

	//³ý·¨
	for(i = 1; i <= 10000; i ++){
		if(vis[i])
			continue;
		for(j = 1; j * i <= 10000; j ++){
			if(vis[j] && vis[i * j]){
				vis[i] = 1;
				break;
			}
		}
	}

	//³Ë·¨
	for(i = 2; i <= 10000; i ++){
		if(!vis[i])
			continue;
		for(j = 1; j * i <= 10000 && j <= i; j ++){
			if(vis[j])
				vis[i * j] = 1;
		}
	}

	scanf("%d", &M);
	while(M --){
		scanf("%d%d", &p, &q);
		d = gcd(p, q);
		a = p / d, b = q / d;
		able = 0;
		for(d = 1; a * d <= 10000 && b * d <= 10000; d ++){
			if(vis[a * d] && vis[b * d]){
				able = 1;
				break;
			}
		}
		if(able){
			printf("Gear ratio %d:%d can be realized.\n", p, q);
		} else{
			printf("Gear ratio %d:%d cannot be realized.\n", p, q);
		}
	}
	return 0;
}