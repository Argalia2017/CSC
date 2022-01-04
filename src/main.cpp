#include "util.h"

using namespace UNITTEST ;

namespace UNITTEST {
exports void DumpMemoryLeaksAtExit () ;

struct ThisModule {
	void enter () const {
		DumpMemoryLeaksAtExit () ;
	}

	void leave () const {
		noop () ;
	}

	imports void hook () {
		static const Scope<CREF<ThisModule>> anonymous (unsafe_array (TYPEAS<ThisModule>::id)[0]) ;
	}
} ;
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	ThisModule::hook () ;

	const auto r1x = BoxBuffer<STRU8 ,RANK9> () ;
	auto rax = TEMP<RegBuffer<STRU8>> () ;
	const auto r2x = XmlParser::make (RegBuffer<STRU8>::from (rax ,r1x ,0 ,r1x.size ())) ;

	assert (TRUE) ;
	return 0 ;
}
#endif