/*
N个人各自有一个数，对所有连续K个人的区间，其中第一大最右边者拿A奖金，第二小（重复不选）最左边者拿B，求获奖人数和方差。要求O(n)
首先，最大单调栈直接解决最右边的第一大
考虑最小单调栈，其中每次有新的元素入栈的时候，将退栈的元素（相等的不算）推到一个新的单调栈里（退役栈）
那么显见最右边的第二小就是最小栈的第二个元素和退役栈的第一个元素的较小者
倒着跑一遍就可以得出最左边的第二小了
如果有强迫症必须从左往右扫描的，我们有O(n)离散化……
*/
#include <cstdio>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 1048576;
int arr[MAXN];
int priz[MAXN];
int N, K, A, B;
//单调栈（其实是队列）
class Stack{
public:
	int qmin[MAXN], qmax[MAXN], q2[MAXN]; //最小栈，最大栈，退役栈
	int hmin, tmin, hmax, tmax, h2, t2;
	void Clear(){
		hmin = tmin = 0;
		hmax = tmax = 0;
		h2 = t2 = 0;
	}
	//得到最大值
	int GetMax(){
		if(hmax == tmax)
			return -1;
		else
			return arr[qmax[hmax]];
	}
	//删除值
	void Pop(int l){
		while(hmax < tmax && qmax[hmax] <= l)
			++ hmax;
		while(hmin < tmin && qmin[hmin] <= l)
			++ hmin;
		while(h2 < t2 && q2[h2] <= l)
			++ h2;
	};
	//从最小值栈里被取出的，送入退役栈
	void Push2(int p){
		while(h2 < t2 && arr[q2[t2-1]] >= arr[p])
			-- t2;
		q2[t2 ++] = p;
	}
	//加入新的值
	void Push(int p){
		int tmp, i;
		//更新最大栈
		while(hmax < tmax && arr[qmax[tmax-1]] <= arr[p])
			-- tmax;
		qmax[tmax ++] = p;
		//更新最小栈。注意这里先更新小于的
		tmp = tmin;
		while(hmin < tmin && arr[qmin[tmin-1]] > arr[p]){
			-- tmin;
		}
		//按顺序退役
		for(i = tmin; i < tmp; i ++){
			Push2(qmin[i]);
		}
		//等值的更新时不退役
		while(hmin < tmin && arr[qmin[tmin-1]] >= arr[p]){
			-- tmin;
		}
		qmin[tmin ++] = p;
	}
	//取得次小值，应该是退役栈的第一个或者最小栈的第二个
	//两个取较小的就可以了
	int GetSnd(){
		int val = -1;
		if(hmin == tmin)
			return -1;
		if(hmin <= tmin - 2)
			val = arr[qmin[hmin + 1]];
		if(h2 < t2){
			if(val == -1 || arr[q2[h2]] < val)
				val = arr[q2[h2]];
		}
		if(val == arr[qmin[hmin]])  //其实没必要
			return -1;
		return val;
	}
	void Show(){  //调试用，显示栈内元素
		int i;
		printf("Max: ");
		for(i = hmax; i < tmax; i ++)
			printf("%d ", qmax[i]);
		printf("\n");
		printf("Min: ");
		for(i = hmin; i < tmin; i ++)
			printf("%d ", qmin[i]);
		printf("\n");
		printf("Bak: ");
		for(i = h2; i < t2; i ++)
			printf("%d ", q2[i]);
		printf("\n");
	}
}stk;
//值域链表，给定一个离散化后的值，查找它的最左端或者最右端的出现
class List{
public:
	int head[MAXN], nxt[MAXN], tail[MAXN];
	int cnt[MAXN];
	void Clear(){
		for(int i = 0; i < N; i ++){
			head[i] = tail[i] = -1;
			cnt[i] = 0;
		}
	}
	void Push(int i){
		nxt[i] = -1;
		if(tail[arr[i]] == -1){
			head[arr[i]] = tail[arr[i]] = i;
		}else{
			nxt[tail[arr[i]]] = i;
		}
		tail[arr[i]] = i;
		cnt[arr[i]] ++;
	}
	void Pop(int i){
		if(head[arr[i]] != i){
			throw 0;
		}
		head[arr[i]] = nxt[i];
		if(head[arr[i]] == -1)
			tail[arr[i]] = -1;
		cnt[arr[i]] --;
	}
}pos;
//O(n)离散化
void Distinct(){
	int i, n2, cur;
	int *cnt = pos.cnt, *tab1 = pos.head, *tab2 = pos.tail;
	int *arr2 = pos.nxt;
	//第一轮排序
	for(i = 0; i < N; i ++)
		cnt[arr[i]&0xFFFF] ++;
	for(i = 0; i < 0xFFFF; i ++) cnt[i + 1] += cnt[i];
	for(i = N - 1; i >= 0; i --){
		tab1[i] = -- cnt[arr[i]&0xFFFF];
		arr2[tab1[i]] = arr[i];
	}
	//第二轮排序
	for(i = 0; i <= 0xFFFF; i ++) cnt[i] = 0;
	for(i = 0; i < N; i ++)
		cnt[arr2[i]>>16] ++;
	for(i = 0; i < 0xFFFF; i ++) cnt[i + 1] += cnt[i];
	for(i = N - 1; i >= 0; i --){
		tab2[i] = -- cnt[arr2[i]>>16];
		arr[tab2[i]] = arr2[i];
	}
	//值离散化
	cur = arr[0];
	n2 = 1;
	arr2[0] = 0;
	for(i = 1; i < N; i ++){
		if(arr[i] != cur){
			cur = arr[i];
			arr2[i] = n2 ++;
		}else{
			arr2[i] = n2 - 1;
		}
	}
	//复原数组
	for(i = 0; i < N; i ++)
		arr[i] = arr2[tab2[tab1[i]]];
}
void Input(){
	int i;
	scanf("%d%d%d%d", &N, &K, &A, &B);
	for(i = 0; i < N; i ++){
		scanf("%d", &arr[i]);
	}
}
void Deal(){
	int i, m, s;
	int cnt = 0;
	double ave = 0., var = 0.;
	stk.Clear();
	pos.Clear();
	fill(priz, priz + N, 0);
	//把最开头的几个入栈
	for(i = 0; i < K - 1; i ++){
		pos.Push(i);
		stk.Push(i);
	}
	//每入栈一个，查询一次，再出栈一个
	for(i = K - 1; i < N; i ++){
		pos.Push(i);
		stk.Push(i);
		m = stk.GetMax();
		s = stk.GetSnd();
		m = pos.tail[m];
		priz[m] += A;
		if(s >= 0){
			s = pos.head[s];
			priz[s] += B;
		}
		pos.Pop(i - K + 1);
		stk.Pop(i - K + 1);
	}
	//统计奖金平均值并算出方差
	for(i = 0; i < N; i ++){
		if(priz[i] > 0){
			ave += priz[i];
			cnt ++;
		}
	}
	ave /= cnt;
	for(i = 0; i < N; i ++){
		if(priz[i] > 0)
			var += (priz[i] - ave) * (priz[i] - ave);
	}
	var /= cnt;
	var = var / (N - K + 1) / (N - K + 1);
	printf("%d %.4lf\n", cnt, var);
}
int main(){
	Input();
	Distinct();
	Deal();
	return 0;
}