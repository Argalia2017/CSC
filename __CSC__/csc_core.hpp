#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
namespace api {
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

using std::is_class ;
using std::is_base_of ;
using std::is_pod ;

#ifndef __CSC_COMPILER_GNUC__
using std::is_trivial ;
#endif

using std::is_default_constructible ;
using std::is_constructible ;
using std::is_nothrow_destructible ;
using std::is_copy_constructible ;
using std::is_copy_assignable ;
using std::is_nothrow_move_constructible ;
using std::is_nothrow_move_assignable ;
using std::is_convertible ;

using std::atomic ;
} ;

using BOOL = bool ;

using VAR32 = api::int32_t ;
using VAR64 = api::int64_t ;

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

using VAL32 = float ;
using VAL64 = double ;

static constexpr auto VAL32_MAX = VAL32 (3.402823466E+38) ;
static constexpr auto VAL32_MIN = VAL32 (1.175494351E-38) ;
static constexpr auto VAL32_EPS = VAL32 (1.192092896E-07) ;
static constexpr auto VAL32_INF = api::numeric_limits<VAL32>::infinity () ;
static constexpr auto VAL32_NAN = api::numeric_limits<VAL32>::quiet_NaN () ;
static constexpr auto VAL64_MAX = VAL64 (1.7976931348623158E+308) ;
static constexpr auto VAL64_MIN = VAL64 (2.2250738585072014E-308) ;
static constexpr auto VAL64_EPS = VAL64 (2.2204460492503131E-016) ;
static constexpr auto VAL64_INF = api::numeric_limits<VAL64>::infinity () ;
static constexpr auto VAL64_NAN = api::numeric_limits<VAL64>::quiet_NaN () ;

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

template <class UNIT>
using DEF = UNIT ;

template <class UNIT>
using PTR = DEF<UNIT *> ;

struct MEMCLASS ;

template <class UNIT1 ,class UNIT2 = MEMCLASS>
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

using BYTE = api::uint8_t ;
using WORD = api::uint16_t ;
using CHAR = api::uint32_t ;
using DATA = api::uint64_t ;

class __uint128_t {
private:
	alignas (16) DEF<BYTE[16]> unused ;
} ;

using MEGA = __uint128_t ;

using STRU8 = unsigned char ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;
using STRA = char ;
using STRW = wchar_t ;

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;
#endif

#ifdef __CSC_CONFIG_STRW__
using STR = STRW ;
#endif

using STRX = signed char ;

template <VAR _ARG1>
struct ARGC {
	enum :VAR {
		value = _ARG1
	} ;
} ;

using ZERO = ARGC<0> ;

template <class _ARG1>
using INCREASE = ARGC<(_ARG1::value + 1)> ;

template <class _ARG1>
using DECREASE = ARGC<(_ARG1::value - 1)> ;

template <class UNIT>
struct ARGV {
	static DEF<void (const ARGV &)> null ;
} ;

template <class UNIT>
inline void ARGV<UNIT>::null (const ARGV &) {}

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

class Interface ;

