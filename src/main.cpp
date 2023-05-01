//#include "util.h"
//
//#include "csc/csc_end.h"
//#ifdef __CSC_COMPILER_GNUC__
//#include "inl.cpp"
//#endif
//#include "csc/csc_begin.h"
//
//using namespace UNITTEST ;
//
//struct G_mY0 {
//	DOUBLE mY0 ;
//
//	explicit G_mY0 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		me.then (Function<void> ([&] () {
//			mY0 = me.later (TYPEAS<DOUBLE>::expr) ;
//		})) ;
//	}
//} ;
//
//struct G_mX1 {
//	DOUBLE mX1 ;
//
//	explicit G_mX1 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mY0 = me.stack (TYPEAS<G_mY0>::expr).mY0 ;
//		me.then (Function<void> ([&] () {
//			noop (mY0) ;
//			mX1 = me.later (TYPEAS<DOUBLE>::expr) ;
//		})) ;
//	}
//} ;
//
//struct G_mY1 {
//	DOUBLE mY1 ;
//
//	explicit G_mY1 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mY0 = me.stack (TYPEAS<G_mY0>::expr).mY0 ;
//		auto &mX1 = me.stack (TYPEAS<G_mX1>::expr).mX1 ;
//		me.then (Function<void> ([&] () {
//			mY1 = MathProc::square (mX1) - mY0 ;
//		})) ;
//	}
//} ;
//
//struct G_mZ1 {
//	DOUBLE mZ1 ;
//
//	explicit G_mZ1 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mX1 = me.stack (TYPEAS<G_mX1>::expr).mX1 ;
//		me.then (Function<void> ([&] () {
//			mZ1 = 2 * mX1 ;
//		})) ;
//	}
//} ;
//
//struct G_mW0 {
//	DOUBLE mW0 ;
//
//	explicit G_mW0 (VREF<SyntaxTree> me) {
//		me.then (Function<void> ([&] () {
//			mW0 = 1 ;
//		})) ;
//	}
//} ;
//
//struct G_mW1 {
//	DOUBLE mW1 ;
//
//	explicit G_mW1 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mW0 = me.stack (TYPEAS<G_mW0>::expr).mW0 ;
//		me.then (Function<void> ([&] () {
//			noop (mW0) ;
//			mW1 = me.later (TYPEAS<DOUBLE>::expr) ;
//		})) ;
//	}
//} ;
//
//struct G_mW2 {
//	DOUBLE mW2 ;
//
//	explicit G_mW2 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mW1 = me.stack (TYPEAS<G_mW1>::expr).mW1 ;
//		me.then (Function<void> ([&] () {
//			mW2 = mW1 * (1 - DOUBLE (1E-3)) ;
//		})) ;
//	}
//} ;
//
//struct G_mX2 {
//	DOUBLE mX2 ;
//
//	explicit G_mX2 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mX1 = me.stack (TYPEAS<G_mX1>::expr).mX1 ;
//		auto &mY1 = me.stack (TYPEAS<G_mY1>::expr).mY1 ;
//		auto &mZ1 = me.stack (TYPEAS<G_mZ1>::expr).mZ1 ;
//		auto &mW1 = me.stack (TYPEAS<G_mW1>::expr).mW1 ;
//		me.then (Function<void> ([&] () {
//			mX2 = mX1 - mY1 / mZ1 * mW1 ;
//		})) ;
//	}
//} ;
//
//struct G_mX3 {
//	DOUBLE mX3 ;
//
//	explicit G_mX3 (VREF<SyntaxTree> me) {
//		me.mark_as_iteration () ;
//		auto &mY0 = me.stack (TYPEAS<G_mY0>::expr).mY0 ;
//		auto &mW0 = me.stack (TYPEAS<G_mW0>::expr).mW0 ;
//		me.maybe (TYPEAS<G_mX1>::expr) ;
//		me.maybe (TYPEAS<G_mX2>::expr) ;
//		me.maybe (TYPEAS<G_mY1>::expr) ;
//		me.maybe (TYPEAS<G_mW1>::expr) ;
//		me.maybe (TYPEAS<G_mW2>::expr) ;
//		me.once (Function<void> ([&] () {
//			auto &mX1 = me.stack (TYPEAS<G_mX1>::expr).mX1 ;
//			auto &mX2 = me.stack (TYPEAS<G_mX2>::expr).mX2 ;
//			auto &mY1 = me.stack (TYPEAS<G_mY1>::expr).mY1 ;
//			auto &mW1 = me.stack (TYPEAS<G_mW1>::expr).mW1 ;
//			auto &mW2 = me.stack (TYPEAS<G_mW2>::expr).mW2 ;
//			me.undo (TYPEAS<G_mX1>::expr) ;
//			me.undo (TYPEAS<G_mW1>::expr) ;
//			me.later (TYPEAS<G_mX1>::expr ,mY0) ;
//			me.later (TYPEAS<G_mW1>::expr ,mW0) ;
//			while (TRUE) {
//				me.play () ;
//				if (MathProc::abs (mW1) < DOUBLE (1E-9))
//					break ;
//				if (MathProc::abs (mY1) < DOUBLE (1E-9))
//					break ;
//				me.undo (TYPEAS<G_mX1>::expr) ;
//				me.undo (TYPEAS<G_mW1>::expr) ;
//				me.later (TYPEAS<G_mX1>::expr ,mX2) ;
//				me.later (TYPEAS<G_mW1>::expr ,mW2) ;
//			}
//			mX3 = mX1 ;
//		})) ;
//	}
//} ;
//
//struct G_mYa {
//	Array<DOUBLE> mYa ;
//
//	explicit G_mYa (VREF<SyntaxTree> me) {
//		me.then (Function<void> ([&] () {
//			mYa = Array<DOUBLE> (4) ;
//			mYa[0] = 26 ;
//			mYa[1] = 54 ;
//			mYa[2] = 22 ;
//			mYa[3] = 90 ;
//		})) ;
//	}
//} ;
//
//struct G_mXn {
//	Array<DOUBLE> mXn ;
//
//	explicit G_mXn (VREF<SyntaxTree> me) {
//		me.maybe (TYPEAS<G_mX3>::expr) ;
//		auto &mYa = me.stack (TYPEAS<G_mYa>::expr).mYa ;
//		me.once (Function<void> ([&] () {
//			auto &mX3 = me.stack (TYPEAS<G_mX3>::expr).mX3 ;
//			mXn = Array<DOUBLE> (mYa.length ()) ;
//			for (auto &&i : mYa.iter ()) {
//				me.undo (TYPEAS<G_mY0>::expr) ;
//				me.later (TYPEAS<G_mY0>::expr ,mYa[i]) ;
//				me.play () ;
//				mXn[i] = mX3 ;
//			}
//		})) ;
//	}
//} ;
//
//struct G_mXn_thread {
//	Array<DOUBLE> mXn ;
//
//	explicit G_mXn_thread (VREF<SyntaxTree> me) {
//		me.maybe (TYPEAS<G_mX3>::expr) ;
//		auto &mYa = me.stack (TYPEAS<G_mYa>::expr).mYa ;
//		me.once (Function<void> ([&] () {
//			auto &mX3 = me.stack (TYPEAS<G_mX3>::expr).mX3 ;
//			for (auto &&i : mYa.iter ()) {
//				me.undo (TYPEAS<G_mY0>::expr) ;
//				me.later (TYPEAS<G_mY0>::expr ,mYa[i]) ;
//				me.play () ;
//				mXn[i] = mX3 ;
//			}
//		})) ;
//	}
//} ;
//
//struct G_mYn {
//	Array<DOUBLE> mYn ;
//
//	explicit G_mYn (VREF<SyntaxTree> me) {
//		auto &mXn = me.stack (TYPEAS<G_mXn>::expr).mXn ;
//		me.then (Function<void> ([&] () {
//			mYn = Array<DOUBLE> (mXn.length ()) ;
//			for (auto &&i : mXn.iter ())
//				mYn[i] = MathProc::square (mXn[i]) ;
//		})) ;
//	}
//} ;
//
//exports void test_main () ;
//
//#ifdef __CSC_TARGET_EXE__
//exports int main () {
//	Singleton<Reporter>::instance ().detect_memory_leaks () ;
//	Singleton<Reporter>::instance ().detect_crash_signal () ;
//	Singleton<Console>::instance ().show () ;
//	Singleton<Console>::instance ().open (slice (".")) ;
//	Singleton<Console>::instance ().info (slice ("start")) ;
//
//	auto rax = SyntaxTree (TRUE) ;
//	auto &mXn = rax.stack (TYPEAS<G_mXn>::expr).mXn ;
//	auto &mYn = rax.stack (TYPEAS<G_mYn>::expr).mYn ;
//	rax.play () ;
//	for (auto &&i : mXn.iter ()) {
//		Singleton<Console>::instance ().print (slice ("square (") ,mXn[i] ,slice (") = ") ,mYn[i]) ;
//	}
//	unittest (rax) ;
//	test_main () ;
//
//	return 0 ;
//}
//#endif