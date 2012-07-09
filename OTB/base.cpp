#include "base.h"
#include <cstdio>

#if defined(WIN32)
void	Win32OSReport(char*	string,...)
{
	char	buf[2048];
	va_list	marker;
	va_start(marker,string);
	vsprintf(buf,string,marker);
	va_end(marker);
	OutputDebugString(buf);
}

void RfxAssertPC (char* msg, const char* file, int line)
{
	Win32OSReport ("*** ASSERT Failed in file %s on line %d\n%s\n", file,line,msg);
}
#endif

#if defined(NGC) && !defined (NDEBUG)
void RfxAssertGC (bool cond, char* msg, const char* file, int line)
{
	if (!cond)
	{
		RfxDebug3 ("*** ASSERT Failed in file %s on line %d\n", file, line);
		RfxDebug (msg);
		RfxDebug ("\n");
	}
}
#endif