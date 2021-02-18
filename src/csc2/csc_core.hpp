#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
inline namespace CORE {
using BOOL = bool ;

static constexpr auto TRUE = BOOL (true) ;
static constexpr auto FALSE = BOOL (false) ;

using VAR32 = std::int32_t ;
using VAR64 = std::int64_t ;

static constexpr auto VAR32_MAX = VAR32 (2147483647) ;
static constexpr auto VAR32_MIN = -VAR32_MAX - 1 ;
static constexpr auto VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr auto VAR64_MIN = -VAR64_MAX - 1 ;

namespace detail {
#ifdef __CSC_CONFIG_VAR32__
struct VAR_HELP {
	using VAR = VAR32 ;

	static constexpr auto VAR_MAX = VAR32_MAX ;
	static constexpr auto VAR_MIN = VAR32_MIN ;
} ;
#endif

#ifdef __CSC_CONFIG_VAR64__
struct VAR_HELP {
	using VAR = VAR64 ;

	static constexpr auto VAR_MAX = VAR64_MAX ;
	static constexpr auto VAR_MIN = VAR64_MIN ;
} ;
#endif
} ;

using VAR = typename detail::VAR_HELP::VAR ;

static constexpr auto VAR_MAX = detail::VAR_HELP::VAR_MAX ;
static constexpr auto VAR_MIN = detail::VAR_HELP::VAR_MIN ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

static constexpr auto ZERO = VAR (+0) ;
static constexpr auto NONE = VAR (-1) ;
static constexpr auto USED = VAR (-2) ;

using SINGLE = float ;
using DOUBLE = double ;

static constexpr auto SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constexpr auto SINGLE_MIN = SINGLE (1.175494351E-38) ;
static constexpr auto SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constexpr auto SINGLE_INF = SINGLE (infinity) ;
static constexpr auto DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constexpr auto DOUBLE_MIN = DOUBLE (2.2250738585072014E-308) ;
static constexpr auto DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constexpr auto DOUBLE_INF = DOUBLE (infinity) ;

using BYTE = std::uint8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using FEAT = std::uint64_t ;

using STRA = char ;
using STRW = wchar_t ;
using STRU8 = unsigned char ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;

namespace detail {
#ifdef __CSC_CONFIG_STRA__
struct STR_HELP {
	using STR = STRA ;
} ;
#endif

#ifdef __CSC_CONFIG_STRW__
struct STR_HELP {
	using STR = STRW ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU8__
struct STR_HELP {
	using STR = STRU8 ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU16__
struct STR_HELP {
	using STR = STRU16 ;
} ;
#endif

#ifdef __CSC_CONFIG_STRU32__
struct STR_HELP {
	using STR = STRU32 ;
} ;
#endif
} ;

using STR = typename detail::STR_HELP::STR ;

template <class UNIT>
using PTR = UNIT * ;

template <class UNIT>
using REF = UNIT && ;

template <class UNIT>
using DEF = const UNIT & ;

namespace detail {
template <VAR ARG1>
struct INTERNEL_ENUM {
	static constexpr auto value = ARG1 ;
} ;

template <class...>
struct INTERNEL_TYPE {} ;

template <class ARG1>
struct INTERNEL_TYPE<ARG1> {
	class ID {} ;

	static constexpr auto id = ID () ;
} ;
} ;

static constexpr auto NULL = nullptr ;

using ENUM_ZERO = enumof (+0) ;
using ENUM_IDEN = enumof (+1) ;

template <class UNIT>
using SIZEOF = enumof (sizeof (UNIT)) ;

template <class UNIT>
using ALIGNOF = enumof (alignof (UNIT)) ;

namespace detail {
template <class...>
struct INTERNEL_COUNTOF_HELP ;

template <class... ARGS>
struct INTERNEL_COUNTOF_HELP<typeas<ARGS...>> {
	using RET = enumof (sizeof... (ARGS)) ;
} ;
} ;

template <class UNIT>
using COUNTOF = enumof (detail::INTERNEL_COUNTOF_HELP<UNIT>::RET::value) ;

template <class UNIT1 ,class UNIT2>
using IS_SAME = enumof (std::is_same<UNIT1 ,UNIT2>::value) ;

namespace detail {
template <class...>
struct ENABLE_HELP ;

template <class ARG1 ,class ARG2>
struct ENABLE_HELP<ARG1 ,ARG2> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;
} ;

template <class COND ,class YES>
using ENABLE = typename detail::ENABLE_HELP<COND ,YES>::RET ;

namespace detail {
template <class...>
struct CONDITIONAL_HELP ;

template <class ARG1 ,class ARG2 ,class ARG3>
struct CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

template <class ARG1 ,class ARG2 ,class ARG3>
struct CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_ZERO>) ;

	using RET = ARG3 ;
} ;
} ;

template <class COND ,class YES ,class NO>
using CONDITIONAL = typename detail::CONDITIONAL_HELP<COND ,YES ,NO>::RET ;

namespace detail {
template <class...>
struct INTERNEL_IS_ENUM_HELP ;

template <VAR ARG1>
struct INTERNEL_IS_ENUM_HELP<CSC::detail::INTERNEL_ENUM<ARG1>> {
	using RET = ENUM_IDEN ;
} ;
} ;

template <class UNIT>
using IS_ENUM = typename detail::INTERNEL_IS_ENUM_HELP<UNIT>::RET ;

template <class UNIT>
using ENUM_CHECK = ENABLE<IS_ENUM<UNIT> ,UNIT> ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = enumof (ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value) ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = enumof (ENUM_CHECK<UNIT1>::value != ENUM_CHECK<UNIT2>::value) ;

template <class UNIT>
using ENUM_BOOL = ENUM_NOT_EQUAL<UNIT ,ENUM_ZERO> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = enumof (CONDITIONAL<enumof (ENUM_CHECK<UNIT1>::value < ENUM_CHECK<UNIT2>::value) ,enumof (-1) ,CONDITIONAL<enumof (ENUM_CHECK<UNIT2>::value < ENUM_CHECK<UNIT1>::value) ,enumof (+1) ,ENUM_ZERO>>) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT_EQ = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO) ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT_EQ = enumof (ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO) ;

