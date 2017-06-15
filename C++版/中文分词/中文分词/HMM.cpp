//#include "StdAfx.h"
#include "hmm.h"

HMM::HMM(void)
{
}

HMM::~HMM(void)
{
}

//*************************************模型训练部分*************************************//

void HMM::InitTrain(int nMaxLen)
{
	ClearMem();
	m_nMaxLen = nMaxLen;
	m_nState = (nMaxLen + 1) * nMaxLen / 2;
	m_vNStart.resize(m_nMaxLen + 2);
	m_vNTrans.resize(m_nMaxLen + 2);
	for(unsigned n = 0; n < (m_nMaxLen + 2); n ++)
	{
		m_vNStart[n] = 1;
		m_vNTrans[n].resize(m_nMaxLen + 2);
		for(unsigned k = 0; k < (m_nMaxLen + 2); k ++)
			m_vNTrans[n][k] = 1;
		m_vNTrans[n][0] = m_nMaxLen + 1;
	}
	m_vNStart[0] = m_nMaxLen + 1;
}

void HMM::TrainEmit(vector < string > vecIdx)
{
	unsigned nHashItem, nBias, nVSize = vecIdx.size();
	if(nVSize < 1)
		return;
	if(nVSize <= m_nMaxLen)
		nBias = (nVSize - 1) * nVSize / 2;
	else
		nBias = (m_nMaxLen - 1) * m_nMaxLen / 2;
	string strItem;
	for(unsigned n = 0; n < nVSize; n ++)
	{
		strItem = vecIdx[n];
		nHashItem = HashStr(strItem, strItem.length());
		if(m_mNEmit.find(nHashItem) == m_mNEmit.end())
		{
			m_mNEmit[nHashItem].resize(m_nState + 1);
			for(unsigned k = 0; k <= m_nState; k ++)
				m_mNEmit[nHashItem][k] = 1;
			for(unsigned k = 1; k < (m_nMaxLen + 1); k++)
				m_vNTrans[k][0] += k;
			m_vNTrans[m_nMaxLen + 1][0] ++;
			if((nBias + n) < m_nState)
				m_mNEmit[nHashItem][nBias + n] ++;
			else
				m_mNEmit[nHashItem][m_nState] ++;
		}
		else
		{
			if((nBias + n) < m_nState)
				m_mNEmit[nHashItem][nBias + n] ++;
			else
				m_mNEmit[nHashItem][m_nState] ++;
		}
	}
}

void HMM::TrainStTr(vector < unsigned > &vecLen)
{
	unsigned nPreLen, nCurLen, nVSize = vecLen.size();
	if(nVSize < 1)
		return;
	nPreLen = vecLen[0];
	if(nPreLen <= m_nMaxLen)
	{
		m_vNStart[nPreLen] ++;
		m_vNTrans[nPreLen][0] ++;
	}
	else
	{
		m_vNStart[m_nMaxLen + 1] ++;
		m_vNTrans[m_nMaxLen + 1][0] ++;
	}
	m_vNStart[0] ++;
	for(unsigned n = 1; n < nVSize; n++)
	{
		nCurLen= vecLen[n];
		if(nPreLen <= m_nMaxLen)
			if(nCurLen <= m_nMaxLen)
			{
				m_vNTrans[nPreLen][nCurLen] ++;
				m_vNTrans[nCurLen][0] ++;
			}
			else
			{
				m_vNTrans[nPreLen][m_nMaxLen + 1] ++;
				m_vNTrans[m_nMaxLen + 1][0] ++;
			}
		else
			if(nCurLen <= m_nMaxLen)
			{
				m_vNTrans[m_nMaxLen + 1][nCurLen] ++;
				m_vNTrans[nCurLen][0] ++;
			}
			else
			{
				m_vNTrans[m_nMaxLen + 1][m_nMaxLen + 1] ++;
				m_vNTrans[m_nMaxLen + 1][0] ++;
			}
		nPreLen = nCurLen;
	}
}

