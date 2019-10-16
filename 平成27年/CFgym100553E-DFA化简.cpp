#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <algorithm>

#define LASTP(x) (1 << (x & 0x07))

#pragma pack(1)
typedef unsigned char byte;
struct TEST{
	byte st[14];	//表示对手状态的位图
	int fin;		//如果终结了，归根于某一个终结状态
};
struct STATE{
	struct{
		char op;
		char unused[4];
	};
	int go[3];	//R, P, S
};
STATE cm[105], om[105];	//输入，必胜状态
int fins[105];			//对应的最终状态的编号
int n;
STATE tm[50005];
int tcnt;
TEST test[50005];
int sums[50005];

//未简化的输入
STATE km[105];
//特别注意：化简自动机时，不能让同一轮中后来的访问到修改后的划分集，不然会产生错误。
//每次划分时，只要是不同的都划分到一起，等下一轮再细分。
int trans[105], trans2[105];
int k;
int split[105];

//简化自动机
void input(){
	int i, p;
	bool flag;
	int temp[256];
	int cur = 0;

	scanf("%d", &k);
	n = 0;
	temp['R'] = temp['P'] = temp['S'] = 0;
	for(i = 1; i <= k; i ++){
		scanf("%s%d%d%d", &km[i].op, &km[i].go[0], &km[i].go[1], &km[i].go[2]);
		if(temp[km[i].op] == 0){
			temp[km[i].op] = ++ n;
			cm[n].op = km[i].op;
		}
		trans[i] = temp[km[i].op];
	}

	flag = true;
	while(flag){
		flag = false;
		//重新计算转移
		for(i = 1; i <= n; i ++){
			cm[i].go[0] = 0;
		}
		for(i = 1; i <= k; i ++){
			if(cm[trans[i]].go[0] == 0){
				cm[trans[i]].go[0] = trans[km[i].go[0]];
				cm[trans[i]].go[1] = trans[km[i].go[1]];
				cm[trans[i]].go[2] = trans[km[i].go[2]];
			}
		}
		//进行一轮划分
		memset(split, 0, sizeof(split));
		memcpy(trans2, trans, sizeof(trans));
		for(i = 1; i <= k; i ++){
			//状态异常，需要分割
			if((cm[trans[i]].go[0] != trans[km[i].go[0]])
				|| (cm[trans[i]].go[1] != trans[km[i].go[1]])
				|| (cm[trans[i]].go[2] != trans[km[i].go[2]]))
			{
				if(split[trans[i]] == 0){
					split[trans[i]] = ++ n;
					cm[n].op = km[i].op;
					cm[n].go[0] = 0;
					cm[n].go[1] = 0;
					cm[n].go[2] = 0;
				}
				trans2[i] = split[trans[i]];
				flag = true;
				continue;
			}
		}
		memcpy(trans, trans2, sizeof(trans2));
	}
}

void init(){
	int i;
	int thr;
	for(i = 1; i <= n; i ++){
		switch(cm[i].op){
		case 'R':
			om[i].op = 'P';
			thr = 1;
			break;
		case 'P':
			om[i].op = 'S';
			thr = 2;
			break;
		case 'S':
			om[i].op = 'R';
			thr = 0;
			break;
		}
		om[i].go[0] = cm[i].go[thr];
		om[i].go[1] = cm[i].go[thr];
		om[i].go[2] = cm[i].go[thr];
	}
}

void buildTest(int index){
	int i, cnt, fst;
	int thr = rand() % 3;	//随机出一个
	int got;
	byte tmp;

	//判断当前状态是否终结
	cnt = 0;
	for(i = 1; i <= n; i ++){
		if(test[index].st[i>>3] & LASTP(i)){
			++ cnt;
			fst = i;
		}
	}
	if(cnt == 0){
		//既然不可能出现，那么就随便转移了
		test[index].fin = 1;
		return ;
	}
	if(cnt == 1){
		//只能出现某一特定状态，说明现在应该处于om对应状态
		test[index].fin = fst;
		return ;
	}
	test[index].fin = 0;
	switch(thr){
	case 0:
		tm[index].op = 'R';
		break;
	case 1:
		tm[index].op = 'P';
		break;
	case 2:
		tm[index].op = 'S';
		break;
	}
	for(i = 1; i <= n; i ++){
		if(test[index].st[i>>3] & LASTP(i)){
			//判断行为
			switch(cm[i].op){
			case 'R':
				got = 1;
				break;
			case 'P':
				got = 2;
				break;
			case 'S':
				got = 3;
				break;
			}
			tmp = test[tcnt + got].st[cm[i].go[thr] >> 3];
			tmp = (tmp | LASTP(cm[i].go[thr]));
			test[tcnt + got].st[cm[i].go[thr] >> 3] = tmp;
		}
	}
	tm[index].go[0] = tcnt + 1;
	tm[index].go[1] = tcnt + 2;
	tm[index].go[2] = tcnt + 3;
	tcnt += 3;
	if(tcnt > 30000){
		tcnt = tcnt;
	}
}

void build(){
	int i;
	tcnt = 1;
	for(i = 1; i <= n; i ++){
		test[1].st[i>>3] = (test[1].st[i>>3] | LASTP(i));
	}
	for(i = 1; i <= tcnt; i ++){
		buildTest(i);
	}
}

void print(){
	int i, j;
	int cnt = tcnt;
	sums[0] = 0;
	for(i = 1; i <= tcnt; i ++){
		sums[i] = sums[i - 1];
		if(test[i].fin > 0){
			cnt --;
			sums[i] ++;
		}
	}
	printf("%d\n", cnt + n);
	for(i = 1; i <= tcnt; i ++){
		if(test[i].fin > 0){
			continue;
		}
		printf("%c", tm[i].op);
		for(j = 0; j < 3; j ++){
			if(test[tm[i].go[j]].fin == 0){
				printf(" %d", tm[i].go[j] - sums[tm[i].go[j]]);
			}else{
				printf(" %d", test[tm[i].go[j]].fin + cnt);
			}
		}
		printf("\n");
	}
	for(i = 1; i <= n; i ++){
		printf("%c %d %d %d\n", om[i].op, om[i].go[0] + cnt, om[i].go[1] + cnt, om[i].go[2] + cnt);
	}
}

int main(){
	//freopen
	srand(73);
	input();
	init();
	build();
	print();
	return 0;
}
