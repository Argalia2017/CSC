#pragma once

#ifndef __CSC__
#define __CSC__
#endif

#ifdef _DEBUG
#define __CSC_VER_DEBUG__
#elif defined _UNITTEST
#define __CSC_VER_UNITTEST__
#else
#define __CSC_VER_RELEASE__
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
#define __CSC_CONFIG_VAL64__
#elif defined _WIN32
#define __CSC_CONFIG_VAL32__
#else
#define __CSC_CONFIG_VAL64__
#endif

#ifdef _UNICODE
#define __CSC_CONFIG_STRW__
#elif defined _MBCS
#define __CSC_CONFIG_STRA__
#else
#define __CSC_CONFIG_STRA__
#endif

#ifdef _DEPRECATED
#define __CSC_CXX_LITE__
#endif

#ifndef __CSC_CXX_LITE__
#ifdef _MSVC_LANG
#if _MSVC_LANG >= 201703L
#define __CSC_CXX_FULL__
#endif
#else
#if __cplusplus >= 201703L
#define __CSC_CXX_FULL__
#elif __cplusplus >= 201103L
#else
#error "∑(っ°Д° ;)っ : unsupported"
#endif
#endif
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4068) //@info: warning C4068: unknown pragma
#pragma warning (disable :4100) //@info: warning C4100: 'xxx': unreferenced formal parameter
#pragma warning (disable :4180) //@info: warning C4180: qualifier applied to function type has no meaning; ignored
#pragma warning (disable :4127) //@info: warning C4127: conditional expression is constant
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4371) //@info: warning C4371: 'xxx': layout of class may have changed from a previous version of the compiler due to better packing of member 'xxx'
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4435) //@info: warning C4435: 'xxx': Object layout under /vd2 will change due to virtual base 'xxx'
#pragma warning (disable :4464) //@info: warning C4464: relative include path contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced inline function has been removed
#pragma warning (disable :4571) //@info: warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning (disable :4574) //@info: warning C4574: 'xxx' is defined to be '0': did you mean to use '#if xxx'?
#pragma warning (disable :4619) //@info: warning C4619: #pragma warning: there is no warning number 'xxx'
#pragma warning (disable :4623) //@info: warning C4623: 'xxx': default constructor was implicitly defined as deleted
#pragma warning (disable :4624) //@info: warning C4624: 'xxx': destructor was implicitly defined as deleted
#pragma warning (disable :4625) //@info: warning C4625: 'xxx': copy constructor was implicitly defined as deleted
#pragma warning (disable :4626) //@info: warning C4626: 'xxx': assignment operator was implicitly defined as deleted
#pragma warning (disable :4643) //@info: warning C4643: Forward declaring 'initializer_list' in namespace std is not permitted by the C++ Standard.
#pragma warning (disable :4668) //@info: warning C4668: 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning (disable :4686) //@info: warning C4686: 'xxx': possible change in behavior, change in UDT return calling convention
#pragma warning (disable :4702) //@info: warning C4702: unreachable code
#pragma warning (disable :4710) //@info: warning C4710: 'xxx': function not inlined
#pragma warning (disable :4711) //@info: warning C4711: function 'xxx' selected for automatic inline expansion
#pragma warning (disable :4717) //@info: warning C4717: 'xxx': recursive on all control paths, function will cause runtime stack overflow
#pragma warning (disable :4774) //@info: warning C4774: 'xxx' : format string expected in argument xxx is not a string literal
#pragma warning (disable :4820) //@info: warning C4820: 'xxx': 'xxx' bytes padding added after data member 'xxx'
#pragma warning (disable :5026) //@info: warning C5026: 'xxx': move constructor was implicitly defined as deleted
#pragma warning (disable :5027) //@info: warning C5027: 'xxx': move assignment operator was implicitly defined as deleted
#pragma warning (disable :5039) //@info: warning C5039: 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function assume an exception.
#pragma warning (disable :5045) //@info: warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "begin.h"
#ifndef __CSC_CXX_LITE__
#include <limits>
#include <initializer_list>
#include <type_traits>
#include <new>
#endif

