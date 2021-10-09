#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
using BOOL = bool ;

static constexpr auto TRUE = BOOL (true) ;
static constexpr auto FALSE = BOOL (false) ;

using VAL32 = std::int32_t ;
using VAL64 = std::int64_t ;

static constexpr auto VAL32_MAX = VAL32 (2147483647) ;
static constexpr auto VAL32_MIN = -VAL32_MAX ;
static constexpr auto VAL64_MAX = VAL64 (9223372036854775807) ;
static constexpr auto VAL64_MIN = -VAL64_MAX ;

namespace U {
template <class...>
trait VAL_HELP ;

template <class UNIT1>
trait VAL_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_VAL32<UNIT1>>> {
	using VAL = VAL32 ;

	static constexpr auto VAL_MAX = VAL32_MAX ;
	static constexpr auto VAL_MIN = VAL32_MIN ;
} ;

template <class UNIT1>
trait VAL_HELP<UNIT1 ,REQUIRE<MACRO_CONFIG_VAL64<UNIT1>>> {
	using VAL = VAL64 ;

	static constexpr auto VAL_MAX = VAL64_MAX ;
	static constexpr auto VAL_MIN = VAL64_MIN ;
} ;
} ;

using VAL = typename U::VAL_HELP<void ,ALWAYS>::VAL ;

static constexpr auto VAL_MAX = U::VAL_HELP<void ,ALWAYS>::VAL_MAX ;
static constexpr auto VAL_MIN = U::VAL_HELP<void ,ALWAYS>::VAL_MIN ;

static constexpr auto ZERO = VAL (+0) ;
static constexpr auto IDEN = VAL (+1) ;
static constexpr auto NONE = VAL (-1) ;
static constexpr auto USED = VAL (-2) ;

using INDEX = VAL ;
using LENGTH = VAL ;
using FLAG = VAL ;

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
using STRU8 = U::csc_char8_t ;
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

using BYTE = std::int8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using FEAT = std::uint64_t ;
using HUGE = U::csc_int128_t ;

static constexpr auto NULL = nullptr ;

template <class UNIT1>
using VREF = DEF<REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using CREF = DEF<const REMOVE_REF<UNIT1> &> ;

template <class UNIT1>
using RREF = DEF<REMOVE_REF<UNIT1> &&> ;

template <class UNIT1>
using XREF = DEF<UNIT1 &&> ;

inline namespace S {
inline constexpr HUGE operator| (CREF<HUGE> thiz ,CREF<HUGE> that) {
	return HUGE (thiz | that) ;
}

inline constexpr HUGE operator& (CREF<HUGE> thiz ,CREF<HUGE> that) {
	return HUGE (thiz & that) ;
}

inline constexpr HUGE operator^ (CREF<HUGE> thiz ,CREF<HUGE> that) {
	return HUGE (thiz ^ that) ;
}

inline constexpr HUGE operator~ (CREF<HUGE> thiz) {
	return HUGE (~thiz) ;
}

inline constexpr HUGE operator<< (CREF<HUGE> thiz ,CREF<LENGTH> s) {
	return HUGE (thiz << s) ;
}

inline constexpr HUGE operator>> (CREF<HUGE> thiz ,CREF<LENGTH> s) {
	return HUGE (thiz >> s) ;
}
} ;

template <class UNIT1>
using SIZE_OF = ENUMAS<VAL ,U::ENUMID<(sizeof (UNIT1))>> ;

template <class UNIT1>
using ALIGN_OF = ENUMAS<VAL ,U::ENUMID<(alignof (UNIT1))>> ;

namespace U {
template <class...>
trait COUNT_OF_HELP ;

template <class...UNIT1>
trait COUNT_OF_HELP<TYPEAS<UNIT1...> ,ALWAYS> {
	using RET = ENUMAS<VAL ,U::ENUMID<(sizeof... (UNIT1))>> ;
} ;
} ;

template <class UNIT1>
using COUNT_OF = typename U::COUNT_OF_HELP<UNIT1 ,ALWAYS>::RET ;

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

namespace U {
template <class...>
trait IS_ENUM_HELP ;

template <class UNIT1>
trait IS_ENUM_HELP<UNIT1 ,ALWAYS> {
	using RET = ENUM_FALSE ;
} ;

template <class...UNIT1>
trait IS_ENUM_HELP<ENUMAS<UNIT1...> ,ALWAYS> {
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

template <class...UNIT1>
trait IS_TYPE_HELP<TYPEAS<UNIT1...> ,ALWAYS> {
	using RET = ENUM_TRUE ;
} ;
} ;

template <class UNIT1>
using IS_TYPE = typename U::IS_TYPE_HELP<UNIT1 ,ALWAYS>::RET ;

using ENUM_ZERO = ENUMAS<VAL ,U::ENUMID<ZERO>> ;
using ENUM_IDEN = ENUMAS<VAL ,U::ENUMID<IDEN>> ;
using ENUM_NONE = ENUMAS<VAL ,U::ENUMID<NONE>> ;
using ENUM_USED = ENUMAS<VAL ,U::ENUMID<USED>> ;

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
using ENUM_ADD = ENUMAS<VAL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = ENUMAS<VAL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = ENUMAS<VAL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = ENUMAS<VAL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = ENUMAS<VAL ,U::ENUMID<(ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1>
using ENUM_MINUS = ENUM_SUB<ENUM_ZERO ,UNIT1> ;

template <class UNIT1>
using ENUM_INC = ENUM_ADD<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_DEC = ENUM_SUB<UNIT1 ,ENUM_IDEN> ;

namespace U {
template <class...>
trait TYPE_FIRST_HELP ;

template <class UNIT1 ,class...UNIT2>
trait TYPE_FIRST_HELP<TYPEAS<UNIT1 ,UNIT2...> ,ALWAYS> {
	using ONE = UNIT1 ;
	using REST = TYPEAS<UNIT2...> ;
} ;
} ;

template <class UNIT1>
using TYPE_FIRST_ONE = typename U::TYPE_FIRST_HELP<UNIT1 ,ALWAYS>::ONE ;

template <class UNIT1>
using TYPE_FIRST_REST = typename U::TYPE_FIRST_HELP<UNIT1 ,ALWAYS>::REST ;

namespace U {
template <class...>
trait TYPE_SECOND_HELP ;

template <class UNIT1 ,class UNIT2 ,class...UNIT3>
trait TYPE_SECOND_HELP<TYPEAS<UNIT1 ,UNIT2 ,UNIT3...> ,ALWAYS> {
	using ONE = UNIT2 ;
	using REST = TYPEAS<UNIT3...> ;
} ;
} ;

template <class UNIT1>
using TYPE_SECOND_ONE = typename U::TYPE_SECOND_HELP<UNIT1 ,ALWAYS>::ONE ;

template <class UNIT1>
using TYPE_SECOND_REST = typename U::TYPE_SECOND_HELP<UNIT1 ,ALWAYS>::REST ;

namespace U {
template <class...>
trait TYPE_THIRD_HELP ;

template <class UNIT1 ,class UNIT2 ,class UNIT3 ,class...UNIT4>
trait TYPE_THIRD_HELP<TYPEAS<UNIT1 ,UNIT2 ,UNIT3 ,UNIT4...> ,ALWAYS> {
	using ONE = UNIT3 ;
	using REST = TYPEAS<UNIT4...> ;
} ;
} ;

template <class UNIT1>
using TYPE_THIRD_ONE = typename U::TYPE_THIRD_HELP<UNIT1 ,ALWAYS>::ONE ;

template <class UNIT1>
using TYPE_THIRD_REST = typename U::TYPE_THIRD_HELP<UNIT1 ,ALWAYS>::REST ;

template <class UNIT1>
using TYPE_UNWIND = ENABLE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>> ,TYPE_FIRST_ONE<UNIT1>> ;

namespace U {
template <class...>
trait TYPE_CAT_HELP ;

template <class...UNIT1 ,class...UNIT2>
trait TYPE_CAT_HELP<TYPEAS<UNIT1...> ,TYPEAS<UNIT2...> ,ALWAYS> {
	using RET = TYPEAS<UNIT1... ,UNIT2...> ;
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
trait TYPE_PICK_HELP ;

template <class PACKS ,class NTH>
trait TYPE_PICK_HELP<PACKS ,NTH ,REQUIRE<ENUM_EQ_ZERO<NTH>>> {
	using RET = TYPE_FIRST_ONE<PACKS> ;
} ;

template <class PACKS ,class NTH>
trait TYPE_PICK_HELP<PACKS ,NTH ,REQUIRE<ENUM_GT_ZERO<NTH>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<PACKS> ,ENUM_DEC<NTH> ,ALWAYS>::RET ;
} ;
} ;

template <class PACKS ,class NTH>
using TYPE_PICK = typename U::TYPE_PICK_HELP<PACKS ,NTH ,ALWAYS>::RET ;

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

template <class...UNIT1>
using ENUM_ALL = typename U::ENUM_ALL_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

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

template <class...UNIT1>
using ENUM_ANY = typename U::ENUM_ANY_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

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

template <class...UNIT1>
using ENUM_SUM = typename U::ENUM_SUM_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

struct Proxy {
	implicit Proxy () = delete ;
	implicit ~Proxy () = delete ;
	implicit Proxy (CREF<Proxy>) = delete ;
	inline VREF<Proxy> operator= (CREF<Proxy>) = delete ;
	implicit Proxy (RREF<Proxy>) = delete ;
	inline VREF<Proxy> operator= (RREF<Proxy>) = delete ;
} ;

struct Interface {
	implicit Interface () = default ;
	virtual ~Interface () = default ;
	implicit Interface (CREF<Interface>) = delete ;
	inline VREF<Interface> operator= (CREF<Interface>) = delete ;
	implicit Interface (RREF<Interface>) = default ;
	inline VREF<Interface> operator= (RREF<Interface>) = default ;
} ;

namespace U {
template <class...>
trait PLACEHOLDER_HELP ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	struct PlaceHolder {} ;
} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using BASE = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::PlaceHolder ;

	struct PlaceHolder implement BASE {} ;
} ;
} ;

