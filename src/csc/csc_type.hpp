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

#ifdef __CSC_CONFIG_VAL32__
using VAL = VAL32 ;

static constexpr auto VAL_MAX = VAL32_MAX ;
static constexpr auto VAL_MIN = VAL32_MIN ;
static constexpr auto VAL_ABS = VAL32_ABS ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using VAL = VAL64 ;

static constexpr auto VAL_MAX = VAL64_MAX ;
static constexpr auto VAL_MIN = VAL64_MIN ;
static constexpr auto VAL_ABS = VAL64_ABS ;
#endif

static constexpr auto ZERO = VAL (+0) ;
static constexpr auto IDEN = VAL (+1) ;
static constexpr auto NONE = VAL (-1) ;
static constexpr auto USED = VAL (-2) ;

using INDEX = VAL ;
using LENGTH = VAL ;
using FLAG = VAL ;

using FLT32 = csc_float32_t ;
using FLT64 = csc_float64_t ;

static constexpr auto FLT32_MAX = FLT32 (3.402823466E+38) ;
static constexpr auto FLT32_MIN = -FLT32_MAX ;
static constexpr auto FLT32_LOW = FLT32 (1.175494351E-38) ;
static constexpr auto FLT32_EPS = FLT32 (1.192092896E-07) ;
static constexpr auto FLT32_INF = FLT32 (infinity) ;
static constexpr auto FLT64_MAX = FLT64 (1.7976931348623158E+308) ;
static constexpr auto FLT64_MIN = -FLT64_MAX ;
static constexpr auto FLT64_LOW = FLT64 (2.2250738585072014E-308) ;
static constexpr auto FLT64_EPS = FLT64 (2.2204460492503131E-016) ;
static constexpr auto FLT64_INF = FLT64 (infinity) ;

enum class BYTE :csc_uint8_t ;
enum class WORD :csc_uint16_t ;
enum class CHAR :csc_uint32_t ;
enum class QUAD :csc_uint64_t ;

forceinline constexpr BYTE operator| (CREF<BYTE> a ,CREF<BYTE> b) noexcept {
	return BYTE (csc_uint8_t (a) | csc_uint8_t (b)) ;
}

forceinline void operator|= (VREF<BYTE> a ,CREF<BYTE> b) noexcept {
	a = a | b ;
}

forceinline constexpr BYTE operator& (CREF<BYTE> a ,CREF<BYTE> b) noexcept {
	return BYTE (csc_uint8_t (a) & csc_uint8_t (b)) ;
}

forceinline void operator&= (VREF<BYTE> a ,CREF<BYTE> b) noexcept {
	a = a & b ;
}

forceinline constexpr BYTE operator^ (CREF<BYTE> a ,CREF<BYTE> b) noexcept {
	return BYTE (csc_uint8_t (a) ^ csc_uint8_t (b)) ;
}

forceinline void operator^= (VREF<BYTE> a ,CREF<BYTE> b) noexcept {
	a = a ^ b ;
}

forceinline constexpr BYTE operator~ (CREF<BYTE> a) noexcept {
	return BYTE (~csc_uint8_t (a)) ;
}

forceinline constexpr BYTE operator<< (CREF<BYTE> a ,CREF<LENGTH> b) noexcept {
	return BYTE (csc_uint8_t (a) << b) ;
}

forceinline constexpr BYTE operator>> (CREF<BYTE> a ,CREF<LENGTH> b) noexcept {
	return BYTE (csc_uint8_t (a) >> b) ;
}

forceinline constexpr WORD operator| (CREF<WORD> a ,CREF<WORD> b) noexcept {
	return WORD (csc_uint16_t (a) | csc_uint16_t (b)) ;
}

forceinline void operator|= (VREF<WORD> a ,CREF<WORD> b) noexcept {
	a = a | b ;
}

forceinline constexpr WORD operator& (CREF<WORD> a ,CREF<WORD> b) noexcept {
	return WORD (csc_uint16_t (a) & csc_uint16_t (b)) ;
}

forceinline void operator&= (VREF<WORD> a ,CREF<WORD> b) noexcept {
	a = a & b ;
}

