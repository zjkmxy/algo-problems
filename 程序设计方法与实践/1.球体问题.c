#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double PI;
int R1, R2, d, w;
double s;

void domain()
{
	double A1, A2, A, h1, h2, h;

	h = R1 + R2 - d;
	h1 = (R2*R2 + h*h - R1*R1) / h / 2.0;
	h2 = (R1*R1 + h*h - R2*R2) / h / 2.0;
	A1 = PI * (2.0 * R1 - h1) * (2.0 * R1 - h1) * (R1 - (2.0 * R1 - h1) / 3.0);
	A2 = PI * (2.0 * R2 - h2) * (2.0 * R2 - h2) * (R2 - (2.0 * R2 - h2) / 3.0);
	A = A1 + A2;
	printf("%.4lf ", A);

	A1 = 2.0 * PI * R1 * (2.0 * R1 - h1);
	A2 = 2.0 * PI * R2 * (2.0 * R2 - h2);
	printf("%.4lf\n", A1 + A2);

	if(w <= A * s)
	{
		printf("The Paired-Sphere Floats.\n");
	}else{
		printf("The Paired-Sphere Sinks.\n");
	}
}

int main()
{
	int t;

	PI = 2.0 * acos(0.0);
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d%d%d%lf", &R1, &R2, &d, &w, &s);
		domain();
	}
	return 0;
}