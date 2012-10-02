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


#ifndef __BASE_TSIGNALS_H__
#define __BASE_TSIGNALS_H__

#pragma once

#include "TList.h"
#include "TVector.h"

//
// SIGNALS USAGE:
//
// - Sender -
// SIGNAL_DECLARE_SENDER(Arrival, Float32, const VECTOR&, Bool&)
// SIGNAL_SEND(Arrival, rTime, vPosition, bReturn)
//
// - Receiver -
// SIGNAL_DECLARE_RECEIVER(CONTROLLER_AGENT_GOTO_ABC, OnArrival, Float32, const VECTOR&, Bool&))
// SIGNAL_CONNECT(pLocomotion, Arrival, this, OnArrival)
//
// vvv More Samples at the End of this file vvv
//
//
// TODO: 
//	 - Add Cyclic connexion check or at least a reentrancy check
//

// Imported macros from -https://forums.embarcadero.com/message.jspa?messageID=319300
// Special trick to allow variadic macros to be empty
#define UCL_PP_AND(a,b)				UCL_PP_CAT3(_UCL_PP_AND_, a, b)
#define UCL_PP_NOT(a)				UCL_PP_CAT2(_UCL_PP_NOT_, a)
#define UCL_PP_IF(a,b,...)			UCL_PP_CAT2(_UCL_PP_IF_, a)(b, __VA_ARGS__)
#define UCL_PP_EMPTY()
#define UCL_PP_EXPAND(...)			__VA_ARGS__
#define UCL_PP_1ST(a,...)			a
#define UCL_PP_2ND(a,b,...)			b
#define UCL_PP_REMOVE_1ST(a,...)	__VA_ARGS__
#define UCL_PP_PAIR(a,...)			a __VA_ARGS__
#define UCL_PP_CAT2(a,...)			UCL_PP_CAT2_(a, __VA_ARGS__)
#define UCL_PP_CAT3(a,b,...)		UCL_PP_CAT3_(a, b, __VA_ARGS__)

// The two macros below are inspired by Laurent Deniau's posting on comp.lang.c from 2006/09/27
// http://groups.google.com/group/comp.lang.c/browse_thread/thread/578912299f8f87ce#msg_937356effc43f569
#define UCL_PP_IS_EMPTY(...)		UCL_PP_AND(UCL_PP_IS_LIST(__VA_ARGS__ ()),UCL_PP_NOT(UCL_PP_IS_LIST(__VA_ARGS__ _)))
#define UCL_PP_IS_LIST(...)			UCL_PP_PAIR(UCL_PP_1ST, (UCL_PP_CAT2(UCL_PP_IS_LIST_RET_, UCL_PP_IS_LIST_TST_ __VA_ARGS__)))

// implementation details
#define UCL_PP_IS_LIST_TST_(...)	1
#define UCL_PP_IS_LIST_RET_UCL_PP_IS_LIST_TST_ 0,
#define UCL_PP_IS_LIST_RET_1		1,

#define UCL_PP_CAT2_(a,...)			UCL_PP_EXPAND(a ## __VA_ARGS__)
#define UCL_PP_CAT3_(a,b,...)		UCL_PP_EXPAND(a ## b ## __VA_ARGS__)

#define _UCL_PP_AND_00				0
#define _UCL_PP_AND_01				0
#define _UCL_PP_AND_10				0
#define _UCL_PP_AND_11				1

#define _UCL_PP_IF_0(a,...)			__VA_ARGS__
#define _UCL_PP_IF_1(a,...)			a

#define _UCL_PP_NOT_0 1
#define _UCL_PP_NOT_1 0

// Argument counter (used in DEFERRED receiver)
#define VA_NUM_ARGS(...)									UCL_PP_IF( UCL_PP_IS_EMPTY(__VA_ARGS__), 0, VA_NUM_ARGS_IMPL_((__VA_ARGS__, 8,7,6,5,4,3,2,1)) )
#define VA_NUM_ARGS_IMPL_(_tuple_)							VA_NUM_ARGS_IMPL _tuple_
#define VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N, ...)	N

