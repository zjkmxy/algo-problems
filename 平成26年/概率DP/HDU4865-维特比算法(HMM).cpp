#include <cstdio>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <string>
#include <map>
using namespace std;

//×ªÒÆ¾ØÕó
const double Trans[3][3] = {
	{0.5, 0.375, 0.125},
	{0.25, 0.125, 0.625},
	{0.25, 0.375, 0.375},
};

//·ÅÉä¾ØÕó
const double Emmi[3][4] = {
	{0.6, 0.2, 0.15, 0.05},
	{0.25, 0.3, 0.2, 0.25},
	{0.05, 0.10, 0.35, 0.50},
};

const double Start[3] = {0.63, 0.17, 0.2};

const int MAXN = 55;
double DP[MAXN][3];
int go[MAXN][3];
int ans[MAXN];

int decode(const string& str){
	if(str == "Dry")
		return 0;
	else if(str == "Dryish")
		return 1;
	else if(str == "Damp")
		return 2;
	else
		return 3;
}

string encode(const int code){
	if(code == 0){
		return "Sunny";
	}else if(code == 1){
		return "Cloudy";
	}else{
		return "Rainy";
	}
}

void domain(){
	int i, j, k, N, code;
	double cur;
	string str;
	cin >> N;
	cin >> str;
	code = decode(str);
	for(j = 0; j < 3; j ++){
		go[0][j] = 0;
		DP[0][j] = Start[j] * Emmi[j][code];
	}
	for(i = 1; i < N; i ++){
		cin >> str;
		code = decode(str);
		for(j = 0; j < 3; j ++){
			DP[i][j] = 0;
			for(k = 0; k < 3; k ++){
				cur = DP[i-1][k] * Trans[k][j] * Emmi[j][code];
				if(cur > DP[i][j]){
					DP[i][j] = cur;
					go[i][j] = k;
				}
			}
		}
	}
	cur = 0;
	for(k = 0; k < 3; k ++){
		if(DP[N-1][k] > cur){
			cur = DP[N-1][k];
			ans[N-1] = k;
		}
	}
	for(i = N - 1; i > 0; i --){
		ans[i - 1] = go[i][ans[i]];
	}
	for(i = 0; i < N; i ++){
		cout << encode(ans[i]) << endl;
	}
}

int main(){
	int T, i;
	cin >> T;
	for(i = 1; i <= T; i ++){
		cout << "Case #" << i << ":" << endl;
		domain();
	}
	return 0;
}