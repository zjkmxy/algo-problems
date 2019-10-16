#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#define nullptr NULL

struct MergeSet;
typedef MergeSet *PSet;
struct MergeSet{
	PSet parent;
	int size;
	PSet root(){
		if(parent == nullptr){
			return this;
		}else{
			parent = parent->root();
			return parent;
		}
	}
	int merge(PSet par){
		PSet tmp = root();
		int ret;
		par = par->root();
		if(tmp != par){
			ret = par->size;
			tmp->parent = par;
			par->size += tmp->size;
			tmp->size = 0;
			return ret;
		}else{
			return tmp->size;
		}
	}
};
struct Node{
	int x, y;
	MergeSet setx, sety;
	int level;
	void init(int x, int y){
		this->x = x;
		this->y = y;
		setx.size = 1;
		setx.parent = nullptr;
		sety.size = 1;
		sety.parent = nullptr;
		scanf("%d", &level);
	}
	bool operator<(const Node& rhs)const{
		return level < rhs.level;
	}
}node[1000005];
Node* map[105][10005];
int n, m, cnt;

bool input(){
	int i, j;
	cnt = 0;
	scanf("%d%d", &n, &m);
	for(i = 0; i < n; i ++)
		for(j = 0; j < m; j ++)
			node[cnt ++].init(i, j);
	std::sort(node, node + cnt);
	for(i = 0; i < cnt; i ++){
		map[node[i].x][node[i].y] = node + i;
	}
	return (n > 0 || m > 0);
}

int calc(){
	int l, i, j, x, y;
	int res = 0;
	for(i = 0, l = 1; i < cnt; l ++){
		for(j = i; j < cnt && node[j].level < l; j ++){
			x = node[j].x;
			y = node[j].y;
			if(x > 0 && map[x-1][y]->level < l)
				node[j].setx.merge(&map[x-1][y]->setx);
			if(x < n - 1 && map[x+1][y]->level < l)
				node[j].setx.merge(&map[x+1][y]->setx);
			if(y > 0 && map[x][y-1]->level < l)
				node[j].sety.merge(&map[x][y-1]->sety);
			if(y < m - 1 && map[x][y+1]->level < l)
				node[j].sety.merge(&map[x][y+1]->sety);
			if(node[j].setx.root()->size == n || node[j].sety.root()->size == m){
				return -1;
			}

			if(map[0][y]->setx.root() == node[j].setx.root() || map[n-1][y]->setx.root() == node[j].setx.root()){
				res = std::max(res, node[j].setx.root()->size + 1);
			}else{
				res = std::max(res, (node[j].setx.root()->size + 1) / 2 + 1);
			}
			if(map[x][0]->sety.root() == node[j].sety.root() || map[x][m-1]->sety.root() == node[j].sety.root()){
				res = std::max(res, node[j].sety.root()->size + 1);
			}else{
				res = std::max(res, (node[j].sety.root()->size + 1) / 2 + 1);
			}
		}
		i = j;
		if(res <= l && i < cnt){
			return l;
		}
	}
	return -1;
}

int main(){
	int ans;
	while(input()){
		ans = calc();
		if(ans >= 0){
			printf("%d\n", ans);
		}else{
			printf("NO ANSWER!\n");
		}
	}
	return 0;
}
