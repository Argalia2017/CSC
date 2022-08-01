﻿#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"

namespace CSC {
struct FUNCTION_noop {
	inline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	inline void operator() (CREF<ARG1> obj) const noexcept {
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
		inline void operator() () const noexcept {
			noop () ;
		}
	} ;
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_barrier {
		inline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_barrier {
		inline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

struct FUNCTION_unsafe_barrier {
	inline void operator() () const noexcept {
		using R1X = typename FUNCTION_unsafe_barrier_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_barrier ;
		const auto r1x = R1X () ;
		return r1x () ;
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
		const auto r1x = R1X () ;
		return r1x () ;
	}
} ;

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

struct FUNCTION_unsafe_abort {
	inline forceinline void operator() () const {
		throw ;
	}
} ;

static constexpr auto unsafe_abort = FUNCTION_unsafe_abort () ;

struct FUNCTION_unsafe_deref {
	template <class ARG1>
	inline VREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (VREF<ARG1> obj) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		return reinterpret_cast<VREF<R2X>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (CREF<ARG1> obj) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		return reinterpret_cast<CREF<R2X>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

static constexpr auto unsafe_deref = FUNCTION_unsafe_deref () ;

struct FUNCTION_unsafe_deptr {
	template <class ARG1>
	inline VREF<TEMP<ARG1>> operator() (VREF<ARG1> obj) const noexcept {
		return reinterpret_cast<VREF<TEMP<ARG1>>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TEMP<ARG1>> operator() (CREF<ARG1> obj) const noexcept {
		return reinterpret_cast<CREF<TEMP<ARG1>>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TEMP<ARG1>> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

static constexpr auto unsafe_deptr = FUNCTION_unsafe_deptr () ;

struct FUNCTION_address {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> obj) const noexcept {
		return FLAG (&unsafe_deptr (obj)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

template <class...>
trait TEMPLATE_unsafe_cast_HELP ;

template <class UNIT1>
trait TEMPLATE_unsafe_cast_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	struct TEMPLATE_unsafe_cast {
		template <class ARG1>
		inline VREF<UNIT1> operator() (VREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		template <class ARG1>
		inline CREF<UNIT1> operator() (CREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			require (IS_TEMP<R1X>) ;
			require (IS_TEMP<R2X>) ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}

		template <class ARG1>
		inline CREF<UNIT1> operator() (RREF<ARG1>) const noexcept = delete ;
	} ;
} ;

template <class UNIT1>
trait TEMPLATE_unsafe_cast_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct TEMPLATE_unsafe_cast {
		template <class ARG1>
		inline VREF<UNIT1> operator() (VREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<TYPE_FIRST_ONE<REFLECT_TEMP<R3X>> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<VREF<R1X>> (obj) ;
		}

		template <class ARG1>
		inline CREF<UNIT1> operator() (CREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			using R3X = ARRAY_ITEM<R1X> ;
			require (IS_TEMP<R3X>) ;
			require (IS_TEMP<R2X>) ;
			using R4X = ARR<TYPE_FIRST_ONE<REFLECT_TEMP<R3X>> ,ARRAY_SIZE<R1X>> ;
			require (ENUM_ANY<IS_SAME<R2X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<R4X>>>) ;
			return reinterpret_cast<CREF<R1X>> (obj) ;
		}

		template <class ARG1>
		inline CREF<UNIT1> operator() (RREF<ARG1>) const noexcept = delete ;
	} ;
} ;

template <class UNIT1>
using TEMPLATE_unsafe_cast = typename TEMPLATE_unsafe_cast_HELP<UNIT1 ,ALWAYS>::TEMPLATE_unsafe_cast ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	inline constexpr TEMPLATE_unsafe_cast<ARG1> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
		using R1X = TEMPLATE_unsafe_cast<ARG1> ;
		return R1X () ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_unsafe_pointer {
	inline VREF<TEMP<void>> operator() (CREF<FLAG> addr) const noexcept {
		assert (addr != ZERO) ;
		const auto r1x = reinterpret_cast<PTR<VREF<TEMP<void>>>> (addr) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_pointer = FUNCTION_unsafe_pointer () ;

struct FUNCTION_unsafe_array {
	template <class ARG1>
	inline VREF<ARR<ARG1>> operator() (VREF<ARG1> obj) const noexcept {
		const auto r1x = reinterpret_cast<PTR<VREF<ARR<ARG1>>>> (address (obj)) ;
		return (*r1x) ;
	}

	template <class ARG1>
	inline CREF<ARR<ARG1>> operator() (CREF<ARG1> obj) const noexcept {
		const auto r1x = reinterpret_cast<PTR<CREF<ARR<ARG1>>>> (address (obj)) ;
		return (*r1x) ;
	}

	template <class ARG1>
	inline VREF<ARR<ARG1>> operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		const auto r1x = reinterpret_cast<PTR<VREF<ARR<ARG1>>>> (ZERO) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_array = FUNCTION_unsafe_array () ;

struct FUNCTION_swap {
	template <class ARG1>
	inline void operator() (VREF<ARG1> left ,VREF<ARG1> right) const noexcept {
		//@warn: no class should depend on its address
		const auto r1x = unsafe_deptr (left) ;
		unsafe_deptr (left) = unsafe_deptr (right) ;
		unsafe_deptr (right) = r1x ;
		unsafe_barrier () ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> left ,RREF<ARG2> right) const noexcept {
		thiz (left ,right) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (RREF<ARG1> left ,VREF<ARG2> right) const noexcept {
		thiz (left ,right) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

template <class...>
trait TEMPLATE_keep_HELP ;

template <class UNIT1>
trait TEMPLATE_keep_HELP<UNIT1 ,ALWAYS> {
	struct TEMPLATE_keep {
		template <class ARG1>
		inline XREF<UNIT1> operator() (VREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT1>> (obj) ;
		}

		template <class ARG1>
		inline XREF<UNIT1> operator() (CREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT1>> (obj) ;
		}

		template <class ARG1>
		inline XREF<UNIT1> operator() (RREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT1>> (obj) ;
		}
	} ;
} ;

template <class UNIT1>
using TEMPLATE_keep = typename TEMPLATE_keep_HELP<UNIT1 ,ALWAYS>::TEMPLATE_keep ;

struct FUNCTION_keep {
	template <class ARG1>
	inline constexpr TEMPLATE_keep<ARG1> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
		using R1X = TEMPLATE_keep<ARG1> ;
		return R1X () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_copy {
	template <class ARG1>
	inline ARG1 operator() (CREF<ARG1> obj) const {
		require (IS_CLONEABLE<ARG1>) ;
		return keep[TYPEAS<CREF<ARG1>>::expr] (obj) ;
	}
} ;

static constexpr auto copy = FUNCTION_copy () ;

struct FUNCTION_move {
	template <class ARG1>
	inline RREF<ARG1> operator() (VREF<ARG1> obj) const noexcept {
		return keep[TYPEAS<RREF<ARG1>>::expr] (obj) ;
	}

	template <class ARG1>
	inline ARG1 operator() (CREF<ARG1> obj) const {
		require (IS_CLONEABLE<ARG1>) ;
		return keep[TYPEAS<CREF<ARG1>>::expr] (obj) ;
	}

	template <class ARG1>
	inline RREF<ARG1> operator() (RREF<ARG1> obj) const noexcept {
		return keep[TYPEAS<RREF<ARG1>>::expr] (obj) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_drop {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const noexcept {
		const auto r1x = move (obj) ;
		noop (r1x) ;
	}
} ;

static constexpr auto drop = FUNCTION_drop () ;

struct FUNCTION_forward {
	template <class ARG1>
	inline constexpr TEMPLATE_keep<ARG1> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
		return keep[TYPEAS<ARG1>::expr] ;
	}
} ;

static constexpr auto forward = FUNCTION_forward () ;

struct FUNCTION_zeroize {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const noexcept {
		require (IS_TRIVIAL<ARG1>) ;
		using R2X = Storage<SIZE_OF<ARG1>> ;
		auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::expr] (unsafe_deptr (obj))) ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= SIZE_OF<ARG1>::expr)
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
	inline void operator() (VREF<ARG1> left) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		require (IS_OBJECT<R2X>) ;
		require (IS_DEFAULT<R2X>) ;
		zeroize (left) ;
		new ((&left)) R2X () ;
		unsafe_barrier () ;
	}
} ;

static constexpr auto unsafe_create = FUNCTION_unsafe_create () ;

struct FUNCTION_unsafe_destroy {
	template <class ARG1>
	inline void operator() (VREF<ARG1> left) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		require (IS_OBJECT<R2X>) ;
		auto &&tmp = unsafe_deref (left) ;
		tmp.~R2X () ;
		unsafe_barrier () ;
	}
} ;

static constexpr auto unsafe_destroy = FUNCTION_unsafe_destroy () ;

template <class...>
trait TEMPLATE_bitwise_HELP ;

template <class UNIT1>
trait TEMPLATE_bitwise_HELP<UNIT1 ,ALWAYS> {
	struct TEMPLATE_bitwise {
		template <class ARG1>
		inline UNIT1 operator() (CREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			require (IS_TRIVIAL<R1X>) ;
			require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<Storage<SIZE_OF<UNIT1>>>>::expr] (unsafe_deptr (obj)) ;
			return thiz (tmp) ;
		}

		inline UNIT1 operator() (CREF<TEMP<Storage<SIZE_OF<UNIT1>>>> obj) const noexcept {
			using R1X = UNIT1 ;
			require (IS_TRIVIAL<R1X>) ;
			using R3X = Storage<SIZE_OF<R1X>> ;
			R1X ret ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<R3X>>::expr] (unsafe_deptr (ret)) ;
			tmp = obj ;
			unsafe_barrier () ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
using TEMPLATE_bitwise = typename TEMPLATE_bitwise_HELP<UNIT1 ,ALWAYS>::TEMPLATE_bitwise ;

struct FUNCTION_bitwise {
	template <class ARG1>
	inline BYTE_BASE<ARG1> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = BYTE_BASE<ARG1> ;
		return thiz[TYPEAS<R1X>::expr] (obj) ;
	}

	template <class ARG1>
	inline constexpr TEMPLATE_bitwise<ARG1> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
		using R1X = TEMPLATE_bitwise<ARG1> ;
		return R1X () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline XREF<ARG1> operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		assert (FALSE) ;
		unsafe_abort () ;
		return thiz (id) ;
	}
} ;

static constexpr auto bad = FUNCTION_bad () ;

struct FUNCTION_invoke {
	template <class ARG1>
	inline FUNCTION_RETURN<ARG1> operator() (CREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

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
	inline VAL operator() (CREF<VAL> left ,CREF<VAL> right) const {
		if (left <= right)
			return left ;
		return right ;
	}
} ;

static constexpr auto vmin = FUNCTION_vmin () ;

struct FUNCTION_vmax {
	inline VAL operator() (CREF<VAL> left ,CREF<VAL> right) const {
		if (left >= right)
			return left ;
		return right ;
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
trait INDEXITERATOR_HELP ;

template <class DEPEND>
trait INDEXITERATOR_HELP<DEPEND ,ALWAYS> {
	class IndexIterator {
	protected:
		INDEX mBegin ;
		INDEX mEnd ;
		INDEX mCurr ;

	public:
		implicit IndexIterator () = delete ;

		explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			mBegin = begin_ ;
			mEnd = vmax (begin_ ,end_) ;
			mCurr = mBegin ;
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

template <class...>
trait FUNCTION_operator_equal_HELP ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			return left == right ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_INTPTR<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			return FLAG (left) == FLAG (right) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	template <class ARG1>
	using HAS_EQUAL = IS_BOOL<typeof (bad (TYPEAS<CREF<ARG1>>::expr).equal (bad (TYPEAS<CREF<ARG1>>::expr)))> ;

	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			return template_equal (PHX ,left ,right) ;
		}

		template <class ARG1 ,class = REQUIRE<HAS_EQUAL<ARG1>>>
		BOOL template_equal (CREF<typeof (PH2)> ,CREF<ARG1> left ,CREF<ARG1> right) const {
			return left.equal (right) ;
		}

		template <class ARG1>
		BOOL template_equal (CREF<typeof (PH1)> ,CREF<ARG1> left ,CREF<ARG1> right) const {
			return TRUE ;
		}
	} ;
} ;

struct FUNCTION_operator_equal {
	template <class ARG1>
	inline BOOL operator() (CREF<ARG1> left ,CREF<ARG1> right) const {
		using R3X = typename FUNCTION_operator_equal_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_equal ;
		const auto r1x = R3X () ;
		return r1x (left ,right) ;
	}
} ;

static constexpr auto operator_equal = FUNCTION_operator_equal () ;

template <class...>
trait FUNCTION_operator_compr_HELP ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			if (left < right)
				return NONE ;
			if (right < left)
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_INTPTR<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			if (FLAG (left) < FLAG (right))
				return NONE ;
			if (FLAG (right) < FLAG (left))
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	template <class ARG1>
	using HAS_COMPR = IS_VALUE<typeof (bad (TYPEAS<CREF<ARG1>>::expr).compr (bad (TYPEAS<CREF<ARG1>>::expr)))> ;

	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> left ,CREF<UNIT1> right) const {
			return template_compr (PHX ,left ,right) ;
		}

		template <class ARG1 ,class = REQUIRE<HAS_COMPR<ARG1>>>
		FLAG template_compr (CREF<typeof (PH2)> ,CREF<ARG1> left ,CREF<ARG1> right) const {
			return left.compr (right) ;
		}

		template <class ARG1>
		FLAG template_compr (CREF<typeof (PH1)> ,CREF<ARG1> left ,CREF<ARG1> right) const {
			return ZERO ;
		}
	} ;
} ;

struct FUNCTION_operator_compr {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> left ,CREF<ARG1> right) const {
		using R3X = typename FUNCTION_operator_compr_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_compr ;
		const auto r1x = R3X () ;
		return r1x (left ,right) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

//@fatal: fuck g++4.8
template <class DEPEND>
using FLAG_BYTE_BASE = DEPENDENT<BYTE_BASE<FLAG> ,DEPEND> ;

template <class...>
trait FUNCTION_hashcode_HELP ;

template <class DEPEND>
trait FUNCTION_hashcode_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL32<DEPEND>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-2128831035) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			using R1X = FLAG_BYTE_BASE<DEPEND> ;
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
			using R1X = FLAG_BYTE_BASE<DEPEND> ;
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
		const auto r1x = R1X () ;
		return r1x () ;
	}

	inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
		using R1X = typename FUNCTION_hashcode_HELP<DEPEND ,ALWAYS>::FUNCTION_hashcode ;
		const auto r1x = R1X () ;
		return r1x (now ,inc) ;
	}
} ;

static constexpr auto hashcode = FUNCTION_hashcode () ;

template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			using R1X = FLAG_BYTE_BASE<UNIT1> ;
			const auto r1x = R1X (bitwise (obj)) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_INTPTR<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			using R1X = FLAG_BYTE_BASE<UNIT1> ;
			const auto r1x = R1X (bitwise (FLAG (obj))) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	template <class ARG1>
	using HAS_HASH = IS_VALUE<typeof (bad (TYPEAS<CREF<UNIT1>>::expr).hash ())> ;

	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return template_hash (PHX ,obj) ;
		}

		template <class ARG1 ,class = REQUIRE<HAS_HASH<ARG1>>>
		FLAG template_hash (CREF<typeof (PH2)> ,CREF<ARG1> obj) const {
			using R1X = FLAG_BYTE_BASE<DEPEND> ;
			const auto r1x = R1X (obj.hash ()) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}

		template <class ARG1>
		FLAG template_hash (CREF<typeof (PH1)> ,CREF<ARG1> obj) const {
			return ZERO ;
		}
	} ;
} ;

struct FUNCTION_operator_hash {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> obj) const {
		using R1X = typename FUNCTION_operator_hash_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_hash ;
		const auto r1x = R1X () ;
		return r1x (obj) ;
	}
} ;

static constexpr auto operator_hash = FUNCTION_operator_hash () ;

template <class...>
trait CABI_HELP ;

template <class UUID>
trait CABI_HELP<UUID ,ALWAYS> {
	require (IS_REGISTER<UUID>) ;

	struct CABI implement Interface {} ;
} ;

template <class...>
trait FUNCTION_operator_cabi_HELP ;

template <class UUID>
trait FUNCTION_operator_cabi_HELP<UUID ,REQUIRE<MACRO_COMPILER_MSVC<UUID>>> {
	struct FUNCTION_operator_cabi {
		inline FLAG operator() () const noexcept {
			using R1X = typename CABI_HELP<UUID ,ALWAYS>::CABI ;
			const auto r1x = R1X () ;
			const auto r2x = bitwise (keep[TYPEAS<CREF<Interface>>::expr] (r1x)) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UUID>
trait FUNCTION_operator_cabi_HELP<UUID ,REQUIRE<MACRO_COMPILER_GNUC<UUID>>> {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC optimize off
	struct FUNCTION_operator_cabi {
		inline FLAG operator() () const noexcept {
			using R1X = typename CABI_HELP<UUID ,ALWAYS>::CABI ;
			const auto r1x = R1X () ;
			const auto r2x = bitwise (keep[TYPEAS<CREF<Interface>>::expr] (r1x)) ;
			return FLAG (r2x) ;
		}
	} ;
#pragma GCC optimize on
#endif
} ;

template <class UUID>
trait FUNCTION_operator_cabi_HELP<UUID ,REQUIRE<MACRO_COMPILER_CLANG<UUID>>> {
#ifdef __CSC_COMPILER_CLANG__
#pragma clang optimize off
	struct FUNCTION_operator_cabi {
		inline FLAG operator() () const noexcept {
			using R1X = typename CABI_HELP<UUID ,ALWAYS>::CABI ;
			const auto r1x = R1X () ;
			const auto r2x = bitwise (keep[TYPEAS<CREF<Interface>>::expr] (r1x)) ;
			return FLAG (r2x) ;
		}
	} ;
#pragma clang optimize on
#endif
} ;

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		using R1X = typename FUNCTION_operator_cabi_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_cabi ;
		const auto r1x = R1X () ;
		return r1x () ;
	}
} ;

static constexpr auto operator_cabi = FUNCTION_operator_cabi () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> left ,CREF<ARG1> expect ,XREF<ARG2> right) const {
		if ifnot (operator_equal (left ,expect))
			return ;
		left = forward[TYPEAS<ARG2>::expr] (right) ;
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

	static constexpr auto value = ENUM_ALL<R1X ,R2X ,R3X ,R4X>::expr ;
	using RET = ENUMAS<BOOL ,ENUMID<value>> ;
} ;

template <class UNIT1 ,class UNIT2>
using IS_BOX_ACQUIRE = typename IS_BOX_ACQUIRE_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RET ;

template <class...>
trait BOX_HELP ;

template <class...>
trait BOX_HOLDER_HELP ;

template <class UNIT1>
trait BOX_HOLDER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT1> ,IS_OBJECT<UNIT1>>>> {
	class Box {
	protected:
		TEMP<UNIT1> mValue ;
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT1> ,IS_OBJECT<UNIT1>>>> {
	using SUPER = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::Box ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
			return move (ret) ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			return fake ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return fake ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_BOX_ACQUIRE<UNIT1 ,ARG1>) ;
			zeroize (mValue) ;
			new ((&mValue)) ARG1 (forward[TYPEAS<ARG2>::expr] (obj)...) ;
			unsafe_barrier () ;
		}

		void release () {
			noop () ;
		}

	private:
		VREF<UNIT1> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT1> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HOLDER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT1>> ,IS_OBJECT<UNIT1>>>> {
	class Box {
	protected:
		TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT1>> ,IS_OBJECT<UNIT1>>>> {
	using SUPER = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::Box ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;
		using SUPER::mExist ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		implicit Box (RREF<Box> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
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
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_BOX_ACQUIRE<UNIT1 ,ARG1>) ;
			assert (ifnot (mExist)) ;
			zeroize (mValue) ;
			new ((&mValue)) ARG1 (forward[TYPEAS<ARG2>::expr] (obj)...) ;
			unsafe_barrier () ;
			mExist = TRUE ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<UNIT1> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT1> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT1> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	using SUPER = typename BOX_HOLDER_HELP<HEAP ,ALWAYS>::Box ;

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::expr] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::expr] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT1> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<CREF<HEAP>>::expr] (tmp) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT1> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<RREF<HEAP>>::expr] (tmp) ;
		}
	} ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
			return move (ret) ;
		}

