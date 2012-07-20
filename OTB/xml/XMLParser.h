//----------------------------------------------------------
//
//	XMLParser Buffer Implements
//
//----------------------------------------------------------

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "QDT_List.h"
#include "DynamicBuffer.h"

#pragma warning( disable : 4996)

class XML_ATTRIBUTE : public QDT_LIST_ELEMENT
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

class XML_ELEMENT : public QDT_LIST_ELEMENT
{
	public:
		inline					XML_ELEMENT(char *pName)				{ _pFather=0; strcpy(_AttrName, pName); }
		inline	void			SetFather(XML_ELEMENT *pFather)			{ _pFather = pFather; }
		inline	XML_ELEMENT		*GetFather(void)						{ return _pFather; }
		inline	void			AddChild(XML_ELEMENT *pChild)			{ _Childs.InsertQueue(pChild); pChild->SetFather(this); }
		inline	void			AddAttribute(XML_ATTRIBUTE *pAttrib)	{ _Attributes.InsertQueue(pAttrib); }
				int				SaveToFile(FILE *pFile, int IndentLevel);
		inline	XML_ELEMENT		*GetFirstChild(void)					{ return (XML_ELEMENT*)_Childs.GetHead(); }

		inline	char			*GetName(void)							{ return _AttrName; }
		inline	char			*GetText(void)							{ return _sText.GetBufferAsString(); }
		inline	DYNAMIC_BUFFER&	GetTextBuffer(void)						{ return _sText; }
		inline	void			SetText(char* pText)					{ _sText.Release(); _sText.AppendStringNull(pText); }

				XML_ATTRIBUTE	*FindAttribute(const char *pAttribName);
				XML_ELEMENT		*FindElement(const char *pName, int bRecursiveSearch=true);
				XML_ELEMENT		*FindElementEx(char *pName, char *pAttributeMatch, char *pMatch);

		inline	XML_ATTRIBUTE	*CreateAttribute(char *pName, unsigned int nValue)	{ return CreateAttribute(pName, (int)nValue); }
				XML_ATTRIBUTE	*CreateAttribute(char *pName, float fValue);
				XML_ATTRIBUTE	*CreateAttribute(char *pName, const char *pValue);
				XML_ATTRIBUTE	*CreateAttribute(char *pName, int nValue);
				XML_ATTRIBUTE	*CreateAttribute(char *pName, unsigned char *pBuffer, int nSize);
		inline	XML_ATTRIBUTE	*CreateAttribute(char *pName, DYNAMIC_BUFFER *pBuffer)				{ return CreateAttribute(pName, (unsigned char*)pBuffer->GetBuffer(), pBuffer->GetSize()); }
				XML_ELEMENT		*CreateChildElement(char *pName);

				const char*		GetAttribute(const char *pAttrib, const char *pDef);
				int				GetAttribute(const char *pAttrib, int nDef);
				float			GetAttribute(const char *pAttrib, float fDef);
				int				GetAttributeAsChar(const char *pAttrib, char *Destination, char *pDefaultValue);
				int				GetAttributeAsInt(const char *pAttrib, int &Destination, int DefaultValue);
				int				GetAttributeAsData(const char *pAttrib, unsigned char *Destination);
				int				GetAttributeAsData(const char *pAttrib, DYNAMIC_BUFFER &Destination);

	protected:
		XML_ELEMENT			*_pFather;
		char				_AttrName[256];
		QDT_LIST			_Childs;
		QDT_LIST			_Attributes;
		DYNAMIC_BUFFER		_sText;
};

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

#endif