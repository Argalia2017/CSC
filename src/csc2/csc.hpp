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
#include <atomic>
#include "end.h"

#ifdef _HAS_CXX17
#if _HAS_CXX17
#define __CSC_CXX_LATEST__
#endif
#endif

#ifdef __CSC_COMPILER_GNUC__
namespace std {
using ::max_align_t ;
} ;
#endif

#ifndef __macro_stringize
#define __macro_stringize_impl(...) #__VA_ARGS__
#define __macro_stringize(...) __macro_stringize_impl (__VA_ARGS__)
#endif

#ifndef __macro_requires
#define __macro_requires(...) static_assert (CSC::ENUM_CHECK<__VA_ARGS__>::value ,"static_assert failed : " __macro_stringize (__VA_ARGS__))
#endif

#ifndef __macro_anonymous
#define __macro_anonymous_impl_impl(A) __anonymous_##A
#define __macro_anonymous_impl(A) __macro_anonymous_impl_impl(A)
#define __macro_anonymous __macro_anonymous_impl (__LINE__)
#endif

#ifndef __macro_slice
#define __macro_slice(...) CSC::Slice<CSC::STR> (CSC::TYPEAS<struct anonymous>::id ,__VA_ARGS__)
#endif

#ifndef __macro_assert
#ifdef __CSC_DEBUG__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::debug_break () ; } while (false)
#endif

#ifdef __CSC_UNITTEST__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::debug_break () ; } while (false)
#endif

#ifdef __CSC_RELEASE__
#define __macro_assert(...)
#endif
#endif

#ifndef __macro_dynamic_assert
#ifdef __CSC_COMPILER_MSVC__
#define __macro_dynamic_assert(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<struct anonymous>::id ,slice ("dynamic_assert failed : " __macro_stringize (__VA_ARGS__) " : at " __FUNCSIG__ " in " __FILE__ " ," __macro_stringize (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_dynamic_assert(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<struct anonymous>::id ,slice ("dynamic_assert failed : " __macro_stringize (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_stringize (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_dynamic_assert(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<struct anonymous>::id ,slice ("dynamic_assert failed : " __macro_stringize (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_stringize (__LINE__))).raise () ; } while (false)
#endif
#endif

#ifndef __macro_dynamic_watch
#define __macro_dynamic_watch(...) do { CSC::debug_watch (slice (__macro_stringize (__VA_ARGS__)) ,(__VA_ARGS__)) ; } while (false)
#endif

#ifndef __macro_ifnot
#define __macro_ifnot(...) (!(__VA_ARGS__))
#endif

#ifndef __macro_ifswitch
#define __macro_ifswitch(...) (__VA_ARGS__) switch (true) case true:
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::REMOVE_ALL<decltype (__VA_ARGS__)>
#endif

namespace CSC {
template <class...>
struct ENUMAS ;

#ifdef __CSC_CONFIG_VAR32__
namespace U {
template <int32_t>
struct ENUMID ;
} ;

template <class UNIT1 ,int32_t UNIT2>
struct ENUMAS<UNIT1 ,U::ENUMID<UNIT2>> {
	static constexpr auto value = UNIT1 (UNIT2) ;
} ;
#endif

#ifdef __CSC_CONFIG_VAR64__
namespace U {
template <int64_t>
struct ENUMID ;
} ;

template <class UNIT1 ,int64_t UNIT2>
struct ENUMAS<UNIT1 ,U::ENUMID<UNIT2>> {
	static constexpr auto value = UNIT1 (UNIT2) ;
} ;
#endif

using ENUM_TRUE = ENUMAS<bool ,U::ENUMID<true>> ;

using ENUM_FALSE = ENUMAS<bool ,U::ENUMID<false>> ;

template <class...>
struct TYPEAS ;

namespace U {
template <class>
struct TYPEID {} ;
} ;

template <class UNIT1>
struct TYPEAS<UNIT1> {
	static constexpr auto id = U::TYPEID<UNIT1> () ;
} ;

using ALWAYS = void ;

namespace U {
template <class...>
trait ENUM_NOT_HELP ;

template <>
trait ENUM_NOT_HELP<ENUM_FALSE ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;

template <>
trait ENUM_NOT_HELP<ENUM_TRUE ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;
} ;

template <class UNIT1>
using ENUM_NOT = typename U::ENUM_NOT_HELP<UNIT1 ,ALWAYS>::RET ;

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
trait REMOVE_ALL_HELP<TYPEID<UNIT1> ,ALWAYS> {
	using RET = UNIT1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_ALL = typename U::REMOVE_ALL_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REQUIRE_HELP ;

template <>
trait REQUIRE_HELP<ENUM_TRUE ,ALWAYS> {
	using RET = void ;
} ;
} ;

template <class UNIT1>
using REQUIRE = typename U::REQUIRE_HELP<UNIT1 ,ALWAYS>::RET ;

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
template <class UNIT1>
using MACRO_DEBUG = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_DEBUG = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_UNITTEST__
template <class UNIT1>
using MACRO_UNITTEST = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_UNITTEST = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_RELEASE__
template <class UNIT1>
using MACRO_RELEASE = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_RELEASE = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_MSVC__
template <class UNIT1>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class UNIT1>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class UNIT1>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
template <class UNIT1>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
template <class UNIT1>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_X86__
template <class UNIT1>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_X64__
template <class UNIT1>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_ARM__
template <class UNIT1>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_ARM64__
template <class UNIT1>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_EXE__
template <class UNIT1>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_DLL__
template <class UNIT1>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_LIB__
template <class UNIT1>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_VAR32__
template <class UNIT1>
using MACRO_CONFIG_VAR32 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_CONFIG_VAR32 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_VAR64__
template <class UNIT1>
using MACRO_CONFIG_VAR64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_CONFIG_VAR64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_STRA__
template <class UNIT1>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_STRW__
template <class UNIT1>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#else
template <class UNIT1>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif
} ;

namespace U {
enum class __char8_t :unsigned char ;
} ;

namespace U {
struct __int128_t {
	alignas (128) char mUnused[128] ;
} ;
} ;
} ;