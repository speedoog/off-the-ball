#ifndef RFXBASE_INCLUDED
#define RFXBASE_INCLUDED

#pragma once


// Platforms defines
// WIN32
// NGC
// PSX2
#if defined(WIN32)
	#define	_WIN32_WINNT		0x0500
	#include  "windows.h"
	#include <cassert>
	#pragma warning (disable: 4127 4100 4511 4512 4702 4189)
#elif defined (NGC)
	#include <dolphin/os.h>
#endif

#define RfxDelete(_Obj)						if (_Obj) {delete (_Obj);(_Obj)=NULL;}
#define RfxDeleteTab(_Tab)				if (_Tab) {delete [](_Tab);(_Tab)=NULL;}
template<typename T> inline void RfxClamp(T& _x, T _min_, T _max_)	{ _x = (_x<_min_  ? _min_ : _x<_max_ ? _x : _max_); }
template<typename T> inline void RfxClipMin(T& _x, T _min_)					{	_x = ((_x)<(_min_) ? (_min_) : (_x));	}
template<typename T> inline void RfxClipMax(T& _x, T _max_)					{ _x = ((_x)>(_max_) ? (_max_) : (_x));	}
template<typename T> inline void RfxMinMax (const T& a, const T& b, T& min, T& max)	{ if (a<b) { min=a; max=b;} else { max=a;min=b;}}

#if defined (NGC)
	#define RfxMsg(a)							OSReport (a)
	#define RfxMsg2(a,b)					OSReport (a,b)
	#define RfxMsg3(a,b,c)				OSReport (a,b,c)
	#define RfxMsg4(a,b,c,d)			OSReport (a,b,c,d)
	#define RfxMsg5(a,b,c,d,e)		OSReport (a,b,c,d,e)
#elif defined (WIN32)
	void	Win32OSReport(char*	string,...);
  #define RfxMsg(a)							Win32OSReport (a)
	#define RfxMsg2(a, b)					Win32OSReport (a,b)
	#define RfxMsg3(a,b,c)				Win32OSReport (a,b,c)
  #define RfxMsg4(a,b,c,d)			Win32OSReport (a,b,c,d)
  #define RfxMsg5(a,b,c,d,e)	  Win32OSReport (a,b,c,d,e)
#endif

#ifdef NDEBUG
  #define RfxAssert(a)      			{}
  #define RfxDebug(a)       			{}
  #define RfxDebug2(a, b)     		{}
  #define RfxDebug3(a,b,c)    		{}
  #define RfxDebug4(a,b,c,d)   	  {}
  #define RfxDebug5(a,b,c,d,e)  	{}
  #define RfxTry(a)       				a 
#else
	#if defined(NGC)
		void	RfxAssertGC (bool cond, char* msg, const char* file, int line);
    #define RfxAssert(a)          RfxAssertGC (a, #a, __FILE__, __LINE__)    
  #elif defined (WIN32)  
		void	RfxAssertPC (char* msg, const char* file, int line);
    #define RfxAssert(a)          if	(!(a))	RfxAssertPC (#a, __FILE__, __LINE__)    
  #endif
    #define RfxDebug(a)           RfxMsg (a)
    #define RfxDebug2(a, b)       RfxMsg2 (a,b)
    #define RfxDebug3(a,b,c)      RfxMsg3 (a,b,c)
    #define RfxDebug4(a,b,c,d)    RfxMsg4 (a,b,c,d)
    #define RfxDebug5(a,b,c,d,e)  RfxMsg5 (a,b,c,d,e)
    #define RfxTry(a)             RfxAssert (a)
#endif

static const char* RFX_BUILD = "Build : "__DATE__" "__TIME__;

#endif