template <class RANK>
using PlaceHolder = typename U::PLACEHOLDER_HELP<RANK ,ALWAYS>::PlaceHolder ;

using RANK0 = ENUMAS<VAL ,U::ENUMID<(+0)>> ;
using RANK1 = ENUMAS<VAL ,U::ENUMID<(+1)>> ;
using RANK2 = ENUMAS<VAL ,U::ENUMID<(+2)>> ;
using RANK3 = ENUMAS<VAL ,U::ENUMID<(+3)>> ;
using RANK4 = ENUMAS<VAL ,U::ENUMID<(+4)>> ;
using RANK5 = ENUMAS<VAL ,U::ENUMID<(+5)>> ;
using RANK6 = ENUMAS<VAL ,U::ENUMID<(+6)>> ;
using RANK7 = ENUMAS<VAL ,U::ENUMID<(+7)>> ;
using RANK8 = ENUMAS<VAL ,U::ENUMID<(+8)>> ;
using RANK9 = ENUMAS<VAL ,U::ENUMID<(+9)>> ;
using RANKX = ENUMAS<VAL ,U::ENUMID<(10)>> ;

static constexpr auto PH0 = PlaceHolder<RANK0> () ;
static constexpr auto PH1 = PlaceHolder<RANK1> () ;
static constexpr auto PH2 = PlaceHolder<RANK2> () ;
static constexpr auto PH3 = PlaceHolder<RANK3> () ;
static constexpr auto PH4 = PlaceHolder<RANK4> () ;
static constexpr auto PH5 = PlaceHolder<RANK5> () ;
static constexpr auto PH6 = PlaceHolder<RANK6> () ;
static constexpr auto PH7 = PlaceHolder<RANK7> () ;
static constexpr auto PH8 = PlaceHolder<RANK8> () ;
static constexpr auto PH9 = PlaceHolder<RANK9> () ;
static constexpr auto PHX = PlaceHolder<RANKX> () ;

template <class UNIT1>
using IS_BOOL = IS_SAME<UNIT1 ,BOOL> ;

template <class UNIT1>
using IS_VAL = ENUM_ANY<
	IS_SAME<UNIT1 ,VAL32> ,
	IS_SAME<UNIT1 ,VAL64>> ;

template <class UNIT1>
using IS_FLOAT = ENUM_ANY<
	IS_SAME<UNIT1 ,SINGLE> ,
	IS_SAME<UNIT1 ,DOUBLE>> ;

template <class UNIT1>
using IS_STR = ENUM_ANY<
	IS_SAME<UNIT1 ,STRA> ,
	IS_SAME<UNIT1 ,STRW> ,
	IS_SAME<UNIT1 ,STRU8> ,
	IS_SAME<UNIT1 ,STRU16> ,
	IS_SAME<UNIT1 ,STRU32>> ;

template <class UNIT1>
using IS_BYTE = ENUM_ANY<
	IS_SAME<UNIT1 ,BYTE> ,
	IS_SAME<UNIT1 ,WORD> ,
	IS_SAME<UNIT1 ,CHAR> ,
	IS_SAME<UNIT1 ,FEAT> ,
	IS_SAME<UNIT1 ,HUGE>> ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,DEF<typeof (NULL)>> ;

template <class UNIT1>
using IS_BASIC = ENUM_ANY<
	IS_BOOL<UNIT1> ,
	IS_VAL<UNIT1> ,
	IS_FLOAT<UNIT1> ,
	IS_STR<UNIT1> ,
	IS_BYTE<UNIT1> ,
	IS_NULL<UNIT1>> ;

template <class UNIT1>
using IS_VOID = IS_SAME<UNIT1 ,void> ;

namespace U {
template <class...>
trait REFLECT_REF_HELP ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VREF<UNIT1>>>> {
	using RET = RANK1 ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,CREF<UNIT1>>>> {
	using RET = RANK2 ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,RREF<UNIT1>>>> {
	using RET = RANK3 ;
} ;
} ;

template <class UNIT1>
using REFLECT_REF = typename U::REFLECT_REF_HELP<REMOVE_REF<UNIT1> ,XREF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_VARIABLE = IS_SAME<REFLECT_REF<UNIT1> ,RANK1> ;

template <class UNIT1>
using IS_CONSTANT = IS_SAME<REFLECT_REF<UNIT1> ,RANK2> ;

template <class UNIT1>
using IS_REGISTER = IS_SAME<REFLECT_REF<UNIT1> ,RANK3> ;

namespace U {
template <class...>
trait PTR_HELP ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,UNIT2 ,void ,REQUIRE<IS_SAME<UNIT2 ,VREF<UNIT1>>>> {
	using RET = DEF<UNIT1 *> ;
} ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,UNIT2 ,void ,REQUIRE<IS_SAME<UNIT2 ,CREF<UNIT1>>>> {
	using RET = DEF<const UNIT1 *> ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
trait PTR_HELP<UNIT1 ,UNIT2 ,UNIT3 ,REQUIRE<IS_SAME<UNIT2 ,VREF<UNIT1>>>> {
	using RET = DEF<UNIT1 UNIT2::*> ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
trait PTR_HELP<UNIT1 ,UNIT2 ,UNIT3 ,REQUIRE<IS_SAME<UNIT2 ,CREF<UNIT1>>>> {
	using RET = DEF<const UNIT1 UNIT2::*> ;
} ;
} ;

template <class UNIT1 ,class UNIT2 = void>
using PTR = typename U::PTR_HELP<REMOVE_REF<UNIT1> ,XREF<UNIT1> ,UNIT2 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REFLECT_ARRAY_HELP ;

template <class UNIT1>
trait REFLECT_ARRAY_HELP<UNIT1 ,ALWAYS> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT1>
trait REFLECT_ARRAY_HELP<DEF<UNIT1[]> ,ALWAYS> {
	using R1X = ENUM_ZERO ;
	using RET = TYPEAS<UNIT1 ,R1X> ;
} ;

template <class UNIT1 ,LENGTH UNIT2>
trait REFLECT_ARRAY_HELP<DEF<UNIT1[UNIT2]> ,ALWAYS> {
	using R1X = ENUMAS<VAL ,U::ENUMID<UNIT2>> ;
	using RET = TYPEAS<UNIT1 ,R1X> ;
} ;
} ;

template <class UNIT1>
using REFLECT_ARRAY = typename U::REFLECT_ARRAY_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_ARRAY = ENUM_GT_ZERO<COUNT_OF<REFLECT_ARRAY<UNIT1>>> ;

template <class UNIT1>
using ARRAY_ITEM = TYPE_FIRST_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class UNIT1>
using ARRAY_SIZE = TYPE_SECOND_ONE<REFLECT_ARRAY<UNIT1>> ;

namespace U {
template <class...>
trait ARR_HELP ;

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = DEF<ITEM[]> ;
} ;

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using RET = DEF<ITEM[SIZE::value]> ;
} ;
} ;

template <class ITEM ,class SIZE = ENUM_ZERO>
using ARR = typename U::ARR_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class UNIT1>
trait REFLECT_FUNCTION_HELP<UNIT1 ,ALWAYS> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT1 ,class UNIT2 ,class...UNIT3>
trait REFLECT_FUNCTION_HELP<DEF<UNIT1 (UNIT2::*) (UNIT3...)> ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = TYPEAS<XREF<UNIT3>...> ;
	using RET = TYPEAS<R1X ,R2X> ;
} ;

