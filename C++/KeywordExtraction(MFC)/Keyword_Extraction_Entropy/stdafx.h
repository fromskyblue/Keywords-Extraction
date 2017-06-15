
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define WM_MyMessage (WM_USER+100)
#include <iostream>
#include <regex>
#include <fstream>  
#include <sstream>
#include <utility>
#include <algorithm>
#include <string>
#include <cctype>
#include <set>

using namespace std;

typedef struct  //用于一个簇类中文档相似度排列的结构
{
	string word;
	double EDnor; 
	int frequency; //需要的
	vector<int> t_loc; //需要的
	vector<int> d_list; //需要的

}Node;


string LoadDoc(string a);
string pre_treatment(string b);
string change(CString p);
void savekeyword(string address);
UINT ThreadFunc(LPVOID lpParam);
UINT ThreadFunc1(LPVOID lpParam);
int delet_stopword(string address,vector<Node> &delet_result);
vector<Node> getresult();

extern string doc;
extern string doc_pretreament;
extern Node *result;
extern string doc_address;
extern int matix;


#pragma comment (lib,"Keyword_Extraction.lib")
extern Node* keyword_extra_entropy(string text);
extern Node* keyword_extra_entropy_MAX(string text);