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
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) CSC::Exception (CSC::Plain<CSC::STR> (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)).raise () ; } while (FALSE)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) CSC::Exception (CSC::Plain<CSC::STR> (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)).raise () ; } while (FALSE)
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
#define _UNITTEST_WATCH_(...) do { struct ARGVPL ; CSC::GlobalWatch::done (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,_PCSTR_ (_STR_ (__VA_ARGS__)) ,(_UNW_ (__VA_ARGS__))) ; } while (FALSE)
#endif
#endif

#ifndef _UNITTEST_WATCH_
#define _UNITTEST_WATCH_(...) do {} while (FALSE)
#endif

#define ANONYMOUS _CAT_ (_anonymous_ ,__LINE__)

#define _SWITCH_CASE_(arg) (arg) goto ANONYMOUS ; while (CSC::U::OPERATOR_ONCE::invoke (arg)) ANONYMOUS:

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

template <class TYPE>
using DEF = TYPE ;

template <class TYPE>
using PTR = DEF<TYPE *> ;

#ifndef __CSC_COMPILER_GNUC__
template <class TYPE>
using ARR = DEF<TYPE[]> ;
#endif

#ifdef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
template <class TYPE>
using ARR = DEF<TYPE[0]> ;
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
#define _PCSTRU8_(arg) CSC::Plain<CSC::STRU8> (_CAST_<STRU8[_COUNTOF_ (decltype (_CAT_ (u8 ,arg)))]> (_CAT_ (u8 ,arg)))
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

template <VAR _VAL1>
struct ARGC {
	enum :VAR {
		value = _VAL1
	} ;
} ;

using ZERO = ARGC<0> ;

template <class _ARG1>
using INCREASE = ARGC<_ARG1::value + 1> ;

template <class _ARG1>
using DECREASE = ARGC<_ARG1::value - 1> ;

template <class>
struct ARGV {
	_STATIC_WARNING_ ("noop") ;
} ;

template <class...>
struct ARGVS ;

template <class>
struct ARGVP ;

template <>
struct ARGV<ARGVP<ZERO>> {
	_STATIC_WARNING_ ("noop") ;
} ;

template <class _ARG1>
struct ARGV<ARGVP<_ARG1>>
	:public ARGV<ARGVP<DECREASE<_ARG1>>> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
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
static constexpr auto ARGVPX = ARGV<VOID> {} ;

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
	UNIT1 P1 ;
} ;

