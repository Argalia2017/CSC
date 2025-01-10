#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csignal>
#include <csc_begin.h>

using namespace SOLUTION ;

namespace SOLUTION {
void test4 () {
	test4 () ;
}

void test3 () {
	auto anonymous = UniqueRef<BOOL> ([&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test3::enter")) ;
	} ,[&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test3::leave")) ;
	}) ;
	const auto mRandom = Random (CurrentRandom ().random_value (0 ,10000)) ;
	const auto mCurrStatus = mRandom.random_value (0 ,6) ;
	Singleton<Console>::instance ().debug (slice ("mCurrStatus = ") ,mCurrStatus) ;
	if (mCurrStatus == 0) {
		Singleton<Console>::instance ().warn (slice ("warn SIGINT")) ;
		for (auto &&i : iter (0 ,1000)) {
			Singleton<Console>::instance ().debug (slice ("i = ") ,i) ;
			RuntimeProc::thread_sleep (Time (500)) ;
		}
		Singleton<Console>::instance ().warn (slice ("???")) ;
	} else if (mCurrStatus == 1) {
		Singleton<Console>::instance ().warn (slice ("warn SIGILL")) ;
#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_CLANG__
		asm volatile ("ud2") ;
#endif
#endif
#ifdef __CSC_SYSTEM_LINUX__
		asm volatile ("ud2") ;
#endif
		Singleton<Console>::instance ().warn (slice ("???")) ;
	} else if (mCurrStatus == 2) {
		Singleton<Console>::instance ().warn (slice ("warn SIGFPE")) ;
STATUS2:
		const auto r1x = 0 ;
		const auto r2x = 100 % MathProc::square (int (r1x)) ;
		Singleton<Console>::instance ().warn (slice ("r2x = ") ,r2x) ;
		Singleton<Console>::instance ().warn (slice ("???")) ;
	} else if (mCurrStatus == 3) {
		Singleton<Console>::instance ().warn (slice ("warn SIGFPE")) ;
		const auto r3x = 0.0 ;
		const auto r4x = 100 / MathProc::square (r3x) ;
		Singleton<Console>::instance ().warn (slice ("r4x = ") ,r4x) ;
		Singleton<Console>::instance ().warn (slice ("???")) ;
		goto STATUS2 ;
	} else if (mCurrStatus == 4) {
		Singleton<Console>::instance ().warn (slice ("warn SIGSEGV")) ;
		const auto r5x = nullof (int) ;
		watch (r5x) ;
		Singleton<Console>::instance ().warn (slice ("???")) ;
	} else if (mCurrStatus == 5) {
		Singleton<Console>::instance ().warn (slice ("warn SIGTERM")) ;
#ifdef __CSC_SYSTEM_WINDOWS__
		test4 () ;
#endif
#ifdef __CSC_SYSTEM_LINUX__
		const auto r8x = String<STR>::make (slice ("kill ") ,r6x) ;
		System ().execute (r8x) ;
#endif
		Singleton<Console>::instance ().warn (slice ("???")) ;
	} else if (mCurrStatus == 6) {
		Singleton<Console>::instance ().warn (slice ("warn SIGABRT")) ;
		inline_abort () ;
		Singleton<Console>::instance ().warn (slice ("???")) ;
	}
}

void test2 () {
	auto anonymous = UniqueRef<BOOL> ([&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test2::enter")) ;
	} ,[&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test2::leave")) ;
	}) ;
	test3 () ;
}

void test1 () {
	auto anonymous = UniqueRef<BOOL> ([&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test1::enter")) ;
	} ,[&] (VREF<BOOL> me) {
		Singleton<Console>::instance ().info (slice ("test1::leave")) ;
	}) ;
	test2 () ;
}

void test () {
#ifdef __CSC_SYSTEM_WINDOWS__
	test1 () ;
#endif
#ifdef __CSC_SYSTEM_LINUX__
	test1 () ;
#endif
}

void test_s () {
	try {
		Singleton<Console>::instance ().info (slice ("begin")) ;
		test () ;
		Singleton<Console>::instance ().info (slice ("end")) ;
	} catch (CREF<Exception> e) {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().error (slice ("ERROR")) ;
		const auto r1x = Format (slice ("assume : $1 at $2 in $3, $4")) ;
		Singleton<Console>::instance ().error (r1x (e.what () ,e.func () ,e.file () ,e.line ())) ;
		Singleton<Console>::instance ().trace () ;
	}
}
} ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	test_s () ;

	Singleton<Console>::instance ().info (slice ("all done")) ;
	GlobalRoot::shutdown () ;
	return 0 ;
}