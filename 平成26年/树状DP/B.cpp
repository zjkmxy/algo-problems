/*
hdu1054 - Strategic Game
dproot[ i ]��ʾ��iΪ������������i�Ϸ���һ��ʿ��������ס����������Ҫ����ʿ����
all[ i ]��ʾ����ס������iΪ����������Ҫ����ʿ����
״̬ת�Ʒ��̣�
     Ҷ�ӽڵ㣺dproot[k] =1�� all[k] = 0��
     ��Ҷ�ӽڵ㣺
      dproot[i] = 1 + ��all[j](j��i�Ķ���)��
      all[i] = min( dproot[i],  ��dproot[j](j��i�Ķ���) );

*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct NODE{
	int no;
	int dproot;  //���Լ�ȫ�����صĸ���
	int dpall;   //ȫ�����صĸ���
	int nchilds; //�ӽڵ����
	int *child;  //�ӽڵ�
};
NODE *tree;
int num;

bool input()
{
	int i,no,cnt,j;

	if(scanf("%d",&num) == EOF)
	{
		return false;
	}
	tree = new NODE[num];
	for(i=0;i<num;i++)
	{
		scanf("%d:(%d)",&no,&cnt);
		tree[no].no = no;
		tree[no].nchilds = cnt;
		tree[no].dproot = 0; //δ����
		if(cnt > 0)
		{
			tree[no].child = new int[cnt];
			for(j=0;j<cnt;j++)
			{
				scanf("%d",&tree[no].child[j]);
			}
		}
	}
	return true;
}

void calc(int no)
{
	int i;

	if(tree[no].dproot > 0)
		return; //�������

	tree[no].dproot = 1;
	tree[no].dpall = 0;
	if(tree[no].nchilds > 0) //��Ҷ���
	{
		for(i=0;i<tree[no].nchilds;i++)
		{
			calc(tree[no].child[i]);
			tree[no].dproot += tree[tree[no].child[i]].dpall;
			tree[no].dpall +=  tree[tree[no].child[i]].dproot;
		}
		if(tree[no].dpall > tree[no].dproot)
			tree[no].dpall = tree[no].dproot;
	}
}

void clean()
{
	int i;

	for(i=0;i<num;i++)
	{
		if(tree[i].nchilds > 0)
			delete [] tree[i].child;
	}
	delete [] tree;
}

void print(int no)
{
	printf("%d\n",tree[no].dpall);
}

void rebuild() //�����ڽӾ���Ȼ���ع����νṹ
{
	bool **mat;
	int i,j,k;
	
	mat = new bool*[num];
	for(i=0;i<num;i++)
	{
		mat[i] = new bool[num];
		for(j=0;j<num;j++)
		{
			mat[i][j] = false;
		}
	}
	for(i=0;i<num;i++)
	{
		for(j=0;j<tree[i].nchilds;j++)
		{
			k = tree[i].child[j];
			mat[i][k] = true;
			mat[k][i] = true;
		}
	}
	for(i=0;i<num;i++)
	{
		k = 0;
		if(tree[i].nchilds > 0)
			delete [] tree[i].child; //Ŀǰ����й¶
		tree[i].child = new int[num];
		for(j=0;j<num;j++)
		{
			if(mat[i][j])
			{
				mat[i][j] = false;
				mat[j][i] = false;
				tree[i].child[k] = j;
				k++;
			}
		}
		tree[i].nchilds = k;
	}
	for(i=0;i<num;i++)
	{
		delete [] mat[i];
	}
	delete [] mat;
}

void rebuild2()
{
	int i,j,k,cnt;
	bool *mat;

	mat = new bool[num];
	for(i=0;i<num;i++)
	{
		for(j=0;j<num;j++)
			mat[j] = false;
		cnt = 0;
		for(j=0;j<tree[i].nchilds;j++)
		{
			if(tree[i].child[j] > 0)
			{
				mat[tree[i].child[j]] = true;
				//tree[i].child[j] = -1;
				cnt++;
			}
		}
		for(j=i+1;j<num;j++)
		{
			for(k=0;k<tree[j].nchilds;k++)
			{
				if(tree[j].child[k] == i)
				{
					tree[j].child[k] = -1;
					mat[j] = true;
					cnt++;
				}
			}
		}
		if(tree[i].nchilds > 0)
			delete [] tree[i].child;
		tree[i].nchilds = cnt;
		if(cnt > 0)
		{
			tree[i].child = new int[cnt];
			for(j=0;j<num;j++)
			{
				if(mat[j])
				{
					tree[i].child[cnt-1] = j;
					cnt--;
				}
			}
		}
	}
	delete [] mat;
}

int main()
{
	while(input())
	{
		//rebuild(); MLE
		rebuild2();
		calc(0);
		printf("%d\n",tree[0].dpall);
		clean();
	}
	return 0;
}
