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

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#pragma once

// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
//#include "../Base/TQuickList.h"
#include "DynamicBuffer.h"
#include "XmlAttribute.h"
#include "XmlElement.h"

#pragma warning( disable : 4996)

class XML_PARSER
{
	public:
		enum XMLRC
		{
			XP_NO_ERROR			= 0,
			XP_CANT_OPEN_FILE,
			XP_PARSER_ERROR,
			XP_NO_TREE,

			XP_FORCE_DWORD		= 0xffffffff
		};
	protected:
		XML_ELEMENT		*_Root;

	public:
							XML_PARSER();
							~XML_PARSER();
				XMLRC		LoadFromFile(const char *pFileName);
				XMLRC		SaveToFile(const char *pFileName);

				XML_ELEMENT	*FindElement(char *pName, int bRecursiveSearch=true);
		inline	XML_ELEMENT	*GetRoot(void)			{ return _Root; }

				XML_ELEMENT	*CreateRootElement(char *pName);

};

#endif	//__XML_PARSER_H__
