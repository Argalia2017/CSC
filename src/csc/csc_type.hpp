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

template <class MACRO>
trait VAL_HELP<MACRO ,REQUIRE<MACRO_CONFIG_VAL32<MACRO>>> {
	using VAL = VAL32 ;

	static constexpr auto VAL_MAX = VAL32_MAX ;
	static constexpr auto VAL_MIN = VAL32_MIN ;
	static constexpr auto VAL_ABS = VAL32_ABS ;
} ;

template <class MACRO>
trait VAL_HELP<MACRO ,REQUIRE<MACRO_CONFIG_VAL64<MACRO>>> {
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

inline constexpr BYTE operator| (CREF<BYTE> left ,CREF<BYTE> right) {
	return BYTE (csc_byte8_t (left) | csc_byte8_t (right)) ;
}

inline void operator|= (VREF<BYTE> left ,CREF<BYTE> right) {
	left = left | right ;
}

inline constexpr BYTE operator& (CREF<BYTE> left ,CREF<BYTE> right) {
	return BYTE (csc_byte8_t (left) & csc_byte8_t (right)) ;
}

inline void operator&= (VREF<BYTE> left ,CREF<BYTE> right) {
	left = left & right ;
}

inline constexpr BYTE operator^ (CREF<BYTE> left ,CREF<BYTE> right) {
	return BYTE (csc_byte8_t (left) ^ csc_byte8_t (right)) ;
}

inline void operator^= (VREF<BYTE> left ,CREF<BYTE> right) {
	left = left ^ right ;
}

inline constexpr BYTE operator~ (CREF<BYTE> left) {
	return BYTE (~csc_byte8_t (left)) ;
}

inline constexpr BYTE operator<< (CREF<BYTE> left ,CREF<LENGTH> scale) {
	return BYTE (csc_byte8_t (left) << scale) ;
}

inline constexpr BYTE operator>> (CREF<BYTE> left ,CREF<LENGTH> scale) {
	return BYTE (csc_byte8_t (left) >> scale) ;
}

inline constexpr WORD operator| (CREF<WORD> left ,CREF<WORD> right) {
	return WORD (csc_byte16_t (left) | csc_byte16_t (right)) ;
}

inline void operator|= (VREF<WORD> left ,CREF<WORD> right) {
	left = left | right ;
}

inline constexpr WORD operator& (CREF<WORD> left ,CREF<WORD> right) {
	return WORD (csc_byte16_t (left) & csc_byte16_t (right)) ;
}

inline void operator&= (VREF<WORD> left ,CREF<WORD> right) {
	left = left & right ;
}

inline constexpr WORD operator^ (CREF<WORD> left ,CREF<WORD> right) {
	return WORD (csc_byte16_t (left) ^ csc_byte16_t (right)) ;
}

inline constexpr WORD operator~ (CREF<WORD> left) {
	return WORD (~csc_byte16_t (left)) ;
}

inline constexpr WORD operator<< (CREF<WORD> left ,CREF<LENGTH> scale) {
	return WORD (csc_byte16_t (left) << scale) ;
}

inline constexpr WORD operator>> (CREF<WORD> left ,CREF<LENGTH> scale) {
	return WORD (csc_byte16_t (left) >> scale) ;
}

inline constexpr CHAR operator| (CREF<CHAR> left ,CREF<CHAR> right) {
	return CHAR (csc_byte32_t (left) | csc_byte32_t (right)) ;
}

inline void operator|= (VREF<CHAR> left ,CREF<CHAR> right) {
	left = left | right ;
}

inline constexpr CHAR operator& (CREF<CHAR> left ,CREF<CHAR> right) {
	return CHAR (csc_byte32_t (left) & csc_byte32_t (right)) ;
}

inline void operator&= (VREF<CHAR> left ,CREF<CHAR> right) {
	left = left & right ;
}

inline constexpr CHAR operator^ (CREF<CHAR> left ,CREF<CHAR> right) {
	return CHAR (csc_byte32_t (left) ^ csc_byte32_t (right)) ;
}

inline constexpr CHAR operator~ (CREF<CHAR> left) {
	return CHAR (~csc_byte32_t (left)) ;
}

inline constexpr CHAR operator<< (CREF<CHAR> left ,CREF<LENGTH> scale) {
	return CHAR (csc_byte32_t (left) << scale) ;
}

inline constexpr CHAR operator>> (CREF<CHAR> left ,CREF<LENGTH> scale) {
	return CHAR (csc_byte32_t (left) >> scale) ;
}

inline constexpr DATA operator| (CREF<DATA> left ,CREF<DATA> right) {
	return DATA (csc_byte64_t (left) | csc_byte64_t (right)) ;
}

inline void operator|= (VREF<DATA> left ,CREF<DATA> right) {
	left = left | right ;
}

inline constexpr DATA operator& (CREF<DATA> left ,CREF<DATA> right) {
	return DATA (csc_byte64_t (left) & csc_byte64_t (right)) ;
}

inline void operator&= (VREF<DATA> left ,CREF<DATA> right) {
	left = left & right ;
}

inline constexpr DATA operator^ (CREF<DATA> left ,CREF<DATA> right) {
	return DATA (csc_byte64_t (left) ^ csc_byte64_t (right)) ;
}

inline constexpr DATA operator~ (CREF<DATA> left) {
	return DATA (~csc_byte64_t (left)) ;
}

inline constexpr DATA operator<< (CREF<DATA> left ,CREF<LENGTH> scale) {
	return DATA (csc_byte64_t (left) << scale) ;
}

inline constexpr DATA operator>> (CREF<DATA> left ,CREF<LENGTH> scale) {
	return DATA (csc_byte64_t (left) >> scale) ;
}

using STRA = csc_char_t ;
using STRW = csc_wchar_t ;
using STRU8 = csc_char8_t ;
using STRU16 = csc_char16_t ;
using STRU32 = csc_char32_t ;

template <class...>
trait STR_HELP ;

template <class MACRO>
trait STR_HELP<MACRO ,REQUIRE<MACRO_CONFIG_STRA<MACRO>>> {
	using STR = STRA ;
} ;

template <class MACRO>
trait STR_HELP<MACRO ,REQUIRE<MACRO_CONFIG_STRW<MACRO>>> {
	using STR = STRW ;
} ;

using STR = typename STR_HELP<DEPEND ,ALWAYS>::STR ;

static constexpr auto NULL = nullptr ;

template <class...>
trait SIZE_OF_HELP ;

template <class UNIT1>
trait SIZE_OF_HELP<UNIT1 ,ALWAYS> {
	static constexpr auto value = sizeof (UNIT1) ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using SIZE_OF = typename SIZE_OF_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait ALIGN_OF_HELP ;

template <class UNIT1>
trait ALIGN_OF_HELP<UNIT1 ,ALWAYS> {
	static constexpr auto value = alignof (UNIT1) ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using ALIGN_OF = typename ALIGN_OF_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait COUNT_OF_HELP ;

template <class...UNIT1>
trait COUNT_OF_HELP<TYPEAS<UNIT1...> ,ALWAYS> {
	static constexpr auto value = sizeof... (UNIT1) ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using COUNT_OF = typename COUNT_OF_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class UNIT1 ,class = REQUIRE<IS_ENUM<UNIT1>>>
using ENUM_CHECK = UNIT1 ;

template <class UNIT1 ,class = REQUIRE<IS_TYPE<UNIT1>>>
using TYPE_CHECK = UNIT1 ;

template <class...>
trait ENUM_NOT_HELP ;

template <class UNIT1>
trait ENUM_NOT_HELP<UNIT1 ,ALWAYS> {
	static constexpr auto value = ifnot (ENUM_CHECK<UNIT1>::expr) ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using ENUM_NOT = typename ENUM_NOT_HELP<UNIT1 ,ALWAYS>::RET ;

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

template <class...>
trait ENUM_EQUAL_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_EQUAL_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = BOOL (ENUM_CHECK<UNIT1>::expr == ENUM_CHECK<UNIT2>::expr) ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_EQUAL = typename ENUM_EQUAL_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<UNIT1 ,UNIT2>> ;

template <class...>
trait ENUM_COMPR_LT_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_COMPR_LT_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = BOOL (ENUM_CHECK<UNIT1>::expr < ENUM_CHECK<UNIT2>::expr) ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LT = typename ENUM_COMPR_LT_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GT = ENUM_COMPR_LT<UNIT2 ,UNIT1> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_LTEQ = ENUM_NOT<ENUM_COMPR_GT<UNIT1 ,UNIT2>> ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR_GTEQ = ENUM_NOT<ENUM_COMPR_LT<UNIT1 ,UNIT2>> ;

template <class...>
trait ENUM_COMPR_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_COMPR_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	using R1X = ENUM_COMPR_LT<UNIT1 ,UNIT2> ;
	using R2X = ENUM_COMPR_GT<UNIT1 ,UNIT2> ;
	using R3X = CONDITIONAL<R1X ,ENUM_NONE ,CONDITIONAL<R2X ,ENUM_IDEN ,ENUM_ZERO>> ;

