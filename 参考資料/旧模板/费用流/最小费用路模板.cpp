const int N = 1010;//点
const int M = 2 * 10010;//边
const int inf = 1000000000;
struct Node{//边，点f到点t，流量为c，费用为w
	int f, t, c, w;
}e[M];
int next1[M], point[N], dis[N], q[N], pre[N], ne;//ne为已添加的边数，next，point为邻接表,dis为花费，pre为父亲节点
bool u[N];
void init(){
	memset(point, -1, sizeof(point));
	ne = 0;
}
void add_edge(int f, int t, int d1, int d2, int w){//f到t的一条边，流量为d1,反向流量d2,花费w,反向边花费-w（可以反悔）
	e[ne].f = f, e[ne].t = t, e[ne].c = d1, e[ne].w = w;
	next1[ne] = point[f], point[f] = ne++;
	e[ne].f = t, e[ne].t = f, e[ne].c = d2, e[ne].w = -w;
	next1[ne] = point[t], point[t] = ne++;
}
bool spfa(int s, int t, int n){
	int i, tmp, l, r;
	memset(pre, -1, sizeof(pre));
	for(i = 0; i < n; ++i)
		dis[i] = inf;
	dis[s] = 0;
	q[0] = s;
	l = 0, r = 1;
	u[s] = true;
	while(l != r) {
		tmp = q[l];
		l = (l + 1) % (n + 1);
		u[tmp] = false;
		for(i = point[tmp]; i != -1; i = next1[i]) {
			if(e[i].c && dis[e[i].t] > dis[tmp] + e[i].w) {
				dis[e[i].t] = dis[tmp] + e[i].w;
				pre[e[i].t] = i;
				if(!u[e[i].t]) {
					u[e[i].t] = true;
					q[r] = e[i].t;
					r = (r + 1) % (n + 1);
				}
			}
		}
	}
	if(pre[t] == -1)
		return false;
	return true;
}
void MCMF(int s, int t, int n, int &flow, int &cost){//起点s，终点t，点数n，最大流flow，最小花费cost
	int tmp, arg;
	flow = cost = 0;
	while(spfa(s, t, n)) {
		arg = inf, tmp = t;
		while(tmp != s) {
			arg = min(arg, e[pre[tmp]].c);
			tmp = e[pre[tmp]].f;
		}
		tmp = t;
		while(tmp != s) {
			e[pre[tmp]].c -= arg;
			e[pre[tmp] ^ 1].c += arg;
			tmp = e[pre[tmp]].f;
		}
		flow += arg;
		cost += arg * dis[t];
	}
}
//建图前运行init()
//节点下标从0开始
//加边时运行add_edge(a,b,c,0,d)表示加一条a到b的流量为c花费为d的边（注意花费为单位流量花费）
//特别注意双向边，运行add_edge(a,b,c,0,d),add_edge(b,a,c,0,d)较好，不要只运行一次add_edge(a,b,c,c,d),费用会不对。
//求解时代入MCMF(s,t,n,v1,v2)，表示起点为s，终点为t，点数为n的图中，最大流为v1，最大花费为v2