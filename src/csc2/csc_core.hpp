#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
using BOOL = bool ;

static constexpr auto TRUE = BOOL (true) ;
static constexpr auto FALSE = BOOL (false) ;

using VAR32 = std::int32_t ;
using VAR64 = std::int64_t ;

static constexpr auto VAR32_MAX = VAR32 (2147483647) ;
static constexpr auto VAR32_MIN = -VAR32_MAX - 1 ;
static constexpr auto VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr auto VAR64_MIN = -VAR64_MAX - 1 ;

namespace U {
template <class...>
trait VAR_HELP ;

#ifdef __CSC_CONFIG_VAR32__
template <>
trait VAR_HELP<> {
	using VAR = VAR32 ;

	static constexpr auto VAR_MAX = VAR32_MAX ;
	static constexpr auto VAR_MIN = VAR32_MIN ;
} ;
#endif

#ifdef __CSC_CONFIG_VAR64__
template <>
trait VAR_HELP<> {
	using VAR = VAR64 ;

	static constexpr auto VAR_MAX = VAR64_MAX ;
	static constexpr auto VAR_MIN = VAR64_MIN ;
} ;
#endif
} ;

using VAR = typename U::VAR_HELP<>::VAR ;

static constexpr auto VAR_MAX = U::VAR_HELP<>::VAR_MAX ;
static constexpr auto VAR_MIN = U::VAR_HELP<>::VAR_MIN ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

static constexpr auto USED = VAR (-2) ;
static constexpr auto NONE = VAR (-1) ;
static constexpr auto ZERO = VAR (+0) ;
static constexpr auto IDEN = VAR (+1) ;

using FLOAT32 = float ;
using FLOAT64 = double ;

static constexpr auto FLOAT32_MAX = FLOAT32 (3.402823466E+38) ;
static constexpr auto FLOAT32_MIN = FLOAT32 (1.175494351E-38) ;
static constexpr auto FLOAT32_EPS = FLOAT32 (1.192092896E-07) ;
static constexpr auto FLOAT32_INF = FLOAT32 (std::numeric_limits<float>::infinity ()) ;
static constexpr auto FLOAT64_MAX = FLOAT64 (1.7976931348623158E+308) ;
static constexpr auto FLOAT64_MIN = FLOAT64 (2.2250738585072014E-308) ;
static constexpr auto FLOAT64_EPS = FLOAT64 (2.2204460492503131E-016) ;
static constexpr auto FLOAT64_INF = FLOAT64 (std::numeric_limits<float>::infinity ()) ;

using BYTE = std::uint8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using FEAT = std::uint64_t ;

namespace U {
enum class easy_char8_t :unsigned char ;
} ;

using STRA = char ;
using STRW = wchar_t ;
using STRU8 = U::easy_char8_t ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;

