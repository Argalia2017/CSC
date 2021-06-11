#pragma once

#ifndef __CSC__
#define __CSC__
#endif

#ifdef _DEBUG
#define __CSC_DEBUG__
#elif defined _UNITTEST
#define __CSC_UNITTEST__
#else
#define __CSC_RELEASE__
#endif

#ifdef __clang__
#define __CSC_COMPILER_CLANG__
#elif defined __GNUC__
#define __CSC_COMPILER_GNUC__
#elif defined _MSC_VER
#define __CSC_COMPILER_MSVC__
#else
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#if defined (linux) || defined (__linux) || defined (__linux__)
#define __CSC_SYSTEM_LINUX__
#elif defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
#define __CSC_SYSTEM_WINDOWS__
#else
#error "∑(っ°Д° ;)っ : unsupported"
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
#error "∑(っ°Д° ;)っ : unsupported"
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

#include "begin.h"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <initializer_list>
#include <cassert>
#include <new>
#include <exception>
#include <typeinfo>
#include <utility>
#include <cstdlib>
#include "end.h"

#ifndef __macro_unwind
#define __macro_unwind(...) __VA_ARGS__
#endif

#ifndef __macro_stringize
#define __macro_stringize(...) #__VA_ARGS__
#endif

#ifndef __macro_requires
#define __macro_requires(...) static_assert (CSC::ENUM_CHECK<__macro_unwind (__VA_ARGS__)>::value ,"static_assert failed : " __macro_stringize (__VA_ARGS__))
#endif

#ifndef __macro_assert
#ifdef __CSC_DEBUG__
#define __macro_assert(...) CSC::debug_assert (__macro_unwind (__VA_ARGS__))
#endif

#ifdef __CSC_UNITTEST__
#define __macro_assert(...) CSC::unittest_assert (__macro_unwind (__VA_ARGS__))
#endif

#ifdef __CSC_RELEASE__
#define __macro_assert(...)
#endif
#endif

#ifndef __macro_ifnot
#define __macro_ifnot(...) (!(__macro_unwind (__VA_ARGS__)))
#endif

#ifndef __macro_ifswitch
#define __macro_ifswitch(...) (true) goto anonymous ; while (false) anonymous:
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::REMOVE_ALL<decltype (__macro_unwind (__VA_ARGS__))>
#endif

namespace CSC {
template <class...>
struct TYPEAS ;

namespace U {
template <class>
struct ALLID {} ;
} ;

template <class UNIT1>
struct TYPEAS<UNIT1> {
	static constexpr auto id = U::ALLID<UNIT1> () ;
} ;

using ALWAYS = void ;

namespace U {
template <class...>
trait REMOVE_REF_HELP ;

template <class UNIT1>
trait REMOVE_REF_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<UNIT1 & ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<UNIT1 && ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<const UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<const UNIT1 & ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<const UNIT1 && ,ALWAYS> {
	using RET = UNIT1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_REF = typename U::REMOVE_REF_HELP<UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REMOVE_ALL_HELP ;

template <class UNIT1>
trait REMOVE_ALL_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_ALL_HELP<ALLID<UNIT1> ,ALWAYS> {
	using RET = UNIT1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_ALL = typename U::REMOVE_ALL_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1 ,UNIT1 UNIT2>
struct ENUMAS {
	static constexpr auto value = UNIT2 ;
} ;

using ENUM_TRUE = ENUMAS<bool ,true> ;

using ENUM_FALSE = ENUMAS<bool ,false> ;

template <class UNIT1>
using ENUM_BOOL = ENUMAS<bool ,bool (UNIT1::value)> ;

namespace U {
template <class...>
trait ENUM_NOT_HELP ;

template <>
trait ENUM_NOT_HELP<ENUM_TRUE ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <>
trait ENUM_NOT_HELP<ENUM_FALSE ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;
} ;

template <class UNIT1>
using ENUM_NOT = typename U::ENUM_NOT_HELP<UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REQUIRE_HELP ;

template <class UNIT1>
trait REQUIRE_HELP<UNIT1 ,ENUM_TRUE ,ALWAYS> {
	using RET = void ;
} ;
} ;

template <class UNIT1>
using REQUIRE = typename U::REQUIRE_HELP<UNIT1 ,ENUM_BOOL<UNIT1> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait DEPENDENT_HELP ;

template <class UNIT1 ,class UNIT2>
trait DEPENDENT_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	using RET = UNIT1 ;
} ;
} ;

template <class UNIT1 ,class UNIT2 = void>
using DEPENDENT = typename U::DEPENDENT_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

namespace U {
#ifdef __CSC_DEBUG__
struct MACRO_DEBUG :public ENUM_TRUE {} ;
#else
struct MACRO_DEBUG :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_UNITTEST__
struct MACRO_UNITTEST :public ENUM_TRUE {} ;
#else
struct MACRO_UNITTEST :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_RELEASE__
struct MACRO_RELEASE :public ENUM_TRUE {} ;
#else
struct MACRO_RELEASE :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_COMPILER_MSVC__
struct MACRO_COMPILER_MSVC :public ENUM_TRUE {} ;
#else
struct MACRO_COMPILER_MSVC :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct MACRO_COMPILER_GNUC :public ENUM_TRUE {} ;
#else
struct MACRO_COMPILER_GNUC :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct MACRO_COMPILER_CLANG :public ENUM_TRUE {} ;
#else
struct MACRO_COMPILER_CLANG :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
struct MACRO_SYSTEM_WINDOWS :public ENUM_TRUE {} ;
#else
struct MACRO_SYSTEM_WINDOWS :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct MACRO_SYSTEM_LINUX :public ENUM_TRUE {} ;
#else
struct MACRO_SYSTEM_LINUX :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_PLATFORM_X86__
struct MACRO_PLATFORM_X86 :public ENUM_TRUE {} ;
#else
struct MACRO_PLATFORM_X86 :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_PLATFORM_X64__
struct MACRO_PLATFORM_X64 :public ENUM_TRUE {} ;
#else
struct MACRO_PLATFORM_X64 :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_PLATFORM_ARM__
struct MACRO_PLATFORM_ARM :public ENUM_TRUE {} ;
#else
struct MACRO_PLATFORM_ARM :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_PLATFORM_ARM64__
struct MACRO_PLATFORM_ARM64 :public ENUM_TRUE {} ;
#else
struct MACRO_PLATFORM_ARM64 :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_TARGET_EXE__
struct MACRO_TARGET_EXE :public ENUM_TRUE {} ;
#else
struct MACRO_TARGET_EXE :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_TARGET_DLL__
struct MACRO_TARGET_DLL :public ENUM_TRUE {} ;
#else
struct MACRO_TARGET_DLL :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_TARGET_LIB__
struct MACRO_TARGET_LIB :public ENUM_TRUE {} ;
#else
struct MACRO_TARGET_LIB :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_CONFIG_VAR32__
struct MACRO_CONFIG_VAR32 :public ENUM_TRUE {} ;
#else
struct MACRO_CONFIG_VAR32 :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_CONFIG_VAR64__
struct MACRO_CONFIG_VAR64 :public ENUM_TRUE {} ;
#else
struct MACRO_CONFIG_VAR64 :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_CONFIG_STRA__
struct MACRO_CONFIG_STRA :public ENUM_TRUE {} ;
#else
struct MACRO_CONFIG_STRA :public ENUM_FALSE {} ;
#endif

#ifdef __CSC_CONFIG_STRW__
struct MACRO_CONFIG_STRW :public ENUM_TRUE {} ;
#else
struct MACRO_CONFIG_STRW :public ENUM_FALSE {} ;
#endif
} ;
} ;