		implicit Box (CREF<Box>) = delete ;

		inline VREF<Box> operator= (CREF<Box>) = delete ;

		implicit Box (RREF<Box> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			return fake.mArray ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return fake.mArray ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT1 ,ARG1>) ;
			const auto r1x = FUNCTION_translation () ;
			noop (r1x) ;
			zeroize (mValue) ;
			new ((&mValue)) HEAP (r1x (forward[TYPEAS<ARG2>::expr] (obj))...) ;
			unsafe_barrier () ;
		}

		void release () {
			noop () ;
		}

	private:
		VREF<HEAP> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT1>> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	using SUPER = typename BOX_HOLDER_HELP<HEAP ,ALWAYS>::Box ;

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::expr] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::expr] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT1> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<CREF<HEAP>>::expr] (tmp) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT1> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<RREF<HEAP>>::expr] (tmp) ;
		}
	} ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;
		using SUPER::mExist ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		implicit Box (RREF<Box> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			assert (exist ()) ;
			return fake.mArray ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			assert (exist ()) ;
			return fake.mArray ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT1 ,ARG1>) ;
			const auto r1x = FUNCTION_translation () ;
			noop (r1x) ;
			assert (ifnot (mExist)) ;
			zeroize (mValue) ;
			new ((&mValue)) HEAP (r1x (forward[TYPEAS<ARG2>::expr] (obj))...) ;
			unsafe_barrier () ;
			mExist = TRUE ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<HEAP> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT1>
