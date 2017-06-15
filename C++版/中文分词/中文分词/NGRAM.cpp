//#include "StdAfx.h"
#include "ngram.h"

NGRAM::NGRAM(void)
{
}

NGRAM::~NGRAM(void)
{
}

//*************************************模型训练部分*************************************//

void NGRAM::InitTrain(int nNG)
{
	ClearMem();
	m_nNGram = nNG;
	m_vNNGram.resize(m_nNGram);
	m_vTotalNGram.resize(m_nNGram);
	for(unsigned n = 0; n < m_nNGram; n ++)
		m_vTotalNGram[n] = 0;
}

void NGRAM::Train(vector < string > &vecIdx)
{
	unsigned nVSize = vecIdx.size();
	if(nVSize < 1)
		return;
	unsigned nHashItem, ng = 0;
	string strItem;
	for(unsigned n = 0; n < nVSize; n++)
	{
		ng = 0;
		strItem.clear();
		while(ng < m_nNGram)
		{
			if(n + ng >= nVSize)
				break;
			strItem += vecIdx[n + ng];
			strItem += " ";
			nHashItem = HashStr(strItem, strItem.length());
			mIt_uu pIt = m_vNNGram[ng].find(nHashItem);
			if(pIt != m_vNNGram[ng].end())
				pIt->second ++;
			else
				m_vNNGram[ng][nHashItem] = 1;
			m_vTotalNGram[ng] ++;
			ng ++;
		}
	}
}

void NGRAM::ExportTrainResult(void)
{
	FILE *fp;
	string strFile;
	string NGName[] =  {"Unigram", "Bigram", "Trigram"};
	for(unsigned n = 0; n < m_nNGram; n ++)
	{
		strFile = NGName[n];
		if(!(fp = fopen(strFile.c_str(),"wb")))
			return;
		fwrite(&m_vTotalNGram[n], sizeof( int ), 1, fp);
		for(mIt_uu pIt = m_vNNGram[n].begin(); pIt != m_vNNGram[n].end(); pIt ++)
		{
			fwrite(&(pIt->first), sizeof( int ), 1, fp);
			fwrite(&(pIt->second), sizeof( int ), 1, fp);
		}
		fclose(fp);
	}	
}

//*************************************模型训练部分*************************************//

//*************************************分词测试部分*************************************//

void NGRAM::InitSegment(unsigned nNG)
{
	assert(1 <= nNG && nNG <= 3);
	ClearMem();
	m_nNGram = nNG;
	m_vNNGram.resize(m_nNGram);
	m_vTotalNGram.resize(m_nNGram);
	if(m_nNGram == 1)
	{
		m_fUniLamed = .9;
		m_fBiLamed = .0;
		m_fTriLamed = .0;
		m_fOOVLamed = .1;
	}
	else if(m_nNGram == 2)
	{
		m_fUniLamed = .55;
		m_fBiLamed = .4;
		m_fTriLamed = .0;
		m_fOOVLamed = .05;
	}
	else
	{
		m_fUniLamed = .75;
		m_fBiLamed = .1;
		m_fTriLamed = .1;
		m_fOOVLamed = .05;
	}
	LoadNGram();
}

void NGRAM::LoadNGram(void)
{
	FILE *fp;
	unsigned nItem, nFre;
	string strFile;
	string NGName[] =  {"Unigram", "Bigram", "Trigram"};
	for(unsigned n = 0; n < m_nNGram; n ++)
	{
		strFile = NGName[n];
		if(!(fp = fopen(strFile.c_str(),"rb")))
			return;
		fread(&m_vTotalNGram[n], sizeof( int ), 1, fp);
		while(fread(&nItem, sizeof( int ), 1, fp) > 0)
		{
			fread(&nFre, sizeof( int ), 1, fp);
			m_vNNGram[n][nItem] = nFre;
		}
		fclose(fp);
	}
	m_fOOVProb = m_fOOVLamed / m_vTotalNGram[0];
}

void NGRAM::MarkTag(vector <string> &vecIdx, 
					vector <short> &vecTag,
					vector <short> &vecBNG)
{
	ClearChain();
	if(m_nNGram == 1)
		GenerateUniChain(vecIdx);
	else if(m_nNGram == 2)
		GenerateBiChain(vecIdx);
	else
		GenerateTriChain(vecIdx);
	Viterbi();
	GetBestChain(vecTag, vecBNG);
}

