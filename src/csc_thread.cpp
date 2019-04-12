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
		auto rax = Promise<int>::async ([&] () {
			return A ().work () ;
		}) ;
		const auto r4x = rax.poll (Optional<int>::nullopt ()) ;
		_UNITTEST_ASSERT_ (!r4x.exist ()) ;
		INDEX ir = 0 ;
		while (!rax.ready ()) {
			ir++ ;
			std::this_thread::sleep_for (std::chrono::milliseconds (10)) ;
		}
		_UNITTEST_ASSERT_ (_ABS_ (ir - 10) < 2) ;
		const auto r5x = rax.poll (Optional<int>::nullopt ()).value (0) ;
		_UNITTEST_ASSERT_ (r5x == 3) ;
		const auto r6x = rax.poll () ;
		_UNITTEST_ASSERT_ (r6x == 3) ;
	}

	TEST_METHOD (TEST_CSC_THREAD_PROMISE) {
		auto rax = CSC::Promise<int>::async ([] () {
			std::this_thread::sleep_for (std::chrono::milliseconds (10)) ;
			return 1 ;
		}) ;
		auto rbx = AutoRef<std::atomic<int>>::make (0) ;
		rax.then ([&] (int &item) {
			rbx.self = item ;
		}) ;
		std::this_thread::sleep_for (std::chrono::milliseconds (100)) ;
		_UNITTEST_ASSERT_ (rbx.self == 1) ;
	}
} ;
} ;
#endif