#ifdef __CSC_CXX_LITE__
//@fatal: fuck initializer_list
namespace std {
template <class>
class initializer_list ;
} ;
#endif

#ifdef __CSC_CXX_LITE__
#ifdef __CSC_COMPILER_GNUC__
//@fatal: fuck g++4.8
#include <type_traits>
#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_nothrow_assignable(...) std::is_nothrow_assignable<__VA_ARGS__>::value
#define __is_convertible_to(...) std::is_convertible<__VA_ARGS__>::value
#define __is_trivially_constructible __has_trivial_constructor
#define __is_trivially_destructible __has_trivial_destructor
#endif
#endif

#ifdef __CSC_COMPILER_GNUC__
#if __GLIBCXX__ <= 20140522L
//@fatal: fuck g++4.8
namespace std {
template <class T>
struct is_trivially_constructible :integral_constant<bool ,__has_trivial_constructor (T)> {} ;
} ;
#endif
#endif
#include "end.h"

#ifndef __macro_exports
#ifdef __CSC_COMPILER_MSVC__
#define __macro_exports
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_exports __attribute__ ((visibility ("default")))
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_exports __attribute__ ((visibility ("default")))
#endif
#endif

#ifndef __macro_str
#define __macro_str_impl(...) #__VA_ARGS__
#define __macro_str(...) __macro_str_impl (__VA_ARGS__)
#endif

#ifndef __macro_cat
#define __macro_cat_impl(A ,B) A##B
#define __macro_cat(A ,B) __macro_cat_impl (A ,B)
#endif

#ifndef __macro_requires
#define __macro_requires(...) static_assert (CSC::ENUM_CHECK<__VA_ARGS__>::expr ,"static_assert failed : " __macro_str (__VA_ARGS__))
#endif

#ifndef __macro_anonymous
#define __macro_anonymous __macro_cat (__anonymous_ ,__LINE__)
#endif

#ifndef __macro_where
#define __macro_where CSC::ENUMAS<CSC::csc_byte32_t ,CSC::ENUMID<__COUNTER__>>
#endif

#ifndef __macro_slice
#ifdef __CSC_CONFIG_STRA__
#define __macro_slice(...) CSC::Slice<CSC::STR> (CSC::TYPEAS<where>::expr ,__VA_ARGS__)
#endif

#ifdef __CSC_CONFIG_STRW__
#define __macro_slice(...) CSC::Slice<CSC::STR> (CSC::TYPEAS<where>::expr ,__macro_cat (L ,__VA_ARGS__))
#endif
#endif

#ifndef __macro_assert
#ifdef __CSC_VER_DEBUG__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::unsafe_break () ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::unsafe_abort () ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_assert(...)
#endif
#endif

