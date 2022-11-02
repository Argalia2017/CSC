#pragma once

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
trait FUNCTION_unsafe_launder_HELP ;

template <class DEPEND>
trait FUNCTION_unsafe_launder_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_unsafe_launder {
		inline void operator() (csc_pointer_t obj) const noexcept {
			noop () ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_launder_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O0")
	struct Binder implement Interface {
		virtual void friend_launder (CREF<csc_pointer_t> obj) const = 0 ;
	} ;

	class ImplBinder implement Binder {
	public:
		void friend_launder (CREF<csc_pointer_t> obj) const override {
			noop () ;
		}
	} ;

	struct FUNCTION_unsafe_launder {
		inline void operator() (CREF<csc_pointer_t> obj) const noexcept {
			//@fatal: fuck g++4.8
			static ImplBinder mInstance ;
			auto &&tmp = static_cast<CREF<Binder>> (mInstance) ;
			tmp.friend_launder (obj) ;
		}
	} ;
#pragma GCC pop_options
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_launder_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_launder {
		inline void operator() (CREF<csc_pointer_t> obj) const noexcept {
			auto rax = obj ;
			asm volatile ("" : "+r,m" (rax) :: "memory") ;
		}
	} ;
#endif
} ;

struct FUNCTION_unsafe_launder {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const noexcept {
		using R1X = typename FUNCTION_unsafe_launder_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_launder ;
		const auto r1x = R1X () ;
		return r1x ((&obj)) ;
	}
} ;

static constexpr auto unsafe_launder = FUNCTION_unsafe_launder () ;

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
		using R1X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		return reinterpret_cast<VREF<R1X>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (LREF<ARG1> obj) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R1X = TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>> ;
		return reinterpret_cast<CREF<R1X>> (obj) ;
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
	inline CREF<TEMP<ARG1>> operator() (LREF<ARG1> obj) const noexcept {
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

template <class UNIT>
trait TEMPLATE_unsafe_cast_HELP<UNIT ,REQUIRE<IS_OBJECT<UNIT>>> {
	struct TEMPLATE_unsafe_cast {
		template <class ARG1>
		inline VREF<UNIT> operator() (VREF<ARG1> obj) const noexcept {
			using R1X = UNIT ;
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
		inline CREF<UNIT> operator() (LREF<ARG1> obj) const noexcept {
			using R1X = UNIT ;
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
		inline CREF<UNIT> operator() (RREF<ARG1>) const noexcept = delete ;
	} ;
} ;

template <class UNIT>
using TEMPLATE_unsafe_cast = typename TEMPLATE_unsafe_cast_HELP<UNIT ,ALWAYS>::TEMPLATE_unsafe_cast ;

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

struct FUNCTION_unsafe_sync {
	template <class ARG1>
	inline void operator() (VREF<TEMP<void>> dst ,CREF<ARG1> src) const noexcept {
		require (IS_TEMP<ARG1>) ;
		auto &&tmp = unsafe_cast[TYPEAS<ARG1>::expr] (dst) ;
		tmp = src ;
	}

	template <class ARG1>
	inline void operator() (VREF<ARG1> dst ,CREF<TEMP<void>> src) const noexcept {
		require (IS_TEMP<ARG1>) ;
		auto &&tmp = unsafe_cast[TYPEAS<ARG1>::expr] (src) ;
		dst = tmp ;
	}
} ;

static constexpr auto unsafe_sync = FUNCTION_unsafe_sync () ;

struct FUNCTION_swap {
	template <class ARG1>
	inline void operator() (VREF<ARG1> left ,VREF<ARG1> right) const noexcept {
		//@warn: no class should depend on its address
		const auto r1x = unsafe_deptr (left) ;
		unsafe_deptr (left) = unsafe_deptr (right) ;
		unsafe_deptr (right) = r1x ;
		unsafe_launder (left) ;
		unsafe_launder (right) ;
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

template <class UNIT>
trait TEMPLATE_keep_HELP<UNIT ,ALWAYS> {
	struct TEMPLATE_keep {
		template <class ARG1>
		inline XREF<UNIT> operator() (VREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT>> (obj) ;
		}

		template <class ARG1>
		inline XREF<UNIT> operator() (CREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT>> (obj) ;
		}

		template <class ARG1>
		inline XREF<UNIT> operator() (RREF<ARG1> obj) const noexcept {
			return static_cast<XREF<UNIT>> (obj) ;
		}
	} ;
} ;

template <class UNIT>
using TEMPLATE_keep = typename TEMPLATE_keep_HELP<UNIT ,ALWAYS>::TEMPLATE_keep ;

struct FUNCTION_keep {
	template <class ARG1>
	inline constexpr TEMPLATE_keep<ARG1> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
		using R1X = TEMPLATE_keep<ARG1> ;
		return R1X () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

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
		unsafe_launder (obj) ;
	}
} ;

static constexpr auto zeroize = FUNCTION_zeroize () ;

template <class...>
trait TEMPLATE_bitwise_HELP ;

template <class UNIT>
trait TEMPLATE_bitwise_HELP<UNIT ,ALWAYS> {
	struct TEMPLATE_bitwise {
		template <class ARG1>
		inline UNIT operator() (CREF<ARG1> obj) const noexcept {
			using R1X = UNIT ;
			using R2X = ARG1 ;
			require (IS_TRIVIAL<R1X>) ;
			require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
			UNIT ret ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (obj)) ;
			unsafe_sync (unsafe_deptr (ret) ,tmp) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT>
using TEMPLATE_bitwise = typename TEMPLATE_bitwise_HELP<UNIT ,ALWAYS>::TEMPLATE_bitwise ;

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
	inline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
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

		CREF<INDEX> peek () const leftvalue {
			return mCurr ;
		}

		inline CREF<INDEX> operator* () const leftvalue {
			return peek () ;
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

template <class UNIT>
trait FUNCTION_operator_equal_HELP<UNIT ,REQUIRE<IS_BASIC<UNIT>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			return left == right ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_equal_HELP<UNIT ,REQUIRE<IS_INTPTR<UNIT>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			return FLAG (left) == FLAG (right) ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_equal_HELP<UNIT ,REQUIRE<IS_CLASS<UNIT>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			return left.equal (right) ;
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

template <class UNIT>
trait FUNCTION_operator_compr_HELP<UNIT ,REQUIRE<IS_BASIC<UNIT>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			if (left < right)
				return NONE ;
			if (right < left)
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_compr_HELP<UNIT ,REQUIRE<IS_INTPTR<UNIT>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			if (FLAG (left) < FLAG (right))
				return NONE ;
			if (FLAG (right) < FLAG (left))
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_compr_HELP<UNIT ,REQUIRE<IS_CLASS<UNIT>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT> left ,CREF<UNIT> right) const {
			return left.compr (right) ;
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

template <class...>
trait FUNCTION_hashcode_HELP ;

template <class DEPEND>
trait FUNCTION_hashcode_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL32<DEPEND>>> {
	struct FUNCTION_hashcode {
		inline FLAG operator() () const {
			return FLAG (-2128831035) ;
		}

		inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const {
			//@fatal: fuck g++4.8
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
			//@fatal: fuck g++4.8
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

template <class UNIT>
trait FUNCTION_operator_hash_HELP<UNIT ,REQUIRE<IS_BASIC<UNIT>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT> obj) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,UNIT> ;
			const auto r1x = R1X (bitwise (obj)) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_hash_HELP<UNIT ,REQUIRE<IS_INTPTR<UNIT>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT> obj) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,UNIT> ;
			const auto r1x = R1X (bitwise (FLAG (obj))) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_hash_HELP<UNIT ,REQUIRE<IS_CLASS<UNIT>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT> obj) const {
			using R1X = DEPENDENT<BYTE_BASE<FLAG> ,UNIT> ;
			const auto r1x = R1X (obj.hash ()) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
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

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (CREF<TYPEID<ARG1>> id) const noexcept {
		static Interface mInstance ;
		return FLAG ((&mInstance)) ;
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
trait BOX_HELP ;

template <class...>
trait BOX_HOLDER_HELP ;

template <class UNIT>
trait BOX_HOLDER_HELP<UNIT ,REQUIRE<IS_TRIVIAL<UNIT>>> {
	class Box {
	protected:
		TEMP<UNIT> mValue ;
	} ;
} ;

template <class UNIT>
trait BOX_HOLDER_HELP<UNIT ,REQUIRE<IS_INTERFACE<UNIT>>> {
	class Box {
	protected:
		TEMP<UNIT> mValue ;

	public:
		implicit Box () noexcept {
			zeroize (mValue) ;
		}

		implicit ~Box () noexcept {
			const auto r1x = bitwise[TYPEAS<FLAG>::expr] (keep[TYPEAS<CREF<Interface>>::expr] (fake)) ;
			if (r1x == 0)
				return ;
			fake.finalize () ;
			fake.~UNIT () ;
			zeroize (mValue) ;
		}

	private:
		VREF<UNIT> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HOLDER_HELP<UNIT ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT>> ,ENUM_NOT<IS_INTERFACE<UNIT>>>>> {
	class Box {
	protected:
		TEMP<UNIT> mValue ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}

		implicit ~Box () noexcept {
			if ifnot (mExist)
				return ;
			fake.~UNIT () ;
			mExist = FALSE ;
		}

	private:
		VREF<UNIT> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HELP<UNIT ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT> ,IS_OBJECT<UNIT>>>> {
	using SUPER = typename BOX_HOLDER_HELP<UNIT ,ALWAYS>::Box ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		VREF<UNIT> self_m () leftvalue {
			return fake ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return fake ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<TEMP<void>> obj) {
			unsafe_sync (mValue ,obj) ;
			unsafe_launder (fake) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT ,ARG1>) ;
			new ((&mValue)) ARG1 (forward[TYPEAS<ARG2>::expr] (obj)...) ;
			unsafe_launder (fake) ;
		}

		void release () {
			noop () ;
		}

	private:
		VREF<UNIT> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HELP<UNIT ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT>> ,ENUM_NOT<IS_INTERFACE<UNIT>> ,IS_OBJECT<UNIT>>>> {
	using SUPER = typename BOX_HOLDER_HELP<UNIT ,ALWAYS>::Box ;

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
			ret.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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
			return mExist ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT> self_m () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<TEMP<void>> obj) {
			assert (ifnot (exist ())) ;
			unsafe_sync (mValue ,obj) ;
			unsafe_launder (fake) ;
			mExist = TRUE ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT ,ARG1>) ;
			assert (ifnot (exist ())) ;
			zeroize (mValue) ;
			new ((&mValue)) ARG1 (forward[TYPEAS<ARG2>::expr] (obj)...) ;
			unsafe_launder (fake) ;
			mExist = TRUE ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<UNIT> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HELP<UNIT ,REQUIRE<IS_INTERFACE<UNIT>>> {
	using SUPER = typename BOX_HOLDER_HELP<UNIT ,ALWAYS>::Box ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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
			const auto r1x = bitwise[TYPEAS<FLAG>::expr] (keep[TYPEAS<CREF<Interface>>::expr] (fake)) ;
			return r1x != 0 ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT> self_m () leftvalue {
			return fake ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return fake ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<TEMP<void>> obj) {
			assert (ifnot (exist ())) ;
			unsafe_sync (mValue ,obj) ;
			unsafe_launder (fake) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_EXTEND<Interface ,ARG1>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,SIZE_OF<UNIT>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,ALIGN_OF<UNIT>>) ;
			assert (ifnot (exist ())) ;
			zeroize (mValue) ;
			new ((&mValue)) ARG1 (forward[TYPEAS<ARG2>::expr] (obj)...) ;
			unsafe_launder (fake) ;
		}

		void release () {
			zeroize (mValue) ;
		}

	private:
		VREF<UNIT> fake_m () leftvalue {
			return unsafe_deref (mValue) ;
		}

		CREF<UNIT> fake_m () const leftvalue {
			return unsafe_deref (mValue) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HELP<UNIT ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT> ,IS_ARRAY<UNIT>>>> {
	using ITEM = ARRAY_ITEM<UNIT> ;
	using SIZE = ARRAY_SIZE<UNIT> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	using SUPER = typename BOX_HOLDER_HELP<HEAP ,ALWAYS>::Box ;

	class Box final extend SUPER {
	protected:
		using SUPER::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		VREF<UNIT> self_m () leftvalue {
			return fake.mArray ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return fake.mArray ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<TEMP<void>> obj) {
			unsafe_sync (mValue ,obj) ;
			unsafe_launder (fake) ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT ,ARG1>) ;
			using R1X = typename DEPENDENT<BOX_HELP<UNIT ,ALWAYS> ,DEPEND>::FUNCTION_translation ;
			const auto r1x = R1X () ;
			noop (r1x) ;
			new ((&mValue)) HEAP (r1x (forward[TYPEAS<ARG2>::expr] (obj))...) ;
			unsafe_launder (fake) ;
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

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::expr] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::expr] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<CREF<HEAP>>::expr] (tmp) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<RREF<HEAP>>::expr] (tmp) ;
		}
	} ;
} ;

template <class UNIT>
trait BOX_HELP<UNIT ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT>> ,IS_ARRAY<UNIT>>>> {
	using ITEM = ARRAY_ITEM<UNIT> ;
	using SIZE = ARRAY_SIZE<UNIT> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	using SUPER = typename BOX_HOLDER_HELP<HEAP ,ALWAYS>::Box ;

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
			ret.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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
			return mExist ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<UNIT> self_m () leftvalue {
			assert (exist ()) ;
			return fake.mArray ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			assert (exist ()) ;
			return fake.mArray ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<TEMP<void>> obj) {
			assert (ifnot (exist ())) ;
			unsafe_sync (mValue ,obj) ;
			unsafe_launder (fake) ;
			mExist = TRUE ;
		}

		template <class ARG1 ,class...ARG2>
		void acquire (CREF<TYPEID<ARG1>> id ,XREF<ARG2>...obj) {
			require (IS_SAME<UNIT ,ARG1>) ;
			using R1X = typename DEPENDENT<BOX_HELP<UNIT ,ALWAYS> ,DEPEND>::FUNCTION_translation ;
			assert (ifnot (exist ())) ;
			zeroize (mValue) ;
			const auto r1x = R1X () ;
			noop (r1x) ;
			new ((&mValue)) HEAP (r1x (forward[TYPEAS<ARG2>::expr] (obj))...) ;
			unsafe_launder (fake) ;
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

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::expr] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::expr] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<CREF<HEAP>>::expr] (tmp) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT> obj) const {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_deptr (obj))) ;
			return keep[TYPEAS<RREF<HEAP>>::expr] (tmp) ;
		}
	} ;
} ;

template <class UNIT>
using Box = typename BOX_HELP<UNIT ,ALWAYS>::Box ;

template <class...>
trait UNKNOWN_HELP ;

template <class...>
trait UNKNOWN_PUREHOLDER_HELP ;

template <class DEPEND>
trait UNKNOWN_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG pointer () const = 0 ;
		virtual void acquire (CREF<INDEX> index ,CREF<TEMP<void>> obj) = 0 ;
		virtual void destroy (CREF<INDEX> index) = 0 ;
	} ;

	class Unknown {
	protected:
		Box<Holder> mThis ;

	public:
		implicit Unknown () = default ;

		template <class ARG1>
		explicit Unknown (CREF<TYPEID<ARG1>> id) {
			using R1X = typename UNKNOWN_PUREHOLDER_HELP<ARG1 ,ALWAYS>::PureHolder ;
			mThis.acquire (TYPEAS<R1X>::expr) ;
		}

		FLAG pointer () const {
			return mThis->pointer () ;
		}

		void acquire (CREF<INDEX> index ,CREF<TEMP<void>> obj) {
			return mThis->acquire (index ,obj) ;
		}

		void destroy (CREF<INDEX> index) {
			return mThis->destroy (index) ;
		}
	} ;
} ;

template <class UNIT>
trait UNKNOWN_PUREHOLDER_HELP<UNIT ,ALWAYS> {
	using Holder = typename UNKNOWN_HELP<DEPEND ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	public:
		FLAG pointer () const override {
			const auto r1x = address (thiz) + SIZE_OF<PureHolder>::expr ;
			const auto r2x = valign (r1x ,ALIGN_OF<UNIT>::expr) ;
			return r2x ;
		}

