/*
������Ĵ�����ʤ������������ϡ�ѧ��ʹ���ֳɴ��롢�侲��˼�����Ͻ������ǳɰܵĹؼ�
��Ҫ��Ϥ���õ�ָ�����
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

const byte FOXPRO = '0';		//FoxPro���ݿ��־
const byte HEADEND = 13;		//�ļ�ͷ��β
const size_t HEADSIZE = 32;		//�ļ�ͷ��С
const size_t FIELDSIZE = 32;	//�ֶ�������С
const size_t NAMESIZE = 11;		//�ֶ�����С
const char DELFLG = '*';
const char UNDELFLG = ' ';

#pragma pack(1)

typedef struct HEADER{
	byte Type;		//���ݿ�����
	byte Date[3];	//����޸�����
	dword RcdCount;	//��¼��
	word HdrSize;	//�ļ�ͷ����
	word RcdSize;	//��¼����
	byte NoData[20];//����
}*PHEADER;
typedef struct FIELD{
	char FldName[NAMESIZE]; //�ֶ���
	char FldType;			//�ֶ�����
	word Address;			//ƫ����
	byte Reserved[2];		//����
	byte FldSize;			//�ֶγ���
	byte DecSize;			//С��λ��
	byte Reserved2[14];		//����
}*PFIELD;

#pragma pack()

//�����ҵ�CRC32У�����
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

//ɾȥ�ַ���ͷβ�Ŀո�
string trim(const string& str){
	int p1 = 0, p2 = str.length() - 1;
	for(; p1 <= p2 && str[p1] == ' '; p1 ++);
	for(; p1 <= p2 && str[p2] == ' '; p2 --);
	return string(str, p1, p2 - p1 + 1);
}

class Table{
private:
	dword crc;		//crcУ����
	size_t size;	//�ļ��ߴ�
	pbyte buf;		//������
	PHEADER header;	//�ļ�ͷָ��
	PFIELD fields;	//�����ֶ�ָ��
	char *record;	//��¼�Ŀ�ʼ��
	byte fldcnt;	//�ֶθ���

	//ת���ض��ֶΣ���ȡ��ʼλ��
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
	//�������ж���
	bool Read(){
		size_t i;
		scanf("%*s%d%8X", &size, &crc);
		for(i = 0; i < size; i ++)
			scanf("%2X", buf + i);
		if(crc::crc32(buf, size) != crc){
			printf("FAILED\n");
			return false;
		}
		//������Ҫ���汾
		if(header->Type != FOXPRO){
			//printf("FAILED\n");
			//return false;
		}
		fldcnt = (header->HdrSize - HEADSIZE) / FIELDSIZE;				//����ָ��������ô�С�������
		fields = reinterpret_cast<PFIELD>(pbyte(header) + HEADSIZE);	//����ָ����������ֽ���ƫ��
		//���¼������
		for(i = 0; i < fldcnt; i ++){
			if(fields[i].Address == 0)
				fldcnt = i;
		}
		record = reinterpret_cast<char*>(buf + header->HdrSize);
		printf("%02d/%02d/%02d %d %d\n", header->Date[0], header->Date[1], header->Date[2], header->RcdCount, fldcnt);
		return true;
	}
	//���crcУ����
	inline void Write(){
		printf("%08X\n", crc::crc32(buf, size));
	}
	//����ֶ�ֵ
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
	//ɾ����¼
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
	//׷�Ӽ�¼
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
				//�ַ����Ǻ��ÿո�
				for(j = 0; j < len; j ++)
					base[addr ++] = str[j];
				for(j = len; j < fields[i].FldSize; j ++)
					base[addr ++] = ' ';
			}else{
				//��ֵ����ǰ�ÿո�
				for(j = len; j < fields[i].FldSize; j ++)
					base[addr ++] = ' ';
				for(j = 0; j < len; j ++)
					base[addr ++] = str[j];
			}
		}
		printf("Count: %d\n", header->RcdCount);
	}
	//�����¼
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
	//ת��Ϊ�ļ������޵���ʹ��
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