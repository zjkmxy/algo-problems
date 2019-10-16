#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

#define MAXN 105
#define TINY 1e-20
typedef double matrix2[MAXN][MAXN];
typedef double vector2[MAXN];

//以下!=0之类的可以按需要改成用EPS判

//三角分解A
//输入：阶数 N，矩阵A
//输出：A为三角分解结果（下L上半U）；INDX：行交换结果
bool LUDCMP(matrix2 A, const int n, int indx[])
{
	double VV[MAXN], aamax, sum, dum;
	int i, j, k, imax;

	//选取行最大元为主元
	for(i=1;i<=n;i++){
		aamax = 0.0;
		for(j=1;j<=n;j++)
			if(abs(A[i][j]) > aamax)
				aamax = abs(A[i][j]);
		if(aamax == 0.0)
			return false;  //奇异矩阵不可分解
		VV[i] = 1.0 / aamax;
	}
	for(j=1;j<=n;j++){
		for(i=1;i<j;i++){
			sum = A[i][j];
			for(k=1;k<i;k++)
				sum -= A[i][k] * A[k][j];
			A[i][j] = sum;
		}
		aamax = 0;
		for(i=j;i<=n;i++){
			sum = A[i][j];
			for(k=1;k<j;k++)
				sum -= A[i][k] * A[k][j];
			A[i][j] = sum;
			dum = VV[i] * abs(sum);
			if(dum >= aamax){
				imax = i;
				aamax = dum;
			}
		}
		//行交换
		if(j != imax){
			for(k=1;k<=n;k++){
				dum = A[imax][k];
				A[imax][k] = A[j][k];
				A[j][k] = dum;
			}
			VV[imax] = VV[j];
		}
		indx[j] = imax;
		if(A[j][j] == 0)
			A[j][j] = TINY;
		if(j < n){
			dum = 1.0 / A[j][j];
			for(i=j+1;i<=n;i++)
				A[i][j] *= dum;
		}
	}
	return true;
}

//LU求解方程
void LUBKSB(matrix2 A, const int n, int indx[], vector2 B)
{
	int i, j, ll, ii = 0;
	double sum;

	for(i=1;i<=n;i++){
		ll = indx[i];
		sum = B[ll];
		B[ll] = B[i];
		if(ii != 0){
			for(j=ii;j<i;j++)
				sum -= A[i][j] * B[j];
		}else if(sum != 0)
			ii = i;
		B[i] = sum;
	}
	for(i=n;i>0;i--){
		sum = B[i];
		if(i < n)
			for(j=i+1;j<=n;j++)
				sum -= A[i][j] * B[j];
		B[i] = sum / A[i][i];
	}
}

