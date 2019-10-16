#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

#define MAXL 1000

//计算一个只含N的算数表达式的类，在标程的算法中没有使用到它，但是其他方法中会用到。
class CALER
{
private:
	char str[MAXL+1];
	int pos;
	struct TOKEN
	{
		int i;
		char op;
	};
	TOKEN list[MAXL];
	int cnt;

	TOKEN ReadToken();
	bool Factor();
	bool Term();
	bool Expression();

public:
	CALER();
	int calc(int x);
};
CALER *pcal;

CALER::CALER()
{
	int n;
	scanf("%s",str);
	pos = 0;
	cnt = 0;
	n = strlen(str);
	if(str[n] != '=')
	{
		str[n] = '=';
		str[n+1] = NULL;
	}
	if(!Expression())
	{
		throw 0;
	}
}

CALER::TOKEN CALER::ReadToken()
{
	char num[10];
	int i = 0;
	TOKEN ret;

	while(isdigit(str[pos]))
	{
		num[i] = str[pos];
		i++;
		pos++;
	}
	if(i == 0)
	{
		ret.i = 0;
		ret.op = str[pos];
		pos++;
	}else{
		num[i] = 0;
		ret.i = atoi(num);
		ret.op = 0;
	}
	if(ret.op == 'N')
	{
		ret.op = 0;
		ret.i = -1;
	}

	return ret;
}

bool CALER::Factor()
{
	TOKEN token;
	token = ReadToken();
	switch(token.op)
	{
	case '(':
		if(!Expression())
			return false;
		token = ReadToken();
		if(token.op != ')')
			return false;
		break;
	case 0:
		list[cnt] = token;
		cnt++;
		break;
	default:
		return false;
	}
	token = ReadToken();
	if((token.op == '+')||(token.op == '-')||(token.op == '*')||(token.op == '/')||(token.op == ')')||(token.op == '='))
	{
		pos--;
		return true;
	}
	if(token.op == '^')
	{
		if(!Factor())
			return false;
		list[cnt] = token;
		cnt++;
		return true;
	}
	return false;
}

bool CALER::Term()
{
	TOKEN token;
	if(!Factor())
		return false;
	while(true)
	{
		token = ReadToken();
		if((token.op == '*')||(token.op == '/'))
		{
			if(!Factor())
				return false;
			list[cnt] = token;
			cnt++;
			continue;
		}
		if((token.op == '+')||(token.op == '-')||(token.op == ')')||(token.op == '='))
		{
			pos--;
			break;
		}
		return false;
	}
	return true;
}

bool CALER::Expression()
{
	TOKEN token;
	if(!Term())
		return false;
	while(true)
	{
		token = ReadToken();
		if((token.op == '+')||(token.op == '-'))
		{
			if(!Term())
				return false;
			list[cnt] = token;
			cnt++;
			continue;
		}
		if((token.op == ')')||(token.op == '='))
		{
			pos--;
			break;
		}
		return false;
	}
	return true;
}

int CALER::calc(int x)
{
	int st[MAXL], top = 0;
	int i, tmp;

	for(i=0;i<cnt;i++)
	{
		if(list[i].op == 0)
		{
			if(list[i].i == -1)
				st[top] = x;
			else
				st[top] = list[i].i;
			top++;
		}else{
			if(list[i].op == '+')
				st[top-2] = st[top-2] + st[top-1];
			if(list[i].op == '*')
				st[top-2] = st[top-2] * st[top-1];
			if(list[i].op == '^')
			{
				tmp = 1;
				while(st[top-1]--)
					tmp *= st[top-2];
				st[top-2] = tmp;
			}
			top--;
		}
	}
	return st[0];
}

//双侧5%的T统计量表（0的是Z检验）
double t_tab[] = 
{
	1.960,
	12.71, 4.303, 3.182, 2.776, 2.571, 2.447, 2.365, 2.306, 2.262, 2.228,
	2.201, 2.179, 2.160, 2.145, 2.131, 2.120, 2.110, 2.101, 2.093, 2.086,
	2.080, 2.074, 2.069, 2.064, 2.060, 2.056, 2.052, 2.048, 2.045, 2.042,
};

//统计学量的计算类，注意我们不需要保存每一个数就可以进行T检验
class STATER
{
private:
	int cnt;
	double sum;
	double sum2;
public:
	STATER():sum(0),sum2(0),cnt(0){};
	void add(double x)
	{
		sum += x;
		sum2 += x * x;
		cnt++;
	};
	double getMean()
	{
		return (cnt==0) ? 0 : (sum/cnt);
	};
	double getSSE()
	{
		double ret;
		if(cnt <= 1)
			return 0;
		ret = sum / cnt;
		ret = sum2 / cnt - ret * ret;
		ret = sqrt(ret / (cnt - 1));
		return ret;
	};
	double getTS(double test)
	{
		double ret;
		if(cnt <= 1)
			throw 0;
		ret = (getMean() - test) / getSSE();
		return ret;
	};
	bool StudentTest(double test)
	{
		double val = (cnt <= 30) ? t_tab[cnt] : t_tab[0];
		return (getTS(test) <= val);
	};
};

int main()
{
	STATER *pst;
	double a, b;
	pst = new STATER();
	scanf("%*s");
	while(scanf("%*s%lf%lf",&a,&b)!=EOF)
	{
		pst->add(a / b);
	}
	printf(pst->StudentTest(1.0) ? "NO\n" : "YES\n");
	return 0;
}