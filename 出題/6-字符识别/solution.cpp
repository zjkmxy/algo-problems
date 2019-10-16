/*
�Ҹ��⣬������Ψһ��

���������϶��Ǳ���ͨ�������ò��鼯ʵ��2��ɨ�跨�����
��Ȼû��MATLAB�Ǹ�ʲô�ȼ۶������㷨�죬���Ǳ���Ҳ�����ˡ�
ע���Ǻ�ɫ�ַ�ʱΪ8����ͨ�����ǰ�ɫ����ʱΪ4����ͨ��
�����ɺ�B��total���Ѿ������жϳ����ˡ�

Ȼ��������ͳ����һ�����ģ����й��������3�������С����֪���Ļ�ȥ����ѧ����
�����һ���Ĵ�ֱת���������Լ�ƽ��������ʶ�������Ϊ������
����ʵ������ǰ���������С���б仯��������ǲ�������������Ϊ��ֵ����I��

�����ÿ���ַ����ĶԳƺ���غϱ��ʣ�������Ϊ���ĶԳƵ÷֡�
���������S��ʣ�µ���T��
*/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
using namespace std;

const int MAXN = 1505;
char img[MAXN][MAXN];
int schwarz[MAXN][MAXN], weiss[MAXN][MAXN];
int cntForChar[26];
int N, M;

//���鼯
struct UnionSet{
  struct Node{
    Node* par;
    int lab;
    Node* root(){
      if (par != this){
        par = par->root();
      }
      return par;
    }
  }node[MAXN * 10];
  int blkCnt;
  int labCnt;

  //��ʼ��
  void Init(){
    blkCnt = labCnt = 0;
  }

  //�����µĿ�
  int NewBlock(){
    ++blkCnt;
    node[blkCnt].par = node + blkCnt;
    node[blkCnt].lab = 0;
    return blkCnt;
  }

  //�ϲ�������
  bool Merge(int la, int lb){
    Node* rootA = node[la].root();
    Node* rootB = node[lb].root();
    if (rootA != rootB){
      rootA->par = rootB;
      return true;
    }
    else
      return false;
  }

  //ȡ��/�������
  int GetLab(int l){
    Node* root = node[l].root();
    if (root->lab == 0){
      ++labCnt;
      root->lab = labCnt;
    }
    return root->lab;
  }
}us;

//��ͨ����
//dir8:�Ƿ���8����
//dst:Ŀ���ַ�
//lab:�������
int LabelContour(bool dir8, char dst, int lab[MAXN][MAXN]){
  int i, j;

  us.Init();
  //��һ��
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      lab[i][j] = 0;
      if (img[i][j] != dst)
        continue;
      //����
      if (j > 0 && img[i][j - 1] == dst){
        lab[i][j] = lab[i][j - 1];
      }
      //�ϱ��
      if (i > 0 && img[i - 1][j] == dst){
        if (lab[i][j] <= 0){
          lab[i][j] = lab[i - 1][j];
        }
        else{
          us.Merge(lab[i][j], lab[i - 1][j]);
        }

      }
      //���ϱ��
      if (dir8 && i > 0 && j > 0 && lab[i][j] <= 0 && img[i - 1][j - 1] == dst){
        lab[i][j] = lab[i - 1][j - 1];
      }
      //���ϱ��
      if (dir8 && i > 0 && j < M - 1 && img[i - 1][j + 1] == dst){
        if (lab[i][j] <= 0){
          lab[i][j] = lab[i - 1][j + 1];
        }
        else{
          us.Merge(lab[i][j], lab[i - 1][j + 1]);
        }
      }
      //�±��
      if (lab[i][j] <= 0){
        lab[i][j] = us.NewBlock();
      }
    }
  }

  //�ڶ���
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      if (img[i][j] != dst)
        continue;
      lab[i][j] = us.GetLab(lab[i][j]);
    }
  }

  return us.labCnt;
}

//��ɫ�ַ�����ͳ��
struct Letter{
  double comX, comY;    //����λ��
  double sumX2, sumY2;  //����ֵ
  double sumXY;         //���������������Ϊ���׾أ�
  int cnt;              //�ܵ��������ֺ�ƽ��Ϊ������
  double rinertia;      //��һ��ƽ��ת������
  int weissNeibour;     //��ɫ�ڿ��
  double eoiRa, eoiRb;  //����������С����
  double theta;         //���ᳯ��
  char ch;              //�ַ�ǩ��
  double centreSymRat;  //���ĶԳ���
}lets[MAXN * 10];

//ͨ��������ڰ׿�������B
void AggreLetter_SignB(int idx, int neib){
  if (lets[idx].weissNeibour == 0){
    lets[idx].weissNeibour = neib;
  }
  else if (lets[idx].weissNeibour != neib){
    lets[idx].ch = 'B';
  }
}

//����������
void AggreLetter_CalcEoi(int idx){
  double a = lets[idx].sumX2;
  double b = lets[idx].sumXY * 2;
  double c = lets[idx].sumY2;
  lets[idx].eoiRa = (a + c) / 2 - ((a - c) * (a - c) + b * b) / 2 / sqrt(b * b + (a - c) * (a - c));
  lets[idx].eoiRb = (a + c) / 2 + ((a - c) * (a - c) + b * b) / 2 / sqrt(b * b + (a - c) * (a - c));

  lets[idx].eoiRa = lets[idx].eoiRa / lets[idx].cnt;
  lets[idx].eoiRb = lets[idx].eoiRb / lets[idx].cnt;

  lets[idx].theta = atan(b / (a - c)) / 2.0;
  lets[idx].theta = lets[idx].theta / 3.1415926535 * 180;
}

