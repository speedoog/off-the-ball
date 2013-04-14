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


// ****************************************************************************************
//	Ctor
// ****************************************************************************************
template <class TType>
TList<TType>::TList()
: _pHead	(NULL)
, _pQueue	(NULL)
, _nSize	(0)
{
}

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
template <class TType>
TList<TType>::TList(const TList& List)
: _pHead(NULL)
, _pQueue(NULL)
, _nSize(0)
{
	PushTail(List);
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
template <class TType>
TList<TType>::~TList()
{
	Clear();
}

// ****************************************************************************************
//	GetSize
// ****************************************************************************************
template <class TType>
UInt32 TList<TType>::GetSize() const
{
	return _nSize;
}

// ****************************************************************************************
//	GetMemoryUsed
// ****************************************************************************************
template <class TType>
UInt32 TList<TType>::GetMemoryUsed() const
{
	UInt32 nMem =0;
 	nMem +=sizeof(TList<TType>);
 	nMem +=GetSize()*sizeof(Node);
	return nMem;
}

// ****************************************************************************************
//	Clear
// ****************************************************************************************
template <class TType>
void TList<TType>::Clear()
{
	Node* pNode =_pHead;
	while (pNode)
	{
		Node* pNext =pNode->GetNext();
		delete pNode;
		pNode =pNext;
	}

	_pHead =NULL;
	_pQueue  =NULL;
	_nSize	=0;
}

// ****************************************************************************************
//	ClearAll
// ****************************************************************************************
template <class TType>
void TList<TType>::ClearAll()
{
	Node* pNode =_pHead;
	while (pNode)
	{
		Node* pNext =pNode->GetNext();
		delete pNode->GetData();				// Call delete DATA !
		delete pNode;
		pNode = pNext;
	}

	_pHead =NULL;
	_pQueue  =NULL;
	_nSize  =0;
}

// ****************************************************************************************
//	MoveToHead
// ****************************************************************************************
template <class TType>
void TList<TType>::MoveToHead(const Iterator& itElt)
{
	TAssert(itElt.GetNode());
	TAssert(itElt.GetList() == this);

	Node* pNodeToMove =itElt.GetNode();

	// Remove
	if (pNodeToMove == _pHead)
		return;

	if (pNodeToMove->GetPrev())
	{
		pNodeToMove->GetPrev()->SetNext(pNodeToMove->GetNext());
	}

	if (pNodeToMove->GetNext())
	{
		pNodeToMove->GetNext()->SetPrev(pNodeToMove->GetPrev());
	}

	if (pNodeToMove == _pQueue)
	{
		_pQueue = _pQueue->GetPrev();
	}

	// Insert Head
	pNodeToMove->SetPrev(NULL);	
	pNodeToMove->SetNext(_pHead);
	_pHead->SetPrev(pNodeToMove);
	_pHead = pNodeToMove;

	++_nSize;
}

// ****************************************************************************************
//	PushTail
// ****************************************************************************************
template <class TType>
void TList<TType>::PushTail(const TType& Elt)
{
	Node* pNode =new Node(Elt);
	if (_pHead==NULL)
	{
		_pHead =pNode;
		_pQueue=pNode;
	}
	else
	{
		_pQueue->SetNext(pNode);
		pNode->SetPrev(_pQueue);
		_pQueue = pNode;
	}

	++_nSize;
}

// ****************************************************************************************
//	PushTail
// ****************************************************************************************
template <class TType>
void TList<TType>::PushTail(const TList& List)
{
	Iterator it = List.GetHead();
	const Iterator tail = List.GetTail();

	while (it!=tail)
	{
		PushTail(*it);
		++it;
	}
}

// ****************************************************************************************
//	PopTail
// ****************************************************************************************
template <class TType>
void TList<TType>::PopTail()
{
	TAssert(IsEmpty()==false);

	Node* pNode =_pQueue;
	_pQueue =pNode->GetPrev();
	delete pNode;
	--_nSize;

	if (_pQueue)
	{
		_pQueue->SetNext(NULL);
	}
	else
	{
		_pHead =NULL;
	}
}

// ****************************************************************************************
//	IsEmpty
// ****************************************************************************************
template <class TType>
Bool TList<TType>::IsEmpty() const
{
	return (GetSize()==0);
}

// ****************************************************************************************
//	InsertHead
// ****************************************************************************************
template <class TType>
void TList<TType>::InsertHead(const TType& Elt)
{
	Node* pNode =new Node(Elt);
	if (_pHead == NULL)
	{
		_pHead =pNode;
		_pQueue  =pNode;
	}
	else
	{
		pNode->SetNext(_pHead);
		_pHead->SetPrev(pNode);
		_pHead =pNode;
	}

	++_nSize;
}

// ****************************************************************************************
//	InsertHead
// ****************************************************************************************
template <class TType>
void TList<TType>::InsertHead(TList& List)
{
	if (List.GetSize()>0)
	{
		Iterator it = List.GetLast();
		Iterator head = List.GetHead();

		while (it!=head)
		{
			InsertHead(*it);
			--it;
		}

		InsertHead(*it);
	}
}

// ****************************************************************************************
//	Reverse
// ****************************************************************************************
template <class TType>
void TList<TType>::Reverse()
{
	Node* pNode =_pHead;
	while (pNode)
	{
		// Swap element
		Node* pTemp =pNode->GetPrev();
		pNode->SetPrev(pNode->GetNext());
		pNode->SetNext(pTemp);

		pNode =pNode->GetPrev();
	}

	_pHead =_pQueue;
	_pQueue  =_pHead;
}

// ****************************************************************************************
//	RemoveHead
// ****************************************************************************************
template <class TType>
void TList<TType>::RemoveHead()
{
	TAssert(GetSize() > 0);
	Node* pNodeToRemove = _pHead;
	_pHead = _pHead->GetNext();
	
	if (_pHead)
	{
		_pHead->SetPrev(NULL);
	}

	--_nSize;
	delete pNodeToRemove;
}

// ****************************************************************************************
//	InsertTail
// ****************************************************************************************
template <class TType>
void TList<TType>::InsertTail(const TType& Elt)
{
	PushTail(Elt);
}

// ****************************************************************************************
//	InsertTail
// ****************************************************************************************
template <class TType>
void TList<TType>::InsertTail(const TList& List)
{
	PushTail(List);
}

// ****************************************************************************************
//	operator +=
// ****************************************************************************************
template <class TType>
TList<TType>& TList<TType>::operator += (const TType& Elt)
{
	PushTail(Elt);
	return *this;
}

// ****************************************************************************************
//	operator +=
// ****************************************************************************************
template <class TType>
TList<TType>& TList<TType>::operator += (const TList<TType>& List)
{
	PushTail(List);
	return *this;
}

// ****************************************************************************************
//	operator =
// ****************************************************************************************
template <class TType>
TList<TType>& TList<TType>::operator = (const TList<TType>& List)
{
	Clear();
	PushTail(List);
	return *this;
}

// ****************************************************************************************
//	operator ==
// ****************************************************************************************
template <class TType>
Bool TList<TType>::operator == (const TList<TType>& List) const
{
	if (GetSize()!=List.GetSize())
	{
		return false;
	}

	Iterator it1 = GetHead();
	Iterator it2 = List.GetHead();
	Iterator itTail = GetTail();
	while (it1 != itTail)
	{
		if ((*it1 != *it2))
		{
			return false;
		}

		++it1;
		++it2;
	}

	return true;
}

// ****************************************************************************************
//	operator !=
// ****************************************************************************************
template <class TType>
Bool TList<TType>::operator != (const TList<TType>& List) const
{
	return ((*this)==List) == false;
}

// ****************************************************************************************
//	InsertSort
// ****************************************************************************************
template <class TType>
Bool TList<TType>::InsertSort(const TList& lOther, const Bool bAllowDup)
{
	Bool bIsIn =false;
	for (Iterator itCur =lOther.GetHead(); itCur!=lOther.GetTail(); ++itCur)
	{
		Bool bTmp;
		InsertSort(*itCur, bAllowDup, bTmp);
		bIsIn |= bTmp;
	}
	return bIsIn;
}

// ****************************************************************************************
//	Remove
// ****************************************************************************************
template <class TType>
Bool TList<TType>::Remove(const TType* pElt)
{
	FOR_EACH(*this, it, TList<TType>)
	{
		TType& t=*it;
		if (&t==pElt)
		{
			Remove(it);
			return true;
		}
	}

	return false;
}

// ****************************************************************************************
//	Remove
// ****************************************************************************************
template <class TType>
Bool TList<TType>::Remove(const TType& Elt)
{
	Iterator it =Find(Elt);
	if (it!= GetTail())
	{
		Remove(it);
		return true;
	}

	return false;
}

// ****************************************************************************************
//	Remove
// ****************************************************************************************
template <class TType>
void TList<TType>::Remove(const Iterator& it)
{
	TAssert(it.GetNode());
	TAssert(it.GetList() == this);

	if (it.GetNode()->GetPrev())
	{
		it.GetNode()->GetPrev()->SetNext(it.GetNode()->GetNext());
	}

	if (it.GetNode()->GetNext())
	{
		it.GetNode()->GetNext()->SetPrev(it.GetNode()->GetPrev());
	}

	if (it.GetNode() == _pHead)
	{
		_pHead = _pHead->GetNext();
	}

	if (it.GetNode() == _pQueue)
	{
		_pQueue = _pQueue->GetPrev();
	}

	delete it.GetNode();
	--_nSize;
}

// ****************************************************************************************
//	Sort (bubble sort)
// ****************************************************************************************
template <class TType>
void TList<TType>::Sort()
{
	if (GetSize() == 2)
	{
		if (_pHead->GetData() < _pQueue->GetData())
		{
			TType Temp = _pHead->GetData();
			_pHead->SetData(_pQueue->GetData());
			_pQueue->SetData(Temp);
		}
	}
	else
	{
		if (GetSize() > 2)
		{
			Iterator itX = GetHead();
			Iterator itY = itX;
			++itY;

			Iterator itTail = GetTail();
			Iterator itTailMinusOne = itTail;
			--itTailMinusOne;

			while (itX != itTail)
			{
				while (itY != itTailMinusOne)
				{
					Node * pNodeY = itY.GetNode();
					Node * pNodeYPlusOne = itY.GetNode()->GetNext();

					if (pNodeYPlusOne->GetData() < pNodeY->GetData())
					{
						TType holder = pNodeYPlusOne->GetData();	
						pNodeYPlusOne->SetData(pNodeY->GetData());
						pNodeY->SetData(holder);
					}

					++itY;
				}

				++itX;
			}
		}
	}
}

// ****************************************************************************************
//	Find
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::Find(const Iterator& itStart, const TType& Elt) const
{
	Iterator itCur = itStart;
	Iterator itEnd = GetTail();
	while (itCur != itEnd)
	{
		if ((*itCur) == Elt)
		{
			break;	
		}

		++itCur;
	}

	return itCur;
}

// ****************************************************************************************
//	Find
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::Find(const TType& Elt) const
{
	return Find(GetHead(), Elt);
}

// ****************************************************************************************
//	PushOnce
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::PushOnce(const TType& Elt)
{
	Iterator itCur = Find(Elt);
	if (itCur==GetTail())
	{
		PushTail(Elt);
		itCur = GetLast();
	}

	return itCur;
}

// ****************************************************************************************
//	Contains
// ****************************************************************************************
template <class TType>
Bool TList<TType>::Contains(const TType& Elt) const
{
	return Find(Elt)!=GetTail();
}

// ****************************************************************************************
//	Insert
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::Insert(const Iterator& it, const TType& Elt)
{
	if (it.GetNode() == NULL) // Tail!
	{
		PushTail(Elt);
		return GetLast();
	}

	Node * pNode = new Node(Elt);

	Node * pAfter = it.GetNode();
	Node * pBefore = it.GetNode()->GetPrev();

	if (pBefore)
	{
		pBefore->SetNext(pNode);
	}
	else
	{
		_pHead = pNode;
	}

	pAfter->SetPrev(pNode);

	pNode->SetPrev(pBefore);
	pNode->SetNext(pAfter);

	++_nSize;

	return Iterator(this, pNode);
}

// ****************************************************************************************
//	InsertOnce
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::InsertOnce(const Iterator& it, const TType& Elt)
{
	Iterator itCur = Find(Elt);
	if (itCur == GetTail())
	{
		itCur = Insert(it, Elt);
	}
	return itCur;
}

// ****************************************************************************************
//	InsertSort
// ****************************************************************************************
template <class TType>
typename TList<TType>::Iterator TList<TType>::InsertSort(const TType& Elt, const Bool bAllowDup, Bool& bIsIn)
{
	Iterator	itCur = GetHead();
	Iterator	itEnd = GetTail();

	bIsIn = false;

	while ( (itCur != itEnd) && (*itCur < Elt) )
	{
		++itCur;
	}

	if (itCur != itEnd)
	{
		bIsIn = (*itCur == Elt);

		if	( (bAllowDup == false) && (bIsIn == true) )
		{
			return itCur;
		}
	}

	return Insert(itCur, Elt);
}
