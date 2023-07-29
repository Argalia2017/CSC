#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

forceinline constexpr BYTE operator| (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_byte8_t (obj1) | csc_byte8_t (obj2)) ;
}

forceinline void operator|= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

forceinline constexpr BYTE operator& (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_byte8_t (obj1) & csc_byte8_t (obj2)) ;
}

forceinline void operator&= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

forceinline constexpr BYTE operator^ (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_byte8_t (obj1) ^ csc_byte8_t (obj2)) ;
}

forceinline void operator^= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 ^ obj2 ;
}

forceinline constexpr BYTE operator~ (CREF<BYTE> obj1) noexcept {
	return BYTE (~csc_byte8_t (obj1)) ;
}

forceinline constexpr BYTE operator<< (CREF<BYTE> obj1 ,CREF<LENGTH> obj2) noexcept {
	return BYTE (csc_byte8_t (obj1) << obj2) ;
}

forceinline constexpr BYTE operator>> (CREF<BYTE> obj1 ,CREF<LENGTH> obj2) noexcept {
	return BYTE (csc_byte8_t (obj1) >> obj2) ;
}

forceinline constexpr WORD operator| (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_byte16_t (obj1) | csc_byte16_t (obj2)) ;
}

forceinline void operator|= (VREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

forceinline constexpr WORD operator& (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_byte16_t (obj1) & csc_byte16_t (obj2)) ;
}

forceinline void operator&= (VREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

forceinline constexpr WORD operator^ (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_byte16_t (obj1) ^ csc_byte16_t (obj2)) ;
}

forceinline constexpr WORD operator~ (CREF<WORD> obj1) noexcept {
	return WORD (~csc_byte16_t (obj1)) ;
}

forceinline constexpr WORD operator<< (CREF<WORD> obj1 ,CREF<LENGTH> obj2) noexcept {
	return WORD (csc_byte16_t (obj1) << obj2) ;
}

forceinline constexpr WORD operator>> (CREF<WORD> obj1 ,CREF<LENGTH> obj2) noexcept {
	return WORD (csc_byte16_t (obj1) >> obj2) ;
}

forceinline constexpr CHAR operator| (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_byte32_t (obj1) | csc_byte32_t (obj2)) ;
}

forceinline void operator|= (VREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

forceinline constexpr CHAR operator& (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_byte32_t (obj1) & csc_byte32_t (obj2)) ;
}

forceinline void operator&= (VREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

forceinline constexpr CHAR operator^ (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_byte32_t (obj1) ^ csc_byte32_t (obj2)) ;
}

forceinline constexpr CHAR operator~ (CREF<CHAR> obj1) noexcept {
	return CHAR (~csc_byte32_t (obj1)) ;
}

forceinline constexpr CHAR operator<< (CREF<CHAR> obj1 ,CREF<LENGTH> obj2) noexcept {
	return CHAR (csc_byte32_t (obj1) << obj2) ;
}

forceinline constexpr CHAR operator>> (CREF<CHAR> obj1 ,CREF<LENGTH> obj2) noexcept {
	return CHAR (csc_byte32_t (obj1) >> obj2) ;
}

forceinline constexpr DATA operator| (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_byte64_t (obj1) | csc_byte64_t (obj2)) ;
}

forceinline void operator|= (VREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

forceinline constexpr DATA operator& (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_byte64_t (obj1) & csc_byte64_t (obj2)) ;
}

forceinline void operator&= (VREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

forceinline constexpr DATA operator^ (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_byte64_t (obj1) ^ csc_byte64_t (obj2)) ;
}

forceinline constexpr DATA operator~ (CREF<DATA> obj1) noexcept {
	return DATA (~csc_byte64_t (obj1)) ;
}

forceinline constexpr DATA operator<< (CREF<DATA> obj1 ,CREF<LENGTH> obj2) noexcept {
	return DATA (csc_byte64_t (obj1) << obj2) ;
}

