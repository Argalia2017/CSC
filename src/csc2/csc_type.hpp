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

using FLT32 = float ;
using FLT64 = double ;

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
enum class DATA :csc_uint64_t ;

inline constexpr BYTE operator| (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_uint8_t (obj1) | csc_uint8_t (obj2)) ;
}

inline void operator|= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

inline constexpr BYTE operator& (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_uint8_t (obj1) & csc_uint8_t (obj2)) ;
}

inline void operator&= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

inline constexpr BYTE operator^ (CREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	return BYTE (csc_uint8_t (obj1) ^ csc_uint8_t (obj2)) ;
}

inline void operator^= (VREF<BYTE> obj1 ,CREF<BYTE> obj2) noexcept {
	obj1 = obj1 ^ obj2 ;
}

inline constexpr BYTE operator~ (CREF<BYTE> obj1) noexcept {
	return BYTE (~csc_uint8_t (obj1)) ;
}

inline constexpr BYTE operator<< (CREF<BYTE> obj1 ,CREF<LENGTH> obj2) noexcept {
	return BYTE (csc_uint8_t (obj1) << obj2) ;
}

inline constexpr BYTE operator>> (CREF<BYTE> obj1 ,CREF<LENGTH> obj2) noexcept {
	return BYTE (csc_uint8_t (obj1) >> obj2) ;
}

inline constexpr WORD operator| (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_uint16_t (obj1) | csc_uint16_t (obj2)) ;
}

inline void operator|= (VREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

inline constexpr WORD operator& (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_uint16_t (obj1) & csc_uint16_t (obj2)) ;
}

inline void operator&= (VREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

inline constexpr WORD operator^ (CREF<WORD> obj1 ,CREF<WORD> obj2) noexcept {
	return WORD (csc_uint16_t (obj1) ^ csc_uint16_t (obj2)) ;
}

inline constexpr WORD operator~ (CREF<WORD> obj1) noexcept {
	return WORD (~csc_uint16_t (obj1)) ;
}

inline constexpr WORD operator<< (CREF<WORD> obj1 ,CREF<LENGTH> obj2) noexcept {
	return WORD (csc_uint16_t (obj1) << obj2) ;
}

inline constexpr WORD operator>> (CREF<WORD> obj1 ,CREF<LENGTH> obj2) noexcept {
	return WORD (csc_uint16_t (obj1) >> obj2) ;
}

inline constexpr CHAR operator| (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_uint32_t (obj1) | csc_uint32_t (obj2)) ;
}

inline void operator|= (VREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

inline constexpr CHAR operator& (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_uint32_t (obj1) & csc_uint32_t (obj2)) ;
}

inline void operator&= (VREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

inline constexpr CHAR operator^ (CREF<CHAR> obj1 ,CREF<CHAR> obj2) noexcept {
	return CHAR (csc_uint32_t (obj1) ^ csc_uint32_t (obj2)) ;
}

inline constexpr CHAR operator~ (CREF<CHAR> obj1) noexcept {
	return CHAR (~csc_uint32_t (obj1)) ;
}

inline constexpr CHAR operator<< (CREF<CHAR> obj1 ,CREF<LENGTH> obj2) noexcept {
	return CHAR (csc_uint32_t (obj1) << obj2) ;
}

inline constexpr CHAR operator>> (CREF<CHAR> obj1 ,CREF<LENGTH> obj2) noexcept {
	return CHAR (csc_uint32_t (obj1) >> obj2) ;
}

inline constexpr DATA operator| (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_uint64_t (obj1) | csc_uint64_t (obj2)) ;
}

inline void operator|= (VREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	obj1 = obj1 | obj2 ;
}

inline constexpr DATA operator& (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_uint64_t (obj1) & csc_uint64_t (obj2)) ;
}

inline void operator&= (VREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	obj1 = obj1 & obj2 ;
}

inline constexpr DATA operator^ (CREF<DATA> obj1 ,CREF<DATA> obj2) noexcept {
	return DATA (csc_uint64_t (obj1) ^ csc_uint64_t (obj2)) ;
}

inline constexpr DATA operator~ (CREF<DATA> obj1) noexcept {
	return DATA (~csc_uint64_t (obj1)) ;
}

