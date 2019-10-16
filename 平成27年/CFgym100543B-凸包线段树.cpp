#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 100005;
#define LCHD(id)		((id) << 1)
#define RCHD(id)		(((id) << 1) + 1)
#define MID(l, r)		(((l) + (r)) >> 1)

struct VECTOR{
	int x, y;
	VECTOR(int x = 0, int y = 0):x(x), y(y){};
	VECTOR operator+(const VECTOR& b)const{
		return VECTOR(x + b.x, y + b.y);
	}
	VECTOR operator-(const VECTOR& b)const{
		return VECTOR(x - b.x, y - b.y);
	}
	//Cross Product
	LL operator*(const VECTOR& b)const{
		LL ret = static_cast<LL>(x) * static_cast<LL>(b.y);
		ret -= static_cast<LL>(y) * static_cast<LL>(b.x);
		return ret;
	}
};
VECTOR point[MAXN];
int n;

class TREE{
private:
	struct NODE{
		int l, r;
		vector<int> convex;
	}node[MAXN<<2];

	//合并两个子节点的凸包为大凸包
	void merge(int id){
		const vector<int> &lc = node[LCHD(id)].convex, &rc = node[RCHD(id)].convex;
		vector<int> &ret = node[id].convex;
		ret.assign(lc.begin(), lc.end());
		//因为rc的第一个元素就是lc的最后一个，防止重复而抛弃
		ret.pop_back();
		for(int z : rc){
			while(ret.size() >= 2){
				int x = ret[ret.size()-1], y = ret[ret.size()-2];
				if((point[y] - point[x]) * (point[z] - point[y]) < 0){
					ret.pop_back();
				}else{
					break;
				}
			}
			ret.push_back(z);
		}
	}

	//point[a]->point[a+1]的线段能否看到hull的凸包
	bool intersec(int a, const vector<int>& hull){
		VECTOR d = point[a + 1] - point[a];
		//找到相切的点mid，然后依据点和线的位置关系判断
		int l = 1, r = hull.size() - 1, mid;
		while(r > l + 1){
			mid = MID(l, r);
			if(d * (point[hull[mid]] - point[hull[mid-1]]) > 0){
				l = mid;
			}else{
				r = mid;
			}
		}
		if(d * (point[hull[r]] - point[hull[r - 1]]) > 0){
			mid = hull[r];
		}else if(d * (point[hull[l]] - point[hull[l - 1]]) < 0){
			mid = hull[l - 1];
		}else{
			mid = hull[l];
		}
		return (d * (point[mid] - point[a])) > 0;
	}

	//连续下降查找部分，寻找第一个能被p->p+1看到的线段
	//没有返回0
	int query(int id, int p){
		int ret;
		if(!intersec(p, node[id].convex)){
			return 0;
		}else{
			if(node[id].r > node[id].l + 1){
				ret = query(LCHD(id), p);
				if(ret > 0){
					return ret;
				}else{
					return query(RCHD(id), p);
				}
			}else{
				return node[id].l;
			}
		}
	}

public:
	//构建线段树
	void build(int id, int l, int r){
		int mid = MID(l, r);
		node[id].l = l;
		node[id].r = r;
		node[id].convex.clear();
		if(l == r - 1){
			node[id].convex.push_back(l);
			node[id].convex.push_back(r);
		}else{
			build(LCHD(id), l, mid);
			build(RCHD(id), mid, r);
			merge(id);
		}
	}

	//上升查找部分
	int query(int p){
		int id = 1, ret = 0, mid;
		while(node[id].l < p || node[id].r > p + 1){
			mid = MID(node[id].l, node[id].r);
			if(p < mid){
				id = LCHD(id);
			}else{
				id = RCHD(id);
			}
		}
		while(id > 1 && ret == 0){
			if((id & 1) == 0){
				ret = query(id + 1, p);
			}
			id >>= 1;
		}
		return ret;
	}
}tree;

void domain(){
	int i;
	scanf("%d", &n);
	for(i = 1; i <= n; i ++){
		scanf("%d%d", &point[i].x, &point[i].y);
	}
	tree.build(1, 1, n);
	for(i = 1; i < n; i ++){
		if(i > 1)
			printf(" ");
		printf("%d", tree.query(i));
	}
	printf("\n");
	return ;
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		domain();
	}
	return 0;
}
