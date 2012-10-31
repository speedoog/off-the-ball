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


// ********************************************
//	Ctor
// ********************************************
template <class TType>
TVector<TType>::TVector()
: _pArray		(NULL)
, _nSize		(0)
, _nAllocSize	(0)
{
}

// ********************************************
//	Ctor
// ********************************************
template <class TType>
TVector<TType>::TVector(UInt32 nSize, const TType& Value)
: _pArray		(NULL)
, _nSize		(0)
, _nAllocSize	(0)
{
	_nSize = nSize;
	Reserve(_nSize);

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		TNewPlaced(&_pArray[i], TType)(Value);
	}
}

// ********************************************
//	Ctor
// ********************************************
template <class TType>
TVector<TType>::TVector(const TVector& vector)
: _pArray		(NULL)
, _nSize		(0)
, _nAllocSize	(0)
{
	PushTail(vector);
}

// ********************************************
//	Dtor
// ********************************************
template <class TType>
TVector<TType>::~TVector()
{
	ClearMemory();
}

// ********************************************
//	GetMemoryUsed
// ********************************************
template <class TType>
UInt32 TVector<TType>::GetMemoryUsed() const
{
	UInt32 nMem =sizeof(TVector<TType>);
	nMem +=GetCapacity()*sizeof(TType);
	return nMem;
}

// ********************************************
//	Clear
// ********************************************
template <class TType>
void TVector<TType>::Clear()
{
	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		TDeletePlaced(&_pArray[i], TType);
	}

	_nSize = 0;
}

// ********************************************
//	ClearMemory
// ********************************************
template <class TType>
void TVector<TType>::ClearMemory()
{
	Clear();
	
	if (_pArray)
	{
		free(_pArray);
		_pArray = NULL;
		_nAllocSize = 0;
	}
}

// ********************************************
//	ClearAll
// ********************************************
template <class TType>
void TVector<TType>::ClearAll()
{
	Iterator It		=GetHead();
	Iterator ItEnd  =GetTail();
	for (; It != ItEnd; ++It)
	{
		delete(*It);
	}

	Clear();
}

// ********************************************
//	Grow
// ********************************************
template <class TType>
void TVector<TType>::Grow()
{
	// We grow faster, by 50% until we have 64 elements then by 19%
	// So the grow is like  : 0,8,12,18,27,40,60,90,106,125,148,175,207,245,290,344,408,484,
	// Instead of (with 20%): 0,8,9,10,12,14,16,19,22,26,31,37,44,52,62,74,88,105,126,151,181,217,260,312,374,448,
	UInt32 nNextAlloc = _nAllocSize>64 ? (_nAllocSize * 3) / 16 : _nAllocSize / 2;//(x*3/16) env 19%
	if (nNextAlloc == 0)
	{
		nNextAlloc = 8;		// first alloc
	}
	Grow(_nAllocSize + nNextAlloc); 
}

// ********************************************
//	Grow
// ********************************************
template <class TType>
void TVector<TType>::Grow(const UInt32 nGrowSize)
{
	TAssert(nGrowSize>_nAllocSize);

	TType* pTempArray = (TType*)malloc( nGrowSize*sizeof(TType) );
	if (_pArray)
	{
		for (UInt32 i = 0 ; i < _nSize ; ++i)
		{
			TNewPlaced(&pTempArray[i], TType)(_pArray[i]);

			TDeletePlaced(&_pArray[i], TType);
		}

		free(_pArray);
	}

	_pArray = pTempArray;
	_nAllocSize = nGrowSize;
}

// ********************************************
//	PushOnce
// ********************************************
template <class TType>
typename TVector<TType>::Iterator TVector<TType>::PushOnce(const TType& Elt)
{
	Iterator Cur = Find(Elt);

	if (Cur == GetTail())
	{
		PushTail(Elt);
		Cur = GetLast();
	}

	return Cur;
}

// ********************************************
//	PushTail
// ********************************************
template <class TType>
void TVector<TType>::PushTail(const	TType& Elt)
{
	if (_nSize == _nAllocSize)
	{
		Grow();
	}

	TNewPlaced(&_pArray[_nSize++], TType)(Elt);
}

// ********************************************
//	PushTail
// ********************************************
template <class TType>
void TVector<TType>::PushTail(const TVector& v)
{
	UInt32 nOldSize = _nSize;
	UInt32 nSumSize = _nSize + v._nSize;

	if (_nSize + v._nSize > _nAllocSize)
	{
		Reserve(_nSize + v._nSize);
	}

	for (UInt32 i = nOldSize ; i < nSumSize ; ++i)
	{
		TNewPlaced(&_pArray[i], TType)(v._pArray[i-nOldSize]);
	}

	_nSize += v._nSize;
}