//奇异值分解
//A=>AWV',M行N列，M>=N
bool SVDCMP(matrix2 A, const int M, const int N, vector2 W, matrix2 V)
{
	double RV1[MAXN];
	int i, j, l, its, nm, k;
	double g = 0, s, scale1 = 0, c, f, h, x, y, z, sgn, anorm = 0;
	int tmp;

	if(M < N)
		return false;
	//构建双对角矩阵?
	for(i=1;i<=N;i++){
		l = i + 1;
		RV1[i] = scale1 * g;
		g = s = scale1 = 0;
		if(i <= M){
			for(k=i;k<=M;k++)
				scale1 += abs(A[k][i]);
			if(scale1 != 0){
				for(k=i;k<=M;k++){
					A[k][i] /= scale1;
					s += A[k][i] * A[k][i];
				}
				f = A[i][i];
				sgn = (f > 0) ? 1 : -1;
				g = -sqrt(s) * sgn;
				h = f * g - s;
				A[i][i] = f - g;
				if(i != N)
					for(j=l;j<=N;j++){
						s = 0;
						for(k=i;k<=M;k++)
							s += A[k][i] * A[k][j];
						f = s / h;
						for(k=i;k<=M;k++)
							A[k][j] += f * A[k][i];
					}
				for(k=i;k<=M;k++)
					A[k][i] *= scale1;
			}
		}
		W[i] = scale1 * g;
		g = s = scale1 = 0;
		if(i <= M && i != N){
			for(k=l;k<=N;k++)
				scale1 += abs(A[i][k]);
			if(scale1 != 0){
				for(k=l;k<=N;k++){
					A[i][k] /= scale1;
					s += A[i][k] * A[i][k];
				}
				f = A[i][l];
				sgn = (f > 0) ? 1 : -1;
				g = -sqrt(s) * sgn;
				h = f * g - s;
				A[i][l] = f - g;
				for(k=l;k<=N;k++)
					RV1[k] = A[i][k] / h;
				if(i != M)
					for(j=l;j<=M;j++){
						s = 0;
						for(k=l;k<=N;k++)
							s += A[j][k] * A[i][k];
						for(k=l;k<=N;k++)
							A[j][k] += s * RV1[k];
					}
				for(k=l;k<=N;k++)
					A[i][k] *= scale1;
			}
		}
		anorm = max(anorm, abs(W[i]) + abs(RV1[i]));
	}
	for(i=N;i>=1;i--){
		if(i < N){
			if(g >= 0){
				for(j=l;j<=N;j++)
					V[j][i] = (A[i][j] / A[i][l]) / g;
				for(j=l;j<=N;j++){
					s = 0;
					for(k=l;k<=N;k++)
						s += A[i][k] * V[k][j];
					for(k=l;k<=N;k++)
						V[k][j] += s * V[k][i];
				}
			}
			for(j=l;j<=N;j++)
				V[i][j] = V[j][i] = 0;
		}
		V[i][i] = 1;
		g = RV1[i];
		l = i;
	}
	for(i=N;i>=1;i--){
		l = i + 1;
		g = W[i];
		for(j=l;j<=N;j++)
			A[i][j] = 0;
		if(g != 0){
			g = 1.0 / g;
			for(j=l;j<=N;j++){
				s = 0;
				for(k=l;k<=M;k++)
					 s += A[k][i] * A[k][j];
				f = (s / A[i][i]) * g;
				for(k=i;k<=M;k++)
					A[k][j] += f * A[k][i];
			}
			for(j=i;j<=M;j++)
				A[j][i] *= g;
		}else{
			for(j=i;j<=M;j++)
				A[j][i] = 0;
		}
		A[i][i] += 1;
	}
	//
	for(k=N;k>=1;k--){
		//迭代30次
		for(its=1;its<=30;its++){
			for(l=k;l>=1;l--){
				nm = l - 1;
				if(abs(RV1[l]) + anorm == anorm)
					goto l2;
				if(abs(W[nm]) + anorm == anorm)
					goto l1;
			}
l1:			c = 0;
			s = 1;
			for(i=l;i<=k;i++){
				f = s * RV1[i];
				if(abs(f) + anorm != anorm){
					g = W[i];
					h = sqrt(f * f + g * g);
					W[i] = h;
					h = 1.0 / h;
					c = g * h;
					s = -f * h;
					for(j=1;j<=M;j++){
						y = A[j][nm];
						z = A[j][i];
						A[j][nm] = y * c + z * s;
						A[j][i] = - y * s + z * c;
					}
				}
			}
l2:			z = W[k];
			if(l == k){
				if(z < 0){
					W[k] = -z;
					for(j=1;j<=N;j++)
						V[j][k] = -V[j][k];
				}
				goto l3;
			}
			if(its == 30)
				return false;
			x = W[l];
			nm = k - 1;
			y = W[nm];
			g = RV1[nm];
			h = RV1[k];
			f = ((y-z)*(y+z) + (g-h)*(g+h)) / (2 * h * y);
			g = sqrt(f * f + 1);
			sgn = (f > 0) ? 1 : -1;
			f = ((x-z)*(x+z) + h * ((y / (f+abs(g)*sgn)) - h)) / x;
			c = s = 1;
			tmp = nm;
			for(j=l;j<=tmp;j++){
				i = j + 1;
				g = RV1[i];
				y = W[i];
				h = s * g;
				g = g * c;
				z = sqrt(f * f + h * h);
				RV1[j] = z;
				c = f / z;
				s = h / z;
				f = (x*c) + (g*s);
				g = -(x*s) + (g*c);
				h = y * s;
				y = y * c;
				for(nm=1;nm<=N;nm++){
					x = V[nm][j];
					z = V[nm][i];
					V[nm][j] = (x * c) + (z * s);
					V[nm][i] = -(x * s) + (z * c);
				}
				z = sqrt(f * f + h * h);
				W[j] = z;
				if(z != 0){
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = -(s * g) + (c * y);
				for(nm=1;nm<=M;nm++){
					y = A[nm][j];
					z = A[nm][i];
					A[nm][j] = (y*c) + (z*s);
					A[nm][i] = -(y*s) + (z*c);
				}
			}
			RV1[l] = 0;
			RV1[k] = f;
			W[k] = x;
		}
l3:		continue;
	}
	return true;
}

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
	for(k=1;k<=N;k++){
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
			continue;
		A[k][k] -= a;
		for(i=k;i<=N;i++)
			A[i][k] /= f;
		//逐列变换A
		for(j=k+1;j<=N;j++){
			f = 0;
			for(i=k;i<=N;i++)
				f += 2 * A[i][j] * A[i][k];
			for(i=k;i<=N;i++)
				A[i][j] -= f * A[i][k];
		}
		//逐列变换Q
		for(j=0;j<=N;j++){
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
