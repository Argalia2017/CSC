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
		//(?<!(Detail|CSC) ){(\r\n[^\r]+)*(?<!private:)\r\n\t+class (?!Lambda {)
	}
} ;

inline static void main_startup () {
	using namespace UNITTEST ;
#ifdef __CSC_COMPILER_MSVC__
	Singleton<DebuggerService>::instance ().output_memory_leaks_report (TRUE) ;
#endif
	Singleton<ConsoleService>::instance ().log (_PCSTR_ ("UNITTEST") ,_PCSTR_ ("static constructor for main.cpp")) ;
}

inline static void main_shutdown () {
	using namespace UNITTEST ;
	Singleton<ConsoleService>::instance ().log (_PCSTR_ ("UNITTEST") ,_PCSTR_ ("static destructor for main.cpp")) ;
}

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept popping {
	using namespace UNITTEST ;
	UniqueRef<void> ANONYMOUS (&main_startup ,&main_shutdown) ;
	UNITTEST_MAIN ().TEST_MAIN () ;
	return 0 ;
}
#endif

#pragma region
#ifdef __CSC_MATH__
#include <csc_math.hpp.default.inl>
#endif

#ifdef __CSC_FUNCTIONAL__
#include <csc_functional.hpp.op.inl>
#endif

#ifdef __CSC_IMAGES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include <csc_images.hpp.opencv.inl>
#include <csc_images.hpp.freeimage.inl>
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
#elif defined __CSC_SYSTEM_LINUX__
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
#elif defined __CSC_SYSTEM_LINUX__
#include <csc_debugger.hpp.linux.inl>
#endif
#endif
#pragma endregion