// ********************************************
//	PushTail
// ********************************************
template <class TType>
void TVector<TType>::PushTail(const TType& Elt, UInt32 nTimes)
{
	Reserve(_nSize + nTimes);

	UInt32 nCount =  _nSize + nTimes;
	for (UInt32 i = _nSize ; i < nCount ; ++i)
	{
		TNewPlaced(&_pArray[i], TType)(Elt);
	}
	
	_nSize += nTimes;
}

// ********************************************
//	Fill
// ********************************************
template <class TType>
void TVector<TType>::Fill(const TType& Elt)
{
	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		_pArray[i] = Elt;
	}
}

// ********************************************
//	PopTail
// ********************************************
template <class TType>
void TVector<TType>::PopTail()
{
	TAssert(GetSize()!=0);
	TDeletePlaced(&_pArray[_nSize-1], TType);
	_nSize--;
}

// ********************************************
//	Reserve
// ********************************************
template <class TType>
Bool TVector<TType>::Reserve(const UInt32 nReserveSize)
{
	if (nReserveSize>_nAllocSize)
	{
		Grow(nReserveSize);
		return true;
	}
	return false;
}

// ********************************************
//	Resize
// ********************************************
template <class TType>
void TVector<TType>::Resize(const UInt32 uNewSize)
{
	UInt32 uActualSize = GetSize();
	if ( uNewSize < uActualSize ) 
	{
		for (UInt32 u =uNewSize; u < uActualSize; ++u)
		{
			TType* ptr = &(_pArray[u]);
			TDeletePlaced(ptr,TType);
		}
	}
	else
	{
		Reserve(uNewSize);
		for (UInt32 u = (uActualSize); u < uNewSize; u++)
		{
			TNewPlaced(&_pArray[u], TType);
		}
	}
	_nSize = uNewSize;
}

// ********************************************
//	operator []
// ********************************************
template <class TType>
TType& TVector<TType>::operator [] (UInt32 nElementIndex)
{
	TAssert(nElementIndex<_nSize);
	return _pArray[nElementIndex];
}

// ********************************************
//	operator []
// ********************************************
template <class TType>
const TType& TVector<TType>::operator [] (UInt32 nElementIndex) const
{
	TAssert(nElementIndex<_nSize);
	return _pArray[nElementIndex];
}

// ********************************************
//	operator +=
// ********************************************
template <class TType>
TVector<TType>& TVector<TType>::operator += (const TType& Elt)
{
	PushTail(Elt);
	return *this;
}

// ********************************************
//	operator +=
// ********************************************
template <class TType>
TVector<TType>& TVector<TType>::operator += (const TVector<TType>& v)
{
	PushTail(v);
	return *this;
}

// ********************************************
//	operator =
// ********************************************
template <class TType>
TVector<TType>& TVector<TType>::operator = (const TVector<TType>& v)
{
	Clear();
	PushTail(v);
	return *this;
}

// ********************************************
//	operator ==
// ********************************************
template <class TType>
Bool TVector<TType>::operator == (const TVector& v) const
{
	if (_nSize != v._nSize)
	{
		return false;
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		if (_pArray[i] != v._pArray[i])
		{
			return false;
		}
	}

	return true;
}

// ********************************************
//	operator !=
// ********************************************
template <class TType>
Bool TVector<TType>::operator!=(const TVector& v) const
{
	return !((*this)==v);
}

// ********************************************
//	At
// ********************************************
template <class TType>
TType& TVector<TType>::At(UInt32 nElementIndex)
{
	return _pArray[nElementIndex];
}

// ********************************************
//	At
// ********************************************
template <class TType>
const TType& TVector<TType>::At(UInt32 nElementIndex) const
{
	return _pArray[nElementIndex];
}

// ********************************************
//	RemoveIndex
// ********************************************
template <class TType>
void TVector<TType>::RemoveIndex(UInt32 i)
{
	TAssert(i<GetSize());

	for (UInt32 index = i ; index < GetSize() - 1 ; ++index)
	{
		_pArray[index] = _pArray[index+1];
	}

	TDeletePlaced(&_pArray[GetSize() - 1], TType);
	--_nSize;
}

// ********************************************
//	RemoveIndexFast
// ********************************************
template <class TType>
void TVector<TType>::RemoveIndexFast(UInt32 index)
{
	TAssert(index < GetSize());

	TType* ptr =&_pArray[index];
	TDeletePlaced(ptr, TType);

	TType* ptrLast =&_pArray[_nSize-1];
	TMemMove(ptr, ptrLast, sizeof(TType));				// move last element in place of [i] removed

	--_nSize;
}

