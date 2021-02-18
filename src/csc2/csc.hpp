#pragma once

#ifndef __CSC__
#define __CSC__
#endif

#ifdef _DEBUG
#define __CSC_DEBUG__
#endif

#ifdef _UNITTEST
#define __CSC_UNITTEST__
#endif

#ifdef __clang__
#define __CSC_COMPILER_CLANG__
#elif defined __GNUC__
#define __CSC_COMPILER_GNUC__
#elif defined _MSC_VER
#define __CSC_COMPILER_MSVC__
#else
#error "б╞(д├бузебу ;)д├ : unsupported"
#endif

#if defined (linux) || defined (__linux) || defined (__linux__)
#define __CSC_SYSTEM_LINUX__
#elif defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
#define __CSC_SYSTEM_WINDOWS__
#else
#error "б╞(д├бузебу ;)д├ : unsupported"
#endif

#if defined (_M_IX86) || defined (__i386__) || defined (__i386)
#define __CSC_PLATFORM_X86__
#elif defined (_M_X64) || defined (_M_AMD64) || defined (__x86_64) || defined (__amd64)
#define __CSC_PLATFORM_X64__
#elif defined (_M_ARM) || defined (__arm__)
#define __CSC_PLATFORM_ARM__
#elif defined (_M_ARM64) || defined (__aarch64__)
#define __CSC_PLATFORM_ARM64__
#else
#error "б╞(д├бузебу ;)д├ : unsupported"
#endif

#ifdef _WINEXE
#define __CSC_TARGET_EXE__
#elif defined _WINDLL
#define __CSC_TARGET_DLL__
#elif defined _WINLIB
#define __CSC_TARGET_LIB__
#else
#define __CSC_TARGET_EXE__
#endif

#ifdef _WIN64
#define __CSC_CONFIG_VAR64__
#elif defined _WIN32
#define __CSC_CONFIG_VAR32__
#else
#define __CSC_CONFIG_VAR64__
#endif

#ifdef _UNICODE
#define __CSC_CONFIG_STRW__
#elif defined _MBCS
#define __CSC_CONFIG_STRA__
#else
#define __CSC_CONFIG_STRA__
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4068) //@info: warning C4068: unknown pragma
#pragma warning (disable :4065) //@info: warning C4065: switch statement contains 'default' but no 'case' labels
#pragma warning (disable :4619) //@info: warning C4619: #pragma warning: there is no warning number 'xxx'
#pragma warning (disable :4100) //@info: warning C4100: 'xxx': unreferenced formal parameter
#pragma warning (disable :4180) //@info: warning C4180: qualifier applied to function type has no meaning; ignored
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4371) //@info: warning C4371: 'xxx': layout of class may have changed from a previous version of the compiler due to better packing of member 'xxx'
#pragma warning (disable :4435) //@info: warning C4435: 'xxx': Object layout under /vd2 will change due to virtual base 'xxx'
#pragma warning (disable :4464) //@info: warning C4464: relative include path contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced inline function has been removed
#pragma warning (disable :4571) //@info: warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning (disable :4574) //@info: warning C4574: 'xxx' is defined to be '0': did you mean to use '#if xxx'?
#pragma warning (disable :4668) //@info: warning C4668: 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning (disable :4710) //@info: warning C4710: 'xxx': function not inlined
#pragma warning (disable :4711) //@info: warning C4711: function 'xxx' selected for automatic inline expansion
#pragma warning (disable :4738) //@info: warning C4738: storing 32-bit float result in memory, possible loss of performance
#pragma warning (disable :4774) //@info: warning C4774: 'xxx' : format string expected in argument ? is not a string literal
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4820) //@info: warning C4820: 'xxx': 'xxx' bytes padding added after data member 'xxx'
#pragma warning (disable :4623) //@info: warning C4623: 'xxx': default constructor was implicitly defined as deleted
#pragma warning (disable :4624) //@info: warning C4624: 'xxx': destructor was implicitly defined as deleted
#pragma warning (disable :4625) //@info: warning C4625: 'xxx': copy constructor was implicitly defined as deleted
#pragma warning (disable :4626) //@info: warning C4626: 'xxx': assignment operator was implicitly defined as deleted
#pragma warning (disable :5026) //@info: warning C5026: 'xxx': move constructor was implicitly defined as deleted
#pragma warning (disable :5027) //@info: warning C5027: 'xxx': move assignment operator was implicitly defined as deleted
#pragma warning (disable :5045) //@info: warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#endif

#include "begin.hh"
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <new>
#include <exception>
#include "end.hh"

#ifdef _HAS_CXX17
#if _HAS_CXX17
#define __CSC_CXX_LATEST__
#endif
#endif

namespace CSC {
/*
*	MIT License
*
*	Copyright (c) 2018 csc-std
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef NULL
#undef NULL
#endif

#define infinity std::numeric_limits<SINGLE>::infinity ()

#define internel_unwind_impl(...) __VA_ARGS__
#define unwind internel_unwind_impl

#define internel_not_impl(...) (!(unwind (__VA_ARGS__)))
#define not internel_not_impl

#define internel_require_impl(...) static_assert ((unwind (__VA_ARGS__)::value) ,"error") ;
#define require internel_require_impl

#define internel_enumof_impl(...) CSC::detail::INTERNEL_ENUM<(unwind (__VA_ARGS__))>
#define enumof internel_enumof_impl

#define internel_typeof_impl(...) CSC::detail::INTERNEL_TYPE<(unwind (__VA_ARGS__))>
#define typeof internel_typeof_impl

#define typeas CSC::detail::INTERNEL_TYPE
} ;