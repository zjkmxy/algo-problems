/*
E  =>  TG
G  =>  +TG|-TG|^
T  =>  FU
U  =>  *FU|/FU|^
F  =>  (E)|i

    FIRST    FOLLOW
E   (i       )=
G   +-^      )=
T   (i       +-)=
U   /*^      +-)=
F   (i       +-/*)=

SELECT:
E  =>  TG   (i
G  =>  +TG  +
G  =>  -TG  -
G  =>  ^    )=
T  =>  FU   (i
U  =>  *FU  *
U  =>  /FU  /
U  =>  ^    +-)=
F  =>  (E)  (
F  =>  i    i
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

char str[1000];
int pos = 0;

struct TOKEN
{
    int i;
    char op;
};

TOKEN ReadToken();
int Factor();
int Term();
int Expression();

TOKEN ReadToken()
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
    
    return ret;
}

int Factor()
{
     TOKEN token;
     token = ReadToken();
     switch(token.op)
     {
     case '(':
          if(Expression())
              return 1;
          token = ReadToken();
          if(token.op != ')')
              return 1;
          break;
     case 0:
          printf("%d ",token.i);
          break;
     default:
          return 1;
     }
     return 0;
}

int Term()
{
    TOKEN token;
    if(Factor())
       return 1;
    while(1)
    {
        token = ReadToken();
        if((token.op == '*')||(token.op == '/'))
        {
            if(Factor())
                return 1;
            printf("%c ",token.op);
			continue;
        }
        if((token.op == '+')||(token.op == '-')||(token.op == ')')||(token.op == '='))
        {
            pos--;
            break;
        }
        return 1;
    }
    return 0;
}

int Expression()
{
    TOKEN token;
    if(Term())
       return 1;
    while(1)
    {
        token = ReadToken();
        if((token.op == '+')||(token.op == '-'))
        {
            if(Term())
                return 1;
            printf("%c ",token.op);
			continue;
        }
        if((token.op == ')')||(token.op == '='))
        {
            pos--;
            break;
        }
        return 1;
    }
    return 0;
}

int main()
{
	int n;
    while(scanf("%s",str) != EOF)
	{
		pos = 0;
		n = strlen(str);
		if(str[n-1] != '=')
		{
			str[n] = '=';
			str[n+1] = 0;
		}
	    if(Expression())
	        printf("\nError!\n");
	    else
	        printf("\n");
	}	
    return 0;
}

