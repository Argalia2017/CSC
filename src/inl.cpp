#include "util.h"

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#pragma warning (disable :4091)
#pragma warning (disable :4996)
#pragma warning (disable :5039)
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <crtdbg.h>
#include <signal.h>
#include <DbgHelp.h>
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <dlfcn.h>
#endif

#include "csc/csc_core.hpp.default.inl"
#include "csc/csc_math.hpp.default.inl"
#include "csc/csc_string.hpp.default.inl"
#include "csc/csc_string.hpp.gbk.inl"

#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc/csc_runtime.hpp.windows.inl"
#include "csc/csc_filesystem.hpp.windows.inl"
#include "csc/csc_debugger.hpp.windows.inl"
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include "csc/csc_runtime.hpp.linux.inl"
#include "csc/csc_filesystem.hpp.linux.inl"
#include "csc/csc_debugger.hpp.linux.inl"
#endif

namespace UNITTEST {
exports void DumpMemoryLeaksAtExit () {
#ifdef __CSC_COMPILER_MSVC__
	const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
	noop (r1x) ;
	const auto r2x = CSC::VAL32 (r1x | _CRTDBG_LEAK_CHECK_DF) ;
	noop (r2x) ;
	const auto r3x = _CrtSetDbgFlag (r2x) ;
	noop (r3x) ;
#endif
}
} ;