forceinline constexpr WORD operator^ (CREF<WORD> a ,CREF<WORD> b) noexcept {
	return WORD (csc_uint16_t (a) ^ csc_uint16_t (b)) ;
}

forceinline constexpr WORD operator~ (CREF<WORD> a) noexcept {
	return WORD (~csc_uint16_t (a)) ;
}

forceinline constexpr WORD operator<< (CREF<WORD> a ,CREF<LENGTH> b) noexcept {
	return WORD (csc_uint16_t (a) << b) ;
}

forceinline constexpr WORD operator>> (CREF<WORD> a ,CREF<LENGTH> b) noexcept {
	return WORD (csc_uint16_t (a) >> b) ;
}

forceinline constexpr CHAR operator| (CREF<CHAR> a ,CREF<CHAR> b) noexcept {
	return CHAR (csc_uint32_t (a) | csc_uint32_t (b)) ;
}

forceinline void operator|= (VREF<CHAR> a ,CREF<CHAR> b) noexcept {
	a = a | b ;
}

forceinline constexpr CHAR operator& (CREF<CHAR> a ,CREF<CHAR> b) noexcept {
	return CHAR (csc_uint32_t (a) & csc_uint32_t (b)) ;
}

forceinline void operator&= (VREF<CHAR> a ,CREF<CHAR> b) noexcept {
	a = a & b ;
}

forceinline constexpr CHAR operator^ (CREF<CHAR> a ,CREF<CHAR> b) noexcept {
	return CHAR (csc_uint32_t (a) ^ csc_uint32_t (b)) ;
}

forceinline constexpr CHAR operator~ (CREF<CHAR> a) noexcept {
	return CHAR (~csc_uint32_t (a)) ;
}

forceinline constexpr CHAR operator<< (CREF<CHAR> a ,CREF<LENGTH> b) noexcept {
	return CHAR (csc_uint32_t (a) << b) ;
}

forceinline constexpr CHAR operator>> (CREF<CHAR> a ,CREF<LENGTH> b) noexcept {
	return CHAR (csc_uint32_t (a) >> b) ;
}

forceinline constexpr QUAD operator| (CREF<QUAD> a ,CREF<QUAD> b) noexcept {
	return QUAD (csc_uint64_t (a) | csc_uint64_t (b)) ;
}

forceinline void operator|= (VREF<QUAD> a ,CREF<QUAD> b) noexcept {
	a = a | b ;
}

forceinline constexpr QUAD operator& (CREF<QUAD> a ,CREF<QUAD> b) noexcept {
	return QUAD (csc_uint64_t (a) & csc_uint64_t (b)) ;
}

forceinline void operator&= (VREF<QUAD> a ,CREF<QUAD> b) noexcept {
	a = a & b ;
}

forceinline constexpr QUAD operator^ (CREF<QUAD> a ,CREF<QUAD> b) noexcept {
	return QUAD (csc_uint64_t (a) ^ csc_uint64_t (b)) ;
}

forceinline constexpr QUAD operator~ (CREF<QUAD> a) noexcept {
	return QUAD (~csc_uint64_t (a)) ;
}

forceinline constexpr QUAD operator<< (CREF<QUAD> a ,CREF<LENGTH> b) noexcept {
	return QUAD (csc_uint64_t (a) << b) ;
}

forceinline constexpr QUAD operator>> (CREF<QUAD> a ,CREF<LENGTH> b) noexcept {
	return QUAD (csc_uint64_t (a) >> b) ;
}

using STRA = csc_char_t ;
using STRW = csc_wchar_t ;
using STRU8 = csc_char8_t ;
using STRU16 = csc_char16_t ;
using STRU32 = csc_char32_t ;

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;
#endif

#ifdef __CSC_CONFIG_STRW__
using STR = STRW ;
#endif

static constexpr auto NULL = nullptr ;

template <class...>
trait SIZE_OF_HELP ;

template <class A>
trait SIZE_OF_HELP<A ,ALWAYS> {
	using RET = ENUM<(sizeof (A))> ;
} ;

template <class A>
using SIZE_OF = typename SIZE_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait ALIGN_OF_HELP ;

