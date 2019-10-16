/*
要点：
1.假设有物品1和2，v2 > v1，比起我们先拿2被打坏后拿1，总是先拿1可能带来的结果更好。因为：
	先拿2再拿1：G = v1 - c2 - c1
	先拿1被打坏：G = v1 - c1
	先拿1再拿2：G = v2 - c2 - c1
  而如果此时2就是最优解，那么没有下一次了，也不会取1。
  所以最优解一定是按价格的递增顺序拿取的。

2.假设上一轮我们能获得的最好收益是G'，本轮拿取物品x，那么妖精的行为如下：
	打坏：G = G' - cx
	不打：G = vx - cx
  所以本轮中物品x的费用实际上不改变，而价格变成了min(G', vx)

3.如果我们严格的每轮分别进行计算，那么如果选择的x恰好是前几轮中的最优解，
  会因为不存在x时G'的值比当前使用的G'值低而使计算出现偏差。但是如果我们只考虑比
  当前物品价格高的物品的G'，我们仍然可以得到最优解，而且不会高估G'。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <algorithm>

struct OBJ{
	//价格、费用、本轮的拿取价值
    int v, c, s;
    bool operator<(const OBJ& other)const{
        return v < other.v;
    }
}obj[150005];
int n, k;

void deal(){
    int i, mg, tmp;

	//mg代表在上一轮、在i之后的全部物品中，所能获取的最大利益
    mg = 0;
    for(i = n - 1; i >= 0; i --){
        tmp = obj[i].s;
        if(obj[i].v > mg){
			//虽然s只会减小不会增大，但是此处用s比较可能提前break
            obj[i].s = mg;
        }else{
            break;
        }
        mg = std::max(mg, tmp - obj[i].c);
    }
}

int getMaxGain(int turn){
    int mg = 0, i;
    for(i = 1; i <= turn; i ++){
        deal();
    }
    for(i = 0; i < n; i ++){
        mg = std::max(mg, obj[i].s - obj[i].c);
    }
    return mg;
}

void domain(){
    int i;
    scanf("%d%d", &n, &k);
    for(i = 0; i < n; i ++){
        scanf("%d%d", &obj[i].v, &obj[i].c);
		obj[i].s = obj[i].v;
    }
    std::sort(obj, obj + n);
    printf("%d\n", getMaxGain(k));
}

int main(){
    int t;
    scanf("%d", &t);
    while(t --){
        domain();
    }
    return 0;
}
