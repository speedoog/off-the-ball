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

#include "../../hge/hge.h"
#include "../../hge/hgevector.h"


// -------------------------------------------- global HGE --------------------------------------------

extern HGE* hge;

// -------------------------------------------- Base types --------------------------------------------

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

// -------------------------------------------- new / delete --------------------------------------------

#define TDelete(_Obj)					if (_Obj) {delete (_Obj);(_Obj)=NULL;}
#define TDeleteTab(_Tab)				if (_Tab) {delete [](_Tab);(_Tab)=NULL;}
#define TRelease(p)						{ if(p) { (p)->Release(); (p)=NULL; } }

// new override / placement

inline void* operator new(size_t, void *_P)	{return (_P); }	// new with placement

#define	TNewPlaced(_ptr_, _type_)		new (_ptr_) _type_
#define	TDeletePlaced(_ptr_, _type_)	(_ptr_)->_type_::~_type_()

inline static void TMemMove(void* dest, const void* src, UInt32 nSize)
{
	UInt8* d		=(UInt8*)dest;
	const UInt8* s	=(UInt8*)src;
	if (s<d && d<(s+nSize))			// Test for overlap that prevents an ascending copy
	{
		s += nSize;					// copy backwards (descending copy)
		d += nSize;
		while (nSize--)
			*--d = *--s;
	}
	else
	{
		while (nSize--)				// Forward copy
			*d++ = *s++;
	}
}

// -------------------------------------------- clip / clamp / Min / Max --------------------------------------------

template<typename T>
inline T TClamp(const T& _x, const T& _min_, const T& _max_)
{
	return (_x<_min_ ? _min_ : _x<_max_ ? _x : _max_);
}

template<typename T>
inline void TClip(T& _x, const T& _min_, const T& _max_)
{
	_x = TClamp(_x, _min_, _max_);
}

template<typename T>
inline void TClipMin(T& _x, T _min_)
{
	_x = ((_x)<(_min_) ? (_min_) : (_x));
}

template<typename T>
inline void TClipMax(T& _x, T _max_)
{
	_x = ((_x)>(_max_) ? (_max_) : (_x));
}

template <typename T>
inline void TSwap(T& a, T& b)
{
	T c(a); a=b; b=c;
}

// -------------------------------------------- Assert --------------------------------------------

void Win32OSReport(char* pFormatedMsg, ...);

#ifdef NDEBUG
// -------------------------------------------------------
// -                       Release                       - 
// -------------------------------------------------------
#	define	TAssert(a)      		{}
#	define	TFail()      			{}
#else

// -------------------------------------------------------
// -                        Debug                        - 
// -------------------------------------------------------
	void	TAssertPC(char* msg, const char* file, int line);
#	define TAssert(a)			if	(!(a))	TAssertPC(#a, __FILE__, __LINE__)
#	define TFail()				TAssertPC("Fail", __FILE__, __LINE__)
#	define T_DEBUG				1

#endif

static const char* UID_BUILD = "Build : "__DATE__" "__TIME__;

#endif	//__BASE_H__
