#include "util.h"

using namespace UNITTEST ;

namespace UNITTEST {

template <class U>
struct GN_sqrt_mFirstY {
	DOUBLE mFirstY ;
	Scope<GN_sqrt_mFirstY> mHandle ;

	explicit GN_sqrt_mFirstY (VREF<SyntaxTree> me) {
		me.mark_as_iteration () ;
		me.then (Function<void> ([&] () {
			mFirstY = me.later (TYPEAS<DOUBLE>::id) ;
			Singleton<Console>::instance ().print (slice ("mFirstY = ") ,mFirstY) ;
		})) ;
		mHandle = Scope<GN_sqrt_mFirstY> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mFirstY[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mFirstY[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mCurrX {
	DOUBLE mCurrX ;
	Scope<GN_sqrt_mCurrX> mHandle ;

	explicit GN_sqrt_mCurrX (VREF<SyntaxTree> me) {
		me.mark_as_iteration () ;
		me.then (Function<void> ([&] () {
			mCurrX = me.later (TYPEAS<DOUBLE>::id) ;
		})) ;
		mHandle = Scope<GN_sqrt_mCurrX> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mCurrX[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mCurrX[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mCurrY {
	DOUBLE mCurrY ;
	Scope<GN_sqrt_mCurrY> mHandle ;

	explicit GN_sqrt_mCurrY (VREF<SyntaxTree> me) {
		auto &mFirstY = me.stack (TYPEAS<GN_sqrt_mFirstY<U>>::id).mFirstY ;
		auto &mCurrX = me.stack (TYPEAS<GN_sqrt_mCurrX<U>>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrY = MathProc::square (mCurrX) - mFirstY ;
			Singleton<Console>::instance ().print (slice ("mCurrY = ") ,mCurrY) ;
		})) ;
		mHandle = Scope<GN_sqrt_mCurrY> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mCurrY[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mCurrY[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mCurrZ {
	DOUBLE mCurrZ ;
	Scope<GN_sqrt_mCurrZ> mHandle ;

	explicit GN_sqrt_mCurrZ (VREF<SyntaxTree> me) {
		auto &mCurrX = me.stack (TYPEAS<GN_sqrt_mCurrX<U>>::id).mCurrX ;
		me.then (Function<void> ([&] () {
			mCurrZ = 2 * mCurrX ;
		})) ;
		mHandle = Scope<GN_sqrt_mCurrZ> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mCurrZ[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mCurrZ[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mNextX {
	DOUBLE mNextX ;
	Scope<GN_sqrt_mNextX> mHandle ;

	explicit GN_sqrt_mNextX (VREF<SyntaxTree> me) {
		auto &mCurrX = me.stack (TYPEAS<GN_sqrt_mCurrX<U>>::id).mCurrX ;
		auto &mCurrY = me.stack (TYPEAS<GN_sqrt_mCurrY<U>>::id).mCurrY ;
		auto &mCurrZ = me.stack (TYPEAS<GN_sqrt_mCurrZ<U>>::id).mCurrZ ;
		me.then (Function<void> ([&] () {
			mNextX = mCurrX - mCurrY * MathProc::inverse (mCurrZ) ;
		})) ;
		mHandle = Scope<GN_sqrt_mNextX> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mNextX[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mNextX[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mCurrTimes {
	LENGTH mCurrTimes ;
	Scope<GN_sqrt_mCurrTimes> mHandle ;

	explicit GN_sqrt_mCurrTimes (VREF<SyntaxTree> me) {
		me.mark_as_iteration () ;
		me.then (Function<void> ([&] () {
			mCurrTimes = me.later (TYPEAS<LENGTH>::id) ;
		})) ;
		mHandle = Scope<GN_sqrt_mCurrTimes> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mCurrTimes[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mCurrTimes[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mNextTimes {
	LENGTH mNextTimes ;
	Scope<GN_sqrt_mNextTimes> mHandle ;

	explicit GN_sqrt_mNextTimes (VREF<SyntaxTree> me) {
		auto &mCurrTimes = me.stack (TYPEAS<GN_sqrt_mCurrTimes<U>>::id).mCurrTimes ;
		me.then (Function<void> ([&] () {
			mNextTimes = mCurrTimes + 1 ;
		})) ;
		mHandle = Scope<GN_sqrt_mNextTimes> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mNextTimes[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mNextTimes[") ,U::value ,slice ("]")) ;
	}
} ;

template <class U>
struct GN_sqrt_mFinalX {
	DOUBLE mFinalX ;
	LENGTH mFinalTimes ;
	Scope<GN_sqrt_mFinalX> mHandle ;

	explicit GN_sqrt_mFinalX (VREF<SyntaxTree> me) {
		me.maybe (TYPEAS<GN_sqrt_mFirstY<U>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mCurrX<U>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mNextX<U>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mCurrY<U>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mCurrTimes<U>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mNextTimes<U>>::id) ;
		me.once (Function<void> ([&] () {
			auto &mFirstY = me.stack (TYPEAS<GN_sqrt_mFirstY<U>>::id).mFirstY ;
			auto &mCurrX = me.stack (TYPEAS<GN_sqrt_mCurrX<U>>::id).mCurrX ;
			auto &mNextX = me.stack (TYPEAS<GN_sqrt_mNextX<U>>::id).mNextX ;
			auto &mCurrY = me.stack (TYPEAS<GN_sqrt_mCurrY<U>>::id).mCurrY ;
			auto &mCurrTimes = me.stack (TYPEAS<GN_sqrt_mCurrTimes<U>>::id).mCurrTimes ;
			auto &mNextTimes = me.stack (TYPEAS<GN_sqrt_mNextTimes<U>>::id).mNextTimes ;
			me.redo (TYPEAS<GN_sqrt_mCurrX<U>>::id ,mFirstY) ;
			me.redo (TYPEAS<GN_sqrt_mCurrTimes<U>>::id ,ZERO) ;
			while (TRUE) {
				me.play () ;
				if (MathProc::abs (mCurrY) < DOUBLE (SINGLE_EPS))
					break ;
				if (mCurrTimes >= 100)
					break ;
				me.undo (TYPEAS<GN_sqrt_mCurrX<U>>::id) ;
				me.undo (TYPEAS<GN_sqrt_mCurrTimes<U>>::id) ;
				me.redo (TYPEAS<GN_sqrt_mCurrX<U>>::id ,mNextX) ;
				me.redo (TYPEAS<GN_sqrt_mCurrTimes<U>>::id ,mNextTimes) ;
			}
			me.then (Function<void> ([&] () {
				mFinalX = mCurrX ;
				mFinalTimes = mCurrTimes ;
				Singleton<Console>::instance ().print (slice ("mFinalX = ") ,mFinalX) ;
				Singleton<Console>::instance ().print (slice ("mFinalTimes = ") ,mFinalTimes) ;
			})) ;
		})) ;
		mHandle = Scope<GN_sqrt_mFinalX> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mFinalX[") ,U::value ,slice ("]")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mFinalX[") ,U::value ,slice ("]")) ;
	}
} ;

struct GN_sqrt_mFirstY_thread {
	Array<DOUBLE> mFirstY ;
	Scope<GN_sqrt_mFirstY_thread> mHandle ;

	explicit GN_sqrt_mFirstY_thread (VREF<SyntaxTree> me) {
		me.then (Function<void> ([&] () {
			mFirstY = Array<DOUBLE> (10) ;
			for (auto &&i : mFirstY) {
				const auto r1x = Random::instance ().random_value (100 ,10000) ;
				i = DOUBLE (r1x) ;
			}
		})) ;
		mHandle = Scope<GN_sqrt_mFirstY_thread> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mFirstY_thread")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mFirstY_thread")) ;
	}
} ;

struct GN_sqrt_mFinalX_thread {
	Array<DOUBLE> mFinalX ;
	Scope<GN_sqrt_mFinalX_thread> mHandle ;

	explicit GN_sqrt_mFinalX_thread (VREF<SyntaxTree> me) {
		auto &&mFirstY = me.stack (TYPEAS<GN_sqrt_mFirstY_thread>::id).mFirstY ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK0>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK1>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK2>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK3>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK4>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK5>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK6>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK7>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK8>>::id) ;
		me.maybe (TYPEAS<GN_sqrt_mFinalX<RANK9>>::id) ;
		me.once (Function<void> ([&] () {
			mFinalX = Array<DOUBLE> (mFirstY.length ()) ;
			process<RANK0> (me ,mFirstY ,mFinalX) ;
			process<RANK1> (me ,mFirstY ,mFinalX) ;
			process<RANK2> (me ,mFirstY ,mFinalX) ;
			process<RANK3> (me ,mFirstY ,mFinalX) ;
			process<RANK4> (me ,mFirstY ,mFinalX) ;
			process<RANK5> (me ,mFirstY ,mFinalX) ;
			process<RANK6> (me ,mFirstY ,mFinalX) ;
			process<RANK7> (me ,mFirstY ,mFinalX) ;
			process<RANK8> (me ,mFirstY ,mFinalX) ;
			process<RANK9> (me ,mFirstY ,mFinalX) ;
			me.then (Function<void> ([&] () {
				Singleton<Console>::instance ().print () ;
				for (auto &&i : mFinalX.iter ()) {
					Singleton<Console>::instance ().print (slice ("sqrt (") ,mFirstY[i] ,slice (") = ") ,mFinalX[i]) ;
				}
				Singleton<Console>::instance ().print () ;
			})) ;
		})) ;
		mHandle = Scope<GN_sqrt_mFinalX_thread> (thiz) ;
	}

	void enter () {
		Singleton<Console>::instance ().info (slice ("enter GN_sqrt_mFinalX_thread")) ;
	}

	void leave () {
		Singleton<Console>::instance ().info (slice ("leave GN_sqrt_mFinalX_thread")) ;
	}

	template <class U>
	imports void process (
		VREF<SyntaxTree> me ,
		CREF<Array<DOUBLE>> mFirstY_thread ,
		VREF<Array<DOUBLE>> mFinalX_thread) {
		auto &mFinalX = me.stack (TYPEAS<GN_sqrt_mFinalX<U>>::id).mFinalX ;
		me.redo (TYPEAS<GN_sqrt_mFirstY<U>>::id ,mFirstY_thread[U::value]) ;
		me.play () ;
		mFinalX_thread[U::value] = mFinalX ;
	}
} ;
} ;

exports void test_main () ;

#ifdef __CSC_TARGET_EXE__
exports int main () {
	Singleton<Reporter>::instance ().detect_memory_leaks () ;
	Singleton<Reporter>::instance ().detect_crash_signal () ;
	Singleton<Console>::instance ().open () ;
	Singleton<Console>::instance ().link (slice (".")) ;
	
	test_main () ;

	return 0 ;
}
#endif