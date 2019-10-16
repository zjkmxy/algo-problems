#include <cstdio>
#include <algorithm>
#include <set>
using namespace std;

const int MAXN = 200005;
const int MAXM = 1000005;
const int INF = 0x7fffffff;
struct DATA{
	int d, l, h;
}arr[MAXN];
int N, S, T;
int dp[MAXN];	//�μ���i�κ����Сƣ��
int pre[MAXN];	//ת��λ��

//ȡ�òμ���ĳ�α������skill
inline int GetSkill(int id, int f){
	if(f >= INF)
		return -1;
	else 
		return arr[id].h - (f - T);
}

//ȡ�ô�i�α�����j�α�����ƣ��
inline int GetFatigue(int i, int j){
	int d = arr[j].d - arr[i].d - 1;
	if(d < 0)
		return INF;
	else if(d > 30)
		return 0;
	else
		return dp[i] >> d;
}

struct GOTO{
	int id, fat;
	GOTO(){
		id = -1;
		fat = INF;
	};
	GOTO(int id):id(id){
		if(id == 0){
			fat = 0;
		}else{
			fat = dp[id];
		}
	};
	bool operator<(const GOTO& rhs)const{
		return fat < rhs.fat;
	};
};
//ȡ��ĳһ�ض�skill�����ڵ�fat��Сֵ
class Segtree{
	GOTO node[MAXM<<2];		//�߶νڵ�
	int flg[MAXM<<2];		//���Ʊ��
	int N, day;
	inline int LCHD(int id){return id << 1;}
	inline int RCHD(int id){return id << 1 | 1;}
	inline int MID(int l, int r){return (l + r) >> 1;}
	inline int SHIFT(int x, int d){
		if(x >= INF)
			return INF;
		if(d > 30)
			return 0;
		return x >> d;
	}
	inline void pushdown(int id){
		if(flg[id] > 0){
			node[LCHD(id)].fat = SHIFT(node[LCHD(id)].fat, flg[id]);
			node[RCHD(id)].fat = SHIFT(node[RCHD(id)].fat, flg[id]);
			flg[LCHD(id)] += flg[id];
			flg[RCHD(id)] += flg[id];
			flg[id] = 0;
		}
	}
	inline void popup(int id){
		node[id] = min(node[LCHD(id)], node[RCHD(id)]);
	}
	//����
	void build(int id, int L, int R){
		int mid = MID(L, R);
		flg[id] = 0;
		if(L == R){
			node[id] = GOTO();
		}else{
			build(LCHD(id), L, mid);
			build(RCHD(id), mid + 1, R);
			popup(id);
		}
	}
	//����ĳһ����ֵ���ƣ��
	void update(int id, int pos, const GOTO& data, int L, int R){
		if(L == R){
			node[id] = min(node[id], data);
			return ;
		}
		int mid = MID(L, R);
		pushdown(id);
		if(pos <= mid){
			update(LCHD(id), pos, data, L, mid);
		}else{
			update(RCHD(id), pos, data, mid + 1, R);
		}
		popup(id);
	}
	//��ѯĳ�������ϵ���Сƣ��
	GOTO query(int id, int l, int r, int L, int R){
		if(L == l && r == R){
			return node[id];
		}
		int mid = MID(L, R);
		pushdown(id);
		if(r <= mid){
			return query(LCHD(id), l, r, L, mid);
		}else if(l > mid){
			return query(RCHD(id), l, r, mid + 1, R);
		}else{
			return min(query(LCHD(id), l, mid, L, mid), query(RCHD(id), mid + 1, r, mid + 1, R));
		}
	}
public:
	inline void Build(){
		N = MAXM - 1;
		day = 0;
		build(1, 0, N);
	}
	inline void Update(int pos, const GOTO& data){
		update(1, pos, data, 0, N);
	}
	inline GOTO Query(int l){
		return query(1, l, MAXM - 1, 0, N);
	}
	//ʱ���Ǩ
	inline void Date(int d){
		if(d < day)
			throw 0;
		if(d > day){
			node[1].fat = SHIFT(node[1].fat, d - day);
			flg[1] += (d - day);
			day = d;
		}
	}
}segt;

void BF_Solve(){
	int i, j, cur;
	for(i = 1; i <= N; i ++){
		dp[i] = (arr[i].l <= S) ? T : INF;
		pre[i] = 0;
		for(j = 1; j < i; j ++){
			if(GetSkill(j, dp[j]) < arr[i].l)
				continue;
			cur = GetFatigue(j, i);
			if(cur > arr[i].h)
				continue;
			if(dp[i] > cur + T){
				dp[i] = cur + T;
				pre[i] = j;
			}
		}
	}
}

void Solve(){
	int i, j;
	GOTO cur;
	segt.Build();
	segt.Update(S, GOTO(0));
	for(i = 1; i <= N; i = j){
		segt.Date(arr[i].d);
		for(j = i; j <= N && arr[j].d == arr[i].d; j ++){
			cur = segt.Query(arr[j].l);
			if(cur.fat >= INF){
				dp[j] = INF;
				pre[j] = 0;
			}else{
				dp[j] = cur.fat + T;
				pre[j] = cur.id;
			}
		}
		segt.Date(arr[i].d + 1);	//ע������1���ǲ���ƣ�͵ģ���������λ1���ټ�
		for(j = i; j <= N && arr[j].d == arr[i].d; j ++){
			segt.Update(GetSkill(j, dp[j]), GOTO(j));
		}
	}
}

void GetAns(){
	int cur, ans, p, i;
	static int stk[MAXN], sp;
	ans = S;
	p = 0;
	for(i = 1; i <= N; i ++){
		cur = GetSkill(i, dp[i]);
		if(cur > ans){
			ans = cur;
			p = i;
		}
	}
	sp = 0;
	while(p > 0){
		stk[sp ++] = p;
		p = pre[p];
	}
	printf("%d %d\n", ans, sp);
	while(sp --){
		printf(sp > 0 ? "%d " : "%d\n", stk[sp]);
	}
	return;
}

int main(){
	int i;
	scanf("%d%d%d", &N, &S, &T);
	for(i = 1; i <= N; i ++){
		scanf("%d%d%d", &arr[i].d, &arr[i].l, &arr[i].h);
	}
	Solve();
	GetAns();
	return 0;
}