template <class A>
trait ALIGN_OF_HELP<A ,ALWAYS> {
	using RET = ENUM<(alignof (A))> ;
} ;

template <class A>
using ALIGN_OF = typename ALIGN_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait RANK_OF_HELP ;

template <class...A>
trait RANK_OF_HELP<TYPE<A...> ,ALWAYS> {
	using RET = ENUM<(sizeof... (A))> ;
} ;

template <class A>
using RANK_OF = typename RANK_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait REFLECT_ENUM_HELP ;

template <class A>
trait REFLECT_ENUM_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <csc_diff_t A>
trait REFLECT_ENUM_HELP<ENUM<A>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_ENUM = typename REFLECT_ENUM_HELP<A>::RET ;

template <class...>
trait REFLECT_TYPE_HELP ;

template <class A>
trait REFLECT_TYPE_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_TYPE_HELP<TYPE<A>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPE<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
trait REFLECT_TYPE_HELP<TYPE<A ,B>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPE<B> ;
	using M2ND_ONE = B ;
	using M2ND_REST = TYPE<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B ,class C ,class...D>
trait REFLECT_TYPE_HELP<TYPE<A ,B ,C ,D...>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPE<B ,C ,D...> ;
	using M2ND_ONE = B ;
	using M2ND_REST = TYPE<C ,D...> ;
	using M3RD_ONE = C ;
	using M3RD_REST = TYPE<D...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_TYPE = typename REFLECT_TYPE_HELP<A>::RET ;

template <class A>
using TYPE_M1ST_ONE = typename REFLECT_TYPE_HELP<A>::M1ST_ONE ;

template <class A>
using TYPE_M1ST_REST = typename REFLECT_TYPE_HELP<A>::M1ST_REST ;

template <class A>
using TYPE_M2ND_ONE = typename REFLECT_TYPE_HELP<A>::M2ND_ONE ;

template <class A>
using TYPE_M2ND_REST = typename REFLECT_TYPE_HELP<A>::M2ND_REST ;

template <class A>
using TYPE_M3RD_ONE = typename REFLECT_TYPE_HELP<A>::M3RD_ONE ;

template <class A>
using TYPE_M3RD_REST = typename REFLECT_TYPE_HELP<A>::M3RD_REST ;

using RANK0 = ENUM<(+0)> ;
using RANK1 = ENUM<(+1)> ;
using RANK2 = ENUM<(+2)> ;
using RANK3 = ENUM<(+3)> ;
using RANK4 = ENUM<(+4)> ;
using RANK5 = ENUM<(+5)> ;
using RANK6 = ENUM<(+6)> ;
using RANK7 = ENUM<(+7)> ;
using RANK8 = ENUM<(+8)> ;
using RANK9 = ENUM<(+9)> ;
using RANKX = ENUM<10> ;

template <class...>
trait ENUM_NOT_HELP ;

template <class A>
trait ENUM_NOT_HELP<A ,ALWAYS> {
	using RET = ENUM<(ifnot (A::expr))> ;
} ;

template <class A>
using ENUM_NOT = typename ENUM_NOT_HELP<A ,ALWAYS>::RET ;

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

template <class A ,class B>
trait ENUM_EQUAL_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr == B::expr)> ;
} ;

template <class A ,class B>
using ENUM_EQUAL = typename ENUM_EQUAL_HELP<A ,B ,ALWAYS>::RET ;

template <class A ,class B>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<A ,B>> ;

template <class...>
trait ENUM_COMPR_LT_HELP ;

template <class A ,class B>
trait ENUM_COMPR_LT_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr < B::expr)> ;
} ;

template <class A ,class B>
using ENUM_COMPR_LT = typename ENUM_COMPR_LT_HELP<A ,B ,ALWAYS>::RET ;

template <class A ,class B>
using ENUM_COMPR_GT = ENUM_COMPR_LT<B ,A> ;

template <class A ,class B>
using ENUM_COMPR_LTEQ = ENUM_NOT<ENUM_COMPR_GT<A ,B>> ;

template <class A ,class B>
using ENUM_COMPR_GTEQ = ENUM_NOT<ENUM_COMPR_LT<A ,B>> ;

