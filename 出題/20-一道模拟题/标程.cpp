/*
最基本的代码量胜负，合理查资料、学会使用现成代码、冷静的思考和严谨编码是成败的关键
需要熟悉常用的指针操作
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstddef>
using namespace std;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wsign-compare"
#define nullptr NULL

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef byte *pbyte;

const dword MAXSIZE = 65536;
const dword DATALEN = 1024;

const byte FOXPRO = '0';		//FoxPro数据库标志
const byte HEADEND = 13;		//文件头结尾
const size_t HEADSIZE = 32;		//文件头大小
const size_t FIELDSIZE = 32;	//字段描述大小
const size_t NAMESIZE = 11;		//字段名大小
const char DELFLG = '*';
const char UNDELFLG = ' ';

#pragma pack(1)

typedef struct HEADER{
	byte Type;		//数据库类型
	byte Date[3];	//最后修改日期
	dword RcdCount;	//记录数
	word HdrSize;	//文件头长度
	word RcdSize;	//记录长度
	byte NoData[20];//保留
}*PHEADER;
typedef struct FIELD{
	char FldName[NAMESIZE]; //字段名
	char FldType;			//字段类型
	word Address;			//偏移量
	byte Reserved[2];		//保留
	byte FldSize;			//字段长度
	byte DecSize;			//小数位数
	byte Reserved2[14];		//保留
}*PFIELD;

#pragma pack()

//网上找的CRC32校验代码
namespace crc{
	dword crctab32[256];
	void init(){
		int i,j;
		unsigned long crc;
		for (i = 0; i < 256; i++){
			crc = i;
			for (j = 0; j < 8; j++){
				if (crc & 1)
					crc = (crc >> 1) ^ 0xEDB88320;
				else
					crc >>= 1;
			}
			crctab32[i] = crc;
		}
	}
	dword crc32(byte *addr, size_t len){
		dword crc = 0xffffffff;
		while(len--){
			crc = (crc >> 8) ^ crctab32[(crc & 0xff) ^ dword(*addr)];
			addr ++;
		}
		return crc ^ 0xffffffff;
	}
};

//删去字符串头尾的空格
string trim(const string& str){
	int p1 = 0, p2 = str.length() - 1;
	for(; p1 <= p2 && str[p1] == ' '; p1 ++);
	for(; p1 <= p2 && str[p2] == ' '; p2 --);
	return string(str, p1, p2 - p1 + 1);
}

class Table{
private:
	dword crc;		//crc校验码
	size_t size;	//文件尺寸
	pbyte buf;		//缓冲区
	PHEADER header;	//文件头指针
	PFIELD fields;	//各个字段指针
	char *record;	//记录的开始点
	byte fldcnt;	//字段个数

	//转到特定字段，获取起始位置
	inline char* GoTo(int pos){
		return (record + header->RcdSize * pos);
	}

public:
	Table(){
		buf = new byte[MAXSIZE];
		header = reinterpret_cast<PHEADER>(buf);
		fields = nullptr;
		record = nullptr;
		size = 0;
	}
	~Table(){
		delete [] buf;
	}
	//从输入中读入
	bool Read(){
		size_t i;
		scanf("%*s%d%8X", &size, &crc);
		for(i = 0; i < size; i ++)
			scanf("%2X", buf + i);
		if(crc::crc32(buf, size) != crc){
			printf("FAILED\n");
			return false;
		}
		//并不需要检查版本
		if(header->Type != FOXPRO){
			//printf("FAILED\n");
			//return false;
		}
		fldcnt = (header->HdrSize - HEADSIZE) / FIELDSIZE;				//常用指针操作：用大小计算个数
		fields = reinterpret_cast<PFIELD>(pbyte(header) + HEADSIZE);	//常用指针操作：按字节数偏移
		//重新计算个数
		for(i = 0; i < fldcnt; i ++){
			if(fields[i].Address == 0)
				fldcnt = i;
		}
		record = reinterpret_cast<char*>(buf + header->HdrSize);
		printf("%02d/%02d/%02d %d %d\n", header->Date[0], header->Date[1], header->Date[2], header->RcdCount, fldcnt);
		return true;
	}
	//输出crc校验码
	inline void Write(){
		printf("%08X\n", crc::crc32(buf, size));
	}
	//输出字段值
	void Show(int pos){
		char *base;
		int i, addr;
		string str;
		pos --;
		if(pos < 0 || pos >= header->RcdCount){
			printf("%d: FAILED\n", pos + 1);
			return;
		}
		printf("%d:", pos + 1);
		base = GoTo(pos);
		for(i = 0; i < fldcnt; i ++){
			addr = fields[i].Address;
			str.assign(base + addr, fields[i].FldSize);
			str = trim(str);
			printf(" %s", str.data());
		}
		printf("\n");
	}
	//删除记录
	void Delete(int pos){
		char *base;
		pos --;
		if(pos < 0 || pos >= header->RcdCount){
			printf("%d: FAILED\n", pos + 1);
			return;
		}
		printf("%d: DELETED\n", pos + 1);
		base = GoTo(pos);
		base[0] = DELFLG;
	}
	//追加记录
	void Append(){
		int i, j, addr, len;
		char *base;
		char str[DATALEN];

		base = GoTo(header->RcdCount);
		header->RcdCount ++;
		size += header->RcdSize;
		base[0] = UNDELFLG;
		for(i = 0; i < fldcnt; i ++){
			addr = fields[i].Address;
			scanf("%s", str);
			len = strlen(str);
			if(fields[i].FldType == 'C'){
				//字符型是后置空格
				for(j = 0; j < len; j ++)
					base[addr ++] = str[j];
				for(j = len; j < fields[i].FldSize; j ++)
					base[addr ++] = ' ';
			}else{
				//数值型是前置空格
				for(j = len; j < fields[i].FldSize; j ++)
					base[addr ++] = ' ';
				for(j = 0; j < len; j ++)
					base[addr ++] = str[j];
			}
		}
		printf("Count: %d\n", header->RcdCount);
	}
	//整理记录
	void Pack(){
		int i, rcdcnt = 0;
		char *ptr = record, *base;
		for(i = 0; i < header->RcdCount; i ++){
			base = GoTo(i);
			if(base[0] == DELFLG)
				continue;
			if(base != ptr)
				copy(base, base + header->RcdSize, ptr);
			ptr += header->RcdSize;
			rcdcnt ++;
		}
		header->RcdCount = rcdcnt;
		size = reinterpret_cast<pbyte>(ptr) - buf;
		printf("Count: %d\n", header->RcdCount);
	}
	//转化为文件，仅限调试使用
	void ToFile(){
		FILE *fp = fopen("test.dbf", "wb");
		fwrite(buf, 1, size, fp);
		fclose(fp);
	}
} tab;

int main(){
	int T, M, rcdno;
	char op[DATALEN];
	bool succ;
	crc::init();
	scanf("%d", &T);
	while(T --){
		succ = tab.Read();
		scanf("%d", &M);
		while(M --){
			scanf("%s", op);
			if(op[0] == 'A'){
				if(succ)
					tab.Append();
			}else if(op[0] == 'D'){
				scanf("%d", &rcdno);
				if(succ)
					tab.Delete(rcdno);
			}else if(op[0] == 'S'){
				scanf("%d", &rcdno);
				if(succ)
					tab.Show(rcdno);
			}else{
				if(succ)
					tab.Pack();
			}
		}
		if(succ)
			tab.Write();
	}
	return 0;
}