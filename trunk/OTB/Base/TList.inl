//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================

// ********************************************
//	Ctor
// ********************************************
template <class TType>
TList<TType>::TList()
: _pFirst	(NULL)
, _pLast	(NULL)
, _nSize	(0)
{
}

// ********************************************
//	Ctor
// ********************************************
template <class TType>
TList<TType>::TList(const TList& List)
: _pFirst(NULL)
, _pLast(NULL)
, _nSize(0)
{
	PushTail(List);
}

// ********************************************
//	Dtor
// ********************************************
template <class TType>
TList<TType>::~TList()
{
	Clear();
}

// ********************************************
//	GetSize
// ********************************************
template <class TType>
UInt32 TList<TType>::GetSize() const
{
	return _nSize;
}

// ********************************************
//	GetMemoryUsed
// ********************************************
template <class TType>
UInt32 TList<TType>::GetMemoryUsed() const
{
	UInt32 nMem =0;
 	nMem +=sizeof(TList<TType>);
 	nMem +=GetSize()*sizeof(Node);
	return nMem;
}

// ********************************************
//	Clear
// ********************************************
template <class TType>
void TList<TType>::Clear()
{
	Node* pNode =_pFirst;
	while (pNode)
	{
		Node* pNext =pNode->GetNext();
		delete pNode;
		pNode =pNext;
	}

	_pFirst =NULL;
	_pLast  =NULL;
	_nSize	=0;
}

// ********************************************
//	ClearAll
// ********************************************
template <class TType>
void TList<TType>::ClearAll()
{
	Node* pNode =_pFirst;
	while (pNode)
	{
		Node* pNext =pNode->GetNext();
		delete pNode->GetData();				// Call delete DATA !
		delete pNode;
		pNode = pNext;
	}

	_pFirst =NULL;
	_pLast  =NULL;
	_nSize  =0;
}

// ********************************************
//	MoveToHead
// ********************************************
template <class TType>
void TList<TType>::MoveToHead(const Iterator& itElt)
{
	TAssert(itElt.GetNode());
	TAssert(itElt.GetList() == this);

	Node* pNodeToMove =itElt.GetNode();

	// Remove
	if (pNodeToMove == _pFirst)
		return;

	if (pNodeToMove->GetPrev())
	{
		pNodeToMove->GetPrev()->SetNext(pNodeToMove->GetNext());
	}

	if (pNodeToMove->GetNext())
	{
		pNodeToMove->GetNext()->SetPrev(pNodeToMove->GetPrev());
	}

	if (pNodeToMove == _pLast)
	{
		_pLast = _pLast->GetPrev();
	}

	// Insert Head
	pNodeToMove->SetPrev(NULL);	
	pNodeToMove->SetNext(_pFirst);
	_pFirst->SetPrev(pNodeToMove);
	_pFirst = pNodeToMove;

	++_nSize;
}

// ********************************************
//	PushTail
// ********************************************
template <class TType>
void TList<TType>::PushTail(const TType& Elt)
{
	Node* pNode =new Node(Elt);
	if (_pFirst==NULL)
	{
		_pFirst =pNode;
		_pLast  =pNode;
	}
	else
	{
		_pLast->SetNext(pNode);
		pNode->SetPrev(_pLast);
		_pLast = pNode;
	}

	++_nSize;
}

// ********************************************
//	PushTail
// ********************************************
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

// ********************************************
//	PopTail
// ********************************************
template <class TType>
void TList<TType>::PopTail()
{
	TAssert(IsEmpty()==false);

	Node* pNode =_pLast;
	_pLast =pNode->GetPrev();
	delete pNode;
	--_nSize;

	if (_pLast)
	{
		_pLast->SetNext(NULL);
	}
	else
	{
		_pFirst =NULL;
	}
}

// ********************************************
//	IsEmpty
// ********************************************
template <class TType>
Bool TList<TType>::IsEmpty() const
{
	return (GetSize()==0);
}

// ********************************************
//	InsertHead
// ********************************************
template <class TType>
void TList<TType>::InsertHead(const TType& Elt)
{
	Node* pNode =new Node(Elt);
	if (_pFirst == NULL)
	{
		_pFirst =pNode;
		_pLast  =pNode;
	}
	else
	{
		pNode->SetNext(_pFirst);
		_pFirst->SetPrev(pNode);
		_pFirst =pNode;
	}

	++_nSize;
}

// ********************************************
//	InsertHead
// ********************************************
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

// ********************************************
//	Reverse
// ********************************************
template <class TType>
void TList<TType>::Reverse()
{
	Node* pNode =_pFirst;
	while (pNode)
	{
		// Swap element
		Node* pTemp =pNode->GetPrev();
		pNode->SetPrev(pNode->GetNext());
		pNode->SetNext(pTemp);

		pNode =pNode->GetPrev();
	}

	_pFirst =_pLast;
	_pLast  =_pFirst;
}

// ********************************************
//	RemoveHead
// ********************************************
template <class TType>
void TList<TType>::RemoveHead()
{
	TAssert(GetSize() > 0);
	Node* pNodeToRemove = _pFirst;
	_pFirst = _pFirst->GetNext();
	
	if (_pFirst)
	{
		_pFirst->SetPrev(NULL);
	}

	--_nSize;
	delete pNodeToRemove;
}