using Box = typename BOX_HELP<UNIT1 ,ALWAYS>::Box ;

template <class...>
trait CELL_HELP ;

template <class...>
trait CELL_HOLDER_HELP ;

template <class UNIT1>
trait CELL_HOLDER_HELP<UNIT1 ,ALWAYS> {
	class Cell {
	protected:
		TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Cell () noexcept {
			mExist = FALSE ;
		}
	} ;
} ;

template <class UNIT1>
trait CELL_HELP<UNIT1 ,ALWAYS> {
	require (IS_CLONEABLE<UNIT1>) ;

	using SUPER = typename CELL_HOLDER_HELP<UNIT1 ,ALWAYS>::Cell ;

	class Cell final extend SUPER {
	protected:
		using SUPER::mValue ;
		using SUPER::mExist ;

	public:
		implicit Cell () = default ;

		implicit Cell (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Cell make (XREF<ARG1>...obj) {
			Cell ret ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		implicit Cell (RREF<Cell> that) noexcept {
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

		void replace (CREF<UNIT1> expect ,CREF<UNIT1> right) const {
			assert (mExist) ;
			if ifnot (operator_equal (fake ,expect))
				return ;
			fake = right ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> right) const {
			assert (mExist) ;
			if ifswitch (TRUE) {
				if ifnot (operator_equal (fake ,expect))
					discard ;
				fake = right ;
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
		VREF<UNIT1> fake_m () const leftvalue {
			const auto r1x = address (mValue) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (r1x))) ;
		}
	} ;
} ;

template <class UNIT1>
using Cell = typename CELL_HELP<UNIT1 ,ALWAYS>::Cell ;

template <class...>
trait ABSTRACT_HELP ;

template <class...>
trait ABSTRACT_PUREHOLDER_HELP ;

template <class...>
trait ABSTRACT_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ABSTRACT_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual LENGTH type_size () const = 0 ;
		virtual LENGTH type_align () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual void create (VREF<TEMP<void>> me ,CREF<TEMP<void>> func) const = 0 ;
		virtual void destroy (VREF<TEMP<void>> me) const = 0 ;
	} ;

