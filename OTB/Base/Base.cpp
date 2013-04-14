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

#include "base.h"
#include <stdio.h>
#include <stdarg.h>
#include "windows.h"

// ****************************************************************************************
//	TPrintMessage
// ****************************************************************************************
void TPrintMessage(char* pFormatedMsg, ...)
{
	const int MAX_SIZE =1024;
	char	buf[MAX_SIZE];
	va_list	marker;
	va_start(marker,pFormatedMsg);
	vsprintf_s(buf,MAX_SIZE,pFormatedMsg,marker);
	va_end(marker);
	OutputDebugString(buf);
}

// ****************************************************************************************
//	TPrintWarning
// ****************************************************************************************
void TPrintWarning(char* pFormatedMsg, ...)
{
	const int MAX_SIZE =1024;
	char	buf[MAX_SIZE];
	va_list	marker;
	va_start(marker,pFormatedMsg);
	vsprintf_s(buf,MAX_SIZE,pFormatedMsg,marker);
	va_end(marker);
	OutputDebugString(buf);
}

// ****************************************************************************************
//	TPrintError
// ****************************************************************************************
void TPrintError(char* pFormatedMsg, ...)
{
	const int MAX_SIZE =1024;
	char	buf[MAX_SIZE];
	va_list	marker;
	va_start(marker,pFormatedMsg);
	vsprintf_s(buf,MAX_SIZE,pFormatedMsg,marker);
	va_end(marker);
	OutputDebugString(buf);
}

// ****************************************************************************************
//	TAssertFunction
// ****************************************************************************************
void TAssertFunction(char* msg, const char* file, int line)
{
	TPrintMessage("*** ASSERT Failed in file %s on line %d ***\n> %s\n", file, line, msg);
}