inline constexpr DATA operator<< (CREF<DATA> obj1 ,CREF<LENGTH> obj2) noexcept {
	return DATA (csc_uint64_t (obj1) << obj2) ;
}

inline constexpr DATA operator>> (CREF<DATA> obj1 ,CREF<LENGTH> obj2) noexcept {
	return DATA (csc_uint64_t (obj1) >> obj2) ;
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
	using RET = ENUMAS<(sizeof (A))> ;
} ;

template <class A>
using SIZE_OF = typename SIZE_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait ALIGN_OF_HELP ;

template <class A>
trait ALIGN_OF_HELP<A ,ALWAYS> {
	using RET = ENUMAS<(alignof (A))> ;
} ;

template <class A>
using ALIGN_OF = typename ALIGN_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait COUNT_OF_HELP ;

template <class...A>
trait COUNT_OF_HELP<TYPEAS<A...> ,ALWAYS> {
	using RET = ENUMAS<(sizeof... (A))> ;
} ;

template <class A>
using COUNT_OF = typename COUNT_OF_HELP<A ,ALWAYS>::RET ;

template <class...>
trait REFLECT_ENUM_HELP ;

template <class A>
trait REFLECT_ENUM_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <csc_diff_t A>
trait REFLECT_ENUM_HELP<ENUMAS<A>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_ENUM = typename REFLECT_ENUM_HELP<A>::RET ;

using ENUM_ZERO = ENUMAS<ZERO> ;
using ENUM_IDEN = ENUMAS<IDEN> ;
using ENUM_NONE = ENUMAS<NONE> ;
using ENUM_USED = ENUMAS<USED> ;

template <class...>
trait ENUM_NOT_HELP ;

template <class A>
trait ENUM_NOT_HELP<A ,ALWAYS> {
	using RET = ENUMAS<(ifnot (A::expr))> ;
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
	using RET = ENUMAS<(A::expr == B::expr)> ;
} ;

template <class A ,class B>
using ENUM_EQUAL = typename ENUM_EQUAL_HELP<A ,B ,ALWAYS>::RET ;

template <class A ,class B>
using ENUM_NOT_EQUAL = ENUM_NOT<ENUM_EQUAL<A ,B>> ;

template <class...>
trait ENUM_COMPR_LT_HELP ;

template <class A ,class B>
trait ENUM_COMPR_LT_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr < B::expr)> ;
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
	using R3X = CONDITIONAL<R1X ,ENUM_NONE ,CONDITIONAL<R2X ,ENUM_IDEN ,ENUM_ZERO>> ;
	using RET = ENUMAS<(R3X::expr)> ;
} ;

template <class A ,class B>
using ENUM_COMPR = typename ENUM_COMPR_HELP<A ,B ,ALWAYS>::RET ;

template <class A>
using ENUM_EQ_ZERO = ENUM_EQUAL<A ,ENUM_ZERO> ;

template <class A>
using ENUM_GT_ZERO = ENUM_COMPR_GT<A ,ENUM_ZERO> ;

template <class A>
using ENUM_EQ_IDEN = ENUM_EQUAL<A ,ENUM_IDEN> ;

template <class A>
using ENUM_GT_IDEN = ENUM_COMPR_GT<A ,ENUM_IDEN> ;

template <class...>
trait ENUM_ADD_HELP ;

template <class A ,class B>
trait ENUM_ADD_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr + B::expr)> ;
} ;

template <class A ,class B>
using ENUM_ADD = typename ENUM_ADD_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUB_HELP ;

template <class A ,class B>
trait ENUM_SUB_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr - B::expr)> ;
} ;

template <class A ,class B>
using ENUM_SUB = typename ENUM_SUB_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_MUL_HELP ;

template <class A ,class B>
trait ENUM_MUL_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr * B::expr)> ;
} ;

template <class A ,class B>
using ENUM_MUL = typename ENUM_MUL_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_DIV_HELP ;

template <class A ,class B>
trait ENUM_DIV_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr / B::expr)> ;
} ;

template <class A ,class B>
using ENUM_DIV = typename ENUM_DIV_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_MOD_HELP ;