template <class UNIT>
using ENUM_EQ_ZERO = ENUM_EQUAL<UNIT ,ENUM_ZERO> ;

template <class UNIT>
using ENUM_LT_ZERO = ENUM_COMPR_LT<UNIT ,ENUM_ZERO> ;

template <class UNIT>
using ENUM_GT_ZERO = ENUM_COMPR_GT<UNIT ,ENUM_ZERO> ;

template <class UNIT>
using ENUM_EQ_IDEN = ENUM_EQUAL<UNIT ,ENUM_IDEN> ;

template <class UNIT>
using ENUM_LT_IDEN = ENUM_COMPR_LT<UNIT ,ENUM_IDEN> ;

template <class UNIT>
using ENUM_GT_IDEN = ENUM_COMPR_GT<UNIT ,ENUM_IDEN> ;

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

template <class UNIT>
using ENUM_INC = ENUM_ADD<UNIT ,ENUM_IDEN> ;

template <class UNIT>
using ENUM_DEC = ENUM_SUB<UNIT ,ENUM_IDEN> ;

namespace detail {
template <class...>
struct INTERNEL_IS_TYPE_HELP ;

template <class... ARGS>
struct INTERNEL_IS_TYPE_HELP<typeas<ARGS...>> {
	using RET = ENUM_IDEN ;
} ;
} ;

template <class UNIT>
using IS_TYPE = typename detail::INTERNEL_IS_TYPE_HELP<UNIT>::RET ;

template <class UNIT>
using TYPE_CHECK = ENABLE<IS_TYPE<UNIT> ,UNIT> ;

namespace detail {
template <class...>
struct INTERNEL_TYPE_FIRST_ONE_HELP ;

template <class ARG1 ,class... ARGS>
struct INTERNEL_TYPE_FIRST_ONE_HELP<typeas<ARG1 ,ARGS...>> {
	using RET = ARG1 ;
} ;
} ;

