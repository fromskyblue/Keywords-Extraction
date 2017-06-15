
// Keyword_Extraction_Entropy.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CKeyword_Extraction_EntropyApp:
// 有关此类的实现，请参阅 Keyword_Extraction_Entropy.cpp
//

class CKeyword_Extraction_EntropyApp : public CWinApp
{
public:
	CKeyword_Extraction_EntropyApp();
	HMODULE m_hRichEdit;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CKeyword_Extraction_EntropyApp theApp;