	static constexpr auto value = R3X::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_COMPR = typename ENUM_COMPR_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

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

template <class...>
trait ENUM_ADD_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_ADD_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = ENUM_CHECK<UNIT1>::expr + ENUM_CHECK<UNIT2>::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_ADD = typename ENUM_ADD_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUB_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_SUB_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = ENUM_CHECK<UNIT1>::expr - ENUM_CHECK<UNIT2>::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_SUB = typename ENUM_SUB_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait ENUM_MUL_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_MUL_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = ENUM_CHECK<UNIT1>::expr * ENUM_CHECK<UNIT2>::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_MUL = typename ENUM_MUL_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait ENUM_DIV_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_DIV_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = ENUM_CHECK<UNIT1>::expr / ENUM_CHECK<UNIT2>::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_DIV = typename ENUM_DIV_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait ENUM_MOD_HELP ;

template <class UNIT1 ,class UNIT2>
trait ENUM_MOD_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	static constexpr auto value = ENUM_CHECK<UNIT1>::expr % ENUM_CHECK<UNIT2>::expr ;
	using RET = ENUMAS<VAL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using ENUM_MOD = typename ENUM_MOD_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

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
	//@fatal: fuck clang5.0
	implicit Interface (Interface &&) = default ;
	inline VREF<Interface> operator= (Interface &&) = default ;
} ;

template <class...>
trait PLACEHOLDER_HELP ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	struct PlaceHolder {} ;
} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using SUPER = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::PlaceHolder ;

