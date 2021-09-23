#include "util.h"

namespace UNITTEST {

} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;

	return 0 ;
}
#endif

#include "csc2/csc_core.hpp.inl"