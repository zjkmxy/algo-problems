/*
HDU 2412 PARTY AT HALI-BULA
n�����γ�һ����ϵ����ÿ���ڵ����һ���ˣ��ڵ�ĸ���ʾ����˵�Ψһ��ֱ����˾��ֻ�и�û����˾��Ҫ��ѡȡһ�����˳�����ʹ��ÿ2����֮�䲻����ֱ�ӵ����¼��Ĺ�ϵ���������ѡ���ٸ��˳������������������������ѡ�˷����Ƿ�Ψһ��
DP, ��dp[i][0]��ʾ��ѡ��i��ʱ��i�㼰��������ѡ�������������dp[i][1]��ʾѡ��i��ʱ��i�㼰�����������������
����Ҷ�ӽڵ� dp[k][0] = 0, dp[k][1] = 1
���ڷ�Ҷ�ӽڵ�i,
dp[i][0] = ��max(dp[j][0], dp[j][1]) (j��i�Ķ���)
dp[i][1] = 1 + ��dp[j][0] (j��i�Ķ���) 
���������Ϊmax(dp[0][0], dp[0][1])
�¼�һ��״̬dup[i][j]����ʾ��Ӧ��dp[i][j]�Ƿ���Ψһ������
����Ҷ�ӽ��, dup[k][0] = dup[k][1] = 1.
���ڷ�Ҷ�ӽ��,
����i����һ����j����(dp[j][0] > dp[j][1] �� dup[j][0] == 0) �� (dp[j][0] < dp[j][1] �� dup[j][1] == 0) �� (dp[j][0] == dp[j][1])����dup[i][0] = 0
����i����һ����j��dup[j][0] = 0, ��dup[i][1] = 0
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef long int DP; 

struct PERSON
{
	PERSON* spvis; //��
	PERSON* lemp;  //����
	PERSON* rbro;  //���ֵ�
	DP dp[2];      //0��ѡ��1ѡ
	bool dup[2];   //ͬ�ϣ���ʾ�Ƿ�Ψһ
};
typedef PERSON* PPERSON;
PERSON head[205];
char name[205][101];
int num,cnt;

#define max(a,b) ((a > b)?a:b)

int search(char *str)
{
	int i;

	for(i=0;i<cnt;i++)
	{
		if(strcmp(str, name[i]) == 0)
			return i;
	}

	//����������
	strcpy(name[cnt], str);
	head[cnt].lemp = NULL;
	head[cnt].rbro = NULL;
	head[cnt].spvis = NULL;
	cnt++;
	return (cnt - 1);
}

void insert(int cur, int s)
{
	head[cur].rbro = head[s].lemp;
	head[s].lemp = &head[cur];
	head[cur].spvis = &head[s];
}

void calc(PPERSON cur)
{
	PPERSON emp; //��ʾ���й�Ա

	for( ; cur != NULL; cur = cur->rbro)  //ѭ����������һ��
	{
		cur->dp[0]  = 0;
		cur->dp[1]  = 1;
		cur->dup[0] = true;
		cur->dup[1] = true;
		if(cur->lemp) //��Ҷ�ڵ�
		{
			calc(cur->lemp);
			for(emp = cur->lemp; emp != NULL; emp = emp->rbro)
			{
				cur->dp[1] += emp->dp[0];
				if(emp->dup[0] == false)
					cur->dup[1] = false;
				if(emp->dp[0] == emp->dp[1])
				{
					cur->dp[0] += emp->dp[0];
					cur->dup[0] = false;
				}else{
					if(emp->dp[0] > emp->dp[1])
					{
						cur->dp[0] += emp->dp[0];
						if(emp->dup[0] == false)
							cur->dup[0] = false;
					}else{
						cur->dp[0] += emp->dp[1];
						if(emp->dup[1] == false)
							cur->dup[0] = false;
					}
				}
			}
		}
	}
}

bool input()
{
	int i;
	int cur,hd;
	char cname[101],sname[101];

	scanf("%d",&num);
	if(num == 0)
	{
		return false;
	}

	memset(name, 0, 205 * 101);
	scanf("%s", name[0]);
	cnt = 1;
	head[0].lemp = NULL;
	head[0].rbro = NULL;
	head[0].spvis = NULL;
	for(i=1;i<num;i++)
	{
		scanf("%s %s", cname, sname);
		cur = search(cname);
		hd = search(sname);
		insert(cur, hd);
	}
	return true;
}

void print()
{
	if(head[0].dp[0] == head[0].dp[1])
	{
		printf("%ld No\n",head[0].dp[0]);
	}else{
		if(head[0].dp[0] > head[0].dp[1])
		{
			printf(head[0].dup[0]?"%ld Yes\n":"%ld No\n",head[0].dp[0]);
		}else{
			printf(head[0].dup[1]?"%ld Yes\n":"%ld No\n",head[0].dp[1]);
		}
	}
}

int main()
{
	while(input())
	{
		calc(&head[0]);
		print();
	}
	return 0;
}
