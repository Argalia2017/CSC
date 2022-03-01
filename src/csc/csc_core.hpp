#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
using BOOL = csc_bool_t ;

static constexpr auto TRUE = BOOL (true) ;
static constexpr auto FALSE = BOOL (false) ;

using VAL32 = csc_int32_t ;
using VAL64 = csc_int64_t ;

static constexpr auto VAL32_MAX = VAL32 (2147483647) ;
static constexpr auto VAL32_MIN = -VAL32_MAX ;
static constexpr auto VAL32_ABS = VAL32_MIN - 1 ;
static constexpr auto VAL64_MAX = VAL64 (9223372036854775807) ;
static constexpr auto VAL64_MIN = -VAL64_MAX ;
static constexpr auto VAL64_ABS = VAL64_MIN - 1 ;

template <class...>
trait VAL_HELP ;

template <class DEPEND>
trait VAL_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL32<DEPEND>>> {
	using VAL = VAL32 ;

	static constexpr auto VAL_MAX = VAL32_MAX ;
	static constexpr auto VAL_MIN = VAL32_MIN ;
	static constexpr auto VAL_ABS = VAL32_ABS ;
} ;

template <class DEPEND>
trait VAL_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL64<DEPEND>>> {
	using VAL = VAL64 ;

	static constexpr auto VAL_MAX = VAL64_MAX ;
	static constexpr auto VAL_MIN = VAL64_MIN ;
	static constexpr auto VAL_ABS = VAL64_ABS ;
} ;

using VAL = typename VAL_HELP<DEPEND ,ALWAYS>::VAL ;

static constexpr auto VAL_MAX = VAL_HELP<DEPEND ,ALWAYS>::VAL_MAX ;
static constexpr auto VAL_MIN = VAL_HELP<DEPEND ,ALWAYS>::VAL_MIN ;
static constexpr auto VAL_ABS = VAL_HELP<DEPEND ,ALWAYS>::VAL_ABS ;

static constexpr auto ZERO = VAL (+0) ;
static constexpr auto IDEN = VAL (+1) ;
static constexpr auto NONE = VAL (-1) ;
static constexpr auto USED = VAL (-2) ;

using INDEX = VAL ;
using LENGTH = VAL ;
using FLAG = VAL ;

using SINGLE = csc_float32_t ;
using DOUBLE = csc_float64_t ;
using TRIPLE = csc_float128_t ;

static constexpr auto SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constexpr auto SINGLE_MIN = -SINGLE_MAX ;
static constexpr auto SINGLE_LOW = SINGLE (1.175494351E-38) ;
static constexpr auto SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constexpr auto SINGLE_INF = SINGLE (infinity) ;
static constexpr auto DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constexpr auto DOUBLE_MIN = -DOUBLE_MAX ;
static constexpr auto DOUBLE_LOW = DOUBLE (2.2250738585072014E-308) ;
static constexpr auto DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constexpr auto DOUBLE_INF = DOUBLE (infinity) ;

enum class BYTE :csc_byte8_t ;
enum class WORD :csc_byte16_t ;
enum class CHAR :csc_byte32_t ;
enum class DATA :csc_byte64_t ;
using HUGE = csc_byte128_t ;

inline constexpr BYTE operator| (CREF<BYTE> thiz_ ,CREF<BYTE> that) {
	return BYTE (csc_byte8_t (thiz_) | csc_byte8_t (that)) ;
}

inline void operator|= (VREF<BYTE> thiz_ ,CREF<BYTE> that) {
	thiz_ = thiz_ | that ;
}

inline constexpr BYTE operator& (CREF<BYTE> thiz_ ,CREF<BYTE> that) {
	return BYTE (csc_byte8_t (thiz_) & csc_byte8_t (that)) ;
}

inline void operator&= (VREF<BYTE> thiz_ ,CREF<BYTE> that) {
	thiz_ = thiz_ & that ;
}

inline constexpr BYTE operator^ (CREF<BYTE> thiz_ ,CREF<BYTE> that) {
	return BYTE (csc_byte8_t (thiz_) ^ csc_byte8_t (that)) ;
}

inline void operator^= (VREF<BYTE> thiz_ ,CREF<BYTE> that) {
	thiz_ = thiz_ ^ that ;
}

inline constexpr BYTE operator~ (CREF<BYTE> thiz_) {
	return BYTE (~csc_byte8_t (thiz_)) ;
}

inline constexpr BYTE operator<< (CREF<BYTE> thiz_ ,CREF<LENGTH> scale) {
	return BYTE (csc_byte8_t (thiz_) << scale) ;
}

inline constexpr BYTE operator>> (CREF<BYTE> thiz_ ,CREF<LENGTH> scale) {
	return BYTE (csc_byte8_t (thiz_) >> scale) ;
}

inline constexpr WORD operator| (CREF<WORD> thiz_ ,CREF<WORD> that) {
	return WORD (csc_byte16_t (thiz_) | csc_byte16_t (that)) ;
}

inline void operator|= (VREF<WORD> thiz_ ,CREF<WORD> that) {
	thiz_ = thiz_ | that ;
}

inline constexpr WORD operator& (CREF<WORD> thiz_ ,CREF<WORD> that) {
	return WORD (csc_byte16_t (thiz_) & csc_byte16_t (that)) ;
}

inline void operator&= (VREF<WORD> thiz_ ,CREF<WORD> that) {
	thiz_ = thiz_ & that ;
}

inline constexpr WORD operator^ (CREF<WORD> thiz_ ,CREF<WORD> that) {
	return WORD (csc_byte16_t (thiz_) ^ csc_byte16_t (that)) ;
}

inline constexpr WORD operator~ (CREF<WORD> thiz_) {
	return WORD (~csc_byte16_t (thiz_)) ;
}

inline constexpr WORD operator<< (CREF<WORD> thiz_ ,CREF<LENGTH> scale) {
	return WORD (csc_byte16_t (thiz_) << scale) ;
}

inline constexpr WORD operator>> (CREF<WORD> thiz_ ,CREF<LENGTH> scale) {
	return WORD (csc_byte16_t (thiz_) >> scale) ;
}

inline constexpr CHAR operator| (CREF<CHAR> thiz_ ,CREF<CHAR> that) {
	return CHAR (csc_byte32_t (thiz_) | csc_byte32_t (that)) ;
}

inline void operator|= (VREF<CHAR> thiz_ ,CREF<CHAR> that) {
	thiz_ = thiz_ | that ;
}

inline constexpr CHAR operator& (CREF<CHAR> thiz_ ,CREF<CHAR> that) {
	return CHAR (csc_byte32_t (thiz_) & csc_byte32_t (that)) ;
}

inline void operator&= (VREF<CHAR> thiz_ ,CREF<CHAR> that) {
	thiz_ = thiz_ & that ;
}

inline constexpr CHAR operator^ (CREF<CHAR> thiz_ ,CREF<CHAR> that) {
	return CHAR (csc_byte32_t (thiz_) ^ csc_byte32_t (that)) ;
}

inline constexpr CHAR operator~ (CREF<CHAR> thiz_) {
	return CHAR (~csc_byte32_t (thiz_)) ;
}

inline constexpr CHAR operator<< (CREF<CHAR> thiz_ ,CREF<LENGTH> scale) {
	return CHAR (csc_byte32_t (thiz_) << scale) ;
}

inline constexpr CHAR operator>> (CREF<CHAR> thiz_ ,CREF<LENGTH> scale) {
	return CHAR (csc_byte32_t (thiz_) >> scale) ;
}

inline constexpr DATA operator| (CREF<DATA> thiz_ ,CREF<DATA> that) {
	return DATA (csc_byte64_t (thiz_) | csc_byte64_t (that)) ;
}

inline void operator|= (VREF<DATA> thiz_ ,CREF<DATA> that) {
	thiz_ = thiz_ | that ;
}

inline constexpr DATA operator& (CREF<DATA> thiz_ ,CREF<DATA> that) {
	return DATA (csc_byte64_t (thiz_) & csc_byte64_t (that)) ;
}

inline void operator&= (VREF<DATA> thiz_ ,CREF<DATA> that) {
	thiz_ = thiz_ & that ;
}

inline constexpr DATA operator^ (CREF<DATA> thiz_ ,CREF<DATA> that) {
	return DATA (csc_byte64_t (thiz_) ^ csc_byte64_t (that)) ;
}

inline constexpr DATA operator~ (CREF<DATA> thiz_) {
	return DATA (~csc_byte64_t (thiz_)) ;
}

inline constexpr DATA operator<< (CREF<DATA> thiz_ ,CREF<LENGTH> scale) {
	return DATA (csc_byte64_t (thiz_) << scale) ;
}

inline constexpr DATA operator>> (CREF<DATA> thiz_ ,CREF<LENGTH> scale) {
	return DATA (csc_byte64_t (thiz_) >> scale) ;
}

using STRA = csc_char_t ;
using STRW = csc_wchar_t ;
using STRU8 = csc_char8_t ;
using STRU16 = csc_char16_t ;
using STRU32 = csc_char32_t ;

template <class...>
trait STR_HELP ;

template <class DEPEND>
trait STR_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRA<DEPEND>>> {
	using STR = STRA ;
} ;

template <class DEPEND>
trait STR_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRW<DEPEND>>> {
	using STR = STRW ;
} ;

using STR = typename STR_HELP<DEPEND ,ALWAYS>::STR ;

static constexpr auto NULL = nullptr ;

template <class...>
trait SIZE_OF_HELP ;

template <class UNIT1>
trait SIZE_OF_HELP<UNIT1 ,ALWAYS> {
	using RET = ENUMAS<VAL ,ENUMID<(sizeof (UNIT1))>> ;
} ;

template <class UNIT1>
using SIZE_OF = typename SIZE_OF_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait ALIGN_OF_HELP ;

template <class UNIT1>
trait ALIGN_OF_HELP<UNIT1 ,ALWAYS> {
	using RET = ENUMAS<VAL ,ENUMID<(alignof (UNIT1))>> ;
} ;

template <class UNIT1>
using ALIGN_OF = typename ALIGN_OF_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait COUNT_OF_HELP ;

template <class...UNIT1>
trait COUNT_OF_HELP<TYPEAS<UNIT1...> ,ALWAYS> {
	using RET = ENUMAS<VAL ,ENUMID<(sizeof... (UNIT1))>> ;
} ;

template <class UNIT1>
using COUNT_OF = typename COUNT_OF_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class UNIT1 ,class UNIT2>
using IS_SAME = typename IS_SAME_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

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

template <class UNIT1>
using IS_ENUM = typename IS_ENUM_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class UNIT1>
using IS_TYPE = typename IS_TYPE_HELP<UNIT1 ,ALWAYS>::RET ;