	class FakeHolder implement Holder {} ;

	template <class ARG1>
	using CRTP_Abstract = typename DEPENDENT<ABSTRACT_HELP<DEPEND ,ALWAYS> ,ARG1>::Abstract ;

	struct FUNCTION_linkage {
		imports FLAG invoke (CREF<TEMP<void>> func ,RREF<CRTP_Abstract<DEPEND>> abst) ;
	} ;

	class Abstract {
	public:
		Box<FakeHolder> mThis ;

	public:
		implicit Abstract () = default ;

		template <class ARG1>
		explicit Abstract (CREF<TYPEID<ARG1>> id) {
			using R1X = typename ABSTRACT_IMPLHOLDER_HELP<ARG1 ,void ,ALWAYS>::ImplHolder ;
			mThis.acquire (TYPEAS<R1X>::expr) ;
		}

		template <class ARG1 ,class ARG2>
		explicit Abstract (CREF<TYPEID<ARG1>> id1 ,CREF<TYPEID<ARG2>> id2) {
			using R1X = typename ABSTRACT_IMPLHOLDER_HELP<ARG1 ,ARG2 ,ALWAYS>::ImplHolder ;
			mThis.acquire (TYPEAS<R1X>::expr) ;
		}

		LENGTH type_size () const {
			return mThis->type_size () ;
		}

		LENGTH type_align () const {
			return mThis->type_align () ;
		}

		FLAG type_cabi () const {
			return mThis->type_cabi () ;
		}

		void create (VREF<TEMP<void>> me ,CREF<TEMP<void>> func) const {
			return mThis->create (me ,func) ;
		}

		void destroy (VREF<TEMP<void>> me) const {
			return mThis->destroy (me) ;
		}

		template <class ARG1>
		FLAG linkage (CREF<ARG1> func) {
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (func)) ;
			return FUNCTION_linkage::invoke (tmp ,move (thiz)) ;
		}
	} ;
} ;

template <class UUID ,class CREATOR>
trait ABSTRACT_IMPLHOLDER_HELP<UUID ,CREATOR ,REQUIRE<IS_VOID<CREATOR>>> {
	using Holder = typename ABSTRACT_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		LENGTH type_size () const override {
			return SIZE_OF<UUID>::expr ;
		}

