#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"

namespace CSC {
using Bool = csc_bool_t ;

static constexpr auto TRUE = Bool (true) ;
static constexpr auto FALSE = Bool (false) ;

using Val32 = csc_int32_t ;
using Val64 = csc_int64_t ;

static constexpr auto VAL32_MAX = Val32 (2147483647) ;
static constexpr auto VAL32_MIN = -VAL32_MAX ;
static constexpr auto VAL32_ABS = VAL32_MIN - 1 ;
static constexpr auto VAL64_MAX = Val64 (9223372036854775807) ;
static constexpr auto VAL64_MIN = -VAL64_MAX ;
static constexpr auto VAL64_ABS = VAL64_MIN - 1 ;

#ifdef __CSC_CONFIG_VAL32__
using Val = Val32 ;

static constexpr auto VAL_MAX = VAL32_MAX ;
static constexpr auto VAL_MIN = VAL32_MIN ;
static constexpr auto VAL_ABS = VAL32_ABS ;
#endif

#ifdef __CSC_CONFIG_VAL64__
using Val = Val64 ;

static constexpr auto VAL_MAX = VAL64_MAX ;
static constexpr auto VAL_MIN = VAL64_MIN ;
static constexpr auto VAL_ABS = VAL64_ABS ;
#endif

static constexpr auto ZERO = Val (+0) ;
static constexpr auto IDEN = Val (+1) ;
static constexpr auto NONE = Val (-1) ;
static constexpr auto USED = Val (-2) ;

using Index = Val ;
using Length = Val ;
using Flag = Val ;

template <class A>
using Just = KILL<Val ,A> ;

using Flt32 = csc_float32_t ;
using Flt64 = csc_float64_t ;

static constexpr auto FLT32_MAX = Flt32 (3.402823466E+38) ;
static constexpr auto FLT32_MIN = -FLT32_MAX ;
static constexpr auto FLT32_LOW = Flt32 (1.175494351E-38) ;
static constexpr auto FLT32_EPS = Flt32 (1.192092896E-07) ;
static constexpr auto FLT32_INF = Flt32 (infinity) ;
static constexpr auto FLT64_MAX = Flt64 (1.7976931348623158E+308) ;
static constexpr auto FLT64_MIN = -FLT64_MAX ;
static constexpr auto FLT64_LOW = Flt64 (2.2250738585072014E-308) ;
static constexpr auto FLT64_EPS = Flt64 (2.2204460492503131E-016) ;
static constexpr auto FLT64_INF = Flt64 (infinity) ;

enum class Byte :csc_uint8_t ;
enum class Word :csc_uint16_t ;
enum class Char :csc_uint32_t ;
enum class Quad :csc_uint64_t ;

static constexpr auto BYTE_ENDIAN = Byte (0X1F) ;
static constexpr auto WORD_ENDIAN = Word (0X2F1F) ;
static constexpr auto CHAR_ENDIAN = Char (0X4F3F2F1F) ;
static constexpr auto QUAD_ENDIAN = Quad (0X8F7F6F5F4F3F2F1F) ;

forceinline constexpr Byte operator| (CR<Byte> a ,CR<Byte> b) noexcept {
	return Byte (csc_uint8_t (a) | csc_uint8_t (b)) ;
}

forceinline void operator|= (VR<Byte> a ,CR<Byte> b) noexcept {
	a = a | b ;
}

forceinline constexpr Byte operator& (CR<Byte> a ,CR<Byte> b) noexcept {
	return Byte (csc_uint8_t (a) & csc_uint8_t (b)) ;
}

forceinline void operator&= (VR<Byte> a ,CR<Byte> b) noexcept {
	a = a & b ;
}

forceinline constexpr Byte operator^ (CR<Byte> a ,CR<Byte> b) noexcept {
	return Byte (csc_uint8_t (a) ^ csc_uint8_t (b)) ;
}

forceinline void operator^= (VR<Byte> a ,CR<Byte> b) noexcept {
	a = a ^ b ;
}

forceinline constexpr Byte operator- (CR<Byte> a ,CR<Byte> b) noexcept {
	return Byte (csc_uint8_t (a) & ~csc_uint8_t (b)) ;
}

forceinline void operator-= (VR<Byte> a ,CR<Byte> b) noexcept {
	a = a - b ;
}

forceinline constexpr Byte operator~ (CR<Byte> a) noexcept {
	return Byte (~csc_uint8_t (a)) ;
}

forceinline constexpr Byte operator<< (CR<Byte> a ,CR<Length> b) noexcept {
	return Byte (csc_uint8_t (a) << b) ;
}

forceinline constexpr Byte operator>> (CR<Byte> a ,CR<Length> b) noexcept {
	return Byte (csc_uint8_t (a) >> b) ;
}

forceinline constexpr Word operator| (CR<Word> a ,CR<Word> b) noexcept {
	return Word (csc_uint16_t (a) | csc_uint16_t (b)) ;
}

forceinline void operator|= (VR<Word> a ,CR<Word> b) noexcept {
	a = a | b ;
}

forceinline constexpr Word operator& (CR<Word> a ,CR<Word> b) noexcept {
	return Word (csc_uint16_t (a) & csc_uint16_t (b)) ;
}

forceinline void operator&= (VR<Word> a ,CR<Word> b) noexcept {
	a = a & b ;
}

forceinline constexpr Word operator^ (CR<Word> a ,CR<Word> b) noexcept {
	return Word (csc_uint16_t (a) ^ csc_uint16_t (b)) ;
}

forceinline void operator^= (VR<Word> a ,CR<Word> b) noexcept {
	a = a ^ b ;
}

forceinline constexpr Word operator- (CR<Word> a ,CR<Word> b) noexcept {
	return Word (csc_uint16_t (a) & ~csc_uint16_t (b)) ;
}

forceinline void operator-= (VR<Word> a ,CR<Word> b) noexcept {
	a = a - b ;
}

forceinline constexpr Word operator~ (CR<Word> a) noexcept {
	return Word (~csc_uint16_t (a)) ;
}

forceinline constexpr Word operator<< (CR<Word> a ,CR<Length> b) noexcept {
	return Word (csc_uint16_t (a) << b) ;
}

forceinline constexpr Word operator>> (CR<Word> a ,CR<Length> b) noexcept {
	return Word (csc_uint16_t (a) >> b) ;
}

forceinline constexpr Char operator| (CR<Char> a ,CR<Char> b) noexcept {
	return Char (csc_uint32_t (a) | csc_uint32_t (b)) ;
}

forceinline void operator|= (VR<Char> a ,CR<Char> b) noexcept {
	a = a | b ;
}

forceinline constexpr Char operator& (CR<Char> a ,CR<Char> b) noexcept {
	return Char (csc_uint32_t (a) & csc_uint32_t (b)) ;
}

forceinline void operator&= (VR<Char> a ,CR<Char> b) noexcept {
	a = a & b ;
}

forceinline constexpr Char operator^ (CR<Char> a ,CR<Char> b) noexcept {
	return Char (csc_uint32_t (a) ^ csc_uint32_t (b)) ;
}

forceinline void operator^= (VR<Char> a ,CR<Char> b) noexcept {
	a = a ^ b ;
}

forceinline constexpr Char operator- (CR<Char> a ,CR<Char> b) noexcept {
	return Char (csc_uint32_t (a) & ~csc_uint32_t (b)) ;
}

forceinline void operator-= (VR<Char> a ,CR<Char> b) noexcept {
	a = a - b ;
}

forceinline constexpr Char operator~ (CR<Char> a) noexcept {
	return Char (~csc_uint32_t (a)) ;
}

forceinline constexpr Char operator<< (CR<Char> a ,CR<Length> b) noexcept {
	return Char (csc_uint32_t (a) << b) ;
}

forceinline constexpr Char operator>> (CR<Char> a ,CR<Length> b) noexcept {
	return Char (csc_uint32_t (a) >> b) ;
}

forceinline constexpr Quad operator| (CR<Quad> a ,CR<Quad> b) noexcept {
	return Quad (csc_uint64_t (a) | csc_uint64_t (b)) ;
}

forceinline void operator|= (VR<Quad> a ,CR<Quad> b) noexcept {
	a = a | b ;
}

forceinline constexpr Quad operator& (CR<Quad> a ,CR<Quad> b) noexcept {
	return Quad (csc_uint64_t (a) & csc_uint64_t (b)) ;
}

forceinline void operator&= (VR<Quad> a ,CR<Quad> b) noexcept {
	a = a & b ;
}

forceinline constexpr Quad operator^ (CR<Quad> a ,CR<Quad> b) noexcept {
	return Quad (csc_uint64_t (a) ^ csc_uint64_t (b)) ;
}

forceinline void operator^= (VR<Quad> a ,CR<Quad> b) noexcept {
	a = a ^ b ;
}

forceinline constexpr Quad operator- (CR<Quad> a ,CR<Quad> b) noexcept {
	return Quad (csc_uint64_t (a) & ~csc_uint64_t (b)) ;
}

forceinline void operator-= (VR<Quad> a ,CR<Quad> b) noexcept {
	a = a - b ;
}

forceinline constexpr Quad operator~ (CR<Quad> a) noexcept {
	return Quad (~csc_uint64_t (a)) ;
}

forceinline constexpr Quad operator<< (CR<Quad> a ,CR<Length> b) noexcept {
	return Quad (csc_uint64_t (a) << b) ;
}

forceinline constexpr Quad operator>> (CR<Quad> a ,CR<Length> b) noexcept {
	return Quad (csc_uint64_t (a) >> b) ;
}

using Stra = csc_char_t ;
using Strw = csc_wchar_t ;
using Stru = csc_char8_t ;
using Stru16 = csc_char16_t ;
using Stru32 = csc_char32_t ;

#ifdef __CSC_CONFIG_STRA__
using Str = Stra ;
#endif

#ifdef __CSC_CONFIG_STRW__
using Str = Strw ;
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
	using M1ST_ITEM = A ;
	using M1ST_REST = TYPE<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
trait REFLECT_TYPE_HELP<TYPE<A ,B>> {
	using M1ST_ITEM = A ;
	using M1ST_REST = TYPE<B> ;
	using M2ND_ITEM = B ;
	using M2ND_REST = TYPE<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B ,class C ,class...D>
trait REFLECT_TYPE_HELP<TYPE<A ,B ,C ,D...>> {
	using M1ST_ITEM = A ;
	using M1ST_REST = TYPE<B ,C ,D...> ;
	using M2ND_ITEM = B ;
	using M2ND_REST = TYPE<C ,D...> ;
	using M3RD_ITEM = C ;
	using M3RD_REST = TYPE<D...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_TYPE = typename REFLECT_TYPE_HELP<A>::RET ;

template <class A>
using TYPE_M1ST_ITEM = typename REFLECT_TYPE_HELP<A>::M1ST_ITEM ;

template <class A>
using TYPE_M1ST_REST = typename REFLECT_TYPE_HELP<A>::M1ST_REST ;

template <class A>
using TYPE_M2ND_ITEM = typename REFLECT_TYPE_HELP<A>::M2ND_ITEM ;

template <class A>
using TYPE_M2ND_REST = typename REFLECT_TYPE_HELP<A>::M2ND_REST ;

template <class A>
using TYPE_M3RD_ITEM = typename REFLECT_TYPE_HELP<A>::M3RD_ITEM ;

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
	using RET = ENUM<(!A::expr)> ;
} ;

template <class A>
using ENUM_NOT = typename ENUM_NOT_HELP<A ,ALWAYS>::RET ;

template <class A ,class B>
using IS_NOT_SAME = ENUM_NOT<IS_SAME<A ,B>> ;

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

template <class...>
trait ENUM_COMPR_HELP ;

template <class A ,class B>
trait ENUM_COMPR_HELP<A ,B ,ALWAYS> {
	using R1X = ENUM<(A::expr < B::expr)> ;
	using R2X = ENUM<(A::expr > B::expr)> ;
	using R3X = CONDITIONAL<R1X ,ENUM<(-1)> ,CONDITIONAL<R2X ,ENUM<(+1)> ,ENUM<(+0)>>> ;
	using RET = ENUM<(R3X::expr)> ;
} ;

template <class A ,class B>
using ENUM_COMPR = typename ENUM_COMPR_HELP<A ,B ,ALWAYS>::RET ;

template <class A ,class B>
using ENUM_COMPR_LT = IS_SAME<ENUM_COMPR<A ,B> ,ENUM<(-1)>> ;

template <class A ,class B>
using ENUM_COMPR_GT = IS_SAME<ENUM_COMPR<A ,B> ,ENUM<(+1)>> ;

template <class A ,class B>
using ENUM_COMPR_LTEQ = ENUM_NOT<ENUM_COMPR_GT<A ,B>> ;

template <class A ,class B>
using ENUM_COMPR_GTEQ = ENUM_NOT<ENUM_COMPR_LT<A ,B>> ;

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
	using R1X = TYPE_M1ST_ITEM<A> ;
	using RET = TYPE<R1X> ;
} ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_GT_IDEN<RANK_OF<A>>>> {
	using R1X = TYPE<TYPE_M1ST_ITEM<A>> ;
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
	using RET = TYPE_M1ST_ITEM<A> ;
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
	using R1X = TYPE_M1ST_ITEM<A> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = CONDITIONAL<R1X ,R2X ,ENUM_FALSE> ;
	using RET = ENUM<R3X::expr> ;
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
	using R1X = TYPE_M1ST_ITEM<A> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = CONDITIONAL<R1X ,ENUM_TRUE ,R2X> ;
	using RET = ENUM<R3X::expr> ;
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
	using R1X = TYPE_M1ST_ITEM<A> ;
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
	using R1X = TYPE_M1ST_ITEM<A> ;
	using R2X = typename ENUM_MAX_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = CONDITIONAL<ENUM_COMPR_GTEQ<R1X ,R2X> ,R1X ,R2X> ;
	using RET = ENUM<R3X::expr> ;
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
	using R1X = TYPE_M1ST_ITEM<A> ;
	using R2X = typename ENUM_MIN_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = CONDITIONAL<ENUM_COMPR_LTEQ<R1X ,R2X> ,R1X ,R2X> ;
	using RET = ENUM<R3X::expr> ;
} ;

template <class...A>
using ENUM_MIN = typename ENUM_MIN_HELP<TYPE<A...> ,ALWAYS>::RET ;

struct Proxy {
	implicit Proxy () = default ;
} ;

struct Interface {
	implicit Interface () = default ;
	virtual ~Interface () = default ;
	implicit Interface (CR<Interface> that) = delete ;
	forceinline VR<Interface> operator= (CR<Interface> that) = delete ;
	implicit Interface (RR<Interface> that) = delete ;
	forceinline VR<Interface> operator= (RR<Interface> that) = delete ;
} ;

class Unknown ;

template <class...>
trait PLACEHOLDER_HELP ;

struct PlaceHolderWrap0 {} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	using RET = PlaceHolderWrap0 ;
} ;