void HMM::ExportTrainResult(void)
{
	FILE *fp;
	unsigned n, k;
	if(!(fp = fopen("Start","wb")))
		return;
	fwrite(&m_nMaxLen, sizeof( int ), 1, fp);
	for(n = 0; n < m_vNStart.size(); n ++)
		fwrite(&m_vNStart[n], sizeof( int ), 1, fp);
	fclose(fp);
	if(!(fp = fopen("Trans","wb")))
		return;
	for(n = 1; n < m_vNTrans.size(); n ++)
		for(k = 0; k < m_vNTrans[n].size(); k ++)
			fwrite(&m_vNTrans[n][k], sizeof( int ), 1, fp);
	fclose(fp);
	if(!(fp = fopen("Emit","wb")))
		return;
	for(n = 1; n < m_nMaxLen + 1; n++)
		for(k = 0; k < n; k ++)
			fwrite(&m_vNTrans[n][0], sizeof( int ), 1, fp);
	fwrite(&m_vNTrans[m_nMaxLen + 1][0], sizeof( int ), 1, fp);
	for(mIt_uvu pIt = m_mNEmit.begin(); pIt != m_mNEmit.end(); pIt ++)
	{
		fwrite(&(pIt->first), sizeof( int ), 1, fp);
		for(n = 0; n < (m_nState + 1); n ++)
			fwrite(&(pIt->second[n]), sizeof( int ), 1, fp);
	}		
	fclose(fp);
}

//*************************************模型训练部分*************************************//

//*************************************分词测试部分*************************************//


unsigned HMM::InitSegment(void)
{
	ClearMem();	
	LoadData();
	GenerateEdgeSet();
	return m_nMaxLen;
}

void HMM::LoadData(void)
{
	FILE *fp;
	unsigned n, k, nPre, nCur;
	//Initial Loading
	if(!(fp = fopen("Start","rb")))
		return;	
	fread(&m_nMaxLen, sizeof( int ), 1, fp);
	m_nState = (m_nMaxLen + 1) * m_nMaxLen / 2;
	m_vFStart.resize(m_nState + 1);
	m_vFTrans.resize(m_nState + 1);
	for(n = 0; n < (m_nState + 1); n ++)
		m_vFTrans[n].resize(m_nState + 1);
    //Load Strat
	unsigned nTotalStart, nStart;
	fread(&nTotalStart, sizeof( int ), 1, fp);
	for(n = 0; n < (m_nMaxLen + 1); n ++)
	{
		fread(&nStart, sizeof( int ), 1, fp);
		if(n < m_nMaxLen)
			nCur = (n + 1) * n / 2;
		else
			nCur = m_nState;
		m_vFStart[nCur] = log((float)nStart / nTotalStart);
	}
	fclose(fp);
	//Load Trans
	if(!(fp = fopen("Trans","rb")))
		return;
	for(n = 0; n < (m_nMaxLen + 1); n ++)
	{
		if(n < m_nMaxLen)
			nPre = (n + 2)*(n + 1)/2 -1;
		else
			nPre = m_nState;
		unsigned nPreNum, nCurNum;
		fread(&nPreNum, sizeof( int ), 1, fp);
		for(k = 0; k < (m_nMaxLen + 1); k ++)
		{
			if(k < m_nMaxLen)
				nCur = (k + 1) * k / 2;
			else
				nCur = m_nState;
			fread(&nCurNum, sizeof( int ), 1, fp);
			m_vFTrans[nPre][nCur] = log((float)nCurNum / nPreNum);
		}
	}
	fclose(fp);
	//Load Emit
	if(!(fp = fopen("Emit","rb")))
		return;
	unsigned nItem, *nNum, *nWL;
	nNum = new unsigned[m_nState + 1];
	nWL = new unsigned[m_nState + 1];
	fread(nWL, sizeof( int ), m_nState + 1, fp);
	while(fread(&nItem, sizeof( int ), 1, fp) > 0)
	{
		m_mFEmit[nItem].resize(m_nState + 1);
		fread(nNum, sizeof( int ), m_nState + 1, fp);
		for(n = 0; n < (m_nState + 1); n ++)
			m_mFEmit[nItem][n] = log((float)nNum[n] / nWL[n]);
	}		
	fclose(fp);
}


void HMM::GenerateEdgeSet(void)
{
	for(unsigned n = 1; n <= m_nMaxLen; n++)
	{
		m_setBegTag.insert((n - 1) * n / 2);
		m_setEndTag.insert((n + 1) * n / 2 - 1);
	}
	m_setEndTag.insert(m_nState);
}



