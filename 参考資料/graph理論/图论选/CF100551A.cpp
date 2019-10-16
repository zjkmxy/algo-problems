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
	int a, b, l, r;	//��ͨ�㣬ʱ��߽�
	edge(int a, int b, int l, int r):a(a),b(b),l(l),r(r){};
};
typedef list <edge> List;
struct NODE{
	list<NODE*> next;	//�ڽӱ�
	int deg;			//����
	bool vis;			//�Ƿ���ʹ�
	int col;			//��ͨ�������
}node[N];

//��ѯ�������𰸣������ñ�ű��������
int cnt[N + 1], ans[N];

//���ӱ�
void add( int a, int b ){
	node[a].next.push_back(&node[b]);
	node[b].next.push_back(&node[a]);
}

//���ѣ���ǰ���ڵ㣬������ͨ�������
void dfs(NODE& cur, int value)
{
	cur.vis = true;
	cur.col = value;
	for(auto& b : cur.next){
		if (!b->vis)
			dfs(*b, value);
	}
}

//ʱ����߽磬�ұ߽磬��Ч�߼��ϣ�������ģ����������̻���ͨ��֧��
//[l, r)
void go(int l, int r, const List &v, int vn, int add_vn){
	//ֻ���޸�û��ѯ
	if (cnt[l] == cnt[r])
		return;
	//ֻ�в�ѯû�޸�
	if (v.size() == 0){
		while (l < r)
			ans[l++] = vn + add_vn;
		return;
	}

	List v1;	//�����õ����ڽӱ�

	//����ڽӱ�
	for_each(node, node + vn, [](NODE& cur){cur.next.clear();});
	for(auto& j : v){
		if (j.a != j.b){
			if (j.l <= l && r <= j.r){
				//����[l,r)�ıߣ����ڼ����ڽӱ�
				add(j.a, j.b);
			}else if (l < j.r && j.l < r){
				//����[l,r)�ڼ���ڹ��ıߣ���������ʹ��
				v1.push_back(j);
			}
		}
	}

	//DFS�õ���ǰ����ͨ��֧��
	int vn1 = 0;
	for_each(node, node + vn, [](NODE& cur){cur.vis = false;});
	for_each(node, node + vn, [&](NODE& cur){
		if(!cur.vis){
			node[vn1].deg = 0;
			dfs(cur, vn1 ++);
		}
	});

	//����ͨ��֧����
	for(edge& j : v1){
		j.a = node[j.a].col;
		j.b = node[j.b].col;
		if (j.a != j.b){
			node[j.a].deg ++;
			node[j.b].deg ++;
		}
	}

	//���������Ҫ�ĵ�
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

	//�ݹ���д���
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
				//�Ի�
				k--;
				i--;
				continue;
			}
			if (a > b)
				swap(a, b);

			//�ӵ�ʱ�򣬼�¼ʱ�䣻����ʱ�򣬼����б�
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
	//��ֱ�����û�м����ı߼����б�
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