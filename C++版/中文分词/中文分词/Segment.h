#pragma once

#include "plib.h"

class NGRAM;
class HMM;

class SEGMENT
{
public:
	SEGMENT(void);
	~SEGMENT(void);
	void AotmSegment(	string &strOrg,
								vector <string> &vecOrg,
								vector <string> &vecIdx);
	void EnhencedProcess(vector <string> &vecOrg, vector <string> &vecIdx);
	void Train(string &strLine, NGRAM *pNGram, HMM *pCpiw);
	void CompleteSegment(string &strLine, 
		NGRAM *pNGram, HMM *pCpiw, vector <string> &vecRes);
};
