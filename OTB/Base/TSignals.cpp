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

#include "TSignals.h"

//
// *********************** USAGE SAMPLES *************************
//
class SENDER
{
public:
	SIGNAL_DECLARE_SENDER(MySignal, int, float, int&);

	void TestMe(const char* sTestName)
	{
		int out=0;
		TPrintMessage("SEND- %s", sTestName);
		SIGNAL_SEND(MySignal, 13, 5.12f, out);
		TPrintMessage("Result : %d\n", out);
	}

	SIGNAL_DECLARE_SENDER(Tester0);
	void TestVirtual()
	{
		TPrintMessage("Virtual test");
		SIGNAL_SEND(Tester0);
	}

	SIGNAL_DECLARE_SENDER(DeferredTester, int, float, bool);
	void TestDeferred()
	{
		TPrintMessage("Push Deferred test");
		SIGNAL_SEND(Tester0);
		SIGNAL_SEND(DeferredTester, 0,  3.14f, false);
		SIGNAL_SEND(DeferredTester, 7,  1.0f,  true);
		SIGNAL_SEND(DeferredTester, 9, -4.0f,  false);
	}
};

class RECEIVER1
{
public:
	virtual ~RECEIVER1() { }
	SIGNAL_DECLARE_RECEIVER(RECEIVER1, Add, int, float, int&);
	SIGNAL_DECLARE_RECEIVER(RECEIVER1, Mul, int, float, int&);

	void Add(int a, float b, int& out)
	{
		SIGNAL_RECEIVER_DISCONNECT_ALL(this, Add);		// try disconnect during call
		out =a+int(b);
		TPrintMessage("Add: %d", out);
	}

	void Mul(int a, float b, int& out)
	{
		out =a*int(b);
		TPrintMessage("this =%x Mul: %d", (IntPtr)this, out);
	}

	SIGNAL_DECLARE_RECEIVER(RECEIVER1, MyVirtual);

	virtual void MyVirtual()
	{
		TPrintMessage("virtual RECEIVER1");
	}
};

class RECEIVER2 : public RECEIVER1
{
public:
	void MyVirtual()
	{
		TPrintMessage("virtual RECEIVER2");
	}
};

class RECEIVER_ECHO
{
public:
	SIGNAL_DECLARE_RECEIVER(RECEIVER_ECHO, InBox, int, float, int&);
	SIGNAL_DECLARE_SENDER(OutBox, int, float, int&)

	void InBox(int a, float b, int& out)
	{
		TPrintMessage("ECHO a=%d, b=%f", a, b);
		SIGNAL_SEND(OutBox, a,b,out);
	}
};

class RECEIVER_DEFERRED
{
public:
	SIGNAL_DECLARE_RECEIVER_DEFERRED(RECEIVER_DEFERRED, Deferred0);
	void Deferred0()
	{
		TPrintMessage("Deferred 0 call");
	}

	SIGNAL_DECLARE_RECEIVER_DEFERRED(RECEIVER_DEFERRED, Deferred, int, float, bool);
	void Deferred(int a, float b, bool c)
	{
		TPrintMessage("Deferred call (%d %f %s)", a, b, c?"true":"false");
	}

	void PopDeferred()
	{
		TPrintMessage("Pop Deferred test");
		SIGNAL_POP_DEFERRED(Deferred0);
		SIGNAL_POP_DEFERRED(Deferred);
	};
};

void TEST_SIGNALS::TestMe()
{
	{
		SENDER		sender1;
		RECEIVER1	receiver1;

		// Test Simple
		SIGNAL_CONNECT(&sender1, MySignal, &receiver1, Add);
		sender1.TestMe("Test a simple connexion, call Add");

		// Test 1 Sender -> 2 receivers (on same object)
		SIGNAL_CONNECT(&sender1, MySignal, &receiver1, Mul);
		sender1.TestMe("Test 1 Sender -> 2 receivers (on same object)");

		// Test disconnect
		SIGNAL_DISCONNECT(&sender1, MySignal, &receiver1, Add);
		sender1.TestMe("Test disconnect (Add disconnection)");

		{
			// Test 1 Sender -> 2 differents receivers but same method
			RECEIVER1	receiver2;
			SIGNAL_CONNECT(&sender1, MySignal, &receiver2, Mul);
			sender1.TestMe("Test 1 Sender -> 2 differents receivers but same method");
		}

		// Scoped delete receiver2 destroy
		SIGNAL_DISCONNECT(&sender1, MySignal, &receiver1, Mul);
		sender1.TestMe("Test scoped deletion");

		{
			// test tunneling
			RECEIVER_ECHO	receiverEcho;
			SIGNAL_CONNECT(&sender1, MySignal, &receiverEcho, InBox);
			SIGNAL_CONNECT(&receiverEcho, OutBox, &receiver1, Add);
			sender1.TestMe("Test a simple tunneling (sender -> transciever -> receiver)");
		}

		// Test 2 signals connected to One receiver
		{
			SENDER	  sender2;
			RECEIVER1 receiver1;
			SIGNAL_CONNECT(&sender1, MySignal, &receiver1, Add);
			SIGNAL_CONNECT(&sender2, MySignal, &receiver1, Add);
			sender1.TestMe("Test 2 signals connected to One receiver - part 1");
			sender2.TestMe("Test 2 signals connected to One receiver - part 2");
		}
	}

	// Test Vcall
	{
		SENDER		sender1;

		RECEIVER1*	pReceiver1 =new RECEIVER1;		// create RECEIVER1
		RECEIVER1*	pReceiver2 =new RECEIVER2;		// create RECEIVER2

		SIGNAL_CONNECT(&sender1, Tester0, pReceiver1, MyVirtual);
		SIGNAL_CONNECT(&sender1, Tester0, pReceiver2, MyVirtual);

		sender1.TestVirtual();

		delete pReceiver1;
		delete pReceiver2;
	}

	// Test Deferred
	{
		SENDER				sender1;
		RECEIVER_DEFERRED	receiverDef;
		SIGNAL_CONNECT(&sender1, Tester0, &receiverDef, Deferred0);
		SIGNAL_CONNECT(&sender1, DeferredTester, &receiverDef, Deferred);

		sender1.TestDeferred();

		receiverDef.PopDeferred();
	}

	// Test copy
	{
		SENDER		sender1;
		RECEIVER1	receiver1;

		SIGNAL_CONNECT(&sender1, MySignal, &receiver1, Mul);

		// Copy Ctor receiver
		{
			RECEIVER1	receiver2=receiver1;
			sender1.TestMe("Test copy receiver, call Mul");
		}

		// Copy receiver
		{
			RECEIVER1	receiver2;
			receiver2=receiver1;
			sender1.TestMe("Test copy receiver, call Mul");
		}

		// Copy Sender
		{
			SENDER sender2=sender1;
			sender2.TestMe("Test copy sender, call Mul");
		}
	}

	// Test Array
// 	TVector<RECEIVER1> aRec;
// 	RECEIVER1 recpush;
// 	aRec.PushTail(recpush);

	TPrintMessage("sizeof(SENDER)=%d",				sizeof(SENDER));
	TPrintMessage("sizeof(RECEIVER1)=%d",			sizeof(RECEIVER1));
	TPrintMessage("sizeof(RECEIVER_ECHO)=%d",		sizeof(RECEIVER_ECHO));
	TPrintMessage("sizeof(RECEIVER2)=%d",			sizeof(RECEIVER2));
	TPrintMessage("sizeof(RECEIVER_DEFERRED)=%d",	sizeof(RECEIVER_DEFERRED));
}
