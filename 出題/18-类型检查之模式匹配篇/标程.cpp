#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"

#include<cstdio>
#include<cctype>
#include<cstring>
#include<list>
using namespace std;

#define MAX_NAME	20
#define MAX_LINE	1000
#define MAX_DT		200
#define MAX_PT		200
#define MAX_TYPE	200

typedef void	(*LPVOID);
typedef int		TYPE;
typedef int		PATT;

typedef struct DATATYPE{
	int type;				//�������ı��
	char cons[MAX_NAME];	//������
	TYPE args;				//�������ͣ���ţ�
}*PDATATYPE;
char tpname[MAX_DT][MAX_NAME];	//������
DATATYPE data[MAX_TYPE];
int ndat, ncons;	//�࣬�������

typedef struct PATTERN{
	PATT t;
	list<int> *tp;		//Tuple��ģʽ����ţ�
	int dt;				//Datatype�����ͺ�
	int dtarg;			//Datatype�Ĳ�����patt�ı�ţ�
}*PPATTERN;
PATTERN	patt[MAX_DT];
int npat;

typedef struct TYPEDAT{
	TYPE t;
	list<struct TYPEDAT*> *tp;	//Tuple�����ͣ���ţ�
	int dt;						//��������ͺ�
}*PTYPEDAT;
typedef list<PTYPEDAT> TPLIST;
TYPEDAT typ[MAX_TYPE];
TYPEDAT ans[MAX_DT];
int ntyp, nans;
//��Ϊ���������ԣ����Ա���������������������ֵ�����洢
const TYPE TYPE_UNIT	= 0;
const TYPE TYPE_ANY		= 1;
const TYPE TYPE_INT		= 2;
const TYPE TYPE_TUPLE	= 3;
const TYPE TYPE_DATATYPE= 4;

const PATT PAT_UNIT		= 0;
const PATT PAT_WILDCARD	= 1;
const PATT PAT_CONST	= 2;
const PATT PAT_TUPLE	= 3;
const PATT PAT_CONSTRUCT= 4;
const PATT PAT_VARIABLE	= 5;

//������
char lines[MAX_LINE];
char token[MAX_NAME];
int pos;

bool read_token()
{
	char ch = ' ';
	int i;

	//���ǰ��ռλ��
	ch = lines[pos++];
	while(ch == ' ' || ch == '\t')
		ch = lines[pos++];
	if(ch == 0){
		gets(lines);
		pos = 0;
		return false;
	}
	token[0] = ch;
	//�������
	if(isdigit(ch)){
		ch = lines[pos++];
		for(i=1;isdigit(ch);i++){
			token[i] = ch;
			ch = lines[pos++];
		}
		pos--;
		token[i] = 0;
	}else if(isalpha(ch)){
		ch = lines[pos++];
		for(i=1;isalpha(ch) || isdigit(ch);i++){
			token[i] = ch;
			ch = lines[pos++];
		}
		pos--;
		token[i] = 0;
	}else{
		if(token[0] == '\''){
			token[1] = lines[pos++];	//'a
			token[2] = 0;
		}else
			token[1] = 0;
	}
	return true;
}

//����һ������
int read_type()
{
	int ret = ntyp, i;
	ntyp++;

	read_token();
	if(token[0] == '\''){
		typ[ret].t = TYPE_ANY;
	}else if(strcmp(token, "int") == 0){
		typ[ret].t = TYPE_INT;
	}else if(strcmp(token, "unit") == 0){
		typ[ret].t = TYPE_UNIT;
	}else if(token[0] == '('){
		//Tuple
		typ[ret].t = TYPE_TUPLE;
		typ[ret].tp = new TPLIST();
		while(token[0] != ')'){
			typ[ret].tp->push_back(typ+read_type());
			read_token();	//, )
		}
	}else{
		//Datatype����
		typ[ret].t = TYPE_DATATYPE;
		for(i=0;i<ndat;i++){
			if(strcmp(tpname[i], token) == 0){
				typ[ret].dt = i;
				return ret;
			}
		}
		throw 0;	//��Ч����
	}
	return ret;
}

void free_type()
{
	int i;
	for(i=0;i<ntyp;i++){
		if(typ[i].t == TYPE_TUPLE)
			delete typ[i].tp;
	}
	ntyp = 0;
	for(i=0;i<nans;i++){
		if(ans[i].t == TYPE_TUPLE)
			delete ans[i].tp;
	}
	nans = 0;
}

//����һ����������
int read_datatype()
{
	int ret, cur;

	ret = ndat;
	ndat++;
	read_token();	//type
	read_token();
	strcpy(tpname[ret], token);
	read_token();	//=
	while(true){
		cur = ncons;
		ncons++;
		data[cur].type = ret;
		read_token();
		strcpy(data[cur].cons, token);
		data[cur].args = -1;
		if(!read_token())	//of |
			break;
		if(strcmp(token, "of") == 0){
			data[cur].args = read_type();
			if(!read_token())	// |
				break;
		}
	}
	return ret;
}

//���ҹ���
int find_cons()
{
	int ret;
	for(ret=0;ret<ncons;ret++){
		if(strcmp(data[ret].cons, token) == 0)
			return ret;
	}
	return -1;
}

