#include "util.h"

#ifdef __CSC_THREAD__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_THREAD) {
public:
	TEST_METHOD (TEST_CSC_THREAD) {
		struct A :public Interface {
			inline int work () const {
				GlobalRuntime::thread_sleep (std::chrono::milliseconds (100)) ;
				return 3 ;
			}
		} ;
		const auto r1x = _XVALUE_<PTR<int ()>> ([] () {
			return A ().work () ;
		}) ;
		auto rax = Promise<int>::async (r1x) ;
		const auto r2x = rax.value (-1) ;
		_UNITTEST_ASSERT_ (r2x == -1) ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (rax.ready ())
				break ;
			GlobalRuntime::thread_sleep (std::chrono::milliseconds (10)) ;
			ix++ ;
		}
		_UNITTEST_ASSERT_ (_ABS_ (ix - 10) < 2) ;
		const auto r3x = rax.value (-1) ;
		_UNITTEST_ASSERT_ (r3x == 3) ;
		const auto r4x = rax.poll () ;
		_UNITTEST_ASSERT_ (r4x == 3) ;
	}

	TEST_METHOD (TEST_CSC_THREAD_PROMISE) {
		const auto r1x = _XVALUE_<PTR<int ()>> ([] () {
			GlobalRuntime::thread_sleep (std::chrono::milliseconds (10)) ;
			return 1 ;
		}) ;
		auto rax = Promise<int>::async (r1x) ;
		auto rbx = AutoRef<std::atomic<int>>::make (0) ;
		UniqueRef<PTR<Promise<int>::Future>> ANONYMOUS ([&] (PTR<Promise<int>::Future> &me) {
			me = &rax ;
		} ,[] (PTR<Promise<int>::Future> &me) {
			me->stop () ;
		}) ;
		const auto r2x = Function<void (int &)> ([&] (int &item) {
			rbx.self = item ;
		}) ;
		rax.then (Function<DEF<void (int &)> NONE::*> (PhanRef<const Function<void (int &)>>::make (r2x) ,&Function<void (int &)>::operator())) ;
		GlobalRuntime::thread_sleep (std::chrono::milliseconds (100)) ;
		_UNITTEST_ASSERT_ (rbx.self == 1) ;
	}
} ;
} ;
#endif