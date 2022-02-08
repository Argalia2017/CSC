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

#define __CSC_CXX_LITE__

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
#ifdef __CSC_CXX_LITE__
#include "lite.h"
#endif

#ifndef __CSC_CXX_LITE__
#include <limits>
#include <initializer_list>
#include <type_traits>
#include <new>
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
#define __macro_requires(...) static_assert (CSC::CORE::ENUM_CHECK<__VA_ARGS__>::value ,"static_assert failed : " __macro_str (__VA_ARGS__))
#endif

#ifndef __macro_anonymous
#define __macro_anonymous __macro_cat (__anonymous_ ,__LINE__)
#endif

#ifndef __macro_slice
#ifdef __CSC_CONFIG_STRA__
#define __macro_slice(...) CSC::CORE::Slice<CSC::CORE::STR> (CSC::U::TYPEAS<struct anonymous>::id ,__VA_ARGS__)
#endif

#ifdef __CSC_CONFIG_STRW__
#define __macro_slice(...) CSC::CORE::Slice<CSC::CORE::STR> (CSC::U::TYPEAS<struct anonymous>::id ,__macro_cat (L ,__VA_ARGS__))
#endif
#endif

#ifndef __macro_assert
#ifdef __CSC_VER_DEBUG__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::CORE::unsafe_break () ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; CSC::CORE::unsafe_abort () ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_assert(...)
#endif
#endif

#ifndef __macro_assume
#ifdef __CSC_COMPILER_MSVC__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::CORE::Exception (CSC::U::TYPEAS<struct anonymous>::id ,slice ("assume failed : " __macro_str (__VA_ARGS__) " : at " __FUNCSIG__ " in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::CORE::Exception (CSC::U::TYPEAS<struct anonymous>::id ,CSC::CORE::Slice<CSC::CORE::STR> (CSC::U::TYPEAS<struct anonymous>::id ,"assume failed : " __macro_str (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::CORE::Exception (CSC::U::TYPEAS<struct anonymous>::id ,CSC::CORE::Slice<CSC::CORE::STR> (CSC::U::TYPEAS<struct anonymous>::id ,"assume failed : " __macro_str (__VA_ARGS__) " : at " ,__PRETTY_FUNCTION__ ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif
#endif

#ifndef __macro_unittest_watch
#ifdef __CSC_VER_DEBUG__
#define __macro_unittest_watch(...) do { CSC::CORE::unsafe_watch (CSC::U::TYPEAS<struct anonymous>::id ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_unittest_watch(...) do { CSC::CORE::unsafe_watch (CSC::U::TYPEAS<struct anonymous>::id ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_unittest_watch(...)
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
#define __macro_ifswitch(A) (A) goto anonymous ; while (CSC::CORE::unsafe_switch (A)) anonymous:
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::U::REMOVE_ALL<decltype (__VA_ARGS__)>
#endif

#ifdef __CSC_COMPILER_MSVC__
#ifndef __CSC_CXX_FULL__
#define nodiscard
#endif
#endif

#ifdef __CSC_COMPILER_GNUC__
#if __GNUC__ <= 6L
namespace std {
template <class T>
struct is_trivially_constructible :integral_constant<bool ,__has_trivial_constructor (T)> {} ;
} ;
#endif
#endif

