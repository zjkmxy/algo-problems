//����һ������ͼ����;���Ƿ�����������ż���Ļ�
#include <cstdio>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <string>
#include <map>
using namespace std;
#pragma comment(linker, "/STACK:102400000,102400000")

const int MAXN = 100005;
const int MAXM = 300005;
struct NODE;
struct EDGE;
typedef struct NODE{
	int id;
	int vis;
	int step;
	int depth;
	EDGE *head;
}*PNODE;
typedef struct EDGE{
	PNODE a, b;
	EDGE *next;
	EDGE *rev;
}*PEDGE;
NODE node[MAXN];
EDGE edge[MAXM<<1];
int N, M, ecnt;
bool ans[2];

void addedge(PNODE a, PNODE b){
	edge[ecnt].a = a;
	edge[ecnt].b = b;
	edge[ecnt].next = a->head;
	edge[ecnt].rev = edge + ecnt + 1;
	a->head = edge + ecnt;
	ecnt ++;

	edge[ecnt].a = b;
	edge[ecnt].b = a;
	edge[ecnt].next = b->head;
	edge[ecnt].rev = edge + ecnt - 1;
	b->head = edge + ecnt;
	ecnt ++;
}

void dfs(PNODE cur, PEDGE par){
	PEDGE j;
	PNODE ed;
	bool ring = false;

	cur->vis = -1;
	for(j = cur->head; j; j = j ->next){
		ed = j->b;
		if(j == par){
			continue;
		}
		if(ed->vis == 0){
			//�����µĵ�
			if(cur->step >= 0){
				ed->step = cur->step ^ 1;
			}else{
				ed->step = -1;
			}
			ed->depth = cur->depth + 1;
			dfs(ed, j->rev);
			ed->vis = 1;
			if(ed->step == -1){
				//������˫�Ե㣬��ô�Լ�Ҳ��˫�Ե�
				cur->step = -1;
			}
		}else if(ed->vis == -1){
			if(cur->step == -1 || ed->step == -1){
				//˫�Ե�Ļ��ж�
				ans[0] = ans[1] = true;
				return ;
			}else if(cur->step == ed->step){
				//������
				cur->step = -1; //���Ϊ˫�Ե�
				ans[1] = true;
				if(ring){  //���ͬһ����֮ǰ�����ҵ���������ôֱ����˫yes
					ans[0] = true;
					return ;
				}
			}else{
				//ż����
				ans[0] = true;
			}
			ring = true; //�ҵ�����һ������
		}
	}
	cur->vis = 1;
}

void domain(){
	int i, u, v;
	scanf("%d%d", &N, &M);
	ecnt = 0;
	ans[0] = ans[1] = false;
	for(i = 0; i <= N; i ++){
		node[i].id = i;
		node[i].head = nullptr;
		node[i].vis = 0;
		node[i].step = 0;
	}
	for(i = 0; i < M; i ++){
		scanf("%d%d", &u, &v);
		addedge(node + u, node + v);
	}
	for(i = 1; i <= N; i ++){
		if(ans[0] && ans[1])
			break;
		if(node[i].vis != 1){
			node[i].step = 0;
			node[i].depth = 0;
			dfs(node + i, nullptr);
		}
	}
	if(ans[1]){
		printf("YES\n");
	}else{
		printf("NO\n");
	}
	if(ans[0]){
		printf("YES\n");
	}else{
		printf("NO\n");
	}
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		domain();
	}
	return 0;
}

