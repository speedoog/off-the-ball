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

//*****************************************************************************
//
// HOW TO USE IT :
//
// Just make your class heritate from TQuickListElement
// Ex : 
//		class myclass : public TQuickListElement
//		{
//			public:
//				...
//			private:
//				...
//		}
// Then use : 
//		myclass*		instance;
//		TQuickList		mylist;
//		instance = new myclass;
// 		mylist.InsertHead(instance);
// 		mylist.DeleteElement(instance);
// 
// Remarks : 
// 		- Sup method just break links inside the list but do not delete the 
//        element itself. To delete an element, use method 
//		  DeleteElement(TQuickListElement*).
// 		- This class is fully independant. No need anything else to compile and run. 
// 		Keep it like this !
// 		- Take care of the basic error : put a "static element".
// Ex : 
//		myclass			instance;
//		TQuickList		mylist;
//		mylist.InsertHead(instance);
//		mylist.DeleteElement(instance);	
//		-> This will cause an assertion failure because cannot delete something 
//         not alloacted by a "new" !
// 
//*****************************************************************************

#ifndef __BASE_TQUICKLIST_H__
#define __BASE_TQUICKLIST_H__

#pragma once

#include "Base.h"

// Usefulls macros

// use it like a classic for
#define FOR_EACH_ELEMENT(LIST,ITER,ETYPE)	for(ETYPE* ITER=(ETYPE*)(LIST).GetHead(); ITER!=NULL; ITER=(ETYPE*)ITER->GetNext())

// find a specific element result in ITER (if ITER=NULL element is not found)
#define FIND_ELEMENT(LIST,ITER,ETYPE,EXP)	ETYPE* ITER=(ETYPE*)LIST.GetHead();		\
											while(ITER)								\
											if	(EXP)								\
												break;								\
											else									\
												ITER=(ETYPE*)ITER->GetNext();		\

// ------------------------------------------------------------------------------------------------------------------------

class TQuickListElement
{
public:
	inline						TQuickListElement()														{ _pPrev=(TQuickListElement*)NULL; _pNext=(TQuickListElement*)NULL; }
	inline						TQuickListElement(TQuickListElement* pPrev, TQuickListElement* pNext)	{ _pPrev=pPrev; _pNext=pNext; }
	virtual						~TQuickListElement()													{}
	inline void					SetNext(TQuickListElement* Next)										{ _pNext=Next; }
	inline void					SetPrev(TQuickListElement* Prev)										{ _pPrev=Prev; }
	inline TQuickListElement*	GetNext() const															{ return _pNext; }
	inline TQuickListElement*	GetPrev() const															{ return _pPrev; }

protected:
	TQuickListElement*	_pPrev;
	TQuickListElement*	_pNext;
};

// ------------------------------------------------------------------------------------------------------------------------

typedef Bool	(*ListApplyFunc)	(TQuickListElement*, UInt32);
typedef Bool	(*ListSortFunc)		(TQuickListElement*, TQuickListElement*);

class TQuickList
{
public:
	inline						TQuickList()										{ Init(); }
	inline						~TQuickList();

	inline void					Init()												{ _pHead=(TQuickListElement*)NULL; _pQueue=(TQuickListElement*)NULL; _nSize=0; _bSupOnDestructor = false;}

	inline void					SetSupOnDestructor(Bool bSupOnDestructor)			{ _bSupOnDestructor = bSupOnDestructor; }

	inline void					InsertHead(TQuickListElement*);
	inline void					InsertQueue(TQuickListElement*);

	inline void					InsertBefore(TQuickListElement*,TQuickListElement* );
	inline void					InsertAfter(TQuickListElement*,TQuickListElement* );

	inline TQuickListElement*	GetHead() const										{ return _pHead; }
	inline TQuickListElement*	GetQueue() const									{ return _pQueue; }
	inline TQuickListElement*	GetIndex(UInt32 nIndex) const;

	inline TQuickListElement*	Sup(TQuickListElement*);						// Do not delete the element! Just break links
	inline void					SupAllElements();								// remove but don't delete all elements
	inline void					DeleteElement(TQuickListElement* pElement);		// Delete element, using virtual destructor
	inline void					DeleteAllElements();

	inline UInt32				GetNbElements() const								{ return _nSize; }

	inline Bool					IsEmpty() const										{ return _nSize == 0; }

	inline TQuickListElement*	operator[](UInt32);

