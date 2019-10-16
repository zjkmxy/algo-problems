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
int ccnt, fcnt, N; //Char个数，Face个数，N
int baseM;  //不得不选的人的Magic的和
int baseF;  //不得不选的人的Face的掩码
int pos;    //中间的分割线，前半部分是face排序的，后半部分是face不会冲突的按char排序的
int pos2;   //后半部分的一半的分界线
DWORD maskPre, maskForm, maskLatt;  //前半部分的性格的掩码，后半部分两半各自的性格的掩码
int comC;   //公共的char，在枚举两半的时候需要额外考虑
bool noans; //是否无解
DWORD ans;

void assert(bool cond){
	if(!cond)
		*(int*)(0) = 0;
}

//按Char排序
int byChar(const void* A, const void* B){
	return ((P*)(A))->C - ((P*)(B))->C;
}

//按Face排序
int byFace(const void* A, const void* B){
	return ((P*)(B))->F - ((P*)(A))->F;
}

//取得掩码
inline DWORD mask(int x){
	return (1 << x);
}

//对char进行重标号
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

//对face进行重标号
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

//把人群进行分类
void splitArr(){
	int i;
	DWORD tmpMask;
	baseM = baseF = 0;
	noans = false;
	//选出不得不选的人
	for(i = N - 1; i >= 0; i --){
		if(arr[i].C == -1){
			//如果是不得不选的人，强制选择
			if(arr[i].F >= 0){
				tmpMask = mask(arr[i].F);
				if(baseF & tmpMask){
					noans = true;
					return;
				}
				baseF = (baseF | tmpMask);
			}
			baseM = (baseM ^ arr[i].M);
			//剔出队列
			swap(arr[i], arr[N - 1]);
			N --;
		}
	}
	//删除根本不能选的人
	for(i = N - 1; i >= 0; i --){
		assert(arr[i].C >= 0);
		if(arr[i].F >= 0){
			tmpMask = mask(arr[i].F);
			if(baseF & tmpMask){
				//如果是根本不能选的人
				//剔出队列
				swap(arr[i], arr[N - 1]);
				N --;
			}
		}
	}
	//排序和分类
	qsort(arr, N, sizeof(P), byFace);
	for(pos = N - 1; pos >= 0; pos --){
		if(arr[pos].F != -1){
			break;
		}
	}
	pos ++;
	//如果就不存在孤立的face，那么后面就不用管了
	if(pos < N){
		qsort(arr + pos, N - pos, sizeof(P), byChar);
		pos2 = pos + ((N - pos) >> 1) - 1;
		if(arr[pos2].C == arr[pos2 + 1].C){
			comC = arr[pos2].C;
		}else{
			comC = -1;
		}
		//计算三个掩码
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
		//此时maskPre是在前半部分的枚举中不得不选的char
	}else{
		maskPre = (1 << ccnt) - 1;
	}
}

//暴力枚举阶段
namespace BF{
	map<DWORD, DWORD> incl;  //后半段包含comC的特定异或值的个数
	map<DWORD, DWORD> excl;  //后半段不含comC的特定异或值的个数

	//枚举后一部分后半段
	void BFLatter(int i, int C, int M){
		if(i == pos2){
			//枚举终止
			if((C & maskLatt) != maskLatt){
				//该选的Char有漏选的
				return ;
			}
			if(comC >= 0 && (C & mask(comC)) == 0){
				excl[M] ++;
			}else{
				incl[M] ++;
			}
		}else{
			//继续枚举
			BFLatter(i - 1, C, M);  //不选的
			BFLatter(i - 1, C | mask(arr[i].C), M ^ arr[i].M);//选的
		}
	}

	//枚举后一部分前半段
	void BFFormer(int i, int C, int M){
		if(i > pos2){
			map<DWORD, DWORD>::iterator it;
			//枚举终止
			if((C & maskForm) != maskForm){
				//该选的Char有漏选的
				return ;
			}
			//后半段选上了comC，前半段不管怎么搞都能要
			it = incl.find(M);
			if(it != incl.end())
				ans += it->second;
			//后半段没选上comC，前半段必须有comC才能要
			it = excl.find(M);
			if((C & mask(comC)) > 0 && it != excl.end())
				ans += it->second;
		}else{
			//继续枚举
			BFFormer(i + 1, C, M);  //不选的
			BFFormer(i + 1, C | mask(arr[i].C), M ^ arr[i].M);//选的
		}
	}

	//暴力枚举前一部分
	void BFPre(int i, int C, int M, int F){
		if(i >= pos){
			//枚举终止
			if((C & maskPre) != maskPre)
				return ;
			if(pos == N){
				//没有后一部分，不用枚举
				if(M == 0)
					ans ++;
			}else{
				incl.clear();
				excl.clear();
				BFLatter(N - 1, C, 0); //先后半段，不需要计算当前magic
				BFFormer(pos, C, M);   //再前半段，需要计算当前magic
				//前半段计算过程中自动更新答案
			}
		}else{
			//继续枚举
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