	struct PlaceHolder implement SUPER {} ;
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

template <class...>
trait IS_VALUE_HELP ;

template <class UNIT1>
trait IS_VALUE_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_SAME<UNIT1 ,VAL32> ;
	using R2X = IS_SAME<UNIT1 ,VAL64> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_VALUE = typename IS_VALUE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_FLOAT_HELP ;

template <class UNIT1>
trait IS_FLOAT_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_SAME<UNIT1 ,SINGLE> ;
	using R2X = IS_SAME<UNIT1 ,DOUBLE> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_FLOAT = typename IS_FLOAT_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_TEXT_HELP ;

template <class UNIT1>
trait IS_TEXT_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_SAME<UNIT1 ,STRA> ;
	using R2X = IS_SAME<UNIT1 ,STRW> ;
	using R3X = IS_SAME<UNIT1 ,STRU8> ;
	using R4X = IS_SAME<UNIT1 ,STRU16> ;
	using R5X = IS_SAME<UNIT1 ,STRU32> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_TEXT = typename IS_TEXT_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_BIT_HELP ;

template <class UNIT1>
trait IS_BIT_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_SAME<UNIT1 ,BYTE> ;
	using R2X = IS_SAME<UNIT1 ,WORD> ;
	using R3X = IS_SAME<UNIT1 ,CHAR> ;
	using R4X = IS_SAME<UNIT1 ,DATA> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X ,R3X ,R4X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_BIT = typename IS_BIT_HELP<UNIT1 ,ALWAYS>::RET ;

template <class UNIT1>
using IS_NULL = IS_SAME<UNIT1 ,DEF<typeof (NULL)>> ;

template <class UNIT1>
using IS_VOID = IS_SAME<UNIT1 ,void> ;

template <class...>
trait IS_SCALAR_HELP ;

template <class UNIT1>
trait IS_SCALAR_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_VALUE<UNIT1> ;
	using R2X = IS_FLOAT<UNIT1> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_SCALAR = typename IS_SCALAR_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_BASIC_HELP ;

template <class UNIT1>
trait IS_BASIC_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_BOOL<UNIT1> ;
	using R2X = IS_VALUE<UNIT1> ;
	using R3X = IS_FLOAT<UNIT1> ;
	using R4X = IS_TEXT<UNIT1> ;
	using R5X = IS_BIT<UNIT1> ;
	using R6X = IS_NULL<UNIT1> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_BASIC = typename IS_BASIC_HELP<UNIT1 ,ALWAYS>::RET ;

using VARIABLE = ENUMAS<VAL ,ENUMID<(-1)>> ;
using CONSTANT = ENUMAS<VAL ,ENUMID<(-2)>> ;
using REGISTER = ENUMAS<VAL ,ENUMID<(-3)>> ;
using DYNAMICS = ENUMAS<VAL ,ENUMID<(-4)>> ;

template <class...>
trait REFLECT_REF_HELP ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VREF<UNIT2>>>> {
	using RET = VARIABLE ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,CREF<UNIT2>>>> {
	using RET = CONSTANT ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_REF_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,RREF<UNIT2>>>> {
	using RET = REGISTER ;
} ;

template <class UNIT1>
using REFLECT_REF = typename REFLECT_REF_HELP<XREF<UNIT1> ,REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

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
trait REFLECT_POINTER_HELP<DEF<const UNIT1 *> ,ALWAYS> {
	using RET = TYPEAS<UNIT1> ;
} ;

template <class UNIT1>
using REFLECT_POINTER = typename REFLECT_POINTER_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

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
using IS_ARRAY = ENUM_NOT<IS_SAME<REFLECT_ARRAY<UNIT1> ,TYPEAS<>>> ;

template <class UNIT1>
using ARRAY_ITEM = TYPE_FIRST_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class UNIT1>
using ARRAY_SIZE = TYPE_SECOND_ONE<REFLECT_ARRAY<UNIT1>> ;

template <class...>
trait ARR_HELP ;

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;
	using RET = DEF<ITEM[]> ;
} ;

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;

