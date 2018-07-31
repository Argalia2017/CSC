#include "util.h"
#include <iostream>
#include <iomanip>
using namespace std ;

//@info:static field of module 'main.cpp'
static const CSC::UniqueRef<void> ANONYMOUS ([] () {
	using namespace UNITTEST ;
	Singleton<DebuggerService>::instance ().check_memory_leaks (TRUE) ;
	_UNITTEST_PRINTS_ (_PCSTR_ ("static constructor for main.cpp")) ;
} ,[] () {
	using namespace UNITTEST ;
	_UNITTEST_PRINTS_ (_PCSTR_ ("static destructor for main.cpp")) ;
}) ;

TEST_CLASS (UNITTEST_MAIN) {
public:
	TEST_METHOD (TEST_MAIN) {
		using namespace UNITTEST ;

	}
} ;

#ifdef __CSC_TARGET_EXE__
export int main () {
	UNITTEST_MAIN ().TEST_MAIN () ;
	return 0 ;
}
#endif

#pragma region
#pragma push_macro ("_STRICTHPPINL")
#pragma push_macro ("__CSC_STRICTHPPINL__")
#undef _STRICTHPPINL
#undef __CSC_STRICTHPPINL__

#ifdef __CSC_MATH__
#include "csc_math.hpp.inl"
#endif

#ifdef __CSC_IMAGES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_images.hpp.opencv.inl"
#include "csc_images.hpp.freeimage.inl"
#endif
#endif
#endif

#ifdef __CSC_GRAPHICS__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_graphics.hpp.opengl.inl"
#endif
#endif
#endif

#ifdef __CSC_STREAM__
#include "csc_stream.hpp.inl"
#endif

#ifdef __CSC_STRING__
#include "csc_string.hpp.inl"
#endif

#ifdef __CSC_FILESYSTEM__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_filesystem.hpp.windows.inl"
#elif defined __CSC_SYSTEM_LINUX__
#include "csc_filesystem.hpp.linux.inl"
#endif
#endif

#ifdef __CSC_NETWORK__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_network.hpp.windows.inl"
#endif
#endif

#ifdef __CSC_DATABASES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_databases.hpp.mysql.inl"
#endif
#endif
#endif

#ifdef __CSC_DEBUGGER__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_debugger.hpp.windows.inl"
#elif defined __CSC_SYSTEM_LINUX__
#include "csc_debugger.hpp.linux.inl"
#endif
#endif

#pragma pop_macro ("_STRICTHPPINL")
#pragma pop_macro ("__CSC_STRICTHPPINL__")
#pragma endregion