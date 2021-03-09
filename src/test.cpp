#include "csc2/csc.hpp"
#include "csc2/csc_core.hpp"
#include "csc2/csc_basic.hpp"

using namespace CSC ;





#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	for (auto &&i : range (0 ,10)) {
		require (IS_SAME<typeof (i) ,INDEX>) ;
	}
	return 0 ;
}
#endif