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
static constexpr auto VAR32_MIN = -VAR32_MAX ;
static constexpr auto VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr auto VAR64_MIN = -VAR64_MAX ;

namespace U {
template <class...>
trait VAR_HELP ;

template <class UNIT1>
trait VAR_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_VAR32<UNIT1>>> {
	using VAR = VAR32 ;

	static constexpr auto VAR_MAX = VAR32_MAX ;
	static constexpr auto VAR_MIN = VAR32_MIN ;
} ;

template <class UNIT1>
trait VAR_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_VAR64<UNIT1>>> {
	using VAR = VAR64 ;

	static constexpr auto VAR_MAX = VAR64_MAX ;
	static constexpr auto VAR_MIN = VAR64_MIN ;
} ;
} ;

using VAR = typename U::VAR_HELP<void ,ALWAYS>::VAR ;

static constexpr auto VAR_MAX = U::VAR_HELP<void ,ALWAYS>::VAR_MAX ;
static constexpr auto VAR_MIN = U::VAR_HELP<void ,ALWAYS>::VAR_MIN ;

static constexpr auto ZERO = VAR (+0) ;
static constexpr auto IDEN = VAR (+1) ;
static constexpr auto NONE = VAR (-1) ;
static constexpr auto USED = VAR (-2) ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

using SINGLE = float ;
using DOUBLE = double ;

static constexpr auto SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constexpr auto SINGLE_MIN = -SINGLE_MAX ;
static constexpr auto SINGLE_LOW = SINGLE (1.175494351E-38) ;
static constexpr auto SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constexpr auto SINGLE_INF = SINGLE (std::numeric_limits<float>::infinity ()) ;
static constexpr auto DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constexpr auto DOUBLE_MIN = -DOUBLE_MAX ;
static constexpr auto DOUBLE_LOW = DOUBLE (2.2250738585072014E-308) ;
static constexpr auto DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constexpr auto DOUBLE_INF = SINGLE (std::numeric_limits<float>::infinity ()) ;

using STRA = char ;
using STRW = wchar_t ;
using STRU8 = U::__char8_t ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;

namespace U {
template <class...>
trait STR_HELP ;

template <class UNIT1>
trait STR_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_STRA<UNIT1>>> {
	using STR = STRA ;
} ;

template <class UNIT1>
trait STR_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_STRW<UNIT1>>> {
	using STR = STRW ;
} ;
} ;

using STR = typename U::STR_HELP<void ,ALWAYS>::STR ;

using BYTE = std::uint8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using FEAT = std::uint64_t ;
using MEGA = U::__int128_t ;

static constexpr auto NULL = nullptr ;

template <class UNIT1>
using DEF = UNIT1 ;

template <class UNIT1>
using VREF = DEF<REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using CREF = DEF<const REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using RREF = DEF<REMOVE_REF<UNIT1> &&> ;

template <class UNIT1>
using XREF = DEF<UNIT1 &&> ;

struct VARIABLE ;
struct CONSTANT ;
struct REGISTER ;

template <class UNIT1>
using SIZE_OF = ENUMAS<VAR ,U::ENUMID<(sizeof (UNIT1))>> ;

template <class UNIT1>
using ALIGN_OF = ENUMAS<VAR ,U::ENUMID<(alignof (UNIT1))>> ;

namespace U {
template <class...>
trait COUNT_OF_HELP ;

template <class...UNITS>
trait COUNT_OF_HELP<TYPEAS<UNITS...> ,ALWAYS> {
	using RET = ENUMAS<VAR ,U::ENUMID<(sizeof... (UNITS))>> ;
} ;
} ;

template <class UNIT1>
using COUNT_OF = typename U::COUNT_OF_HELP<UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait IS_ENUM_HELP ;

template <class UNIT1>
trait IS_ENUM_HELP<UNIT1 ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <class...UNITS>
trait IS_ENUM_HELP<ENUMAS<UNITS...> ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;
} ;

template <class UNIT1>
using IS_ENUM = typename U::IS_ENUM_HELP<UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait IS_TYPE_HELP ;

template <class UNIT1>
trait IS_TYPE_HELP<UNIT1 ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <class...UNITS>
trait IS_TYPE_HELP<TYPEAS<UNITS...> ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;
} ;

template <class UNIT1>
using IS_TYPE = typename U::IS_TYPE_HELP<UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait IS_SAME_HELP ;

template <class UNIT1 ,class UNIT2>
trait IS_SAME_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT1>
trait IS_SAME_HELP<UNIT1 ,UNIT1 ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using IS_SAME = typename U::IS_SAME_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

using ENUM_ZERO = ENUMAS<VAR ,U::ENUMID<ZERO>> ;
using ENUM_IDEN = ENUMAS<VAR ,U::ENUMID<IDEN>> ;
using ENUM_NONE = ENUMAS<VAR ,U::ENUMID<NONE>> ;
using ENUM_USED = ENUMAS<VAR ,U::ENUMID<USED>> ;

namespace U {
template <class...>
trait ENABLE_HELP ;

template <class COND ,class YES>
trait ENABLE_HELP<COND ,YES ,REQUIRE<COND>> {
	using RET = YES ;
} ;
} ;

template <class COND ,class YES = void>
using ENABLE = typename U::ENABLE_HELP<COND ,YES ,ALWAYS>::RET ;

template <class UNIT1>
using ENUM_CHECK = ENABLE<IS_ENUM<UNIT1> ,UNIT1> ;

template <class UNIT1>
using TYPE_CHECK = ENABLE<IS_TYPE<UNIT1> ,UNIT1> ;

namespace U {
template <class...>
trait CONDITIONAL_HELP ;

template <class COND ,class YES ,class NO>
trait CONDITIONAL_HELP<COND ,YES ,NO ,REQUIRE<COND>> {
	using RET = YES ;
} ;

template <class COND ,class YES ,class NO>
trait CONDITIONAL_HELP<COND ,YES ,NO ,REQUIRE<ENUM_NOT<COND>>> {
	using RET = NO ;
} ;
} ;

template <class COND ,class YES ,class NO>
using CONDITIONAL = typename U::CONDITIONAL_HELP<COND ,YES ,NO ,ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = ENUMAS<BOOL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<UNIT1 ,UNIT2>> ;

