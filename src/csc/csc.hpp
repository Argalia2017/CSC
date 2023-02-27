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
#pragma warning (disable :4127) //@info: warning C4127: conditional expression is constant
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4459) //@info: warning C4459: declaration of 'xxx' hides global declaration
#pragma warning (disable :4464) //@info: warning C4464: relative include path contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced inline function has been removed
#pragma warning (disable :4571) //@info: warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning (disable :4574) //@info: warning C4574: 'xxx' is defined to be '0': did you mean to use '#if xxx'?
#pragma warning (disable :4584) //@info: warning C4584: 'xxx': base-class 'xxx' is already a base-class of 'xxx'
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

#include "csc_end.h"
#ifdef __CSC_CXX_LITE__
#include "csc_lite.h"
#endif

#ifndef __CSC_CXX_LITE__
#include <limits>
#include <initializer_list>
#include <type_traits>
#include <new>
#endif
#include "csc_begin.h"

#ifndef __CSC_CXX_LITE__
#ifdef __CSC_COMPILER_GNUC__
#if __GLIBCXX__ <= 20140522L
//@fatal: fuck gnuc
namespace std {
template <class UNIT>
struct is_trivially_default_constructible :integral_constant<bool ,__has_trivial_constructor (UNIT)> {} ;
} ;
#endif
#endif
#endif

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
#define __macro_where CSC::ENUMAS<CSC::csc_byte32_t ,__COUNTER__>
#endif

#ifndef __macro_slice
#ifdef __CSC_CONFIG_STRA__
#define __macro_slice(...) CSC::Slice<CSC::STR> (__VA_ARGS__)
#endif

