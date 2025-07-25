﻿#pragma once

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

#if defined __GNUC__
#define __CSC_COMPILER_GNUC__
#elif defined __clang__
#define __CSC_COMPILER_CLANG__
#elif defined _MSC_VER
#define __CSC_COMPILER_MSVC__
#else
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#ifdef __CUDACC__
#define __CSC_COMPILER_NVCC__
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

#ifdef _CPPRTTI
#define __CSC_CXX_RTTI__
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
#pragma warning (disable :4068) //@info: warning C4068: reflect pragma
#pragma warning (disable :4100) //@info: warning C4100: 'xxx': unreferenced formal parameter
#pragma warning (disable :4127) //@info: warning C4127: conditional expression is constant
#pragma warning (disable :4266) //@info: warning C4266: 'xxx'; function is hidden
#pragma warning (disable :4297) //@info: warning C4297: 'xxx': function assumed not to throw an exception but does
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4459) //@info: warning C4459: declaration of 'xxx' hides global declaration
#pragma warning (disable :4464) //@info: warning C4464: relative include dire contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced inline function has been removed
#pragma warning (disable :4571) //@info: warning C4571: Reflectrmational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning (disable :4574) //@info: warning C4574: 'xxx' is defined to be '0': did you mean to use '#if xxx'?
#pragma warning (disable :4584) //@info: warning C4584: 'xxx': base-class 'xxx' is already a base-class of 'xxx'
#pragma warning (disable :4619) //@info: warning C4619: #pragma warning: there is no warning number 'xxx'
#pragma warning (disable :4623) //@info: warning C4623: 'xxx': default constructor was implicitly defined as deleted
#pragma warning (disable :4624) //@info: warning C4624: 'xxx': destructor was implicitly defined as deleted
#pragma warning (disable :4625) //@info: warning C4625: 'xxx': copy constructor was implicitly defined as deleted
#pragma warning (disable :4626) //@info: warning C4626: 'xxx': assignment operator was implicitly defined as deleted
#pragma warning (disable :4643) //@info: warning C4643: Forward declaring 'initializer_list' in namespace std is not permitted by the C++ Standard.
#pragma warning (disable :4661) //@info: 'xxx': no suitable definition provided for explicit template instantiation request
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
#pragma warning (disable :5246) //@info: warning C5246: 'xxx': the initialization of a subobject should be wrapped in braces
#pragma warning (disable :5039) //@info: warning C5039: 'xxx': pointer or reference to potentially throwing function passed to 'xxx' function under -EHc. Undefined behavior may occur if this function throws an exception.
#pragma warning (disable :26440) //@info: warning C26440: Function 'xxx' can be declared 'noexcept' (f.6).
#pragma warning (disable :26447) //@info: warning C26447: The function is declared 'noexcept' but calls function 'xxx' which may throw exceptions (f.6).
#pragma warning (disable :26475) //@info: warning C26475: Do not use function style casts (es.49). Prefer 'Type{value}' over 'Type(value)'..
#pragma warning (disable :26485) //@info: warning C26485: Expression 'xxx': No array to pointer decay (bounds.3).
#pragma warning (disable :26490) //@info: warning C26490: Don't use reinterpret_cast (type.1).
#pragma warning (disable :26493) //@info: warning C26494: Don't use C-style casts (type.4).
#pragma warning (disable :26495) //@info: warning C26495: Variable 'xxx' is uninitialized. Always initialize a member variable (type.6).
#pragma warning (disable :26497) //@info: warning C26497: You can attempt to make 'xxx' constexpr unless it contains any undefined behavior (f.4).
#pragma warning (disable :26496) //@info: warning C26496: The variable 'xxx' does not change after construction, mark it as const (con.4).
#pragma warning (disable :26814) //@info: warning C26814: The const variable 'r5x' can be computed at compile-time. Consider using constexpr (con.5).
#pragma warning (disable :26820) //@info: warning C26820: This is a potentially expensive copy operation. Consider using a reference unless a copy is required (p.9).
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic warning "-Wsuggest-override"
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmicrosoft-template"
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#endif

#include "csc_end.h"

#ifdef __CSC_COMPILER_GNUC__
#if __GLIBCXX__ <= 20230528L
//@fatal: GCC is so bad
#include <type_traits>
#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_trivially_constructible(...) std::is_trivially_constructible<__VA_ARGS__>::value
#define __is_trivially_destructible(...) std::is_trivially_destructible<__VA_ARGS__>::value
#endif
#endif

