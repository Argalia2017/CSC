#include "util.h"

#include "csc2/begin.h"
#include <random>
#include "csc2/end.h"

namespace UNITTEST {
void print (VREF<Set<INDEX>> t) {
	const auto r1x = t.range_sort () ;
	printf_s ("\n[") ;
	auto rax = VAL_MIN ;
	for (auto &&i : r1x) {
		assert (rax < t[r1x[i]]) ;
		printf_s ("%lld," ,t[r1x[i]]) ;
		rax = t[r1x[i]] ;
	}
	printf_s ("]\n") ;
}
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;

	auto rax = Set<INDEX> (100) ;
	std::random_device rd ;
	for (auto &&i : range (0 ,100)) {
		const auto r1x = INDEX (rd ()) % 100 ;
		rax.add (r1x ,rax.length ()) ;
		if (i % 10 != 0)
			continue ;
		print (rax) ;
	}
	for (auto &&i : range (0 ,100)) {
		noop (i) ;
		const auto r1x = INDEX (rd ()) % 100 ;
		rax.erase (r1x) ;
		print (rax) ;
	}

	assert (TRUE) ;
	return 0 ;
}
#endif