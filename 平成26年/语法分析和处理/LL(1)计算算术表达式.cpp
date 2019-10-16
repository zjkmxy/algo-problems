/*
LL(1)�㷨�����۲�����ƪ���ӣ�http://blog.csdn.net/fengguoyousheng/article/details/6070701

���ʽ�����ķ�������������������һ����Ԫ����������ݹ飬���ǿհף�i�����֣�
���׿����Ƶ�˳����Ǽ���˳�򣬵��������﷨�����Ǳ��ʽ�ļ��������
[Eexpreesion]  =>  [Term][GResidue]
[GResidue]  =>  +[Term][GResidue] | -[Term][GResidue] | ��
[Term]  =>  [Factor][UResidue]
[UResidue]  =>  *[Factor][UResidue] | /[Factor][UResidue] | %[Factor][UResidue] | ��
[Factor]  =>  ([Eexpreesion])[RResidue] | i[RResidue]
[RResidue] => ^[Factor] | ��
���Ժ���ս��ȫ��������ĸ��ʾ��

First��ʾ����ʽ�Ŀ�ͷ���ս����Follow��ʾ����ʽ�ĺ�����ܸ�����Щ�ս��
	FIRST	FOLLOW
E	(i		)=
G	+-��	)=
T	(i		+-)=
U	/ *��	+-)=
F	(i		+-/ *%)=
R	^��		+-/ *%)=

����Ĺ����ʾ����Ӧ������Ƶ�
��ߵĹ�ʽ���õ������ǣ�����ķ���Ϊ�ұߵķ���֮һ
E	=>	TG		(i
G	=>	+TG		+
G	=>	-TG		-
G	=>	��		)=
T	=>	FU		(i
U	=>	*FU		*
U	=>	/FU		/
U	=>	%FU		%
U	=>	��		+-)=
F	=>	(E)R	(
F	=>	iR		i
R	=>	^F		^
R	=>	��		+-/ *%)=
*/
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<cctype>
#include<cstring>
using namespace std;

#define MAXL 10000

//�������ʽ����
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

	TOKEN ReadToken();		//����һ���﷨��Ԫ��������������֣�
	bool Factor(int &val);	//����F��R
	bool Term(int &val);	//����T��U
	bool Expression(int &val);//����E��G

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
	if(str[n] != '=')	//��������ֹ��
	{
		str[n] = '=';
		str[n+1] = NULL;
	}
	if(!Expression(val))	//������Ǵ�E=��ʼ����չ���Ƶ�
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

	//�������ж�
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
