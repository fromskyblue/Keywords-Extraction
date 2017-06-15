#include "../PRIS-CLAS-DEMO V0.6/PRISCLAS.h"
//#include <boost/tokenizer.hpp>
#include <vector>
#include <string>
#include <set>

using namespace std;
//using namespace boost;

typedef struct {
	int spamall,hamall,spamerror,hamerror;
} m_Error_Unit;

typedef struct {
	int spamall,hamall,spameq,hameq;
	float MI;
} Weight_Unit;

class HASHBAGGING{
public:
	
	string StrCh;  //分别放邮件的中英文字符串
	vector<string> vecWordsCh;


public:
	HASHBAGGING();//Bagging初始化,n为构造数
	//HASHBAGGING(int num);//Bagging初始化,n为构造数
	~HASHBAGGING(void);
	void Initial(int num);//Bagging初始化,n为构造数
	void Release();//Bagging初始化,n为构造数

public:	
	void LoadFea(void);
	void SaveFea(void);
	void StrToVecChineseWord(string str, string & StrEng,string & StrCh);
	long StrHash(char *str, long len);
	void WordParserMail(void);//以word为basic unit对邮件进行解析	

	void WordTrain(string mail_mode);
	float WordClassify();

private:
	//added by xuxin 2010.3.26
	//用于分割英文单词
	static bool charactor_number(char c);
	static bool not_charactor_number(char c);
};