#include <stdio.h>
#include <stdlib.h>

int graph[10][10];

void Floyd(int n)   //Floyd±©Á¦Èý½ÇÇó¾àÀë
{
	int i,j,k;
	int ao,an;
	for(k=0;k<n;k++)
	{
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if((i != k)&&(j != k))
				{
					ao = graph[i][j];
					an = graph[i][k] + graph[k][j];
					graph[i][j] = (ao > an ? an : ao);
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	int n,i,j;

	scanf("%d",&n);
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			scanf("%d",&graph[i][j]);

	Floyd(n);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			printf("%d ",graph[i][j]);
		printf("\n");
	}
	system("pause");
	return 0;
}