template <class UNIT>
using TYPE_FIRST_ONE = typename detail::INTERNEL_TYPE_FIRST_ONE_HELP<TYPE_CHECK<UNIT>>::RET ;

namespace detail {
template <class...>
struct INTERNEL_TYPE_FIRST_REST_HELP ;

template <class ARG1 ,class... ARGS>
struct INTERNEL_TYPE_FIRST_REST_HELP<typeas<ARG1 ,ARGS...>> {
	using RET = typeas<ARGS...> ;
} ;
} ;

template <class UNIT>
using TYPE_FIRST_REST = typename detail::INTERNEL_TYPE_FIRST_REST_HELP<TYPE_CHECK<UNIT>>::RET ;

namespace detail {
template <class...>
struct INTERNEL_TYPE_SECOND_ONE_HELP ;

template <class ARG1 ,class ARG2 ,class... ARGS>
struct INTERNEL_TYPE_SECOND_ONE_HELP<typeas<ARG1 ,ARG2 ,ARGS...>> {
	using RET = ARG2 ;
} ;
} ;

template <class UNIT>
using TYPE_SECOND_ONE = typename detail::INTERNEL_TYPE_SECOND_ONE_HELP<TYPE_CHECK<UNIT>>::RET ;

namespace detail {
template <class...>
struct INTERNEL_TYPE_SECOND_REST_HELP ;

template <class ARG1 ,class ARG2 ,class... ARGS>
struct INTERNEL_TYPE_SECOND_REST_HELP<typeas<ARG1 ,ARG2 ,ARGS...>> {
	using RET = typeas<ARGS...> ;
} ;
} ;

template <class UNIT>
using TYPE_SECOND_REST = typename detail::INTERNEL_TYPE_SECOND_REST_HELP<TYPE_CHECK<UNIT>>::RET ;

namespace detail {
template <class...>
struct INTERNEL_TYPE_CAT_HELP ;

template <class... ARGS1 ,class... ARGS2>
struct INTERNEL_TYPE_CAT_HELP<typeas<ARGS1...> ,typeas<ARGS2...>> {
	using RET = typeas<ARGS1... ,ARGS2...> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using TYPE_CAT = typename detail::INTERNEL_TYPE_CAT_HELP<TYPE_CHECK<UNIT1> ,TYPE_CHECK<UNIT2>>::RET ;

namespace detail {
template <class...>
struct TYPE_REPEAT_HELP ;

template <class ARG1 ,class ARG2>
struct TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = typeas<> ;
} ;

template <class ARG1 ,class ARG2>
struct TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using R1X = typename TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2>>::RET ;

	using RET = TYPE_CAT<ARG1 ,R1X> ;
} ;
} ;

template <class UNIT ,class SIZE>
using TYPE_REPEAT = typename detail::TYPE_REPEAT_HELP<typeas<UNIT> ,SIZE>::RET ;

namespace detail {
template <class...>
struct TYPE_REVERSE_HELP ;

template <class ARG1>
struct TYPE_REVERSE_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = typeas<> ;
} ;

template <class ARG1>
struct TYPE_REVERSE_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = TYPE_CAT<R2X ,typeas<R1X>> ;
} ;
} ;

template <class UNIT>
using TYPE_REVERSE = typename detail::TYPE_REVERSE_HELP<UNIT>::RET ;

namespace detail {
template <class...>
struct TYPE_PICK_HELP ;

template <class ARG1 ,class ARG2>
struct TYPE_PICK_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;

template <class ARG1 ,class ARG2>
struct TYPE_PICK_HELP<ARG1 ,ARG2> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<ARG1> ,ENUM_DEC<ARG2>>::RET ;
} ;
} ;

template <class LIST ,class INDEX>
using TYPE_PICK = typename detail::TYPE_PICK_HELP<LIST ,INDEX>::RET ;

namespace detail {
template <class...>
struct ENUM_ALL_HELP ;

template <class ARG1>
struct ENUM_ALL_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
struct ENUM_ALL_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_BOOL<TYPE_FIRST_ONE<ARG1>> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,R2X ,ENUM_ZERO> ;
} ;
} ;

