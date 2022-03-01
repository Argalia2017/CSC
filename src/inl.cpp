#include "util.h"

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

#ifdef __CSC_SYSTEM_LINUX__
#ifdef __CSC_CONFIG_STRA__
#define TEXT(...) __VA_ARGS__
#elif defined __CSC_CONFIG_STRW__
#define TEXT(...) __macro_cat (L ,__VA_ARGS__)
#endif
#endif

#include "csc/csc_core.hpp.default.inl"
#include "csc/csc_math.hpp.default.inl"
#include "csc/csc_string.hpp.gbks.inl"
#include "csc/csc_string.hpp.default.inl"
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