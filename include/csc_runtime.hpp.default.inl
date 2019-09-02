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
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <setjmp.h>
#include <cmath>
#include <random>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("discard")
#endif

namespace CSC {
#ifdef __CSC_DEPRECATED__
template <class UNIT>
class Coroutine<UNIT>::Implement final :private Interface {
private:
	struct CONTEXT_EBP {
		jmp_buf mEbp ;
	} ;

	using CONTEXT_EBP_SIZE = ARGC<_SIZEOF_ (CONTEXT_EBP) + _ALIGNOF_ (CONTEXT_EBP)> ;
	using STACK_FRAME_SIZE = ARGC<65536> ;

	struct BREAKPOINT {
		DEF<BYTE[CONTEXT_EBP_SIZE::value]> mContextEbp ;
		ARRAY3<PTR<VOID>> mStackPoint ;
		DEF<BYTE[STACK_FRAME_SIZE::value]> mStackFrame ;
	} ;

private:
	friend Coroutine<UNIT> ;

public:
	static void init_break_point (PTR<AnyRef<void>> bp) {
		auto rax = BREAKPOINT () ;
		_ZERO_ (rax) ;
		rax.mStackPoint[0] = &bp ;
		rax.mStackPoint[1] = NULL ;
		rax.mStackPoint[2] = NULL ;
		(*bp) = AnyRef<BREAKPOINT>::make (std::move (rax)) ;
	}

	static void store_break_point (PTR<AnyRef<void>> bp) noexcept {
		auto &r1 = bp->rebind<BREAKPOINT> ().self ;
		_DEBUG_ASSERT_ (r1.mStackPoint[0] != NULL) ;
		r1.mStackPoint[1] = &bp ;
		const auto r1x = _ADDRESS_ (r1.mStackPoint[1]) - _ADDRESS_ (r1.mStackPoint[0]) ;
		_DEBUG_ASSERT_ (_ABS_ (r1x) <= _COUNTOF_ (decltype (r1.mStackFrame))) ;
		const auto r2x = EFLAG (r1x < 0) ;
		_MEMCOPY_ (PTRTOARR[r1.mStackFrame] ,_LOAD_<ARR<BYTE>> (r1.mStackPoint[r2x]) ,_ABS_ (r1x)) ;
		auto &r2 = load_context_ebp (&r1.mContextEbp) ;
		const auto r3x = setjmp (r2.mEbp) ;
		(void) r3x ;
	}

	static void goto_break_point (PTR<AnyRef<void>> bp) noexcept {
		auto &r1 = bp->rebind<BREAKPOINT> ().self ;
		_DEBUG_ASSERT_ (r1.mStackPoint[0] != NULL) ;
		_DEBUG_ASSERT_ (r1.mStackPoint[1] != NULL) ;
		r1.mStackPoint[2] = &bp ;
		_STATIC_WARNING_ ("mark") ;
		_DEBUG_ASSERT_ (r1.mStackPoint[2] == r1.mStackPoint[1]) ;
		const auto r1x = _ADDRESS_ (r1.mStackPoint[1]) - _ADDRESS_ (r1.mStackPoint[0]) ;
		_DEBUG_ASSERT_ (_ABS_ (r1x) <= _COUNTOF_ (decltype (r1.mStackFrame))) ;
		const auto r2x = EFLAG (r1x < 0) ;
		_MEMCOPY_ (_LOAD_<ARR<BYTE>> (r1.mStackPoint[r2x]) ,PTRTOARR[r1.mStackFrame] ,_ABS_ (r1x)) ;
		auto &r2 = load_context_ebp (&r1.mContextEbp) ;
		longjmp (r2.mEbp ,1) ;
	}

	static CONTEXT_EBP &load_context_ebp (PTR<BYTE[CONTEXT_EBP_SIZE::value]> ebp) noexcept {
		const auto r1x = _ADDRESS_ (ebp) % _ALIGNOF_ (CONTEXT_EBP) ;
		const auto r2x = _SWITCH_ (
			(r1x != 0) ? (_ALIGNOF_ (CONTEXT_EBP) - r1x) :
			0) ;
		const auto r3x = _ADDRESS_ (ebp) + r1x ;
		return _LOAD_<CONTEXT_EBP> (NULL ,r3x) ;
	}
} ;

template <class UNIT>
inline exports void Coroutine<UNIT>::init_break_point (AnyRef<void> &bp) {
	Implement::init_break_point (&bp) ;
}

template <class UNIT>
inline exports void Coroutine<UNIT>::store_break_point (AnyRef<void> &bp) noexcept {
	Implement::store_break_point (&bp) ;
}

template <class UNIT>
inline exports void Coroutine<UNIT>::goto_break_point (AnyRef<void> &bp) noexcept {
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

	VAL entropy () const override {
		return VAL (mRandomSeed->entropy ()) ;
	}

	void reset_seed (VAR _seed) override {
		mRandomDevice = AutoRef<std::mt19937>::make (CHAR (_seed)) ;
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