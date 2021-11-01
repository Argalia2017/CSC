#include "util.h"

#include "csc/begin.h"
#include <random>
#include "csc/end.h"

namespace UNITTEST {
struct A {} ;
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;

	const auto r4x = Array<INDEX> ({1 ,2 ,3}) ;
	auto rax = BitSet<> (10 ,r4x) ;
	rax[1] = TRUE ;

	assert (TRUE) ;
	return 0 ;
}
#endif