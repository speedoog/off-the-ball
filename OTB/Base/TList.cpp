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

	Int32 nSum =0;
	FOR_EACH_ELEMENT_OF_DLIST(lInt, it, IntList)
	{
		nSum +=*it;
	}

	IntList lInt2;
	lInt2 +=19;
	lInt2 +=78;
	lInt2 +=12;

	IntList lInt3(lInt2);
	lInt3 +=lInt;

	// another test

	class My_test
	{
	public:
		My_test(int t, int v):_t(t), _v(v) {}
		int _t,_v;
	};

	typedef TList<My_test> TestList;
	TestList lTest;
	/*My_test* pTest =*/lTest.PushTail(My_test(0,1));

	int break_here=0;
}
