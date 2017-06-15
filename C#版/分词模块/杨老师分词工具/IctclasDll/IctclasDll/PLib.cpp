#include "StdAfx.h"
#include "plib.h"

unsigned HashStr (string str, unsigned len)
{
	unsigned i, hval;
	char *hstr;
	char chtmp;
	hval= len;
	hstr = (char *) &hval;
	for ( i = 0; i < len; i++)
	{
		hstr[0] ^= str.at(i);
		hstr[1] ^= str.at(i);
		hstr[2] ^= str.at(i);
		hstr[3] ^= str.at(i);
		hval = hval + (( hval >> 12) & 0x0000ffff) ;
		chtmp = hstr [0];
		hstr[0] = hstr[3];
		hstr [3] = chtmp;
		hval = (hval << 3 ) + (hval >> 29);
	}
	return (hval);
}/**/

//unsigned strnhash (char *str, long len)
/*unsigned HashStr (string str, unsigned len)
{
  long i;
  // unsigned long hval;
  long hval;
  unsigned long tmp;

  // initialize hval
  hval= len;

  //  for each character in the incoming text:
  for ( i = 0; i < len; i++)
    {
      //    xor in the current byte against each byte of hval
      //    (which alone gaurantees that every bit of input will have
      //    an effect on the output)

      tmp = str[i];
      tmp = tmp | (tmp << 8) | (tmp << 16) | (tmp << 24);
      hval ^= tmp;

      //    add some bits out of the middle as low order bits.
      hval = hval + (( hval >> 12) & 0x0000ffff) ;

      //     swap most and min significative bytes 
      tmp = (hval << 24) | ((hval >> 24) & 0xff);
      hval &= 0x00ffff00;           // zero most and min significative bytes of hval
      hval |= tmp;                  // OR with swapped bytes

      //    rotate hval 3 bits to the left (thereby making the
      //    3rd msb of the above mess the hsb of the output hash)
      hval = (hval << 3) + (hval >> 29);
    }
  return (hval);
}*/

bool IsCHN(const char chr1, const char chr2)
{
	if(0xA1 <= (unsigned char)chr1 && (unsigned char)chr1 <= 0xF7
		&& 0xA1 <= (unsigned char)chr2 && (unsigned char)chr2 <= 0xFE)
		return true;
	return false;
}

bool IsCHNChar(const char chr1, const char chr2)
{
	if(0xB0 <= (unsigned char)chr1 && (unsigned char)chr1 <= 0xF7
		&& 0xA1 <= (unsigned char)chr2 && (unsigned char)chr2 <= 0xFE)
		return true;
	return false;
}

bool IsCHNEng(const char chr1, const char chr2)
{
	if(0xA3 == (unsigned char)chr1 &&
		((0xC1 <= (unsigned char)chr2 && (unsigned char)chr2 <= 0xDA) ||
		(0xE1 <= (unsigned char)chr2 && (unsigned char)chr2 <= 0xFA)))
		return true;
	return false;
}

bool IsCHNNum(const char chr1, const char chr2)
{
	if((0xA3 == (unsigned char)chr1 &&
		0xB0 <= (unsigned char)chr2 &&
		(unsigned char)chr2 <= 0xB9) ||
		(0xA2 == (unsigned char)chr1 &&
		0xA1 <= (unsigned char)chr2 &&
		(unsigned char)chr2 <= 0xFE))
		return true;
	return false;
}

bool IsCHNSNum(const string str)
{
	string CHNSNum("○ 一 二 三 四 五 六 七 八 九 十 两 零 ");//壹 貮 叁 肆 伍 陆 柒 捌 玖 拾 佰 仟 萬几 数 百千万亿
	if(CHNSNum.find(str) != -1)
		return true;
	return false;
}

bool IsCHNPun(const string str)
{
	string CHNPun("， 。 ！ ； … ‘ ’");// ：·MSR“ ”
	if(CHNPun.find(str) != -1)
		return true;
	return false;
}

void AsciStrProc(const string strAsci, vector <string> &vecIdx)
{
	unsigned np, nNu = 0, nCr = 0, nLen = strAsci.length();	
	for(np = 0; np < nLen; np ++)
	{
		if(isdigit(strAsci[np]))
			nNu ++;
		else if(isalpha(strAsci[np]))
			nCr ++;
		else
			break;
	}
	if(isdigit(strAsci[0]) && nCr == 0)
	{
		if(strAsci.find(":") != -1)
			vecIdx.push_back("#T");
		else
			vecIdx.push_back("#N");
		return;
	}
	if(isalpha(strAsci[0]) && nCr >= nLen - 1 && nNu == 0)
	{
		vecIdx.push_back("#E");
			return;
	}
	vecIdx.push_back("#A");    
}

void ToSafeString(string & strOrg)
{
	for(unsigned np = 0; np < strOrg.length(); np++)
	{
		if(strOrg[np] >= 0)
		{
			if(!isprint(strOrg[np]))
			{
				strOrg.erase(np,1);
				np--;
			}
		}
		else if(np < strOrg.length() - 1)
		{
			if(IsCHN(strOrg[np], strOrg[np + 1]))
				np ++;
			else
			{
				strOrg.erase(np,2);
				np --;
			}	
		}
		else
		{
			strOrg.erase(np,1);
			np--;
		}
	}
}