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


#include "XMLParser.h"

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

