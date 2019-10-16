/*
Ҫ�㣺
1.��������Ʒ1��2��v2 > v1��������������2���򻵺���1����������1���ܴ����Ľ�����á���Ϊ��
	����2����1��G = v1 - c2 - c1
	����1���򻵣�G = v1 - c1
	����1����2��G = v2 - c2 - c1
  �������ʱ2�������Ž⣬��ôû����һ���ˣ�Ҳ����ȡ1��
  �������Ž�һ���ǰ��۸�ĵ���˳����ȡ�ġ�

2.������һ�������ܻ�õ����������G'��������ȡ��Ʒx����ô��������Ϊ���£�
	�򻵣�G = G' - cx
	����G = vx - cx
  ���Ա�������Ʒx�ķ���ʵ���ϲ��ı䣬���۸�����min(G', vx)

3.��������ϸ��ÿ�ֱַ���м��㣬��ô���ѡ���xǡ����ǰ�����е����Ž⣬
  ����Ϊ������xʱG'��ֵ�ȵ�ǰʹ�õ�G'ֵ�Ͷ�ʹ�������ƫ������������ֻ���Ǳ�
  ��ǰ��Ʒ�۸�ߵ���Ʒ��G'��������Ȼ���Եõ����Ž⣬���Ҳ���߹�G'��
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <algorithm>

struct OBJ{
	//�۸񡢷��á����ֵ���ȡ��ֵ
    int v, c, s;
    bool operator<(const OBJ& other)const{
        return v < other.v;
    }
}obj[150005];
int n, k;

void deal(){
    int i, mg, tmp;

	//mg��������һ�֡���i֮���ȫ����Ʒ�У����ܻ�ȡ���������
    mg = 0;
    for(i = n - 1; i >= 0; i --){
        tmp = obj[i].s;
        if(obj[i].v > mg){
			//��Ȼsֻ���С�������󣬵��Ǵ˴���s�ȽϿ�����ǰbreak
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
