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

#include "TString.h"

/*
sWString &  __ltrim16 (sWString &arg)
////////////////////////////////////
{
	UInt32 pos=0;
	while (arg.is_ws(arg.m_data[pos])) pos++;
	if (pos)
	{
		arg.int_cpy(arg.m_data,arg.m_data+pos,arg.m_size-pos+1);
		arg.m_size-=pos;
	}
	return arg;
}


sWString &  __rtrim16 (sWString &arg)
////////////////////////////////////////////
{
	while (arg.m_size && arg.is_ws(arg.m_data[arg.m_size-1])) 
	{
		arg.m_size--;
		arg.int_null();
	}
	return arg;
}



int  __format16 (sWString &arg, const sWChar* format, va_list arglist)
////////////////////////////////////////////////////////////////////////////
{
  // try to write directly into the buffer (without realloc) 
  if (arg.m_allocsize)
  {
	  int n = _vsnwprintf(arg.m_data, arg.m_allocsize, format, arglist);
    if (n>0)
    {
      arg.m_data[arg.m_allocsize-1]=0;
      arg.m_size = n;
      return n;
    }
  }

  UInt32 MaxSize = 8192;
  Int32 n;
  do 
  {
    sWChar * temp = new sWChar[MaxSize];
	  n = _vsnwprintf(temp, MaxSize-1, format, arglist);
    temp[MaxSize-1]=0;

    if (n>=0)
    {
      arg = temp;
      delete [] temp;
      return n;
    }
    else MaxSize += 8192;
    TAssert (MaxSize < 65536); // well, 64k of text is very unlikely
    delete [] temp;
  } while (1);
}
*/

int  __format8 (TString &arg, const Char * format, va_list arglist)
////////////////////////////////////////////////////////////////////////////
{
  // try to write directly into the buffer (without realloc) 
  if (arg.m_allocsize)
  {
	  int n = _vsnprintf(arg.m_data, arg.m_allocsize, format, arglist);
    if (n>0)
    {
      arg.m_data[arg.m_allocsize-1]=0;
      arg.m_size = n;
      return n;
    }
  }

  UInt32 MaxSize = 8192;
  Int32 n;
  do 
  {
    char * temp = new char[MaxSize];
	  n = _vsnprintf(temp, MaxSize-1, format, arglist);
    temp[MaxSize-1]=0;

    if (n>=0)
    {
      arg = temp;
      delete [] temp;
      return n;
    }
    else MaxSize += 8192;
    TAssert (MaxSize < 65536); // well, 64k of text is very unlikely
    delete [] temp;
  } while (1);
}
