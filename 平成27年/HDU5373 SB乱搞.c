#include <stdio.h>

int n, t;
void solve(){
	int sum = 0, mod = n % 11, j;
	while(t --){
		while(n > 0){
			sum += n % 10;
			n /= 10;
		}
		for(j = 1; j <= sum; j *= 10);
		mod = (mod * j + sum) % 11;
		n = sum;
	}
	puts(mod == 0 ? "Yes" : "No");
}

int main() {
	int cas;
	for(cas = 1; 1; ++ cas){
		scanf("%d%d", &n, &t);
		if(n == -1)
			break;
		printf("Case #%d: ", cas);
		solve();
	}
	return 0;
}
