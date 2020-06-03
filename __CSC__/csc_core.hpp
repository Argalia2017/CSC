#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
namespace stl {
using std::int32_t ;
using std::int64_t ;
using std::uint8_t ;
using std::uint16_t ;
using std::uint32_t ;
using std::uint64_t ;

using std::nothrow ;

#ifndef __CSC_COMPILER_GNUC__
using std::max_align_t ;
#endif

#ifdef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
using ::max_align_t ;
#endif

using std::numeric_limits ;
using std::initializer_list ;

using std::is_reference ;
using std::is_lvalue_reference ;
using std::is_rvalue_reference ;
using std::is_const ;
using std::is_volatile ;
using std::is_class ;
using std::is_pointer ;
using std::is_member_pointer ;
using std::is_array ;
using std::is_function ;
using std::is_pod ;

#ifndef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
using std::is_trivial ;
using std::is_final ;
#endif

using std::is_same ;
using std::is_base_of ;
using std::is_abstract ;
using std::is_default_constructible ;
using std::is_constructible ;
using std::is_nothrow_destructible ;
using std::is_copy_constructible ;
using std::is_copy_assignable ;
using std::is_nothrow_move_constructible ;
using std::is_nothrow_move_assignable ;
using std::is_convertible ;
using std::is_integral ;

using std::exception ;
using std::atomic ;
} ;

#define M_DATE __DATE__
#define M_HOUR __TIME__
#define M_FILE __FILE__
#define M_LINE _STR_ (__LINE__)

#ifdef __CSC_COMPILER_MSVC__
#define M_FUNC __FUNCSIG__
#endif

#ifdef __CSC_COMPILER_GNUC__
#define M_FUNC __PRETTY_FUNCTION__
#endif

#ifdef __CSC_COMPILER_CLANG__
#define M_FUNC __PRETTY_FUNCTION__
#endif

#ifdef __CSC_COMPILER_MSVC__
#define DLLABI_IMPORT __declspec (dllimport)
#define DLLABI_EXPORT __declspec (dllexport)
#define DLLABI_API __stdcall
#define DLLABI_NATIVE extern "C"
#endif

#ifdef __CSC_COMPILER_GNUC__
#define DLLABI_IMPORT
#define DLLABI_EXPORT __attribute__ ((visibility ("default")))
#define DLLABI_API
#define DLLABI_NATIVE extern "C"
#endif

#ifdef __CSC_COMPILER_CLANG__
#define DLLABI_IMPORT
#define DLLABI_EXPORT __attribute__ ((visibility ("default")))
#define DLLABI_API
#define DLLABI_NATIVE extern "C"
#endif

#define _UNWIND_IMPL_(...) __VA_ARGS__
#define _UNW_(...) _UNWIND_IMPL_(__VA_ARGS__)
#define _STRINGIZE_IMPL_(...) #__VA_ARGS__
#define _STR_(...) _STRINGIZE_IMPL_(__VA_ARGS__)
#define _CONCAT_IMPL_(arg ,arg_) arg##arg_
#define _CAT_(arg ,arg_) _CONCAT_IMPL_(arg ,arg_)

#define _STATIC_ASSERT_(...) static_assert ((_UNW_ (__VA_ARGS__)) ,"static_assert failed : " _STR_ (__VA_ARGS__))

#define _STATIC_WARNING_(...)

#define _STATIC_UNUSED_(...) (void) _UNW_ (__VA_ARGS__) ;

#ifdef __CSC_COMPILER_MSVC__
#define _DYNAMIC_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) CSC::Exception (_PCSTR_ ("dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," M_LINE)).raise () ; } while (FALSE)
#endif

#ifdef __CSC_COMPILER_GNUC__
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) CSC::Exception (CSC::Plain<CSC::STR> (CSC::ARGV<ARGVPL>::null ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)).raise () ; } while (FALSE)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) CSC::Exception (CSC::Plain<CSC::STR> (CSC::ARGV<ARGVPL>::null ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)).raise () ; } while (FALSE)
#endif

#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_MSVC__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __debugbreak () ; } while (FALSE)
#endif

#ifdef __CSC_COMPILER_GNUC__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __builtin_trap () ; } while (FALSE)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) assert (FALSE) ; } while (FALSE)
#endif
#endif

#ifndef __CSC_DEBUG__
#ifdef __CSC_UNITTEST__
#define _DEBUG_ASSERT_ _DYNAMIC_ASSERT_
#endif
#endif

#ifndef _DEBUG_ASSERT_
#define _DEBUG_ASSERT_(...) do {} while (FALSE)
#endif

#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_MSVC__
#define _UNITTEST_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) CSC::GlobalWatch::done (CSC::Exception (_PCSTR_ ("unittest_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," M_LINE))) ; } while (FALSE)
#endif
#endif

#ifndef _UNITTEST_ASSERT_
#define _UNITTEST_ASSERT_(...) do {} while (FALSE)
#endif

#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_MSVC__
#define _UNITTEST_WATCH_(...) do { struct ARGVPL ; CSC::GlobalWatch::done (CSC::ARGV<ARGVPL>::null ,_PCSTR_ (_STR_ (__VA_ARGS__)) ,(_UNW_ (__VA_ARGS__))) ; } while (FALSE)
#endif
#endif

#ifndef _UNITTEST_WATCH_
#define _UNITTEST_WATCH_(...) do {} while (FALSE)
#endif

#define ANONYMOUS _CAT_ (_anonymous_ ,__LINE__)

#define _SWITCH_ONCE_(arg) (arg) goto ANONYMOUS ; while (CSC::U::OPERATOR_ONCE::invoke (arg)) ANONYMOUS:

using BOOL = bool ;

#ifdef FALSE
#undef FALSE
#endif
#define FALSE false

#ifdef TRUE
#undef TRUE
#endif
#define TRUE true

using VAR32 = stl::int32_t ;
using VAR64 = stl::int64_t ;

static constexpr auto VAR32_MAX = VAR32 (2147483647) ;
static constexpr auto VAR32_MIN = ~VAR32_MAX ;
static constexpr auto VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr auto VAR64_MIN = ~VAR64_MAX ;

#ifdef __CSC_CONFIG_VAR32__
using VAR = VAR32 ;

static constexpr auto VAR_MAX = VAR32_MAX ;
static constexpr auto VAR_MIN = VAR32_MIN ;
#endif

#ifdef __CSC_CONFIG_VAR64__
using VAR = VAR64 ;

static constexpr auto VAR_MAX = VAR64_MAX ;
static constexpr auto VAR_MIN = VAR64_MIN ;
#endif

