#include <stdio.h>
#include <string.h>
#include <math.h>

int domain(){
	double W, L, X0, Y0, X1, Y1, ans;
	char str[1005], *ch;

	if(scanf("%lf%lf%lf%lf%lf%lf%s", &W, &L, &X0, &Y0, &X1, &Y1, str) == EOF)
		return 0;
	for(ch = str; *ch; ++ ch){
		switch(*ch){
		case 'F':
			Y0 = - Y0;
			break;
		case 'B':
			Y0 = L * 2. - Y0;
			break;
		case 'L':
			X0 = - X0;
			break;
		case 'R':
			X0 = W * 2. - X0;
			break;
		}
	}
	ans = sqrt((Y1 - Y0) * (Y1 - Y0) + (X1 - X0) * (X1 - X0));
	printf("%.4lf\n", ans);

	return 1;
}

int main(){
	while(domain());
	return 0;
}