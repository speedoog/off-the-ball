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
//                   �>   .                                      <�                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SMARTENUM_H__
#define __SMARTENUM_H__
#pragma once

#include "Base.h"

// ---------------------------- SMART ENUM ---------------------------- 

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

class ENUM_STRINGIZER
{
public:
	class ENUM_REF
	{
	public:
		unsigned short _nOffset;
		unsigned short _nLength;
	};

	ENUM_STRINGIZER(const char* pEnums)
	{
		// 		int len =strlen(pEnums);
		// 		_pBuffer =new char[len];
		// 		memcpy(_pBuffer, pEnums, len+1);
		_pBuffer =pEnums;

		// enum parser
		const char* pRead =_pBuffer;

		// count elements
		int iCurrent=0;
		while (*pRead)
		{
			if (*pRead==',')
			{
				++iCurrent;
			}
			++pRead;
		}
		++iCurrent;

		// Alloc offset array
		_nEnumCount =iCurrent;
		_pEnums =new ENUM_REF[_nEnumCount];

		iCurrent=0;
		pRead =_pBuffer;
		const char* pLastStart =_pBuffer;
		while (*pRead)
		{
			char c =*pRead;
			if (c==' ' || c==',')
			{
				if (pLastStart)
				{
					_pEnums[iCurrent]._nOffset	=pLastStart-_pBuffer;
					_pEnums[iCurrent]._nLength	=pRead-pLastStart;
					++iCurrent;
					pLastStart =NULL;
				}
			}
			else
			{
				if (pLastStart==NULL)
					pLastStart =pRead;
			}
			++pRead;
		}

		// push last
		if (pLastStart)
		{
			_pEnums[iCurrent]._nOffset	=pLastStart-_pBuffer;
			_pEnums[iCurrent]._nLength	=pRead-_pBuffer;
		}
	}
	const char* GetString(int i)
	{
		// TODO: fix leaks :)
		int nLen =_pEnums[i]._nLength;
		char* p =new char[nLen+1];
		memcpy((void*)p, (void*)(_pBuffer+_pEnums[i]._nOffset), nLen);
		p[nLen] ='\0';

		return p;
	}

	unsigned int GetEnumCount()
	{
		return _nEnumCount;
	}

protected:
	const char*		_pBuffer;
	unsigned int	_nEnumCount;
	ENUM_REF*		_pEnums;
};

#define SMARTENUM_DECLARE(_enumname_, ...)									\
	enum _enumname_															\
	{																		\
		__VA_ARGS__															\
	};																		\
	static ENUM_STRINGIZER& _enumname_##_GetEnumStringizer()				\
	{																		\
		static ENUM_STRINGIZER	Stringizer(STRINGIZE(__VA_ARGS__));			\
		return Stringizer;													\
	};																		\
	static const char* _enumname_##_GetEnumName(int i)						\
	{																		\
		ENUM_STRINGIZER& Stringizer= _enumname_##_GetEnumStringizer();		\
		return Stringizer.GetString(i);										\
	};																		\
	static int _enumname_##_FindEnum(const char* pFind, const int nOffset)	\
	{																		\
		ENUM_STRINGIZER& Stringizer= _enumname_##_GetEnumStringizer();		\
		const int nSize=Stringizer.GetEnumCount();							\
		for(int i=0; i<nSize; ++i)											\
		{																	\
			const char* pName =Stringizer.GetString(i)+nOffset;				\
			if (!strcmp(pFind, pName))										\
				return i;													\
		}																	\
		return -1;															\
	};

#define	SMARTENUM_GET_STRING(_enumname_, _i_)								\
	_enumname_##_GetEnumName(_i_)

#define	SMARTENUM_FIND(_enumname_, _string_, _offset_)						\
	(_enumname_)_enumname_##_FindEnum(_string_, _offset_)

#endif	//__SMARTENUM_H__