void NGRAM::GenerateUniChain(vector <string> &vecIdx)
{
	unsigned n, k, nVSize = vecIdx.size();	
	string strUniItem;
	unsigned nUniHash;
	float fUni;
	mIt_uu pItUni;
	bool bOOV;
	m_vNGChain.resize(nVSize);
	for(n = 0; n < nVSize; n ++)
	{
		bOOV = true;
		strUniItem.clear();
		for(k = n; (k < (n + 15) && k < nVSize); k ++)
		{
			strUniItem += vecIdx[k];
			nUniHash = HashStr(strUniItem + " ", strUniItem.length() + 1);
			pItUni = m_vNNGram[0].find(nUniHash);
			if(pItUni != m_vNNGram[0].end())
			{
				bOOV = false;
				fUni = (float)pItUni->second / m_vTotalNGram[0];
				NGNode node;
				node.strUniItem = strUniItem + " ";
				node.strBiItem = "";
				node.nUni = pItUni->second;
				node.nBi = 0;
				node.fWordProb = log(m_fUniLamed * fUni + m_fOOVProb);
				node.nPreNode = n - 1;
				node.nPreSubNode = -1;
				node.bInNG = false;
				m_vNGChain[k].push_back(node);
			}
		}
		if(bOOV && (n == 0 || (m_vNGChain[n].size() < 1 && m_vNGChain[n - 1].size() > 0)))
		//if(bOOV && (n == 0 || m_vNGChain[n - 1].size() > 0))
		{
			NGNode node;
			node.strUniItem = vecIdx[n] + " ";
			node.strBiItem = "";
			node.nUni = 1;
			node.nBi = 0;
			node.fWordProb = log(m_fOOVProb);
			node.nPreNode = n - 1;
			node.nPreSubNode = -1;
			node.bInNG = false;
			m_vNGChain[n].push_back(node);
		}
	}
}

void NGRAM::GenerateBiChain(vector <string> &vecIdx)
{
	unsigned n, k, nVSize = vecIdx.size();	
	string strUniItem, strBiItem;
	unsigned nUniHash, nBiHash;
	float fUni, fBi;
	mIt_uu pItUni, pItBi;
	bool bOOV;
	m_vNGChain.resize(nVSize);
	for(n = 0; n < nVSize; n ++)
	{
		bOOV = true;
		strUniItem.clear();
		for(k = n; (k < (n + 15) && k < nVSize); k ++)
		{
			strUniItem += vecIdx[k];
			nUniHash = HashStr(strUniItem + " ", strUniItem.length() + 1);
			pItUni = m_vNNGram[0].find(nUniHash);
			if(pItUni != m_vNNGram[0].end())
			{
				bOOV = false;
				fUni = (float)pItUni->second / m_vTotalNGram[0];
				if(n > 0)
				{					
					for(unsigned i = 0; i < m_vNGChain[n - 1].size(); i ++)
					{
						strBiItem.clear();
						strBiItem = m_vNGChain[n - 1][i].strUniItem + strUniItem + " ";
						nBiHash = HashStr(strBiItem, strBiItem.length());
						pItBi = m_vNNGram[1].find(nBiHash);
						if(pItBi != m_vNNGram[1].end())
						{
							
							fBi = (float)pItBi->second / m_vNGChain[n - 1][i].nUni;
							NGNode node;
							node.strUniItem = strUniItem + " ";
							node.strBiItem = strBiItem;
							node.nUni = pItUni->second;
							node.nBi = pItBi->second;
							node.fWordProb = log(m_fBiLamed * fBi + m_fUniLamed * fUni + m_fOOVProb);
							node.nPreNode = n - 1;
							node.nPreSubNode = i;
							node.bInNG = true;
							m_vNGChain[k].push_back(node);
						}
						
					}					
				}
				//if(n == 0 || (m_vNGChain[n].size() < 1 && m_vNGChain[n - 1].size() > 0))
				if(n == 0 || m_vNGChain[n - 1].size() > 0)
				{
					NGNode node;
					node.strUniItem = strUniItem + " ";
					node.strBiItem = "";
					node.nUni = pItUni->second;
					node.nBi = 0;
					node.fWordProb = log(m_fUniLamed * fUni + m_fOOVProb);
					node.nPreNode = n - 1;
					node.nPreSubNode = -1;
					node.bInNG = false;
					m_vNGChain[k].push_back(node);
				}
			}
		}
		if(bOOV && (n == 0 || (m_vNGChain[n].size() < 1 && m_vNGChain[n - 1].size() > 0)))
		//if(bOOV && (n == 0 || m_vNGChain[n - 1].size() > 0))
		{
			NGNode node;
			node.strUniItem = vecIdx[n] + " ";
			node.strBiItem = "";
			node.nUni = 1;
			node.nBi = 0;
			node.fWordProb = log(m_fOOVProb);
			node.nPreNode = n - 1;
			node.nPreSubNode = -1;
			node.bInNG = false;
			m_vNGChain[n].push_back(node);
		}
	}
}