template <class...>
trait ENUM_COMPR_HELP ;

template <class A ,class B>
trait ENUM_COMPR_HELP<A ,B ,ALWAYS> {
	using R1X = ENUM_COMPR_LT<A ,B> ;
	using R2X = ENUM_COMPR_GT<A ,B> ;
	using R3X = CONDITIONAL<R1X ,ENUM<(-1)> ,CONDITIONAL<R2X ,RANK1 ,RANK0>> ;

	using RET = ENUM<(R3X::expr)> ;
} ;

template <class A ,class B>
using ENUM_COMPR = typename ENUM_COMPR_HELP<A ,B ,ALWAYS>::RET ;

template <class A>
using ENUM_EQ_ZERO = ENUM_EQUAL<A ,RANK0> ;

template <class A>
using ENUM_GT_ZERO = ENUM_COMPR_GT<A ,RANK0> ;

template <class A>
using ENUM_EQ_IDEN = ENUM_EQUAL<A ,RANK1> ;

template <class A>
using ENUM_GT_IDEN = ENUM_COMPR_GT<A ,RANK1> ;

template <class...>
trait ENUM_ADD_HELP ;

template <class A ,class B>
trait ENUM_ADD_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr + B::expr)> ;
} ;

template <class A ,class B>
using ENUM_ADD = typename ENUM_ADD_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUB_HELP ;

template <class A ,class B>
trait ENUM_SUB_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr - B::expr)> ;
} ;

template <class A ,class B>
using ENUM_SUB = typename ENUM_SUB_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_MUL_HELP ;

template <class A ,class B>
trait ENUM_MUL_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr *B::expr)> ;
} ;

template <class A ,class B>
using ENUM_MUL = typename ENUM_MUL_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_DIV_HELP ;

template <class A ,class B>
trait ENUM_DIV_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr / B::expr)> ;
} ;

template <class A ,class B>
using ENUM_DIV = typename ENUM_DIV_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_MOD_HELP ;

template <class A ,class B>
trait ENUM_MOD_HELP<A ,B ,ALWAYS> {
	using RET = ENUM<(A::expr %B::expr)> ;
} ;

template <class A ,class B>
using ENUM_MOD = typename ENUM_MOD_HELP<A ,B ,ALWAYS>::RET ;

template <class A>
using ENUM_MINUS = ENUM_SUB<RANK0 ,A> ;

template <class A>
using ENUM_INC = ENUM_ADD<A ,RANK1> ;

template <class A>
using ENUM_DEC = ENUM_SUB<A ,RANK1> ;

template <class...>
trait TYPE_CAT_HELP ;

template <class...A ,class...B>
trait TYPE_CAT_HELP<TYPE<A...> ,TYPE<B...> ,ALWAYS> {
	using RET = TYPE<A... ,B...> ;
} ;

template <class A ,class B>
using TYPE_CAT = typename TYPE_CAT_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait TYPE_REVERSE_HELP ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = TYPE<> ;
} ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_EQ_IDEN<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using RET = TYPE<R1X> ;
} ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_GT_IDEN<RANK_OF<A>>>> {
	using R1X = TYPE<TYPE_M1ST_ONE<A>> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R2X ,R1X> ;
} ;

template <class A>
using TYPE_REVERSE = typename TYPE_REVERSE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait TYPE_REPEAT_HELP ;

template <class A ,class SIZE>
trait TYPE_REPEAT_HELP<A ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = TYPE<> ;
} ;

template <class A ,class SIZE>
trait TYPE_REPEAT_HELP<A ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using R1X = typename TYPE_REPEAT_HELP<A ,ENUM_DEC<SIZE> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R1X ,TYPE<A>> ;
} ;

template <class A ,class SIZE>
using TYPE_REPEAT = typename TYPE_REPEAT_HELP<A ,SIZE ,ALWAYS>::RET ;

template <class...>
trait TYPE_SENQUENCE_HELP ;

template <class SIZE ,class CURR>
trait TYPE_SENQUENCE_HELP<SIZE ,CURR ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	using RET = TYPE<> ;
} ;

