#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

double minv[3];
const double PI = 3.1415926535897932384626;

typedef double FLOAT4[4];
typedef double FLOAT3[3];
typedef FLOAT4 QUATERNION;

//归一化矢量
void VecNormalize(double Vec[], int dim){
	int i;
	double mag = 0;

	for(i = 0; i<dim; i++)
		mag += Vec[i] * Vec[i];
	if((mag != 0.f) && (fabs(mag - 1.0f) > 1e-8)){
		mag = sqrt(mag);
		for(i = 0; i < dim; i++)
			Vec[i] /= mag;
	}
}

//四元数的乘法
void QuatProduct(QUATERNION Dst, QUATERNION A, QUATERNION B){
	QUATERNION tmp;

	tmp[0] = A[3] * B[0] + A[0] * B[3] - A[1] * B[2] + A[2] * B[1];
	tmp[1] = A[3] * B[1] + A[1] * B[3] - A[2] * B[0] + A[0] * B[2];
	tmp[2] = A[3] * B[2] + A[2] * B[3] - A[0] * B[1] + A[1] * B[0];
	tmp[3] = A[3] * B[3] - A[0] * B[0] - A[1] * B[1] - A[2] * B[2];
	Dst[0] = tmp[0], Dst[1] = tmp[1], Dst[2] = tmp[2], Dst[3] = tmp[3];
}

//要求(x,y,z)是归一化矢量
void QuatFromAxis(QUATERNION Dst, double x, double y, double z, double ang){
	ang /= 2.0f;
	Dst[3] = cos(ang);
	ang = sin(ang);
	Dst[0] = x * ang;
	Dst[1] = y * ang;
	Dst[2] = z * ang;
}

//对特定矢量应用四元数旋转
void QuatApply(FLOAT3 Dst, FLOAT3 Src, QUATERNION Rot){
	QUATERNION tmp, tmp2;

	tmp[0] = -Rot[0], tmp[1] = -Rot[1], tmp[2] = -Rot[2], tmp[3] = Rot[3];
	tmp2[0] = Src[0], tmp2[1] = Src[1], tmp2[2] = Src[2], tmp2[3] = 0.0f;
	QuatProduct(tmp2, tmp, tmp2);
	QuatProduct(tmp2, tmp2, Rot);
	Dst[0] = tmp2[0], Dst[1] = tmp2[1], Dst[2] = tmp2[2];
}

bool domain(){
	double x, y, z, p, q, r;
	FLOAT3 pos;
	QUATERNION rot1, rot2;
	int N, M;

	if(scanf("%d", &N) == EOF)
		return false;
	minv[1] = - PI * 10;
	while(N --){
		scanf("%lf%lf%lf%lf%lf%lf%d", &x, &y, &z, &p, &q, &r, &M);
		QuatFromAxis(rot1, 0, 0, 1, p / 180.0 * PI);
		QuatFromAxis(rot2, 1, 0, 0, q / 180.0 * PI);
		QuatProduct(rot1, rot2, rot1);
		QuatFromAxis(rot2, 0, 0, 1, r / 180.0 * PI);
		QuatProduct(rot1, rot2, rot1);
		while(M --){
			scanf("%lf%lf%lf", &pos[0], &pos[1], &pos[2]);
			QuatApply(pos, pos, rot1);
			pos[0] += x, pos[1] += y, pos[2] += z;
			VecNormalize(pos, 3);

			if(minv[1] <= pos[1]){
				minv[0] = pos[0], minv[1] = pos[1], minv[2] = pos[2];
			}
		}
	}
	//printf("%.3lf %.3lf %.3lf\n", minv[0], minv[1], minv[2]);
	printf("%.3lf\n", acos(minv[1]) * 180.0 / PI);

	return true;
}

int main(){
	while(domain());
	return 0;
}
