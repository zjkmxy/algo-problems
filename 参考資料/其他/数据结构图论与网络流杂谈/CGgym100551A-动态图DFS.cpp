#include <cstdio>
#include <algorithm>
#include <list>
#include <map>

using namespace std;

typedef list <int> vi;
typedef pair <int, int> pii;

const int N = (int)3e5;

struct edge
{
	int a, b, l, r;	//连通点，时间边界
	edge(int a, int b, int l, int r):a(a),b(b),l(l),r(r){};
};
typedef list <edge> List;
struct NODE{
	list<NODE*> next;	//邻接表
	int deg;			//度数
	bool vis;			//是否访问过
	int col;			//连通分量编号
}node[N];

//查询个数，答案，缩点用编号表，顶点度数
int cnt[N + 1], ans[N];

//增加边
void add( int a, int b ){
	node[a].next.push_back(&node[b]);
	node[b].next.push_back(&node[a]);
}

//深搜：当前根节点，新设连通分量编号
void dfs(NODE& cur, int value)
{
	cur.vis = true;
	cur.col = value;
	for(auto& b : cur.next){
		if (!b->vis)
			dfs(*b, value);
	}
}

//时间左边界，右边界，有效边集合，（缩完的）顶点数，固化连通分支数
//[l, r)
void go(int l, int r, const List &v, int vn, int add_vn){
	//只有修改没查询
	if (cnt[l] == cnt[r])
		return;
	//只有查询没修改
	if (v.size() == 0){
		while (l < r)
			ans[l++] = vn + add_vn;
		return;
	}

	List v1;	//孩子用的新邻接表

	//清空邻接表
	for_each(node, node + vn, [](NODE& cur){cur.next.clear();});
	for(auto& j : v){
		if (j.a != j.b){
			if (j.l <= l && r <= j.r){
				//覆盖[l,r)的边，现在加入邻接表
				add(j.a, j.b);
			}else if (l < j.r && j.l < r){
				//其他[l,r)期间存在过的边，给孩子们使用
				v1.push_back(j);
			}
		}
	}

	//DFS得到当前的连通分支数
	int vn1 = 0;
	for_each(node, node + vn, [](NODE& cur){cur.vis = false;});
	for_each(node, node + vn, [&](NODE& cur){
		if(!cur.vis){
			node[vn1].deg = 0;
			dfs(cur, vn1 ++);
		}
	});

	//按连通分支缩边
	for(edge& j : v1){
		j.a = node[j.a].col;
		j.b = node[j.b].col;
		if (j.a != j.b){
			node[j.a].deg ++;
			node[j.b].deg ++;
		}
	}

	//清理掉不需要的点
	vn = vn1;
	vn1 = 0;
	for_each(node, node + vn, [&](NODE& cur){
		cur.col = vn1;
		if(cur.deg > 0)
			vn1 ++;
		else
			add_vn ++;
	});
	for(edge& j : v1){
		j.a = node[j.a].col;
		j.b = node[j.b].col;
	}

	//递归进行处理
	int mid = (l + r) >> 1;
	go(l, mid, v1, vn1, add_vn);
	go(mid, r, v1, vn1, add_vn);
}

int main()
{
#define NAME "connect"
#ifndef _DEBUG
	freopen(NAME ".in", "r", stdin);
	freopen(NAME ".out", "w", stdout);
#endif

	map <pii, int> m;
	List v;
	int n, k;
	scanf("%d%d", &n, &k);
	for(int i = 0; i < k; i ++){
		char type;
		scanf(" %c", &type);
		cnt[i + 1] = cnt[i];
		if (type == '+' || type == '-'){
			int a, b;
			scanf("%d%d", &a, &b);
			a--;
			b--;
			if (a == b){
				//自环
				k--;
				i--;
				continue;
			}
			if (a > b)
				swap(a, b);

			//加的时候，记录时间；减的时候，加入列表
			if (type == '+'){
				m[make_pair(a, b)] = i;
			}else{
				auto it = m.find(make_pair(a, b));
				v.push_back(edge(a, b, it->second, i));
				m.erase(it);
			}
		}else{
			cnt[i + 1]++;
		}
	}
	//把直到最后都没有减掉的边加入列表
	for(auto &it : m){
		v.push_back(edge(it.first.first, it.first.second, it.second, k));
	}

	go(0, k, v, n, 0);

	for(int i = 0; i < k; i ++){
		if (cnt[i + 1] != cnt[i])
			printf("%d\n", ans[i]);
	}
	return 0;
}