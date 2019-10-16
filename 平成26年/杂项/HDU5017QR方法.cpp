#include<cmath>
#include<cstdlib>
#include<algorithm>

#define MAXN 10
#define TINY 1e-20
typedef double matrix2[MAXN][MAXN];
typedef double vector1[MAXN];
//1起始

//QR分解，输入矩阵A和阶数N，输出Q和R
void QRDCMP(matrix2 A, matrix2 Q, matrix2 R, int N)
{
    int i, j, k;
    double f, a;

    //单位化i,j
    for(i=1;i<=N;i++)
        for(j=1;j<=N;j++)
            Q[i][j] = (i==j);
    //k轮分解
    for(k=1;k<=N;k++)
    {
        //把A[k][k]..A[N][k]变成变换基准向量v
        a = 0;
        for(i=k;i<=N;i++)
            a += A[i][k] * A[i][k];
        a = sqrt(a);
        //f = sqrt(2 * a * (a - A[k][k]));
        //上面那种取巧的写法会导致精度的严重损失
        f = (A[k][k] - a) * (A[k][k] - a);
        for(i=k+1;i<=N;i++)
            f += A[i][k] * A[i][k];
        f = sqrt(f);
        if(fabs(f) < TINY)
        {
            continue;
        }
        A[k][k] -= a;
        for(i=k;i<=N;i++)
            A[i][k] /= f;
        //逐列变换A
        for(j=k+1;j<=N;j++)
        {
            f = 0;
            for(i=k;i<=N;i++)
                f += 2 * A[i][j] * A[i][k];
            for(i=k;i<=N;i++)
                A[i][j] -= f * A[i][k];
        }
        //逐列变换Q
        for(j=0;j<=N;j++)
        {
            f = 0;
            for(i=k;i<=N;i++)
                f += 2 * Q[i][j] * A[i][k];
            for(i=k;i<=N;i++)
                Q[i][j] -= f * A[i][k];
        }
        //确定R的第k列
        for(i=k+1;i<=N;i++)
            A[i][k] = 0;
        A[k][k] = a;
    }
    //转置Q
    for(i=1;i<=N;i++)
        for(j=i+1;j<=N;j++)
            std::swap(Q[i][j], Q[j][i]);
    for(i=1;i<=N;i++)
        for(j=1;j<=N;j++)
            R[i][j] = A[i][j];
}

void MATMUL(matrix2 A, matrix2 B, matrix2 C, int N)
{
    int i, j, k;
    double temp;
    for(i=1;i<=N;i++)
        for(j=1;j<=N;j++)
        {
            temp = 0;
            for(k=1;k<=N;k++)
                temp += A[i][k] * B[k][j];
            C[i][j] = temp;
        }
}

matrix2 MAT, MATQ, MATR;
bool domain()
{
    int i;
    double a, b, c, d, e, f;
    double max;
    if(scanf("%lf%lf%lf%lf%lf%lf",&a,&b,&c,&d,&e,&f)==EOF)
        return false;

    MAT[1][1] = a;
    MAT[2][2] = b;
    MAT[3][3] = c;
    MAT[2][3] = MAT[3][2] = d / 2.0;
    MAT[1][3] = MAT[3][1] = e / 2.0;
    MAT[1][2] = MAT[2][1] = f / 2.0;
    for(i=0;i<1000;i++)
    {
        QRDCMP(MAT, MATQ, MATR, 3);
        MATMUL(MATR, MATQ, MAT, 3);
    }
    max = 10000000000;
    for(i=1;i<=3;i++)
    {
        MATR[i][i] = fabs(MATR[i][i]);
        if(1.0 / MATR[i][i] <= max)
            max = 1.0 / sqrt(MATR[i][i]);
    }
    
    printf("%.7lf\n",max);
    return true;
}

int main()
{
    while(domain());
    return 0;
}