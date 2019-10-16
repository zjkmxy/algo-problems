#include <stdio.h>
#include <string.h>

#define LEN 1005
typedef struct _TERM{
	int v[LEN];
	int h, t;
} TERM, *PTERM;
TERM term[105];
int lq[105], qt;
int N, S, Q;
int tim;

void load(TERM* pt){
	int i;
	while(pt->t != pt->h && qt < S){
		lq[qt ++] = pt->v[pt->h ++];
		if(pt->h >= LEN)
			pt->h -= LEN;
		tim ++;
	}
}

void unload(int id){
	int siz;
	TERM* pt = &term[id];
	while(qt > 0){
		if(lq[qt - 1] != id){
			siz = pt->t - pt->h;
			if(siz < 0)
				siz += LEN;
			if(siz >= Q)
				break;
			pt->v[pt->t ++] = lq[qt - 1];
			if(pt->t >= LEN)
				pt->t -= LEN;
		}
		qt --;
		tim ++;
	}
}

int finish(){
	int i;
	for(i = 1; i <= N; i ++)
	if(term[i].t != term[i].h)
		return 0;
	return qt == 0;
}

void domain(){
	int i, j, pos;

	scanf("%d%d%d", &N, &S, &Q);
	qt = 0;
	for(i = 1; i <= N; i ++){
		term[i].h = 0;
		scanf("%d", &term[i].t);
		for(j = 0; j < term[i].t; j ++)
			scanf("%d", &term[i].v[j]);
	}
	pos = 1;
	tim = 0;
	while(!finish()){
		unload(pos);
		load(&term[pos]);
		if(finish())
			break;
		tim += 2;
		pos ++;
		if(pos > N)
			pos -= N;
	}
	printf("%d\n", tim);
}

int main(){
	int T;
	scanf("%d", &T);
	while(T --)
		domain();
	return 0;
}