//����һ��ģʽ
int read_pattern()
{
	int ret = npat, con;
	bool end;	//�����ǲ�����ֹ�� , ) \0
	npat++;

	while(!read_token());
	if(isdigit(token[0])){
		patt[ret].t = PAT_CONST;
	}else if(token[0] == '_'){
		patt[ret].t = PAT_WILDCARD;
	}else if(token[0] == '('){
		while(lines[pos] == ' ' || lines[pos] == '\t')
			pos++;
		if(lines[pos] == ')'){
			patt[ret].t = PAT_UNIT;
			read_token();
		}else{
			//Tuple
			patt[ret].t = PAT_TUPLE;
			patt[ret].tp = new list<int>();
			while(token[0] != ')'){
				patt[ret].tp->push_back(read_pattern());
				read_token();	//, )
			}
		}
	}else{
		//������߱���
		while(lines[pos] == ' ' || lines[pos] == '\t')
			pos++;
		end = (lines[pos] == ',' || lines[pos] == ')' || lines[pos] == '\0');
		con = find_cons();
		if(con >= 0){
			//����
			if(data[con].args == -1){
				patt[ret].t = PAT_CONSTRUCT;
				patt[ret].dt = con;
				patt[ret].dtarg = -1;	//�޲���
				if(!end){
					throw 0;	//������
				}
			}else{
				if(end){
					throw 0;	//������ʹ�ù�����Shadow
				}else{
					patt[ret].t = PAT_CONSTRUCT;
					patt[ret].dt = con;
					patt[ret].dtarg = read_pattern();
				}
			}
		}else{
			//�����Ǳ������߲����ڵĹ���
			if(!end)
				throw 0;
			patt[ret].t = PAT_VARIABLE;
		}
	}
	return ret;
}

//���ÿ��һ�ж�Ҫ�����
void free_pattern()
{
	int i;
	for(i=0;i<npat;i++){
		if(patt[i].t == PAT_TUPLE)
			delete patt[i].tp;
	}
	npat = 0;
}

//���ans[T]
void print_match(PTYPEDAT T)
{
	TPLIST::iterator it;
	switch(T->t){
	case TYPE_ANY:
		printf("\'a");
		break;
	case TYPE_UNIT:
		printf("unit");
		break;
	case TYPE_INT:
		printf("int");
		break;
	case TYPE_TUPLE:
		printf("(");
		for(it=T->tp->begin();it!=T->tp->end();it++){
			if(it != T->tp->begin())
				printf("*");
			print_match(*it);
		}
		printf(")");
		break;
	case TYPE_DATATYPE:
		printf("%s",tpname[T->dt]);
		break;
	}
}

int copy_type(PTYPEDAT T)
{
	int ret = nans;
	TPLIST::iterator it;
	nans++;
	ans[ret].t = T->t;
	ans[ret].dt = T->dt;
	if(ans[ret].t == TYPE_TUPLE){
		ans[ret].tp = new TPLIST();
		for(it=T->tp->begin();it!=T->tp->end();it++){
			ans[ret].tp->push_back(ans + copy_type(*it));
		}
	}
	return ret;
}

//����ǰ��T��P����ƥ�䲢������ʧ�����쳣
void match(PTYPEDAT T, PPATTERN P)
{
	list<int>::iterator it;
	TPLIST::iterator it2;
	int cur;

	switch(P->t){
	case PAT_WILDCARD:
	case PAT_VARIABLE:
		return ;
	case PAT_UNIT:
		if(T->t == TYPE_ANY || T->t == TYPE_UNIT){
			T->t = TYPE_UNIT;
			return ;
		}else
			throw 0;
	case PAT_CONST:
		if(T->t == TYPE_ANY || T->t == TYPE_INT){
			T->t = TYPE_INT;
			return ;
		}else
			throw 0;
	case PAT_TUPLE:
		if(T->t == TYPE_ANY){
			//������֮
			T->t = TYPE_TUPLE;
			T->tp = new TPLIST();
			for(it=P->tp->begin();it!=P->tp->end();it++){
				cur = nans;
				nans++;
				ans[cur].t = TYPE_ANY;
				match(ans+cur, patt+(*it));
				T->tp->push_back(ans+cur);
			}
			return ;
		}else if(T->t == TYPE_TUPLE){
			//����ƥ��
			if(P->tp->size() != T->tp->size())
				throw 0;
			for(it=P->tp->begin(),it2=T->tp->begin();it!=P->tp->end();it++,it2++){
				match(*it2, patt+(*it));
			}
			return ;
		}else
			throw 0;
	case PAT_CONSTRUCT:
		if(T->t != TYPE_ANY && T->t != TYPE_DATATYPE)
			throw 0;
		//���Ʊ�֤���޸�
		cur = copy_type(typ+data[P->dt].args);
		match(ans+cur, patt+P->dtarg);
		if(T->t == TYPE_ANY){
			T->t = TYPE_DATATYPE;
			T->dt = data[P->dt].type;
		}else{
			if(T->dt != data[P->dt].type)
				throw 0;
		}
		return ;
	}
}

int main()
{
	int N, i;

	try{
		scanf("%d\n", &N);
		gets(lines);
		pos = 0;
		for(i=0;i<N;i++)
			read_datatype();
		sscanf(lines, "%d\n", &N);
		gets(lines);
		pos = 0;
		nans = 1;
		ans[0].t = TYPE_ANY;
		printf("0: 'a\n");
		for(i=1;i<=N;i++){
			read_pattern();
			match(ans, patt);
			printf("%d: ",i);
			print_match(ans);
			printf("\n");
			free_pattern();
		}
	}catch(int ex)
	{
		if(ex != 0)
			throw -1;
		else
			printf("ERROR!\n");
	}
	free_type();
	return 0;
}