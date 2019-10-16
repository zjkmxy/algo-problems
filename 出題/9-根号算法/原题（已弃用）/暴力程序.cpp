#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

const int MAXN = 10001;
map<int, int> item[MAXN];
int A, B, P, Q, K;

inline int getVal(int id){
  auto ita = item[id].find(A);
  auto itb = item[id].find(B);
  int ret = 0;
  if (ita != item[id].end())
    ret += P * ita->second;
  if (itb != item[id].end())
    ret += Q * itb->second;
  return ret;
}

bool comp(const int& l, const int& r){
  int vl = getVal(l), vr = getVal(r);
  if (vl != vr)
    return vl > vr;
  else
    return l < r;
}

inline char getc(){
  static const int BUFLEN = 1 << 15;
  static char B[BUFLEN], *S = B, *T = B;
  if (S == T) S = B, T = S + fread(B, 1, BUFLEN, stdin);
  return (S == T) ? 0 : *(S++);
}
int ReadInt(){
  char ch; int aa = 0;
  do ch = getc(); while (!isdigit(ch) && ch);
  if (!ch)
    return -1;
  do aa = aa * 10 + ch - '0', ch = getc();
  while (isdigit(ch));
  return aa;
}
int ReadFloat(){
  char ch; int aa = 0;
  do ch = getc(); while (!isdigit(ch));
  char tmp = getc();
  if (tmp != '.')
    return (ch - '0') * 1000;
  int z = 4;
  do aa = aa * 10 + ch - '0', ch = getc(), z--;
  while (isdigit(ch));
  while (z--) aa *= 10;
  return aa;
}

int arr[MAXN];
bool domain(){
  int N, M, c, x, y;
  N = ReadInt(), M = ReadInt();
  if (N <= 0)
    return false;
  for (int i = 0; i < N; i++){
    c = ReadInt();
    item[i].clear();
    while (c--){
      x = ReadInt();
      y = ReadFloat();
      item[i][x] = y;
    }
  }

  for (int i = 0; i < N; i ++)
    arr[i] = i; 

  while (M--){
    c = ReadInt();
    if (c == 1){
      //O(N)²éÑ¯
      A = ReadInt();
      P = ReadFloat();
      B = ReadInt();
      Q = ReadFloat();
      K = ReadInt();
      K--;

      nth_element(arr, arr + K, arr + N, comp);
      x = arr[K];
      if (getVal(x) == 0)
        x = -1;
      x++;
      printf("%d\n", x);
    }
    else{
      //O(log)ÐÞ¸Ä
      A = ReadInt();
      A--;
      x = ReadInt();
      y = ReadFloat();
      if (y <= 1e-10){
        item[A].erase(x);
      }
      else{
        item[A][x] = y;
      }
    }
  }
  return true;
}


int main(){
  while (domain());
  return 0;
}
