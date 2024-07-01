#include "util.h"

#include "csc/csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

#include <initializer_list>

#include <opencv2/opencv.hpp>
#include "csc/csc_begin.h"

using namespace SOLUTION ;

exports int main () {
	Singleton<Console>::instance ().start () ;
	Singleton<Global<int>>::instance () ;
	if (TRUE) {
		
	}
	assert (FALSE) ;
	return 0 ;
}