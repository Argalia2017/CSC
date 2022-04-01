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
	imports void invoke () ;

	inline forceinline void operator() () const noexcept {
		return invoke () ;
	}
} ;

static constexpr auto unsafe_abort = FUNCTION_unsafe_abort () ;

struct FUNCTION_unsafe_deref {
	template <class ARG1>
	inline VREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (VREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		require (IS_TEMP<R1X>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<R1X>> ;
		return reinterpret_cast<VREF<R2X>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		require (IS_TEMP<R1X>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<R1X>> ;
		return reinterpret_cast<CREF<R2X>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TYPE_FIRST_ONE<REFLECT_TEMP<ARG1>>> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

static constexpr auto unsafe_deref = FUNCTION_unsafe_deref () ;

struct FUNCTION_unsafe_deptr {
	template <class ARG1>
	inline VREF<TEMP<ARG1>> operator() (VREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		return reinterpret_cast<VREF<TEMP<R1X>>> (obj) ;
	}

	template <class ARG1>
	inline CREF<TEMP<ARG1>> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		return reinterpret_cast<CREF<TEMP<R1X>>> (obj) ;
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
trait FUNCTION_unsafe_cast_impl_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,REQUIRE<IS_OBJECT<UNIT1>>> {
	struct FUNCTION_unsafe_cast_impl {
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
trait FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,REQUIRE<IS_ARRAY<UNIT1>>> {
	struct FUNCTION_unsafe_cast_impl {
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
using FUNCTION_unsafe_cast_impl = typename FUNCTION_unsafe_cast_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_unsafe_cast_impl ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	inline constexpr FUNCTION_unsafe_cast_impl<REMOVE_ALL<ARG1>> operator[] (CREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_unsafe_cast_impl<R1X> ;
		return R2X () ;
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
	template <class ARG1 ,class = REQUIRE<IS_SAME<REMOVE_ALL<ARG1> ,REMOVE_REF<ARG1>>>>
	inline VREF<ARR<REMOVE_ALL<ARG1>>> operator() (VREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		const auto r1x = reinterpret_cast<PTR<VREF<ARR<R1X>>>> (address (obj)) ;
		return (*r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SAME<REMOVE_ALL<ARG1> ,REMOVE_REF<ARG1>>>>
	inline CREF<ARR<REMOVE_ALL<ARG1>>> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		const auto r1x = reinterpret_cast<PTR<CREF<ARR<R1X>>>> (address (obj)) ;
		return (*r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,REMOVE_REF<ARG1>>>>>
	inline VREF<ARR<REMOVE_ALL<ARG1>>> operator() (CREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		const auto r1x = reinterpret_cast<PTR<VREF<ARR<R1X>>>> (ZERO) ;
		return (*r1x) ;
	}

	template <class ARG1>
	inline CREF<ARR<REMOVE_ALL<ARG1>>> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

static constexpr auto unsafe_array = FUNCTION_unsafe_array () ;

struct FUNCTION_swap {
	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> obj1 ,VREF<ARG2> obj2) const noexcept {
		//@warn: no class should depend on its address
		using R1X = ARG1 ;
		using R2X = ARG2 ;
		require (IS_SAME<R1X ,R2X>) ;
		auto rax = unsafe_deptr (obj1) ;
		unsafe_deptr (obj1) = unsafe_deptr (obj2) ;
		unsafe_deptr (obj2) = rax ;
		unsafe_barrier () ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> obj1 ,RREF<ARG2> obj2) const noexcept {
		thiz (obj1 ,obj2) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (RREF<ARG1> obj1 ,VREF<ARG2> obj2) const noexcept {
		thiz (obj1 ,obj2) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class UNIT1>
trait FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS> {
	struct FUNCTION_keep_impl {
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
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_keep_impl ;

struct FUNCTION_keep {
	template <class ARG1>
	inline constexpr FUNCTION_keep_impl<REMOVE_ALL<ARG1>> operator[] (CREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_keep_impl<R1X> ;
		return R2X () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	inline RREF<ARG1> operator() (VREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		return keep[TYPEAS<RREF<R1X>>::id] (obj) ;
	}

	template <class ARG1>
	inline ARG1 operator() (CREF<ARG1> obj) const {
		using R1X = ARG1 ;
		require (IS_CLONEABLE<R1X>) ;
		return keep[TYPEAS<CREF<R1X>>::id] (obj) ;
	}

	template <class ARG1>
	inline RREF<ARG1> operator() (RREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		return keep[TYPEAS<RREF<R1X>>::id] (obj) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_drop {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const {
		const auto r1x = move (obj) ;
		noop (r1x) ;
	}
} ;

static constexpr auto drop = FUNCTION_drop () ;

struct FUNCTION_forward {
	template <class ARG1>
	inline constexpr FUNCTION_keep_impl<REMOVE_ALL<ARG1>> operator[] (CREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		return keep[TYPEAS<R1X>::id] ;
	}
} ;

static constexpr auto forward = FUNCTION_forward () ;

struct FUNCTION_zeroize {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
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
	inline void operator() (VREF<ARG1> thiz_) const noexcept {
		using R1X = ARG1 ;
		require (IS_TEMP<R1X>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<R1X>> ;
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
	inline void operator() (VREF<ARG1> thiz_) const noexcept {
		using R1X = ARG1 ;
		require (IS_TEMP<R1X>) ;
		using R2X = TYPE_FIRST_ONE<REFLECT_TEMP<R1X>> ;
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
		inline UNIT1 operator() (CREF<ARG1> obj) const noexcept {
			using R1X = UNIT1 ;
			using R2X = ARG1 ;
			require (IS_TRIVIAL<R1X>) ;
			require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
			using R3X = Storage<SIZE_OF<R1X>> ;
			R1X ret ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<R3X>>::id] (unsafe_deptr (ret)) ;
			tmp = unsafe_cast[TYPEAS<TEMP<R3X>>::id] (unsafe_deptr (obj)) ;
			unsafe_barrier () ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
using FUNCTION_bitwise_impl = typename FUNCTION_bitwise_impl_HELP<UNIT1 ,ALWAYS>::FUNCTION_bitwise_impl ;

struct FUNCTION_bitwise {
	template <class ARG1>
	inline BYTE_BASE<ARG1> operator() (CREF<ARG1> obj) const noexcept {
		using R1X = ARG1 ;
		using R2X = BYTE_BASE<R1X> ;
		return thiz[TYPEAS<R2X>::id] (obj) ;
	}

	template <class ARG1>
	inline constexpr FUNCTION_bitwise_impl<REMOVE_ALL<ARG1>> operator[] (CREF<ARG1> id) const noexcept {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = FUNCTION_bitwise_impl<R1X> ;
		return R2X () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline XREF<REMOVE_ALL<ARG1>> operator() (CREF<ARG1> id) const noexcept {
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

struct FUNCTION_memorize {
	template <class ARG1>
	inline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		using R1X = ARG1 ;
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
trait CABI_HELP<UUID ,REQUIRE<IS_REGISTER<UUID>>> {
	class CABI implement Interface {} ;
} ;

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> id) const {
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
trait FUNCTION_operator_equal_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return template_equal (PHX ,obj1 ,obj2) ;
		}

		template <class ARG1 = DEPEND ,class = REQUIRE<HAS_EQUAL<DEPENDENT<UNIT1 ,ARG1>>>>
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
	inline BOOL operator() (CREF<ARG1> obj1 ,CREF<ARG2> obj2) const {
		using R1X = ARG1 ;
		using R2X = ARG2 ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename FUNCTION_operator_equal_HELP<R1X ,ALWAYS>::FUNCTION_operator_equal ;
		const auto r1x = R3X () ;
		return r1x (obj1 ,obj2) ;
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
trait FUNCTION_operator_compr_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<UNIT1> obj1 ,CREF<UNIT1> obj2) const {
			return template_compr (PHX ,obj1 ,obj2) ;
		}

		template <class ARG1 = DEPEND ,class = REQUIRE<HAS_COMPR<DEPENDENT<UNIT1 ,ARG1>>>>
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
	inline FLAG operator() (CREF<ARG1> obj1 ,CREF<ARG2> obj2) const {
		using R1X = ARG1 ;
		using R2X = ARG1 ;
		require (IS_SAME<R1X ,R2X>) ;
		using R3X = typename FUNCTION_operator_compr_HELP<R1X ,ALWAYS>::FUNCTION_operator_compr ;
		const auto r1x = R3X () ;
		return r1x (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

//@warn: fuck g++4.8
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

template <class UNIT1>
using HAS_HASH = IS_VALUE<typeof (bad (TYPEAS<CREF<UNIT1>>::id).hash ())> ;

template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			using R1X = FLAG_BYTE_BASE<UNIT1> ;
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
			using R1X = FLAG_BYTE_BASE<UNIT1> ;
			const auto r1x = bitwise (FLAG (obj)) ;
			const auto r2x = R1X (r1x) & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_operator_hash_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<UNIT1> obj) const {
			return template_hash (PHX ,obj) ;
		}

		template <class ARG1 = DEPEND ,class = REQUIRE<HAS_HASH<DEPENDENT<UNIT1 ,ARG1>>>>
		FLAG template_hash (CREF<typeof (PH2)> ,CREF<UNIT1> obj) const {
			using R1X = FLAG_BYTE_BASE<DEPEND> ;
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
	inline FLAG operator() (CREF<ARG1> obj) const {
		using R1X = ARG1 ;
		using R2X = typename FUNCTION_operator_hash_HELP<R1X ,ALWAYS>::FUNCTION_operator_hash ;
		const auto r1x = R2X () ;
		return r1x (obj) ;
	}
} ;

static constexpr auto operator_hash = FUNCTION_operator_hash () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2 ,class ARG3>
	inline void operator() (VREF<ARG1> thiz_ ,CREF<ARG2> expect ,XREF<ARG3> obj) const {
		using R1X = ARG1 ;
		using R2X = ARG2 ;
		require (IS_SAME<R1X ,R2X>) ;
		if ifnot (operator_equal (thiz_ ,expect))
			return ;
		thiz_ = forward[TYPEAS<ARG3>::id] (obj) ;
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
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
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
		void acquire (CREF<ARG1> id ,XREF<ARG2>...obj) {
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
		void acquire (CREF<ARG1> id ,XREF<ARG2>...obj) {
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
trait BOX_HOLDER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT1> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mValue ;
	} ;

	class Box {
	protected:
		TEMP<HEAP> mArray ;
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<IS_TRIVIAL<UNIT1> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	using HEAP = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::HEAP ;
	using SUPER = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::Box ;

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::id] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::id] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT1> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj)))) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT1> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj)))) ;
		}
	} ;

	class Box final extend SUPER {
	protected:
		using SUPER::mArray ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...obj) {
			Box ret ;
			ret.acquire (TYPEAS<UNIT1>::id ,forward[TYPEAS<ARG1>::id] (obj)...) ;
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
		void acquire (CREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<UNIT1 ,R1X>) ;
			const auto r1x = FUNCTION_translation () ;
			noop (r1x) ;
			zeroize (mArray) ;
			new ((&mArray)) HEAP (r1x (forward[TYPEAS<ARG2>::id] (obj))...) ;
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
	} ;
} ;

template <class UNIT1>
trait BOX_HOLDER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT1>> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	struct HEAP {
		ARR<ITEM ,SIZE> mValue ;
	} ;

	class Box {
	protected:
		TEMP<HEAP> mArray ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}
	} ;
} ;

template <class UNIT1>
trait BOX_HELP<UNIT1 ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<UNIT1>> ,IS_ARRAY<UNIT1>>>> {
	using ITEM = ARRAY_ITEM<UNIT1> ;
	using SIZE = ARRAY_SIZE<UNIT1> ;
	require (ENUM_GT_ZERO<SIZE>) ;

	using HEAP = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::HEAP ;
	using SUPER = typename BOX_HOLDER_HELP<UNIT1 ,ALWAYS>::Box ;

	struct FUNCTION_translation {
		inline CREF<HEAP> operator() (CREF<HEAP> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::id] (obj) ;
		}

		inline RREF<HEAP> operator() (RREF<HEAP> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::id] (obj) ;
		}

		inline CREF<HEAP> operator() (CREF<UNIT1> obj) const {
			return keep[TYPEAS<CREF<HEAP>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj)))) ;
		}

		inline RREF<HEAP> operator() (RREF<UNIT1> obj) const {
			return keep[TYPEAS<RREF<HEAP>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_deptr (obj)))) ;
		}
	} ;

	class Box final extend SUPER {
	protected:
		using SUPER::mArray ;
		using SUPER::mExist ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

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
		void acquire (CREF<ARG1> id ,XREF<ARG2>...obj) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<UNIT1 ,R1X>) ;
			const auto r1x = FUNCTION_translation () ;
			noop (r1x) ;
			assert (ifnot (mExist)) ;
			zeroize (mArray) ;
			new ((&mArray)) HEAP (r1x (forward[TYPEAS<ARG2>::id] (obj))...) ;
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
	class Cell final {
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

	class Cell final {
	protected:
		TEMP<UNIT1> mValue ;
		BOOL mExist ;

	public:
		implicit Cell () = default ;

		implicit Cell (CREF<typeof (NULL)>) {}

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
		virtual void initialize () = 0 ;
		virtual LENGTH align () const = 0 ;
		virtual LENGTH usage_size () const = 0 ;
		virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
		virtual void free (CREF<FLAG> addr_) const = 0 ;
	} ;

	using HEAPPROC_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using HEAPPROC_MAX_ALIGN = ALIGN_OF<DATA> ;

	class FakeHolder implement Holder {
	protected:
		Storage<HEAPPROC_MAX_SIZE ,HEAPPROC_MAX_ALIGN> mStorage ;

	public:
		void initialize () override ;
		LENGTH align () const override ;
		LENGTH usage_size () const override ;
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
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		LENGTH align () const {
			return mThis->align () ;
		}

		LENGTH usage_size () const {
			return mThis->usage_size () ;
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
trait STATICHEAPPROC_HELP ;

template <class...>
trait STATICHEAPPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait STATICHEAPPROC_HELP<DEPEND ,ALWAYS> {
	using Holder = typename HEAPPROC_HELP<DEPEND ,ALWAYS>::Holder ;
	using FakeHolder = typename HEAPPROC_HELP<DEPEND ,ALWAYS>::FakeHolder ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class StaticHeapProc {
	protected:
		Box<FakeHolder> mThis ;

	public:
		imports CREF<StaticHeapProc> instance () {
			return memorize ([&] () {
				StaticHeapProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		LENGTH align () const {
			return mThis->align () ;
		}

		LENGTH usage_size () const {
			return mThis->usage_size () ;
		}

		FLAG alloc (CREF<LENGTH> size_) const {
			return mThis->alloc (size_) ;
		}

		void free (CREF<FLAG> addr_) const {
			return mThis->free (addr_) ;
		}
	} ;
} ;

using StaticHeapProc = typename STATICHEAPPROC_HELP<DEPEND ,ALWAYS>::StaticHeapProc ;

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
	} ;
} ;

template <class UNIT1>
trait VREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;

	template <class ARG1>
	using MACRO_VRef = typename DEPENDENT<VREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::VRef ;

	template <class ARG1>
	using MACRO_CRef = typename DEPENDENT<CREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::CRef ;

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
		implicit VRef (RREF<ARG1> that) : VRef (forward[TYPEAS<VRef>::id] (move (that).rebind (TYPEAS<UNIT1>::id))) {}

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
				ret.fake.acquire (unsafe_cast[TYPEAS<TEMP<void>>::id] (unsafe_deptr (rax.self))) ;
				rax.release () ;
				ret.mPointer = ret.fake.addr () ;
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
		MACRO_VRef<REMOVE_ALL<ARG1>> rebind (CREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			MACRO_VRef<R1X> ret ;
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
		imports LENGTH dynamic_size (CREF<ARG1>...obj) {
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
		imports LENGTH dynamic_size (CREF<ARG1>...obj) {
			using R1X = TYPEAS<ARG1...> ;
			require (ENUM_EQ_IDEN<COUNT_OF<R1X>>) ;
			using R2X = TYPE_FIRST_ONE<R1X> ;
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

template <class UNIT1>
trait CREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename VREF_HOLDER_HELP<DEPEND ,ALWAYS>::VRef ;

	template <class ARG1>
	using MACRO_VRef = typename DEPENDENT<VREF_HELP<UNIT1 ,ALWAYS> ,ARG1>::VRef ;

	template <class ARG1>
	using MACRO_CRef = typename DEPENDENT<CREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::CRef ;

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
		implicit CRef (RREF<ARG1> that) : CRef (forward[TYPEAS<CRef>::id] (move (that).rebind (TYPEAS<UNIT1>::id))) {}

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
				ret.fake.acquire (unsafe_cast[TYPEAS<TEMP<void>>::id] (unsafe_deptr (rax.self))) ;
				rax.release () ;
				ret.mPointer = ret.fake.addr () ;
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
		MACRO_CRef<REMOVE_ALL<ARG1>> rebind (CREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_EXTEND<R1X ,UNIT1>) ;
			MACRO_CRef<R1X> ret ;
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
		virtual void acquire (CREF<TEMP<void>> obj) = 0 ;
		virtual void release () = 0 ;
		virtual FLAG addr () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
	} ;

	using AUTO_MAX_SIZE = ENUMAS<VAL ,ENUMID<1024>> ;
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
	using SUPER = typename AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::Auto ;

	class Auto final extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mGood ;

	public:
		implicit Auto () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
		implicit Auto (RREF<ARG1> that) noexcept {
			using R1X = ARG1 ;
			require (IS_OBJECT<R1X>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<R1X> ,SIZE_OF<FakeHolder>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<FakeHolder>>) ;
			using R2X = typename AUTO_IMPLHOLDER_HELP<R1X ,ALWAYS>::ImplHolder ;
			mThis.acquire (TYPEAS<R2X>::id) ;
			if ifswitch (TRUE) {
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (that)) ;
				mThis->acquire (unsafe_cast[TYPEAS<TEMP<void>>::id] (unsafe_deptr (rax.self))) ;
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

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			if ifnot (mGood)
				return FALSE ;
			return TRUE ;
		}

		template <class ARG1>
		REMOVE_ALL<ARG1> poll (CREF<ARG1> id) rightvalue noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			assert (exist ()) ;
			const auto r1x = mThis->type_cabi () ;
			const auto r2x = operator_cabi (id) ;
			assert (r1x == r2x) ;
			const auto r3x = mThis->addr () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R1X>>::id] (unsafe_pointer (r3x))) ;
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
trait BINDER_HELP ;

template <class...UNIT1>
trait BINDER_HELP<TYPEAS<UNIT1...> ,ALWAYS> {
	using PARAMS = TYPEAS<UNIT1...> ;
	using SIZE = COUNT_OF<PARAMS> ;

	class Binder {
	protected:
		ARR<FLAG ,SIZE> mParams ;

	public:
		explicit Binder (CREF<DEPENDENT<FLAG ,UNIT1>>...params) {
			INDEX ix = 0 ;
			assign (ix ,params...) ;
		}

		void assign (VREF<INDEX> iw) {
			assert (iw == SIZE::value) ;
		}

		template <class ARG1 ,class...ARG2 >
		void assign (VREF<INDEX> iw ,CREF<ARG1> param1 ,CREF<ARG2>...param2) {
			assert (iw < SIZE::value) ;
			mParams[iw++] = param1 ;
			assign (iw ,param2...) ;
		}

		template <class ARG1>
		inline FUNCTION_RETURN<ARG1> operator() (CREF<ARG1> func) const {
			INDEX ix = 0 ;
			return template_invoke (PHX ,func ,TYPEAS<PARAMS>::id ,ix) ;
		}

	private:
		template <class ARG1 ,class ARG2 ,class...ARG3 ,class = REQUIRE<ENUM_EQ_ZERO<COUNT_OF<REMOVE_ALL<ARG2>>>>>
		FUNCTION_RETURN<ARG1> template_invoke (CREF<typeof (PH1)> ,CREF<ARG1> func ,CREF<ARG2> id ,VREF<INDEX> iw ,CREF<ARG3>...params) const {
			return func (params...) ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3 ,class = REQUIRE<ENUM_GT_ZERO<COUNT_OF<REMOVE_ALL<ARG2>>>>>
		FUNCTION_RETURN<ARG1> template_invoke (CREF<typeof (PH2)> ,CREF<ARG1> func ,CREF<ARG2> id ,VREF<INDEX> iw ,CREF<ARG3>...params) const {
			using R1X = REMOVE_ALL<ARG2> ;
			using R2X = TYPE_FIRST_ONE<R1X> ;
			using R3X = TYPE_FIRST_REST<R1X> ;
			const auto r1x = mParams[iw++] ;
			auto &&tmp = keep[TYPEAS<CREF<R2X>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::id] (unsafe_pointer (r1x)))) ;
			return template_invoke (PHX ,func ,TYPEAS<R3X>::id ,iw ,params... ,tmp) ;
		}
	} ;
} ;

template <class...PARAMS>
using Binder = typename BINDER_HELP<TYPEAS<PARAMS...> ,ALWAYS>::Binder ;

struct FUNCTION_bind {
	template <class...ARG1>
	inline Binder<ARG1...> operator() (CREF<ARG1>...obj) const {
		return Binder<ARG1...> (address (obj)...) ;
	}
} ;

static constexpr auto bind = FUNCTION_bind () ;

template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class ITEM>
trait SLICE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	struct Holder implement Interface {
		virtual LENGTH size () const = 0 ;
		virtual ITEM at (CREF<INDEX> index) const = 0 ;
	} ;

	class Slice {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Slice>>>>
		explicit Slice (CREF<ARG1> id ,CREF<ARG2>...text) {
			require (ENUM_ALL<IS_ARRAY<ARG2>...>) ;
			using R1X = COUNT_OF<TYPEAS<ARG2...>> ;
			using R3X = typename SLICE_IMPLHOLDER_HELP<ITEM ,R1X ,ALWAYS>::ImplHolder ;
			//@warn: fuck g++4.8
			const auto r1x = bind (text...) ;
			mThis = memorize ([&] () {
				return r1x ([&] (CREF<ARG2>...text) {
					return CRef<R3X>::make (text...).rebind (TYPEAS<Holder>::id) ;
				}) ;
			}) ;
		}

		imports CREF<Slice> zero () {
			return memorize ([&] () {
				return Slice (TYPEAS<where>::id ,"") ;
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
	} ;
} ;

template <class ITEM ,class RANK>
trait SLICE_IMPLHOLDER_HELP<ITEM ,RANK ,ALWAYS> {
	using Holder = typename SLICE_HELP<ITEM ,ALWAYS>::Holder ;

	struct NODE {
		FLAG mPointer ;
		LENGTH mAlign ;
		LENGTH mCount ;
	} ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mSize ;
		Box<ARR<NODE ,RANK>> mSlice ;

	public:
		implicit ImplHolder () = delete ;

		template <class...ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<TYPE_FIRST_ONE<TYPEAS<ARG1...>> ,ImplHolder>>>>
		explicit ImplHolder (CREF<ARG1>...text) {
			mSize = 0 ;
			mSlice = Box<ARR<NODE ,RANK>>::make () ;
			INDEX ix = 0 ;
			template_write (ix ,text...) ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		ITEM at (CREF<INDEX> index) const override {
			INDEX ix = index ;
			for (auto &&i : iter (0 ,RANK::value)) {
				if ifswitch (TRUE) {
					if ifnot (vbetween (ix ,0 ,mSlice.self[i].mCount))
						discard ;
					const auto r1x = mSlice.self[i].mPointer + ix * mSlice.self[i].mAlign ;
					if (mSlice.self[i].mAlign == 1)
						return ITEM (unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU8>>::id] (unsafe_pointer (r1x)))) ;
					if (mSlice.self[i].mAlign == 2)
						return ITEM (unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU16>>::id] (unsafe_pointer (r1x)))) ;
					if (mSlice.self[i].mAlign == 4)
						return ITEM (unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU32>>::id] (unsafe_pointer (r1x)))) ;
					assert (FALSE) ;
				}
				ix -= mSlice.self[i].mCount ;
			}
			return bad (TYPEAS<ITEM>::id) ;
		}

	private:
		void template_write (VREF<INDEX> iw) {
			assert (iw == RANK::value) ;
		}

		template <class ARG1 ,class...ARG2>
		void template_write (VREF<INDEX> iw ,CREF<ARG1> text1 ,CREF<ARG2>...text2) {
			using R1X = ARG1 ;
			using R2X = ARRAY_ITEM<R1X> ;
			require (ENUM_ANY<IS_SAME<R2X ,STRA> ,IS_SAME<R2X ,ITEM>>) ;
			using R3X = ARRAY_SIZE<R1X> ;
			assert (iw < RANK::value) ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (jx >= R3X::value)
					break ;
				if (ITEM (text1[jx]) == ITEM (0))
					break ;
				jx++ ;
			}
			INDEX ix = iw++ ;
			mSlice.self[ix].mPointer = address (text1) ;
			mSlice.self[ix].mAlign = ALIGN_OF<R2X>::value ;
			mSlice.self[ix].mCount = jx ;
			mSize += jx ;
			template_write (iw ,text2...) ;
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
			auto &&tmp = memorize ([&] () {
				Box<ARR<STR ,R1X>> ret = Box<ARR<STR ,R1X>>::make () ;
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
			return Slice<STR> (TYPEAS<UNIT1>::id ,tmp.self) ;
		}

		csc_text_t internel_name () const {
			require (IS_REGISTER<UNIT1>) ;
			const auto r1x = FUNCTION_internel_name () ;
			return r1x (TYPEAS<UNIT1>::id) ;
		}
	} ;
} ;

struct FUNCTION_operator_name {
	template <class ARG1>
	inline Slice<STR> operator() (CREF<ARG1> id) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = typename FUNCTION_operator_name_HELP<R1X ,ALWAYS>::FUNCTION_operator_name ;
		const auto r1x = R2X () ;
		return memorize ([&] () {
			return r1x () ;
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

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Clazz>>>>
		explicit Clazz (CREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_REGISTER<R1X>) ;
			using R2X = typename CLAZZ_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			mThis = memorize ([&] () {
				return CRef<R2X>::make (id).rebind (TYPEAS<Holder>::id) ;
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

template <class DEPEND>
trait CLAZZ_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mTypeSize ;
		LENGTH mTypeAlign ;
		FLAG mTypeCABI ;
		Slice<STR> mTypeName ;

	public:
		implicit ImplHolder () = delete ;

		template <class ARG1>
		explicit ImplHolder (CREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			mTypeSize = SIZE_OF<R1X>::value ;
			mTypeAlign = ALIGN_OF<R1X>::value ;
			mTypeCABI = operator_cabi (TYPEAS<R1X>::id) ;
			mTypeName = operator_name (TYPEAS<R1X>::id) ;
		}

		LENGTH type_size () const override {
			return mTypeSize ;
		}

		LENGTH type_align () const override {
			return mTypeAlign ;
		}

		FLAG type_cabi () const override {
			return mTypeCABI ;
		}

		Slice<STR> type_name () const override {
			return mTypeName ;
		}
	} ;
} ;

using Clazz = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Clazz ;

template <class...>
trait WATCH_HELP ;

template <class UUID>
trait WATCH_HELP<UUID ,REQUIRE<IS_REGISTER<UUID>>> {
	struct WATCH implement Interface {
		Slice<STR> mName ;
		FLAG mAddress ;
		Clazz mClazz ;
	} ;
} ;

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class ARG2>
	inline forceinline void operator() (CREF<ARG1> id ,CREF<Slice<STR>> name ,CREF<ARG2> expr) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = ARG2 ;
		using R3X = typename WATCH_HELP<R1X ,ALWAYS>::WATCH ;
		static auto mInstance = R3X () ;
		mInstance.mName = name ;
		mInstance.mAddress = address (expr) ;
		mInstance.mClazz = Clazz (TYPEAS<R2X>::id) ;
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
		virtual Slice<STR> what () const = 0 ;
	} ;

	class Exception {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Exception () = default ;

		template <class ARG1>
		explicit Exception (CREF<ARG1> id ,CREF<Slice<STR>> what_) noexcept {
			using R2X = typename EXCEPTION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			mThis = memorize ([&] () {
				return CRef<R2X>::make (what_).rebind (TYPEAS<Holder>::id) ;
			}) ;
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

		void raise () {
			throw thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait EXCEPTION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename EXCEPTION_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Slice<STR> mWhat ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (CREF<Slice<STR>> what_) {
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
	inline void operator() (CREF<ARG1> proc1) const noexcept {
		using R1X = ARG1 ;
		require (IS_VOID<FUNCTION_RETURN<R1X>>) ;
		return proc1 () ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (CREF<ARG1> proc1 ,CREF<ARG2>...proc2) const {
		using R1X = ARG1 ;
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

		explicit Scope (VREF<UNIT1> that) {
			const auto r1x = address (that) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (r1x))) ;
			tmp.enter () ;
			mPointer = r1x ;
		}

		explicit Scope (CREF<UNIT1> that) {
			const auto r1x = address (that) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (r1x))) ;
			tmp.enter () ;
			mPointer = r1x ;
		}

		explicit Scope (RREF<UNIT1>) = delete ;

		implicit ~Scope () noexcept {
			if (mPointer == ZERO)
				return ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
			try_invoke ([&] () {
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