		void acquire (CREF<INDEX> index ,CREF<TEMP<void>> obj) override {
			const auto r1x = pointer () + index * SIZE_OF<UNIT>::expr ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (r1x)) ;
			unsafe_sync (tmp ,obj) ;
		}

		void destroy (CREF<INDEX> index) override {
			const auto r1x = pointer () + index * SIZE_OF<UNIT>::expr ;
			auto rax = Box<UNIT> () ;
			rax.acquire (unsafe_pointer (r1x)) ;
		}
	} ;
} ;

using Unknown = typename UNKNOWN_HELP<DEPEND ,ALWAYS>::Unknown ;

struct FUNCTION_memorize {
	template <class ARG1>
	inline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		using R1X = FUNCTION_RETURN<ARG1> ;
		static R1X mInstance = func () ;
		return mInstance ;
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

	class FakeHolder implement Holder {
	protected:
		FLAG mPointer ;
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
trait XREF_HOLDER_HELP ;

template <class...>
trait XREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait XREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Unknown> unknown) = 0 ;
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void destroy () = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual BOOL lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

	//@warn: base class of two need diff name
	class XRef {
	protected:
		FLAG mHolder ;
		FLAG mPointer ;

	public:
		implicit XRef () noexcept {
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

		implicit ~XRef () noexcept {
			if (mHolder == ZERO)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = fake.decrease () ;
				if (r1x > 0)
					discard ;
				if ifswitch (TRUE) {
					if (mPointer == ZERO)
						discard ;
					fake.destroy () ;
				}
				fake.finalize () ;
			}
			mHolder = ZERO ;
			mPointer = ZERO ;
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT>
trait VREF_HELP<UNIT ,REQUIRE<IS_OBJECT<UNIT>>> {
	class VRef ;

	using Holder = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::XRef ;
	using CRef = typename CREF_HELP<UNIT ,ALWAYS>::CRef ;

	template <class ARG1>
	using CastVRef = typename DEPENDENT<VREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::VRef ;

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

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<ENUM_ANY<IS_SAME<ARG1 ,VRef> ,IS_SAME<ARG1 ,CRef>>>>>
		implicit VRef (RREF<ARG1> that)
			:VRef (forward[TYPEAS<VRef>::expr] (that.as_cast (TYPEAS<UNIT>::expr))) {}

		implicit VRef (RREF<CRef> that) {
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				const auto r1x = that.fake.lock () ;
				assert (r1x) ;
				mHolder = that.mHolder ;
				that.mHolder = ZERO ;
			}
			mPointer = that.mPointer ;
			that.mPointer = ZERO ;
		}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...obj) {
			using R1X = UNIT ;
			using R2X = typename DEPENDENT<XREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> ,UNIT>::PureHolder ;
			VRef ret ;
			const auto r1x = LENGTH (1) ;
			assert (r1x > 0) ;
			ret.mHolder = R2X::create (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
			ret.fake.initialize (Unknown (TYPEAS<R1X>::expr)) ;
			const auto r2x = ret.fake.lock () ;
			assert (r2x) ;
			if ifswitch (TRUE) {
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self)) ;
				ret.fake.acquire (tmp) ;
				rax.release () ;
			}
			ret.mPointer = ret.fake.pointer () ;
			return move (ret) ;
		}