#pragma region
namespace U {
template <class>
struct ENABLE ;

template <>
struct ENABLE<ARGC<TRUE>> {
	using TYPE = NONE ;
} ;

template <BOOL _ARG1>
using ENABLE_TYPE = typename ENABLE<ARGC<_ARG1>>::TYPE ;
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

template <BOOL _ARG1 ,class _ARG2 ,class _ARG3>
using CONDITIONAL_TYPE = typename CONDITIONAL<ARGC<_ARG1> ,_ARG2 ,_ARG3>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_SAME {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_SAME<_ARG1 ,_ARG1> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_SAME_HELP = typename IS_SAME<_ARG1 ,_ARG2>::TYPE ;
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
using IS_LVALUE_REFERENCE_HELP = IS_SAME_HELP<_ARG1 ,REMOVE_REFERENCE_TYPE<_ARG1> &> ;
} ;

namespace U {
template <class _ARG1>
using IS_RVALUE_REFERENCE_HELP = IS_SAME_HELP<_ARG1 ,REMOVE_REFERENCE_TYPE<_ARG1> &&> ;
} ;

namespace U {
template <class>
struct IS_REFERENCE {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_REFERENCE<_ARG1 &> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_REFERENCE<_ARG1 &&> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_REFERENCE_HELP = typename IS_REFERENCE<_ARG1>::TYPE ;
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
using IS_CONST_HELP = IS_SAME_HELP<_ARG1 ,const REMOVE_CONST_TYPE<_ARG1>> ;
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
using IS_VOLATILE_HELP = IS_SAME_HELP<_ARG1 ,volatile REMOVE_VOLATILE_TYPE<_ARG1>> ;
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
using IS_POINTER_HELP = IS_SAME_HELP<_ARG1 ,PTR<REMOVE_POINTER_TYPE<_ARG1>>> ;
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

template <class _ARG1 ,LENGTH _ARG2>
struct REMOVE_ARRAY<_ARG1[_ARG2]> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_ARRAY_TYPE = typename REMOVE_ARRAY<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct IS_ARRAY {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_ARRAY<ARR<_ARG1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,LENGTH _ARG2>
struct IS_ARRAY<_ARG1[_ARG2]> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_ARRAY_HELP = typename IS_ARRAY<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct COUNT_OF ;

template <class _ARG1>
struct COUNT_OF<ARR<_ARG1>> {
	using TYPE = ZERO ;
} ;

template <class _ARG1 ,LENGTH _ARG2>
struct COUNT_OF<_ARG1[_ARG2]> {
	using TYPE = ARGC<_ARG2> ;
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
template <class ,class ,class ,class>
struct IS_ARRAY_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_ARRAY_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,REMOVE_ARRAY_TYPE<_ARG2>>::value)> ,ENABLE_TYPE<(_COUNTOF_ (_ARG2) > 0)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_ARRAY_OF_HELP = typename IS_ARRAY_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,NONE ,NONE>::TYPE ;
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
struct IS_MEMPTR {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_MEMPTR<MEMPTR<_ARG1 ,_ARG2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_MEMPTR_HELP = typename IS_MEMPTR<_ARG1>::TYPE ;
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
struct REMOVE_FUNCTION {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_FUNCTION<_ARG1 (_ARGS...)> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_FUNCTION_TYPE = typename REMOVE_FUNCTION<REMOVE_FUNCATTR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct FUNCTION_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct FUNCTION_PARAMS<_ARG1 (_ARGS...)> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using FUNCTION_PARAMS_TYPE = typename FUNCTION_PARAMS<REMOVE_FUNCATTR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class ,class>
struct FUNCTION_BIND ;

template <class _ARG1 ,class... _ARGS>
struct FUNCTION_BIND<_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class _ARG2>
using FUNCTION_BIND_TYPE = typename FUNCTION_BIND<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class>
struct IS_FUNCTION {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct IS_FUNCTION<_ARG1 (_ARGS...)> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_FUNCTION_HELP = typename IS_FUNCTION<REMOVE_FUNCATTR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class ,class>
struct FUNCTION_OF ;

template <class _ARG1>
struct FUNCTION_OF<_ARG1 ,ENABLE_TYPE<(IS_FUNCTION_HELP<REMOVE_POINTER_TYPE<_ARG1>>::value)>> {
	using TYPE = REMOVE_FUNCATTR_TYPE<REMOVE_POINTER_TYPE<_ARG1>> ;
} ;

template <class _ARG1>
struct FUNCTION_OF<_ARG1 ,ENABLE_TYPE<(_SIZEOF_ (DEF<decltype (&_ARG1::operator())>) > 0)>> {
	using TYPE = REMOVE_FUNCATTR_TYPE<REMOVE_MEMPTR_TYPE<decltype (&_ARG1::operator())>> ;
} ;

template <class _ARG1>
using FUNCTION_OF_TYPE = typename FUNCTION_OF<REMOVE_CVR_TYPE<_ARG1> ,NONE>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct RESULT_OF ;

template <class _ARG1 ,class _ARG2>
struct RESULT_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG2 ,FUNCTION_PARAMS_TYPE<_ARG1>>::value)>> {
	using TYPE = REMOVE_FUNCTION_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
using RESULT_OF_TYPE = typename RESULT_OF<FUNCTION_OF_TYPE<_ARG1> ,_ARG2 ,NONE>::TYPE ;
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
struct RANGE_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct RANGE_PARAMS<ZERO ,_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct RANGE_PARAMS<_ARG1 ,_ARG2 ,ARGVS<_ARGS...>> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename RANGE_PARAMS<DECREASE<_ARG1> ,INCREASE<_ARG2> ,ARGVS<_ARGS... ,_ARG2>>::TYPE ;
} ;

template <class _ARG1>
using RANGE_PARAMS_TYPE = typename RANGE_PARAMS<_ARG1 ,ARGC<1> ,ARGVS<>>::TYPE ;
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
template <class _ARG1>
using IS_VOID_HELP = IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,NONE> ;
} ;

namespace U {
template <class ,class>
struct IS_VAR_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_VAR_XYZ<VAR32 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_VAR_XYZ<VAR64 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,VARX>::value && !IS_SAME_HELP<_ARG1 ,VAR32>::value && !IS_SAME_HELP<_ARG1 ,VAR64>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAR_XYZ<_ARG1 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,VARY>::value && !IS_SAME_HELP<_ARG1 ,CHAR>::value && !IS_SAME_HELP<_ARG1 ,DATA>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAR_XYZ_HELP = typename IS_VAR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,NONE>::TYPE ;
} ;

namespace U {
template <class ,class>
struct IS_VAL_XYZ {
	using TYPE = ARGC<FALSE> ;
} ;

template <>
struct IS_VAL_XYZ<VAL32 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_VAL_XYZ<VAL64 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_VAL_XYZ<_ARG1 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,VALX>::value && !IS_SAME_HELP<_ARG1 ,VAL32>::value && !IS_SAME_HELP<_ARG1 ,VAL64>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_VAL_XYZ_HELP = typename IS_VAL_XYZ<REMOVE_CVR_TYPE<_ARG1> ,NONE>::TYPE ;
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
struct IS_STR_XYZ<STRU8 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRU16 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRU32 ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRA ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct IS_STR_XYZ<STRW ,NONE> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_STR_XYZ<_ARG1 ,ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,STRX>::value && !IS_SAME_HELP<_ARG1 ,STRA>::value && !IS_SAME_HELP<_ARG1 ,STRW>::value)>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
using IS_STR_XYZ_HELP = typename IS_STR_XYZ<REMOVE_CVR_TYPE<_ARG1> ,NONE>::TYPE ;
} ;

namespace U {
template <class _ARG1>
using IS_BOOL_HELP = IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,BOOL> ;

template <class _ARG1>
using IS_EFLAG_HELP = IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,EFLAG> ;

template <class _ARG1>
using IS_XYZ_HELP = ARGC<IS_VOID_HELP<_ARG1>::value || IS_BOOL_HELP<_ARG1>::value || IS_EFLAG_HELP<_ARG1>::value || IS_VAR_XYZ_HELP<_ARG1>::value || IS_VAL_XYZ_HELP<_ARG1>::value || IS_BYTE_XYZ_HELP<_ARG1>::value || IS_STR_XYZ_HELP<_ARG1>::value> ;
} ;


namespace U {
template <class _ARG1>
struct FORWARD_TRAITS {
	//@warn: should not be used
	using TYPE = DEF<const _ARG1 &> ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &> {
	using TYPE = DEF<_ARG1 &> ;
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
	using TYPE = DEF<const REMOVE_REFERENCE_TYPE<_ARG1>> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2> {
	using TYPE = DEF<volatile REMOVE_REFERENCE_TYPE<_ARG1>> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2> {
	using TYPE = DEF<const volatile REMOVE_REFERENCE_TYPE<_ARG1>> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &> {
	using TYPE = DEF<REMOVE_REFERENCE_TYPE<_ARG1> &> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &> {
	using TYPE = DEF<const REMOVE_REFERENCE_TYPE<_ARG1> &> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &> {
	using TYPE = DEF<volatile REMOVE_REFERENCE_TYPE<_ARG1> &> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &> {
	using TYPE = DEF<const volatile REMOVE_REFERENCE_TYPE<_ARG1> &> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &&> {
	using TYPE = DEF<REMOVE_REFERENCE_TYPE<_ARG1> &&> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &&> {
	using TYPE = DEF<const REMOVE_REFERENCE_TYPE<_ARG1> &&> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &&> {
	using TYPE = DEF<volatile REMOVE_REFERENCE_TYPE<_ARG1> &&> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &&> {
	using TYPE = DEF<const volatile REMOVE_REFERENCE_TYPE<_ARG1> &&> ;
} ;

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = typename CAST_TRAITS<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class ,class ,class>
struct IS_SAFE_ALIASING {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,TEMP<_ARG1> ,NONE ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<TEMP<_ARG1> ,_ARG1 ,NONE ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<_ARG1> ,ARR<TEMP<_ARG1>> ,NONE ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<TEMP<_ARG1>> ,ARR<_ARG1> ,NONE ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<1>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_SAFE_ALIASING<TEMP<_ARG1> ,TEMP<_ARG2> ,ENABLE_TYPE<((_SIZEOF_ (TEMP<_ARG2>) >= _SIZEOF_ (TEMP<_ARG1>) && _ALIGNOF_ (TEMP<_ARG2>) % _ALIGNOF_ (TEMP<_ARG1>) == 0))> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>>::TYPE ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(IS_BOOL_HELP<_ARG1>::value)> ,ARGC<3>> {
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

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>>::TYPE ;
} ;

//@info: compatible for old c api
template <>
struct IS_SAFE_ALIASING<ARR<STRA> ,ARR<BYTE> ,NONE ,ARGC<4>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>> {
	using TYPE = typename IS_SAFE_ALIASING<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<5>>::TYPE ;
} ;

template <class _ARG1>
struct IS_SAFE_ALIASING<_ARG1 ,NONE ,NONE ,ARGC<5>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_SAFE_ALIASING_HELP = typename IS_SAFE_ALIASING<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,NONE ,ARGC<1>>::TYPE ;
} ;

namespace U {
template <class _ARG1>
using IS_CLASS_HELP = ARGC<(api::is_class<_ARG1>::value)> ;
} ;

namespace U {
#ifndef __CSC_CXX_LATEST__
template <class _ARG1>
using IS_TRIVIAL_HELP = ARGC<(api::is_pod<_ARG1>::value)> ;
#endif

#ifdef __CSC_CXX_LATEST__
template <class _ARG1>
using IS_TRIVIAL_HELP = ARGC<(api::is_trivial<_ARG1>::value)> ;
#endif
} ;

namespace U {
template <class _ARG1>
using IS_DEFAULT_CONSTRUCTIBLE_HELP = ARGC<(api::is_default_constructible<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1 ,class... _ARGS>
using IS_CONSTRUCTIBLE_HELP = ARGC<(api::is_constructible<_ARG1 ,_ARGS...>::value)> ;
} ;

namespace U {
template <class _ARG1>
using IS_DESTRUCTIBLE_HELP = ARGC<(api::is_nothrow_destructible<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1>
using IS_COPY_CONSTRUCTIBLE_HELP = ARGC<(api::is_copy_constructible<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1>
using IS_COPY_ASSIGNABLE_HELP = ARGC<(api::is_copy_assignable<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1>
using IS_MOVE_CONSTRUCTIBLE_HELP = ARGC<(api::is_nothrow_move_constructible<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1>
using IS_MOVE_ASSIGNABLE_HELP = ARGC<(api::is_nothrow_move_assignable<_ARG1>::value)> ;
} ;

namespace U {
template <class _ARG1 ,class _ARG2>
using IS_IMPLICIT_CONVERTIBLE_HELP = ARGC<(api::is_convertible<_ARG1 ,_ARG2>::value)> ;
} ;

namespace U {
template <class _ARG1 ,class _ARG2>
using IS_EXPLICIT_CONVERTIBLE_HELP = ARGC<(api::is_constructible<_ARG2 ,_ARG1>::value && !api::is_convertible<_ARG1 ,_ARG2>::value)> ;
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
using IS_COMPLETE_HELP = typename IS_COMPLETE<REMOVE_CVR_TYPE<_ARG1> ,NONE>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct IS_BASE_OF {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_BASE_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(IS_COMPLETE_HELP<_ARG1>::value && IS_COMPLETE_HELP<_ARG2>::value)>> {
	using TYPE = ARGC<(api::is_base_of<_ARG1 ,_ARG2>::value)> ;
} ;

template <class _ARG1 ,class _ARG2>
using IS_BASE_OF_HELP = typename IS_BASE_OF<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,NONE>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct IS_INTERFACE {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct IS_INTERFACE<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2) && _ALIGNOF_ (_ARG1) == _ALIGNOF_ (_ARG2))>> {
	using TYPE = ARGC<(IS_BASE_OF_HELP<_ARG2 ,_ARG1>::value)> ;
} ;

template <class _ARG1>
using IS_INTERFACE_HELP = typename IS_INTERFACE<REMOVE_CVR_TYPE<_ARG1> ,Interface ,NONE>::TYPE ;
} ;

namespace U {
template <class ,class>
struct BYTE_BASE ;

template <>
struct BYTE_BASE<ARGC<_SIZEOF_ (BYTE)> ,ARGC<_ALIGNOF_ (BYTE)>> {
	using TYPE = BYTE ;
} ;

template <>
struct BYTE_BASE<ARGC<_SIZEOF_ (WORD)> ,ARGC<_ALIGNOF_ (WORD)>> {
	using TYPE = WORD ;
} ;

template <>
struct BYTE_BASE<ARGC<_SIZEOF_ (CHAR)> ,ARGC<_ALIGNOF_ (CHAR)>> {
	using TYPE = CHAR ;
} ;

template <>
struct BYTE_BASE<ARGC<_SIZEOF_ (DATA)> ,ARGC<_ALIGNOF_ (DATA)>> {
	using TYPE = DATA ;
} ;

template <class _ARG1>
using BYTE_BASE_TYPE = typename BYTE_BASE<ARGC<_SIZEOF_ (_ARG1)> ,ARGC<_ALIGNOF_ (_ARG1)>>::TYPE ;
} ;

namespace U {
template <class ,class>
struct TEXT_BASE ;

template <>
struct TEXT_BASE<ARGC<_SIZEOF_ (STRU8)> ,ARGC<_ALIGNOF_ (STRU8)>> {
	using TYPE = STRU8 ;
} ;

template <>
struct TEXT_BASE<ARGC<_SIZEOF_ (STRU16)> ,ARGC<_ALIGNOF_ (STRU16)>> {
	using TYPE = STRU16 ;
} ;

template <>
struct TEXT_BASE<ARGC<_SIZEOF_ (STRU32)> ,ARGC<_ALIGNOF_ (STRU32)>> {
	using TYPE = STRU32 ;
} ;

template <class _ARG1>
using TEXT_BASE_TYPE = typename TEXT_BASE<ARGC<_SIZEOF_ (_ARG1)> ,ARGC<_ALIGNOF_ (_ARG1)>>::TYPE ;
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
#pragma endregion

using U::ENABLE_TYPE ;
using U::DEPENDENT_TYPE ;
using U::CONDITIONAL_TYPE ;
using U::IS_SAME_HELP ;
using U::REMOVE_REFERENCE_TYPE ;
using U::IS_LVALUE_REFERENCE_HELP ;
using U::IS_RVALUE_REFERENCE_HELP ;
using U::IS_REFERENCE_HELP ;
using U::REMOVE_CONST_TYPE ;
using U::IS_CONST_HELP ;
using U::REMOVE_VOLATILE_TYPE ;
using U::IS_VOLATILE_HELP ;
using U::REMOVE_CVR_TYPE ;
using U::REMOVE_POINTER_TYPE ;
using U::IS_POINTER_HELP ;
using U::REMOVE_ARRAY_TYPE ;
using U::IS_ARRAY_HELP ;
using U::COUNT_OF_TYPE ;
using U::CAPACITY_OF_TYPE ;
using U::IS_ARRAY_OF_HELP ;
using U::REMOVE_MEMPTR_TYPE ;
using U::MEMPTR_CLASS_TYPE ;
using U::IS_MEMPTR_HELP ;
using U::REMOVE_TEMP_TYPE ;
using U::REMOVE_FUNCATTR_TYPE ;
using U::REMOVE_FUNCTION_TYPE ;
using U::FUNCTION_PARAMS_TYPE ;
using U::FUNCTION_BIND_TYPE ;
using U::IS_FUNCTION_HELP ;
using U::FUNCTION_OF_TYPE ;
using U::RESULT_OF_TYPE ;
using U::REPEAT_PARAMS_TYPE ;
using U::RANGE_PARAMS_TYPE ;
using U::ARGVS_ONE_TYPE ;
using U::ARGVS_REST_TYPE ;
using U::ARGVS_CAT_TYPE ;
using U::INDEX_OF_TYPE ;
using U::INDEX_TO_TYPE ;
using U::IS_VOID_HELP ;
using U::IS_VAR_XYZ_HELP ;
using U::IS_VAL_XYZ_HELP ;
using U::IS_BYTE_XYZ_HELP ;
using U::IS_STR_XYZ_HELP ;
using U::IS_XYZ_HELP ;
using U::FORWARD_TRAITS_TYPE ;
using U::CAST_TRAITS_TYPE ;
using U::IS_SAFE_ALIASING_HELP ;
using U::IS_CLASS_HELP ;
using U::IS_TRIVIAL_HELP ;
using U::IS_DEFAULT_CONSTRUCTIBLE_HELP ;
using U::IS_CONSTRUCTIBLE_HELP ;
using U::IS_DESTRUCTIBLE_HELP ;
using U::IS_COPY_CONSTRUCTIBLE_HELP ;
using U::IS_COPY_ASSIGNABLE_HELP ;
using U::IS_MOVE_CONSTRUCTIBLE_HELP ;
using U::IS_MOVE_ASSIGNABLE_HELP ;
using U::IS_IMPLICIT_CONVERTIBLE_HELP ;
using U::IS_EXPLICIT_CONVERTIBLE_HELP ;
using U::IS_TEMPLATE_HELP ;
using U::TEMPLATE_PARAMS_TYPE ;
using U::IS_COMPLETE_HELP ;
using U::IS_BASE_OF_HELP ;
using U::IS_INTERFACE_HELP ;
using U::BYTE_BASE_TYPE ;
using U::TEXT_BASE_TYPE ;
using U::IS_ALL_SAME_HELP ;
using U::IS_ANY_SAME_HELP ;

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
inline LENGTH _ADDRESS_ (const PTR<_ARG1> &address) {
	_STATIC_ASSERT_ (IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" ::: "memory") ;
#endif
	return LENGTH (address) ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (const PTR<const _ARG1> &address) {
	_STATIC_ASSERT_ (IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
	return LENGTH (address) ;
}

inline constexpr INDEX _ALIGNAS_ (const INDEX &base ,const LENGTH &align_) {
	return base + (align_ - base % align_) % align_ ;
}

//@warn: not type-safe ,be careful about strict-aliasing
template <class _ARG1 ,class _ARG2>
inline CAST_TRAITS_TYPE<_ARG1 ,_ARG2> &_CAST_ (const ARGVF<_ARG1> & ,_ARG2 &object) {
	_STATIC_ASSERT_ (!(IS_POINTER_HELP<_ARG1>::value && !IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG2> ,TEMP<_ARG1>>::value)) ;
	_STATIC_ASSERT_ (!(IS_POINTER_HELP<_ARG2>::value && !IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,TEMP<_ARG2>>::value)) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG2) % _ALIGNOF_ (_ARG1) == 0) ;
	const auto r1x = _ADDRESS_ (DEPTR[object]) ;
	const auto r2x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_ARG1 ,_ARG2>>> (r1x) ;
	return DEREF[r2x] ;
}

template <class _ARG1>
inline void _ZERO_ (_ARG1 &object) {
	_STATIC_ASSERT_ (IS_TRIVIAL_HELP<_ARG1>::value) ;
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
inline constexpr _ARG1 &_FORWARD_ (const ARGVF<_ARG1> & ,REMOVE_CVR_TYPE<_ARG1> &object) {
	return static_cast<_ARG1 &> (object) ;
}

template <class _ARG1>
inline constexpr _ARG1 &&_FORWARD_ (const ARGVF<_ARG1 &&> & ,REMOVE_CVR_TYPE<_ARG1> &object) {
	return static_cast<_ARG1 &&> (object) ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_FORWARD_ (const ARGVF<_ARG1> & ,const REMOVE_CVR_TYPE<_ARG1> &object) {
	return static_cast<const _ARG1 &> (object) ;
}

template <class _ARG1>
inline constexpr _ARG1 &&_FORWARD_ (const ARGVF<_ARG1> & ,REMOVE_CVR_TYPE<_ARG1> &&object) {
	_STATIC_ASSERT_ (!IS_LVALUE_REFERENCE_HELP<_ARG1>::value) ;
	return static_cast<_ARG1 &&> (object) ;
}

template <class _ARG1>
inline void _SWAP_ (_ARG1 &lhs ,_ARG1 &rhs) {
	_STATIC_ASSERT_ (api::is_nothrow_move_constructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (api::is_nothrow_move_assignable<_ARG1>::value) ;
	auto rax = _MOVE_ (lhs) ;
	lhs = _MOVE_ (rhs) ;
	rhs = _MOVE_ (rax) ;
}

template <class _ARG1 ,class _ARG2>
inline REMOVE_CVR_TYPE<_ARG1> _BITWISE_CAST_ (const ARGVF<_ARG1> & ,const _ARG2 &object) {
	using HINT_T1 = TEMP<BYTE[_SIZEOF_ (_ARG1)]> ;
	_STATIC_ASSERT_ (!IS_POINTER_HELP<_ARG1>::value) ;
	_STATIC_ASSERT_ (!IS_POINTER_HELP<_ARG2>::value) ;
	_STATIC_ASSERT_ (IS_TRIVIAL_HELP<_ARG1>::value) ;
	_STATIC_ASSERT_ (IS_TRIVIAL_HELP<_ARG2>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2)) ;
	TEMP<_ARG1> ret ;
	_ZERO_ (ret) ;
	_CAST_ (ARGV<HINT_T1>::null ,ret) = _CAST_ (ARGV<HINT_T1>::null ,object) ;
	return _MOVE_ (_CAST_ (ARGV<_ARG1>::null ,ret)) ;
}

//@warn: not type-safe ,be careful about strict-aliasing
template <class _ARG1 ,class _ARG2>
inline PTR<CAST_TRAITS_TYPE<_ARG1 ,_ARG2>> _POINTER_CAST_ (const ARGVF<_ARG1> & ,const PTR<_ARG2> &pointer) {
	using HINT_T1 = PTR<CAST_TRAITS_TYPE<_ARG1 ,_ARG2>> ;
	_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<_ARG1 ,_ARG2>::value) ;
	if (pointer == NULL)
		return NULL ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,NONE>::value) ,BYTE ,_ARG1>) ;
	const auto r2x = _ADDRESS_ (pointer) ;
	if (r2x % r1x != 0)
		return NULL ;
	const auto r3x = _CAST_ (ARGV<TEMP<HINT_T1>>::null ,r2x) ;
	return _CAST_ (ARGV<HINT_T1>::null ,r3x) ;
}

template <class _ARG1>
inline PTR<_ARG1> _UNSAFE_POINTER_CAST_ (const ARGVF<_ARG1> & ,const LENGTH &address) {
	const auto r1x = DEPTR[_NULL_ (ARGV<BYTE>::null)] + address ;
	const auto r2x = _FORWARD_ (ARGV<PTR<NONE>>::null ,r1x) ;
	return _POINTER_CAST_ (ARGV<_ARG1>::null ,r2x) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (const MEMPTR<_ARG1 ,_ARG2> &mptr ,_ARG3 &mref) {
	_STATIC_ASSERT_ (IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG3>>::value) ;
	const auto r1x = DEPTR[(_NULL_ (ARGV<_ARG2>::null).*mptr)] ;
	const auto r2x = _ADDRESS_ (DEPTR[mref]) - _ADDRESS_ (r1x) ;
	const auto r3x = _UNSAFE_POINTER_CAST_ (ARGV<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>>::null ,r2x) ;
	return DEREF[r3x] ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CREATE_ (const PTR<TEMP<_ARG1>> &address ,_ARGS &&...initval) {
	_STATIC_ASSERT_ (IS_DESTRUCTIBLE_HELP<_ARG1>::value) ;
	_STATIC_ASSERT_ (!IS_ARRAY_HELP<_ARG1>::value) ;
	const auto r1x = _POINTER_CAST_ (ARGV<_ARG1>::null ,address) ;
	if (r1x == NULL)
		return ;
	_ZERO_ (DEREF[address]) ;
	new (r1x) _ARG1 (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
}

template <class _ARG1>
inline void _DESTROY_ (const PTR<TEMP<_ARG1>> &address) {
	_STATIC_ASSERT_ (IS_DESTRUCTIBLE_HELP<_ARG1>::value) ;
	_STATIC_ASSERT_ (!IS_ARRAY_HELP<_ARG1>::value) ;
	const auto r1x = _POINTER_CAST_ (ARGV<_ARG1>::null ,address) ;
	if (r1x == NULL)
		return ;
	DEREF[r1x].~_ARG1 () ;
}

template <class _ARG1>
inline constexpr _ARG1 &_SWITCH_ (_ARG1 &expr) {
	return expr ;
}

namespace U {
template <class UNIT>
struct CONSTEXPR_ABS_SWITCH_CASE1 {
	imports constexpr UNIT invoke (const UNIT &val) {
		return -val ;
	}
} ;

template <class UNIT>
struct CONSTEXPR_ABS_SWITCH_CASE2 {
	imports constexpr UNIT invoke (const UNIT &val) {
		return +val ;
	}
} ;
} ;

template <class _ARG1>
inline constexpr REMOVE_CVR_TYPE<_ARG1> _ABS_ (const _ARG1 &val) {
	return _SWITCH_ (
		(val < _ARG1 (0)) ? U::CONSTEXPR_ABS_SWITCH_CASE1<_ARG1>::invoke :
		U::CONSTEXPR_ABS_SWITCH_CASE2<_ARG1>::invoke)
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
struct CONSTEXPR_EBOOL_SWITCH_CASE1 {
	imports constexpr UNIT invoke () {
		return UNIT (1) ;
	}
} ;

template <class UNIT>
struct CONSTEXPR_EBOOL_SWITCH_CASE2 {
	imports constexpr UNIT invoke () {
		return UNIT (0) ;
	}
} ;
} ;

inline constexpr INDEX _EBOOL_ (const BOOL &flag) {
	return _SWITCH_ (
		flag ? U::CONSTEXPR_EBOOL_SWITCH_CASE1<INDEX>::invoke :
		U::CONSTEXPR_EBOOL_SWITCH_CASE2<INDEX>::invoke)
		() ;
}

namespace U {
struct OPERATOR_FOR_ONCE {
	inline BOOL operator() (const BOOL &) const {
		return FALSE ;
	}

	inline BOOL operator() (BOOL &flag) const {
		flag = FALSE ;
		return FALSE ;
	}
} ;
} ;

static constexpr auto FOR_ONCE = U::OPERATOR_FOR_ONCE {} ;

class Interface {
public:
	implicit Interface () = default ;

	implicit virtual ~Interface () = default ;

	implicit Interface (const Interface &) = delete ;

	inline Interface &operator= (const Interface &) = delete ;

	implicit Interface (Interface &&) = delete ;

	inline Interface &operator= (Interface &&) = delete ;
} ;

struct TYPEABI {
	LENGTH mAlign ;
	LENGTH mSize ;
	CSC::BOOL mF1 ;
	CSC::BOOL mF2 ;
	CSC::BOOL mF3 ;
	CSC::BOOL mF4 ;
	CSC::BOOL mF5 ;
	CSC::BOOL mF6 ;
	CSC::BOOL mF7 ;
	CSC::BOOL mF8 ;
	CSC::BOOL mF9 ;
} ;

template <class _ARG1>
inline TYPEABI _TYPEABI_ (const ARGVF<_ARG1> &) {
	TYPEABI ret ;
	_ZERO_ (ret) ;
	ret.mAlign = _ALIGNOF_ (_ARG1) ;
	ret.mSize = _SIZEOF_ (_ARG1) ;
	ret.mF1 = IS_POINTER_HELP<_ARG1>::value ;
	ret.mF2 = IS_ARRAY_HELP<_ARG1>::value ;
	ret.mF3 = IS_MEMPTR_HELP<_ARG1>::value ;
	ret.mF4 = IS_FUNCTION_HELP<_ARG1>::value ;
	ret.mF5 = IS_CLASS_HELP<_ARG1>::value ;
	ret.mF6 = IS_TEMPLATE_HELP<_ARG1>::value ;
	ret.mF7 = IS_INTERFACE_HELP<_ARG1>::value ;
	ret.mF8 = IS_TRIVIAL_HELP<_ARG1>::value ;
	ret.mF9 = IS_XYZ_HELP<_ARG1>::value ;
	return _MOVE_ (ret) ;
}

template <class UNIT>
class TypeInterface
	:private Interface {
	_STATIC_ASSERT_ (IS_SAME_HELP<REMOVE_CVR_TYPE<UNIT> ,UNIT>::value) ;
} ;

template <class _ARG1>
inline FLAG _TYPEMID_ (const ARGVF<_ARG1> &) {
	TypeInterface<REMOVE_CVR_TYPE<_ARG1>> ret ;
	return _MOVE_ (_CAST_ (ARGV<FLAG>::null ,ret)) ;
}

template <class UNIT>
struct TEMP {
	_STATIC_ASSERT_ (!IS_REFERENCE_HELP<UNIT>::value) ;
	alignas (UNIT) DEF<BYTE[_SIZEOF_ (UNIT)]> unused ;
} ;

template <class UNIT = NONE>
class Wrapped ;

template <>
class Wrapped<NONE> {
public:
	implicit Wrapped () = delete ;

	implicit ~Wrapped () = delete ;

	implicit Wrapped (const Wrapped &) = delete ;

	inline Wrapped &operator= (const Wrapped &) = delete ;

	implicit Wrapped (Wrapped &&) = delete ;

	inline Wrapped &operator= (Wrapped &&) = delete ;
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

class Proxy {
public:
	implicit Proxy () = default ;

	implicit Proxy (const Proxy &) = delete ;

	inline Proxy &operator= (const Proxy &) = delete ;

#ifndef __CSC_CXX_LATEST__
	implicit Proxy (Proxy &&) = default ;

	inline Proxy &operator= (Proxy &&) = delete ;
#endif

#ifdef __CSC_CXX_LATEST__
	implicit Proxy (Proxy &&) = delete ;

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
		mIEnd = _MAX_ (ibegin_ ,iend_) ;
	}

	LENGTH size () const {
		return mIEnd - mIBegin ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET begin () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		return Iterator (mIBegin) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET end () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		return Iterator (mIEnd) ;
	}
} ;

class ArrayRange<ZERO>::Private::Iterator
	:private Proxy {
private:
	INDEX mIndex ;

public:
	implicit Iterator () = delete ;

	explicit Iterator (const INDEX &index) {
		mIndex = index ;
	}

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

template <class ,class>
class Array ;

template <class _ARG1>
inline ArrayRange<_ARG1> _RANGE_ (const Array<LENGTH ,_ARG1> &range_) {
	struct Dependent ;
	using ArrayRange = DEPENDENT_TYPE<ArrayRange<_ARG1> ,Dependent> ;
	return ArrayRange (range_) ;
}

template <class _ARG1>
inline const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> &_CACHE_ (const _ARG1 &proc) {
	_STATIC_ASSERT_ (!IS_REFERENCE_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	_STATIC_ASSERT_ (!IS_VOID_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	static const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> mInstance = proc () ;
	return mInstance ;
}

namespace U {
struct CONSTEXPR_CACHE_STRING_SIZE {
	imports constexpr LENGTH invoke (const ARGVF<ARGVS<>> &) {
		return 1 ;
	}

	template <class _ARG1>
	imports constexpr LENGTH invoke (const ARGVF<_ARG1> &) {
		using HINT_T1 = ARGVS_ONE_TYPE<_ARG1> ;
		using HINT_T2 = ARGVS_REST_TYPE<_ARG1> ;
		return _COUNTOF_ (HINT_T1) - 1 + invoke (ARGV<HINT_T2>::null) ;
	}
} ;
} ;

namespace U {
struct OPERATOR_PTRTOARR {
	template <class _ARG1>
	inline constexpr ARR<_ARG1> &operator[] (const PTR<_ARG1> &that) const {
		return DEREF[PTR<ARR<_ARG1>> (that)] ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_ARRAY_OF_HELP<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value)>>
	inline constexpr ARR<REMOVE_ARRAY_TYPE<_ARG1>> &operator[] (_ARG1 &that) const {
		return DEREF[PTR<ARR<REMOVE_ARRAY_TYPE<_ARG1>>> (DEPTR[that])] ;
	}
} ;
} ;

static constexpr auto PTRTOARR = U::OPERATOR_PTRTOARR {} ;

template <class REAL>
class Plain
	:private Proxy {
	_STATIC_ASSERT_ (IS_STR_XYZ_HELP<REAL>::value) ;

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

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_CONST_HELP<_ARG1>::value && IS_ARRAY_OF_HELP<REAL ,_ARG1>::value)>>
	implicit Plain (_ARG1 &that)
		:mPlain (DEPTR[that[0]]) ,mSize (_COUNTOF_ (_ARG1) - 1) {}

	template <class _ARG1 ,class... _ARGS>
	explicit Plain (const ARGVF<_ARG1> & ,const _ARGS &...text)
		:Plain (cache_string (ARGV<_ARG1>::null ,text...)) {
		_STATIC_WARNING_ ("noop") ;
	}

	LENGTH size () const {
		return mSize ;
	}

	const ARR<REAL> &to () const leftvalue {
		_STATIC_WARNING_ ("mark") ;
		return PTRTOARR[mPlain] ;
	}

	inline implicit operator const ARR<REAL> & () const leftvalue {
		return self ;
	}

private:
	template <class _ARG1 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<REAL[U::CONSTEXPR_CACHE_STRING_SIZE::invoke (ARGV<ARGVS<_ARGS...>>::null)]>>
	imports const _RET &cache_string (const ARGVF<_ARG1> & ,const _ARGS &...text) {
		struct Dependent ;
		using HINT_T1 = ARGC<(U::CONSTEXPR_CACHE_STRING_SIZE::invoke (ARGV<ARGVS<_ARGS...>>::null))> ;
		using PlainString = typename DEPENDENT_TYPE<Private ,Dependent>::template PlainString<HINT_T1> ;
		const auto r1x = PlainString (text...) ;
		return _CACHE_ ([&] () {
			return r1x ;
		}) ;
	}
} ;

template <class REAL>
template <class SIZE>
class Plain<REAL>::Private::PlainString {
	_STATIC_ASSERT_ (SIZE::value > 0) ;

private:
	DEF<REAL[SIZE::value]> mString ;

public:
	implicit PlainString () = delete ;

	template <class... _ARGS>
	explicit PlainString (const _ARGS &...text) {
		template_write (ARGV<ZERO>::null ,text...) ;
	}

	const DEF<REAL[SIZE::value]> &to () const leftvalue {
		return mString ;
	}

	inline implicit operator const DEF<REAL[SIZE::value]> & () const leftvalue {
		return self ;
	}

private:
	template <class _ARG1>
	void template_write (const ARGVF<_ARG1> &) {
		_STATIC_ASSERT_ (_ARG1::value == SIZE::value - 1) ;
		mString[_ARG1::value] = 0 ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	void template_write (const ARGVF<_ARG1> & ,const _ARG2 &text_one ,const _ARGS &...text_rest) {
		using HINT_T1 = ARGC<(_ARG1::value + _COUNTOF_ (_ARG2) - 1)> ;
		_STATIC_ASSERT_ (_ARG1::value >= 0 && _ARG1::value < LENGTH (SIZE::value)) ;
		_STATIC_ASSERT_ (IS_ARRAY_OF_HELP<STRX ,_ARG2>::value || IS_ARRAY_OF_HELP<STRA ,_ARG2>::value || IS_ARRAY_OF_HELP<STRW ,_ARG2>::value) ;
		for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (_ARG2) - 1))
			mString[i + _ARG1::value] = REAL (text_one[i]) ;
		template_write (ARGV<HINT_T1>::null ,text_rest...) ;
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

template <class _ARG1>
inline RESULT_OF_TYPE<_ARG1 ,ARGVS<>> _CALL_ (const _ARG1 &proc) {
	_STATIC_ASSERT_ (!IS_REFERENCE_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	return proc () ;
}

template <class _ARG1>
inline void _CALL_TRY_ (_ARG1 &&proc) {
	_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	proc () ;
}

//@warn: check ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&proc_one ,_ARGS &&...proc_rest) {
	_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	try {
		proc_one () ;
		return ;
	} catch (const Exception &e) {
		_STATIC_UNUSED_ (e) ;
	} catch (...) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
	_CALL_TRY_ (proc_rest...) ;
}
} ;