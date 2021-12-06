#include "util.h"

#include "csc/begin.h"
#ifdef __CSC_COMPILER_MSVC__
#ifdef __CSC_SYSTEM_WINDOWS__
#pragma warning (push)
#pragma warning (disable :4091)
#pragma warning (disable :4996)
#pragma warning (disable :5039)
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <crtdbg.h>
#include <signal.h>
#include <DbgHelp.h>
#pragma warning (pop)
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include "csc/end.h"

namespace UNITTEST {
imports void DumpMemoryLeaksAtExit () {
#ifdef __CSC_COMPILER_MSVC__
	const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
	noop (r1x) ;
	const auto r2x = CSC::VAL32 (r1x | _CRTDBG_LEAK_CHECK_DF) ;
	noop (r2x) ;
	const auto r3x = _CrtSetDbgFlag (r2x) ;
	noop (r3x) ;
#endif
}

struct ThisModule {
	void enter () const {
		DumpMemoryLeaksAtExit () ;
		const auto r30x = unsafe_usage () ;
		printf_s ("r30x = %d\n" ,(int) r30x) ;

	}

	void leave () const {
		const auto r31x = unsafe_usage () ;
		printf_s ("r31x = %d\n" ,(int) r31x) ;
	}

	imports void hook () {
		static const Scope<CREF<ThisModule>> anonymous (unsafe_array (TYPEAS<ThisModule>::id)[0]) ;
	}
} ;
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;

	ThisModule::hook () ;
	assert (FALSE) ;

	const auto r1x = invoke ([&] () {
		const auto r1x = Vector<DOUBLE> (1 ,2 ,3 ,0).normalize () ;
		const auto r2x = Vector<DOUBLE>::axis_y () ;
		const auto r3x = Matrix<DOUBLE>::make_view_xy (r1x ,r2x) ;
		const auto r4x = r3x * Matrix<DOUBLE>::make_diag (1 ,1 ,0 ,1) * r3x.inverse () ;
		const auto r25x = r4x * Vector<DOUBLE> (1 ,0 ,0 ,1) ;
		const auto r26x = r4x * Vector<DOUBLE> (0 ,1 ,0 ,1) ;
		const auto r27x = r4x * Vector<DOUBLE> (0 ,0 ,1 ,1) ;
		const auto r28x = r4x * Vector<DOUBLE> (0 ,0 ,0 ,1) ;
		const auto r20x = Matrix<DOUBLE> (r25x ,r26x ,r27x ,r28x) ;
		return r20x.transpose () ;
	}) ;

	const auto r2x = r1x.singular () ;
	unittest_watch (r2x) ;

	assert (TRUE) ;
	return 0 ;
}
#endif