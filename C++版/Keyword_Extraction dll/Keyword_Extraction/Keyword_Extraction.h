#include <regex>
#include <fstream>  
#include <sstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <set>
#include <queue>
#include <hash_map>
#include "Term.h"

using namespace std;



#ifdef Keyword_EXPORTS
#define Keyword_API __declspec(dllimport)
#else
#define Keyword_API __declspec(dllexport)
#endif

typedef struct  //用于一个簇类中文档相似度排列的结构
{
	string word;
	double EDnor; 
	int frequency; //需要的
	vector<int> t_loc; //需要的
	vector<int> d_list; //需要的
	
}Node;



Keyword_API Node* keyword_extra_entropy_MAX(string text);
Keyword_API Node* keyword_extra_entropy(string text);
//Keyword_API void Quick_sort(Node ArrayInput[],int nLow,int nHigh);
//Keyword_API int Partition(Node ArrayInput[],int nLow,int nHigh);
//Keyword_API void Swap(Node &p,Node &q);