template <class UNIT1 ,class UNIT2>
struct PACK<UNIT1 ,UNIT2> {
	UNIT1 P1 ;
	UNIT2 P2 ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
struct PACK<UNIT1 ,UNIT2 ,UNIT3> {
	UNIT1 P1 ;
	UNIT2 P2 ;
	UNIT3 P3 ;
} ;

template <class _ARG1>
using CALL = typename _ARG1::TYPE ;

#pragma region
namespace U {
template <class>
struct ENABLE ;

template <>
struct ENABLE<ARGC<TRUE>> {
	using TYPE = VOID ;
} ;

template <BOOL _VAL1>
using ENABLE_TYPE = CALL<ENABLE<ARGC<_VAL1>>> ;
} ;

namespace U {
template <class _ARG1 ,class>
struct DEPENDENT {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
using DEPENDENT_TYPE = CALL<DEPENDENT<_ARG1 ,_ARG2>> ;
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

template <BOOL _VAL1 ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = CALL<CONDITIONAL<ARGC<_VAL1> ,_ARG1 ,_ARG2>> ;
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
using REMOVE_REFERENCE_TYPE = CALL<REMOVE_REFERENCE<_ARG1>> ;
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
using REMOVE_CONST_TYPE = CALL<REMOVE_CONST<_ARG1>> ;
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
using REMOVE_VOLATILE_TYPE = CALL<REMOVE_VOLATILE<_ARG1>> ;
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
using REMOVE_POINTER_TYPE = CALL<REMOVE_POINTER<REMOVE_CVR_TYPE<_ARG1>>> ;
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

template <class _ARG1 ,LENGTH _VAL1>
struct REMOVE_ARRAY<_ARG1[_VAL1]> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_ARRAY_TYPE = CALL<REMOVE_ARRAY<_ARG1>> ;
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
using REMOVE_TEMP_TYPE = CALL<REMOVE_TEMP<REMOVE_CVR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class>
struct REMOVE_FUNCATTR ;

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
using REMOVE_FUNCATTR_TYPE = CALL<REMOVE_FUNCATTR<_ARG1>> ;
} ;

namespace U {
template <class>
struct REMOVE_MEMPTR ;

template <class _ARG1 ,class _ARG2>
struct REMOVE_MEMPTR<_ARG1 _ARG2::*> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_MEMPTR_TYPE = CALL<REMOVE_MEMPTR<REMOVE_CVR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class>
struct MEMPTR_CLASS ;

template <class _ARG1 ,class _ARG2>
struct MEMPTR_CLASS<_ARG1 _ARG2::*> {
	using TYPE = _ARG2 ;
} ;

template <class _ARG1>
using MEMPTR_CLASS_TYPE = CALL<MEMPTR_CLASS<REMOVE_CVR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class>
struct COUNT_OF ;

template <class _ARG1>
struct COUNT_OF<ARR<_ARG1>> {
	using TYPE = ZERO ;
} ;

template <class _ARG1 ,LENGTH _VAL1>
struct COUNT_OF<_ARG1[_VAL1]> {
	using TYPE = ARGC<_VAL1> ;
} ;

template <class _ARG1>
using COUNT_OF_TYPE = CALL<COUNT_OF<REMOVE_CVR_TYPE<_ARG1>>> ;
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
	using TYPE = ARGC<sizeof... (_ARGS)> ;
} ;

template <class _ARG1>
using CAPACITY_OF_TYPE = CALL<CAPACITY_OF<REMOVE_CVR_TYPE<_ARG1>>> ;
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
using FORWARD_TRAITS_TYPE = CALL<FORWARD_TRAITS<_ARG1>> ;
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
using CAST_TRAITS_TYPE = CALL<CAST_TRAITS<_ARG1 ,_ARG2>> ;
} ;

namespace U {
template <class>
struct INVOKE_RESULT ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_RESULT<_ARG1 (_ARGS...)> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using INVOKE_RESULT_TYPE = CALL<INVOKE_RESULT<REMOVE_FUNCATTR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class>
struct INVOKE_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_PARAMS<_ARG1 (_ARGS...)> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using INVOKE_PARAMS_TYPE = CALL<INVOKE_PARAMS<REMOVE_FUNCATTR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class ,class ,class>
struct RESULT_OF ;

template <class _ARG1 ,class _ARG2>
struct RESULT_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<!stl::is_class<REMOVE_POINTER_TYPE<_ARG1>>::value>> {
	using TYPE = INVOKE_RESULT_TYPE<REMOVE_POINTER_TYPE<_ARG1>> ;
} ;

template <class _ARG1 ,class _ARG2>
struct RESULT_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (decltype (&_ARG1::operator())) > 0)>> {
	using TYPE = CALL<RESULT_OF<REMOVE_MEMPTR_TYPE<decltype (&_ARG1::operator())> ,_ARG2 ,VOID>> ;
} ;

template <class _ARG1 ,class _ARG2>
using RESULT_OF_TYPE = CALL<RESULT_OF<REMOVE_CVR_TYPE<_ARG1> ,_ARG2 ,VOID>> ;
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
	using TYPE = CALL<REPEAT_PARAMS<DECREASE<_ARG1> ,_ARG2 ,ARGVS<_ARG2 ,_ARGS...>>> ;
} ;

template <class _ARG1 ,class _ARG2>
using REPEAT_PARAMS_TYPE = CALL<REPEAT_PARAMS<_ARG1 ,_ARG2 ,ARGVS<>>> ;
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
	using TYPE = CALL<SEQUENCE_PARAMS<DECREASE<_ARG1> ,INCREASE<_ARG2> ,ARGVS<_ARGS... ,_ARG2>>> ;
} ;