void NGRAM::GenerateTriChain(vector <string> &vecIdx)
{
	unsigned n, k, nVSize = vecIdx.size();	
	string strUniItem, strBiItem, strTriItem;
	unsigned nUniHash, nBiHash, nTriHash;
	float fUni, fBi, fTri;
	mIt_uu pItUni, pItBi, pItTri;
	bool bOOV;
	m_vNGChain.resize(nVSize);
	for(n = 0; n < nVSize; n ++)
	{
		bOOV = true;
		strUniItem.clear();
		for(k = n; (k < (n + 15) && k < nVSize); k ++)
		{
			strUniItem += vecIdx[k];
			nUniHash = HashStr(strUniItem + " ", strUniItem.length() + 1);
			pItUni = m_vNNGram[0].find(nUniHash);
			if(pItUni != m_vNNGram[0].end())
			{
				bOOV = false;
				fUni = (float)pItUni->second / m_vTotalNGram[0];
				if(n > 0)
				{					
					for(unsigned i = 0; i < m_vNGChain[n - 1].size(); i ++)
					{
						strBiItem.clear();
						strBiItem = m_vNGChain[n - 1][i].strUniItem + strUniItem + " ";
						nBiHash = HashStr(strBiItem, strBiItem.length());
						pItBi = m_vNNGram[1].find(nBiHash);
						if(pItBi != m_vNNGram[1].end())
						{
							m_vNGChain[n - 1][i].bInNG = true;
							fBi = (float)pItBi->second / m_vNGChain[n - 1][i].nUni;
							if(m_vNGChain[n - 1][i].strBiItem.length() > 0)
							{
								strTriItem.clear();
								strTriItem = m_vNGChain[n - 1][i].strBiItem + strUniItem + " ";
								nTriHash = HashStr(strTriItem,strTriItem.length());
								pItTri = m_vNNGram[2].find(nTriHash);
								if(pItTri != m_vNNGram[2].end())
									fTri = (float)pItTri->second / m_vNGChain[n - 1][i].nBi;
								else
									fTri = .0;
							}
							else
								fTri = .0;
							NGNode node;
							node.strUniItem = strUniItem + " ";							
							node.strBiItem = strBiItem;							
							node.nUni = pItUni->second;							
							node.nBi = pItBi->second;							
							node.fWordProb = log(m_fTriLamed * fTri + m_fBiLamed * fBi + m_fUniLamed * fUni + m_fOOVProb);
							//node.fWordProb = m_fTriLamed * fTri + m_fBiLamed * fBi + m_fUniLamed * fUni + m_fOOVProb;
							node.nPreNode = n - 1;							
							node.nPreSubNode = i;
							node.bInNG = true;
							m_vNGChain[k].push_back(node);						
						}
						
					}					
				}
				//if(n == 0 || (m_vNGChain[n].size() < 1 && m_vNGChain[n - 1].size() > 0))
				if(n == 0 || m_vNGChain[n - 1].size() > 0)
				{
					NGNode node;
					node.strUniItem = strUniItem + " ";
					node.strBiItem = "";
					node.nUni = pItUni->second;
					node.nBi = 0;
					node.fWordProb = log(m_fUniLamed * fUni + m_fOOVProb);
					//node.fWordProb = m_fUniLamed * fUni + m_fOOVProb;
					node.nPreNode = n - 1;
					node.nPreSubNode = -1;
					node.bInNG = false;
					m_vNGChain[k].push_back(node);
				}
			}
		}
		//if(bOOV && (n == 0 || m_vNGChain[n - 1].size() > 0))
		if(bOOV && (n == 0 || (m_vNGChain[n].size() < 1 && m_vNGChain[n - 1].size() > 0)))
		{
			NGNode node;
			node.strUniItem = vecIdx[n] + " ";
			node.strBiItem = "";
			node.nUni = 1;
			node.nBi = 0;
			node.fWordProb = log(m_fOOVProb);
			//node.fWordProb = m_fOOVProb;
			node.nPreNode = n - 1;
			node.nPreSubNode = -1;
			node.bInNG = false;
			m_vNGChain[n].push_back(node);
		}
	}
}

