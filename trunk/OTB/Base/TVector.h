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

#ifndef __BASE_TVECTOR_H__
#define __BASE_TVECTOR_H__

#pragma once

#include "Base.h"

typedef Bool (*COMPARISON_CALLBACK)(void* pParam1, void* pParam2);

template <class TType>
class TVector
{
public:

	class Iterator
	{
	public:

		inline 				Iterator(TVector* pVector=NULL, const UInt32 nIndex=0) : _pVector(pVector), _nIndex(nIndex)	{ }
		inline 				~Iterator(){}

		// Operators
		inline	Iterator&	operator ++ ()								{ ++_nIndex; TAssert(IsValid()); return	*this; }
		inline	Iterator&	operator -- ()								{ --_nIndex; TAssert(IsValid()); return	*this; }
		inline	Iterator	operator +  (const UInt32 n) const			{ Iterator Tmp = *this; Tmp._nIndex += n; TAssert(IsValid()); return Tmp; }
		inline	Iterator&	operator += (const UInt32 n)				{ _nIndex += n; TAssert(IsValid()); return *this; }
		inline	Iterator	operator -	(const UInt32 n) const			{ Iterator Tmp = *this; Tmp._nIndex -= n; TAssert(IsValid()); return Tmp; }
		inline	Iterator&	operator -= (const UInt32 n)				{ _nIndex -= n; TAssert(IsValid()); return *this; }

		// Dereference
		inline	TType&		operator *  () const						{ TAssert(IsValid()); TAssert(_pVector!=NULL); return (_pVector->_pArray[_nIndex]);	}
		inline	Iterator&	operator =  (const Iterator& it)			{ _pVector = it._pVector; _nIndex = it._nIndex; return *this;						}

		// Compare
		inline	Bool		operator == (const Iterator& it) const		{ AssertCompatible(it);	return _nIndex==it._nIndex;	}
		inline	Bool		operator != (const Iterator& it) const		{ AssertCompatible(it);	return _nIndex!=it._nIndex;	}
		inline	Bool		operator >	(const Iterator& it) const		{ AssertCompatible(it);	return _nIndex> it._nIndex;	}
		inline	Bool		operator <	(const Iterator& it) const		{ AssertCompatible(it);	return _nIndex< it._nIndex;	}
		inline	Bool		operator >= (const Iterator& it) const		{ AssertCompatible(it);	return _nIndex>=it._nIndex;	}
		inline	Bool		operator <= (const Iterator& it) const		{ AssertCompatible(it);	return _nIndex<=it._nIndex;	}

		// Validity
		inline	Bool		IsValid() const								{ return (_pVector&&_nIndex<=_pVector->GetSize());					}
		inline	void		AssertValid() const							{ TAssert(IsValid());												}
		inline	void		AssertCompatible(const Iterator& it) const	{ AssertValid(); it.AssertValid(); TAssert(_pVector==it._pVector);	}

		// Classic Access
		inline	UInt32		GetIndex() const							{ return _nIndex;									}
		inline	TVector*	GetVector() const							{ return _pVector;									}

	protected:
		TVector* _pVector;
		UInt32	 _nIndex;
	};

	inline 				TVector();
	inline 				TVector(UInt32 nSize, const TType& Value);
	inline 				TVector(const TVector& vector);
	inline 				~TVector();

	inline UInt32		GetCapacity() const					{ return _nAllocSize;	}
	inline UInt32		GetAllocatedSize()					{ return _nAllocSize;	}
	inline UInt32		GetSize() const						{ return _nSize;		}
	inline Bool			IsEmpty() const						{ return _nSize==0;		}

	inline UInt32		GetMemoryUsed() const;
	inline void			Clear();
	inline void			ClearMemory();	// release all mem
	inline void			ClearAll();

	inline Bool 		Reserve(const UInt32 nReserveSize);
	inline void			Resize(const UInt32 uSize);

	// Classic access
	inline TType*		GetArray() const					{ return _pArray; }
	inline const TType* GetArrayConst() const				{ return _pArray; }
	inline Iterator 	GetHead() const						{ return (Iterator(const_cast<TVector<TType>*>(this), 0));			}
	inline Iterator 	GetLast() const						{ return (Iterator(const_cast<TVector<TType>*>(this), _nSize - 1)); }
	inline Iterator 	GetTail() const						{ return (Iterator(const_cast<TVector<TType>*>(this), _nSize));		}

	// Push
	inline void 		PushTail(const TType& Elt);
	inline void 		PushTail(const TType& Elt, UInt32 nTimes);
	inline void 		PushTail(const TVector& v);
	inline Iterator		PushOnce(const TType& Elt);

	inline Iterator		Insert(const Iterator& it, const TType& Elt);
	inline void			Fill(const TType& Elt);

	inline void			PopTail();

	inline Iterator		Find(const Iterator& it, const TType& Elt) const;
	inline Iterator		Find(const TType& Elt) const							{ return Find(GetHead(), Elt); }

	//	Operators
	inline TType&		operator [] (UInt32 nElementIndex);
	inline const TType&	operator [] (UInt32 nElementIndex) const;
	inline TVector& 	operator += (const TType& Elt);
	inline TVector& 	operator += (const TVector<TType>& v);
	inline TVector& 	operator =  (const TVector<TType>& QV);
	inline Bool 		operator == (const TVector<TType>& v) const;
	inline Bool 		operator != (const TVector<TType>& v) const;

	inline TType&		At(UInt32 nElementIndex);
	inline const TType&	At(UInt32 nElementIndex) const;

	inline Iterator		InsertOnce(const Iterator& it, const TType& N);

	inline void 		RemoveIndex(UInt32 i);
	inline void 		RemoveIndexFast(UInt32 i);
	inline void			Remove(const Iterator& it);
	inline void			RemoveFast(const TType& Elt);
	inline UInt32		Remove(const TType& Elt, Bool bRemoveAll = false);


	// Sort
	inline Iterator		InsertSort(const TType& Element, const Bool bAllowDoublon, Bool& bFound);
	inline void			Sort();
	inline Bool			IsSorted() const;
	inline void			QuickSort(Int32 Left, Int32 Right);
	inline Int32		Partition(Int32 Left, Int32 Right, Int32 Pivot);
	inline void			Swap(Int32 X, Int32 Y);
	inline void			Swap(TVector<TType> & V);

protected:
	inline void			Grow(const UInt32 nGrowSize);
	inline void			Grow();

protected:
	TType*		_pArray;
	UInt32		_nSize;
	UInt32		_nAllocSize;
};


#include "TVector.inl"

#endif //__BASE_TVECTOR_H__
