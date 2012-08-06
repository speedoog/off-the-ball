//----------------------------------------------------------
//
//
//
//
//	XML_PARSER
//
//
//
//
//----------------------------------------------------------

#ifndef __XML_XMLPARSER_H__
#define __XML_XMLPARSER_H__

#pragma once

#include "DynamicBuffer.h"
#include "XmlAttribute.h"
#include "XmlElement.h"

class XML_PARSER
{
public:
	enum XMLRC
	{
		XP_NO_ERROR			= 0,
		XP_CANT_OPEN_FILE,
		XP_PARSER_ERROR,
		XP_NO_TREE
	};

							XML_PARSER();
							~XML_PARSER();
			XMLRC			LoadFromFile(const char* pFileName);
			XMLRC			SaveToFile(const char* pFileName);

			XML_ELEMENT*	FindElement(char* pName, bool bRecursiveSearch=true);
	inline	XML_ELEMENT*	GetRoot(void)			{ return _Root; }

			XML_ELEMENT*	CreateRootElement(char *pName);

protected:
	XML_ELEMENT* _Root;
};

#endif	//__XML_XMLPARSER_H__