template <class SIZE ,class CURR>
trait TYPE_SENQUENCE_HELP<SIZE ,CURR ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using R1X = typename TYPE_SENQUENCE_HELP<ENUM_DEC<SIZE> ,ENUM_INC<CURR> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<TYPE<CURR> ,R1X> ;
} ;

template <class SIZE>
using TYPE_SENQUENCE = typename TYPE_SENQUENCE_HELP<SIZE ,RANK0 ,ALWAYS>::RET ;

template <class...>
trait TYPE_PICK_HELP ;

template <class A ,class CURR>
trait TYPE_PICK_HELP<A ,CURR ,REQUIRE<ENUM_EQ_ZERO<CURR>>> {
	using RET = TYPE_M1ST_ONE<A> ;
} ;

template <class A ,class CURR>
trait TYPE_PICK_HELP<A ,CURR ,REQUIRE<ENUM_GT_ZERO<CURR>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_M1ST_REST<A> ,ENUM_DEC<CURR> ,ALWAYS>::RET ;
} ;

template <class A ,class CURR>
using TYPE_PICK = typename TYPE_PICK_HELP<A ,CURR ,ALWAYS>::RET ;

template <class...>
trait ENUM_ALL_HELP ;

template <class A>
trait ENUM_ALL_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait ENUM_ALL_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,R2X ,ENUM_FALSE> ;
} ;

template <class...A>
using ENUM_ALL = typename ENUM_ALL_HELP<TYPE<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_ANY_HELP ;

template <class A>
trait ENUM_ANY_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait ENUM_ANY_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_TRUE ,R2X> ;
} ;

template <class...A>
using ENUM_ANY = typename ENUM_ANY_HELP<TYPE<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUM_HELP ;

template <class A>
trait ENUM_SUM_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = RANK0 ;
} ;

template <class A>
trait ENUM_SUM_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_SUM_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = ENUM_ADD<R1X ,R2X> ;
} ;

template <class...A>
using ENUM_SUM = typename ENUM_SUM_HELP<TYPE<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_MAX_HELP ;

template <class A>
trait ENUM_MAX_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = RANK0 ;
} ;

template <class A>
trait ENUM_MAX_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_MAX_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<ENUM_COMPR_GTEQ<R1X ,R2X> ,R1X ,R2X> ;
} ;

template <class...A>
using ENUM_MAX = typename ENUM_MAX_HELP<TYPE<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_MIN_HELP ;

template <class A>
trait ENUM_MIN_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	using RET = RANK0 ;
} ;

template <class A>
trait ENUM_MIN_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_MIN_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<ENUM_COMPR_LTEQ<R1X ,R2X> ,R1X ,R2X> ;
} ;

template <class...A>
using ENUM_MIN = typename ENUM_MIN_HELP<TYPE<A...> ,ALWAYS>::RET ;

struct Proxy {
	implicit Proxy () = delete ;
	implicit ~Proxy () = delete ;
	implicit Proxy (CREF<Proxy> that) = delete ;
	forceinline VREF<Proxy> operator= (CREF<Proxy> that) = delete ;
	implicit Proxy (RREF<Proxy> that) = delete ;
	forceinline VREF<Proxy> operator= (RREF<Proxy> that) = delete ;
} ;

struct Interface {
	implicit Interface () = default ;
	virtual ~Interface () = default ;
	implicit Interface (CREF<Interface> that) = delete ;
	forceinline VREF<Interface> operator= (CREF<Interface> that) = delete ;
	implicit Interface (RREF<Interface> that) = default ;
	forceinline VREF<Interface> operator= (RREF<Interface> that) = default ;
} ;

struct Unknown implement Interface {
	virtual FLAG reflect (CREF<FLAG> uuid) const = 0 ;
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

template <class A>
using IS_BOOL = IS_SAME<A ,BOOL> ;

template <class...>
trait IS_VALUE_HELP ;

template <class A>
trait IS_VALUE_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,VAL32> ;
	using R2X = IS_SAME<A ,VAL64> ;

	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A>
using IS_VALUE = typename IS_VALUE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_FLOAT_HELP ;

template <class A>
trait IS_FLOAT_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,FLT32> ;
	using R2X = IS_SAME<A ,FLT64> ;

	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A>
using IS_FLOAT = typename IS_FLOAT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_TEXT_HELP ;

template <class A>
trait IS_TEXT_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,STRA> ;
	using R2X = IS_SAME<A ,STRW> ;
	using R3X = IS_SAME<A ,STRU8> ;
	using R4X = IS_SAME<A ,STRU16> ;
	using R5X = IS_SAME<A ,STRU32> ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X> ;
} ;