#ifdef __CSC_CONFIG_STRW__
#define __macro_slice(...) CSC::Slice<CSC::STR> (__macro_cat (L ,__VA_ARGS__))
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
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (slice ("assume failed : " __macro_str (__VA_ARGS__) " : at " __FUNCSIG__ " in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_GNUC__
//@fatal: fuck gnuc
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::Slice<CSC::STR> (CSC::Slice<CSC::STR> (CSC::Slice<CSC::STR> ("assume failed : " __macro_str (__VA_ARGS__) " : at ") ,__PRETTY_FUNCTION__) ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
#endif

#ifdef __CSC_COMPILER_CLANG__
//@fatal: fuck clang
#define __macro_assume(...) do { if (__VA_ARGS__) break ; CSC::Exception (CSC::Slice<CSC::STR> (CSC::Slice<CSC::STR> (CSC::Slice<CSC::STR> ("assume failed : " __macro_str (__VA_ARGS__) " : at ") ,__PRETTY_FUNCTION__) ," in " __FILE__ " ," __macro_str (__LINE__))).raise () ; } while (false)
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
#define __macro_ifnot(...) (CSC::csc_bool_t (__VA_ARGS__) == false)
#endif

#ifndef __macro_ifswitch
#define __macro_ifswitch(A) (A) for (CSC::csc_bool_t anonymous = true ; anonymous ; anonymous = CSC::unsafe_switch (A))
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::REMOVE_REF<decltype (__VA_ARGS__)>
#endif

namespace CSC {
template <class UNIT>
using DEF = UNIT ;

using csc_bool_t = bool ;

using csc_int32_t = int ;
using csc_int64_t = DEF<long long> ;

using csc_float32_t = float ;
using csc_float64_t = double ;

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

using csc_byte8_t = DEF<unsigned char> ;
using csc_byte16_t = DEF<unsigned short> ;
using csc_byte32_t = DEF<unsigned int> ;
using csc_byte64_t = DEF<unsigned long long> ;

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
using csc_diff_t = int ;
using csc_size_t = DEF<unsigned int> ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using csc_diff_t = DEF<long long> ;
using csc_size_t = DEF<unsigned long long> ;
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_diff_t = long ;
using csc_size_t = DEF<unsigned long> ;
#endif
#endif

#ifndef __CSC_CXX_LITE__
using csc_diff_t = std::ptrdiff_t ;
using csc_size_t = std::size_t ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
using csc_enum_t = DEF<unsigned long> ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_enum_t = int ;
#endif

struct csc_span_t {
	csc_diff_t mBegin ;
	csc_diff_t mEnd ;
	csc_diff_t mStep ;
} ;

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using csc_initializer_t = std::initializer_list<UNIT> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using csc_initializer_t = std::initializer_list<UNIT> ;
#endif

template <class UNIT ,csc_diff_t SIDE>
struct ENUMAS {
	//@fatal: fuck ODR
	imports forceinline constexpr UNIT expr_m () noexcept {
		return UNIT (SIDE) ;
	}
} ;

using ENUM_TRUE = ENUMAS<csc_bool_t ,true> ;

using ENUM_FALSE = ENUMAS<csc_bool_t ,false> ;

template <class...>
struct TYPEID {} ;

template <class...UNIT>
struct TYPEAS {
	//@fatal: fuck ODR
	imports forceinline constexpr TYPEID<UNIT...> expr_m () noexcept {
		return TYPEID<UNIT...> () ;
	}
} ;

template <class UNIT ,class SIDE>
struct TEMPAS {
	SIDE mUnused ;
} ;

template <class UNIT>
struct TEMPAS<UNIT ,void> ;

using DEPEND = bool ;
using ALWAYS = void ;

template <class...>
trait REQUIRE_SPEC ;

template <>
trait REQUIRE_SPEC<ENUM_TRUE> {
	using RET = ALWAYS ;
} ;

template <class UNIT>
using REQUIRE = typename REQUIRE_SPEC<UNIT>::RET ;

template <class...>
trait DEPENDENT_SPEC ;

template <class UNIT ,class SIDE>
trait DEPENDENT_SPEC<UNIT ,SIDE> {
	using RET = UNIT ;
} ;

template <class UNIT ,class SIDE>
using DEPENDENT = typename DEPENDENT_SPEC<UNIT ,SIDE>::RET ;

#ifdef __CSC_CXX_LITE__
template <class...>
trait IS_SAME_SPEC ;

template <class UNIT>
trait IS_SAME_SPEC<UNIT ,UNIT> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT ,class SIDE>
trait IS_SAME_SPEC<UNIT ,SIDE> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT ,class SIDE>
using IS_SAME = typename IS_SAME_SPEC<UNIT ,SIDE>::RET ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT ,class SIDE>
using IS_SAME = ENUMAS<csc_bool_t ,std::is_same<UNIT ,SIDE>::value> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class...>
trait REMOVE_REF_SPEC ;

template <class UNIT>
trait REMOVE_REF_SPEC<UNIT> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait REMOVE_REF_SPEC<DEF<UNIT &>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait REMOVE_REF_SPEC<DEF<UNIT &&>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait REMOVE_REF_SPEC<DEF<const UNIT>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait REMOVE_REF_SPEC<DEF<const UNIT &>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait REMOVE_REF_SPEC<DEF<const UNIT &&>> {
	using RET = UNIT ;
} ;

template <class UNIT>
using REMOVE_REF = typename REMOVE_REF_SPEC<UNIT>::RET ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using REMOVE_REF = typename std::remove_const<typename std::remove_reference<UNIT>::type>::type ;
#endif

template <class UNIT ,class = REQUIRE<IS_SAME<UNIT ,REMOVE_REF<UNIT>>>>
using VREF = DEF<UNIT &> ;

template <class UNIT ,class = REQUIRE<IS_SAME<UNIT ,REMOVE_REF<UNIT>>>>
using CREF = DEF<const UNIT &> ;

template <class UNIT ,class = REQUIRE<IS_SAME<UNIT ,REMOVE_REF<UNIT>>>>
using RREF = DEF<UNIT &&> ;

template <class UNIT>
using XREF = DEF<UNIT &&> ;

struct FUNCTION_MACRO_A {} ;
struct FUNCTION_MACRO_B {} ;
struct FUNCTION_MACRO_C {} ;
struct FUNCTION_MACRO_D {} ;

#ifdef __CSC_VER_DEBUG__
template <class DEPEND>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_VER_DEBUG__
template <class DEPEND>
using MACRO_VER_DEBUG = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_VER_UNITTEST__
template <class DEPEND>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_VER_UNITTEST__
template <class DEPEND>
using MACRO_VER_UNITTEST = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_VER_RELEASE__
template <class DEPEND>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifndef __CSC_VER_RELEASE__
template <class DEPEND>
using MACRO_VER_RELEASE = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_MSVC__
template <class DEPEND>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_MSVC__
template <class DEPEND>
using MACRO_COMPILER_MSVC = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class DEPEND>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_GNUC__
template <class DEPEND>
using MACRO_COMPILER_GNUC = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class DEPEND>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifndef __CSC_COMPILER_CLANG__
template <class DEPEND>
using MACRO_COMPILER_CLANG = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
template <class DEPEND>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
template <class DEPEND>
using MACRO_SYSTEM_WINDOWS = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
template <class DEPEND>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_SYSTEM_LINUX__
template <class DEPEND>
using MACRO_SYSTEM_LINUX = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_X86__
template <class DEPEND>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_X86__
template <class DEPEND>
using MACRO_PLATFORM_X86 = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_X64__
template <class DEPEND>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_X64__
template <class DEPEND>
using MACRO_PLATFORM_X64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_ARM__
template <class DEPEND>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_ARM__
template <class DEPEND>
using MACRO_PLATFORM_ARM = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifdef __CSC_PLATFORM_ARM64__
template <class DEPEND>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_D ,DEPEND>> ;
#endif

#ifndef __CSC_PLATFORM_ARM64__
template <class DEPEND>
using MACRO_PLATFORM_ARM64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_D ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_EXE__
template <class DEPEND>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_EXE__
template <class DEPEND>
using MACRO_TARGET_EXE = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_DLL__
template <class DEPEND>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_DLL__
template <class DEPEND>
using MACRO_TARGET_DLL = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_TARGET_LIB__
template <class DEPEND>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifndef __CSC_TARGET_LIB__
template <class DEPEND>
using MACRO_TARGET_LIB = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_C ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_VAL32__
template <class DEPEND>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_VAL32__
template <class DEPEND>
using MACRO_CONFIG_VAL32 = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_VAL64__
template <class DEPEND>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_VAL64__
template <class DEPEND>
using MACRO_CONFIG_VAL64 = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_STRA__
template <class DEPEND>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_STRA__
template <class DEPEND>
using MACRO_CONFIG_STRA = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_CONFIG_STRW__
template <class DEPEND>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_CONFIG_STRW__
template <class DEPEND>
using MACRO_CONFIG_STRW = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class DEPEND>
using MACRO_CXX_LITE = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class DEPEND>
using MACRO_CXX_LITE = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_A ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_FULL__
template <class DEPEND>
using MACRO_CXX_FULL = DEPENDENT<ENUM_TRUE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifndef __CSC_CXX_FULL__
template <class DEPEND>
using MACRO_CXX_FULL = DEPENDENT<ENUM_FALSE ,DEPENDENT<FUNCTION_MACRO_B ,DEPEND>> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_INTCLASS = ENUMAS<csc_bool_t ,(__is_enum (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_INTCLASS = ENUMAS<csc_bool_t ,(std::is_enum<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_CLASS = ENUMAS<csc_bool_t ,(__is_class (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_CLASS = ENUMAS<csc_bool_t ,(std::is_class<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_nothrow_constructible (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_nothrow_constructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_DESTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_nothrow_destructible (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_DESTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_nothrow_destructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_constructible (UNIT ,CREF<UNIT>))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_copy_constructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_nothrow_constructible (UNIT ,RREF<UNIT>))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_nothrow_move_constructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_COPY_ASSIGNABLE = ENUMAS<csc_bool_t ,(__is_assignable (VREF<UNIT> ,CREF<UNIT>))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_COPY_ASSIGNABLE = ENUMAS<csc_bool_t ,(std::is_copy_assignable<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_MOVE_ASSIGNABLE = ENUMAS<csc_bool_t ,(__is_assignable (VREF<UNIT> ,RREF<UNIT>))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_MOVE_ASSIGNABLE = ENUMAS<csc_bool_t ,(std::is_move_assignable<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_TRIVIAL_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_trivially_constructible (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_TRIVIAL_CONSTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_trivially_default_constructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_TRIVIAL_DESTRUCTIBLE = ENUMAS<csc_bool_t ,(__is_trivially_destructible (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_TRIVIAL_DESTRUCTIBLE = ENUMAS<csc_bool_t ,(std::is_trivially_destructible<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_INTERFACE = ENUMAS<csc_bool_t ,(__is_abstract (UNIT))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class UNIT>
using MACRO_IS_INTERFACE = ENUMAS<csc_bool_t ,(std::is_abstract<UNIT>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class INTO>
using MACRO_IS_EXTEND = ENUMAS<csc_bool_t ,(__is_base_of (FROM ,INTO))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class INTO>
using MACRO_IS_EXTEND = ENUMAS<csc_bool_t ,(std::is_base_of<FROM ,INTO>::value)> ;
#endif

#ifdef __CSC_CXX_LITE__
template <class FROM ,class INTO>
using MACRO_IS_CONVERTIBLE = ENUMAS<csc_bool_t ,(__is_convertible_to (FROM ,INTO))> ;
#endif

#ifndef __CSC_CXX_LITE__
template <class FROM ,class INTO>
using MACRO_IS_CONVERTIBLE = ENUMAS<csc_bool_t ,(std::is_convertible<FROM ,INTO>::value)> ;
#endif

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_internel_name {
	template <class UNIT>
	imports csc_span_t __cdecl invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__FUNCSIG__) ;
		rax.mEnd = rax.mBegin + sizeof (__FUNCSIG__) - 1 ;
		rax.mBegin += sizeof ("struct CSC::csc_span_t __cdecl CSC::FUNCTION_internel_name::invoke<") - 1 ;
		rax.mEnd -= sizeof (">(void)") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_internel_name {
	template <class UNIT>
	imports __attribute__ ((cdecl)) csc_span_t invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__PRETTY_FUNCTION__) ;
		rax.mEnd = rax.mBegin + sizeof (__PRETTY_FUNCTION__) - 1 ;
		rax.mBegin += sizeof ("static CSC::csc_span_t CSC::FUNCTION_internel_name::invoke() [with UNIT = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_internel_name {
	template <class UNIT>
	imports csc_span_t __cdecl invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__PRETTY_FUNCTION__) ;
		rax.mEnd = rax.mBegin + sizeof (__PRETTY_FUNCTION__) - 1 ;
		rax.mBegin += sizeof ("static CSC::csc_span_t CSC::FUNCTION_internel_name::invoke() [UNIT = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_internel_span {
	template <class ARG1>
	inline csc_span_t operator() (CREF<csc_initializer_t<ARG1>> list) const noexcept {
		auto rax = csc_span_t () ;
		const auto r1x = DEF<const csc_diff_t *> (&list) ;
		rax.mBegin = r1x[0] ;
		rax.mEnd = r1x[1] ;
		rax.mStep = sizeof (ARG1) ;
		return rax ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_internel_span {
	template <class ARG1>
	inline csc_span_t operator() (CREF<csc_initializer_t<ARG1>> list) const noexcept {
		auto rax = csc_span_t () ;
		const auto r1x = DEF<const csc_diff_t *> (&list) ;
		rax.mBegin = r1x[0] ;
		rax.mEnd = rax.mBegin + r1x[1] * sizeof (ARG1) ;
		rax.mStep = sizeof (ARG1) ;
		return rax ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_internel_span {
	template <class ARG1>
	inline csc_span_t operator() (CREF<csc_initializer_t<ARG1>> list) const noexcept {
		auto rax = csc_span_t () ;
		const auto r1x = DEF<const csc_diff_t *> (&list) ;
		rax.mBegin = r1x[0] ;
		rax.mEnd = r1x[1] ;
		rax.mStep = sizeof (ARG1) ;
		return rax ;
	}
} ;
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