//
// -------------------------- Internal Macros (error checking) -------------------------
//

#ifndef T_MASTER
#	define	SIGNAL_FORMAT_CHECK(_ptr_, _signal_, ...)		_ptr_->BAD_FORMAT_FOR_SIGNAL_##_signal_(__VA_ARGS__)
#else
#	define	SIGNAL_FORMAT_CHECK(_ptr_, _signal_, ...)
#endif

#ifndef T_MASTER
#	define	SIGNAL_FORMAT_INSURANCE(_signal_, ...)	static inline void BAD_FORMAT_FOR_SIGNAL_##_signal_(__VA_ARGS__)	{ };
#else
#	define	SIGNAL_FORMAT_INSURANCE(_signal_, ...)
#endif

#ifndef T_MASTER
#	define	SIGNAL_POINTER_CHECK(_ptr_)
#else
#	define	SIGNAL_POINTER_CHECK(_ptr_)				if (!(_ptr_)) {	TPrintError("Signal connexion error on ptr: %s", #_ptr_ ); } else
#endif

// macro helper if no args => no coma !
#define SIGNAL_CLEAN_ARGS(...)	UCL_PP_IF(UCL_PP_IS_EMPTY(__VA_ARGS__), UCL_PP_EMPTY(), ,) __VA_ARGS__

//
// ---------------------------------- User Macros Here ---------------------------------
//

// --------------------------- Declarations (usage in xxx.h) ---------------------------

#define SIGNAL_DECLARE_SENDER(_signal_, ...)																		\
	SIGNAL_FORMAT_INSURANCE(_signal_, __VA_ARGS__)																	\
	SIGNAL_SENDER<__VA_ARGS__> _Signal##_signal_;

