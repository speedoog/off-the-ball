#include	"Stdafx.h"
#include	"Root.h"

#include	INCL_KCORE(Template/QDT_Vector)
#include	INCL_KCORE(Stream/iostream)

namespace QDT
{
	namespace KCORE
	{
#if 1
		template <class T>
		IO_STREAM &	operator << (IO_STREAM & Stream, const QDT_VECTOR<T> & Vector)
		{
			Stream << Vector.GetSize();

			for (UInt32 i = 0 ; i < Vector.GetSize() ; ++i)
			{
				Stream << Vector[i];
			}

			return (Stream);
		}

		template <class T>
		IO_STREAM &	operator >>  (IO_STREAM & Stream, QDT_VECTOR<T> & Vector)
		{
			//
			// [MBI] - OK, that's a VERY nasty trick, but it will work.
			//
			// On GCC, if you do:
			//
			//			UInt32 nSize = 0; Stream >> nSize;
			//
			// It wont compile because GCC try to find the actual implementation of operator >> for stream.
			// But we dont know it because QDT_VECTOR is a template class...
			//
			// So, here, the trick is to force the compiler to delay the method compilation.
			// If we do:
			//
			//			Stream >> Vector._nSize;
			//
			// The compiler here is force to delay the compilation, because VECTOR is a template class.
			// In this case, the compiler will wait to implement the inline directly in the appropriate CPP files.
			//
			// Tada, problem resolved ;-)
			//

			// Backup Vector._nSize.
			UInt32 nSize = Vector._nSize;

			// Read the actual value.
			Stream >> Vector._nSize;

			// Swap the value with the backup.
			UInt32 nTmp		= nSize;
			nSize			= Vector._nSize;
			Vector._nSize	= nTmp;

			Vector.Reserve( nSize );

			for (UInt32 i = 0 ; i < nSize ; ++i)
			{
				T t;
				Stream >> t;
				Vector.PushTail(t);
			}

			return (Stream);
		}
#endif
	}
}