		imports VRef reference (VREF<UNIT> that) {
			VRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
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

		VREF<UNIT> self_m () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		CastVRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (IS_INTERFACE<ARG1>) ;
			require (IS_EXTEND<ARG1 ,UNIT>) ;
			CastVRef<ARG1> ret ;
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

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT>
trait VREF_HELP<UNIT ,REQUIRE<IS_ARRAY<UNIT>>> {
	class VRef ;

	require (ENUM_EQ_ZERO<ARRAY_SIZE<UNIT>>) ;
	using Holder = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::XRef ;
	using CRef = typename CREF_HELP<UNIT ,ALWAYS>::CRef ;

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

		implicit VRef (RREF<CRef> that) {
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				const auto r1x = that.fake.lock () ;
				assert (r1x) ;
				mHolder = that.mHolder ;
				that.mHolder = ZERO ;
			}
			mPointer = that.mPointer ;
			that.mPointer = ZERO ;
		}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...obj) {
			using R1X = ARRAY_ITEM<UNIT> ;
			using R2X = typename DEPENDENT<XREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> ,UNIT>::PureHolder ;
			VRef ret ;
			const auto r1x = LENGTH (obj...) ;
			assert (r1x > 0) ;
			ret.mHolder = R2X::create (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
			ret.fake.initialize (Unknown (TYPEAS<R1X>::expr)) ;
			const auto r2x = ret.fake.lock () ;
			assert (r2x) ;
			ret.mPointer = ret.fake.pointer () ;
			if ifswitch (TRUE) {
				if (IS_TRIVIAL<R1X>::expr)
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

		imports VRef reference (VREF<UNIT> that) {
			VRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
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

		VREF<UNIT> self_m () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT>
trait CREF_HELP<UNIT ,REQUIRE<IS_OBJECT<UNIT>>> {
	class CRef ;

	using Holder = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::XRef ;
	using VRef = typename VREF_HELP<UNIT ,ALWAYS>::VRef ;

	template <class ARG1>
	using CastCRef = typename DEPENDENT<CREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::CRef ;

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

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<ENUM_ANY<IS_SAME<ARG1 ,CRef> ,IS_SAME<ARG1 ,VRef>>>>>
		implicit CRef (RREF<ARG1> that)
			:CRef (forward[TYPEAS<CRef>::expr] (that.as_cast (TYPEAS<UNIT>::expr))) {}

		implicit CRef (RREF<VRef> that) {
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				that.fake.unlock () ;
				mHolder = that.mHolder ;
				that.mHolder = ZERO ;
			}
			mPointer = that.mPointer ;
			that.mPointer = ZERO ;
		}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...obj) {
			using R1X = UNIT ;
			using R2X = typename DEPENDENT<XREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> ,UNIT>::PureHolder ;
			CRef ret ;
			const auto r1x = LENGTH (1) ;
			assert (r1x > 0) ;
			ret.mHolder = R2X::create (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
			ret.fake.initialize (Unknown (TYPEAS<R1X>::expr)) ;
			if ifswitch (TRUE) {
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax)) ;
				ret.fake.acquire (tmp) ;
				rax.release () ;
			}
			ret.mPointer = ret.fake.pointer () ;
			return move (ret) ;
		}

		imports CRef reference (LREF<UNIT> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		imports CRef reference (RREF<UNIT>) = delete ;

		implicit CRef (CREF<CRef> that) {
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

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		CastCRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (IS_INTERFACE<ARG1>) ;
			require (IS_EXTEND<ARG1 ,UNIT>) ;
			CastCRef<ARG1> ret ;
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

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class UNIT>
trait CREF_HELP<UNIT ,REQUIRE<IS_ARRAY<UNIT>>> {
	class CRef ;

	require (ENUM_EQ_ZERO<ARRAY_SIZE<UNIT>>) ;
	using Holder = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::XRef ;
	using VRef = typename VREF_HELP<UNIT ,ALWAYS>::VRef ;

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

		implicit CRef (RREF<VRef> that) {
			if ifswitch (TRUE) {
				if (that.mHolder == ZERO)
					discard ;
				that.fake.unlock () ;
				mHolder = that.mHolder ;
				that.mHolder = ZERO ;
			}
			mPointer = that.mPointer ;
			that.mPointer = ZERO ;
		}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...obj) {
			using R1X = ARRAY_ITEM<UNIT> ;
			using R2X = typename DEPENDENT<XREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> ,UNIT>::PureHolder ;
			CRef ret ;
			const auto r1x = LENGTH (obj...) ;
			assert (r1x > 0) ;
			ret.mHolder = R2X::create (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
			ret.fake.initialize (Unknown (TYPEAS<R1X>::expr)) ;
			ret.mPointer = ret.fake.pointer () ;
			if ifswitch (TRUE) {
				if (IS_TRIVIAL<R1X>::expr)
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

		imports CRef reference (LREF<UNIT> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		imports CRef reference (RREF<UNIT>) = delete ;

		implicit CRef (CREF<CRef> that) {
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

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Holder>>::expr] (unsafe_pointer (mHolder))) ;
		}
	} ;
} ;

template <class DEPEND>
trait XREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename XREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;
		Unknown mUnknown ;

	public:
		imports FLAG create (CREF<LENGTH> size_ ,CREF<LENGTH> align_) {
			const auto r1x = dynamic_size (size_ ,align_) ;
			const auto r2x = HeapProc::instance ().alloc (r1x) ;
			const auto r3x = valign (r2x ,ALIGN_OF<PureHolder>::expr) ;
			if ifswitch (TRUE) {
				auto rax = Box<PureHolder>::make () ;
				unsafe_sync (unsafe_pointer (r3x) ,unsafe_deptr (rax.self)) ;
				rax.release () ;
			}
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<PureHolder>>::expr] (unsafe_pointer (r3x))) ;
			tmp.mOrigin = r2x ;
			return address (keep[TYPEAS<VREF<Holder>>::expr] (tmp)) ;
		}

		imports LENGTH dynamic_size (CREF<LENGTH> size_ ,CREF<LENGTH> align_) {
			const auto r1x = HeapProc::instance ().basic_align () ;
			const auto r2x = vmax (ALIGN_OF<PureHolder>::expr ,align_) ;
			const auto r3x = vmax (r2x - r1x ,ZERO) ;
			const auto r4x = valign (SIZE_OF<PureHolder>::expr ,align_) ;
			const auto r5x = r3x + r4x + size_ ;
			return r5x ;
		}

		void initialize (RREF<Unknown> unknown) override {
			mCounter = 1 ;
			mSize = 0 ;
			mUnknown = move (unknown) ;
		}

		void finalize () override {
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			auto rax = Box<PureHolder> () ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (thiz)) ;
			rax.acquire (tmp) ;
			HeapProc::instance ().free (r1x) ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			INDEX ix = mSize ;
			mUnknown.acquire (ix ,obj) ;
			mSize++ ;
		}

		void destroy () override {
			while (TRUE) {
				INDEX ix = mSize - 1 ;
				if (ix < 0)
					break ;
				mUnknown.destroy (ix) ;
				mSize-- ;
			}
		}

		FLAG pointer () const override {
			return mUnknown.pointer () ;
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

template <class UNIT>
using VRef = typename VREF_HELP<UNIT ,ALWAYS>::VRef ;

template <class UNIT>
using CRef = typename CREF_HELP<UNIT ,ALWAYS>::CRef ;

template <class...>
trait AUTO_HELP ;

template <class...>
trait AUTO_HOLDER_HELP ;

template <class...>
trait AUTO_IMPLHOLDER_HELP ;

template <class DEPEND>
trait AUTO_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Unknown> unknown) = 0 ;
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void destroy () = 0 ;
		virtual FLAG pointer () const = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,1024> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Unknown mUnknown ;
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class Auto {
	protected:
		Box<FakeHolder> mThis ;
		FLAG mCabi ;

	public:
		implicit Auto () noexcept {
			mCabi = ZERO ;
		}

		implicit ~Auto () noexcept {
			if ifnot (mThis == NULL)
				return ;
			if ifswitch (TRUE) {
				if (mCabi == ZERO)
					discard ;
				mThis->destroy () ;
			}
			mThis = NULL ;
			mCabi = ZERO ;
		}
	} ;
} ;

template <class DEPEND>
trait AUTO_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FakeHolder = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FakeHolder ;
	using FAKE_MAX_SIZE = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FAKE_MAX_SIZE ;
	using FAKE_MAX_ALIGN = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FAKE_MAX_ALIGN ;
	using FUNCTION_extern = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;
	using SUPER = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::Auto ;

