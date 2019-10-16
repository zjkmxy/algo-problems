#include <stdio.h>
int main(){
    float a, b;
    while(scanf("%f%f", &a, &b) != EOF)
        printf("%.1f\n", a - b);
    return 0;
}
