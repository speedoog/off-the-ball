//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	CLASS:	QDT_VECTOR
//	This class is in fact nothing else than a growable array.
//	It's here to replace our old wrapper which map to the stl vector.  
//	The stl::vector didn't satisfy us in term of memory control
//
//	05-08-17:	RMA - Created
//*****************************************************************************


//-----------------------------------------------------------------------------
//	Name:		SetDebugAllocation
//	Object:		
//	08-03-25:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>	
void	QDT_VECTOR<T>::SetDebugAllocation(bool b)
{
	QDT_ASSERT(_pArray == NULL);
#ifndef QDT_MASTER
	_bDebugAllocation = b;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetDebugAllocation
//	Object:		
//	08-03-25:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>	
Bool	QDT_VECTOR<T>::GetDebugAllocation() const
{
#ifndef QDT_MASTER
	return _bDebugAllocation;
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR()
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nAllocationSize(8)
#ifndef QDT_MASTER
,_bDebugAllocation(false)
#endif
{
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR(UInt32 nSize, 
						  const T & initializationValue)
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nAllocationSize(8)
#ifndef QDT_MASTER
,_bDebugAllocation(false)
#endif
{
	_nSize = nSize;
	Reserve(_nSize);

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
#if defined (PLATFORM_PC) || defined (PLATFORM_O)
		new (&_pArray[i]) T (initializationValue);
#else
		QDT_NEW_PLACED(&_pArray[i], T)(initializationValue);
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR(const QDT_VECTOR & vector)
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nAllocationSize(vector._nAllocationSize)
#ifndef QDT_MASTER
,_bDebugAllocation(false)
#endif
{
	PushTail(vector);
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR destructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::~QDT_VECTOR()
{
	Clear();

	if(_pArray)
	{
#ifndef QDT_MASTER
		if (_bDebugAllocation)
		{
			MEM::GetInstance()->FreeDebug(_pArray);
		}
		else
#endif
		{
#if defined (PLATFORM_PC)
			MEM::GetInstance()->AlignedFree(_pArray);
#else
			if ((sizeof(T) % 16) == 0)
			{
				MEM::GetInstance()->AlignedFree(_pArray);
			}
			else
			{
				MEM::GetInstance()->Free(_pArray);
			}
#endif
		}

		_pArray = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetSize() const
{
	return (_nSize);
}

//-----------------------------------------------------------------------------
//! @brief		GetMemoryUsed
//! @details		
//! @param		
//! @return		
//! @author		BFA : 2010-11-16
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetMemoryUsed() const
{
	UInt32 nMem =0;
	nMem +=sizeof(QDT_VECTOR<T>);
	nMem +=GetCapacity()*sizeof(T);
	return nMem;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Clear()
{
	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		QDT_DELETE_PLACED(&_pArray[i], T);
	}

	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		ClearMemory
//	Object:		
//	08-01-04:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::ClearMemory()
{
	Clear();
	
	if(_pArray)
	{
#ifndef QDT_MASTER
		if (_bDebugAllocation)
		{
			MEM::GetInstance()->FreeDebug(_pArray);
		}
		else
#endif
		{
#if defined (PLATFORM_PC)
			MEM::GetInstance()->AlignedFree(_pArray);
#else
			if ((sizeof(T) % 16) == 0)
			{
				MEM::GetInstance()->AlignedFree(_pArray);
			}
			else
			{
				MEM::GetInstance()->Free(_pArray);
			}
#endif
		}

		_pArray = NULL;
		_nAllocatedSize = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClearAll
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::ClearAll()
{
	ITERATOR	It = GetHead();
	ITERATOR	ItEnd = GetTail();
	for (; It != ItEnd; ++It)
	{
		delete(*It);
	}

	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		_Grow
//	Object:		
//	11-08-12:	GLE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::_Grow()
{
	// Don't use float here, it will generate 2 LHS for conversion 
	/*
	if (_nSize)
	{
		// Patch to avoid mass reallocate of QDT_VECTOR (4 elements by 4) => we reallocate 20% of the precedent allocated size
		UInt32 nNextAlloc = UInt32(0.2f * _nAllocatedSize); 
		if (nNextAlloc == 0) nNextAlloc = _nAllocationSize;
		Reserve(_nAllocatedSize + nNextAlloc);			
	}
	else
	{
		Reserve(_nAllocatedSize + _nAllocationSize);
	}*/

	// We grow faster, by 50% until we have 64 elements then by 19%
	// So the grow is like  : 0,8,12,18,27,40,60,90,106,125,148,175,207,245,290,344,408,484,
	// Instead of (with 20%): 0,8,9,10,12,14,16,19,22,26,31,37,44,52,62,74,88,105,126,151,181,217,260,312,374,448,
	UInt32 nNextAlloc = _nAllocatedSize>64 ? (_nAllocatedSize * 3) / 16 : _nAllocatedSize / 2;//(x*3/16) env 19%
	if (nNextAlloc == 0) nNextAlloc = _nAllocationSize;
	_Grow(_nAllocatedSize + nNextAlloc); 
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const	T& t)
{
	if (_nSize == _nAllocatedSize)
	{
		_Grow();
	}

#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
		new (&_pArray[_nSize++]) T(t);
#else
		QDT_NEW_PLACED(&_pArray[_nSize++], T)(t);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const QDT_VECTOR& v)
{
	unsigned int nOldSize = _nSize;
	unsigned int nSumSize = _nSize + v._nSize;

	if (_nSize + v._nSize > _nAllocatedSize)
	{
		Reserve(_nSize + v._nSize);
	}

	for (unsigned int i = nOldSize ; i < nSumSize ; ++i)
	{
#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
		new (&_pArray[i]) T (v._pArray[i-nOldSize]);
#else
		QDT_NEW_PLACED(&_pArray[i], T)(v._pArray[i-nOldSize]);
#endif
	}

	_nSize += v._nSize;
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-22:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const T & t, UInt32 nTimes)
{
	Reserve(_nSize + nTimes);

	UInt32 nCount =  _nSize + nTimes;
	for (UInt32 i = _nSize ; i < nCount ; ++i)
	{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
		new (&_pArray[i]) T (t);
#else
		QDT_NEW_PLACED(&_pArray[i], T)(t);
#endif
	}
	
	_nSize += nTimes;
}

//-----------------------------------------------------------------------------
//! @brief		Fill
//! @details		
//! @param		
//! @return		
//! @author		JID : 2011-02-01
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::Fill(const T &	Elt)
{
	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		_pArray[i] = Elt;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PopTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PopTail()
{
	QDT_ASSERT(GetSize()!=0);
	QDT_DELETE_PLACED(&_pArray[_nSize-1], T);
	_nSize--;
}

//-----------------------------------------------------------------------------
//	Name:		IsExist
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::IsExist(const T& t) const
{
	return (Find(GetHead(), t) != GetTail());
}

//-----------------------------------------------------------------------------
//	Name:		Reserve
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		_Grow(nReserveSize);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		_Grow
//	Object:		
//	11-08-12:	GLE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::_Grow(UInt32 nGrowSize)
{
	QDT_ASSERT(nGrowSize > _nAllocatedSize);
	T * pTempArray;

#ifndef QDT_MASTER
	if (_bDebugAllocation)
	{
		pTempArray = (T*)MEM::GetInstance()->AllocateDebug(16, nGrowSize * sizeof(T));			
	}
	else
#endif
	{
#if defined(PLATFORM_PC)
		pTempArray = (T*)MEM::GetInstance()->AllocateAlign(16, nGrowSize * sizeof(T));
#else
		if ((sizeof(T) % 16) == 0)
		{
			pTempArray = (T*)MEM::GetInstance()->AllocateAlign(16, nGrowSize * sizeof(T));
		}
		else
		{
			pTempArray = (T*)MEM::GetInstance()->Allocate(nGrowSize * sizeof(T));
		}
#endif
	}
	
	if (_pArray)
	{
		for (unsigned int i = 0 ; i < _nSize ; ++i)
		{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
			new (&pTempArray[i]) T(_pArray[i]);
#else	
			QDT_NEW_PLACED(&pTempArray[i], T)(_pArray[i]);
#endif
	
			QDT_DELETE_PLACED(&_pArray[i], T);
		}
			
#ifndef QDT_MASTER
		if (_bDebugAllocation)
		{
			MEM::GetInstance()->FreeDebug(_pArray);
		}
		else
#endif
		{
#if defined (PLATFORM_PC)
			MEM::GetInstance()->AlignedFree(_pArray);
#else
			if ((sizeof(T) % 16) == 0)
			{
				MEM::GetInstance()->AlignedFree(_pArray);
			}
			else
			{
				MEM::GetInstance()->Free(_pArray);
			}
#endif
		}
	}

	_pArray = pTempArray;
	_nAllocatedSize = nGrowSize;
}

//-----------------------------------------------------------------------------
//	Name:		CreateEntries
//	Object:		
//	09-06-09:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::CreateEntries(UInt32	nNbrEntries)
{
	UInt32 nCount = _nSize + nNbrEntries;
	Reserve(nCount);

	for (UInt32 i = 0 ; i < nCount ; ++i)
	{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
		new (&_pArray[i]) T;
#else
		QDT_NEW_PLACED(&_pArray[i], T);
#endif
	}
	_nSize = nCount;
}

//-----------------------------------------------------------------------------
//	Name:		CreateEntriesWithoutConstruct
//	Object:		
//	09-06-09:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::CreateEntriesWithoutConstruct(UInt32	nNbrEntries)
{
	UInt32 nCount = _nSize + nNbrEntries;
	Reserve(nCount);
	
	_nSize = nCount;
}

//-----------------------------------------------------------------------------
//	Name:		Resize
//	Object:	Use resize on simple object, not tested yet.
//					Tests must be run on PS3, placement new problem with templates + 
//					QDT_NEW_PLACED (Visual C++ 7.0 )
//	01-02-16:	GRI - Created
//  07-11-20:	FBO: This .. is just so stupid ... a vector should never use ctors
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Resize(UInt32 uNewSize)
{
	UInt32 uActualSize = GetSize();
	if ( uNewSize < uActualSize ) 
	{
		for (UInt32 u =uNewSize; u < uActualSize; ++u)
		{
			T* ptr = &(_pArray[u]);
			QDT_DELETE_PLACED(ptr,T);
		}
	}
	else
	{
		Reserve(uNewSize);
		for (UInt32 u = (uActualSize); u < uNewSize; u++)
		{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
			new (&_pArray[u]) T;	
#else
			QDT_NEW_PLACED(&_pArray[u], T);
#endif
		}
  }
	_nSize = uNewSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetCapacity
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetCapacity() const
{
	return (_nAllocatedSize);
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::IsEmpty() const
{
	return (_nSize == 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
T & QDT_VECTOR<T>::operator[](UInt32 nElementIndex)
{
	QDT_ASSERT(nElementIndex < _nSize);
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
const T & QDT_VECTOR<T>::operator[](UInt32 nElementIndex) const
{
	QDT_ASSERT(nElementIndex < _nSize);
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator+=(const T& t)
{
	PushTail(t);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator+=(const QDT_VECTOR<T>& v)
{
	PushTail(v);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator=(const QDT_VECTOR<T>& v)
{
	Clear();
	PushTail(v);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::operator==(const QDT_VECTOR& v) const
{
	if (_nSize != v._nSize)
	{
		return (false);
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		if (_pArray[i] != v._pArray[i])
		{
			return (false);
		}
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::operator!=(const QDT_VECTOR& v) const
{
	if (_nSize != v._nSize)
	{
		return (true);
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		if (_pArray[i] != v._pArray[i])
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		At
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
T & QDT_VECTOR<T>::At(UInt32 nElementIndex)
{
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		At
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
const T & QDT_VECTOR<T>::At(UInt32 nElementIndex) const
{
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIndex
//	Object:		Note GRI : Attention , on n'appelle pas le destructor ???
//				Note RMA : Fixed
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::RemoveIndex(UInt32 i)
{
	QDT_ASSERT(i < GetSize());
#if defined PLATFORM_PC
	if (GetSize() > 0)
#endif
	{
		for (UInt32 index = i ; index < GetSize() - 1 ; ++index)
		{
			_pArray[index] = _pArray[index+1];
		}

		QDT_DELETE_PLACED(&_pArray[GetSize() - 1], T);
		--_nSize;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveRange
//	Object:		
//	08-02-04:	CAU - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::RemoveRange(UInt32	i,
								UInt32	nCount)
{
	QDT_ASSERT(i < GetSize() &&
			   i <= GetSize() - nCount);

	UInt32 index;
	for (index = i; index < GetSize() - nCount; ++index)
	{
		_pArray[index] = _pArray[index + nCount];
	}

	for (index = GetSize() - nCount ; index < GetSize(); ++index)
	{
		QDT_DELETE_PLACED(&_pArray[index], T);
	}

	_nSize -= nCount;
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIndexFast ( fast but Sort Not Stable ) 
//	Object:	Complexity of one but be carreful than the inner order change.
//			!! change vector element order !!	
//			Note that the element that replace the current one have a higher index
//			of the current be erase. So you can do  : 
//
//			/* Int32 nSize = _vector.GetSize();*/ do NOT precompute SIZE as it changed
//			for(index = 0;index < _vector.GetSize();)
//			{
//				if ( _vector[index] == DELETE_ME )
//				{
//					_vector.RemoveIndexFast(index);
//					/* index++*/  DON'T INCREMENT
//				}
//				else
//				{
//					index++;	// do increment !! 
//				}
//			}
//					
//				
//	06-01-31:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::RemoveIndexFast(UInt32 index)
{
	QDT_ASSERT(index < GetSize());

	T* ptr =&_pArray[index];
	QDT_DELETE_PLACED(ptr, T);

	T* ptrLast =&_pArray[_nSize-1];
	QDT_MEMMOVE(ptr, ptrLast, sizeof(T));				// move last element in place of [i] removed

	--_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32	QDT_VECTOR<T>::Remove(const T&	t,
							  bool bRemoveAll)
{
	UInt32 nbr = 0;
	ITERATOR	Cur = GetHead();

	while (Cur != GetTail())
	{
		if ((*Cur) == t)
		{
			Cur = Remove(Cur);
			++nbr;
			if ((bRemoveAll == false) || (Cur == GetTail()))
			{
				break;				
			}
		}
		else
		{
			++Cur;
		}
	}

	return (nbr);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetIndex(const ITERATOR& I) const
{
	return (I.GetIndex());
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt16 QDT_VECTOR<T>::GetAllocationSize() const
{
	QDT_ASSERT(_nAllocationSize>1);
	return (_nAllocationSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::SetAllocationSize(UInt16 n)
{
	_nAllocationSize = n;
}

//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:		Does not work with a vector of pointers !	
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Sort()
{
	Int32	nSize = GetSize();

	if	(nSize == 2)
	{
		if	(_pArray[1] < _pArray[0])
		{
			Swap(0, 1);
		}
	}
	else
	{
		if	(nSize > 2)
		{
			QuickSort(0, nSize - 1);
		}
	}
}

//-----------------------------------------------------------------------------
//! @brief		IsSorted
//! @details	
//! @author		JID : 2011-09-15
//-----------------------------------------------------------------------------
template <class T>
Bool	QDT_VECTOR<T>::IsSorted() const
{
	for (UInt32 i = 1; i < _nSize; ++i)
	{
		if (_pArray[i] < _pArray[i-1])
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		QuickSort
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::QuickSort(Int32	Left,
							  Int32	Right)
{
	if	(Right > Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = Partition(Left, Right, Pivot);
		QuickSort(Left, NewPivot - 1);
		QuickSort(NewPivot + 1, Right);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Partition
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
Int32 QDT_VECTOR<T>::Partition(Int32	Left,
							   Int32	Right,
							   Int32	Pivot)
{
	T	PivotValue = _pArray[Pivot];
	Swap(Pivot, Right);					// Move pivot to end
	Int32	i, StoreIndex = Left;
	for	(i = Left ; i < Right ; ++i)
	{
		if	(_pArray[i] < PivotValue)
		{
			Swap(StoreIndex, i);
			StoreIndex++;
		}
	}
	Swap(Right, StoreIndex);			// Move pivot to its final place
	return	(StoreIndex);
}


//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:	Use it when you got a vector of pointers.	
//	06-04-20:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::SortPointers()
{
	Int32	nSize = GetSize();
	if	(nSize == 2)
	{
		if	( *(_pArray[1]) < *(_pArray[0]))
		{
			Swap(0, 1);
		}
	}
	else
	{
		if	(nSize > 2)
		{
			QuickSortPointers(0, nSize - 1);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		QuickSortPointers
//	Object:		
//	06-01-26:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::QuickSortPointers(Int32	Left,
									  Int32	Right)
{
	if	(Right > Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = PartitionPointers(Left, Right, Pivot);
		QuickSortPointers(Left, NewPivot - 1);
		QuickSortPointers(NewPivot + 1, Right);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Partition
//	Object:		
//	06-01-26:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
Int32 QDT_VECTOR<T>::PartitionPointers(Int32	Left,
									   Int32	Right,
									   Int32	Pivot)
{
	T	PivotValue = _pArray[Pivot];
	Swap(Pivot, Right);					// Move pivot to end
	Int32	i, StoreIndex = Left;
	for	(i = Left ; i < Right ; ++i)
	{
		if	( (*_pArray[i]) < (*PivotValue) )
		{
			Swap(StoreIndex, i);
			StoreIndex++;
		}
	}
	Swap(Right, StoreIndex);			// Move pivot to its final place
	return	(StoreIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SortPointers
//	Object:		
//	06-09-29:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::SortPointers(COMPARISON_CALLBACK	ComparisonCallback)
{
	Int32	nSize = GetSize();
	if	(nSize == 2)
	{
		if	((*ComparisonCallback)(_pArray[1], _pArray[0]))
		{
			Swap(0, 1);
		}
	}
	else
	{
		if	(nSize > 2)
		{
			QuickSortPointers(0, nSize - 1, ComparisonCallback);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		QuickSortPointers
//	Object:		
//	06-09-29:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::QuickSortPointers(Int32					Left,
										 Int32					Right,
										 COMPARISON_CALLBACK	ComparisonCallback)
{
	if	(Right > Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = PartitionPointers(Left, Right, Pivot, ComparisonCallback);
		QuickSortPointers(Left, NewPivot - 1, ComparisonCallback);
		QuickSortPointers(NewPivot + 1, Right, ComparisonCallback);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PartitionPointers
//	Object:		
//	06-09-29:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
Int32	QDT_VECTOR<T>::PartitionPointers(Int32					Left,
										 Int32					Right,
										 Int32					Pivot,
										 COMPARISON_CALLBACK	ComparisonCallback)
{
	T	PivotValue = _pArray[Pivot];
	Swap(Pivot, Right);					// Move pivot to end
	Int32	i, StoreIndex = Left;
	for	(i = Left ; i < Right ; ++i)
	{
		if	((*ComparisonCallback)(_pArray[i], PivotValue))
		{
			Swap(StoreIndex, i);
			StoreIndex++;
		}
	}
	Swap(Right, StoreIndex);			// Move pivot to its final place
	return	(StoreIndex);
}

//-----------------------------------------------------------------------------
//	Name:		Swap
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Swap(Int32	X,
						 Int32	Y)
{
	T	Temp = _pArray[X];
	_pArray[X] = _pArray[Y];
	_pArray[Y] = Temp;
}

//-----------------------------------------------------------------------------
//	Name:		Swap
//	Object:		swap attributes between 2 vectors (avoid unuseful and expensive copy)
//	06-02-08:	SBE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::Swap(QDT_VECTOR<T> &	V)
{
	T *		pArray			= _pArray;
	UInt32	nSize			= _nSize;
	UInt32	nAllocatedSize	= _nAllocatedSize;
	UInt16	nAllocationSize = _nAllocationSize;

	_pArray				= V._pArray;
	_nSize				= V._nSize;
	_nAllocatedSize		= V._nAllocatedSize;
	_nAllocationSize	= V._nAllocationSize;

	V._pArray			= pArray;
	V._nSize			= nSize;
	V._nAllocatedSize	= nAllocatedSize;
	V._nAllocationSize	= nAllocationSize;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
