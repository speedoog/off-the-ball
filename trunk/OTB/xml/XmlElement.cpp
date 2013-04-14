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


#include "XmlElement.h"

#pragma warning( disable : 4996)

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
const char *XML_ELEMENT::GetAttribute(const char *pAttrib, const char *pDef)
{
	XML_ATTRIBUTE *pAtt = FindAttribute(pAttrib);

	if(pAtt)
		return pAtt->GetValue();

	return pDef;
}

// ****************************************************************************************
int XML_ELEMENT::GetAttribute(const char *pAttrib, int nDef)
{
	int nDest;
	GetAttributeAsInt(pAttrib, nDest, nDef);
	return nDest;
}

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
XML_ELEMENT	*XML_ELEMENT::FindElement(const char *pName, bool bRecursiveSearch)
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

// ****************************************************************************************
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

// ****************************************************************************************
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

// ****************************************************************************************
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, const char *pValue)
{
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	pNewAttrib->SetValue(pValue);
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

// ****************************************************************************************
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, int nValue)
{
	char	Tmp[32];
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	pNewAttrib->SetValue( itoa(nValue, Tmp, 10) );
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

// ****************************************************************************************
XML_ATTRIBUTE *XML_ELEMENT::CreateAttribute(char *pName, float fValue)
{
	char	Tmp[32];
	XML_ATTRIBUTE	*pNewAttrib = new XML_ATTRIBUTE(pName);
	sprintf(Tmp, "%g", fValue);
	pNewAttrib->SetValue( Tmp );
	AddAttribute(pNewAttrib);
	return pNewAttrib;
}

// ****************************************************************************************
XML_ELEMENT	*XML_ELEMENT::CreateChildElement(char *pName)
{
	XML_ELEMENT	*pNewElem = new XML_ELEMENT(pName);
	AddChild(pNewElem);
	return pNewElem;
}