namespace U {
template <class...>
trait ENUM_COMPR_HELP ;

template <>
trait ENUM_COMPR_HELP<ENUM_TRUE ,ENUM_FALSE ,ALWAYS> {
	using RET = ENUM_NONE ;
} ;

template <>
trait ENUM_COMPR_HELP<ENUM_FALSE ,ENUM_TRUE ,ALWAYS> {
	using RET = ENUM_IDEN ;
} ;

template <>
trait ENUM_COMPR_HELP<ENUM_FALSE ,ENUM_FALSE ,ALWAYS> {
	using RET = ENUM_ZERO ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = typename U::ENUM_COMPR_HELP<
	ENUMAS<BOOL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value < ENUM_CHECK<UNIT2>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(ENUM_CHECK<UNIT2>::value < ENUM_CHECK<UNIT1>::value)>> ,
	ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = ENUMAS<BOOL ,U::ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LTEQ = ENUMAS<BOOL ,U::ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = ENUMAS<BOOL ,U::ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GTEQ = ENUMAS<BOOL ,U::ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO)>> ;

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
using ENUM_ADD = ENUMAS<VAR ,U::ENUMID<(ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = ENUMAS<VAR ,U::ENUMID<(ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = ENUMAS<VAR ,U::ENUMID<(ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = ENUMAS<VAR ,U::ENUMID<(ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = ENUMAS<VAR ,U::ENUMID<(ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1>
using ENUM_MINUS = ENUM_SUB<ENUM_ZERO ,UNIT1> ;

template <class UNIT1>
using ENUM_INC = ENUM_ADD<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_DEC = ENUM_SUB<UNIT1 ,ENUM_IDEN> ;

namespace U {
template <class...>
trait TYPE_FIRST_HELP ;

template <class UNIT1 ,class...UNITS>
trait TYPE_FIRST_HELP<TYPEAS<UNIT1 ,UNITS...> ,ALWAYS> {
	using ONE = UNIT1 ;
	using REST = TYPEAS<UNITS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_ONE = typename U::TYPE_FIRST_HELP<UNIT1 ,ALWAYS>::ONE ;

template <class UNIT1>
using TYPE_FIRST_REST = typename U::TYPE_FIRST_HELP<UNIT1 ,ALWAYS>::REST ;

namespace U {
template <class...>
trait TYPE_SECOND_HELP ;

template <class UNIT1 ,class UNIT2 ,class...UNITS>
trait TYPE_SECOND_HELP<TYPEAS<UNIT1 ,UNIT2 ,UNITS...> ,ALWAYS> {
	using ONE = UNIT2 ;
	using REST = TYPEAS<UNITS...> ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_ONE = typename U::TYPE_SECOND_HELP<UNIT1 ,ALWAYS>::ONE ;

template <class UNIT1>
using TYPE_SECOND_REST = typename U::TYPE_SECOND_HELP<UNIT1 ,ALWAYS>::REST ;

template <class UNIT1>
using TYPE_UNWIND = ENABLE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>> ,TYPE_FIRST_ONE<UNIT1>> ;

namespace U {
template <class...>
trait TYPE_CAT_HELP ;

template <class...UNITS1 ,class...UNITS2>
trait TYPE_CAT_HELP<TYPEAS<UNITS1...> ,TYPEAS<UNITS2...> ,ALWAYS> {
	using RET = TYPEAS<UNITS1... ,UNITS2...> ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using TYPE_CAT = typename U::TYPE_CAT_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait TYPE_REPEAT_HELP ;

template <class ITEM ,class SIZE>
trait TYPE_REPEAT_HELP<ITEM ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = TYPEAS<> ;
} ;

template <class ITEM ,class SIZE>
trait TYPE_REPEAT_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using R1X = typename TYPE_REPEAT_HELP<ITEM ,ENUM_DEC<SIZE> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R1X ,TYPEAS<ITEM>> ;
} ;
} ;

template <class ITEM ,class SIZE>
using TYPE_REPEAT = typename U::TYPE_REPEAT_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

namespace U {
template <class...>
trait ENUM_ALL_HELP ;

template <class UNIT1>
trait ENUM_ALL_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT1>
trait ENUM_ALL_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = ENUM_CHECK<TYPE_FIRST_ONE<UNIT1>> ;
	using R3X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,R3X ,ENUM_FALSE> ;
} ;
} ;

template <class...UNITS>
using ENUM_ALL = typename U::ENUM_ALL_HELP<TYPEAS<UNITS...> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait ENUM_ANY_HELP ;

template <class UNIT1>
trait ENUM_ANY_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT1>
trait ENUM_ANY_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = ENUM_CHECK<TYPE_FIRST_ONE<UNIT1>> ;
	using R3X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_TRUE ,R3X> ;
} ;
} ;

template <class...UNITS>
using ENUM_ANY = typename U::ENUM_ANY_HELP<TYPEAS<UNITS...> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait ENUM_SUM_HELP ;

template <class UNIT1>
trait ENUM_SUM_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class UNIT1>
trait ENUM_SUM_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = ENUM_CHECK<TYPE_FIRST_ONE<UNIT1>> ;
	using R3X = typename ENUM_SUM_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = ENUM_ADD<R1X ,R3X> ;
} ;
} ;

template <class...UNITS>
using ENUM_SUM = typename U::ENUM_SUM_HELP<TYPEAS<UNITS...> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait PLACEHOLDER_HELP ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	class PlaceHolder {} ;
} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using BASE = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::PlaceHolder ;

	class PlaceHolder extend BASE {} ;
} ;
} ;

template <class RANK>
using PlaceHolder = typename U::PLACEHOLDER_HELP<RANK ,ALWAYS>::PlaceHolder ;

static constexpr auto PH0 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+0)>>> () ;
static constexpr auto PH1 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+1)>>> () ;
static constexpr auto PH2 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+2)>>> () ;
static constexpr auto PH3 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+3)>>> () ;
static constexpr auto PH4 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+4)>>> () ;
static constexpr auto PH5 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+5)>>> () ;
static constexpr auto PH6 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+6)>>> () ;
static constexpr auto PH7 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+7)>>> () ;
static constexpr auto PH8 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+8)>>> () ;
static constexpr auto PH9 = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(+9)>>> () ;
static constexpr auto PHX = PlaceHolder<ENUMAS<VAR ,U::ENUMID<(10)>>> () ;

namespace U {
template <class...>
trait REFLECT_REF_HELP ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,VREF<UNIT1> ,ALWAYS> {
	using RET = VARIABLE ;
} ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,CREF<UNIT1> ,ALWAYS> {
	using RET = CONSTANT ;
} ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,RREF<UNIT1> ,ALWAYS> {
	using RET = REGISTER ;
} ;
} ;

template <class UNIT1>
using REFLECT_REF = typename U::REFLECT_REF_HELP<REMOVE_REF<UNIT1> ,XREF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_VARIABLE = IS_SAME<REFLECT_REF<UNIT1> ,VARIABLE> ;

template <class UNIT1>
using IS_CONSTANT = IS_SAME<REFLECT_REF<UNIT1> ,CONSTANT> ;

template <class UNIT1>
using IS_REGISTER = IS_SAME<REFLECT_REF<UNIT1> ,REGISTER> ;

namespace U {
template <class...>
trait PTR_HELP ;

template <class UNIT1>
trait PTR_HELP<UNIT1 ,VREF<UNIT1> ,void ,ALWAYS> {
	using RET = DEF<UNIT1 *> ;
} ;

template <class UNIT1>
trait PTR_HELP<UNIT1 ,CREF<UNIT1> ,void ,ALWAYS> {
	using RET = DEF<const UNIT1 *> ;
} ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,VREF<UNIT1> ,UNIT2 ,ALWAYS> {
	using RET = DEF<UNIT1 UNIT2::*> ;
} ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,CREF<UNIT1> ,UNIT2 ,ALWAYS> {
	using RET = DEF<const UNIT1 UNIT2::*> ;
} ;
} ;

template <class UNIT1 ,class UNIT2 = void>
using PTR = typename U::PTR_HELP<REMOVE_REF<UNIT1> ,XREF<UNIT1> ,UNIT2 ,ALWAYS>::RET ;

template <class UNIT1>
using IS_CLASS = ENUMAS<BOOL ,U::ENUMID<(std::is_class<UNIT1>::value)>> ;

template <class UNIT1>
using IS_DEFAULT = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_default_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_destructible<UNIT1>::value)>>> ;

namespace U {
template <class...>
trait IS_CONSTRUCTIBLE_HELP ;

template <class RETURN ,class...PARAMS>
trait IS_CONSTRUCTIBLE_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS> {
	using RET = ENUMAS<BOOL ,U::ENUMID<(std::is_constructible<RETURN ,PARAMS...>::value)>> ;
} ;
} ;

template <class RETURN ,class PARAMS>
using IS_CONSTRUCTIBLE = typename U::IS_CONSTRUCTIBLE_HELP<RETURN ,PARAMS ,ALWAYS>::RET ;

template <class UNIT1>
using IS_TRIVIAL = ENUMAS<BOOL ,U::ENUMID<(std::is_trivial<UNIT1>::value)>> ;

template <class UNIT1>
using IS_CLONEABLE = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_copy_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_copy_assignable<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_move_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_move_assignable<UNIT1>::value)>>> ;

struct Interface {
	implicit Interface () = default ;
	virtual ~Interface () = default ;
	implicit Interface (CREF<Interface>) = delete ;
	inline VREF<Interface> operator= (CREF<Interface>) = delete ;
	implicit Interface (RREF<Interface>) = default ;
	inline VREF<Interface> operator= (RREF<Interface>) = default ;
} ;

template <class UNIT1>
using IS_INTERFACE = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_abstract<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_base_of<Interface ,UNIT1>::value)>>> ;

template <class BASE ,class DERIVED>
using IS_EXTEND = ENUM_ANY<
	IS_SAME<BASE ,DERIVED> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_base_of<BASE ,DERIVED>::value)>>> ;

template <class BASE ,class DERIVED>
using IS_IMPLEMENT = ENABLE<IS_INTERFACE<BASE> ,IS_EXTEND<BASE ,DERIVED>> ;

struct Proxy {
	implicit Proxy () = delete ;
	implicit ~Proxy () = delete ;
	implicit Proxy (CREF<Proxy>) = delete ;
	inline VREF<Proxy> operator= (CREF<Proxy>) = delete ;
	implicit Proxy (RREF<Proxy>) = delete ;
	inline VREF<Proxy> operator= (RREF<Proxy>) = delete ;
} ;

template <class UNIT1>
using IS_ARRAY = ENUMAS<BOOL ,U::ENUMID<(std::is_array<UNIT1>::value)>> ;

namespace U {
template <class...>
trait REFLECT_ARRAY_HELP ;

template <class UNIT1>
trait REFLECT_ARRAY_HELP<DEF<UNIT1[]> ,ALWAYS> {
	using R1X = ENUM_ZERO ;
	using RET = TYPEAS<UNIT1 ,R1X> ;
} ;

template <class UNIT1 ,LENGTH UNIT2>
trait REFLECT_ARRAY_HELP<DEF<UNIT1[UNIT2]> ,ALWAYS> {
	using R1X = ENUMAS<VAR ,U::ENUMID<UNIT2>> ;
	using RET = TYPEAS<UNIT1 ,R1X> ;
} ;
} ;

template <class UNIT1>
using REFLECT_ARRAY = typename U::REFLECT_ARRAY_HELP<REMOVE_ALL<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using ARRAY_ITEM = TYPE_FIRST_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class UNIT1>
using ARRAY_SIZE = TYPE_SECOND_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class UNIT1>
using IS_FUNCTION = ENUMAS<BOOL ,U::ENUMID<(std::is_function<UNIT1>::value)>> ;

namespace U {
template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class UNIT1 ,class UNIT2 ,class...UNITS>
trait REFLECT_FUNCTION_HELP<DEF<UNIT1 (UNIT2::*) (UNITS...)> ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = TYPEAS<XREF<UNITS>...> ;
	using RET = TYPEAS<R1X ,R2X> ;
} ;