template <class... ARGS>
using ENUM_ALL = typename detail::ENUM_ALL_HELP<typeas<ARGS...>>::RET ;

namespace detail {
template <class...>
struct ENUM_ANY_HELP ;

template <class ARG1>
struct ENUM_ANY_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
struct ENUM_ANY_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_BOOL<TYPE_FIRST_ONE<ARG1>> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R2X> ;
} ;
} ;

template <class... ARGS>
using ENUM_ANY = typename detail::ENUM_ANY_HELP<typeas<ARGS...>>::RET ;

namespace detail {
template <class...>
struct IS_ALL_SAME_HELP ;

template <class ARG1>
struct IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
struct IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

template <class ARG1>
struct IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ALL_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>>>::RET ;

	using RET = ENUM_ALL<IS_SAME<R1X ,R2X> ,R3X> ;
} ;
} ;

template <class... ARGS>
using IS_ALL_SAME = typename detail::IS_ALL_SAME_HELP<typeas<ARGS...>>::RET ;

namespace detail {
template <class...>
struct IS_ANY_SAME_HELP ;

template <class ARG1>
struct IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
struct IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

template <class ARG1>
struct IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = typename IS_ANY_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>>>::RET ;
	using R4X = typename IS_ANY_SAME_HELP<TYPE_CAT<R2X ,TYPE_SECOND_REST<ARG1>>>::RET ;

	using RET = ENUM_ANY<IS_SAME<R1X ,R2X> ,R3X ,R4X> ;
} ;
} ;

template <class... ARGS>
using IS_ANY_SAME = typename detail::IS_ANY_SAME_HELP<typeas<ARGS...>>::RET ;

namespace detail {
template <class...>
struct PLACEHOLDER_HELP ;

template <class ARG1>
struct PLACEHOLDER_HELP<ARG1> {
	require (ENUM_EQ_ZERO<ARG1>) ;

	class PlaceHolder {} ;
} ;

template <class ARG1>
struct PLACEHOLDER_HELP<ARG1> {
	require (ENUM_GT_ZERO<ARG1>) ;

	using BASE = typename PLACEHOLDER_HELP<ENUM_DEC<ARG1>>::PlaceHolder ;

	class PlaceHolder :public BASE {} ;
} ;
} ;

template <class RANK>
using PlaceHolder = typename detail::PLACEHOLDER_HELP<RANK>::PlaceHolder ;

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

template <class UNIT>
using IS_BOOL = IS_SAME<UNIT ,BOOL> ;

template <class UNIT>
using IS_VAR = ENUM_ANY<IS_SAME<UNIT ,VAR32> ,IS_SAME<UNIT ,VAR64>> ;

template <class UNIT>
using IS_FLOAT = ENUM_ANY<IS_SAME<UNIT ,SINGLE> ,IS_SAME<UNIT ,DOUBLE>> ;

template <class UNIT>
using IS_BYTE = ENUM_ANY<IS_SAME<UNIT ,BYTE> ,IS_SAME<UNIT ,WORD> ,IS_SAME<UNIT ,CHAR> ,IS_SAME<UNIT ,FEAT>> ;

template <class UNIT>
using IS_STR = ENUM_ANY<IS_SAME<UNIT ,STRA> ,IS_SAME<UNIT ,STRU8> ,IS_SAME<UNIT ,STRU16> ,IS_SAME<UNIT ,STRU32>> ;

template <class UNIT>
using IS_NULL = IS_SAME<UNIT ,typeof (NULL)> ;

template <class UNIT>
using IS_BASIC = ENUM_ANY<IS_BOOL<UNIT> ,IS_VAR<UNIT> ,IS_FLOAT<UNIT> ,IS_BYTE<UNIT> ,IS_STR<UNIT> ,IS_NULL<UNIT>> ;

template <class UNIT>
using IS_FUNCTION = enumof (rust::is_function (UNIT)) ;

template <class UNIT>
using IS_MUTABLE = enumof (TRUE) ;

template <class UNIT>
using IS_NOEXCPET = enumof (FALSE) ;