using ENUM_ZERO = ENUMAS<VAL ,ENUMID<ZERO>> ;
using ENUM_IDEN = ENUMAS<VAL ,ENUMID<IDEN>> ;
using ENUM_NONE = ENUMAS<VAL ,ENUMID<NONE>> ;
using ENUM_USED = ENUMAS<VAL ,ENUMID<USED>> ;

template <class...>
trait ENABLE_HELP ;

template <class COND ,class YES>
trait ENABLE_HELP<COND ,YES ,REQUIRE<COND>> {
	using RET = YES ;
} ;

template <class COND ,class YES = void>
using ENABLE = typename ENABLE_HELP<COND ,YES ,ALWAYS>::RET ;

template <class UNIT1>
using ENUM_CHECK = ENABLE<IS_ENUM<UNIT1> ,UNIT1> ;

template <class UNIT1>
using TYPE_CHECK = ENABLE<IS_TYPE<UNIT1> ,UNIT1> ;

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

template <class COND ,class YES ,class NO>
using CONDITIONAL = typename CONDITIONAL_HELP<COND ,YES ,NO ,ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = ENUMAS<BOOL ,ENUMID<(ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<UNIT1 ,UNIT2>> ;

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

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = typename ENUM_COMPR_HELP<
	ENUMAS<BOOL ,ENUMID<(ENUM_CHECK<UNIT1>::value < ENUM_CHECK<UNIT2>::value)>> ,
	ENUMAS<BOOL ,ENUMID<(ENUM_CHECK<UNIT2>::value < ENUM_CHECK<UNIT1>::value)>> ,
	ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = ENUMAS<BOOL ,ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LTEQ = ENUMAS<BOOL ,ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = ENUMAS<BOOL ,ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GTEQ = ENUMAS<BOOL ,ENUMID<(ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO)>> ;

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
using ENUM_ADD = ENUMAS<VAL ,ENUMID<(ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = ENUMAS<VAL ,ENUMID<(ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = ENUMAS<VAL ,ENUMID<(ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = ENUMAS<VAL ,ENUMID<(ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = ENUMAS<VAL ,ENUMID<(ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value)>> ;

template <class UNIT1>
using ENUM_MINUS = ENUM_SUB<ENUM_ZERO ,UNIT1> ;

template <class UNIT1>
using ENUM_INC = ENUM_ADD<UNIT1 ,ENUM_IDEN> ;

template <class UNIT1>
using ENUM_DEC = ENUM_SUB<UNIT1 ,ENUM_IDEN> ;

template <class...>
trait TYPE_CAT_HELP ;

template <class...UNIT1 ,class...UNIT2>
trait TYPE_CAT_HELP<TYPEAS<UNIT1...> ,TYPEAS<UNIT2...> ,ALWAYS> {
	using RET = TYPEAS<UNIT1... ,UNIT2...> ;
} ;

template <class UNIT1 ,class UNIT2>
using TYPE_CAT = typename TYPE_CAT_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait REFLECT_TYPE_HELP ;

template <class UNIT1>
trait REFLECT_TYPE_HELP<TYPEAS<UNIT1> ,ALWAYS> {
	using FIRST_ONE = UNIT1 ;
	using FIRST_REST = TYPEAS<> ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_TYPE_HELP<TYPEAS<UNIT1 ,UNIT2> ,ALWAYS> {
	using FIRST_ONE = UNIT1 ;
	using FIRST_REST = TYPEAS<UNIT2> ;
	using SECOND_ONE = UNIT2 ;
	using SECOND_REST = TYPEAS<> ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3 ,class...UNIT4>
trait REFLECT_TYPE_HELP<TYPEAS<UNIT1 ,UNIT2 ,UNIT3 ,UNIT4...> ,ALWAYS> {
	using FIRST_ONE = UNIT1 ;
	using FIRST_REST = TYPEAS<UNIT2 ,UNIT3 ,UNIT4...> ;
	using SECOND_ONE = UNIT2 ;
	using SECOND_REST = TYPEAS<UNIT3 ,UNIT4...> ;
	using THIRD_ONE = UNIT3 ;
	using THIRD_REST = TYPEAS<UNIT4...> ;
} ;

template <class UNIT1>
using TYPE_FIRST_ONE = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::FIRST_ONE ;

template <class UNIT1>
using TYPE_FIRST_REST = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::FIRST_REST ;

template <class UNIT1>
using TYPE_SECOND_ONE = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::SECOND_ONE ;

template <class UNIT1>
using TYPE_SECOND_REST = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::SECOND_REST ;

template <class UNIT1>
using TYPE_THIRD_ONE = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::THIRD_ONE ;

template <class UNIT1>
using TYPE_THIRD_REST = typename REFLECT_TYPE_HELP<UNIT1 ,ALWAYS>::THIRD_REST ;

template <class UNIT1>
using TYPE_UNWIND = ENABLE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>> ,TYPE_FIRST_ONE<UNIT1>> ;

template <class...>
trait TYPE_REVERSE_HELP ;

template <class UNIT1>
trait TYPE_REVERSE_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT1>
trait TYPE_REVERSE_HELP<UNIT1 ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using RET = TYPEAS<R1X> ;
} ;

template <class UNIT1>
trait TYPE_REVERSE_HELP<UNIT1 ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT1>>>> {
	using R1X = TYPEAS<TYPE_FIRST_ONE<UNIT1>> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R2X ,R1X> ;
} ;

template <class UNIT1>
using TYPE_REVERSE = typename TYPE_REVERSE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class UNIT1>
using TYPE_LAST_ONE = TYPE_FIRST_ONE<TYPE_REVERSE<UNIT1>> ;

template <class UNIT1>
using TYPE_LAST_REST = TYPE_REVERSE<TYPE_FIRST_REST<TYPE_REVERSE<UNIT1>>> ;

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

template <class ITEM ,class SIZE>
using TYPE_REPEAT = typename TYPE_REPEAT_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

template <class...>
trait TYPE_SENQUENCE_HELP ;

template <class SIZE ,class NTH>
trait TYPE_SENQUENCE_HELP<SIZE ,NTH ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = TYPEAS<> ;
} ;

template <class SIZE ,class NTH>
trait TYPE_SENQUENCE_HELP<SIZE ,NTH ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using R1X = typename TYPE_REPEAT_HELP<ENUM_DEC<SIZE> ,ENUM_INC<NTH> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<TYPEAS<NTH> ,R1X> ;
} ;

template <class SIZE>
using TYPE_SENQUENCE = typename TYPE_SENQUENCE_HELP<SIZE ,ENUM_ZERO ,ALWAYS>::RET ;

template <class...>
trait TYPE_PICK_HELP ;

template <class UNIT1 ,class NTH>
trait TYPE_PICK_HELP<UNIT1 ,NTH ,REQUIRE<ENUM_EQ_ZERO<NTH>>> {
	using RET = TYPE_FIRST_ONE<UNIT1> ;
} ;

template <class UNIT1 ,class NTH>
trait TYPE_PICK_HELP<UNIT1 ,NTH ,REQUIRE<ENUM_GT_ZERO<NTH>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<UNIT1> ,ENUM_DEC<NTH> ,ALWAYS>::RET ;
} ;

template <class UNIT1 ,class NTH>
using TYPE_PICK = typename TYPE_PICK_HELP<UNIT1 ,NTH ,ALWAYS>::RET ;

template <class...>
trait ENUM_ALL_HELP ;

template <class UNIT1>
trait ENUM_ALL_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT1>
trait ENUM_ALL_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,R2X ,ENUM_FALSE> ;
} ;

template <class...UNIT1>
using ENUM_ALL = typename ENUM_ALL_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_ANY_HELP ;

template <class UNIT1>
trait ENUM_ANY_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT1>
trait ENUM_ANY_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_TRUE ,R2X> ;
} ;

template <class...UNIT1>
using ENUM_ANY = typename ENUM_ANY_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUM_HELP ;

template <class UNIT1>
trait ENUM_SUM_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class UNIT1>
trait ENUM_SUM_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using R2X = typename ENUM_SUM_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using RET = ENUM_ADD<R1X ,R2X> ;
} ;

template <class...UNIT1>
using ENUM_SUM = typename ENUM_SUM_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

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
	//@warn: clang5.0 is useless to compile
	implicit Interface (Interface &&) = default ;
	inline VREF<Interface> operator= (Interface &&) = default ;
} ;

template <class...>
trait PLACEHOLDER_HELP ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	class PlaceHolder {} ;
} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using SUPER = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::PlaceHolder ;

	class PlaceHolder implement SUPER {} ;
} ;

template <class RANK>
using PlaceHolder = typename PLACEHOLDER_HELP<RANK ,ALWAYS>::PlaceHolder ;

using RANK0 = ENUMAS<VAL ,ENUMID<(+0)>> ;
using RANK1 = ENUMAS<VAL ,ENUMID<(+1)>> ;
using RANK2 = ENUMAS<VAL ,ENUMID<(+2)>> ;
using RANK3 = ENUMAS<VAL ,ENUMID<(+3)>> ;
using RANK4 = ENUMAS<VAL ,ENUMID<(+4)>> ;
using RANK5 = ENUMAS<VAL ,ENUMID<(+5)>> ;
using RANK6 = ENUMAS<VAL ,ENUMID<(+6)>> ;
using RANK7 = ENUMAS<VAL ,ENUMID<(+7)>> ;
using RANK8 = ENUMAS<VAL ,ENUMID<(+8)>> ;
using RANK9 = ENUMAS<VAL ,ENUMID<(+9)>> ;
using RANKX = ENUMAS<VAL ,ENUMID<10>> ;

template <class...>
trait ENUM_POW2_HELP ;

template <class UNIT1>
trait ENUM_POW2_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<UNIT1>>> {
	using RET = ENUM_IDEN ;
} ;

template <class UNIT1>
trait ENUM_POW2_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<UNIT1>>> {
	using R1X = typename ENUM_POW2_HELP<ENUM_DEC<UNIT1> ,ALWAYS>::RET ;
	using RET = ENUM_MUL<R1X ,RANK2> ;
} ;

