#pragma once
#include "plib.h"
class SEGMENT;
class NGRAM;
class HMM;

class PRISCLAS
{
private:
	unsigned	m_nNG;
	unsigned	m_nMaxWordLen;
	bool		m_bUseNG;
	bool		m_bUseCPIW;
	SEGMENT		*pSeg;
	NGRAM		*pNGram;
	HMM			*pCpiw;
public:
	PRISCLAS(void);
	~PRISCLAS(void);	
	void	InitTrain(void);
	void	InitTrain(int nNG, int nMaxWLen, bool bTrainNG, bool bTrainCPIW);
	void	Train(string strFile);	
	void	ExportTrainResult(void);
	void	InitSegment(void);
	void	InitSegment(int nNG, bool bUseNG, bool bUseCPIW);
	void	ReleaseMem(void);	
	void Segment(string &strLine, vector <string> &vecRes);
};
