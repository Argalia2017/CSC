#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <exception>
#include <csc_begin.h>

using namespace ROUTINE ;

/*
1. VR<[^\r]+> expr_m
2. Super<Ref<\w+Layout>>
3. struct \w+ (implement [^\r]+ )?\{(\r\n(?!\} ;)[^\r]*)*(private|public):(?!\r\n\t+implicit \w+Layout)
*/

/*
1. Sparse
2. Property
3. Serialize
4. RTree
5. FenwickTree
*/

void fatal_handler (int sig) {
	if ifdo (TRUE) {
		CLOG.trace () ;
		CLOG.fatal (slice ("FATAL : ")) ;
		const auto r1x = invoke ([&] () {
			if (sig == SIGINT)
				return slice ("Ctrl+C") ;
			if (sig == SIGILL)
				return slice ("Illegal Instruction") ;
			if (sig == SIGABRT)
				return slice ("Abort") ;
			if (sig == SIGFPE)
				return slice ("Floating Point Error") ;
			if (sig == SIGSEGV)
				return slice ("Segmentation Fault") ;
			if (sig == SIGTERM)
				return slice ("Killed") ;
			return slice ("???") ;
		}) ;
		CLOG.fatal (slice ("Code = ") ,r1x) ;
		const auto r2x = RuntimeProc::stack_trace (1) ;
		CLOG.error (slice ("Stack Trace :") ,GAP ,r2x) ;
		CLOG.trace () ;
	}
	RuntimeProc::process_exit () ;
}

void error_handler () {
	if ifdo (TRUE) {
		CLOG.trace () ;
		CLOG.error (slice ("ERROR : ")) ;
		try {
			Exception ().raise () ;
		} catch (CR<Exception> e) {
			const auto r1x = Format (slice ("assume : $1 at $2 in $3, $4")) ;
			CLOG.error (r1x (e.what () ,e.func () ,e.file () ,e.line ())) ;
		} catch (...) {
			CLOG.error (slice ("unknown C++ exception")) ;
		}
		const auto r2x = RuntimeProc::stack_trace (1) ;
		CLOG.error (slice ("Stack Trace :") ,GAP ,r2x) ;
		CLOG.trace () ;
	}
	RuntimeProc::process_exit () ;
}

void install_crash_handler () {
	if ifdo (TRUE) {
		std::signal (SIGINT ,fatal_handler) ;
		std::signal (SIGILL ,fatal_handler) ;
		std::signal (SIGABRT ,fatal_handler) ;
		std::signal (SIGFPE ,fatal_handler) ;
		std::signal (SIGSEGV ,fatal_handler) ;
		std::signal (SIGTERM ,fatal_handler) ;
		std::set_terminate (error_handler) ;
	}
}

int test () {
	install_crash_handler () ;
	return 0 ;
}