template <class A>
struct PlaceHolderWrap1 implement A {} ;

template <class RANK>
trait PLACEHOLDER_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using R1X = typename PLACEHOLDER_HELP<ENUM_DEC<RANK> ,ALWAYS>::RET ;
	using RET = PlaceHolderWrap1<R1X> ;
} ;

template <class RANK>
using PlaceHolder = typename PLACEHOLDER_HELP<RANK ,ALWAYS>::RET ;

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
using IS_BOOL = IS_SAME<A ,Bool> ;

template <class...>
trait IS_VALUE_HELP ;

template <class A>
trait IS_VALUE_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,Val32> ;
	using R2X = IS_SAME<A ,Val64> ;
	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A>
using IS_VALUE = typename IS_VALUE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_FLOAT_HELP ;

template <class A>
trait IS_FLOAT_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,Flt32> ;
	using R2X = IS_SAME<A ,Flt64> ;
	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class A>
using IS_FLOAT = typename IS_FLOAT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_TEXT_HELP ;

template <class A>
trait IS_TEXT_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,Stra> ;
	using R2X = IS_SAME<A ,Strw> ;
	using R3X = IS_SAME<A ,Stru> ;
	using R4X = IS_SAME<A ,Stru16> ;
	using R5X = IS_SAME<A ,Stru32> ;
	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X> ;
} ;

