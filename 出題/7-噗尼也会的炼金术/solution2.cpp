/*
���2�ǳ�����ԭ�������������
���ǵ�̰��ѡȡ����Ч�ļ��������𣿣����Ĳ�����ν�ġ�ͨ�׽��͡�����ע�⣩

���ȣ�һ����Ҫ�����ǲ��������ԡ�
Ҳ����˵�������ȡN����Ʒû���꣬��ô����Щ��Ʒ���ӵ����ɸ���Ҳ�ǺϷ��ġ�
��2�����͵���������С�����������·��ָ���㵽���е�����·����ͼ����
��С�����������У�ֻҪȡ�ı߲��γɻ����ǺϷ�����ѡ�õ�һ���������ɾ��Ҳ�����γɻ���
���·�����У�ÿһ��ȡ�õıߣ�����������ֻ��ȡ���ıߵ�����¿ɴ
������S->b->c->d�ǺϷ�ѡȡ��ɾȥ��b->c��c�㲻�ɴѡȡ�������Ϸ��ˡ�
������������ʵ�������Ȼ�ǲ�����̰�ĵģ�
��Ϊ���Ž�����û�취ͨ����һ��һ��ȡ���ķ�ʽ�õ���

��ֶ�����Ҫ�������ͺܶ��ˡ�
һ���ǣ��ȼ���ȡ���������Ž⣬��̰�ĵĲ��ԸĽ�����
����Ľ���Ľ�����䣬��ô̰�Ĳ�����Ч��
�㷨���ϸָ�Ӧ�ý�����ôд֤�������ﲻ��׸����

��һ����Ҫ�ĳ�������ǡ����󡱣�Ҳ����˵�ںϷ���ǰ���£�
��������ôѡ��ѡ��������ѡΪֹ���ܻ�õ���ȵĸ�����
�����������ʵ�����һ������̰�ĵõ����Ž⡣
�����پ�2�����͵����ӣ���С�������Ͷ���ͼ���Ȩƥ�䡣
��С�������У�ֻҪȡ�ı���С��N-1���޻����Ϸ�����һ�������ڼ���һ���ߣ�
ʹ�ü����µıߺ���Ȼ�޻�������������������Kruskal̰���㷨������
���Ȩƥ���У��������Ž���{1->2,3->4,5->6}��������ȡ��{1->4,3->6}��
���ֲ�û��5->2�����ߣ���ʱ�Ѿ��޷�ȡ�����ˣ�������Ȼ�����Ž���һ����
�������Ȩƥ�����ⲻ������Ҳ����̰�ġ�

�ص�������⣬ÿ���䷽��̰���������ֲ��Եģ�
��һ���ǣ������ؿ��ǡ������䷽��ҪAx2+Bx1������������A���ز���ȡ2���������ģ�
Ȼ���ʣ�µ��ز���ȡ����B���������ġ�
����㷨��Ȼ���ԣ���������2�ĵ�һ���䷽���Ƿ�����
��һ�ֲ����ǣ�����Ҫ�������Ϊ�����ޡ�����A������2����B������1����
Ȼ��������ز���һ����ȡ�������ģ����ǲ�ָ���Ѿ�ȡ�õ��زĵ��׹���A�»��ǹ���B�¡�
�׼���ô�����ϡ����󡱣����������ж�̰�Ľ�����š�

�������������������زģ�ÿ�γ���ȡһ����Ȼ���ж��Ƿ�Ϸ������Ϸ����˵�������Kruskalһ���ģ�
��ô�ж��Ƿ�Ϸ�����ʵ�Ǹ���������⡣�������嵽����һ����20���㣬�����ѱ������Ϳ��ԡ�
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

bool Cmp(const int& lhs, const int& rhs){
  //�����������ǰ
  return ing[lhs].second > ing[rhs].second;
}
int bakQueue[MAXN];   //��ѡ�زĵĺ󱸶���
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

//�ж��ڵ�ǰ��ȡ��״���Ƿ񳬱�
bool Legal(){
  int residue[4];
  for (int i = 0; i < rcnt; i++)
    residue[i] = rec[i].second;
  return Legal_DFS(0, residue);
}

Quality Solve(){
  int i, j;
  int cnt, now;
  Quality sum;

  //����̰��ȡ��
  cnt = now = 0;
  sum = 0;
  for (j = 0; j < rcnt; j++)
    cnt += rec[j].second;
  selected.clear();
  selected.reserve(cnt);
  for (i = 0; i < P && now < cnt; i++){
    int cur = bakQueue[i];
    selected.push_back(cur);
    if (!Legal()){
      selected.pop_back();
      continue;
    }

    now++;
    sum += ing[cur].second;
  }

  if (now < cnt)
    return -1;
  else
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
  //һ��ɨ�裬׼�����е��ز�
  for (i = 0; i < P; i++)
    bakQueue[i] = i;
  sort(bakQueue, bakQueue + P, Cmp);

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
