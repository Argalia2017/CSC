#include "util.h"

#include "csc/begin.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_MSVC__
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
#endif
#include "csc/end.h"

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_GNUC__
#define no_comment_lib_dbghelp
#endif
#endif

#include "csc/csc_core.hpp.default.inl"
#include "csc/csc_basic.hpp.default.inl"
#include "csc/csc_math.hpp.default.inl"
#include "csc/csc_stream.hpp.default.inl"
#include "csc/csc_string.hpp.gbks.inl"
#include "csc/csc_string.hpp.default.inl"
#include "csc/csc_serialization.hpp.default.inl"
#include "csc/csc_algorithm.hpp.default.inl"
#include "csc/csc_functional.hpp.default.inl"
#include "csc/csc_thread.hpp.default.inl"
#include "csc/csc_runtime.hpp.default.inl"

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