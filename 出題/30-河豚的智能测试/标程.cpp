#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

typedef long long int LL;
#define MAXC 401
#define MAXL 11
#define MAXN 100
#define ALB  65
#define INF  100000000

typedef double vector2[MAXC];
typedef vector2 matrix2[MAXC];
matrix2 Mat;
vector2 Res;
int idx[MAXN];
//三角分解A
//输入：阶数 N，矩阵A
//输出：A为三角分解结果（下L上半U）；INDX：行交换结果
bool LUDCMP(matrix2 A, const int n, int indx[])
{
	double VV[MAXC], aamax, sum, dum;
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
			A[j][j] = 1e-20;
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


class ACA
{
public:
	struct NODE
	{
		char ch;		//当前字符
		int fail;		//失败指针
		int next[ALB];	//转移指针
		bool ed;		//终结状态
		inline int& operator[](int i){return next[i];};
		bool vis;
	}node[MAXC];
	int cnt;
	void init()
	{
		cnt = 1;
		memset(&node[0], -1, sizeof(NODE));
		node[0].ed = false;
	}
	void insert(char *str, int len)
	{
		int i, cur = 0;
		for(i=0;i<len;i++){
			if(node[cur][str[i]] == -1){
				node[cur][str[i]] = cnt;
				memset(&node[cnt], -1, sizeof(NODE));
				node[cnt].ch = str[i];
				node[cnt].ed = false;
				cnt++;
			}
			cur = node[cur][str[i]];
		}
		node[cur].ed = true;
	}
	void build()
	{
		queue<int> Q;
		int cur, j, p;
		for(j=0;j<ALB;j++){
			if(node[0][j] >= 0){
				Q.push(node[0][j]);
				node[node[0][j]].fail = 0;
			}else
				node[0][j] = 0;
		}
		while(!Q.empty()){
			cur = Q.front();
			Q.pop();
			p = node[cur].fail;
			for(j=0;j<ALB;j++){
				if(node[cur][j] >= 0){
					node[node[cur][j]].fail = node[p][j];
					node[node[cur][j]].ed |= node[node[p][j]].ed;
					Q.push(node[cur][j]);
				}else
					node[cur][j] = node[p][j];
			}
		}
	}
	double make_mat()
	{
		int i, j;
		memset(Mat, 0, sizeof(Mat));
		memset(Res, 0, sizeof(Res));
		for(i=0;i<cnt;i++)
		{
			if(node[i].ed)
			{
				Mat[i+1][i+1] = 1.;
				Res[i+1] = 0.;
			}else{
				Mat[i+1][i+1] = ALB;
				Res[i+1] = ALB;
				for(j=0;j<ALB;j++)
				{
					Mat[i+1][node[i][j]+1] += -1.;
				}
			}
		}
		LUDCMP(Mat, cnt, idx);
		LUBKSB(Mat, cnt, idx, Res);
		return Res[1];
	}
}aca;



char str[MAXL];
int len;

void deal()
{
	int i;
	len = strlen(str);
	for(i=0;i<len;i++)
	{
		if(islower(str[i]))
			str[i] = str[i] - 'a' + 0;
		else if(isupper(str[i]))
			str[i] = str[i] - 'A' + 26;
		else if(isdigit(str[i]))
			str[i] = str[i] - '0' + 52;
		else if(str[i] == '+')
			str[i] = 62;
		else if(str[i] == '/')
			str[i] = 63;
		else if(str[i] == '=')
			str[i] = 64;
	}
}
int main()
{
	int N;
	double ret;
	scanf("%d",&N);
	aca.init();
	while(N--)
	{
		scanf("%s",str);
		deal();
		aca.insert(str, len);
	}
	aca.build();
	ret = aca.make_mat();
	if(ret < 100000000)
		printf("%.2lf\n",ret);
	else
		printf("\"Poor River Dolphin!\"\n");
	return 0;
}