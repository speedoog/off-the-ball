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

#include "base.h"
#include <stdio.h>

void Win32OSReport(char* pFormatedMsg, ...)
{
	const int MAX_SIZE =1024;
	char	buf[MAX_SIZE];
	va_list	marker;
	va_start(marker,pFormatedMsg);
	vsprintf_s(buf,MAX_SIZE,pFormatedMsg,marker);
	va_end(marker);
	OutputDebugString(buf);
}

void TAssertPC(char* msg, const char* file, int line)
{
	Win32OSReport("*** ASSERT Failed in file %s on line %d\n%s\n", file, line, msg);
}