void HMM::MarkTag(	vector <string> &vecIdx, 
					vector <short> &vecTag,
					vector <short> &vecBNG)
{	
	unsigned nCrSize = vecIdx.size();
	unsigned nWrSize = vecBNG.size();
	if(nCrSize < 1)
		return;
	if(nWrSize < 1)
	{
		vecTag.clear();
		MarkTag(vecIdx, vecTag, 0 , 0);
		return;
	}
	unsigned nPreLen, nPostLen, nPre = 0, nCur = 0, nBeg = 0, nEnd = 0;
	vector <short> vecPos, vecTTag;//vecPTag, 
	for(nCur = 0; nCur < nWrSize; nCur ++)
	{
		vecPos.push_back(nBeg);
		nBeg += vecTag[nCur];
	}
	for(nCur = 0; nCur < nWrSize; nCur ++)
	{
		while(nCur < nWrSize && vecBNG[nCur] == 0)// || vecTag[nCur] > 1))
		{
			nCur ++;
		}
		if((nCur - nPre) > 1)
		{
			nBeg = vecPos[nPre];
			if(nPre > 0)
				nPreLen = vecTag[nPre - 1];
			else
				nPreLen = 0;
			if(nCur < nWrSize)
			{
				nEnd = vecPos[nCur] - 1;
				nPostLen = vecTag[nCur];
			}
			else
			{
				nEnd = nCrSize - 1;
				nPostLen = 0;
			}
			vector <string> vecPIdx(vecIdx.begin() + nBeg, vecIdx.begin() + nEnd + 1);
			vector <short> vecPTag(vecTag.begin() + nPre, vecTag.begin() + nCur);
			MarkTag(vecPIdx, vecPTag, nPreLen, nPostLen);
			for(unsigned n = 0; n < vecPTag.size(); n ++)
				vecTTag.push_back(vecPTag[n]);
			vecPIdx.clear();
			vecPTag.clear();			
		}
		else if((nCur - nPre) == 1)
			vecTTag.push_back(vecTag[nCur - 1]);
		if(nCur >= nWrSize)
			break;
		vecTTag.push_back(vecTag[nCur]);
		nPre = nCur + 1;
	} 
	vecTag.clear();
	for(nCur = 0; nCur < vecTTag.size(); nCur ++)
		vecTag.push_back(vecTTag[nCur]);
}

void HMM::MarkTag(vector <string> &vecIdx,
				  vector <short> &vecTag,
				  unsigned nPreLen,
				  unsigned nPostLen)
{
	m_nChainLen = vecIdx.size();
	if(m_nChainLen < 1)
		return;
	if(m_nChainLen == 1)
	{
		vecTag.clear();
		vecTag.push_back(1);
		return;
	}
	GenerateHMMChain(vecIdx, vecTag, nPreLen, nPostLen);
	Viterbi();
	GetBestChain(vecTag);
}

