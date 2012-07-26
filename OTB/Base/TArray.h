
#ifndef __BASE_TARRAY_H__
#define __BASE_TARRAY_H__

#pragma once

#include "../Base.h"

template <class TType, UInt32 TCapacity>
class TArray
{
public:
	typedef TArray<TType, TCapacity>	TTypeContainer;
	typedef TType						TTypeArray[TCapacity];
	typedef Bool						(*COMPARISON_CALLBACK)(void* pParam1, void* pParam2);

	// ------------------------------------------------ TIterator ------------------------------------------------

	class TIterator
	{
	public:
		inline				TIterator(const TIterator& it): _pArray(it._pArray), _nIndex(it._nIndex)						{ /* Empty */		}
		inline				TIterator(TArray* pArray =NULL, const UInt32 nIndex =0): _pArray(pArray), _nIndex(nIndex)	{ AssertValid();	}
		inline				~TIterator()																					{ /* Empty */		}

		// Inc, Dec operators
		inline	TIterator&	operator ++ ()								{ ++_nIndex;						AssertValid(); return *this;	}
		inline	TIterator	operator +  (const Int32 n) const			{ TIterator it(_pArray, _nIndex+n);	AssertValid(); return it;		}
		inline	TIterator&	operator += (const Int32 n)					{ _nIndex += n;						AssertValid(); return *this;	}
									 																						 
		inline	TIterator&	operator -- ()								{ --_nIndex;						AssertValid(); return *this;	}
		inline	TIterator	operator -  (const Int32 n) const			{ TIterator it(_pArray, _nIndex-n);	AssertValid(); return it;		}
		inline	TIterator&	operator -= (const Int32 n)					{ _nIndex -= n;						AssertValid(); return *this;	}

		// Copy, Dereference
		inline	TIterator&	operator =  (const TIterator& it)			{ _pArray =it._pArray; _nIndex =it._nIndex;	return *this;			}
		inline	TType&		operator *  () const						{ AssertValid();		return _pArray->At(_nIndex);				}

		// Compare
		inline	Bool		operator == (const TIterator& it) const		{ AssertCompatible(it);	return _nIndex==it._nIndex;					}
		inline	Bool		operator != (const TIterator& it) const		{ AssertCompatible(it); return _nIndex!=it._nIndex;					}
		inline	Bool		operator >  (const TIterator& it) const		{ AssertCompatible(it); return _nIndex>it._nIndex;					}
		inline	Bool		operator >= (const TIterator& it) const		{ AssertCompatible(it); return _nIndex>=it._nIndex;					}
		inline	Bool		operator <  (const TIterator& it) const		{ AssertCompatible(it); return _nIndex<it._nIndex;					}
		inline	Bool		operator <= (const TIterator& it) const		{ AssertCompatible(it); return _nIndex<=it._nIndex;					}

		// Validity
		inline	Bool		IsValid() const								{ return (_pArray && _nIndex<=_pArray->GetSize());					}
		inline	void		AssertValid() const							{ RfxAssert(IsValid());												}
		inline	void		AssertCompatible(const TIterator& it) const	{ AssertValid(); it.AssertValid(); RfxAssert(_pArray==it._pArray); }

		// Classic access
		inline	UInt32		GetIndex() const							{ return _nIndex;	}
		inline	TArray*	GetArray() const							{ return _pArray;	}

	protected:
		TArray*	_pArray;
		UInt32	_nIndex;
	};

	// ------------------------------------------------ CONTAINER ------------------------------------------------

	inline						TArray();
	inline						~TArray()							{ Clear(); }

	// Clear / ClearAll
	inline	void				Clear();
	inline	void				ClearAll();

	// Accessors
	inline	UInt32				GetSize() const							{ return _nSize;	}
	inline	UInt32				GetCapacity() const						{ return TCapacity;	}
	inline	void				Reserve(const UInt32 nSize) const		{ RfxAssert(nSize<=GetCapacity()); /* nothing */ }
	inline	Bool				IsEmpty() const							{ return _nSize==0;		}
	inline	Bool				IsFull() const							{ return (GetSize()>=GetCapacity()); }

	// Filling methods
	inline	void				PushTail(const TType& Obj);
	inline	void				PushTail(const TTypeContainer& Container);
	inline	void				Fill(const TType& Obj);
	inline	TIterator			PushOnce(const TType& Obj)
	{
		TIterator it = Find(GetHead(), Obj);
		if (it== GetTail())
		{
			PushTail(Obj);
			it= TIterator(this, GetSize()-1);
		}
		return it;
	}

	inline	void				Resize(const UInt32 nNewSize);
	inline	void				RemoveIndex(const UInt32 nIndex);
	inline	void				RemoveIndexFast(const UInt32 nIndex);
	inline	void				Remove(const TIterator& it);

	// Iterator
	inline	TIterator			GetHead() const							{ return (TIterator(const_cast<TTypeContainer*>(this), 0));		}
	inline	TIterator			GetTail() const							{ return (TIterator(const_cast<TTypeContainer*>(this), _nSize));	}

	// Array accessors
	inline	TType&				GetLast()								{ RfxAssert(!IsEmpty()); return At(_nSize-1); }
	inline	const TType&		GetLast() const							{ RfxAssert(!IsEmpty()); return At(_nSize-1); }
	inline	TType&				At(const UInt32 nIndex);
	inline	const TType&		At(const UInt32 nIndex) const;
	inline	TType&				operator [] (const UInt32 nIndex)		{ return At(nIndex); }
	inline	const TType&		operator [] (const UInt32 nIndex) const	{ return At(nIndex); }

	inline	TTypeContainer&		operator = (const TTypeContainer& Source);

	// Pointers accessors
	inline	TType*				GetArray()								{ return (TType*)&_Raw;  }
	inline	const TType*		GetArray() const						{ return (TType*)&_Raw;  }
	inline	TType*				GetTailPtr()							{ return GetArray()+_nSize; }
	inline	const TType*		GetTailPtr() const						{ return GetArray()+_nSize; }

	// Find
	inline	UInt32				Find(const UInt32& nStart, const TType& Obj) const;
	inline	TIterator			Find(const TIterator& it, const TType& Obj) const { return TIterator(const_cast<TTypeContainer*>(this), Find(it.GetIndex(), Obj)); }

	inline	void				Swap(const Int32 i, const Int32 j);

	// Sorting (use function template as sorter)
	template <Bool TCompare(const TType& a0, const TType& a1)>
	inline	void				Sort();

	// Helper
	inline UInt32				GetMemoryUsed() const					{ return sizeof(TTypeContainer); }

protected:
	template <Bool TCompare(const TType& a0, const TType& a1)>
	inline	void				QuickSort(const Int32 nLeft, const Int32 nRight);
	template <Bool TCompare(const TType& a0, const TType& a1)>
	inline	Int32				Partition(const Int32 nLeft, const Int32 nRight, const Int32 nPivot);

protected:
	UInt8				_Raw[sizeof(TType)*TCapacity];	// Ned POD type here in order to avoid implicit call to TType::Ctor()
	UInt32				_nSize;							// Current size (nb of items in container != TCapacity )

#	ifndef OTB_MASTER
	const TTypeArray&	_Debug;							// For debugging usage only !
#	endif	//OTB_MASTER
};

#include "TArray.inl"

#endif //__BASE_TARRAY_H__
