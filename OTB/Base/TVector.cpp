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

#include "TVector.h"


class TVector_Test
{
public:
	TVector_Test();
};

static TVector_Test _TVector_Test;

TVector_Test::TVector_Test()
{
	typedef TVector<Int32>	IntVector;

	IntVector lInt;
	lInt.PushTail(0);
	lInt.PushTail(1);
	lInt.PushTail(5);

	lInt.PushOnce(6);
	lInt.PushOnce(6);

	lInt.Swap(0,2);
	lInt.Sort();

	IntVector::Iterator itFind =lInt.Find(1);

	IntVector lInt2(50, 12);
	lInt2.Clear();

	IntVector lInt3(lInt);
	lInt2.ClearMemory();

	// array of pointers
	typedef TVector<Int32*>	IntPtrVector;
	IntPtrVector lpInt;
	for(UInt32 i=0; i<10; ++i)
	{
		Int32* pInt =new Int32;
		*pInt =i;
		lpInt.PushTail(pInt);
	}
	lpInt.ClearAll();

}
