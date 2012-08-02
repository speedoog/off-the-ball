//
//	XMLParser Buffer Implements
//
//----------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Std Includes
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// Libraries Includes
#include "XMLParser.h"
#include "DynamicBuffer.h"


//-------------------------------------------------------------------------------
XML_PARSER::XML_PARSER()
:
_Root(0)
{
}

//-------------------------------------------------------------------------------
XML_PARSER::~XML_PARSER()
{
	if(_Root)
		delete _Root;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE::~XML_ATTRIBUTE()
{
	if(_pAttrName)
		free(_pAttrName);
	if(_pAttrValue)
		free(_pAttrValue);

	_pAttrValue = _pAttrName = 0;
}

//-------------------------------------------------------------------------------
int XML_ATTRIBUTE::WriteToFile(FILE *pFile)
{
	fwrite(" ", 1, 1, pFile);
	fwrite(_pAttrName, strlen(_pAttrName), 1, pFile);
	fwrite("=\"", 2, 1, pFile);
	fwrite(_pAttrValue, strlen(_pAttrValue), 1, pFile);
	fwrite("\"", 1, 1, pFile);

	return 0;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE *XML_ELEMENT::FindAttribute(const char *pAttribName)
{
	XML_ATTRIBUTE *pAtt = (XML_ATTRIBUTE*)_lAttributes.GetHead();

	while (pAtt)
	{
		if(!strcmp(pAtt->GetName(), pAttribName))
			return pAtt;
		pAtt = (XML_ATTRIBUTE*)pAtt->GetNext();
	}
	return 0;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::GetAttributeAsData(const char *pAttrib, unsigned char *Destination)
{
	XML_ATTRIBUTE	*pAtt = FindAttribute(pAttrib);
	char			Tmp[3];

	Tmp[2] = 0;
	if(pAtt)
	{
		char	*pAttribData = pAtt->GetValue();

		while(*pAttribData)
		{
			int	IntDest;
			Tmp[0] = pAttribData[0];
			Tmp[1] = pAttribData[1];
			sscanf(Tmp, "%02x", &IntDest);
			*Destination = (unsigned char)IntDest;
			Destination++;
			pAttribData+=2;
		}

		return 0;
	}

	return -1;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::GetAttributeAsData(const char *pAttrib, DYNAMIC_BUFFER &Destination)
{
	XML_ATTRIBUTE	*pAtt = FindAttribute(pAttrib);
	char			Tmp[3];

	Tmp[2] = 0;
	if(pAtt)
	{
		char	*pAttribData = pAtt->GetValue();

		while(*pAttribData)
		{
			int	IntDest;
			Tmp[0] = pAttribData[0];
			Tmp[1] = pAttribData[1];
			sscanf(Tmp, "%02x", &IntDest);
			unsigned char Conv = (unsigned char)IntDest;
			Destination.Append( &Conv, 1 );
			pAttribData+=2;
		}

		return 0;
	}

	return -1;
}

//-------------------------------------------------------------------------------
float XML_ELEMENT::GetAttribute(const char *pAttrib, float fDef)
{
	XML_ATTRIBUTE *pAtt = FindAttribute(pAttrib);

	if(pAtt)
	{
		float	fDest;
		sscanf(pAtt->GetValue(), "%f", &fDest);
		return fDest;
	}

	return fDef;
}

//-------------------------------------------------------------------------------
const char *XML_ELEMENT::GetAttribute(const char *pAttrib, const char *pDef)
{
	XML_ATTRIBUTE *pAtt = FindAttribute(pAttrib);

	if(pAtt)
		return pAtt->GetValue();

	return pDef;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::GetAttribute(const char *pAttrib, int nDef)
{
	int nDest;
	GetAttributeAsInt(pAttrib, nDest, nDef);
	return nDest;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::GetAttributeAsChar(const char *pAttrib, char *Destination, char *pDefaultValue)
{
	XML_ATTRIBUTE *pAtt = FindAttribute(pAttrib);

	if(pAtt)
	{
		strcpy(Destination, pAtt->GetValue());
		return 0;
	}

	strcpy(Destination, pDefaultValue);
	return -1;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::GetAttributeAsInt(const char *pAttrib, int &Destination, int DefaultValue)
{
	XML_ATTRIBUTE *pAtt = FindAttribute(pAttrib);

	if(pAtt)
	{
		Destination = atoi( pAtt->GetValue() );
		return 0;
	}

	Destination = DefaultValue;
	return -1;
}

//-------------------------------------------------------------------------------
int XML_ELEMENT::SaveToFile(FILE *pFile, int IndentLevel)
{
	int				indent;
	XML_ATTRIBUTE	*pAttribs = (XML_ATTRIBUTE*)_lAttributes.GetHead();

	for(indent=0; indent<IndentLevel; indent++)
		fwrite("\t", 1, 1, pFile);
	fwrite("<", 1, 1, pFile);
	fwrite(_AttrName, strlen(_AttrName), 1, pFile);

	while(pAttribs)
	{
		pAttribs->WriteToFile(pFile);
		pAttribs = (XML_ATTRIBUTE*)pAttribs->GetNext();
	}

	if( _lChilds.GetNbElements() || _sText.GetSize()>1)
	{
		fwrite(">", 1, 1, pFile);
		
		if (_sText.GetSize()>1)
		{
			fwrite(_sText.GetBufferAsString(), _sText.GetSize()-1, 1, pFile);
		}

		fwrite("\n", 1, 1, pFile);

		XML_ELEMENT *pChilds = (XML_ELEMENT*)_lChilds.GetHead();
		while(pChilds)
		{
			pChilds->SaveToFile(pFile, IndentLevel+1);
			pChilds = (XML_ELEMENT*)pChilds->GetNext();
		}
		for(indent=0; indent<IndentLevel; indent++)
			fwrite("\t", 1, 1, pFile);
		fwrite("</", 2, 1, pFile);
		fwrite(_AttrName, strlen(_AttrName), 1, pFile);
		fwrite(">\n", 2, 1, pFile);
	}
	else
	{
		fwrite("/>\n", 3, 1, pFile);
	}

	return 0;
}

//-------------------------------------------------------------------------------
XML_PARSER::XMLRC XML_PARSER::SaveToFile(const char *pFileName)
{
	if(!_Root)
		return XP_NO_TREE;

	FILE	*pFile = fopen(pFileName, "wb");

	if(!pFile)
		return XP_CANT_OPEN_FILE;

	XML_ELEMENT	*pRoot = _Root->GetFirstChild();
	while(pRoot)
	{
		pRoot->SaveToFile(pFile, 0);
		pRoot = (XML_ELEMENT*)pRoot->GetNext();
	}

	fclose(pFile);

	return XP_NO_ERROR;
}

//-------------------------------------------------------------------------------
XML_PARSER::XMLRC XML_PARSER::LoadFromFile(const char *pFileName)
{
	FILE			*pFile			= fopen(pFileName, "rb");
	XML_ELEMENT		*TreePos		= 0;
	XML_ATTRIBUTE	*CurrentAttrib	= 0;
	int				FileSize;
	char			*pLoaded;

	if(!pFile)
		return XP_CANT_OPEN_FILE;

	if(_Root)
		delete _Root;
	_Root = new XML_ELEMENT("Root");

	fseek(pFile, 0, SEEK_END);
	FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	
	pLoaded = new char[FileSize];
	fread(pLoaded, FileSize, 1, pFile);
	fclose(pFile);

	char	*pScan			= pLoaded;
	char	*pOSection		= 0;
	char	*pESection		= 0;
	char	*pOSecText		= 0;
	int		bClosingSect	= false;

	TreePos = _Root;

	while(FileSize > 0)
	{
		switch(*pScan) 
		{
			case '\r':
			case '\n':
			case '\t':
				break;

			case '<':
				if(*(pScan+1) == '/')
				{
					FileSize--;
					pScan++;
					bClosingSect = true;
				}
				pOSection = pScan+1;
				pESection = 0;
				break;

			case '>':
				if(pOSection)
				{
					if(bClosingSect)
					{
						TreePos->GetTextBuffer().AppendStringNull("");
						pOSection		= 0;
						TreePos			= TreePos->GetFather();
						bClosingSect	= false;
						break;
					}
					char	SectName[64];
					int		SectSize = (int)(pScan-pOSection);
					memcpy(SectName, pOSection, SectSize);
					SectName[SectSize] = 0;
					XML_ELEMENT	*pNewSect = new XML_ELEMENT(SectName);
					TreePos->AddChild(pNewSect);
					TreePos		= pNewSect;
					pOSection	= 0;
				}
				break;

			case '/':
				if(*(pScan+1) == '>')
				{
					if(pOSection)
					{
						char	SectName[64];
						int		SectSize = (int)(pScan-pOSection);
						memcpy(SectName, pOSection, SectSize);
						SectName[SectSize] = 0;
						XML_ELEMENT	*pNewSect = new XML_ELEMENT(SectName);
						TreePos->AddChild(pNewSect);
						pScan++;
						FileSize--;
						pOSection	= 0;
					}
					else
					{
						TreePos->GetTextBuffer().AppendStringNull("");
						TreePos = TreePos->GetFather();
					}

					pScan++;
					FileSize--;
				}
				break;

			case ' ':
				if(pOSecText)
					break;

				if(pOSection)
				{
					char	SectName[64];
					int		SectSize = (int)(pScan-pOSection);
					memcpy(SectName, pOSection, SectSize);
					SectName[SectSize] = 0;
					XML_ELEMENT	*pNewSect = new XML_ELEMENT(SectName);
					TreePos->AddChild(pNewSect);
					TreePos		= pNewSect;
					pOSection	= 0;

					pESection = pScan+1;
				}
				else
				{
					pESection = pScan+1;
				}
				break;

			case '=':
				if(pOSecText)
					break;
				if(pESection)
				{
					if(CurrentAttrib)
					{
						delete pLoaded;
						return XP_PARSER_ERROR;
					}
					char	AttribName[64];
					int		AttribSize = (int)(pScan-pESection);
					memcpy(AttribName, pESection, AttribSize);
					AttribName[AttribSize] = 0;
					CurrentAttrib = new XML_ATTRIBUTE(AttribName);
					pESection = 0;
				}
				else
				{
					delete pLoaded;
					return XP_PARSER_ERROR;
				}
				break;

			case '"':
				if(!CurrentAttrib)
				{
					delete pLoaded;
					return XP_PARSER_ERROR;
				}

				if(!pOSecText)
					pOSecText = pScan+1;
				else
				{
					char	*pAttribValue;
					int		AttribSize = (int)(pScan-pOSecText);
					pAttribValue = (char*)malloc(AttribSize+1);
					memcpy(pAttribValue, pOSecText, AttribSize);
					pAttribValue[AttribSize] = 0;
					CurrentAttrib->SetValue(pAttribValue);
					TreePos->AddAttribute(CurrentAttrib);
					CurrentAttrib	= 0;
					pOSecText		= 0;
					free(pAttribValue);
				}

				break;

			default:
				if( TreePos && (pOSection==NULL) && (pOSecText==NULL) && (pESection==NULL))
				{
					TreePos->GetTextBuffer().Append(pScan, 1);
				}
				break;
		}
		pScan++;
		FileSize--;
	}

	delete pLoaded;
	return XP_NO_ERROR;
}

//-------------------------------------------------------------------------------
XML_ELEMENT	*XML_ELEMENT::FindElement(const char *pName, int bRecursiveSearch)
{
	XML_ELEMENT	*pElem = (XML_ELEMENT*)_lChilds.GetHead();

	while(pElem)
	{
		if( !strcmp(pElem->GetName(), pName) )
			return pElem;
		if( bRecursiveSearch )
		{
			XML_ELEMENT	*pRecursiveElem;
			pRecursiveElem = pElem->FindElement(pName, bRecursiveSearch);
			if(pRecursiveElem)
				return pRecursiveElem;
		}
		pElem = (XML_ELEMENT*)pElem->GetNext();
	}

	return 0;
}

//-------------------------------------------------------------------------------
XML_ELEMENT	*XML_ELEMENT::FindElementEx(char *pName, char *pAttributeMatch, char *pMatch)
{
	XML_ELEMENT	*pScanChilds = (XML_ELEMENT*)_lChilds.GetHead();
	while(pScanChilds)
	{
		if( !stricmp(pName, pScanChilds->GetName()) )
		{
			char	Temp[1024];
			pScanChilds->GetAttributeAsChar(pAttributeMatch, Temp, "");
			if( !stricmp(Temp, pMatch) )
				return pScanChilds;
		}
		pScanChilds = (XML_ELEMENT*)pScanChilds->GetNext();
	}
	return 0;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, unsigned char *pBuffer, int nSize)
{
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	DYNAMIC_BUFFER	DataBuffer;

	while(nSize)
	{
		DataBuffer.AppendString("%02x", (unsigned int)*pBuffer);
		pBuffer++;
		nSize--;
	}
	DataBuffer.Append("\0", 1);

	pNewAttrib->SetValue(DataBuffer.GetBuffer());
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, const char *pValue)
{
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	pNewAttrib->SetValue(pValue);
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, int nValue)
{
	char	Tmp[32];
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	pNewAttrib->SetValue( itoa(nValue, Tmp, 10) );
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, float fValue)
{
	char	Tmp[32];
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	sprintf(Tmp, "%g", fValue);
	pNewAttrib->SetValue( Tmp );
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

//-------------------------------------------------------------------------------
XML_ELEMENT	*XML_ELEMENT::CreateChildElement(char *pName)
{
	XML_ELEMENT	*pNewElem = new XML_ELEMENT(pName);
	AddChild(pNewElem);
	return pNewElem;
}

//-------------------------------------------------------------------------------
XML_ELEMENT	*XML_PARSER::FindElement(char *pName, int bRecursiveSearch)
{
	if(!_Root)
		return 0;
	return _Root->FindElement(pName, bRecursiveSearch);
}

//-------------------------------------------------------------------------------
XML_ELEMENT	*XML_PARSER::CreateRootElement(char *pName)
{
	if(!_Root)
		_Root = new XML_ELEMENT("Root");
	XML_ELEMENT	*pNewElem = new XML_ELEMENT(pName);
	_Root->AddChild(pNewElem);
	return pNewElem;
}