template <class UNIT1>
using ENUM_POW2 = typename ENUM_POW2_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class...>
trait PLACEHOLDER_RANK_HELP ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK0>>>> {
	using RET = RANK0 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK1>>>> {
	using RET = RANK1 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK2>>>> {
	using RET = RANK2 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK3>>>> {
	using RET = RANK3 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK4>>>> {
	using RET = RANK4 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK5>>>> {
	using RET = RANK5 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK6>>>> {
	using RET = RANK6 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK7>>>> {
	using RET = RANK7 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK8>>>> {
	using RET = RANK8 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANK9>>>> {
	using RET = RANK9 ;
} ;

template <class UNIT1>
trait PLACEHOLDER_RANK_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,PlaceHolder<RANKX>>>> {
	using RET = RANKX ;
} ;

template <class UNIT1>
using PLACEHOLDER_RANK = typename PLACEHOLDER_RANK_HELP<UNIT1 ,ALWAYS>::RET ;

template <class UNIT1>
using IS_BOOL = IS_SAME<UNIT1 ,BOOL> ;

template <class UNIT1>
using IS_VALUE = ENUM_ANY<
	IS_SAME<UNIT1 ,VAL32> ,
	IS_SAME<UNIT1 ,VAL64>> ;

template <class UNIT1>
using IS_FLOAT = ENUM_ANY<
	IS_SAME<UNIT1 ,SINGLE> ,
	IS_SAME<UNIT1 ,DOUBLE> ,
	IS_SAME<UNIT1 ,TRIPLE>> ;

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
	IS_SAME<UNIT1 ,DATA> ,
	IS_SAME<UNIT1 ,HUGE>> ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,DEF<typeof (NULL)>> ;

template <class UNIT1>
using IS_VOID = IS_SAME<UNIT1 ,void> ;

template <class UNIT1>
using IS_SCALAR = ENUM_ANY<
	IS_VALUE<UNIT1> ,
	IS_FLOAT<UNIT1>> ;

template <class UNIT1>
using IS_BASIC = ENUM_ANY<
	IS_BOOL<UNIT1> ,
	IS_VALUE<UNIT1> ,
	IS_FLOAT<UNIT1> ,
	IS_STR<UNIT1> ,
	IS_BYTE<UNIT1> ,
	IS_NULL<UNIT1>> ;

using VARIABLE = ENUMAS<VAL ,ENUMID<(-1)>> ;
using CONSTANT = ENUMAS<VAL ,ENUMID<(-2)>> ;
using REGISTER = ENUMAS<VAL ,ENUMID<(-3)>> ;
using FLEXIBLE = ENUMAS<VAL ,ENUMID<(-4)>> ;

template <class...>
trait REFLECT_REF_HELP ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,VREF<UNIT1>>>> {
	using RET = VARIABLE ;
} ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,CREF<UNIT1>>>> {
	using RET = CONSTANT ;
} ;

template <class UNIT1>
trait REFLECT_REF_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,RREF<UNIT1>>>> {
	using RET = REGISTER ;
} ;

template <class UNIT1>
using REFLECT_REF = typename REFLECT_REF_HELP<XREF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_VARIABLE = IS_SAME<REFLECT_REF<UNIT1> ,VARIABLE> ;

template <class UNIT1>
using IS_CONSTANT = IS_SAME<REFLECT_REF<UNIT1> ,CONSTANT> ;

template <class UNIT1>
using IS_REGISTER = IS_SAME<REFLECT_REF<UNIT1> ,REGISTER> ;

template <class...>
trait REFLECT_POINTER_HELP ;

template <class UNIT1>
trait REFLECT_POINTER_HELP<UNIT1 ,ALWAYS> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT1>
trait REFLECT_POINTER_HELP<DEF<UNIT1 *> ,ALWAYS> {
	using RET = TYPEAS<UNIT1> ;
} ;

template <class UNIT1>
using REFLECT_POINTER = typename REFLECT_POINTER_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_POINTER = ENUM_GT_ZERO<COUNT_OF<REFLECT_POINTER<UNIT1>>> ;

template <class...>
trait PTR_HELP ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_VARIABLE<UNIT1>>> {
	using RET = DEF<const DEF<UNIT2 *>> ;
} ;

template <class UNIT1 ,class UNIT2>
trait PTR_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_CONSTANT<UNIT1>>> {
	using RET = DEF<const DEF<const UNIT2 *>> ;
} ;

template <class UNIT1>
using PTR = typename PTR_HELP<XREF<UNIT1> ,REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

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
	using R1X = ENUMAS<VAL ,ENUMID<UNIT2>> ;
	using RET = TYPEAS<UNIT1 ,R1X> ;
} ;

template <class UNIT1>
using REFLECT_ARRAY = typename REFLECT_ARRAY_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_ARRAY = ENUM_GT_ZERO<COUNT_OF<REFLECT_ARRAY<UNIT1>>> ;

template <class UNIT1>
using ARRAY_ITEM = TYPE_FIRST_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class UNIT1>
using ARRAY_SIZE = TYPE_SECOND_ONE<REFLECT_ARRAY<UNIT1>> ;

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

template <class ITEM ,class SIZE = ENUM_ZERO>
using ARR = typename ARR_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

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

template <class UNIT1>
using REFLECT_FUNCTION = typename REFLECT_FUNCTION_HELP<DEF<typeof (&UNIT1::operator())> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_FUNCTION = ENUM_GT_ZERO<COUNT_OF<REFLECT_FUNCTION<UNIT1>>> ;

template <class UNIT1>
using FUNCTION_RETURN = TYPE_FIRST_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using FUNCTION_PARAMS = TYPE_SECOND_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class...>
trait IS_CLASS_HELP ;

template <class UNIT1>
trait IS_CLASS_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_ENUM<UNIT1> ;
	using R2X = MACRO_IS_CLASS<UNIT1> ;
	using R3X = IS_ENUM<UNIT1> ;
	using R4X = IS_TYPE<UNIT1> ;
	using R5X = IS_BASIC<UNIT1> ;
	using RET = ENUM_ALL<ENUM_ANY<R1X ,R2X> ,ENUM_NOT<ENUM_ANY<R3X ,R4X ,R5X>>> ;
} ;

template <class UNIT1>
using IS_CLASS = typename IS_CLASS_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_DEFAULT_HELP ;

template <class UNIT1>
trait IS_DEFAULT_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_CONSTRUCTIBLE<UNIT1> ;
	using R2X = MACRO_IS_DESTRUCTIBLE<UNIT1> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<UNIT1> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<UNIT1> ;
	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class UNIT1>
using IS_DEFAULT = typename IS_DEFAULT_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_CLONEABLE_HELP ;

template <class UNIT1>
trait IS_CLONEABLE_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_COPY_CONSTRUCTIBLE<UNIT1> ;
	using R2X = MACRO_IS_COPY_ASSIGNABLE<UNIT1> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<UNIT1> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<UNIT1> ;
	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class UNIT1>
using IS_CLONEABLE = typename IS_CLONEABLE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_TRIVIAL_HELP ;

template <class UNIT1>
trait IS_TRIVIAL_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_TRIVIAL_CONSTRUCTIBLE<UNIT1> ;
	using R2X = MACRO_IS_TRIVIAL_DESTRUCTIBLE<UNIT1> ;
	using RET = ENUM_ALL<R1X ,R2X> ;
} ;

template <class UNIT1>
using IS_TRIVIAL = typename IS_TRIVIAL_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_INTERFACE_HELP ;

template <class UNIT1>
trait IS_INTERFACE_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	using R1X = MACRO_IS_INTERFACE<UNIT1> ;
	using R2X = MACRO_IS_EXTEND<Interface ,UNIT1> ;
	using RET = ENUM_ALL<R1X ,R2X> ;
} ;

template <class UNIT1>
using IS_INTERFACE = typename IS_INTERFACE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_EXTEND_HELP ;

template <class FROM ,class TO>
trait IS_EXTEND_HELP<FROM ,TO ,ALWAYS> {
	using R1X = IS_SAME<FROM ,TO> ;
	using R2X = MACRO_IS_EXTEND<FROM ,TO> ;
	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class FROM ,class TO>
using IS_EXTEND = typename IS_EXTEND_HELP<FROM ,TO ,ALWAYS>::RET ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,DEF<typeof (PH0)>> ;

template <class...>
trait IS_CONVERTIBLE_HELP ;

template <class FROM ,class TO>
trait IS_CONVERTIBLE_HELP<FROM ,TO ,ALWAYS> {
	using R1X = MACRO_IS_CONVERTIBLE<FROM ,TO> ;
	using RET = ENUM_ANY<R1X> ;
} ;

template <class FROM ,class TO>
using IS_CONVERTIBLE = typename IS_CONVERTIBLE_HELP<XREF<FROM> ,XREF<TO> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_OBJECT = ENUM_ANY<
	IS_BASIC<UNIT1> ,
	IS_POINTER<UNIT1> ,
	IS_CLASS<UNIT1>> ;

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
	ENUM_EQUAL<SIZE ,SIZE_OF<DATA>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<DATA>>>>> {
	using RET = DATA ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<HUGE>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<HUGE>>>>> {
	using RET = HUGE ;
} ;

//@warn: g++4.8 is too useless to compile
template <class UNIT1>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<UNIT1> ,ALIGN_OF<UNIT1> ,ALWAYS>::RET ;

template <class...>
trait TEXT_BASE_HELP ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<STRU8>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU8>>>>> {
	using RET = STRU8 ;
} ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<STRU16>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU16>>>>> {
	using RET = STRU16 ;
} ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<STRU32>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU32>>>>> {
	using RET = STRU32 ;
} ;

template <class UNIT1>
using TEXT_BASE = typename TEXT_BASE_HELP<SIZE_OF<UNIT1> ,ALIGN_OF<UNIT1> ,ALWAYS>::RET ;

using STRUA = TEXT_BASE<STRA> ;
using STRUW = TEXT_BASE<STRW> ;

template <class...>
trait STORAGE_HELP ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<BYTE>>>> {
	using R1X = BYTE ;
	using R2X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<ALIGN>> ,ALIGN> ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<WORD>>>> {
	using R1X = WORD ;
	using R2X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<ALIGN>> ,ALIGN> ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<CHAR>>>> {
	using R1X = CHAR ;
	using R2X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<ALIGN>> ,ALIGN> ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<DATA>>>> {
	using R1X = DATA ;
	using R2X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<ALIGN>> ,ALIGN> ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<HUGE>>>> {
	using R1X = HUGE ;
	using R2X = ENUM_DIV<ENUM_ADD<SIZE ,ENUM_DEC<ALIGN>> ,ALIGN> ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN = ENUM_IDEN>
using Storage = typename STORAGE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

template <class...>
trait TEMP_HELP ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using RET = TEMPID<UNIT1 ,void> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,TEMPID<UNIT1 ,void>>>> {
	using RET = TEMPID<TEMPID<UNIT1 ,void> ,void> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	using R1X = ENUM_GT_ZERO<ARRAY_SIZE<UNIT1>> ;
	using R2X = CONDITIONAL<R1X ,UNIT1 ,void> ;
	using RET = TEMPID<UNIT1 ,R2X> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	ENUM_NOT<IS_VOID<UNIT1>> ,
	ENUM_NOT<IS_SAME<UNIT1 ,TEMPID<void ,void>>> ,
	IS_OBJECT<UNIT1>>>> {
	using RET = TEMPID<UNIT1 ,UNIT1> ;
} ;

template <class UNIT1>
using TEMP = typename TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class...>
trait REMOVE_TEMP_HELP ;

template <class UNIT1>
trait REMOVE_TEMP_HELP<UNIT1 ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1 ,class UNIT2>
trait REMOVE_TEMP_HELP<TEMPID<UNIT1 ,UNIT2> ,ALWAYS> {
	using RET = UNIT1 ;
} ;

template <class UNIT1>
using REMOVE_TEMP = typename REMOVE_TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_TEMP = IS_SAME<UNIT1 ,TEMP<REMOVE_TEMP<UNIT1>>> ;

template <class UNIT1>
using ENUM_ABS = CONDITIONAL<ENUM_COMPR_GTEQ<UNIT1 ,ENUM_ZERO> ,UNIT1 ,ENUM_MINUS<UNIT1>> ;

template <class...>
trait ENUM_MIN_HELP ;

template <class UNIT1>
trait ENUM_MIN_HELP<UNIT1 ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>>>> {
	using RET = TYPE_FIRST_ONE<UNIT1> ;
} ;

template <class UNIT1>
trait ENUM_MIN_HELP<UNIT1 ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using R2X = typename ENUM_MIN_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_LTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...UNIT1>
using ENUM_MIN = typename ENUM_MIN_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_MAX_HELP ;

template <class UNIT1>
trait ENUM_MAX_HELP<UNIT1 ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT1>>>> {
	using RET = TYPE_FIRST_ONE<UNIT1> ;
} ;

template <class UNIT1>
trait ENUM_MAX_HELP<UNIT1 ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT1>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT1> ;
	using R2X = typename ENUM_MAX_HELP<TYPE_FIRST_REST<UNIT1> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_GTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...UNIT1>
using ENUM_MAX = typename ENUM_MAX_HELP<TYPEAS<UNIT1...> ,ALWAYS>::RET ;

template <class CURR ,class BEGIN ,class END>
using ENUM_BETWEEN = ENUM_ALL<
	ENUM_COMPR_GTEQ<CURR ,BEGIN> ,
	ENUM_COMPR_LT<CURR ,END>> ;

struct FUNCTION_noop {
	inline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	inline void operator() (XREF<ARG1> obj) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_unsafe_switch {
	inline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	inline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto unsafe_switch = FUNCTION_unsafe_switch () ;

template <class...>
trait FUNCTION_unsafe_barrier_HELP ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			noop () ;
		}
	} ;
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

struct FUNCTION_unsafe_barrier {
	inline forceinline void operator() () const noexcept {
		using R1X = typename FUNCTION_unsafe_barrier_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_barrier ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto unsafe_barrier = FUNCTION_unsafe_barrier () ;

template <class...>
trait FUNCTION_unsafe_break_HELP ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__debugbreak () ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

struct FUNCTION_unsafe_break {
	inline forceinline void operator() () const noexcept {
		using R1X = typename FUNCTION_unsafe_break_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_break ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

struct FUNCTION_unsafe_abort {
	imports void invoke () ;

	inline forceinline void operator() () const noexcept {
		return invoke () ;
	}
} ;

static constexpr auto unsafe_abort = FUNCTION_unsafe_abort () ;

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
	inline FLAG operator() (XREF<ARG1> obj) const noexcept {
		return FLAG (&unsafe_deptr (obj)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

template <class...>
trait FUNCTION_unsafe_cast_impl_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	struct FUNCTION_unsafe_cast_impl {
		template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
		inline VREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = REMOVE_ALL<ARG1> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
		inline CREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = REMOVE_ALL<ARG1> ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_unsafe_cast_impl {
		template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
		inline VREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = REMOVE_ALL<ARG1> ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
		inline CREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = REMOVE_ALL<ARG1> ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<REMOVE_TEMP<R3X> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}
	} ;
} ;

template <class UNIT1>
using FUNCTION_unsafe_cast_impl = typename FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_unsafe_cast_impl ;

struct FUNCTION_unsafe_cast {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<TEMP<void>> operator() (XREF<ARG1> obj) const noexcept {
		return thiz[TYPEAS<TEMP<void>>::id] (obj) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<TEMP<void>> operator() (XREF<ARG1> obj) const noexcept {
		return thiz[TYPEAS<TEMP<void>>::id] (obj) ;
	}

	template <class ARG1>
	inline CREF<FUNCTION_unsafe_cast_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_unsafe_cast_impl<R1X> ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_unsafe_pointer {
	inline VREF<TEMP<void>> operator() (CREF<FLAG> addr_) const noexcept {
		assert (addr_ != ZERO) ;
		const auto r1x = reinterpret_cast<PTR<VREF<TEMP<void>>>> (addr_) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_pointer = FUNCTION_unsafe_pointer () ;

struct FUNCTION_unsafe_array {
	template <class ARG1 ,class = ENABLE<IS_VARIABLE<ARG1>>>
	inline VREF<ARR<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		const auto r1x = reinterpret_cast<PTR<VREF<ARR<R1X>>>> (address (obj)) ;
		return (*r1x) ;
	}

	template <class ARG1 ,class = ENABLE<IS_CONSTANT<ARG1>>>
	inline CREF<ARR<REMOVE_ALL<ARG1>>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		const auto r1x = reinterpret_cast<PTR<CREF<ARR<R1X>>>> (address (obj)) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_array = FUNCTION_unsafe_array () ;

struct FUNCTION_swap {
	template <class ARG1 ,class ARG2>
	inline void operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const noexcept {
		//@warn: no class should depend on its address
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		auto rax = unsafe_deptr (obj1) ;
		unsafe_deptr (obj1) = unsafe_deptr (obj2) ;
		unsafe_deptr (obj2) = rax ;
		unsafe_barrier () ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class UNIT1>
trait FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_keep_impl {
		template <class ARG1 ,class = ENABLE<ENUM_ANY<IS_VARIABLE<ARG1> ,IS_CONSTANT<ARG1>>>>
		inline XREF<UNIT1> operator() (XREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT1>> (obj) ;
		}
	} ;
} ;

template <class UNIT1>
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_keep_impl ;

struct FUNCTION_keep {
	template <class ARG1>
	inline CREF<FUNCTION_keep_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_keep_impl<R1X> ;
		static constexpr auto M_INVOKE = R2X () ;
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

struct FUNCTION_drop {
	template <class ARG1>
	inline void operator() (XREF<ARG1> obj) const {
		const auto r1x = move (obj) ;
		noop (r1x) ;
	}
} ;

static constexpr auto drop = FUNCTION_drop () ;

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
		using R2X = Storage<SIZE_OF<R1X>> ;
		auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::id] (unsafe_deptr (obj))) ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= SIZE_OF<R1X>::value)
				break ;
			tmp[ix] = BYTE (0X00) ;
			ix++ ;
		}
		unsafe_barrier () ;
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
		require (IS_DEFAULT<R2X>) ;
		zeroize (thiz_) ;
		new ((&thiz_)) R2X () ;
		unsafe_barrier () ;
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
		unsafe_barrier () ;
	}
} ;

static constexpr auto unsafe_destroy = FUNCTION_unsafe_destroy () ;

template <class...>
trait FUNCTION_bitwise_impl_HELP ;

template <class UNIT1>
trait FUNCTION_bitwise_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_bitwise_impl {
		template <class ARG1>
		inline UNIT1 operator() (XREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = REMOVE_ALL<ARG1> ;
			require (IS_TRIVIAL<R1X>) ;
			require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
			using R3X = Storage<SIZE_OF<R1X>> ;
			R1X ret ;
			auto &&tmp_a = unsafe_cast[TYPEAS<TEMP<R3X>>::id] (unsafe_deptr (ret)) ;
			auto &&tmp_b = unsafe_cast[TYPEAS<TEMP<R3X>>::id] (unsafe_deptr (obj)) ;
			tmp_a = tmp_b ;
			unsafe_barrier () ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
using FUNCTION_bitwise_impl = typename FUNCTION_bitwise_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_bitwise_impl ;

struct FUNCTION_bitwise {
	template <class ARG1>
	inline BYTE_BASE<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = BYTE_BASE<R1X> ;
		return thiz[TYPEAS<R2X>::id] (obj) ;
	}

	template <class ARG1>
	inline CREF<FUNCTION_bitwise_impl<REMOVE_ALL<ARG1>>> operator[] (XREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_bitwise_impl<R1X> ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline XREF<REMOVE_ALL<ARG1>> operator() (XREF<ARG1> id) const noexcept {
		assert (FALSE) ;
		unsafe_abort () ;
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

struct FUNCTION_vabs {
	inline VAL operator() (CREF<VAL> obj) const {
		assert (obj != VAL_ABS) ;
		if (obj >= 0)
			return obj ;
		return -obj ;
	}
} ;

static constexpr auto vabs = FUNCTION_vabs () ;

struct FUNCTION_vmin {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const {
		if (obj1 <= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto vmin = FUNCTION_vmin () ;

struct FUNCTION_vmax {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const {
		if (obj1 >= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto vmax = FUNCTION_vmax () ;

struct FUNCTION_vbetween {
	inline BOOL operator() (CREF<VAL> curr ,CREF<VAL> begin_ ,CREF<VAL> end_) const {
		if (curr < begin_)
			return FALSE ;
		if (curr >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto vbetween = FUNCTION_vbetween () ;

struct FUNCTION_valign {
	inline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto valign = FUNCTION_valign () ;

template <class...>
trait CABI_HELP ;

template <class UUID>
trait CABI_HELP<UUID ,ALWAYS> {
	class CABI implement Interface {} ;
} ;

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename CABI_HELP<R1X ,ALWAYS>::CABI ;
		const auto r1x = R2X () ;
		const auto r2x = bitwise (keep[TYPEAS<CREF<Interface>>::id] (r1x)) ;
		return FLAG (r2x) ;
	}
} ;

static constexpr auto operator_cabi = FUNCTION_operator_cabi () ;

template <class...>
trait INDEXITERATOR_HELP ;

template <class DEPEND>
trait INDEXITERATOR_HELP<DEPEND ,ALWAYS> {
	class IndexIterator {
	protected:
		INDEX mBegin ;
		INDEX mCurr ;
		INDEX mEnd ;

	public:
		implicit IndexIterator () = delete ;

		explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			mBegin = begin_ ;
			mCurr = begin_ ;
			mEnd = vmax (begin_ ,end_) ;
		}

		LENGTH length () const {
			return mEnd - mBegin ;
		}

		IndexIterator begin () const {
			return thiz ;
		}

		IndexIterator end () const {
			return thiz ;
		}

		BOOL good () const {
			return mCurr < mEnd ;
		}

		inline BOOL operator== (CREF<IndexIterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<IndexIterator>) const {
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

using IndexIterator = typename INDEXITERATOR_HELP<DEPEND ,ALWAYS>::IndexIterator ;

struct FUNCTION_iter {
	inline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return IndexIterator (begin_ ,end_) ;
	}
} ;

static constexpr auto iter = FUNCTION_iter () ;

template <class UNIT1>
using HAS_EQUAL = IS_BOOL<typeof (bad (TYPEAS<CREF<UNIT1>>::id).equal (bad (TYPEAS<CREF<UNIT1>>::id)))> ;

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
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_POINTER<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return FLAG (obj1) == FLAG (obj2) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			for (auto &&i : iter (0 ,ARRAY_SIZE<UNIT1>::value)) {
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
			return template_equal (PHX ,obj1 ,obj2) ;
		}

		template <class ARG1 = DEPEND ,class = ENABLE<HAS_EQUAL<DEPENDENT<UNIT1 ,ARG1>>>>
		BOOL template_equal (CREF<typeof (PH2)> ,CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return obj1.equal (obj2) ;
		}

		template <class ARG1 = DEPEND>
		BOOL template_equal (CREF<typeof (PH1)> ,CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return TRUE ;
		}
	} ;
} ;

struct FUNCTION_operator_equal {
	template <class ARG1 ,class ARG2>
	inline BOOL operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename FUNCTION_operator_equal_HELP<R1X ,ALWAYS>::FUNCTION_operator_equal ;
		static constexpr auto M_INVOKE = R3X () ;
		return M_INVOKE (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_equal = FUNCTION_operator_equal () ;

template <class UNIT1>
using HAS_COMPR = IS_VALUE<typeof (bad (TYPEAS<CREF<UNIT1>>::id).compr (bad (TYPEAS<CREF<UNIT1>>::id)))> ;

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
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_POINTER<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			if (FLAG (obj1) < FLAG (obj2))
				return NONE ;
			if (FLAG (obj2) < FLAG (obj1))
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			for (auto &&i : iter (0 ,ARRAY_SIZE<UNIT1>::value)) {
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
			return template_compr (PHX ,obj1 ,obj2) ;
		}

		template <class ARG1 = DEPEND ,class = ENABLE<HAS_COMPR<DEPENDENT<UNIT1 ,ARG1>>>>
		FLAG template_compr (CREF<typeof (PH2)> ,CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return obj1.compr (obj2) ;
		}

		template <class ARG1 = DEPEND>
		FLAG template_compr (CREF<typeof (PH1)> ,CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return ZERO ;
		}
	} ;
} ;

struct FUNCTION_operator_compr {
	template <class ARG1 ,class ARG2>
	inline FLAG operator() (XREF<ARG1> obj1 ,XREF<ARG2> obj2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename FUNCTION_operator_compr_HELP<R1X ,ALWAYS>::FUNCTION_operator_compr ;
		static constexpr auto M_INVOKE = R3X () ;
		return M_INVOKE (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

template <class...>
trait FUNCTION_hashcode_HELP ;

template <class DEPEND>
trait FUNCTION_hashcode_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL32<DEPEND>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-2128831035) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,DEPEND> ;
			const auto r1x = R1X (now) ^ R1X (inc) ;
			const auto r2x = R1X (FLAG (r1x) * FLAG (16777619)) ;
			const auto r3x = r2x & R1X (VAL_MAX) ;
			return FLAG (r3x) ;
		}
	} ;
} ;

template <class DEPEND>
trait FUNCTION_hashcode_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL64<DEPEND>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-3750763034362895579) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,DEPEND> ;
			const auto r1x = R1X (now) ^ R1X (inc) ;
			const auto r2x = R1X (FLAG (r1x) * FLAG (1099511628211)) ;
			const auto r3x = r2x & R1X (VAL_MAX) ;
			return FLAG (r3x) ;
		}
	} ;
} ;

struct FUNCTION_hashcode {
	inline FLAG operator() () const {
		using R1X = typename FUNCTION_hashcode_HELP<DEPEND ,ALWAYS>::FUNCTION_hashcode ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}

	inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
		using R1X = typename FUNCTION_hashcode_HELP<DEPEND ,ALWAYS>::FUNCTION_hashcode ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE (now ,inc) ;
	}
} ;

static constexpr auto hashcode = FUNCTION_hashcode () ;

template <class UNIT1>
using HAS_HASH = IS_VALUE<typeof (bad (TYPEAS<CREF<UNIT1>>::id).hash ())> ;

template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,UNIT1> ;
			const auto r1x = bitwise (obj) ;
			const auto r2x = R1X (r1x) & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_POINTER<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,UNIT1> ;
			const auto r1x = bitwise (FLAG (obj)) ;
			const auto r2x = R1X (r1x) & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,ARRAY_SIZE<UNIT1>::value)) {
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
			return template_hash (PHX ,obj) ;
		}

		template <class ARG1 = DEPEND ,class = ENABLE<HAS_HASH<DEPENDENT<UNIT1 ,ARG1>>>>
		FLAG template_hash (CREF<typeof (PH2)> ,CREF<UNIT1> obj) const {
			using R1X = BYTE_BASE<FLAG> ;
			const auto r1x = obj.hash () ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}

		template <class ARG1 = DEPEND>
		FLAG template_hash (CREF<typeof (PH1)> ,CREF<UNIT1> obj) const {
			return ZERO ;
		}
	} ;
} ;

struct FUNCTION_operator_hash {
	template <class ARG1>
	inline FLAG operator() (XREF<ARG1> obj) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename FUNCTION_operator_hash_HELP<R1X ,ALWAYS>::FUNCTION_operator_hash ;
		static constexpr auto M_INVOKE = R2X () ;
		return M_INVOKE (obj) ;
	}
} ;

static constexpr auto operator_hash = FUNCTION_operator_hash () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2 ,class ARG3>
	inline void operator() (XREF<ARG1> thiz_ ,XREF<ARG2> expect ,XREF<ARG3> obj) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		if ifnot (operator_equal (thiz_ ,expect))
			return ;
		thiz_ = R1X (forward[TYPEAS<ARG3>::id] (obj)) ;
	}
} ;

static constexpr auto replace = FUNCTION_replace () ;

template <class...>
trait IS_BOX_ACQUIRE_HELP ;

template <class UNIT1 ,class UNIT2>
trait IS_BOX_ACQUIRE_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,UNIT2>>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT1 ,class UNIT2>
trait IS_BOX_ACQUIRE_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_NOT<IS_SAME<UNIT1 ,UNIT2>>>> {
	using R1X = IS_EXTEND<Interface ,UNIT1> ;
	using R2X = IS_EXTEND<Interface ,UNIT2> ;
	using R3X = ENUM_COMPR_LTEQ<SIZE_OF<UNIT2> ,SIZE_OF<UNIT1>> ;
	using R4X = ENUM_COMPR_LTEQ<ALIGN_OF<UNIT2> ,ALIGN_OF<UNIT1>> ;
	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class UNIT1 ,class UNIT2>
using IS_BOX_ACQUIRE = typename IS_BOX_ACQUIRE_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait BOX_HELP ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	IS_TRIVIAL<UNIT1> ,
	IS_OBJECT<UNIT1>>>> {
	class Box final {
	protected:
		TEMP<UNIT1> mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
			return move (ret) ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept :Box () {
			swap (thiz ,that) ;
		}

		inline VREF<Box> operator= (RREF<Box> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			return TRUE ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT1> m_self () leftvalue {
			return fake ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return fake ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (XREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_BOX_ACQUIRE<UNIT1 ,R1X>) ;
			zeroize (mValue) ;
			new ((&mValue)) R1X (forward[TYPEAS<ARG2>::id] (obj)...) ;
			unsafe_barrier () ;
		}

		void release () {
			noop () ;
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
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	ENUM_NOT<IS_TRIVIAL<UNIT1>> ,
	IS_OBJECT<UNIT1>>>> {
	class Box final {
	protected:
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
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
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
			swap (thiz ,that) ;
		}

		inline VREF<Box> operator= (RREF<Box> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
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

		VREF<UNIT1> m_self () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (XREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_BOX_ACQUIRE<UNIT1 ,R1X>) ;
			assert (ifnot (mExist)) ;
			zeroize (mValue) ;
			new ((&mValue)) R1X (forward[TYPEAS<ARG2>::id] (obj)...) ;
			unsafe_barrier () ;
			mExist = TRUE ;
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
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	IS_TRIVIAL<UNIT1> ,
	IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mValue ;
	} ;

	class Box final {
	protected:
		TEMP<HEAP> mArray ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
			return move (ret) ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept :Box () {
			swap (thiz ,that) ;
		}

		inline VREF<Box> operator= (RREF<Box> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			return TRUE ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT1> m_self () leftvalue {
			return fake.mValue ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return fake.mValue ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (XREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<UNIT1 ,R1X>) ;
			zeroize (mArray) ;
			new ((&mArray)) HEAP (translation (forward[TYPEAS<ARG2>::id] (obj))...) ;
			unsafe_barrier () ;
		}

		void release () {
			noop () ;
		}

	private:
		VREF<HEAP> m_fake () leftvalue {
			return unsafe_deref (mArray) ;
		}

		CREF<HEAP> m_fake () const leftvalue {
			return unsafe_deref (mArray) ;
		}

		imports CREF<HEAP> translation (CREF<HEAP> obj) {
			return forward[TYPEAS<CREF<HEAP>>::id] (obj) ;
		}

		imports RREF<HEAP> translation (RREF<HEAP> obj) {
			return forward[TYPEAS<RREF<HEAP>>::id] (obj) ;
		}

		imports CREF<HEAP> translation (CREF<UNIT1> obj) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj))) ;
			return forward[TYPEAS<CREF<HEAP>>::id] (tmp) ;
		}

		imports RREF<HEAP> translation (RREF<UNIT1> obj) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj))) ;
			return forward[TYPEAS<RREF<HEAP>>::id] (tmp) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	ENUM_NOT<IS_TRIVIAL<UNIT1>> ,
	IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mValue ;
	} ;

	class Box final {
	protected:
		TEMP<HEAP> mArray ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}

		implicit Box (CREF<typeof (NULL)>) :Box () {
			noop () ;
		}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
			return move (ret) ;
		}

		implicit ~Box () noexcept {
			if ifnot (mExist)
				return ;
			unsafe_destroy (mArray) ;
			mExist = FALSE ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept :Box () {
			swap (thiz ,that) ;
		}

		inline VREF<Box> operator= (RREF<Box> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
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

		VREF<UNIT1> m_self () leftvalue {
			assert (exist ()) ;
			return fake.mValue ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			assert (exist ()) ;
			return fake.mValue ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (XREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<UNIT1 ,R1X>) ;
			assert (ifnot (mExist)) ;
			zeroize (mArray) ;
			new ((&mArray)) HEAP (translation (forward[TYPEAS<ARG2>::id] (obj))...) ;
			unsafe_barrier () ;
			mExist = TRUE ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<HEAP> m_fake () leftvalue {
			return unsafe_deref (mArray) ;
		}

		CREF<HEAP> m_fake () const leftvalue {
			return unsafe_deref (mArray) ;
		}

		imports CREF<HEAP> translation (CREF<HEAP> obj) {
			return forward[TYPEAS<CREF<HEAP>>::id] (obj) ;
		}

		imports RREF<HEAP> translation (RREF<HEAP> obj) {
			return forward[TYPEAS<RREF<HEAP>>::id] (obj) ;
		}

		imports CREF<HEAP> translation (CREF<UNIT1> obj) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj))) ;
			return forward[TYPEAS<CREF<HEAP>>::id] (tmp) ;
		}

		imports RREF<HEAP> translation (RREF<UNIT1> obj) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj))) ;
			return forward[TYPEAS<RREF<HEAP>>::id] (tmp) ;
		}
	} ;
} ;

template <class UNIT1>
using Box = typename BOX_HELP<UNIT1 ,ALWAYS>::Box ;

template <class...>
trait CELL_HELP ;

template <class UNIT1>
trait CELL_HELP<UNIT1 ,ALWAYS> {
	require (IS_CLONEABLE<UNIT1>) ;

	class Cell final {
	protected:
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
		imports Cell make (XREF<ARG1>...obj) {
			Cell ret ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (obj)...) ;
			ret.mValue = unsafe_deptr (rax.self) ;
			rax.release () ;
			ret.mExist = TRUE ;
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
			swap (thiz ,that) ;
		}

		inline VREF<Cell> operator= (RREF<Cell> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
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
			assert (mExist) ;
			return fake ;
		}

		void store (CREF<UNIT1> obj) const {
			assert (mExist) ;
			fake = obj ;
		}

		UNIT1 exchange (CREF<UNIT1> obj) const {
			assert (mExist) ;
			UNIT1 ret = fake ;
			fake = obj ;
			return move (ret) ;
		}

		void replace (CREF<UNIT1> expect ,CREF<UNIT1> obj) const {
			assert (mExist) ;
			if ifnot (operator_equal (fake ,expect))
				return ;
			fake = obj ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> obj) const {
			assert (mExist) ;
			if ifswitch (TRUE) {
				if ifnot (operator_equal (fake ,expect))
					discard ;
				fake = obj ;
				return TRUE ;
			}
			expect = fake ;
			return FALSE ;
		}

		UNIT1 fetch_add (CREF<UNIT1> obj) const {
			require (IS_SCALAR<UNIT1>) ;
			assert (mExist) ;
			UNIT1 ret = fake ;
			fake += obj ;
			return move (ret) ;
		}

		UNIT1 fetch_sub (CREF<UNIT1> obj) const {
			require (IS_SCALAR<UNIT1>) ;
			assert (mExist) ;
			UNIT1 ret = fake ;
			fake += obj ;
			return move (ret) ;
		}

		UNIT1 increase () const {
			return fetch_add (UNIT1 (1)) + UNIT1 (1) ;
		}

		UNIT1 decrease () const {
			return fetch_sub (UNIT1 (1)) + UNIT1 (1) ;
		}

	private:
		VREF<UNIT1> m_fake () const leftvalue {
			const auto r1x = address (mValue) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (r1x))) ;
		}
	} ;
} ;

template <class UNIT1>
using Cell = typename CELL_HELP<UNIT1 ,ALWAYS>::Cell ;

template <class...>
trait HEAPPROC_HELP ;

template <class...>
trait HEAPPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait HEAPPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual LENGTH align () const = 0 ;
		virtual LENGTH usage () const = 0 ;
		virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
		virtual void free (CREF<FLAG> addr_) const = 0 ;
	} ;

	using HEAPPROC_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using HEAPPROC_MAX_ALIGN = ALIGN_OF<DATA> ;

	class FakeHolder implement Holder {
	protected:
		Storage<HEAPPROC_MAX_SIZE ,HEAPPROC_MAX_ALIGN> mStorage ;

	public:
		LENGTH align () const override ;
		LENGTH usage () const override ;
		FLAG alloc (CREF<LENGTH> size_) const override ;
		void free (CREF<FLAG> addr_) const override ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class HeapProc {
	protected:
		Box<FakeHolder> mThis ;

	public:
		imports CREF<HeapProc> instance () {
			return memorize ([&] () {
				HeapProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				return move (ret) ;
			}) ;
		}

		LENGTH align () const {
			return mThis->align () ;
		}

		LENGTH usage () const {
			return mThis->usage () ;
		}

		FLAG alloc (CREF<LENGTH> size_) const {
			return mThis->alloc (size_) ;
		}

		void free (CREF<FLAG> addr_) const {
			return mThis->free (addr_) ;
		}
	} ;
} ;

using HeapProc = typename HEAPPROC_HELP<DEPEND ,ALWAYS>::HeapProc ;

template <class...>
trait VREF_HELP ;

template <class...>
trait CREF_HELP ;

template <class...>
trait VREF_HOLDER_HELP ;

template <class...>
trait VREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait VREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void destroy () = 0 ;
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void release () = 0 ;
		virtual FLAG addr () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual BOOL lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

	class VRef {
	protected:
		FLAG mHolder ;
		FLAG mPointer ;

	public:
		implicit VRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit VRef (CREF<VRef>) = delete ;

		inline VREF<VRef> operator= (CREF<VRef>) = delete ;

		implicit VRef (RREF<VRef> that) noexcept :VRef () {
			swap (thiz ,that) ;
		}

		inline VREF<VRef> operator= (RREF<VRef> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class UNIT1>
trait VREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	template <class ARG1 ,class ARG2>
	using MACRO_VRef = typename DEPENDENT<VREF_HELP<ARG1 ,ALWAYS> ,ARG2>::VRef ;

	template <class ARG1>
	using MACRO_CRef = typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef ;

	class VRef final {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		FLAG mHolder ;
		FLAG mPointer ;

	public:
		implicit VRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit VRef (CREF<typeof (NULL)>) :VRef () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<IS_REGISTER<ARG1> ,ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,VRef>>>>>
		implicit VRef (XREF<ARG1> that) : VRef (forward[TYPEAS<ARG1>::id] (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...obj) {
			using R1X = UNIT1 ;
			using R2X = typename VREF_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = CONDITIONAL<IS_ARRAY<R1X> ,LENGTH ,R1X> ;
			VRef ret ;
			if ifswitch (TRUE) {
				const auto r1x = R2X::dynamic_size (obj...) ;
				ret.mHolder = R2X::create (r1x) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				const auto r3x = ret.fake.lock () ;
				assert (r3x) ;
				auto rax = Box<R3X>::make (forward[TYPEAS<ARG1>::id] (obj)...) ;
				ret.fake.acquire (unsafe_cast (unsafe_deptr (rax.self))) ;
				rax.release () ;
				ret.mPointer = ret.fake.addr () ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class = ENABLE<IS_SAME<REMOVE_ALL<ARG1> ,UNIT1>>>
		imports VRef reference (XREF<ARG1> that) {
			require (IS_VARIABLE<ARG1>) ;
			VRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		implicit ~VRef () noexcept {
			if (mHolder == ZERO)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = fake.decrease () ;
				if (r1x > 0)
					discard ;
				if ifswitch (TRUE) {
					if (mPointer == ZERO)
						discard ;
					fake.release () ;
				}
				fake.destroy () ;
			}
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit VRef (CREF<VRef>) = delete ;

		inline VREF<VRef> operator= (CREF<VRef>) = delete ;

		implicit VRef (RREF<VRef> that) noexcept :VRef () {
			swap (thiz ,that) ;
		}

		inline VREF<VRef> operator= (RREF<VRef> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
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

		VREF<UNIT1> m_self () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		MACRO_VRef<REMOVE_ALL<ARG1> ,DEPEND> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			MACRO_VRef<R1X ,DEPEND> ret ;
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

		template <class ARG1 = DEPEND>
		MACRO_CRef<ARG1> unlock () {
			MACRO_CRef<ARG1> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if (mHolder == ZERO)
					discard ;
				fake.unlock () ;
				ret.mPointer = mPointer ;
				mPointer = ZERO ;
				ret.mHolder = mHolder ;
				mHolder = ZERO ;
			}
			return move (ret) ;
		}

	private:
		VREF<Holder> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::id] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
trait VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename VREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		imports FLAG create (CREF<LENGTH> size_) {
			assert (size_ > 0) ;
			const auto r1x = HeapProc::instance ().alloc (size_) ;
			const auto r2x = valign (r1x ,ALIGN_OF<ImplHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r2x))) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r1x ;
			tmp.mCounter = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		void destroy () override {
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
			HeapProc::instance ().free (r1x) ;
		}

		template <class...ARG1>
		imports LENGTH dynamic_size (XREF<ARG1>...obj) {
			const auto r1x = HeapProc::instance ().align () ;
			const auto r2x = vmax (ALIGN_OF<ImplHolder>::value - r1x ,ZERO) ;
			const auto r3x = r2x + SIZE_OF<ImplHolder>::value ;
			return r3x ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			mValue = unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (obj) ;
		}

		void release () override {
			unsafe_destroy (mValue) ;
		}

		FLAG addr () const override {
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
			mCounter-- ;
			return mCounter ;
		}

		BOOL lock () override {
			replace (mCounter ,IDEN ,NONE) ;
			return mCounter == NONE ;
		}

		void unlock () override {
			replace (mCounter ,NONE ,IDEN) ;
		}
	} ;
} ;

template <class UNIT1>
trait VREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_EQ_ZERO<SIZE>) ;
	using Holder = typename VREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class RealHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;
		TEMP<ITEM> mArray ;
	} ;

	class ImplHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;

	public:
		implicit ImplHolder () = default ;

		imports FLAG create (CREF<LENGTH> size_) {
			assert (size_ > 0) ;
			const auto r1x = HeapProc::instance ().alloc (size_) ;
			const auto r2x = valign (r1x ,ALIGN_OF<RealHolder>::value) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r2x))) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r1x ;
			tmp.mCounter = 0 ;
			tmp.mSize = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		void destroy () override {
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
			HeapProc::instance ().free (r1x) ;
		}

		template <class...ARG1>
		imports LENGTH dynamic_size (XREF<ARG1>...obj) {
			using R1X = TYPEAS<REMOVE_ALL<ARG1>...> ;
			require (ENUM_EQ_IDEN<COUNT_OF<R1X>>) ;
			using R2X = TYPE_UNWIND<R1X> ;
			require (IS_VALUE<R2X>) ;
			const auto r1x = LENGTH (obj...) ;
			assert (r1x > 0) ;
			const auto r2x = HeapProc::instance ().align () ;
			const auto r3x = vmax (ALIGN_OF<RealHolder>::value - r2x ,ZERO) ;
			const auto r4x = vmax (r1x - 1 ,ZERO) ;
			const auto r5x = r3x + SIZE_OF<RealHolder>::value + SIZE_OF<ITEM>::value * r4x ;
			return r5x ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			const auto r1x = unsafe_deref (unsafe_cast[TYPEAS<TEMP<LENGTH>>::id] (obj)) ;
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				mSize = r1x ;
			}
			auto &&tmp = fake ;
			while (TRUE) {
				if (mSize >= r1x)
					break ;
				INDEX ix = mSize ;
				auto rax = Box<ITEM>::make () ;
				tmp[ix] = unsafe_deptr (rax.self) ;
				rax.release () ;
				mSize++ ;
			}
		}

		void release () override {
			if ifswitch (TRUE) {
				if ifnot (IS_TRIVIAL<ITEM>::value)
					discard ;
				mSize = 0 ;
			}
			auto &&tmp = fake ;
			while (TRUE) {
				if (mSize <= 0)
					break ;
				INDEX ix = mSize - 1 ;
				unsafe_destroy (tmp[ix]) ;
				mSize-- ;
			}
		}

		FLAG addr () const override {
			return address (fake) ;
		}

		VREF<ARR<TEMP<ITEM>>> m_fake () const leftvalue {
			const auto r1x = address (thiz) + SIZE_OF<RealHolder>::value - SIZE_OF<ITEM>::value ;
			assert (r1x % ALIGN_OF<ITEM>::value == 0) ;
			return unsafe_cast[TYPEAS<ARR<TEMP<ITEM>>>::id] (unsafe_pointer (r1x)) ;
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
			mCounter-- ;
			return mCounter ;
		}

		BOOL lock () override {
			replace (mCounter ,IDEN ,NONE) ;
			return mCounter == NONE ;
		}

		void unlock () override {
			replace (mCounter ,NONE ,IDEN) ;
		}
	} ;
} ;

