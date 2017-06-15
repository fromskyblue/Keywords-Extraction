#include <vector>
using namespace std;
typedef struct  
{
	string word;  //词项
	double EDnor; //熵差值，值越大说明词项越关键
	int frequency; //词项出现的频率
	vector<int> t_loc; //词项在文本中出现位置
	vector<int> d_list; //词项在文本中间距
}Node;  //

#pragma comment (lib,"Keyword_Extraction.lib")

//输入 string text 为经过预处理后的文本
extern Node* keyword_extra_entropy(string text);  //一般熵
extern Node* keyword_extra_entropy_MAX(string text);  //最大熵