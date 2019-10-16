#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char byte;
byte mem[256];

int domain(){
	int i, pc = 0;
	byte A = 0, B = 0, op;
	int fend = 0;

	for(i = 0; i < 256; i ++)
		scanf("%1X", &mem[i]);
	if(mem[0] == 8)
		return 0;

	for(pc = 0; !fend; ++ pc){
		switch(mem[pc]){
		case 0:
			op = (mem[pc + 1] << 4) | mem[pc + 2];
			pc += 2;
			A = mem[op];
			break;
		case 1:
			op = (mem[pc + 1] << 4) | mem[pc + 2];
			pc += 2;
			mem[op] = A;
			break;
		case 2:
			op = A;
			A = B;
			B = op;
			break;
		case 3:
			op = A + B;
			A = op & 0x0F;
			B = op >> 4;
			break;
		case 4:
			++ A;
			if(A >= 0x10)
				A -= 0x10;
			break;
		case 5:
			if(A == 0)
				A = 0x0F;
			else
				-- A;
			break;
		case 6:
			op = (mem[pc + 1] << 4) | mem[pc + 2];
			pc += 2;
			if(A == 0)
				pc = op - 1;
			break;
		case 7:
			op = (mem[pc + 1] << 4) | mem[pc + 2];
			pc += 2;
			pc = op - 1;
			break;
		case 8:
			fend = 1;
			break;
		default:
			pc = *(int*)NULL;
			break;
		}
	}

	for(i = 0; i < 256; i ++){
		printf("%1X", mem[i]);
	}
	printf("\n");

	return 1;
}

int main(){
	while(domain());
	return 0;
}