//本地对拍正确，上交WA
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 105;
const LL INF = 0x3fffffffffffffffLL;
const int ROUND = 2000;

int N, M;
namespace solve{
	struct DATA{
		LL t;
		int v;
		//按钱数比较
		bool operator<(const DATA& rhs)const{
			return v < rhs.v;
		}
		DATA(){};
		DATA(LL t, int v):t(t),v(v){};
	};
	vector<DATA> sel[7];	//染色后，每一种的选择
	vector<DATA> tab;	//3种的打表
	LL ans;

	//后3种打表（要求清空tab）
	void BuildTab(int col, int val, LL tim){
		vector<DATA>::iterator it;
		if(col == 7){
			tab.push_back(DATA(tim, val));
			return ;
		}
		for(it = sel[col].begin(); it != sel[col].end(); it ++){
			if(val + it->v <= M){
				BuildTab(col + 1, val + it->v, tim + it->t);
			}
		}
	}

	//前4种暴搜（要求tab不为空，且已经排序去重好）
	void Search4(int col, int val, LL tim){
		vector<DATA>::iterator it;
		if(col == 4){
			val = M - val;
			it = upper_bound(tab.begin(), tab.end(), DATA(0, val));
			if(it != tab.begin()){
				-- it;
				tim += it->t;
				solve::ans = min(solve::ans, tim);
			}
			return ;
		}
		for(it = sel[col].begin(); it != sel[col].end(); it ++){
			if(val + it->v <= M){
				Search4(col + 1, val + it->v, tim + it->t);
			}
		}
	}

	//规范化一个vec
	void Regular(vector<DATA>& vec){
		int i, j, sz = vec.size();
		LL mv = INF;
		sort(vec.begin(), vec.end());
		i = 0;
		j = 0;
		for(i = j = 0; j < sz; i ++){
			vec[i] = vec[j];
			mv = vec[i].t;
			do{
				j++;
			}while((j < sz) && (vec[j].t >= mv));
		}
		while(vec.size() > i)
			vec.pop_back();
	}

	//完整求解7色问题
	LL Solve(){
		int i;
		ans = INF;
		for(i = 0; i < 7; i ++)
			Regular(sel[i]);
		solve::tab.clear();
		BuildTab(4, 0, 0);
		Regular(tab);
		if(tab.size() > 0)
			Search4(0, 0, 0);
		return ans;
	}
};


struct NODE{
	int S, T, V;	//星，时间，钱
}node[MAXN];
int strcol[MAXN];	//每种颜色的随机染色
int ns;				//星数
LL ans;

//完整地求解100色问题
LL Solve(){
	int i, j;
	LL cur;

	ans = INF;
	//排序离散化
	for(i = 0; i < N; i ++){
		strcol[ns ++] = node[i].S;
	}
	sort(strcol, strcol + ns);
	ns = unique(strcol, strcol + ns) - strcol;
	for(i = 0; i < N; i ++){
		node[i].S = lower_bound(strcol, strcol + ns, node[i].S) - strcol;
	}
	if(ns < 7)
		return ans;
	//随机染色
	j = ROUND;
	while(j --){
		for(i = 0; i < 7; i ++)
			strcol[i] = i;
		for(i = 7; i < ns; i ++){
			strcol[i] = rand() % 7;
		}
		for(i = 0; i < 7; i ++)
			solve::sel[i].clear();
		for(i = 0; i < N; i ++){
			solve::sel[strcol[node[i].S]].push_back(solve::DATA(node[i].T, node[i].V));
		}

		//bool flag = false;
		//for(i = 0; i < 7; i ++){
		//	if(solve::sel[i].size() == 0){
		//		flag = true;
		//		break;
		//	}
		//}
		//if(flag){
		//	j ++;
		//	continue;
		//}

		cur = solve::Solve();
		ans = min(ans, cur);
	}
	return ans;
}

void domain(){
	int i;
	scanf("%d%d", &N, &M);
	for(i = 0; i < N; i ++){
		scanf("%d%d%d", &node[i].S, &node[i].V, &node[i].T);
	}
	Solve();
	if(ans >= INF){
		printf("-1\n");
	}else{
		printf("%lld\n", ans);
	}
}

int main(){

	freopen("dat.in", "r", stdin);
	freopen("dat.my", "w", stdout);

	int T;
	srand(97);
	scanf("%d", &T);
	while(T --){
		domain();
	}
	return 0;
}
