

#include "TList.h"


class TList_Test
{
public:
	TList_Test();
};

static TList_Test _TList_Test;

TList_Test::TList_Test()
{
	typedef TList<int>	IntList;

	IntList lInt;
	lInt.PushTail(0);
	lInt.PushTail(1);
	lInt.PushTail(5);

	lInt.PushOnce(6);
	lInt.PushOnce(6);

	IntList::Iterator itFind =lInt.Find(1);

}
