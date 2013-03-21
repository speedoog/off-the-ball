//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//     ________   _____  _____    __  .__             __________        .__  .__        //
//     \_____  \_/ ____\/ ____\ _/  |_|  |__   ____   \______   \_____  |  | |  |       //
//      /   |   \   __\\   __\  \   __\  |  \_/ __ \   |    |  _/\__  \ |  | |  |       //
//     /    |    \  |   |  |     |  | |   Y  \  ___/   |    |   \ / __ \|  |_|  |__     //
//     \_______  /__|   |__|     |__| |___|  /\___  >  |______  /(____  /____/____/     //
//             \/                          \/     \/          \/      \/                //
//                                                                                      //
//                          .o                                                          //
//                   �>   .                                      <�                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////


#include "TString.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

TString::TString(const char* str)
{
	if(str == 0)
	{
		_nLength = 0;
		_pString = 0;
	}
	else
	{
		_nLength = strlen(str);
		_pString = new char[_nLength + 1];
		assert(_pString != 0);
		strcpy_s(_pString, _nLength+1, str);
	}
}

TString::TString(const TString& str)
{ 
	if(str.GetCharconst() == 0)
	{
		_nLength = 0;
		_pString = 0;
	}
	else
	{
		_nLength = str._nLength; 
		_pString = new char[_nLength + 1];
		assert(_pString != 0);
		strcpy_s(_pString, _nLength+1, str._pString); 
	}
}

TString& TString::operator +=(const TString& str)
{
	_nLength += str._nLength;
	char* pNew = new char[_nLength + 1];
	assert(pNew != 0);
	strcpy_s(pNew, _nLength+1, _pString);
	strcat_s(pNew, _nLength+1, str._pString);
	delete _pString;
	_pString = pNew;

	return *this;
}

TString& TString::operator =(const char* str)
{
	if (_pString) delete _pString;
	_nLength = strlen(str);
	_pString = new char[_nLength + 1]; 
	assert(_pString != 0);
	strcpy_s(_pString, _nLength+1, str);

	return *this;
}

TString& TString::operator =(const TString& str)
{
	assert(str.GetCharconst() != 0);
	if (_pString) delete _pString;
	_nLength = strlen(str._pString);
	_pString = new char[_nLength + 1]; 
	assert(_pString != 0);
	strcpy_s(_pString, _nLength+1, str._pString);

	return *this;
}

void TString::VarToString(const double var)
{
	const int MAX_SIZE=32;
	char str[MAX_SIZE];

	//gcvt(var, 16, str);
	_gcvt_s(str, MAX_SIZE, var, 16);

	_nLength = strlen(str);
	if (str[_nLength - 1] == '.')
	{
		str[_nLength - 1] = '\0';
		_nLength --;
	}
	_nLength = strlen(str);
	_pString = new char[_nLength + 1];
	assert(_pString != 0);
	strcpy_s(_pString, _nLength+1, str);
}

int TString::Format(const char* format, ...)
{
	assert(format != 0);

	int len;
	char* MaxBuf;
	for(int i = 5; ; i ++)
	{
		len = (int)pow(2, float(i));
		MaxBuf = new char[len];
		if (!MaxBuf) return 0;
		// some UNIX's do not support vsnprintf and snprintf
		len = _vsnprintf_s(MaxBuf, len, len, format, (char*)(&format + 1));
		if (len > 0) break;
		delete []MaxBuf;
		if (len == 0) return 0;
	}

	if (!_pString)
	{
		_nLength = len;
		_pString = new char[_nLength + 1];
	}
	else if (_nLength < len)
	{
		delete _pString;
		_nLength = len;
		_pString = new char[_nLength + 1];
	}
	if (_pString) 
		strcpy_s(_pString, _nLength+1, MaxBuf);
	else
		len = 0;
	delete []MaxBuf;

	return len;
}

bool TString::Match(char* Wildcards, char* str)
{
	bool Yes = 1;

	//iterate and delete '?' and '*' one by one
	while(*Wildcards != '\0' && Yes && *str != '\0')
	{
		if (*Wildcards == '?') str ++;
		else if (*Wildcards == '*')
		{
			Yes = Scan(Wildcards, str);
			Wildcards --;
		}
		else
		{
			Yes = (*Wildcards == *str);
			str ++;
		}
		Wildcards ++;
	}
	while (*Wildcards == '*' && Yes)  Wildcards ++;

	return Yes && *str == '\0' && *Wildcards == '\0';
}

// scan '?' and '*'
bool TString::Scan(char*& Wildcards, char*& str)
{
	// remove the '?' and '*'
	for(Wildcards ++; *str != '\0' && (*Wildcards == '?' || *Wildcards == '*'); Wildcards ++)
		if (*Wildcards == '?') str ++;
	while ( *Wildcards == '*') Wildcards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if (*str == '\0' && *Wildcards != '\0') return false;
	if (*str == '\0' && *Wildcards == '\0')	return true; 
	// else search substring
	else
	{
		char* wdsCopy = Wildcards;
		char* strCopy = str;
		bool  Yes     = 1;
		do 
		{
			if (!Match(Wildcards, str))	strCopy ++;
			Wildcards = wdsCopy;
			str		  = strCopy;
			while ((*Wildcards != *str) && (*str != '\0')) str ++;
			wdsCopy = Wildcards;
			strCopy = str;
		}while ((*str != '\0') ? !Match(Wildcards, str) : (Yes = false) != false);

		if (*str == '\0' && *Wildcards == '\0')	return true;

		return Yes;
	}
}

