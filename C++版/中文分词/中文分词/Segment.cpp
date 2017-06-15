//#include "StdAfx.h"
#include "segment.h"
#include "ngram.h"
#include "hmm.h"

//*************************************公用函数部分*************************************//

SEGMENT::SEGMENT(void)
{
}

SEGMENT::~SEGMENT(void)
{
}

void SEGMENT::AotmSegment(	string &strOrg, 
							vector <string> &vecOrg,
							vector <string> &vecIdx)
{
	if(strOrg.length() < 1)
		return;
	unsigned np = 0, nl = 0;
	vecOrg.clear();
	vecIdx.clear();
	while(np < strOrg.length())
	{		
		if(strOrg[np] < 0 && np < strOrg.length() -1)
		{
			if(IsCHNSNum(strOrg.substr(np, 2)))
			{
				nl += 2;
				np += 2;
				if(np >= strOrg.length() - 1 ||
					strOrg[np] >=0 ||
					!IsCHNSNum(strOrg.substr(np, 2)))
					//(!IsCHNSNum(strOrg.substr(np, 2)) &&
					//!IsCHNNum(strOrg[np], strOrg[np + 1])))
				{
					vecOrg.push_back(strOrg.substr(np - nl, nl));
					if( np < (strOrg.length() - 1) &&
						((nl == 8 && strOrg.substr(np, 2) == "年") ||
						 strOrg.substr(np, 2) == "月" ||
						 strOrg.substr(np, 2) == "日" ))
						vecIdx.push_back("#D");					
					else
						vecIdx.push_back("#N");
					nl = 0;
				}
				/*vecOrg.push_back(strOrg.substr(np, 2));
				vecIdx.push_back("#N");
				np += 2;*/
								
			}			
			else if(IsCHNNum(strOrg[np], strOrg[np + 1]))
			{
				nl += 2;
				np += 2;
				if(np < strOrg.length() - 1)
				{
					if(IsCHNNum(strOrg[np], strOrg[np + 1]))// || IsCHNSNum(strOrg.substr(np, 2)))
						continue;
					if(strOrg.find("％",np) == np ||
							strOrg.find("‰",np) == np)
					{
						nl += 2;
						np += 2;
					}
				}
				vecOrg.push_back(strOrg.substr(np - nl, nl));
				if( (np < (strOrg.length() - 1) &&
					((nl == 8 && strOrg.substr(np, 2) == "年") ||
					strOrg.substr(np, 2) == "月" ||
					strOrg.substr(np, 2) == "日" )) ||
					(((np - nl) > 1) && strOrg.substr(np - nl - 2, 2) == "’"))
					vecIdx.push_back("#D");
				else
                    vecIdx.push_back("#N");
				nl = 0;
			}
			else if(IsCHNEng(strOrg[np], strOrg[np + 1]))
			{
				nl += 2;
				np += 2;				
				if(np >= strOrg.length() - 1 || !IsCHNEng(strOrg[np], strOrg[np + 1]))
				{
					vecOrg.push_back(strOrg.substr(np - nl, nl));
					vecIdx.push_back(strOrg.substr(np - nl, nl));
					//vecIdx.push_back("#E");
					nl = 0;
				}				
			}
			else if(IsCHNPun(strOrg.substr(np, 2)))
			{
				vecOrg.push_back(strOrg.substr(np, 2));
				vecIdx.push_back("#S");
				np +=2;
			}
			else// if(IsCHNChar(strOrg[np], strOrg[np + 1]))
			{
				vecOrg.push_back(strOrg.substr(np, 2));
				vecIdx.push_back(strOrg.substr(np, 2));
				np +=2;
			}	
		}
		else if(strOrg[np] >= 0)
		{			
			if(isspace(strOrg[np]))
			{
				vecOrg.push_back(strOrg.substr(np, 1));
				vecIdx.push_back("#W");
				np ++;
			}
			else
			{
				np ++;
				nl ++;
				if(np == strOrg.length() -1 || 
					strOrg[np] < 0 || isspace(strOrg[np]))
				{
					vecOrg.push_back(strOrg.substr(np - nl, nl));
					vecIdx.push_back("#A");
					//AsciStrProc(strOrg.substr(np - nl, nl), vecIdx);
					nl = 0;
				}
			}
		}
		else
			np++;
	}
	vecIdx.push_back("#W");
	vecIdx.push_back("#S");
	//EnhencedProcess(vecOrg, vecIdx);
}