template <class A ,class B>
trait ENUM_MOD_HELP<A ,B ,ALWAYS> {
	using RET = ENUMAS<(A::expr % B::expr)> ;
} ;

template <class A ,class B>
using ENUM_MOD = typename ENUM_MOD_HELP<A ,B ,ALWAYS>::RET ;

template <class A>
using ENUM_MINUS = ENUM_SUB<ENUM_ZERO ,A> ;

template <class A>
using ENUM_INC = ENUM_ADD<A ,ENUM_IDEN> ;

template <class A>
using ENUM_DEC = ENUM_SUB<A ,ENUM_IDEN> ;

template <class...>
trait REFLECT_TYPE_HELP ;

template <class A>
trait REFLECT_TYPE_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_TYPE_HELP<TYPEAS<A>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPEAS<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B>
trait REFLECT_TYPE_HELP<TYPEAS<A ,B>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPEAS<B> ;
	using M2ND_ONE = B ;
	using M2ND_REST = TYPEAS<> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class B ,class C ,class...D>
trait REFLECT_TYPE_HELP<TYPEAS<A ,B ,C ,D...>> {
	using M1ST_ONE = A ;
	using M1ST_REST = TYPEAS<B ,C ,D...> ;
	using M2ND_ONE = B ;
	using M2ND_REST = TYPEAS<C ,D...> ;
	using M3RD_ONE = C ;
	using M3RD_REST = TYPEAS<D...> ;
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

template <class...>
trait TYPE_CAT_HELP ;

template <class...A ,class...B>
trait TYPE_CAT_HELP<TYPEAS<A...> ,TYPEAS<B...> ,ALWAYS> {
	using RET = TYPEAS<A... ,B...> ;
} ;

template <class A ,class B>
using TYPE_CAT = typename TYPE_CAT_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait TYPE_REVERSE_HELP ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<A>>>> {
	using RET = TYPEAS<> ;
} ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using RET = TYPEAS<R1X> ;
} ;

template <class A>
trait TYPE_REVERSE_HELP<A ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<A>>>> {
	using R1X = TYPEAS<TYPE_M1ST_ONE<A>> ;
	using R2X = typename TYPE_REVERSE_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = TYPE_CAT<R2X ,R1X> ;
} ;

template <class A>
using TYPE_REVERSE = typename TYPE_REVERSE_HELP<A ,ALWAYS>::RET ;

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

template <class A ,class B>
trait TYPE_PICK_HELP<A ,B ,REQUIRE<ENUM_EQ_ZERO<B>>> {
	using RET = TYPE_M1ST_ONE<A> ;
} ;

template <class A ,class B>
trait TYPE_PICK_HELP<A ,B ,REQUIRE<ENUM_GT_ZERO<B>>> {
	using RET = typename TYPE_PICK_HELP<TYPE_M1ST_REST<A> ,ENUM_DEC<B> ,ALWAYS>::RET ;
} ;

template <class A ,class B>
using TYPE_PICK = typename TYPE_PICK_HELP<A ,B ,ALWAYS>::RET ;

template <class...>
trait ENUM_ALL_HELP ;

template <class A>
trait ENUM_ALL_HELP<A ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<A>>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
trait ENUM_ALL_HELP<A ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,R2X ,ENUM_FALSE> ;
} ;

template <class...A>
using ENUM_ALL = typename ENUM_ALL_HELP<TYPEAS<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_ANY_HELP ;

template <class A>
trait ENUM_ANY_HELP<A ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<A>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait ENUM_ANY_HELP<A ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_ANY_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_TRUE ,R2X> ;
} ;

template <class...A>
using ENUM_ANY = typename ENUM_ANY_HELP<TYPEAS<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_SUM_HELP ;

template <class A>
trait ENUM_SUM_HELP<A ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<A>>>> {
	using RET = ENUM_ZERO ;
} ;

template <class A>
trait ENUM_SUM_HELP<A ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_SUM_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using RET = ENUM_ADD<R1X ,R2X> ;
} ;

template <class...A>
using ENUM_SUM = typename ENUM_SUM_HELP<TYPEAS<A...> ,ALWAYS>::RET ;

template <class BASE ,class ALIGN>
using ENUM_ALIGN = ENUM_MUL<ENUM_DIV<ENUM_ADD<BASE ,ENUM_DEC<ALIGN>> ,ALIGN> ,ALIGN> ;

