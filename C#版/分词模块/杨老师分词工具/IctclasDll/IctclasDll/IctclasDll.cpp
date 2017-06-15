// IctclasDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "IctclasDll.h"

ICTCLASDLL_API void IctclasProcess(char* s, char* r)
{
	string StrCh;
	StrCh = s;
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

	NewStr.copy(r,strlen(NewStr.c_str()),0);
	*(r+strlen(NewStr.c_str())) = '\0';
}