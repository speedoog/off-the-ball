//----------------------------------------------------------
//
//
//
//
//	XML_ATTRIBUTE
//
//
//
//
//----------------------------------------------------------

#ifndef __XML_XMLATTRIBUTE_H__
#define __XML_XMLATTRIBUTE_H__

#pragma once

#include <stdio.h>
#include "../Base/TQuickList.h"
#include "../Base/TString.h"

class XML_ATTRIBUTE : public TQuickListElement
{
	public:
						XML_ATTRIBUTE(char* pName);
						~XML_ATTRIBUTE();
				void	SetName(const char* pName);
				void	SetValue(const char* pValue);
				int		WriteToFile(FILE* pFile);

		inline	char*	GetName()						{ return _sName; }
		inline	char*	GetValue()						{ return _sValue; }
		inline	int		GetValueAsInt()					{ return _sValue.GetAsInt(); }

	protected:
		TString	_sName, _sValue;
};

#endif	//__XML_XMLATTRIBUTE_H__
