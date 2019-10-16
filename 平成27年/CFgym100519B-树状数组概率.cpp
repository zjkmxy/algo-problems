#include <algorithm>
#include <iostream>
#include <iomanip>
#include <complex>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <time.h>
using namespace std;

#define lowbit(x) ((x)&(-(x)))
const int MAXN = 200005;
double R[MAXN * 6], C[MAXN * 6];
double rraw[MAXN * 6], craw[MAXN * 6];
struct BOMB{
    int x, y;
    int p1, p2;
}bomb[MAXN];
struct RECT{
    int x0, y0, x1, y1;
    int tx0, ty0, tx1, ty1;
}rect[MAXN];
int xmap[MAXN * 6], ymap[MAXN * 6];
int xn, ycnt;
int n, m;

double getsum(double arr[], int x)
{
    int i;
    double sum = 0.0;
    for(i=x;i>0;i-=lowbit(i)){
        sum += arr[i];
    }
    return sum;
}

void update(double arr[], int x, double val)
{
    int i;
    for(i=x;i<=MAXN * 6;i+=lowbit(i))
    {
        arr[i] += val;
    }
}

void input(){
    int i, x0, y0, x1, y1;
    xmap[0] = -0x3fffffff;
    ymap[0] = -0x3fffffff;
    scanf("%d%d", &n, &m);
    for(i = 0; i < n; i ++){
        scanf("%d%d%d%d", &x0, &y0, &x1, &y1);
        xmap[++ xn] = x0;
        xmap[++ xn] = x1;
        ymap[++ ycnt] = y0;
        ymap[++ ycnt] = y1;
        rect[i].tx0 = rect[i].x0 = x0;
        rect[i].tx1 = rect[i].x1 = x1;
        rect[i].ty0 = rect[i].y0 = y0;
        rect[i].ty1 = rect[i].y1 = y1;
    }
    for(i = 0; i < m; i ++){
        scanf("%d%d%d%d", &x0, &y0, &bomb[i].p2, &bomb[i].p1);
        xmap[++ xn] = x0;
        ymap[++ ycnt] = y0;
        bomb[i].x = x0;
        bomb[i].y = y0;
    }
    xn ++;
    ycnt ++;
    sort(xmap, xmap + xn);
    xn = unique(xmap, xmap + xn) - xmap;
    sort(ymap, ymap + ycnt);
    ycnt = unique(ymap, ymap + ycnt) - ymap;
    for(i = 0; i < n; i ++){
        rect[i].x0 = lower_bound(xmap, xmap + xn, rect[i].x0) - xmap;
        rect[i].x1 = lower_bound(xmap, xmap + xn, rect[i].x1) - xmap;
        rect[i].y0 = lower_bound(ymap, ymap + ycnt, rect[i].y0) - ymap;
        rect[i].y1 = lower_bound(ymap, ymap + ycnt, rect[i].y1) - ymap;
    }
    for(i = 0; i < m; i ++){
        bomb[i].x = lower_bound(xmap, xmap + xn, bomb[i].x) - xmap;
        bomb[i].y = lower_bound(ymap, ymap + ycnt, bomb[i].y) - ymap;
    }
}

void prepare(){
    int i;
    for(i = 0; i <= xn; i ++){
        rraw[i] = 0.;
    }
    for(i = 0; i <= ycnt; i ++){
        craw[i] = 0.;
    }
    for(i = 0; i < m; i ++){
        rraw[bomb[i].x] += log10(static_cast<double>(100 - bomb[i].p1)) - 2.;
        craw[bomb[i].y] += log10(static_cast<double>(100 - bomb[i].p2)) - 2.;
    }
    for(i = 1; i <= xn; i ++){
        update(R, i, pow(10., rraw[i]));
    }
    for(i = 1; i <= ycnt; i ++){
        update(C, i, pow(10., craw[i]));
    }
}

double calc(){
    int i;
    double ret = 0.0;
    double rsum, csum, area;
    double rless, cless;
    for(i = 0; i < n; i ++){
        rsum = getsum(R, rect[i].x1) - getsum(R, rect[i].x0 - 1);
        rless = double(rect[i].tx1 - rect[i].tx0) - double(rect[i].x1 - rect[i].x0);
        csum = getsum(C, rect[i].y1) - getsum(C, rect[i].y0 - 1);
        cless = double(rect[i].ty1 - rect[i].ty0) - double(rect[i].y1 - rect[i].y0);
        //area = static_cast<double>(rect[i].tx1 - rect[i].tx0 + 1) * static_cast<double>(rect[i].ty1 - rect[i].ty0 + 1);
        area = double(rect[i].x1 - rect[i].x0 + 1) * double(rect[i].y1 - rect[i].y0 + 1);

        ret += area - rsum * csum;
        ret += rless * (double(rect[i].y1 - rect[i].y0 + 1) - csum);
        ret += cless * (double(rect[i].x1 - rect[i].x0 + 1) - rsum);
    }
    return ret;
}

int main(){
    input();
    prepare();
    printf("%.15lf\n", calc());
    return 0;
}