template <class _ARG1>
using SEQUENCE_PARAMS_TYPE = CALL<SEQUENCE_PARAMS<_ARG1 ,ARGC<1> ,ARGVS<>>> ;
} ;

namespace U {
template <class>
struct ARGVS_ONE ;

template <class _ARG1 ,class... _ARGS>
struct ARGVS_ONE<ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using ARGVS_ONE_TYPE = CALL<ARGVS_ONE<_ARG1>> ;
} ;

namespace U {
template <class>
struct ARGVS_REST ;

template <class _ARG1 ,class... _ARGS>
struct ARGVS_REST<ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using ARGVS_REST_TYPE = CALL<ARGVS_REST<_ARG1>> ;
} ;

namespace U {
template <class ,class>
struct ARGVS_CAT ;

template <class... _ARGS1 ,class... _ARGS2>
struct ARGVS_CAT<ARGVS<_ARGS1...> ,ARGVS<_ARGS2...>> {
	using TYPE = ARGVS<_ARGS1... ,_ARGS2...> ;
} ;

template <class _ARG1 ,class _ARG2>
using ARGVS_CAT_TYPE = CALL<ARGVS_CAT<_ARG1 ,_ARG2>> ;
} ;

namespace U {
template <class ,class ,class>
struct IS_BOUNDED_ARRAY_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_BOUNDED_ARRAY_OF<_ARG1 ,ARR<_ARG1> ,VOID> {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,LENGTH _VAL1>
struct IS_BOUNDED_ARRAY_OF<_ARG1 ,DEF<_ARG1[_VAL1]> ,ENABLE_TYPE<(_VAL1 > 0)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_BOUNDED_ARRAY_OF_HELP = CALL<IS_BOUNDED_ARRAY_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID>> ;
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
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VARX>::value && !stl::is_same<_ARG1 ,VAR32>::value && !stl::is_same<_ARG1 ,VAR64>::value>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VARY>::value && !stl::is_same<_ARG1 ,CHAR>::value && !stl::is_same<_ARG1 ,DATA>::value>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAR_XYZ_HELP = CALL<IS_VAR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>> ;
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
struct IS_VAL_XYZ<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VALX>::value && !stl::is_same<_ARG1 ,VAL32>::value && !stl::is_same<_ARG1 ,VAL64>::value>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAL_XYZ_HELP = CALL<IS_VAL_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>> ;
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
using IS_BYTE_XYZ_HELP = CALL<IS_BYTE_XYZ<REMOVE_CVR_TYPE<_ARG1>>> ;
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
struct IS_STR_XYZ<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,STRX>::value && !stl::is_same<_ARG1 ,STRA>::value && !stl::is_same<_ARG1 ,STRW>::value>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_STR_XYZ_HELP = CALL<IS_STR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,VOID>> ;
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
	using TYPE = CALL<IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>>> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,NONE ,ENABLE_TYPE<stl::is_class<_ARG1>::value> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<NONE ,_ARG1 ,ENABLE_TYPE<stl::is_class<_ARG1>::value> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>> {
	using TYPE = CALL<IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>>> ;
} ;

template <>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<BOOL> ,VOID ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<IS_VAR_XYZ_HELP<_ARG1>::value> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<IS_VAL_XYZ_HELP<_ARG1>::value> ,ARGC<3>> {
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
	using TYPE = CALL<IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>>> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_SAFE_ALIASING<TEMP<_ARG1> ,TEMP<_ARG2> ,ENABLE_TYPE<(_SIZEOF_ (TEMP<_ARG2>) >= _SIZEOF_ (TEMP<_ARG1>) && _ALIGNOF_ (TEMP<_ARG2>) % _ALIGNOF_ (TEMP<_ARG1>) == 0)> ,ARGC<4>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>> {
	using TYPE = CALL<IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<5>>> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,VOID ,ENABLE_TYPE<!stl::is_pointer<_ARG1>::value> ,ARGC<5>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_SAFE_ALIASING_HELP = CALL<IS_SAFE_ALIASING<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID ,ARGC<1>>> ;
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
	using TYPE = CALL<INDEX_OF<INCREASE<_ARG1> ,_ARG2 ,ARGVS<_ARGS...>>> ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_OF_TYPE = CALL<INDEX_OF<ZERO ,_ARG1 ,_ARG2>> ;
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
	using TYPE = CALL<INDEX_TO<DECREASE<_ARG1> ,ARGVS<_ARGS...>>> ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_TO_TYPE = CALL<INDEX_TO<_ARG1 ,_ARG2>> ;
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
using IS_COMPLETE_HELP = CALL<IS_COMPLETE<REMOVE_CVR_TYPE<_ARG1> ,VOID>> ;
} ;

namespace U {
template <class ,class ,class>
struct IS_INTERFACE {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_INTERFACE<_ARG1 ,_ARG2 ,ENABLE_TYPE<_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2) && _ALIGNOF_ (_ARG1) == _ALIGNOF_ (_ARG2)>> {
	using TYPE = ARGC<stl::is_base_of<_ARG2 ,_ARG1>::value> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_INTERFACE_HELP = CALL<IS_INTERFACE<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID>> ;
} ;

namespace U {
template <class ,class ,class>
struct IS_ALWAYS_BASE_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_ALWAYS_BASE_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0 && _SIZEOF_ (_ARG2) > 0)>> {
	using TYPE = ARGC<stl::is_base_of<_ARG1 ,_ARG2>::value> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_ALWAYS_BASE_OF_HELP = CALL<IS_ALWAYS_BASE_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID>> ;
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
	using TYPE = CALL<IS_ALL_SAME<_ARG1 ,_ARGS...>> ;
} ;

template <class... _ARGS>
using IS_ALL_SAME_HELP = CALL<IS_ALL_SAME<_ARGS...>> ;
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
	using TYPE = ARGC<IS_ANY_SAME<_ARG1 ,_ARGS...>::value && IS_ANY_SAME<_ARG2 ,_ARGS...>::value> ;
} ;

template <class... _ARGS>
using IS_ANY_SAME_HELP = CALL<IS_ANY_SAME<_ARGS...>> ;
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
using IS_TEMPLATE_HELP = CALL<IS_TEMPLATE<REMOVE_CVR_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class>
struct TEMPLATE_PARAMS ;

template <template <class...> class _ARGT ,class... _ARGS>
struct TEMPLATE_PARAMS<_ARGT<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using TEMPLATE_PARAMS_TYPE = CALL<TEMPLATE_PARAMS<REMOVE_CVR_TYPE<_ARG1>>> ;
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

template <class _RET>
inline constexpr _RET &_NULL_ () {
	return DEREF[PTR<REMOVE_REFERENCE_TYPE<_RET>> (NULL)] ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (const PTR<_ARG1> &address) popping {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" :: "rm" (address) : "memory") ;
#endif
	return LENGTH (address) ;
}

inline LENGTH _ADDRESS_ (const PTR<VOID> &address) popping {
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" ::: "memory") ;
#endif
	return LENGTH (address) ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (const PTR<const _ARG1> &address) popping {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
	return LENGTH (address) ;
}

inline constexpr INDEX _ALIGNAS_ (const INDEX &base ,const LENGTH &align_) {
	return base + (align_ - base % align_) % align_ ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_CAST_ (_ARG1 &object) {
	_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (!(stl::is_pointer<_RET>::value && !stl::is_same<_ARG1 ,TEMP<_RET>>::value)) ;
	_STATIC_ASSERT_ (!(stl::is_pointer<_ARG1>::value && !stl::is_same<_RET ,TEMP<_ARG1>>::value)) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG1)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG1) % _ALIGNOF_ (_RET) == 0) ;
	const auto r1x = _ADDRESS_ (DEPTR[object]) ;
	const auto r2x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (r1x) ;
	return DEREF[r2x] ;
}

template <class _ARG1>
inline void _ZERO_ (_ARG1 &object) {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	_CAST_<TEMP<_ARG1>> (object) = {0} ;
}

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _COPY_ (const _ARG1 &object) {
	return object ;
}

template <class _ARG1>
inline constexpr REMOVE_REFERENCE_TYPE<_ARG1> &&_MOVE_ (_ARG1 &&object) {
	return static_cast<REMOVE_REFERENCE_TYPE<_ARG1> &&> (object) ;
}

template <class _RET>
inline constexpr _RET &&_FORWARD_ (REMOVE_REFERENCE_TYPE<_RET> &object) {
	return static_cast<_RET &&> (object) ;
}

template <class _RET>
inline constexpr _RET &&_FORWARD_ (REMOVE_REFERENCE_TYPE<_RET> &&object) {
	_STATIC_ASSERT_ (!stl::is_lvalue_reference<_RET>::value) ;
	return static_cast<_RET &&> (object) ;
}

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _EXCHANGE_ (_ARG1 &handle) popping {
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	REMOVE_CVR_TYPE<_ARG1> ret = handle ;
	_ZERO_ (handle) ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline REMOVE_CVR_TYPE<_ARG1> _EXCHANGE_ (_ARG1 &handle ,const REMOVE_CVR_TYPE<_ARG1> &val) popping {
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

template <class _RET ,class _ARG1>
inline _RET _BITWISE_CAST_ (const _ARG1 &object) {
	_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (stl::is_pod<_RET>::value) ;
	_STATIC_ASSERT_ (stl::is_pod<_ARG1>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG1)) ;
	TEMP<_RET> ret ;
	_ZERO_ (ret) ;
	_CAST_<TEMP<BYTE[_SIZEOF_ (_RET)]>> (ret) = _CAST_<TEMP<BYTE[_SIZEOF_ (_ARG1)]>> (object) ;
	return _MOVE_ (_CAST_<_RET> (ret)) ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_LOAD_ (const PTR<_ARG1> &address) ;

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET>
inline _RET &_LOAD_UNSAFE_ (const LENGTH &address) {
	const auto r1x = _BITWISE_CAST_<PTR<VOID>> (address) ;
	if (r1x == NULL)
		return _NULL_<_RET> () ;
	return _LOAD_<_RET> (r1x) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (const DEF<_ARG1 _ARG2::*> &mptr ,_ARG3 &mref) {
	_STATIC_ASSERT_ (stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG3>>::value) ;
	auto &r1x = (_NULL_<_ARG2> ().*mptr) ;
	const auto r2x = _ADDRESS_ (DEPTR[mref]) - _ADDRESS_ (DEPTR[r1x]) ;
	return _LOAD_UNSAFE_<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>> (r2x) ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CREATE_ (const PTR<TEMP<_ARG1>> &address ,_ARGS &&...initval) {
	_STATIC_ASSERT_ (stl::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_array<_ARG1>::value) ;
	auto &r1x = _LOAD_<_ARG1> (address) ;
	new (DEPTR[r1x]) _ARG1 (_FORWARD_<_ARGS> (initval)...) ;
}

template <class _ARG1>
inline void _DESTROY_ (const PTR<TEMP<_ARG1>> &address) {
	_STATIC_ASSERT_ (stl::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_array<_ARG1>::value) ;
	auto &r1x = _LOAD_<_ARG1> (address) ;
	r1x.~_ARG1 () ;
	_STATIC_UNUSED_ (r1x) ;
}

template <class _ARG1>
inline constexpr _ARG1 &_SWITCH_ (_ARG1 &expr) {
	return expr ;
}

//@error: fuck g++4.8
namespace U {
template <class UNIT>
struct CONSTEXPR_SWITCH_ABS {
	inline static constexpr UNIT case1 (const UNIT &val) {
		return -val ;
	}

	inline static constexpr UNIT case2 (const UNIT &val) {
		return +val ;
	}
} ;
} ;

template <class _ARG1>
inline constexpr REMOVE_CVR_TYPE<_ARG1> _ABS_ (const _ARG1 &val) {
	return _SWITCH_ (
		(val < _ARG1 (0)) ? U::CONSTEXPR_SWITCH_ABS<_ARG1>::case1 :
		U::CONSTEXPR_SWITCH_ABS<_ARG1>::case2)
		(val) ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MIN_ (const _ARG1 &lhs ,const _ARG1 &rhs) {
	return _SWITCH_ (
		!(rhs < lhs) ? lhs :
		rhs) ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MAX_ (const _ARG1 &lhs ,const _ARG1 &rhs) {
	return _SWITCH_ (
		!(lhs < rhs) ? lhs :
		rhs) ;
}

//@error: fuck g++4.8
namespace U {
template <class UNIT>
struct CONSTEXPR_SWITCH_EBOOL {
	inline static constexpr UNIT case1 () {
		return UNIT (1) ;
	}

	inline static constexpr UNIT case2 () {
		return UNIT (0) ;
	}
} ;
} ;

inline constexpr VAR32 _EBOOL_ (const BOOL &flag) {
	return _SWITCH_ (
		flag ? U::CONSTEXPR_SWITCH_EBOOL<VAR32>::case1 :
		U::CONSTEXPR_SWITCH_EBOOL<VAR32>::case2)
		() ;
}

namespace U {
struct OPERATOR_ONCE {
	inline static BOOL invoke (const BOOL &) {
		return FALSE ;
	}

	inline static BOOL invoke (BOOL &flag) popping {
		flag = FALSE ;
		return FALSE ;
	}
} ;
} ;

class Interface {
public:
	inline Interface () = default ;
	inline virtual ~Interface () = default ;
	inline Interface (const Interface &) = delete ;
	inline Interface &operator= (const Interface &) = delete ;
	inline Interface (Interface &&) = delete ;
	inline Interface &operator= (Interface &&) = delete ;
} ;

template <class UNIT>
class TypeInterface final
	:private Interface {
	_STATIC_ASSERT_ (stl::is_same<UNIT ,REMOVE_CVR_TYPE<UNIT>>::value) ;
} ;

template <class _RET>
inline FLAG _TYPEMID_ () {
	_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
	TypeInterface<REMOVE_CVR_TYPE<_RET>> ret ;
	return _MOVE_ (_CAST_<FLAG> (ret)) ;
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
	inline Wrapped () = delete ;
	inline ~Wrapped () = delete ;
	inline Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;
	inline Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

template <>
class Wrapped<void> {
public:
	inline Wrapped () = delete ;
	inline ~Wrapped () = delete ;
	inline Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;
	inline Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

class Proxy {
public:
	inline Proxy () = default ;

	inline Proxy (const Proxy &) = delete ;
	inline Proxy &operator= (const Proxy &) = delete ;

#ifdef __CSC_CXX_LATEST__
	inline Proxy (Proxy &&) = delete ;
#endif

#ifndef __CSC_CXX_LATEST__
	inline Proxy (Proxy &&) = default ;
#endif

	inline Proxy &operator= (Proxy &&) = delete ;
} ;

template <class SIZE>
class ArrayRange ;

template <>
class ArrayRange<ZERO> final
	:private Proxy {
private:
	struct Detail ;
	INDEX mIBegin ;
	INDEX mIEnd ;

public:
	inline ArrayRange () = delete ;

	inline explicit ArrayRange (const INDEX &ibegin_ ,const INDEX &iend_) {
		mIBegin = ibegin_ ;
		mIEnd = iend_ ;
	}

	template <class _RET = NONE>
	inline DEF<typename DEPENDENT_TYPE<Detail ,_RET>::Iterator> begin () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Detail ,Dependent>::Iterator ;
		return Iterator (DEREF[this] ,mIBegin) ;
	}

	template <class _RET = NONE>
	inline DEF<typename DEPENDENT_TYPE<Detail ,_RET>::Iterator> end () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Detail ,Dependent>::Iterator ;
		const auto r1x = _MAX_ (mIBegin ,mIEnd) ;
		return Iterator (DEREF[this] ,r1x) ;
	}
} ;

struct ArrayRange<ZERO>::Detail {
	class Iterator final
		:private Proxy {
	private:
		friend ArrayRange ;
		const ArrayRange &mBase ;
		INDEX mIndex ;

	public:
		inline Iterator () = delete ;

		inline BOOL operator!= (const Iterator &that) const {
			return BOOL (mIndex != that.mIndex) ;
		}

		inline const INDEX &operator* () const leftvalue {
			return mIndex ;
		}

		inline void operator++ () {
			mIndex++ ;
		}

	private:
		inline explicit Iterator (const ArrayRange &base ,const INDEX &index)
			: mBase (base) ,mIndex (index) {}
	} ;
} ;

inline ArrayRange<ZERO> _RANGE_ (const INDEX &ibegin_ ,const INDEX &iend_) {
	return ArrayRange<ZERO> (ibegin_ ,iend_) ;
}

template <class _ARG1>
inline const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> &_CACHE_ (_ARG1 &&func) popping {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	static const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> mInstance = func () ;
	return mInstance ;
}

namespace U {
inline constexpr LENGTH constexpr_cache_string_size (const ARGV<ARGVS<>> &) {
	return 1 ;
}

template <class _ARG1>
inline constexpr LENGTH constexpr_cache_string_size (const ARGV<_ARG1> &) {
	using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
	using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
	return _COUNTOF_ (ONE_HINT) - 1 + constexpr_cache_string_size (_NULL_<ARGV<REST_HINT>> ()) ;
}
} ;

namespace U {
struct OPERATOR_PTRTOARR {
	template <class _ARG1>
	inline constexpr ARR<_ARG1> &operator[] (const PTR<_ARG1> &that) const {
		return DEREF[PTR<ARR<_ARG1>> (that)] ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_bounded_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value>>
	inline constexpr ARR<REMOVE_ARRAY_TYPE<_ARG1>> &operator[] (_ARG1 &that) const {
		return DEREF[PTR<ARR<REMOVE_ARRAY_TYPE<_ARG1>>> (DEPTR[that])] ;
	}
} ;
} ;

static constexpr auto PTRTOARR = U::OPERATOR_PTRTOARR {} ;

template <class REAL>
class Plain final
	:private Proxy {
	_STATIC_ASSERT_ (stl::is_str_xyz<REAL>::value) ;

private:
	struct Detail ;
	PTR<const REAL> mPlain ;
	LENGTH mSize ;

public:
	inline Plain () = delete ;

	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_const<_ARG1>::value && stl::is_bounded_array_of<REAL ,_ARG1>::value>>
	inline constexpr implicit Plain (_ARG1 &that)
		:mPlain (DEPTR[that[0]]) ,mSize (_COUNTOF_ (_ARG1) - 1) {}

	template <class _ARG1 ,class... _ARGS>
	inline explicit Plain (const ARGV<_ARG1> & ,const _ARGS &...text)
		:Plain (cache_string (_NULL_<ARGV<_ARG1>> () ,text...)) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline constexpr LENGTH size () const {
		return mSize ;
	}

	inline constexpr const ARR<REAL> &to () const leftvalue {
		_STATIC_WARNING_ ("mark") ;
		return PTRTOARR[mPlain] ;
	}

	inline constexpr implicit operator const ARR<REAL> & () const leftvalue {
		return to () ;
	}

private:
	template <class _ARG1 ,class... _ARGS>
	inline static auto cache_string (const ARGV<_ARG1> & ,const _ARGS &...text)
		->DEPENDENT_TYPE<DEF<const DEF<REAL[U::constexpr_cache_string_size (_NULL_<ARGV<ARGVS<_ARGS...>>> ())]> &> ,Plain> {
		using PlainString = typename Detail::template PlainString<ARGC<U::constexpr_cache_string_size (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>> ;
		const auto r1x = PlainString (text...) ;
		auto &r2x = _CACHE_ ([&] () {
			return r1x ;
		}) ;
		return r2x.mString ;
	}
} ;

template <class REAL>
struct Plain<REAL>::Detail {
	template <class SIZE>
	class PlainString {
		_STATIC_ASSERT_ (SIZE::value > 0) ;

	private:
		friend Plain ;
		DEF<REAL[SIZE::value]> mString ;

	public:
		inline PlainString () = delete ;

		template <class... _ARGS>
		inline explicit PlainString (const _ARGS &...text) {
			template_write (_NULL_<ARGV<ZERO>> () ,text...) ;
		}

	private:
		template <class _ARG1>
		inline void template_write (const ARGV<_ARG1> &) {
			_STATIC_ASSERT_ (_ARG1::value == SIZE::value - 1) ;
			mString[_ARG1::value] = 0 ;
		}

		template <class _ARG1 ,class _ARG2 ,class... _ARGS>
		inline void template_write (const ARGV<_ARG1> & ,const _ARG2 &text_one ,const _ARGS &...text_rest) {
			_STATIC_ASSERT_ (_ARG1::value >= 0 && _ARG1::value < LENGTH (SIZE::value)) ;
			_STATIC_ASSERT_ (stl::is_bounded_array_of<STRX ,_ARG2>::value || stl::is_bounded_array_of<STRA ,_ARG2>::value || stl::is_bounded_array_of<STRW ,_ARG2>::value) ;
			for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (_ARG2) - 1))
				mString[i + _ARG1::value] = REAL (text_one[i]) ;
			auto &r1x = _NULL_<ARGV<ARGC<_ARG1::value + _COUNTOF_ (_ARG2) - 1> >> () ;
			template_write (r1x ,text_rest...) ;
		}
	} ;
} ;

class Exception final {
private:
	PTR<const ARR<STR>> mWhat ;

public:
	inline Exception () = delete ;

	inline explicit Exception (const Plain<STR> &what_) {
		mWhat = DEPTR[what_.self] ;
	}

	inline const ARR<STR> &what () const leftvalue {
		return DEREF[mWhat] ;
	}

	inline void raise[[noreturn]] () const {
		throw DEREF[this] ;
	}
} ;

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_LOAD_ (const PTR<_ARG1> &address) {
	_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<REMOVE_CVR_TYPE<_RET> ,REMOVE_CVR_TYPE<_ARG1>>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(stl::is_same<REMOVE_CVR_TYPE<_RET> ,NONE>::value) ,BYTE ,_RET>) ;
	const auto r2x = _ADDRESS_ (address) ;
	_DEBUG_ASSERT_ (r2x % r1x == 0) ;
	_STATIC_UNUSED_ (r1x) ;
	const auto r3x = _BITWISE_CAST_<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (r2x) ;
	return DEREF[r3x] ;
}

template <class _ARG1>
inline RESULT_OF_TYPE<_ARG1 ,ARGVS<>> _CALL_ (_ARG1 &&func) popping {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!stl::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	return func () ;
}

//@warn: check ruined object when an exception was thrown
template <class _ARG1>
inline void _CALL_TRY_ (_ARG1 &&proc) {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	proc () ;
}

//@warn: check ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&proc_one ,_ARGS &&...proc_rest) {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	try {
		proc_one () ;
		return ;
	} catch (const Exception &) {
		_STATIC_WARNING_ ("noop") ;
	}
	_CALL_TRY_ (_FORWARD_<_ARGS> (proc_rest)...) ;
}

template <class _ARG1 ,class _ARG2>
inline void _CATCH_ (_ARG1 &&try_proc ,_ARG2 &&catch_proc) {
	_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	_STATIC_ASSERT_ (!stl::is_reference<_ARG2>::value) ;
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