template <class UNIT1 ,class UNIT2 ,class...UNITS>
trait REFLECT_FUNCTION_HELP<DEF<UNIT1 (UNIT2::*) (UNITS...) const> ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = TYPEAS<XREF<UNITS>...> ;
	using RET = TYPEAS<R1X ,R2X> ;
} ;
} ;

template <class UNIT1>
using REFLECT_FUNCTION = typename U::REFLECT_FUNCTION_HELP<typeof (&UNIT1::operator()) ,ALWAYS>::RET ;

template <class UNIT1>
using FUNCTION_RETURN = TYPE_FIRST_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using FUNCTION_PARAMS = TYPE_SECOND_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using IS_BOOL = IS_SAME<UNIT1 ,BOOL> ;

template <class UNIT1>
using IS_VAR = ENUM_ANY<
	IS_SAME<UNIT1 ,VAR32> ,
	IS_SAME<UNIT1 ,VAR64>> ;

template <class UNIT1>
using IS_FLOAT = ENUM_ANY<
	IS_SAME<UNIT1 ,SINGLE> ,
	IS_SAME<UNIT1 ,DOUBLE>> ;

template <class UNIT1>
using IS_STR = ENUM_ANY<
	IS_SAME<UNIT1 ,STRA> ,
	IS_SAME<UNIT1 ,STRW>> ;

template <class UNIT1>
using IS_BYTE = ENUM_ANY<
	IS_SAME<UNIT1 ,BYTE> ,
	IS_SAME<UNIT1 ,WORD> ,
	IS_SAME<UNIT1 ,CHAR> ,
	IS_SAME<UNIT1 ,FEAT> ,
	IS_SAME<UNIT1 ,MEGA>> ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,typeof (NULL)> ;

template <class UNIT1>
using IS_BASIC = ENUM_ANY<
	IS_BOOL<UNIT1> ,
	IS_VAR<UNIT1> ,
	IS_FLOAT<UNIT1> ,
	IS_STR<UNIT1> ,
	IS_BYTE<UNIT1> ,
	IS_NULL<UNIT1>> ;

template <class UNIT1>
using IS_VOID = IS_SAME<UNIT1 ,void> ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,typeof (PH0)> ;