forceinline constexpr DATA operator>> (CREF<DATA> obj1 ,CREF<LENGTH> obj2) noexcept {
	return DATA (csc_byte64_t (obj1) >> obj2) ;
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

template <class UNIT>
trait SIZE_OF_HELP<UNIT ,ALWAYS> {
	enum { value = sizeof (UNIT) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT>
using SIZE_OF = typename SIZE_OF_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait ALIGN_OF_HELP ;

template <class UNIT>
trait ALIGN_OF_HELP<UNIT ,ALWAYS> {
	enum { value = alignof (UNIT) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT>
using ALIGN_OF = typename ALIGN_OF_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait COUNT_OF_HELP ;

template <class...UNIT>
trait COUNT_OF_HELP<TYPEAS<UNIT...> ,ALWAYS> {
	enum { value = sizeof... (UNIT) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT>
using COUNT_OF = typename COUNT_OF_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait REFLECT_ENUM_HELP ;

template <class UNIT>
trait REFLECT_ENUM_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT ,csc_diff_t SIDE>
trait REFLECT_ENUM_HELP<ENUMAS<UNIT ,SIDE>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_ENUM = typename REFLECT_ENUM_HELP<UNIT>::RET ;

template <class...>
trait REFLECT_TYPE_HELP ;

template <class UNIT>
trait REFLECT_TYPE_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class FIRST>
trait REFLECT_TYPE_HELP<TYPEAS<FIRST>> {
	using FIRST_ONE = FIRST ;
	using FIRST_REST = TYPEAS<> ;
	using RET = ENUM_TRUE ;
} ;

template <class FIRST ,class SECOND>
trait REFLECT_TYPE_HELP<TYPEAS<FIRST ,SECOND>> {
	using FIRST_ONE = FIRST ;
	using FIRST_REST = TYPEAS<SECOND> ;
	using SECOND_ONE = SECOND ;
	using SECOND_REST = TYPEAS<> ;
	using RET = ENUM_TRUE ;
} ;

template <class FIRST ,class SECOND ,class THIRD ,class...REST>
trait REFLECT_TYPE_HELP<TYPEAS<FIRST ,SECOND ,THIRD ,REST...>> {
	using FIRST_ONE = FIRST ;
	using FIRST_REST = TYPEAS<SECOND ,THIRD ,REST...> ;
	using SECOND_ONE = SECOND ;
	using SECOND_REST = TYPEAS<THIRD ,REST...> ;
	using THIRD_ONE = THIRD ;
	using THIRD_REST = TYPEAS<REST...> ;
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_TYPE = typename REFLECT_TYPE_HELP<UNIT>::RET ;

template <class UNIT>
using TYPE_FIRST_ONE = typename REFLECT_TYPE_HELP<UNIT>::FIRST_ONE ;

template <class UNIT>
using TYPE_FIRST_REST = typename REFLECT_TYPE_HELP<UNIT>::FIRST_REST ;

template <class UNIT>
using TYPE_SECOND_ONE = typename REFLECT_TYPE_HELP<UNIT>::SECOND_ONE ;

template <class UNIT>
using TYPE_SECOND_REST = typename REFLECT_TYPE_HELP<UNIT>::SECOND_REST ;

template <class UNIT>
using TYPE_THIRD_ONE = typename REFLECT_TYPE_HELP<UNIT>::THIRD_ONE ;

template <class UNIT>
using TYPE_THIRD_REST = typename REFLECT_TYPE_HELP<UNIT>::THIRD_REST ;

using ENUM_ZERO = ENUMAS<VAL ,ZERO> ;
using ENUM_IDEN = ENUMAS<VAL ,IDEN> ;
using ENUM_NONE = ENUMAS<VAL ,NONE> ;
using ENUM_USED = ENUMAS<VAL ,USED> ;

template <class UNIT ,class = REQUIRE<IS_ENUM<UNIT>>>
using ENUM_CHECK = UNIT ;

template <class UNIT ,class = REQUIRE<IS_TYPE<UNIT>>>
using TYPE_CHECK = UNIT ;

template <class...>
trait ENUM_NOT_HELP ;

template <class UNIT>
trait ENUM_NOT_HELP<UNIT ,ALWAYS> {
	enum { value = ifnot (ENUM_CHECK<UNIT>::expr) } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using ENUM_NOT = typename ENUM_NOT_HELP<UNIT ,ALWAYS>::RET ;

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

template <class UNIT ,class SIDE>
trait ENUM_EQUAL_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr == ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_EQUAL = typename ENUM_EQUAL_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class UNIT ,class SIDE>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<UNIT ,SIDE>> ;

template <class...>
trait ENUM_COMPR_LT_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_COMPR_LT_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr < ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_COMPR_LT = typename ENUM_COMPR_LT_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class UNIT ,class SIDE>
using ENUM_COMPR_GT = ENUM_COMPR_LT<SIDE ,UNIT> ;

template <class UNIT ,class SIDE>
using ENUM_COMPR_LTEQ = ENUM_NOT<ENUM_COMPR_GT<UNIT ,SIDE>> ;

template <class UNIT ,class SIDE>
using ENUM_COMPR_GTEQ = ENUM_NOT<ENUM_COMPR_LT<UNIT ,SIDE>> ;

template <class...>
trait ENUM_COMPR_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_COMPR_HELP<UNIT ,SIDE ,ALWAYS> {
	using R1X = ENUM_COMPR_LT<UNIT ,SIDE> ;
	using R2X = ENUM_COMPR_GT<UNIT ,SIDE> ;
	using R3X = CONDITIONAL<R1X ,ENUM_NONE ,CONDITIONAL<R2X ,ENUM_IDEN ,ENUM_ZERO>> ;

	enum { value = R3X::expr } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_COMPR = typename ENUM_COMPR_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class UNIT>
using ENUM_EQ_ZERO = ENUM_EQUAL<UNIT ,ENUM_ZERO> ;

template <class UNIT>
using ENUM_GT_ZERO = ENUM_COMPR_GT<UNIT ,ENUM_ZERO> ;

template <class UNIT>
using ENUM_EQ_IDEN = ENUM_EQUAL<UNIT ,ENUM_IDEN> ;

template <class UNIT>
using ENUM_GT_IDEN = ENUM_COMPR_GT<UNIT ,ENUM_IDEN> ;

template <class...>
trait ENUM_ADD_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_ADD_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr + ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_ADD = typename ENUM_ADD_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUB_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_SUB_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr - ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_SUB = typename ENUM_SUB_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class...>
trait ENUM_MUL_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_MUL_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr * ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_MUL = typename ENUM_MUL_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class...>
trait ENUM_DIV_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_DIV_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr / ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_DIV = typename ENUM_DIV_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class...>
trait ENUM_MOD_HELP ;

template <class UNIT ,class SIDE>
trait ENUM_MOD_HELP<UNIT ,SIDE ,ALWAYS> {
	enum { value = (ENUM_CHECK<UNIT>::expr % ENUM_CHECK<SIDE>::expr) } ;
	using RET = ENUMAS<VAL ,value> ;
} ;

template <class UNIT ,class SIDE>
using ENUM_MOD = typename ENUM_MOD_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class UNIT>
using ENUM_MINUS = ENUM_SUB<ENUM_ZERO ,UNIT> ;

template <class UNIT>
using ENUM_INC = ENUM_ADD<UNIT ,ENUM_IDEN> ;

template <class UNIT>
using ENUM_DEC = ENUM_SUB<UNIT ,ENUM_IDEN> ;

template <class...>
trait TYPE_CAT_HELP ;

template <class...UNIT ,class...SIDE>
trait TYPE_CAT_HELP<TYPEAS<UNIT...> ,TYPEAS<SIDE...> ,ALWAYS> {
	using RET = TYPEAS<UNIT... ,SIDE...> ;
} ;

template <class UNIT ,class SIDE>
using TYPE_CAT = typename TYPE_CAT_HELP<UNIT ,SIDE ,ALWAYS>::RET ;

template <class...>
trait TYPE_REVERSE_HELP ;

template <class UNIT>
trait TYPE_REVERSE_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
	using RET = TYPEAS<> ;
} ;

template <class UNIT>
trait TYPE_REVERSE_HELP<UNIT ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using RET = TYPEAS<R1X> ;
} ;

template <class UNIT>
trait TYPE_REVERSE_HELP<UNIT ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT>>>> {
	using R1X = TYPEAS<TYPE_FIRST_ONE<UNIT>> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R2X ,R1X> ;
} ;

template <class UNIT>
using TYPE_REVERSE = typename TYPE_REVERSE_HELP<UNIT ,ALWAYS>::RET ;

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

template <class SIZE ,class CURR>
trait TYPE_SENQUENCE_HELP<SIZE ,CURR ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = TYPEAS<> ;
} ;

template <class SIZE ,class CURR>
trait TYPE_SENQUENCE_HELP<SIZE ,CURR ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using R1X = typename TYPE_REPEAT_HELP<ENUM_DEC<SIZE> ,ENUM_INC<CURR> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<TYPEAS<CURR> ,R1X> ;
} ;

template <class SIZE>
using TYPE_SENQUENCE = typename TYPE_SENQUENCE_HELP<SIZE ,ENUM_ZERO ,ALWAYS>::RET ;

template <class...>
trait TYPE_PICK_HELP ;

template <class UNIT ,class CURR>
trait TYPE_PICK_HELP<UNIT ,CURR ,REQUIRE<ENUM_EQ_ZERO<CURR>>> {
	using RET = TYPE_FIRST_ONE<UNIT> ;
} ;

template <class UNIT ,class CURR>
trait TYPE_PICK_HELP<UNIT ,CURR ,REQUIRE<ENUM_GT_ZERO<CURR>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_FIRST_REST<UNIT> ,ENUM_DEC<CURR> ,ALWAYS>::RET ;
} ;

template <class UNIT ,class CURR>
using TYPE_PICK = typename TYPE_PICK_HELP<UNIT ,CURR ,ALWAYS>::RET ;

template <class...>
trait ENUM_ALL_HELP ;

template <class UNIT>
trait ENUM_ALL_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
trait ENUM_ALL_HELP<UNIT ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,R2X ,ENUM_FALSE> ;
} ;

template <class...UNIT>
using ENUM_ALL = typename ENUM_ALL_HELP<TYPEAS<UNIT...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_ANY_HELP ;

template <class UNIT>
trait ENUM_ANY_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT>
trait ENUM_ANY_HELP<UNIT ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_TRUE ,R2X> ;
} ;

template <class...UNIT>
using ENUM_ANY = typename ENUM_ANY_HELP<TYPEAS<UNIT...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUM_HELP ;

template <class UNIT>
trait ENUM_SUM_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class UNIT>
trait ENUM_SUM_HELP<UNIT ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using R2X = typename ENUM_SUM_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using RET = ENUM_ADD<R1X ,R2X> ;
} ;

template <class...UNIT>
using ENUM_SUM = typename ENUM_SUM_HELP<TYPEAS<UNIT...> ,ALWAYS>::RET ;

struct Proxy {
	implicit Proxy () = delete ;
	implicit ~Proxy () = delete ;
	implicit Proxy (CREF<Proxy>) = delete ;
	forceinline VREF<Proxy> operator= (CREF<Proxy>) = delete ;
	implicit Proxy (RREF<Proxy>) = delete ;
	forceinline VREF<Proxy> operator= (RREF<Proxy>) = delete ;
} ;

struct Interface {
	implicit Interface () = default ;
	virtual ~Interface () = default ;
	implicit Interface (CREF<Interface>) = delete ;
	forceinline VREF<Interface> operator= (CREF<Interface>) = delete ;
	//@info: fuck clang
	implicit Interface (Interface &&) = default ;
	forceinline VREF<Interface> operator= (Interface &&) = default ;
	virtual void finalize () {}
} ;

template <class...>
trait PLACEHOLDER_HELP ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	struct PlaceHolder {} ;
} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using Holder = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::PlaceHolder ;

	struct PlaceHolder implement Holder {} ;
} ;

