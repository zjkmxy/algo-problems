const int N = 1010;//��
const int M = 2 * 10010;//��
const int inf = 1000000000;
struct Node{//�ߣ���f����t������Ϊc������Ϊw
	int f, t, c, w;
}e[M];
int next1[M], point[N], dis[N], q[N], pre[N], ne;//neΪ����ӵı�����next��pointΪ�ڽӱ�,disΪ���ѣ�preΪ���׽ڵ�
bool u[N];
void init(){
	memset(point, -1, sizeof(point));
	ne = 0;
}
void add_edge(int f, int t, int d1, int d2, int w){//f��t��һ���ߣ�����Ϊd1,��������d2,����w,����߻���-w�����Է��ڣ�
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
void MCMF(int s, int t, int n, int &flow, int &cost){//���s���յ�t������n�������flow����С����cost
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
//��ͼǰ����init()
//�ڵ��±��0��ʼ
//�ӱ�ʱ����add_edge(a,b,c,0,d)��ʾ��һ��a��b������Ϊc����Ϊd�ıߣ�ע�⻨��Ϊ��λ�������ѣ�
//�ر�ע��˫��ߣ�����add_edge(a,b,c,0,d),add_edge(b,a,c,0,d)�Ϻã���Ҫֻ����һ��add_edge(a,b,c,c,d),���û᲻�ԡ�
//���ʱ����MCMF(s,t,n,v1,v2)����ʾ���Ϊs���յ�Ϊt������Ϊn��ͼ�У������Ϊv1����󻨷�Ϊv2