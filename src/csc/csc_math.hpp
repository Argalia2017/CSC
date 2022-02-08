#pragma once

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

template <class DEPEND>
trait MATHPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual BOOL infinite (CREF<SINGLE> x) const = 0 ;
		virtual BOOL infinite (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE inverse (CREF<SINGLE> x) const = 0 ;
		virtual DOUBLE inverse (CREF<DOUBLE> x) const = 0 ;
		virtual SINGLE floor (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE floor (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual SINGLE ceil (CREF<SINGLE> x ,CREF<SINGLE> y) const = 0 ;
		virtual DOUBLE ceil (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual DOUBLE sqrt (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE cbrt (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE exp (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE log (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE pow (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
		virtual DOUBLE ncdf (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE npdf (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE sin (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE cos (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE tan (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE arcsin (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE arccos (CREF<DOUBLE> x) const = 0 ;
		virtual DOUBLE arctan (CREF<DOUBLE> x ,CREF<DOUBLE> y) const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class MathProc {
	private:
		VRef<Holder> mThis ;

	public:
		imports CREF<MathProc> instance () {
			return memorize ([&] () {
				MathProc ret ;
				ret.mThis = FUNCTION_link::invoke () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> infinite (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return instance ().mThis->infinite (x) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sign (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			if (x >= 0)
				return SINGLE (1) ;
			return SINGLE (-1) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> step (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			if (x >= 0)
				return SINGLE (1) ;
			return SINGLE (0) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> abs (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			if (x >= 0)
				return x ;
			return -x ;
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
			return x * x ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sqrt (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->sqrt (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cube (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SCALAR<R1X>) ;
			return x * x * x ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cbrt (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->cbrt (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> exp (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->exp (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> log (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->log (DOUBLE (x))) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> pow (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return R1X (instance ().mThis->pow (DOUBLE (x) ,DOUBLE (y))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> ncdf (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->ncdf (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> npdf (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->npdf (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> sin (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->sin (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> cos (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->cos (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> tan (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->tan (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> arcsin (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->arcsin (DOUBLE (x))) ;
		}

		template <class ARG1>
		imports REMOVE_ALL<ARG1> arccos (XREF<ARG1> x) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FLOAT<R1X>) ;
			return R1X (instance ().mThis->arccos (DOUBLE (x))) ;
		}

		template <class ARG1 ,class ARG2>
		imports REMOVE_ALL<ARG1> arctan (XREF<ARG1> x ,XREF<ARG2> y) {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = REMOVE_ALL<ARG2> ;
			require (IS_FLOAT<R1X>) ;
			require (IS_SAME<R1X ,R2X>) ;
			return R1X (instance ().mThis->arctan (DOUBLE (x) ,DOUBLE (y))) ;
		}
	} ;
} ;

using MathProc = typename MATHPROC_HELP<DEPEND ,ALWAYS>::MathProc ;

struct FUNCTION_choose {
	template <class ARG1 ,class...ARG2>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> id ,XREF<ARG2>...xn) const {
		return template_choose (PHX ,id ,forward[TYPEAS<ARG2>::id] (xn)...) ;
	}

	template <class ARG1>
	REMOVE_ALL<ARG1> template_choose (CREF<typeof (PH3)> ,XREF<ARG1> id) const {
		return bad (id) ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3 ,class = ENABLE<IS_SAME<REMOVE_ALL<ARG1> ,REMOVE_ALL<ARG2>>>>
	REMOVE_ALL<ARG1> template_choose (CREF<typeof (PH2)> ,XREF<ARG1> id ,XREF<ARG2> x1 ,XREF<ARG3>...xn) const {
		return forward[TYPEAS<ARG2>::id] (x1) ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	REMOVE_ALL<ARG1> template_choose (CREF<typeof (PH1)> ,XREF<ARG1> id ,XREF<ARG2> x1 ,XREF<ARG3>...xn) const {
		return template_choose (PHX ,id ,forward[TYPEAS<ARG3>::id] (xn)...) ;
	}
} ;

static constexpr auto choose = FUNCTION_choose () ;

struct FUNCTION_min_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1) const {
		return forward[TYPEAS<ARG1>::id] (x1) ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		const auto r1x = operator_compr (x1 ,x2) ;
		if (r1x <= ZERO)
			return thiz (forward[TYPEAS<ARG1>::id] (x1) ,forward[TYPEAS<ARG3>::id] (xn)...) ;
		return thiz (forward[TYPEAS<ARG2>::id] (x2) ,forward[TYPEAS<ARG3>::id] (xn)...) ;
	}
} ;

static constexpr auto min_of = FUNCTION_min_of () ;

struct FUNCTION_max_of {
	template <class ARG1>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1) const {
		return forward[TYPEAS<ARG1>::id] (x1) ;
	}

	template <class ARG1 ,class ARG2 ,class...ARG3>
	inline REMOVE_ALL<ARG1> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3>...xn) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		const auto r1x = operator_compr (x1 ,x2) ;
		if (r1x >= ZERO)
			return thiz (forward[TYPEAS<ARG1>::id] (x1) ,forward[TYPEAS<ARG3>::id] (xn)...) ;
		return thiz (forward[TYPEAS<ARG2>::id] (x2) ,forward[TYPEAS<ARG3>::id] (xn)...) ;
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
		if (operator_compr (x ,lb) <= ZERO)
			return lb ;
		if (operator_compr (x ,rb) >= ZERO)
			return rb ;
		return x ;
	}
} ;

static constexpr auto clamp = FUNCTION_clamp () ;

struct FUNCTION_sort_of {
	template <class ARG1>
	inline Array<REMOVE_ALL<ARG1> ,RANK1> operator() (XREF<ARG1> x1) const {
		using R1X = REMOVE_ALL<ARG1> ;
		Array<R1X ,RANK1> ret ;
		ret[0] = forward[TYPEAS<ARG1>::id] (x1) ;
		return move (ret) ;
	}

	template <class ARG1 ,class ARG2>
	inline Array<REMOVE_ALL<ARG1> ,RANK2> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		require (IS_SAME<R1X ,R2X>) ;
		Array<R1X ,RANK2> ret ;
		ret[0] = forward[TYPEAS<ARG1>::id] (x1) ;
		ret[1] = forward[TYPEAS<ARG1>::id] (x2) ;
		if ifswitch (TRUE) {
			if (operator_compr (ret[0] ,ret[1]) <= ZERO)
				discard ;
			swap (ret[0] ,ret[1]) ;
		}
		return move (ret) ;
	}

	template <class ARG1 ,class ARG2 ,class ARG3>
	inline Array<REMOVE_ALL<ARG1> ,RANK3> operator() (XREF<ARG1> x1 ,XREF<ARG2> x2 ,XREF<ARG3> x3) const {
		using R1X = REMOVE_ALL<ARG1> ;
		using R2X = REMOVE_ALL<ARG2> ;
		using R3X = REMOVE_ALL<ARG3> ;
		require (IS_SAME<R1X ,R2X>) ;
		require (IS_SAME<R1X ,R3X>) ;
		Array<R1X ,RANK3> ret ;
		ret[0] = forward[TYPEAS<ARG1>::id] (x1) ;
		ret[1] = forward[TYPEAS<ARG1>::id] (x2) ;
		ret[2] = forward[TYPEAS<ARG1>::id] (x3) ;
		if ifswitch (TRUE) {
			if (operator_compr (ret[0] ,ret[1]) <= ZERO)
				discard ;
			swap (ret[0] ,ret[1]) ;
		}
		if ifswitch (TRUE) {
			if (operator_compr (ret[0] ,ret[2]) <= ZERO)
				discard ;
			swap (ret[0] ,ret[2]) ;
		}
		if ifswitch (TRUE) {
			if (operator_compr (ret[1] ,ret[2]) <= ZERO)
				discard ;
			swap (ret[1] ,ret[2]) ;
		}
		return move (ret) ;
	}
} ;

static constexpr auto sort_of = FUNCTION_sort_of () ;

template <class...>
trait FLOATPROC_HELP ;

template <class DEPEND>
trait FLOATPROC_HELP<DEPEND ,ALWAYS> {
	struct NOTATION {
		FLAG mRadix ;
		BOOL mSign ;
		LENGTH mPrecision ;
		VAL64 mMantissa ;
		VAL64 mExponent ;
	} ;

	struct Holder implement Interface {
		virtual DOUBLE encode (CREF<NOTATION> fexp2) const = 0 ;
		virtual NOTATION decode (CREF<DOUBLE> float_) const = 0 ;
		virtual NOTATION exp2_from_exp10 (CREF<NOTATION> fexp10) const = 0 ;
		virtual NOTATION exp10_from_exp2 (CREF<NOTATION> fexp2) const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class FloatProc {
	private:
		VRef<Holder> mThis ;

	public:
		imports CREF<FloatProc> instance () {
			return memorize ([&] () {
				FloatProc ret ;
				ret.mThis = FUNCTION_link::invoke () ;
				return move (ret) ;
			}) ;
		}

		imports DOUBLE encode (CREF<NOTATION> fexp2) {
			return instance ().mThis->encode (fexp2) ;
		}

		imports NOTATION decode (CREF<DOUBLE> float_) {
			return instance ().mThis->decode (float_) ;
		}

		imports NOTATION exp2_from_exp10 (CREF<NOTATION> fexp10) {
			return instance ().mThis->exp2_from_exp10 (fexp10) ;
		}

		imports NOTATION exp10_from_exp2 (CREF<NOTATION> fexp2) {
			return instance ().mThis->exp10_from_exp2 (fexp2) ;
		}
	} ;
} ;

using FloatProc = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::FloatProc ;
} ;
} ;

namespace CSC {
using namespace MATH ;
} ;