namespace CSC {
namespace U {
template <class UNIT1>
using DEF = UNIT1 ;

using csc_bool_t = bool ;

using csc_int32_t = int ;
using csc_int32_64_t = long ;
using csc_int64_t = long long ;

using csc_float32_t = float ;
using csc_float64_t = double ;
using csc_float128_t = long double ;

#ifdef __CSC_CXX_LITE__
struct FUNCTION_infinity {
	inline constexpr operator csc_float32_t () const noexcept {
		return (__builtin_huge_valf ()) ;
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

using csc_char_t = char ;
using csc_wchar_t = wchar_t ;

using csc_char8_t = unsigned char ;
using csc_char16_t = char16_t ;
using csc_char32_t = char32_t ;

using csc_byte8_t = unsigned char ;
using csc_byte16_t = unsigned short ;
using csc_byte32_t = unsigned int ;
using csc_byte32_64_t = unsigned long ;
using csc_byte64_t = unsigned long long ;

struct csc_byte128_t {
	alignas (128) DEF<csc_byte8_t[128]> __unused ;
} ;

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_CONFIG_VAL32__
using csc_ptrdiff_t = csc_int32_t ;
using csc_size_t = csc_byte32_t ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using csc_ptrdiff_t = csc_int64_t ;
using csc_size_t = csc_byte64_t ;
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_ptrdiff_t = csc_int32_64_t ;
using csc_size_t = csc_byte32_64_t ;
#endif

using csc_pointer_t = DEF<void *> ;

template <class...>
struct ENUMAS ;

template <csc_size_t>
struct ENUMID ;

template <class UNIT1 ,csc_size_t UNIT2>
struct ENUMAS<UNIT1 ,ENUMID<UNIT2>> {
	static constexpr UNIT1 value = UNIT1 (UNIT2) ;
} ;

#ifndef __CSC_CXX_FULL__
#ifdef __CSC_COMPILER_GNUC__
template <class UNIT1 ,csc_size_t UNIT2>
constexpr UNIT1 ENUMAS<UNIT1 ,ENUMID<UNIT2>>::value ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class UNIT1 ,csc_size_t UNIT2>
constexpr UNIT1 ENUMAS<UNIT1 ,ENUMID<UNIT2>>::value ;
#endif
#endif

using ENUM_TRUE = ENUMAS<csc_bool_t ,ENUMID<true>> ;

using ENUM_FALSE = ENUMAS<csc_bool_t ,ENUMID<false>> ;

template <class...>
struct TYPEAS ;

template <class>
struct TYPEID {} ;

template <class UNIT1>
struct TYPEAS<UNIT1> {
	static constexpr TYPEID<UNIT1> id = TYPEID<UNIT1> () ;
} ;

#ifndef __CSC_CXX_FULL__
#ifdef __CSC_COMPILER_GNUC__
template <class UNIT1>
constexpr TYPEID<UNIT1> TYPEAS<UNIT1>::id ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class UNIT1>
constexpr TYPEID<UNIT1> TYPEAS<UNIT1>::id ;
#endif
#endif

#ifdef __CSC_COMPILER_MSVC__
template <class UNIT1 ,class UNIT2>
struct TEMPID {
	static constexpr auto M_SIZE = sizeof (UNIT2) ;
	static constexpr auto M_ALIGN = alignof (UNIT2) ;

	alignas (M_ALIGN) DEF<csc_byte8_t[M_SIZE]> __unused ;
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class UNIT1 ,class UNIT2>
struct TEMPID {
	static constexpr auto M_SIZE = sizeof (UNIT2) ;

	alignas (UNIT2) DEF<csc_byte8_t[M_SIZE]> __unused ;
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class UNIT1 ,class UNIT2>
struct TEMPID {
	static constexpr auto M_SIZE = sizeof (UNIT2) ;
	static constexpr auto M_ALIGN = alignof (UNIT2) ;

	alignas (M_ALIGN) DEF<csc_byte8_t[M_SIZE]> __unused ;
} ;
#endif

template <class UNIT1>
struct TEMPID<UNIT1 ,void> ;

struct DEPEND ;
struct ALWAYS ;

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

template <class UNIT1>
using ENUM_NOT = typename ENUM_NOT_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class UNIT1>
using VREF = DEF<REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using CREF = DEF<const REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using RREF = DEF<REMOVE_REF<UNIT1> &&> ;

template <class UNIT1>
using XREF = DEF<UNIT1 &&> ;

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

template <class UNIT1>
using REMOVE_ALL = typename REMOVE_ALL_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

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

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_initializer_list = std::initializer_list<UNIT1> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_initializer_list = std::initializer_list<UNIT1> ;
#endif

struct csc_slice_t {
	DEF<const char *> mBegin ;
	DEF<const char *> mEnd ;
} ;

struct FUNCTION_slice_t {
	template <class ARG1>
	inline csc_slice_t operator() (XREF<ARG1> obj) const {
		auto rax = csc_slice_t () ;
		rax.mBegin = obj ;
		rax.mEnd = obj + sizeof (obj) ;
		return rax ;
	}
} ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_internel_name {
	template <class T>
	imports csc_slice_t __cdecl invoke () {
		static constexpr auto M_INVOKE = FUNCTION_slice_t () ;
		auto rax = M_INVOKE (__FUNCSIG__) ;
		rax.mBegin += sizeof ("struct CSC::U::csc_slice_t __cdecl CSC::U::FUNCTION_internel_name::invoke<") - 1 ;
		rax.mEnd -= sizeof (">(void)") ;
		return rax ;
	}

	template <class ARG1>
	inline csc_slice_t operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		return invoke<R1X> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_internel_name {
	template <class T>
	imports csc_slice_t __cdecl invoke () {
		static constexpr auto M_INVOKE = FUNCTION_slice_t () ;
		auto rax = M_INVOKE (__PRETTY_FUNCTION__) ;
		rax.mBegin += sizeof ("static CSC::U::csc_slice_t CSC::U::FUNCTION_internel_name::invoke() [with T = ") ;
		rax.mEnd -= sizeof ("]") ;
		return rax ;
	}

	template <class ARG1>
	inline csc_slice_t operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		return invoke<R1X> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_internel_name {
	template <class T>
	imports csc_slice_t __cdecl invoke () {
		static constexpr auto M_INVOKE = FUNCTION_slice_t () ;
		auto rax = M_INVOKE (__PRETTY_FUNCTION__) ;
		rax.mBegin += sizeof ("static CSC::U::csc_slice_t CSC::U::FUNCTION_internel_name::invoke() [T = ") ;
		rax.mEnd -= sizeof ("]") ;
		return rax ;
	}

	template <class ARG1>
	inline csc_slice_t operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		return invoke<R1X> () ;
	}
} ;
#endif

static constexpr auto internel_name = FUNCTION_internel_name () ;

struct FUNCTION_internel_time {
	template <class T>
	imports csc_slice_t __cdecl invoke () {
		static constexpr auto M_INVOKE = FUNCTION_slice_t () ;
		auto rax = M_INVOKE (__DATE__ " " __TIME__) ;
		rax.mEnd -= sizeof ("") ;
		return rax ;
	}

	template <class ARG1>
	inline csc_slice_t operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		return invoke<R1X> () ;
	}
} ;

static constexpr auto internel_time = FUNCTION_internel_time () ;

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_enum = ENUMAS<csc_bool_t ,ENUMID<(__is_enum (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_enum = ENUMAS<csc_bool_t ,ENUMID<(std::is_enum<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_class = ENUMAS<csc_bool_t ,ENUMID<(__is_class (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_class = ENUMAS<csc_bool_t ,ENUMID<(std::is_class<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_constructible = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_constructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_constructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_destructible = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_destructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_destructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_destructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_copy_constructible = ENUMAS<csc_bool_t ,ENUMID<(__is_constructible (UNIT1 ,CREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_copy_constructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_copy_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_copy_assignable = ENUMAS<csc_bool_t ,ENUMID<(__is_assignable (VREF<UNIT1> ,CREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_copy_assignable = ENUMAS<csc_bool_t ,ENUMID<(std::is_copy_assignable<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_move_constructible = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_constructible (UNIT1 ,RREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_move_constructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_move_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_move_assignable = ENUMAS<csc_bool_t ,ENUMID<(__is_nothrow_assignable (VREF<UNIT1> ,RREF<UNIT1>))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_nothrow_move_assignable = ENUMAS<csc_bool_t ,ENUMID<(std::is_nothrow_move_assignable<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_trivial_constructible = ENUMAS<csc_bool_t ,ENUMID<(__is_trivially_constructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_trivial_constructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_trivially_constructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_trivial_destructible = ENUMAS<csc_bool_t ,ENUMID<(__is_trivially_destructible (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_trivial_destructible = ENUMAS<csc_bool_t ,ENUMID<(std::is_trivially_destructible<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_abstract = ENUMAS<csc_bool_t ,ENUMID<(__is_abstract (UNIT1))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using csc_is_abstract = ENUMAS<csc_bool_t ,ENUMID<(std::is_abstract<UNIT1>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class TO>
using csc_is_base_of = ENUMAS<csc_bool_t ,ENUMID<(__is_base_of (FROM ,TO))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class TO>
using csc_is_base_of = ENUMAS<csc_bool_t ,ENUMID<(std::is_base_of<FROM ,TO>::value)>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class TO>
using csc_is_convertible = ENUMAS<csc_bool_t ,ENUMID<(__is_convertible_to (FROM ,TO))>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class TO>
using csc_is_convertible = ENUMAS<csc_bool_t ,ENUMID<(std::is_convertible<FROM ,TO>::value)>> ;
#endif

#ifdef __CSC_VER_DEBUG__
template <class UNIT1>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_VER_DEBUG__
template <class UNIT1>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_VER_UNITTEST__
template <class UNIT1>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_VER_UNITTEST__
template <class UNIT1>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_VER_RELEASE__
template <class UNIT1>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_VER_RELEASE__
template <class UNIT1>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_MSVC__
template <class UNIT1>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_COMPILER_MSVC__
template <class UNIT1>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class UNIT1>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_COMPILER_GNUC__
template <class UNIT1>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class UNIT1>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_COMPILER_CLANG__
template <class UNIT1>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
template <class UNIT1>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
template <class UNIT1>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
template <class UNIT1>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_SYSTEM_LINUX__
template <class UNIT1>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_X86__
template <class UNIT1>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_PLATFORM_X86__
template <class UNIT1>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_X64__
template <class UNIT1>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_PLATFORM_X64__
template <class UNIT1>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_ARM__
template <class UNIT1>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_PLATFORM_ARM__
template <class UNIT1>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_PLATFORM_ARM64__
template <class UNIT1>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_PLATFORM_ARM64__
template <class UNIT1>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_EXE__
template <class UNIT1>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_TARGET_EXE__
template <class UNIT1>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_DLL__
template <class UNIT1>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_TARGET_DLL__
template <class UNIT1>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_TARGET_LIB__
template <class UNIT1>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_TARGET_LIB__
template <class UNIT1>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_VAL32__
template <class UNIT1>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CONFIG_VAL32__
template <class UNIT1>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_VAL64__
template <class UNIT1>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CONFIG_VAL64__
template <class UNIT1>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_STRA__
template <class UNIT1>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CONFIG_STRA__
template <class UNIT1>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CONFIG_STRW__
template <class UNIT1>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CONFIG_STRW__
template <class UNIT1>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_CXX_LITE = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT1>
using MACRO_CXX_LITE = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifdef __CSC_CXX_FULL__
template <class UNIT1>
using MACRO_CXX_FULL = DEPENDENT<ENUM_TRUE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif

#ifndef __CSC_CXX_FULL__
template <class UNIT1>
using MACRO_CXX_FULL = DEPENDENT<ENUM_FALSE ,DEPENDENT<struct anonymous ,UNIT1>> ;
#endif
} ;
} ;

namespace CSC {
using namespace U ;
} ;