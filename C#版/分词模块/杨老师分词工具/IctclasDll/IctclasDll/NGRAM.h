#pragma once

#include "plib.h"

struct NGNode
{
	string strUniItem;
	unsigned nUni;
	string strBiItem;
	unsigned nBi;
	float fWordProb;
	short nPreNode;
	short nPreSubNode;
	bool bInNG;
};

class NGRAM
{
private:
	vector < map < unsigned, unsigned > > m_vNNGram;
	vector < unsigned > m_vTotalNGram;
	unsigned m_nNGram;
	float m_fUniLamed;
	float m_fBiLamed;
	float m_fTriLamed;
	float m_fOOVLamed;
	float m_fOOVProb;
	vector < vector < NGNode > > m_vNGChain;
public:
	NGRAM(void);
	~NGRAM(void);
	void InitTrain(int nNG);
	void Train(vector < string> &vecIdx);
	void ClearMem(void);
	void ExportTrainResult(void);
	void InitSegment(unsigned nNG);
	void LoadNGram(void);
	void MarkTag(vector <string> &vecIdx, vector <short> &vecTag, vector <short> &vecBNG);
	void GenerateUniChain(vector <string> &vecIdx);
	void GenerateBiChain(vector <string> &vecIdx);
	void GenerateTriChain(vector <string> &vecIdx);
	void ClearChain(void);
	void Viterbi(void);
	void GetBestChain(vector <short> &vecTag, vector <short> &vecBNG);
	short GetLastTag(void);
};
