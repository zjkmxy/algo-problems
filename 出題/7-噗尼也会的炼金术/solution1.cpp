/*
Ϊ��ӦBվ��λ������һ����·����һ����ϡ��ĺ��٣���һ���ǳ�ԭ����Ʒ��ġ�
��Ȼ���Ҳ�����⡣
�������ݿ����A17���ݿ��һ���֣�����Ǵ��շ������µİ͹�ķ�ص�������ġ�
�����1��DP�㷨��14���Ľ��ϰ��������
�����Ǳ�ʾ��л��

ÿ����ѯ�϶��Ƿֿ�����ġ���Ϊ�䷽Ҫ���ز����̶��������õ����ز����������Ϳ��ԡ�
���ǵ�ÿ���زĿ������ڶ������༴����ͬʱ�����䷽�еĶ����������
����������Ҫ��ȷ��ÿ���زĵ�����Ϊ��һ��ԭ�ϱ�ʹ�á���
��dp[i][x][y][z][w]��ʾ��
�Ѿ�������ǰi���زģ���ʣx��1��ԭ�ϣ�y��2�࣬z��3�࣬w��4�������£�����ȡ������������͡�
DP�����Զ��׼���
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

const int MAXN = 15005;
typedef int Catagory; //�زĴ���
typedef int Recipe;   //�䷽����
typedef int Quality;  //�ز�����
struct Ingredient{
  int cnt;            //���ز����ڼ������
  Catagory cata[4];   //�������ļ���
  static Ingredient dat[301]; //���ָ������زģ�ȫ233����
  static int N, M;

  //���ز��ܷ������䷽��req�������
  bool Satisfy(Recipe req){
    if (req > M)
      return (this - dat) == (req - M);
    else
      return find(cata, cata + cnt, req) < (cata + cnt);
  }

  //����һ���ز�����
  void Read(){
    scanf("%d", &cnt);
    for (int j = 0; j < cnt; j++)
      scanf("%d", cata + j);
  }

  //���빲�����ݿ�
  static void GetDatabase(){
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++)
      dat[i].Read();
  }
};
Ingredient Ingredient::dat[301];
int Ingredient::N = 0;
int Ingredient::M = 0;

pair<Ingredient*, Quality> ing[MAXN]; //���������������زģ����������
int P;
pair<Recipe, int> rec[4];   //���β�ѯ�䷽��ԭ��Ҫ����𣬸�����
int rcnt;

Quality Solve(){
  static Quality dp[2][6][6][6][6];
  int i, x, y, z, w;
  int rd = 0, wt = 1;

  for (i = rcnt; i < 4; i++)
    rec[i] = make_pair(0, 0);

  memset(dp, -1, sizeof(dp));
  dp[rd][rec[0].second][rec[1].second][rec[2].second][rec[3].second] = 0;
  for (i = 0; i < P; i++, rd ^= 1, wt ^= 1){
    for (x = 0; x <= rec[0].second; x++){
      for (y = 0; y <= rec[1].second; y++){
        for (z = 0; z <= rec[2].second; z++){
          for (w = 0; w <= rec[3].second; w++){
            if (dp[rd][x][y][z][w] < 0)
              continue;
            //1
            if (x > 0 && ing[i].first->Satisfy(rec[0].first)){
              dp[wt][x - 1][y][z][w] = max(dp[wt][x - 1][y][z][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //2
            if (y > 0 && ing[i].first->Satisfy(rec[1].first)){
              dp[wt][x][y - 1][z][w] = max(dp[wt][x][y - 1][z][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //3
            if (z > 0 && ing[i].first->Satisfy(rec[2].first)){
              dp[wt][x][y][z - 1][w] = max(dp[wt][x][y][z - 1][w], dp[rd][x][y][z][w] + ing[i].second);
            }
            //4
            if (w > 0 && ing[i].first->Satisfy(rec[3].first)){
              dp[wt][x][y][z][w - 1] = max(dp[wt][x][y][z][w - 1], dp[rd][x][y][z][w] + ing[i].second);
            }
            //����
            dp[wt][x][y][z][w] = max(dp[wt][x][y][z][w], dp[rd][x][y][z][w]);
          }//w
        }//z
      }//y
    }//x
  }//i

  if (dp[rd][0][0][0][0] == -1)
    return -1;
  else
    return dp[rd][0][0][0][0] / (rec[0].second + rec[1].second + rec[2].second + rec[3].second);
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
