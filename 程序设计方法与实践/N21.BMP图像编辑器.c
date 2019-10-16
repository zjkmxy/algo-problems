#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

typedef unsigned char BYTE;
typedef signed char CHAR;
typedef unsigned short WORD;
typedef short SHORT;
typedef unsigned long DWORD;
typedef long LONG;
typedef int BOOL;
const BOOL TRUE = 1;
const BOOL FALSE = 0;

//颜色组成
typedef struct _DAT{
	BYTE b, g, r;
}DAT, *PDAT;

//BMP固定文件头
typedef struct _BMP {
	WORD    bfType;
	DWORD   bfSize;	//文件总尺寸
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;

	DWORD      biSize;
	LONG       biWidth;	//宽度
	LONG       biHeight;//高度
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;	//BMP数据尺寸
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BMP, *PBMP;

const BYTE FIXHEADER[] = {
	0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00,
	0x13, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

CHAR szBuf[70000];
PBMP pHead;
CHAR *pDat;
LONG lLineSize;	//行字节数
LONG M, N;

void swap(LONG* pX, LONG* pY){
	(*pX) ^= (*pY);
	(*pY) ^= (*pX);
	(*pX) ^= (*pY);
}

//清理操作
void Clear(){
	LONG i;
	memset(pDat, 0x00, pHead->biSizeImage);
	for(i = 0; i < N; i ++)
		memset(pDat + i * lLineSize, 0xFF, M * 3);
}

//初始化新的图
void Init(LONG lWidth, LONG lHeight){
	memcpy(szBuf, FIXHEADER, sizeof(BMP));
	pHead->biWidth = lWidth;
	pHead->biHeight = lHeight;
	lLineSize = lWidth * 3;
	if(lLineSize % 4 > 0)
		lLineSize += 4 - (lLineSize % 4);
	pHead->biSizeImage = lLineSize * lHeight;
	pHead->bfSize = sizeof(BMP)+ pHead->biSizeImage;
	Clear();
}

//取得点
PDAT GetPoint(LONG X, LONG Y){
	return (PDAT)(pDat + (N - Y) * lLineSize + (X - 1) * sizeof(DAT));
}

//设置点颜色
void Color(LONG X, LONG Y, DWORD dwColor){
	PDAT p = GetPoint(X, Y);
	p->r = (dwColor >> 16) & 0xff;
	p->g = (dwColor >> 8) & 0xff;
	p->b = (dwColor >> 0) & 0xff;
}

//得到点颜色
DWORD GetColor(LONG X, LONG Y){
	PDAT p = GetPoint(X, Y);
	return (p->r << 16) | (p->g << 8) | (p->b << 0);
}

//设置线颜色
void Vertical(LONG X, LONG Y1, LONG Y2, DWORD dwColor){
	LONG i;
	if(Y1 > Y2)
		swap(&Y1, &Y2);
	for(i = Y1; i <= Y2; i ++)
		Color(X, i, dwColor);
}

//设置线颜色
void Horizon(LONG X1, LONG X2, LONG Y, DWORD dwColor){
	LONG i;
	if(X1 > X2)
		swap(&X1, &X2);
	for(i = X1; i <= X2; i ++)
		Color(i, Y, dwColor);
}

//设置矩形
void Rectangle(LONG X1, LONG X2, LONG Y1, LONG Y2, DWORD dwColor){
	LONG i, j;
	if(X1 > X2)
		swap(&X1, &X2);
	if(Y1 > Y2)
		swap(&Y1, &Y2);
	for(i = X1; i <= X2; i ++)
	for(j = Y1; j <= Y2; j ++)
		Color(i, j, dwColor);
}

//区域填充
void Fill(LONG X, LONG Y, DWORD dwColor){
	static DWORD q[65536];
	LONG qh = 0, qt = 0, x, y;
	LONG d, nx, ny;
	DWORD dwOri = GetColor(X, Y);
	LONG dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

	if(dwOri == dwColor)
		return;
	q[qt ++] = (X << 16) | Y;
	Color(X, Y, dwColor);
	while(qt > qh){
		x = q[qh] >> 16;
		y = q[qh] & 0xFFFF;
		qh ++;

		//防止重复入队列
		for(d = 0; d < 4; d ++){
			nx = x + dir[d][0];
			ny = y + dir[d][1];
			if(nx <= 0 || nx > M || ny <= 0 || ny > N || GetColor(nx, ny) != dwOri)
				continue;
			Color(nx, ny, dwColor);
			q[qt ++] = (nx << 16) | ny;
		}
	}
}

void Save(CHAR szFilename[]){
	FILE* fp = fopen(szFilename, "wb");
	fwrite(szBuf, 1, pHead->bfSize, fp);
	fclose(fp);
}

int main(){
	CHAR op[3], buf[20];
	LONG a, b, c, d;
	DWORD col;

	pHead = (PBMP)(szBuf);
	pDat = szBuf + sizeof(BMP);
	while(scanf("%s", op) != EOF){
		if(op[0] == 'I'){
			scanf("%ld%ld", &M, &N);
			Init(M, N);
		} else if(op[0] == 'C'){
			Clear();
		} else if(op[0] == 'L' || op[0] == 'G'){
			scanf("%ld%ld%x", &a, &b, &col);
			Color(a, b, col);
		} else if(op[0] == 'V'){
			scanf("%ld%ld%ld%x", &a, &b, &c, &col);
			Vertical(a, b, c, col);
		} else if(op[0] == 'H'){
			scanf("%ld%ld%ld%x", &a, &b, &c, &col);
			Horizon(a, b, c, col);
		} else if(op[0] == 'K'){
			scanf("%ld%ld%ld%ld%x", &a, &b, &c, &d, &col);
			Rectangle(a, c, b, d, col);
		} else if(op[0] == 'F'){
			scanf("%ld%ld%x", &a, &b, &col);
			Fill(a, b, col);
		} else if(op[0] == 'S'){
			scanf("%s", buf);
			Save(buf);
		} else if(op[0] == 'X'){
			break;
		} else{
			//未知命令，报错
			op[0] = *(CHAR*)NULL;
		}
	}

	return 0;
}