#define SIGNAL_DECLARE_RECEIVER(_class_, _method_, ...)																\
	class SIGNAL_RECEIVER_##_class_##_method_ : public SIGNAL_RECEIVER												\
		<_class_ SIGNAL_CLEAN_ARGS(__VA_ARGS__)>																	\
	{																												\
	public:																											\
		inline SIGNAL_RECEIVER_##_class_##_method_()												{ Init();  }	\
		inline SIGNAL_RECEIVER_##_class_##_method_(const SIGNAL_RECEIVER_##_class_##_method_& _that_)				\
			:SIGNAL_RECEIVER<_class_ SIGNAL_CLEAN_ARGS(__VA_ARGS__)>(_that_)						{ Init();  }	\
		inline _class_*	GetClass() const			{ return GET_CONTAINER_OF(this, _class_, _Slot##_method_); }	\
		inline void Init()							{ SetReceiver(GetClass()); SetMethod(&_class_::_method_);  }	\
	};																												\
	SIGNAL_RECEIVER_##_class_##_method_	_Slot##_method_;

// - DEFERRED receiver -
#define SIGNAL_DECLARE_RECEIVER_DEFERRED(_class_, _method_, ...)													\
	class SIGNAL_RECEIVER_##_class_##_method_ : public SIGNAL_RECEIVER_DEFERRED										\
		<_class_ SIGNAL_CLEAN_ARGS(__VA_ARGS__)>																	\
	{																												\
	public:																											\
		inline SIGNAL_RECEIVER_##_class_##_method_()												{ Init();  }	\
		inline SIGNAL_RECEIVER_##_class_##_method_(const SIGNAL_RECEIVER_##_class_##_method_& _that_)				\
			:SIGNAL_RECEIVER_DEFERRED<_class_ SIGNAL_CLEAN_ARGS(__VA_ARGS__)>(_that_)				{ Init();  }	\
		inline _class_*	GetClass() const			{ return GET_CONTAINER_OF(this, _class_, _Slot##_method_); }	\
		inline void Init()							{ SetReceiver(GetClass()); SetMethod(&_class_::_method_);  }	\
		inline void PopDeferred()					{ T_JOIN(PopDeferred, VA_NUM_ARGS(__VA_ARGS__))();		   }	\
	};																												\
	SIGNAL_RECEIVER_##_class_##_method_	_Slot##_method_;

// ------------------------------------ Runtime usage ----------------------------------

#define SIGNAL_CONNECT(_sender_, _signal_, _receiver_, _method_)						\
	SIGNAL_POINTER_CHECK(_sender_)														\
	SIGNAL_POINTER_CHECK(_receiver_)													\
	(_sender_)->_Signal##_signal_.ConnectSlot(&(_receiver_)->_Slot##_method_);			\

#define SIGNAL_DISCONNECT(_sender_, _signal_, _receiver_, _method_)						\
	SIGNAL_POINTER_CHECK(_sender_)														\
	SIGNAL_POINTER_CHECK(_receiver_)													\
	(_sender_)->_Signal##_signal_.DisconnectSlot(&(_receiver_)->_Slot##_method_);		\

#define SIGNAL_RECEIVER_DISCONNECT_ALL(_receiver_, _method_)							\
	SIGNAL_POINTER_CHECK(_receiver_)													\
	(_receiver_)->_Slot##_method_.DisconnectAllSignals();

#define SIGNAL_SENDER_DISCONNECT_ALL(_sender_, _signal_)								\
	SIGNAL_POINTER_CHECK(_sender_)														\
	(_sender_)->_Signal##_signal_.DisconnectAllSlots();

#define SIGNAL_SEND(_signal_, ...)														\
	SIGNAL_FORMAT_CHECK(this, _signal_, __VA_ARGS__);									\
	_Signal##_signal_.Call(__VA_ARGS__);

#define SIGNAL_SEND_EX(_ptr_, _signal_, ...)											\
	SIGNAL_FORMAT_CHECK(_ptr_, _signal_, __VA_ARGS__);									\
	(_ptr_)->_Signal##_signal_.Call(__VA_ARGS__);

#define SIGNAL_POP_DEFERRED(_method_)													\
	_Slot##_method_.PopDeferred()


#define	SIGNAL_ARG_LIST				TArg0, TArg1, TArg2, TArg3
#define	SIGNAL_ARG_LIST_CLASS		class TArg0, class TArg1, class TArg2, class TArg3
#define SIGNAL_ARG_LIST_CLASS_INI	class TArg0=void*, class TArg1=void*, class TArg2=void*, class TArg3=void*
#define SIGNAL_SETMETHOD(_n_)		void SetMethod(METHOD_TYPE##_n_ V##_n_)	{ _Method.V##_n_ =V##_n_; }
#define SIGNAL_FOR_EACH_SLOT		FOR_EACH_ELEMENT_OF_DLIST_SAFE(_lpSlots, it, SLOT_LIST) reinterpret_cast<SLOT_CALL*>(*it)

		template<SIGNAL_ARG_LIST_CLASS_INI>
		class SIGNAL_SENDER;

		template<SIGNAL_ARG_LIST_CLASS_INI>
		class SIGNAL_CALL
		{
		public:
			typedef SIGNAL_SENDER<SIGNAL_ARG_LIST>	SIGNAL_SENDER_TYPE;
			typedef TList<void*>					SIGNAL_LIST;

			inline	SIGNAL_CALL()		{ }
			virtual	~SIGNAL_CALL()		{ DisconnectAllSignals(); }

			// Copy
			inline	SIGNAL_CALL(const SIGNAL_CALL& SignalCall)
			{
				Copy(SignalCall);
			}
			inline	SIGNAL_CALL& operator= (const SIGNAL_CALL& SignalCall)
			{
				return Copy(SignalCall);
			}
			inline	SIGNAL_CALL& Copy(const SIGNAL_CALL& SignalCall)
			{
				DisconnectAllSignals();
				FOR_EACH(SignalCall._lpSignals, itSignalSender, SIGNAL_LIST)
				{
					SIGNAL_SENDER_TYPE* pSender =reinterpret_cast<SIGNAL_SENDER_TYPE*>(*itSignalSender);
					pSender->ConnectSlot(this);
				}
				return *this;
			}

			inline void DisconnectAllSignals()
			{
				SIGNAL_LIST::Iterator it=_lpSignals.GetHead();
				while(it!=_lpSignals.GetTail())
				{
					reinterpret_cast<SIGNAL_SENDER_TYPE*>(*it)->DisconnectSlot(this);
					it =_lpSignals.GetHead();
				}
			}

			virtual void OnCall()=0;
			virtual void OnCall(TArg0)=0;
			virtual void OnCall(TArg0, TArg1)=0;
			virtual void OnCall(TArg0, TArg1, TArg2)=0;
			virtual void OnCall(TArg0, TArg1, TArg2, TArg3)=0;

		private:
			friend class SIGNAL_SENDER<SIGNAL_ARG_LIST>;
			inline void	RegisterSignal(SIGNAL_SENDER_TYPE* pSignal)
			{
				TAssert(_lpSignals.Find(_lpSignals.GetHead(), pSignal)==_lpSignals.GetTail());
				_lpSignals.PushTail(pSignal);
			}
			inline void	UnregisterSignal(SIGNAL_SENDER_TYPE* pSignal)
			{
				TAssert(_lpSignals.Find(_lpSignals.GetHead(), pSignal)!=_lpSignals.GetTail());
				_lpSignals.Remove(pSignal);
			}

		protected:
			SIGNAL_LIST _lpSignals;
		};

		template <class TReceiver, SIGNAL_ARG_LIST_CLASS_INI>
		class SIGNAL_RECEIVER : public SIGNAL_CALL<SIGNAL_ARG_LIST>
		{
		public:
			inline	SIGNAL_RECEIVER():SIGNAL_CALL<SIGNAL_ARG_LIST>() { }

			// Copy
			inline	SIGNAL_RECEIVER(const SIGNAL_RECEIVER& SignalReceiver) : SIGNAL_CALL<SIGNAL_ARG_LIST>(SignalReceiver)   { }
			inline	SIGNAL_RECEIVER& operator= (const SIGNAL_RECEIVER& SignalReceiver)			{ return static_cast<SIGNAL_RECEIVER&>(Copy(SignalReceiver)); }

			inline void	SetReceiver(TReceiver* pReceiver)							{ _pReceiver=pReceiver;	}

			typedef void (TReceiver::*METHOD_TYPE0)();
			typedef void (TReceiver::*METHOD_TYPE1)(TArg0);
			typedef void (TReceiver::*METHOD_TYPE2)(TArg0, TArg1);
			typedef void (TReceiver::*METHOD_TYPE3)(TArg0, TArg1, TArg2);
			typedef void (TReceiver::*METHOD_TYPE4)(TArg0, TArg1, TArg2, TArg3);

			SIGNAL_SETMETHOD(0);		// no params
			SIGNAL_SETMETHOD(1);		// 1 parameter
			SIGNAL_SETMETHOD(2);		// etc ...
			SIGNAL_SETMETHOD(3);
			SIGNAL_SETMETHOD(4);

			// overload SIGNAL_CALL virtuals
			virtual void OnCall()													{ (_pReceiver->*_Method.V0)();								}
			virtual void OnCall(TArg0 a0)											{ (_pReceiver->*_Method.V1)(a0);							}
			virtual void OnCall(TArg0 a0, TArg1 a1)									{ (_pReceiver->*_Method.V2)(a0, a1);						}
			virtual void OnCall(TArg0 a0, TArg1 a1, TArg2 a2)						{ (_pReceiver->*_Method.V3)(a0, a1, a2);					}
			virtual void OnCall(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3)				{ (_pReceiver->*_Method.V4)(a0, a1, a2, a3);				}

		protected:
			TReceiver* _pReceiver;
			union
			{
				METHOD_TYPE0 V0;		// no params
				METHOD_TYPE1 V1;		// 1 parameter
				METHOD_TYPE2 V2;		// etc ...
				METHOD_TYPE3 V3;
				METHOD_TYPE4 V4;
			} _Method;
		};

		template <class TReceiver, SIGNAL_ARG_LIST_CLASS_INI>
		class SIGNAL_RECEIVER_DEFERRED : public SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>
		{
		public:
			class ARG_PACK	
			{
			public:
				ARG_PACK()																													{	}
				ARG_PACK(TArg0 a0)													:_a0(a0)												{	}
				ARG_PACK(TArg0 a0, TArg1 a1)										:_a0(a0), _a1(a1)										{	}
				ARG_PACK(TArg0 a0, TArg1 a1, TArg2 a2)								:_a0(a0), _a1(a1), _a2(a2)								{	}
				ARG_PACK(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3)					:_a0(a0), _a1(a1), _a2(a2), _a3(a3)						{	}

				TArg0 _a0;
				TArg1 _a1;
				TArg2 _a2;
				TArg3 _a3;
			};

			// Copy
//			inline	SIGNAL_RECEIVER_DEFERRED() : SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>()									{ }
// 			inline	SIGNAL_RECEIVER_DEFERRED(const SIGNAL_RECEIVER_DEFERRED& SignalReceiver) : SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>(SignalReceiver)  { 		 }
// 			inline	SIGNAL_RECEIVER_DEFERRED& operator= (const SIGNAL_RECEIVER_DEFERRED& SignalReceiver)						{ return Copy(SignalReceiver); }

			typedef TVector<ARG_PACK*>	ARG_PACK_VECTOR;

			virtual void OnCall()													{ _aArgs.PushTail(new ARG_PACK());							}
			virtual void OnCall(TArg0 a0)											{ _aArgs.PushTail(new ARG_PACK(a0));						}
			virtual void OnCall(TArg0 a0, TArg1 a1)									{ _aArgs.PushTail(new ARG_PACK(a0, a1));					}
			virtual void OnCall(TArg0 a0, TArg1 a1, TArg2 a2)						{ _aArgs.PushTail(new ARG_PACK(a0, a1, a2));				}
			virtual void OnCall(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3)				{ _aArgs.PushTail(new ARG_PACK(a0, a1, a2, a3));			}

			inline void PopDeferred0()
			{
				for(UInt32 i=0; i<_aArgs.GetSize(); ++i)
				{
					ARG_PACK* pArgPack =_aArgs[i];
					SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>::OnCall();
				}
				_aArgs.ClearAll();
			}

			inline void PopDeferred1()
			{
				for(UInt32 i=0; i<_aArgs.GetSize(); ++i)
				{
					ARG_PACK* pArgPack =_aArgs[i];
					SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>::OnCall(pArgPack->_a0);
				}
				_aArgs.ClearAll();
			}

			inline void PopDeferred2()
			{
				for(UInt32 i=0; i<_aArgs.GetSize(); ++i)
				{
					ARG_PACK* pArgPack =_aArgs[i];
					SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>::OnCall(pArgPack->_a0, pArgPack->_a1);
				}
				_aArgs.ClearAll();
			}

			inline void PopDeferred3()
			{
				for(UInt32 i=0; i<_aArgs.GetSize(); ++i)
				{
					ARG_PACK* pArgPack =_aArgs[i];
					SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>::OnCall(pArgPack->_a0, pArgPack->_a1, pArgPack->_a2);
				}
				_aArgs.ClearAll();
			}

			inline void PopDeferred4()
			{
				for(UInt32 i=0; i<_aArgs.GetSize(); ++i)
				{
					ARG_PACK* pArgPack =_aArgs[i];
					SIGNAL_RECEIVER<TReceiver, SIGNAL_ARG_LIST>::OnCall(pArgPack->_a0, pArgPack->_a1, pArgPack->_a2, pArgPack->_a3);
				}
				_aArgs.ClearAll();
			}

			ARG_PACK_VECTOR	_aArgs;
		};

		template <SIGNAL_ARG_LIST_CLASS>
		class SIGNAL_SENDER
		{
		public:
			typedef SIGNAL_CALL<SIGNAL_ARG_LIST>	SLOT_CALL;
			typedef TList<void*>					SLOT_LIST;

			inline 		SIGNAL_SENDER()		{ }
			inline 		~SIGNAL_SENDER()	{ DisconnectAllSlots(); }

			// Copy
			inline					SIGNAL_SENDER(const SIGNAL_SENDER& SignalSender)	{ Copy(SignalSender);		 }
			inline	SIGNAL_SENDER&	operator= (const SIGNAL_SENDER& SignalSender)		{ return Copy(SignalSender); }
			inline	SIGNAL_SENDER&	Copy(const SIGNAL_SENDER& SignalSender)
			{
				DisconnectAllSlots();
				FOR_EACH(SignalSender._lpSlots, itSlot, SLOT_LIST)
				{
					SLOT_CALL* pCall =reinterpret_cast<SLOT_CALL*>(*itSlot);
					ConnectSlot(pCall);
				}
				return *this;
			}

			inline void ConnectSlot(SLOT_CALL* pSlot)
			{
				SLOT_LIST::Iterator it =_lpSlots.Find(_lpSlots.GetHead(), reinterpret_cast<void*>(pSlot));
				if (it==_lpSlots.GetTail())
				{	// Not found
					pSlot->RegisterSignal(this);
					_lpSlots.PushTail(pSlot);
				}
			}

			inline void DisconnectSlot(SLOT_CALL* pSlot)
			{
				SLOT_LIST::Iterator it =_lpSlots.Find(_lpSlots.GetHead(), pSlot);
				if (it!=_lpSlots.GetTail())
				{	// found
					pSlot->UnregisterSignal(this);
					_lpSlots.Remove(it);
				}
			}

			inline void DisconnectAllSlots()
			{
				SLOT_LIST::Iterator it=_lpSlots.GetHead();
				while(it!=_lpSlots.GetTail())
				{
					DisconnectSlot(reinterpret_cast<SLOT_CALL*>(*it));
					it =_lpSlots.GetHead();
				}
			}

			inline void Call()														{ SIGNAL_FOR_EACH_SLOT->OnCall();								}
			inline void Call(TArg0 a0)												{ SIGNAL_FOR_EACH_SLOT->OnCall(a0);								}
			inline void Call(TArg0 a0, TArg1 a1)									{ SIGNAL_FOR_EACH_SLOT->OnCall(a0, a1);							}
			inline void Call(TArg0 a0, TArg1 a1, TArg2 a2)							{ SIGNAL_FOR_EACH_SLOT->OnCall(a0, a1, a2);						}
			inline void Call(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3)				{ SIGNAL_FOR_EACH_SLOT->OnCall(a0, a1, a2, a3);					}

		protected:
			SLOT_LIST	_lpSlots;
		};

		// Signal System Tester (see .cpp)
		class TEST_SIGNALS
		{
		public:
			static void TestMe();
		};

// cleanup internal macros
#undef SIGNAL_ARG_LIST
#undef SIGNAL_ARG_LIST_CLASS
#undef SIGNAL_ARG_LIST_CLASS_INI
#undef SIGNAL_SETMETHOD
#undef SIGNAL_FOR_EACH_SLOT

#endif //__BASE_TSIGNALS_H__