		LENGTH type_align () const override {
			return ALIGN_OF<UUID>::expr ;
		}

		FLAG type_cabi () const override {
			return operator_cabi (TYPEAS<UUID>::expr) ;
		}

		void create (VREF<TEMP<void>> me ,CREF<TEMP<void>> func) const override {
			assert (FALSE) ;
		}

		void destroy (VREF<TEMP<void>> me) const override {
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<UUID>>::expr] (me) ;
			unsafe_destroy (tmp) ;
		}
	} ;
} ;

template <class UUID ,class CREATOR>
trait ABSTRACT_IMPLHOLDER_HELP<UUID ,CREATOR ,REQUIRE<ENUM_NOT<IS_VOID<CREATOR>>>> {
	using Holder = typename ABSTRACT_HELP<DEPEND ,ALWAYS>::Holder ;
	require (IS_SAME<UUID ,FUNCTION_RETURN<CREATOR>>) ;

	class ImplHolder implement Holder {
	public:
		LENGTH type_size () const override {
			return SIZE_OF<UUID>::expr ;
		}

		LENGTH type_align () const override {
			return ALIGN_OF<UUID>::expr ;
		}

		FLAG type_cabi () const override {
			//@warn: type_cabi is depend on CREATOR
			return operator_cabi (TYPEAS<CREATOR>::expr) ;
		}

		void create (VREF<TEMP<void>> me ,CREF<TEMP<void>> func) const override {
			auto rax = Box<UUID> () ;
			if ifswitch (TRUE) {
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CREATOR>>::expr] (func)) ;
				rax = Box<UUID>::make (tmp ()) ;
			}
			if ifswitch (TRUE) {
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<UUID>>::expr] (me) ;
				tmp = unsafe_deptr (rax.self) ;
				rax.release () ;
			}
		}

		void destroy (VREF<TEMP<void>> me) const override {
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<UUID>>::expr] (me) ;
			unsafe_destroy (tmp) ;
		}
	} ;
} ;

using Abstract = typename ABSTRACT_HELP<DEPEND ,ALWAYS>::Abstract ;

struct FUNCTION_memorize {
	template <class ARG1>
	inline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		using R1X = FUNCTION_RETURN<ARG1> ;
		auto rax = Abstract (TYPEAS<R1X>::expr ,TYPEAS<ARG1>::expr) ;
		const auto r1x = rax.linkage (func) ;
		auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R1X>>::expr] (unsafe_pointer (r1x))) ;
		return keep[TYPEAS<CREF<R1X>>::expr] (tmp) ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

template <class...>
trait HEAPPROC_HELP ;

template <class...>
trait HEAPPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait HEAPPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual LENGTH usage_size () const = 0 ;
		virtual LENGTH basic_align () const = 0 ;
		virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
		virtual void free (CREF<FLAG> addr) const = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
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
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		LENGTH usage_size () const {
			return mThis->usage_size () ;
		}

		LENGTH basic_align () const {
			return mThis->basic_align () ;
		}

		FLAG alloc (CREF<LENGTH> size_) const {
			return mThis->alloc (size_) ;
		}

		void free (CREF<FLAG> addr) const {
			return mThis->free (addr) ;
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
trait VREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait VREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Abstract> abst) = 0 ;
		virtual void destroy () = 0 ;
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void release () = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual BOOL lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

	struct FUNCTION_linkage {
		imports FLAG invoke (CREF<LENGTH> size_ ,CREF<LENGTH> align_) ;
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
	} ;
} ;

