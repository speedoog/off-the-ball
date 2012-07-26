
//-----------------------------------------------------------------------------
//! @brief		Ctor
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
TArray<TType, TCapacity>::TArray()
: _nSize(0)
#ifndef OTB_MASTER
, _Debug((TTypeArray&)_Raw)
#endif	//OTB_MASTER
{
}

//-----------------------------------------------------------------------------
//! @brief		Clear
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Clear()
{
	for (UInt32 i=0; i<_nSize; ++i)
	{
		OTB_DELETE_PLACED(&At(i), TType);
	}
	_nSize =0;
}

//-----------------------------------------------------------------------------
//! @brief		ClearAll
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::ClearAll()
{
	for (UInt32 i=0; i<_nSize; ++i)
	{
		delete At(i);
	}
	Clear();
}

//-----------------------------------------------------------------------------
//! @brief		PushTail
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::PushTail(const TType& Obj)
{
	RfxAssert(!IsFull());
	OTB_NEW_PLACED(GetArray()+_nSize, TType)(Obj);
	++_nSize;
}

//-----------------------------------------------------------------------------
//! @brief		PushTail
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::PushTail(const TArray<TType, TCapacity>& Container)
{
	const UInt32 nSumSize =_nSize+Container._nSize;

	RfxAssert(nSumSize<=GetCapacity());

	TType* pDst =GetTailPtr();
	const TType* pSrc =Container.GetArray();
	const TType* pEnd =Container.GetTailPtr();
	for (; pSrc!=pEnd; ++pSrc, ++pDst)
	{
		OTB_NEW_PLACED(pDst, TType)(*pSrc);
	}
	_nSize =nSumSize;
}

//-----------------------------------------------------------------------------
//! @brief		Fill
//! @details	Sets the value to all elements of the array
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Fill(const TType& Obj)
{
	for (UInt32 i =0; i<_nSize; ++i)
	{
		At(i) =Obj;
	}
}

//-----------------------------------------------------------------------------
//! @brief		Resize
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Resize(const UInt32 nNewSize)
{
	RfxAssert(nNewSize<=GetCapacity());

	const UInt32 uActualSize =GetSize();
	if (nNewSize<uActualSize)
	{
		for (UInt32 i =nNewSize; i<uActualSize; ++i)
		{
			OTB_DELETE_PLACED(GetArray()+i, TType);
		}
	}
	else
	{
		for (UInt32 i =uActualSize; i<nNewSize; ++i)
		{
			OTB_NEW_PLACED(GetArray()+i, TType);
		}
	}
	_nSize = nNewSize;
}

//-----------------------------------------------------------------------------
//! @brief		Remove
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Remove(const TIterator& it)
{
	RfxAssert(it.GetArray()==this);
	RemoveIndex(it.GetIndex());
}

//-----------------------------------------------------------------------------
//! @brief		RemoveIndex
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::RemoveIndex(const UInt32 nIndex)
{
	RfxAssert(nIndex<GetSize());

	const UInt32 nLast =GetSize()-1;
	for (UInt32 i =nIndex; i<nLast; ++i)
	{
		At(i) = At(i+1);
	}

	OTB_DELETE_PLACED(GetArray()+nLast, TType);
	--_nSize;
}

//-----------------------------------------------------------------------------
//! @brief		RemoveIndexFast
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::RemoveIndexFast(UInt32 nIndex)
{
	RfxAssert(nIndex<GetSize());

	TType* ptr =&At(nIndex);
	OTB_DELETE_PLACED(ptr, TType);

	TType* ptrLast =GetTailPtr()-1;
	QDT_MEMMOVE(ptr, ptrLast, sizeof(TType));				// move last element in place of [i] removed

	--_nSize;
}

//-----------------------------------------------------------------------------
//! @brief		At
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
TType& TArray<TType, TCapacity>::At(const UInt32 nIndex)
{
#	ifdef QDT_DEBUG
	if (nIndex>=GetSize())
	{
		QDT_FAIL();
		return GetArray()[0];			// Ensure not accessing out of boundaries ! (it's the caller's responsability to check index)
	}
#	endif
	return GetArray()[nIndex];
}

//-----------------------------------------------------------------------------
//! @brief		At
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
const TType& TArray<TType, TCapacity>::At(const UInt32 nIndex) const
{
#	ifdef QDT_DEBUG
	if (nIndex>=GetSize())
	{
		QDT_FAIL();
		return GetArray()[0];			// Ensure not accessing out of boundaries ! (it's the caller's responsability to check index)
	}
#	endif
	return GetArray()[nIndex];
}

//-----------------------------------------------------------------------------
//! @brief		Find
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
UInt32 TArray<TType, TCapacity>::Find(const UInt32& nStart, const TType& Obj) const
{
	const UInt32 nSize =GetSize();
	RfxAssert((nSize==0) || (nStart<nSize));

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

//-----------------------------------------------------------------------------
//! @brief		Sort
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
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

//-----------------------------------------------------------------------------
//! @brief		QuickSort
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
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

//-----------------------------------------------------------------------------
//! @brief		Partition
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
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

//-----------------------------------------------------------------------------
//! @brief		Swap
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::Swap(const Int32 i, const Int32 j)
{
	TType objTemp =At(i);
	At(i) =At(j);
	At(j) =objTemp;
}

//-----------------------------------------------------------------------------
//! @brief		operator = (copy from)
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
TArray<TType, TCapacity>& TArray<TType, TCapacity>::operator = (const TArray<TType, TCapacity>& Source)
{
	Clear();
	PushTail(Source);
	return (*this);
}

/*
//-----------------------------------------------------------------------------
//! @brief		StreamWrite
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::StreamWrite(IO_STREAM& Stream) const
{
	const UInt32 nSize =GetSize();
	Stream << nSize;

	for (UInt32 i =0; i<GetSize(); ++i)
	{
		Stream << At(i);
	}
}

//-----------------------------------------------------------------------------
//! @brief		StreamRead
//! @author		BFA : 2012-02-08
//-----------------------------------------------------------------------------
template <typename TType, UInt32 TCapacity>
void TArray<TType, TCapacity>::StreamRead(IO_CHUNK_STREAM& Stream)
{
	Clear();

	UInt32 nSize;
	Stream >> nSize;

	Resize(nSize);
	for (UInt32 i =0; i<nSize; ++i)
	{
		Stream >> At(i);
	}
}
*/