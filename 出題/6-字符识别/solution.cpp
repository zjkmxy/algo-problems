/*
乱搞题，做法不唯一。

首先上来肯定是标联通域，这里用并查集实现2遍扫描法解决。
虽然没有MATLAB那个什么等价对消简算法快，但是本题也够用了。
注意标记黑色字符时为8向联通，但是白色背景时为4向联通。
标记完成后B和total就已经可以判断出来了。

然后我这里统计了一下重心，还有惯量椭球的3个主轴大小（不知道的回去补大学物理）
这里归一化的垂直转动惯量，以及平面的离心率都可以作为特征，
但是实践表明前者受字体大小略有变化，因此我们采用离心率来作为阈值卡出I。

最后考虑每个字符中心对称后的重合比率，将其作为中心对称得分。
用这个卡出S，剩下的是T。
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

//并查集
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

  //初始化
  void Init(){
    blkCnt = labCnt = 0;
  }

  //生成新的块
  int NewBlock(){
    ++blkCnt;
    node[blkCnt].par = node + blkCnt;
    node[blkCnt].lab = 0;
    return blkCnt;
  }

  //合并两个块
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

  //取得/分配块标号
  int GetLab(int l){
    Node* root = node[l].root();
    if (root->lab == 0){
      ++labCnt;
      root->lab = labCnt;
    }
    return root->lab;
  }
}us;

//联通域标记
//dir8:是否是8向标记
//dst:目标字符
//lab:结果矩阵
int LabelContour(bool dir8, char dst, int lab[MAXN][MAXN]){
  int i, j;

  us.Init();
  //第一遍
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      lab[i][j] = 0;
      if (img[i][j] != dst)
        continue;
      //左标记
      if (j > 0 && img[i][j - 1] == dst){
        lab[i][j] = lab[i][j - 1];
      }
      //上标记
      if (i > 0 && img[i - 1][j] == dst){
        if (lab[i][j] <= 0){
          lab[i][j] = lab[i - 1][j];
        }
        else{
          us.Merge(lab[i][j], lab[i - 1][j]);
        }

      }
      //左上标记
      if (dir8 && i > 0 && j > 0 && lab[i][j] <= 0 && img[i - 1][j - 1] == dst){
        lab[i][j] = lab[i - 1][j - 1];
      }
      //右上标记
      if (dir8 && i > 0 && j < M - 1 && img[i - 1][j + 1] == dst){
        if (lab[i][j] <= 0){
          lab[i][j] = lab[i - 1][j + 1];
        }
        else{
          us.Merge(lab[i][j], lab[i - 1][j + 1]);
        }
      }
      //新标记
      if (lab[i][j] <= 0){
        lab[i][j] = us.NewBlock();
      }
    }
  }

  //第二遍
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      if (img[i][j] != dst)
        continue;
      lab[i][j] = us.GetLab(lab[i][j]);
    }
  }

  return us.labCnt;
}

//黑色字符数据统计
struct Letter{
  double comX, comY;    //质心位置
  double sumX2, sumY2;  //方均值
  double sumXY;         //交叉积（以上两个为二阶矩）
  int cnt;              //总点数（以字号平方为比例）
  double rinertia;      //归一化平面转动惯量
  int weissNeibour;     //白色邻块号
  double eoiRa, eoiRb;  //最大惯量和最小惯量
  double theta;         //主轴朝向
  char ch;              //字符签名
  double centreSymRat;  //中心对称率
}lets[MAXN * 10];

//通过标记相邻白块号来标记B
void AggreLetter_SignB(int idx, int neib){
  if (lets[idx].weissNeibour == 0){
    lets[idx].weissNeibour = neib;
  }
  else if (lets[idx].weissNeibour != neib){
    lets[idx].ch = 'B';
  }
}

//计算离心率
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

//判断某特定点得点
double GetSymScore_Point(int x, int y, int idx){
  if (x >= 0 && x < N && y >= 0 && y < M && schwarz[x][y] == idx)
    return 1.0;
  else
    return 0.0;
}

//中心对称得点
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

//字符统计信息汇总
void AggreLetter(int labCnt){
  int i, j, idx;

  //清空数据
  for (idx = 0; idx <= labCnt; idx++){
    lets[idx].comX = lets[idx].comY = lets[idx].sumX2 = lets[idx].sumY2 = 0.0;
    lets[idx].sumXY = 0.0;
    lets[idx].cnt = 0;
    lets[idx].weissNeibour = 0;
    lets[idx].ch = ' ';
    lets[idx].centreSymRat = 0.0;
  }

  //先统计重心
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

  //再计算
  for (idx = 1; idx <= labCnt; idx++){
    lets[idx].comX /= lets[idx].cnt;
    lets[idx].comY /= lets[idx].cnt;
    lets[idx].sumX2 = lets[idx].sumX2 / lets[idx].cnt - lets[idx].comX * lets[idx].comX;
    lets[idx].sumY2 = lets[idx].sumY2 / lets[idx].cnt - lets[idx].comY * lets[idx].comY;
    lets[idx].sumXY = lets[idx].sumXY / lets[idx].cnt - lets[idx].comX * lets[idx].comY;

    //计算归一化转动惯量
    //这个值旋转不变，但是受字体大小影响，不便二值化
    lets[idx].rinertia = lets[idx].sumX2 + lets[idx].sumY2;
    lets[idx].rinertia = lets[idx].rinertia / lets[idx].cnt;

    //计算惯量椭球半径
    AggreLetter_CalcEoi(idx);

    //标记I
    if (lets[idx].ch == ' ' && lets[idx].eoiRa / lets[idx].eoiRb < 0.15){
      lets[idx].ch = 'I';
    }
  }

  //最后统计中心对称率
  for (i = 0; i < N; i++){
    for (j = 0; j < M; j++){
      idx = schwarz[i][j];
      if (idx <= 0)
        continue;
      lets[idx].centreSymRat += GetSymScore(2 * lets[idx].comX - i, 2 * lets[idx].comY - j, idx);
    }
  }

  //再计算
  for (idx = 1; idx <= labCnt; idx++){
    lets[idx].centreSymRat /= lets[idx].cnt;
    //标记T、S
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

  //读入
  scanf("%d%d", &N, &M);
  for (i = 0; i < N; i++){
    scanf("%s", img[i + 1] + 1);
  }
  N += 2;
  M += 2;

  //8向标黑，4向标白
  total = LabelContour(true, '#', schwarz);
  blk = LabelContour(false, '.', weiss);

  AggreLetter(total);
  CountAll(total);

  //输出个数
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