// ********************************************
//	Remove
// ********************************************
template <class TType>
UInt32 TVector<TType>::Remove(const TType& Elt, Bool bRemoveAll)
{
	UInt32 nbr = 0;
	Iterator	Cur = GetHead();

	while (Cur != GetTail())
	{
		if ((*Cur) == Elt)
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

// ********************************************
//	Remove
// ********************************************
template <class TType>
void TVector<TType>::Remove(const Iterator& it)
{
	TAssert(it.GetVector() == this);
	RemoveIndex(it.GetIndex());
}

// ********************************************
//	RemoveFast
// ********************************************
template <class TType>
void TVector<TType>::RemoveFast(const Iterator& it)
{
	RemoveIndexFast(it.GetIndex());
}

// ********************************************
//	RemoveFast
// ********************************************
template <class TType>
void TVector<TType>::RemoveFast(const TType& Elt)
{
	Iterator Cur = Find(Elt);
	if (Cur != GetTail())
	{
		RemoveIndexFast(Cur.GetIndex());
	}
}

// ********************************************
//	Find
// ********************************************
template <class TType>
typename TVector<TType>::Iterator TVector<TType>::Find(const Iterator& it, const TType& Elt) const
{
	TAssert(it.IsValid());

	UInt32 i;
	for (i = it.GetIndex() ; i < _nSize ; ++i)
	{
		if (_pArray[i] == Elt)
		{
			break;
		}
	}

	return (Iterator(const_cast<TVector<TType>*>(this), i));
}

// ********************************************
//	Sort
// ********************************************
template <class TType>
void TVector<TType>::Sort()
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

// ********************************************
//	IsSorted
// ********************************************
template <class TType>
Bool TVector<TType>::IsSorted() const
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

// ********************************************
//	QuickSort
// ********************************************
template <class TType>
void TVector<TType>::QuickSort(Int32 Left, Int32 Right)
{
	if	(Right>Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = Partition(Left, Right, Pivot);
		QuickSort(Left, NewPivot - 1);
		QuickSort(NewPivot + 1, Right);
	}
}

// ********************************************
//	Partition
// ********************************************
template <class TType>
Int32 TVector<TType>::Partition(Int32 Left, Int32 Right, Int32 Pivot)
{
	TType	PivotValue = _pArray[Pivot];
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

// ********************************************
//	Swap
// ********************************************
template <class TType>
void TVector<TType>::Swap(Int32	X, Int32 Y)
{
	TSwap(_pArray[X], _pArray[Y]);
}

// ********************************************
//	Swap
// ********************************************
template <class TType>
void TVector<TType>::Swap(TVector<TType>& V)
{
	TSwap( _pArray,		V._pArray	  );
	TSwap( _nSize,		V._nSize	  );
	TSwap( _nAllocSize, V._nAllocSize );
}

// ********************************************
//	Insert
// ********************************************
template <class TType>
typename TVector<TType>::Iterator TVector<TType>::Insert(const Iterator& it, const TType& Elt)
{
	TAssert(it.IsValid());

	if (GetSize() > 0)
	{
		TType temp = *GetLast(); // We used a copy because we can have unwanted destruction of GetLast if the vector is resized
		PushTail(temp);
		UInt32 i = _nSize-1;
		for (; i > it.GetIndex() ; --i)
		{
			_pArray[i] = _pArray[i - 1];
		}

		_pArray[i] = Elt;
	}
	else
	{
		PushTail(Elt);
		return GetHead();
	}

	return it;
}

// ********************************************
//	InsertSort
// ********************************************
template <class TType>
typename TVector<TType>::Iterator TVector<TType>::InsertSort(const TType& Element, const Bool bAllowDoublon, Bool& bFound)
{
	Iterator	Cur = GetHead();
	Iterator	End = GetTail();

	bFound = false;
	while	((Cur != End) && (*Cur < Element))
	{
		++Cur;
	}

	if (Cur != End)
	{
		bFound = (*Cur == Element);

		if	((bAllowDoublon == false) && (bFound == true))
		{
			return Cur;
		}
	}

	Cur = Insert(Cur, Element);
	return	Cur;
}

// ********************************************
//	InsertOnce
// ********************************************
template <class TType>
typename TVector<TType>::Iterator TVector<TType>::InsertOnce(const Iterator& I, const TType& N)
{
	Iterator Cur = Find(N);
	if (Cur == GetTail())
	{
		Cur = Insert(I, N);
	}

	return Cur;
}
