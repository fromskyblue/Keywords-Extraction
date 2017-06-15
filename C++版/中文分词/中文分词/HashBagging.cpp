#include "HashBagging.h"
#include "iostream"
#include <fstream>//test用
#include <cctype>
#include <algorithm>

HASHBAGGING::HASHBAGGING()
{
}


//HASHBAGGING::HASHBAGGING(int num)
//{
//	This_Threshold=0;
//	m_reader= new MailReader;
//
//	m_Bagging_Gram.resize(num);
//
//	//%%可能是构造两个分类器
//	for(int i=0;i<m_Bagging_Gram.size();i++)
//		m_Bagging_Gram[i]=new HASHGRAMBASED(2);
//
//	//%%对于每一个分类器
//	for(int i=0;i<m_Bagging_Gram.size();i++)
//	{
//		vector<string> Temp_Path;
//		Temp_Path.clear();
//
//		//%%Temp_Path向量每个元素就是string，为什么还要用c_str()，转成char*
//		//%%什么是特征得提取地址
//		Temp_Path.push_back(Feature_Path[2*i].c_str());
//		Temp_Path.push_back(Feature_Path[2*i+1].c_str());
//
//		m_Bagging_Gram[i]->Clear();
//		
//		//每个分类器要创建两个哈希表
//		m_Bagging_Gram[i]->Initialize(2);
//		//从文件中为每个分类器导入哈希表
//		m_Bagging_Gram[i]->LoadData(Temp_Path);
//
//	}
//
//}
//

/*
Initial函数：
功能：初始化分类器
参数：num分类器的个数，每个分类器有2个哈希表
*/
void HASHBAGGING::Initial(int num)
{
	//%%这个在主函数中已经初始化了
	This_Threshold=0;

	//%%创建一个邮件阅读器
	m_reader= new MailReader;

	//%%m_Bagging_Gram是一个向量，每个元素是HASHGRAMBASED
	//%%调用函数传入的值为1
	m_Bagging_Gram.resize(num);

	//%%实例化每个向量元素
	for(int i=0;i<m_Bagging_Gram.size();i++)
		m_Bagging_Gram[i]=new HASHGRAMBASED(2);

}



/*
Release函数与析构函数的工作相同
*/
HASHBAGGING::~HASHBAGGING()
{
	SaveFea ();	

	for(unsigned n = 0; n < m_Bagging_Gram.size(); n ++)
		delete m_Bagging_Gram[n];

	delete m_reader;

}

void HASHBAGGING::Release()
{
	SaveFea ();	

	for(unsigned n = 0; n < m_Bagging_Gram.size(); n ++)
		delete m_Bagging_Gram[n];

	delete m_reader;

}

/*
LoadFea函数：
功能：指定特征文件，调用HashGramBase初始化哈希表的方法，建立分类用的哈希表
参数：
*/
void HASHBAGGING::LoadFea()
{
   //%%此处size()为1，
	for(int i=0;i<m_Bagging_Gram.size();i++)
	{
		vector<string> Temp_Path;
		Temp_Path.clear();

		//%%在调用函数中已经完成初始化
		//%%Feature_Path[0]=".\\kidult\\src\\spamOriginal.dat"
		//%%Feature_Path[0]=".\\kidult\\src\\hamOriginal.dat"
		Temp_Path.push_back(Feature_Path[2*i].c_str());
		Temp_Path.push_back(Feature_Path[2*i+1].c_str());

		//%%HASHGRAMBASED的Initial中已经包含clear()，并且在创建HASHGRAMBASED的时候
		//%%已经调用了Initialize（），此处应该没有必要
		m_Bagging_Gram[i]->Clear();
		m_Bagging_Gram[i]->Initialize(2);

		//%%从文件中读取特征到哈希表中
		//%%共有两个特征文件，两个哈希表
		m_Bagging_Gram[i]->LoadData(Temp_Path);
	}
}


/*
SaveFea函数：
功能：指定特征文件，调用HashGramBase写哈希表的方法，将分类用的哈希表写入文件
参数：
*/
void HASHBAGGING::SaveFea()
{
	//%%将训练好的特征写入文件
	//%%实质就是将哈希表写入文件
	for(int i=0;i<m_Bagging_Gram.size();i++)
	{
		vector<string> Temp_Path;
		Temp_Path.clear();
		Temp_Path.push_back(Feature_Path[2*i].c_str());
		Temp_Path.push_back(Feature_Path[2*i+1].c_str());
		m_Bagging_Gram[i]->ExportData(Temp_Path);
	}
}