template <class RANK>
using PlaceHolder = typename PLACEHOLDER_HELP<RANK ,ALWAYS>::PlaceHolder ;

using RANK0 = ENUMAS<VAL ,(+0)> ;
using RANK1 = ENUMAS<VAL ,(+1)> ;
using RANK2 = ENUMAS<VAL ,(+2)> ;
using RANK3 = ENUMAS<VAL ,(+3)> ;
using RANK4 = ENUMAS<VAL ,(+4)> ;
using RANK5 = ENUMAS<VAL ,(+5)> ;
using RANK6 = ENUMAS<VAL ,(+6)> ;
using RANK7 = ENUMAS<VAL ,(+7)> ;
using RANK8 = ENUMAS<VAL ,(+8)> ;
using RANK9 = ENUMAS<VAL ,(+9)> ;
using RANKX = ENUMAS<VAL ,10> ;

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

template <class UNIT>
using IS_BOOL = IS_SAME<UNIT ,BOOL> ;

template <class...>
trait IS_VALUE_HELP ;

template <class UNIT>
trait IS_VALUE_HELP<UNIT ,ALWAYS> {
	using R1X = IS_SAME<UNIT ,VAL32> ;
	using R2X = IS_SAME<UNIT ,VAL64> ;

	enum { value = ENUM_ANY<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_VALUE = typename IS_VALUE_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_FLOAT_HELP ;

template <class UNIT>
trait IS_FLOAT_HELP<UNIT ,ALWAYS> {
	using R1X = IS_SAME<UNIT ,SINGLE> ;
	using R2X = IS_SAME<UNIT ,DOUBLE> ;

