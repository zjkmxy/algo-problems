/*
��������⡣
��Ŀ��������ʲ���⣬������Ϊ���������ؽ�����2x2˫������ʲô����ʲ����
����������ʲ��������ٶ����ҵľ����ߡ�һ�Σ���ϲ�����ʲ�������Ȼ����������¡�һ�Σ�
��˼�ʹ���Ȳ���֪����ʲ����Ҳ��������

����Դ�ںϷʱ������ǵ������⣬�ĳ���˫��ͬʱ���ߣ��Ұ��߷��ò�����յİ汾��
��������Ȼһ�����ڻ�ϲ�����ʲ���⣬�����������н����������̫�鷳�ˡ�
����ֻ�ð�����򻯳���ÿ��2x2��˫�����ģ�������������Щ����������ʹ���Ҳ������ͬһ�ֻ��Ҽ����ˡ�
��ԭ����ֻ��������棬���ܿ��ǳɱ�������ò�����޽�ģ�

�����׿��������ǵ���DP��������Ŀ˵��������������ֲ�ͬ��������������Ž���ںͲ����ڡ�
ע�⵽��ֻҪ�κ�һ�����������Լ����Ƕ��棩���ڴ��������ţ���ô��һ�ֵ�����ѡ����Ψһȷ���ġ�
��һ����Ҫ�ڴ�����д�塣
����֮��һ���ǻ�ϲ����ˣ���Ϊ˫��˭�����涼�޷���ߣ�
�༴�ҷ�����ǲ����x��Ӧ��ʹ�Է����������溯��G(x,y)��ԶԷ�����ǲ����y�䵼����Ϊ0��
�༴�Է�������ǲ���ǲ���ǲ�����ҷ�����ǲ����x�£��������涼����仯��
��һԪһ�����Է��̾Ϳ��Եõ�x��ֵ��

ϸ�ںܶ�ע�ⲻҪд��
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

const int MAXN = 103;

double dp[MAXN][MAXN][MAXN];     //dp[����][A����][B����]=A����������
double profit[MAXN], cost[MAXN]; //�����󣬴���

//����rnd�֣�Aʣpa�ˣ�Bʣpb�˵�ʱ��A��һ�ֳ��˵ĸ���
double Solve(int rnd, int pa, int pb){
  //û�˵����
  if (pa <= 0){
    return 0;
  }
  //�Է�û�˵����
  if (pb <= 0){
    if (dp[rnd + 1][pa - 1][0] + profit[rnd] > dp[rnd + 1][pa][pb]){
      return 1.0;
    }
    else{
      return 0.0;
    }
  }
  //���㼺�������棨1,0��ʾ�Ƿ���ˣ�
  double w11, w10, w01, w00;
  w11 = dp[rnd + 1][pa - 1][pb - 1] + cost[rnd];
  w10 = dp[rnd + 1][pa - 1][pb] + profit[rnd];
  w01 = dp[rnd + 1][pa][pb - 1];
  w00 = dp[rnd + 1][pa][pb];
  //����Է�������
  double z11, z10, z01, z00;
  z11 = dp[rnd + 1][pb - 1][pb - 1] + cost[rnd];
  z10 = dp[rnd + 1][pb][pa - 1];
  z01 = dp[rnd + 1][pb - 1][pa] + profit[rnd];
  z00 = dp[rnd + 1][pb][pa];
  //����û��ѡ����������������ţ�
  if (w01 >= w11 && w00 >= w10){
    return 0.0;
  }
  if (w11 >= w01 && w10 >= w00){
    return 1.0;
  }
  //�Է�û��ѡ�����
  if (z10 >= z11 && z00 >= z01){
    return (w00 >= w10) ? 0.0 : 1.0;
  }
  if (z11 >= z10 && z01 >= z00){
    return (w01 >= w11) ? 0.0 : 1.0;
  }
  //��������������������ʲ�����
  double p = (z00 - z01) / (z11 + z00 - z01 - z10);
  assert(p >= 1e-6 && p <= (1 - 1e-6));
  return p;
}

//��дdp[rnd][pa][pb]������ļ������ظ��ģ�
void FillDp(int rnd, int pa, int pb){
  //���㼺�������棨1,0��ʾ�Ƿ���ˣ�
  double w11, w10, w01, w00;
  w11 = dp[rnd + 1][pa - 1][pb - 1] + cost[rnd];
  w10 = dp[rnd + 1][pa - 1][pb] + profit[rnd];
  w01 = dp[rnd + 1][pa][pb - 1];
  w00 = dp[rnd + 1][pa][pb];
  //����˫�����ж�����
  double qa, qb;
  qa = Solve(rnd, pa, pb);
  qb = Solve(rnd, pb, pa);
  //��дDpֵ
  dp[rnd][pa][pb] = qa * qb * w11 + qa * (1 - qb) * w10
    + (1 - qa) * qb * w01 + (1 - qa) * (1 - qb) * w00;
}

bool domain(){
  int A, B, N, K;
  int i, j, k;
  int v, c;

  if (scanf("%d%d%d", &N, &A, &B) == EOF){
    return false;
  }
  for (i = 1; i <= N; i++){
    scanf("%d%d", &v, &c);
    cost[i] = -c;
    profit[i] = v - c;
  }

  memset(dp, 0, sizeof(dp));
  //����ע��DP������������˫�����������ֵ������ֻ�����Լ�
  K = max(A, B);
  for (i = N; i >= 1; i--){
    for (j = 0; j <= K; j++){
      for (k = 0; k <= K; k++){
        FillDp(i, j, k);
      }
    }
  }

  printf("%.2lf%%\n", Solve(1, A, B) * 100);
  return true;
}

int main(){
  while (domain());
  return 0;
}
