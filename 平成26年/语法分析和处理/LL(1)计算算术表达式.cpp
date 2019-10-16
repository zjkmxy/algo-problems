/*
LL(1)算法的理论参照这篇帖子：http://blog.csdn.net/fengguoyousheng/article/details/6070701

表达式正则文法（方括号括起来的是一个单元，已消除左递归，ε是空白，i是数字）
容易看出推导顺序就是计算顺序，但产生的语法树不是表达式的计算二叉树
[Eexpreesion]  =>  [Term][GResidue]
[GResidue]  =>  +[Term][GResidue] | -[Term][GResidue] | ε
[Term]  =>  [Factor][UResidue]
[UResidue]  =>  *[Factor][UResidue] | /[Factor][UResidue] | %[Factor][UResidue] | ε
[Factor]  =>  ([Eexpreesion])[RResidue] | i[RResidue]
[RResidue] => ^[Factor] | ε
（以后非终结符全部用首字母表示）

First表示产生式的开头的终结符，Follow表示产生式的后面可能跟着那些终结符
	FIRST	FOLLOW
E	(i		)=
G	+-ε	)=
T	(i		+-)=
U	/ *ε	+-)=
F	(i		+-/ *%)=
R	^ε		+-/ *%)=

下面的规则表示我们应该如何推导
左边的公式运用的条件是：读入的符号为右边的符号之一
E	=>	TG		(i
G	=>	+TG		+
G	=>	-TG		-
G	=>	ε		)=
T	=>	FU		(i
U	=>	*FU		*
U	=>	/FU		/
U	=>	%FU		%
U	=>	ε		+-)=
F	=>	(E)R	(
F	=>	iR		i
R	=>	^F		^
R	=>	ε		+-/ *%)=
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<cctype>
#include<cstring>
using namespace std;

#define MAXL 10000

//算数表达式的类
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

	TOKEN ReadToken();		//读入一个语法单元（运算符或者数字）
	bool Factor(int &val);	//处理F和R
	bool Term(int &val);	//处理T和U
	bool Expression(int &val);//处理E和G

public:
	int calc();
};
CALER pcal;

int CALER::calc()
{
	int n, val;
	scanf("%s",str);
	pos = 0;
	n = strlen(str);
	if(str[n] != '=')	//增加右终止符
	{
		str[n] = '=';
		str[n+1] = NULL;
	}
	if(!Expression(val))	//最初我们从E=开始向下展开推导
		throw 0;
	if(ReadToken().op != '=')
		throw 0;
	return val;
}

CALER::TOKEN CALER::ReadToken()
{
	char num[10];
	int i = 0;
	bool neg = false;
	TOKEN ret;

	//负数的判定
	if(str[pos] == '-' && (pos == 0 || !isdigit(str[pos-1]))){
		neg = true;
		pos++;
	}
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
		if(neg)
			ret.i = - ret.i;
	}

	return ret;
}

bool CALER::Factor(int& val)
{
	TOKEN token;
	int v1, v2;

	token = ReadToken();
	switch(token.op)
	{
	case '(':
		if(!Expression(v1))
			return false;
		token = ReadToken();
		if(token.op != ')')
			return false;
		break;
	case 0:
		v1 = token.i;
		break;
	default:
		return false;
	}
	token = ReadToken();
	if((token.op == '+')||(token.op == '-')||(token.op == '*')||(token.op == '/')||(token.op == '%')||(token.op == ')')||(token.op == '='))
	{
		pos--;
		val = v1;
		return true;
	}
	if(token.op == '^')
	{
		if(!Factor(v2))
			return false;
		if(v2 < 0)
			throw -2;
		val = 1;
		while(v2 > 0){
			val *= v1;
			v2--;
		}
		return true;
	}
	return false;
}

bool CALER::Term(int& val)
{
	TOKEN token;
	int v1, v2;

	if(!Factor(v1))
		return false;
	while(true)
	{
		token = ReadToken();
		if((token.op == '*')||(token.op == '/')||(token.op == '%'))
		{
			if(!Factor(v2))
				return false;
			switch(token.op){
			case '*':
				v1 *= v2;
				break;
			case '/':
				if(v2 == 0)
					throw -1;
				v1 /= v2;
				break;
			case '%':
				if(v2 == 0)
					throw -1;
				v1 %= v2;
				break;
			}
			continue;
		}
		if((token.op == '+')||(token.op == '-')||(token.op == ')')||(token.op == '='))
		{
			pos--;
			break;
		}
		return false;
	}
	val = v1;
	return true;
}

bool CALER::Expression(int& val)
{
	TOKEN token;
	int v1, v2;

	if(!Term(v1))
		return false;
	while(true)
	{
		token = ReadToken();
		if((token.op == '+')||(token.op == '-'))
		{
			if(!Term(v2))
				return false;
			switch(token.op){
			case '+':
				v1 += v2;
				break;
			case '-':
				v1 -= v2;
				break;
			}
			continue;
		}
		if((token.op == ')')||(token.op == '='))
		{
			pos--;
			break;
		}
		return false;
	}
	val = v1;
	return true;
}

int main()
{
	int T, val;

	scanf("%d",&T);
	while(T--){
		try{
			val = pcal.calc();
			printf("%d\n",val);
		}catch(int x){
			if(x == 0)
				printf("error.\n");
			else if(x == -1)
				printf("Divide 0.\n");
			else if(x == -2)
				printf("error.\n");
			else throw -1;
		}
	}
	return 0;
}