void HMM::GenerateHMMChain(vector <string> &vecIdx,
						   vector <short> &vecTag,
						   unsigned nPreLen, unsigned nPostLen)
{
	m_vHMMChain.clear();
	m_vHMMChain.resize(m_nChainLen);	

	short nPreEnd, nPostBeg;
	if(nPreLen <= m_nMaxLen)
		nPreEnd = (nPreLen + 1) * nPreLen / 2 -1;
	else
		nPreEnd = m_nState;
	if(nPostLen <= m_nMaxLen)
		nPostBeg = (nPostLen - 1) * nPostLen / 2;
	else
		nPostBeg = (m_nMaxLen - 1) * m_nMaxLen / 2;

	unsigned nSize = vecIdx.size();
	unsigned nMaxTag;
	if(nSize <= m_nMaxLen)
		nMaxTag = (nSize + 1) * nSize /2 - 1;
	else
		nMaxTag = m_nState;

	vector <short> vecBEnd;
	vecBEnd.resize(nSize);
	short np, nEnd = -1;	
	if(vecTag.size() > 0)
	{
		for(np = 0; np < nSize; np ++)
			vecBEnd[np] = 0;		
		for(np = 0; np < vecTag.size(); np ++)
		{
			nEnd += vecTag[np];
			vecBEnd[nEnd] = 1;
		}
	}
	else
	{
		for(np = 0; np < nSize; np ++)
			vecBEnd[np] = 1;
	}

	bool bPreHasEnd = false;
	unsigned n, k, m, nPreTag, nHash = HashStr(vecIdx[0], vecIdx[0].length());
	mIt_uvf pItEm = m_mFEmit.find(nHash);
	for (k = 0; k <= nMaxTag; k ++)
		if(m_setBegTag.find(k) != m_setBegTag.end())
		{
			if(vecBEnd[0] == 0 && k == 0)
				continue;
			HMMNode node;
			node.bAddIn = true;
			if(pItEm != m_mFEmit.end())
				node.accumProb = pItEm->second[k];
			else
				node.accumProb = .0;
			if(nPreLen > 0)
				node.accumProb += m_vFTrans[nPreEnd][k];
			else
				node.accumProb += m_vFStart[k];
			node.nTag = k;
			node.preSubNode = -1;
			m_vHMMChain[0].push_back(node);
		}
	for(n = 1; n < (m_nChainLen - 1); n ++)
	{
		nSize --;
		if(nSize <= m_nMaxLen)
			nMaxTag = (nSize + 1) * nSize /2 - 1;
		else
			nMaxTag = m_nState;

		nHash = HashStr(vecIdx[n], vecIdx[n].length());
		pItEm = m_mFEmit.find(nHash);
		bPreHasEnd = false;		
		for(m = 0; m < m_vHMMChain[n - 1].size(); m ++)
		{
			nPreTag = m_vHMMChain[n - 1][m].nTag;
			if(m_setEndTag.find(nPreTag) != m_setEndTag.end())
			{
				if(nPreTag >= m_nState -1 && vecBEnd[n] ==1)
				{
					HMMNode node;
					node.bAddIn = true;
					if(pItEm != m_mFEmit.end())
						node.accumProb = pItEm->second[m_nState];
					else
						node.accumProb = .0;
					node.nTag = m_nState;
					node.preSubNode = m;
					m_vHMMChain[n].push_back(node);
				}
				if(bPreHasEnd)
					continue;
				bPreHasEnd = true;
				for (k = 0; k <= nMaxTag; k ++)
					if(m_setBegTag.find(k) != m_setBegTag.end())
					{
						if(vecBEnd[n] == 0 && k == 0)
							continue;
						HMMNode node;
						node.bAddIn = true;
						if(pItEm != m_mFEmit.end())
							node.accumProb = pItEm->second[k];
						else
							node.accumProb = .0;
						node.accumProb += m_vFTrans[nPreTag][k];
						node.nTag = k;
						node.preSubNode = -1;
						m_vHMMChain[n].push_back(node);
					}
			}
			else
			{				
				if(	vecBEnd[n] == 0 &&
					m_setEndTag.find(nPreTag + 1) != m_setEndTag.end() &&
					nPreTag < (m_nState - 2))
					continue;
				HMMNode node;
				node.bAddIn = true;
				if(pItEm != m_mFEmit.end())
					node.accumProb = pItEm->second[nPreTag + 1];
				else
					node.accumProb = .0;
				node.nTag = nPreTag + 1;
				node.preSubNode = m;
				m_vHMMChain[n].push_back(node);
			}
		}
	}
	
	nMaxTag = 1;
	nHash = HashStr(vecIdx[m_nChainLen - 1], vecIdx[m_nChainLen - 1].length());
	pItEm = m_mFEmit.find(nHash);
	bPreHasEnd = false;
	for(m = 0; m < m_vHMMChain[m_nChainLen - 2].size(); m ++)
	{
		nPreTag = m_vHMMChain[m_nChainLen - 2][m].nTag;
		if(m_setEndTag.find(nPreTag) != m_setEndTag.end())
		{
			if(nPreTag >= (m_nState -1))
			{
				HMMNode node;
				node.bAddIn = true;
				if(pItEm != m_mFEmit.end())
					node.accumProb = pItEm->second[m_nState];
				else
					node.accumProb = .0;
				if(nPostLen > 0)
				{
					if(nPreTag == m_nState)
						node.accumProb += m_vFTrans[m_nState][nPostBeg];
					else
						node.accumProb += m_vFTrans[m_nState - 1][nPostBeg];
				}
				node.nTag = m_nState;
				node.preSubNode = m;
				m_vHMMChain[m_nChainLen - 1].push_back(node);
			}
			if(bPreHasEnd)
				continue;
			bPreHasEnd = true;
			HMMNode node;
			node.bAddIn = true;
			if(pItEm != m_mFEmit.end())
				node.accumProb = pItEm->second[0];
			else
				node.accumProb = .0;
			if(nPostLen > 0)
				node.accumProb += m_vFTrans[0][nPostBeg];
			node.accumProb += m_vFTrans[nPreTag][0];
			node.nTag = 0;
			node.preSubNode = -1;
			m_vHMMChain[m_nChainLen - 1].push_back(node);
		}
		else if(m_setEndTag.find(nPreTag + 1) != m_setEndTag.end())
		{
			HMMNode node;
			node.bAddIn = true;
			if(pItEm != m_mFEmit.end())
				node.accumProb = pItEm->second[nPreTag + 1];
			else
				node.accumProb = .0;
			if(nPostLen > 0)
				node.accumProb += m_vFTrans[nPreTag + 1][nPostBeg];
			node.nTag = nPreTag + 1;
			node.preSubNode = m;
			m_vHMMChain[m_nChainLen - 1].push_back(node);
		}
	}
}

