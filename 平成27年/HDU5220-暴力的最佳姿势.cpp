#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

const int MAXN = 35;
typedef unsigned long DWORD;
struct P{
	int C, F, M;
}arr[MAXN];
int ccnt, fcnt, N; //Char������Face������N
int baseM;  //���ò�ѡ���˵�Magic�ĺ�
int baseF;  //���ò�ѡ���˵�Face������
int pos;    //�м�ķָ��ߣ�ǰ�벿����face����ģ���벿����face�����ͻ�İ�char�����
int pos2;   //��벿�ֵ�һ��ķֽ���
DWORD maskPre, maskForm, maskLatt;  //ǰ�벿�ֵ��Ը�����룬��벿��������Ե��Ը������
int comC;   //������char����ö�������ʱ����Ҫ���⿼��
bool noans; //�Ƿ��޽�
DWORD ans;

void assert(bool cond){
	if(!cond)
		*(int*)(0) = 0;
}

//��Char����
int byChar(const void* A, const void* B){
	return ((P*)(A))->C - ((P*)(B))->C;
}

//��Face����
int byFace(const void* A, const void* B){
	return ((P*)(B))->F - ((P*)(A))->F;
}

//ȡ������
inline DWORD mask(int x){
	return (1 << x);
}

//��char�����ر��
void relabChar(){
	int i;
	int chars[MAXN], sign[MAXN];
	ccnt = 0;
	memset(chars, 0, sizeof(chars));
	memset(sign, -1, sizeof(sign));
	for(i = 0; i < N; i ++){
		chars[arr[i].C]++;
	}
	for(i = 0; i < N; i ++){
		if(chars[arr[i].C] > 1){
			if(sign[arr[i].C] == -1){
				sign[arr[i].C] = ccnt;
				ccnt ++;
			}
			arr[i].C = sign[arr[i].C];
		}else{
			arr[i].C = -1;
		}
	}
}

//��face�����ر��
void relabFace(){
	int i;
	int faces[MAXN], sign[MAXN];
	fcnt = 0;
	memset(faces, 0, sizeof(faces));
	memset(sign, -1, sizeof(sign));
	for(i = 0; i < N; i ++){
		faces[arr[i].F]++;
	}
	for(i = 0; i < N; i ++){
		if(faces[arr[i].F] > 1){
			if(sign[arr[i].F] == -1){
				sign[arr[i].F] = fcnt;
				fcnt ++;
			}
			arr[i].F = sign[arr[i].F];
		}else{
			arr[i].F = -1;
		}
	}
}

//����Ⱥ���з���
void splitArr(){
	int i;
	DWORD tmpMask;
	baseM = baseF = 0;
	noans = false;
	//ѡ�����ò�ѡ����
	for(i = N - 1; i >= 0; i --){
		if(arr[i].C == -1){
			//����ǲ��ò�ѡ���ˣ�ǿ��ѡ��
			if(arr[i].F >= 0){
				tmpMask = mask(arr[i].F);
				if(baseF & tmpMask){
					noans = true;
					return;
				}
				baseF = (baseF | tmpMask);
			}
			baseM = (baseM ^ arr[i].M);
			//�޳�����
			swap(arr[i], arr[N - 1]);
			N --;
		}
	}
	//ɾ����������ѡ����
	for(i = N - 1; i >= 0; i --){
		assert(arr[i].C >= 0);
		if(arr[i].F >= 0){
			tmpMask = mask(arr[i].F);
			if(baseF & tmpMask){
				//����Ǹ�������ѡ����
				//�޳�����
				swap(arr[i], arr[N - 1]);
				N --;
			}
		}
	}
	//����ͷ���
	qsort(arr, N, sizeof(P), byFace);
	for(pos = N - 1; pos >= 0; pos --){
		if(arr[pos].F != -1){
			break;
		}
	}
	pos ++;
	//����Ͳ����ڹ�����face����ô����Ͳ��ù���
	if(pos < N){
		qsort(arr + pos, N - pos, sizeof(P), byChar);
		pos2 = pos + ((N - pos) >> 1) - 1;
		if(arr[pos2].C == arr[pos2 + 1].C){
			comC = arr[pos2].C;
		}else{
			comC = -1;
		}
		//������������
		maskForm = 0;
		for(i = pos; i <= pos2; i ++){
			if(arr[i].C != comC)
				maskForm = maskForm | mask(arr[i].C);
		}
		maskLatt = 0;
		for(i = pos2 + 1; i < N; i ++){
			if(arr[i].C != comC)
				maskLatt = maskLatt | mask(arr[i].C);
		}
		maskPre = (1 << ccnt) - 1;
		maskPre = maskPre ^ maskForm;
		maskPre = maskPre ^ maskLatt;
		if(comC >= 0)
			maskPre = maskPre ^ mask(comC);
		//��ʱmaskPre����ǰ�벿�ֵ�ö���в��ò�ѡ��char
	}else{
		maskPre = (1 << ccnt) - 1;
	}
}