	inline int					Find(const TQuickListElement* pElement) const;

	inline TQuickListElement*	Apply(ListApplyFunc, UInt32);

	inline void					TransfertElementsTo(TQuickList& lDestination);

	inline void					Sort(ListSortFunc);
	inline void					QSort(ListSortFunc);

protected:
	inline void					q_sort_internal(TQuickListElement *pLeft, TQuickListElement *pRight, ListSortFunc a_Function);

protected:
	TQuickListElement*	_pHead;
	TQuickListElement*	_pQueue;
	UInt32				_nSize;
	Bool				_bSupOnDestructor;
};

TQuickList::~TQuickList()
{
	if(_bSupOnDestructor)
	{
		SupAllElements();
	}
	else
	{
		DeleteAllElements();
	}
}

//	Usage : u must supply a function which compares two elements and return true if element should be zapped
//  bubble sort, quite slow except in particular cases, add other sorting methods !
void inline TQuickList::Sort(ListSortFunc Function)
{
	int					Zapped = true;
	TQuickListElement	*Element;

	if(_nSize < 2)
		return;

	while(Zapped)
	{
		Zapped = 0;
		Element = _pHead;
		while(Element)
		{
			if(Element->GetNext())
			{
				if(Function(Element,Element->GetNext()))
				{
					// Zap
					Zapped = true;
					TQuickListElement *Element2;
					TQuickListElement *bckp2;

					Element2	=	Element->GetNext();
					bckp2		=	Element->GetPrev();

					if(Element->GetPrev())	Element->GetPrev()->SetNext(Element2);
					if(Element2->GetNext())	Element2->GetNext()->SetPrev(Element);

					Element->SetNext(Element2->GetNext());
					Element->SetPrev(Element2);

					Element2->SetNext(Element);
					Element2->SetPrev(bckp2);

					if(Element == _pHead)
						_pHead = Element2;

					if(Element2 == _pQueue)
						_pQueue = Element;

					Element = Element2;
				}
			}
			Element = Element->GetNext();
		}
	}
}

void inline TQuickList::QSort(ListSortFunc a_Function)
{
	q_sort_internal(GetHead(), GetQueue(), a_Function);
}

void TQuickList::q_sort_internal(TQuickListElement *pLeft, TQuickListElement *pRight, ListSortFunc a_Function)
{
	if( (pLeft==NULL) || (pRight==NULL) )
		return;
	if( pLeft == pRight )
		return;

	TQuickListElement	*pPivot		= pLeft;
	TQuickListElement	*pScan		= pLeft->GetNext();
	TQuickListElement	*pNewRight	= pRight;

	pLeft = NULL;

	while(pScan)
	{
		TQuickListElement	*pNext = pScan->GetNext();
		if( a_Function(pPivot, pScan) )
		{
			if( pScan == pNewRight )
				pNewRight = pScan->GetPrev();
			Sup(pScan);
			InsertBefore(pPivot, pScan);
			if( pLeft == NULL )
				pLeft = pScan;
		}
		if(pScan == pRight)
			break;

		pScan = pNext;
	}

	q_sort_internal( pLeft, pPivot, a_Function );
	q_sort_internal( pPivot->GetNext(), pNewRight, a_Function );
}

void TQuickList::InsertHead(TQuickListElement* pElement)
{
	TAssert(pElement != NULL && "TQuickList: trying to insert a NULL element");

	pElement->SetNext(_pHead);
	pElement->SetPrev((TQuickListElement*)NULL);

	if (_pHead)
		_pHead->SetPrev(pElement);
	else
		_pQueue = pElement;

	_pHead = pElement;
	_nSize++;
}

void TQuickList::InsertQueue(TQuickListElement* pElement)
{
	TAssert(pElement != NULL && "TQuickList: trying to insert a NULL element");

	pElement->SetPrev(_pQueue);
	pElement->SetNext((TQuickListElement*)NULL);

	if (_pQueue)
		_pQueue->SetNext(pElement);
	else
		_pHead = pElement;

	_pQueue = pElement;
	_nSize++;
}

void TQuickList::InsertBefore(TQuickListElement* pExistingElement,TQuickListElement* pElementToInsert)
{
	TAssert(pElementToInsert != NULL && "TQuickList: trying to insert a NULL element");
	TAssert(pExistingElement != NULL && "TQuickList: trying to insert an element before a NULL element");

	pElementToInsert->SetPrev(pExistingElement->GetPrev());
	pElementToInsert->SetNext(pExistingElement);

	if (pExistingElement->GetPrev())
		pExistingElement->GetPrev()->SetNext(pElementToInsert);
	else
		_pHead = pElementToInsert;

	pExistingElement->SetPrev(pElementToInsert);
	_nSize++;
}