template <class UNIT1>
using VRef = typename VREF_HELP<UNIT1 ,ALWAYS>::VRef ;

template <class...>
trait CREF_HELP ;

template <class...>
trait CREF_IMPLHOLDER_HELP ;

template <class...>
trait CREF_PUREHOLDER_HELP ;

template <class UNIT1>
trait CREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	template <class ARG1>
	using MACRO_VRef = typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef ;

	template <class ARG1 ,class ARG2>
	using MACRO_CRef = typename DEPENDENT<CREF_HELP<ARG1 ,ALWAYS> ,ARG2>::CRef ;

	class CRef final {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		FLAG mHolder ;
		FLAG mPointer ;

	public:
		implicit CRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit CRef (CREF<typeof (NULL)>) :CRef () {
			noop () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<IS_REGISTER<ARG1> ,ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,CRef>>>>>
		implicit CRef (XREF<ARG1> that) : CRef (forward[TYPEAS<ARG1>::id] (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...obj) {
			using R1X = UNIT1 ;
			using R2X = typename CREF_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			using R3X = CONDITIONAL<IS_ARRAY<R1X> ,LENGTH ,R1X> ;
			CRef ret ;
			if ifswitch (TRUE) {
				const auto r1x = R2X::dynamic_size (obj...) ;
				ret.mHolder = R2X::create (r1x) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				auto rax = Box<R3X>::make (forward[TYPEAS<ARG1>::id] (obj)...) ;
				ret.fake.acquire (unsafe_cast (unsafe_deptr (rax.self))) ;
				rax.release () ;
				ret.mPointer = ret.fake.addr () ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class = ENABLE<IS_SAME<REMOVE_ALL<ARG1> ,UNIT1>>>
		imports CRef reference (XREF<ARG1> that) {
			require (ENUM_ANY<IS_VARIABLE<ARG1> ,IS_CONSTANT<ARG1>>) ;
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		template <class ARG1>
		imports CRef memorize (XREF<ARG1> proc) {
			using R1X = UNIT1 ;
			using R2X = typename CREF_PUREHOLDER_HELP<R1X ,ALWAYS>::PureHolder ;
			static auto mInstance = TEMP<R2X> () ;
			return CSC::memorize ([&] () {
				CRef ret ;
				if ifswitch (TRUE) {
					ret.mHolder = R2X::create (mInstance) ;
				}
				if ifswitch (TRUE) {
					const auto r1x = ret.fake.increase () ;
					assert (r1x == IDEN) ;
					auto rax = Box<R1X>::make (proc ()) ;
					ret.fake.acquire (unsafe_cast (unsafe_deptr (rax.self))) ;
					rax.release () ;
					ret.mPointer = ret.fake.addr () ;
				}
				return move (ret) ;
			}) ;
		}

		implicit ~CRef () noexcept {
			if (mHolder == ZERO)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = fake.decrease () ;
				if (r1x > 0)
					discard ;
				if ifswitch (TRUE) {
					if (mPointer == ZERO)
						discard ;
					fake.release () ;
				}
				fake.destroy () ;
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
				const auto r1x = that.fake.increase () ;
				if (r1x == NONE)
					discard ;
				assert (r1x >= 2) ;
				mHolder = that.mHolder ;
			}
			mPointer = that.mPointer ;
		}

		inline VREF<CRef> operator= (CREF<CRef> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit CRef (RREF<CRef> that) noexcept :CRef () {
			swap (thiz ,that) ;
		}

		inline VREF<CRef> operator= (RREF<CRef> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
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

		CREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		MACRO_CRef<REMOVE_ALL<ARG1> ,DEPEND> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			MACRO_CRef<R1X ,DEPEND> ret ;
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

		template <class ARG1 = DEPEND>
		MACRO_VRef<ARG1> lock () {
			MACRO_VRef<ARG1> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if (mHolder == ZERO)
					discard ;
				const auto r1x = fake.lock () ;
				if ifnot (r1x)
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
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::id] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
trait CREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::Holder ;
	using ImplHolder = typename VREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
} ;

template <class UNIT1>
trait CREF_PUREHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename CREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		TEMP<UNIT1> mValue ;

	public:
		implicit PureHolder () = default ;

		imports FLAG create (VREF<TEMP<PureHolder>> where_) {
			auto &&tmp = unsafe_deref (where_) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = address (where_) ;
			tmp.mCounter = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::id] (tmp)) ;
		}

		void destroy () override {
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			mValue = unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (obj) ;
		}

		void release () override {
			unsafe_destroy (mValue) ;
		}

		FLAG addr () const override {
			return address (mValue) ;
		}

		LENGTH increase () override {
			mCounter++ ;
			return mCounter ;
		}

		LENGTH decrease () override {
			mCounter-- ;
			return mCounter ;
		}

		BOOL lock () override {
			return FALSE ;
		}

		void unlock () override {
			noop () ;
		}
	} ;
} ;

template <class UNIT1>
using CRef = typename CREF_HELP<UNIT1 ,ALWAYS>::CRef ;

template <class...>
trait AUTO_HELP ;

template <class...>
trait AUTO_IMPLHOLDER_HELP ;

template <class DEPEND>
trait AUTO_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void release () = 0 ;
		virtual FLAG addr () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
	} ;

	using AUTO_MAX_SIZE = ENUMAS<VAL ,ENUMID<4032>> ;
	using AUTO_MAX_ALIGN = ALIGN_OF<DATA> ;

	class FakeHolder implement Holder {
	protected:
		Storage<AUTO_MAX_SIZE ,AUTO_MAX_ALIGN> mStorage ;

	public:
		void acquire (CREF<TEMP<void>> obj) override ;
		void release () override ;
		FLAG addr () const override ;
		FLAG type_cabi () const override ;
	} ;

	class Auto final {
	protected:
		Box<FakeHolder> mThis ;
		BOOL mGood ;

	public:
		implicit Auto () noexcept {
			mGood = FALSE ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Auto>>>>
		implicit Auto (XREF<ARG1> that) noexcept :Auto () {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_OBJECT<R1X>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R1X> ,SIZE_OF<FakeHolder>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<FakeHolder>>) ;
			using R2X = typename AUTO_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			mThis.acquire (TYPEAS<R2X>::id) ;
			if ifswitch (TRUE) {
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (that)) ;
				mThis->acquire (unsafe_cast (unsafe_deptr (rax.self))) ;
				rax.release () ;
				mGood = TRUE ;
			}
		}

		implicit ~Auto () noexcept {
			if ifnot (mThis == NULL)
				return ;
			if ifswitch (TRUE) {
				if ifnot (mGood)
					discard ;
				mThis->release () ;
			}
			mThis = NULL ;
			mGood = FALSE ;
		}

		implicit Auto (CREF<Auto>) = delete ;

		inline VREF<Auto> operator= (CREF<Auto>) = delete ;

		implicit Auto (RREF<Auto> that) noexcept :Auto () {
			swap (thiz ,that) ;
		}

		inline VREF<Auto> operator= (RREF<Auto> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			if ifnot (mGood)
				return FALSE ;
			return TRUE ;
		}

		template <class ARG1>
		REMOVE_ALL<ARG1> poll (XREF<ARG1> id) rightvalue noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			assert (exist ()) ;
			const auto r1x = mThis->type_cabi () ;
			const auto r2x = operator_cabi (id) ;
			assert (r1x == r2x) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R1X>>::id] (unsafe_pointer (mThis->addr ()))) ;
			R1X ret = move (tmp) ;
			mThis->release () ;
			mGood = FALSE ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait AUTO_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename AUTO_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mOrigin ;
		TEMP<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		void acquire (CREF<TEMP<void>> obj) override {
			mValue = unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (obj) ;
		}

		void release () override {
			unsafe_destroy (mValue) ;
		}

		FLAG addr () const override {
			return address (mValue) ;
		}

		FLAG type_cabi () const override {
			return operator_cabi (TYPEAS<UNIT1>::id) ;
		}
	} ;
} ;

