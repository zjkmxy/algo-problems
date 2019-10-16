#include <cstdio>
#include <algorithm>
#include <map>
#include <cstdlib>
using namespace std;

const int MAXN = 200001;
const int MAXI = 0x7fffffff;

int n;
int f[MAXN], g[MAXN];
map<int, int> tab;
int main(){
#ifndef _DEBUG
    freopen("commuting.in", "r", stdin);
    freopen("commuting.out", "w", stdout);
#endif

    int i, x, len, y;
    int fac, best;

    scanf("%d", &n);
    for(i = 1; i <= n; i ++){
        scanf("%d", &f[i]);
    }

    for(i = 1; i <= n; i ++){
        if(g[i] != 0)
            continue;
		for(x = f[i], len = 1; x != i; x = f[x], len ++);

        best = MAXI;
        for(fac = 1; fac * fac <= len; fac ++){
            if(len % fac == 0){
                if(tab.find(fac) != tab.end()){
                    best = std::min(best, tab[fac]);
                }
                if(tab.find(len / fac) != tab.end()){
                    best = std::min(best, tab[len / fac]);
                }
            }
        }

        y = best;
        x = i;
        if(y >= i){
            tab[len] = i;
            y = i;
        }
        do{
            g[x] = y;
            x = f[x];
            y = f[y];
        }while(x != i);
    }

    for(i = 1; i <= n; i ++){
        if(f[g[i]] != g[f[i]]){
            throw 0;
        }
    }

    for(i = 1; i <= n; i ++){
        printf("%d", g[i]);
        if(i == n){
            printf("\n");
        }else{
            printf(" ");
        }
    }

    return 0;
}