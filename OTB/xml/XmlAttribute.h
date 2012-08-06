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

class XML_ATTRIBUTE : public TQuickListElement
{
	public:
		inline				XML_ATTRIBUTE(char *pName)			{ _pAttrName = _strdup(pName); }
							~XML_ATTRIBUTE();
		inline	void		SetValue(const char *pValue)		{ _pAttrValue = _strdup(pValue); }
				int			WriteToFile(FILE *pFile);

		inline	char		*GetValue(void)						{ return _pAttrValue; }
		inline	int			GetValueAsInt(void)					{ return atoi(_pAttrValue); }
		inline	char		*GetName(void)						{ return _pAttrName; }

	protected:
		char		*_pAttrName;
		char		*_pAttrValue;
};

#endif	//__XML_XMLATTRIBUTE_H__
