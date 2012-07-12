//----------------------------------------------------------
//
//	Dynamic Buffer Implements
//
//
//----------------------------------------------------------


#ifndef __DYNAMIC_BUFFER_H__
#define __DYNAMIC_BUFFER_H__

#pragma once

class DYNAMIC_BUFFER
{
	protected:
		class BUFFER
		{
			protected:
				int		_MaxSize;
				int		_FreeSpace;
				int		_UsedSpace;
				char	*_pBuffer;
				BUFFER	*_pNext;

			public:
								BUFFER();
								~BUFFER();
						void	AllocBuffer(int Size);
						void	FreeBuffer(void);
				inline	void	SetNext(BUFFER *pNext)				{ _pNext = pNext; }
				inline	BUFFER	*GetNext(void)						{ return _pNext; }
				inline	int		GetAvailableSpace(void)				{ return _FreeSpace; }
				inline	int		GetUsedSpace(void)					{ return _UsedSpace; }
				inline	void	SubToUsedSpace(int Space)			{ _UsedSpace -= Space; }
				inline	void	ReleaseSpace(int Space)				{ _UsedSpace -= Space; _FreeSpace += Space; }
				inline	char	*GetBuffer(void)					{ return _pBuffer; }
				inline	char	*GetAppendPointer(void)				{ return &_pBuffer[_UsedSpace]; }
				inline	void	AddUsedSpace(int nSpace)			{ _FreeSpace -= nSpace; _UsedSpace += nSpace; }

						void	FillBufferWith(char *pData, int Offset, int Size);
						int		AppendToBuffer(char *pData, int Size);
		};

	public:
						DYNAMIC_BUFFER(int BlockSize=1024);
		virtual			~DYNAMIC_BUFFER()							{ Release(); }
				int		InvalidateBuffer(void);
				void	Append(DYNAMIC_BUFFER &pBuffer);
				void	Append(char *pBuffer, int Size);
				void	Append(void *pBuffer, int Size)				{ Append((char*)pBuffer, Size); }
				void	AppendString(const char *pString, ...);
				void	AppendStringNull(const char *pString, ...);
				void	MergeAllBlocks(void);
				void	ReleaseSpace(int Space);
				void	Empty(void);
				int		Pop(void *pBuffer, int nSize);
				char	*GetBuffer(bool bNullIfEmpty=true);
				char *	GetBufferAsString(void);
				void	Release(void);
		inline	int		GetSize(void)								{ return _TotalSize; }
				int		Memcmp(void *pToCompare, int nSize);
				int		Memicmp(void *pToCompare, int nSize);
				void	SwapContentWith(DYNAMIC_BUFFER &Swap);
				void	ReleaseAtBegining(int nBytes);
				bool	ReadFile(char *pFileName);
				void	EnsureOneBlockExists(void);
				void	AddUsedSpace(int nSize);

	protected:
		BUFFER			*AllocNewBuffer(void);
		BUFFER			*GetNextFreeBuffer(void);

	protected:
		int			_nbBlocks;
		int			_TotalSize;
		int			_BlockSize;
		BUFFER		*_pFirst;
		BUFFER		*_pLast;

};


#endif //__DYNAMIC_BUFFER_H__
