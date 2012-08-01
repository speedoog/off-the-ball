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


#ifndef __BASE_TLIST_H__
#define __BASE_TLIST_H__

#pragma once

#include "Base.h"

typedef Bool (*DLIST_COMPARISON_CALLBACK)(void* pParam1, void* pParam2);


// (new impl 05/05/2011) Use it like a classic for
#define FOR_EACH_ELEMENT_OF_DLIST(_list_, _it_, _type_)			\
	FOR_EACH(_list_, _it_, _type_)

// (new impl 05/05/2011) Same as above but next is getted before the loop so you can delete an element safely
#define FOR_EACH_ELEMENT_OF_DLIST_SAFE(_list_, _it_, _type_)	\
	for(_type_::Iterator _it_, _itnext_ = (_list_).GetHead()	\
		, _ITEND_ = (_list_).GetTail();							\
		(_it_ = _itnext_.SafePostIncrement()) != _ITEND_; )

template <class TType>
class TList
{
public:
	class Node
	{
	public:

		inline			Node(const TType& data) : _pPrev(NULL), _pNext(NULL), _Data(data)	{ };
		inline void		SetPrev(Node * pPrev)							{ _pPrev =pPrev;	}
		inline void		SetNext(Node * pNext)							{ _pNext =pNext;	}
		inline void		SetData(const TType & data)						{ _Data  =data;		}
		inline Node*	GetNext() const									{ return _pNext; 	}
		inline Node*	GetPrev() const									{ return _pPrev; 	}
		inline TType&	GetData()										{ return _Data;		}

	protected:
		TType	_Data;
		Node* 	_pPrev;
		Node* 	_pNext;
	};

	class Iterator
	{
	public:

		inline Iterator(TList* pList =NULL, Node* pNode =NULL) : _pNode(pNode), _pList(pList)		{  }

		// Operators
		inline Iterator&	operator++()							{ TAssert(_pNode); _pNode = _pNode->GetNext(); return *this; }
		inline Iterator		operator++(int)							{ Iterator itTmp=*this; ++(*this); return itTmp; }
		inline Iterator		SafePostIncrement()						{ Iterator itTmp=*this; if (_pNode) { _pNode = _pNode->GetNext(); }	return itTmp; }
		inline Iterator		operator+(int n) const					{ Iterator itTmp=*this; int i = 0; while (i<n) { ++itTmp; ++i; }	return itTmp; }
		inline Iterator&	operator+=(int n)						{ int i=0; while (i<n) { ++(*this); ++i; } return *this;	}
		inline Iterator&	operator--()							{ TAssert(_pNode!=_pList->GetHead().GetNode()); if	(_pNode) { _pNode = _pNode->GetPrev(); } else { _pNode = _pList->GetLast().GetNode(); } return *this; }
		inline Iterator		operator--(int)							{ Iterator itTmp=*this; --(*this); return	(itTmp); }
		inline Iterator		operator-(int n) const					{ Iterator itTmp=*this; int i=0; while (i<n) { --itTmp; ++i; } return itTmp; }
		inline Iterator&	operator-=(int n)						{ int i=0; while (i<n) { --(*this); ++i; } return *this; }
		inline Iterator&	operator=(const Iterator& I)			{ _pNode=I._pNode; _pList=I._pList; return *this; }

		// Dereference
		inline TType&		operator*() const						{ TAssert(_pNode); return _pNode->GetData(); }

		// Compare
		inline Bool			operator!=(const Iterator&	I) const	{ TAssert(_pList==I._pList); return (_pNode!=I._pNode);	}
		inline Bool			operator==(const Iterator&	I) const	{ TAssert(_pList==I._pList); return (_pNode==I._pNode);	}

		// LowLevel access
		inline Node*		GetNode() const							{ return _pNode; }
		inline TList*		GetList() const							{ return _pList; }

	private:
		Node*	_pNode;
		TList*	_pList;
	};

public:
	inline 				TList();
	inline 				TList(const TList& List);
	inline 				~TList();

	inline UInt32 		GetSize() const;
	inline UInt32 		GetMemoryUsed() const;
	inline void 		Clear();
	inline void 		ClearAll();

	inline Iterator 	GetHead() const	{ return Iterator(const_cast<TList<TType>*>(this), _pHead);	}
	inline Iterator 	GetLast() const	{ return Iterator(const_cast<TList<TType>*>(this), _pQueue);}
	inline Iterator 	GetTail() const	{ return Iterator(const_cast<TList<TType>*>(this), NULL);	}

	inline void 		PushTail(const TType& Elt);
	inline void 		PushTail(const TList& List);
	inline void 		PopTail();
	inline Bool 		IsEmpty() const;
	inline void 		InsertTail(const TType& Elt);
	inline void 		InsertTail(const TList& List);
	inline void 		InsertHead(const TType& Elt);
	inline void 		InsertHead(TList& List);
	inline void 		RemoveHead();
	inline void 		Push(const TType& Elt, const Bool bReverse);
	inline void 		MoveToHead(const Iterator& I);

	// Find an element in the list! return tail if the element has not been found
	inline Iterator 	Find(const Iterator& itStart, const TType& Elt) const;
	inline Iterator 	Find(const TType& Elt) const;
	inline Iterator 	PushOnce(const TType& Elt);
	inline Bool 		Contains(const TType& Elt) const;
	inline void 		Reverse();

	// Insert / Remove
	inline Iterator 	Insert(const Iterator& it, const TType& Elt);
	inline Iterator 	InsertOnce(const Iterator& it, const TType& Elt);
	inline Iterator 	InsertSort(const TType& Elt, const Bool bAllowDup, Bool& bIsIn);
	inline Bool			InsertSort(const TList& List, const Bool bAllowDup);
	inline Bool 		Remove(const TType& Elt);
	inline void 		Remove(const Iterator& it);

	// Sort
	inline void 		Sort();

	// Operators
	inline TList&		operator +=	(const TType& Elt);
	inline TList&		operator +=	(const TList<TType>& List);
	inline TList&		operator =	(const TList<TType>& List);
	inline Bool			operator == (const TList<TType>& List) const;
	inline Bool			operator != (const TList<TType>& List) const;

private:
	Node*  	_pHead;
	Node*  	_pQueue;
	UInt32	_nSize;
};

#include "TList.inl"

#endif //__BASE_TLIST_H__
