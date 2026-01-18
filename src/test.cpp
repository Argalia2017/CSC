#include "util.h"

#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

using namespace ROUTINE ;

struct ATrace {
	int mA ;

	implicit ATrace () noexcept {
		static int mAutoA = 1 ;
		mA = mAutoA++ ;
		CLOG.info (Format (slice ("A[$1] : $2")) (mA ,slice ("create"))) ;
	}

	implicit ~ATrace () noexcept {
		CLOG.info (Format (slice ("A[$1] : $2")) (mA ,slice ("destroy"))) ;
	}

	implicit ATrace (CR<ATrace> that) = delete ;

	forceinline VR<ATrace> operator= (CR<ATrace> that) = delete ;

	implicit ATrace (RR<ATrace> that) noexcept :ATrace () {
		swap (thiz ,that) ;
	}

	forceinline VR<ATrace> operator= (RR<ATrace> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

int test () {

	return 0 ;
}