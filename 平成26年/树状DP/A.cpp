/*
HDU 2412 PARTY AT HALI-BULA
n个人形成一个关系树，每个节点代表一个人，节点的根表示这个人的唯一的直接上司，只有根没有上司。要求选取一部分人出来，使得每2个人之间不能有直接的上下级的关系，求最多能选多少个人出来，并且求出获得最大人数的选人方案是否唯一。
DP, 用dp[i][0]表示不选择i点时，i点及其子树能选出的最多人数，dp[i][1]表示选择i点时，i点及其子树的最多人数。
对于叶子节点 dp[k][0] = 0, dp[k][1] = 1
对于非叶子节点i,
dp[i][0] = ∑max(dp[j][0], dp[j][1]) (j是i的儿子)
dp[i][1] = 1 + ∑dp[j][0] (j是i的儿子) 
最多人数即为max(dp[0][0], dp[0][1])
新加一个状态dup[i][j]，表示相应的dp[i][j]是否是唯一方案。
对于叶子结点, dup[k][0] = dup[k][1] = 1.
对于非叶子结点,
对于i的任一儿子j，若(dp[j][0] > dp[j][1] 且 dup[j][0] == 0) 或 (dp[j][0] < dp[j][1] 且 dup[j][1] == 0) 或 (dp[j][0] == dp[j][1])，则dup[i][0] = 0
对于i的任一儿子j有dup[j][0] = 0, 则dup[i][1] = 0
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef long int DP; 

struct PERSON
{
	PERSON* spvis; //父
	PERSON* lemp;  //左孩子
	PERSON* rbro;  //右兄弟
	DP dp[2];      //0不选，1选
	bool dup[2];   //同上，表示是否唯一
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

	//加入了新人
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
	PPERSON emp; //表示所有雇员

	for( ; cur != NULL; cur = cur->rbro)  //循环遍历计算一层
	{
		cur->dp[0]  = 0;
		cur->dp[1]  = 1;
		cur->dup[0] = true;
		cur->dup[1] = true;
		if(cur->lemp) //非叶节点
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