	class Auto final extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mCabi ;

	public:
		implicit Auto () = delete ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
		implicit Auto (RREF<ARG1> that) noexcept {
			require (IS_OBJECT<ARG1>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,FAKE_MAX_SIZE>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,FAKE_MAX_ALIGN>) ;
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (Unknown (TYPEAS<ARG1>::expr)) ;
			if ifswitch (TRUE) {
				auto rax = Box<ARG1>::make (forward[TYPEAS<ARG1>::expr] (that)) ;
				auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self)) ;
				mThis->acquire (tmp) ;
				rax.release () ;
			}
			mCabi = operator_cabi (TYPEAS<ARG1>::expr) ;
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
			const auto r1x = operator_cabi (id) ;
			assert (mCabi == r1x) ;
			auto rax = Box<ARG1> () ;
			const auto r2x = mThis->pointer () ;
			rax.acquire (unsafe_pointer (r2x)) ;
			mCabi = ZERO ;
			return move (rax.self) ;
		}
	} ;
} ;

using Auto = typename AUTO_HELP<DEPEND ,ALWAYS>::Auto ;

template <class...>
trait CAPTURE_HELP ;

template <class...UNIT>
trait CAPTURE_HELP<TYPEAS<UNIT...> ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<TYPEAS<UNIT...>>>>> {
	class Capture {
	public:
		using VARIADIC_PARAMS = TYPEAS<> ;

