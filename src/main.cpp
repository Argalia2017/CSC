#include "util.h"

#include <iostream>
#include <iomanip>
using namespace std ;

namespace UNITTEST {

} ;

TEST_CLASS (UNITTEST_MAIN) {
public:
	TEST_METHOD (TEST_MAIN) {
		using namespace UNITTEST ;

	}
} ;

imports void main_startup () {
	using namespace UNITTEST ;
#ifdef __CSC_COMPILER_MSVC__
	Singleton<DebuggerService>::instance ().output_memory_leaks_report (TRUE) ;
#endif
	Singleton<ConsoleService>::instance ().log (_PCSTR_ ("UNITTEST") ,_PCSTR_ ("main_startup")) ;
}

imports void main_shutdown () {
	using namespace UNITTEST ;
	Singleton<ConsoleService>::instance ().log (_PCSTR_ ("UNITTEST") ,_PCSTR_ ("main_shutdown")) ;
}

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;
	UniqueRef<> ANONYMOUS (DEPTR[main_startup] ,DEPTR[main_shutdown]) ;
	try {
		UNITTEST_MAIN ().TEST_MAIN () ;
	} catch (const Exception &e) {
		const auto r1x = String<STR> (e.what ()) ;
		Singleton<ConsoleService>::instance ().fatal (r1x) ;
#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_MSVC__
		const auto r2x = StringProc::build_strs (ARGV<STRW>::ID ,r1x) ;
		Assert::Fail (r2x.raw ().self) ;
#endif
#endif
	}
	return 0 ;
}
#endif

#ifdef __CSC__
#ifdef __CSC_MATH__
#include <csc_math.hpp.default.inl>
#endif

#ifdef __CSC_IMAGES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#ifdef OPENCV
#include <csc_images.hpp.opencv.inl>
#else
#include <csc_images.hpp.freeimage.inl>
#endif
#endif
#endif
#endif

#ifdef __CSC_STRING__
#include <csc_string.hpp.default.inl>
#include <csc_string.hpp.gbk.inl>
#endif

#ifdef __CSC_RUNTIME__
#include <csc_runtime.hpp.default.inl>
#endif

#ifdef __CSC_GRAPHICS__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include <csc_graphics.hpp.opengl.inl>
#endif
#endif
#endif

#ifdef __CSC_FILESYSTEM__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_filesystem.hpp.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_filesystem.hpp.linux.inl>
#endif
#endif

#ifdef __CSC_NETWORK__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_network.hpp.windows.inl>
#endif
#endif

#ifdef __CSC_DATABASES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include <csc_databases.hpp.mysql.inl>
#endif
#endif
#endif

#ifdef __CSC_DEBUGGER__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_debugger.hpp.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_debugger.hpp.linux.inl>
#endif
#endif
#endif