void SEGMENT::EnhencedProcess(vector <string> &vecOrg, vector <string> &vecIdx)
{
	if(vecIdx.size() < 4)
		return;
	unsigned np, nl = 1;	
	for(np = 0; np < (vecIdx.size()); np ++)
	{
		if(	vecIdx[np] == "百" &&
			(	vecIdx[np + 1] == "万" ||
				vecIdx[np + 1] == "亿" ||
				vecIdx[np + 1] == "兆"))
		{
			vecOrg[np] += vecOrg[np + 1];
			vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + 2);
			vecIdx[np] = "#Q";
			vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + 2);
		}
		if(	vecIdx[np] == "千" &&
			(	vecIdx[np + 1] == "万" ||
				vecIdx[np + 1] == "亿" ||
				vecIdx[np + 1] == "兆") &&
				vecIdx[np + 2] != "万")
		{
			vecOrg[np] += vecOrg[np + 1];
			vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + 2);
			vecIdx[np] = "#Q";
			vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + 2);
		}
		if(	vecIdx[np] == "万" &&
			(	//vecIdx[np + 1] == "万" ||
				vecIdx[np + 1] == "亿" ||
				vecIdx[np + 1] == "兆"))
		{
			vecOrg[np] += vecOrg[np + 1];
			vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + 2);
			vecIdx[np] = "#Q";
			vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + 2);
		}
		if(	vecIdx[np] == "亿" &&
			(	vecIdx[np + 1] == "万" ||
				vecIdx[np + 1] == "亿" ||
				vecIdx[np + 1] == "兆"))
		{
			vecOrg[np] += vecOrg[np + 1];
			vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + 2);
			vecIdx[np] = "#Q";
			vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + 2);
		}
	}
	for(np = 0; np < (vecIdx.size()); np ++)
	{
		nl = 1;
		if(vecIdx[np] == "#N" || IsCHNSNum(vecIdx[np]) || vecIdx[np] == "第")
		{
			bool bNum = true;			
			while(	vecIdx[np + nl] == "#N" ||
					vecIdx[np + nl] == "#E" ||
					IsCHNSNum(vecIdx[np + nl]) ||
					vecIdx[np + nl] == "#A" ||
					vecIdx[np + nl] == "百" ||
					vecIdx[np + nl] == "千" ||
					vecIdx[np + nl] == "万" ||
					vecIdx[np + nl] == "亿" ||
					vecIdx[np + nl] == "点" ||
					vecIdx[np + nl] == "分" ||
					vecIdx[np + nl] == "．" ||
					vecIdx[np + nl] == "·" ||
					//vecIdx[np + nl] == "、" ||
					//vecIdx[np + nl] == "—" ||
					//vecIdx[np + nl] == "－" ||					
					vecIdx[np + nl] == "：" ||
					vecIdx[np + nl] == "∶" ||
					vecIdx[np + nl] == "％" ||
					vecIdx[np + nl] == "‰" ||
					vecIdx[np + nl] == "＋" ||
					vecIdx[np + nl] == "／" ||
					vecIdx[np + nl] == "×" )
			{
				if(vecIdx[np + nl] == "#E" || vecIdx[np + nl] == "#A")					
				{
					bNum = false;
					if(IsCHNSNum(vecIdx[np]))
						break;
				}
				if(vecIdx[np + nl] == "点" && vecIdx[np + nl + 1] == "钟")
					break;
				if(vecIdx[np + nl] == "分")
					if( vecIdx[np + nl + 1] == "之")
					{
						vecOrg[np] += vecOrg[np + nl];
						nl ++;
					}
					else
						break;
				vecOrg[np] += vecOrg[np + nl];
				nl++;
			}			
			if(nl > 1)
			{
				if(np + nl < vecIdx.size() - 3 &&
					(vecIdx[np + nl - 1] == "百" ||
					vecIdx[np + nl - 1] == "千" ||
					vecIdx[np + nl - 1] == "万" ||
					vecIdx[np + nl - 1] == "亿"))
				{
					vecOrg[np].erase(vecOrg[np].length() - 2, 2);
					nl --;
				}
				if(nl > 1)
				{
					vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
					if(!bNum)
						vecIdx[np] = "#A";
					vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
				}
			}
			nl = 1;
		}
	}
	for(np = 0; np < (vecIdx.size()); np ++)
	{
		nl = 1;
		if(vecIdx[np] == "#E")
		{
			bool bEng = true;
			while(	vecIdx[np + nl] == "#E" ||
					vecIdx[np + nl] == "#N" ||
					vecIdx[np + nl] == "#A" ||
					vecIdx[np + nl] == "．" ||
					vecIdx[np + nl] == "·" ||
					vecIdx[np + nl] == "—" ||
					vecIdx[np + nl] == "－" ||
					vecIdx[np + nl] == "／")
			{
				if(vecIdx[np + nl] == "#N" || vecIdx[np + nl] == "#A")
					bEng = false;
				vecOrg[np] += vecOrg[np + nl];
				nl++;
			}
			if(nl > 1)
			{
				vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
				if(!bEng)
					vecIdx[np] = "#A";
				vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
			}
			nl = 1;
		}
	}	
	for(np = 0; np < (vecIdx.size()); np ++)
	{
		nl = 1;
		if(vecIdx[np] == "#A")
		{
			while(	vecIdx[np + nl] == "#A" ||
					vecIdx[np + nl] == "#E" ||
					vecIdx[np + nl] == "#N" ||
					vecIdx[np + nl] == "．" ||
					vecIdx[np + nl] == "·" ||
					vecIdx[np + nl] == "—" ||
					vecIdx[np + nl] == "－" ||
					vecIdx[np + nl] == "／" ||
					vecIdx[np + nl] == "＠")
			{
				vecOrg[np] += vecOrg[np + nl];
				nl++;
			}
			if(nl > 1)
			{
				vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
				vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
			}
			nl = 1;
		}
	}
	
	/*for(np = 0; np < (vecIdx.size()); np ++)
	{
		nl = 1;
		if(np > 0)
		if( vecIdx[np] == "·" && np > 0 &&
			vecIdx[np - 1].find("#") == -1 && vecIdx[np + 1].find("#") == -1)
			vecIdx[np] = "#S";
		if( vecIdx[np] == "—" )
		{
			while(vecIdx[np + nl] == "—")
			{
				vecOrg[np] += vecOrg[np + nl];
				vecIdx[np] += vecIdx[np + nl];
				nl++;
			}
			if(nl > 1)
			{
				vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
				vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
			}
			nl = 1;
		}
		if( vecIdx[np] == "—" && np > 0 &&
			vecIdx[np - 1].find("#") == -1 && vecIdx[np + 1].find("#") == -1)
			vecIdx[np] = "#S";
		if( vecIdx[np] == "…" )
		{
			while(vecIdx[np + nl] == "…")
			{
				vecOrg[np] += vecOrg[np + nl];
				vecIdx[np] += vecIdx[np + nl];
				nl++;
			}
			if(nl > 1)
			{
				vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
				vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
			}
			nl = 1;
		}
		if( vecIdx[np] == "×" )
		{
			while(vecIdx[np + nl] == "×")
			{
				vecOrg[np] += vecOrg[np + nl];
				vecIdx[np] += vecIdx[np + nl];
				nl++;
			}
			if(nl > 1)
			{
				vecOrg.erase(vecOrg.begin() + np + 1, vecOrg.begin() + np + nl);
				vecIdx.erase(vecIdx.begin() + np + 1, vecIdx.begin() + np + nl);
			}
			nl = 1;
		}
	}*/
}