	enum { value = ENUM_ANY<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_FLOAT = typename IS_FLOAT_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_TEXT_HELP ;

template <class UNIT>
trait IS_TEXT_HELP<UNIT ,ALWAYS> {
	using R1X = IS_SAME<UNIT ,STRA> ;
	using R2X = IS_SAME<UNIT ,STRW> ;
	using R3X = IS_SAME<UNIT ,STRU8> ;
	using R4X = IS_SAME<UNIT ,STRU16> ;
	using R5X = IS_SAME<UNIT ,STRU32> ;

	enum { value = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_TEXT = typename IS_TEXT_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_BYTE_HELP ;

template <class UNIT>
trait IS_BYTE_HELP<UNIT ,ALWAYS> {
	using R1X = IS_SAME<UNIT ,BYTE> ;
	using R2X = IS_SAME<UNIT ,WORD> ;
	using R3X = IS_SAME<UNIT ,CHAR> ;
	using R4X = IS_SAME<UNIT ,DATA> ;

	enum { value = ENUM_ANY<R1X ,R2X ,R3X ,R4X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_BYTE = typename IS_BYTE_HELP<UNIT ,ALWAYS>::RET ;

template <class UNIT>
using IS_NULL = IS_SAME<UNIT ,DEF<typeof (NULL)>> ;

template <class UNIT>
using IS_VOID = IS_SAME<UNIT ,void> ;

template <class...>
trait IS_SCALAR_HELP ;

template <class UNIT>
trait IS_SCALAR_HELP<UNIT ,ALWAYS> {
	using R1X = IS_VALUE<UNIT> ;
	using R2X = IS_FLOAT<UNIT> ;

