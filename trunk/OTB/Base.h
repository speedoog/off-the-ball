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

#ifndef __BASE_H__
#define __BASE_H__

#pragma once

#include "../hge/hge.h"
#include "../hge/hgevector.h"
extern HGE* hge;

/*
#if defined(WIN32)
#	ifndef _WIN32_WINNT
#		define	_WIN32_WINNT		0x0500
#	endif
#	include  "windows.h"
#	include <cassert>
#endif
*/

typedef char				Char;
typedef char				Int8;
typedef short				Int16;
typedef int					Int32;
typedef __int64				Int64;
typedef unsigned char		UInt8;
typedef unsigned char		UChar;
typedef unsigned short		UInt16;
typedef unsigned int		UInt32;
typedef unsigned __int64	UInt64;
typedef float				Float32;
typedef double				Float64;
typedef bool				Bool;
typedef UInt32				IntPtr;


#define RfxDelete(_Obj)						if (_Obj) {delete (_Obj);(_Obj)=NULL;}
#define RfxDeleteTab(_Tab)					if (_Tab) {delete [](_Tab);(_Tab)=NULL;}
#define RfxRelease(p)						{ if(p) { (p)->Release(); (p)=NULL; } }

// new override / placement

inline void* operator new(size_t, void *_P)	{return (_P); }	// new with placement

#define	OTB_NEW_PLACED(_ptr_, _type_)		new (_ptr_) _type_
#define	OTB_DELETE_PLACED(_ptr_, _type_)	(_ptr_)->_type_::~_type_()


template<typename T> inline T		RfxClamp(const T& _x, const T& _min_, const T& _max_)	{ return (_x<_min_ ? _min_ : _x<_max_ ? _x : _max_);	}
template<typename T> inline void 	RfxClip(T& _x, const T& _min_, const T& _max_)			{ _x = RfxClamp(_x, _min_, _max_);						}
template<typename T> inline void 	RfxClipMin(T& _x, T _min_)								{ _x = ((_x)<(_min_) ? (_min_) : (_x));					}
template<typename T> inline void 	RfxClipMax(T& _x, T _max_)								{ _x = ((_x)>(_max_) ? (_max_) : (_x));					}
template<typename T> inline void 	RfxMinMax (const T& a, const T& b, T& min, T& max)	{ if (a<b) { min=a; max=b;} else { max=a;min=b;}}

#ifdef WIN32
	void	Win32OSReport(char*	string,...);
#	define RfxMsg(a)							Win32OSReport (a)
#	define RfxMsg2(a, b)					Win32OSReport (a,b)
#	define RfxMsg3(a,b,c)				Win32OSReport (a,b,c)
#	define RfxMsg4(a,b,c,d)			Win32OSReport (a,b,c,d)
#	define RfxMsg5(a,b,c,d,e)	  Win32OSReport (a,b,c,d,e)
#endif	//WIN32

#ifdef NDEBUG
#	define RfxAssert(a)      		{}
#	define RfxDebug(a)       		{}
#	define RfxDebug2(a, b)     		{}
#	define RfxDebug3(a,b,c)    		{}
#	define RfxDebug4(a,b,c,d)   	{}
#	define RfxDebug5(a,b,c,d,e)  	{}
#	define RfxTry(a)       			a 
#else
	void	RfxAssertPC (char* msg, const char* file, int line);
#	define RfxAssert(a)          if	(!(a))	RfxAssertPC (#a, __FILE__, __LINE__)    
#	define RfxDebug(a)           RfxMsg (a)
#	define RfxDebug2(a, b)       RfxMsg2 (a,b)
#	define RfxDebug3(a,b,c)      RfxMsg3 (a,b,c)
#	define RfxDebug4(a,b,c,d)    RfxMsg4 (a,b,c,d)
#	define RfxDebug5(a,b,c,d,e)  RfxMsg5 (a,b,c,d,e)
#	define RfxTry(a)             RfxAssert (a)
#endif

static const char* UID_BUILD = "Build : "__DATE__" "__TIME__;

#endif	//__BASE_H__