template <class A>
using IS_TEXT = typename IS_TEXT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_BYTE_HELP ;

template <class A>
trait IS_BYTE_HELP<A ,ALWAYS> {
	using R1X = IS_SAME<A ,Byte> ;
	using R2X = IS_SAME<A ,Word> ;
	using R3X = IS_SAME<A ,Char> ;
	using R4X = IS_SAME<A ,Quad> ;
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

using ORDINARY = RANK0 ;
using VARIABLE = RANK1 ;
using CONSTANT = RANK2 ;
using REGISTER = RANK3 ;

template <class...>
trait REFLECT_REF_HELP ;

template <class A>
trait REFLECT_REF_HELP<A ,A> {
	using RET = ORDINARY ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,VR<A>> {
	using RET = VARIABLE ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,CR<A>> {
	using RET = CONSTANT ;
} ;

template <class A>
trait REFLECT_REF_HELP<A ,RR<A>> {
	using RET = REGISTER ;
} ;

template <class A>
using REFLECT_REF = typename REFLECT_REF_HELP<REMOVE_CVR<A> ,A>::RET ;

template <class...>
trait REFLECT_POINTER_HELP ;

template <class A>
trait REFLECT_POINTER_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_POINTER_HELP<DEF<A *>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait REFLECT_POINTER_HELP<DEF<const A *>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_POINTER = typename REFLECT_POINTER_HELP<REMOVE_CVR<A>>::RET ;

template <class...>
trait PTR_HELP ;

template <class A ,class B>
trait PTR_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	using RET = DEF<A *> ;
} ;

template <class A ,class B>
trait PTR_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	using RET = DEF<const A *> ;
} ;

template <class A>
using PTR = typename PTR_HELP<REMOVE_CVR<A> ,REFLECT_REF<A> ,ALWAYS>::RET ;

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
using IS_ARRAY = typename REFLECT_ARRAY_HELP<REMOVE_CVR<A>>::RET ;

template <class A>
using ARRAY_ITEM = typename REFLECT_ARRAY_HELP<REMOVE_CVR<A>>::ITEM ;

template <class A>
using ARRAY_SIZE = typename REFLECT_ARRAY_HELP<REMOVE_CVR<A>>::SIZE ;

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
trait REFLECT_INVOKE_HELP ;

template <class A ,class OTHERWISE>
trait REFLECT_INVOKE_HELP<A ,OTHERWISE> {
	using RET = A ;
} ;

template <class A>
trait REFLECT_INVOKE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (&A::operator())>>> {
	using RET = typeof (&A::operator()) ;
} ;

template <class A>
using REFLECT_INVOKE = typename REFLECT_INVOKE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class A>
trait REFLECT_FUNCTION_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C:: *) (B...)>> {
	using RETURN = A ;
	using PARAMS = TYPE<B...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C:: *) (B...) const>> {
	using RETURN = A ;
	using PARAMS = TYPE<B...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_FUNCTION = typename REFLECT_FUNCTION_HELP<REFLECT_INVOKE<A>>::RET ;

template <class A>
using FUNCTION_RETURN = typename REFLECT_FUNCTION_HELP<REFLECT_INVOKE<A>>::RETURN ;

template <class A>
using FUNCTION_PARAMS = typename REFLECT_FUNCTION_HELP<REFLECT_INVOKE<A>>::PARAMS ;

template <class...>
trait IS_UNDER_HELP ;

template <class A>
trait IS_UNDER_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait IS_UNDER_HELP<A ,REQUIRE<ENUM_NOT<IS_BASIC<A>>>> {
	using R1X = MACRO_IS_UNDER1<A> ;
	using R2X = MACRO_IS_UNDER2<A> ;
	using R3X = IS_SAME<A ,csc_uint8_t> ;
	using R4X = IS_SAME<A ,csc_uint16_t> ;
	using R5X = IS_SAME<A ,csc_uint32_t> ;
	using R6X = IS_SAME<A ,csc_uint64_t> ;
	using R7X = IS_SAME<A ,csc_diff_t> ;
	using R8X = IS_SAME<A ,csc_size_t> ;
	using R9X = IS_SAME<A ,csc_enum_t> ;
	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X ,R7X ,R8X ,R9X> ;
} ;

template <class A>
using IS_UNDER = typename IS_UNDER_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_CLASS_HELP ;

template <class A>
trait IS_CLASS_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_CLASS<A> ;
	using R3X = ENUM_NOT<IS_ENUM<A>> ;
	using R4X = ENUM_NOT<IS_TYPE<A>> ;
	using R5X = ENUM_NOT<IS_BASIC<A>> ;
	using R6X = ENUM_NOT<IS_UNDER<A>> ;
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
trait IS_CLONEABLE_HELP ;

template <class A>
trait IS_CLONEABLE_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_COPY_CONSTRUCTIBLE<A> ;
	using R2X = MACRO_IS_COPY_ASSIGNABLE<A> ;
	using R3X = MACRO_IS_MOVE_CONSTRUCTIBLE<A> ;
	using R4X = MACRO_IS_MOVE_ASSIGNABLE<A> ;
	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class A>
using IS_CLONEABLE = typename IS_CLONEABLE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_EQUALABLE_HELP ;

template <class A ,class OTHERWISE>
trait IS_EQUALABLE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait IS_EQUALABLE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A) == nullof (A))>>> {
	using RET = ENUM_NOT<IS_CLASS<A>> ;
} ;

