#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int plines, llines;
int enable;
int str1, str2;
char cur;
int sudden_break;

int deal_line()
{
	int pos = 0;

	if(cur == '#')
	{
		sudden_break = 0;
		return 1;
	}
	for(pos=0;cur != '\n';cur = getchar(),pos++)
	{
		if(cur == '#')
		{
			sudden_break = 1;
			return 1;
		}
		//if(((!enable)&&str1) || (str2&&str1) || ((!enable)&&str2))
		//	enable = *(int*)NULL;
		//if(!str1 && !str2)
		//{
			if(cur == '(' && (cur = getchar()) == '(')
			{
				enable = 0;
				pos += 1;
				continue;
			}
			if(cur == ')' && (cur = getchar()) == ')')
			{
				enable = 1;
				pos += 1;
				continue;
			}
		//}
		if(!enable)
			continue;
		if(!str2 && cur == '\'')
		{
			str1 = !str1;
			continue;
		}
		if(!str1 && cur == '\"')
		{
			str2 = !str2;
			continue;
		}
		if(str1 || str2)
			continue;
		if(cur == ';')
		{
			llines++;
			continue;
		}
		if(cur == '@')
		{
			while(cur != '\n')
			{
				cur = getchar();
				if(cur == '#')
				{
					sudden_break = 1;
					return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}

int domain(int pcnt)
{
	enable = 1;
	str1 = str2 = 0;
	plines = llines = 0;
	sudden_break = 0;
	if(cur == '\n' && pcnt != 1)
		cur = getchar();
	while(!deal_line())
	{
		plines++;
		if(str1 || str2)
			printf("Unterminated string in line %d.\n", plines);
		str1 = str2 = 0;
		cur = getchar();
	}
	//if(sudden_break)
	//	plines++;
	if(str1 || str2)
		printf("Unterminated string in line %d.\n", plines);
	if(!enable)
		printf("Unterminated block comment at end of program.\n");
	printf("Program %d contains %d logical lines and %d physical lines.\n", pcnt, llines, plines);
	return ((cur = getchar()) == '#');
}

int main()
{
	int cnt = 1;
	cur = getchar();
	while(!domain(cnt))
	{
		cnt++;
	}
	return 0;
}

//�����С���#�����ĳ��ϣ�#�Ÿ��������������������һ��
//�����ǲ�Ƕ�׵ģ������ں�����ֱ�����ӾͿ�����
//ע���ڲ���#��֪�����ǻ��ǲ�����
//�����ڲ��Ŀ�ע����ô���ǣ�
//"((#��ô����
//������