	public:
		implicit Capture () = default ;

		template <class ARG1>
		inline FUNCTION_RETURN<ARG1> operator() (CREF<ARG1> func) const {
			return func () ;
		}
	} ;
} ;

template <class...UNIT>
trait CAPTURE_HELP<TYPEAS<UNIT...> ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<TYPEAS<UNIT...>>>>> {
	using PARAMS = TYPEAS<UNIT...> ;
	using RANK = COUNT_OF<PARAMS> ;

	class Capture {
	public:
		using VARIADIC_PARAMS = PARAMS ;

	protected:
		Box<ARR<FLAG ,RANK>> mCapture ;

	public:
		implicit Capture () = delete ;

		explicit Capture (CREF<DEPENDENT<FLAG ,UNIT>>...params) {
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
			mCapture.self[ix] = params1 ;
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
			const auto r1x = mCapture.self[R1X::expr] ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::expr] (unsafe_pointer (r1x))) ;
			return template_invoke (PHX ,func ,TYPEAS<R3X>::expr ,params... ,keep[TYPEAS<CREF<R2X>>::expr] (tmp)) ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3 ,class = REQUIRE<ENUM_EQ_ZERO<COUNT_OF<ARG2>>>>
		FUNCTION_RETURN<ARG1> template_invoke (CREF<typeof (PH1)> ,CREF<ARG1> func ,CREF<TYPEID<ARG2>> id ,CREF<ARG3>...params) const {
			return func (params...) ;
		}
	} ;
} ;