template <class UNIT>
using IS_INTERFACE = enumof (rust::is_interface (UNIT)) ;

template <class BASE ,class DERIVED>
using IS_EXTEND = enumof (std::is_base_of<BASE ,DERIVED>::value) ;

template <class UNIT>
using IS_PLACEHOLDER = IS_EXTEND<UNIT ,typeof (PH0)> ;

template <class UNIT>
using IS_STRUCT = enumof (rust::is_struct (UNIT)) ;

template <class UNIT>
using IS_CLASS = enumof (std::is_class<UNIT>::value) ;

template <class UNIT>
using IS_TRIVIAL = enumof (std::is_trivial<UNIT>::value) ;

template <class UNIT>
using IS_DYNAMIC = enumof (rust::is_dynamic (UNIT)) ;

namespace detail {
template <class...>
struct IS_CONSTRUCTIBLE ;

template <class ARG1 ,class...ARGS>
struct IS_CONSTRUCTIBLE<ARG1 ,typeas<ARGS...>> {
	using RET = std::is_constructible<ARG1 ,ARGS...>::value ;
} ;
} ;

template <class UNIT ,class PARAMS>
using IS_CONSTRUCTIBLE = typename detail::IS_CONSTRUCTIBLE<UNIT ,PARAMS>::RET ;

template <class UNIT>
using IS_CLONEABLE = enumof (rust::is_cloneable (UNIT)) ;

template <class UNIT>
using IS_SHAREABLE = enumof (rust::is_shareable (UNIT)) ;

namespace detail {
template <class...>
struct BYTE_TRAIT_HELP ;

template <class ARG1>
struct BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<BYTE>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<BYTE>>) ;

	using RET = BYTE ;
} ;

template <class ARG1>
struct BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<WORD>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<WORD>>) ;

	using RET = WORD ;
} ;

template <class ARG1>
struct BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<CHAR>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<CHAR>>) ;

	using RET = CHAR ;
} ;

template <class ARG1>
struct BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<FEAT>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<FEAT>>) ;

	using RET = FEAT ;
} ;
} ;

template <class UNIT>
using BYTE_TRAIT = typename detail::BYTE_TRAIT_HELP<UNIT>::RET ;

namespace detail {
struct FUNCTION_NOOP {
	inline void operator() () const {}
} ;
} ;

static constexpr auto noop = detail::FUNCTION_NOOP () ;

namespace detail {
struct FUNCTION_ASSERT {
	inline void operator() (DEF<BOOL> cond) const {
		if (cond)
			return ;
		__debugbreak () ;
		std::terminate () ;
	}
} ;
} ;

static constexpr auto assert = detail::FUNCTION_ASSERT () ;

namespace detail {
struct FUNCTION_BAD {
	template <class ARG1>
	inline ARG1 operator() (REF<ARG1> id) const {
		using R1X = typeof (id) ;
		assert (FALSE) ;
		const auto r1x = LAMBDA_01 () ;
		return ARG1 (r1x ()) ;
	}
} ;
} ;

static constexpr auto bad = detail::FUNCTION_BAD () ;

namespace detail {
template <class...>
struct FORWARD_HELP ;

template <class ARG1>
struct FORWARD_HELP<ARG1> {
	require (IS_CLASS<ARG1>) ;
	require (IS_SHAREABLE<ARG1>) ;

	struct FUNCTION_FORWARD {
		template <class ARG1>
		inline ARG1 operator() (REF<ARG1> obj) const {
			return obj.share () ;
		}
	} ;
} ;

template <class ARG1>
struct FORWARD_HELP<ARG1> {
	require (IS_CLASS<ARG1>) ;
	require not (IS_SHAREABLE<ARG1>) ;
	require (IS_CLONEABLE<ARG1>) ;

	struct FUNCTION_FORWARD {
		template <class ARG1>
		inline ARG1 operator() (DEF<ARG1> obj) const {
			return obj.clone () ;
		}
	} ;
} ;

template <class ARG1>
struct FORWARD_HELP<ARG1> {
	require not (IS_CLASS<ARG1>) ;