template <class A>
using IS_EQUALABLE = typename IS_EQUALABLE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_COMPRABLE_HELP ;

template <class A ,class OTHERWISE>
trait IS_COMPRABLE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait IS_COMPRABLE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A) < nullof (A))>>> {
	using RET = ENUM_NOT<IS_CLASS<A>> ;
} ;

template <class A>
using IS_COMPRABLE = typename IS_COMPRABLE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_CLONE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_CLONE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_CLONE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).clone ())>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_CLONE = typename HAS_CLONE_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_EQUAL_HELP ;

template <class A ,class OTHERWISE>
trait HAS_EQUAL_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_EQUAL_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).equal (nullof (A)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_EQUAL = typename HAS_EQUAL_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_COMPR_HELP ;

template <class A ,class OTHERWISE>
trait HAS_COMPR_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_COMPR_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).compr (nullof (A)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_COMPR = typename HAS_COMPR_HELP<A ,ALWAYS>::RET ;

class Visitor ;

template <class...>
trait HAS_VISIT_HELP ;

template <class A ,class OTHERWISE>
trait HAS_VISIT_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_VISIT_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).visit (nullof (Visitor)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_VISIT = typename HAS_VISIT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_SCOPE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_SCOPE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_SCOPE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,TYPE<typeof (nullof (A).enter ()) ,typeof (nullof (A).leave ())>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_SCOPE = typename HAS_SCOPE_HELP<A ,ALWAYS>::RET ;

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
trait IS_RELATE_HELP ;

template <class A ,class B ,class OTHERWISE>
trait IS_RELATE_HELP<A ,B ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class B>
trait IS_RELATE_HELP<A ,B ,REQUIRE<KILL<ENUM_TRUE ,typeof (static_cast<A> (nullof (B)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
using IS_RELATE = typename IS_RELATE_HELP<XR<A> ,XR<B> ,ALWAYS>::RET ;

template <class...>
trait IS_VIRTUAL_HELP ;

template <class A ,class B ,class OTHERWISE>
trait IS_VIRTUAL_HELP<A ,B ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class B>
trait IS_VIRTUAL_HELP<A ,B ,REQUIRE<IS_SAME<A ,typeof (nullof (B).ref)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
using IS_VIRTUAL = typename IS_VIRTUAL_HELP<A ,B ,ALWAYS>::RET ;

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
trait IS_OBJECT_HELP ;

template <class A>
trait IS_OBJECT_HELP<A ,ALWAYS> {
	using R1X = IS_BASIC<A> ;
	using R2X = IS_UNDER<A> ;
	using R3X = IS_CLASS<A> ;
	using RET = ENUM_ANY<R1X ,R2X ,R3X> ;
} ;

template <class A>
using IS_OBJECT = typename IS_OBJECT_HELP<A ,ALWAYS>::RET ;

template <class...>
trait BYTE_BASE_HELP ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<Byte>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<Byte>>>>> {
	using RET = Byte ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<Word>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<Word>>>>> {
	using RET = Word ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<Char>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<Char>>>>> {
	using RET = Char ;
} ;

template <class SIZE ,class ALIGN>
trait BYTE_BASE_HELP<SIZE ,ALIGN ,REQUIRE<ENUM_ALL<
	ENUM_EQUAL<SIZE ,SIZE_OF<Quad>> ,
	ENUM_EQUAL<ALIGN ,ALIGN_OF<Quad>>>>> {
	using RET = Quad ;
} ;

template <class A>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<A> ,ALIGN_OF<A> ,ALWAYS>::RET ;

template <class SIZE ,class ALIGN = RANK1>
struct Storage {
	alignas (ALIGN::expr) ARR<Byte ,SIZE> mUnused ;
} ;

template <class A>
struct Union implement Storage<SIZE_OF<A> ,ALIGN_OF<A>> {} ;

template <class...>
trait TUPLE_HELP ;

struct TupleWrap0 {} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<PARAMS>>>> {
	using RET = TupleWrap0 ;
} ;

template <class A>
struct TupleWrap1 {
	A m1st ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK1>>> {
	using R1X = TYPE_M1ST_ITEM<PARAMS> ;
	using RET = TupleWrap1<R1X> ;
} ;

template <class A ,class B>
struct TupleWrap2 {
	A m1st ;
	B m2nd ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK2>>> {
	using R1X = TYPE_M1ST_ITEM<PARAMS> ;
	using R2X = TYPE_M2ND_ITEM<PARAMS> ;
	using RET = TupleWrap2<R1X ,R2X> ;
} ;

template <class A ,class B ,class C>
struct TupleWrap3 {
	A m1st ;
	B m2nd ;
	C m3rd ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK3>>> {
	using R1X = TYPE_M1ST_ITEM<PARAMS> ;
	using R2X = TYPE_M2ND_ITEM<PARAMS> ;
	using R3X = TYPE_M3RD_ITEM<PARAMS> ;
	using RET = TupleWrap3<R1X ,R2X ,R3X> ;
} ;

template <class...A>
using Tuple = typename TUPLE_HELP<TYPE<A...> ,ALWAYS>::RET ;

template <class...>
trait HAS_M1ST_HELP ;

template <class A ,class OTHERWISE>
trait HAS_M1ST_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_M1ST_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).m1st)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_M1ST = typename HAS_M1ST_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_M2ND_HELP ;

template <class A ,class OTHERWISE>
trait HAS_M2ND_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_M2ND_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).m2nd)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_M2ND = typename HAS_M2ND_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_M3RD_HELP ;

template <class A ,class OTHERWISE>
trait HAS_M3RD_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_M3RD_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).m3rd)>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_M3RD = typename HAS_M3RD_HELP<A ,ALWAYS>::RET ;

template <class...>
trait HAS_COMPILE_HELP ;

template <class A ,class B ,class OTHERWISE>
trait HAS_COMPILE_HELP<A ,B ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class B>
trait HAS_COMPILE_HELP<A ,B ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).compile (nullof (B)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
using HAS_COMPILE = typename HAS_COMPILE_HELP<A ,B ,ALWAYS>::RET ;

class Pointer implement Proxy {
public:
	implicit Pointer () = delete ;

	implicit ~Pointer () = delete ;

	implicit Pointer (CR<Pointer> that) = delete ;

	forceinline VR<Pointer> operator= (CR<Pointer> that) = delete ;

	implicit Pointer (RR<Pointer> that) = delete ;

	forceinline VR<Pointer> operator= (RR<Pointer> that) = delete ;

	static VR<Pointer> make (CR<Flag> that) noexcept {
		return *(reinterpret_cast<PTR<VR<Pointer>>> (that)) ;
	}

	template <class ARG1>
	static VR<Pointer> from (VR<ARG1> that) noexcept {
		return reinterpret_cast<VR<Pointer>> (that) ;
	}

	template <class ARG1>
	static CR<Pointer> from (CR<ARG1> that) noexcept {
		return reinterpret_cast<CR<Pointer>> (that) ;
	}

	template <class ARG1>
	static RR<Pointer> from (RR<ARG1> that) noexcept = delete ;

	template <class ARG1>
	forceinline operator VR<ARG1> () leftvalue noexcept {
		return reinterpret_cast<VR<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator CR<ARG1> () leftvalue noexcept {
		return reinterpret_cast<CR<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () leftvalue noexcept = delete ;

	template <class ARG1>
	forceinline operator CR<ARG1> () const leftvalue noexcept {
		return reinterpret_cast<CR<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () const leftvalue noexcept = delete ;
} ;

struct FatLayout {
	Flag mHolder ;
	Flag mLayout ;
} ;

template <class A ,class B>
class Fat implement A {
private:
	require (IS_INTERFACE<A>) ;

protected:
	Flag mLayout ;

public:
	template <class ARG1 = B>
	VR<ARG1> self_m () leftvalue {
		return Pointer::make (mLayout) ;
	}

	template <class ARG1 = B>
	CR<ARG1> self_m () const leftvalue {
		return Pointer::make (mLayout) ;
	}
} ;

template <class A>
class VFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit VFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit VFat (CR<ARG1> holder ,VR<ARG2> that) ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<ARG1 ,A>>>
	implicit operator CR<VFat<ARG1>> () const leftvalue {
		return Pointer::from (thiz) ;
	}

	VR<A> ref_m () const {
		return Pointer::from (const_cast<VR<VFat>> (thiz)) ;
	}

	forceinline PTR<VR<A>> operator-> () const {
		return (&ref) ;
	}
} ;

template <class A>
class CFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit CFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit CFat (CR<ARG1> holder ,CR<ARG2> that) ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<ARG1 ,A>>>
	implicit operator CR<CFat<ARG1>> () const leftvalue {
		return Pointer::from (thiz) ;
	}

	CR<A> ref_m () const {
		return Pointer::from (const_cast<CR<CFat>> (thiz)) ;
	}

	forceinline PTR<CR<A>> operator-> () const {
		return (&ref) ;
	}
} ;

template <class A>
class RFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit RFat () = delete ;

	explicit RFat (CR<Unknown> unknown) ;

	VR<A> ref_m () const {
		return Pointer::from (const_cast<VR<RFat>> (thiz)) ;
	}

	forceinline PTR<VR<A>> operator-> () const {
		return (&ref) ;
	}
} ;
} ;