void TQuickList::InsertAfter(TQuickListElement* pExistingElement,TQuickListElement* pElementToInsert)
{
	TAssert(pElementToInsert != NULL && "TQuickList: trying to insert a NULL element");
	TAssert(pExistingElement != NULL && "TQuickList: trying to insert an element after a NULL element");

	pElementToInsert->SetNext(pExistingElement->GetNext());
	pElementToInsert->SetPrev(pExistingElement);

	if (pExistingElement->GetNext())
		pExistingElement->GetNext()->SetPrev(pElementToInsert);
	else
		_pQueue = pElementToInsert;

	pExistingElement->SetNext(pElementToInsert);
	_nSize++;
}

TQuickListElement* TQuickList::Sup(TQuickListElement* pElement)
{
	TAssert(pElement != NULL && "TQuickList: trying to sup a NULL element");

	if (pElement->GetPrev())
	{
		pElement->GetPrev()->SetNext(pElement->GetNext());
	}
	else
	{
		_pHead = pElement->GetNext();
	}

	if (pElement->GetNext())
	{
		pElement->GetNext()->SetPrev(pElement->GetPrev());
	}
	else
	{
		_pQueue = pElement->GetPrev();
	}

	_nSize--;

	return pElement;
}

TQuickListElement* TQuickList::Apply(ListApplyFunc pFunc, UInt32 UserValue=0)
{
	TQuickListElement* pElement;

	pElement = _pHead;
	while (pElement)
	{
		if (!pFunc(pElement,UserValue))
			return pElement;
		pElement = pElement->GetNext();
	}
	
	return (TQuickListElement*)NULL;
}

int	TQuickList::Find(const TQuickListElement* pElement) const
{
	TQuickListElement* pElem = GetHead();
	int	iElement= 0;
	while (pElem)
	{
		if (pElem == pElement)
			return iElement;

		iElement++;
		pElem = pElem->GetNext();
	}

	return -1;
}

TQuickListElement* TQuickList::GetIndex(UInt32 nIndex) const
{
	TQuickListElement*	pElement	= GetHead();
	UInt32		i			= 0;

	if( nIndex >= _nSize)
		return (TQuickListElement*)NULL;
	
	while (pElement && i!=nIndex)
	{
		pElement = pElement->GetNext();
		i++;
	}

	return pElement;
}

TQuickListElement* TQuickList::operator[](UInt32 nIndex)
{
	TQuickListElement*	pElement=GetHead();
	UInt32		i=0;

	if( nIndex >= _nSize)
		return (TQuickListElement*)NULL;
	
	while (pElement && i!=nIndex)
	{
		pElement = pElement->GetNext();
		i++;
	}

	return pElement;
}

void TQuickList::DeleteElement(TQuickListElement* pElmt)
{
	Sup(pElmt);
	delete pElmt;
}

void TQuickList::DeleteAllElements()
{
	while( GetHead() )
	{
		DeleteElement(GetHead());
	}
}

void TQuickList::SupAllElements()
{
	while( GetHead() )
	{
		Sup(GetHead());
	}
}

void TQuickList::TransfertElementsTo(TQuickList& lDestination)
{
	TQuickListElement*	pSourceHead =GetHead();
	if (pSourceHead)	// Check source list isn't empty
	{
		TQuickListElement*	pDestQueue =lDestination.GetQueue();
		if (pDestQueue==NULL)
		{
			// Destination is empty -> do a simple copy
			lDestination._pHead		=_pHead;
			lDestination._pQueue	=_pQueue;
			lDestination._nSize		=_nSize;
		}
		else
		{
			// Enqueue items (link list extremities)
			pDestQueue->SetNext(pSourceHead);
			pSourceHead->SetPrev(pDestQueue);
			lDestination._pQueue	=_pQueue;
			lDestination._nSize +=_nSize;
		}

		// Clear source list
		_pHead	=(TQuickListElement*)NULL;
		_pQueue	=(TQuickListElement*)NULL;
		_nSize	=0;
	}
}

#endif	//__BASE_TQUICKLIST_H__