	enum { value = ENUM_ANY<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_SCALAR = typename IS_SCALAR_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_BASIC_HELP ;

template <class UNIT>
trait IS_BASIC_HELP<UNIT ,ALWAYS> {
	using R1X = IS_BOOL<UNIT> ;
	using R2X = IS_VALUE<UNIT> ;
	using R3X = IS_FLOAT<UNIT> ;
	using R4X = IS_TEXT<UNIT> ;
	using R5X = IS_BYTE<UNIT> ;
	using R6X = IS_NULL<UNIT> ;

	enum { value = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_BASIC = typename IS_BASIC_HELP<UNIT ,ALWAYS>::RET ;

using VARIABLE = ENUMAS<VAL ,(-1)> ;
using CONSTANT = ENUMAS<VAL ,(-2)> ;
using REGISTER = ENUMAS<VAL ,(-3)> ;
using ORDINARY = ENUMAS<VAL ,(-4)> ;

template <class...>
trait REFLECT_REF_HELP ;

template <class UNIT ,class SIDE>
trait REFLECT_REF_HELP<UNIT ,SIDE> {
	using RET = ORDINARY ;
} ;

template <class UNIT>
trait REFLECT_REF_HELP<UNIT ,VREF<UNIT>> {
	using RET = VARIABLE ;
} ;

template <class UNIT>
trait REFLECT_REF_HELP<UNIT ,CREF<UNIT>> {
	using RET = CONSTANT ;
} ;

template <class UNIT>
trait REFLECT_REF_HELP<UNIT ,RREF<UNIT>> {
	using RET = REGISTER ;
} ;

template <class UNIT>
using REFLECT_REF = typename REFLECT_REF_HELP<REMOVE_REF<UNIT> ,UNIT>::RET ;

template <class UNIT>
using IS_VARIABLE = IS_SAME<REFLECT_REF<UNIT> ,VARIABLE> ;

template <class UNIT>
using IS_CONSTANT = IS_SAME<REFLECT_REF<UNIT> ,CONSTANT> ;

template <class UNIT>
using IS_REGISTER = IS_SAME<REFLECT_REF<UNIT> ,REGISTER> ;

template <class UNIT>
using IS_ORDINARY = IS_SAME<REFLECT_REF<UNIT> ,ORDINARY> ;

template <class...>
trait REFLECT_POINTER_HELP ;

template <class UNIT>
trait REFLECT_POINTER_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT>
trait REFLECT_POINTER_HELP<DEF<UNIT *>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
trait REFLECT_POINTER_HELP<DEF<const UNIT *>> {
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_POINTER = typename REFLECT_POINTER_HELP<REMOVE_REF<UNIT>>::RET ;

template <class...>
trait PTR_HELP ;

template <class UNIT ,class COND>
trait PTR_HELP<UNIT ,COND ,REQUIRE<COND>> {
	using RET = DEF<const DEF<UNIT *>> ;
} ;

template <class UNIT ,class COND>
trait PTR_HELP<UNIT ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	using RET = DEF<const DEF<const UNIT *>> ;
} ;

template <class UNIT>
using PTR = typename PTR_HELP<REMOVE_REF<UNIT> ,IS_VARIABLE<UNIT> ,ALWAYS>::RET ;

template <class...>
trait REFLECT_ARRAY_HELP ;

template <class UNIT>
trait REFLECT_ARRAY_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT>
trait REFLECT_ARRAY_HELP<DEF<UNIT[]>> {
	using ITEM = UNIT ;
	using SIZE = ENUM_ZERO ;
	using RET = ENUM_TRUE ;
} ;

template <class UNIT ,csc_diff_t SIDE>
trait REFLECT_ARRAY_HELP<DEF<UNIT[SIDE]>> {
	using ITEM = UNIT ;
	using SIZE = ENUMAS<VAL ,SIDE> ;
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_ARRAY = typename REFLECT_ARRAY_HELP<REMOVE_REF<UNIT>>::RET ;

template <class UNIT>
using ARRAY_ITEM = typename REFLECT_ARRAY_HELP<REMOVE_REF<UNIT>>::ITEM ;

template <class UNIT>
using ARRAY_SIZE = typename REFLECT_ARRAY_HELP<REMOVE_REF<UNIT>>::SIZE ;

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
	enum { value = SIZE::expr } ;
	using RET = DEF<ITEM[value]> ;
} ;

template <class ITEM ,class SIZE = ENUM_ZERO>
using ARR = typename ARR_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class UNIT>
trait REFLECT_FUNCTION_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class FIRST ,class...REST ,class UNIT>
trait REFLECT_FUNCTION_HELP<DEF<FIRST (UNIT::*) (REST...)>> {
	using RETURN = FIRST ;
	using PARAMS = TYPEAS<XREF<REST>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class FIRST ,class...REST ,class UNIT>
trait REFLECT_FUNCTION_HELP<DEF<FIRST (UNIT::*) (REST...) const>> {
	using RETURN = FIRST ;
	using PARAMS = TYPEAS<XREF<REST>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_FUNCTION = typename REFLECT_FUNCTION_HELP<DEF<typeof (&UNIT::operator())>>::RET ;

template <class UNIT>
using FUNCTION_RETURN = typename REFLECT_FUNCTION_HELP<DEF<typeof (&UNIT::operator())>>::RETURN ;

template <class UNIT>
using FUNCTION_PARAMS = typename REFLECT_FUNCTION_HELP<DEF<typeof (&UNIT::operator())>>::PARAMS ;

template <class...>
trait IS_INTPTR_HELP ;

template <class UNIT>
trait IS_INTPTR_HELP<UNIT ,REQUIRE<IS_BASIC<UNIT>>> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT>
trait IS_INTPTR_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_BASIC<UNIT>>>> {
	using R1X = MACRO_IS_ENUMCLASS<UNIT> ;
	using R2X = IS_POINTER<UNIT> ;
	using R3X = IS_SAME<UNIT ,csc_byte8_t> ;
	using R4X = IS_SAME<UNIT ,csc_byte16_t> ;
	using R5X = IS_SAME<UNIT ,csc_byte32_t> ;
	using R6X = IS_SAME<UNIT ,csc_byte64_t> ;
	using R7X = IS_SAME<UNIT ,csc_enum_t> ;

