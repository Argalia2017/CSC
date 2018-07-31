#include "util.h"

#ifdef __CSC_THREAD__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_THREAD) {
public:
	TEST_METHOD (TEST_CSC_THREAD) {
		struct A {
			inline int work () const {
				std::this_thread::sleep_for (std::chrono::seconds (1)) ;
				return 3 ;
			}
		} ;
		const auto r3x = Promise<int>::async ([&] () {
			return A ().work () ;
		}) ;
		const auto r4x = r3x.peek () ;
		_UNITTEST_ASSERT_ (!r4x.exist ()) ;
		auto rax = VAR_ZERO ;
		while (!r3x.ready ()) {
			rax++ ;
			std::this_thread::sleep_for (std::chrono::milliseconds (100)) ;
		}
		_UNITTEST_ASSERT_ (_ABS_ (rax - 10) < 2) ;
		const auto r5x = r3x.peek ().value (0) ;
		_UNITTEST_ASSERT_ (r5x == 3) ;
		const auto r6x = r3x.get () ;
		_UNITTEST_ASSERT_ (r6x == 3) ;
	}
} ;
} ;
#endif