template <class...UNIT>
using Capture = typename CAPTURE_HELP<TYPEAS<UNIT...> ,ALWAYS>::Capture ;

struct FUNCTION_capture {
	template <class...ARG1>
	inline Capture<ARG1...> operator() (CREF<ARG1>...obj) const {
		return Capture<ARG1...> (address (obj)...) ;
	}
} ;

static constexpr auto capture = FUNCTION_capture () ;

template <class...>
trait VARIADIC_HELP ;

template <class UNIT>
trait VARIADIC_HELP<UNIT ,ALWAYS> {
	class Variadic {
	protected:
		FLAG mBegin ;
		FLAG mEnd ;

	public:
		implicit Variadic () = delete ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Variadic>>>>
		implicit Variadic (RREF<ARG1> that) {
			using R1X = typename DEPENDENT<ARG1 ,DEPEND>::VARIADIC_PARAMS ;
			using R2X = TYPE_REPEAT<UNIT ,COUNT_OF<R1X>> ;
			require (IS_SAME<R1X ,R2X>) ;
			mBegin = address (that) ;
			mEnd = mBegin + ENUM_MUL<COUNT_OF<R1X> ,SIZE_OF<FLAG>>::expr ;
		}

		LENGTH rank () const {
			return (mEnd - mBegin) / SIZE_OF<FLAG>::expr ;
		}

