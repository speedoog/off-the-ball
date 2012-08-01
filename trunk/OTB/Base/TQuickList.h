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

//#define NULL    0

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
protected:
	TQuickListElement*	_Prev;
	TQuickListElement*	_Next;

public:
	inline			TQuickListElement()													{ _Prev=(TQuickListElement*)NULL; _Next=(TQuickListElement*)NULL; }
	inline			TQuickListElement(TQuickListElement* Prev, TQuickListElement* Next)	{ _Prev=Prev; _Next=Next; }
	inline virtual ~TQuickListElement()													{}
	inline void		SetNext(TQuickListElement* Next)									{ _Next=Next; }
	inline void		SetPrev(TQuickListElement* Prev)									{ _Prev=Prev; }
	inline			TQuickListElement*	GetNext() const									{ return _Next; }
	inline			TQuickListElement*	GetPrev() const									{ return _Prev; }
};

// ------------------------------------------------------------------------------------------------------------------------

typedef bool	(*ListApplyFunc)	(TQuickListElement*, unsigned long);
typedef bool	(*ListSortFunc)		(TQuickListElement*, TQuickListElement*);

class TQuickList
{
protected:
	TQuickListElement*			_Head;
	TQuickListElement*			_Queue;
	unsigned long				_nNbElements;
	bool						_bSupOnDestructor;

public:
	inline						TQuickList()											{ Init(); }
	inline						~TQuickList();

	inline void					Init(void)											{ _Head=(TQuickListElement*)NULL; _Queue=(TQuickListElement*)NULL; _nNbElements=0; _bSupOnDestructor = false;}

	inline void					SetSupOnDestructor(bool bSupOnDestructor)			{ _bSupOnDestructor = bSupOnDestructor; }

	inline void					InsertHead(TQuickListElement*);
	inline void					InsertQueue(TQuickListElement*);

	inline void					InsertBefore(TQuickListElement*,TQuickListElement* );
	inline void					InsertAfter(TQuickListElement*,TQuickListElement* );

	inline void					Sort(ListSortFunc);

	inline TQuickListElement*	GetHead() const										{ return _Head; }
	inline TQuickListElement*	GetQueue() const									{ return _Queue; }
	inline TQuickListElement*	GetIndex(unsigned long Index) const;

	inline TQuickListElement*	Sup(TQuickListElement*);						// Do not delete the element! Just break links
	inline void					SupAllElements(void);						// remove but don't delete all elements
	inline void					DeleteElement(TQuickListElement* pElement);	// Delete element, using virtual destructor
	inline void					DeleteAllElements(void);

	inline unsigned long		GetNbElements() const								{ return _nNbElements; }

	inline bool							IsEmpty() const { return _nNbElements == 0; }

	inline TQuickListElement*	operator[](unsigned long);

	inline int					Find(const TQuickListElement* pElement) const;

	inline TQuickListElement*	Apply(ListApplyFunc, unsigned long);

	inline void					TransfertElementsTo(TQuickList& lDestination);

	inline void					QSort(ListSortFunc);

protected:
	inline void					q_sort_internal(TQuickListElement *pLeft, TQuickListElement *pRight, ListSortFunc a_Function);
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

	if(_nNbElements < 2)
		return;

	while(Zapped)
	{
		Zapped = 0;
		Element = _Head;
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

					if(Element == _Head)
						_Head = Element2;

					if(Element2 == _Queue)
						_Queue = Element;

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

	pElement->SetNext(_Head);
	pElement->SetPrev((TQuickListElement*)NULL);

	if (_Head)
		_Head->SetPrev(pElement);
	else
		_Queue = pElement;

	_Head = pElement;
	_nNbElements++;
}

void TQuickList::InsertQueue(TQuickListElement* pElement)
{
	TAssert(pElement != NULL && "TQuickList: trying to insert a NULL element");

	pElement->SetPrev(_Queue);
	pElement->SetNext((TQuickListElement*)NULL);

	if (_Queue)
		_Queue->SetNext(pElement);
	else
		_Head = pElement;

	_Queue = pElement;
	_nNbElements++;
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
		_Head = pElementToInsert;

	pExistingElement->SetPrev(pElementToInsert);
	_nNbElements++;
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
		_Queue = pElementToInsert;

	pExistingElement->SetNext(pElementToInsert);
	_nNbElements++;
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
		_Head = pElement->GetNext();
	}

	if (pElement->GetNext())
	{
		pElement->GetNext()->SetPrev(pElement->GetPrev());
	}
	else
	{
		_Queue = pElement->GetPrev();
	}

	_nNbElements--;

	return pElement;
}

TQuickListElement* TQuickList::Apply(ListApplyFunc pFunc, unsigned long UserValue=0)
{
	TQuickListElement* pElement;

	pElement = _Head;
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

TQuickListElement* TQuickList::GetIndex(unsigned long Index) const
{
	TQuickListElement*	pElement	= GetHead();
	unsigned long		i			= 0;

	if( Index >= _nNbElements)
		return (TQuickListElement*)NULL;
	
	while (pElement && i!=Index)
	{
		pElement = pElement->GetNext();
		i++;
	}

	return pElement;
}

TQuickListElement* TQuickList::operator[](unsigned long Index)
{
	TQuickListElement*	pElement=GetHead();
	unsigned long		i=0;

	if( Index >= _nNbElements)
		return (TQuickListElement*)NULL;
	
	while (pElement && i!=Index)
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

void TQuickList::DeleteAllElements(void)
{
	while( GetHead() )
	{
		DeleteElement(GetHead());
	}
}

void TQuickList::SupAllElements(void)
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
			lDestination._Head			=_Head;
			lDestination._Queue			=_Queue;
			lDestination._nNbElements	=_nNbElements;
		}
		else
		{
			// Enqueue items (link list extremities)
			pDestQueue->SetNext(pSourceHead);
			pSourceHead->SetPrev(pDestQueue);
			lDestination._Queue	=_Queue;
			lDestination._nNbElements +=_nNbElements;
		}

		// Clear source list
		_Head		=(TQuickListElement*)NULL;
		_Queue		=(TQuickListElement*)NULL;
		_nNbElements=0;
	}
}

#endif	//__BASE_TQUICKLIST_H__
