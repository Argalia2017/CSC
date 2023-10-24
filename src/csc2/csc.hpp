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

#ifdef _MSVC_LANG
#if _MSVC_LANG >= 202002L
#define __CSC_CXX_LATEST__
#endif
#else
#if __cplusplus >= 202002L
#define __CSC_CXX_LATEST__
#endif
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4068) //@info: warning C4068: unknown pragma
#pragma warning (disable :4100) //@info: warning C4100: 'xxx': unreferenced formal parameter
#pragma warning (disable :4127) //@info: warning C4127: conditional expression is constant
#pragma warning (disable :4266) //@info: warning C4266: 'xxx'; function is hidden
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4459) //@info: warning C4459: declaration of 'xxx' hides global declaration
#pragma warning (disable :4464) //@info: warning C4464: relative include path contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced forceinline function has been removed
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
#pragma warning (disable :4714) //@info: warning C4714: function 'xxx' marked as __forceinline not inlined
#pragma warning (disable :4717) //@info: warning C4717: 'xxx': recursive on all control paths, function will cause runtime stack overflow
#pragma warning (disable :4774) //@info: warning C4774: 'xxx' : format string expected in argument xxx is not a string literal
#pragma warning (disable :4820) //@info: warning C4820: 'xxx': 'xxx' bytes padding added after data member 'xxx'
#pragma warning (disable :5026) //@info: warning C5026: 'xxx': move constructor was implicitly defined as deleted
#pragma warning (disable :5027) //@info: warning C5027: 'xxx': move assignment operator was implicitly defined as deleted
#pragma warning (disable :5045) //@info: warning C5045: 'xxx': move assignment operator was implicitly defined as deleted
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
#ifdef __CSC_COMPILER_GNUC__
//@fatal: fuck gnuc
#include <type_traits>
#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_convertible_to(...) std::is_convertible<__VA_ARGS__>::value
#define __is_trivial std::is_trivial<__VA_ARGS__>::value
#endif
#include "csc_begin.h"

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
#define __macro_requires(...) static_assert (CSC::DEF<__VA_ARGS__>::expr ,"static_assert failed : " __macro_str (__VA_ARGS__))
#endif

#ifndef __macro_anonymous
#define __macro_anonymous __macro_cat (__anonymous_ ,__LINE__)
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
#define __macro_unittest(...) do { CSC::unsafe_watch (CSC::TYPEAS<CSC::ENUMAS<CSC::csc_uint32_t ,__COUNTER__>>::expr ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_unittest(...) do { CSC::unsafe_watch (CSC::TYPEAS<CSC::ENUMAS<CSC::csc_uint32_t ,__COUNTER__>>::expr ,slice (__macro_str (__VA_ARGS__)) ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_unittest(...)
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
template <class A>
using DEF = A ;

using csc_bool_t = bool ;

using csc_int32_t = int ;
using csc_int64_t = DEF<long long> ;

using csc_float32_t = float ;
using csc_float64_t = double ;

#ifndef __CSC_CXX_LATEST__
//@fatal: fuck ODR
#define consteval constexpr
#endif

struct FUNCTION_infinity {
	inline consteval operator csc_float32_t () const noexcept {
		return csc_float32_t (__builtin_huge_valf ()) ;
	}
} ;

static constexpr auto infinity = FUNCTION_infinity () ;

using csc_uint8_t = DEF<unsigned char> ;
using csc_uint16_t = DEF<unsigned short> ;
using csc_uint32_t = DEF<unsigned int> ;
using csc_uint64_t = DEF<unsigned long long> ;

using csc_char_t = char ;
using csc_wchar_t = wchar_t ;

#ifdef __CSC_CXX_LATEST__
using csc_char8_t = char8_t ;
#endif

#ifndef __CSC_CXX_LATEST__
using csc_char8_t = csc_uint8_t ;
#endif

using csc_char16_t = char16_t ;
using csc_char32_t = char32_t ;

using csc_pointer_t = DEF<const void *> ;

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

template <csc_diff_t B>
struct ENUMAS {
	imports inline consteval csc_diff_t expr_m () noexcept {
		return csc_diff_t (B) ;
	}
} ;

using ENUM_TRUE = ENUMAS<true> ;

using ENUM_FALSE = ENUMAS<false> ;

template <class...>
struct TYPEID {} ;

template <class...A>
struct TYPEAS {
	imports inline consteval TYPEID<A...> expr_m () noexcept {
		return TYPEID<A...> () ;
	}
} ;

using DEPEND = bool ;
using ALWAYS = void ;

template <class...>
trait REQUIRE_HELP ;

template <>
trait REQUIRE_HELP<ENUM_TRUE> {
	using RET = ALWAYS ;
} ;

template <class A>
using REQUIRE = typename REQUIRE_HELP<A>::RET ;

template <class...>
trait KILL_HELP ;

template <class A ,class B>
trait KILL_HELP<A ,B> {
	using RET = A ;
} ;

template <class A ,class B>
using KILL = typename KILL_HELP<A ,B>::RET ;

template <class...>
trait REMOVE_REF_HELP ;

template <class A>
trait REMOVE_REF_HELP<A> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_REF_HELP<DEF<A &>> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_REF_HELP<DEF<A &&>> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_REF_HELP<DEF<const A>> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_REF_HELP<DEF<const A &>> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_REF_HELP<DEF<const A &&>> {
	using RET = A ;
} ;

template <class A>
using REMOVE_REF = typename REMOVE_REF_HELP<A>::RET ;

template <class...>
struct IS_SAME_HELP ;

template <class A>
struct IS_SAME_HELP<A ,A> {
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
struct IS_SAME_HELP<A ,B> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class B>
using IS_SAME = typename IS_SAME_HELP<A ,B>::RET ;

template <class A ,class = REQUIRE<IS_SAME<A ,REMOVE_REF<A>>>>
using VREF = DEF<A &> ;

template <class A ,class = REQUIRE<IS_SAME<A ,REMOVE_REF<A>>>>
using CREF = DEF<const A &> ;

template <class A ,class = REQUIRE<IS_SAME<A ,REMOVE_REF<A>>>>
using RREF = DEF<A &&> ;

template <class A>
using XREF = DEF<A &&> ;

template <class A>
using MACRO_IS_INTPTR = ENUMAS<(__is_enum (A))> ;

template <class A>
using MACRO_IS_CLASS = ENUMAS<(__is_class (A))> ;

template <class A>
using MACRO_IS_CONSTRUCTIBLE = ENUMAS<(__is_nothrow_constructible (A))> ;

template <class A>
using MACRO_IS_DESTRUCTIBLE = ENUMAS<(__is_nothrow_destructible (A))> ;

template <class A>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUMAS<(__is_constructible (A ,CREF<A>))> ;

template <class A>
using MACRO_IS_COPY_ASSIGNABLE = ENUMAS<(__is_assignable (A ,CREF<A>))> ;

template <class A>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUMAS<(__is_nothrow_constructible (A ,CREF<A>))> ;

template <class A>
using MACRO_IS_MOVE_ASSIGNABLE = ENUMAS<(__is_assignable (VREF<A> ,RREF<A>))> ;

template <class A ,class B>
using MACRO_IS_CONVERTIBLE = ENUMAS<(__is_convertible_to (A ,B))> ;

template <class A>
using MACRO_IS_TRIVIAL = ENUMAS<(__is_trivial (A))> ;

template <class A>
using MACRO_IS_INTERFACE = ENUMAS<(__is_polymorphic (A))> ;

template <class A ,class B>
using MACRO_IS_EXTEND = ENUMAS<(__is_base_of (A ,B))> ;
} ;