template <class UNIT1 ,class UNIT2 ,class...UNIT3>
trait REFLECT_FUNCTION_HELP<DEF<UNIT1 (UNIT2::*) (UNIT3...) const> ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = TYPEAS<XREF<UNIT3>...> ;
	using RET = TYPEAS<R1X ,R2X> ;
} ;
} ;

template <class UNIT1>
using REFLECT_FUNCTION = typename U::REFLECT_FUNCTION_HELP<DEF<typeof (&UNIT1::operator())> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_FUNCTION = ENUM_GT_ZERO<COUNT_OF<REFLECT_FUNCTION<UNIT1>>> ;

template <class UNIT1>
using FUNCTION_RETURN = TYPE_FIRST_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using FUNCTION_PARAMS = TYPE_SECOND_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using IS_CLASS = ENUM_ANY<
	ENUMAS<BOOL ,U::ENUMID<(std::is_enum<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_class<UNIT1>::value)>>> ;

template <class UNIT1>
using IS_DEFAULT = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_default_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_destructible<UNIT1>::value)>>> ;

template <class UNIT1>
using IS_TRIVIAL = ENUM_ANY<
	ENUMAS<BOOL ,U::ENUMID<(std::is_trivial<UNIT1>::value)>>> ;

template <class UNIT1>
using IS_CLONEABLE = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_copy_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_copy_assignable<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_move_constructible<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_move_assignable<UNIT1>::value)>>> ;

template <class UNIT1>
using IS_INTERFACE = ENUM_ALL<
	ENUMAS<BOOL ,U::ENUMID<(std::is_abstract<UNIT1>::value)>> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_base_of<Interface ,UNIT1>::value)>>> ;

template <class BASE ,class DERIVED>
using IS_EXTEND = ENUM_ANY<
	IS_SAME<BASE ,DERIVED> ,
	ENUMAS<BOOL ,U::ENUMID<(std::is_base_of<BASE ,DERIVED>::value)>>> ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,DEF<typeof (PH0)>> ;