	struct FUNCTION_FORWARD {
		template <class ARG1>
		inline ARG1 operator() (DEF<ARG1> obj) const {
			return obj ;
		}
	} ;
} ;

struct FUNCTION_FORWARD {
	template <class ARG1>
	inline ARG1 operator() (REF<ARG1> obj) const {
		using R1X = typename FORWARD_HELP<ARG1>::FUNCTION_FORWARD ;
		static constexpr auto M_FUNC = R1X () ;
		return M_FUNC (obj) ;
	}
} ;
} ;

static constexpr auto forward = detail::FUNCTION_FORWARD () ;

namespace detail {
struct FUNCTION_SWAP {
	template <class ARG1>
	inline void operator() (REF<ARG1> obj1 ,REF<ARG1> obj2) const {
		auto rax = forward (obj1) ;
		obj1 = forward (obj2) ;
		obj2 = forward (rax) ;
	}
} ;
} ;

static constexpr auto swap = detail::FUNCTION_SWAP () ;

/*
function exchange = (lhs ,rhs) => {
	variable ret = forward (lhs) ;
	lhs = forward (rhs) ;
	return ret ;
} ;
*/

namespace detail {
struct FUNCTION_ADDRESS {
	template <class ARG1>
	inline LENGTH operator() (REF<ARG1> obj) const {
		return LENGTH (&reinterpret_cast<VAR &> (obj)) ;
	}
} ;
} ;

static constexpr auto address = detail::FUNCTION_ADDRESS () ;

namespace detail {
struct FUNCTION_ALIGNTO {
	inline LENGTH operator() (DEF<LENGTH> base ,DEF<LENGTH> align) const {
		const auto r1x = align - base % align ;
		return base + r1x % align ;
	}
} ;
} ;

static constexpr auto alignto = detail::FUNCTION_ALIGNTO () ;

namespace detail {
struct FUNCTION_BETWEEN {
	inline BOOL operator() (DEF<INDEX> index ,DEF<INDEX> begin ,DEF<INDEX> end) const {
		return index >= begin && index < end ;
	}
} ;
} ;

static constexpr auto between = detail::FUNCTION_BETWEEN () ;

namespace detail {
struct FUNCTION_ABS {
	template <class ARG1>
	inline ARG1 operator() (DEF<ARG1> obj) const {
		using R1X = typeof (obj) ;
		if (obj < R1X (ZERO))
			return -obj ;
		return +obj ;
	}
} ;
} ;

static constexpr auto abs = detail::FUNCTION_ABS () ;

namespace detail {
struct FUNCTION_MIN {
	template <class ARG1>
	inline ARG1 operator() (DEF<ARG1> obj1 ,DEF<ARG1> obj2) const {
		if (obj1 < obj2)
			return obj1 ;
		return obj2 ;
	}
} ;
} ;

static constexpr auto min = detail::FUNCTION_MIN () ;

namespace detail {
struct FUNCTION_MAX {
	template <class ARG1>
	inline ARG1 operator() (DEF<ARG1> obj1 ,DEF<ARG1> obj2) const {
		if (obj1 < obj2)
			return obj2 ;
		return obj1 ;
	}
} ;
} ;

static constexpr auto max = detail::FUNCTION_MAX () ;

namespace detail {
struct FUNCTION_HASHCODE {
	template <class ARG1>
	inline FLAG operator() () const {
		const auto r2x = VAR64 (-3750763034362895579) ;
		const auto r3x = VAR64 (FEAT (r2x) & FEAT (VAR_MAX)) ;
		return FLAG (r3x) ;
	}

	template <class ARG1>
	inline FLAG operator() (DEF<FLAG> obj1 ,DEF<FLAG> obj2) const {
		const auto r1x = VAR64 (FEAT (obj1) ^ FEAT (obj2)) ;
		const auto r2x = r1x * VAR64 (1099511628211) ;
		const auto r3x = VAR64 (FEAT (r2x) & FEAT (VAR_MAX)) ;
		return FLAG (r3x) ;
	}
} ;
} ;

static constexpr auto hashcode = detail::FUNCTION_HASHCODE () ;
} ;
} ;