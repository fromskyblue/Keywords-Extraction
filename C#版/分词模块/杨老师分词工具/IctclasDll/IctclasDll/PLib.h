#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <FLOAT.H>
#include <math.h>
#include <algorithm>
#include <assert.h>
using namespace std;

typedef map < unsigned, unsigned >::iterator mIt_uu;
typedef map < unsigned, vector < unsigned > >::iterator mIt_uvu;
typedef map < unsigned, vector < float > >::iterator mIt_uvf;

/*class PLib
{
public:
	PLib(void);
	~PLib(void);	
};*/
unsigned HashStr (string str, unsigned len);
bool IsCHN(const char chr1, const char chr2);
bool IsCHNChar(const char chr1, const char chr2);
bool IsCHNEng(const char chr1, const char chr2);
bool IsCHNNum(const char chr1, const char chr2);
bool IsCHNSNum(const string str);
bool IsCHNPun(const string str);
void AsciStrProc(const string strAsci, vector <string> &vecIdx);
unsigned short strnhash (string str, short len);
void ToSafeString(string & strOrg);