bool TString::NumericParse(void* pvar, char flag)
{
	assert(_pString != 0);
	char* pTmpStr = _pString;

	// remove the leading ' ' and '\t' at the beginning
	while (*pTmpStr == ' ' || *pTmpStr == '\t')
		pTmpStr++;

	// no desired character found
	if (strlen(pTmpStr) == 0)
		return false;

	char a = pTmpStr[0];
	if ((flag == 'b' || flag == 'C' || flag == 'S' || 
		flag == 'I' || flag == 'L') && a == '-')
		return false;

	if (flag == 'b')
	{
		bool var;
		if (strcmp(pTmpStr, "true") == 0 || strcmp(pTmpStr, "1") == 0 ||
			strcmp(pTmpStr, "TRUE") == 0) var = true;
		else if (strcmp(pTmpStr, "false") == 0 || strcmp(pTmpStr, "0") == 0 ||
			strcmp(pTmpStr, "FALSE") == 0) var = false;
		else // failed
			return false;
		memcpy(pvar, &var, sizeof(bool));
		return true;
	}
	else
	{
		double tmpvar = strtod(pTmpStr, (char**)&pTmpStr);
		if (tmpvar == 0.0 && a != '0')
			return false;   // convertion wrong

		if (flag == 'f' || flag == 'd')
		{
			// allow any float value with one 'f' or 'F' terminated
			if (*pTmpStr == 'f' || *pTmpStr == 'F') 
				pTmpStr++;
		}
		else if (flag == 'l' || flag == 'L')
		{
			// allow any float value with one 'l' or 'L terminated
			if (*pTmpStr == 'l' || *pTmpStr == 'L') 
				pTmpStr++;
		}

		switch(flag)
		{
		case 'c':
			{
				//if (tmpvar < -(0xff / 2 + 1) || tmpvar > 0xff / 2)
				if (tmpvar < -128 || tmpvar > 127)
					return false;   // onerflow
				char var = (char)tmpvar;
				memcpy(pvar, &var, sizeof(char));
			}
			break;
		case 's':
			{
				//if (tmpvar < -(0xffff / 2 + 1) || tmpvar > 0xffff / 2)
				if (tmpvar < -32768.0 || tmpvar > 32768.0)
					return false;   // onerflow
				short var = (short)tmpvar;
				memcpy(pvar, &var, sizeof(short));
			}
			break;
		case 'i':
			{
				//if (tmpvar < -(0xffffffff / 2 + 1) || tmpvar > 0xffffffff / 2)
				if (tmpvar < -2147483648.0 || tmpvar > 2147483647.0)
					return false;   // onerflow
				int var = (int)tmpvar;
				memcpy(pvar, &var, sizeof(int));
			}
			break;
		case 'l':
			{
				//if (tmpvar < -(0xffffffff / 2 + 1) || tmpvar > 0xffffffff / 2)
				if (tmpvar < -2147483648.0 || tmpvar > 2147483647.0)
					return false;   // onerflow
				long var = (long)tmpvar;
				memcpy(pvar, &var, sizeof(long));

			}
			break;
		case 'C':
			{
				//if (tmpvar < 0 || tmpvar > 0xff)
				if (tmpvar < 0.0 || tmpvar > 255)
					return false;   // onerflow
				unsigned char var = (unsigned char)tmpvar;
				memcpy(pvar, &var, sizeof(unsigned char));
			}
			break;
		case 'S':
			{
				//if (tmpvar < 0 || tmpvar > 0xffff)
				if (tmpvar < 0.0 || tmpvar > 65535.0)
					return false;   // onerflow
				unsigned short var = (unsigned short)tmpvar;
				memcpy(pvar, &var, sizeof(unsigned short));
			}
			break;
		case 'I':
			{
				//if (tmpvar < 0 || tmpvar > 0xffffffff)
				if (tmpvar < 0.0 || tmpvar > 4294967295.0)
					return false;   // onerflow
				unsigned int var = (unsigned int)tmpvar;
				memcpy(pvar, &var, sizeof(unsigned int));
			}
			break;
		case 'L':
			{
				//if (tmpvar < 0 || tmpvar > 0xffffffff)
				if (tmpvar < 0.0 || tmpvar > 4294967295.0)
					return false;   // onerflow
				unsigned long var = (unsigned long)tmpvar;
				memcpy(pvar, &var, sizeof(unsigned long));
			}
			break;
		case 'f':
			{
				if (tmpvar < -3.402823466e+38 || tmpvar > 3.402823466e+38)
					return false;   // onerflow
				float var = (float)tmpvar;
				memcpy(pvar, &var, sizeof(float));
			}
			break;
		case 'd':
			memcpy(pvar, &tmpvar, sizeof(double));
			break;
		}

		// remove the leading ' ' and '\t' at the end
		while (*pTmpStr == ' ' || *pTmpStr == '\t')
			pTmpStr++;

		if (*pTmpStr != '\0')
			return false;   // non digital character detected

		return true;
	}
}

void TString::_Test_Me_()
{
	TString sTest;
	sTest="tt";
	sTest="oo";
	sTest+="ii";
	sTest.Format("this %s %d", "test", 18);
	bool b0 =sTest.Search("*hi*");
	bool b1 =sTest.Search("test");
	sTest =3.14f;
	char* pBuff =sTest.GetChar();

	sTest.Clear();
}
