#include "PRISCLAS.h"
#include "iostream"
#include <fstream>//test用
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

string LoadDoc(string address);

void main()
{
	const char* s= "我爱北京天安门，天安门上太阳升，伟大领袖毛主席，指引我们向前进。";
	string StrCh;
	//StrCh = s;
	StrCh= "我爱北京天安门，天安门上太阳升，伟大领袖毛主席，指引我们向前进。";
	vector<string> vecWordsCh;
	PRISCLAS *pClas = new PRISCLAS;
	pClas->InitSegment(1, true, true);
	pClas->Segment(StrCh, vecWordsCh);
	pClas->ReleaseMem();

	for(int i=0;i<vecWordsCh.size();i++)
		if(vecWordsCh[i].empty())
			vecWordsCh[i].erase();

	string NewStr="";
	for(int i=0;i<vecWordsCh.size();i++)
	{
		if (i!=0)
		{
			NewStr+=" ";
		}
		NewStr+=vecWordsCh[i];
	}

	const char* r=NewStr.c_str();

	system("pause");
}


string LoadDoc(string address)
{
	filebuf *pbuf;  
	ifstream filestr;  
	long size;  
	char * buffer;  
	// 要读入整个文件，必须采用二进制打开   
	filestr.open (address,ios::binary);  //TheOriginofSpeciesv6_rvPun
	// 获取filestr对应buffer对象的指针   
	pbuf=filestr.rdbuf();  

	// 调用buffer对象方法获取文件大小  
	size=pbuf->pubseekoff (0,ios::end,ios::in);  
	pbuf->pubseekpos (0,ios::in);  

	// 分配内存空间  
	buffer=new char[size];  

	// 获取文件内容  
	pbuf->sgetn (buffer,size);  

	filestr.close();  
	// 输出到标准输出  

	string text=buffer;

	return text;
}