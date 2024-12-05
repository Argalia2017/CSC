#include "util.h"

#include "Common/export.h"

#include "csc/csc_end.h"
#ifdef __CSC_COMPILER_MSVC__
#include <intrin.h>
#include <yvals.h>
#endif

#include <initializer_list>
#include <type_traits>
#include <typeinfo>
#include <cstdio>
#include <exception>
#include <string>
#include <atomic>
#include <assert.h>
#include <signal.h>
#include "csc/csc_begin.h"

using namespace SOLUTION ;

/*
1. Pin & Ref
*/

int main () {
	Singleton<Console>::instance ().start () ;
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().info (slice ("main")) ;

	return 0 ;
}