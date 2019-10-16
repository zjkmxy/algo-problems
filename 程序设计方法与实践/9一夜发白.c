#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef union _CHAR
{
	unsigned short us;
	unsigned char uc[2];
}TCHAR, *PCHAR;
unsigned short count[65536];

TCHAR get_char()
{
	TCHAR ret;
	unsigned char tmp0, tmp1, tmp2;

	ret.uc[0] = getchar();
	if(ret.uc[0] < 128)
	{
		//ASCII
		ret.uc[1] = 0;
	}else{
		if((ret.uc[0] & 0xE0) == 0xC0)
		{
			//两位
			tmp1 = ret.uc[0];
			tmp0 = getchar();
			ret.uc[0] = tmp0 & 0x3F;
			ret.uc[0] = ret.uc[0] | ((tmp1 & 0x03) << 6);
			ret.uc[1] = (tmp1 & 0x1C) >> 2;
		}else{
			//三位
			tmp2 = ret.uc[0];
			tmp1 = getchar();
			tmp0 = getchar();
			ret.uc[0] = tmp0 & 0x3F;
			ret.uc[0] = ret.uc[0] | ((tmp1 & 0x03) << 6);
			ret.uc[1] = (tmp1 & 0x3C) >> 2;
			ret.uc[1] = ret.uc[1] | ((tmp2 & 0x0F) << 4);
		}
	}
	return ret;
}

void output(TCHAR ch)
{
	if(ch.us >= 0x800)
	{
		printf("%c",0xE0 | ((ch.us & 0xF000) >> 12));
		printf("%c",0x80 | ((ch.us & 0x0FC0) >> 6));
		printf("%c",0x80 | (ch.us & 0x003F));
	}else{
		printf("%c",0xC0 | ((ch.us & 0x07C0) >> 6));
		printf("%c",0x80 | (ch.us & 0x003F));
	}
}

int main()
{
	TCHAR ch;
	int flag = 1;

	memset(count, 0, sizeof(count));
	ch = get_char();
	while(ch.us != 65535)
	{
		if(ch.us >= 128)
		{
			count[ch.us]++;
		}
		ch = get_char();
	}
	for(ch.us=128;ch.us<65535;ch.us++)
	{
		if(count[ch.us] > 1)
		{
			flag = 0;
			output(ch);
			printf(" 0x%04x %d\n",ch.us,count[ch.us]);
		}
	}
	if(flag)
		printf("No repeat!\n");
	return 0;
}