template <class A>
using IS_TEXT = typename IS_TEXT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_BYTE_HELP ;

template <class A>
trait IS_BYTE_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,BYTE> ;
	using R2X = IS_SAME<A ,WORD> ;
	using R3X = IS_SAME<A ,CHAR> ;
	using R4X = IS_SAME<A ,QUAD> ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class A>
using IS_BYTE = typename IS_BYTE_HELP<A ,ALWAYS>::RET ;

template <class A>
using IS_NULL = IS_SAME<A ,DEF<typeof (NULL)>> ;

template <class A>
using IS_VOID = IS_SAME<A ,void> ;

template <class...>
trait IS_SCALAR_HELP ;

template <class A>
trait IS_SCALAR_HELP<A ,ALWAYS> {
	using R1X = IS_VALUE<A> ;
	using R2X = IS_FLOAT<A> ;

	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A>
using IS_SCALAR = typename IS_SCALAR_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_BASIC_HELP ;

template <class A>
trait IS_BASIC_HELP<A ,ALWAYS> {
	using R1X = IS_BOOL<A> ;
	using R2X = IS_VALUE<A> ;
	using R3X = IS_FLOAT<A> ;
	using R4X = IS_TEXT<A> ;
	using R5X = IS_BYTE<A> ;
	using R6X = IS_NULL<A> ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X> ;
} ;

template <class A>
using IS_BASIC = typename IS_BASIC_HELP<A ,ALWAYS>::RET ;

using VARIABLE = ENUM<(-1)> ;
using CONSTANT = ENUM<(-2)> ;
using REGISTER = ENUM<(-3)> ;
using ORDINARY = ENUM<(-4)> ;

template <class...>
trait REFLECT_REF_HELP ;

template <class A ,class B>
trait REFLECT_REF_HELP<A ,B> {
	using RET = ORDINARY ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,VREF<A>> {
	using RET = VARIABLE ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,CREF<A>> {
	using RET = CONSTANT ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,RREF<A>> {
	using RET = REGISTER ;
} ;

template <class A>
using REFLECT_REF = typename REFLECT_REF_HELP<REMOVE_REF<A> ,A>::RET ;

template <class...>
trait REF_HELP ;

template <class A ,class B>
trait REF_HELP<A ,B ,REQUIRE<IS_SAME<B ,ORDINARY>>> {
	using RET = A ;
} ;

template <class A ,class B>
trait REF_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	using RET = VREF<A> ;
} ;

template <class A ,class B>
trait REF_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	using RET = CREF<A> ;
} ;

template <class A ,class B>
trait REF_HELP<A ,B ,REQUIRE<IS_SAME<B ,REGISTER>>> {
	using RET = RREF<A> ;
} ;

template <class A ,class B>
using REF = typename REF_HELP<REMOVE_REF<A> ,B ,ALWAYS>::RET ;

template <class...>
trait REFLECT_POINTER_HELP ;

template <class A>
trait REFLECT_POINTER_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_POINTER_HELP<DEF<A *>> {
	using BASE = A ;
	using CVR = VARIABLE ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait REFLECT_POINTER_HELP<DEF<const A *>> {
	using BASE = A ;
	using CVR = CONSTANT ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_POINTER = typename REFLECT_POINTER_HELP<REMOVE_REF<A>>::RET ;

template <class A>
using POINTER_CVR = typename REFLECT_POINTER_HELP<REMOVE_REF<A>>::CVR ;

template <class A ,class = REQUIRE<IS_SAME<A ,REMOVE_REF<A>>>>
using VPTR = DEF<const DEF<A *>> ;

template <class A ,class = REQUIRE<IS_SAME<A ,REMOVE_REF<A>>>>
using CPTR = DEF<const DEF<const A *>> ;

template <class...>
trait REFLECT_ARRAY_HELP ;

template <class A>
trait REFLECT_ARRAY_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_ARRAY_HELP<DEF<A[]>> {
	using ITEM = A ;
	using SIZE = RANK0 ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,csc_diff_t B>
trait REFLECT_ARRAY_HELP<DEF<A[B]>> {
	using ITEM = A ;
	using SIZE = ENUM<B> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_ARRAY = typename REFLECT_ARRAY_HELP<REMOVE_REF<A>>::RET ;

template <class A>
using ARRAY_ITEM = typename REFLECT_ARRAY_HELP<REMOVE_REF<A>>::ITEM ;

template <class A>
using ARRAY_SIZE = typename REFLECT_ARRAY_HELP<REMOVE_REF<A>>::SIZE ;

template <class...>
trait ARR_HELP ;

template <class A ,class SIZE>
trait ARR_HELP<A ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<A>>) ;
	using RET = DEF<A[]> ;
} ;

template <class A ,class SIZE>
trait ARR_HELP<A ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<A>>) ;
	using RET = DEF<A[SIZE::expr]> ;
} ;

template <class A ,class SIZE = RANK0>
using ARR = typename ARR_HELP<A ,SIZE ,ALWAYS>::RET ;

template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class A>
trait REFLECT_FUNCTION_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C:: *) (B...)>> {
	using RETURN = A ;
	using PARAMS = TYPE<XREF<B>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C:: *) (B...) const>> {
	using RETURN = A ;
	using PARAMS = TYPE<XREF<B>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_FUNCTION = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::RET ;

template <class A>
using FUNCTION_RETURN = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::RETURN ;

template <class A>
using FUNCTION_PARAMS = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::PARAMS ;

template <class...>
trait IS_UINT_HELP ;

template <class A>
trait IS_UINT_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait IS_UINT_HELP<A ,REQUIRE<ENUM_NOT<IS_BASIC<A>>>> {
	using R1X = MACRO_IS_UINT<A> ;
	using R2X = IS_POINTER<A> ;
	using R3X = IS_SAME<A ,csc_uint8_t> ;
	using R4X = IS_SAME<A ,csc_uint16_t> ;
	using R5X = IS_SAME<A ,csc_uint32_t> ;
	using R6X = IS_SAME<A ,csc_uint64_t> ;
	using R7X = IS_SAME<A ,csc_enum_t> ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X ,R7X> ;
} ;

template <class A>
using IS_UINT = typename IS_UINT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_CLASS_HELP ;

template <class A>
trait IS_CLASS_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_CLASS<A> ;
	using R3X = ENUM_NOT<IS_ENUM<A>> ;
	using R4X = ENUM_NOT<IS_TYPE<A>> ;
	using R5X = ENUM_NOT<IS_BASIC<A>> ;
	using R6X = ENUM_NOT<IS_UINT<A>> ;

	using RET = ENUM_ALL<R1X ,R3X ,R4X ,R5X ,R6X> ;
} ;

template <class A>
using IS_CLASS = typename IS_CLASS_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_DEFAULT_HELP ;

template <class A>
trait IS_DEFAULT_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_CONSTRUCTIBLE<A> ;
	using R2X = MACRO_IS_DESTRUCTIBLE<A> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<A> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<A> ;

	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class A>
using IS_DEFAULT = typename IS_DEFAULT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_TRIVIAL_HELP ;

template <class A>
trait IS_TRIVIAL_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_TRIVIAL_CONSTRUCTIBLE<A> ;
	using R2X = MACRO_IS_TRIVIAL_DESTRUCTIBLE<A> ;

