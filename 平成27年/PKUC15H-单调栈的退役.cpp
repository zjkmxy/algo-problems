/*
N���˸�����һ����������������K���˵����䣬���е�һ�����ұ�����A���𣬵ڶ�С���ظ���ѡ�����������B����������ͷ��Ҫ��O(n)
���ȣ���󵥵�ջֱ�ӽ�����ұߵĵ�һ��
������С����ջ������ÿ�����µ�Ԫ����ջ��ʱ�򣬽���ջ��Ԫ�أ���ȵĲ��㣩�Ƶ�һ���µĵ���ջ�����ջ��
��ô�Լ����ұߵĵڶ�С������Сջ�ĵڶ���Ԫ�غ�����ջ�ĵ�һ��Ԫ�صĽ�С��
������һ��Ϳ��Եó�����ߵĵڶ�С��
�����ǿ��֢�����������ɨ��ģ�������O(n)��ɢ������
*/
#include <cstdio>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 1048576;
int arr[MAXN];
int priz[MAXN];
int N, K, A, B;
//����ջ����ʵ�Ƕ��У�
class Stack{
public:
	int qmin[MAXN], qmax[MAXN], q2[MAXN]; //��Сջ�����ջ������ջ
	int hmin, tmin, hmax, tmax, h2, t2;
	void Clear(){
		hmin = tmin = 0;
		hmax = tmax = 0;
		h2 = t2 = 0;
	}
	//�õ����ֵ
	int GetMax(){
		if(hmax == tmax)
			return -1;
		else
			return arr[qmax[hmax]];
	}
	//ɾ��ֵ
	void Pop(int l){
		while(hmax < tmax && qmax[hmax] <= l)
			++ hmax;
		while(hmin < tmin && qmin[hmin] <= l)
			++ hmin;
		while(h2 < t2 && q2[h2] <= l)
			++ h2;
	};
	//����Сֵջ�ﱻȡ���ģ���������ջ
	void Push2(int p){
		while(h2 < t2 && arr[q2[t2-1]] >= arr[p])
			-- t2;
		q2[t2 ++] = p;
	}
	//�����µ�ֵ
	void Push(int p){
		int tmp, i;
		//�������ջ
		while(hmax < tmax && arr[qmax[tmax-1]] <= arr[p])
			-- tmax;
		qmax[tmax ++] = p;
		//������Сջ��ע�������ȸ���С�ڵ�
		tmp = tmin;
		while(hmin < tmin && arr[qmin[tmin-1]] > arr[p]){
			-- tmin;
		}
		//��˳������
		for(i = tmin; i < tmp; i ++){
			Push2(qmin[i]);
		}
		//��ֵ�ĸ���ʱ������
		while(hmin < tmin && arr[qmin[tmin-1]] >= arr[p]){
			-- tmin;
		}
		qmin[tmin ++] = p;
	}
	//ȡ�ô�Сֵ��Ӧ��������ջ�ĵ�һ��������Сջ�ĵڶ���
	//����ȡ��С�ľͿ�����
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
		if(val == arr[qmin[hmin]])  //��ʵû��Ҫ
			return -1;
		return val;
	}
	void Show(){  //�����ã���ʾջ��Ԫ��
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
//ֵ����������һ����ɢ�����ֵ��������������˻������Ҷ˵ĳ���
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
//O(n)��ɢ��
void Distinct(){
	int i, n2, cur;
	int *cnt = pos.cnt, *tab1 = pos.head, *tab2 = pos.tail;
	int *arr2 = pos.nxt;
	//��һ������
	for(i = 0; i < N; i ++)
		cnt[arr[i]&0xFFFF] ++;
	for(i = 0; i < 0xFFFF; i ++) cnt[i + 1] += cnt[i];
	for(i = N - 1; i >= 0; i --){
		tab1[i] = -- cnt[arr[i]&0xFFFF];
		arr2[tab1[i]] = arr[i];
	}
	//�ڶ�������
	for(i = 0; i <= 0xFFFF; i ++) cnt[i] = 0;
	for(i = 0; i < N; i ++)
		cnt[arr2[i]>>16] ++;
	for(i = 0; i < 0xFFFF; i ++) cnt[i + 1] += cnt[i];
	for(i = N - 1; i >= 0; i --){
		tab2[i] = -- cnt[arr2[i]>>16];
		arr[tab2[i]] = arr2[i];
	}
	//ֵ��ɢ��
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
	//��ԭ����
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
	//���ͷ�ļ�����ջ
	for(i = 0; i < K - 1; i ++){
		pos.Push(i);
		stk.Push(i);
	}
	//ÿ��ջһ������ѯһ�Σ��ٳ�ջһ��
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
	//ͳ�ƽ���ƽ��ֵ���������
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