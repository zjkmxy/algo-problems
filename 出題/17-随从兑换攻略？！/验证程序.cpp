#include<cstdio>
#include<cctype>
#include<cstring>
using namespace std;

#define MAXN	18
#define MAXC	145
#define MAXS	262144
struct MINION{
	char name[MAXC];
	int a, h;
	bool ds, used;
}mins[MAXN];
int pre[MAXN][MAXS];	//前一步的位置
int dp[MAXN][MAXS];		//最优的结果
int rate[MAXN][MAXN];	//兑换比率（限正向）
int N;
//计算汇率
int CalcRate(int A, int B)
{
	int ret = 0, h1 = mins[A].h, h2 = mins[B].h;
	bool ds1 = mins[A].ds, ds2 = mins[B].ds;
	while(h1 > 0){
		if(ds1)
			ds1 = false;
		else
			h1 -= mins[B].a;
		if(ds2)
			ds2 = false;
		else
			h2 -= mins[A].a;
		if(h2 <= 0){
			ret++;
			h2 = mins[B].h;
		}
	}
	return ret;
}
//检索随从
int FindMinion(char *str)
{
	int i;
	for(i=0;i<N;i++)
		if(strcmp(mins[i].name, str) == 0)
			return (mins[i].used ? -1 : i);
	return -1;
}
//检查答案合法性，计算长度和数值
bool Check(FILE *fp, int& len, int& val)
{
	char str[MAXC];
	int cur, pos, val0, tmp;

	pos = -1;
	val = 1;
	len = 1;
	for(cur=0;cur<N;cur++)
		mins[cur].used = false;
	while(fscanf(fp, "%d", &tmp) != EOF){
		cur = pos;
		val0 = val;
		//检查格式
		if(fgetc(fp) != ' ')
			return false;
		if(fgetc(fp) != '*')
			return false;
		if(fgetc(fp) != ' ')
			return false;
		if(fscanf(fp, "%s", str) == EOF)
			return false;
		//检查生物和汇率
		pos = FindMinion(str);
		if(pos < 0)
			return false;
		mins[pos].used = true;
		val = tmp;
		if(cur == -1 && val != 1)
			return false;
		if(cur >= 0 && val > val0 * rate[cur][pos])
			return false;
		if(fgetc(fp) == '\n')
			break;
		else{
			len++;
			if(fgetc(fp) != '=')
				return false;
		}
	}
	return (fgetc(fp) == -1);
}

FILE *fin, *fout, *fjug;
int main(int argc, char *argv[], char *env[])
{
	int i, j, l1, v1, l2, v2;
	char ch[3];

	//打开输入、输出和评测文件（实际上除了JSE的输出是WA）
	fin = fopen(argv[1], "r");
	if(!fin){
		printf("JSE");
		return -1;
	}
	fout = fopen(argv[2], "r");
	if(!fin){
		printf("JSE");
		fclose(fin);
		return -1;
	}
	fjug = fopen(argv[3], "r");
	if(!fin){
		printf("JSE");
		fclose(fout);
		fclose(fin);
		return -1;
	}
	//读入基本的随从信息，计算汇率
	fscanf(fin, "%d",&N);
	for(i=0;i<N;i++){
		fscanf(fin, "%s %d %d %s", mins[i].name, &mins[i].a, &mins[i].h, ch);
		mins[i].ds = (ch[0] == 'D');
	}
	fclose(fin);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			rate[i][j] = CalcRate(i, j);
	if(!Check(fout, l1, v1)){
		printf("PE");
		fclose(fjug);
		fclose(fout);
		return -1;
	}
	if(Check(fjug, l2, v2) && l2 >= l1 && v2 >= v1)
		printf("AC");
	else
		printf("WA");

	fclose(fjug);
	fclose(fout);
	return 0;
}