		BOOL empty () const {
			return mBegin == mEnd ;
		}

		CREF<UNIT> one () const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<FLAG>>::expr] (unsafe_pointer (mBegin))) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (tmp))) ;
		}

		Variadic rest () const leftvalue {
			Variadic ret = thiz ;
			ret.mBegin += SIZE_OF<FLAG>::expr ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT>
using Variadic = typename VARIADIC_HELP<UNIT ,ALWAYS>::Variadic ;

template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ITEM>
trait SLICE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	struct Holder implement Interface {
		virtual void initialize (CREF<Variadic<csc_text_t>> text) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual ITEM at (CREF<INDEX> index) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Slice {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class...ARG2 ,class = REQUIRE<ENUM_NOT<ENUM_ANY<IS_SAME<ARG2 ,Slice>...>>>>
		explicit Slice (CREF<ARG2>...text) {
			require (ENUM_COMPR_GTEQ<COUNT_OF<TYPEAS<ARG2...>> ,RANK1>) ;
			require (ENUM_COMPR_LTEQ<COUNT_OF<TYPEAS<ARG2...>> ,RANK3>) ;
			using R1X = typename DEPENDENT<SLICE_HELP<ITEM ,ALWAYS> ,DEPEND>::FUNCTION_translation ;
			const auto r1x = R1X () ;
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize (capture (r1x (text)...)) ;
			mThis = move (rax) ;
		}

		LENGTH size () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->size () ;
		}

		ITEM at (CREF<INDEX> index) const {
			return mThis->at (index) ;
		}

		inline ITEM operator[] (CREF<INDEX> index) const {
			return at (index) ;
		}

		BOOL equal (CREF<Slice> that) const {
			const auto r1x = operator_compr (mThis == NULL ,that.mThis == NULL) ;
			if (r1x != 0)
				return FALSE ;
			if (mThis == NULL)
				return TRUE ;
			return mThis->equal (that.mThis.self) ;
		}

		inline BOOL operator== (CREF<Slice> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Slice> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Slice> that) const {
			const auto r1x = operator_compr (mThis == NULL ,that.mThis == NULL) ;
			if (r1x != 0)
				return r1x ;
			if (mThis == NULL)
				return r1x ;
			return mThis->compr (that.mThis.self) ;
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
			if (mThis == NULL)
				return hashcode () ;
			return mThis->hash () ;
		}
	} ;

	struct FUNCTION_translation {
		template <class ARG1>
		inline csc_text_t operator() (CREF<ARG1> text) const {
			require (IS_ARRAY<ARG1>) ;
			csc_text_t ret ;
			ret.mBegin = address (text) ;
			ret.mEnd = ret.mBegin + SIZE_OF<ARG1>::expr ;
			ret.mStep = ALIGN_OF<ARG1>::expr ;
			return move (ret) ;
		}

		inline CREF<csc_text_t> operator() (CREF<csc_text_t> text) const {
			return text ;
		}
	} ;
} ;

