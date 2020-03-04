#pragma once

#ifndef __CSC_RUNTIME__
#error "б╞(д├бузебу ;)д├ : require 'csc_runtime.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef switch_case
#undef discard
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef _INC_WINDOWS
#error "б╞(д├бузебу ;)д├ : require 'Windows.h'"
#endif
#elif defined __CSC_SYSTEM_LINUX__
#ifdef __CSC_DEPRECATED__
#include <unistd.h>
#include <sys/syscall.h>
#endif
#endif

#ifdef __CSC_DEPRECATED__
#include <cmath>
#include <random>

#include <setjmp.h>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
inline FLAG GlobalRuntime::thread_tid () {
	return FLAG (GetCurrentThreadId ()) ;
}

inline FLAG GlobalRuntime::process_pid () {
	return FLAG (GetCurrentProcessId ()) ;
}

inline Buffer<BYTE ,ARGC<128>> GlobalRuntime::process_info (FLAG pid) {
	Buffer<BYTE ,ARGC<128>> ret ;
	auto wos = ByteWriter (PhanBuffer<STRU8>::make (ret)) ;
	if switch_case (TRUE) {
		const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,VARY (pid)) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
		}) ;
		if (r1x == NULL)
			discard ;
		wos << VAR64 (pid) ;
		wos << _GAP_ ;
		wos << _PCSTRU8_ ("windows") ;
		wos << _GAP_ ;
		auto rax = ARRAY4<FILETIME> () ;
		_ZERO_ (rax[0]) ;
		_ZERO_ (rax[1]) ;
		_ZERO_ (rax[2]) ;
		_ZERO_ (rax[3]) ;
		GetProcessTimes (r1x ,&rax[0] ,&rax[1] ,&rax[2] ,&rax[3]) ;
		const auto r2x = (VAR64 (rax[0].dwHighDateTime) << 32) | VAR64 (rax[0].dwLowDateTime) ;
		wos << VAR64 (r2x) ;
		wos << _GAP_ ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

inline FLAG GlobalRuntime::process_info_pid (const PhanBuffer<const STRU8> &info) {
	_DEBUG_ASSERT_ (info.size () == 128) ;
	auto ris = ByteReader (info) ;
	return ris.template read<VAR64> () ;
}
#elif defined __CSC_SYSTEM_LINUX__
inline FLAG GlobalRuntime::thread_tid () {
	return FLAG (syscall (SYS_gettid)) ;
}

inline FLAG GlobalRuntime::process_pid () {
	return FLAG (syscall (SYS_getpid)) ;
}

