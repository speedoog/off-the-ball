//----------------------------------------------------------
//
//
//
//
//	DYNAMIC_BUFFER
//
//
//
//
//----------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Std Includes
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////
// Libraries Includes
#include "DynamicBuffer.h"

#pragma warning( disable : 4996)

//------------------------------------------------------------------------
DYNAMIC_BUFFER::BUFFER::BUFFER()
:
_pBuffer(0),
_pNext(0),
_UsedSpace(0),
_MaxSize(0)
{
}

//------------------------------------------------------------------------
DYNAMIC_BUFFER::BUFFER::~BUFFER()
{
	FreeBuffer();
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::BUFFER::AllocBuffer(int Size)
{
	FreeBuffer();
	_pBuffer	= (char*)malloc(Size);
	_MaxSize	= Size;
	_FreeSpace	= Size;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::BUFFER::FreeBuffer(void)
{
	if(_pBuffer)
		free(_pBuffer);
	_pBuffer	= 0;
	_UsedSpace	= 0;
}

//------------------------------------------------------------------------
int DYNAMIC_BUFFER::BUFFER::AppendToBuffer(char *pData, int Size)
{
	if( Size > _FreeSpace )
		Size = _FreeSpace;

	memcpy(&_pBuffer[_UsedSpace], pData, Size);
	_FreeSpace -= Size;
	_UsedSpace += Size;

	return Size;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::BUFFER::FillBufferWith(char *pData, int Offset, int Size)
{
	memcpy(&_pBuffer[Offset], pData, Size);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
DYNAMIC_BUFFER::DYNAMIC_BUFFER(int BlockSize/* =1024 */)
:
_pFirst(0),
_pLast(0),
_TotalSize(0),
_nbBlocks(0)
{
	_BlockSize = BlockSize;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::EnsureOneBlockExists(void)
{
	GetNextFreeBuffer();
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::SwapContentWith(DYNAMIC_BUFFER &Swap)
{
	BUFFER	*TmpBuffer;
	int		TmpInt;

	TmpInt = _nbBlocks;		_nbBlocks = Swap._nbBlocks;		Swap._nbBlocks = TmpInt;
	TmpInt = _TotalSize;	_TotalSize = Swap._TotalSize;	Swap._TotalSize = TmpInt;
	TmpInt = _BlockSize;	_BlockSize = Swap._BlockSize;	Swap._BlockSize = TmpInt;

	TmpBuffer = _pFirst;	_pFirst = Swap._pFirst;	Swap._pFirst = TmpBuffer;
	TmpBuffer = _pLast;		_pLast = Swap._pLast;	Swap._pLast = TmpBuffer;
}

//------------------------------------------------------------------------
bool DYNAMIC_BUFFER::ReadFile(char *pFileName)
{
	FILE	*pFile = fopen(pFileName, "rb");

	if(!pFile)
		return false;

	int		AddedSize;
	BUFFER	*pBuff;

	do
	{
		pBuff		=	GetNextFreeBuffer();
		AddedSize	=	(int)fread( pBuff->GetAppendPointer(), 1, pBuff->GetAvailableSpace(), pFile );
		if( !AddedSize )
			break;
		pBuff->AddUsedSpace(AddedSize);
		_TotalSize += AddedSize;
	} while (true);

	fclose(pFile);
	return true;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::AddUsedSpace(int nSize)
{
	BUFFER	*pBuff = GetNextFreeBuffer();

	if( nSize > pBuff->GetAvailableSpace() )
		return;

	pBuff->AddUsedSpace(nSize);
	_TotalSize += nSize;
}

//------------------------------------------------------------------------
int DYNAMIC_BUFFER::Memcmp(void *pToCompare, int nSize)
{
	if(nSize > _TotalSize)
		return -1;
	return memcmp(GetBuffer(), pToCompare, nSize);
}

//------------------------------------------------------------------------
int DYNAMIC_BUFFER::Memicmp(void *pToCompare, int nSize)
{
	if(nSize > _TotalSize)
		return -1;
	return memicmp(GetBuffer(), pToCompare, nSize);
}

//------------------------------------------------------------------------
int DYNAMIC_BUFFER::InvalidateBuffer(void)
{
	int	TotalSizeBckp = _TotalSize;
	_pFirst		= 0;
	_pLast		= 0;
	_TotalSize	= 0;
	_nbBlocks	= 0;
	return TotalSizeBckp;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::Empty(void)
{
	if( _pFirst )
	{
		BUFFER *pScan = _pFirst->GetNext();

		while (pScan) 
		{
			BUFFER *pNext = pScan->GetNext();
			delete pScan;
			pScan = pNext;
		}

		_pFirst->ReleaseSpace( _pFirst->GetUsedSpace() );

		_pLast		= _pFirst;
		_TotalSize	= 0;
		_nbBlocks	= 1;
	}
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::Release(void)
{
	BUFFER *pScan = _pFirst;

	while (pScan) 
	{
		BUFFER *pNext = pScan->GetNext();
		delete pScan;
		pScan = pNext;
	}

	_pFirst		= 0;
	_pLast		= 0;
	_TotalSize	= 0;
	_nbBlocks	= 0;
}

//------------------------------------------------------------------------
DYNAMIC_BUFFER::BUFFER *DYNAMIC_BUFFER::GetNextFreeBuffer(void)
{
	if(_pLast)
		if( _pLast->GetAvailableSpace() )
			return _pLast;

	_nbBlocks++;
	return AllocNewBuffer();
}

//------------------------------------------------------------------------
char *DYNAMIC_BUFFER::GetBuffer(bool bNullIfEmpty)
{
	if(!_nbBlocks)
		return 0;

	if(_nbBlocks > 1)
		MergeAllBlocks();

	if( (!_pFirst->GetUsedSpace()) && bNullIfEmpty )
		return 0;

	return _pFirst->GetBuffer();
}

//------------------------------------------------------------------------
char *DYNAMIC_BUFFER::GetBufferAsString(void)
{
	char	*pReturn = GetBuffer(true);
	if( pReturn == NULL )
	{
		return "";
	}
	return pReturn;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::MergeAllBlocks(void)
{
	if(_nbBlocks > 1)
	{
		BUFFER	*pMerged	= new BUFFER;
		BUFFER	*pCurrent	= _pFirst;
		
		pMerged->AllocBuffer(_TotalSize);

		while(pCurrent)
		{
			BUFFER	*pNext	= pCurrent->GetNext();
			pMerged->AppendToBuffer( pCurrent->GetBuffer(), pCurrent->GetUsedSpace() );
			delete pCurrent;
			pCurrent = pNext;
		}

		_pFirst		= pMerged;
		_pLast		= pMerged;
		_nbBlocks	= 1;
	}
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::ReleaseAtBegining(int nBytes)
{
	char *pSource	= GetBuffer() + nBytes;
	char *pDest		= GetBuffer();
	int	nSize		= GetSize() - nBytes;

	/*
	while(nSize)
	{
		*pDest++ = *pSource++;
		nSize--;
	}*/
	while(nSize)
	{
		if(nBytes < nSize)
		{
			memcpy(pDest, pSource, nBytes);
			pDest+=nBytes;
			pSource+=nBytes;
			nSize-=nBytes;
		}
		else
		{
			memcpy(pDest, pSource, nSize);
			break;
		}
	}

	_pFirst->ReleaseSpace(nBytes);
	_TotalSize -= nBytes;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::Append(DYNAMIC_BUFFER &pBuffer)
{
	// TODO: Consider doing a Getbuffer less version ... would be faster & preserve buffers structure
	Append( pBuffer.GetBuffer(), pBuffer.GetSize() );
}

//------------------------------------------------------------------------
int DYNAMIC_BUFFER::Pop(void *pBuffer, int nSize)
{
	if(nSize > GetSize())
		return -1;

	char *pSource	= GetBuffer() + GetSize() - nSize;
	memcpy(pBuffer, pSource, nSize);
	ReleaseSpace(nSize);
	return 0;
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::ReleaseSpace(int Space)
{
	if(GetSize())
	{
		MergeAllBlocks();
		_pFirst->ReleaseSpace(Space);
		_TotalSize-=Space;
	}
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::Append(char *pBuffer, int Size)
{
	int		AddedSize;
	BUFFER	*pBuff;

	_TotalSize += Size;
	do
	{
		pBuff		=  GetNextFreeBuffer();
		AddedSize	=  pBuff->AppendToBuffer(pBuffer, Size);
		pBuffer		=  &pBuffer[AddedSize];
		Size		-= AddedSize;
	} while (Size);
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::AppendString(const char *pString, ...)
{
	if( pString )
	{
		char	Tmp[10240];

		va_list lst;
		va_start(lst, pString);
		vsprintf(Tmp, pString, lst);
		va_end(lst);

		Append(Tmp, (int)strlen(Tmp));
	}
}

//------------------------------------------------------------------------
void DYNAMIC_BUFFER::AppendStringNull(const char *pString, ...)
{
	if( pString )
	{
		char	Tmp[10240];

		va_list lst;
		va_start(lst, pString);
		vsprintf(Tmp, pString, lst);
		va_end(lst);

		Append(Tmp, (int)strlen(Tmp));
		Append("\0", 1);
	}
}

//------------------------------------------------------------------------
DYNAMIC_BUFFER::BUFFER *DYNAMIC_BUFFER::AllocNewBuffer(void)
{
	BUFFER *pReturn = new BUFFER;

	pReturn->AllocBuffer(_BlockSize);

	if(_pLast)
		_pLast->SetNext( pReturn );
	else
		_pFirst = pReturn;

	_pLast = pReturn;

	return pReturn;
}