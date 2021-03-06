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
//                   �>   .                                      <�                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////


// ****************************************************************************************
// Ctor
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
TArray<TType, TCapacity>::TArray()
: _nSize(0)
#ifndef T_MASTER
, _Debug((TTypeArray&)_Raw)
#endif	//T_MASTER
{
}

// ****************************************************************************************
// Clear
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Clear()
{
	for (UInt32 i=0; i<_nSize; ++i)
	{
		TDeletePlaced(&At(i), TType);
	}
	_nSize =0;
}

// ****************************************************************************************
// ClearAll
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::ClearAll()
{
	for (UInt32 i=0; i<_nSize; ++i)
	{
		delete At(i);
	}
	Clear();
}

// ****************************************************************************************
// PushTail
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::PushTail(const TType& Obj)
{
	TAssert(!IsFull());
	TNewPlaced(GetArray()+_nSize, TType)(Obj);
	++_nSize;
}

// ****************************************************************************************
// PushTail
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::PushTail(const TArray<TType, TCapacity>& Container)
{
	const UInt32 nSumSize =_nSize+Container._nSize;

	TAssert(nSumSize<=GetCapacity());

	TType* pDst =GetTailPtr();
	const TType* pSrc =Container.GetArray();
	const TType* pEnd =Container.GetTailPtr();
	for (; pSrc!=pEnd; ++pSrc, ++pDst)
	{
		TNewPlaced(pDst, TType)(*pSrc);
	}
	_nSize =nSumSize;
}

// ****************************************************************************************
// Fill
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Fill(const TType& Obj)
{
	for (UInt32 i =0; i<_nSize; ++i)
	{
		At(i) =Obj;
	}
}

// ****************************************************************************************
// PushOnce
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
typename TArray<TType, TCapacity>::Iterator TArray<TType, TCapacity>::PushOnce(const TType& Obj)
{
	Iterator it = Find(GetHead(), Obj);
	if (it== GetTail())
	{
		PushTail(Obj);
		it= Iterator(this, GetSize()-1);
	}
	return it;
}

// ****************************************************************************************
// Resize
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Resize(const UInt32 nNewSize)
{
	TAssert(nNewSize<=GetCapacity());

	const UInt32 uActualSize =GetSize();
	if (nNewSize<uActualSize)
	{
		for (UInt32 i =nNewSize; i<uActualSize; ++i)
		{
			TDeletePlaced(GetArray()+i, TType);
		}
	}
	else
	{
		for (UInt32 i =uActualSize; i<nNewSize; ++i)
		{
			TNewPlaced(GetArray()+i, TType);
		}
	}
	_nSize = nNewSize;
}

// ****************************************************************************************
// Remove
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Remove(const Iterator& it)
{
	TAssert(it.GetArray()==this);
	RemoveIndex(it.GetIndex());
}

// ****************************************************************************************
// RemoveIndex
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::RemoveIndex(const UInt32 nIndex)
{
	TAssert(nIndex<GetSize());

	const UInt32 nLast =GetSize()-1;
	for (UInt32 i =nIndex; i<nLast; ++i)
	{
		At(i) = At(i+1);
	}

	TDeletePlaced(GetArray()+nLast, TType);
	--_nSize;
}

// ****************************************************************************************
// RemoveIndexFast
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::RemoveIndexFast(UInt32 nIndex)
{
	TAssert(nIndex<GetSize());

	TType* ptr =&At(nIndex);
	TDeletePlaced(ptr, TType);

	TType* ptrLast =GetTailPtr()-1;
	TMemMove(ptr, ptrLast, sizeof(TType));				// move last element in place of [i] removed

	--_nSize;
}

// ****************************************************************************************
// At
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
TType& TArray<TType, TCapacity>::At(const UInt32 nIndex)
{
#	ifdef T_DEBUG
	if (nIndex>=GetSize())
	{
		TFail();
		return GetArray()[0];			// Ensure not accessing out of boundaries ! (it's the caller's responsability to check index)
	}
#	endif // T_DEBUG
	return GetArray()[nIndex];
}

// ****************************************************************************************
// At
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
const TType& TArray<TType, TCapacity>::At(const UInt32 nIndex) const
{
#	ifdef T_DEBUG
	if (nIndex>=GetSize())
	{
		TFail();
		return GetArray()[0];			// Ensure not accessing out of boundaries ! (it's the caller's responsability to check index)
	}
#	endif // T_DEBUG
	return GetArray()[nIndex];
}

// ****************************************************************************************
// Find
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
UInt32 TArray<TType, TCapacity>::Find(const UInt32& nStart, const TType& Obj) const
{
	const UInt32 nSize =GetSize();
	TAssert((nSize==0) || (nStart<nSize));

	UInt32 i =nStart;
	for (; i<nSize; ++i)
	{
		if (At(i)==Obj)
		{
			break;
		}
	}
	return i;
}

// ****************************************************************************************
// Sort
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
template <Bool TCompare(const TType& a0, const TType& a1)>
void TArray<TType, TCapacity>::Sort()
{
	const Int32 nSize =GetSize();
	if (nSize==2)
	{
		if (TCompare(At(1), At(0)))
		{
			Swap(0, 1);
		}
	}
	else
	{
		if (nSize>2)
		{
			QuickSort<TCompare>(0, nSize-1);
		}
	}
}

// ****************************************************************************************
// QuickSort
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
template <Bool TCompare(const TType& a0, const TType& a1)>
void TArray<TType, TCapacity>::QuickSort(const Int32 nLeft, const Int32 nRight)
{
	if (nRight>nLeft)
	{
		const Int32 nPivot	  =nLeft + ((nRight-nLeft)/2);
		const Int32 nNewPivot =Partition<TCompare>(nLeft, nRight, nPivot);
		QuickSort<TCompare>(nLeft, nNewPivot-1);
		QuickSort<TCompare>(nNewPivot+1, nRight);
	}
}

// ****************************************************************************************
// Partition
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
template <Bool TCompare(const TType& a0, const TType& a1)>
Int32 TArray<TType, TCapacity>::Partition(const Int32 nLeft, const Int32	nRight, const Int32	nPivot)
{
	TType PivotValue = At(nPivot);
	Swap(nPivot, nRight);					// Move pivot to end
	Int32 nStoreIndex = nLeft;
	for	(Int32 i =nLeft; i<nRight; ++i)
	{
		if (TCompare(At(i), PivotValue))
		{
			Swap(nStoreIndex, i);
			++nStoreIndex;
		}
	}
	Swap(nRight, nStoreIndex);				// Move pivot to its final place
	return nStoreIndex;
}

// ****************************************************************************************
// Swap
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Swap(const Int32 i, const Int32 j)
{
	TType objTemp =At(i);
	At(i) =At(j);
	At(j) =objTemp;
}

// ****************************************************************************************
// operator = (copy from)
// ****************************************************************************************
template <class TType, UInt32 TCapacity>
TArray<TType, TCapacity>& TArray<TType, TCapacity>::operator = (const TArray<TType, TCapacity>& Source)
{
	Clear();
	PushTail(Source);
	return (*this);
}