	enum { value = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X ,R7X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_INTPTR = typename IS_INTPTR_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_CLASS_HELP ;

template <class UNIT>
trait IS_CLASS_HELP<UNIT ,ALWAYS> {
	using R1X = MACRO_IS_CLASS<UNIT> ;
	using R3X = ENUM_NOT<IS_ENUM<UNIT>> ;
	using R4X = ENUM_NOT<IS_TYPE<UNIT>> ;
	using R5X = ENUM_NOT<IS_BASIC<UNIT>> ;
	using R6X = ENUM_NOT<IS_INTPTR<UNIT>> ;

	enum { value = ENUM_ALL<R1X ,R3X ,R4X ,R5X ,R6X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_CLASS = typename IS_CLASS_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_DEFAULT_HELP ;

template <class UNIT>
trait IS_DEFAULT_HELP<UNIT ,ALWAYS> {
	using R1X = MACRO_IS_CONSTRUCTIBLE<UNIT> ;
	using R2X = MACRO_IS_DESTRUCTIBLE<UNIT> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<UNIT> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<UNIT> ;

	enum { value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_DEFAULT = typename IS_DEFAULT_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_CLONEABLE_HELP ;

template <class UNIT>
trait IS_CLONEABLE_HELP<UNIT ,ALWAYS> {
	using R1X = MACRO_IS_COPY_CONSTRUCTIBLE<UNIT> ;
	using R2X = MACRO_IS_COPY_ASSIGNABLE<UNIT> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<UNIT> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<UNIT> ;

	enum { value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_CLONEABLE = typename IS_CLONEABLE_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_TRIVIAL_HELP ;

template <class UNIT>
trait IS_TRIVIAL_HELP<UNIT ,ALWAYS> {
	using R1X = MACRO_IS_TRIVIAL_CONSTRUCTIBLE<UNIT> ;
	using R2X = MACRO_IS_TRIVIAL_DESTRUCTIBLE<UNIT> ;

	enum { value = ENUM_ALL<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_TRIVIAL = typename IS_TRIVIAL_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_EXTEND_HELP ;

template <class FROM ,class INTO>
trait IS_EXTEND_HELP<FROM ,INTO ,ALWAYS> {
	using R1X = IS_SAME<FROM ,INTO> ;
	using R2X = MACRO_IS_EXTEND<FROM ,INTO> ;