//����ö�ٽ׶�
namespace BF{
	map<DWORD, DWORD> incl;  //���ΰ���comC���ض����ֵ�ĸ���
	map<DWORD, DWORD> excl;  //���β���comC���ض����ֵ�ĸ���

	//ö�ٺ�һ���ֺ���
	void BFLatter(int i, int C, int M){
		if(i == pos2){
			//ö����ֹ
			if((C & maskLatt) != maskLatt){
				//��ѡ��Char��©ѡ��
				return ;
			}
			if(comC >= 0 && (C & mask(comC)) == 0){
				excl[M] ++;
			}else{
				incl[M] ++;
			}
		}else{
			//����ö��
			BFLatter(i - 1, C, M);  //��ѡ��
			BFLatter(i - 1, C | mask(arr[i].C), M ^ arr[i].M);//ѡ��
		}
	}

	//ö�ٺ�һ����ǰ���
	void BFFormer(int i, int C, int M){
		if(i > pos2){
			map<DWORD, DWORD>::iterator it;
			//ö����ֹ
			if((C & maskForm) != maskForm){
				//��ѡ��Char��©ѡ��
				return ;
			}
			//����ѡ����comC��ǰ��β�����ô�㶼��Ҫ
			it = incl.find(M);
			if(it != incl.end())
				ans += it->second;
			//����ûѡ��comC��ǰ��α�����comC����Ҫ
			it = excl.find(M);
			if((C & mask(comC)) > 0 && it != excl.end())
				ans += it->second;
		}else{
			//����ö��
			BFFormer(i + 1, C, M);  //��ѡ��
			BFFormer(i + 1, C | mask(arr[i].C), M ^ arr[i].M);//ѡ��
		}
	}

	//����ö��ǰһ����
	void BFPre(int i, int C, int M, int F){
		if(i >= pos){
			//ö����ֹ
			if((C & maskPre) != maskPre)
				return ;
			if(pos == N){
				//û�к�һ���֣�����ö��
				if(M == 0)
					ans ++;
			}else{
				incl.clear();
				excl.clear();
				BFLatter(N - 1, C, 0); //�Ⱥ��Σ�����Ҫ���㵱ǰmagic
				BFFormer(pos, C, M);   //��ǰ��Σ���Ҫ���㵱ǰmagic
				//ǰ��μ���������Զ����´�
			}
		}else{
			//����ö��
			if((F & mask(arr[i].F)) == 0){
				BFPre(i + 1, C | mask(arr[i].C), M ^ arr[i].M, F | mask(arr[i].F));
			}
			BFPre(i + 1, C, M, F);
		}
	}

	void Run(){
		ans = 0;
		if(noans)
			return ;
		BFPre(0, 0, baseM, baseF);
	}
};

void input(){
	int i;
	scanf("%d", &N);
	for(i = 0; i < N; i ++){
		scanf("%d%d%d", &arr[i].F, &arr[i].C, &arr[i].M);
	}
}

void domain(){
	input();
	relabChar();
	relabFace();
	splitArr();
	BF::Run();
	printf("%lu\n", ans);
}

int main(){
	int T;
	scanf("%d", &T);
	while(T--){
		domain();
	}
	return 0;
}