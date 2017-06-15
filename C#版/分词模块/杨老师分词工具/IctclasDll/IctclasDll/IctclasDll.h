#ifdef ICTCLASDLL_EXPORTS
#define ICTCLASDLL_API __declspec(dllexport)
#else
#define ICTCLASDLL_API __declspec(dllimport)
#endif

#include "PRISCLAS.h"
#include "iostream"
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

extern ICTCLASDLL_API void IctclasProcess(char* s, char*r);
