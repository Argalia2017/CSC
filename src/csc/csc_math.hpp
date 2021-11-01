﻿#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
namespace MATH {
static constexpr auto MATH_INV2 = TRIPLE (0.5) ;
static constexpr auto MATH_INV3 = TRIPLE (0.33333333333333333333) ;
static constexpr auto MATH_E = TRIPLE (2.71828182845904523536) ;
static constexpr auto MATH_PI = TRIPLE (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = TRIPLE (1.41421356237309504880) ;
static constexpr auto MATH_SQRT3 = TRIPLE (1.73205080756887729353) ;
static constexpr auto MATH_LN2 = TRIPLE (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = TRIPLE (2.30258509299404568402) ;
static constexpr auto MATH_INVSQRT2PI = TRIPLE (0.39894228040143267794) ;

template <class...>
trait MATHPROC_HELP ;

template <>
trait MATHPROC_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual BOOL is_infinity (CREF<SINGLE> x) const = 0 ;
		virtual BOOL is_infinity (CREF<DOUBLE> x) const = 0 ;
		virtual VAL32 sign (CREF<VAL32> x) const = 0 ;
		virtual VAL64 sign (CREF<VAL64> x) const = 0 ;
		virtual SINGLE sign (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE sign (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE step (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE step (CREF<DOUBLE> x) const = 0 ;
		virtual VAL32 abs (CREF<VAL32> x) const = 0 ;
		virtual VAL64 abs (CREF<VAL64> x) const = 0 ;
		virtual SINGLE abs (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE abs (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE inverse (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE inverse (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE floor (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE floor (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual SINGLE ceil (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE ceil (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual VAL32 square (CREF<VAL32> x) const = 0 ;
		virtual VAL64 square (CREF<VAL64> x) const = 0 ;
		virtual SINGLE square (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE square (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE sqrt (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE sqrt (CREF<DOUBLE> x) const = 0 ;
		virtual VAL32 cube (CREF<VAL32> x) const = 0 ;
		virtual VAL64 cube (CREF<VAL64> x) const = 0 ;
		virtual SINGLE cube (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE cube (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE cbrt (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE cbrt (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE exp (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE exp (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE log (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE log (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE pow (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE pow (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual SINGLE cndf (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE cndf (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE pndf (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE pndf (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE sin (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE sin (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE cos (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE cos (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE tan (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE tan (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE arcsin (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE arcsin (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE arccos (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE arccos (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE arctan (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE arctan (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class MathProc {
	private:
		VRef<Holder> mThis ;

	public:
		imports CREF<MathProc> instance () {
			return memorize ([&] () {
				MathProc ret ;
				ret.mThis = FUNCTION_make::extern_invoke () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> is_infinity (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->is_infinity (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sign (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			return instance ().mThis->sign (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> step (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->step (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> abs (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			return instance ().mThis->abs (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> inverse (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->inverse (x) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> floor (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return instance ().mThis->floor (x ,y) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> ceil (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return instance ().mThis->ceil (x ,y) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> round (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			const auto r1x = x + y * R1X (MATH_INV2) ;
			return floor (r1x ,y) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> trunc (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return floor (abs (x) ,y) * sign (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> square (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			return instance ().mThis->square (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sqrt (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->sqrt (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cube (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			return instance ().mThis->cube (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cbrt (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->cbrt (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> exp (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->exp (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> log (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->log (x) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> pow (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return instance ().mThis->pow (x ,y) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cndf (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->cndf (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> pndf (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->pndf (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sin (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->sin (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cos (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->cos (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> tan (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->tan (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> arcsin (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->arcsin (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> arccos (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->arccos (x) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> arctan (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return instance ().mThis->arctan (x ,y) ;
		}
	} ;
} ;

using MathProc = typename MATHPROC_HELP<ALWAYS>::MathProc ;

struct FUNCTION_min_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x) const {
		return x ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		const auto r1x = operator_compr (x1 ,x2) ;
		if (r1x <= ZERO)
			return thiz (x1 ,xn...) ;
		return thiz (x2 ,xn...) ;
	}
} ;

static constexpr auto min_of = FUNCTION_min_of () ;

struct FUNCTION_max_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x) const {
		return x ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		const auto r1x = operator_compr (x1 ,x2) ;
		if (r1x >= ZERO)
			return thiz (x1 ,xn...) ;
		return thiz (x2 ,xn...) ;
	}
} ;

static constexpr auto max_of = FUNCTION_max_of () ;

struct FUNCTION_all_of {
	inline BOOL operator() () const {
		return TRUE ;
	}

	template <class ARG1 ,class...ARG3>
	inline BOOL operator() (XREF<ARG1> x1 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_BOOL<R1X>) ;
		if ifnot (x1)
			return FALSE ;
		return thiz (xn...) ;
	}
} ;

static constexpr auto all_of = FUNCTION_all_of () ;

struct FUNCTION_any_of {
	inline BOOL operator() () const {
		return FALSE ;
	}

	template <class ARG1 ,class...ARG3>
	inline BOOL operator() (XREF<ARG1> x1 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		require (IS_BOOL<R1X>) ;
		if (x1)
			return TRUE ;
		return thiz (xn...) ;
	}
} ;

static constexpr auto any_of = FUNCTION_any_of () ;

struct FUNCTION_sum_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x) const {
		return x ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SCALAR<R1X>) ;
		require (IS_SAME<R1X ,R2X>) ;
		return thiz (x1 + x2 ,xn...) ;
	}
} ;

static constexpr auto sum_of = FUNCTION_sum_of () ;

struct FUNCTION_acc_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x) const {
		return x ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SCALAR<R1X>) ;
		require (IS_SAME<R1X ,R2X>) ;
		return thiz (x1 * x2 ,xn...) ;
	}
} ;

static constexpr auto acc_of = FUNCTION_acc_of () ;

struct FUNCTION_clamp {
	template <class ARG1 ,class ARG2 ,class ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x ,XREF<ARG2> lb ,XREF<ARG3> rb) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		using R3X = REMOVE_ALL<ARG3> ;
		require (IS_SAME<R1X ,R2X>) ;
		require (IS_SAME<R1X ,R3X>) ;
		if (x <= lb)
			return lb ;
		if (x >= rb)
			return rb ;
		return x ;
	}
} ;

static constexpr auto clamp = FUNCTION_clamp () ;

template <class...>
trait FLOATPROC_HELP ;

template <>
trait FLOATPROC_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual DOUBLE encode (CREF<ARRAY3<VAL64>> fexp2) const = 0 ;
		virtual ARRAY3<VAL64> decode (CREF<DOUBLE> float_) const = 0 ;
		virtual ARRAY3<VAL64> exp2_from_exp10 (CREF<ARRAY3<VAL64>> fexp10) const = 0 ;
		virtual ARRAY3<VAL64> exp10_from_exp2 (CREF<ARRAY3<VAL64>> fexp2) const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class FloatProc {
	private:
		VRef<Holder> mThis ;

	public:
		imports CREF<FloatProc> instance () {
			return memorize ([&] () {
				FloatProc ret ;
				ret.mThis = FUNCTION_make::extern_invoke () ;
				return move (ret) ;
			}) ;
		}

		DOUBLE encode (CREF<ARRAY3<VAL64>> fexp2) const {
			return mThis->encode (fexp2) ;
		}

		ARRAY3<VAL64> decode (CREF<DOUBLE> float_) const {
			return mThis->decode (float_) ;
		}

		ARRAY3<VAL64> exp2_from_exp10 (CREF<ARRAY3<VAL64>> fexp10) const {
			return mThis->exp2_from_exp10 (fexp10) ;
		}

		ARRAY3<VAL64> exp10_from_exp2 (CREF<ARRAY3<VAL64>> fexp2) const {
			return mThis->exp10_from_exp2 (fexp2) ;
		}
	} ;
} ;

using FloatProc = typename FLOATPROC_HELP<ALWAYS>::FloatProc ;
} ;
} ;

namespace CSC {
using namespace MATH ;
} ;