	static constexpr auto value = VAL (SIZE::expr) ;
	using RET = DEF<ITEM[value]> ;
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
using IS_FUNCTION = ENUM_NOT<IS_SAME<REFLECT_FUNCTION<UNIT1> ,TYPEAS<>>> ;

template <class UNIT1>
using FUNCTION_RETURN = TYPE_FIRST_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class UNIT1>
using FUNCTION_PARAMS = TYPE_SECOND_ONE<REFLECT_FUNCTION<UNIT1>> ;

template <class...>
trait IS_INTPTR_HELP ;

template <class UNIT1>
trait IS_INTPTR_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_INTCLASS<UNIT1> ;
	using R2X = ENUM_NOT<IS_SAME<REFLECT_POINTER<UNIT1> ,TYPEAS<>>> ;
	using R3X = IS_BASIC<UNIT1> ;

	static constexpr auto value = ENUM_ALL<ENUM_ANY<R1X ,R2X> ,ENUM_NOT<ENUM_ANY<R3X>>>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_INTPTR = typename IS_INTPTR_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_CLASS_HELP ;

template <class UNIT1>
trait IS_CLASS_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_CLASS<UNIT1> ;
	using R3X = IS_ENUM<UNIT1> ;
	using R4X = IS_TYPE<UNIT1> ;
	using R5X = IS_BASIC<UNIT1> ;
	using R6X = IS_INTPTR<UNIT1> ;

