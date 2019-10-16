#include <cstdio>
#include <cstdlib>
#include <cctype>

typedef long long ll;
const int MAXN = 100005;
int arr[MAXN], q[MAXN];
int sp, bp, si, n;
ll ans;

char B[1<<15], *S=B, *T=B, ch;
#define getc() (S==T&&(T=(S=B)+fread(B,1,1<<15,stdin),S==T)?0:*S++)
int ReadInt(){
    while(ch=getc(), !isdigit(ch));
    int aa = ch - '0';
    while(ch=getc(), isdigit(ch)) aa = aa * 10 + ch - '0';
    return aa;
}

void domain(){
	int i, cur;
	n = ReadInt();
	for(i = 0; i < n; i ++)
		arr[i] = ReadInt();
	si = sp = bp = n - 1;
	ans = 0;
	for(i = 1; i < n; i ++){
		if(bp - sp >= 2 && (si < 0 || q[bp - 1] < arr[si])){
			cur = q[bp - 1] + q[bp];
			bp -= 2;
		}else if(si >= 1 && (bp <= sp || arr[si - 1] < q[bp])){
			cur = arr[si - 1] + arr[si];
			si -= 2;
		}else{
			cur = arr[si] + q[bp];
			si --;
			bp --;
		}
		ans += cur;
		q[sp --] = cur;
	}
	printf("%I64d\n", ans);
}

int main(){
	int T = ReadInt();
	while(T --) domain();
	return 0;
}