namespace U {
template <class...>
trait STR_HELP ;

#ifdef __CSC_CONFIG_STRA__
template <>
trait STR_HELP<> {
	using STR = STRA ;
} ;
#endif

#ifdef __CSC_CONFIG_STRW__
template <>
trait STR_HELP<> {
	using STR = STRW ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU8__
template <>
trait STR_HELP<> {
	using STR = STRU8 ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU16__
template <>
trait STR_HELP<> {
	using STR = STRU16 ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU32__
template <>
trait STR_HELP<> {
	using STR = STRU32 ;
} ;
#endif
} ;

using STR = typename U::STR_HELP<>::STR ;

static constexpr auto NULL = nullptr ;

namespace U {
template <class...>
trait ENUMAS ;

template <VAR>
trait ENUMID {} ;

template <VAR ARG1>
trait ENUMAS<ENUMID<ARG1>> {
	static constexpr auto value = ARG1 ;
} ;
} ;

namespace U {
template <class...>
trait TYPEAS ;

template <class ARG1>
trait TYPEID {} ;

template <class ARG1>
trait TYPEAS<ARG1> {
	static constexpr auto id = TYPEID<ARG1> () ;
} ;
} ;

template <class UNIT1>
using REMOVE_CVR = typename std::remove_cv<typename std::remove_reference<UNIT1>::type>::type ;

namespace U {
template <class...>
trait REMOVE_REF_HELP ;

template <>
trait REMOVE_REF_HELP<void> ;

template <class ARG1>
trait REMOVE_REF_HELP<ARG1> {
	using RET = REMOVE_CVR<ARG1> ;
} ;

template <class ARG1>
trait REMOVE_REF_HELP<TYPEID<ARG1>> {
	using RET = REMOVE_CVR<ARG1> ;
} ;
} ;

template <class UNIT1>
using REMOVE_REF = typename U::REMOVE_REF_HELP<REMOVE_CVR<UNIT1>>::RET ;

template <class UNIT1>
using DEF = UNIT1 ;

template <class UNIT1>
using PTR = DEF<typename std::remove_reference<UNIT1>::type> * ;

template <class UNIT1>
using VREF = DEF<UNIT1 &> ;

template <class UNIT1>
using CREF = DEF<const UNIT1 &> ;

template <class UNIT1>
using RREF = DEF<UNIT1 &&> ;

using ENUM_USED = enumof (-2) ;
using ENUM_NONE = enumof (-1) ;
using ENUM_ZERO = enumof (+0) ;
using ENUM_IDEN = enumof (+1) ;

template <class UNIT1>
using SIZEOF = enumof (sizeof (UNIT1)) ;

template <class UNIT1>
using ALIGNOF = enumof (alignof (UNIT1)) ;

namespace U {
template <class...>
trait COUNTOF_HELP ;

template <class...ARGS>
trait COUNTOF_HELP<typeas<ARGS...>> {
	using RET = enumof (sizeof... (ARGS)) ;
} ;
} ;

template <class UNIT1>
using COUNTOF = typename U::COUNTOF_HELP<UNIT1>::RET ;

template <class UNIT1 ,class UNIT2>
using IS_SAME = enumof (std::is_same<UNIT1 ,UNIT2>::value) ;

namespace U {
template <class...>
trait REQUIRE_HELP ;

template <>
trait REQUIRE_HELP<ENUM_IDEN> {
	using RET = void ;
} ;
} ;

template <class UNIT1>
using REQUIRE = typename U::REQUIRE_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait ENABLE_HELP ;

template <class ARG1 ,class ARG2>
trait ENABLE_HELP<ARG1 ,ARG2 ,REQUIRE<IS_SAME<ARG1 ,ENUM_IDEN>>> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;
} ;

template <class COND ,class YES = typeas<>>
using ENABLE = typename U::ENABLE_HELP<COND ,YES ,void>::RET ;

namespace U {
template <class...>
trait DEPENDENT_HELP ;

template <class ARG1 ,class ARG2>
trait DEPENDENT_HELP<ARG1 ,ARG2> {
	using RET = ARG1 ;
} ;
} ;

template <class BASE ,class DEP>
using DEPENDENT = typename U::DEPENDENT_HELP<BASE ,DEP>::RET ;

namespace U {
template <class...>
trait CONDITIONAL_HELP ;

template <class ARG1 ,class ARG2 ,class ARG3>
trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3 ,REQUIRE<IS_SAME<ARG1 ,ENUM_IDEN>>> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

template <class ARG1 ,class ARG2 ,class ARG3>
trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3 ,REQUIRE<IS_SAME<ARG1 ,ENUM_ZERO>>> {
	require (IS_SAME<ARG1 ,ENUM_ZERO>) ;

	using RET = ARG3 ;
} ;
} ;

template <class COND ,class YES ,class NO>
using CONDITIONAL = typename U::CONDITIONAL_HELP<COND ,YES ,NO ,void>::RET ;

namespace U {
template <class...>
trait IS_ENUM_HELP ;

template <class...ARGS>
trait IS_ENUM_HELP<ENUMAS<ARGS...>> {
	using RET = ENUM_IDEN ;
} ;
} ;

template <class UNIT1>
using IS_ENUM = typename U::IS_ENUM_HELP<UNIT1>::RET ;

template <class UNIT1>
using ENUM_CHECK = ENABLE<IS_ENUM<UNIT1> ,UNIT1> ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = enumof (ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = enumof (ENUM_CHECK<UNIT1>::value != ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1>
using ENUM_NOT = ENUM_EQUAL<UNIT1 ,ENUM_ZERO> ;

namespace U {
template <class...>
trait ENUM_COMPR_HELP ;

template <>
trait ENUM_COMPR_HELP<ENUM_IDEN ,ENUM_ZERO> {
	using RET = enumof (-1) ;
} ;

template <>
trait ENUM_COMPR_HELP<ENUM_ZERO ,ENUM_IDEN> {
	using RET = enumof (+1) ;
} ;

template <>
trait ENUM_COMPR_HELP<ENUM_ZERO ,ENUM_ZERO> {
	using RET = ENUM_ZERO ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = typename U::ENUM_COMPR_HELP<enumof (ENUM_CHECK<UNIT1>::value < ENUM_CHECK<UNIT2>::value) ,enumof (ENUM_CHECK<UNIT2>::value < ENUM_CHECK<UNIT1>::value)>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT_EQ = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT_EQ = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO) ;

template <class UNIT1>
using ENUM_EQ_ZERO = ENUM_EQUAL<UNIT1 ,ENUM_ZERO> ;

template <class UNIT1>
using ENUM_LT_ZERO = ENUM_COMPR_LT<UNIT1 ,ENUM_ZERO> ;

template <class UNIT1>
using ENUM_GT_ZERO = ENUM_COMPR_GT<UNIT1 ,ENUM_ZERO> ;

template <class UNIT1>
using ENUM_EQ_IDEN = ENUM_EQUAL<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_LT_IDEN = ENUM_COMPR_LT<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_GT_IDEN = ENUM_COMPR_GT<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1 ,class UNIT2>
using ENUM_ADD = enumof (ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = enumof (ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = enumof (ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = enumof (ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = enumof (ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1>
using ENUM_INC = ENUM_ADD<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_DEC = ENUM_SUB<UNIT1 ,ENUM_IDEN> ;

namespace U {
template <class...>
trait IS_TYPE_HELP ;

template <class...ARGS>
trait IS_TYPE_HELP<TYPEAS<ARGS...>> {
	using RET = ENUM_IDEN ;
} ;
} ;

template <class UNIT1>
using IS_TYPE = typename U::IS_TYPE_HELP<UNIT1>::RET ;

template <class UNIT1>
using TYPE_CHECK = ENABLE<IS_TYPE<UNIT1> ,UNIT1> ;

namespace U {
template <class...>
trait TYPE_FIRST_ONE_HELP ;

template <class ARG1 ,class...ARGS>
trait TYPE_FIRST_ONE_HELP<typeas<ARG1 ,ARGS...>> {
	using RET = ARG1 ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_ONE = typename U::TYPE_FIRST_ONE_HELP<TYPE_CHECK<UNIT1>>::RET ;

namespace U {
template <class...>
trait TYPE_FIRST_REST_HELP ;

template <class ARG1 ,class...ARGS>
trait TYPE_FIRST_REST_HELP<typeas<ARG1 ,ARGS...>> {
	using RET = typeas<ARGS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_REST = typename U::TYPE_FIRST_REST_HELP<TYPE_CHECK<UNIT1>>::RET ;

namespace U {
template <class...>
trait TYPE_SECOND_ONE_HELP ;

template <class ARG1 ,class ARG2 ,class...ARGS>
trait TYPE_SECOND_ONE_HELP<typeas<ARG1 ,ARG2 ,ARGS...>> {
	using RET = ARG2 ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_ONE = typename U::TYPE_SECOND_ONE_HELP<TYPE_CHECK<UNIT1>>::RET ;

namespace U {
template <class...>
trait TYPE_SECOND_REST_HELP ;

template <class ARG1 ,class ARG2 ,class...ARGS>
trait TYPE_SECOND_REST_HELP<typeas<ARG1 ,ARG2 ,ARGS...>> {
	using RET = typeas<ARGS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_REST = typename U::TYPE_SECOND_REST_HELP<TYPE_CHECK<UNIT1>>::RET ;

namespace U {
template <class...>
trait TYPE_UNWRAP_HELP ;

template <class ARG1>
trait TYPE_UNWRAP_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;
} ;

template <class UNIT1>
using TYPE_UNWRAP = typename U::TYPE_UNWRAP_HELP<UNIT1 ,void>::RET ;

namespace U {
template <class...>
trait TYPE_CAT_HELP ;

template <class...ARGS1 ,class...ARGS2>
trait TYPE_CAT_HELP<typeas<ARGS1...> ,typeas<ARGS2...>> {
	using RET = typeas<ARGS1... ,ARGS2...> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using TYPE_CAT = typename U::TYPE_CAT_HELP<TYPE_CHECK<UNIT1> ,TYPE_CHECK<UNIT2>>::RET ;

namespace U {
template <class...>
trait TYPE_REPEAT_HELP ;

template <class ARG1 ,class ARG2>
trait TYPE_REPEAT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQ_ZERO<ARG2>>> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = typeas<> ;
} ;

template <class ARG1 ,class ARG2>
trait TYPE_REPEAT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_GT_ZERO<ARG2>>> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using R1X = typename TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2> ,void>::RET ;

	using RET = TYPE_CAT<ARG1 ,R1X> ;
} ;
} ;

template <class UNIT1 ,class SIZE>
using TYPE_REPEAT = typename U::TYPE_REPEAT_HELP<typeas<UNIT1> ,SIZE ,void>::RET ;

namespace U {
template <class...>
trait TYPE_REVERSE_HELP ;

template <class ARG1>
trait TYPE_REVERSE_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = typeas<> ;
} ;

template <class ARG1>
trait TYPE_REVERSE_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = TYPE_CAT<R2X ,typeas<R1X>> ;
} ;
} ;

template <class UNIT1>
using TYPE_REVERSE = typename U::TYPE_REVERSE_HELP<UNIT1 ,void>::RET ;

namespace U {
template <class...>
trait TYPE_PICK_HELP ;

template <class ARG1 ,class ARG2>
trait TYPE_PICK_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQ_ZERO<ARG2>>> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait TYPE_PICK_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_GT_ZERO<ARG2>>> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<ARG1> ,ENUM_DEC<ARG2>>::RET ;
} ;
} ;

template <class LIST ,class INDEX>
using TYPE_PICK = typename U::TYPE_PICK_HELP<LIST ,INDEX ,void>::RET ;

namespace U {
template <class...>
trait ENUM_ALL_HELP ;

template <class ARG1>
trait ENUM_ALL_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait ENUM_ALL_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_NOT<ENUM_EQ_ZERO<TYPE_FIRST_ONE<ARG1>>> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1> ,void>::RET ;

	using RET = CONDITIONAL<R1X ,R2X ,ENUM_ZERO> ;
} ;
} ;

template <class...ARGS>
using ENUM_ALL = typename U::ENUM_ALL_HELP<typeas<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait ENUM_ANY_HELP ;

template <class ARG1>
trait ENUM_ANY_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait ENUM_ANY_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_NOT<ENUM_EQ_ZERO<TYPE_FIRST_ONE<ARG1>>> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<ARG1> ,void>::RET ;

	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R2X> ;
} ;
} ;

template <class...ARGS>
using ENUM_ANY = typename U::ENUM_ANY_HELP<typeas<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait IS_ALL_SAME_HELP ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_GT_IDEN<COUNTOF<ARG1>>>> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ALL_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>> ,void>::RET ;

	using RET = ENUM_ALL<IS_SAME<R1X ,R2X> ,R3X> ;
} ;
} ;

template <class...ARGS>
using IS_ALL_SAME = typename U::IS_ALL_SAME_HELP<typeas<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait IS_ANY_SAME_HELP ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_GT_IDEN<COUNTOF<ARG1>>>> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ANY_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>>>::RET ;
	using R4X = typename IS_ANY_SAME_HELP<TYPE_CAT<R2X ,TYPE_SECOND_REST<ARG1>>>::RET ;

	using RET = ENUM_ANY<IS_SAME<R1X ,R2X> ,R3X ,R4X> ;
} ;
} ;

template <class...ARGS>
using IS_ANY_SAME = typename U::IS_ANY_SAME_HELP<typeas<ARGS...>>::RET ;

namespace U {
template <class...>
trait PLACEHOLDER_HELP ;

template <class ARG1>
trait PLACEHOLDER_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<ARG1>>> {
	require (ENUM_EQ_ZERO<ARG1>) ;

	class PlaceHolder ;
} ;

template <class UNIT1>
class PLACEHOLDER_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<UNIT1>>>::PlaceHolder {} ;

template <class ARG1>
trait PLACEHOLDER_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<ARG1>>> {
	require (ENUM_GT_ZERO<ARG1>) ;

	using BASE = typename PLACEHOLDER_HELP<ENUM_DEC<ARG1> ,void>::PlaceHolder ;

	class PlaceHolder ;
} ;

template <class UNIT1>
class PLACEHOLDER_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<UNIT1>>>::PlaceHolder :
	public BASE {} ;
} ;

template <class RANK>
using PlaceHolder = typename U::PLACEHOLDER_HELP<RANK ,void>::PlaceHolder ;

static constexpr auto PH0 = PlaceHolder<enumof (0)> () ;
static constexpr auto PH1 = PlaceHolder<enumof (1)> () ;
static constexpr auto PH2 = PlaceHolder<enumof (2)> () ;
static constexpr auto PH3 = PlaceHolder<enumof (3)> () ;
static constexpr auto PH4 = PlaceHolder<enumof (4)> () ;
static constexpr auto PH5 = PlaceHolder<enumof (5)> () ;
static constexpr auto PH6 = PlaceHolder<enumof (6)> () ;
static constexpr auto PH7 = PlaceHolder<enumof (7)> () ;
static constexpr auto PH8 = PlaceHolder<enumof (8)> () ;
static constexpr auto PH9 = PlaceHolder<enumof (9)> () ;
static constexpr auto PHX = PlaceHolder<enumof (10)> () ;

template <class UNIT1>
using IS_BOOL = IS_SAME<UNIT1 ,BOOL> ;

template <class UNIT1>
using IS_VAR = ENUM_ANY<IS_SAME<UNIT1 ,VAR32> ,IS_SAME<UNIT1 ,VAR64>> ;

template <class UNIT1>
using IS_FLOAT = ENUM_ANY<IS_SAME<UNIT1 ,FLOAT32> ,IS_SAME<UNIT1 ,FLOAT64>> ;

template <class UNIT1>
using IS_BYTE = ENUM_ANY<IS_SAME<UNIT1 ,BYTE> ,IS_SAME<UNIT1 ,WORD> ,IS_SAME<UNIT1 ,CHAR> ,IS_SAME<UNIT1 ,FEAT>> ;

template <class UNIT1>
using IS_STR = ENUM_ANY<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT1 ,STRU32>> ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,typeof (NULL)> ;

template <class UNIT1>
using IS_BASIC = ENUM_ANY<IS_BOOL<UNIT1> ,IS_VAR<UNIT1> ,IS_FLOAT<UNIT1> ,IS_BYTE<UNIT1> ,IS_STR<UNIT1> ,IS_NULL<UNIT1>> ;

template <class UNIT1>
using IS_FUNCTION = enumof (std::is_function<UNIT1>::value) ;

template <class UNIT1>
using IS_NOEXCPET = enumof (FALSE) ;

template <class UNIT1>
using IS_CLASS = enumof (std::is_class<UNIT1>::value) ;

template <class UNIT1>
using IS_STRUCT = enumof (std::is_class<UNIT1>::value) ;

template <class UNIT1>
using IS_TRIVIAL = enumof (std::is_trivial<UNIT1>::value) ;

namespace U {
template <class...>
trait IS_CONSTRUCTIBLE_HELP ;

template <class ARG1 ,class...ARGS>
trait IS_CONSTRUCTIBLE_HELP<ARG1 ,typeas<ARGS...>> {
	using RET = enumof (std::is_constructible<ARG1 ,ARGS...>::value) ;
} ;
} ;

template <class UNIT1 ,class PARAMS>
using IS_CONSTRUCTIBLE = typename U::IS_CONSTRUCTIBLE_HELP<UNIT1 ,PARAMS>::RET ;

template <class UNIT1>
using IS_CLONEABLE = enumof (std::is_copy_constructible<UNIT1>::value && std::is_copy_assignable<UNIT1>::value) ;

class Interface {
public:
	implicit Interface () = default ;
	implicit virtual ~Interface () noexcept = default ;
	implicit Interface (CREF<Interface>) = delete ;
	inline void operator= (CREF<Interface>) = delete ;
	implicit Interface (RREF<Interface>) = delete ;
	inline void operator= (RREF<Interface>) = delete ;
} ;

template <class UNIT1>
using IS_INTERFACE = enumof (std::is_base_of<Interface ,UNIT1>::value && std::is_abstract<UNIT1>::value) ;

template <class BASE ,class DERIVED>
using IS_EXTEND = enumof (std::is_base_of<BASE ,DERIVED>::value) ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,typeof (PH0)> ;

namespace U {
template <class...>
trait BYTE_TRAIT_HELP ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<BYTE>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<BYTE>>>> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<BYTE>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGNOF<BYTE>>) ;

	using RET = BYTE ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<WORD>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<WORD>>>> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<WORD>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGNOF<WORD>>) ;

	using RET = WORD ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<CHAR>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<CHAR>>>> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<CHAR>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGNOF<CHAR>>) ;

	using RET = CHAR ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<FEAT>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<FEAT>>>> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<FEAT>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGNOF<FEAT>>) ;

	using RET = FEAT ;
} ;
} ;

template <class SIZE ,class ALIGN>
using BYTE_TRAIT = typename U::BYTE_TRAIT_HELP<SIZE ,ALIGN ,void ,void>::RET ;

namespace U {
template <class...>
trait CAST_TRAIT_HELP ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,ARG2> {
	using RET = ARG1 ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const ARG2>> {
	using RET = DEF<const ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<volatile ARG2>> {
	using RET = DEF<volatile ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const volatile ARG2>> {
	using RET = DEF<const volatile ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,VREF<ARG2>> {
	using RET = VREF<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const ARG2 &>> {
	using RET = DEF<const ARG1 &> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<volatile ARG2 &>> {
	using RET = DEF<volatile ARG1 &> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const volatile ARG2 &>> {
	using RET = DEF<const volatile ARG1 &> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,RREF<ARG2>> {
	using RET = RREF<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const ARG2 &&>> {
	using RET = DEF<const ARG1 &&> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<volatile ARG2 &&>> {
	using RET = DEF<volatile ARG1 &&> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,DEF<const volatile ARG2 &&>> {
	using RET = DEF<const volatile ARG1 &&> ;
} ;
} ;

template <class TO ,class FROM>
using CAST_TRAIT = typename U::CAST_TRAIT_HELP<TO ,FROM>::RET ;

namespace U {
template <class...>
trait PACK_HELP ;

template <>
trait PACK_HELP<> {
	struct PACK ;
} ;

struct PACK_HELP<>::PACK {} ;

template <class ARG1>
trait PACK_HELP<ARG1> {
	struct PACK ;
} ;

template <class UNIT1>
struct PACK_HELP<UNIT1>::PACK {
	UNIT1 mP1 ;
} ;

template <class ARG1 ,class ARG2>
trait PACK_HELP<ARG1 ,ARG2> {
	struct PACK ;
} ;

template <class UNIT1 ,class UNIT2>
struct PACK_HELP<UNIT1 ,UNIT2>::PACK {
	UNIT1 mP1 ;
	UNIT2 mP2 ;
} ;

template <class ARG1 ,class ARG2 ,class ARG3>
trait PACK_HELP<ARG1 ,ARG2 ,ARG3> {
	struct PACK ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
struct PACK_HELP<UNIT1 ,UNIT2 ,UNIT3>::PACK {
	UNIT1 mP1 ;
	UNIT2 mP2 ;
	UNIT3 mP3 ;
} ;
} ;

template <class...UNITS>
using PACK = typename U::PACK_HELP<UNITS...>::PACK ;

namespace U {
class FUNCTION_noop {
public:
	inline void operator() () const {
		return ;
	}
} ;
} ;

static constexpr auto noop = U::FUNCTION_noop () ;

namespace U {
#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_MSVC__
class FUNCTION_abort {
public:
	inline void operator() () const {
		__debugbreak () ;
		std::terminate () ;
	}
} ;
#endif
#endif

#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_GNUC__
class FUNCTION_abort {
public:
	inline void operator() () const {
		std::terminate () ;
	}
} ;
#endif
#endif

#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_CLANG__
class FUNCTION_abort {
public:
	inline void operator() () const {
		std::terminate () ;
	}
} ;
#endif
#endif
} ;

static constexpr auto abort = U::FUNCTION_abort () ;

namespace U {
class FUNCTION_address {
public:
	template <class ARG1>
	inline LENGTH operator() (CREF<ARG1> obj) const {
		return LENGTH (property (reinterpret_cast<CREF<BOOL>> (obj))) ;
	}
} ;
} ;

static constexpr auto address = U::FUNCTION_address () ;

namespace U {
class FUNCTION_bitwise {
public:
	template <class ARG1>
	inline BYTE_TRAIT<SIZEOF<ARG1> ,ALIGNOF<ARG1>> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = typeof (obj) ;
		require (IS_TRIVIAL<R1X>) ;
		using R2X = BYTE_TRAIT<SIZEOF<R1X> ,ALIGNOF<R1X>> ;
		return reinterpret_cast<CREF<R2X>> (obj) ;
	}
} ;
} ;

static constexpr auto bitwise = U::FUNCTION_bitwise () ;

namespace U {
#ifdef __CSC_COMPILER_MSVC__
using FUNCTION_barrier = FUNCTION_noop ;
#endif

#ifdef __CSC_COMPILER_GNUC__
class FUNCTION_barrier {
public:
	inline void operator() () const noexcept {
		asm volatile ("" ::: "memory") ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
class FUNCTION_barrier {
public:
	inline void operator() () const noexcept {
		asm volatile ("" ::: "memory") ;
	}
} ;
#endif
} ;

static constexpr auto barrier = U::FUNCTION_barrier () ;

namespace U {
template <class...>
trait TEMP_HELP ;

template <class ARG1>
trait TEMP_HELP<ARG1> {
	struct TEMP ;
} ;

template <class UNIT1>
struct TEMP_HELP<UNIT1>::TEMP {
	using ITEM = BYTE_TRAIT<ALIGNOF<UNIT1> ,ALIGNOF<UNIT1>> ;
	using SIZE = ENUM_DIV<ENUM_DEC<ENUM_ADD<SIZEOF<UNIT1> ,SIZEOF<ITEM>>> ,SIZEOF<ITEM>> ;

	DEF<ITEM[SIZE::value]> mUnused ;
} ;
} ;

template <class...UNIT1>
using TEMP = typename U::TEMP_HELP<UNIT1...>::TEMP ;

namespace U {
class FUNCTION_property {
public:
	template <class ARG1>
	inline VREF<ARG1> operator[] (CREF<DEF<ARG1 *>> arg1) const noexcept {
		return (*arg1) ;
	}

	template <class ARG1>
	inline PTR<ARG1> operator() (VREF<ARG1> arg1) const noexcept {
		return (&arg1) ;
	}
} ;
} ;

static constexpr auto property = U::FUNCTION_property () ;

namespace U {
class FUNCTION_temp_ref {
public:
	template <class ARG1>
	inline PTR<ARG1> operator() (VREF<TEMP<ARG1>> tmp) const noexcept {
		return property (reinterpret_cast<VREF<ARG1>> (tmp)) ;
	}
} ;
} ;

static constexpr auto temp_ref = U::FUNCTION_temp_ref () ;

namespace U {
class FUNCTION_temp_ptr {
public:
	template <class ARG1>
	inline PTR<TEMP<ARG1>> operator() (VREF<ARG1> tmp) const noexcept {
		return property (reinterpret_cast<VREF<TEMP<ARG1>>> (tmp)) ;
	}
} ;
} ;

static constexpr auto temp_ptr = U::FUNCTION_temp_ptr () ;

namespace U {
class FUNCTION_swap {
public:
	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> arg1 ,VREF<ARG2> arg2) const noexcept {
		using R1X = typeof (arg1) ;
		using R2X = typeof (arg2) ;
		require (IS_SAME<R1X ,R2X>) ;
		auto rax = property[temp_ptr (arg1)] ;
		property[temp_ptr (arg1)] = property[temp_ptr (arg2)] ;
		property[temp_ptr (arg2)] = rax ;
		barrier () ;
	}
} ;
} ;

static constexpr auto swap = U::FUNCTION_swap () ;

namespace U {
class FUNCTION_forward {
public:
	template <class ARG1>
	inline CREF<REMOVE_REF<ARG1>> operator() (CREF<ARG1> obj) const {
		return static_cast<CREF<REMOVE_REF<ARG1>>> (obj) ;
	}

	template <class ARG1>
	inline RREF<REMOVE_REF<ARG1>> operator() (VREF<ARG1> obj) const {
		return static_cast<RREF<REMOVE_REF<ARG1>>> (obj) ;
	}
} ;
} ;

static constexpr auto forward = U::FUNCTION_forward () ;

namespace U {
class FUNCTION_create {
public:
	template <class ARG1 ,class...ARGS>
	inline void operator() (VREF<TEMP<ARG1>> tmp ,RREF<ARGS>...args) const {
		new (property (tmp)) ARG1 (forward (args)...) ;
	}
} ;
} ;

static constexpr auto create = U::FUNCTION_create () ;

namespace U {
class FUNCTION_recreate {
public:
	template <class ARG1>
	inline void operator() (VREF<ARG1> tmp ,RREF<ARG1> obj) const noexcept {
		tmp.~ARG1 () ;
		new (property (tmp)) ARG1 (forward (obj)) ;
	}
} ;
} ;

static constexpr auto recreate = U::FUNCTION_recreate () ;

namespace U {
class FUNCTION_destroy {
public:
	template <class ARG1>
	inline void operator() (VREF<TEMP<ARG1>> tmp) const noexcept {
		property[temp_ref (tmp)].~ARG1 () ;
	}
} ;
} ;

static constexpr auto destroy = U::FUNCTION_destroy () ;

namespace U {
class FUNCTION_zeroize {
public:
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const noexcept {
		using R1X = typeof (obj) ;
		require (IS_TRIVIAL<R1X>) ;
		property[temp_ptr (obj)] = {0} ;
		barrier () ;
	}
} ;
} ;

static constexpr auto zeroize = U::FUNCTION_zeroize () ;

namespace U {
class FUNCTION_alignto {
public:
	inline LENGTH operator() (CREF<LENGTH> base ,CREF<LENGTH> align) const {
		const auto r1x = align - base % align ;
		return base + r1x % align ;
	}
} ;
} ;

static constexpr auto alignto = U::FUNCTION_alignto () ;

namespace U {
class FUNCTION_between {
public:
	inline BOOL operator() (CREF<INDEX> index ,CREF<INDEX> begin ,CREF<INDEX> end) const {
		return index >= begin && index < end ;
	}
} ;
} ;

static constexpr auto between = U::FUNCTION_between () ;

namespace U {
class FUNCTION_abs {
public:
	template <class ARG1>
	inline REMOVE_REF<ARG1> operator() (CREF<ARG1> arg1) const {
		using R1X = typeof (arg1) ;
		if (arg1 < R1X (ZERO))
			return -arg1 ;
		return +arg1 ;
	}
} ;
} ;

static constexpr auto abs = U::FUNCTION_abs () ;

namespace U {
class FUNCTION_min {
public:
	template <class ARG1>
	inline REMOVE_REF<ARG1> operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const {
		if (arg1 < arg2)
			return arg1 ;
		return arg2 ;
	}
} ;
} ;

static constexpr auto min = U::FUNCTION_min () ;

namespace U {
class FUNCTION_max {
public:
	template <class ARG1>
	inline REMOVE_REF<ARG1> operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const {
		if (arg1 < arg2)
			return arg2 ;
		return arg1 ;
	}
} ;
} ;

static constexpr auto max = U::FUNCTION_max () ;

namespace U {
template <class...>
trait FUNCTION_COMPR_HELP ;

template <class ARG1>
trait FUNCTION_COMPR_HELP<ARG1 ,REQUIRE<IS_BASIC<ARG1>>> {
	struct FUNCTION_COMPR ;
} ;

template <class ARG1>
struct FUNCTION_COMPR_HELP<ARG1 ,REQUIRE<IS_BASIC<ARG1>>>::FUNCTION_COMPR {
	inline FLAG operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const {
		if (arg1 < arg2)
			return FLAG (-1) ;
		if (arg2 < arg1)
			return FLAG (+1) ;
		return ZERO ;
	}
} ;

template <class ARG1>
trait FUNCTION_COMPR_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>> {
	struct FUNCTION_COMPR ;
} ;

template <class ARG1>
struct FUNCTION_COMPR_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>>::FUNCTION_COMPR {
	inline FLAG operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const {
		return arg1.compr (arg2) ;
	}
} ;

class FUNCTION_compr {
public:
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const {
		using R1X = typeof (arg1) ;
		using R2X = typename FUNCTION_COMPR_HELP<R1X ,void>::FUNCTION_COMPR ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (arg1 ,arg2) ;
	}
} ;
} ;

static constexpr auto operator_compr = U::FUNCTION_compr () ;

namespace U {
template <class...>
trait FUNCTION_HASH_HELP ;

template <class ARG1>
trait FUNCTION_HASH_HELP<ARG1 ,REQUIRE<IS_BASIC<ARG1>>> {
	struct FUNCTION_HASH ;
} ;

template <class ARG1>
struct FUNCTION_HASH_HELP<ARG1 ,REQUIRE<IS_BASIC<ARG1>>>::FUNCTION_HASH {
	inline FLAG operator() (CREF<ARG1> arg1) const {
		return FLAG (bitwise (arg1)) ;
	}
} ;

template <class ARG1>
trait FUNCTION_HASH_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>> {
	struct FUNCTION_HASH ;
} ;

template <class ARG1>
struct FUNCTION_HASH_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>>::FUNCTION_HASH {
	inline FLAG operator() (CREF<ARG1> arg1) const {
		return arg1.hash () ;
	}
} ;

class FUNCTION_hash {
public:
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> arg1) const {
		using R1X = typeof (arg1) ;
		using R2X = typename FUNCTION_HASH_HELP<R1X ,void>::FUNCTION_HASH ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (arg1) ;
	}
} ;
} ;

static constexpr auto operator_hash = U::FUNCTION_hash () ;

namespace U {
class FUNCTION_hashcode {
public:
	inline FLAG operator() () const {
		const auto r1x = VAR64 (-3750763034362895579) ;
		const auto r2x = VAR64 (FEAT (r1x) & FEAT (VAR64_MAX)) ;
		return FLAG (r2x) ;
	}

	inline FLAG operator() (CREF<FLAG> arg1 ,CREF<FLAG> arg2) const {
		const auto r1x = VAR64 (FEAT (arg1) ^ FEAT (arg2)) ;
		const auto r2x = r1x * VAR64 (1099511628211) ;
		const auto r3x = VAR64 (FEAT (r2x) & FEAT (VAR64_MAX)) ;
		return FLAG (r3x) ;
	}
} ;
} ;

static constexpr auto hashcode = U::FUNCTION_hashcode () ;

namespace U {
template <class...>
trait TYPEMID_HELP ;

template <class ARG1>
trait TYPEMID_HELP<ARG1> {
	class TYPEMID ;
} ;

template <class UNIT1>
class TYPEMID_HELP<UNIT1>::TYPEMID :
	private Interface {} ;

class FUNCTION_type_mid {
public:
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> id) const {
		using R1X = typeof (id) ;
		using R2X = typename TYPEMID_HELP<R1X>::TYPEMID ;
		require (ENUM_EQUAL<SIZEOF<FLAG> ,SIZEOF<R2X>>) ;
		require (ENUM_EQUAL<ALIGNOF<FLAG> ,ALIGNOF<R2X>>) ;
		return FLAG (bitwise (R2X ())) ;
	}
} ;
} ;

static constexpr auto type_mid = U::FUNCTION_type_mid () ;

namespace U {
template <class...>
trait LAMBDA_HELP ;

template <class ARG1>
trait LAMBDA_HELP<ARG1> {
	class Lambda ;
} ;

template <class UNIT1>
class LAMBDA_HELP<UNIT1>::Lambda {
public:
	UNIT1 mInvoke ;

public:
	implicit Lambda () = delete ;

	implicit Lambda (CREF<Lambda>) = delete ;

	inline void operator= (CREF<Lambda>) = delete ;

	implicit Lambda (RREF<Lambda>) = delete ;

	inline void operator= (RREF<Lambda>) = delete ;

	CREF<UNIT1> to () const leftvalue {
		return mInvoke ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}
} ;

template <class UNIT1>
using Lambda = typename LAMBDA_HELP<UNIT1>::Lambda ;

class FUNCTION_capture {
public:
	template <class ARG1>
	inline CREF<Lambda<REMOVE_REF<ARG1>>> operator() (RREF<ARG1> lambda) const {
		using R1X = typeof (lambda) ;
		require (ENUM_EQUAL<SIZEOF<R1X> ,SIZEOF<Lambda<R1X>>>) ;
		require (ENUM_EQUAL<ALIGNOF<R1X> ,ALIGNOF<Lambda<R1X>>>) ;
		return reinterpret_cast<CREF<Lambda<R1X>>> (lambda) ;
	}
} ;
} ;

static constexpr auto capture = U::FUNCTION_capture () ;

namespace U {
template <class...>
trait AUTO_HELP ;

template <>
trait AUTO_HELP<> {
	class Auto ;

	struct EXTERN {
		interface Holder ;

		struct BLOCK ;
		struct SLOT ;
		class AutoHeap ;
	} ;
} ;

interface AUTO_HELP<>::EXTERN::Holder :
	public Interface {
	virtual void destroy () const = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_mid () const = 0 ;
} ;

struct AUTO_HELP<>::EXTERN::BLOCK {
	VAR32 mBlockCheck1 ;
	VAR32 mSlot ;
	VAR32 mBlockCheck2 ;
} ;

struct AUTO_HELP<>::EXTERN::SLOT {
	FLAG mUsed ;
	LENGTH mBlockSize ;
	PTR<BLOCK> mBlock ;
} ;

class AUTO_HELP<>::EXTERN::AutoHeap {
public:
	template <class ARG1>
	static PTR<REMOVE_REF<ARG1>> alloc (CREF<ARG1> id) {
		using R1X = typeof (id) ;
		require (IS_TRIVIAL<R1X>) ;
		const auto r10x = property (slot_array ()) ;
		const auto r1x = alloc (SIZEOF<R1X>::value ,ALIGNOF<R1X>::value) ;
		const auto r2x = address (property[r10x][r1x].mBlock) + SIZEOF<BLOCK>::value ;
		const auto r3x = alignto (r2x ,ALIGNOF<R1X>::value) ;
		return reinterpret_cast<PTR<REMOVE_REF<ARG1>>> (r3x) ;
	}

	static INDEX alloc (CREF<LENGTH> size_len ,CREF<LENGTH> align_len) {
		assert (size_len > ZERO) ;
		assert (align_len > ZERO) ;
		const auto r10x = property (slot_array ()) ;
		const auto r1x = align_len - ALIGNOF<BLOCK>::value ;
		const auto r2x = size_len + max (r1x ,ZERO) ;
		const auto r3x = property[r10x][0] ;
		assert (r3x.mBlockSize >= SIZEOF<BLOCK>::value + r2x) ;

	}

	static void free (CREF<LENGTH> addr) {
		const auto r10x = property (slot_array ()) ;
		const auto r1x = property[r10x][0] ;
		assert (r1x.mBlockCheck1 == VAR32 (0XCCCCCCCC)) ;
		assert (r1x.mBlockCheck2 == VAR32 (0XCCCCCCCC)) ;
		
	}

private:
	static VREF<BLOCK[]> block_array () {

	}

	static VREF<SLOT[]> slot_array () {

	}

	static VAR32 root () {
		thread_local auto M_STORAGE = TEMP<BYTE[4096]> () ;
		auto &&r1x = capture ([&] () {
			const auto r2x = INDEX (property (M_STORAGE)) ;
			PTR<BLOCK> ret = reinterpret_cast<PTR<BLOCK>> (r2x) ;
			ret->mPrev = NONE ;
			ret->mNext = NONE ;
			ret->mBlockSize = VAR32 (SIZEOF<typeof (M_STORAGE)>::value) ;
			ret->mBlockCheck1 = VAR32 (0XCCCCCCCC) ;
			ret->mBlockCheck2 = VAR32 (0XCCCCCCCC) ;
			return forward (ret) ;
		}) ;
		thread_local auto M_ROOT = r1x.self () ;
		return M_ROOT ;
	}
} ;

template <class...>
trait AUTO_IMPLHOLDER_HELP ;

template <class ARG1>
trait AUTO_IMPLHOLDER_HELP<ARG1> {
	using EXTERN = typename AUTO_HELP<>::EXTERN ;
	using Holder = typename EXTERN::Holder ;

	class ImplHolder ;
} ;

class AUTO_HELP<>::Auto {
private:
	using Holder = typename EXTERN::Holder ;
	using AutoHeap = typename EXTERN::AutoHeap ;

private:
	PTR<Holder> mPointer ;

public:
	implicit Auto () noexcept :
		delegate mPointer (NULL) {}

	template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_REF<ARG1> ,Auto>>>>
	explicit Auto (RREF<ARG1> that) {
		struct PHDEP ;
		using R1X = typeof (that) ;
		using R2X = typename DEPENDENT<AUTO_IMPLHOLDER_HELP<R1X> ,PHDEP>::ImplHolder ;
		Auto ret ;
		const auto r1x = AutoHeap::alloc (typeas<TEMP<R2X>>::id) ;
		assert (r1x != NULL) ;
		create (property[r1x] ,forward (that)) ;
		ret.mPointer = property[temp_ref (r1x)] ;
		return forward (ret) ;
	}

	implicit ~Auto () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->destroy () ;
		mPointer = NULL ;
	}

	implicit Auto (CREF<Auto>) = delete ;

	inline void operator= (CREF<Auto>) = delete ;

	implicit Auto (RREF<Auto> that) noexcept :
		delegate Auto () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Auto>) = delete ;

	template <class ARG1>
	REMOVE_REF<ARG1> poll (CREF<ARG1> id) {
		using R1X = typeof (id) ;
		using R2X = typename AUTO_IMPLHOLDER_HELP<R1X>::ImplHolder ;
		assert (mPointer != NULL) ;
		const auto r1x = type_mid (typeas<R2X>::id) ;
		const auto r2x = mPointer->type_mid () ;
		assert (r1x == r2x) ;
		const auto r3x = property (m_derived (typeas<R2X>::id)) ;
		return r3x->poll () ;
	}

	template <class ARG1>
	implicit operator ARG1 () rightvalue {
		using R1X = typeof (ARG1 ()) ;
		return poll (typeas<R1X>::id) ;
	}

private:
	template <class ARG1>
	VREF<REMOVE_REF<ARG1>> m_derived (CREF<ARG1> id) leftvalue {
		using R1X = typeof (id) ;
		return static_cast<VREF<R1X>> (property[mPointer]) ;
	}
} ;

template <class UNIT1>
class AUTO_IMPLHOLDER_HELP<UNIT1>::ImplHolder :
	public Holder {
private:
	using AutoHeap = typename EXTERN::AutoHeap ;

private:
	UNIT1 mValue ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNIT1> that) :
		delegate mValue (forward (that)) {}

	void destroy () const override {
		auto &&thiz = property[this] ;
		CSC::destroy (property[temp_ptr (thiz)]) ;
		AutoHeap::free (address (thiz)) ;
		barrier () ;
	}

	LENGTH type_size () const override {
		return SIZEOF<ImplHolder>::value ;
	}

	LENGTH type_align () const override {
		return ALIGNOF<ImplHolder>::value ;
	}

	FLAG type_mid () const override {
		return CSC::type_mid (typeas<ImplHolder>::id) ;
	}

	UNIT1 poll () {
		return forward (mValue) ;
	}
} ;
} ;

using Auto = typename U::AUTO_HELP<>::Auto ;

namespace U {
class FUNCTION_bad {
public:
	template <class ARG1>
	inline REMOVE_REF<ARG1> operator() (CREF<ARG1> id) const {
		using R1X = typeof (id) ;
		assert (FALSE) ;
		return R1X (Auto (ZERO)) ;
	}
} ;
} ;

static constexpr auto bad = U::FUNCTION_bad () ;

namespace U {
template <class...>
trait BOX_HELP ;

template <class ARG1>
trait BOX_HELP<ARG1 ,REQUIRE<IS_INTERFACE<ARG1>>> {
	require (IS_INTERFACE<ARG1>) ;

	struct EXTERN {
		interface Holder ;
	} ;

	class Box ;
} ;

template <class UNIT1>
interface BOX_HELP<UNIT1 ,REQUIRE<IS_INTERFACE<UNIT1>>>::EXTERN::Holder :
	public Interface {
	virtual void destroy () = 0 ;
	virtual VREF<UNIT1> to () leftvalue = 0 ;
	virtual CREF<UNIT1> to () const leftvalue = 0 ;
} ;

template <class...>
trait BOX_IMPLHOLDER_HELP ;

template <class ARG1 ,class ARG2>
trait BOX_IMPLHOLDER_HELP<ARG1 ,ARG2> {
	using EXTERN = typename BOX_HELP<ARG1 ,REQUIRE<IS_INTERFACE<ARG1>>>::EXTERN ;
	using Holder = typename EXTERN::Holder ;

	class ImplHolder ;
} ;

template <class UNIT1>
class BOX_HELP<UNIT1 ,REQUIRE<IS_INTERFACE<UNIT1>>>::Box {
private:
	using Holder = typename EXTERN::Holder ;

private:
	PTR<Holder> mPointer ;

public:
	implicit Box () noexcept :
		delegate mPointer (NULL) {}

	template <class ARG1>
	static Box make (RREF<ARG1> that) {
		struct PHDEP ;
		Box ret ;
		using R1X = typeof (that) ;
		require (IS_EXTEND<UNIT1 ,R1X>) ;
		using R2X = typename DEPENDENT<BOX_IMPLHOLDER_HELP<R1X> ,PHDEP>::ImplHolder ;
		ret.mPointer = new R2X (forward (that)) ;
		return forward (ret) ;
	}

	implicit ~Box () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->destroy () ;
		mPointer = NULL ;
	}

	implicit Box (CREF<Box>) = delete ;

	inline void operator= (CREF<Box>) = delete ;

	implicit Box (RREF<Box> that) noexcept :
		delegate Box () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Box> that) noexcept {
		auto &&thiz = property[this] ;
		if (property (thiz) == property (that))
			return ;
		recreate (thiz ,forward (that)) ;
	}

	BOOL exist () const {
		return mPointer != NULL ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<UNIT1> to () leftvalue {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator VREF<UNIT1> () leftvalue {
		return self ;
	}

	inline PTR<VREF<UNIT1>> operator-> () leftvalue {
		return property (self) ;
	}

	CREF<UNIT1> to () const leftvalue {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}

	inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
		return property (self) ;
	}
} ;

template <class UNIT1 ,class UNTI2>
class BOX_IMPLHOLDER_HELP<UNIT1 ,UNTI2>::ImplHolder :
	public Holder {
private:
	UNTI2 mValue ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNTI2> that) :
		delegate mValue (forward (that)) {}

	void destroy () const override {
		auto &&thiz = property[this] ;
		delete property (thiz) ;
	}

	VREF<UNIT1> to () leftvalue override {
		return mValue ;
	}

	CREF<UNIT1> to () const leftvalue override {
		return mValue ;
	}
} ;
} ;

template <class UNIT1>
using Box = typename U::BOX_HELP<UNIT1 ,void>::Box ;

namespace U {
template <class...>
trait CELL_HELP ;

template <class ARG1>
trait CELL_HELP<ARG1 ,REQUIRE<IS_CLONEABLE<ARG1>>> {
	require (IS_CLONEABLE<ARG1>) ;

	class Cell ;
} ;

template <class UNIT1>
class CELL_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>>::Cell {
private:
	mutable TEMP<UNIT1> mValue ;
	BOOL mExist ;

public:
	implicit Cell () noexcept :
		delegate mExist (FALSE) {}

	static Cell make (RREF<UNIT1> that) {
		Cell ret ;
		create (ret.mValue ,forward (that)) ;
		ret.mExist = TRUE ;
		return forward (ret) ;
	}

	implicit ~Cell () noexcept {
		if ifnot (mExist)
			return ;
		destroy (mValue) ;
		mExist = FALSE ;
	}

	implicit Cell (CREF<Cell> that) :
		delegate Cell () {
		auto &&thiz = property[this] ;
		if ifnot (that.exist ())
			return ;
		thiz = Cell::make (that.fetch ()) ;
	}

	inline void operator= (CREF<Cell> that) {
		auto &&thiz = property[this] ;
		if (property (thiz) == property (that))
			return ;
		recreate (thiz ,forward (that)) ;
	}

	implicit Cell (RREF<Cell> that) noexcept :
		delegate Cell () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Cell> that) noexcept {
		auto &&thiz = property[this] ;
		if (property (thiz) == property (that))
			return ;
		recreate (thiz ,forward (that)) ;
	}

	BOOL exist () const {
		return mExist ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	UNIT1 fetch () const {
		assert (exist ()) ;
		return m_fake () ;
	}

	UNIT1 value (RREF<UNIT1> obj) const {
		if ifnot (exist ())
			return forward (obj) ;
		return m_fake () ;
	}

	void store (RREF<UNIT1> obj) const {
		assert (exist ()) ;
		m_fake () = forward (obj) ;
	}

	UNIT1 exchange (RREF<UNIT1> obj) const {
		assert (exist ()) ;
		UNIT1 ret = m_fake () ;
		m_fake () = forward (obj) ;
		return forward (ret) ;
	}

	BOOL change (VREF<UNIT1> expect ,RREF<UNIT1> obj) const {
		assert (exist ()) ;
		if ifswitch (TRUE) {
			if (m_fake () == expect)
				discard ;
			expect = m_fake () ;
			return FALSE ;
		}
		m_fake () = forward (obj) ;
		return TRUE ;
	}

private:
	VREF<UNIT1> m_fake () const leftvalue {
		return property[temp_ref (mValue)] ;
	}
} ;
} ;

template <class UNIT1>
using Cell = typename U::CELL_HELP<UNIT1 ,void>::Cell ;

namespace U {
template <class...>
trait RC_HELP ;

template <class ARG1>
trait RC_HELP<ARG1> {
	struct EXTERN {
		interface Holder ;
	} ;

	class RC ;
} ;

template <class UNIT1>
interface RC_HELP<UNIT1>::EXTERN::Holder :
	public Interface {
	virtual void destroy () = 0 ;
	virtual CREF<UNIT1> to () const leftvalue = 0 ;
	virtual LENGTH increase () = 0 ;
	virtual LENGTH decrease () = 0 ;
} ;

template <class...>
trait RC_IMPLHOLDER_HELP ;

template <class ARG1>
trait RC_IMPLHOLDER_HELP<ARG1> {
	using EXTERN = typename RC_HELP<ARG1>::EXTERN ;
	using Holder = typename EXTERN::Holder ;

	class ImplHolder ;
} ;

template <class UNIT1>
class RC_HELP<UNIT1>::RC {
private:
	using Holder = typename EXTERN::Holder ;

private:
	PTR<Holder> mPointer ;

public:
	implicit RC () noexcept :
		delegate mPointer (NULL) {}

	template <class ARG1>
	static RC make (RREF<ARG1> that) {
		struct PHDEP ;
		using R1X = typeof (that) ;
		require (IS_SAME<UNIT1 ,R1X>) ;
		using R2X = typename DEPENDENT<RC_IMPLHOLDER_HELP<R1X> ,PHDEP>::ImplHolder ;
		RC ret ;
		ret.mPointer = new R2X (forward (that)) ;
		const auto r1x = property[ret.mPointer].increase () ;
		assert (r1x == 1) ;
		return forward (ret) ;
	}

	implicit ~RC () noexcept {
		if (mPointer == NULL)
			return ;
		if ifswitch (TRUE) {
			const auto r1x = mPointer->decrease () ;
			if (r1x > ZERO)
				discard ;
			mPointer->destroy () ;
		}
		mPointer = NULL ;
	}

	implicit RC (CREF<RC> that) :
		delegate RC () {
		if ifnot (that.exist ())
			return ;
		mPointer = that.mPointer ;
		const auto r1x = mPointer->increase () ;
		assert (r1x >= 2) ;
	}

	inline void operator= (CREF<RC> that) {
		auto &&thiz = property[this] ;
		if (property (thiz) == property (that))
			return ;
		recreate (thiz ,forward (that)) ;
	}

	implicit RC (RREF<RC> that) noexcept :
		delegate RC () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<RC> that) noexcept {
		auto &&thiz = property[this] ;
		if (property (thiz) == property (that))
			return ;
		recreate (thiz ,forward (that)) ;
	}

	BOOL exist () const {
		return mPointer != NULL ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	CREF<UNIT1> to () const leftvalue {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}

	inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
		return property (self) ;
	}
} ;

template <class UNIT1>
class RC_IMPLHOLDER_HELP<UNIT1>::ImplHolder :
	public Holder {
private:
	UNIT1 mValue ;
	LENGTH mCounter ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNIT1> that) :
		delegate mValue (forward (that)) ,mCounter (ZERO) {}

	void destroy () const override {
		auto &&thiz = property[this] ;
		delete property (thiz) ;
	}

	CREF<UNIT1> to () const leftvalue override {
		return mValue ;
	}

	LENGTH increase () override {
		return ++mCounter ;
	}

	LENGTH decrease () override {
		return --mCounter ;
	}
} ;
} ;

template <class UNIT1>
using RC = typename U::RC_HELP<UNIT1>::RC ;

namespace U {
template <class...>
trait ITERATOR_HELP ;

template <>
trait ITERATOR_HELP<> {
	class IteratorRange ;

	class Iterator ;
} ;

class ITERATOR_HELP<>::Iterator {
private:
	INDEX mCurr ;

public:
	implicit Iterator () = delete ;

	explicit Iterator (CREF<INDEX> curr) {
		mCurr = curr ;
	}

	inline BOOL operator!= (CREF<Iterator> that) const {
		return mCurr != that.mCurr ;
	}

	inline void operator++ () {
		mCurr = mCurr + 1 ;
	}

	inline CREF<INDEX> operator* () const leftvalue {
		return mCurr ;
	}
} ;

class ITERATOR_HELP<>::IteratorRange {
private:
	INDEX mBegin ;
	INDEX mEnd ;

public:
	implicit IteratorRange () = delete ;

	explicit IteratorRange (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = max (begin_ ,end_) ;
	}

	Iterator begin () const {
		return Iterator (mBegin) ;
	}

	Iterator end () const {
		return Iterator (mEnd) ;
	}
} ;
} ;

namespace U {
class FUNCTION_range {
private:
	using IteratorRange = typename U::ITERATOR_HELP<>::IteratorRange ;

public:
	inline IteratorRange operator() (CREF<INDEX> begin ,CREF<INDEX> end) const {
		return IteratorRange (begin ,end) ;
	}
} ;
} ;

static constexpr auto range = U::FUNCTION_range () ;

namespace U {
template <class...>
trait SLICE_HELP ;

template <class ARG1>
trait SLICE_HELP<ARG1 ,REQUIRE<IS_STR<ARG1>>> {
	require (IS_STR<ARG1>) ;

	struct EXTERN {
		interface Holder ;
	} ;

	class Slice ;
} ;

template <class UNIT1>
interface SLICE_HELP<UNIT1 ,REQUIRE<IS_STR<UNIT1>>>::EXTERN::Holder :
	public Interface {
	virtual LENGTH size () const = 0 ;
	virtual CREF<UNIT1> at (CREF<INDEX> index) const = 0 ;
} ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ARG1>
trait SLICE_IMPLHOLDER_HELP<ARG1> {
	class ImplHolder ;
} ;

template <class UNIT1>
class SLICE_HELP<UNIT1 ,REQUIRE<IS_STR<UNIT1>>>::Slice {
private:
	using Holder = typename EXTERN::Holder ;

private:
	RC<Box<Holder>> mPointer ;

public:
	implicit Slice () = default ;

	LENGTH size () const {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->size () ;
	}

	LENGTH addr () const {
		if (mPointer == NULL)
			return ZERO ;
		return address (mPointer.self->at[0]) ;
	}

	UNIT1 get (CREF<INDEX> index) const {
		assert (between (index ,0 ,size ())) ;
		return mPointer.self->at (index) ;
	}

	inline UNIT1 operator[] (CREF<INDEX> index) const {
		return get (index) ;
	}

	BOOL equal (CREF<Slice> that) const {
		if (size () != that.size ())
			return FALSE ;
		for (auto &&i : range (0 ,size ())) {
			if (mPointer.self->at (i) != that.mPointer.self->at (i))
				return FALSE ;
		}
		return TRUE ;
	}

	inline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Slice> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Slice> that) const {
		const auto r1x = min (size () ,that.size ()) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = operator_compr (mPointer.self->at (i) ,that.mPointer.self->at (i)) ;
			if (r2x != ZERO)
				return r2x ;
		}
		const auto r3x = operator_compr (size () ,that.size ()) ;
		if (r3x != ZERO)
			return r3x ;
		return ZERO ;
	}

	inline BOOL operator< (CREF<Slice> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Slice> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	FLAG hash () const {
		FLAG ret = hashcode () ;
		for (auto &&i : range (0 ,size ())) {
			const auto r1x = FLAG (mPointer.self->at (i)) ;
			ret = hashcode (ret ,r1x) ;
		}
		return forward (ret) ;
	}
} ;
} ;

template <class UNIT1>
using Slice = typename U::SLICE_HELP<UNIT1 ,void>::Slice ;

namespace U {
template <class...>
trait CLAZZ_HELP ;

template <>
trait CLAZZ_HELP<> {
	struct EXTERN {
		interface Holder ;
	} ;

	class Clazz ;
} ;

interface CLAZZ_HELP<>::EXTERN::Holder :
	public Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_mid () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
} ;

class CLAZZ_HELP<>::Clazz {
private:
	using Holder = typename EXTERN::Holder ;

private:
	RC<Box<Holder>> mPointer ;

public:
	implicit Clazz () = default ;

	BOOL equal (CREF<Clazz> that) const {
		if (type_mid () == that.type_mid ())
			return TRUE ;
		if (type_name () != that.type_name ())
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Clazz> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Clazz> that) const {
		return operator_compr (type_name () ,that.type_name ()) ;
	}

	inline BOOL operator< (CREF<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	FLAG hash () const {
		const auto r1x = type_name () ;
		return r1x.hash () ;
	}

	LENGTH type_size () const {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	LENGTH type_align () const {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	FLAG type_mid () const {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	Slice<STR> type_name () const {
		if (mPointer == NULL)
			return Slice<STR> () ;
		return mPointer.self->type_name () ;
	}
} ;
} ;

using Clazz = typename U::CLAZZ_HELP<>::Clazz ;

namespace U {
template <class...>
trait EXCEPTION_HELP ;

template <>
trait EXCEPTION_HELP<> {
	class Exception ;
} ;

class EXCEPTION_HELP<>::Exception {
private:
	Slice<STR> mWhat ;

public:
	implicit Exception () = default ;

	CREF<Slice<STR>> what () const leftvalue {
		return mWhat ;
	}

	void raise[[noreturn]] () {
		auto &&thiz = property[this] ;
		throw thiz ;
	}
} ;
} ;

using Exception = typename U::EXCEPTION_HELP<>::Exception ;
} ;