	using RET = ENUM_ALL<R1X ,R2X> ;
} ;

template <class A>
using IS_TRIVIAL = typename IS_TRIVIAL_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_EXTEND_HELP ;

template <class A ,class B>
trait IS_EXTEND_HELP<A ,B ,ALWAYS> {
	using R1X = IS_SAME<A ,B> ;
	using R2X = MACRO_IS_EXTEND<A ,B> ;

	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A ,class B>
using IS_EXTEND = typename IS_EXTEND_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait IS_INTERFACE_HELP ;

template <class A>
trait IS_INTERFACE_HELP<A ,REQUIRE<IS_CLASS<A>>> {
	using R1X = IS_EXTEND<Interface ,A> ;
	using R3X = ENUM_EQUAL<SIZE_OF<A> ,SIZE_OF<Interface>> ;
	using R4X = ENUM_EQUAL<ALIGN_OF<A> ,ALIGN_OF<Interface>> ;

	using RET = ENUM_ALL<R1X ,R3X ,R4X> ;
} ;

template <class A>
trait IS_INTERFACE_HELP<A ,REQUIRE<ENUM_NOT<IS_CLASS<A>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
using IS_INTERFACE = typename IS_INTERFACE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait TOGETHER_HELP ;

template <class A>
trait TOGETHER_HELP<A ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<A>>>> {
	struct Together implement Interface {} ;
} ;

template <class A>
trait TOGETHER_HELP<A ,REQUIRE<ENUM_GT_ZERO<RANK_OF<A>>>> {
	using Binder = TYPE_M1ST_ONE<A> ;
	require (IS_INTERFACE<Binder>) ;
	using Holder = typename TOGETHER_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::Together ;