using Auto = typename AUTO_HELP<DEPEND ,ALWAYS>::Auto ;

template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ITEM>
trait SLICE_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	struct Holder implement Interface {
		virtual LENGTH size () const = 0 ;
		virtual ITEM at (CREF<INDEX> index) const = 0 ;
	} ;

	using SLICE_MAX_SIZE = ENUMAS<VAL ,ENUMID<4096>> ;

	class Slice {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Slice>>>>
		explicit Slice (XREF<ARG1> id ,XREF<ARG2>...text) {
			require (ENUM_ALL<IS_ARRAY<REMOVE_ALL<ARG2>>...>) ;
			using R1X = ENUM_INC<ENUM_SUM<ENUM_DEC<ARRAY_SIZE<REMOVE_ALL<ARG2>>>...>> ;
			require (ENUM_COMPR_LTEQ<R1X ,SLICE_MAX_SIZE>) ;
			using R2X = typename SLICE_IMPLHOLDER_HELP<ITEM ,R1X ,ALWAYS>::ImplHolder ;
			//@warn: g++4.8 is too useless to compile
			static auto mInstance = R2X (text...) ;
			mThis = CRef<R2X>::memorize ([&] () {
				return move (mInstance) ;
			}) ;
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

		ITEM at (CREF<INDEX> index) const {
			assert (vbetween (index ,0 ,size ())) ;
			return mThis->at (index) ;
		}

		inline ITEM operator[] (CREF<INDEX> index) const {
			return at (index) ;
		}

		BOOL equal (CREF<Slice> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : iter (0 ,size ())) {
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
			const auto r1x = vmin (size () ,that.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
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
			for (auto &&i : iter (0 ,size ())) {
				const auto r1x = operator_hash (at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait SLICE_IMPLHOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	using Holder = typename SLICE_HELP<ITEM ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<ARR<ITEM ,SIZE>> mSlice ;
		LENGTH mSize ;

	public:
		implicit ImplHolder () = delete ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,ImplHolder>>>>
		explicit ImplHolder (XREF<ARG1> text1 ,XREF<ARG2>...text2) {
			mSize = 0 ;
			mSlice = Box<ARR<ITEM ,SIZE>>::make () ;
			template_write (0 ,text1 ,text2...) ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		ITEM at (CREF<INDEX> index) const override {
			return mSlice.self[index] ;
		}

	private:
		void template_write (CREF<INDEX> pos) {
			assert (pos < SIZE::value) ;
			mSize = pos ;
			mSlice.self[pos] = ITEM (0) ;
		}

		template <class ARG1 ,class...ARG2>
		void template_write (CREF<INDEX> pos ,XREF<ARG1> text1 ,XREF<ARG2>...text2) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = ARRAY_ITEM<R1X> ;
			require (ENUM_ANY<IS_SAME<R2X ,STRA> ,IS_SAME<R2X ,ITEM>>) ;
			using R3X = ENUM_DEC<ARRAY_SIZE<R1X>> ;
			assert (pos + R3X::value < SIZE::value) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= R3X::value)
					break ;
				const auto r1x = ITEM (text1[ix]) ;
				if (r1x == ITEM (0))
					break ;
				mSlice.self[pos + ix] = r1x ;
				ix++ ;
			}
			template_write (pos + ix ,text2...) ;
		}
	} ;
} ;

template <class ITEM>
using Slice = typename SLICE_HELP<ITEM ,ALWAYS>::Slice ;

template <class...>
trait FUNCTION_operator_name_HELP ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,BOOL>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("BOOL") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,VAL32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAL32") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,VAL64>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("VAL64") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,SINGLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("SINGLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,DOUBLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("DOUBLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,TRIPLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("TRIPLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRA") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRW>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRW") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU8>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU8") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU16>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU16") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("STRU32") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,BYTE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("BYTE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,WORD>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("WORD") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,CHAR>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("CHAR") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,DATA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			return slice ("DATA") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,HUGE>>> {
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
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_BASIC<UNIT1>>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() () const {
			using R1X = ENUMAS<VAL ,ENUMID<1024>> ;
			const auto r1x = internel_name () ;
			const auto r2x = r1x.mEnd - r1x.mBegin ;
			const auto r3x = invoke ([&] () {
				Box<ARR<STRA ,R1X>> ret = Box<ARR<STRA ,R1X>>::make () ;
				INDEX ix = 0 ;
				while (TRUE) {
					if (ix >= r2x)
						break ;
					ret.self[ix] = r1x.mBegin[ix] ;
					ix++ ;
				}
				while (TRUE) {
					if (ix >= R1X::value)
						break ;
					ret.self[ix] = 0 ;
					ix++ ;
				}
				return move (ret) ;
			}) ;
			return Slice<STR> (TYPEAS<struct anonymous>::id ,r3x.self) ;
		}

		csc_text_t internel_name () const {
			static constexpr auto M_INVOKE = FUNCTION_internel_name () ;
			return M_INVOKE (TYPEAS<UNIT1>::id) ;
		}
	} ;
} ;