inline Buffer<BYTE ,ARGC<128>> GlobalRuntime::process_info (FLAG pid) {
	Buffer<BYTE ,ARGC<128>> ret ;
	auto wos = ByteWriter (PhanBuffer<STRU8>::make (ret)) ;
	if switch_case (TRUE) {
		const auto r1x = getpgid (pid_t (pid)) ;
		if (r1x < 0)
			discard ;
		wos << VAR64 (pid) ;
		wos << _GAP_ ;
		wos << _PCSTRU8_ ("linux") ;
		wos << _GAP_ ;
		wos << VAR64 (r1x) ;
		wos << _GAP_ ;
		const auto r2x = getsid (pid_t (pid)) ;
		wos << VAR64 (r2x) ;
		wos << _GAP_ ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

inline FLAG GlobalRuntime::process_info_pid (const PhanBuffer<const STRU8> &info) {
	_DEBUG_ASSERT_ (info.size () == 128) ;
	auto ris = ByteReader (info) ;
	return ris.template read<VAR64> () ;
}
#else
inline FLAG GlobalRuntime::thread_tid () {
	_DYNAMIC_ASSERT_ (FALSE) ;
	return 0 ;
}

inline FLAG GlobalRuntime::process_pid () {
	_DYNAMIC_ASSERT_ (FALSE) ;
	return 0 ;
}

inline Buffer<BYTE ,ARGC<128>> GlobalRuntime::process_info (FLAG pid) {
	_DYNAMIC_ASSERT_ (FALSE) ;
	return Buffer<BYTE ,ARGC<128>> () ;
}

inline FLAG GlobalRuntime::process_info_pid (const PhanBuffer<const STRU8> &info) {
	_DEBUG_ASSERT_ (info.size () == 128) ;
	_DYNAMIC_ASSERT_ (FALSE) ;
	return 0 ;
}
#endif

#ifdef __CSC_DEPRECATED__
template <class CONT>
class Coroutine<CONT>::Implement final :private Interface {
private:
	struct CONTEXT_EBP {
		jmp_buf mEbp ;
	} ;

	using CONTEXT_EBP_SIZE = ARGC<_ALIGNOF_ (CONTEXT_EBP) - 1 + _SIZEOF_ (CONTEXT_EBP)> ;
	using STACK_FRAME_SIZE = ARGC<65536> ;

	struct BREAKPOINT {
		DEF<BYTE[CONTEXT_EBP_SIZE::value]> mContextEbp ;
		ARRAY3<LENGTH> mStackPoint ;
		DEF<BYTE[STACK_FRAME_SIZE::value]> mStackFrame ;
	} ;

private:
	friend Coroutine<CONT> ;

public:
	static void init_break_point (PTR<AnyRef<void>> bp) {
		auto tmp = BREAKPOINT () ;
		_ZERO_ (tmp) ;
		tmp.mStackPoint[0] = _ADDRESS_ (&bp) ;
		tmp.mStackPoint[1] = 0 ;
		tmp.mStackPoint[2] = 0 ;
		(*bp) = AnyRef<BREAKPOINT>::make (std::move (tmp)) ;
	}

	static void store_break_point (PTR<AnyRef<void>> bp) noexcept {
		auto &r1y = bp->rebind<BREAKPOINT> ().self ;
		_DEBUG_ASSERT_ (r1y.mStackPoint[0] != 0) ;
		r1y.mStackPoint[1] = _ADDRESS_ (&bp) ;
		const auto r2x = r1y.mStackPoint[1] - r1y.mStackPoint[0] ;
		_DEBUG_ASSERT_ (_ABS_ (r2x) <= _SIZEOF_ (decltype (r1y.mStackFrame))) ;
		const auto r3x = EFLAG (r2x < 0) ;
		auto &r4y = _LOAD_<ARR<BYTE>> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r1y.mStackPoint[r3x])) ;
		_MEMCOPY_ (PTRTOARR[r1y.mStackFrame] ,r4y ,_ABS_ (r2x)) ;
		auto &r5y = load_context_ebp (r1y.mContextEbp) ;
		const auto r6x = ::setjmp (r5y.mEbp) ;
		(void) r6x ;
	}

	static void goto_break_point (PTR<AnyRef<void>> bp) noexcept {
		auto &r1y = bp->rebind<BREAKPOINT> ().self ;
		_DEBUG_ASSERT_ (r1y.mStackPoint[0] != 0) ;
		_DEBUG_ASSERT_ (r1y.mStackPoint[1] != 0) ;
		r1y.mStackPoint[2] = _ADDRESS_ (&bp) ;
		_STATIC_WARNING_ ("mark") ;
		_DEBUG_ASSERT_ (r1y.mStackPoint[2] == r1y.mStackPoint[1]) ;
		const auto r2x = r1y.mStackPoint[1] - r1y.mStackPoint[0] ;
		_DEBUG_ASSERT_ (_ABS_ (r2x) <= _SIZEOF_ (decltype (r1y.mStackFrame))) ;
		const auto r3x = EFLAG (r2x < 0) ;
		auto &r4y = _LOAD_<ARR<BYTE>> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r1y.mStackPoint[r3x])) ;
		_MEMCOPY_ (r4y ,PTRTOARR[r1y.mStackFrame] ,_ABS_ (r2x)) ;
		auto &r5y = load_context_ebp (r1y.mContextEbp) ;
		::longjmp (r5y.mEbp ,1) ;
	}

	static CONTEXT_EBP &load_context_ebp (DEF<BYTE[CONTEXT_EBP_SIZE::value]> &ebp) noexcept {
		const auto r1x = _ALIGNAS_ (_ADDRESS_ (&ebp) ,_ALIGNOF_ (CONTEXT_EBP)) ;
		return _LOAD_<CONTEXT_EBP> (_UNSAFE_ALIASING_ (r1x)) ;
	}
} ;

template <class CONT>
inline exports void Coroutine<CONT>::init_break_point (AnyRef<void> &bp) popping {
	Implement::init_break_point (&bp) ;
}

template <class CONT>
inline exports void Coroutine<CONT>::store_break_point (AnyRef<void> &bp) noexcept popping {
	Implement::store_break_point (&bp) ;
}

template <class CONT>
inline exports void Coroutine<CONT>::goto_break_point (AnyRef<void> &bp) noexcept popping {
	Implement::goto_break_point (&bp) ;
}
#endif

class RandomService::Implement :public RandomService::Abstract {
private:
	SharedRef<std::random_device> mRandomSeed ;
	AutoRef<std::mt19937> mRandomDevice ;

public:
	Implement () {
		mRandomSeed = SharedRef<std::random_device>::make () ;
		mRandomDevice = AutoRef<std::mt19937>::make (CHAR (mRandomSeed.self ())) ;
	}

	VAR entropy () const override {
		return VAR (mRandomSeed->entropy ()) ;
	}

	void reset_seed (VAR seed_) override {
		mRandomDevice = AutoRef<std::mt19937>::make (CHAR (seed_)) ;
	}

	VAR random_value () popping override {
		return VAR (mRandomDevice.self ()) ;
	}

	void random_skip (LENGTH len) override {
#pragma push_macro ("discard")
#undef discard
		mRandomDevice->discard (len) ;
#pragma pop_macro ("discard")
	}
} ;

inline exports RandomService::RandomService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;