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
