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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../hge/hge.h"
#include "../../hge/hgevector.h"
#include "../../hge/hgecolor.h"

// -------------------------------------------- global HGE --------------------------------------------

class HGE;
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
typedef wchar_t				sWChar;   // type for unicode strings

// -------------------------------------------- new / delete --------------------------------------------

#define TDelete(_Obj)					if (_Obj) {delete (_Obj);(_Obj)=NULL;}
#define TDeleteTab(_Tab)				if (_Tab) {delete [](_Tab);(_Tab)=NULL;}
#define TRelease(p)						{ if(p) { (p)->Release(); (p)=NULL; } }

// new override / placement

inline void* operator new	(size_t, void *_P)		{ return (_P); }	// new with placement
inline void	 operator delete(void*,void*)			{ };

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

// -------------------------------------------- Assert --------------------------------------------

void TPrintMessage(char* pFormatedMsg, ...);
void TPrintWarning(char* pFormatedMsg, ...);
void TPrintError(char* pFormatedMsg, ...);
void TAssertFunction(char* msg, const char* file, int line);

#ifdef NDEBUG
// ************************* Release *************************
#	define	TAssert(a)      	{}
#	define	TFail()      		{}
#else
// ************************* Debug *************************
#	define TAssert(a)			if	(!(a))	TAssertFunction(#a, __FILE__, __LINE__)
#	define TFail()				TAssertFunction("Fail", __FILE__, __LINE__)
#	define T_DEBUG				1
#endif

// Helper macro T_JOIN:
// The following piece of macro magic joins the two arguments together, even when one of the arguments is itself a macro 
// The key is that macro expansion of macro arguments does not occur in T_DO_JOIN2 but does in T_DO_JOIN.
#define T_JOIN(_part1_, _part2_)		T_DO_JOIN(_part1_, _part2_)
#define T_DO_JOIN(_part1_, _part2_)		T_DO_JOIN2(_part1_, _part2_)
#define T_DO_JOIN2(_part1_, _part2_)	_part1_##_part2_

static const char* UID_BUILD = "Build : "__DATE__" "__TIME__;

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

template<typename T>
inline T TAbs(const T& _x)
{
	return (_x<0)?-_x:_x;
}

template<typename T>
inline T TSign(const T& _x_)
{
	return (_x_<0.0f) ? (-1.0f) : (_x_>0.0f) ? (1.0f) : (0.0f);
}

template <typename T>
inline T TMax(T Value1, T Value2)
{
	return (Value1>Value2) ? Value1 : Value2;
}

template <typename T>
inline T TMin(T Value1, T Value2)
{
	return (Value1<Value2) ? Value1 : Value2;
}

template <class C> static C	TBlend(const C Source, const C Destination, const float rRatio)
{
	return (Source+(Destination-Source)*rRatio);
}

// How to use this blend :
// - This method returns the blended value corresponding to the inertia you should apply according to its inertial factor
// and the elapsed time.
// - rInertialFactor should vary from 0.0f to 1.0f : if 0.0f, no inertia, if 1.0f, no movement
template <class C> static C TBlend(C Source, C Destination, const float rInertialFactor, const float rDeltaTime)
{
	const float rConservativeRatio =(1.0f-powf(rInertialFactor, rDeltaTime));
	return TBlend(Source, Destination, rConservativeRatio);
}

template <class C> static C TChangeRange(C SourceRangeMin, C SourceRangeMax, C DestinationRangeMin, C DestinationRangeMax, Float32 Value)
{
	C dx=SourceRangeMax-SourceRangeMin;
	C dy=DestinationRangeMax-DestinationRangeMin;
	C k=(Value-SourceRangeMin)/dx;
	return DestinationRangeMin+dy*k;
}

template <class C> static C TChangeRangeClamped(C SourceRangeMin, C SourceRangeMax, C DestinationRangeMin, C DestinationRangeMax, Float32 Value)
{
	return TClamp( TChangeRange(SourceRangeMin, SourceRangeMax, DestinationRangeMin, DestinationRangeMax, Value), DestinationRangeMin, DestinationRangeMax);
}

// Range Check
// Boundaries are inclusive
template <typename C> static Bool	TIsInRange(const C Val, const C Min, const C Max)
{
	return (Val>=Min) && (Val<=Max);
}

// Overlapping check:               
//								    
//    [ ------ A ------- ]		    
//           [ ------- B ------- ]  
//								    
// Warning : The overlapping check is boundary exclusive 
//			 so for exemple: [1..3] does not overlap [3..8]
//           use the Epsilon verion to manage this case
template <typename C> static Bool	TOverlap(const C& Amin, const C& Amax, const C& Bmin, const C& Bmax)
{
	return (Amin<Bmax) && (Bmin<Amax);
}

