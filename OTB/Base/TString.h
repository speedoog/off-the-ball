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
//                   ¨>   .                                      <¨                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////


#ifndef __BASE_TSTRING_H__
#define __BASE_TSTRING_H__

#pragma once

#include "base.h"

class TString
{
public:
	// Ctor & Dtor
			TString(const TString& str);
			TString(const char* str);
			TString(const double var)				{ VarToString(var); }
			TString()								{ _nLength = 0; _pString = NULL; }
			~TString()								{ Clear(); }

	void	Clear()									{ if (_pString) delete _pString; _pString =NULL; }

	// operator overloading helper
	template <class T> friend TString _cdecl operator +(T var, const TString& str);

	// operator overloading
	TString& operator  =(const char* str);
	TString& operator  =(const TString& str);
	TString& operator  =(const double var)	{ VarToString(var); return *this; }
	template <class T>
	TString  operator  +(T var)				{ TString tstr = *this; return tstr += var; }
	TString& operator +=(double str)		{ return *this += (TString)str; }
	TString& operator +=(const char* str)	{ return *this += (TString)str; }
	TString& operator +=(const TString& str);

	// add more logic comparison operators as following, for example, although not efficient
	virtual bool operator !=(char* str)	{ return strcmp(str, _pString) != 0; }

	// c type string conversion
	operator char* ()					{ return _pString; }
	operator const char* ()	const		{ return _pString; }
	char*	GetChar()					{ return _pString; }
	int		GetAsInt() const			{ return atoi(_pString); }

	// numeric conversion
	template <class T>
	bool GetValue(T& var)	{ return GetVar(var); }

	// search the match string : WildCards can be '?' and '*' combination
	// return value : true (pattern matchs string), false (no match)
	bool Search(const char* WildCards)	{ return Match((char*)WildCards, _pString); }

	// format string
	int Format(const char* format, ...);

protected:
	// can use faster algorithm for search ?
	virtual bool Match(char*, char*);
	virtual bool Scan(char*&, char*&);

	// have any good conversion method ?
	virtual void VarToString(const double var);

	// numeric conversion helpers
	bool NumericParse(void* pvar, char flag);
	bool GetVar(bool& var)				{ return NumericParse((void*)&var, 'b'); }
	bool GetVar(char& var)				{ return NumericParse((void*)&var, 'c'); }
	bool GetVar(short& var)				{ return NumericParse((void*)&var, 's'); }
	bool GetVar(int& var)				{ return NumericParse((void*)&var, 'i'); }
	bool GetVar(long& var)				{ return NumericParse((void*)&var, 'l'); }
	bool GetVar(float& var)				{ return NumericParse((void*)&var, 'f'); }
	bool GetVar(double& var)			{ return NumericParse((void*)&var, 'd'); }
	bool GetVar(unsigned char& var)		{ return NumericParse((void*)&var, 'C'); }
	bool GetVar(unsigned short& var)	{ return NumericParse((void*)&var, 'S'); }
	bool GetVar(unsigned int& var)		{ return NumericParse((void*)&var, 'I'); }
	bool GetVar(unsigned long& var)		{ return NumericParse((void*)&var, 'L'); }

	static void _Test_Me_();

protected:
	// data block
	int   _nLength;
	char* _pString;
};

template <class T>
TString operator +(T var, const TString& str) 
{ 
	TString svar = var;
	return svar += str; 
}

#endif //__BASE_TSTRING_H__