struct FUNCTION_operator_name {
	template <class ARG1>
	inline Slice<STR> operator() (XREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename FUNCTION_operator_name_HELP<R1X ,ALWAYS>::FUNCTION_operator_name ;
		static constexpr auto M_INVOKE = R2X () ;
		return memorize ([&] () {
			return M_INVOKE () ;
		}) ;
	}
} ;

static constexpr auto operator_name = FUNCTION_operator_name () ;

template <class...>
trait CLAZZ_HELP ;

template <class...>
trait CLAZZ_IMPLHOLDER_HELP ;

template <class DEPEND>
trait CLAZZ_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual LENGTH type_size () const = 0 ;
		virtual LENGTH type_align () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual Slice<STR> type_name () const = 0 ;
	} ;

	class Clazz {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Clazz () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Clazz>>>>
		explicit Clazz (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_ANY<IS_POINTER<R1X> ,IS_SAME<R1X ,REMOVE_REF<R1X>>>) ;
			using R2X = typename CLAZZ_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			mThis = CRef<R2X>::memorize ([&] () {
				return R2X () ;
			}) ;
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
			const auto r1x = BOOL (mThis == NULL) ;
			const auto r2x = BOOL (that.mThis == NULL) ;
			if (r1x != r2x)
				return FALSE ;
			if (r1x)
				return TRUE ;
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
	using Holder = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Holder ;

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

using Clazz = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Clazz ;

template <class...>
trait WATCH_HELP ;

template <class UUID>
trait WATCH_HELP<UUID ,ALWAYS> {
	struct WATCH implement Interface {
		Slice<STR> mName ;
		FLAG mAddress ;
		Clazz mClazz ;
	} ;
} ;

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class ARG2 ,class = ENABLE<ENUM_ANY<IS_VARIABLE<ARG2> ,IS_CONSTANT<ARG2>>>>
	inline forceinline void operator() (XREF<ARG1> id ,CREF<Slice<STR>> name ,XREF<ARG2> expr) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		using R3X = typename WATCH_HELP<R1X ,ALWAYS>::WATCH ;
		static auto mInstance = R3X () ;
		mInstance.mName = name ;
		mInstance.mAddress = address (expr) ;
		mInstance.mClazz = Clazz (TYPEAS<R2X>::id) ;
	}
} ;

