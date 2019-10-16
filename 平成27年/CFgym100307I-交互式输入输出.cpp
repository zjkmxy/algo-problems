#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
using namespace std;

typedef long long int LL;
const int MAXN = 100000;
int xmin[MAXN + 1], xmax[MAXN + 1];
bool valid[MAXN + 1];

//如果以p分割，小于p的点数有多少个
LL getCnt(int p){
	int v;
	LL sum = 0;
	for(v = 0; v <= MAXN; v ++){
		if(!valid[v])
			continue;
		if(xmax[v] >= p && xmin[v] <= p){
			sum += p - xmin[v] + 1;
		}else if(xmax[v] <= p){
			sum += xmax[v] - xmin[v] + 1;
		}
	}
	return sum;
}

//按p分割，根据大于小于的回答更新结果
void update(int p, bool less){
	int v;
	for(v = 0; v <= MAXN; v ++){
		if(!valid[v])
			continue;
		if(less){
			if(xmin[v] > p){
				valid[v] = false;
			}else{
				xmax[v] = min(p, xmax[v]);
			}
		}else{
			if(xmax[v] < p){
				valid[v] = false;
			}else{
				xmin[v] = max(p, xmin[v]);
			}
		}
	}
}

void deal(){
	int v, l, r, mid, t;
	LL sum = 0, cur;
	char str[4];
	for(v = 0; v <= MAXN; v ++){
		xmin[v] = 0;
		xmax[v] = MAXN;
		valid[v] = true;
	}
	for(t = 0; t < 99; t ++){
		sum = 0;
		for(v = 0; v <= MAXN; v ++){
			if(valid[v]){
				sum += xmax[v] - xmin[v] + 1;
			}
		}
		l = 0;
		r = MAXN * (1 + t);
		if(t == 50){
			t = t;
		}
		while(r > l + 1){
			mid = (l + r) >> 1;
			cur = getCnt(mid) << 1;
			if(cur < sum){
				l = mid;
			}else if(cur > sum){
				r = mid;
			}else{
				break;
			}
		}
		if(r > l + 1){
			l = mid;
		}
		//二分找到半分位点，将可能的结果范围去一半
		printf("check 0 %d\n", l);
		fflush(stdout);
		scanf("%s", str);
		if(str[0] == 'Y'){
			update(l, true);
		}else{
			update(l + 1, false);
		}
		for(v = 0; v <= MAXN; v ++){
			if(valid[v]){
				xmin[v] += v;
				xmax[v] += v;
			}
		}
	}
	int mv = -1;
	for(v = 0; v <= MAXN; v ++){
		if(valid[v]){
			if(xmax[v] > xmin[v] + 1){
				throw 0;
			}else{
				if(mv > 0){
					throw 0;
				}else{
					mv = v;
				}
			}
		}
	}
	printf("check %d %d\n", xmin[mv], xmin[mv]);
	fflush(stdout);
	scanf("%s", str);
	if(str[0] == 'Y'){
		printf("answer %d\n", xmin[mv] + mv);
	}else{
		printf("answer %d\n", xmax[mv] + mv);
	}
	fflush(stdout);
}

int main() {
	scanf("%*d%*d");
	deal();
	return 0;
}
