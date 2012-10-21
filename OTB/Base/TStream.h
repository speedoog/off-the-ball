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

#ifndef __TSTREAM_H__
#define __TSTREAM_H__
#pragma once

#include "Base.h"
#include "TString.h"
//#include "TVector.h"

//class TVector;

class TStream
{
public:
	enum TStreamType
	{
		ST_MEMORY =0,
		ST_FILE
	};

	enum TStreamOpenMode
	{
		SM_READ	=0,
		SM_WRITE,
		SM_APPEND,
	};

				TStream();
				~TStream();

	Bool		OpenFile(const TString& sFilename, const TStreamOpenMode OpenMode);
	void		CloseFile();

	void		Write(const void* pBuffer, const UInt32 nSize);
	void		Read(void* pBuffer, const UInt32 nSize);

	template<class TData>
	inline void	operator << (const TData& Data)
	{
		Write(&Data, sizeof(Data));
	}
	template<class TData>
	inline void	operator >> (TData& Data)
	{
		Read(&Data, sizeof(Data));
	}

protected:
	
protected:
	TStreamOpenMode _OpenMode;

	// File mode
	TString 		_sFilename;
	FILE*			_pFile;

	// Memory mode
// 	typedef TVector<UInt8>	MemBuffer;
// 	MemBuffer*		_pMemBuffer;
};

#endif	//__TSTREAM_H__