template <typename C> static Bool	TOverlap(const C& Amin, const C& Amax, const C& Bmin, const C& Bmax, const C& Epsilon)
{
	TAssert(Amin<=Amax);
	TAssert(Bmin<=Bmax);
	return TOverlap(Amin+Epsilon, Amax-Epsilon, Bmin, Bmax);
}

// Inside check:                       
//							           
//        [ ---- A --- ]  	           
//    [ ------ B ------- ]	           
//							           
// Checks B range is entirely inside A 
//							           
template <typename C> static Bool	TInside(const C& Amin, const C& Amax, const C& Bmin, const C& Bmax)
{
	TAssert(Amin<=Amax);
	TAssert(Bmin<=Bmax);
	return (Amin>=Bmin) && (Amax<=Bmax);
}

// SmoothStep(fMin, fMax, x)   y = 3 x^2 - 2 x^3 
//                                               
// /!\ Warning : ONLY VALID in [0..1] range ! /!\_ 
//                                               
// 1 ^            ,~-+   Output                  
//   |         ,²¨   .                           
//   |        .      .                           
//   |       /       .                           
//   |      i        .                           
//   |    ,²         .                           
// 0 +-~²'-----------+-> x                       
//   0               1                           
//                                               
static Float32 TSmoothStep(const Float32 x)
{
	TAssert(x>=0.0f);
	TAssert(x<=1.0f);

	const Float32 fRes = x*x*(3.f - 2.f*x);
	return fRes;
}

// SmoothStep(fMin, fMax, x)   same with an input range
//                                              
// 1 ^      .            ,~-+------  Output     
//   |	    .         ,²¨   .                   
//   |	    .        .      .                   
//   |	    .       /       .                   
//   |	    .      i        .                   
//   |      .    ,²         .                   
// 0 + -----+-~²'-----------+------> x          
//         fmin            fmax                 
//                                              
// ref: http://www.planetside.co.uk/docs/tg2/noderef/window_1_16_2_image_0.png
static Float32 TSmoothStep(const Float32 fMin, const Float32 fMax, const Float32 x)
{
	TAssert(fMin <= fMax);

	if (x >= fMax) return 1.0f;
	if (x <= fMin) return 0.0f;

	const Float32 fRange =fMax - fMin;
	const Float32 fDiv	 =(x - fMin) / fRange;
	const Float32 fRes	 =TSmoothStep(fDiv);
	return fRes;
}
static inline Float32 TRand()
{
	return Float32(rand())/Float32(RAND_MAX);
}

static inline UInt32 TRand(const UInt32 nMax)
{
	return (rand()%nMax);
}

static inline UInt32 TRand(const UInt32 nMin, const UInt32 nMax)
{
	UInt32 nRange =nMax-nMin;
	return nMin+(rand()%nRange);
}

static inline Float32 TRand(const Float32 rMin, const Float32 rMax)
{
	return TChangeRange(0.0f, Float32(RAND_MAX), rMin, rMax, Float32(rand()) );
}

static Bool TEqual(const Float32 rValue1, const Float32 rValue2, const Float32 rEpsilon = 0.05f)
{
	return (TAbs(rValue1-rValue2) < rEpsilon);
}

#define FOR_EACH(_CONTAINER_, _ITER_, _CONTAINER_TYPE_)					\
	for (_CONTAINER_TYPE_::Iterator _ITER_ = (_CONTAINER_).GetHead(),	\
	_ITEND_ = (_CONTAINER_).GetTail(); _ITER_!=_ITEND_; ++_ITER_)


// Attribute & methods offsets
// class CHEAT_ON_COMPILER
// {
// public:
// 	static void* pCheatOnCompiler;
// };
// #define GET_ATTR_OFFSET(BaseClass, Attribute)					(UInt32)(&(((BaseClass*)CHEAT_ON_COMPILER::pCheatOnCompiler)->Attribute))
//#define GET_ATTR_OFFSET_FROM_METHOD(ClassInstance, Attribute)	(UInt32)((IntPtr)(&Attribute) - (IntPtr)(&ClassInstance))
#define GET_ATTR_OFFSET(st, m)									((size_t) ( (char *)&((st *)0)->m - (char *)0 ))

#define GET_CONTAINER_OF(_ptr_, _type_, _member_)				(_type_*) ( (IntPtr)(_ptr_)-GET_ATTR_OFFSET(_type_, _member_) )


#endif	//__BASE_H__
