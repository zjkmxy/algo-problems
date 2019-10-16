const int maxnode = 1000 + 5;
const int maxedge = 1000 + 5;
const int oo = 1000000000;
int node, src, dest, nedge;
int head[maxnode], point[maxedge], next1[maxedge], flow[maxedge], capa[maxedge];//point[x]==y��ʾ��x��������y��head��nextΪ�ڽӱ�flow[x]��ʾx�ߵĶ�ֵ̬��capa[x]��ʾx�ߵĳ�ʼֵ
int dist[maxnode], Q[maxnode], work[maxnode];//dist[i]��ʾi��ĵȼ�
void init(int _node, int _src, int _dest){//��ʼ����node��ʾ��ĸ�����src��ʾ��㣬dest��ʾ�յ�
    node = _node;
    src = _src;
    dest = _dest;
    for (int i = 0; i < node; i++) head[i] = -1;
    nedge = 0;
}
void addedge(int u, int v, int c1, int c2){//����һ��u��v����Ϊc1��v��u����Ϊc2��������
    point[nedge] = v, capa[nedge] = c1, flow[nedge] = 0, next1[nedge] = head[u], head[u] = (nedge++);
    point[nedge] = u, capa[nedge] = c2, flow[nedge] = 0, next1[nedge] = head[v], head[v] = (nedge++);
}
bool dinic_bfs(){
    memset(dist, 255, sizeof (dist));
    dist[src] = 0;
    int sizeQ = 0;
    Q[sizeQ++] = src;
    for (int cl = 0; cl < sizeQ; cl++)
        for (int k = Q[cl], i = head[k]; i >= 0; i = next1[i])
            if (flow[i] < capa[i] && dist[point[i]] < 0){
                dist[point[i]] = dist[k] + 1;
                Q[sizeQ++] = point[i];
            }
    return dist[dest] >= 0;
}
int dinic_dfs(int x, int exp){
    if (x == dest) return exp;
    for (int &i = work[x]; i >= 0; i = next1[i]){
        int v = point[i], tmp;
        if (flow[i] < capa[i] && dist[v] == dist[x] + 1 && (tmp = dinic_dfs(v, min(exp, capa[i] - flow[i]))) > 0){
            flow[i] += tmp;
            flow[i^1] -= tmp;
            return tmp;
        }
    }
    return 0;
}
int dinic_flow(){
    int result = 0;
    while (dinic_bfs()){
        for (int i = 0; i < node; i++) work[i] = head[i];
        while (1){
            int delta = dinic_dfs(src, oo);
            if (delta == 0) break;
            result += delta;
        }
    }
    return result;
}
//��ͼǰ,����һ��init()��
//�ӱ�ʱ������addedge(a,b,c,0),��ʾ��a��b����Ϊc�ı߽��ɣ�ע������Ҫ��0��ʼ��
//������������dinic_flow(),����ֵ��Ϊ��