static constexpr auto VAR_ZERO = VAR (+0) ;
static constexpr auto VAR_NONE = VAR (-1) ;
static constexpr auto VAR_USED = VAR (-2) ;

using VARX = long ;
using VARY = unsigned long ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

enum class EFLAG :VAR ;

#define _SIZEOF_(...) CSC::LENGTH (sizeof (CSC::U::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>))
#define _ALIGNOF_(...) CSC::LENGTH (alignof (CSC::U::REMOVE_CVR_TYPE<CSC::U::REMOVE_ARRAY_TYPE<_UNW_ (__VA_ARGS__)>>))
#define _COUNTOF_(...) CSC::LENGTH (CSC::U::COUNT_OF_TYPE<_UNW_ (__VA_ARGS__)>::value)
#define _CAPACITYOF_(...) CSC::LENGTH (CSC::U::CAPACITY_OF_TYPE<_UNW_ (__VA_ARGS__)>::value)

using VAL32 = float ;
using VAL64 = double ;

static constexpr auto VAL32_MAX = VAL32 (3.402823466E+38) ;
static constexpr auto VAL32_MIN = VAL32 (1.175494351E-38) ;
static constexpr auto VAL32_EPS = VAL32 (1.192092896E-07) ;
static constexpr auto VAL32_INF = stl::numeric_limits<VAL32>::infinity () ;
static constexpr auto VAL32_NAN = stl::numeric_limits<VAL32>::quiet_NaN () ;
static constexpr auto VAL64_MAX = VAL64 (1.7976931348623158E+308) ;
static constexpr auto VAL64_MIN = VAL64 (2.2250738585072014E-308) ;
static constexpr auto VAL64_EPS = VAL64 (2.2204460492503131E-016) ;
static constexpr auto VAL64_INF = stl::numeric_limits<VAL64>::infinity () ;
static constexpr auto VAL64_NAN = stl::numeric_limits<VAL64>::quiet_NaN () ;

#ifdef __CSC_CONFIG_VAL32__
using VAL = VAL32 ;

static constexpr auto VAL_MAX = VAL32_MAX ;
static constexpr auto VAL_MIN = VAL32_MIN ;
static constexpr auto VAL_EPS = VAL32_EPS ;
static constexpr auto VAL_INF = VAL32_INF ;
static constexpr auto VAL_NAN = VAL32_NAN ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using VAL = VAL64 ;

static constexpr auto VAL_MAX = VAL64_MAX ;
static constexpr auto VAL_MIN = VAL64_MIN ;
static constexpr auto VAL_EPS = VAL64_EPS ;
static constexpr auto VAL_INF = VAL64_INF ;
static constexpr auto VAL_NAN = VAL64_NAN ;
#endif

using VALX = long double ;