void HMM::Viterbi(void)
{
	float tmpProb;
	short tmpPreSubNode;
	for (unsigned n = 1; n < m_nChainLen; n ++)
	{
		for (unsigned k = 0; k < m_vHMMChain[n].size(); k ++)
		{
			short nPreSubNode = m_vHMMChain[n][k].preSubNode;
			if(nPreSubNode != -1)
				m_vHMMChain[n][k].accumProb += m_vHMMChain[n - 1][nPreSubNode].accumProb;
			else
			{
				tmpProb = -FLT_MAX;
				tmpPreSubNode = -1;
				for (unsigned m = 0; m < m_vHMMChain[n - 1].size(); m ++)
				{
					if(m_setEndTag.find(m_vHMMChain[n - 1][m].nTag) == m_setEndTag.end())
						continue;
					if(m_vHMMChain[n - 1][m].accumProb > tmpProb)
					{
						tmpProb = m_vHMMChain[n - 1][m].accumProb;
						tmpPreSubNode = m;
					}
				}
				m_vHMMChain[n][k].accumProb += tmpProb;
				m_vHMMChain[n][k].preSubNode = tmpPreSubNode;
			}			
		}
	}
}

void HMM::GetBestChain(vector <short> &vecTag)
{
	ofstream resfp("tmp.txt",ios::app);     
    if(!resfp)
		return;
	resfp<<endl<<"********************************"<<endl<<endl;

	vecTag.clear();
	short nTag, nPre, nLen;	
	short nLast = m_nChainLen - 1;	
	short nSub = GetLastTag();
	for(nPre = m_nChainLen - 1; nPre >= 0; nPre --)
	{
		nTag = m_vHMMChain[nPre][nSub].nTag;
		nSub = m_vHMMChain[nPre][nSub].preSubNode;
		if(m_setBegTag.find(nTag) != m_setBegTag.end())
		{
			nLen = nLast - nPre + 1;
			vecTag.push_back(nLen);
			resfp<<nLen<<" ";
			nLast = nPre - 1;
		}
	}
	resfp<<endl;
	resfp.close();

	reverse(vecTag.begin(), vecTag.end());
}

short HMM::GetLastTag(void)
{
	float tmpProb = -FLT_MAX;
	short lastTag;
	short nEnd = m_nChainLen - 1;
	for (short n = m_vHMMChain[nEnd].size() - 1; n >= 0; n --)
		if(m_vHMMChain[nEnd][n].accumProb > tmpProb)
		{
			tmpProb = m_vHMMChain[nEnd][n].accumProb;
			lastTag = n;
		}
	return lastTag;
}

//*************************************分词测试部分*************************************//

//*************************************公用函数部分*************************************//

void HMM::ClearMem(void)
{
	m_vNStart.clear();
	m_vFStart.clear();
	for(unsigned n = 0; n < m_vNTrans.size(); n ++)
		m_vNTrans[n].clear();
	m_vNTrans.clear();
	for(unsigned n = 0; n < m_vFTrans.size(); n ++)
		m_vFTrans[n].clear();
	m_vFTrans.clear();
	for(mIt_uvu pIt = m_mNEmit.begin(); pIt != m_mNEmit.end(); pIt ++)
		pIt->second.clear();
	m_mNEmit.clear();
	for(mIt_uvf pItf = m_mFEmit.begin(); pItf != m_mFEmit.end(); pItf ++)
		pItf->second.clear();
	m_mFEmit.clear();
	m_nMaxLen = 0;
	m_nState = 0;
}

