#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

int w0, w1, w2, b0, b1, b2;
int N, M;
struct EDGE{
	int a, b, col;
	EDGE(){}
	EDGE(int a, int b, int col):a(a),b(b),col(col){}
}edge[24005];
int tab[24005];

void spj(){
	if(N == 3){
		//1 1 1 1 1 1
		puts("-1");
	}else{
		//1 2 1 1 2 1
		printf("4\n1 2 0\n2 3 0\n1 4 1\n3 4 1\n");
	}
}

int main(){
	int T, p;
	scanf("%d", &T);
	while(T --){
		scanf("%d%d%d%d%d%d", &w0, &w1, &w2, &b0, &b1, &b2);
		N = w0 + w1 + w2;
		M = 0;
		if((w1 & 1) || (b1 & 1)){
			puts("-1");
			continue;
		}
		if(N <= 4){
			spj();
			continue;
		}
		if(w1 < 2 || b1 < 2){
			w1 = *(int*)NULL;
		}
		for(p = 1; w2 > 0; p ++, w2 --){
			edge[M ++] = EDGE(p, p + 1, 0);
		}
		edge[M ++] = EDGE(p, p + 1, 0);
		p += 2;
		w1 -= 2;
		for(; w1 > 0; p += 2, w1 -= 2)
			edge[M ++] = EDGE(p, p + 1, 0);

		tab[1] = 1;
		for(p = 2; p <= N; p ++){
			tab[p] = tab[p - 1] + 2;
			if(tab[p] > N)
				tab[p] = 2;
		}
		for(p = 1; b2 > 0; p ++, b2 --){
			edge[M ++] = EDGE(tab[p], tab[p + 1], 1);
		}
		edge[M ++] = EDGE(tab[p], tab[p + 1], 1);
		p += 2;
		b1 -= 2;
		for(; b1 > 0; p += 2, b1 -= 2)
			edge[M ++] = EDGE(tab[p], tab[p + 1], 1);

		printf("%d\n", M);
		for(p = 0; p < M; p ++){
			printf("%d %d %d\n", edge[p].a, edge[p].b, edge[p].col);
		}
	}
	return 0;
}