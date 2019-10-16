/*
�Ѿ�����ǰ2����������

������������⻹��������Ȥ����������һ������һ��ʱ��Ч�ʰɡ�
�������ǰ��������󣬼���ÿ���ز�����100�࣬ÿ������������Ҫ100���زģ�ÿ������100����
���1��Ȼ�޷�������֮������𰸡�

�Ա��2��˵�����ǵ�Ȼ��Ҫ��������㷨���ϡ���Ϊ�زĵ�������ֻ��233�֣�
�����ز�һ�ߵĽڵ�������100*100������233��������ӶȲ��ᳬ��(335)^3��ʵ�ʳ�����С��
��ÿ�β�ѯ������������+ö��*�������ж�=15000*log(15000)+15000*335^3��
���1��Сʱ���ܸ��������š�

���⣬ʵ����������۸��Ӷ���ʱ���ǲ�ͬ�ģ��Դ󲿷��䷽��˵��ʵ�����õ����ز��������١�
�����һ�㣬Ҳ�������ܸ����Ż���
�������Ǹ�ÿ���䷽��ÿһ���زĽ����ѣ�Ȼ���ÿ���ز������Ӧ�Ķ���ظ��ľ��ظ����롣
Ȼ��ÿ�μ��㵱ǰ����ȡ�Ǽ����زģ���ѡ��Ӧ�Ķѣ������ǵ����Ԫ����ѡ��һ��ȫ�����
Ҳ�����ǻ������ð������Ż���
������Ԫ����ȻӦ��ȡ�á����ǽ����Ӷ��е�����
�����ͬʱ���ڶ���ѣ���ô��Ȼ��ͬʱ�Ƕ���ѵĶ���һ�𵯳����ɡ�
����㷨�����۸��Ӷȱ�ԭʼ�ı��2�ߣ���ôʵ����û�п��ܷ��������أ�
���ǲ�û�С���HDU�Ͻ�3����̷�����ʱ1>>2f>2��
���1    1388ms
���2    31ms
���2��  109ms
����������2����ÿ����������һ�β�ѯ��Σ�����ÿ�β�ѯ������Ļ�����ôҪ��187ms����2f�ߡ�
ʱ���Ǹ���Ȥ�Ķ����š�
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <queue>
#include <list>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//�����������ݺͱ��1��һ����
const int MAXN = 15005;
typedef int Catagory;
typedef int Recipe;
typedef int Quality;
struct Ingredient{
  int cnt;
  Catagory cata[4];
  static Ingredient dat[301];
  static int N, M;

  bool Satisfy(Recipe req){
    if (req > M)
      return (this - dat) == (req - M);
    else
      return find(cata, cata + cnt, req) < (cata + cnt);
  }

  void Read(){
    scanf("%d", &cnt);
    for (int j = 0; j < cnt; j++)
      scanf("%d", cata + j);
  }

  static void GetDatabase(){
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++)
      dat[i].Read();
  }
};
Ingredient Ingredient::dat[301];
int Ingredient::N = 0;
int Ingredient::M = 0;

pair<Ingredient*, Quality> ing[MAXN];
int P;
pair<Recipe, int> rec[4];
int rcnt;
//////////////////////////////////////////////////////////////////////////

struct Cmp{
  bool operator()(const int& lhs, const int& rhs) const{
    //����������ڶѶ�
    return ing[lhs].second < ing[rhs].second;
  }
};
priority_queue<int, vector<int>, Cmp> bakHeap[4]; //��ѡ�زĶ�
vector<int> selected; //��ѡ�ز�

//Legal���ӹ��̣�ͨ��DFS�ͻ�������ж�
bool Legal_DFS(int i, int residue[]){
  if (i == selected.size())
    return true;
  Ingredient* cur = ing[selected[i]].first;
  for (int j = 0; j < rcnt; j++){
    if (cur->Satisfy(rec[j].first) && residue[j] > 0){
      residue[j] --;
      if (Legal_DFS(i + 1, residue))
        return true;
      residue[j] ++;
    }
  }
  return false;
}

//�ж��ڵ�ǰ��ȡ��״���£��ٶ���ȡ��һ��rec[cata].first�����زģ��Ƿ񳬱�
bool Legal(int cata){
  int residue[4];
  for (int i = 0; i < rcnt; i++)
    residue[i] = rec[i].second;
  residue[cata] --;
  return Legal_DFS(0, residue);
}

Quality Solve(){
  list<int> bakCata;
  int i, j;
  int cnt, now;
  Quality sum;
  list<int>::iterator it;

  //һ��ɨ�裬׼�����е��ز�
  for (j = 0; j < rcnt; j++){
    while (!bakHeap[j].empty())
      bakHeap[j].pop();
  }
  for (i = 0; i < P; i++){
    for (j = 0; j < rcnt; j++){
      if (ing[i].first->Satisfy(rec[j].first)){
        bakHeap[j].push(i);
      }
    }
  }

  //׼������
  for (j = 0; j < rcnt; j++)
    bakCata.push_back(j);

  //����̰��ȡ��
  cnt = now = 0;
  sum = 0;
  for (j = 0; j < rcnt; j++)
    cnt += rec[j].second;
  selected.clear();
  selected.reserve(cnt);
  for (; now < cnt; now++){
    //�ҵ����ֵ����4����û��Ҫ�ð�������
    int cur = -1, curQual = -1, tmp;
    curQual = -1;
    for (it = bakCata.begin(); it != bakCata.end(); it++){
      j = *it;
      if (bakHeap[j].empty())
        continue;
      tmp = bakHeap[j].top();
      if (ing[tmp].second >= curQual){
        curQual = ing[tmp].second;
        cur = tmp;
      }
    }

    //���û��ѡ�ˣ���ʧ��
    if (curQual < 0)
      return -1;

    //������һ��ѡ�������
    sum += curQual;
    selected.push_back(cur);
    for (it = bakCata.begin(); it != bakCata.end(); it++){
      j = *it;
      if (ing[cur].first->Satisfy(rec[j].first)){
        bakHeap[j].pop();
      }
    }

    //Ȼ���жϻ���ȡ��Щ�����ز�
    for (it = bakCata.begin(); it != bakCata.end();){
      j = *it;
      if (Legal(j))
        it++;
      else
        it = bakCata.erase(it);
    }
  }

  return sum / cnt;
}

bool domain(){
  int i, idx, Q;
  Quality q;

  if (scanf("%d%d", &P, &Q) == EOF)
    return false;

  for (i = 0; i < P; i++){
    scanf("%d%d", &idx, &q);
    ing[i] = make_pair(Ingredient::dat + idx, q);
  }
  while (Q--){
    scanf("%d", &rcnt);
    for (i = 0; i < rcnt; i++){
      scanf("%d%d", &idx, &q);
      rec[i] = make_pair(idx, q);
    }

    q = Solve();
    printf("%d\n", q);
  }

  return true;
}

int main(){
  Ingredient::GetDatabase();
  while (domain());
  return 0;
}