/*
	StrToVecChineseWord函数：
	功能：将一个中英文混合的词串分离成中文和英文
	参数：str中英文混合的词串， StrEng英文词串，StrCh中文词串
*/
void HASHBAGGING::StrToVecChineseWord(string str, string & StrEng,string & StrCh)
{
	StrCh.clear(); 
	StrEng.clear();

	for(unsigned n = 0; n < str.length(); n ++)
	{
		//中文词
		if(str[n] < 0)
		{
			//vecStrCh.push_back(str.substr(n, 2));
			//%%%用两个字节表示一个中文字，但是所有的词都将连在一起
			StrCh+=str.substr(n, 2);
			n++;
		}
		else
			//vecItem.push_back(strLine.substr(n, 1));
			//vecStrEng.push_back(str.substr(n,1));
			
			//%%只拆出一个英文字符，但是要接到StrEng尾
			StrEng+=str.substr(n,1);
	}
}

/*
WordParserMail函数：
功能：1、读邮件 2、将读入内容中英文分离 3、分别对中英文分词
*/
//void HASHBAGGING::WordParserMail()
void HASHBAGGING::WordParserMail( )
{
	//每次开始新的字串分析，都应该初始化
	vecWordsEng.clear();
	vecWordsCh.clear();
	StrEng.clear();
	StrCh.clear();

	//%%调用函数已经完成了邮件名称的初始化
	//%%这里调用邮件阅读类的读邮件的方法
	m_reader->ReadMail(m_mailname);

	//++++++++++++++++++++++++++++++++++++++++++
	/*if(mode)
	{
		cout<<"***************body******************"<<endl;
		cout<<m_reader->GetBody()<<endl;
		cout<<"***************subject******************"<<endl;
		cout<<m_reader->GetSubject()<<endl;
		cout<<"**************************************"<<endl;
	}*/
	
	//++++++++++++++++++++++++++++++++++++++++++

	//%%对邮件的主题以及全文进行解析，将结果分别存入strEng（英文）strch（中文）
	StrToVecChineseWord(m_reader->GetBody()+m_reader->GetSubject(),StrEng, StrCh);

	//++++++++++++++++++++++++++++++++++++++++++
	/*if(mode)
	{
		ofstream outfile("body.txt");
		if(!outfile.is_open() )
			cout<<"failed open"<<endl;

		outfile.write(StrEng.c_str(), StrEng.size() );
		outfile.write(StrCh.c_str(), StrCh.size() );

		outfile.close();
	}*/

	//++++++++++++++++++++++++++++++++++++++++++

	//cout<<StrEng.c_str()<<endl;
	//cout<<StrCh.c_str()<<endl;
    
	/*if(StrCh.length()>2){
	//LoadSegRes();
	void* pSegger = CreateSegger();	
	WordSegment(pSegger, StrCh, vecWordsCh);
	ReleaseSegger(pSegger);
	//ReleaseSegRes();
	}*/
	//else{
	//	vecWordsCh.push_back(StrCh);
	//}

	//%%如果含有至少一个中文词
	if(StrCh.length()>2)
	{
		//分词，所有中文都是连在一起的，通过分词，剥离出一个个词来
		PRISCLAS *pClas = new PRISCLAS;
		pClas->InitSegment(1, false, true);		
		pClas->Segment(StrCh, vecWordsCh);
		pClas->ReleaseMem();
	}

//for(int i=0;i<vecWordsCh.size();i++)
 // cout<<vecWordsCh[i].c_str()<<endl;

	//%%如果含有至少一个英文字符
   if(StrEng.length()>2)
   {
	   /*
		//m_mailterm.clear();

		//%%因为在ReadMail中，将文件中换行符用空格代替了
		string tmpline,tmpword;
		basic_string <char>::size_type npos1,npos2,npos3,npos4;
		npos2 = npos1 =0;

		//%%从npos1 + 1位置开始遍历，直到遇到"\n"。
		//%%其实就是文件中的一行
		while((npos2 = StrEng.find("\n",npos1 + 1)) != -1)
		{
			//从strEng中从npos1开始，截取长度为npos2-npos1的字符串
			//实质就是拿出一行分析
			tmpline = StrEng.substr(npos1,npos2 - npos1);

			//若一行少于一个字符，就找下一行
			if(tmpline.length() < 2)
			{
				npos1 = npos2 + 1;
				continue;
			}

			//在一行末尾添加一个空格
			tmpline += " ";

			npos4 = npos3 = 0;
			//long wordhash;
			//%%以空格为拆分点，将一行字符串拆分成一个个词
			while((npos4 = tmpline.find(" ",npos3 + 1))!= -1)
			{
				tmpword = tmpline.substr(npos3,npos4 - npos3);
				vecWordsEng.push_back(tmpword);
				//wordhash = hash((char *)tmpword.c_str(),tmpword.length());
				//if(m_mailterm.find(wordhash) != m_mailterm.end())
					//m_mailterm[wordhash] ++;
				//else
					//m_mailterm[wordhash] = 1;
				npos3 = npos4 + 1;
			}

			npos1 = npos2 + 1;
		}
		*/

	   string::iterator h_iter = StrEng.begin();
	   string::iterator t_iter;
	   while(h_iter != StrEng.end() )
	   {
		   h_iter = find_if(h_iter,StrEng.end(), charactor_number );
		   t_iter = find_if(h_iter, StrEng.end(), not_charactor_number );

		   if(h_iter != StrEng.end() )
				vecWordsEng.push_back(string(h_iter, t_iter) );

		   h_iter = t_iter;
		   
	   }

   }

   else
   {
		vecWordsEng.push_back(StrEng);
   }
	StrEng.clear();
	StrCh.clear();
}

