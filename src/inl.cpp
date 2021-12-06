#include "util.h"

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
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