namespace U {
template <class...>
trait BYTE_BASE_HELP ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<BYTE>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<BYTE>>>>> {
	using RET = BYTE ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<WORD>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<WORD>>>>> {
	using RET = WORD ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<CHAR>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<CHAR>>>>> {
	using RET = CHAR ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<FEAT>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<FEAT>>>>> {
	using RET = FEAT ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<MEGA>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<MEGA>>>>> {
	using RET = MEGA ;
} ;
} ;

template <class SIZE ,class ALIGN>
using BYTE_BASE = typename U::BYTE_BASE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

namespace U {
template <class...>
trait STORAGE_HELP ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,ALWAYS> {
	using R3X = BYTE_BASE<ALIGN ,ALIGN> ;
	using R1X = SIZE_OF<R3X> ;
	using R4X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<R1X>> ,R1X> ;
	using RET = DEF<R3X[ENUM_CHECK<R4X>::value]> ;
} ;
} ;

template <class SIZE ,class ALIGN = ENUM_IDEN>
using Storage = typename U::STORAGE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

namespace U {
template <class...>
struct TEMPID ;

template <class UNIT1>
struct TEMPID<UNIT1> {
	Storage<SIZE_OF<UNIT1> ,ALIGN_OF<UNIT1>> mStorage ;
} ;

template <>
struct TEMPID<void> ;

template <>
struct TEMPID<TEMPID<void>> ;
} ;

template <class UNIT1>
using TEMP = U::TEMPID<UNIT1> ;

namespace U {
template <class...>
trait REMOVE_TEMP_HELP ;

template <class UNIT1>
trait REMOVE_TEMP_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_TEMP_HELP<TEMP<UNIT1> ,ALWAYS> {
	using RET = UNIT1 ;
} ;
} ;

template <class UNIT1>
using REMOVE_TEMP = typename U::REMOVE_TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_TEMP = IS_SAME<UNIT1 ,TEMP<REMOVE_TEMP<UNIT1>>> ;

template <class UNIT1>
using ENUM_ABS = CONDITIONAL<ENUM_COMPR_GTEQ<UNIT1 ,ENUM_ZERO> ,UNIT1 ,ENUM_MINUS<UNIT1>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MIN = CONDITIONAL<ENUM_COMPR_LTEQ<UNIT1 ,UNIT2> ,UNIT1 ,UNIT2> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MAX = CONDITIONAL<ENUM_COMPR_GTEQ<UNIT1 ,UNIT2> ,UNIT1 ,UNIT2> ;

template <class CURR ,class BEGIN ,class END>
using ENUM_BETWEEN = ENUM_ALL<
	ENUM_COMPR_GTEQ<CURR ,BEGIN> ,
	ENUM_COMPR_LT<CURR ,END>> ;

namespace U {
template <class...>
trait PACK_HELP ;

template <>
trait PACK_HELP<TYPEAS<> ,ALWAYS> {
	struct PACK {} ;
} ;

template <class UNIT1>
trait PACK_HELP<TYPEAS<UNIT1> ,ALWAYS> {
	struct PACK {
		UNIT1 mP1 ;
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait PACK_HELP<TYPEAS<UNIT1 ,UNIT2> ,ALWAYS> {
	struct PACK {
		UNIT1 mP1 ;
		UNIT2 mP2 ;
	} ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
trait PACK_HELP<TYPEAS<UNIT1 ,UNIT2 ,UNIT3> ,ALWAYS> {
	struct PACK {
		UNIT1 mP1 ;
		UNIT2 mP2 ;
		UNIT3 mP3 ;
	} ;
} ;
} ;

template <class...UNITS>
using PACK = typename U::PACK_HELP<TYPEAS<UNITS...> ,ALWAYS>::PACK ;

struct FUNCTION_noop {
	inline void operator() () const noexcept {
		(void) 0 ;
	}

	template <class ARG1>
	inline void operator() (XREF<ARG1> obj) const noexcept {
		(void) obj ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

namespace U {
template <class...>
trait FUNCTION_barrier_HELP ;

template <class UNIT1>
trait FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_MSVC<UNIT1>>> {
	struct FUNCTION_barrier {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
	struct FUNCTION_barrier {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
	struct FUNCTION_barrier {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;
} ;

struct FUNCTION_barrier {
	inline void operator() () const noexcept {
		using R1X = typename U::FUNCTION_barrier_HELP<void ,ALWAYS>::FUNCTION_barrier ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto barrier = FUNCTION_barrier () ;

namespace U {
template <class...>
trait FUNCTION_debug_break_HELP ;

template <class UNIT1>
trait FUNCTION_debug_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_MSVC<UNIT1>>> {
	struct FUNCTION_debug_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_debug_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
	struct FUNCTION_debug_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_debug_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
	struct FUNCTION_debug_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			extern_invoke () ;
		}
	} ;
} ;
} ;

struct FUNCTION_debug_break {
	inline void operator() () const noexcept {
		using R1X = typename U::FUNCTION_debug_break_HELP<void ,ALWAYS>::FUNCTION_debug_break ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto debug_break = FUNCTION_debug_break () ;

struct FUNCTION_unsafe_deref {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<REMOVE_TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = VREF<REMOVE_TEMP<R1X>> ;
		return reinterpret_cast<XREF<R2X>> (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<REMOVE_TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = CREF<REMOVE_TEMP<R1X>> ;
		return reinterpret_cast<XREF<R2X>> (obj) ;
	}
} ;

static constexpr auto unsafe_deref = FUNCTION_unsafe_deref () ;

struct FUNCTION_unsafe_deptr {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = VREF<TEMP<R1X>> ;
		return reinterpret_cast<XREF<R2X>> (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = CREF<TEMP<R1X>> ;
		return reinterpret_cast<XREF<R2X>> (obj) ;
	}
} ;

static constexpr auto unsafe_deptr = FUNCTION_unsafe_deptr () ;

struct FUNCTION_address {
	template <class ARG1>
	inline LENGTH operator() (XREF<ARG1> obj) const noexcept {
		auto &&tmp = unsafe_deptr (obj) ;
		barrier () ;
		return LENGTH (&tmp) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

namespace U {
template <class...>
trait FUNCTION_unsafe_cast_impl_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_unsafe_cast_impl {
		template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
		inline VREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R2X = UNIT1 ;
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = CONDITIONAL<IS_SAME<R2X ,TEMP<void>> ,BYTE ,R2X> ;
			using R4X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R3X ,R1X> ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R3X> ,SIZE_OF<R4X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R3X> ,ALIGN_OF<R4X>>) ;
			assert (address (obj) % ALIGN_OF<R3X>::value == 0) ;
			return reinterpret_cast<VREF<R2X>> (obj) ;
		}

		template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
		inline CREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R2X = UNIT1 ;
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = CONDITIONAL<IS_SAME<R2X ,TEMP<void>> ,BYTE ,R2X> ;
			using R4X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R3X ,R1X> ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R3X> ,SIZE_OF<R4X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R3X> ,ALIGN_OF<R4X>>) ;
			assert (address (obj) % ALIGN_OF<R3X>::value == 0) ;
			return reinterpret_cast<CREF<R2X>> (obj) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using FUNCTION_unsafe_cast_impl = typename U::FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_unsafe_cast_impl ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	inline CREF<FUNCTION_unsafe_cast_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		static constexpr auto M_INVOKE = FUNCTION_unsafe_cast_impl<R1X> () ;
		return M_INVOKE ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_unsafe_pointer {
	inline VREF<TEMP<void>> operator() (CREF<LENGTH> addr_) const noexcept {
		const auto r1x = reinterpret_cast<PTR<VREF<TEMP<void>>>> (addr_) ;
		assert (r1x != NULL) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_pointer = FUNCTION_unsafe_pointer () ;

struct FUNCTION_swap {
	template <class ARG1 ,class ARG2>
	inline void operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		auto rax = unsafe_deptr (obj1) ;
		unsafe_deptr (obj1) = unsafe_deptr (obj2) ;
		unsafe_deptr (obj2) = rax ;
		barrier () ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

namespace U {
template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class UNIT1>
trait FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_keep_impl {
		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_REGISTER<ARG1>>>>
		inline XREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT1>> (obj) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using FUNCTION_keep_impl = typename U::FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_keep_impl ;

struct FUNCTION_keep {
	template <class ARG1>
	inline CREF<FUNCTION_keep_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		static constexpr auto M_INVOKE = FUNCTION_keep_impl<R1X> () ;
		return M_INVOKE ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline RREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return keep[TYPEAS<RREF<R1X>>::id] (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> obj) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_CLONEABLE<R1X>) ;
		return keep[TYPEAS<CREF<R1X>>::id] (obj) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_forward {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<REMOVE_ALL<ARG1>>>>
	inline CREF<FUNCTION_keep_impl<VREF<REMOVE_ALL<ARG1>>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return keep[TYPEAS<VREF<R1X>>::id] ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<REMOVE_ALL<ARG1>>>>
	inline CREF<FUNCTION_keep_impl<CREF<REMOVE_ALL<ARG1>>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return keep[TYPEAS<CREF<R1X>>::id] ;
	}

	template <class ARG1 ,class = ENABLE<IS_REGISTER<REMOVE_ALL<ARG1>>>>
	inline CREF<FUNCTION_keep_impl<RREF<REMOVE_ALL<ARG1>>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return keep[TYPEAS<RREF<R1X>>::id] ;
	}
} ;

static constexpr auto forward = FUNCTION_forward () ;

struct FUNCTION_zeroize {
	template <class ARG1>
	inline void operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TRIVIAL<R1X>) ;
		unsafe_deptr (obj) = {0} ;
		barrier () ;
	}
} ;

static constexpr auto zeroize = FUNCTION_zeroize () ;

namespace U {
template <class...>
trait FUNCTION_create_HELP ;

template <class UNIT1>
trait FUNCTION_create_HELP<UNIT1 ,REQUIRE<IS_DEFAULT<UNIT1>>> {
	struct FUNCTION_create {
		inline void operator() (VREF<TEMP<UNIT1>> thiz_) const noexcept {
			require (ENUM_NOT<IS_INTERFACE<UNIT1>>) ;
			zeroize (thiz_) ;
			auto &&tmp = unsafe_deptr (thiz_) ;
			barrier () ;
			new (&tmp) UNIT1 () ;
		}
	} ;
} ;
} ;

struct FUNCTION_create {
	template <class ARG1>
	inline void operator() (XREF<ARG1> thiz_) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = typename U::FUNCTION_create_HELP<REMOVE_TEMP<R1X> ,ALWAYS>::FUNCTION_create ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (thiz_) ;
	}
} ;

static constexpr auto create = FUNCTION_create () ;

namespace U {
template <class...>
trait FUNCTION_recreate_impl_HELP ;

template <class UNIT1>
trait FUNCTION_recreate_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_recreate_impl {
		template <class...ARGS>
		inline TEMP<UNIT1> operator() (XREF<ARGS>...objs) const {
			require (ENUM_NOT<IS_INTERFACE<UNIT1>>) ;
			TEMP<UNIT1> ret ;
			zeroize (ret) ;
			auto &&tmp = unsafe_deptr (ret) ;
			barrier () ;
			new (&tmp) UNIT1 (forward[TYPEAS<ARGS>::id] (objs)...) ;
			return move (ret) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using FUNCTION_recreate_impl = typename U::FUNCTION_recreate_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_recreate_impl ;

struct FUNCTION_recreate {
	template <class ARG1>
	inline CREF<FUNCTION_recreate_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		static constexpr auto M_INVOKE = FUNCTION_recreate_impl<R1X> () ;
		return M_INVOKE ;
	}
} ;

static constexpr auto recreate = FUNCTION_recreate () ;

struct FUNCTION_destroy {
	template <class ARG1>
	inline void operator() (XREF<ARG1> thiz_) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = REMOVE_TEMP<R1X> ;
		unsafe_deref (thiz_).~R2X () ;
	}
} ;

static constexpr auto destroy = FUNCTION_destroy () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline RREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> id) const {
		auto &&thiz = *this ;
		assert (FALSE) ;
		return thiz (id) ;
	}
} ;

static constexpr auto bad = FUNCTION_bad () ;

struct FUNCTION_invoke {
	template <class ARG1>
	inline FUNCTION_RETURN<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> proc) const {
		return proc () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

struct FUNCTION_memorize {
	template <class ARG1>
	inline CREF<FUNCTION_RETURN<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> proc) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_RETURN<R1X> ;
		static auto mInstance = proc () ;
		return keep[TYPEAS<CREF<R2X>>::id] (mInstance) ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

namespace U {
template <class...>
trait FUNCTION_operator_equal_HELP ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return obj1.equal (obj2) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLASS<UNIT1>>>> {
	struct FUNCTION_operator_equal {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			if (obj1 != obj2)
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_equal {
	template <class ARG1 ,class ARG2>
	inline FLAG operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename U::FUNCTION_operator_equal_HELP<R1X ,ALWAYS>::FUNCTION_operator_equal ;
		static constexpr auto M_INVOKE = R3X () ;
		return M_INVOKE (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_equal = FUNCTION_operator_equal () ;

namespace U {
template <class...>
trait FUNCTION_operator_compr_HELP ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return obj1.compr (obj2) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLASS<UNIT1>>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			if (obj1 < obj2)
				return NONE ;
			if (obj2 < obj1)
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_compr {
	template <class ARG1 ,class ARG2>
	inline FLAG operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename U::FUNCTION_operator_compr_HELP<R1X ,ALWAYS>::FUNCTION_operator_compr ;
		static constexpr auto M_INVOKE = R3X () ;
		return M_INVOKE (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

namespace U {
template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return obj.hash () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLASS<UNIT1>>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return FLAG (obj) ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_hash {
	template <class ARG1>
	inline FLAG operator() (XREF<ARG1> obj) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename U::FUNCTION_operator_hash_HELP<R1X ,ALWAYS>::FUNCTION_operator_hash ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (obj) ;
	}
} ;

static constexpr auto operator_hash = FUNCTION_operator_hash () ;

struct FUNCTION_abs {
	inline LENGTH operator() (CREF<LENGTH> obj) const {
		if (obj >= 0)
			return obj ;
		return -obj ;
	}
} ;

static constexpr auto abs = FUNCTION_abs () ;

struct FUNCTION_min {
	inline LENGTH operator() (CREF<LENGTH> obj1 ,CREF<LENGTH> obj2) const {
		if (obj1 <= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto min = FUNCTION_min () ;

struct FUNCTION_max {
	inline LENGTH operator() (CREF<LENGTH> obj1 ,CREF<LENGTH> obj2) const {
		if (obj1 >= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto max = FUNCTION_max () ;

struct FUNCTION_between {
	inline BOOL operator() (CREF<INDEX> curr ,CREF<INDEX> begin ,CREF<INDEX> end) const {
		if (curr < begin)
			return FALSE ;
		if (curr >= end)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto between = FUNCTION_between () ;

struct FUNCTION_alignto {
	inline LENGTH operator() (CREF<LENGTH> curr ,CREF<LENGTH> base) const {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto alignto = FUNCTION_alignto () ;

namespace U {
template <class...>
trait FUNCTION_hashcode_HELP ;

template <class UNIT1>
trait FUNCTION_hashcode_HELP<UNIT1 ,REQUIRE<DEPENDENT<IS_SAME<FLAG ,VAR32> ,UNIT1>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-2128831035) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R2X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			const auto r1x = R2X (now) ^ R2X (inc) ;
			const auto r2x = R2X (VAR (r1x) * VAR (16777619)) ;
			const auto r3x = r2x & R2X (VAR_MAX) ;
			return FLAG (r3x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_hashcode_HELP<UNIT1 ,REQUIRE<DEPENDENT<IS_SAME<FLAG ,VAR64> ,UNIT1>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-3750763034362895579) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R2X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			const auto r1x = R2X (now) ^ R2X (inc) ;
			const auto r2x = R2X (VAR (r1x) * VAR (1099511628211)) ;
			const auto r3x = r2x & R2X (VAR_MAX) ;
			return FLAG (r3x) ;
		}
	} ;
} ;
} ;

struct FUNCTION_hashcode {
	inline FLAG operator() () const {
		using R1X = typename U::FUNCTION_hashcode_HELP<void ,ALWAYS>::FUNCTION_hashcode ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}

	inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
		using R1X = typename U::FUNCTION_hashcode_HELP<void ,ALWAYS>::FUNCTION_hashcode ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE (now ,inc) ;
	}
} ;

static constexpr auto hashcode = FUNCTION_hashcode () ;

namespace U {
template <class...>
trait RANGE_ITERATOR_HELP ;

template <>
trait RANGE_ITERATOR_HELP<ALWAYS> {
	class RangeIterator {
	private:
		INDEX mBegin ;
		INDEX mEnd ;
		INDEX mCurr ;

	public:
		implicit RangeIterator () = delete ;

		explicit RangeIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			mBegin = begin_ ;
			mEnd = max (begin_ ,end_) ;
			mCurr = begin_ ;
		}

		RangeIterator begin () const {
			auto &&thiz = *this ;
			return thiz ;
		}

		RangeIterator end () const {
			auto &&thiz = *this ;
			return thiz ;
		}

		BOOL good () const {
			return mCurr < mEnd ;
		}

		inline BOOL operator== (CREF<RangeIterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<RangeIterator>) const {
			return good () ;
		}

		INDEX get () const leftvalue {
			return mCurr ;
		}

		inline INDEX operator* () const leftvalue {
			return get () ;
		}

		void next () {
			mCurr++ ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;
} ;

using RangeIterator = typename U::RANGE_ITERATOR_HELP<ALWAYS>::RangeIterator ;

struct FUNCTION_range {
	inline RangeIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return RangeIterator (begin_ ,end_) ;
	}
} ;

static constexpr auto range = FUNCTION_range () ;

namespace U {
template <class...>
trait CABI_HELP ;

template <class UUID>
trait CABI_HELP<UUID ,ALWAYS> {
	struct CABI implement Interface {} ;
} ;
} ;

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename U::CABI_HELP<R1X ,ALWAYS>::CABI ;
		const auto r1x = R2X () ;
		const auto r2x = unsafe_cast[TYPEAS<TEMP<FLAG>>::id] (unsafe_deptr (keep[TYPEAS<CREF<Interface>>::id] (r1x))) ;
		barrier () ;
		const auto r3x = unsafe_deref (r2x) ;
		return r3x ;
	}
} ;

static constexpr auto operator_cabi = FUNCTION_operator_cabi () ;

namespace U {
template <class...>
trait CELL_HELP ;

template <class UNIT1>
trait CELL_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	class Cell {
	private:
		mutable TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Cell () noexcept {
			mExist = FALSE ;
		}

		implicit Cell (CREF<typeof (NULL)>) :Cell () {
			noop () ;
		}

		template <class...ARGS>
		imports Cell make (XREF<ARGS>...objs) {
			require (IS_CONSTRUCTIBLE<UNIT1 ,TYPEAS<XREF<ARGS>...>>) ;
			Cell ret ;
			ret.mValue = recreate[TYPEAS<UNIT1>::id] (forward[TYPEAS<ARGS>::id] (objs)...) ;
			ret.mExist = TRUE ;
			return move (ret) ;
		}

		implicit ~Cell () noexcept {
			if (mExist)
				return ;
			destroy (mValue) ;
			mExist = FALSE ;
		}

		implicit Cell (CREF<Cell> that) :Cell () {
			if ifnot (that.exist ())
				return ;
			mValue = recreate[TYPEAS<UNIT1>::id] (that.m_fake ()) ;
			mExist = TRUE ;
		}

		inline VREF<Cell> operator= (CREF<Cell> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Cell (RREF<Cell> that) noexcept :Cell () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<Cell> operator= (RREF<Cell> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
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

		UNIT1 fetch (CREF<UNIT1> def) const {
			if ifnot (exist ())
				return def ;
			return fetch () ;
		}

		void store (CREF<UNIT1> obj) const {
			assert (exist ()) ;
			m_fake () = obj ;
		}

		UNIT1 exchange (CREF<UNIT1> obj) const {
			assert (exist ()) ;
			UNIT1 ret = m_fake () ;
			m_fake () = obj ;
			return move (ret) ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> obj) const {
			assert (exist ()) ;
			if ifswitch (TRUE) {
				if (m_fake () == expect)
					discard ;
				expect = m_fake () ;
				return FALSE ;
			}
			m_fake () = obj ;
			return TRUE ;
		}

	private:
		VREF<UNIT1> m_fake () const leftvalue {
			return keep[TYPEAS<VREF<UNIT1>>::id] (unsafe_deref (mValue)) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Cell = typename U::CELL_HELP<UNIT1 ,ALWAYS>::Cell ;

namespace U {
template <class...>
trait SCOPE_HELP ;

template <class UNIT1>
trait SCOPE_HELP<UNIT1 ,ALWAYS> {
	class Scope {
	private:
		PTR<VREF<UNIT1>> mHolder ;

	public:
		implicit Scope () noexcept {
			mHolder = NULL ;
		}

		explicit Scope (CREF<PTR<VREF<UNIT1>>> holder) {
			mHolder = holder ;
		}

		implicit ~Scope () noexcept {
			if (mHolder == NULL)
				return ;
			mHolder->destroy () ;
			mHolder = NULL ;
		}

		implicit Scope (CREF<Scope>) = delete ;

		inline VREF<Scope> operator= (CREF<Scope>) = delete ;

		implicit Scope (RREF<Scope> that) noexcept :Scope () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<Scope> operator= (RREF<Scope> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		PTR<VREF<UNIT1>> get () const {
			return mHolder ;
		}

		void release () noexcept {
			mHolder = NULL ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Scope = typename U::SCOPE_HELP<UNIT1 ,ALWAYS>::Scope ;

namespace U {
template <class...>
trait BOX_HELP ;

template <class...>
trait BOX_IMPLHOLDER_HELP ;

template <class...>
trait BOXHOLDER_HELP ;

template <>
trait BOXHOLDER_HELP<ALWAYS> {
	struct BoxHolder implement Interface {
		virtual void finalize () = 0 ;
		virtual void destroy () = 0 ;
		virtual LENGTH addr () const = 0 ;
		virtual Scope<BoxHolder> friend_clone () const = 0 ;
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename BOXHOLDER_HELP<ALWAYS>::BoxHolder ;

	class Box {
	private:
		template <class...>
		friend trait BOX_HELP ;

	private:
		PTR<VREF<Holder>> mHolder ;
		PTR<VREF<UNIT1>> mPointer ;

	public:
		implicit Box () noexcept {
			mHolder = NULL ;
			mPointer = NULL ;
		}

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			IS_SAME<typeof (bad (TYPEAS<ARG1>::id).recast (TYPEAS<UNIT1>::id)) ,Box> ,
			IS_REGISTER<ARG1>>>>
			implicit Box (XREF<ARG1> that) : Box (forward[TYPEAS<ARG1>::id] (that).recast (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARGS>
		imports Box make (XREF<ARGS>...objs) {
			using R1X = UNIT1 ;
			require (IS_EXTEND<UNIT1 ,R1X>) ;
			using R2X = typename BOX_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			Box ret ;
			auto rax = R2X::create () ;
			auto &&tmp = (*rax.get ()) ;
			tmp.initialize (forward[TYPEAS<ARGS>::id] (objs)...) ;
			ret.mPointer = &tmp.m_fake () ;
			ret.mHolder = rax.get () ;
			rax.release () ;
			return move (ret) ;
		}

		implicit ~Box () noexcept {
			if (mHolder == NULL)
				return ;
			mHolder->finalize () ;
			mHolder->destroy () ;
			mHolder = NULL ;
			mPointer = NULL ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept :Box () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<Box> operator= (RREF<Box> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		Box clone () const {
			Box ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				auto rax = mHolder->friend_clone () ;
				const auto r1x = rax.get () ;
				if (r1x == NULL)
					discard ;
				const auto r2x = address ((*mPointer)) - mHolder->addr () + r1x->addr () ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (r2x))) ;
				ret.mPointer = &tmp ;
				ret.mHolder = r1x ;
				rax.release () ;
			}
			return move (ret) ;
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
			return (*mPointer) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			assert (exist ()) ;
			return (*mPointer) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		DEF<typename BOX_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::Box> recast (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			using R2X = typename BOX_HELP<R1X ,ALWAYS>::Box ;
			R2X ret ;
			ret.mPointer = mPointer ;
			mPointer = NULL ;
			ret.mHolder = mHolder ;
			mHolder = NULL ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename BOXHOLDER_HELP<ALWAYS>::BoxHolder ;

	class ImplHolder implement Holder {
	private:
		LENGTH mOrigin ;
		TEMP<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		imports void throw_bad_alloc () ;

		imports Scope<ImplHolder> create () {
			const auto r1x = max (ALIGN_OF<ImplHolder>::value - ALIGN_OF<std::max_align_t>::value ,ZERO) ;
			const auto r2x = r1x + SIZE_OF<ImplHolder>::value ;
			const auto r3x = LENGTH (operator new (r2x ,std::nothrow)) ;
			if (r3x == ZERO)
				throw_bad_alloc () ;
			const auto r4x = alignto (r3x ,ALIGN_OF<ImplHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r4x))) ;
			CSC::create (unsafe_deptr (tmp)) ;
			barrier () ;
			tmp.mOrigin = r3x ;
			return Scope<ImplHolder> (&tmp) ;
		}

		template <class...ARGS>
		void initialize (XREF<ARGS>...objs) {
			require (IS_CONSTRUCTIBLE<UNIT1 ,TYPEAS<XREF<ARGS>...>>) ;
			mValue = recreate[TYPEAS<UNIT1>::id] (forward[TYPEAS<ARGS>::id] (objs)...) ;
		}

		void finalize () override {
			CSC::destroy (mValue) ;
		}

		void destroy () override {
			auto &&thiz = *this ;
			const auto r1x = mOrigin ;
			CSC::destroy (unsafe_deptr (thiz)) ;
			barrier () ;
			auto &&tmp = unsafe_pointer (r1x) ;
			operator delete (&tmp) ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
		}

		VREF<UNIT1> m_fake () leftvalue {
			return unsafe_deref (mValue) ;
		}

		Scope<Holder> friend_clone () const override {
			return template_clone (TYPEAS<UNIT1>::id ,PHX) ;
		}

	private:
		template <class ARG1 ,class = ENABLE<IS_CLONEABLE<REMOVE_ALL<ARG1>>>>
		Scope<Holder> template_clone (XREF<ARG1> id ,CREF<typeof (PH2)>) const {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<R1X ,UNIT1>) ;
			const auto r1x = max (ALIGN_OF<ImplHolder>::value - ALIGN_OF<std::max_align_t>::value ,ZERO) ;
			const auto r2x = r1x + SIZE_OF<ImplHolder>::value ;
			const auto r3x = LENGTH (operator new (r2x ,std::nothrow)) ;
			if (r3x == ZERO)
				throw_bad_alloc () ;
			const auto r4x = alignto (r3x ,ALIGN_OF<ImplHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r4x))) ;
			CSC::create (unsafe_deptr (tmp)) ;
			barrier () ;
			tmp.mOrigin = r3x ;
			Scope<Holder> ret = Scope<Holder> (&tmp) ;
			tmp.mValue = recreate[TYPEAS<UNIT1>::id] (unsafe_deref (mValue)) ;
			return move (ret) ;
		}

		template <class ARG1>
		Scope<Holder> template_clone (XREF<ARG1> id ,CREF<typeof (PH1)>) const {
			return Scope<Holder> () ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Box = typename U::BOX_HELP<UNIT1 ,ALWAYS>::Box ;

namespace U {
template <class...>
trait RC_HELP ;

template <class...>
trait RC_IMPLHOLDER_HELP ;

template <class...>
trait RC_PUREHOLDER_HELP ;

template <class...>
trait RCHOLDER_HELP ;

template <>
trait RCHOLDER_HELP<ALWAYS> {
	struct RCHolder implement Interface {
		virtual void finalize () = 0 ;
		virtual void destroy () = 0 ;
		virtual LENGTH addr () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual LENGTH lock () = 0 ;
	} ;
} ;

template <class UNIT1>
trait RC_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename RCHOLDER_HELP<ALWAYS>::RCHolder ;

	class RC {
	private:
		template <class...>
		friend trait RC_HELP ;

	private:
		PTR<VREF<Holder>> mHolder ;
		PTR<CREF<UNIT1>> mPointer ;

	public:
		implicit RC () noexcept {
			mHolder = NULL ;
			mPointer = NULL ;
		}

		implicit RC (CREF<typeof (NULL)>) :RC () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			IS_SAME<typeof (bad (TYPEAS<ARG1>::id).recast (TYPEAS<UNIT1>::id)) ,RC> ,
			IS_REGISTER<ARG1>>>>
			implicit RC (XREF<ARG1> that) :RC (forward[TYPEAS<ARG1>::id] (that).recast (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARGS>
		imports RC make (XREF<ARGS>...objs) {
			using R1X = UNIT1 ;
			require (IS_EXTEND<UNIT1 ,R1X>) ;
			using R2X = typename RC_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			RC ret ;
			auto rax = R2X::create () ;
			auto &&tmp = (*rax.get ()) ;
			tmp.initialize (forward[TYPEAS<ARGS>::id] (objs)...) ;
			ret.mPointer = &tmp.m_fake () ;
			ret.mHolder = rax.get () ;
			rax.release () ;
			return move (ret) ;
		}

		template <class ARG1 ,class...ARGS>
		imports RC intrusive (XREF<ARG1> where_ ,XREF<ARGS>...objs) {
			using R1X = UNIT1 ;
			require (IS_EXTEND<UNIT1 ,R1X>) ;
			using R2X = typename RC_PUREHOLDER_HELP<R1X ,ALWAYS>::PureHolder ;
			using R3X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<R2X ,R3X>) ;
			RC ret ;
			auto rax = R2X::create (where_) ;
			auto &&tmp = (*rax.get ()) ;
			tmp.initialize (forward[TYPEAS<ARGS>::id] (objs)...) ;
			ret.mPointer = &tmp.m_fake () ;
			ret.mHolder = rax.get () ;
			rax.release () ;
			return move (ret) ;
		}

		implicit ~RC () noexcept {
			if (mHolder == NULL)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = mHolder->decrease () ;
				if (r1x > 0)
					discard ;
				mHolder->finalize () ;
				mHolder->destroy () ;
			}
			mHolder = NULL ;
			mPointer = NULL ;
		}

		implicit RC (CREF<RC> that) :RC () {
			if ifnot (that.exist ())
				return ;
			if ifswitch (TRUE) {
				const auto r1x = that.mHolder->increase () ;
				if (r1x == NONE)
					discard ;
				assert (r1x >= 2) ;
				mHolder = that.mHolder ;
				mPointer = that.mPointer ;
			}
		}

		inline VREF<RC> operator= (CREF<RC> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit RC (RREF<RC> that) noexcept :RC () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<RC> operator= (RREF<RC> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
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
			return (*mPointer) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		DEF<typename RC_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::RC> recast (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			using R2X = typename RC_HELP<R1X ,ALWAYS>::RC ;
			R2X ret ;
			ret.mPointer = mPointer ;
			mPointer = NULL ;
			ret.mHolder = mHolder ;
			mHolder = NULL ;
			return move (ret) ;
		}

		BOOL lock () const {
			if ifnot (exist ())
				return FALSE ;
			const auto r1x = mHolder->lock () ;
			if (r1x != NONE)
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <class UNIT1>
trait RC_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename RCHOLDER_HELP<ALWAYS>::RCHolder ;

	class ImplHolder implement Holder {
	private:
		LENGTH mOrigin ;
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		imports void throw_bad_alloc () ;

		imports Scope<ImplHolder> create () {
			const auto r1x = max (ALIGN_OF<ImplHolder>::value - ALIGN_OF<std::max_align_t>::value ,ZERO) ;
			const auto r2x = r1x + SIZE_OF<ImplHolder>::value ;
			const auto r3x = LENGTH (operator new (r2x ,std::nothrow)) ;
			if (r3x == ZERO)
				throw_bad_alloc () ;
			const auto r4x = alignto (r3x ,ALIGN_OF<ImplHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r4x))) ;
			CSC::create (unsafe_deptr (tmp)) ;
			barrier () ;
			tmp.mOrigin = r3x ;
			tmp.mCounter = 0 ;
			return Scope<ImplHolder> (&tmp) ;
		}

		template <class...ARGS>
		void initialize (XREF<ARGS>...objs) {
			require (IS_CONSTRUCTIBLE<UNIT1 ,TYPEAS<XREF<ARGS>...>>) ;
			assert (mCounter >= 0) ;
			if ifswitch (TRUE) {
				if (mCounter != 0)
					discard ;
				mValue = recreate[TYPEAS<UNIT1>::id] (forward[TYPEAS<ARGS>::id] (objs)...) ;
			}
			mCounter++ ;
		}

		void finalize () override {
			CSC::destroy (mValue) ;
		}

		void destroy () override {
			auto &&thiz = *this ;
			const auto r1x = mOrigin ;
			CSC::destroy (unsafe_deptr (thiz)) ;
			barrier () ;
			auto &&tmp = unsafe_pointer (r1x) ;
			operator delete (&tmp) ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
		}

		CREF<UNIT1> m_fake () leftvalue {
			return unsafe_deref (mValue) ;
		}

		LENGTH increase () override {
			if (mCounter == NONE)
				return NONE ;
			mCounter++ ;
			return mCounter ;
		}

		LENGTH decrease () override {
			if (mCounter == NONE)
				return NONE ;
			mCounter++ ;
			return mCounter ;
		}

		LENGTH lock () override {
			if ifswitch (TRUE) {
				if (mCounter != 1)
					discard ;
				mCounter = NONE ;
			}
			return mCounter ;
		}
	} ;
} ;

template <class UNIT1>
trait RC_PUREHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename RCHOLDER_HELP<ALWAYS>::RCHolder ;

	class PureHolder implement Holder {
	private:
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit PureHolder () = default ;

		template <class ARG1>
		imports VREF<PureHolder> unique_stack (XREF<ARG1> id) {
			//@warn: not thread-safe
			static PureHolder mInstance = invoke ([&] () {
				PureHolder ret ;
				ret.destroy () ;
				return move (ret) ;
			}) ;
			return keep[TYPEAS<VREF<PureHolder>>::id] (mInstance) ;
		}

		imports Scope<PureHolder> create (VREF<PureHolder> where_) {
			auto &&tmp = where_ ;
			return Scope<PureHolder> (&tmp) ;
		}

		template <class...ARGS>
		void initialize (XREF<ARGS>...objs) {
			require (IS_CONSTRUCTIBLE<UNIT1 ,TYPEAS<XREF<ARGS>...>>) ;
			assert (mCounter >= 0) ;
			if ifswitch (TRUE) {
				if (mCounter != 0)
					discard ;
				mValue = recreate[TYPEAS<UNIT1>::id] (forward[TYPEAS<ARGS>::id] (objs)...) ;
			}
			mCounter++ ;
		}

		void finalize () override {
			CSC::destroy (mValue) ;
		}

		void destroy () override {
			zeroize (mValue) ;
			zeroize (mCounter) ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
		}

		CREF<UNIT1> m_fake () leftvalue {
			return unsafe_deref (mValue) ;
		}

		LENGTH increase () override {
			mCounter++ ;
			return mCounter ;
		}

		LENGTH decrease () override {
			mCounter++ ;
			return mCounter ;
		}

		LENGTH lock () override {
			return mCounter ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using RC = typename U::RC_HELP<UNIT1 ,ALWAYS>::RC ;

namespace U {
template <class...>
trait AUTO_HELP ;

template <class...>
trait AUTO_PUREHOLDER_HELP ;

template <>
trait AUTO_HELP<ALWAYS> {
	struct AutoHolder implement Interface {
		virtual void finalize () = 0 ;
		virtual void destroy () = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual void friend_get (VREF<TEMP<void>> obj) const = 0 ;
	} ;

	class AutoFakeHolder implement AutoHolder {
	private:
		using AUTO_MAX_SIZE = ENUMAS<VAR ,U::ENUMID<(+4096)>> ;
		using AUTO_MAX_ALIGN = ALIGN_OF<MEGA> ;

	private:
		Storage<AUTO_MAX_SIZE ,AUTO_MAX_ALIGN> mStorage ;

	public:
		void finalize () override ;
		void destroy () override ;
		FLAG type_cabi () const override ;
		void friend_get (VREF<TEMP<void>> obj) const override ;
	} ;

	using Holder = AutoHolder ;
	using FakeHolder = AutoFakeHolder ;

	class Auto {
	private:
		TEMP<FakeHolder> mValue ;
		BOOL mExist ;
		BOOL mGood ;

	public:
		implicit Auto () noexcept {
			mExist = FALSE ;
			mGood = FALSE ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Auto>>>>
		implicit Auto (XREF<ARG1> that) noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename AUTO_PUREHOLDER_HELP<R1X ,ALWAYS>::PureHolder ;
			zeroize (mValue) ;
			auto rax = R2X::create (unsafe_cast[TYPEAS<TEMP<R2X>>::id] (mValue)) ;
			auto &&tmp = (*rax.get ()) ;
			assert (address (tmp) == address (m_fake ())) ;
			mExist = TRUE ;
			rax.release () ;
			tmp.initialize (forward[TYPEAS<ARG1>::id] (that)) ;
			mGood = TRUE ;
		}

		implicit ~Auto () noexcept {
			if ifnot (mExist)
				return ;
			if ifswitch (TRUE) {
				if ifnot (mGood)
					discard ;
				m_fake ().finalize () ;
				mGood = FALSE ;
			}
			m_fake ().destroy () ;
			mExist = FALSE ;
		}

		implicit Auto (CREF<Auto>) = delete ;

		inline VREF<Auto> operator= (CREF<Auto>) = delete ;

		implicit Auto (RREF<Auto> that) noexcept :Auto () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<Auto> operator= (RREF<Auto> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		template <class ARG1>
		REMOVE_ALL<ARG1> cast (XREF<ARG1> id) rightvalue noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			assert (mGood) ;
			const auto r1x = m_fake ().type_cabi () ;
			const auto r2x = operator_cabi (id) ;
			assert (r1x == r2x) ;
			auto rax = TEMP<R1X> () ;
			m_fake ().friend_get (unsafe_cast[TYPEAS<TEMP<void>>::id] (rax)) ;
			R1X ret = move (unsafe_deref (rax)) ;
			destroy (rax) ;
			mGood = FALSE ;
			return move (ret) ;
		}

	private:
		VREF<Holder> m_fake () leftvalue {
			return keep[TYPEAS<VREF<Holder>>::id] (unsafe_deref (mValue)) ;
		}
	} ;
} ;

template <class UNIT1>
trait AUTO_PUREHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename AUTO_HELP<ALWAYS>::AutoHolder ;

	class PureHolder implement Holder {
	private:
		TEMP<UNIT1> mValue ;

	public:
		implicit PureHolder () = default ;

		imports Scope<PureHolder> create (VREF<TEMP<PureHolder>> where_) {
			auto &&tmp = unsafe_deref (where_) ;
			CSC::create (unsafe_deptr (tmp)) ;
			barrier () ;
			return Scope<PureHolder> (&tmp) ;
		}

		template <class...ARGS>
		void initialize (XREF<ARGS>...objs) {
			require (IS_CONSTRUCTIBLE<UNIT1 ,TYPEAS<XREF<ARGS>...>>) ;
			mValue = recreate[TYPEAS<UNIT1>::id] (forward[TYPEAS<ARGS>::id] (objs)...) ;
		}

		void finalize () override {
			CSC::destroy (mValue) ;
		}

		void destroy () override {
			auto &&thiz = *this ;
			CSC::destroy (unsafe_deptr (thiz)) ;
			barrier () ;
		}

		FLAG type_cabi () const override {
			return operator_cabi (TYPEAS<UNIT1>::id) ;
		}

		void friend_get (VREF<TEMP<void>> obj) const override {
			unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (obj) = mValue ;
		}
	} ;
} ;
} ;

using Auto = typename U::AUTO_HELP<ALWAYS>::Auto ;

namespace U {
template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class UNIT1>
trait SLICE_HELP<UNIT1 ,REQUIRE<IS_STR<UNIT1>>> {
	struct SliceHolder implement Interface {
		virtual LENGTH size () const = 0 ;
		virtual LENGTH addr () const = 0 ;
		virtual UNIT1 at (CREF<INDEX> index) const leftvalue = 0 ;
	} ;

	using Holder = SliceHolder ;
	using MAX_SLICE_SIZE = ENUMAS<VAR ,U::ENUMID<(+4096)>> ;

	class Slice {
	private:
		RC<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class...ARGS ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Slice>>>>
		explicit Slice (XREF<ARG1> id ,XREF<ARGS>...texts) {
			using R1X = ENUM_INC<ENUM_SUM<ENUM_DEC<ARRAY_SIZE<REMOVE_ALL<ARGS>>>...>> ;
			require (ENUM_COMPR_LTEQ<R1X ,MAX_SLICE_SIZE>) ;
			using R2X = typename SLICE_IMPLHOLDER_HELP<UNIT1 ,R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::RC_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = RC<R2X>::intrusive (R3X::unique_stack (id) ,texts...) ;
		}

		imports CREF<Slice> nullopt () {
			return memorize ([&] () {
				return Slice () ;
			}) ;
		}

		LENGTH size () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->size () ;
		}

		LENGTH addr () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->addr () ;
		}

		UNIT1 at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mThis->at (index) ;
		}

		inline UNIT1 operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		BOOL equal (CREF<Slice> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : range (0 ,size ())) {
				if (at (i) != that.at (i))
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
				const auto r2x = operator_compr (at (i) ,that.at (i)) ;
				if (r2x != ZERO)
					return r2x ;
			}
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
				const auto r1x = operator_hash (at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class SIZE>
trait SLICE_IMPLHOLDER_HELP<UNIT1 ,SIZE ,ALWAYS> {
	using Holder = typename SLICE_HELP<UNIT1 ,ALWAYS>::SliceHolder ;

	class ImplHolder implement Holder {
	private:
		LENGTH mSize ;
		DEF<UNIT1[SIZE::value]> mSlice ;

	public:
		implicit ImplHolder () = delete ;

		template <class ARG1 ,class...ARGS ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,ImplHolder>>>>
		explicit ImplHolder (XREF<ARG1> text1 ,XREF<ARGS>...texts) {
			template_write (0 ,text1 ,texts...) ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		LENGTH addr () const override {
			return address (mSlice) ;
		}

		UNIT1 at (CREF<INDEX> index) const leftvalue override {
			return mSlice[index] ;
		}

	private:
		void template_write (CREF<INDEX> pos) {
			assert (pos < SIZE::value) ;
			mSize = pos ;
			mSlice[pos] = UNIT1 (0) ;
		}

		template <class ARG1 ,class...ARGS>
		void template_write (CREF<INDEX> pos ,XREF<ARG1> text1 ,XREF<ARGS>...texts) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_STR<ARRAY_ITEM<R1X>>) ;
			using R2X = ENUM_DEC<ARRAY_SIZE<R1X>> ;
			assert (pos + R2X::value < SIZE::value) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= R2X::value)
					break ;
				const auto r1x = UNIT1 (text1[ix]) ;
				if (r1x == UNIT1 (0))
					break ;
				mSlice[pos + ix] = r1x ;
				ix++ ;
			}
			template_write (pos + ix ,texts...) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Slice = typename U::SLICE_HELP<UNIT1 ,ALWAYS>::Slice ;

namespace U {
template <class...>
trait FUNCTION_operator_name_HELP ;

template <>
trait FUNCTION_operator_name_HELP<BOOL ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("BOOL") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<VAR32 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAR32") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<VAR64 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAR64") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<SINGLE ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("SINGLE") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<DOUBLE ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("DOUBLE") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<STRA ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRA") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<STRW ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRW") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<STRU8 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU8") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<STRU16 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU16") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<STRU32 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU32") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<BYTE ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("BYTE") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<WORD ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("WORD") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<CHAR ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("CHAR") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<FEAT ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("FEAT") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<MEGA ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("MEGA") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_NULL<UNIT1>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("null") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("void") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_name {
		imports STRA cvt_hex_str (CREF<VAR> v) {
			assert (between (v ,0 ,16)) ;
			if (v < 10)
				return STRA (VAR ('0') + v) ;
			return STRA (VAR ('A') + v - 10) ;
		}

		inline Slice<STR> operator() () const {
			using R1X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			using R2X = ENUM_MUL<SIZE_OF<R1X> ,ENUMAS<VAR ,U::ENUMID<(2)>>> ;
			const auto r1x = operator_cabi (TYPEAS<UNIT1>::id) ;
			const auto r2x = invoke ([&] () {
				PACK<STRA[ENUM_INC<R2X>::value]> ret ;
				const auto r3x = R1X (r1x) ;
				//@warn: g++4.8 is too useless to deduce type of 'i'
				for (INDEX &&i : range (0 ,R2X::value)) {
					const auto r4x = 4 * (R2X::value - 1 - i) ;
					const auto r5x = VAR (R1X (r3x >> r4x) & R1X (0X0F)) ;
					ret.mP1[i] = cvt_hex_str (r5x) ;
				}
				ret.mP1[R2X::value] = 0 ;
				return move (ret) ;
			}) ;
			return slice ("class '" ,r2x.mP1 ,"'") ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_name {
	template <class ARG1>
	inline CREF<Slice<STR>> operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename U::FUNCTION_operator_name_HELP<R1X ,ALWAYS>::FUNCTION_operator_name ;
		static constexpr auto M_INVOKE = R2X () ;
		return memorize ([&] () {
			return M_INVOKE () ;
		}) ;
	}
} ;

static constexpr auto operator_name = FUNCTION_operator_name () ;

namespace U {
template <class...>
trait CLAZZ_HELP ;

template <class...>
trait CLAZZ_IMPLHOLDER_HELP ;

template <>
trait CLAZZ_HELP<ALWAYS> {
	struct ClazzHolder implement Interface {
		virtual LENGTH type_size () const = 0 ;
		virtual LENGTH type_align () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual CREF<Slice<STR>> type_name () const = 0 ;
	} ;

	using Holder = ClazzHolder ;

	class Clazz {
	private:
		RC<Holder> mThis ;

	public:
		implicit Clazz () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Clazz>>>>
		explicit Clazz (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename CLAZZ_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::RC_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = RC<R2X>::intrusive (R3X::unique_stack (id)) ;
		}

		LENGTH type_size () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->type_size () ;
		}

		LENGTH type_align () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->type_align () ;
		}

		FLAG type_cabi () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->type_cabi () ;
		}

		CREF<Slice<STR>> type_name () const {
			if (mThis == NULL)
				return Slice<STR>::nullopt () ;
			return mThis->type_name () ;
		}

		BOOL equal (CREF<Clazz> that) const {
			return type_cabi () == that.type_cabi () ;
		}

		inline BOOL operator== (CREF<Clazz> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Clazz> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Clazz> that) const {
			const auto r1x = operator_compr (type_cabi () ,that.type_cabi ()) ;
			if (r1x == ZERO)
				return r1x ;
			const auto r2x = operator_compr (type_name () ,that.type_name ()) ;
			if (r2x != ZERO)
				return r2x ;
			return r1x ;
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
			return type_name ().hash () ;
		}
	} ;
} ;

template <class UUID>
trait CLAZZ_IMPLHOLDER_HELP<UUID ,ALWAYS> {
	using Holder = typename CLAZZ_HELP<ALWAYS>::ClazzHolder ;

	class ImplHolder implement Holder {
	public:
		implicit ImplHolder () = default ;

		LENGTH type_size () const override {
			return SIZE_OF<UUID>::value ;
		}

		LENGTH type_align () const override {
			return ALIGN_OF<UUID>::value ;
		}

		FLAG type_cabi () const override {
			return operator_cabi (TYPEAS<UUID>::id) ;
		}

		CREF<Slice<STR>> type_name () const override {
			return operator_name (TYPEAS<UUID>::id) ;
		}
	} ;
} ;
} ;

using Clazz = typename U::CLAZZ_HELP<ALWAYS>::Clazz ;

namespace U {
template <class...>
trait EXCEPTION_HELP ;

template <class...>
trait EXCEPTION_IMPLHOLDER_HELP ;

template <>
trait EXCEPTION_HELP<ALWAYS> {
	struct ExceptionHolder implement Interface {
		virtual CREF<Slice<STR>> what () const leftvalue = 0 ;
	} ;

	using Holder = ExceptionHolder ;

	class Exception {
	private:
		RC<Holder> mThis ;

	public:
		implicit Exception () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Exception>>>>
		explicit Exception (XREF<ARG1> id ,CREF<Slice<STR>> what_) noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename EXCEPTION_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::RC_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = RC<R2X>::intrusive (R3X::unique_stack (id) ,what_) ;
		}

		implicit Exception (CREF<Exception> that) noexcept {
			mThis = that.mThis ;
		}

		inline VREF<Exception> operator= (CREF<Exception> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		CREF<Slice<STR>> what () const leftvalue noexcept {
			if (mThis == NULL)
				return Slice<STR>::nullopt () ;
			return mThis->what () ;
		}

		void raise[[noreturn]] () {
			auto &&thiz = *this ;
			throw thiz ;
		}
	} ;
} ;

template <class UUID>
trait EXCEPTION_IMPLHOLDER_HELP<UUID ,ALWAYS> {
	using Holder = typename EXCEPTION_HELP<ALWAYS>::ExceptionHolder ;

	class ImplHolder implement Holder {
	private:
		Slice<STR> mWhat ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (CREF<Slice<STR>> what_) {
			mWhat = what_ ;
		}

		CREF<Slice<STR>> what () const leftvalue override {
			return mWhat ;
		}
	} ;
} ;
} ;

using Exception = typename U::EXCEPTION_HELP<ALWAYS>::Exception ;

struct FUNCTION_catch_invoke {
	template <class ARG1>
	inline void operator() (XREF<ARG1> proc1) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARGS>
	inline void operator() (XREF<ARG1> proc1 ,XREF<ARGS>...procs) const {
		auto &&thiz = *this ;
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		try {
			return proc1 () ;
		} catch (CREF<Exception> e) {
			noop () ;
		} catch (...) {
			dynamic_assert (FALSE) ;
		}
		return thiz (procs...) ;
	}
} ;

static constexpr auto catch_invoke = FUNCTION_catch_invoke () ;

namespace U {
template <class...>
trait WATCH_HELP ;

template <class UUID>
trait WATCH_HELP<UUID ,ALWAYS> {
	struct WATCH implement Interface {
		Slice<STR> mName ;
		LENGTH mAddress ;
		Clazz mClazz ;
	} ;
} ;
} ;

struct FUNCTION_debug_watch {
	template <class ARG1>
	inline void operator() (CREF<Slice<STR>> name ,XREF<ARG1> expr) const {
		require (ENUM_ANY<IS_VARIABLE<ARG1> ,IS_CONSTANT<ARG1>>) ;
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename U::WATCH_HELP<R1X ,ALWAYS>::WATCH ;
		static R2X mInstance ;
		mInstance.mName = name ;
		mInstance.mAddress = address (expr) ;
		mInstance.mClazz = Clazz (TYPEAS<R1X>::id) ;
	}
} ;

static constexpr auto debug_watch = FUNCTION_debug_watch () ;

template <class UNIT1>
inline void U::BOX_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder::throw_bad_alloc () {
	dynamic_assert (FALSE) ;
}

template <class UNIT1>
inline void U::RC_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder::throw_bad_alloc () {
	dynamic_assert (FALSE) ;
}
} ;