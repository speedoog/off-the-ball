//----------------------------------------------------------
//
//
//
//
//	XML_ELEMENT
//
//
//
//
//----------------------------------------------------------

#ifndef __XML_XMLELEMENT_H__
#define __XML_XMLELEMENT_H__

#pragma once

#include "../Base/TQuickList.h"
#include "DynamicBuffer.h"
#include "XmlAttribute.h"

class XML_ELEMENT : public TQuickListElement
{
	public:
		inline					XML_ELEMENT(char *pName)				{ _pFather=0; strcpy_s(_AttrName, 256, pName); }
		inline	void			SetFather(XML_ELEMENT *pFather)			{ _pFather = pFather; }
		inline	XML_ELEMENT		*GetFather(void)						{ return _pFather; }
		inline	void			AddChild(XML_ELEMENT *pChild)			{ _lChilds.InsertQueue(pChild); pChild->SetFather(this); }
		inline	void			AddAttribute(XML_ATTRIBUTE *pAttrib)	{ _lAttributes.InsertQueue(pAttrib); }
				int				SaveToFile(FILE *pFile, int IndentLevel);
		inline	XML_ELEMENT		*GetFirstChild(void)					{ return (XML_ELEMENT*)_lChilds.GetHead(); }

		inline	char			*GetName(void)							{ return _AttrName; }
		inline	char			*GetText(void)							{ return _sText.GetBufferAsString(); }
		inline	DYNAMIC_BUFFER&	GetTextBuffer(void)						{ return _sText; }
		inline	void			SetText(char* pText)					{ _sText.Release(); _sText.AppendStringNull(pText); }

				XML_ATTRIBUTE	*FindAttribute(const char *pAttribName);
				XML_ELEMENT		*FindElement(const char *pName, bool bRecursiveSearch=true);
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
		TQuickList			_lChilds;
		TQuickList			_lAttributes;
		DYNAMIC_BUFFER		_sText;
};

#endif	//__XML_XMLELEMENT_H__
