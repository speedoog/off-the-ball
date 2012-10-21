//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//     ________   _____  _____    __  .__             __________        .__  .__        //
//     \_____  \_/ ____\/ ____\ _/  |_|  |__   ____   \______   \_____  |  | |  |       //
//      /   |   \   __\\   __\  \   __\  |  \_/ __ \   |    |  _/\__  \ |  | |  |       //
//     /    |    \  |   |  |     |  | |   Y  \  ___/   |    |   \ / __ \|  |_|  |__     //
//     \_______  /__|   |__|     |__| |___|  /\___  >  |______  /(____  /____/____/     //
//             \/                          \/     \/          \/      \/                //
//                                                                                      //
//                          .o                                                          //
//                   ¨>   .                                      <¨                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////

#include "TStream.h"

// ********************************************
//	Ctor
// ********************************************
TStream::TStream()
: _pFile (NULL)
{
}

// ********************************************
//	Dtor
// ********************************************
TStream::~TStream()
{
}

// ********************************************
//	OpenFile
// ********************************************
Bool TStream::OpenFile(const TString& sFilename, const TStream::TStreamOpenMode OpenMode)
{
	_OpenMode =OpenMode;
	const char*	pFilename =sFilename;
	static const char* pModes[] = { "rb", "wb", "ab" };

	errno_t err;
	err = fopen_s(&_pFile, pFilename, pModes[OpenMode]);

	if (_pFile==NULL)
		return false;

	return true;
}

// ********************************************
//	OpenFile
// ********************************************
void TStream::CloseFile()
{
	fclose(_pFile);
	_pFile =NULL;
	_sFilename.Clear();
}

// ********************************************
//	Write
// ********************************************
void TStream::Write(const void* pBuffer, const UInt32 nSize)
{
	TAssert(_OpenMode==SM_WRITE || _OpenMode==SM_APPEND);

	fwrite((UInt8*)pBuffer, nSize, 1, _pFile);
}

// ********************************************
//	Read
// ********************************************
void TStream::Read(void* pBuffer, const UInt32 nSize)
{
	TAssert(_OpenMode==SM_READ);

	fread((UInt8*)pBuffer, nSize, 1, _pFile);
}