static constexpr auto unsafe_watch = FUNCTION_unsafe_watch () ;

template <class...>
trait EXCEPTION_HELP ;

template <class...>
trait EXCEPTION_HOLDER_HELP ;

template <class...>
trait EXCEPTION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait EXCEPTION_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual Slice<STR> what () const = 0 ;
	} ;

	class Exception {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Exception () = default ;

		implicit Exception (CREF<Exception> that) noexcept :Exception () {
			mThis = that.mThis ;
		}

		inline VREF<Exception> operator= (CREF<Exception> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait EXCEPTION_HELP<DEPEND ,ALWAYS> {
	using Holder = typename EXCEPTION_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename EXCEPTION_HOLDER_HELP<DEPEND ,ALWAYS>::Exception ;

	class Exception extend SUPER {
	protected:
		using SUPER::mThis ;

	public:
		implicit Exception () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Exception>>>>
		explicit Exception (XREF<ARG1> id ,CREF<Slice<STR>> what_) noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename EXCEPTION_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			mThis = CRef<R2X>::memorize ([&] () {
				return R2X (move (what_)) ;
			}) ;
		}

		Slice<STR> what () const noexcept {
			if (mThis == NULL)
				return Slice<STR>::zero () ;
			return mThis->what () ;
		}

		void raise () {
			throw thiz ;
		}
	} ;
} ;

template <class UUID>
trait EXCEPTION_IMPLHOLDER_HELP<UUID ,ALWAYS> {
	using Holder = typename EXCEPTION_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Slice<STR> mWhat ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Slice<STR>> what_) {
			mWhat = move (what_) ;
		}

		Slice<STR> what () const override {
			return mWhat ;
		}
	} ;
} ;