//*************************************公用函数部分*************************************//

//*************************************模型训练部分*************************************//

void SEGMENT::Train(string &strLine, NGRAM *pNGram, HMM *pCpiw)
{
	unsigned np1, np2, np3, np4;
	string strWord;
	vector <string> vecOrg, vecIdx, vecWord, vecWSeq;
	vector <unsigned> vecWLen;
	AotmSegment(strLine, vecOrg, vecIdx);
	np2 = 0;
	for(np1 = 0; np1 < vecIdx.size(); np1++)
	{
		if(vecIdx[np1] == "#S")
		{
			if(np1 != np2)
			{
				np4 = np2;
				for(np3 = np2; np3 < np1; np3 ++)
				{
					if(vecIdx[np3] == "#W")
					{
						if(np3 != np4)
						{
							if(pCpiw != NULL)
								pCpiw->TrainEmit(vecWord);
							vecWLen.push_back(vecWord.size());
							vecWSeq.push_back(strWord);
						}
						vecWord.clear();
						strWord.clear();
						np4 = np3 + 1;
					}
					else
					{
						vecWord.push_back(vecIdx[np3]);
						strWord += vecIdx[np3];
					}
				}
				if(vecWord.size() > 0 && pCpiw != NULL)
					pCpiw->TrainEmit(vecWord);
				if(vecWSeq.size() > 0 && pNGram != NULL)
					pNGram->Train(vecWSeq);
				if(vecWLen.size() > 0 && pCpiw != NULL)
					pCpiw->TrainStTr(vecWLen);
				strWord.clear();
				vecWord.clear();
				vecWSeq.clear();
				vecWLen.clear();
			}
			strWord.clear();
			vecWord.clear();
			vecWSeq.clear();
			vecWLen.clear();
			np2 = np1 + 1;
		}
	}
}