void NGRAM::Viterbi(void)
{
	float tmpProb;
	short tmpPreSubNode;
	for (unsigned n = 1; n < m_vNGChain.size(); n ++)
	{
		for (unsigned k = 0; k < m_vNGChain[n].size(); k ++)
		{
			short nPreNode = m_vNGChain[n][k].nPreNode;
			short nPreSubNode = m_vNGChain[n][k].nPreSubNode;
			if(nPreNode == - 1)
				;
			else if(nPreSubNode != -1)
				m_vNGChain[n][k].fWordProb += m_vNGChain[nPreNode][nPreSubNode].fWordProb;
			else
			{
				tmpProb = -FLT_MAX;
				tmpPreSubNode = -1;
				for (unsigned i = 0; i < m_vNGChain[nPreNode].size(); i ++)
				{
					if(m_vNGChain[nPreNode][i].fWordProb > tmpProb)
					{
						tmpProb = m_vNGChain[nPreNode][i].fWordProb;
						tmpPreSubNode = i;
					}
				}
				if(tmpPreSubNode != -1)
				{
					m_vNGChain[n][k].fWordProb += tmpProb;
					m_vNGChain[n][k].nPreSubNode = tmpPreSubNode;
				}
				else
				{
					m_vNGChain[n][k].fWordProb = -FLT_MAX;
					m_vNGChain[n][k].nPreSubNode = tmpPreSubNode;
				}
			}			
		}
	}
}


void NGRAM::GetBestChain(vector <short> &vecTag, vector <short> &vecBNG)
{
	vecTag.clear();
	vecBNG.clear();
	short nItemLen, nPre, nPreSub;
	short nLastSub = GetLastTag();
	short nLast = m_vNGChain.size() - 1;
	do
	{
		nPre = m_vNGChain[nLast][nLastSub].nPreNode;
		nPreSub = m_vNGChain[nLast][nLastSub].nPreSubNode;
		nItemLen = nLast - nPre;
		vecTag.push_back(nItemLen);
		if(m_vNGChain[nLast][nLastSub].bInNG)
			vecBNG.push_back(1);
		else
			vecBNG.push_back(0);
		nLast = nPre;
		nLastSub = nPreSub;		
	}while(nLast != -1);

	reverse(vecTag.begin(), vecTag.end());
	reverse(vecBNG.begin(), vecBNG.end());
}

short NGRAM::GetLastTag(void)
{
	float tmpProb = -FLT_MAX;
	short lastTag;
	short nEnd = m_vNGChain.size() - 1;
	for (short n = m_vNGChain[nEnd].size() - 1; n >= 0; n --)
		if(m_vNGChain[nEnd][n].fWordProb > tmpProb)
		{
			tmpProb = m_vNGChain[nEnd][n].fWordProb;
			lastTag = n;
		}
	return lastTag;
}
//*************************************分词测试部分*************************************//

//*************************************公用函数部分*************************************//

void NGRAM::ClearMem(void)
{
	ClearChain();
	for(unsigned n = 0; n < m_vNNGram.size(); n++)
		m_vNNGram[n].clear();
	m_vTotalNGram.clear();
	m_nNGram = 0;
}


void NGRAM::ClearChain(void)
{
	for(unsigned n = 0; n < m_vNGChain.size(); n++)
		m_vNGChain[n].clear();
	m_vNGChain.clear();
}