namespace std {
//@fatal: STL is so bad
template <class>
class initializer_list ;
} ;
#include "csc_begin.h"

#ifndef __macro_dllextern
#ifdef __CSC_COMPILER_MSVC__
#define __macro_dllextern __declspec (dllexport)
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_dllextern
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_dllextern __declspec (dllexport)
#endif
#endif

#ifndef __macro_exports
#ifdef __CSC_COMPILER_MSVC__
#define __macro_exports __macro_dllextern
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_exports __attribute__ ((visibility ("default"))) __macro_dllextern
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_exports __attribute__ ((visibility ("default"))) __macro_dllextern
#endif
#endif

#ifndef __macro_imports
#ifdef __CSC_COMPILER_MSVC__
#define __macro_imports static __macro_dllextern
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_imports static __macro_dllextern
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_imports static __macro_dllextern
#endif
#endif

#ifndef __macro_forceinline
#ifdef __CSC_COMPILER_MSVC__
#define __macro_forceinline inline __forceinline
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_forceinline inline __attribute__ ((always_inline))
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_forceinline inline
#endif
#endif

#ifndef __macro_ex
#define __macro_ex(a) a
#endif

#ifndef __macro_sz
#define __macro_sz_impl(...) #__VA_ARGS__
#define __macro_sz(...) __macro_ex (__macro_sz_impl (__VA_ARGS__))
#endif

#ifndef __macro_cc
#define __macro_cc_impl(a ,b) a##b
#define __macro_cc(...) __macro_ex (__macro_cc_impl (__VA_ARGS__))
#endif

#ifndef __macro_require
#define __macro_require(...) static_assert (CSC::DEF<__VA_ARGS__>::expr ,"require : " __macro_sz (__VA_ARGS__))
#endif

#ifndef __macro_anonymous
#define __macro_anonymous __macro_cc (__anonymous_ ,__LINE__)
#endif

#ifndef __macro_slice
#ifdef __CSC_CONFIG_STRA__
#define __macro_slice(...) CSC::Slice (__VA_ARGS__)
#endif

#ifdef __CSC_CONFIG_STRW__
#define __macro_slice(...) CSC::Slice (__macro_cc (L ,__VA_ARGS__))
#endif
#endif

#ifndef __macro_break
#ifdef __CSC_COMPILER_MSVC__
#define __macro_break __debugbreak
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_break __builtin_trap
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_break __builtin_trap
#endif
#endif

#ifndef __macro_assert
#ifdef __CSC_VER_DEBUG__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; __macro_break () ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_assert(...) do { if (__VA_ARGS__) break ; if (inline_unittest ()) { __macro_break () ; } else { inline_abort () ; } } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_assert(...) do {} while (false)
#endif
#endif

#ifndef __macro_function
#ifdef __CSC_COMPILER_MSVC__
#define __macro_function __FUNCSIG__
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_function __PRETTY_FUNCTION__
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_function __PRETTY_FUNCTION__
#endif
#endif

#ifndef __macro_assume
#ifdef __CSC_VER_DEBUG__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; throw CSC::Exception (slice (__macro_sz (__VA_ARGS__)) ,CSC::Slice (__macro_function) ,slice (__FILE__) ,slice (__macro_sz (__LINE__))) ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; throw CSC::Exception (slice (__macro_sz (__VA_ARGS__)) ,CSC::Slice (__macro_function) ,slice (__FILE__) ,slice (__macro_sz (__LINE__))) ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_assume(...) do { if (__VA_ARGS__) break ; throw CSC::Exception (slice (__macro_sz (__VA_ARGS__)) ,CSC::Slice (__FUNCTION__) ,CSC::Slice () ,CSC::Slice ()) ; } while (false)
#endif
#endif

#ifndef __macro_notice
#ifdef __CSC_VER_DEBUG__
#define __macro_notice(...) do { struct LINE ; CSC::inline_notice (TYPE<LINE>::expr ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_UNITTEST__
#define __macro_notice(...) do { struct LINE ; CSC::inline_notice (TYPE<LINE>::expr ,__VA_ARGS__) ; } while (false)
#endif

#ifdef __CSC_VER_RELEASE__
#define __macro_notice(...) do {} while (false)
#endif
#endif

#ifndef __macro_ifdo
#define __macro_ifdo(a) (a) for (auto anonymous = true ; anonymous ; anonymous = CSC::inline_ifdo (a))
#endif

#ifndef __macro_typeof
#define __macro_typeof(...) CSC::REMOVE_CVR<decltype (__VA_ARGS__)>
#endif

