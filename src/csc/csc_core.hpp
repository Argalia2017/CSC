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
	inline void operator() (CREF<ARG1> a) const noexcept {
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

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__debugbreak () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_abort {
	inline void operator() () const {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_abort {
	inline void operator() () const {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_abort {
	inline void operator() () const {
		throw ;
	}
} ;
#endif

static constexpr auto unsafe_abort = FUNCTION_unsafe_abort () ;

#define unsafe_launder(...)

template <class...>
trait TEMPLATE_unsafe_cast_HELP ;

template <class A>
trait TEMPLATE_unsafe_cast_HELP<A ,ALWAYS> {
	struct TEMPLATE_unsafe_cast {
		template <class ARG1>
		inline VREF<A> operator() (VREF<ARG1> a) const noexcept {
			using R1X = A ;
			using R2X = ARG1 ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<VREF<R1X>> (a) ;
		}

		template <class ARG1>
		inline CREF<A> operator() (CREF<ARG1> a) const noexcept {
			using R1X = A ;
			using R2X = ARG1 ;
			using R3X = ENUM_ANY<IS_SAME<R1X ,TEMP<void>> ,IS_SAME<R2X ,TEMP<void>>> ;
			using R4X = CONDITIONAL<R3X ,BYTE ,R1X> ;
			using R5X = CONDITIONAL<R3X ,BYTE ,R2X> ;
			require (ENUM_EQUAL<SIZE_OF<R4X> ,SIZE_OF<R5X>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<R4X> ,ALIGN_OF<R5X>>) ;
			return reinterpret_cast<CREF<R1X>> (a) ;
		}

		template <class ARG1>
		inline CREF<A> operator() (RREF<ARG1>) const noexcept = delete ;
	} ;
} ;

template <class A>
using TEMPLATE_unsafe_cast = typename TEMPLATE_unsafe_cast_HELP<A ,ALWAYS>::TEMPLATE_unsafe_cast ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	inline consteval TEMPLATE_unsafe_cast<ARG1> operator[] (TYPEID<ARG1> id) const noexcept {
		return TEMPLATE_unsafe_cast<ARG1> () ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_address {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&unsafe_cast[TYPE<TEMP<void>>::expr] (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_unsafe_pointer {
	inline VREF<TEMP<void>> operator() (CREF<FLAG> addr) const noexcept {
		assert (addr != ZERO) ;
		const auto r1x = reinterpret_cast<VPTR<TEMP<void>>> (addr) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_pointer = FUNCTION_unsafe_pointer () ;

struct FUNCTION_unsafe_array {
	template <class ARG1>
	inline VREF<ARR<ARG1>> operator() (VREF<ARG1> a) const noexcept {
		const auto r1x = reinterpret_cast<VPTR<ARR<ARG1>>> (address (a)) ;
		return (*r1x) ;
	}

	template <class ARG1>
	inline CREF<ARR<ARG1>> operator() (CREF<ARG1> a) const noexcept {
		const auto r1x = reinterpret_cast<CPTR<ARR<ARG1>>> (address (a)) ;
		return (*r1x) ;
	}

	template <class ARG1>
	inline VREF<ARR<ARG1>> operator() (TYPEID<ARG1> id) const noexcept {
		const auto r1x = reinterpret_cast<VPTR<ARR<ARG1>>> (ZERO) ;
		return (*r1x) ;
	}
} ;

static constexpr auto unsafe_array = FUNCTION_unsafe_array () ;

struct FUNCTION_swap {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj1 ,VREF<ARG1> obj2) const noexcept {
		//@warn: no class should depend on its address
		auto &&tmp1 = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (obj1) ;
		auto &&tmp2 = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (obj2) ;
		const auto r1x = tmp1 ;
		tmp1 = tmp2 ;
		tmp2 = r1x ;
		unsafe_launder (obj1) ;
		unsafe_launder (obj2) ;
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
trait TEMPLATE_keep_HELP ;

template <class A>
trait TEMPLATE_keep_HELP<A ,ALWAYS> {
	struct TEMPLATE_keep {
		template <class ARG1>
		inline XREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<XREF<A>> (a) ;
		}

		template <class ARG1>
		inline XREF<A> operator() (CREF<ARG1> a) const noexcept {
			return static_cast<XREF<A>> (a) ;
		}

		template <class ARG1>
		inline XREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<XREF<A>> (a) ;
		}
	} ;
} ;

template <class A>
using TEMPLATE_keep = typename TEMPLATE_keep_HELP<A ,ALWAYS>::TEMPLATE_keep ;

struct FUNCTION_keep {
	template <class ARG1>
	inline consteval TEMPLATE_keep<ARG1> operator[] (TYPEID<ARG1> id) const noexcept {
		return TEMPLATE_keep<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	inline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	inline ARG1 operator() (CREF<ARG1> a) const {
		require (IS_CLONEABLE<ARG1>) ;
		return keep[TYPE<CREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	inline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_init {
	template <class ARG1>
	inline void operator() (VREF<ARG1> a) const noexcept {
		require (IS_TRIVIAL<ARG1>) ;
		//@mark
	}
} ;

static constexpr auto init = FUNCTION_init () ;

struct FUNCTION_drop {
	template <class ARG1>
	inline void operator() (VREF<ARG1> a) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R1X = TEMP_BASE<ARG1> ;
		auto &&tmp1 = unsafe_cast[TYPE<R1X>::expr] (a) ;
		tmp1.~R1X () ;
	}
} ;

static constexpr auto drop = FUNCTION_drop () ;

template <class...>
trait TEMPLATE_bitwise_HELP ;

template <class A>
trait TEMPLATE_bitwise_HELP<A ,ALWAYS> {
	struct TEMPLATE_bitwise {
		template <class ARG1>
		inline A operator() (CREF<ARG1> a) const noexcept {
			using R1X = A ;
			using R2X = ARG1 ;
			require (IS_TRIVIAL<R1X>) ;
			require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
			using R3X = Storage<SIZE_OF<R1X>> ;
			A ret ;
			auto &&tmp1 = unsafe_cast[TYPE<R3X>::expr] (ret) ;
			auto &&tmp2 = unsafe_cast[TYPE<R3X>::expr] (a) ;
			tmp1 = tmp2 ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class A>
using TEMPLATE_bitwise = typename TEMPLATE_bitwise_HELP<A ,ALWAYS>::TEMPLATE_bitwise ;

struct FUNCTION_bitwise {
	template <class ARG1>
	inline BYTE_BASE<ARG1> operator() (CREF<ARG1> a) const noexcept {
		using R1X = BYTE_BASE<ARG1> ;
		return thiz[TYPE<R1X>::expr] (a) ;
	}

	template <class ARG1>
	inline consteval TEMPLATE_bitwise<ARG1> operator[] (TYPEID<ARG1> id) const noexcept {
		return TEMPLATE_bitwise<ARG1> () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_bad {
	template <class ARG1>
	inline XREF<ARG1> operator() (TYPEID<ARG1> id) const noexcept {
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

struct FUNCTION_vmin {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const noexcept {
		if (obj1 <= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto operator_min = FUNCTION_vmin () ;

struct FUNCTION_vmax {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const noexcept {
		if (obj1 >= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto operator_max = FUNCTION_vmax () ;

struct FUNCTION_vbetween {
	inline BOOL operator() (CREF<VAL> curr ,CREF<VAL> begin_ ,CREF<VAL> end_) const noexcept {
		if (curr < begin_)
			return FALSE ;
		if (curr >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto operator_between = FUNCTION_vbetween () ;

struct FUNCTION_valign {
	inline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const noexcept {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto operator_alignas = FUNCTION_valign () ;

template <class...>
trait INDEXITERATOR_HELP ;

template <>
trait INDEXITERATOR_HELP<ALWAYS> {
	class IndexIterator {
	protected:
		LENGTH mRank ;
		INDEX mBegin ;
		INDEX mEnd ;

	public:
		implicit IndexIterator () = delete ;

		explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			mBegin = begin_ ;
			mEnd = operator_max (begin_ ,end_) ;
			mRank = mEnd - mBegin ;
		}

		IndexIterator begin () const {
			return thiz ;
		}

		IndexIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mRank ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<IndexIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<IndexIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<INDEX> peek () const leftvalue {
			return mBegin ;
		}

		inline CREF<INDEX> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mBegin++ ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

using IndexIterator = typename INDEXITERATOR_HELP<ALWAYS>::IndexIterator ;

template <class...>
trait PIXELITERATOR_HELP ;

template <>
trait PIXELITERATOR_HELP<ALWAYS> {
	struct PIXEL {
		INDEX x ;
		INDEX y ;
	} ;

	class PixelIterator {
	protected:
		LENGTH mRank ;
		PIXEL mBegin ;
		PIXEL mEnd ;

	public:
		implicit PixelIterator () = delete ;

		explicit PixelIterator (CREF<PIXEL> begin_ ,CREF<PIXEL> end_) {
			mBegin = begin_ ;
			mEnd.x = operator_min (begin_.x ,end_.x) ;
			mEnd.y = operator_min (begin_.y ,end_.y) ;
			mRank = (mEnd.x - mBegin.x) * (mEnd.y - mBegin.y) ;
			if (mRank > 0)
				return ;
			mBegin = mEnd ;
		}

		PixelIterator begin () const {
			return thiz ;
		}

		PixelIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mRank ;
		}

		BOOL bad () const {
			return mBegin.y == mEnd.y ;
		}

		inline BOOL operator== (CREF<PixelIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<PixelIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<PIXEL> peek () const leftvalue {
			return mBegin ;
		}

		inline CREF<PIXEL> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mBegin.x++ ;
			if (mBegin.x < mEnd.x)
				return ;
			mBegin.x = 0 ;
			mBegin.y++ ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

using PIXEL = typename PIXELITERATOR_HELP<ALWAYS>::PIXEL ;
using PixelIterator = typename PIXELITERATOR_HELP<ALWAYS>::PixelIterator ;

struct FUNCTION_iter {
	inline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	inline PixelIterator operator() (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) const noexcept {
		const auto r1x = PIXEL ({begin_x ,begin_y}) ;
		const auto r2x = PIXEL ({end_x ,end_y}) ;
		return PixelIterator (r1x ,r2x) ;
	}
} ;

static constexpr auto iter = FUNCTION_iter () ;

template <class...>
trait FUNCTION_operator_equal_HELP ;

template <class A>
trait FUNCTION_operator_equal_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<A> obj1 ,CREF<A> obj2) const noexcept {
			return obj1 == obj2 ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_equal_HELP<A ,REQUIRE<IS_INTPTR<A>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<A> obj1 ,CREF<A> obj2) const noexcept {
			using R1X = typename FUNCTION_operator_equal_HELP<FLAG ,ALWAYS>::FUNCTION_operator_equal ;
			const auto r1x = R1X () ;
			return r1x (FLAG (obj1) ,FLAG (obj2)) ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_equal_HELP<A ,REQUIRE<IS_CLASS<A>>> {
	struct FUNCTION_operator_equal {
		inline BOOL operator() (CREF<A> obj1 ,CREF<A> obj2) const {
			return obj1.equal (obj2) ;
		}
	} ;
} ;

struct FUNCTION_operator_equal {
	template <class ARG1>
	inline BOOL operator() (CREF<ARG1> obj1 ,CREF<ARG1> obj2) const {
		using R1X = typename FUNCTION_operator_equal_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_equal ;
		const auto r1x = R1X () ;
		return r1x (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_equal = FUNCTION_operator_equal () ;

template <class...>
trait FUNCTION_operator_compr_HELP ;

template <class A>
trait FUNCTION_operator_compr_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<A> obj1 ,CREF<A> obj2) const noexcept {
			if (obj1 < obj2)
				return NONE ;
			if (obj2 < obj1)
				return IDEN ;
			return ZERO ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_compr_HELP<A ,REQUIRE<IS_INTPTR<A>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<A> obj1 ,CREF<A> obj2) const noexcept {
			using R1X = typename FUNCTION_operator_compr_HELP<FLAG ,ALWAYS>::FUNCTION_operator_compr ;
			const auto r1x = R1X () ;
			return r1x (FLAG (obj1) ,FLAG (obj2)) ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_compr_HELP<A ,REQUIRE<IS_CLASS<A>>> {
	struct FUNCTION_operator_compr {
		inline FLAG operator() (CREF<A> obj1 ,CREF<A> obj2) const {
			return obj1.compr (obj2) ;
		}
	} ;
} ;

struct FUNCTION_operator_compr {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> obj1 ,CREF<ARG1> obj2) const {
		using R3X = typename FUNCTION_operator_compr_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_compr ;
		const auto r1x = R3X () ;
		return r1x (obj1 ,obj2) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

#ifdef __CSC_CONFIG_VAL32__
struct FUNCTION_hashcode {
	inline FLAG operator() () const noexcept {
		return FLAG (-2128831035) ;
	}

	inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const noexcept {
		using R1X = BYTE_BASE<FLAG> ;
		const auto r1x = R1X (now) ^ R1X (inc) ;
		const auto r2x = R1X (FLAG (r1x) * FLAG (16777619)) ;
		const auto r3x = r2x & R1X (VAL_MAX) ;
		return FLAG (r3x) ;
	}
} ;
#endif

#ifdef __CSC_CONFIG_VAL64__
struct FUNCTION_hashcode {
	inline FLAG operator() () const noexcept {
		return FLAG (-3750763034362895579) ;
	}

	inline FLAG operator() (CREF<FLAG> now ,CREF<FLAG> inc) const noexcept {
		using R1X = BYTE_BASE<FLAG> ;
		const auto r1x = R1X (now) ^ R1X (inc) ;
		const auto r2x = R1X (FLAG (r1x) * FLAG (1099511628211)) ;
		const auto r3x = r2x & R1X (VAL_MAX) ;
		return FLAG (r3x) ;
	}
} ;
#endif

static constexpr auto hashcode = FUNCTION_hashcode () ;

template <class...>
trait FUNCTION_operator_hash_HELP ;

template <class A>
trait FUNCTION_operator_hash_HELP<A ,REQUIRE<IS_BASIC<A>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<A> a) const noexcept {
			using R1X = KILL<BYTE_BASE<FLAG> ,A> ;
			const auto r1x = R1X (bitwise (a)) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_hash_HELP<A ,REQUIRE<IS_INTPTR<A>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<A> a) const noexcept {
			using R1X = typename FUNCTION_operator_hash_HELP<FLAG ,ALWAYS>::FUNCTION_operator_hash ;
			const auto r1x = R1X () ;
			return r1x (FLAG (a)) ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_hash_HELP<A ,REQUIRE<IS_CLASS<A>>> {
	struct FUNCTION_operator_hash {
		inline FLAG operator() (CREF<A> a) const {
			using R1X = KILL<BYTE_BASE<FLAG> ,A> ;
			const auto r1x = R1X (a.hash ()) ;
			const auto r2x = r1x & R1X (VAL_MAX) ;
			return FLAG (r2x) ;
		}
	} ;
} ;

struct FUNCTION_operator_hash {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> a) const {
		using R1X = typename FUNCTION_operator_hash_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_hash ;
		const auto r1x = R1X () ;
		return r1x (a) ;
	}
} ;

static constexpr auto operator_hash = FUNCTION_operator_hash () ;

struct FUNCTION_operator_cabi {
	template <class ARG1>
	inline FLAG operator() (TYPEID<ARG1> id) const noexcept {
		static Interface mInstance ;
		return FLAG ((&mInstance)) ;
	}
} ;

static constexpr auto operator_cabi = FUNCTION_operator_cabi () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> curr ,CREF<ARG1> expect ,XREF<ARG2> next) const {
		if ifnot (operator_equal (curr ,expect))
			return ;
		curr = keep[TYPE<ARG2>::expr] (next) ;
	}
} ;

static constexpr auto replace = FUNCTION_replace () ;

struct FUNCTION_interface_vptr {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> a) const noexcept {
		require (IS_POLYMORPHIC<ARG1>) ;
		const auto r1x = address (a) ;
		return unsafe_cast[TYPE<FLAG>::expr] (unsafe_pointer (r1x)) ;
	}
} ;

static constexpr auto interface_vptr = FUNCTION_interface_vptr () ;

template <class A>
class VFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	inline VPTR<A> operator-> () rightvalue {
		return unsafe_cast[TYPE<A>::expr] (mHolder) ;
	}
} ;

template <class A>
class CFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	inline CPTR<A> operator-> () rightvalue {
		return unsafe_cast[TYPE<A>::expr] (mHolder) ;
	}
} ;

template <class A>
class RFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	inline implicit operator A () rightvalue {
		return move (unsafe_cast[TYPE<A>::expr] (mHolder)) ;
	}
} ;

template <class...>
trait BOX_HELP ;

class BoxBase {} ;

template <class...>
trait BOX_HOLDER_HELP ;

template <class A>
trait BOX_HOLDER_HELP<A ,REQUIRE<IS_TRIVIAL<A>>> {
	class Box implement BoxBase {
	protected:
		TEMP<A> mValue ;
	} ;
} ;

template <class A>
trait BOX_HOLDER_HELP<A ,REQUIRE<IS_POLYMORPHIC<A>>> {
	class Box implement BoxBase {
	protected:
		TEMP<A> mValue ;

	public:
		implicit Box () noexcept {
			init (mValue) ;
		}

		implicit ~Box () noexcept {
			const auto r1x = interface_vptr (fake) ;
			if (r1x == ZERO)
				return ;
			drop (mValue) ;
			init (mValue) ;
		}

	private:
		VREF<A> fake_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}
	} ;
} ;

template <class A>
trait BOX_HOLDER_HELP<A ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<A>> ,ENUM_NOT<IS_POLYMORPHIC<A>>>>> {
	class Box implement BoxBase {
	protected:
		TEMP<A> mValue ;
		BOOL mExist ;

	public:
		implicit Box () noexcept {
			mExist = FALSE ;
		}

		implicit ~Box () noexcept {
			if ifnot (mExist)
				return ;
			drop (mValue) ;
			mExist = FALSE ;
		}
	} ;
} ;

template <class A>
trait BOX_HELP<A ,REQUIRE<ENUM_ALL<IS_TRIVIAL<A> ,IS_OBJECT<A>>>> {
	using Super = typename BOX_HOLDER_HELP<A ,ALWAYS>::Box ;

	class Box final implement Super {
	protected:
		using Super::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...a) {
			Box ret ;
			ret.remake (TYPE<A>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
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

		VREF<A> self_m () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline VPTR<A> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<A> self_m () const leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<BoxBase> a) {
			assert (ifnot (exist ())) ;
			auto &&tmp1 = keep[TYPE<CREF<Box>>::expr] (a) ;
			mValue = tmp1.mValue ;
			unsafe_launder (fake) ;
		}

		template <class ARG1 ,class...ARG2>
		void remake (TYPEID<ARG1> id ,XREF<ARG2>...a) {
			require (IS_SAME<A ,ARG1>) ;
			assert (ifnot (exist ())) ;
			new ((&mValue)) ARG1 (keep[TYPE<ARG2>::expr] (a)...) ;
			unsafe_launder (fake) ;
		}

		void release () {
			noop () ;
		}

	private:
		VREF<A> fake_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}

		CREF<A> fake_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}
	} ;
} ;

template <class A>
trait BOX_HELP<A ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<A>> ,ENUM_NOT<IS_POLYMORPHIC<A>> ,IS_OBJECT<A>>>> {
	using Super = typename BOX_HOLDER_HELP<A ,ALWAYS>::Box ;

	class Box final implement Super {
	protected:
		using Super::mValue ;
		using Super::mExist ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...a) {
			Box ret ;
			ret.remake (TYPE<A>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
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

		VREF<A> self_m () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline VPTR<A> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<A> self_m () const leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<BoxBase> a) {
			assert (ifnot (exist ())) ;
			auto &&tmp1 = keep[TYPE<CREF<Box>>::expr] (a) ;
			mValue = tmp1.mValue ;
			unsafe_launder (fake) ;
			mExist = TRUE ;
		}

		template <class ARG1 ,class...ARG2>
		void remake (TYPEID<ARG1> id ,XREF<ARG2>...a) {
			require (IS_SAME<A ,ARG1>) ;
			assert (ifnot (exist ())) ;
			init (mValue) ;
			new ((&mValue)) ARG1 (keep[TYPE<ARG2>::expr] (a)...) ;
			unsafe_launder (fake) ;
			mExist = TRUE ;
		}

		void release () {
			mExist = FALSE ;
		}

	private:
		VREF<A> fake_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}

		CREF<A> fake_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}
	} ;
} ;

template <class A>
trait BOX_HELP<A ,REQUIRE<IS_POLYMORPHIC<A>>> {
	using Super = typename BOX_HOLDER_HELP<A ,ALWAYS>::Box ;

	class Box final implement Super {
	protected:
		using Super::mValue ;

	public:
		implicit Box () = default ;

		implicit Box (CREF<typeof (NULL)>) {}

		template <class...ARG1>
		imports Box make (XREF<ARG1>...a) {
			Box ret ;
			ret.remake (TYPE<A>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
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
			const auto r1x = interface_vptr (fake) ;
			return r1x != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) const {
			return ifnot (exist ()) ;
		}

		inline BOOL operator!= (CREF<typeof (NULL)>) const {
			return exist () ;
		}

		VREF<A> self_m () leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline VPTR<A> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<A> self_m () const leftvalue {
			assert (exist ()) ;
			return fake ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

		void acquire (CREF<BoxBase> a) {
			assert (ifnot (exist ())) ;
			auto &&tmp1 = keep[TYPE<CREF<Box>>::expr] (a) ;
			mValue = tmp1.mValue ;
			unsafe_launder (fake) ;
		}

		template <class ARG1 ,class...ARG2>
		void remake (TYPEID<ARG1> id ,XREF<ARG2>...a) {
			require (IS_POLYMORPHIC<ARG1>) ;
			require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,SIZE_OF<A>>) ;
			require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,ALIGN_OF<A>>) ;
			assert (ifnot (exist ())) ;
			init (mValue) ;
			new ((&mValue)) ARG1 (keep[TYPE<ARG2>::expr] (a)...) ;
			unsafe_launder (fake) ;
		}

		void release () {
			init (mValue) ;
		}

	private:
		VREF<A> fake_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}

		CREF<A> fake_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (mValue) ;
		}
	} ;
} ;

template <class A>
using Box = typename BOX_HELP<A ,ALWAYS>::Box ;

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

template <>
trait HEAPPROC_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual LENGTH usage_size () const = 0 ;
		virtual LENGTH basic_align () const = 0 ;
		virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
		virtual void free (CREF<FLAG> addr) const = 0 ;
	} ;

	class FakeHolder implement Holder {} ;

	struct FakeImplHolder {
		imports Box<FakeHolder> create () ;
	} ;

	class HeapProc {
	protected:
		Box<FakeHolder> mThis ;

	public:
		imports CREF<HeapProc> instance () {
			return memorize ([&] () {
				HeapProc ret ;
				ret.mThis = FakeImplHolder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports LENGTH usage_size () {
			return instance ().mThis->usage_size () ;
		}

		imports LENGTH basic_align () {
			return instance ().mThis->basic_align () ;
		}

		imports FLAG alloc (CREF<LENGTH> size_) {
			return instance ().mThis->alloc (size_) ;
		}

		imports void free (CREF<FLAG> addr) {
			return instance ().mThis->free (addr) ;
		}
	} ;
} ;

using HeapProc = typename HEAPPROC_HELP<ALWAYS>::HeapProc ;

template <class...>
trait XREF_HELP ;

template <class...>
trait XREF_HOLDER_HELP ;

template <class...>
trait XREF_PUREHOLDER_HELP ;

template <>
trait XREF_HOLDER_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void destroy () = 0 ;
		virtual void acquire (CREF<BoxBase> a) = 0 ;
		virtual void recycle (VREF<BoxBase> a) = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
		virtual void lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

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
			//@mark
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_cast[TYPE<Holder>::expr] (unsafe_pointer (mHolder)) ;
		}
	} ;
} ;

template <>
trait XREF_PUREHOLDER_HELP<ALWAYS> {
	using Holder = typename XREF_HOLDER_HELP<ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;

	public:
		imports FLAG create (CREF<LENGTH> size_ ,CREF<LENGTH> align_) {
			const auto r1x = HeapProc::basic_align () ;
			const auto r2x = operator_max (ALIGN_OF<PureHolder>::expr - r1x ,0) ;
			const auto r3x = operator_alignas (SIZE_OF<PureHolder>::expr ,align_) ;
			const auto r4x = r2x + r3x ;
			const auto r5x = HeapProc::alloc (r4x) ;
			const auto r6x = operator_alignas (r5x ,ALIGN_OF<PureHolder>::expr) ;
			auto &&tmp1 = unsafe_cast[TYPE<Box<PureHolder>>::expr] (unsafe_pointer (r6x)) ;
			tmp1.release () ;
			tmp1.remake (TYPE<PureHolder>::expr) ;
			return r6x ;
		}

		void initialize () override {
			mCounter = NONE ;
			mSize = 0 ;
		}

		void destroy () override {
			//@mark
		}

		void acquire (CREF<BoxBase> a) override {
			//@mark
		}

		void recycle (VREF<BoxBase> a) override {
			//@mark
		}

		FLAG pointer () const override {
			//@mark
			return 0 ;
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

		void lock () override {
			replace (mCounter ,IDEN ,NONE) ;
			assert (mCounter == IDEN) ;
		}

		void unlock () override {
			replace (mCounter ,NONE ,IDEN) ;
		}
	} ;
} ;

template <class A>
trait XREF_HELP<A ,REQUIRE<IS_OBJECT<A>>> {
	using Holder = typename XREF_HOLDER_HELP<ALWAYS>::Holder ;
	using Super = typename XREF_HOLDER_HELP<ALWAYS>::XRef ;

	class VRef final implement Super {
	protected:
		using Super::mHolder ;
		using Super::mPointer ;

	public:
		implicit VRef () = default ;

		implicit VRef (CREF<typeof (NULL)>) {}

		implicit VRef (RREF<Super> that) {
			//@mark
		}

		template <class...ARG1>
		imports VRef make (XREF<ARG1>...a) {
			using R1X = A ;
			using R2X = typename KILL<XREF_PUREHOLDER_HELP<ALWAYS> ,A>::PureHolder ;
			VRef ret ;
			const auto r1x = LENGTH (1) ;
			assert (r1x > 0) ;
			ret.mHolder = R2X::create (r1x * SIZE_OF<R1X>::expr ,ALIGN_OF<R1X>::expr) ;
			ret.fake.initialize () ;
			if ifswitch (TRUE) {
				auto rax = Box<A>::make (keep[TYPE<ARG1>::expr] (a)...) ;
				ret.fake.acquire (rax) ;
				rax.release () ;
			}
			ret.mPointer = ret.fake.pointer () ;
			return move (ret) ;
		}

		imports VRef reference (VREF<A> that) {
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

		VREF<A> self_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline VPTR<A> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<A> self_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_cast[TYPE<Holder>::expr] (unsafe_pointer (mHolder)) ;
		}
	} ;

	class CRef final implement Super {
	protected:
		using Super::mHolder ;
		using Super::mPointer ;

	public:
		implicit CRef () = default ;

		implicit CRef (CREF<typeof (NULL)>) {}

		implicit CRef (RREF<Super> that) {
			//@mark
		}

		template <class...ARG1>
		imports CRef make (XREF<ARG1>...a) {
			return VRef::make (keep[TYPE<ARG1>::expr] (a)...) ;
		}

		imports CRef reference (CREF<A> that) {
			CRef ret ;
			ret.mPointer = address (that) ;
			return move (ret) ;
		}

		imports CRef reference (RREF<A>) = delete ;

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

		CREF<A> self_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

	private:
		VREF<Holder> fake_m () const leftvalue {
			return unsafe_cast[TYPE<Holder>::expr] (unsafe_pointer (mHolder)) ;
		}
	} ;
} ;

template <class A>
using VRef = typename XREF_HELP<A ,ALWAYS>::VRef ;

template <class A>
using CRef = typename XREF_HELP<A ,ALWAYS>::CRef ;

template <class...>
trait CAPTURE_HELP ;

template <class...>
trait CAPTURE_HOLDER_HELP ;

template <>
trait CAPTURE_HOLDER_HELP<ALWAYS> {
	class Capture {
	protected:
		LENGTH mRank ;

	public:
		implicit Capture () noexcept {
			mRank = 0 ;
		}

		LENGTH rank () const {
			return mRank ;
		}
	} ;
} ;

template <class PARAMS>
trait CAPTURE_HELP<PARAMS ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<PARAMS>>>> {
	using Super = typename CAPTURE_HOLDER_HELP<ALWAYS>::Capture ;

	class Capture implement Super {
	protected:
		using Super::mRank ;

	public:
		implicit Capture () = default ;

		template <class ARG1>
		inline void operator() (CREF<ARG1> func) const {
			func () ;
		}
	} ;
} ;

template <class...PARAM>
trait CAPTURE_HELP<TYPE<PARAM...> ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<TYPE<PARAM...>>>>> {
	using PARAMS = TYPE<PARAM...> ;
	using RANK = COUNT_OF<PARAMS> ;
	using Super = typename CAPTURE_HOLDER_HELP<ALWAYS>::Capture ;

	class Capture implement Super {
	protected:
		using Super::mRank ;
		ARR<FLAG ,RANK> mCapture ;

	public:
		implicit Capture () = delete ;

		explicit Capture (CREF<KILL<FLAG ,PARAM>>...params) {
			//@mark
		}

		template <class ARG1>
		inline void operator() (CREF<ARG1> func) const {
			//@mark
		}
	} ;
} ;

template <class...A>
using Capture = typename CAPTURE_HELP<TYPE<A...> ,ALWAYS>::Capture ;

struct FUNCTION_capture {
	template <class...ARG1>
	inline Capture<ARG1...> operator() (CREF<ARG1>...a) const noexcept {
		return Capture<ARG1...> (address (a)...) ;
	}
} ;

static constexpr auto capture = FUNCTION_capture () ;

template <class...>
trait SLICE_HELP ;

template <class...>
trait SLICE_IMPLHOLDER_HELP ;

template <class A>
trait SLICE_HELP<A ,REQUIRE<IS_TEXT<A>>> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<csc_span_t> text) = 0 ;
		virtual void initialize (RREF<CRef<Holder>> prefix ,CREF<csc_span_t> text) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual A at (CREF<INDEX> index) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
	} ;

	class Slice {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Slice () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_ANY<IS_ARRAY<ARG1> ,IS_SAME<ARG1 ,csc_span_t>>>>
		explicit Slice (CREF<ARG1> text) {
			//@mark
		}

		template <class ARG1>
		explicit Slice (CREF<Slice> prefix ,CREF<ARG1> text) {
			//@mark
		}

		LENGTH size () const {
			if (mThis == NULL)
				return 0 ;
			return mThis->size () ;
		}

		A at (CREF<INDEX> index) const {
			return mThis->at (index) ;
		}

		inline A operator[] (CREF<INDEX> index) const {
			return at (index) ;
		}

		BOOL equal (CREF<Slice> that) const {
			const auto r1x = operator_compr (mThis == NULL ,that.mThis == NULL) ;
			if (r1x != 0)
				return FALSE ;
			if (mThis == NULL)
				return TRUE ;
			return mThis->equal (that) ;
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
			return mThis->compr (that) ;
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
} ;

template <class A>
using Slice = typename SLICE_HELP<A ,ALWAYS>::Slice ;

template <class...>
trait FUNCTION_operator_name_HELP ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,BOOL>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("BOOL") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,VAL32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("VAL32") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,VAL64>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("VAL64") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,FLT32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("FLT32") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,FLT64>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("FLT64") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,STRA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("STRA") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,STRW>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("STRW") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,STRU8>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("STRU8") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,STRU16>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("STRU16") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,STRU32>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("STRU32") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,BYTE>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("BYTE") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,WORD>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("WORD") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,CHAR>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("CHAR") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_SAME<A ,DATA>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("DATA") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_NULL<A>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("null") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<IS_VOID<A>>> {
	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return slice ("void") ;
		}
	} ;
} ;

template <class A>
trait FUNCTION_operator_name_HELP<A ,REQUIRE<ENUM_NOT<IS_BASIC<A>>>> {
	using SIZE = ENUM<1024> ;

	struct FUNCTION_operator_name {
		inline Slice<STR> operator() (CREF<csc_span_t> text) const {
			return Slice<STR> (text) ;
		}
	} ;
} ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_internel_name {
	template <class A>
	imports csc_span_t __cdecl invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__FUNCSIG__) ;
		rax.mEnd = rax.mBegin + sizeof (__FUNCSIG__) - 1 ;
		rax.mBegin += sizeof ("struct CSC::csc_span_t __cdecl CSC::FUNCTION_internel_name::invoke<") - 1 ;
		rax.mEnd -= sizeof (">(void)") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (TYPEID<ARG1> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_internel_name {
	template <class A>
	imports __attribute__ ((cdecl)) csc_span_t invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__PRETTY_FUNCTION__) ;
		rax.mEnd = rax.mBegin + sizeof (__PRETTY_FUNCTION__) - 1 ;
		rax.mBegin += sizeof ("static CSC::csc_span_t CSC::FUNCTION_internel_name::invoke() [with A = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (TYPEID<ARG1> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_internel_name {
	template <class A>
	imports csc_span_t __cdecl invoke () {
		auto rax = csc_span_t () ;
		rax.mBegin = csc_diff_t (&__PRETTY_FUNCTION__) ;
		rax.mEnd = rax.mBegin + sizeof (__PRETTY_FUNCTION__) - 1 ;
		rax.mBegin += sizeof ("static CSC::csc_span_t CSC::FUNCTION_internel_name::invoke() [A = ") - 1 ;
		rax.mEnd -= sizeof ("]") - 1 ;
		rax.mStep = 1 ;
		return rax ;
	}

	template <class ARG1>
	inline csc_span_t operator() (TYPEID<ARG1> id) const noexcept {
		return invoke<ARG1> () ;
	}
} ;
#endif

struct FUNCTION_operator_name {
	template <class ARG1>
	inline Slice<STR> operator() (TYPEID<ARG1> id) const {
		using R1X = typename FUNCTION_operator_name_HELP<ARG1 ,ALWAYS>::FUNCTION_operator_name ;
		const auto r1x = R1X () ;
		const auto r2x = FUNCTION_internel_name () ;
		return memorize ([&] () {
			return r1x (r2x (id)) ;
		}) ;
	}
} ;

static constexpr auto operator_name = FUNCTION_operator_name () ;

template <class...>
trait CLAZZ_HELP ;

template <class...>
trait CLAZZ_IMPLHOLDER_HELP ;

template <>
trait CLAZZ_HELP<ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> align_ ,CREF<FLAG> cabi ,CREF<Slice<STR>> name) = 0 ;
		virtual LENGTH type_size () const = 0 ;
		virtual LENGTH type_align () const = 0 ;
		virtual FLAG type_cabi () const = 0 ;
		virtual Slice<STR> type_name () const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
	} ;

	class Clazz {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Clazz () = default ;

		template <class ARG1>
		explicit Clazz (TYPEID<ARG1> id) {
			auto rax = Holder::create () ;
			const auto r1x = SIZE_OF<ARG1>::expr ;
			const auto r2x = ALIGN_OF<ARG1>::expr ;
			const auto r3x = operator_cabi (id) ;
			const auto r4x = operator_name (id) ;
			rax->initialize (r1x ,r2x ,r3x ,r4x) ;
			mThis = move (rax) ;
		}

		LENGTH type_size () const {
			if (mThis == NULL)
				return 0 ;
			return mThis->type_size () ;
		}

		LENGTH type_align () const {
			if (mThis == NULL)
				return 0 ;
			return mThis->type_align () ;
		}

		FLAG type_cabi () const {
			if (mThis == NULL)
				return 0 ;
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

using Clazz = typename CLAZZ_HELP<ALWAYS>::Clazz ;

template <class...>
trait WATCH_HELP ;

template <class A>
trait WATCH_HELP<A ,REQUIRE<IS_ORDINARY<A>>> {
	struct WATCH implement Interface {
		FLAG mHint ;
		CRef<A> mValue ;
		Slice<STR> mName ;
		Clazz mClazz ;
	} ;
} ;

template <class A>
using WATCH = typename WATCH_HELP<A ,ALWAYS>::WATCH ;

struct FUNCTION_unsafe_watch {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (TYPEID<ARG1> id ,CREF<Slice<STR>> name ,XREF<ARG2> value_) const {
		using R1X = REMOVE_REF<ARG2> ;
		static WATCH<R1X> mInstance ;
		mInstance.mHint = address (keep[TYPE<CREF<Interface>>::expr] (mInstance)) ;
		mInstance.mName = name ;
		mInstance.mValue = CRef<R1X>::reference (value_) ;
		mInstance.mClazz = Clazz (TYPE<R1X>::expr) ;
		unsafe_launder (value_) ;
	}
} ;

static constexpr auto unsafe_watch = FUNCTION_unsafe_watch () ;

template <class...>
trait EXCEPTION_HELP ;

template <class...>
trait EXCEPTION_IMPLHOLDER_HELP ;

template <>
trait EXCEPTION_HELP<ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<Slice<STR>> what_) = 0 ;
		virtual Slice<STR> what () const = 0 ;
	} ;

	class Exception {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Exception () = default ;

		explicit Exception (CREF<Slice<STR>> what_) {
			auto rax = Holder::create () ;
			rax->initialize (what_) ;
			mThis = move (rax) ;
		}

		Slice<STR> what () const {
			if (mThis == NULL)
				return Slice<STR> () ;
			return mThis->what () ;
		}

		inline void raise () {
			throw thiz ;
		}
	} ;
} ;

using Exception = typename EXCEPTION_HELP<ALWAYS>::Exception ;

struct FUNCTION_unimplemented {
	inline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

template <class...>
trait SCOPE_HELP ;

template <class...>
trait SCOPE_HOLDER_HELP ;

template <class A>
trait SCOPE_HOLDER_HELP<A ,ALWAYS> {
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
			//@warn: unsafe_cast ignore cv-qualifier
			auto &&tmp1 = unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
			tmp1.leave () ;
			mPointer = ZERO ;
		}
	} ;
} ;

template <class A>
trait SCOPE_HELP<A ,ALWAYS> {
	using Super = typename SCOPE_HOLDER_HELP<A ,ALWAYS>::Scope ;

	class Scope final implement Super {
	protected:
		using Super::mPointer ;

	public:
		implicit Scope () = default ;

		explicit Scope (CREF<A> that) {
			const auto r1x = address (that) ;
			//@warn: unsafe_cast ignore cv-qualifier
			auto &&tmp1 = unsafe_cast[TYPE<A>::expr] (unsafe_pointer (r1x)) ;
			tmp1.enter () ;
			mPointer = r1x ;
		}

		explicit Scope (RREF<A>) = delete ;

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

template <class A>
using Scope = typename SCOPE_HELP<A ,ALWAYS>::Scope ;
} ;