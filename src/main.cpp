#include "util.h"

#include "csc/begin.h"
#include <type_traits>
#include <iostream>
#include "csc/end.h"

using namespace UNITTEST ;

namespace UNITTEST {
exports void DumpMemoryLeaksAtExit () ;

struct ThisModule {
	void enter () const {
#ifdef __CSC_COMPILER_MSVC__
		DumpMemoryLeaksAtExit () ;
#endif
	}

	void leave () const {
		noop () ;
	}

	imports void hook () {
		static const ThisModule mInstance ;
		static const Scope<CREF<ThisModule>> anonymous (mInstance) ;
	}
} ;

struct GN_sqrt_mFirstY {
	DOUBLE mFirstY ;

	explicit GN_sqrt_mFirstY (VREF<SyntaxTree> me) {
		me.then (Function<void> ([&] () {
			mFirstY = 26 ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrX {
	DOUBLE mCurrX ;

	explicit GN_sqrt_mCurrX (VREF<SyntaxTree> me) {
		me.mark_as_iteration () ;
		me.later (TYPEAS<GN_sqrt_mNextX>::id) ;
		auto &mFirstY = me.value (TYPEAS<GN_sqrt_mFirstY>::id).mFirstY ;
		me.then (Function<void> ([&] () {
			mCurrX = mFirstY ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrY {
	DOUBLE mCurrY ;

	explicit GN_sqrt_mCurrY (VREF<SyntaxTree> me) {
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrY = mCurrX ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrZ {
	DOUBLE mCurrZ ;

	explicit GN_sqrt_mCurrZ (VREF<SyntaxTree> me) {
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrZ = 2 * mCurrX ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrY {
	DOUBLE mCurrY ;

	explicit GN_sqrt_mCurrY (VREF<SyntaxTree> me) {
		auto &mFirstY = me.value (TYPEAS<GN_sqrt_mFirstY>::id).mFirstY ;
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrY = MathProc::square (mCurrX) - mFirstY ;
		})) ;
	}
} ;

struct GN_sqrt_mNextX {
	DOUBLE mNextX ;

	explicit GN_sqrt_mNextX (VREF<SyntaxTree> me) {
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		auto &mCurrY = me.value (TYPEAS<GN_sqrt_mCurrY>::id).mCurrY ;
		auto &mCurrZ = me.value (TYPEAS<GN_sqrt_mCurrZ>::id).mCurrZ ;
		me.then (Function<void> ([&] () {
			mNextX = mCurrX - mCurrY * MathProc::inverse (mCurrZ) ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrTimes {
	LENGTH mCurrTimes ;

	explicit GN_sqrt_mCurrTimes (VREF<SyntaxTree> me) {
		me.mark_as_iteration () ;
		me.later (TYPEAS<GN_sqrt_mNextTimes>::id) ;
		me.then (Function<void> ([&] () {
			mCurrTimes = 0 ;
		})) ;
	}
} ;

struct GN_sqrt_mNextTimes {
	LENGTH mNextTimes ;

	explicit GN_sqrt_mNextTimes (VREF<SyntaxTree> me) {
		auto &mCurrTimes = me.value (TYPEAS<GN_sqrt_mCurrTimes>::id).mCurrTimes ;
		me.then (Function<void> ([&] () {
			mNextTimes = mCurrTimes + 1 ;
		})) ;
	}
} ;

struct GN_sqrt_mIteration {
	SharedRef<SyntaxTree> mIteration ;

	explicit GN_sqrt_mIteration (VREF<SyntaxTree> me) {
		me.maybe (TYPEAS<GN_sqrt_mNextX>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mNextTimes>::id) ;
		me.once (Function<void> ([&] () {
			mIteration = SharedRef<SyntaxTree>::make (me.thread ()) ;
			auto &mCurrY = mIteration->value (TYPEAS<GN_sqrt_mCurrY>::id).mCurrY ;
			auto &mCurrTimes = mIteration->value (TYPEAS<GN_sqrt_mCurrTimes>::id).mCurrTimes ;
			me.then (Function<void> ([&] () {
				while (TRUE) {
					mIteration->play () ;
					if (MathProc::abs (mCurrY) < DOUBLE (1E-9))
						discard ;
					if (mCurrTimes >= 10000)
						discard ;
					mIteration->undo (TYPEAS<GN_sqrt_mCurrX>::id) ;
					mIteration->undo (TYPEAS<GN_sqrt_mCurrTimes>::id) ;
				}
			})) ;
		})) ;
	}
} ;

struct GN_sqrt_mFinalX {
	DOUBLE mFinalX ;

	explicit GN_sqrt_mFinalX (VREF<SyntaxTree> me) {
		auto &mIteration = me.value (TYPEAS<GN_sqrt_mIteration>::id).mIteration ;
		me.once (Function<void> ([&] () {
			auto &mCurrX = mIteration->value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
			me.then (Function<void> ([&] () {
				mFinalX = mCurrX ;
			})) ;
		})) ;
	}
} ;



#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	ThisModule::hook () ;

	assert (TRUE) ;
	return 0 ;
}
#endif