template <class UNIT1>
using IS_OBJECT = ENUM_ANY<
	IS_BASIC<UNIT1> ,
	IS_CLASS<UNIT1>> ;

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
	ENUM_EQUAL<SIZE ,SIZE_OF<HUGE>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<HUGE>>>>> {
	using RET = HUGE ;
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
	using RET = ARR<R3X ,R4X> ;
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

namespace U {
template <class...>
trait TEMP_HELP ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,ALWAYS> {
	using RET = TEMPID<UNIT1> ;
} ;
} ;

template <class UNIT1>
using TEMP = typename U::TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

namespace U {
template <class...>
trait REMOVE_TEMP_HELP ;

template <class UNIT1>
trait REMOVE_TEMP_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
trait REMOVE_TEMP_HELP<TEMPID<UNIT1> ,ALWAYS> {
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

template <class...UNIT1>
using PACK = typename U::PACK_HELP<TYPEAS<UNIT1...> ,ALWAYS>::PACK ;

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
			return extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
	struct FUNCTION_barrier {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			return extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
	struct FUNCTION_barrier {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			return extern_invoke () ;
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
trait FUNCTION_unsafe_break_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_MSVC<UNIT1>>> {
	struct FUNCTION_unsafe_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			return extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
	struct FUNCTION_unsafe_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			return extern_invoke () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
	struct FUNCTION_unsafe_break {
		imports void extern_invoke () ;

		inline void operator() () const noexcept {
			return extern_invoke () ;
		}
	} ;
} ;
} ;

struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		using R1X = typename U::FUNCTION_unsafe_break_HELP<void ,ALWAYS>::FUNCTION_unsafe_break ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

struct FUNCTION_unsafe_deref {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<REMOVE_TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		return reinterpret_cast<VREF<REMOVE_TEMP<R1X>>> (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<REMOVE_TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		return reinterpret_cast<CREF<REMOVE_TEMP<R1X>>> (obj) ;
	}
} ;

static constexpr auto unsafe_deref = FUNCTION_unsafe_deref () ;

struct FUNCTION_unsafe_deptr {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return reinterpret_cast<VREF<TEMP<R1X>>> (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<TEMP<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return reinterpret_cast<CREF<TEMP<R1X>>> (obj) ;
	}
} ;

static constexpr auto unsafe_deptr = FUNCTION_unsafe_deptr () ;

struct FUNCTION_address {
	template <class ARG1>
	inline LENGTH operator() (XREF<ARG1> obj) const noexcept {
		return LENGTH (&unsafe_deptr (obj)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

namespace U {
template <class...>
trait FUNCTION_unsafe_cast_impl_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<
	IS_OBJECT<UNIT1> ,
	IS_OBJECT<UNIT2>>>> {
	struct FUNCTION_unsafe_cast_impl {
		inline VREF<UNIT1> operator() (VREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R2X ,R1X> ;
			using R4X = CONDITIONAL<IS_SAME<R2X ,TEMP<void>> ,R3X ,R2X> ;
			using R5X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R4X ,R1X> ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R5X> ,SIZE_OF<R4X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R5X> ,ALIGN_OF<R4X>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		inline CREF<UNIT1> operator() (CREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R2X ,R1X> ;
			using R4X = CONDITIONAL<IS_SAME<R2X ,TEMP<void>> ,R3X ,R2X> ;
			using R5X = CONDITIONAL<IS_SAME<R1X ,TEMP<void>> ,R4X ,R1X> ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R5X> ,SIZE_OF<R4X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R5X> ,ALIGN_OF<R4X>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<
	IS_ARRAY<UNIT1> ,
	IS_OBJECT<UNIT2>>>> {
	struct FUNCTION_unsafe_cast_impl {
		inline VREF<UNIT1> operator() (VREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		inline CREF<UNIT1> operator() (CREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<
	IS_OBJECT<UNIT1> ,
	IS_ARRAY<UNIT2>>>> {
	struct FUNCTION_unsafe_cast_impl {
		inline VREF<UNIT1> operator() (VREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R2X> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R3X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R2X>> ;
			require (ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R1X ,TEMP<R4X>>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		inline CREF<UNIT1> operator() (CREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R2X> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R3X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R2X>> ;
			require (ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R1X ,TEMP<R4X>>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<
	IS_ARRAY<UNIT1> ,
	IS_ARRAY<UNIT2>>>> {
	struct FUNCTION_unsafe_cast_impl {
		inline VREF<UNIT1> operator() (VREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R1X> ;
			using R4X = ARRAY_ITEM<R2X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R4X>) ;
			require (ENUM_ANY<IS_SAME<R3X ,TEMP<R4X>> ,IS_SAME<TEMP<R3X> ,R4X>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		inline CREF<UNIT1> operator() (CREF<UNIT2> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = UNIT2 ;
			using R3X = ARRAY_ITEM<R1X> ;
			using R4X = ARRAY_ITEM<R2X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R4X>) ;
			require (ENUM_ANY<IS_SAME<R3X ,TEMP<R4X>> ,IS_SAME<TEMP<R3X> ,R4X>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;
} ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_unsafe_cast_impl = typename U::FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,UNIT2 ,ALWAYS>::FUNCTION_unsafe_cast_impl ;

struct FUNCTION_unsafe_cast {
	template <class ARG1 ,class ARG2 ,class = ENABLE<IS_VARIABLE<ARG2>>>
	inline VREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> id ,XREF<ARG2> from) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		static constexpr auto M_INVOKE = FUNCTION_unsafe_cast_impl<R1X ,R2X> () ;
		return M_INVOKE (from) ;
	}

	template <class ARG1 ,class ARG2 ,class = ENABLE<IS_CONSTANT<ARG2>>>
	inline CREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> id ,XREF<ARG2> from) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		static constexpr auto M_INVOKE = FUNCTION_unsafe_cast_impl<R1X ,R2X> () ;
		return M_INVOKE (from) ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_unsafe_pointer {
	inline VREF<TEMP<void>> operator() (CREF<LENGTH> addr_) const noexcept {
		const auto r1x = reinterpret_cast<PTR<VREF<TEMP<void>>>> (addr_) ;
		assert (r1x != NULL) ;
		auto &&tmp = *r1x ;
		return tmp ;
	}
} ;

static constexpr auto unsafe_pointer = FUNCTION_unsafe_pointer () ;

struct FUNCTION_swap {
	template <class ARG1 ,class ARG2>
	inline void operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const noexcept {
		//@info: no class should depend on its address
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
		template <class ARG1 ,class = ENABLE<ENUM_ANY<
			IS_VARIABLE<ARG1> ,
			IS_CONSTANT<ARG1>>>>
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

struct FUNCTION_unsafe_create {
	template <class ARG1>
	inline void operator() (XREF<ARG1> thiz_) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = REMOVE_TEMP<R1X> ;
		require (IS_OBJECT<R2X>) ;
		zeroize (thiz_) ;
		new (&unsafe_deptr (thiz_)) R2X () ;
		barrier () ;
	}
} ;

static constexpr auto unsafe_create = FUNCTION_unsafe_create () ;

struct FUNCTION_unsafe_destroy {
	template <class ARG1>
	inline void operator() (XREF<ARG1> thiz_) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_TEMP<R1X>) ;
		using R2X = REMOVE_TEMP<R1X> ;
		require (IS_OBJECT<R2X>) ;
		unsafe_deref (thiz_).~R2X () ;
		barrier () ;
	}
} ;

static constexpr auto unsafe_destroy = FUNCTION_unsafe_destroy () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline RREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> id) const noexcept {
		auto &&thiz = *this ;
		assert (FALSE) ;
		return thiz (id) ;
	}
} ;

static constexpr auto bad = FUNCTION_bad () ;

struct FUNCTION_invoke {
	template <class ARG1>
	inline FUNCTION_RETURN<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

struct FUNCTION_memorize {
	template <class ARG1>
	inline CREF<FUNCTION_RETURN<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> func) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_RETURN<R1X> ;
		static auto mInstance = func () ;
		return keep[TYPEAS<CREF<R2X>>::id] (mInstance) ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

struct FUNCTION_abs {
	inline LENGTH operator() (CREF<LENGTH> obj) const {
		assert (obj >= VAL_MIN) ;
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

struct FUNCTION_align_as {
	inline LENGTH operator() (CREF<LENGTH> curr ,CREF<LENGTH> base) const {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto align_as = FUNCTION_align_as () ;

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
		const auto r2x = unsafe_deptr (keep[TYPEAS<CREF<Interface>>::id] (r1x)) ;
		const auto r3x = unsafe_deref (unsafe_cast (TYPEAS<TEMP<FLAG>>::id ,r2x)) ;
		return r3x ;
	}
} ;

static constexpr auto operator_cabi = FUNCTION_operator_cabi () ;

namespace U {
template <class...>
trait ITERATOR_HELP ;

template <class UNIT1>
trait ITERATOR_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
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

		CREF<INDEX> at () const leftvalue {
			return mCurr ;
		}

		inline CREF<INDEX> operator* () const leftvalue {
			return at () ;
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

using RangeIterator = typename U::ITERATOR_HELP<void ,ALWAYS>::RangeIterator ;

struct FUNCTION_range {
	inline RangeIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return RangeIterator (begin_ ,end_) ;
	}
} ;

static constexpr auto range = FUNCTION_range () ;

template <class UNIT1>
using HAS_EQUAL = IS_BOOL<typeof (bad (TYPEAS<UNIT1>::id).equal (bad (TYPEAS<UNIT1>::id)))> ;

namespace U {
template <class...>
trait FUNCTION_operator_equal_HELP ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return obj1 == obj2 ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			for (auto &&i : range (0 ,ARRAY_SIZE<UNIT1>::value)) {
				const auto r1x = operator_equal (obj1[i] ,obj2[i]) ;
				if ifnot (r1x)
					return FALSE ;
			}
			return TRUE ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return template_equal (obj1 ,obj2 ,PHX) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<HAS_EQUAL<DEPENDENT<UNIT1 ,ARG1>>>>
		BOOL template_equal (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2 ,CREF<typeof (PH2)>) const {
			return obj1.equal (obj2) ;
		}

		template <class ARG1 = ALWAYS>
		BOOL template_equal (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2 ,CREF<typeof (PH1)>) const {
			return TRUE ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_equal {
	template <class ARG1 ,class ARG2>
	inline BOOL operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename U::FUNCTION_operator_equal_HELP<R1X ,ALWAYS>::FUNCTION_operator_equal ;
		static constexpr auto M_INVOKE = R3X () ;
		return M_INVOKE (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_equal = FUNCTION_operator_equal () ;

template <class UNIT1>
using HAS_COMPR = IS_VAL<typeof (bad (TYPEAS<UNIT1>::id).compr (bad (TYPEAS<UNIT1>::id)))> ;

namespace U {
template <class...>
trait FUNCTION_operator_compr_HELP ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
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

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			for (auto &&i : range (0 ,ARRAY_SIZE<UNIT1>::value)) {
				const auto r1x = operator_compr (obj1[i] ,obj2[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return template_compr (obj1 ,obj2 ,PHX) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<HAS_COMPR<DEPENDENT<UNIT1 ,ARG1>>>>
		FLAG template_compr (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2 ,CREF<typeof (PH2)>) const {
			return obj1.compr (obj2) ;
		}

		template <class ARG1 = ALWAYS>
		FLAG template_compr (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2 ,CREF<typeof (PH1)>) const {
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
trait FUNCTION_hashcode_HELP ;

template <class UNIT1>
trait FUNCTION_hashcode_HELP<UNIT1 ,REQUIRE<DEPENDENT<IS_SAME<FLAG ,VAL32> ,UNIT1>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-2128831035) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R2X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			const auto r1x = R2X (now) ^ R2X (inc) ;
			const auto r2x = R2X (VAL (r1x) * VAL (16777619)) ;
			const auto r3x = R2X (r2x & R2X (VAL_MAX)) ;
			return FLAG (r3x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_hashcode_HELP<UNIT1 ,REQUIRE<DEPENDENT<IS_SAME<FLAG ,VAL64> ,UNIT1>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-3750763034362895579) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R2X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			const auto r1x = R2X (now) ^ R2X (inc) ;
			const auto r2x = R2X (VAL (r1x) * VAL (1099511628211)) ;
			const auto r3x = R2X (r2x & R2X (VAL_MAX)) ;
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

template <class UNIT1>
using HAS_HASH = IS_VAL<typeof (bad (TYPEAS<UNIT1>::id).hash ())> ;

namespace U {
template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return FLAG (obj) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			FLAG ret = hashcode () ;
			for (auto &&i : range (0 ,ARRAY_SIZE<UNIT1>::value)) {
				const auto r1x = operator_hash (obj[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return template_hash (obj ,PHX) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<HAS_HASH<DEPENDENT<UNIT1 ,ARG1>>>>
		FLAG template_hash (CREF<UNIT1> obj ,CREF<typeof (PH2)>) const {
			return obj.hash () ;
		}

		template <class ARG1 = ALWAYS>
		FLAG template_hash (CREF<UNIT1> obj ,CREF<typeof (PH1)>) const {
			return ZERO ;
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

namespace U {
template <class...>
trait BOX_HELP ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	class Box final {
	private:
		TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...objs) {
			Box ret ;
			zeroize (ret.mValue) ;
			new (&unsafe_deptr (ret.mValue)) UNIT1 (forward[TYPEAS<ARG1>::id] (objs)...) ;
			barrier () ;
			ret.mExist = TRUE ;
			return move (ret) ;
		}

		implicit ~Box () noexcept {
			if ifnot (mExist)
				return ;
			unsafe_destroy (mValue) ;
			mExist = FALSE ;
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

		BOOL exist () const {
			return mExist ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT1> to () leftvalue {
			assert (exist ()) ;
			return m_fake () ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			assert (exist ()) ;
			return m_fake () ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<UNIT1> m_fake () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT1> m_fake () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	class Box final {
	private:
		TEMP<ARR<ITEM ,SIZE>> mArray ;
		LENGTH mSize ;

	public:
		implicit Box () noexcept {
			mSize = 0 ;
		}

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		imports Box make () {
			using R1X = typename BOX_HELP<ITEM ,ALWAYS>::Box ;
			Box ret ;
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM> ,SIZE>>::id ,ret.mArray) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				zeroize (ret.mArray) ;
				ret.mSize = SIZE::value ;
			}
			while (TRUE) {
				if (ret.mSize >= SIZE::value)
					break ;
				INDEX ix = ret.mSize ;
				auto rax = R1X::make () ;
				tmp[ix] = unsafe_deptr (rax.self) ;
				rax.release () ;
				ret.mSize++ ;
			}
			return move (ret) ;
		}

		imports Box make (CREF<UNIT1> that) {
			using R1X = typename BOX_HELP<ITEM ,ALWAYS>::Box ;
			Box ret ;
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM> ,SIZE>>::id ,ret.mArray) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				ret.mArray = unsafe_deptr (that) ;
				ret.mSize = SIZE::value ;
			}
			while (TRUE) {
				if (ret.mSize >= SIZE::value)
					break ;
				INDEX ix = ret.mSize ;
				auto rax = R1X::make (move (that[ix])) ;
				tmp[ix] = unsafe_deptr (rax.self) ;
				rax.release () ;
				ret.mSize++ ;
			}
			return move (ret) ;
		}

		imports Box make (RREF<UNIT1> that) {
			using R1X = typename BOX_HELP<ITEM ,ALWAYS>::Box ;
			Box ret ;
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM> ,SIZE>>::id ,ret.mArray) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				ret.mArray = unsafe_deptr (that) ;
				ret.mSize = SIZE::value ;
			}
			while (TRUE) {
				if (ret.mSize >= SIZE::value)
					break ;
				INDEX ix = ret.mSize ;
				auto rax = R1X::make (move (that[ix])) ;
				tmp[ix] = unsafe_deptr (rax.self) ;
				rax.release () ;
				ret.mSize++ ;
			}
			return move (ret) ;
		}

		implicit ~Box () noexcept {
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM> ,SIZE>>::id ,mArray) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					return ;
				mSize = 0 ;
			}
			while (TRUE) {
				if (mSize <= 0)
					break ;
				INDEX ix = mSize - 1 ;
				unsafe_destroy (tmp[ix]) ;
				mSize-- ;
			}
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

		BOOL exist () const {
			return mSize == SIZE::value ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT1> to () leftvalue {
			assert (exist ()) ;
			return m_fake () ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			assert (exist ()) ;
			return m_fake () ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		void release () {
			mSize = 0 ;
		}

	private:
		VREF<UNIT1> m_fake () leftvalue {
			return unsafe_deref (mArray) ;
		}

		CREF<UNIT1> m_fake () const leftvalue {
			return unsafe_deref (mArray) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Box = typename U::BOX_HELP<UNIT1 ,ALWAYS>::Box ;

namespace U {
template <class...>
trait CELL_HELP ;

template <class UNIT1>
trait CELL_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	class Cell final {
	private:
		TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Cell () noexcept {
			mExist = FALSE ;
		}

		implicit Cell (CREF<typeof (NULL)>) :Cell () {
			noop () ;
		}

		template <class...ARG1>
		imports Cell make (XREF<ARG1>...objs) {
			Cell ret ;
			ret.initialize (forward[TYPEAS<ARG1>::id] (objs)...) ;
			return move (ret) ;
		}

		implicit ~Cell () noexcept {
			if ifnot (mExist)
				return ;
			unsafe_destroy (mValue) ;
			mExist = FALSE ;
		}

		implicit Cell (CREF<Cell>) = delete ;

		inline VREF<Cell> operator= (CREF<Cell>) = delete ;

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

		template <class...ARG1>
		void initialize (XREF<ARG1>...objs) {
			if (mExist)
				return ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			mValue = unsafe_deptr (rax.self) ;
			rax.release () ;
			mExist = TRUE ;
		}

		UNIT1 fetch () const {
			assert (mExist) ;
			return m_fake () ;
		}

		UNIT1 fetch (CREF<UNIT1> obj) const {
			if ifnot (mExist)
				return obj ;
			return m_fake () ;
		}

		void store (CREF<UNIT1> obj) const {
			assert (mExist) ;
			m_fake () = obj ;
		}

		UNIT1 exchange (CREF<UNIT1> obj) const {
			assert (mExist) ;
			UNIT1 ret = m_fake () ;
			m_fake () = obj ;
			return move (ret) ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> obj) const {
			assert (mExist) ;
			if ifswitch (TRUE) {
				if ifnot (operator_equal (m_fake () ,expect))
					discard ;
				expect = m_fake () ;
				return FALSE ;
			}
			m_fake () = obj ;
			return TRUE ;
		}

	private:
		VREF<UNIT1> m_fake () const leftvalue {
			const auto r1x = address (mValue) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (r1x))) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Cell = typename U::CELL_HELP<UNIT1 ,ALWAYS>::Cell ;

namespace U {
template <class...>
trait VREF_HELP ;

template <class...>
trait CREF_HELP ;

template <class...>
trait VREF_HOLDER_HELP ;

template <>
trait VREF_HOLDER_HELP<ALWAYS> {
	struct VRefHolder implement Interface {
		virtual void finalize () = 0 ;
		virtual void destroy () = 0 ;
		virtual LENGTH addr () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual LENGTH lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;
} ;
} ;

template <class FROM ,class ELEM ,class TO>
using HAS_REBIND = IS_SAME<DEF<typeof (bad (TYPEAS<FROM>::id).rebind (TYPEAS<ELEM>::id))> ,TO> ;

namespace U {
template <class...>
trait VREF_HELP ;

template <class...>
trait VREF_IMPLHOLDER_HELP ;

template <class UNIT1>
trait VREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;

	class VRef final {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	private:
		LENGTH mHolder ;
		LENGTH mPointer ;

	public:
		implicit VRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit VRef (CREF<typeof (NULL)>) :VRef () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,VRef>> ,
			HAS_REBIND<ARG1 ,UNIT1 ,VRef>>>>
			implicit VRef (XREF<ARG1> that) : VRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...objs) {
			using R1X = UNIT1 ;
			using R2X = typename VREF_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			VRef ret ;
			if ifswitch (TRUE) {
				const auto r1x = R2X::dynamic_size (forward[TYPEAS<ARG1>::id] (objs)...) ;
				ret.mHolder = R2X::create (r1x) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.m_fake ().increase () ;
				assert (r2x == IDEN) ;
				const auto r3x = ret.m_fake ().lock () ;
				assert (r3x == NONE) ;
				auto &&tmp = keep[TYPEAS<VREF<R2X>>::id] (ret.m_fake ()) ;
				tmp.initialize (forward[TYPEAS<ARG1>::id] (objs)...) ;
			}
			if ifswitch (TRUE) {
				ret.mPointer = ret.m_fake ().addr () ;
			}
			return move (ret) ;
		}

		imports VRef reference (VREF<UNIT1> that) {
			VRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		implicit ~VRef () noexcept {
			if (mHolder == ZERO)
				return ;
			if ifswitch (TRUE) {
				if (mPointer == ZERO)
					discard ;
				m_fake ().finalize () ;
			}
			m_fake ().destroy () ;
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit VRef (CREF<VRef>) = delete ;

		inline VREF<VRef> operator= (CREF<VRef>) = delete ;

		implicit VRef (RREF<VRef> that) noexcept :VRef () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<VRef> operator= (RREF<VRef> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT1> to () leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,template_at (PHX))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,template_at (PHX))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		DEF<typename VREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::VRef> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			using R2X = typename VREF_HELP<R1X ,ALWAYS>::VRef ;
			R2X ret ;
			const auto r1x = invoke ([&] () {
				if (mPointer == ZERO)
					return ZERO ;
				return address (keep[TYPEAS<VREF<R1X>>::id] (self)) ;
			}) ;
			ret.mPointer = r1x ;
			mPointer = ZERO ;
			ret.mHolder = mHolder ;
			mHolder = ZERO ;
			return move (ret) ;
		}

		template <class ARG1 = ALWAYS>
		DEF<typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef> unlock () {
			using R1X = typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef ;
			R1X ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if (mHolder == ZERO)
					discard ;
				m_fake ().unlock () ;
				ret.mPointer = mPointer ;
				mPointer = ZERO ;
				ret.mHolder = mHolder ;
				mHolder = ZERO ;
			}
			return move (ret) ;
		}

	private:
		VREF<Holder> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Holder>>::id ,unsafe_pointer (mHolder))) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<IS_ARRAY<DEPENDENT<UNIT1 ,ARG1>>>>
		VREF<ARR<TEMP<ARRAY_ITEM<DEPENDENT<UNIT1 ,ARG1>>> ,ARRAY_SIZE<DEPENDENT<UNIT1 ,ARG1>>>> template_at (CREF<typeof (PH2)>) const leftvalue {
			using R1X = DEPENDENT<UNIT1 ,ARG1> ;
			using R2X = ARR<TEMP<ARRAY_ITEM<R1X>> ,ARRAY_SIZE<R1X>> ;
			return unsafe_cast (TYPEAS<R2X>::id ,unsafe_pointer (mPointer)) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<IS_OBJECT<DEPENDENT<UNIT1 ,ARG1>>>>
		VREF<TEMP<void>> template_at (CREF<typeof (PH1)>) const leftvalue {
			return unsafe_pointer (mPointer) ;
		}
	} ;
} ;

template <class UNIT1>
trait VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;

	class ImplHolder implement Holder {
	private:
		LENGTH mOrigin ;
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		imports void throw_bad_alloc () ;

		imports LENGTH create[[nodiscard]] (CREF<LENGTH> size_) {
			assert (size_ > 0) ;
			const auto r1x = LENGTH (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (r1x != ZERO)
					discard ;
				throw_bad_alloc () ;
			}
			const auto r2x = align_as (r1x ,ALIGN_OF<ImplHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<ImplHolder>>::id ,unsafe_pointer (r2x))) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r1x ;
			tmp.mCounter = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		template <class...ARG1>
		imports LENGTH dynamic_size (XREF<ARG1>...objs) {
			const auto r1x = max (ALIGN_OF<ImplHolder>::value - ALIGN_OF<std::max_align_t>::value ,ZERO) ;
			const auto r2x = r1x + SIZE_OF<ImplHolder>::value ;
			return r2x ;
		}

		template <class...ARG1>
		void initialize (XREF<ARG1>...objs) {
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			mValue = unsafe_deptr (rax.self) ;
			rax.release () ;
		}

		void finalize () override {
			unsafe_destroy (mValue) ;
		}

		void destroy () override {
			auto &&thiz = *this ;
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
			operator delete (&unsafe_pointer (r1x) ,std::nothrow) ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
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
				if (mCounter != IDEN)
					discard ;
				mCounter = NONE ;
			}
			return mCounter ;
		}

		void unlock () override {
			if ifswitch (TRUE) {
				if (mCounter != NONE)
					discard ;
				mCounter = IDEN ;
			}
		}
	} ;
} ;

template <class UNIT1>
trait VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_EQ_ZERO<SIZE>) ;
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;

	class RealHolder implement Holder {
	private:
		LENGTH mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;
		TEMP<ITEM> mArray ;
	} ;

	class ImplHolder implement Holder {
	private:
		LENGTH mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;

	public:
		implicit ImplHolder () = default ;

		imports void throw_bad_alloc () ;

		imports LENGTH create[[nodiscard]] (CREF<LENGTH> size_) {
			assert (size_ > 0) ;
			const auto r1x = LENGTH (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (r1x != ZERO)
					discard ;
				throw_bad_alloc () ;
			}
			const auto r2x = align_as (r1x ,ALIGN_OF<RealHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<ImplHolder>>::id ,unsafe_pointer (r2x))) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r1x ;
			tmp.mCounter = 0 ;
			tmp.mSize = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		imports LENGTH dynamic_size (CREF<LENGTH> size_) {
			assert (size_ > 0) ;
			const auto r1x = max (ALIGN_OF<RealHolder>::value - ALIGN_OF<std::max_align_t>::value ,ZERO) ;
			const auto r2x = max (size_ - 1 ,ZERO) ;
			const auto r3x = r1x + SIZE_OF<RealHolder>::value + SIZE_OF<ITEM>::value * r2x ;
			return r3x ;
		}

		void initialize (CREF<LENGTH> size_) {
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM>>>::id ,m_fake ()) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				for (auto &&i : range (0 ,size_))
					zeroize (tmp[i]) ;
				mSize = size_ ;
			}
			while (TRUE) {
				if (mSize >= size_)
					break ;
				INDEX ix = mSize ;
				auto rax = Box<ITEM>::make () ;
				tmp[ix] = unsafe_deptr (rax.self) ;
				rax.release () ;
				mSize++ ;
			}
		}

		void finalize () override {
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM>>>::id ,m_fake ()) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				mSize = 0 ;
			}
			while (TRUE) {
				if (mSize <= 0)
					break ;
				INDEX ix = mSize - 1 ;
				unsafe_destroy (tmp[ix]) ;
				mSize-- ;
			}
		}

		void destroy () override {
			auto &&thiz = *this ;
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
			operator delete (&unsafe_pointer (r1x) ,std::nothrow) ;
		}

		LENGTH addr () const override {
			return address (m_fake ()) ;
		}

		VREF<TEMP<void>> m_fake () const leftvalue {
			auto &&thiz = *this ;
			const auto r1x = address (thiz) + SIZE_OF<RealHolder>::value - SIZE_OF<ITEM>::value ;
			assert (r1x % ALIGN_OF<ITEM>::value == 0) ;
			return unsafe_pointer (r1x) ;
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
				if (mCounter != IDEN)
					discard ;
				mCounter = NONE ;
			}
			return mCounter ;
		}

		void unlock () override {
			if ifswitch (TRUE) {
				if (mCounter != NONE)
					discard ;
				mCounter = IDEN ;
			}
		}
	} ;
} ;
} ;

template <class UNIT1>
using VRef = typename U::VREF_HELP<UNIT1 ,ALWAYS>::VRef ;

namespace U {
template <class...>
trait CREF_HELP ;

template <class...>
trait CREF_IMPLHOLDER_HELP ;

template <class...>
trait CREF_PUREHOLDER_HELP ;

template <class UNIT1>
trait CREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;

	class CRef final {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	private:
		LENGTH mHolder ;
		LENGTH mPointer ;

	public:
		implicit CRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit CRef (CREF<typeof (NULL)>) :CRef () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,CRef>> ,
			HAS_REBIND<ARG1 ,UNIT1 ,CRef>>>>
			implicit CRef (XREF<ARG1> that) :CRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...objs) {
			using R1X = UNIT1 ;
			using R2X = typename CREF_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			CRef ret ;
			if ifswitch (TRUE) {
				const auto r1x = R2X::dynamic_size (forward[TYPEAS<ARG1>::id] (objs)...) ;
				ret.mHolder = R2X::create (r1x) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.m_fake ().increase () ;
				assert (r2x == IDEN) ;
				auto &&tmp = keep[TYPEAS<VREF<R2X>>::id] (ret.m_fake ()) ;
				tmp.initialize (forward[TYPEAS<ARG1>::id] (objs)...) ;
			}
			if ifswitch (TRUE) {
				ret.mPointer = ret.mHolder->addr () ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class...ARG2>
		imports CRef intrusive (XREF<ARG1> where_ ,XREF<ARG2>...objs) {
			using R1X = UNIT1 ;
			using R2X = typename CREF_PUREHOLDER_HELP<R1X ,ALWAYS>::PureHolder ;
			using R3X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<R3X ,TEMP<R2X>>) ;
			CRef ret ;
			if ifswitch (TRUE) {
				ret.mHolder = R2X::create (where_) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.m_fake ().increase () ;
				if (r2x != IDEN)
					discard ;
				auto &&tmp = keep[TYPEAS<VREF<R2X>>::id] (ret.m_fake ()) ;
				tmp.initialize (forward[TYPEAS<ARG2>::id] (objs)...) ;
			}
			if ifswitch (TRUE) {
				ret.mPointer = ret.m_fake ().addr () ;
			}
			return move (ret) ;
		}

		imports CRef reference (CREF<UNIT1> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		implicit ~CRef () noexcept {
			if (mHolder == ZERO)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = m_fake ().decrease () ;
				if (r1x > 0)
					discard ;
				if ifswitch (TRUE) {
					if (mPointer == ZERO)
						discard ;
					m_fake ().finalize () ;
				}
				m_fake ().destroy () ;
			}
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit CRef (CREF<CRef> that) :CRef () {
			if ifnot (that.exist ())
				return ;
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				const auto r1x = that.m_fake ().increase () ;
				if (r1x == NONE)
					discard ;
				assert (r1x >= 2) ;
				mHolder = that.mHolder ;
			}
			mPointer = that.mPointer ;
		}

		inline VREF<CRef> operator= (CREF<CRef> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit CRef (RREF<CRef> that) noexcept :CRef () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<CRef> operator= (RREF<CRef> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		CREF<UNIT1> to () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,template_at (PHX))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		DEF<typename CREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::CRef> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			using R2X = typename CREF_HELP<R1X ,ALWAYS>::CRef ;
			R2X ret ;
			const auto r1x = invoke ([&] () {
				if (mPointer == ZERO)
					return ZERO ;
				return address (keep[TYPEAS<CREF<R1X>>::id] (self)) ;
			}) ;
			ret.mPointer = r1x ;
			mPointer = ZERO ;
			ret.mHolder = mHolder ;
			mHolder = ZERO ;
			return move (ret) ;
		}

		template <class ARG1 = ALWAYS>
		DEF<typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef> lock () {
			using R1X = typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef ;
			R1X ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				assert (mHolder != ZERO) ;
				const auto r1x = m_fake ().lock () ;
				if (r1x != NONE)
					discard ;
				ret.mPointer = mPointer ;
				mPointer = ZERO ;
				ret.mHolder = mHolder ;
				mHolder = ZERO ;
			}
			return move (ret) ;
		}

	private:
		VREF<Holder> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Holder>>::id ,unsafe_pointer (mHolder))) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<IS_ARRAY<DEPENDENT<UNIT1 ,ARG1>>>>
		VREF<ARR<TEMP<ARRAY_ITEM<DEPENDENT<UNIT1 ,ARG1>>> ,ARRAY_SIZE<DEPENDENT<UNIT1 ,ARG1>>>> template_at (CREF<typeof (PH2)>) const leftvalue {
			using R1X = DEPENDENT<UNIT1 ,ARG1> ;
			using R2X = ARR<TEMP<ARRAY_ITEM<R1X>> ,ARRAY_SIZE<R1X>> ;
			return unsafe_cast (TYPEAS<R2X>::id ,unsafe_pointer (mPointer)) ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<IS_OBJECT<DEPENDENT<UNIT1 ,ARG1>>>>
		VREF<TEMP<void>> template_at (CREF<typeof (PH1)>) const leftvalue {
			return unsafe_pointer (mPointer) ;
		}
	} ;
} ;

template <class UNIT1>
trait CREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;
	using ImplHolder = typename VREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
} ;

template <class UNIT1>
trait CREF_PUREHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename VREF_HOLDER_HELP<ALWAYS>::VRefHolder ;

	class PureHolder implement Holder {
	private:
		LENGTH mOrigin ;
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit PureHolder () = default ;

		template <class ARG1>
		imports VREF<TEMP<PureHolder>> unique_stack (XREF<ARG1> id) {
			//@warn: not thread-safe
			static PureHolder mInstance = invoke ([&] () {
				PureHolder ret ;
				ret.mOrigin = ZERO ;
				ret.mCounter = 0 ;
				return move (ret) ;
			}) ;
			return unsafe_deptr (mInstance) ;
		}

		FLAG this_cabi () const {
			auto &&thiz = *this ;
			const auto r1x = unsafe_deptr (keep[TYPEAS<CREF<Interface>>::id] (thiz)) ;
			const auto r2x = unsafe_deref (unsafe_cast (TYPEAS<TEMP<FLAG>>::id ,r1x)) ;
			return r2x ;
		}

		imports LENGTH create[[nodiscard]] (VREF<TEMP<PureHolder>> where_) {
			auto &&tmp = unsafe_deref (where_) ;
			const auto r1x = tmp.this_cabi () ;
			const auto r2x = PureHolder ().this_cabi () ;
			assert (r1x == r2x) ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		template <class...ARG1>
		void initialize (XREF<ARG1>...objs) {
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			mValue = unsafe_deptr (rax.self) ;
			rax.release () ;
		}

		void finalize () override {
			unsafe_destroy (mValue) ;
		}

		void destroy () override {
			noop () ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
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

		void unlock () override {
			noop () ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using CRef = typename U::CREF_HELP<UNIT1 ,ALWAYS>::CRef ;

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
		virtual LENGTH addr () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
	} ;

	using AUTO_MAX_SIZE = ENUMAS<VAL ,U::ENUMID<(+4096)>> ;
	using AUTO_MAX_ALIGN = ALIGN_OF<HUGE> ;

	class AutoFakeHolder implement AutoHolder {
	private:
		Storage<AUTO_MAX_SIZE ,AUTO_MAX_ALIGN> mStorage ;

	public:
		void finalize () override ;
		void destroy () override ;
		LENGTH addr () const override ;
		FLAG type_cabi () const override ;
	} ;

	using Holder = AutoHolder ;
	using FakeHolder = AutoFakeHolder ;

	class Auto final {
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
		implicit Auto (XREF<ARG1> that) noexcept :Auto () {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_OBJECT<R1X>) ;
			using R2X = typename AUTO_PUREHOLDER_HELP<R1X ,ALWAYS>::PureHolder ;
			const auto r1x = R2X::create (unsafe_cast (TYPEAS<TEMP<R2X>>::id ,mValue)) ;
			assert (r1x == address (m_fake ())) ;
			mExist = TRUE ;
			auto &&tmp = keep[TYPEAS<VREF<R2X>>::id] (m_fake ()) ;
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
			}
			m_fake ().destroy () ;
			mExist = FALSE ;
			mGood = FALSE ;
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
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<R1X>>::id ,unsafe_pointer (m_fake ().addr ()))) ;
			R1X ret = move (tmp) ;
			m_fake ().finalize () ;
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
trait AUTO_PUREHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename AUTO_HELP<ALWAYS>::AutoHolder ;

	class PureHolder implement Holder {
	private:
		LENGTH mOrigin ;
		TEMP<UNIT1> mValue ;

	public:
		implicit PureHolder () = default ;

		imports LENGTH create[[nodiscard]] (VREF<TEMP<PureHolder>> where_) {
			const auto r1x = address (where_) ;
			auto &&tmp = unsafe_deref (where_) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r1x ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		template <class...ARG1>
		void initialize (XREF<ARG1>...objs) {
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			mValue = unsafe_deptr (rax.self) ;
			rax.release () ;
		}

		void finalize () override {
			unsafe_destroy (mValue) ;
		}

		void destroy () override {
			auto &&thiz = *this ;
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
		}

		LENGTH addr () const override {
			return address (mValue) ;
		}

		FLAG type_cabi () const override {
			return operator_cabi (TYPEAS<UNIT1>::id) ;
		}
	} ;
} ;
} ;

using Auto = typename U::AUTO_HELP<ALWAYS>::Auto ;

namespace U {
template <class...>
trait SCOPE_HELP ;

template <class UNIT1>
trait SCOPE_HELP<UNIT1 ,REQUIRE<ENUM_ANY<
	IS_VARIABLE<UNIT1> ,
	IS_CONSTANT<UNIT1>>>> {
	class Scope final {
	private:
		LENGTH mPointer ;

	public:
		implicit Scope () noexcept {
			mPointer = ZERO ;
		}

		explicit Scope (XREF<UNIT1> that) :Scope () {
			that.enter () ;
			mPointer = address (that) ;
		}

		implicit ~Scope () noexcept {
			auto &&thiz = *this ;
			if (mPointer == ZERO)
				return ;
			try_invoke ([&] () {
				m_fake ().leave () ;
			} ,[&] () {
				assert (FALSE) ;
			}) ;
			mPointer = ZERO ;
		}

		implicit Scope (CREF<Scope>) = delete ;

		inline VREF<Scope> operator= (CREF<Scope>) = delete ;

		implicit Scope (RREF<Scope> that) noexcept :Scope () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<Scope> operator= (RREF<Scope> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		void release () {
			mPointer = ZERO ;
		}

	private:
		XREF<UNIT1> m_fake () const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
			return keep[TYPEAS<XREF<UNIT1>>::id] (tmp) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Scope = typename U::SCOPE_HELP<UNIT1 ,ALWAYS>::Scope ;

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
		virtual CREF<UNIT1> at (CREF<INDEX> index) const leftvalue = 0 ;
	} ;

	using Holder = SliceHolder ;
	using SLICE_MAX_SIZE = ENUMAS<VAL ,U::ENUMID<(+4096)>> ;

	class Slice {
	private:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Slice>>>>
		explicit Slice (XREF<ARG1> id ,XREF<ARG2>...texts) {
			require (ENUM_ALL<IS_ARRAY<REMOVE_ALL<ARG2>>...>) ;
			using R1X = ENUM_INC<ENUM_SUM<ENUM_DEC<ARRAY_SIZE<REMOVE_ALL<ARG2>>>...>> ;
			require (ENUM_COMPR_LTEQ<R1X ,SLICE_MAX_SIZE>) ;
			using R2X = typename SLICE_IMPLHOLDER_HELP<UNIT1 ,R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::CREF_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = CRef<R2X>::intrusive (R3X::unique_stack (id) ,texts...) ;
		}

		imports CREF<Slice> zero () {
			return memorize ([&] () {
				return Slice (TYPEAS<struct anonymous>::id ,"") ;
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

		CREF<UNIT1> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mThis->at (index) ;
		}

		inline CREF<UNIT1> operator[] (CREF<INDEX> index) const leftvalue {
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
		Box<ARR<UNIT1 ,SIZE>> mSlice ;

	public:
		implicit ImplHolder () = delete ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,ImplHolder>>>>
		explicit ImplHolder (XREF<ARG1> text1 ,XREF<ARG2>...texts) {
			mSize = 0 ;
			mSlice = Box<ARR<UNIT1 ,SIZE>>::make () ;
			template_write (0 ,text1 ,texts...) ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		LENGTH addr () const override {
			return address (mSlice.self) ;
		}

		CREF<UNIT1> at (CREF<INDEX> index) const leftvalue override {
			return mSlice.self[index] ;
		}

	private:
		void template_write (CREF<INDEX> pos) {
			assert (pos < SIZE::value) ;
			mSize = pos ;
			mSlice.self[pos] = UNIT1 (0) ;
		}

		template <class ARG1 ,class...ARG2>
		void template_write (CREF<INDEX> pos ,XREF<ARG1> text1 ,XREF<ARG2>...texts) {
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
				mSlice.self[pos + ix] = r1x ;
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
trait FUNCTION_operator_name_HELP<VAL32 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAL32") ;
		}
	} ;
} ;

template <>
trait FUNCTION_operator_name_HELP<VAL64 ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAL64") ;
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
trait FUNCTION_operator_name_HELP<HUGE ,ALWAYS> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("HUGE") ;
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
		imports STRA cvt_hex_str (CREF<VAL> v) {
			assert (between (v ,0 ,16)) ;
			if (v < 10)
				return STRA (VAL ('0') + v) ;
			return STRA (VAL ('A') + v - 10) ;
		}

		inline Slice<STR> operator() () const {
			using R1X = BYTE_BASE<SIZE_OF<FLAG> ,ALIGN_OF<FLAG>> ;
			using R2X = ENUM_MUL<SIZE_OF<R1X> ,RANK2> ;
			const auto r1x = operator_cabi (TYPEAS<UNIT1>::id) ;
			const auto r2x = invoke ([&] () {
				Box<ARR<STRA ,ENUM_INC<R2X>>> ret = Box<ARR<STRA ,ENUM_INC<R2X>>>::make () ;
				const auto r3x = R1X (r1x) ;
				for (auto &&i : range (0 ,R2X::value)) {
					const auto r4x = 4 * (R2X::value - 1 - i) ;
					const auto r5x = VAL (R1X (r3x >> r4x) & R1X (0X0F)) ;
					ret.self[i] = cvt_hex_str (r5x) ;
				}
				ret.self[R2X::value] = 0 ;
				return move (ret) ;
			}) ;
			return slice ("class '" ,r2x.self ,"'") ;
		}
	} ;
} ;
} ;

struct FUNCTION_operator_name {
	template <class ARG1>
	inline Slice<STR> operator() (XREF<ARG1> id) const {
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
		virtual Slice<STR> type_name () const = 0 ;
	} ;

	using Holder = ClazzHolder ;

	class Clazz {
	private:
		CRef<Holder> mThis ;

	public:
		implicit Clazz () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Clazz>>>>
		explicit Clazz (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename CLAZZ_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::CREF_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = CRef<R2X>::intrusive (R3X::unique_stack (id)) ;
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

		Slice<STR> type_name () const {
			if (mThis == NULL)
				return Slice<STR>::zero () ;
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

		Slice<STR> type_name () const override {
			return operator_name (TYPEAS<UUID>::id) ;
		}
	} ;
} ;
} ;

using Clazz = typename U::CLAZZ_HELP<ALWAYS>::Clazz ;

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

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class = ENABLE<ENUM_ANY<
		IS_VARIABLE<ARG1> ,
		IS_CONSTANT<ARG1>>>>
		inline void operator() (CREF<Slice<STR>> name ,XREF<ARG1> expr) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename U::WATCH_HELP<R1X ,ALWAYS>::WATCH ;
		static R2X mInstance ;
		mInstance.mName = name ;
		mInstance.mAddress = address (expr) ;
		mInstance.mClazz = Clazz (TYPEAS<R1X>::id) ;
	}
} ;

static constexpr auto unsafe_watch = FUNCTION_unsafe_watch () ;

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
		CRef<Holder> mThis ;

	public:
		implicit Exception () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Exception>>>>
		explicit Exception (XREF<ARG1> id ,CREF<Slice<STR>> what_) noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename EXCEPTION_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename U::CREF_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			mThis = CRef<R2X>::intrusive (R3X::unique_stack (id) ,what_) ;
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
				return Slice<STR>::zero () ;
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

struct FUNCTION_try_invoke {
	template <class ARG1>
	inline void operator() (XREF<ARG1> proc1) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (XREF<ARG1> proc1 ,XREF<ARG2>...procs) const {
		auto &&thiz = *this ;
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		try {
			return proc1 () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		} catch (...) {
			dynamic_assert (FALSE) ;
		}
		return thiz (procs...) ;
	}
} ;

static constexpr auto try_invoke = FUNCTION_try_invoke () ;

template <class UNIT1>
inline void U::VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>>::ImplHolder::throw_bad_alloc () {
	dynamic_assert (FALSE) ;
}

template <class UNIT1>
inline void U::VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>>::ImplHolder::throw_bad_alloc () {
	dynamic_assert (FALSE) ;
}
} ;