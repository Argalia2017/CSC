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

using SINGLE = float ;
using DOUBLE = double ;

static constexpr auto SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constexpr auto SINGLE_MIN = SINGLE (1.175494351E-38) ;
static constexpr auto SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constexpr auto SINGLE_INF = SINGLE (std::numeric_limits<float>::infinity ()) ;
static constexpr auto DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constexpr auto DOUBLE_MIN = DOUBLE (2.2250738585072014E-308) ;
static constexpr auto DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constexpr auto DOUBLE_INF = DOUBLE (std::numeric_limits<float>::infinity ()) ;

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

template <class...>
struct ENUMAS ;

template <VAR ARG1>
struct ENUMID {
	imports constexpr VAR compile () {
		return ARG1 ;
	}
} ;

template <class ARG1>
struct ENUMAS<ARG1> {
	static constexpr auto value = ARG1::compile () ;
} ;

template <class...>
struct TYPEAS ;

template <class>
struct TYPEID {} ;

template <class ARG1>
struct TYPEAS<ARG1> {
	static constexpr auto id = TYPEID<ARG1> () ;
} ;

template <class UNIT1>
using REMOVE_CVR = typename std::remove_cv<typename std::remove_reference<UNIT1>::type>::type ;

namespace U {
template <class...>
trait REMOVE_TYPEID_HELP ;

template <>
trait REMOVE_TYPEID_HELP<void> ;

template <class ARG1>
trait REMOVE_TYPEID_HELP<ARG1> {
	using RET = ARG1 ;
} ;

template <class ARG1>
trait REMOVE_TYPEID_HELP<TYPEID<ARG1>> {
	using RET = ARG1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_TYPEID = typename U::REMOVE_TYPEID_HELP<UNIT1>::RET ;

template <class UNIT1>
using REMOVE_ALL = REMOVE_CVR<REMOVE_TYPEID<REMOVE_CVR<UNIT1>>> ;

template <class UNIT1>
using DEF = UNIT1 ;

template <class UNIT1>
using UNSAFE_PTR = DEF<typename std::remove_reference<UNIT1>::type> * ;

template <class UNIT1>
using VREF = DEF<UNIT1 &> ;

template <class UNIT1>
using CREF = DEF<const UNIT1 &> ;

template <class UNIT1>
using RREF = DEF<UNIT1 &&> ;

using ENUM_USED = ENUMAS<ENUMID<(USED)>> ;
using ENUM_NONE = ENUMAS<ENUMID<(NONE)>> ;
using ENUM_ZERO = ENUMAS<ENUMID<(ZERO)>> ;
using ENUM_IDEN = ENUMAS<ENUMID<(IDEN)>> ;

using ENUM_0TH = ENUMAS<ENUMID<(0)>> ;
using ENUM_1ST = ENUMAS<ENUMID<(1)>> ;
using ENUM_2ND = ENUMAS<ENUMID<(2)>> ;
using ENUM_3RD = ENUMAS<ENUMID<(3)>> ;
using ENUM_4TH = ENUMAS<ENUMID<(4)>> ;
using ENUM_5TH = ENUMAS<ENUMID<(5)>> ;
using ENUM_6TH = ENUMAS<ENUMID<(6)>> ;
using ENUM_7TH = ENUMAS<ENUMID<(7)>> ;
using ENUM_8TH = ENUMAS<ENUMID<(8)>> ;
using ENUM_9TH = ENUMAS<ENUMID<(9)>> ;
using ENUM_XTH = ENUMAS<ENUMID<(10)>> ;

namespace U {
template <class...>
trait SIZEOF_HELP ;

template <class ARG1>
trait SIZEOF_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return sizeof (ARG1) ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using SIZEOF = typename U::SIZEOF_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait ALIGNOF_HELP ;

template <class ARG1>
trait ALIGNOF_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return alignof (ARG1) ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using ALIGNOF = typename U::ALIGNOF_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait COUNTOF_HELP ;

template <class...ARGS>
trait COUNTOF_HELP<TYPEAS<ARGS...>> {
	struct ID1 {
		imports constexpr VAR compile () {
			return sizeof... (ARGS) ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using COUNTOF = typename U::COUNTOF_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait IS_SAME_HELP ;

template <class ARG1>
trait IS_SAME_HELP<ARG1 ,ARG1> {
	using RET = ENUM_IDEN ;
} ;

template <class ARG1 ,class ARG2>
trait IS_SAME_HELP<ARG1 ,ARG2> {
	using RET = ENUM_ZERO ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using IS_SAME = typename U::IS_SAME_HELP<UNIT1 ,UNIT2>::RET ;

namespace U {
template <class...>
trait REQUIRE_HELP ;

template <>
trait REQUIRE_HELP<ENUM_IDEN> {
	using RET = void ;
} ;

template <class ARG1>
struct REQUIRE_ID {
	imports constexpr VAR compile () {
		return ARG1::value ;
	}
} ;
} ;

template <class UNIT1>
using REQUIRE = typename U::REQUIRE_HELP<ENUMAS<U::REQUIRE_ID<UNIT1>>>::RET ;

namespace U {
template <class...>
trait ENABLE_HELP ;

template <class ARG1 ,class ARG2>
trait ENABLE_HELP<ARG1 ,ARG2 ,REQUIRE<ARG1>> {
	using RET = ARG2 ;
} ;
} ;

template <class COND ,class YES = TYPEAS<>>
using ENABLE = typename U::ENABLE_HELP<COND ,YES ,void>::RET ;

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

namespace U {
template <class...>
trait ENUM_NOT_HELP ;

template <class ARG1>
trait ENUM_NOT_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ifnot (ENUM_CHECK<ARG1>::value) ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using ENUM_NOT = typename U::ENUM_NOT_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait CONDITIONAL_HELP ;

template <class ARG1 ,class ARG2 ,class ARG3>
trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3 ,REQUIRE<ARG1>> {
	using RET = ARG2 ;
} ;

template <class ARG1 ,class ARG2 ,class ARG3>
trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3 ,REQUIRE<ENUM_NOT<ARG1>>> {
	using RET = ARG3 ;
} ;
} ;

template <class COND ,class YES ,class NO>
using CONDITIONAL = typename U::CONDITIONAL_HELP<COND ,YES ,NO ,void>::RET ;

namespace U {
template <class...>
trait ENUM_EQUAL_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_EQUAL_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value == ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = typename U::ENUM_EQUAL_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<UNIT1 ,UNIT2>> ;

namespace U {
template <class...>
trait ENUM_COMPR_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_COMPR_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			if (ENUM_CHECK<ARG1>::value < ENUM_CHECK<ARG2>::value)
				return NONE ;
			if (ENUM_CHECK<ARG1>::value > ENUM_CHECK<ARG2>::value)
				return IDEN ;
			return ZERO ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = typename U::ENUM_COMPR_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = ENUM_EQUAL<ENUM_COMPR<UNIT1 ,UNIT2> ,ENUM_NONE> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GTEQ = ENUM_NOT<ENUM_COMPR_LT<UNIT1 ,UNIT2>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = ENUM_EQUAL<ENUM_COMPR<UNIT1 ,UNIT2> ,ENUM_IDEN> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LTEQ = ENUM_NOT<ENUM_COMPR_GT<UNIT1 ,UNIT2>> ;

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

namespace U {
template <class...>
trait ENUM_ADD_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_ADD_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value + ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;

template <class...>
trait ENUM_SUB_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_SUB_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value - ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;

template <class...>
trait ENUM_MUL_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_MUL_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value * ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;

template <class...>
trait ENUM_DIV_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_DIV_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value / ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;

template <class...>
trait ENUM_MOD_HELP ;

template <class ARG1 ,class ARG2>
trait ENUM_MOD_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return ENUM_CHECK<ARG1>::value % ENUM_CHECK<ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_ADD = typename U::ENUM_ADD_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = typename U::ENUM_SUB_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = typename U::ENUM_MUL_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = typename U::ENUM_DIV_HELP<UNIT1 ,UNIT2>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = typename U::ENUM_MOD_HELP<UNIT1 ,UNIT2>::RET ;

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
trait TYPE_FIRST_ONE_HELP<TYPEAS<ARG1 ,ARGS...>> {
	using RET = ARG1 ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_ONE = typename U::TYPE_FIRST_ONE_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait TYPE_FIRST_REST_HELP ;

template <class ARG1 ,class...ARGS>
trait TYPE_FIRST_REST_HELP<TYPEAS<ARG1 ,ARGS...>> {
	using RET = TYPEAS<ARGS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_REST = typename U::TYPE_FIRST_REST_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait TYPE_SECOND_ONE_HELP ;

template <class ARG1 ,class ARG2 ,class...ARGS>
trait TYPE_SECOND_ONE_HELP<TYPEAS<ARG1 ,ARG2 ,ARGS...>> {
	using RET = ARG2 ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_ONE = typename U::TYPE_SECOND_ONE_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait TYPE_SECOND_REST_HELP ;

template <class ARG1 ,class ARG2 ,class...ARGS>
trait TYPE_SECOND_REST_HELP<TYPEAS<ARG1 ,ARG2 ,ARGS...>> {
	using RET = TYPEAS<ARGS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_REST = typename U::TYPE_SECOND_REST_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait TYPE_UNWRAP_HELP ;

template <class ARG1>
trait TYPE_UNWRAP_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;
} ;

template <class UNIT1>
using TYPE_UNWRAP = typename U::TYPE_UNWRAP_HELP<UNIT1 ,void>::RET ;

namespace U {
template <class...>
trait TYPE_CAT_HELP ;

template <class...ARGS1 ,class...ARGS2>
trait TYPE_CAT_HELP<TYPEAS<ARGS1...> ,TYPEAS<ARGS2...>> {
	using RET = TYPEAS<ARGS1... ,ARGS2...> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using TYPE_CAT = typename U::TYPE_CAT_HELP<UNIT1 ,UNIT2>::RET ;

namespace U {
template <class...>
trait TYPE_REPEAT_HELP ;

template <class ARG1 ,class ARG2>
trait TYPE_REPEAT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQ_ZERO<ARG2>>> {
	using RET = TYPEAS<> ;
} ;

template <class ARG1 ,class ARG2>
trait TYPE_REPEAT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_GT_ZERO<ARG2>>> {
	using R1X = typename TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2> ,void>::RET ;

	using RET = TYPE_CAT<TYPEAS<ARG1> ,R1X> ;
} ;
} ;

template <class ITEM ,class SIZE>
using TYPE_REPEAT = typename U::TYPE_REPEAT_HELP<ITEM ,SIZE ,void>::RET ;

namespace U {
template <class...>
trait TYPE_REVERSE_HELP ;

template <class ARG1>
trait TYPE_REVERSE_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	using RET = TYPEAS<> ;
} ;

template <class ARG1>
trait TYPE_REVERSE_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_FIRST_REST<ARG1> ,void>::RET ;

	using RET = TYPE_CAT<R2X ,TYPEAS<R1X>> ;
} ;
} ;

template <class UNIT1>
using TYPE_REVERSE = typename U::TYPE_REVERSE_HELP<UNIT1 ,void>::RET ;

namespace U {
template <class...>
trait TYPE_PICK_HELP ;

template <class ARG1 ,class ARG2>
trait TYPE_PICK_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQ_ZERO<ARG2>>> {
	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait TYPE_PICK_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_GT_ZERO<ARG2>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<ARG1> ,ENUM_DEC<ARG2> ,void>::RET ;
} ;
} ;

template <class LIST ,class INDEX>
using TYPE_PICK = typename U::TYPE_PICK_HELP<LIST ,INDEX ,void>::RET ;

namespace U {
template <class...>
trait ENUM_ALL_HELP ;

template <class ARG1>
trait ENUM_ALL_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait ENUM_ALL_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1> ,void>::RET ;

	using RET = CONDITIONAL<R1X ,R2X ,ENUM_ZERO> ;
} ;
} ;

template <class...ARGS>
using ENUM_ALL = typename U::ENUM_ALL_HELP<TYPEAS<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait ENUM_ANY_HELP ;

template <class ARG1>
trait ENUM_ANY_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait ENUM_ANY_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<ARG1> ,void>::RET ;

	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R2X> ;
} ;
} ;

template <class...ARGS>
using ENUM_ANY = typename U::ENUM_ANY_HELP<TYPEAS<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait PLACEHOLDER_HELP ;

template <class ARG1>
trait PLACEHOLDER_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<ARG1>>> {
	struct PLACEHOLDER ;
} ;

template <class UNIT1>
struct PLACEHOLDER_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<UNIT1>>>::PLACEHOLDER {} ;

template <class ARG1>
trait PLACEHOLDER_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<ARG1>>> {
	using BASE = typename PLACEHOLDER_HELP<ENUM_DEC<ARG1> ,void>::PLACEHOLDER ;

	struct PLACEHOLDER ;
} ;

template <class UNIT1>
struct PLACEHOLDER_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<UNIT1>>>::PLACEHOLDER :public BASE {} ;
} ;

template <class RANK>
using PLACEHOLDER = typename U::PLACEHOLDER_HELP<RANK ,void>::PLACEHOLDER ;

static constexpr auto PH0 = PLACEHOLDER<ENUM_0TH> () ;
static constexpr auto PH1 = PLACEHOLDER<ENUM_1ST> () ;
static constexpr auto PH2 = PLACEHOLDER<ENUM_2ND> () ;
static constexpr auto PH3 = PLACEHOLDER<ENUM_3RD> () ;
static constexpr auto PH4 = PLACEHOLDER<ENUM_4TH> () ;
static constexpr auto PH5 = PLACEHOLDER<ENUM_5TH> () ;
static constexpr auto PH6 = PLACEHOLDER<ENUM_6TH> () ;
static constexpr auto PH7 = PLACEHOLDER<ENUM_7TH> () ;
static constexpr auto PH8 = PLACEHOLDER<ENUM_8TH> () ;
static constexpr auto PH9 = PLACEHOLDER<ENUM_9TH> () ;
static constexpr auto PHX = PLACEHOLDER<ENUM_XTH> () ;

template <class UNIT1>
using IS_BOOL = IS_SAME<UNIT1 ,BOOL> ;

template <class UNIT1>
using IS_VAR = ENUM_ANY<IS_SAME<UNIT1 ,VAR32> ,IS_SAME<UNIT1 ,VAR64>> ;

template <class UNIT1>
using IS_REAL = ENUM_ANY<IS_SAME<UNIT1 ,SINGLE> ,IS_SAME<UNIT1 ,DOUBLE>> ;

template <class UNIT1>
using IS_BYTE = ENUM_ANY<IS_SAME<UNIT1 ,BYTE> ,IS_SAME<UNIT1 ,WORD> ,IS_SAME<UNIT1 ,CHAR> ,IS_SAME<UNIT1 ,FEAT>> ;

template <class UNIT1>
using IS_STR = ENUM_ANY<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT1 ,STRU32>> ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,typeof (NULL)> ;

template <class UNIT1>
using IS_BASIC = ENUM_ANY<IS_BOOL<UNIT1> ,IS_VAR<UNIT1> ,IS_REAL<UNIT1> ,IS_BYTE<UNIT1> ,IS_STR<UNIT1> ,IS_NULL<UNIT1>> ;

namespace U {
template <class...>
trait IS_FUNCTION_HELP ;

template <class ARG1>
trait IS_FUNCTION_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_function<ARG1>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using IS_FUNCTION = typename U::IS_FUNCTION_HELP<UNIT1>::RET ;

template <class UNIT1>
using IS_NOEXCPET = ENUM_ZERO ;

namespace U {
template <class...>
trait IS_CLASS_HELP ;

template <class ARG1>
trait IS_CLASS_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_class<ARG1>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using IS_CLASS = typename U::IS_CLASS_HELP<UNIT1>::RET ;

template <class UNIT1>
using IS_STRUCT = IS_CLASS<UNIT1> ;

namespace U {
template <class...>
trait IS_TRIVIAL_HELP ;

template <class ARG1>
trait IS_TRIVIAL_HELP<ARG1> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_trivial<ARG1>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1>
using IS_TRIVIAL = typename U::IS_TRIVIAL_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait IS_CONSTRUCTIBLE_HELP ;

template <class ARG1 ,class...ARGS>
trait IS_CONSTRUCTIBLE_HELP<ARG1 ,TYPEAS<ARGS...>> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_constructible<ARG1 ,ARGS...>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class UNIT1 ,class PARAMS>
using IS_CONSTRUCTIBLE = typename U::IS_CONSTRUCTIBLE_HELP<UNIT1 ,PARAMS>::RET ;

namespace U {
template <class...>
trait IS_CLONEABLE_HELP ;

template <class ARG1 ,class ARG2>
trait IS_CLONEABLE_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_copy_constructible<ARG1>::value ;
		}
	} ;

	struct ID2 {
		imports constexpr VAR compile () {
			return std::is_copy_assignable<ARG1>::value ;
		}
	} ;

	using RET = ENUM_ALL<ENUMAS<ID1> ,ENUMAS<ID2>> ;
} ;
} ;

template <class UNIT1>
using IS_CLONEABLE = typename U::IS_CLONEABLE_HELP<UNIT1>::RET ;

class Interface {
public:
	implicit Interface () = default ;
	implicit virtual ~Interface () noexcept = default ;
	implicit Interface (CREF<Interface>) = delete ;
	inline void operator= (CREF<Interface>) = delete ;
	implicit Interface (RREF<Interface>) = delete ;
	inline void operator= (RREF<Interface>) = delete ;
} ;

namespace U {
template <class...>
trait IS_INTERFACE_HELP ;

template <class ARG1 ,class ARG2>
trait IS_INTERFACE_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_base_of<ARG2 ,ARG1>::value ;
		}
	} ;

	struct ID2 {
		imports constexpr VAR compile () {
			return std::is_abstract<ARG1>::value ;
		}
	} ;

	using RET = ENUM_ALL<ENUMAS<ID1> ,ENUMAS<ID2>> ;
} ;
} ;

template <class UNIT1>
using IS_INTERFACE = typename U::IS_INTERFACE_HELP<UNIT1 ,Interface>::RET ;

namespace U {
template <class...>
trait IS_EXTEND_HELP ;

template <class ARG1 ,class ARG2>
trait IS_EXTEND_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_base_of<ARG1 ,ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class BASE ,class DERIVED>
using IS_EXTEND = typename U::IS_EXTEND_HELP<BASE ,DERIVED>::RET ;

namespace U {
template <class...>
trait IS_CONVERTIBLE_HELP ;

template <class ARG1 ,class ARG2>
trait IS_CONVERTIBLE_HELP<ARG1 ,ARG2> {
	struct ID1 {
		imports constexpr VAR compile () {
			return std::is_convertible<ARG1 ,ARG2>::value ;
		}
	} ;

	using RET = ENUMAS<ID1> ;
} ;
} ;

template <class FROM ,class TO>
using IS_CONVERTIBLE = typename U::IS_CONVERTIBLE_HELP<FROM ,TO>::RET ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,typeof (PH0)> ;

namespace U {
template <class...>
trait IS_ALL_SAME_HELP ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
trait IS_ALL_SAME_HELP<ARG1 ,REQUIRE<ENUM_GT_IDEN<COUNTOF<ARG1>>>> {
	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ALL_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>> ,void>::RET ;

	using RET = ENUM_ALL<IS_SAME<R1X ,R2X> ,R3X> ;
} ;
} ;

template <class...ARGS>
using IS_ALL_SAME = typename U::IS_ALL_SAME_HELP<TYPEAS<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait IS_ANY_SAME_HELP ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_EQ_IDEN<COUNTOF<ARG1>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
trait IS_ANY_SAME_HELP<ARG1 ,REQUIRE<ENUM_GT_IDEN<COUNTOF<ARG1>>>> {
	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ANY_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>> ,void>::RET ;
	using R4X = typename IS_ANY_SAME_HELP<TYPE_CAT<R2X ,TYPE_SECOND_REST<ARG1>> ,void>::RET ;

	using RET = ENUM_ANY<IS_SAME<R1X ,R2X> ,R3X ,R4X> ;
} ;
} ;

template <class...ARGS>
using IS_ANY_SAME = typename U::IS_ANY_SAME_HELP<TYPEAS<ARGS...> ,void>::RET ;

namespace U {
template <class...>
trait BYTE_TRAIT_HELP ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<BYTE>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<BYTE>>>> {
	using RET = BYTE ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<WORD>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<WORD>>>> {
	using RET = WORD ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<CHAR>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<CHAR>>>> {
	using RET = CHAR ;
} ;

template <class ARG1 ,class ARG2>
trait BYTE_TRAIT_HELP<ARG1 ,ARG2 ,REQUIRE<ENUM_EQUAL<ARG1 ,SIZEOF<FEAT>>> ,REQUIRE<ENUM_EQUAL<ARG2 ,ALIGNOF<FEAT>>>> {
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
trait CAST_TRAIT_HELP<ARG1 ,VREF<ARG2>> {
	using RET = VREF<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,CREF<ARG2>> {
	using RET = CREF<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
trait CAST_TRAIT_HELP<ARG1 ,RREF<ARG2>> {
	using RET = RREF<ARG1> ;
} ;
} ;

template <class TO ,class FROM>
using CAST_TRAIT = typename U::CAST_TRAIT_HELP<TO ,FROM>::RET ;

namespace U {
template <class...>
trait STORAGE_HELP ;

template <class ARG1 ,class ARG2>
trait STORAGE_HELP<ARG1 ,ARG2> {
	using ITEM = BYTE_TRAIT<ARG2 ,ARG2> ;
	using SIZE = ENUM_DIV<ENUM_DEC<ENUM_ADD<ARG1 ,SIZEOF<ITEM>>> ,SIZEOF<ITEM>> ;

	struct STORAGE {
		DEF<ITEM[SIZE::value]> mUnused ;
	} ;
} ;
} ;

template <class SIZE ,class ALIGN = ENUM_IDEN>
using STORAGE = typename U::STORAGE_HELP<SIZE ,ALIGN>::STORAGE ;

namespace U {
template <class...>
trait PACK_HELP ;

template <class ARG1>
trait PACK_HELP<ARG1 ,REQUIRE<ENUM_EQUAL<COUNTOF<ARG1> ,ENUM_0TH>>> {
	struct PACK ;
} ;

template <class UNIT1>
struct PACK_HELP<UNIT1 ,REQUIRE<ENUM_EQUAL<COUNTOF<UNIT1> ,ENUM_0TH>>>::PACK {} ;

template <class ARG1>
trait PACK_HELP<ARG1 ,REQUIRE<ENUM_EQUAL<COUNTOF<ARG1> ,ENUM_1ST>>> {
	using FIRST = TYPE_PICK<ARG1 ,ENUM_0TH> ;

	struct PACK ;
} ;

template <class UNIT1>
struct PACK_HELP<UNIT1 ,REQUIRE<ENUM_EQUAL<COUNTOF<UNIT1> ,ENUM_1ST>>>::PACK {
	FIRST mP1 ;
} ;

template <class ARG1>
trait PACK_HELP<ARG1 ,REQUIRE<ENUM_EQUAL<COUNTOF<ARG1> ,ENUM_2ND>>> {
	using FIRST = TYPE_PICK<ARG1 ,ENUM_0TH> ;
	using SECOND = TYPE_PICK<ARG1 ,ENUM_1ST> ;

	struct PACK ;
} ;

template <class UNIT1>
struct PACK_HELP<UNIT1 ,REQUIRE<ENUM_EQUAL<COUNTOF<UNIT1> ,ENUM_2ND>>>::PACK {
	FIRST mP1 ;
	SECOND mP2 ;
} ;

template <class ARG1>
trait PACK_HELP<ARG1 ,REQUIRE<ENUM_EQUAL<COUNTOF<ARG1> ,ENUM_3RD>>> {
	using FIRST = TYPE_PICK<ARG1 ,ENUM_0TH> ;
	using SECOND = TYPE_PICK<ARG1 ,ENUM_1ST> ;
	using THIRD = TYPE_PICK<ARG1 ,ENUM_2ND> ;

	struct PACK ;
} ;

template <class UNIT1>
struct PACK_HELP<UNIT1 ,REQUIRE<ENUM_EQUAL<COUNTOF<UNIT1> ,ENUM_3RD>>>::PACK {
	FIRST mP1 ;
	SECOND mP2 ;
	THIRD mP3 ;
} ;
} ;

template <class...UNITS>
using PACK = typename U::PACK_HELP<TYPEAS<UNITS...> ,void>::PACK ;

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
	inline void operator()[[noreturn]] () const {
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
	inline void operator()[[noreturn]] () const {
		std::terminate () ;
	}
} ;
#endif
#endif

#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_CLANG__
class FUNCTION_abort {
public:
	inline void operator()[[noreturn]] () const {
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
	inline auto operator() (CREF<ARG1> arg1) const
		->LENGTH {
		return LENGTH (&(reinterpret_cast<CREF<BOOL>> (arg1))) ;
	}
} ;
} ;

static constexpr auto address = U::FUNCTION_address () ;

namespace U {
class FUNCTION_bitwise {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> arg1) const noexcept
		->BYTE_TRAIT<SIZEOF<ARG1> ,ALIGNOF<ARG1>> {
		using R1X = typeof (arg1) ;
		using R2X = BYTE_TRAIT<SIZEOF<R1X> ,ALIGNOF<R1X>> ;
		return reinterpret_cast<CREF<R2X>> (arg1) ;
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

static constexpr auto unsafe_barrier = U::FUNCTION_barrier () ;

namespace U {
template <class...>
trait TEMP_HELP ;

template <class UNIT1>
struct TEMPAS {
	STORAGE<SIZEOF<UNIT1> ,ALIGNOF<UNIT1>> mStorage ;
} ;

template <class ARG1>
trait TEMP_HELP<ARG1> {
	using RET = TEMPAS<REMOVE_ALL<ARG1>> ;
} ;
} ;

template <class UNIT1>
using TEMP = typename U::TEMP_HELP<UNIT1>::RET ;

namespace U {
template <class...>
trait REMOVE_TEMP_HELP ;

template <class ARG1>
trait REMOVE_TEMP_HELP<ARG1> {
	using RET = ARG1 ;
} ;

template <class ARG1>
trait REMOVE_TEMP_HELP<TEMPAS<ARG1>> {
	using RET = ARG1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_TEMP = typename U::REMOVE_TEMP_HELP<UNIT1>::RET ;

namespace U {
class FUNCTION_property {
public:
	template <class ARG1>
	inline auto operator[] (CREF<DEF<ARG1 *>> arg1) const noexcept
		->VREF<ARG1> {
		return (*arg1) ;
	}
} ;
} ;

static constexpr auto property = U::FUNCTION_property () ;

namespace U {
class FUNCTION_pointer {
public:
	template <class ARG1>
	inline auto operator[] (VREF<ARG1> arg1) const noexcept
		->UNSAFE_PTR<ARG1> {
		return (&arg1) ;
	}
} ;
} ;

static constexpr auto unsafe_pointer = U::FUNCTION_pointer () ;

namespace U {
class FUNCTION_deref {
public:
	template <class ARG1>
	inline auto operator[] (VREF<ARG1> tmp) const noexcept
		->VREF<REMOVE_TEMP<REMOVE_ALL<ARG1>>> {
		using R1X = typeof (tmp) ;
		using R2X = REMOVE_TEMP<R1X> ;
		require (IS_SAME<R1X ,TEMP<R2X>>) ;
		return reinterpret_cast<VREF<R2X>> (tmp) ;
	}
} ;
} ;

static constexpr auto unsafe_deref = U::FUNCTION_deref () ;

namespace U {
class FUNCTION_deptr {
public:
	template <class ARG1>
	inline auto operator[] (VREF<ARG1> tmp) const noexcept
		->VREF<TEMP<ARG1>> {
		using R1X = typeof (tmp) ;
		return reinterpret_cast<VREF<TEMP<R1X>>> (tmp) ;
	}
} ;
} ;

static constexpr auto unsafe_deptr = U::FUNCTION_deptr () ;

namespace U {
class FUNCTION_swap {
public:
	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> arg1 ,VREF<ARG2> arg2) const noexcept {
		using R1X = typeof (arg1) ;
		using R2X = typeof (arg2) ;
		require (IS_SAME<R1X ,R2X>) ;
		auto rax = unsafe_deptr[arg1] ;
		unsafe_deptr[arg1] = unsafe_deptr[arg2] ;
		unsafe_deptr[arg2] = rax ;
		unsafe_barrier () ;
	}
} ;
} ;

static constexpr auto swap = U::FUNCTION_swap () ;

namespace U {
class FUNCTION_forward {
public:
	template <class ARG1>
	inline auto operator[] (CREF<ARG1> arg1) const
		->CREF<REMOVE_ALL<ARG1>> {
		using R1X = typeof (arg1) ;
		return static_cast<CREF<R1X>> (arg1) ;
	}

	template <class ARG1>
	inline auto operator[] (VREF<ARG1> arg1) const
		->RREF<REMOVE_ALL<ARG1>> {
		using R1X = typeof (arg1) ;
		return static_cast<RREF<R1X>> (arg1) ;
	}
} ;
} ;

static constexpr auto forward = U::FUNCTION_forward () ;

namespace U {
class FUNCTION_create {
public:
	template <class ARG1 ,class...ARGS>
	inline void operator() (VREF<ARG1> tmp ,RREF<ARGS>...args) const {
		using R1X = typeof (tmp) ;
		using R2X = REMOVE_TEMP<R1X> ;
		require (IS_SAME<R1X ,TEMP<R2X>>) ;
		new (unsafe_pointer[tmp]) R2X (forward[args]...) ;
	}
} ;
} ;

static constexpr auto unsafe_create = U::FUNCTION_create () ;

namespace U {
class FUNCTION_recreate {
public:
	template <class ARG1>
	inline void operator() (VREF<ARG1> tmp ,RREF<ARG1> obj) const noexcept {
		using R1X = typeof (tmp) ;
		tmp.~R1X () ;
		new (unsafe_pointer[tmp]) R1X (forward[obj]) ;
	}
} ;
} ;

static constexpr auto unsafe_recreate = U::FUNCTION_recreate () ;

namespace U {
class FUNCTION_destroy {
public:
	template <class ARG1>
	inline void operator() (VREF<ARG1> tmp) const noexcept {
		using R1X = typeof (tmp) ;
		using R2X = REMOVE_TEMP<R1X> ;
		require (IS_SAME<R1X ,TEMP<R2X>>) ;
		unsafe_deref[tmp].~R2X () ;
	}
} ;
} ;

static constexpr auto unsafe_destroy = U::FUNCTION_destroy () ;

namespace U {
class FUNCTION_zeroize {
public:
	template <class ARG1>
	inline void operator() (VREF<ARG1> arg1) const noexcept {
		using R1X = typeof (arg1) ;
		require (IS_TRIVIAL<R1X>) ;
		unsafe_deptr[arg1] = {0} ;
		unsafe_barrier () ;
	}
} ;
} ;

static constexpr auto unsafe_zeroize = U::FUNCTION_zeroize () ;

namespace U {
class FUNCTION_alignax {
public:
	inline auto operator() (CREF<LENGTH> base ,CREF<LENGTH> align) const
		->LENGTH {
		const auto r1x = align - base % align ;
		return base + r1x % align ;
	}
} ;
} ;

static constexpr auto alignax = U::FUNCTION_alignax () ;

namespace U {
class FUNCTION_between {
public:
	inline auto operator() (CREF<INDEX> index ,CREF<INDEX> begin ,CREF<INDEX> end) const
		->BOOL {
		if ifnot (index >= begin)
			return FALSE ;
		if ifnot (index < end)
			return FALSE ;
		return TRUE ;
	}
} ;
} ;

static constexpr auto between = U::FUNCTION_between () ;

namespace U {
class FUNCTION_abs {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> arg1) const
		->REMOVE_ALL<ARG1> {
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
	inline auto operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const
		->REMOVE_ALL<ARG1> {
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
	inline auto operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const
		->REMOVE_ALL<ARG1> {
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
	class FUNCTION_compr ;
} ;

template <class UNIT1>
class FUNCTION_COMPR_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>>::FUNCTION_compr {
public:
	inline auto operator() (CREF<UNIT1> arg1 ,CREF<UNIT1> arg2) const
		->FLAG {
		if (arg1 < arg2)
			return FLAG (-1) ;
		if (arg2 < arg1)
			return FLAG (+1) ;
		return ZERO ;
	}
} ;

template <class ARG1>
trait FUNCTION_COMPR_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>> {
	class FUNCTION_compr ;
} ;

template <class UNIT1>
class FUNCTION_COMPR_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>>::FUNCTION_compr {
public:
	inline auto operator() (CREF<UNIT1> arg1 ,CREF<UNIT1> arg2) const
		->FLAG {
		return arg1.compr (arg2) ;
	}
} ;

class FUNCTION_compr {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> arg1 ,CREF<ARG1> arg2) const
		->FLAG {
		using R1X = typeof (arg1) ;
		using R2X = typename FUNCTION_COMPR_HELP<R1X ,void>::FUNCTION_compr ;
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
	class FUNCTION_hash ;
} ;

template <class UNIT1>
class FUNCTION_HASH_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>>::FUNCTION_hash {
public:
	inline auto operator() (CREF<UNIT1> arg1) const
		->FLAG {
		return FLAG (bitwise (arg1)) ;
	}
} ;

template <class ARG1>
trait FUNCTION_HASH_HELP<ARG1 ,REQUIRE<IS_CLASS<ARG1>>> {
	class FUNCTION_hash ;
} ;

template <class UNIT1>
class FUNCTION_HASH_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>>::FUNCTION_hash {
public:
	inline auto operator() (CREF<UNIT1> arg1) const
		->FLAG {
		return arg1.hash () ;
	}
} ;

class FUNCTION_hash {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> arg1) const
		->FLAG {
		using R1X = typeof (arg1) ;
		using R2X = typename FUNCTION_HASH_HELP<R1X ,void>::FUNCTION_hash ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (arg1) ;
	}
} ;
} ;

static constexpr auto operator_hash = U::FUNCTION_hash () ;

namespace U {
class FUNCTION_hashcode {
public:
	inline auto operator() () const
		->FLAG {
		const auto r1x = VAR64 (-3750763034362895579) ;
		const auto r2x = VAR64 (FEAT (r1x) & FEAT (VAR64_MAX)) ;
		return FLAG (r2x) ;
	}

	inline auto operator() (CREF<FLAG> arg1 ,CREF<FLAG> arg2) const
		->FLAG {
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
trait CABI_HELP ;

template <class ARG1>
trait CABI_HELP<ARG1> {
	struct CABI ;
} ;

template <class UNIT1>
struct CABI_HELP<UNIT1>::CABI :private Interface {} ;

class FUNCTION_cabi {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> nid) const
		->FLAG {
		using R1X = typeof (nid) ;
		using R2X = typename CABI_HELP<R1X>::CABI ;
		require (ENUM_EQUAL<SIZEOF<VAR> ,SIZEOF<R2X>>) ;
		require (ENUM_EQUAL<ALIGNOF<VAR> ,ALIGNOF<R2X>>) ;
		return FLAG (bitwise (R2X ())) ;
	}
} ;
} ;

static constexpr auto operator_cabi = U::FUNCTION_cabi () ;

namespace U {
template <class...>
trait ANY_HELP ;

template <>
trait ANY_HELP<> {
	class Any ;

	struct DETAIL {
		struct Holder ;
		struct NULLOPT ;

		struct BLOCK ;
		struct SLOT ;
		class AnyHeap ;
	} ;
} ;

struct ANY_HELP<>::DETAIL::Holder :public Interface {
	virtual void destroy () = 0 ;
	virtual LENGTH unsafe_addr () = 0 ;
	virtual FLAG type_cabi () const = 0 ;
} ;

struct ANY_HELP<>::DETAIL::NULLOPT {} ;

template <class...>
trait ANY_IMPLHOLDER_HELP ;

template <class ARG1>
trait ANY_IMPLHOLDER_HELP<ARG1> {
	using DETAIL = typename ANY_HELP<>::DETAIL ;
	using Holder = typename DETAIL::Holder ;

	class ImplHolder ;

	struct EXTERN ;
} ;

template <class UNIT1>
struct ANY_IMPLHOLDER_HELP<UNIT1>::EXTERN {
	imports UNSAFE_PTR<Holder> create (RREF<UNIT1> that) ;
} ;

class ANY_HELP<>::Any {
private:
	using Holder = typename DETAIL::Holder ;
	using NULLOPT = typename DETAIL::NULLOPT ;

private:
	UNSAFE_PTR<Holder> mPointer ;

public:
	implicit Any () = delete ;

	template <class ARG1 ,class = ENABLE<ENUM_NOT<ENUM_ANY<IS_SAME<REMOVE_ALL<ARG1> ,Any> ,IS_SAME<REMOVE_ALL<ARG1> ,NULLOPT>>>>>
	explicit Any (RREF<ARG1> that) :Any (NULLOPT ()) {
		using R1X = typeof (that) ;
		using R2X = typename ANY_IMPLHOLDER_HELP<R1X>::EXTERN ;
		mPointer = R2X::create (forward[that]) ;
	}

	implicit ~Any () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->destroy () ;
		mPointer = NULL ;
	}

	implicit Any (CREF<Any>) = delete ;

	inline void operator= (CREF<Any>) = delete ;

	implicit Any (RREF<Any> that) noexcept :Any (NULLOPT ()) {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Any>) = delete ;

	template <class ARG1>
	auto poll (CREF<ARG1> nid)
		->REMOVE_ALL<ARG1> {
		using R1X = typeof (nid) ;
		using R2X = typename ANY_IMPLHOLDER_HELP<R1X>::ImplHolder ;
		assert (mPointer != NULL) ;
		const auto r1x = operator_cabi (TYPEAS<R2X>::id) ;
		const auto r2x = mPointer->type_cabi () ;
		assert (r1x == r2x) ;
		const auto r3x = mPointer->unsafe_addr () ;
		const auto r4x = reinterpret_cast<UNSAFE_PTR<R1X>> (r3x) ;
		return forward[property[r4x]] ;
	}

	template <class ARG1>
	inline explicit operator ARG1 () rightvalue {
		using R1X = typeof (ARG1 ()) ;
		return poll (TYPEAS<R1X>::id) ;
	}

private:
	explicit Any (CREF<NULLOPT>) noexcept :mPointer (NULL) {}
} ;
} ;

using Any = typename U::ANY_HELP<>::Any ;

namespace U {
class FUNCTION_bad {
public:
	template <class ARG1>
	inline auto operator() (CREF<ARG1> nid) const
		->REMOVE_ALL<ARG1> {
		using R1X = typeof (nid) ;
		assert (FALSE) ;
		return R1X (Any (ZERO)) ;
	}
} ;
} ;

static constexpr auto bad = U::FUNCTION_bad () ;

namespace U {
template <class...>
trait BOX_HELP ;

template <class ARG1>
trait BOX_HELP<ARG1> {
	struct DETAIL {
		struct Holder ;
	} ;
} ;

template <class ARG1>
trait BOX_HELP<ARG1 ,REQUIRE<IS_INTERFACE<ARG1>>> {
	using DETAIL = typename BOX_HELP<ARG1>::DETAIL ;

	class Box ;
} ;

template <class UNIT1>
struct BOX_HELP<UNIT1>::DETAIL::Holder :public Interface {
	virtual void destroy () = 0 ;
	virtual VREF<UNIT1> to () leftvalue = 0 ;
	virtual CREF<UNIT1> to () const leftvalue = 0 ;
} ;

template <class...>
trait BOX_IMPLHOLDER_HELP ;

template <class ARG1 ,class ARG2>
trait BOX_IMPLHOLDER_HELP<ARG1 ,ARG2> {
	using DETAIL = typename BOX_HELP<ARG1>::DETAIL ;
	using Holder = typename DETAIL::Holder ;

	class ImplHolder ;

	struct EXTERN ;
} ;

template <class UNIT1 ,class UNIT2>
struct BOX_IMPLHOLDER_HELP<UNIT1 ,UNIT2>::EXTERN {
	imports UNSAFE_PTR<Holder> create (RREF<UNIT2> that) ;
} ;

template <class UNIT1>
class BOX_HELP<UNIT1 ,REQUIRE<IS_INTERFACE<UNIT1>>>::Box {
private:
	using Holder = typename DETAIL::Holder ;

private:
	UNSAFE_PTR<Holder> mPointer ;

public:
	implicit Box () noexcept :mPointer (NULL) {}

	template <class ARG1>
	imports auto make (RREF<ARG1> that)
		->Box {
		using R1X = typeof (that) ;
		require (IS_EXTEND<UNIT1 ,R1X>) ;
		using R2X = typename BOX_IMPLHOLDER_HELP<UNIT1 ,R1X>::EXTERN ;
		Box ret ;
		ret.mPointer = R2X::create (forward[that]) ;
		return forward[ret] ;
	}

	implicit ~Box () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->destroy () ;
		mPointer = NULL ;
	}

	implicit Box (CREF<Box>) = delete ;

	inline void operator= (CREF<Box>) = delete ;

	implicit Box (RREF<Box> that) noexcept :Box () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Box> that) noexcept {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	auto exist () const
		->BOOL {
		return mPointer != NULL ;
	}

	inline auto operator== (CREF<typeof (NULL)>) const
		->BOOL {
		return ifnot (exist ()) ;
	}

	inline auto operator!= (CREF<typeof (NULL)>) const
		->BOOL {
		return exist () ;
	}

	auto to () leftvalue
		->VREF<UNIT1> {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator VREF<UNIT1> () leftvalue {
		return self ;
	}

	inline auto operator-> () leftvalue
		->UNSAFE_PTR<VREF<UNIT1>> {
		return unsafe_pointer[self] ;
	}

	auto to () const leftvalue
		->CREF<UNIT1> {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}

	inline auto operator-> () const leftvalue
		->UNSAFE_PTR<CREF<UNIT1>> {
		return unsafe_pointer[self] ;
	}
} ;

template <class ARG1>
trait BOX_HELP<ARG1 ,REQUIRE<ENUM_NOT<IS_INTERFACE<ARG1>>>> {
	using DETAIL = typename BOX_HELP<ARG1>::DETAIL ;

	class Box ;
} ;

template <class UNIT1>
class BOX_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_INTERFACE<UNIT1>>>>::Box {
private:
	using Holder = typename DETAIL::Holder ;

private:
	UNSAFE_PTR<Holder> mPointer ;

public:
	implicit Box () noexcept :mPointer (NULL) {}

	template <class...ARGS>
	imports auto make (RREF<ARGS>...that)
		->Box {
		using R2X = typename BOX_IMPLHOLDER_HELP<UNIT1 ,UNIT1>::EXTERN ;
		Box ret ;
		ret.mPointer = R2X::create (UNIT1 (forward[that]...)) ;
		return forward[ret] ;
	}

	implicit ~Box () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->destroy () ;
		mPointer = NULL ;
	}

	implicit Box (CREF<Box>) = delete ;

	inline void operator= (CREF<Box>) = delete ;

	implicit Box (RREF<Box> that) noexcept :Box () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Box> that) noexcept {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	auto exist () const
		->BOOL {
		return mPointer != NULL ;
	}

	inline auto operator== (CREF<typeof (NULL)>) const
		->BOOL {
		return ifnot (exist ()) ;
	}

	inline auto operator!= (CREF<typeof (NULL)>) const
		->BOOL {
		return exist () ;
	}

	auto to () leftvalue
		->VREF<UNIT1> {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator VREF<UNIT1> () leftvalue {
		return self ;
	}

	inline auto operator-> () leftvalue
		->UNSAFE_PTR<VREF<UNIT1>> {
		return unsafe_pointer (self) ;
	}

	auto to () const leftvalue
		->CREF<UNIT1> {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}

	inline auto operator-> () const leftvalue
		->UNSAFE_PTR<CREF<UNIT1>> {
		return unsafe_pointer (self) ;
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
	class Cell ;
} ;

template <class UNIT1>
class CELL_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>>::Cell {
private:
	mutable TEMP<UNIT1> mValue ;
	BOOL mExist ;

public:
	implicit Cell () noexcept :mExist (FALSE) {}

	template <class...ARGS>
	imports auto make (RREF<ARGS>...that)
		->Cell {
		Cell ret ;
		unsafe_create (ret.mValue ,UNIT1 (forward[that]...)) ;
		ret.mExist = TRUE ;
		return forward[ret] ;
	}

	implicit ~Cell () noexcept {
		if ifnot (mExist)
			return ;
		unsafe_destroy (mValue) ;
		mExist = FALSE ;
	}

	implicit Cell (CREF<Cell> that) :Cell () {
		auto &&thiz = property[this] ;
		if ifnot (that.exist ())
			return ;
		thiz = Cell::make (that.fetch ()) ;
	}

	inline void operator= (CREF<Cell> that) {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	implicit Cell (RREF<Cell> that) noexcept :Cell () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<Cell> that) noexcept {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	auto exist () const
		->BOOL {
		return mExist ;
	}

	inline auto operator== (CREF<typeof (NULL)>) const
		->BOOL {
		return ifnot (exist ()) ;
	}

	inline auto operator!= (CREF<typeof (NULL)>) const
		->BOOL {
		return exist () ;
	}

	auto fetch () const
		->UNIT1 {
		assert (exist ()) ;
		return m_fake () ;
	}

	auto value (RREF<UNIT1> obj) const
		->UNIT1 {
		if ifnot (exist ())
			return forward[obj] ;
		return m_fake () ;
	}

	void store (RREF<UNIT1> obj) const {
		assert (exist ()) ;
		m_fake () = forward[obj] ;
	}

	auto exchange (RREF<UNIT1> obj) const
		->UNIT1 {
		assert (exist ()) ;
		UNIT1 ret = m_fake () ;
		m_fake () = forward[obj] ;
		return forward[ret] ;
	}

	auto change (VREF<UNIT1> expect ,RREF<UNIT1> obj) const
		->BOOL {
		assert (exist ()) ;
		if ifswitch (TRUE) {
			if (m_fake () == expect)
				discard ;
			expect = m_fake () ;
			return FALSE ;
		}
		m_fake () = forward[obj] ;
		return TRUE ;
	}

private:
	auto m_fake () const leftvalue
		->VREF<UNIT1> {
		return unsafe_deref[mValue] ;
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
	struct DETAIL {
		struct Holder ;
	} ;

	class RC ;
} ;

template <class UNIT1>
struct RC_HELP<UNIT1>::DETAIL::Holder :public Interface {
	virtual void destroy () = 0 ;
	virtual CREF<UNIT1> to () const leftvalue = 0 ;
	virtual LENGTH increase () = 0 ;
	virtual LENGTH decrease () = 0 ;
} ;

template <class...>
trait RC_IMPLHOLDER_HELP ;

template <class ARG1>
trait RC_IMPLHOLDER_HELP<ARG1> {
	using DETAIL = typename RC_HELP<ARG1>::DETAIL ;
	using Holder = typename DETAIL::Holder ;

	class ImplHolder ;

	struct EXTERN ;
} ;

template <class UNIT1>
struct RC_IMPLHOLDER_HELP<UNIT1>::EXTERN {
	imports UNSAFE_PTR<Holder> create (RREF<UNIT1> that) ;
} ;

template <class UNIT1>
class RC_HELP<UNIT1>::RC {
private:
	using Holder = typename DETAIL::Holder ;

private:
	UNSAFE_PTR<Holder> mPointer ;

public:
	implicit RC () noexcept :mPointer (NULL) {}

	template <class...ARGS>
	imports auto make (RREF<ARGS>...that)
		->RC {
		using R1X = typeof (that) ;
		require (IS_SAME<UNIT1 ,R1X>) ;
		using R2X = typename RC_IMPLHOLDER_HELP<R1X>::EXTERN ;
		RC ret ;
		ret.mPointer = R2X::create (UNIT1 (forward[that]...)) ;
		const auto r1x = property[ret.mPointer].increase () ;
		assert (r1x == 1) ;
		return forward[ret] ;
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

	implicit RC (CREF<RC> that) :RC () {
		if ifnot (that.exist ())
			return ;
		mPointer = that.mPointer ;
		const auto r1x = mPointer->increase () ;
		assert (r1x >= 2) ;
	}

	inline void operator= (CREF<RC> that) {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	implicit RC (RREF<RC> that) noexcept :RC () {
		auto &&thiz = property[this] ;
		swap (thiz ,that) ;
	}

	inline void operator= (RREF<RC> that) noexcept {
		auto &&thiz = property[this] ;
		if (address (thiz) == address (that))
			return ;
		unsafe_destroy (unsafe_deptr[thiz]) ;
		unsafe_create (unsafe_deptr[thiz] ,forward[that]) ;
		unsafe_barrier () ;
	}

	auto exist () const
		->BOOL {
		return mPointer != NULL ;
	}

	inline auto operator== (CREF<typeof (NULL)>) const
		->BOOL {
		return ifnot (exist ()) ;
	}

	inline auto operator!= (CREF<typeof (NULL)>) const
		->BOOL {
		return exist () ;
	}

	auto to () const leftvalue
		->CREF<UNIT1> {
		assert (exist ()) ;
		return mPointer->self ;
	}

	inline implicit operator CREF<UNIT1> () const leftvalue {
		return self ;
	}

	inline auto operator-> () const leftvalue
		->UNSAFE_PTR<CREF<UNIT1>> {
		return unsafe_pointer (self) ;
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

	inline auto operator!= (CREF<Iterator> that) const
		->BOOL {
		return mCurr != that.mCurr ;
	}

	inline void operator++ () {
		mCurr = mCurr + 1 ;
	}

	inline auto operator* () const leftvalue
		->CREF<INDEX> {
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

	auto begin () const
		->Iterator {
		return Iterator (mBegin) ;
	}

	auto end () const
		->Iterator {
		return Iterator (mEnd) ;
	}
} ;
} ;

namespace U {
class FUNCTION_range {
private:
	using IteratorRange = typename U::ITERATOR_HELP<>::IteratorRange ;

public:
	inline auto operator() (CREF<INDEX> begin ,CREF<INDEX> end) const
		->IteratorRange {
		return IteratorRange (begin ,end) ;
	}
} ;
} ;

static constexpr auto range = U::FUNCTION_range () ;

namespace U {
template <class...>
trait SLICE_HELP ;

template <class ARG1>
trait SLICE_HELP<ARG1> {
	struct DETAIL {
		struct Holder ;
	} ;
} ;

template <class ARG1>
trait SLICE_HELP<ARG1 ,REQUIRE<IS_STR<ARG1>>> {
	using DETAIL = typename SLICE_HELP<ARG1>::DETAIL ;

	class Slice ;
} ;

template <class UNIT1>
struct SLICE_HELP<UNIT1>::DETAIL::Holder :public Interface {
	virtual LENGTH size () const = 0 ;
	virtual CREF<UNIT1> at (CREF<INDEX> index) const = 0 ;
} ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ARG1 ,class ARG2>
trait SLICE_IMPLHOLDER_HELP<ARG1 ,ARG2> {
	using DETAIL = typename SLICE_HELP<ARG1 ,void>::DETAIL ;
	using Holder = typename DETAIL::Holder ;

	class ImplHolder ;
} ;

template <class UNIT1>
class SLICE_HELP<UNIT1 ,REQUIRE<IS_STR<UNIT1>>>::Slice {
private:
	using Holder = typename DETAIL::Holder ;

private:
	RC<Box<Holder>> mPointer ;

public:
	implicit Slice () = default ;

	auto size () const
		->LENGTH {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->size () ;
	}

	auto addr () const
		->LENGTH {
		if (mPointer == NULL)
			return ZERO ;
		return address (mPointer.self->at[0]) ;
	}

	auto at (CREF<INDEX> index) const
		->UNIT1 {
		assert (between (index ,0 ,size ())) ;
		return mPointer.self->at (index) ;
	}

	inline auto operator[] (CREF<INDEX> index) const
		->UNIT1 {
		return at (index) ;
	}

	auto equal (CREF<Slice> that) const
		->BOOL {
		if (size () != that.size ())
			return FALSE ;
		for (auto &&i : range (0 ,size ())) {
			if (mPointer.self->at (i) != that.mPointer.self->at (i))
				return FALSE ;
		}
		return TRUE ;
	}

	inline auto operator== (CREF<Slice> that) const
		->BOOL {
		return equal (that) ;
	}

	inline auto operator!= (CREF<Slice> that) const
		->BOOL {
		return ifnot (equal (that)) ;
	}

	auto compr (CREF<Slice> that) const
		->FLAG {
		const auto r1x = min (size () ,that.size ()) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = operator_compr (mPointer.self->at (i) ,that.mPointer.self->at (i)) ;
			if (r2x != ZERO)
				return r2x ;
		}
		return operator_compr (size () ,that.size ()) ;
	}

	inline auto operator< (CREF<Slice> that) const
		->BOOL {
		return compr (that) < ZERO ;
	}

	inline auto operator<= (CREF<Slice> that) const
		->BOOL {
		return compr (that) <= ZERO ;
	}

	inline auto operator> (CREF<Slice> that) const
		->BOOL {
		return compr (that) > ZERO ;
	}

	inline auto operator>= (CREF<Slice> that) const
		->BOOL {
		return compr (that) >= ZERO ;
	}

	auto hash () const
		->FLAG {
		FLAG ret = hashcode () ;
		for (auto &&i : range (0 ,size ())) {
			const auto r1x = FLAG (mPointer.self->at (i)) ;
			ret = hashcode (ret ,r1x) ;
		}
		return forward[ret] ;
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
	struct DETAIL {
		struct Holder ;
	} ;

	class Clazz ;
} ;

struct CLAZZ_HELP<>::DETAIL::Holder :public Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_cabi () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
} ;

class CLAZZ_HELP<>::Clazz {
private:
	using Holder = typename DETAIL::Holder ;

private:
	RC<Box<Holder>> mPointer ;

public:
	implicit Clazz () = default ;

	auto type_size () const
		->LENGTH {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	auto type_align () const
		->LENGTH {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	auto type_cabi () const
		->FLAG {
		if (mPointer == NULL)
			return ZERO ;
		return mPointer.self->type_size () ;
	}

	auto type_name () const
		->Slice<STR> {
		if (mPointer == NULL)
			return Slice<STR> () ;
		return mPointer.self->type_name () ;
	}

	auto equal (CREF<Clazz> that) const
		->BOOL {
		if (type_cabi () == that.type_cabi ())
			return TRUE ;
		return type_name () == that.type_name () ;
	}

	inline auto operator== (CREF<Clazz> that) const
		->BOOL {
		return equal (that) ;
	}

	inline auto operator!= (CREF<Clazz> that) const
		->BOOL {
		return ifnot (equal (that)) ;
	}

	auto compr (CREF<Clazz> that) const
		->FLAG {
		if (type_cabi () == that.type_cabi ())
			return ZERO ;
		return operator_compr (type_name () ,that.type_name ()) ;
	}

	inline auto operator< (CREF<Clazz> that) const
		->BOOL {
		return compr (that) < ZERO ;
	}

	inline auto operator<= (CREF<Clazz> that) const
		->BOOL {
		return compr (that) <= ZERO ;
	}

	inline auto operator> (CREF<Clazz> that) const
		->BOOL {
		return compr (that) > ZERO ;
	}

	inline auto operator>= (CREF<Clazz> that) const
		->BOOL {
		return compr (that) >= ZERO ;
	}

	auto hash () const
		->FLAG {
		if (type_cabi () == ZERO)
			return ZERO ;
		const auto r1x = type_name () ;
		return r1x.hash () ;
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

	explicit Exception (CREF<Slice<STR>> what) :mWhat (what) {}

	auto what () const leftvalue
		->CREF<Slice<STR>> {
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