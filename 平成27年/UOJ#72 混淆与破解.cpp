#include <cstdio>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <random>
using namespace std;

const int MAXN = 64; //�����ĸ���
const int MAXM = 4;  //������ĸ���
const int MAXL = 256;
const int MAXQ = 1024;
typedef bitset<MAXN> set;
typedef unsigned long ul;
typedef unsigned long long ull;
const int PREN = 50000;	//Ԥ������������
const int MAXB = MAXM;	//�������������
const int TEST = 10;	//������Ը���

int N, M, Q, L;

class calcor{
public:
	struct OP{
		int u, v, s, d, e;
	}op[MAXQ];
	default_random_engine re;
	uniform_int_distribution<int> ran;
	calcor():re(73),ran(0,1){};

	//�������ж������
	inline void read(){
		for(int i = 0; i < Q; i ++){
			scanf("%d%d%d%d%d", &op[i].u, &op[i].v, &op[i].s, &op[i].d, &op[i].e);
		}
	}
	//ִ�в���
	void run(const set in[], int out[], const int n) const{
		int i, j, k;
		static ul y[MAXL];
		//һ��32��һ��
		for(i = 0; i < n; i += 32){
			fill(y, y + L, 0);
			for(k = 0; k < N; k ++)
				for(j = 0; j < 32 && i + j < n; j ++)
					y[k] |= in[i + j][k] << j;
			for(j = 0; j < Q; j ++)
				y[op[j].u] = (~(y[op[j].v] & y[op[j].s])) ^ y[op[j].d] ^ y[op[j].e];
			for(j = 0; j < 32 && i + j < n; j ++)
				out[i + j] = (y[0] >> j) & 1;
		}
	}
	//�������
	inline set randvec(){
		ull ret = 0;
		for(int i = 0; i < N; i ++)
			ret |= ran(re) << i;
		return set(ret);
	}
}calc;

class frequency{
public:
	set testcase[PREN];
	int testans[PREN];

	set f[MAXM];	//z�ĸ�������
	int nfreq;
	int h[1<<MAXM];//h�ı�

	//�������������𰸣���Ϊ����ϵ���ʹ�H��Ļ���
	inline void prepare(){
		for(int i = 0; i < PREN; i ++)
			testcase[i] = calc.randvec();
		calc.run(testcase, testans, PREN);
	}
	//����Fourier Coefficients
	double getcoe(const set z){
		int ret = 0;
		for(int i = 0; i < PREN; i ++){
			if(((z & testcase[i]).count() & 1) == testans[i])
				ret ++;
		}
		return ret * 2.0 / PREN - 1.0;
	}
	//�Ƿ��������
	bool linerel(const set v[], const int m){
		ull tmp[MAXB];
		int i, j;
		for(i = 0; i < m; i ++)
			tmp[i] = v[i].to_ullong();
		for(i = 0; i < m; i ++){
			for(j = 0; j < i; j ++)
				if(tmp[i] & (tmp[j] & -tmp[j]))
					tmp[i] ^= tmp[j];
			if(tmp[i] == 0)
				return true;
		}
		return false;
	}
	//�����µ�freq
	inline void insert(const set nf){
		f[nfreq ++] = nf;
		if(linerel(f, nfreq))
			nfreq --;
	}
	//ͨ�����ó�H
	inline void makeH(){
		static int tab[1<<MAXM][2];
		int i, j, msk;
		for(i = 0; i < PREN; i ++){
			for(msk = j = 0; j < M; j ++, msk <<= 1){
				msk |= ((testcase[i] & f[j]).count() & 1);
			}
			msk >>= 1;
			tab[msk][testans[i]] ++;
		}
		//����ͬ���������������͸��ʸߵ��Ǵ�
		for(msk = 0; msk < (1 << M); msk ++){
			h[msk] = tab[msk][1] > tab[msk][0] ? 1 : 0;
		}
	}

	//�����
	void print(){
		int i, j;
		for(i = 0; i < M; i ++){
			for(j = 0; j < N; j ++){
				int tmp = f[i][j];
				printf("%d", tmp);
			}
			printf("\n");
		}
		for(i = 0; i < (1 << M); i ++){
			printf("%d", h[i]);
		}
		printf("\n");
	}
}freq;

class engine{
public:
	set coll[MAXN * TEST * 2 * (1 << MAXB)];
	int res[MAXN * TEST * 2 * (1 << MAXB)];
	int ntest, nb;	//������������������

	//�������������
	void buildTest(){
		ntest = 0;
		nb = min(N, MAXB);	//����Խ��������Խ��

		//����������
		set base[MAXB];
		for(int bcnt = 0; bcnt < nb; ){
			base[bcnt ++] = calc.randvec();
			if(freq.linerel(base, bcnt))
				bcnt --;
		}

		//ȡ����λ���0ȡ��N
		for(int pnt = 0; pnt < N; pnt ++){
			//ÿ�����TEST������
			for(int __t = 0; __t < TEST; __t ++){
				set x0 = calc.randvec();
				//ÿ����������״̬
				for(int st = 0; st <= 1; st ++){
					x0[pnt] = st ? true : false;
					//ÿ������������в�ͬ��������
					for(int bs = 0; bs < (1 << nb); bs ++){
						set x = x0;
						for(int l = 0; l < nb; l ++)
							if(bs & (1 << l))
								x ^= base[l];
						coll[ntest ++] = x;
					}//bs
				}//st
			}//__t
		}//pnt
	};

	//ͨ�����Խ�����Բ���freq
	//bst : ���������Ĵ���
	set seperate(int bst){
		set ret(0);
		ntest = 0;

		//��ÿһ��λ�����ͳ��
		for(int pnt = 0; pnt < N; pnt ++){
			int diff = 0;	//����pntλ�������������ж��ٸ�
			//ÿ����ͳ�����е�����
			for(int __t = 0; __t < TEST; __t ++){
				int observ[2] = {0, 0};	//������
				//��ÿһ������������������
				for(int st = 0; st <= 1; st ++){
					//���������������2^b���ӽ�״̬�����ǰ�bst���мӼ�����
					//Ȼ��õ���
					for(int bs = 0; bs < (1 << nb); bs ++){
						observ[st] += res[ntest] == (set(bs & bst).count() & 1) ? 1 : -1;
						ntest ++;
					}
				}
				if((observ[0] > 0) != (observ[1] > 0))
					diff ++;
			}
			//��ת�����㹻�ߣ�������Ϊ����
			ret[pnt] = diff > TEST / 2;
		}

		return ret;
	}

	void run(){
		bool end = false;
		const double alpha = 1.0 / (1 << M);
		while(!end){
			buildTest();
			calc.run(coll, res, ntest);
			for(int bst = 0; !end && bst < (1 << nb); bst ++){
				set f = seperate(bst);
				if(abs(freq.getcoe(f)) > alpha * 0.9){
					freq.insert(f);
					end = freq.nfreq == M;
				}
			}
		}
	}
}eng;

int main(){
	scanf("%d%d%d%d", &N, &M, &L, &Q);
	calc.read();
	freq.prepare();
	eng.run();
	freq.makeH();
	freq.print();
	return 0;
}
