#include <cstdio>
#include <algorithm>
#include <cstring>

typedef long long ll;
const int MAXN = 1000005;
const int MODN = 1000000007;
char str[MAXN];
struct DATA{
	int match;	//匹配位点
	int par;	//父节点
	int lcnt, rcnt;	//左右兄弟个数
	ll ans;		//答案
}arr[MAXN];
int stk[MAXN], sp, N;

void domatch(){
	int i;
	for(i = 1; i <= N; i ++){
		if(str[i] == '('){
			arr[i].par = sp > 0 ? stk[sp - 1] : 0;
			stk[sp ++] = i;
			arr[i].match = -1;
		}else{
			if(sp > 0){
				//有匹配
				-- sp;
				arr[i].match = stk[sp];
				arr[i].par = arr[stk[sp]].par;
				arr[stk[sp]].match = i;
			}else{
				//失配的右括号
				arr[i].match = -1;
				arr[i].par = 0;
			}
		}
	}
}

void countbro(){
	int i;

	for(i = 1; i <= N; i ++){
		if(arr[i].match == -1){
			arr[i].lcnt = -1;
			continue;
		}
		if(str[i] == '('){
			if(str[i - 1] == ')'){
				arr[i].lcnt = arr[i - 1].lcnt + 1;
			}else{
				arr[i].lcnt = 0;
			}
		}else{
			arr[i].lcnt = arr[arr[i].match].lcnt;
		}
	}
	for(i = N; i >= 1; i --){
		if(arr[i].match == -1){
			arr[i].rcnt = -1;
			continue;
		}
		if(str[i] == ')'){
			if(str[i + 1] == '('){
				arr[i].rcnt = arr[i + 1].rcnt + 1;
			}else{
				arr[i].rcnt = 0;
			}
		}else{
			arr[i].rcnt = arr[arr[i].match].rcnt;
		}
	}
}

ll getans(){
	int i;
	ll ans = 0;

	for(i = 1; i <= N; i ++){
		if(arr[i].match == -1){
			arr[i].ans = 0;
			continue;
		}
		if(str[i] == '('){
			arr[i].ans = arr[arr[i].par].ans;
			arr[i].ans += ll(arr[i].lcnt + 1) * ll(arr[i].rcnt + 1);
			arr[i].ans %= MODN;
		}else{
			arr[i].ans = arr[arr[i].match].ans;
		}
		ans += i * arr[i].ans % MODN;
		//ans不取模……
	}
	return ans;
}

int main(){
	int T;
	scanf("%d\n", &T);
	while(T --){
		scanf("%s", str + 1);
		N = strlen(str + 1);
		str[0] = '(';
		str[N + 1] = ')';
		sp = 0;

		domatch();
		countbro();
		printf("%I64d\n", getans());
	}
	return 0;
}