	enum { value = ENUM_ANY<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class FROM ,class INTO>
using IS_EXTEND = typename IS_EXTEND_HELP<FROM ,INTO ,ALWAYS>::RET ;

template <class...>
trait IS_INTERFACE_HELP ;

template <class UNIT>
trait IS_INTERFACE_HELP<UNIT ,REQUIRE<IS_CLASS<UNIT>>> {
	using R1X = IS_EXTEND<Interface ,UNIT> ;
	using R2X = MACRO_IS_INTERFACE<UNIT> ;
	using R3X = ENUM_EQUAL<SIZE_OF<UNIT> ,SIZE_OF<Interface>> ;
	using R4X = ENUM_EQUAL<ALIGN_OF<UNIT> ,ALIGN_OF<Interface>> ;

	enum { value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
trait IS_INTERFACE_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_CLASS<UNIT>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT>
using IS_INTERFACE = typename IS_INTERFACE_HELP<UNIT ,ALWAYS>::RET ;

template <class UNIT>
using IS_POLYMORPHIC = IS_EXTEND<Interface ,UNIT> ;

template <class UNIT>
using IS_PLACEHOLDER = IS_EXTEND<DEF<typeof (PH0)> ,UNIT> ;

template <class...>
trait TOGETHER_HELP ;

template <class UNIT>
trait TOGETHER_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
	struct Together implement Interface {} ;
} ;

using InterfaceTogether = typename TOGETHER_HELP<TYPEAS<> ,ALWAYS>::Together ;

template <class UNIT>
trait TOGETHER_HELP<UNIT ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT>>>> {
	using Binder = TYPE_FIRST_ONE<UNIT> ;
	require (IS_INTERFACE<Binder>) ;
	require (ENUM_NOT<IS_EXTEND<InterfaceTogether ,Binder>>) ;
	using Holder = typename TOGETHER_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::Together ;

	struct Together implement Holder ,Binder {} ;
} ;

template <class...UNIT>
using Together = typename TOGETHER_HELP<TYPE_REVERSE<TYPEAS<UNIT...>> ,ALWAYS>::Together ;

template <class...>
trait IS_CONVERTIBLE_HELP ;

template <class FROM ,class INTO>
trait IS_CONVERTIBLE_HELP<FROM ,INTO ,ALWAYS> {
	using R1X = MACRO_IS_CONVERTIBLE<FROM ,INTO> ;

	enum { value = ENUM_ANY<R1X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class FROM ,class INTO>
using IS_CONVERTIBLE = typename IS_CONVERTIBLE_HELP<XREF<FROM> ,XREF<INTO> ,ALWAYS>::RET ;

template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...PARAM ,class UNIT>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<PARAM...> ,UNIT ,ALWAYS> {
	using R1X = UNIT ;
	using R2X = DEF<RETURN (*) (PARAM...)> ;
	using R3X = IS_CONVERTIBLE<CREF<R1X> ,RREF<R2X>> ;
	using RET = ENUM_NOT<R3X> ;
} ;

template <class UNIT>
using IS_EFFECTIVE = typename IS_EFFECTIVE_HELP<FUNCTION_RETURN<UNIT> ,FUNCTION_PARAMS<UNIT> ,UNIT ,ALWAYS>::RET ;

template <class...>
trait IS_OBJECT_HELP ;

template <class UNIT>
trait IS_OBJECT_HELP<UNIT ,ALWAYS> {
	using R1X = IS_BASIC<UNIT> ;
	using R2X = IS_INTPTR<UNIT> ;
	using R3X = IS_CLASS<UNIT> ;