// ********************************************
//	InsertTail
// ********************************************
template <class TType>
void TList<TType>::InsertTail(const TType& Elt)
{
	PushTail(Elt);
}

// ********************************************
//	InsertTail
// ********************************************
template <class TType>
void TList<TType>::InsertTail(const TList& List)
{
	PushTail(List);
}

// ********************************************
//	operator+=
// ********************************************
template <class TType>
TList<TType>& TList<TType>::operator+=(const TType&	Elt)
{
	PushTail(Elt);
	return *this;
}

// ********************************************
//	operator+=
// ********************************************
template <class TType>
TList<TType>& TList<TType>::operator+=(const TList<TType>& List)
{
	PushTail(List);
	return *this;
}

// ********************************************
//	operator=
// ********************************************
template <class TType>
TList<TType>& TList<TType>::operator=(const TList<TType>& List)
{
	Clear();
	PushTail(List);
	return *this;
}

// ********************************************
//	InsertSort
// ********************************************
template <class TType>
Bool TList<TType>::InsertSort(const TList& lOther, Bool bAllowDup)
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

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class TType>
Bool TList<TType>::Remove(const TType& Elt)
{
	Iterator it;
	it = Find(GetHead(), Elt);
	if (it!= GetTail())
	{
		Remove(it);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class TType>
void TList<TType>::Remove(const Iterator& I)
{
	TAssert(I.GetNode());
	TAssert(I.GetList() == this);

	if (I.GetNode()->GetPrev())
	{
		I.GetNode()->GetPrev()->SetNext(I.GetNode()->GetNext());
	}

	if (I.GetNode()->GetNext())
	{
		I.GetNode()->GetNext()->SetPrev(I.GetNode()->GetPrev());
	}

	if (I.GetNode() == _pFirst)
	{
		_pFirst = _pFirst->GetNext();
	}

	if (I.GetNode() == _pLast)
	{
		_pLast = _pLast->GetPrev();
	}

	delete I.GetNode();
	--_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class TType>
Bool TList<TType>::operator == (const TList<TType>& List) const
{
	if (GetSize() != List.GetSize())
	{
		return (false);
	}

	Iterator it1 = GetHead();
	Iterator it2 = List.GetHead();
	Iterator itTail = GetTail();

	while (it1 != itTail)
	{
		if ((*it1 != *it2))
		{
			return (false);
		}

		++it1;
		++it2;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-14:	RMA - Created
//-----------------------------------------------------------------------------
template <class TType>
void TList<TType>::Push(const TType & Elt, 
						Bool bReverse)
{
	if (bReverse)
	{
		InsertHead(Elt);
	}
	else
	{
		PushTail(Elt);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-14:	RMA - Created
//				This is a common bubble sort... 
//				I suppose it can be greatly optimized!
//-----------------------------------------------------------------------------
template <class TType>
void TList<TType>::Sort()
{
	if (GetSize() == 2)
	{
		if (_pFirst->GetData() < _pLast->GetData())
		{
			TType Temp = _pFirst->GetData();
			_pFirst->SetData(_pLast->GetData());
			_pLast->SetData(Temp);
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

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-14:	RMA - Created
//				This is a common bubble sort... 
//				I suppose it can be greatly optimized!
//-----------------------------------------------------------------------------
template <class TType>
void TList<TType>::SortByPointers()
{
	if (GetSize() == 2)
	{
		if (*_pFirst->GetData() < *_pLast->GetData())
		{
			TType Temp = _pFirst->GetData();
			_pFirst->SetData(_pLast->GetData());
			_pLast->SetData(Temp);
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

					if (*pNodeYPlusOne->GetData() < *pNodeY->GetData())
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

template <class TType>
typename TList<TType>::Iterator TList<TType>::Find(const TType& Elt) const
{
	return Find(GetHead(), Elt);
}

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

template <class TType>
Bool TList<TType>::Contains(const TType& Elt) const
{
	return Find(Elt)!=GetTail();
}

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
		_pFirst = pNode;
	}

	pAfter->SetPrev(pNode);

	pNode->SetPrev(pBefore);
	pNode->SetNext(pAfter);

	++_nSize;

	return Iterator(this, pNode);
}

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

template <class TType>
typename TList<TType>::Iterator TList<TType>::InsertSort(const TType& Elt, Bool bAllowDup, Bool& bIsIn)
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

// Insère un élément dans la liste en le triant (necessite operateur < et == définit pour la classe template)
template <class TType>
typename TList<TType>::Iterator TList<TType>::InsertSortPointers(const TType& Elt, Bool bAllowDup, Bool& bIsIn)
{
	Iterator itCur =GetHead();
	Iterator itEnd =GetTail();

	bIsIn = false;

	while ( (itCur != itEnd) && (**itCur < *Elt) )
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

template <class TType>
typename TList<TType>::Iterator TList<TType>::InsertSortPointers(const TType& Elt, Bool bAllowDup, Bool& bIsIn, DLIST_COMPARISON_CALLBACK ComparisonCallback)
{
	Iterator itCur =GetHead();
	Iterator itEnd =GetTail();

	bIsIn = false;
	while ( (itCur != itEnd) && (*ComparisonCallback)(*itCur, Elt) )
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