template <class UNIT1>
trait VREF_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;
	using FUNCTION_linkage = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage ;

	template <class ARG1>
	using CRTP_CastVRef = typename DEPENDENT<VREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::VRef ;

	template <class ARG1>
	using CRTP_CRef = typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef ;

	class VRef final extend SUPER {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		using SUPER::mHolder ;
		using SUPER::mPointer ;

	public:
		implicit VRef () = default ;

		implicit VRef (CREF<typeof (NULL)>) {}

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,VRef>>>>
		implicit VRef (RREF<ARG1> that)
			:VRef (forward[TYPEAS<VRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

		template <class ARG1 = DEPEND>
		implicit VRef (RREF<CRTP_CRef<ARG1>> that)
			: VRef (that.as_vref ()) {}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...obj) {
			using R1X = UNIT1 ;
			VRef ret ;
			const auto r1x = LENGTH (1) ;
			if ifswitch (TRUE) {
				assert (r1x > 0) ;
				ret.mHolder = FUNCTION_linkage::invoke (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
				ret.fake.initialize (Abstract (TYPEAS<R1X>::expr)) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				const auto r3x = ret.fake.lock () ;
				assert (r3x) ;
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self)) ;
				ret.fake.acquire (tmp) ;
				rax.release () ;
				ret.mPointer = ret.fake.pointer () ;
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

		implicit VRef (RREF<VRef> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		CRTP_CastVRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (IS_INTERFACE<ARG1>) ;
			require (IS_EXTEND<ARG1 ,UNIT1>) ;
			CRTP_CastVRef<ARG1> ret ;
			const auto r1x = invoke ([&] () {
				if (mPointer == ZERO)
					return ZERO ;
				return address (keep[TYPEAS<VREF<ARG1>>::expr] (self)) ;
			}) ;
			ret.mPointer = r1x ;
			mPointer = ZERO ;
			ret.mHolder = mHolder ;
			mHolder = ZERO ;
			return move (ret) ;
		}

		template <class ARG1 = DEPEND>
		CRTP_CRef<ARG1> as_cref () {
			CRTP_CRef<ARG1> ret ;
			if ifswitch (TRUE) {
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
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
trait VREF_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	require (ENUM_EQ_ZERO<ARRAY_SIZE<UNIT1>>) ;
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;
	using FUNCTION_linkage = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage ;

	template <class ARG1>
	using CRTP_CastVRef = typename DEPENDENT<VREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::VRef ;

	template <class ARG1>
	using CRTP_CRef = typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef ;

	class VRef final extend SUPER {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		using SUPER::mHolder ;
		using SUPER::mPointer ;

	public:
		implicit VRef () = default ;

		implicit VRef (CREF<typeof (NULL)>) {}

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,VRef>>>>
		implicit VRef (RREF<ARG1> that)
			:VRef (forward[TYPEAS<VRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

		template <class ARG1 = DEPEND>
		implicit VRef (RREF<CRTP_CRef<ARG1>> that)
			: VRef (that.as_vref ()) {}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...obj) {
			using R1X = ARRAY_ITEM<UNIT1> ;
			VRef ret ;
			const auto r1x = LENGTH (obj...) ;
			if ifswitch (TRUE) {
				assert (r1x > 0) ;
				ret.mHolder = FUNCTION_linkage::invoke (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
				ret.fake.initialize (Abstract (TYPEAS<R1X>::expr)) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				const auto r3x = ret.fake.lock () ;
				assert (r3x) ;
				ret.mPointer = ret.fake.pointer () ;
				if (IS_TRIVIAL<UNIT1>::expr)
					discard ;
				for (auto &&i : iter (0 ,r1x)) {
					noop (i) ;
					auto rax = Box<R1X>::make () ;
					auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self)) ;
					ret.fake.acquire (tmp) ;
					rax.release () ;
				}
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

		implicit VRef (RREF<VRef> that) noexcept {
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

		VREF<UNIT1> self_m () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		template <class ARG1 = DEPEND>
		CRTP_CRef<ARG1> as_cref () {
			CRTP_CRef<ARG1> ret ;
			if ifswitch (TRUE) {
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
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
using VRef = typename VREF_HELP<UNIT1 ,ALWAYS>::VRef ;

template <class...>
trait CREF_HELP ;

template <class...>
trait CREF_IMPLHOLDER_HELP ;

template <class UNIT1>
trait CREF_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;
	using FUNCTION_linkage = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage ;

	template <class ARG1>
	using CRTP_VRef = typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef ;

	template <class ARG1>
	using CRTP_CastCRef = typename DEPENDENT<CREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::CRef ;

	class CRef final extend SUPER {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		using SUPER::mHolder ;
		using SUPER::mPointer ;

	public:
		implicit CRef () = default ;

		implicit CRef (CREF<typeof (NULL)>) {}

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,CRef>>>>
		implicit CRef (RREF<ARG1> that)
			:CRef (forward[TYPEAS<CRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

		template <class ARG1 = DEPEND>
		implicit CRef (RREF<CRTP_VRef<ARG1>> that)
			: CRef (that.as_cref ()) {}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...obj) {
			using R1X = UNIT1 ;
			CRef ret ;
			const auto r1x = LENGTH (1) ;
			if ifswitch (TRUE) {
				assert (r1x > 0) ;
				ret.mHolder = FUNCTION_linkage::invoke (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
				ret.fake.initialize (Abstract (TYPEAS<R1X>::expr)) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax)) ;
				ret.fake.acquire (tmp) ;
				rax.release () ;
				ret.mPointer = ret.fake.pointer () ;
			}
			return move (ret) ;
		}

		imports CRef reference (CREF<UNIT1> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		imports CRef reference (RREF<UNIT1>) = delete ;

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

		implicit CRef (CREF<CRef> that) {
			if ifnot (that.exist ())
				return ;
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				const auto r1x = that.fake.increase () ;
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

		implicit CRef (RREF<CRef> that) noexcept {
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

		CREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		CRTP_CastCRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (IS_INTERFACE<ARG1>) ;
			require (IS_EXTEND<ARG1 ,UNIT1>) ;
			CRTP_CastCRef<ARG1> ret ;
			const auto r1x = invoke ([&] () {
				if (mPointer == ZERO)
					return ZERO ;
				return address (keep[TYPEAS<CREF<ARG1>>::expr] (self)) ;
			}) ;
			ret.mPointer = r1x ;
			mPointer = ZERO ;
			ret.mHolder = mHolder ;
			mHolder = ZERO ;
			return move (ret) ;
		}

		template <class ARG1 = DEPEND>
		CRTP_VRef<ARG1> as_vref () {
			CRTP_VRef<ARG1> ret ;
			if ifswitch (TRUE) {
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
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
trait CREF_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	require (ENUM_EQ_ZERO<ARRAY_SIZE<UNIT1>>) ;
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;
	using FUNCTION_linkage = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage ;

	template <class ARG1>
	using CRTP_VRef = typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef ;

	template <class ARG1>
	using CRTP_CastCRef = typename DEPENDENT<CREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::CRef ;

	class CRef final extend SUPER {
	private:
		template <class...>
		friend trait VREF_HELP ;
		template <class...>
		friend trait CREF_HELP ;

	protected:
		using SUPER::mHolder ;
		using SUPER::mPointer ;

	public:
		implicit CRef () = default ;

		implicit CRef (CREF<typeof (NULL)>) {}

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,CRef>>>>
		implicit CRef (RREF<ARG1> that)
			:CRef (forward[TYPEAS<CRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

		template <class ARG1 = DEPEND>
		implicit CRef (RREF<CRTP_VRef<ARG1>> that)
			: CRef (that.as_cref ()) {}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...obj) {
			using R1X = ARRAY_ITEM<UNIT1> ;
			CRef ret ;
			const auto r1x = LENGTH (obj...) ;
			if ifswitch (TRUE) {
				assert (r1x > 0) ;
				ret.mHolder = FUNCTION_linkage::invoke (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
				ret.fake.initialize (Abstract (TYPEAS<R1X>::expr)) ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.fake.increase () ;
				assert (r2x == IDEN) ;
				ret.mPointer = ret.fake.pointer () ;
				if (IS_TRIVIAL<UNIT1>::expr)
					discard ;
				for (auto &&i : iter (0 ,r1x)) {
					noop (i) ;
					auto rax = Box<R1X>::make () ;
					auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax)) ;
					ret.fake.acquire (tmp) ;
					rax.release () ;
				}
			}
			return move (ret) ;
		}

		imports CRef reference (CREF<UNIT1> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		imports CRef reference (RREF<UNIT1>) = delete ;

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

		implicit CRef (CREF<CRef> that) {
			if ifnot (that.exist ())
				return ;
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				const auto r1x = that.fake.increase () ;
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

		implicit CRef (RREF<CRef> that) noexcept {
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

		CREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		template <class ARG1 = DEPEND>
		CRTP_VRef<ARG1> as_vref () {
			CRTP_VRef<ARG1> ret ;
			if ifswitch (TRUE) {
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
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT1>
using CRef = typename CREF_HELP<UNIT1 ,ALWAYS>::CRef ;

template <class...>
trait AUTO_HELP ;

template <class...>
trait AUTO_HOLDER_HELP ;

template <class...>
trait AUTO_IMPLHOLDER_HELP ;

template <class DEPEND>
trait AUTO_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Abstract> abst) = 0 ;
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void release () = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,ENUMID<1024>> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Abstract mAbstract ;
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class Auto {
	protected:
		Box<FakeHolder> mThis ;
		BOOL mGood ;

	public:
		implicit Auto () noexcept {
			mGood = FALSE ;
		}
	} ;
} ;

template <class DEPEND>
trait AUTO_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FakeHolder = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FakeHolder ;
	using FAKE_MAX_SIZE = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FAKE_MAX_SIZE ;
	using FAKE_MAX_ALIGN = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FAKE_MAX_ALIGN ;
	using SUPER = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::Auto ;
	using FUNCTION_extern = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	class Auto final extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mGood ;

	public:
		implicit Auto () = delete ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
		implicit Auto (RREF<ARG1> that) noexcept {
			require (IS_OBJECT<ARG1>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,FAKE_MAX_SIZE>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,FAKE_MAX_ALIGN>) ;
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (Abstract (TYPEAS<ARG1>::expr)) ;
			if ifswitch (TRUE) {
				auto rax = Box<ARG1>::make (forward[TYPEAS<ARG1>::expr] (that)) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self)) ;
				mThis->acquire (tmp) ;
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

		implicit Auto (RREF<Auto> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Auto> operator= (RREF<Auto> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		template <class ARG1>
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			assert (mGood) ;
			const auto r1x = mThis->type_cabi () ;
			const auto r2x = operator_cabi (id) ;
			assert (r1x == r2x) ;
			const auto r3x = mThis->pointer () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARG1>>::expr] (unsafe_pointer (r3x))) ;
			ARG1 ret = move (tmp) ;
			mThis->release () ;
			mGood = FALSE ;
			return move (ret) ;
		}
	} ;
} ;

using Auto = typename AUTO_HELP<DEPEND ,ALWAYS>::Auto ;

template <class...>
trait BINDPARAMS_HELP ;

template <class...UNIT1>
trait BINDPARAMS_HELP<TYPEAS<UNIT1...> ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<TYPEAS<UNIT1...>>>>> {
	class BindParams {
	public:
		implicit BindParams () = default ;

		template <class ARG1>
		inline FUNCTION_RETURN<ARG1> operator() (CREF<ARG1> func) const {
			return func () ;
		}
	} ;
} ;

template <class...UNIT1>
trait BINDPARAMS_HELP<TYPEAS<UNIT1...> ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<TYPEAS<UNIT1...>>>>> {
	using PARAMS = TYPEAS<UNIT1...> ;
	using RANK = COUNT_OF<PARAMS> ;

	class BindParams {
	protected:
		Box<ARR<FLAG ,RANK>> mBindParams ;

	public:
		implicit BindParams () = delete ;

		explicit BindParams (CREF<DEPENDENT<FLAG ,UNIT1>>...params) {
			template_assign (PHX ,TYPEAS<RANK>::expr ,params...) ;
		}

		template <class ARG1>
		inline FUNCTION_RETURN<ARG1> operator() (CREF<ARG1> func) const {
			return template_invoke (PHX ,func ,TYPEAS<PARAMS>::expr) ;
		}

	private:
		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG1>>>
		void template_assign (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id ,CREF<FLAG> params1 ,CREF<ARG2>...params2) {
			INDEX ix = ENUM_SUB<RANK ,ARG1>::expr ;
			mBindParams.self[ix] = params1 ;
			template_assign (PHX ,TYPEAS<ENUM_DEC<ARG1>>::expr ,params2...) ;
		}

		template <class ARG1>
		void template_assign (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id) {
			noop () ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3 ,class = REQUIRE<ENUM_GT_ZERO<COUNT_OF<ARG2>>>>
		FUNCTION_RETURN<ARG1> template_invoke (CREF<typeof (PH2)> ,CREF<ARG1> func ,CREF<TYPEID<ARG2>> id ,CREF<ARG3>...params) const {
			using R1X = ENUM_SUB<RANK ,COUNT_OF<ARG2>> ;
			using R2X = TYPE_FIRST_ONE<ARG2> ;
			using R3X = TYPE_FIRST_REST<ARG2> ;
			const auto r1x = mBindParams.self[R1X::expr] ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::expr] (unsafe_pointer (r1x))) ;
			return template_invoke (PHX ,func ,TYPEAS<R3X>::expr ,params... ,keep[TYPEAS<CREF<R2X>>::expr] (tmp)) ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3 ,class = REQUIRE<ENUM_EQ_ZERO<COUNT_OF<ARG2>>>>
		FUNCTION_RETURN<ARG1> template_invoke (CREF<typeof (PH1)> ,CREF<ARG1> func ,CREF<TYPEID<ARG2>> id ,CREF<ARG3>...params) const {
			return func (params...) ;
		}
	} ;
} ;

template <class...PARAMS>
using BindParams = typename BINDPARAMS_HELP<TYPEAS<PARAMS...> ,ALWAYS>::BindParams ;

struct FUNCTION_bind {
	template <class...ARG1>
	inline BindParams<ARG1...> operator() (CREF<ARG1>...obj) const {
		return BindParams<ARG1...> (address (obj)...) ;
	}
} ;

static constexpr auto bind = FUNCTION_bind () ;

template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ITEM>
trait SLICE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	struct NODE {
		FLAG mPointer ;
		LENGTH mAlign ;
		LENGTH mCount ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (CREF<NODE> text1) = 0 ;
		virtual void initialize (CREF<NODE> text1 ,CREF<NODE> text2) = 0 ;
		virtual void initialize (CREF<NODE> text1 ,CREF<NODE> text2 ,CREF<NODE> text3) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual ITEM at (CREF<INDEX> index) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Slice {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Slice>>>>
		explicit Slice (CREF<TYPEID<ARG1>> id ,CREF<ARG2>...text) {
			require (ENUM_COMPR_GTEQ<COUNT_OF<TYPEAS<ARG2...>> ,RANK1>) ;
			require (ENUM_COMPR_LTEQ<COUNT_OF<TYPEAS<ARG2...>> ,RANK3>) ;
			require (ENUM_ALL<IS_ARRAY<ARG2>...>) ;
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize (slice_text (text)...) ;
			mThis = rax.as_cref () ;
		}

		imports CREF<Slice> zero () {
			return memorize ([&] () {
				return Slice (TYPEAS<where>::expr ,"") ;
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
				if ifnot (operator_equal (mThis->at (i) ,that.mThis->at (i)))
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
				const auto r2x = operator_compr (mThis->at (i) ,that.mThis->at (i)) ;
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
				const auto r1x = operator_hash (mThis->at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

	private:
		template <class ARG1>
		imports NODE slice_text (CREF<ARG1> text) {
			NODE ret ;
			ret.mPointer = address (text) ;
			ret.mAlign = ALIGN_OF<ARRAY_ITEM<ARG1>>::expr ;
			ret.mCount = ARRAY_SIZE<ARG1>::expr ;
			return move (ret) ;
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
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("BOOL") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,VAL32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("VAL32") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,VAL64>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("VAL64") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,SINGLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("SINGLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,DOUBLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("DOUBLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,TRIPLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("TRIPLE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRA") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRW>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRW") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU8>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU8") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU16>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU16") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,STRU32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU32") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,BYTE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("BYTE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,WORD>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("WORD") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,CHAR>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("CHAR") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,DATA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("DATA") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_SAME<UNIT1 ,HUGE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("HUGE") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_NULL<UNIT1>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("null") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("void") ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_name_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_BASIC<UNIT1>>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			using R1X = ENUMAS<VAL ,ENUMID<1024>> ;
			const auto r1x = text.mEnd - text.mBegin ;
			auto &&tmp = memorize ([&] () {
				Box<ARR<STR ,R1X>> ret = Box<ARR<STR ,R1X>>::make () ;
				INDEX ix = 0 ;
				while (TRUE) {
					if (ix >= r1x)
						break ;
					ret.self[ix] = text.mBegin[ix] ;
					ix++ ;
				}
				while (TRUE) {
					if (ix >= R1X::expr)
						break ;
					ret.self[ix] = 0 ;
					ix++ ;
				}
				return move (ret) ;
			}) ;
			return Slice<STR> (TYPEAS<UNIT1>::expr ,tmp.self) ;
		}
	} ;
} ;

struct FUNCTION_operator_name {
	template <class ARG1>
	inline Slice<STR> operator() (CREF<TYPEID<ARG1>> id) const {
		using R1X = typename FUNCTION_operator_name_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_name ;
		const auto r1x = R1X () ;
		const auto r2x = FUNCTION_internel_name () ;
		return memorize ([&] () {
			return r1x (r2x (TYPEAS<ARG1>::expr)) ;
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
		virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> align_ ,CREF<FLAG> cabi ,CREF<Slice<STR>> name) = 0 ;
		virtual LENGTH type_size () const = 0 ;
		virtual LENGTH type_align () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual Slice<STR> type_name () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Clazz {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Clazz () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Clazz>>>>
		explicit Clazz (CREF<TYPEID<ARG1>> id) {
			require (IS_REGISTER<ARG1>) ;
			auto rax = FUNCTION_extern::invoke () ;
			const auto r1x = SIZE_OF<ARG1>::expr ;
			const auto r2x = ALIGN_OF<ARG1>::expr ;
			const auto r3x = operator_cabi (TYPEAS<ARG1>::expr) ;
			const auto r4x = operator_name (TYPEAS<ARG1>::expr) ;
			rax->initialize (r1x ,r2x ,r3x ,r4x) ;
			mThis = rax.as_cref () ;
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

using Clazz = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Clazz ;

template <class...>
trait WATCH_HELP ;

template <class UUID>
trait WATCH_HELP<UUID ,ALWAYS> {
	require (IS_REGISTER<UUID>) ;

	struct WATCH implement Interface {
		Slice<STR> mName ;
		FLAG mPointer ;
		Clazz mClazz ;
	} ;
} ;

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class ARG2>
	inline forceinline void operator() (CREF<TYPEID<ARG1>> id ,CREF<Slice<STR>> name ,CREF<ARG2> expr_) const {
		using R3X = typename WATCH_HELP<ARG2 ,ALWAYS>::WATCH ;
		static R3X mInstance ;
		mInstance.mName = name ;
		mInstance.mPointer = address (expr_) ;
		mInstance.mClazz = Clazz (TYPEAS<ARG2>::expr) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (CREF<ARG1> ,CREF<Slice<STR>> ,RREF<ARG2>) const = delete ;
} ;

static constexpr auto unsafe_watch = FUNCTION_unsafe_watch () ;

template <class...>
trait EXCEPTION_HELP ;

template <class...>
trait EXCEPTION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait EXCEPTION_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<Slice<STR>> what_) = 0 ;
		virtual Slice<STR> what () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Exception {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Exception () = default ;

		template <class ARG1>
		explicit Exception (CREF<TYPEID<ARG1>> id ,CREF<Slice<STR>> what_) noexcept {
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize (what_) ;
			mThis = rax.as_cref () ;
		}

		implicit Exception (CREF<Exception> that) noexcept {
			mThis = that.mThis ;
		}

		inline VREF<Exception> operator= (CREF<Exception> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		Slice<STR> what () const noexcept {
			if (mThis == NULL)
				return Slice<STR>::zero () ;
			return mThis->what () ;
		}

		forceinline void raise () {
			throw thiz ;
		}
	} ;
} ;

using Exception = typename EXCEPTION_HELP<DEPEND ,ALWAYS>::Exception ;

struct FUNCTION_try_invoke {
	template <class ARG1>
	inline void operator() (CREF<ARG1> proc1) const noexcept {
		require (IS_VOID<FUNCTION_RETURN<ARG1>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (CREF<ARG1> proc1 ,CREF<ARG2>...proc2) const {
		require (IS_VOID<FUNCTION_RETURN<ARG1>>) ;
		try {
			return proc1 () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		} catch (...) {
			assert (FALSE) ;
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

template <class...>
trait SCOPE_HOLDER_HELP ;

template <class...>
trait SCOPE_IMPLHOLDER_HELP ;

template <class UNIT1>
trait SCOPE_HOLDER_HELP<UNIT1 ,ALWAYS> {
	class Scope {
	protected:
		FLAG mPointer ;

	public:
		implicit Scope () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait SCOPE_HELP<UNIT1 ,ALWAYS> {
	using SUPER = typename SCOPE_HOLDER_HELP<UNIT1 ,ALWAYS>::Scope ;

	class Scope final extend SUPER {
	protected:
		using SUPER::mPointer ;

	public:
		implicit Scope () = default ;

		explicit Scope (CREF<UNIT1> that) {
			const auto r1x = address (that) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (r1x))) ;
			tmp.enter () ;
			mPointer = r1x ;
		}

		explicit Scope (RREF<UNIT1>) = delete ;

		implicit ~Scope () noexcept {
			if (mPointer == ZERO)
				return ;
			try_invoke ([&] () {
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
				tmp.leave () ;
			}) ;
			mPointer = ZERO ;
		}

		implicit Scope (CREF<Scope>) = delete ;

		inline VREF<Scope> operator= (CREF<Scope>) = delete ;

		implicit Scope (RREF<Scope> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Scope> operator= (RREF<Scope> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class UNIT1>
using Scope = typename SCOPE_HELP<UNIT1 ,ALWAYS>::Scope ;
} ;