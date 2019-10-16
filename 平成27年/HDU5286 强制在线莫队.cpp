//Ԥ�ȴ�����������ͷβ�Ĵ𰸣�Ȼ��Ѳ���ֵ������뵽����ȥ
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long LL;
const LL MODN = 1000000007;
const int MAXN = 50005;
const int BLKN = 233;

int oriarr[MAXN];	//ԭʼ��������
vector<LL> pval[MAXN];	//Ԥ����ÿ��ֵ���ݴ�
int N, Q, bsiz, bcnt;
int G[BLKN][MAXN];	//Ԥ����Ī��ֵ[�����][�յ����λ��]
int gcnt[MAXN];		//ÿ�����ݵĸ�����ȫ�ּ�����
int oricnt[MAXN];

struct BLOCK{
	int val[MAXN];	//���ڵ�Ԫ��ֵ
	int cnt[MAXN];	//����ÿ��ֵ���ۼƸ���
	int len;		//��ĳ���
}block[BLKN];

//��ɢ��
void Distinct(){
	int i, vcnt, b, p;
	static int val[MAXN];

	bsiz = ceil(sqrt(N));
	memset(block, 0, sizeof(block));
	for(i = 0; i < N; i ++){
		val[i] = oriarr[i];
		pval[i].clear();
		pval[i].push_back(1);
	}
	sort(val, val + N);
	vcnt = unique(val, val + N) - val;
	for(i = 0, b = 0; i < N; b ++){
		if(b > 0){
			for(p = 0; p < vcnt; p ++)
				block[b].cnt[p] = block[b - 1].cnt[p];
		}
		for(p = 0; p < bsiz && i < N; p ++, i ++){
			block[b].val[p] = lower_bound(val, val + vcnt, oriarr[i]) - val;
			LL nv = pval[block[b].val[p]].back() * oriarr[i] % MODN;
			pval[block[b].val[p]].push_back(nv);

			block[b].cnt[block[b].val[p]] ++;
		}
		block[b].len = p;
	}
	bcnt = b;
	for(p = 0; p < vcnt; p ++)
		block[b].cnt[p] = block[b - 1].cnt[p];
	for(i = 0; i < N; i ++)
		pval[i][0] = 0;	//������0ʱ��0����1
}

//Ԥ����G
void PrepareG(){
	int stb, edb, p, v, i;
	LL ans;

	for(stb = 0; stb < bcnt; stb ++){
		ans = 0;
		i = stb * bsiz;
		for(edb = stb; edb < bcnt; edb ++){
			for(p = 0; p < block[edb].len; p ++, i ++){
				v = block[edb].val[p];
				ans = ans - pval[v][gcnt[v]];
				++ gcnt[v];
				ans = ans + pval[v][gcnt[v]];
				ans = (ans % MODN + MODN) % MODN;
				G[stb][i] = (int)ans;
			}
		}
		memset(gcnt, 0, sizeof(gcnt));
	}
}

LL Query(int l, int r){
	const int b = l / bsiz, edp = l % bsiz;
	const int b2 = (r + 1) / bsiz, stp2 = (r + 1) % bsiz;
	int p, v;
	LL ans = G[b][r];

	//ͳ�ƶ������Ԫ�ظ����������ǿ��ڵĸ���
	for(p = 0; p < edp; p ++){
		v = block[b].val[p];
		gcnt[v] ++;
		oricnt[v] = block[b2].cnt[v];
		if(b > 0)
			oricnt[v] -= block[b - 1].cnt[v];
	}
	//����������
	for(p = stp2; p < block[b2].len; p ++){
		v = block[b2].val[p];
		oricnt[v] --;
	}
	//���ݶ������Ԫ�ظ���������
	for(p = 0; p < edp; p ++){
		v = block[b].val[p];
		if(gcnt[v] == 0)
			continue;	//�Ѿ������
		ans = ans - pval[v][oricnt[v]] + pval[v][oricnt[v] - gcnt[v]];
		ans = (ans % MODN + MODN) % MODN;
		gcnt[v] = 0;
	}

	return ans;
}

int ReadInt(){
	int ret = 0;
	char c;
	while(!isdigit(c = getchar()));
	ret = c - '0';
	while(isdigit(c = getchar())){
		ret = ret * 10 + (c - '0');
	}
	return ret; 
}

LL lastans;
int T;
int main(){
	int i, l, r;
	T = ReadInt();
	while(T --){
		N = ReadInt(), Q = ReadInt();
		for(i = 0; i < N; i ++)
			oriarr[i] = ReadInt();
		Distinct();
		PrepareG();
		lastans = 0;
		while(Q --){
			l = ReadInt(), r = ReadInt();
			l = (l ^ lastans) % N;
			r = (r ^ lastans) % N;
			if(r < l)
				swap(l, r);
			lastans = Query(l, r);
			printf("%I64d\n", lastans);
		}
	}
	return 0;
}