	enum { value = ENUM_ANY<R1X ,R2X ,R3X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class UNIT>
using IS_OBJECT = typename IS_OBJECT_HELP<UNIT ,ALWAYS>::RET ;

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

template <class UNIT>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<UNIT> ,ALIGN_OF<UNIT> ,ALWAYS>::RET ;

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

template <class UNIT>
using TEXT_BASE = typename TEXT_BASE_HELP<SIZE_OF<UNIT> ,ALIGN_OF<UNIT> ,ALWAYS>::RET ;

using STRUA = TEXT_BASE<STRA> ;
using STRUW = TEXT_BASE<STRW> ;

template <class BASE ,class ALIGN>
using ENUM_ALIGN = ENUM_MUL<ENUM_DIV<ENUM_ADD<BASE ,ENUM_DEC<ALIGN>> ,ALIGN> ,ALIGN> ;

template <class...>
trait STORAGE_HELP ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<BYTE>>>> {
	using R1X = BYTE ;
	using R2X = ENUM_DIV<ENUM_ALIGN<SIZE ,ALIGN> ,ALIGN> ;
	require (ENUM_GT_ZERO<R2X>) ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<WORD>>>> {
	using R1X = WORD ;
	using R2X = ENUM_DIV<ENUM_ALIGN<SIZE ,ALIGN> ,ALIGN> ;
	require (ENUM_GT_ZERO<R2X>) ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<CHAR>>>> {
	using R1X = CHAR ;
	using R2X = ENUM_DIV<ENUM_ALIGN<SIZE ,ALIGN> ,ALIGN> ;
	require (ENUM_GT_ZERO<R2X>) ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN>
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<DATA>>>> {
	using R1X = DATA ;
	using R2X = ENUM_DIV<ENUM_ALIGN<SIZE ,ALIGN> ,ALIGN> ;
	require (ENUM_GT_ZERO<R2X>) ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN = ENUM_IDEN>
using Storage = typename STORAGE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

template <class...>
trait REFLECT_TEMP_HELP ;

template <class UNIT>
trait REFLECT_TEMP_HELP<UNIT> {
	using RET = ENUM_FALSE ;
} ;

template <class UNIT ,class SIDE>
trait REFLECT_TEMP_HELP<TEMPAS<UNIT ,SIDE>> {
	using ITEM = UNIT ;
	using SIZE = SIDE ;
	using RET = ENUM_TRUE ;
} ;

template <class UNIT>
using IS_TEMP = typename REFLECT_TEMP_HELP<REMOVE_REF<UNIT>>::RET ;

template <class UNIT>
using TEMP_ITEM = typename REFLECT_TEMP_HELP<REMOVE_REF<UNIT>>::ITEM ;

template <class UNIT>
using TEMP_SIZE = typename REFLECT_TEMP_HELP<REMOVE_REF<UNIT>>::SIZE ;

template <class...>
trait TEMP_HELP ;

template <class UNIT>
trait TEMP_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	using RET = TEMPAS<void ,void> ;
} ;

template <class UNIT>
trait TEMP_HELP<UNIT ,REQUIRE<IS_TEMP<UNIT>>> {
	using R1X = TEMP_SIZE<UNIT> ;
	using RET = TEMPAS<UNIT ,R1X> ;
} ;

template <class UNIT>
trait TEMP_HELP<UNIT ,REQUIRE<IS_ARRAY<UNIT>>> {
	using R1X = ARRAY_ITEM<UNIT> ;
	using R2X = ENUM_EQ_ZERO<ARRAY_SIZE<UNIT>> ;
	using R3X = CONDITIONAL<R2X ,R1X ,UNIT> ;
	using R4X = Storage<SIZE_OF<R3X> ,ALIGN_OF<R3X>> ;
	using R5X = CONDITIONAL<R2X ,void ,R4X> ;
	using RET = TEMPAS<UNIT ,R5X> ;
} ;

template <class UNIT>
trait TEMP_HELP<UNIT ,REQUIRE<ENUM_ALL<IS_OBJECT<UNIT> ,ENUM_NOT<IS_TEMP<UNIT>>>>> {
	using R1X = Storage<SIZE_OF<UNIT> ,ALIGN_OF<UNIT>> ;
	using RET = TEMPAS<UNIT ,R1X> ;
} ;

template <class UNIT>
using TEMP = typename TEMP_HELP<REMOVE_REF<UNIT> ,ALWAYS>::RET ;

template <class UNIT>
using ENUM_ABS = CONDITIONAL<ENUM_COMPR_GTEQ<UNIT ,ENUM_ZERO> ,UNIT ,ENUM_MINUS<UNIT>> ;

template <class...>
trait ENUM_MIN_HELP ;

template <class UNIT>
trait ENUM_MIN_HELP<UNIT ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT>>>> {
	using RET = TYPE_FIRST_ONE<UNIT> ;
} ;

template <class UNIT>
trait ENUM_MIN_HELP<UNIT ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using R2X = typename ENUM_MIN_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_LTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...UNIT>
using ENUM_MIN = typename ENUM_MIN_HELP<TYPEAS<UNIT...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_MAX_HELP ;

template <class UNIT>
trait ENUM_MAX_HELP<UNIT ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<UNIT>>>> {
	using RET = TYPE_FIRST_ONE<UNIT> ;
} ;

template <class UNIT>
trait ENUM_MAX_HELP<UNIT ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<UNIT>>>> {
	using R1X = TYPE_FIRST_ONE<UNIT> ;
	using R2X = typename ENUM_MAX_HELP<TYPE_FIRST_REST<UNIT> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_GTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...UNIT>
using ENUM_MAX = typename ENUM_MAX_HELP<TYPEAS<UNIT...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_BETWEEN_HELP ;

template <class CURR ,class BEGIN ,class END>
trait ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS> {
	using R1X = ENUM_COMPR_GTEQ<CURR ,BEGIN> ;
	using R2X = ENUM_COMPR_LT<CURR ,END> ;

	enum { value = ENUM_ALL<R1X ,R2X>::expr } ;
	using RET = ENUMAS<BOOL ,value> ;
} ;

template <class CURR ,class BEGIN ,class END>
using ENUM_BETWEEN = typename ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS>::RET ;
} ;