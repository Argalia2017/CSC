#include "util.h"

#ifdef __CSC_THREAD__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_THREAD) {
public:
	TEST_METHOD (TEST_CSC_THREAD) {
		struct A {
			inline int work () const {
				std::this_thread::sleep_for (std::chrono::milliseconds (100)) ;
				return 3 ;
			}
		} ;
		const auto r1x = _XVALUE_<PTR<int ()>> ([] () {
			return A ().work () ;
		}) ;
		auto rax = Promise<int>::async (r1x) ;
		const auto r4x = rax.value (-1) ;
		_UNITTEST_ASSERT_ (r4x == -1) ;
		INDEX ir = 0 ;
		while (TRUE) {
			if (rax.ready ())
				break ;
			ir++ ;
			std::this_thread::sleep_for (std::chrono::milliseconds (10)) ;
		}
		_UNITTEST_ASSERT_ (_ABS_ (ir - 10) < 2) ;
		const auto r5x = rax.value (-1) ;
		_UNITTEST_ASSERT_ (r5x == 3) ;
		const auto r6x = rax.poll () ;
		_UNITTEST_ASSERT_ (r6x == 3) ;
	}

	TEST_METHOD (TEST_CSC_THREAD_PROMISE) {
		const auto r1x = _XVALUE_<PTR<int ()>> ([] () {
			std::this_thread::sleep_for (std::chrono::milliseconds (10)) ;
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
		std::this_thread::sleep_for (std::chrono::milliseconds (100)) ;
		_UNITTEST_ASSERT_ (rbx.self == 1) ;
	}
} ;
} ;
#endif