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

#include "XmlAttribute.h"


//-------------------------------------------------------------------------------
XML_ATTRIBUTE::XML_ATTRIBUTE(char* pName)
{
	SetName(pName);
}

//-------------------------------------------------------------------------------
XML_ATTRIBUTE::~XML_ATTRIBUTE()
{
}

//-------------------------------------------------------------------------------
void XML_ATTRIBUTE::SetName(const char* pName)
{
	_sName =pName;
}

//-------------------------------------------------------------------------------
void XML_ATTRIBUTE::SetValue(const char* pValue)
{
	_sValue =pValue;
}

//-------------------------------------------------------------------------------
int XML_ATTRIBUTE::WriteToFile(FILE *pFile)
{
	fwrite(" ", 1, 1, pFile);
	fwrite(GetName(), strlen(GetName()), 1, pFile);
	fwrite("=\"", 2, 1, pFile);
	fwrite(GetValue(), strlen(GetValue()), 1, pFile);
	fwrite("\"", 1, 1, pFile);

	return 0;
}