	static constexpr auto value = ENUM_ALL<R1X ,ENUM_NOT<ENUM_ANY<R3X ,R4X ,R5X ,R6X>>>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
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

	static constexpr auto value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
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

	static constexpr auto value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_CLONEABLE = typename IS_CLONEABLE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_TRIVIAL_HELP ;

template <class UNIT1>
trait IS_TRIVIAL_HELP<UNIT1 ,ALWAYS> {
	using R1X = MACRO_IS_TRIVIAL_CONSTRUCTIBLE<UNIT1> ;
	using R2X = MACRO_IS_TRIVIAL_DESTRUCTIBLE<UNIT1> ;

	static constexpr auto value = ENUM_ALL<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_TRIVIAL = typename IS_TRIVIAL_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_INTERFACE_HELP ;

template <class UNIT1>
trait IS_INTERFACE_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	using R1X = MACRO_IS_INTERFACE<UNIT1> ;
	using R2X = MACRO_IS_EXTEND<Interface ,UNIT1> ;

	static constexpr auto value = ENUM_ALL<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_INTERFACE = typename IS_INTERFACE_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_EXTEND_HELP ;

template <class FROM ,class TO>
trait IS_EXTEND_HELP<FROM ,TO ,ALWAYS> {
	using R1X = IS_SAME<FROM ,TO> ;
	using R2X = MACRO_IS_EXTEND<FROM ,TO> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class FROM ,class TO>
using IS_EXTEND = typename IS_EXTEND_HELP<FROM ,TO ,ALWAYS>::RET ;

template <class UNIT1>
using IS_PLACEHOLDER = IS_EXTEND<UNIT1 ,DEF<typeof (PH0)>> ;

template <class...>
trait TOGETHER_HELP ;

template <class PARAMS>
trait TOGETHER_HELP<PARAMS ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<PARAMS>>>> {
	using R1X = TYPE_FIRST_ONE<PARAMS> ;

	struct Together implement R1X {} ;
} ;

template <class PARAMS>
trait TOGETHER_HELP<PARAMS ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<PARAMS>>>> {
	using R1X = TYPE_FIRST_ONE<PARAMS> ;
	using R2X = TYPE_FIRST_REST<PARAMS> ;
	using SUPER = typename TOGETHER_HELP<R2X ,ALWAYS>::Together ;

	struct Together implement R1X ,SUPER {} ;
} ;

template <class...PARAMS>
using Together = typename TOGETHER_HELP<TYPEAS<PARAMS...> ,ALWAYS>::Together ;

template <class...>
trait IS_CONVERTIBLE_HELP ;

template <class FROM ,class TO>
trait IS_CONVERTIBLE_HELP<FROM ,TO ,ALWAYS> {
	using R1X = MACRO_IS_CONVERTIBLE<FROM ,TO> ;

	static constexpr auto value = ENUM_ANY<R1X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class FROM ,class TO>
using IS_CONVERTIBLE = typename IS_CONVERTIBLE_HELP<XREF<FROM> ,XREF<TO> ,ALWAYS>::RET ;

template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...UNIT1 ,class UNIT2>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<UNIT1...> ,UNIT2 ,ALWAYS> {
	using R1X = UNIT2 ;
	using R2X = DEF<RETURN (*) (UNIT1...)> ;
	using R3X = IS_CONVERTIBLE<CREF<R1X> ,RREF<R2X>> ;
	using RET = ENUM_NOT<R3X> ;
} ;

template <class UNIT1>
using IS_EFFECTIVE = typename IS_EFFECTIVE_HELP<FUNCTION_RETURN<UNIT1> ,FUNCTION_PARAMS<UNIT1> ,UNIT1 ,ALWAYS>::RET ;

template <class...>
trait IS_OBJECT_HELP ;

template <class UNIT1>
trait IS_OBJECT_HELP<UNIT1 ,ALWAYS> {
	using R1X = IS_BASIC<UNIT1> ;
	using R2X = IS_INTPTR<UNIT1> ;
	using R3X = IS_CLASS<UNIT1> ;

