#include "util.h"

using namespace UNITTEST ;

namespace UNITTEST {
struct GN_sqrt_mFirstY {
	UniqueRef<> mInfo ;
	DOUBLE mFirstY ;

	explicit GN_sqrt_mFirstY (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mFirstY")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mFirstY")) ;
		}) ;
		me.then (Function<void> ([&] () {
			mFirstY = 726 ;
		})) ;
	}
} ;

struct GN_sqrt_mNextX ;

struct GN_sqrt_mCurrX {
	UniqueRef<> mInfo ;
	DOUBLE mCurrX ;

	explicit GN_sqrt_mCurrX (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mCurrX")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mCurrX")) ;
		}) ;
		me.mark_as_iteration () ;
		auto &mFirstY = me.value (TYPEAS<GN_sqrt_mFirstY>::id).mFirstY ;
		me.once (Function<void> ([&] () {
			mCurrX = mFirstY ;
			me.later (TYPEAS<GN_sqrt_mNextX>::id) ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrY {
	UniqueRef<> mInfo ;
	DOUBLE mCurrY ;

	explicit GN_sqrt_mCurrY (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mCurrY")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mCurrY")) ;
		}) ;
		auto &mFirstY = me.value (TYPEAS<GN_sqrt_mFirstY>::id).mFirstY ;
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrY = MathProc::square (mCurrX) - mFirstY ;
			Console::instance ().print (slice ("mCurrY = ") ,mCurrY) ;
		})) ;
	}
} ;

struct GN_sqrt_mCurrZ {
	UniqueRef<> mInfo ;
	DOUBLE mCurrZ ;

	explicit GN_sqrt_mCurrZ (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mCurrZ")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mCurrZ")) ;
		}) ;
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrZ = 2 * mCurrX ;
		})) ;
	}
} ;

struct GN_sqrt_mNextX {
	UniqueRef<> mInfo ;
	DOUBLE mNextX ;

	explicit GN_sqrt_mNextX (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mNextX")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mNextX")) ;
		}) ;
		auto &mCurrX = me.value (TYPEAS<GN_sqrt_mCurrX>::id).mCurrX ;
		auto &mCurrY = me.value (TYPEAS<GN_sqrt_mCurrY>::id).mCurrY ;
		auto &mCurrZ = me.value (TYPEAS<GN_sqrt_mCurrZ>::id).mCurrZ ;
		me.then (Function<void> ([&] () {
			mNextX = mCurrX - mCurrY * MathProc::inverse (mCurrZ) ;
			Console::instance ().print (slice ("mNextX = ") ,mNextX) ;
		})) ;
	}
} ;

struct GN_sqrt_mNextTimes ;

struct GN_sqrt_mCurrTimes {
	UniqueRef<> mInfo ;
	LENGTH mCurrTimes ;

	explicit GN_sqrt_mCurrTimes (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mCurrTimes")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mCurrTimes")) ;
		}) ;
		me.mark_as_iteration () ;
		me.once (Function<void> ([&] () {
			mCurrTimes = 0 ;
			me.later (TYPEAS<GN_sqrt_mNextTimes>::id) ;
		})) ;
	}
} ;

struct GN_sqrt_mNextTimes {
	UniqueRef<> mInfo ;
	LENGTH mNextTimes ;

	explicit GN_sqrt_mNextTimes (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mNextTimes")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mNextTimes")) ;
		}) ;
		auto &mCurrTimes = me.value (TYPEAS<GN_sqrt_mCurrTimes>::id).mCurrTimes ;
		me.then (Function<void> ([&] () {
			mNextTimes = mCurrTimes + 1 ;
		})) ;
	}
} ;

struct GN_sqrt_mIteration {
	UniqueRef<> mInfo ;

	explicit GN_sqrt_mIteration (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mIteration")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mIteration")) ;
		}) ;
		me.once (Function<void> ([&] () {

		})) ;
	}
} ;

struct GN_sqrt_mFinalX {
	UniqueRef<> mInfo ;
	DOUBLE mFinalX ;

	explicit GN_sqrt_mFinalX (VREF<SyntaxTree> me) {
		mInfo = UniqueRef<> ([] () {
			Console::instance ().print (slice ("CREATE GN_sqrt_mFinalX")) ;
		} ,[] () {
			Console::instance ().print (slice ("DESTROY GN_sqrt_mFinalX")) ;
		}) ;
		me.once (Function<void> ([&] () {

		})) ;
	}
} ;
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () {
	Reporter::instance ().detect_memory_leaks () ;

	//private:
	//imports (?!\w+(<[^\r]+>)? (from|make(_\w+)?|invoke) \(|[^\r]+\{\r\n\t+return instance \(\)\.mThis->\w+|void \w+ \(CREF<PlaceHolder<\w+>>\) \{|CREF<\w+> \w+ \(\) \{)

	assert (TRUE) ;
	return 0 ;
}
#endif