	struct Together implement Holder ,Binder {} ;
} ;

template <class...A>
using Together = typename TOGETHER_HELP<TYPE_REVERSE<TYPE<A...>> ,ALWAYS>::Together ;

template <class...>
trait IS_OBJECT_HELP ;

template <class A>
trait IS_OBJECT_HELP<A ,ALWAYS> {
	using R1X = IS_BASIC<A> ;
	using R2X = IS_UINT<A> ;
	using R3X = IS_CLASS<A> ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X> ;
} ;

template <class A>
using IS_OBJECT = typename IS_OBJECT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_CLONE_HELP ;

template <class A>
trait HAS_CLONE_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_COPY_CONSTRUCTIBLE<A> ;
	using R2X = MACRO_IS_COPY_ASSIGNABLE<A> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<A> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<A> ;

	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class A>
using HAS_CLONE = typename HAS_CLONE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_EQUAL_HELP ;

template <class A>
trait HAS_EQUAL_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait HAS_EQUAL_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (&A::equal)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_EQUAL = typename HAS_EQUAL_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_COMPR_HELP ;

template <class A>
trait HAS_COMPR_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait HAS_COMPR_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (&A::compr)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_COMPR = typename HAS_COMPR_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_VISIT_HELP ;

template <class A>
trait HAS_VISIT_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait HAS_VISIT_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (&A::visit)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_VISIT = typename HAS_VISIT_HELP<A ,ALWAYS>::RET ;

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
	ENUM_EQUAL<SIZE ,SIZE_OF<QUAD>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<QUAD>>>>> {
	using RET = QUAD ;
} ;

template <class A>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<A> ,ALIGN_OF<A> ,ALWAYS>::RET ;

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

template <class A>
using TEXT_BASE = typename TEXT_BASE_HELP<SIZE_OF<A> ,ALIGN_OF<A> ,ALWAYS>::RET ;

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
trait STORAGE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_EQUAL<ALIGN ,ALIGN_OF<QUAD>>>> {
	using R1X = QUAD ;
	using R2X = ENUM_DIV<ENUM_ALIGN<SIZE ,ALIGN> ,ALIGN> ;
	require (ENUM_GT_ZERO<R2X>) ;
	using RET = ARR<R1X ,R2X> ;
} ;

template <class SIZE ,class ALIGN = RANK1>
using Storage = typename STORAGE_HELP<SIZE ,ALIGN ,ALWAYS>::RET ;

template <class A>
struct TEMP {
	Storage<SIZE_OF<A> ,ALIGN_OF<A>> mUnused ;
} ;

template <class...>
trait REFLECT_TEMP_HELP ;

template <class A>
trait REFLECT_TEMP_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_TEMP_HELP<TEMP<A>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_TEMP = typename REFLECT_TEMP_HELP<A>::RET ;
} ;