bool HASHBAGGING::charactor_number(char c)
{
	return isalnum(c);
}

bool HASHBAGGING::not_charactor_number(char c)
{
	return !isalnum(c);
}



/*
WordTrain函数：
功能：根据mail_mode，利用分离出来的中英文词表训练对应的哈希表

*/
void HASHBAGGING::WordTrain(string mail_mode)
{
	////%%body中的0~14是什么？
	//std::string temst=m_reader->GetFrom()+m_reader->GetTo()+m_reader->GetSubject()+(m_reader->GetBody()).substr(0,14);

	////%%为什么要对整个字符串做？？？？？？？？？？？？
	//long tempname=StrHash((char *)temst.c_str(),temst.length());

	//%%只构造了一个分类器的时候
	if(m_Bagging_Gram.size()==1)
	{
		//%%两种什么类型？？？？
		//%%mail_mode只在训练时出现
		if(!mail_mode.compare("spam"))
		{
			//%%通过train的第二个参数确定训练的哈希表
			//%%训练spam类哈希表
			m_Bagging_Gram[0]->Train(vecWordsEng,0);
			m_Bagging_Gram[0]->Train(vecWordsCh,0);
		}		
		else if(!mail_mode.compare("ham"))
		{
			m_Bagging_Gram[0]->Train(vecWordsEng,1);
			m_Bagging_Gram[0]->Train(vecWordsCh,1);
		}
	}
}

/*
WordClassify函数：
功能：将中英文词表传入分类器计算分类的阈值

*/
float HASHBAGGING::WordClassify()
{
	float sumeng=0,sumch=0;

	for(unsigned i=0;i<m_Bagging_Gram.size();i++)
	{
		//英语的分类
		sumeng+=m_Bagging_Gram[i]->Classify(vecWordsEng);
		//中文的文类
		sumch+=m_Bagging_Gram[i]->Classify(vecWordsCh);
	}

	return sumch+sumeng;
}

long HASHBAGGING::StrHash (char *str, long len)
{
	long i;
	long hval;
	char *hstr;
	char chtmp;

	hval= len;

	hstr = (char *) &hval;

	for ( i = 0; i < len; i++)
	{

		hstr[0] ^= str[i];
		hstr[1] ^= str[i];
		hstr[2] ^= str[i];
		hstr[3] ^= str[i];

		hval = hval + (( hval >> 12) & 0x0000ffff) ;

		chtmp = hstr [0];
		hstr[0] = hstr[3];
		hstr [3] = chtmp;

		hval = (hval << 3 ) + (hval >> 29);
	}
	return (hval);
}