static constexpr auto MATH_E = VALX (2.71828182845904523536) ;
static constexpr auto MATH_PI = VALX (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = VALX (1.41421356237309504880) ;
static constexpr auto MATH_LN2 = VALX (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = VALX (2.30258509299404568402) ;

#ifdef VOID
#undef VOID
#endif
using VOID = void ;

#ifdef NONE
#undef NONE
#endif
struct NONE ;

#ifdef NULL
#undef NULL
#endif
#define NULL nullptr

template <class UNIT>
using DEF = UNIT ;

template <class UNIT>
using PTR = DEF<UNIT *> ;

template <class UNIT1 ,class UNIT2 = NONE>
using MEMPTR = DEF<UNIT1 UNIT2::*> ;

#ifndef __CSC_COMPILER_GNUC__
template <class UNIT>
using ARR = DEF<UNIT[]> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
template <class UNIT>
using ARR = DEF<UNIT[0]> ;
#endif

using BYTE = stl::uint8_t ;
using WORD = stl::uint16_t ;
using CHAR = stl::uint32_t ;
using DATA = stl::uint64_t ;

class __uint128_t {
private:
	alignas (16) DEF<BYTE[16]> unused ;
} ;

using MEGA = __uint128_t ;

using STRU8 = unsigned char ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;

//@error: fuck std
#define _PCSTRU8_(arg) CSC::Plain<CSC::STRU8> (_CAST_ (ARGV<STRU8[_COUNTOF_ (DEF<decltype (_CAT_ (u8 ,arg))>)]>::null ,_CAT_ (u8 ,arg)))
#define _PCSTRU16_(arg) CSC::Plain<CSC::STRU16> (_CAT_ (u ,arg))
#define _PCSTRU32_(arg) CSC::Plain<CSC::STRU32> (_CAT_ (U ,arg))

using STRA = char ;
using STRW = wchar_t ;

#define _PCSTRA_(arg) CSC::Plain<CSC::STRA> (_UNW_ (arg))
#define _PCSTRW_(arg) CSC::Plain<CSC::STRW> (_CAT_ (L ,arg))

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;

#define _PCSTR_ _PCSTRA_
#endif

#ifdef __CSC_CONFIG_STRW__
using STR = STRW ;

#define _PCSTR_ _PCSTRW_
#endif

using STRX = signed char ;

template <VAR _ARGC>
struct ARGC {
	enum :VAR {
		value = _ARGC
	} ;
} ;

using ZERO = ARGC<0> ;

template <class _ARG1>
using INCREASE = ARGC<(_ARG1::value + 1)> ;

template <class _ARG1>
using DECREASE = ARGC<(_ARG1::value - 1)> ;

template <class UNIT>
struct ARGV {
	imports DEF<void (const ARGV &)> null ;
} ;

template <class UNIT>
inline exports void ARGV<UNIT>::null (const ARGV &) {}

template <class _ARG1>
using ARGVF = DEF<void (const ARGV<_ARG1> &)> ;

template <class...>
struct ARGVS ;

template <class>
struct ARGVP ;

template <>
struct ARGV<ARGVP<ZERO>> {
	_STATIC_WARNING_ ("noop") ;
} ;

template <class UNIT>
struct ARGV<ARGVP<UNIT>>
	:public ARGV<ARGVP<DECREASE<UNIT>>> {
	_STATIC_ASSERT_ (UNIT::value > 0) ;
} ;

static constexpr auto ARGVP0 = ARGV<ARGVP<ZERO>> {} ;
static constexpr auto ARGVP1 = ARGV<ARGVP<ARGC<1>>> {} ;
static constexpr auto ARGVP2 = ARGV<ARGVP<ARGC<2>>> {} ;
static constexpr auto ARGVP3 = ARGV<ARGVP<ARGC<3>>> {} ;
static constexpr auto ARGVP4 = ARGV<ARGVP<ARGC<4>>> {} ;
static constexpr auto ARGVP5 = ARGV<ARGVP<ARGC<5>>> {} ;
static constexpr auto ARGVP6 = ARGV<ARGVP<ARGC<6>>> {} ;
static constexpr auto ARGVP7 = ARGV<ARGVP<ARGC<7>>> {} ;
static constexpr auto ARGVP8 = ARGV<ARGVP<ARGC<8>>> {} ;
static constexpr auto ARGVP9 = ARGV<ARGVP<ARGC<9>>> {} ;
static constexpr auto ARGVPX = ARGV<ARGVP<ARGC<10>>> {} ;

using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
using DEFAULT_HUGESTRING_SIZE = ARGC<8388607> ;

template <class ,BOOL...>
struct SPECIALIZATION ;

template <class>
struct TEMP ;

template <class...>
struct PACK ;

template <>
struct PACK<> {
	_STATIC_WARNING_ ("noop") ;
} ;

template <class UNIT1>
struct PACK<UNIT1> {
	UNIT1 mP1 ;
} ;

template <class UNIT1 ,class UNIT2>
struct PACK<UNIT1 ,UNIT2> {
	UNIT1 mP1 ;
	UNIT2 mP2 ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
struct PACK<UNIT1 ,UNIT2 ,UNIT3> {
	UNIT1 mP1 ;
	UNIT2 mP2 ;
	UNIT3 mP3 ;
} ;

#pragma region
namespace U {
template <class>
struct ENABLE ;

template <>
struct ENABLE<ARGC<TRUE>> {
	using TYPE = VOID ;
} ;

template <BOOL _ARGC>
using ENABLE_TYPE = typename ENABLE<ARGC<_ARGC>>::TYPE ;
} ;

namespace U {
template <class _ARG1 ,class>
struct DEPENDENT {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
using DEPENDENT_TYPE = typename DEPENDENT<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct CONDITIONAL ;

template <class _ARG1 ,class _ARG2>
struct CONDITIONAL<ARGC<TRUE> ,_ARG1 ,_ARG2> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
struct CONDITIONAL<ARGC<FALSE> ,_ARG1 ,_ARG2> {
	using TYPE = _ARG2 ;
} ;

template <BOOL _ARGC ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = typename CONDITIONAL<ARGC<_ARGC> ,_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_REFERENCE {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_REFERENCE<_ARG1 &> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_REFERENCE<_ARG1 &&> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_REFERENCE_TYPE = typename REMOVE_REFERENCE<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_CONST {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_CONST<const _ARG1> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_CONST_TYPE = typename REMOVE_CONST<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_VOLATILE {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_VOLATILE<volatile _ARG1> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_VOLATILE_TYPE = typename REMOVE_VOLATILE<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_CVR_TYPE = REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<REMOVE_REFERENCE_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_POINTER {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_POINTER<PTR<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_POINTER_TYPE = typename REMOVE_POINTER<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_ARRAY {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_ARRAY<ARR<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,LENGTH _ARGC>
struct REMOVE_ARRAY<_ARG1[_ARGC]> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_ARRAY_TYPE = typename REMOVE_ARRAY<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_TEMP {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_TEMP<TEMP<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_TEMP_TYPE = typename REMOVE_TEMP<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_FUNCATTR {
	using TYPE = _ARG1 ;
} ;

//@warn: exclude reference attribute of function
template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...)> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) const> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) volatile> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) const volatile> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

//@warn: behaviors of 'noexcept' changed in C++17
#ifdef __CSC_CXX_LATEST__
template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) const noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) volatile noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCATTR<_ARG1 (_ARGS...) const volatile noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;
#endif

template <class _ARG1>
using REMOVE_FUNCATTR_TYPE = typename REMOVE_FUNCATTR<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_MEMPTR {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
struct REMOVE_MEMPTR<MEMPTR<_ARG1 ,_ARG2>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_MEMPTR_TYPE = typename REMOVE_MEMPTR<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct MEMPTR_CLASS ;

template <class _ARG1 ,class _ARG2>
struct MEMPTR_CLASS<MEMPTR<_ARG1 ,_ARG2>> {
	using TYPE = _ARG2 ;
} ;

template <class _ARG1>
using MEMPTR_CLASS_TYPE = typename MEMPTR_CLASS<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct COUNT_OF ;

template <class _ARG1>
struct COUNT_OF<ARR<_ARG1>> {
	using TYPE = ZERO ;
} ;

template <class _ARG1 ,LENGTH _ARGC>
struct COUNT_OF<_ARG1[_ARGC]> {
	using TYPE = ARGC<_ARGC> ;
} ;

template <class _ARG1>
using COUNT_OF_TYPE = typename COUNT_OF<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct CAPACITY_OF ;

template <>
struct CAPACITY_OF<ARGVS<>> {
	using TYPE = ZERO ;
} ;

template <class... _ARGS>
struct CAPACITY_OF<ARGVS<_ARGS...>> {
	using TYPE = ARGC<(sizeof... (_ARGS))> ;
} ;

template <class _ARG1>
using CAPACITY_OF_TYPE = typename CAPACITY_OF<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct FORWARD_TRAITS {
	//@warn: should not be used
	using TYPE = const _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &> {
	using TYPE = _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &&> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using FORWARD_TRAITS_TYPE = typename FORWARD_TRAITS<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &&> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &&> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &&> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &&> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = typename CAST_TRAITS<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class>
struct INVOKE_RESULT ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_RESULT<_ARG1 (_ARGS...)> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using INVOKE_RESULT_TYPE = typename INVOKE_RESULT<REMOVE_FUNCATTR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct INVOKE_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_PARAMS<_ARG1 (_ARGS...)> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using INVOKE_PARAMS_TYPE = typename INVOKE_PARAMS<REMOVE_FUNCATTR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class ,class>
struct REBIND_INVOKE ;

template <class _ARG1 ,class... _ARGS>
struct REBIND_INVOKE<_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class _ARG2>
using REBIND_INVOKE_TYPE = typename REBIND_INVOKE<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct FUNCTION_OF ;

template <class _ARG1>
struct FUNCTION_OF<_ARG1 ,ENABLE_TYPE<(stl::is_function<REMOVE_POINTER_TYPE<_ARG1>>::value)>> {
	using TYPE = REMOVE_POINTER_TYPE<_ARG1> ;
} ;

template <class _ARG1>
struct FUNCTION_OF<_ARG1 ,ENABLE_TYPE<(_SIZEOF_ (DEF<decltype (&_ARG1::operator())>) > 0)>> {
	using TYPE = REMOVE_POINTER_TYPE<REMOVE_FUNCATTR_TYPE<REMOVE_MEMPTR_TYPE<decltype (&_ARG1::operator())>>> ;
} ;

template <class _ARG1>
using FUNCTION_OF_TYPE = typename FUNCTION_OF<REMOVE_CVR_TYPE<_ARG1> ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct RESULT_OF ;

template <class _ARG1 ,class _ARG2>
struct RESULT_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(stl::is_same<_ARG2 ,INVOKE_PARAMS_TYPE<_ARG1>>::value)>> {
	using TYPE = INVOKE_RESULT_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
using RESULT_OF_TYPE = typename RESULT_OF<FUNCTION_OF_TYPE<_ARG1> ,_ARG2 ,VOID>::TYPE ;
} ;


namespace U {
template <class ,class ,class>
struct REPEAT_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct REPEAT_PARAMS<ZERO ,_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct REPEAT_PARAMS<_ARG1 ,_ARG2 ,ARGVS<_ARGS...>> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename REPEAT_PARAMS<DECREASE<_ARG1> ,_ARG2 ,ARGVS<_ARG2 ,_ARGS...>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using REPEAT_PARAMS_TYPE = typename REPEAT_PARAMS<_ARG1 ,_ARG2 ,ARGVS<>>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct SEQUENCE_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct SEQUENCE_PARAMS<ZERO ,_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct SEQUENCE_PARAMS<_ARG1 ,_ARG2 ,ARGVS<_ARGS...>> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename SEQUENCE_PARAMS<DECREASE<_ARG1> ,INCREASE<_ARG2> ,ARGVS<_ARGS... ,_ARG2>>::TYPE ;
} ;

template <class _ARG1>
using SEQUENCE_PARAMS_TYPE = typename SEQUENCE_PARAMS<_ARG1 ,ARGC<1> ,ARGVS<>>::TYPE ;
} ;

namespace U {
template <class>
struct ARGVS_ONE ;

template <class _ARG1 ,class... _ARGS>
struct ARGVS_ONE<ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using ARGVS_ONE_TYPE = typename ARGVS_ONE<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct ARGVS_REST ;

template <class _ARG1 ,class... _ARGS>
struct ARGVS_REST<ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using ARGVS_REST_TYPE = typename ARGVS_REST<_ARG1>::TYPE ;
} ;

namespace U {
template <class ,class>
struct ARGVS_CAT ;

template <class... _ARGS1 ,class... _ARGS2>
struct ARGVS_CAT<ARGVS<_ARGS1...> ,ARGVS<_ARGS2...>> {
	using TYPE = ARGVS<_ARGS1... ,_ARGS2...> ;
} ;

template <class _ARG1 ,class _ARG2>
using ARGVS_CAT_TYPE = typename ARGVS_CAT<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class ,class ,class>
struct IS_BOUNDED_ARRAY_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_BOUNDED_ARRAY_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(std::is_same<_ARG1 ,REMOVE_ARRAY_TYPE<_ARG2>>::value)> ,ENABLE_TYPE<(_COUNTOF_ (_ARG2) > 0)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_BOUNDED_ARRAY_OF_HELP = typename IS_BOUNDED_ARRAY_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_VAR_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_VAR_XYZ<VAR32 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_VAR_XYZ<VAR64 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<(stl::is_same<_ARG1 ,VARX>::value && !stl::is_same<_ARG1 ,VAR32>::value && !stl::is_same<_ARG1 ,VAR64>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<(stl::is_same<_ARG1 ,VARY>::value && !stl::is_same<_ARG1 ,CHAR>::value && !stl::is_same<_ARG1 ,DATA>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAR_XYZ_HELP = typename IS_VAR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_VAL_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_VAL_XYZ<VAL32 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_VAL_XYZ<VAL64 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAL_XYZ<_ARG1 ,ENABLE_TYPE<(stl::is_same<_ARG1 ,VALX>::value && !stl::is_same<_ARG1 ,VAL32>::value && !stl::is_same<_ARG1 ,VAL64>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAL_XYZ_HELP = typename IS_VAL_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>::TYPE ;
} ;

namespace U {
template <class>
struct IS_BYTE_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_BYTE_XYZ<BYTE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_BYTE_XYZ<WORD> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_BYTE_XYZ<CHAR> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_BYTE_XYZ<DATA> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_BYTE_XYZ<MEGA> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_BYTE_XYZ_HELP = typename IS_BYTE_XYZ<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_STR_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_STR_XYZ<STRU8 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRU16 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRU32 ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRA ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRW ,VOID> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_STR_XYZ<_ARG1 ,ENABLE_TYPE<(stl::is_same<_ARG1 ,STRX>::value && !stl::is_same<_ARG1 ,STRA>::value && !stl::is_same<_ARG1 ,STRW>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_STR_XYZ_HELP = typename IS_STR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class ,class ,class>
struct IS_SAFE_ALIASING {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,TEMP<_ARG1> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<TEMP<_ARG1> ,_ARG1 ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<_ARG1> ,ARR<TEMP<_ARG1>> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<TEMP<_ARG1>> ,ARR<_ARG1> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<1>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>>::TYPE ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,NONE ,ENABLE_TYPE<(stl::is_class<_ARG1>::value)> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<NONE ,_ARG1 ,ENABLE_TYPE<(stl::is_class<_ARG1>::value)> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>>::TYPE ;
} ;

template <>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<BOOL> ,VOID ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(IS_VAR_XYZ_HELP<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(IS_VAL_XYZ_HELP<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(IS_BYTE_XYZ_HELP<_ARG1>::value && !IS_STR_XYZ_HELP<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(IS_STR_XYZ_HELP<_ARG1>::value && !IS_BYTE_XYZ_HELP<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

//@info: compatible for old c api
template <>
struct IS_SAFE_ALIASING<ARR<STRA> ,ARR<BYTE> ,VOID ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_SAFE_ALIASING<TEMP<_ARG1> ,TEMP<_ARG2> ,ENABLE_TYPE<((_SIZEOF_ (TEMP<_ARG2>) >= _SIZEOF_ (TEMP<_ARG1>) && _ALIGNOF_ (TEMP<_ARG2>) % _ALIGNOF_ (TEMP<_ARG1>) == 0))> ,ARGC<4>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<5>>::TYPE ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,VOID ,ENABLE_TYPE<(!stl::is_pointer<_ARG1>::value)> ,ARGC<5>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_SAFE_ALIASING_HELP = typename IS_SAFE_ALIASING<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID ,ARGC<1>>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct INDEX_OF {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEX_OF<_ARG1 ,_ARG2 ,ARGVS<_ARG2 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
struct INDEX_OF<_ARG1 ,_ARG2 ,ARGVS<_ARG3 ,_ARGS...>> {
	using TYPE = typename INDEX_OF<INCREASE<_ARG1> ,_ARG2 ,ARGVS<_ARGS...>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_OF_TYPE = typename INDEX_OF<ZERO ,_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct INDEX_TO ;

template <class _ARG1 ,class... _ARGS>
struct INDEX_TO<ZERO ,ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct INDEX_TO<_ARG1 ,ARGVS<>> {
	//@info: bad index_to
	using TYPE = NONE ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEX_TO<_ARG1 ,ARGVS<_ARG2 ,_ARGS...>> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename INDEX_TO<DECREASE<_ARG1> ,ARGVS<_ARGS...>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_TO_TYPE = typename INDEX_TO<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_COMPLETE {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_COMPLETE<_ARG1 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_COMPLETE_HELP = typename IS_COMPLETE<REMOVE_CVR_TYPE<_ARG1> ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct IS_INTERFACE {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_INTERFACE<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2) && _ALIGNOF_ (_ARG1) == _ALIGNOF_ (_ARG2))>> {
	using TYPE = ARGC<(stl::is_base_of<_ARG2 ,_ARG1>::value)> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_INTERFACE_HELP = typename IS_INTERFACE<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct IS_ALWAYS_BASE_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_ALWAYS_BASE_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0 && _SIZEOF_ (_ARG2) > 0)>> {
	using TYPE = ARGC<(stl::is_base_of<_ARG1 ,_ARG2>::value)> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_ALWAYS_BASE_OF_HELP = typename IS_ALWAYS_BASE_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID>::TYPE ;
} ;

namespace U {
template <class...>
struct IS_ALL_SAME {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_ALL_SAME<_ARG1> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_ALL_SAME<_ARG1 ,_ARG1> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct IS_ALL_SAME<_ARG1 ,_ARG1 ,_ARGS...> {
	using TYPE = typename IS_ALL_SAME<_ARG1 ,_ARGS...>::TYPE ;
} ;

template <class... _ARGS>
using IS_ALL_SAME_HELP = typename IS_ALL_SAME<_ARGS...>::TYPE ;
} ;

namespace U {
template <class...>
struct IS_ANY_SAME {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_ANY_SAME<_ARG1> {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct IS_ANY_SAME<_ARG1 ,_ARG1 ,_ARGS...> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct IS_ANY_SAME<_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = ARGC<(IS_ANY_SAME<_ARG1 ,_ARGS...>::value && IS_ANY_SAME<_ARG2 ,_ARGS...>::value)> ;
} ;

template <class... _ARGS>
using IS_ANY_SAME_HELP = typename IS_ANY_SAME<_ARGS...>::TYPE ;
} ;

namespace U {
template <class>
struct IS_TEMPLATE {
	using TYPE = ARGC<FALSE> ;
} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct IS_TEMPLATE<_ARGT<_ARGS...>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_TEMPLATE_HELP = typename IS_TEMPLATE<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct TEMPLATE_PARAMS ;

template <template <class...> class _ARGT ,class... _ARGS>
struct TEMPLATE_PARAMS<_ARGT<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using TEMPLATE_PARAMS_TYPE = typename TEMPLATE_PARAMS<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;
#pragma endregion

using U::ENABLE_TYPE ;
using U::DEPENDENT_TYPE ;
using U::CONDITIONAL_TYPE ;
using U::REMOVE_REFERENCE_TYPE ;
using U::REMOVE_CONST_TYPE ;
using U::REMOVE_VOLATILE_TYPE ;
using U::REMOVE_CVR_TYPE ;
using U::REMOVE_POINTER_TYPE ;
using U::REMOVE_ARRAY_TYPE ;
using U::REMOVE_FUNCATTR_TYPE ;
using U::REMOVE_MEMPTR_TYPE ;
using U::MEMPTR_CLASS_TYPE ;
using U::FORWARD_TRAITS_TYPE ;
using U::CAST_TRAITS_TYPE ;
using U::INVOKE_RESULT_TYPE ;
using U::INVOKE_PARAMS_TYPE ;
using U::REBIND_INVOKE_TYPE ;
using U::FUNCTION_OF_TYPE ;
using U::RESULT_OF_TYPE ;
using U::REPEAT_PARAMS_TYPE ;
using U::SEQUENCE_PARAMS_TYPE ;
using U::ARGVS_ONE_TYPE ;
using U::ARGVS_REST_TYPE ;
using U::ARGVS_CAT_TYPE ;
using U::INDEX_OF_TYPE ;
using U::INDEX_TO_TYPE ;
using U::TEMPLATE_PARAMS_TYPE ;

namespace stl {
template <class _ARG1>
using is_var_xyz = U::IS_VAR_XYZ_HELP<_ARG1> ;

template <class _ARG1>
using is_val_xyz = U::IS_VAL_XYZ_HELP<_ARG1> ;

template <class _ARG1>
using is_byte_xyz = U::IS_BYTE_XYZ_HELP<_ARG1> ;

template <class _ARG1>
using is_str_xyz = U::IS_STR_XYZ_HELP<_ARG1> ;

template <class _ARG1 ,class _ARG2>
using is_bounded_array_of = U::IS_BOUNDED_ARRAY_OF_HELP<_ARG1 ,_ARG2> ;

template <class... _ARGS>
using is_all_same = U::IS_ALL_SAME_HELP<_ARGS...> ;

template <class... _ARGS>
using is_any_same = U::IS_ANY_SAME_HELP<_ARGS...> ;
} ;

namespace U {
struct OPERATOR_DEREF {
	template <class _ARG1>
	inline constexpr _ARG1 &operator[] (const PTR<_ARG1> &address) const {
		return (*address) ;
	}
} ;

struct OPERATOR_DEPTR {
	template <class _ARG1>
	inline constexpr PTR<_ARG1> operator[] (_ARG1 &object) const {
		return (&object) ;
	}
} ;
} ;

static constexpr auto DEREF = U::OPERATOR_DEREF {} ;
static constexpr auto DEPTR = U::OPERATOR_DEPTR {} ;

template <class _ARG1>
inline constexpr _ARG1 &_NULL_ (const ARGVF<_ARG1> &) {
	return DEREF[PTR<REMOVE_REFERENCE_TYPE<_ARG1>> (NULL)] ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (const PTR<_ARG1> &address) side_effects {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" :: "rm" (address) : "memory") ;
#endif
	return LENGTH (address) ;
}

inline LENGTH _ADDRESS_ (const PTR<VOID> &address) side_effects {
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" ::: "memory") ;
#endif
	return LENGTH (address) ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (const PTR<const _ARG1> &address) side_effects {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
	return LENGTH (address) ;
}

inline constexpr INDEX _ALIGNAS_ (const INDEX &base ,const LENGTH &align_) {
	return base + (align_ - base % align_) % align_ ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _ARG1 ,class _ARG2>
inline CAST_TRAITS_TYPE<_ARG1 ,_ARG2> &_CAST_ (const ARGVF<_ARG1> & ,_ARG2 &object) {
	_STATIC_ASSERT_ (!(stl::is_pointer<_ARG1>::value && !stl::is_same<_ARG2 ,TEMP<_ARG1>>::value)) ;
	_STATIC_ASSERT_ (!(stl::is_pointer<_ARG2>::value && !stl::is_same<_ARG1 ,TEMP<_ARG2>>::value)) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG2) % _ALIGNOF_ (_ARG1) == 0) ;
	const auto r1x = _ADDRESS_ (DEPTR[object]) ;
	const auto r2x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_ARG1 ,_ARG2>>> (r1x) ;
	return DEREF[r2x] ;
}

template <class _ARG1>
inline void _ZERO_ (_ARG1 &object) {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	_CAST_ (ARGV<TEMP<_ARG1>>::null ,object) = {0} ;
}

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _COPY_ (const _ARG1 &object) {
	return object ;
}

template <class _ARG1>
inline constexpr REMOVE_REFERENCE_TYPE<_ARG1> &&_MOVE_ (_ARG1 &&object) {
	return static_cast<REMOVE_REFERENCE_TYPE<_ARG1> &&> (object) ;
}

template <class _ARG1>
inline constexpr _ARG1 &&_FORWARD_ (const ARGVF<_ARG1> & ,REMOVE_REFERENCE_TYPE<_ARG1> &object) {
	return static_cast<_ARG1 &&> (object) ;
}

template <class _ARG1>
inline constexpr _ARG1 &&_FORWARD_ (const ARGVF<_ARG1> & ,REMOVE_REFERENCE_TYPE<_ARG1> &&object) {
	_STATIC_ASSERT_ (!stl::is_lvalue_reference<_ARG1>::value) ;
	return static_cast<_ARG1 &&> (object) ;
}

template <class _ARG1>
inline constexpr _ARG1 &_XVALUE_ (const ARGVF<_ARG1> & ,REMOVE_CVR_TYPE<_ARG1> &object) {
	return object ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_XVALUE_ (const ARGVF<_ARG1> & ,const REMOVE_CVR_TYPE<_ARG1> &object) {
	return object ;
}

#ifndef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
template <class _ARG1>
inline constexpr _ARG1 &_XVALUE_ (const ARGVF<_ARG1> & ,REMOVE_CVR_TYPE<_ARG1> &&) = delete ;
#endif

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _EXCHANGE_ (_ARG1 &handle) side_effects {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	REMOVE_CVR_TYPE<_ARG1> ret = handle ;
	_ZERO_ (handle) ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _EXCHANGE_ (_ARG1 &handle ,const REMOVE_CVR_TYPE<_ARG1> &val) side_effects {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	REMOVE_CVR_TYPE<_ARG1> ret = handle ;
	handle = val ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline void _SWAP_ (_ARG1 &lhs ,_ARG1 &rhs) {
	_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<_ARG1>::value) ;
	auto tmp = _MOVE_ (lhs) ;
	lhs = _MOVE_ (rhs) ;
	rhs = _MOVE_ (tmp) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG1 _BITWISE_CAST_ (const ARGVF<_ARG1> & ,const _ARG2 &object) {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	_STATIC_ASSERT_ (stl::is_pod<_ARG2>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2)) ;
	TEMP<_ARG1> ret ;
	_ZERO_ (ret) ;
	auto &r1x = ARGV<TEMP<BYTE[_SIZEOF_ (_ARG1)]>>::null ;
	auto &r2x = ARGV<TEMP<BYTE[_SIZEOF_ (_ARG2)]>>::null ;
	_CAST_ (r1x ,ret) = _CAST_ (r2x ,object) ;
	return _MOVE_ (_CAST_ (ARGV<_ARG1>::null ,ret)) ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _ARG1 ,class _ARG2>
inline CAST_TRAITS_TYPE<_ARG1 ,_ARG2> &_LOAD_ (const ARGVF<_ARG1> & ,const PTR<_ARG2> &address) ;

//@warn: not type-safe; be careful about strict-aliasing
template <class _ARG1>
inline _ARG1 &_LOAD_UNSAFE_ (const ARGVF<_ARG1> & ,const LENGTH &address) {
	const auto r1x = _BITWISE_CAST_ (ARGV<PTR<VOID>>::null ,address) ;
	if (r1x == NULL)
		return _NULL_ (ARGV<_ARG1>::null) ;
	return _LOAD_ (ARGV<_ARG1>::null ,r1x) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (const MEMPTR<_ARG1 ,_ARG2> &mptr ,_ARG3 &mref) {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG3>>::value) ;
	const auto r1x = _ADDRESS_ (DEPTR[(_NULL_ (ARGV<_ARG2>::null).*mptr)]) ;
	const auto r2x = _ADDRESS_ (DEPTR[mref]) - r1x ;
	return _LOAD_UNSAFE_ (ARGV<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>>::null ,r2x) ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CREATE_ (const PTR<TEMP<_ARG1>> &address ,_ARGS &&...initval) {
	_STATIC_ASSERT_ (stl::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_array<_ARG1>::value) ;
	auto &r1x = _LOAD_ (ARGV<_ARG1>::null ,address) ;
	new (DEPTR[r1x]) _ARG1 (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ;
}

template <class _ARG1>
inline void _DESTROY_ (const PTR<TEMP<_ARG1>> &address) {
	_STATIC_ASSERT_ (stl::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_array<_ARG1>::value) ;
	auto &r1x = _LOAD_ (ARGV<_ARG1>::null ,address) ;
	_STATIC_UNUSED_ (r1x) ;
	r1x.~_ARG1 () ;
}

template <class _ARG1>
inline constexpr _ARG1 &_SWITCH_ (_ARG1 &expr) {
	return expr ;
}

namespace U {
template <class UNIT>
struct CONSTEXPR_ABS_SWITCH {
	imports constexpr UNIT case1 (const UNIT &val) {
		return -val ;
	}

	imports constexpr UNIT case2 (const UNIT &val) {
		return +val ;
	}
} ;
} ;

template <class _ARG1>
inline constexpr REMOVE_CVR_TYPE<_ARG1> _ABS_ (const _ARG1 &val) {
	return _SWITCH_ (
		(val < _ARG1 (0)) ? U::CONSTEXPR_ABS_SWITCH<_ARG1>::case1 :
		U::CONSTEXPR_ABS_SWITCH<_ARG1>::case2)
		(val) ;
}

template <class _ARG1>
inline constexpr _ARG1 &_MIN_ (_ARG1 &lhs ,_ARG1 &rhs) {
	return _SWITCH_ (
		!(rhs < lhs) ? lhs :
		rhs) ;
}

template <class _ARG1>
inline constexpr _ARG1 &_MAX_ (_ARG1 &lhs ,_ARG1 &rhs) {
	return _SWITCH_ (
		!(lhs < rhs) ? lhs :
		rhs) ;
}

namespace U {
template <class UNIT>
struct CONSTEXPR_EBOOL_SWITCH {
	imports constexpr UNIT case1 () {
		return UNIT (1) ;
	}

	imports constexpr UNIT case2 () {
		return UNIT (0) ;
	}
} ;
} ;

inline constexpr VAR32 _EBOOL_ (const BOOL &flag) {
	return _SWITCH_ (
		flag ? U::CONSTEXPR_EBOOL_SWITCH<VAR32>::case1 :
		U::CONSTEXPR_EBOOL_SWITCH<VAR32>::case2)
		() ;
}

namespace U {
struct OPERATOR_ONCE {
	imports BOOL invoke (const BOOL &) {
		return FALSE ;
	}

	imports BOOL invoke (BOOL &flag) side_effects {
		flag = FALSE ;
		return FALSE ;
	}
} ;
} ;

class Interface {
public:
	implicit Interface () = default ;

	implicit virtual ~Interface () = default ;

	implicit Interface (const Interface &) = delete ;
	inline Interface &operator= (const Interface &) = delete ;

	implicit Interface (Interface &&) = delete ;
	inline Interface &operator= (Interface &&) = delete ;
} ;

template <class UNIT>
class TypeInterface
	:private Interface {
	_STATIC_ASSERT_ (stl::is_same<UNIT ,REMOVE_CVR_TYPE<UNIT>>::value) ;
} ;

template <class _ARG1>
inline FLAG _TYPEMID_ (const ARGVF<_ARG1> &) {
	TypeInterface<REMOVE_CVR_TYPE<_ARG1>> ret ;
	return _MOVE_ (_CAST_ (ARGV<FLAG>::null ,ret)) ;
}

namespace stl {
template <class _ARG1>
using is_template = U::IS_TEMPLATE_HELP<_ARG1> ;

template <class _ARG1>
using is_complete = U::IS_COMPLETE_HELP<_ARG1> ;

template <class _ARG1>
using is_interface = U::IS_INTERFACE_HELP<_ARG1 ,Interface> ;

template <class _ARG1 ,class _ARG2>
using is_always_base_of = U::IS_ALWAYS_BASE_OF_HELP<_ARG1 ,_ARG2> ;
} ;

template <class UNIT>
struct TEMP {
	_STATIC_ASSERT_ (!stl::is_reference<UNIT>::value) ;
	alignas (UNIT) DEF<BYTE[_SIZEOF_ (UNIT)]> unused ;
} ;

template <class UNIT>
class Wrapped {
protected:
	UNIT mSelf ;

public:
	implicit Wrapped () = delete ;

	implicit ~Wrapped () = delete ;

	implicit Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;

	implicit Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

template <>
class Wrapped<void> {
public:
	implicit Wrapped () = delete ;

	implicit ~Wrapped () = delete ;

	implicit Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;

	implicit Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

class Proxy {
public:
	implicit Proxy () = default ;

	implicit Proxy (const Proxy &) = delete ;
	inline Proxy &operator= (const Proxy &) = delete ;

#ifdef __CSC_CXX_LATEST__
	implicit Proxy (Proxy &&) = delete ;
	inline Proxy &operator= (Proxy &&) = delete ;
#endif

#ifndef __CSC_CXX_LATEST__
	implicit Proxy (Proxy &&) = default ;
	inline Proxy &operator= (Proxy &&) = delete ;
#endif
} ;

template <class SIZE>
class ArrayRange ;

template <>
class ArrayRange<ZERO>
	:private Proxy {
private:
	struct Private {
		class Iterator ;
	} ;

private:
	INDEX mIBegin ;
	INDEX mIEnd ;

public:
	implicit ArrayRange () = delete ;

	explicit ArrayRange (const INDEX &ibegin_ ,const INDEX &iend_) {
		mIBegin = ibegin_ ;
		mIEnd = iend_ ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET begin () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		return Iterator (DEREF[this] ,mIBegin) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET end () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		const auto r1x = _MAX_ (mIBegin ,mIEnd) ;
		return Iterator (DEREF[this] ,r1x) ;
	}
} ;

class ArrayRange<ZERO>::Private::Iterator
	:private Proxy {
private:
	const ArrayRange &mBase ;
	INDEX mIndex ;

public:
	implicit Iterator () = delete ;

	explicit Iterator (const ArrayRange &base ,const INDEX &index)
		: mBase (base) ,mIndex (index) {}

	inline BOOL operator!= (const Iterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	inline const INDEX &operator* () const leftvalue {
		return mIndex ;
	}

	inline void operator++ () {
		mIndex++ ;
	}
} ;

inline ArrayRange<ZERO> _RANGE_ (const INDEX &ibegin_ ,const INDEX &iend_) {
	return ArrayRange<ZERO> (ibegin_ ,iend_) ;
}

template <class _ARG1>
inline const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> &_CACHE_ (const _ARG1 &proc) side_effects {
	_STATIC_ASSERT_ (!stl::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	_STATIC_ASSERT_ (!stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	imports const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> mInstance = proc () ;
	return mInstance ;
}

namespace U {
struct CONSTEXPR_CACHE_STRING_SIZE {
	imports constexpr LENGTH invoke (const ARGVF<ARGVS<>> &) {
		return 1 ;
	}

	template <class _ARG1>
	imports constexpr LENGTH invoke (const ARGVF<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		return _COUNTOF_ (ONE_HINT) - 1 + invoke (ARGV<REST_HINT>::null) ;
	}
} ;
} ;

namespace U {
struct OPERATOR_PTRTOARR {
	template <class _ARG1>
	inline constexpr ARR<_ARG1> &operator[] (const PTR<_ARG1> &that) const {
		return DEREF[PTR<ARR<_ARG1>> (that)] ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_bounded_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value)>>
	inline constexpr ARR<REMOVE_ARRAY_TYPE<_ARG1>> &operator[] (_ARG1 &that) const {
		return DEREF[PTR<ARR<REMOVE_ARRAY_TYPE<_ARG1>>> (DEPTR[that])] ;
	}
} ;
} ;

static constexpr auto PTRTOARR = U::OPERATOR_PTRTOARR {} ;

template <class REAL>
class Plain
	:private Proxy {
	_STATIC_ASSERT_ (stl::is_str_xyz<REAL>::value) ;

private:
	struct Private {
		template <class>
		class PlainString ;
	} ;

private:
	PTR<const REAL> mPlain ;
	LENGTH mSize ;

public:
	implicit Plain () = delete ;

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_const<_ARG1>::value && stl::is_bounded_array_of<REAL ,_ARG1>::value)>>
	constexpr implicit Plain (_ARG1 &that)
		:mPlain (DEPTR[that[0]]) ,mSize (_COUNTOF_ (_ARG1) - 1) {}

	template <class _ARG1 ,class... _ARGS>
	explicit Plain (const ARGVF<_ARG1> & ,const _ARGS &...text)
		:Plain (cache_string (ARGV<_ARG1>::null ,text...)) {
		_STATIC_WARNING_ ("noop") ;
	}

	constexpr LENGTH size () const {
		return mSize ;
	}

	constexpr const ARR<REAL> &to () const leftvalue {
		_STATIC_WARNING_ ("mark") ;
		return PTRTOARR[mPlain] ;
	}

	inline constexpr implicit operator const ARR<REAL> & () const leftvalue {
		return to () ;
	}

private:
	template <class _ARG1 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<REAL[U::CONSTEXPR_CACHE_STRING_SIZE::invoke (ARGV<ARGVS<_ARGS...>>::null)]>>
	imports const _RET &cache_string (const ARGVF<_ARG1> & ,const _ARGS &...text) {
		struct Dependent ;
		using PlainString = typename DEPENDENT_TYPE<Private ,Dependent>::template PlainString<ARGC<(U::CONSTEXPR_CACHE_STRING_SIZE::invoke (ARGV<ARGVS<_ARGS...>>::null))>> ;
		const auto r1x = PlainString (text...) ;
		auto &r2x = _CACHE_ ([&] () {
			return r1x ;
		}) ;
		return r2x.mString ;
	}
} ;

template <class REAL>
template <class SIZE>
class Plain<REAL>::Private::PlainString {
	_STATIC_ASSERT_ (SIZE::value > 0) ;

private:
	friend Plain ;
	DEF<REAL[SIZE::value]> mString ;

public:
	implicit PlainString () = delete ;

	template <class... _ARGS>
	explicit PlainString (const _ARGS &...text) {
		template_write (ARGV<ZERO>::null ,text...) ;
	}

private:
	template <class _ARG1>
	void template_write (const ARGVF<_ARG1> &) {
		_STATIC_ASSERT_ (_ARG1::value == SIZE::value - 1) ;
		mString[_ARG1::value] = 0 ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	void template_write (const ARGVF<_ARG1> & ,const _ARG2 &text_one ,const _ARGS &...text_rest) {
		_STATIC_ASSERT_ (_ARG1::value >= 0 && _ARG1::value < LENGTH (SIZE::value)) ;
		_STATIC_ASSERT_ (stl::is_bounded_array_of<STRX ,_ARG2>::value || stl::is_bounded_array_of<STRA ,_ARG2>::value || stl::is_bounded_array_of<STRW ,_ARG2>::value) ;
		for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (_ARG2) - 1))
			mString[i + _ARG1::value] = REAL (text_one[i]) ;
		auto &r1x = ARGV<ARGC<(_ARG1::value + _COUNTOF_ (_ARG2) - 1)>>::null ;
		template_write (r1x ,text_rest...) ;
	}
} ;

class Exception final {
private:
	PTR<const ARR<STR>> mWhat ;

public:
	implicit Exception () = delete ;

	explicit Exception (const Plain<STR> &what_) {
		mWhat = DEPTR[what_.self] ;
	}

	const ARR<STR> &what () const leftvalue {
		return DEREF[mWhat] ;
	}

	void raise[[noreturn]] () const {
		throw DEREF[this] ;
	}
} ;

//@warn: not type-safe; be careful about strict-aliasing
template <class _ARG1 ,class _ARG2>
inline CAST_TRAITS_TYPE<_ARG1 ,_ARG2> &_LOAD_ (const ARGVF<_ARG1> & ,const PTR<_ARG2> &address) {
	_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2>>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,NONE>::value) ,BYTE ,_ARG1>) ;
	_STATIC_UNUSED_ (r1x) ;
	const auto r2x = _ADDRESS_ (address) ;
	_DEBUG_ASSERT_ (r2x % r1x == 0) ;
	const auto r3x = _BITWISE_CAST_ (ARGV<PTR<CAST_TRAITS_TYPE<_ARG1 ,_ARG2>>>::null ,r2x) ;
	return DEREF[r3x] ;
}

template <class _ARG1>
inline RESULT_OF_TYPE<_ARG1 ,ARGVS<>> _CALL_ (_ARG1 &&proc) side_effects {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	return proc () ;
}

//@warn: check ruined object when an exception was thrown
template <class _ARG1>
inline void _CALL_TRY_ (const _ARG1 &proc) {
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	proc () ;
}

//@warn: check ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (const _ARG1 &proc_one ,const _ARGS &...proc_rest) {
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	try {
		proc_one () ;
		return ;
	} catch (const Exception &) {
		_STATIC_WARNING_ ("noop") ;
	}
	_CALL_TRY_ (proc_rest...) ;
}

template <class _ARG1 ,class _ARG2>
inline void _CATCH_ (const _ARG1 &try_proc ,const _ARG2 &catch_proc) {
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<const Exception &>> ,void>::value) ;
	try {
		try_proc () ;
		return ;
	} catch (const Exception &e) {
		catch_proc (e) ;
	} catch (const stl::exception &e) {
		catch_proc (Exception (_PCSTR_ ("stl::exception : unknown"))) ;
		_STATIC_UNUSED_ (e) ;
	} catch (...) {
		catch_proc (Exception (_PCSTR_ ("unknown C++ exception"))) ;
	}
}
} ;