	static constexpr auto value = ENUM_ANY<R1X ,R2X ,R3X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1>
using IS_OBJECT = typename IS_OBJECT_HELP<UNIT1 ,ALWAYS>::RET ;

template <class...>
trait BYTE_BASE_HELP ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<BYTE>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<BYTE>>>>> {
	using RET = BYTE ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<WORD>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<WORD>>>>> {
	using RET = WORD ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<CHAR>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<CHAR>>>>> {
	using RET = CHAR ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<DATA>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<DATA>>>>> {
	using RET = DATA ;
} ;

template <class UNIT1>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<UNIT1> ,ALIGN_OF<UNIT1> ,ALWAYS>::RET ;

template <class...>
trait TEXT_BASE_HELP ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<STRU8>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU8>>>>> {
	using RET = STRU8 ;
} ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<STRU16>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU16>>>>> {
	using RET = STRU16 ;
} ;

template <class SIZE ,class ALIGN>
trait TEXT_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<ENUM_EQUAL<SIZE ,SIZE_OF<STRU32>> ,ENUM_EQUAL<ALIGN ,ALIGN_OF<STRU32>>>>> {
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

template <class SIZE ,class ALIGN = ENUM_IDEN>
using Storage = typename STORAGE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

template <class...>
trait REFLECT_TEMP_HELP ;

template <class UNIT1>
trait REFLECT_TEMP_HELP<UNIT1 ,ALWAYS> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT1 ,class UNIT2>
trait REFLECT_TEMP_HELP<TEMPAS<UNIT1 ,UNIT2> ,ALWAYS> {
	using RET = TYPEAS<UNIT1 ,UNIT2> ;
} ;

template <class UNIT1>
using REFLECT_TEMP = typename REFLECT_TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
using IS_TEMP = ENUM_NOT<IS_SAME<REFLECT_TEMP<UNIT1> ,TYPEAS<>>> ;

template <class...>
trait TEMP_HELP ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using RET = TEMPAS<void ,void> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_TEMP<UNIT1>>> {
	using R1X = TYPE_SECOND_ONE<REFLECT_TEMP<UNIT1>> ;
	using RET = TEMPAS<UNIT1 ,R1X> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	using R1X = ARRAY_ITEM<UNIT1> ;
	using R2X = ENUM_EQ_ZERO<ARRAY_SIZE<UNIT1>> ;
	using R3X = CONDITIONAL<R2X ,R1X ,UNIT1> ;
	using R4X = Storage<SIZE_OF<R3X> ,ALIGN_OF<R3X>> ;
	using R5X = CONDITIONAL<R2X ,void ,R4X> ;
	using RET = TEMPAS<UNIT1 ,R5X> ;
} ;

template <class UNIT1>
trait TEMP_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_OBJECT<UNIT1> ,ENUM_NOT<IS_TEMP<UNIT1>>>>> {
	using R1X = Storage<SIZE_OF<UNIT1> ,ALIGN_OF<UNIT1>> ;
	using RET = TEMPAS<UNIT1 ,R1X> ;
} ;

template <class UNIT1>
using TEMP = typename TEMP_HELP<REMOVE_REF<UNIT1> ,ALWAYS>::RET ;

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

template <class...>
trait ENUM_BETWEEN_HELP ;

template <class CURR ,class BEGIN ,class END>
trait ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS> {
	using R1X = ENUM_COMPR_GTEQ<CURR ,BEGIN> ;
	using R2X = ENUM_COMPR_LT<CURR ,END> ;

	static constexpr auto value = ENUM_ALL<R1X ,R2X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class CURR ,class BEGIN ,class END>
using ENUM_BETWEEN = typename ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS>::RET ;

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
} ;