#ifndef __macro_assume
#ifdef __CSC_COMPILER_MSVC__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<where>::expr ,slice ("assume failed : " __macro_str (__VA_ARGS__) " : at " __FUNCSIG__ " in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_GNUC__
//@fatal: fuck g++4.8
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<where>::expr ,CSC::Slice<CSC::STR> (CSC::TYPEAS<where>::expr ,"assume failed : " __macro_str (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_CLANG__
//@fatal: fuck clang5.0
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::TYPEAS<where>::expr ,CSC::Slice<CSC::STR> (CSC::TYPEAS<where>::expr ,"assume failed : " __macro_str (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif
#endif

#ifndef __macro_unittest
#ifdef __CSC_VER_DEBUG__
#define __macro_unittest(...) do { CSC::unsafe_watch (CSC::TYPEAS<where>::expr ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_unittest(...) do { CSC::unsafe_watch (CSC::TYPEAS<where>::expr ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_unittest(...)
#endif
#endif

#ifndef __macro_forceinline
#ifdef __CSC_COMPILER_MSVC__
#define __macro_avoidinline __declspec (noinline)
#define __macro_forceinline __forceinline
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_avoidinline __attribute__ ((noinline))
#define __macro_forceinline __attribute__ ((always_inline))
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_avoidinline __attribute__ ((noinline))
#define __macro_forceinline __attribute__ ((always_inline))
#endif
#endif

#ifndef __macro_ifnot
#define __macro_ifnot(...) (bool (__VA_ARGS__) == false)
#endif

#ifndef __macro_ifswitch
#define __macro_ifswitch(A) (A) for (bool anonymous = true ; anonymous ; anonymous = CSC::unsafe_switch (A))
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::REMOVE_REF<decltype (__VA_ARGS__)>
#endif

namespace CSC {
template <class UNIT1>
using DEF = UNIT1 ;

using csc_bool_t = bool ;

using csc_int32_t = int ;
using csc_int64_t = long long ;

using csc_float32_t = float ;
using csc_float64_t = double ;
using csc_float128_t = long double ;

#ifdef __CSC_CXX_LITE__
struct FUNCTION_infinity {
	inline constexpr operator csc_float32_t () const noexcept {
		return csc_float32_t (__builtin_huge_valf ()) ;
	}
} ;
#endif

#ifndef __CSC_CXX_LITE__
struct FUNCTION_infinity {
	inline constexpr operator csc_float32_t () const noexcept {
		return std::numeric_limits<csc_float32_t>::infinity () ;
	}
} ;
#endif

static constexpr auto infinity = FUNCTION_infinity () ;

using csc_byte8_t = unsigned char ;
using csc_byte16_t = unsigned short ;
using csc_byte32_t = unsigned int ;
using csc_byte64_t = unsigned long long ;

struct csc_byte128_t {
	alignas (16) DEF<csc_byte8_t[16]> mUnused ;
} ;

using csc_char_t = char ;
using csc_wchar_t = wchar_t ;
using csc_char8_t = csc_byte8_t ;
using csc_char16_t = char16_t ;
using csc_char32_t = char32_t ;

using csc_pointer_t = DEF<void *> ;
using csc_const_pointer_t = DEF<const void *> ;

#ifdef __CSC_CXX_LITE__
#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_CONFIG_VAL32__
using csc_ptrdiff_t = int ;
using csc_size_t = unsigned int ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using csc_ptrdiff_t = long long ;
using csc_size_t = unsigned long long ;
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_ptrdiff_t = long ;
using csc_size_t = unsigned long ;
#endif
#endif

#ifndef __CSC_CXX_LITE__
using csc_ptrdiff_t = std::ptrdiff_t ;
using csc_size_t = std::size_t ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_initializer_t = std::initializer_list<UNIT1> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_initializer_t = std::initializer_list<UNIT1> ;
#endif

template <class...>
struct ENUMAS ;

template <csc_ptrdiff_t>
struct ENUMID {} ;

template <class UNIT1 ,csc_ptrdiff_t UNIT2>
struct ENUMAS<UNIT1 ,ENUMID<UNIT2>> {
	//@fatal: fuck ODR
	imports constexpr UNIT1 expr_m () noexcept {
		return UNIT1 (UNIT2) ;
	}

	inline constexpr operator UNIT1 () noexcept {
		return expr ;
	}
} ;

using ENUM_TRUE = ENUMAS<csc_bool_t ,ENUMID<true>> ;

using ENUM_FALSE = ENUMAS<csc_bool_t ,ENUMID<false>> ;

template <class...>
struct TYPEAS ;

template <class>
struct TYPEID {} ;

template <class UNIT1>
struct TYPEAS<UNIT1> {
	//@fatal: fuck ODR
	imports constexpr TYPEID<UNIT1> expr_m () noexcept {
		return TYPEID<UNIT1> () ;
	}

	inline constexpr operator TYPEID<UNIT1> () noexcept {
		return expr ;
	}
} ;

template <class...>
struct TEMPAS ;

template <class UNIT1 ,class UNIT2>
struct TEMPAS<UNIT1 ,UNIT2> {
	UNIT2 mUnused ;
} ;

template <class UNIT1>
struct TEMPAS<UNIT1 ,void> ;

using DEPEND = struct D ;
using ALWAYS = struct A ;

template <class...>
trait REQUIRE_HELP ;

template <>
trait REQUIRE_HELP<ENUM_TRUE ,ALWAYS> {
	using RET = ALWAYS ;
} ;

template <class UNIT1>
using REQUIRE = typename REQUIRE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait DEPENDENT_HELP ;

template <class UNIT1 ,class UNIT2>
trait DEPENDENT_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1 ,class UNIT2>
using DEPENDENT = typename DEPENDENT_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait REMOVE_REF_HELP ;

template <class UNIT1>
trait REMOVE_REF_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<DEF<UNIT1 &> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<DEF<UNIT1 &&> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<DEF<const UNIT1> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<DEF<const UNIT1 &> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_REF_HELP<DEF<const UNIT1 &&> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
using REMOVE_REF = typename REMOVE_REF_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_SAME_HELP ;

template <class UNIT1 ,class UNIT2>
trait IS_SAME_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT1>
trait IS_SAME_HELP<UNIT1 ,UNIT1 ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT1 ,class UNIT2>
using IS_SAME = typename IS_SAME_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class UNIT1 ,class = REQUIRE<IS_SAME<UNIT1 ,REMOVE_REF<UNIT1>>>>
using VREF = DEF<UNIT1 &> ;

template <class UNIT1 ,class = REQUIRE<IS_SAME<UNIT1 ,REMOVE_REF<UNIT1>>>>
using CREF = DEF<const UNIT1 &> ;

template <class UNIT1 ,class = REQUIRE<IS_SAME<UNIT1 ,REMOVE_REF<UNIT1>>>>
using RREF = DEF<UNIT1 &&> ;

template <class UNIT1>
using XREF = DEF<UNIT1 &&> ;

struct csc_text_t {
	DEF<const char *> mBegin ;
	DEF<const char *> mEnd ;

	template <class ARG1>
	explicit csc_text_t (CREF<ARG1> obj) {
		mBegin = obj ;
		mEnd = obj + sizeof (obj) - 1 ;
	}
} ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_internel_name {
	template <class T>
	imports csc_text_t __cdecl stupid () {
		auto rax = csc_text_t (__FUNCSIG__) ;
		rax.mBegin += sizeof ("struct CSC::csc_text_t __cdecl CSC::FUNCTION_internel_name::stupid<") - 1 ;
		rax.mEnd -= sizeof (">(void)") - 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_text_t operator() (CREF<TYPEID<ARG1>> id) const {
		return stupid<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_internel_name {
	template <class T>
	imports __attribute__ ((cdecl)) csc_text_t stupid () {
		auto rax = csc_text_t (__PRETTY_FUNCTION__) ;
		rax.mBegin += sizeof ("static CSC::csc_text_t CSC::FUNCTION_internel_name::stupid() [with T = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_text_t operator() (CREF<TYPEID<ARG1>> id) const {
		return stupid<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_internel_name {
	template <class T>
	imports csc_text_t __cdecl stupid () {
		auto rax = csc_text_t (__PRETTY_FUNCTION__) ;
		rax.mBegin += sizeof ("static CSC::csc_text_t CSC::FUNCTION_internel_name::stupid() [T = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_text_t operator() (CREF<TYPEID<ARG1>> id) const {
		return stupid<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_VER_DEBUG__
template <class DEPEND>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_VER_DEBUG__
template <class DEPEND>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_VER_UNITTEST__
template <class DEPEND>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_VER_UNITTEST__
template <class DEPEND>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_VER_RELEASE__
template <class DEPEND>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_VER_RELEASE__
template <class DEPEND>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_MSVC__
template <class DEPEND>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_MSVC__
template <class DEPEND>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class DEPEND>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_GNUC__
template <class DEPEND>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class DEPEND>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_CLANG__
template <class DEPEND>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
template <class DEPEND>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
template <class DEPEND>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
template <class DEPEND>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_SYSTEM_LINUX__
template <class DEPEND>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_X86__
template <class DEPEND>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_X86__
template <class DEPEND>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_X64__
template <class DEPEND>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_X64__
template <class DEPEND>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_ARM__
template <class DEPEND>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_ARM__
template <class DEPEND>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_ARM64__
template <class DEPEND>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_ARM64__
template <class DEPEND>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_EXE__
template <class DEPEND>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_EXE__
template <class DEPEND>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_DLL__
template <class DEPEND>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_DLL__
template <class DEPEND>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_LIB__
template <class DEPEND>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_LIB__
template <class DEPEND>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_VAL32__
template <class DEPEND>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_VAL32__
template <class DEPEND>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_VAL64__
template <class DEPEND>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_VAL64__
template <class DEPEND>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_STRA__
template <class DEPEND>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_STRA__
template <class DEPEND>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_STRW__
template <class DEPEND>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_STRW__
template <class DEPEND>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class DEPEND>
using MACRO_CXX_LITE = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class DEPEND>
using MACRO_CXX_LITE = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_FULL__
template <class DEPEND>
using MACRO_CXX_FULL = DEPENDENT<ENUM_TRUE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifndef __CSC_CXX_FULL__
template <class DEPEND>
using MACRO_CXX_FULL = DEPENDENT<ENUM_FALSE ,DEPENDENT<where ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_INTCLASS = ENUMAS<csc_bool_t ,ENUMID<(__is_enum (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_INTCLASS = ENUMAS<csc_bool_t ,ENUMID<(std::is_enum<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_CLASS = ENUMAS<csc_bool_t ,ENUMID<(__is_class (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_CLASS = ENUMAS<csc_bool_t ,ENUMID<(std::is_class<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_constructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_DESTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_destructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_DESTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_destructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_constructible (UNIT1 ,CREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_copy_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_COPY_ASSIGNABLE = ENUMAS<csc_bool_t ,ENUMID<(__is_assignable (VREF<UNIT1> ,CREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_COPY_ASSIGNABLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_copy_assignable<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_constructible (UNIT1 ,RREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_move_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_MOVE_ASSIGNABLE = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_assignable (VREF<UNIT1> ,RREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_MOVE_ASSIGNABLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_move_assignable<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_TRIVIAL_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_trivially_constructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_TRIVIAL_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_trivially_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_TRIVIAL_DESTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_trivially_destructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_TRIVIAL_DESTRUCTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_trivially_destructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_INTERFACE = ENUMAS<csc_bool_t ,ENUMID<(__is_abstract (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_IS_INTERFACE = ENUMAS<csc_bool_t ,ENUMID<(std::is_abstract<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class TO>
using MACRO_IS_EXTEND = ENUMAS<csc_bool_t ,ENUMID<(__is_base_of (FROM ,TO))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class TO>
using MACRO_IS_EXTEND = ENUMAS<csc_bool_t ,ENUMID<(std::is_base_of<FROM ,TO>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class TO>
using MACRO_IS_CONVERTIBLE = ENUMAS<csc_bool_t ,ENUMID<(__is_convertible_to (FROM ,TO))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class TO>
using MACRO_IS_CONVERTIBLE = ENUMAS<csc_bool_t ,ENUMID<(std::is_convertible<FROM ,TO>::value)>> ;
#endif
} ;

#ifdef __CSC_CXX_LITE__
template <class ARG1 ,class ARG2>
inline constexpr CSC::csc_pointer_t operator new (CSC::csc_size_t ,CSC::DEF<CSC::TEMPAS<ARG1 ,ARG2> *> where_) noexcept {
	return where_ ;
}

template <class ARG1 ,class ARG2>
inline constexpr void operator delete (CSC::csc_pointer_t ,CSC::DEF<CSC::TEMPAS<ARG1 ,ARG2> *> where_) noexcept {
	return ;
}
#endif