template <class ITEM>
using Slice = typename SLICE_HELP<ITEM ,ALWAYS>::Slice ;

template <class...>
trait FUNCTION_operator_name_HELP ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,BOOL>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("BOOL") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,VAL32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("VAL32") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,VAL64>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("VAL64") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,SINGLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("SINGLE") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,DOUBLE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("DOUBLE") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,STRA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRA") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,STRW>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRW") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,STRU8>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU8") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,STRU16>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU16") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,STRU32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("STRU32") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,BYTE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("BYTE") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,WORD>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("WORD") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,CHAR>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("CHAR") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_SAME<UNIT ,DATA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("DATA") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_NULL<UNIT>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("null") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return slice ("void") ;
		}
	} ;
} ;

template <class UNIT>
trait FUNCTION_operator_name_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_BASIC<UNIT>>>> {
	using SIZE = ENUMAS<VAL ,1024> ;

	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_text_t> text) const {
			return Slice<STR> (text) ;
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
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
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
			mThis = move (rax) ;
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
				return Slice<STR> () ;
			return mThis->type_name () ;
		}

		BOOL equal (CREF<Clazz> that) const {
			const auto r1x = operator_compr (mThis == NULL ,that.mThis == NULL) ;
			if (r1x != 0)
				return FALSE ;
			if (mThis == NULL)
				return TRUE ;
			return mThis->equal (that.mThis.self) ;
		}

		inline BOOL operator== (CREF<Clazz> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Clazz> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Clazz> that) const {
			const auto r1x = operator_compr (mThis == NULL ,that.mThis == NULL) ;
			if (r1x != 0)
				return r1x ;
			if (mThis == NULL)
				return r1x ;
			return mThis->compr (that.mThis.self) ;
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
			if (mThis == NULL)
				return hashcode () ;
			return mThis->hash () ;
		}
	} ;
} ;

using Clazz = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Clazz ;

template <class...>
trait WATCH_HELP ;

template <class UNIT>
trait WATCH_HELP<UNIT ,ALWAYS> {
	require (IS_REGISTER<UNIT>) ;

	struct WATCH implement Interface {
		FLAG mOffset ;
		FLAG mPointer ;
		Slice<STR> mName ;
		Clazz mClazz ;
	} ;
} ;

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class ARG2>
	inline forceinline void operator() (CREF<TYPEID<ARG1>> id ,CREF<Slice<STR>> name ,LREF<ARG2> expr_) const {
		using R1X = typename WATCH_HELP<ARG2 ,ALWAYS>::WATCH ;
		static R1X mInstance ;
		mInstance.mOffset = address (keep[TYPEAS<CREF<Interface>>::expr] (mInstance)) ;
		mInstance.mPointer = address (expr_) ;
		mInstance.mName = name ;
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

		explicit Exception (CREF<Slice<STR>> what_) noexcept {
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize (what_) ;
			mThis = move (rax) ;
		}

		Slice<STR> what () const noexcept {
			if (mThis == NULL)
				return Slice<STR> () ;
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
	inline void operator() (RREF<ARG1> proc1) const noexcept {
		require (IS_VOID<FUNCTION_RETURN<ARG1>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (RREF<ARG1> proc1 ,RREF<ARG2>...proc2) const noexcept {
		require (IS_VOID<FUNCTION_RETURN<ARG1>>) ;
		try {
			return proc1 () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		} catch (...) {
			assert (FALSE) ;
		}
		return thiz (forward[TYPEAS<ARG2>::expr] (proc2)...) ;
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

template <class UNIT>
trait SCOPE_HOLDER_HELP<UNIT ,ALWAYS> {
	class Scope {
	protected:
		FLAG mPointer ;

	public:
		implicit Scope () noexcept {
			mPointer = ZERO ;
		}

		implicit ~Scope () noexcept {
			if (mPointer == ZERO)
				return ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mPointer))) ;
			tmp.leave () ;
			mPointer = ZERO ;
		}
	} ;
} ;

template <class UNIT>
trait SCOPE_HELP<UNIT ,ALWAYS> {
	using SUPER = typename SCOPE_HOLDER_HELP<UNIT ,ALWAYS>::Scope ;

	class Scope final extend SUPER {
	protected:
		using SUPER::mPointer ;

	public:
		implicit Scope () = default ;

		explicit Scope (LREF<UNIT> that) {
			const auto r1x = address (that) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (r1x))) ;
			tmp.enter () ;
			mPointer = r1x ;
		}

		explicit Scope (RREF<UNIT>) = delete ;

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

template <class UNIT>
using Scope = typename SCOPE_HELP<UNIT ,ALWAYS>::Scope ;
} ;