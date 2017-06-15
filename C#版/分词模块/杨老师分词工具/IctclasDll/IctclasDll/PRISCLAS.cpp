#include "StdAfx.h"
#include "prisclas.h"
#include "segment.h"
#include "ngram.h"
#include "hmm.h"

PRISCLAS::PRISCLAS(void)
{
	pNGram = NULL;
	pCpiw = NULL;
}

PRISCLAS::~PRISCLAS(void)
{
}

//*************************************模型训练部分*************************************//

void PRISCLAS::InitTrain(int nNG, int nMaxWLen, bool bTrainNG, bool bTrainCPIW)
{
	m_nNG = nNG;
	m_nMaxWordLen = nMaxWLen;
	m_bUseNG = bTrainNG;
	m_bUseCPIW = bTrainCPIW;
	InitTrain();
}

void PRISCLAS::InitTrain(void)
{
	ReleaseMem();
	if(m_bUseNG)
	{
		pNGram = new NGRAM;
		pNGram->InitTrain(m_nNG);
	}
	else
		pNGram = NULL;
	if(m_bUseCPIW)
	{
		pCpiw = new HMM;
		pCpiw->InitTrain(m_nMaxWordLen);
	}
	else
		pCpiw = NULL;
}

void PRISCLAS::Train(string strFile)
{
	ifstream fp(strFile.c_str(), ios::in);     
    if(!fp)
		return;
	string strLine;
	while(getline(fp,strLine))
	{
		pSeg->Train(strLine, pNGram, pCpiw);
		strLine.clear();
	}
	fp.close();
}

void PRISCLAS::ExportTrainResult(void)
{
	if(pNGram != NULL)
		pNGram->ExportTrainResult();
	if(pCpiw != NULL)
		pCpiw->ExportTrainResult();
}

//*************************************模型训练部分*************************************//

//*************************************分词测试部分*************************************//

void PRISCLAS::InitSegment(int nNG, bool bUseNG, bool bUseCPIW)
{
	m_nNG = nNG;
	m_bUseNG = bUseNG;
	m_bUseCPIW = bUseCPIW;
	InitSegment();
}

void PRISCLAS::InitSegment(void)
{
	ReleaseMem();
	if(m_bUseNG)
	{
		pNGram = new NGRAM;
		pNGram->InitSegment(m_nNG);
	}
	else
		pNGram = NULL;
	if(m_bUseCPIW)
	{
		pCpiw = new HMM;
		m_nMaxWordLen = pCpiw->InitSegment();
	}
	else
		pCpiw = NULL;
}

void PRISCLAS::Segment(string &strLine, vector <string> &vecRes)
{
	pSeg->CompleteSegment(strLine, pNGram, pCpiw, vecRes);
}

//*************************************分词测试部分*************************************//

//*************************************公用函数部分*************************************//
void PRISCLAS::ReleaseMem(void)
{
	if(pNGram != NULL)
	{
		pNGram->ClearMem();
		delete pNGram;
	}
	if(pCpiw != NULL)
	{
		pCpiw->ClearMem();
		delete pCpiw;
	}
}