//*************************************模型训练部分*************************************//

//*************************************分词测试部分*************************************//

void SEGMENT::CompleteSegment(string &strLine, 
								NGRAM *pNGram, HMM *pCpiw, vector <string> &vecRes)
{
	unsigned np1, np2;
	string strSeged;
	vector <string> vecOrg, vecIdx, vecPart;
	vector <short> vecTag, vecSTag, vecBNG;
	AotmSegment(strLine, vecOrg, vecIdx);
	np2 = 0;	
	for(np1 = 0; np1 < vecIdx.size(); np1++)
	{
		if(vecIdx[np1] == "#W" || vecIdx[np1] == "#S")
		{
			if(np1 != np2)
			{
				vecTag.clear();
				vecBNG.clear();
				if(pNGram != NULL)
					pNGram->MarkTag(vecPart, vecTag, vecBNG);
				if(pCpiw != NULL)
					pCpiw->MarkTag(vecPart, vecTag, vecBNG);
				for(unsigned k = 0; k < vecTag.size(); k ++)
					vecSTag.push_back(vecTag[k]);
			}
			vecSTag.push_back(1);							
			vecPart.clear();
			np2 = np1 + 1;
		}
		else
			vecPart.push_back(vecIdx[np1]);
	}
	np1 = 0;
	strSeged.clear();
	for(unsigned n = 0; n < vecSTag.size(); n ++)
	{
		strSeged.clear();
		for(np2 = 0; np2 < vecSTag[n]; np2 ++)
		{
			if(np1 >= vecOrg.size())
				break;
			strSeged += vecOrg[np1].c_str();
			np1 ++;
		}
		vecRes.push_back(strSeged);
		//strSeged += " ";
	}
	//while((np1 = strSeged.find(" · ")) != -1)
	//	strSeged.replace(np1, 4, "·");
	//return strSeged;
}

//*************************************分词测试部分*************************************//