template <class A>
using ENUM_ABS = CONDITIONAL<ENUM_COMPR_GTEQ<A ,ENUM_ZERO> ,A ,ENUM_MINUS<A>> ;

template <class...>
trait ENUM_MIN_HELP ;

template <class A>
trait ENUM_MIN_HELP<A ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<A>>>> {
	using RET = TYPE_M1ST_ONE<A> ;
} ;

template <class A>
trait ENUM_MIN_HELP<A ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_MIN_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_LTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...A>
using ENUM_MIN = typename ENUM_MIN_HELP<TYPEAS<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_MAX_HELP ;

template <class A>
trait ENUM_MAX_HELP<A ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<A>>>> {
	using RET = TYPE_M1ST_ONE<A> ;
} ;

template <class A>
trait ENUM_MAX_HELP<A ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<A>>>> {
	using R1X = TYPE_M1ST_ONE<A> ;
	using R2X = typename ENUM_MAX_HELP<TYPE_M1ST_REST<A> ,ALWAYS>::RET ;
	using R3X = ENUM_COMPR_GTEQ<R1X ,R2X> ;
	using RET = CONDITIONAL<R3X ,R1X ,R2X> ;
} ;

template <class...A>
using ENUM_MAX = typename ENUM_MAX_HELP<TYPEAS<A...> ,ALWAYS>::RET ;

template <class...>
trait ENUM_BETWEEN_HELP ;

template <class CURR ,class BEGIN ,class END>
trait ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS> {
	using R1X = ENUM_COMPR_GTEQ<CURR ,BEGIN> ;
	using R2X = ENUM_COMPR_LT<CURR ,END> ;
	using RET = ENUM_ALL<R1X ,R2X> ;
} ;

template <class CURR ,class BEGIN ,class END>
using ENUM_BETWEEN = typename ENUM_BETWEEN_HELP<CURR ,BEGIN ,END ,ALWAYS>::RET ;

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
	implicit Interface (Interface &&) = default ;
	inline VREF<Interface> operator= (Interface &&) = default ;
} ;

template <class...>
trait PLACEHOLDER_HELP ;

template <class A>
trait PLACEHOLDER_HELP<A ,REQUIRE<ENUM_EQ_ZERO<A>>> {
	struct PlaceHolder {} ;
} ;

template <class A>
trait PLACEHOLDER_HELP<A ,REQUIRE<ENUM_GT_ZERO<A>>> {
	using Holder = typename PLACEHOLDER_HELP<ENUM_DEC<A> ,ALWAYS>::PlaceHolder ;

	struct PlaceHolder implement Holder {} ;
} ;

template <class A>
using PlaceHolder = typename PLACEHOLDER_HELP<A ,ALWAYS>::PlaceHolder ;

using RANK0 = ENUMAS<(+0)> ;
using RANK1 = ENUMAS<(+1)> ;
using RANK2 = ENUMAS<(+2)> ;
using RANK3 = ENUMAS<(+3)> ;
using RANK4 = ENUMAS<(+4)> ;
using RANK5 = ENUMAS<(+5)> ;
using RANK6 = ENUMAS<(+6)> ;
using RANK7 = ENUMAS<(+7)> ;
using RANK8 = ENUMAS<(+8)> ;
using RANK9 = ENUMAS<(+9)> ;
using RANKX = ENUMAS<10> ;

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
	using R4X = IS_SAME<A ,DATA> ;
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

using ORDINARY = ENUMAS<(+0)> ;
using VARIABLE = ENUMAS<(-1)> ;
using CONSTANT = ENUMAS<(-2)> ;
using REGISTER = ENUMAS<(-3)> ;

template <class...>
trait REFLECT_REF_HELP ;

template <class A>
trait REFLECT_REF_HELP<A ,A> {
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

template <class A>
using IS_ORDINARY = IS_SAME<REFLECT_REF<A> ,ORDINARY> ;

template <class A>
using IS_VARIABLE = IS_SAME<REFLECT_REF<A> ,VARIABLE> ;

template <class A>
using IS_CONSTANT = IS_SAME<REFLECT_REF<A> ,CONSTANT> ;

template <class A>
using IS_REGISTER = IS_SAME<REFLECT_REF<A> ,REGISTER> ;

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
using IS_POINTER = typename REFLECT_POINTER_HELP<REMOVE_REF<A>>::RET ;

template <class...>
trait PTR_HELP ;

template <class A ,class B>
trait PTR_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	using RET = DEF<const DEF<A *>> ;
} ;