#ifndef __macro_nullof
#define __macro_nullof(...) CSC::FUNCTION_nullof::invoke<CSC::DEF<__VA_ARGS__> &> ()
#endif

#ifndef __macro_for_bind
#define __macro_for_bind_0(F ,...)
#define __macro_for_bind_1(F ,a) F (a)
#define __macro_for_bind_2(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_1 (F ,__VA_ARGS__))
#define __macro_for_bind_3(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_2 (F ,__VA_ARGS__))
#define __macro_for_bind_4(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_3 (F ,__VA_ARGS__))
#define __macro_for_bind_5(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_4 (F ,__VA_ARGS__))
#define __macro_for_bind_6(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_5 (F ,__VA_ARGS__))
#define __macro_for_bind_7(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_6 (F ,__VA_ARGS__))
#define __macro_for_bind_8(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_7 (F ,__VA_ARGS__))
#define __macro_for_bind_9(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_8 (F ,__VA_ARGS__))
#define __macro_for_bind_X(F ,a ,...) F (a) ,__macro_ex (__macro_for_bind_9 (F ,__VA_ARGS__))
#define __macro_for_bind_choose(F ,a0 ,a1 ,a2 ,a3 ,a4 ,a5 ,a6 ,a7 ,a8 ,a9 ,ax ,...) ax
#define __macro_for_bind_impl(F ,...) __macro_ex (__macro_for_bind_choose(F ,__VA_ARGS__ ,__macro_for_bind_X ,__macro_for_bind_9 ,__macro_for_bind_8 ,__macro_for_bind_7 ,__macro_for_bind_6 ,__macro_for_bind_5 ,__macro_for_bind_4 ,__macro_for_bind_3 ,__macro_for_bind_2 ,__macro_for_bind_1 ,__macro_for_bind_0) (F ,__VA_ARGS__))
#define __macro_for_bind(F ,...) __macro_ex (__macro_for_bind_impl (F ,__VA_ARGS__))
#endif

struct HINSTANCE__ ;

