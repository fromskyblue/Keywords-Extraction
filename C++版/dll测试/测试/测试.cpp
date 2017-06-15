#include <iostream>
 #include <string>
 //#include <regex>
 #include <fstream>  
 //#include <sstream>
 //#include <utility>
 //#include <algorithm>
 //#include <string>
 //#include <cctype>
 //#include <set>
 //#include <queue>
 //
 //#include <hash_map>

#include "Node.h"



void main(){
	
	
	
	/////////////////////////////          C++ TXT 整个文件的读取 //////////////////////////
	filebuf *pbuf;  
	ifstream filestr;  
	long size;  
	char * buffer;  
	// 要读入整个文件，必须采用二进制打开   
	filestr.open ("D:\\test.txt", ios::binary);  //TheOriginofSpeciesv6_rvPun
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
	//string test;
	
	

	/*
	char *p;
	p=new char[text.size()]; 
	text.copy(p,text.size());
	*(p+text.size())='\0';
	ofstream fout;
	fout.open("D:\\1.txt", ios::binary);
	fout.write(p,text.size());
	fout.close(); 
	*/

	int i;
	Node *result;
	result=keyword_extra_entropy_MAX(text);
	for(i=0;i<30;i++)
		cout<<endl<<result[i].word<<"==="<<result[i].EDnor<<"==="<<result[i].frequency;


	vector<int> p=result[0].t_loc;
	for(i=0;i<p.size();i++)
		cout<<endl<<p[i];
	//cout<<word_loc.size();
//	cout<<Term_list.size();
//	cout<<word_frequency.size();
	system("pause");
}