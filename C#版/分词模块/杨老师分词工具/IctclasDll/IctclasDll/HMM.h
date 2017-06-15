#pragma once

#include "plib.h"
struct HMMNode
{
	float accumProb;
	short nTag;
	short preSubNode;
	bool bAddIn;
};

class HMM
{
private:
	unsigned m_nState;
	unsigned m_nMaxLen;
	unsigned m_nChainLen;
	vector < vector < HMMNode > > m_vHMMChain;
	vector < unsigned > m_vNStart;
	vector < vector < unsigned > > m_vNTrans;
	map < unsigned, vector < unsigned > > m_mNEmit;
	vector < float > m_vFStart;
	vector < vector < float > > m_vFTrans;
	map < unsigned, vector < float > > m_mFEmit;
	set < short > m_setBegTag;
	set < short > m_setEndTag;
public:
	HMM(void);
	~HMM(void);
	void InitTrain(int nMaxLen);
	void TrainEmit(vector < string > vecIdx);
	void TrainStTr(vector < unsigned > &vecLen);
	void ClearMem(void);	
	void ExportTrainResult(void);
	unsigned InitSegment(void);
	void LoadData(void);
	void GenerateEdgeSet(void);
	void MarkTag(	vector <string> &vecIdx,
					vector <short> &vecTag,
					vector <short> &vecBNG);
	void MarkTag(	vector <string> &vecIdx,
					vector <short> &vecTag,
					unsigned nPreLen,
					unsigned nPostLen);	
	void GenerateHMMChain(	vector <string> &vecIdx, 
							vector <short> &vecTag,
							unsigned nPreLen, unsigned nPostLen);
	void Viterbi(void);
	void GetBestChain(vector <short> &vecTag);
	short GetLastTag(void);
};