namespace CSC {
template <class A>
using DEF = A ;

using csc_bool_t = bool ;

using csc_int32_t = int ;
using csc_int64_t = DEF<long long> ;

using csc_float32_t = float ;
using csc_float64_t = double ;

#ifndef __CSC_CXX_LATEST__
#define consteval constexpr
#endif

struct FUNCTION_infinity {
	forceinline consteval operator csc_float32_t () const noexcept {
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

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_CONFIG_VAL32__
using csc_diff_t = int ;
using csc_size_t = DEF<unsigned int> ;
using csc_enum_t = DEF<unsigned long> ;
using csc_handle_t = DEF<void *> ;
using csc_device_t = DEF<HINSTANCE__ *> ;
using csc_pipe_t = DEF<void *> ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using csc_diff_t = DEF<long long> ;
using csc_size_t = DEF<unsigned long long> ;
using csc_enum_t = DEF<unsigned long> ;
using csc_handle_t = DEF<void *> ;
using csc_device_t = DEF<HINSTANCE__ *> ;
using csc_pipe_t = DEF<void *> ;
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_diff_t = DEF<long int> ;
using csc_size_t = DEF<long unsigned int> ;
using csc_enum_t = int ;
using csc_handle_t = DEF<void *> ;
using csc_device_t = DEF<HINSTANCE__ *> ;
using csc_pipe_t = int ;
#endif

template <class A>
using csc_initializer_list_t = std::initializer_list<A> ;

struct FUNCTION_nullof {
	template <class A>
	forceinline static consteval A invoke () noexcept ;
} ;

template <csc_diff_t A>
struct ENUM {
	forceinline static consteval csc_diff_t expr_m () noexcept {
		return A ;
	}
} ;

using ENUM_TRUE = ENUM<true> ;

using ENUM_FALSE = ENUM<false> ;

template <class...A>
struct TYPE {
	forceinline static consteval TYPE expr_m () noexcept {
		return TYPE () ;
	}
} ;

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

#ifdef __CSC_COMPILER_MSVC__
template <class...>
trait IS_SAME_HELP ;

template <class A>
trait IS_SAME_HELP<A ,A> {
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
trait IS_SAME_HELP<A ,B> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class B>
using IS_SAME = typename IS_SAME_HELP<A ,B>::RET ;
#endif

#ifdef __CSC_COMPILER_GNUC__
template <class A ,class B>
using IS_SAME = ENUM<(__is_same (A ,B))> ;
#endif

#ifdef __CSC_COMPILER_CLANG__
template <class A ,class B>
using IS_SAME = ENUM<(__is_same (A ,B))> ;
#endif

template <class...>
trait REMOVE_CVR1_HELP ;

template <class A>
trait REMOVE_CVR1_HELP<A> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_CVR1_HELP<A &> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_CVR1_HELP<A &&> {
	using RET = A ;
} ;

template <class...>
trait REMOVE_CVR2_HELP ;

template <class A>
trait REMOVE_CVR2_HELP<A> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_CVR2_HELP<const A> {
	using RET = A ;
} ;

template <class A>
trait REMOVE_CVR2_HELP<volatile A> {
	using RET = A ;
} ;

template <class A>
using REMOVE_CVR = typename REMOVE_CVR2_HELP<typename REMOVE_CVR1_HELP<A>::RET>::RET ;

template <class A>
using CHECK_CVR = REQUIRE<IS_SAME<A ,REMOVE_CVR<A>>> ;

template <class A ,class = CHECK_CVR<A>>
using VREF = DEF<A &> ;

template <class A ,class = CHECK_CVR<A>>
using CREF = DEF<const A &> ;

template <class A ,class = CHECK_CVR<A>>
using RREF = DEF<A &&> ;

template <class A>
using XREF = DEF<A &&> ;

template <class A>
using MACRO_IS_UNDER1 = ENUM<(__is_enum (A))> ;

template <class A>
using MACRO_IS_UNDER2 = ENUM<(__is_union (A))> ;

template <class A>
using MACRO_IS_CLASS = ENUM<(__is_class (A))> ;

template <class A>
using MACRO_IS_CONSTRUCTIBLE = ENUM<(__is_nothrow_constructible (A))> ;

template <class A>
using MACRO_IS_DESTRUCTIBLE = ENUM<(__is_nothrow_destructible (A))> ;

template <class A>
using MACRO_IS_COPY_CONSTRUCTIBLE = ENUM<(__is_constructible (A ,CREF<A>))> ;

template <class A>
using MACRO_IS_MOVE_CONSTRUCTIBLE = ENUM<(__is_nothrow_constructible (A ,RREF<A>))> ;

template <class A>
using MACRO_IS_COPY_ASSIGNABLE = ENUM<(__is_assignable (VREF<A> ,CREF<A>))> ;

template <class A>
using MACRO_IS_MOVE_ASSIGNABLE = ENUM<(__is_assignable (VREF<A> ,RREF<A>))> ;

template <class A>
using MACRO_IS_TRIVIAL_CONSTRUCTIBLE = ENUM<(__is_trivially_constructible (A))> ;

template <class A>
using MACRO_IS_TRIVIAL_DESTRUCTIBLE = ENUM<(__is_trivially_destructible (A))> ;

template <class A ,class B>
using MACRO_IS_EXTEND = ENUM<(__is_base_of (A ,B))> ;

#ifndef __macro_memcpy
#ifdef __CSC_COMPILER_MSVC__
#define __macro_memcpy invoke
#define __macro_memset invoke
#define __macro_memcmp invoke
#endif

#ifdef __CSC_COMPILER_GNUC__
#define __macro_memcpy(a ,b ,c) (void) __builtin_memcpy ((&a) ,(&b) ,c)
#define __macro_memset(a ,b) (void) __builtin_memset ((&a) ,0 ,b)
#define __macro_memcmp(a ,b ,c) CSC::FLAG (__builtin_memcmp ((&a) ,(&b) ,c))
#endif

#ifdef __CSC_COMPILER_CLANG__
#define __macro_memcpy(a ,b ,c) (void) __builtin_memcpy ((&a) ,(&b) ,c)
#define __macro_memset(a ,b) (void) __builtin_memset ((&a) ,0 ,b)
#define __macro_memcmp(a ,b ,c) CSC::FLAG (__builtin_memcmp ((&a) ,(&b) ,c))
#endif
#endif

#ifndef __macro_type_rtti
#ifdef __CSC_CXX_RTTI__
#define __macro_type_rtti ""
#endif

#ifndef __CSC_CXX_RTTI__
#define __macro_type_rtti __macro_function
#endif
#endif
} ;

forceinline CSC::csc_handle_t operator new (CSC::csc_size_t ,CSC::csc_device_t where_) noexcept {
	return CSC::csc_handle_t (where_) ;
}

forceinline void operator delete (CSC::csc_handle_t ,CSC::csc_device_t where_) noexcept {
	return ;
}