using Exception = typename EXCEPTION_HELP<DEPEND ,ALWAYS>::Exception ;

struct FUNCTION_try_invoke {
	template <class ARG1>
	inline void operator() (XREF<ARG1> proc1) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (XREF<ARG1> proc1 ,XREF<ARG2>...proc2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		try {
			return proc1 () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		} catch (...) {
			assume (FALSE) ;
		}
		return thiz (proc2...) ;
	}
} ;

static constexpr auto try_invoke = FUNCTION_try_invoke () ;

struct FUNCTION_unimplemented {
	inline forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

template <class...>
trait SCOPE_HELP ;

template <class UNIT1 ,class UNIT2>
trait SCOPE_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ANY<
	IS_VARIABLE<UNIT1> ,
	IS_CONSTANT<UNIT1>>>> {
	class Scope final {
	protected:
		FLAG mPointer ;

	public:
		implicit Scope () noexcept {
			mPointer = ZERO ;
		}

		explicit Scope (XREF<UNIT1> that) :Scope () {
			assert (mPointer == ZERO) ;
			that.enter () ;
			mPointer = address (that) ;
		}

		implicit ~Scope () noexcept {
			if (mPointer == ZERO)
				return ;
			try_invoke ([&] () {
				fake.leave () ;
			}) ;
			mPointer = ZERO ;
		}

		implicit Scope (CREF<Scope>) = delete ;

		inline VREF<Scope> operator= (CREF<Scope>) = delete ;

		implicit Scope (RREF<Scope> that) noexcept :Scope () {
			swap (thiz ,that) ;
		}

		inline VREF<Scope> operator= (RREF<Scope> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

	private:
		XREF<UNIT1> m_fake () const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT2>>::id] (unsafe_pointer (mPointer))) ;
			return forward[TYPEAS<UNIT1>::id] (tmp) ;
		}
	} ;
} ;

template <class UNIT1>
using Scope = typename SCOPE_HELP<XREF<UNIT1> ,REMOVE_REF<UNIT1> ,ALWAYS>::Scope ;
} ;