//�ж�ĳ�ض���õ�
double GetSymScore_Point(int x, int y, int idx){
  if (x >= 0 && x < N && y >= 0 && y < M && schwarz[x][y] == idx)
    return 1.0;
  else
    return 0.0;
}

//���ĶԳƵõ�
double GetSymScore(double x, double y, int idx){
  int x0 = (int)floor(x), x1 = (int)ceil(x + 1e-8);
  int y0 = (int)floor(y), y1 = (int)ceil(y + 1e-8);
  double p11 = (x - x0) * (y - y0);
  double p10 = (x - x0) * (y1 - y);
  double p01 = (x1 - x) * (y - y0);
  double p00 = (x1 - x) * (y1 - y);
  double tot = 0.0;
  tot += GetSymScore_Point(x0, y0, idx) * p00;
  tot += GetSymScore_Point(x0, y1, idx) * p01;
  tot += GetSymScore_Point(x1, y0, idx) * p10;
  tot += GetSymScore_Point(x1, y1, idx) * p11;
  tot /= (p00 + p01 + p10 + p11);
  return tot;
}

//�ַ�ͳ����Ϣ����
void AggreLetter(int labCnt){
  int i, j, idx;

  //�������
  for (idx = 0; idx <= labCnt; idx++){
    lets[idx].comX = lets[idx].comY = lets[idx].sumX2 = lets[idx].sumY2 = 0.0;
    lets[idx].sumXY = 0.0;
    lets[idx].cnt = 0;
    lets[idx].weissNeibour = 0;
    lets[idx].ch = ' ';
    lets[idx].centreSymRat = 0.0;
  }

  //��ͳ������
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      idx = schwarz[i][j];
      if (idx <= 0)
        continue;
      if (j > 0 && weiss[i][j - 1] > 0){
        AggreLetter_SignB(idx, weiss[i][j - 1]);
      }
      if (i > 0 && weiss[i - 1][j] > 0){
        AggreLetter_SignB(idx, weiss[i - 1][j]);
      }
      if (j < M - 1 && weiss[i][j + 1] > 0){
        AggreLetter_SignB(idx, weiss[i][j + 1]);
      }
      if (i < N - 1 && weiss[i + 1][j] > 0){
        AggreLetter_SignB(idx, weiss[i + 1][j]);
      }

      lets[idx].cnt++;
      lets[idx].comX += i;
      lets[idx].comY += j;
      lets[idx].sumX2 += 1.0 * i * i;
      lets[idx].sumY2 += 1.0 * j * j;
      lets[idx].sumXY += 1.0 * i * j;
    }
  }

  //�ټ���
  for (idx = 1; idx <= labCnt; idx++){
    lets[idx].comX /= lets[idx].cnt;
    lets[idx].comY /= lets[idx].cnt;
    lets[idx].sumX2 = lets[idx].sumX2 / lets[idx].cnt - lets[idx].comX * lets[idx].comX;
    lets[idx].sumY2 = lets[idx].sumY2 / lets[idx].cnt - lets[idx].comY * lets[idx].comY;
    lets[idx].sumXY = lets[idx].sumXY / lets[idx].cnt - lets[idx].comX * lets[idx].comY;

    //�����һ��ת������
    //���ֵ��ת���䣬�����������СӰ�죬�����ֵ��
    lets[idx].rinertia = lets[idx].sumX2 + lets[idx].sumY2;
    lets[idx].rinertia = lets[idx].rinertia / lets[idx].cnt;

    //�����������뾶
    AggreLetter_CalcEoi(idx);

    //���I
    if (lets[idx].ch == ' ' && lets[idx].eoiRa / lets[idx].eoiRb < 0.15){
      lets[idx].ch = 'I';
    }
  }

  //���ͳ�����ĶԳ���
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      idx = schwarz[i][j];
      if (idx <= 0)
        continue;
      lets[idx].centreSymRat += GetSymScore(2 * lets[idx].comX - i, 2 * lets[idx].comY - j, idx);
    }
  }

  //�ټ���
  for (idx = 1; idx <= labCnt; idx++){
    lets[idx].centreSymRat /= lets[idx].cnt;
    //���T��S
    if (lets[idx].ch == ' '){
      if (lets[idx].centreSymRat >= 0.5)
        lets[idx].ch = 'S';
      else
        lets[idx].ch = 'T';
    }
  }
}

void CountAll(int total){
  fill_n(cntForChar, 26, 0);
  for (int i = 1; i <= total; i++){
    cntForChar[lets[i].ch - 'A'] ++;
  }
}

void domain(){
  int i;
  int total, blk;

  //����
  scanf("%d%d", &N, &M);
  for (i = 0; i < N; i++){
    scanf("%s", img[i + 1] + 1);
  }
  N += 2;
  M += 2;

  //8���ڣ�4����
  total = LabelContour(true, '#', schwarz);
  blk = LabelContour(false, '.', weiss);

  AggreLetter(total);
  CountAll(total);

  //�������
  printf("B: %d\n", cntForChar['B' - 'A']);
  assert(cntForChar['B' - 'A'] == (blk - 1) / 2);
  //printf("I: %d\n", cntForChar['I' - 'A']);
  //printf("T: %d\n", cntForChar['T' - 'A']);
  //printf("S: %d\n", cntForChar['S' - 'A']);
  printf("total: %d\n", total);
}

int main(){
  int T;
  scanf("%d", &T);
  while (T--){
    domain();
  }
  return 0;
}