template <class A ,class B>
trait PTR_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	using RET = DEF<const DEF<const A *>> ;
} ;

template <class A>
using PTR = typename PTR_HELP<REMOVE_REF<A> ,REFLECT_REF<A> ,ALWAYS>::RET ;

template <class...>
trait REFLECT_ARRAY_HELP ;

template <class A>
trait REFLECT_ARRAY_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait REFLECT_ARRAY_HELP<DEF<A[]>> {
	using ITEM = A ;
	using SIZE = ENUM_ZERO ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,csc_diff_t B>
trait REFLECT_ARRAY_HELP<DEF<A[B]>> {
	using ITEM = A ;
	using SIZE = ENUMAS<B> ;
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

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_EQ_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;
	using RET = DEF<ITEM[]> ;
} ;

template <class ITEM ,class SIZE>
trait ARR_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	require (ENUM_NOT<IS_ARRAY<ITEM>>) ;
	using RET = DEF<ITEM[SIZE::expr]> ;
} ;

template <class ITEM ,class SIZE = ENUM_ZERO>
using ARR = typename ARR_HELP<ITEM ,SIZE ,ALWAYS>::RET ;

template <class...>
trait REFLECT_FUNCTION_HELP ;

template <class A>
trait REFLECT_FUNCTION_HELP<A> {
	using RET = ENUM_FALSE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C::*) (B...)>> {
	using RETURN = A ;
	using PARAMS = TYPEAS<XREF<B>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A ,class...B ,class C>
trait REFLECT_FUNCTION_HELP<DEF<A (C::*) (B...) const>> {
	using RETURN = A ;
	using PARAMS = TYPEAS<XREF<B>...> ;
	using RET = ENUM_TRUE ;
} ;

template <class A>
using IS_FUNCTION = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::RET ;

template <class A>
using FUNCTION_RETURN = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::RETURN ;

template <class A>
using FUNCTION_PARAMS = typename REFLECT_FUNCTION_HELP<DEF<typeof (&A::operator())>>::PARAMS ;

template <class...>
trait IS_INTPTR_HELP ;

template <class A>
trait IS_INTPTR_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait IS_INTPTR_HELP<A ,REQUIRE<ENUM_NOT<IS_BASIC<A>>>> {
	using R1X = MACRO_IS_INTPTR<A> ;
	using R2X = IS_POINTER<A> ;
	using R3X = IS_SAME<A ,csc_uint8_t> ;
	using R4X = IS_SAME<A ,csc_uint16_t> ;
	using R5X = IS_SAME<A ,csc_uint32_t> ;
	using R6X = IS_SAME<A ,csc_uint64_t> ;
	using R7X = IS_SAME<A ,csc_enum_t> ;
	using RET = ENUM_ANY<R1X ,R2X ,R3X ,R4X ,R5X ,R6X ,R7X> ;
} ;

template <class A>
using IS_INTPTR = typename IS_INTPTR_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_CLASS_HELP ;

template <class A>
trait IS_CLASS_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_CLASS<A> ;
	using R3X = ENUM_NOT<IS_ENUM<A>> ;
	using R4X = ENUM_NOT<IS_TYPE<A>> ;
	using R5X = ENUM_NOT<IS_BASIC<A>> ;
	using R6X = ENUM_NOT<IS_INTPTR<A>> ;
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
trait IS_TRIVIAL_HELP ;

template <class A>
trait IS_TRIVIAL_HELP<A ,ALWAYS> {
	using R1X = MACRO_IS_TRIVIAL<A> ;
	using RET = ENUM_ALL<R1X> ;
} ;

template <class A>
using IS_TRIVIAL = typename IS_TRIVIAL_HELP<A ,ALWAYS>::RET ;

template <class...>
trait IS_EXTEND_HELP ;

template <class FROM ,class INTO>
trait IS_EXTEND_HELP<FROM ,INTO ,ALWAYS> {
	using R1X = IS_SAME<FROM ,INTO> ;
	using R2X = MACRO_IS_EXTEND<FROM ,INTO> ;
	using RET = ENUM_ANY<R1X ,R2X> ;
} ;

template <class FROM ,class INTO>
using IS_EXTEND = typename IS_EXTEND_HELP<FROM ,INTO ,ALWAYS>::RET ;

template <class...>
trait IS_INTERFACE_HELP ;

template <class A>
trait IS_INTERFACE_HELP<A ,REQUIRE<IS_CLASS<A>>> {
	using R1X = IS_EXTEND<Interface ,A> ;
	using R2X = MACRO_IS_INTERFACE<A> ;
	using R3X = ENUM_EQUAL<SIZE_OF<A> ,SIZE_OF<Interface>> ;
	using R4X = ENUM_EQUAL<ALIGN_OF<A> ,ALIGN_OF<Interface>> ;
	using RET = ENUM_ALL<R1X ,R2X ,R3X ,R4X> ;
} ;

template <class A>
trait IS_INTERFACE_HELP<A ,REQUIRE<ENUM_NOT<IS_CLASS<A>>>> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
using IS_INTERFACE = typename IS_INTERFACE_HELP<A ,ALWAYS>::RET ;

template <class A>
using IS_POLYMORPHIC = IS_EXTEND<Interface ,A> ;

template <class A>
using IS_PLACEHOLDER = IS_EXTEND<DEF<typeof (PH0)> ,A> ;

template <class...>
trait IS_CONVERTIBLE_HELP ;

template <class FROM ,class INTO>
trait IS_CONVERTIBLE_HELP<FROM ,INTO ,ALWAYS> {
	using R1X = MACRO_IS_CONVERTIBLE<FROM ,INTO> ;
	using RET = ENUM_ANY<R1X> ;
} ;

template <class FROM ,class INTO>
using IS_CONVERTIBLE = typename IS_CONVERTIBLE_HELP<XREF<FROM> ,XREF<INTO> ,ALWAYS>::RET ;

template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...PARAM ,class A>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<PARAM...> ,A ,ALWAYS> {
	using R1X = A ;
	using R2X = DEF<RETURN (*) (PARAM...)> ;
	using R3X = IS_CONVERTIBLE<CREF<R1X> ,RREF<R2X>> ;
	using RET = ENUM_NOT<R3X> ;
} ;

template <class A>
using IS_EFFECTIVE = typename IS_EFFECTIVE_HELP<FUNCTION_RETURN<A> ,FUNCTION_PARAMS<A> ,A ,ALWAYS>::RET ;

template <class...>
trait IS_OBJECT_HELP ;

template <class A>
trait IS_OBJECT_HELP<A ,ALWAYS> {
	using R1X = IS_BASIC<A> ;
	using R2X = IS_INTPTR<A> ;
	using R3X = IS_CLASS<A> ;
	using RET = ENUM_ANY<R1X ,R2X ,R3X> ;
} ;

template <class A>
using IS_OBJECT = typename IS_OBJECT_HELP<A ,ALWAYS>::RET ;

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

template <class A>
using BYTE_BASE = typename BYTE_BASE_HELP<SIZE_OF<A> ,ALIGN_OF<A> ,ALWAYS>::RET ;

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

template <class A>
using TEXT_BASE = typename TEXT_BASE_HELP<SIZE_OF<A> ,ALIGN_OF<A> ,ALWAYS>::RET ;

using STRUA = TEXT_BASE<STRA> ;
using STRUW = TEXT_BASE<STRW> ;

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
trait TEMP_HELP ;

template <class A>
trait TEMP_HELP<A ,REQUIRE<IS_VOID<A>>> {
	struct TEMP ;
} ;

template <class A>
trait TEMP_HELP<A ,REQUIRE<ENUM_NOT<IS_VOID<A>>>> {
	struct TEMP {
		Storage<SIZE_OF<A> ,ALIGN_OF<A>> mUnused ;
	} ;
} ;

template <class A>
using TEMP = typename TEMP_HELP<REMOVE_REF<A> ,ALWAYS>::RET ;
} ;