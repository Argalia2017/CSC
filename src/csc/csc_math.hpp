#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
static constexpr auto MATH_INV2 = DOUBLE (0.5) ;
static constexpr auto MATH_INV3 = DOUBLE (0.33333333333333333333) ;
static constexpr auto MATH_E = DOUBLE (2.71828182845904523536) ;
static constexpr auto MATH_PI = DOUBLE (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = DOUBLE (1.41421356237309504880) ;
static constexpr auto MATH_SQRT3 = DOUBLE (1.73205080756887729353) ;
static constexpr auto MATH_LN2 = DOUBLE (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = DOUBLE (2.30258509299404568402) ;
static constexpr auto MATH_INVSQRT2PI = DOUBLE (0.39894228040143267794) ;

template <class...>
trait MATHPROC_HELP ;

template <class...>
trait MATHPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MATHPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual BOOL is_infinite (CREF<SINGLE> obj) const = 0 ;
		virtual BOOL is_infinite (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 sign (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 sign (CREF<VAL64> obj) const = 0 ;
		virtual SINGLE sign (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE sign (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 step (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 step (CREF<VAL64> obj) const = 0 ;
		virtual SINGLE step (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE step (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 abs (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 abs (CREF<VAL64> obj) const = 0 ;
		virtual SINGLE abs (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE abs (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE inverse (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE inverse (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 clamp (CREF<VAL32> curr ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
		virtual VAL64 clamp (CREF<VAL64> curr ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
		virtual SINGLE clamp (CREF<SINGLE> curr ,CREF<SINGLE> lb ,CREF<SINGLE> rb) const = 0 ;
		virtual DOUBLE clamp (CREF<DOUBLE> curr ,CREF<DOUBLE> lb ,CREF<DOUBLE> rb) const = 0 ;
		virtual SINGLE floor (CREF<SINGLE> curr ,CREF<SINGLE> base) const = 0 ;
		virtual DOUBLE floor (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const = 0 ;
		virtual SINGLE ceil (CREF<SINGLE> curr ,CREF<SINGLE> base) const = 0 ;
		virtual DOUBLE ceil (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const = 0 ;
		virtual SINGLE round (CREF<SINGLE> curr ,CREF<SINGLE> base) const = 0 ;
		virtual DOUBLE round (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const = 0 ;
		virtual SINGLE trunc (CREF<SINGLE> curr ,CREF<SINGLE> base) const = 0 ;
		virtual DOUBLE trunc (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const = 0 ;
		virtual VAL32 square (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 square (CREF<VAL64> obj) const = 0 ;
		virtual SINGLE square (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE square (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE sqrt (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE sqrt (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 cube (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 cube (CREF<VAL64> obj) const = 0 ;
		virtual SINGLE cube (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE cube (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE cbrt (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE cbrt (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE exp (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE exp (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE log (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE log (CREF<DOUBLE> obj) const = 0 ;
		virtual VAL32 vlog (CREF<VAL32> curr ,CREF<VAL32> base) const = 0 ;
		virtual VAL64 vlog (CREF<VAL64> curr ,CREF<VAL64> base) const = 0 ;
		virtual SINGLE pow (CREF<SINGLE> curr ,CREF<SINGLE> exp_) const = 0 ;
		virtual DOUBLE pow (CREF<DOUBLE> curr ,CREF<DOUBLE> exp_) const = 0 ;
		virtual SINGLE ncdf (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE ncdf (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE npdf (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE npdf (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE sin (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE sin (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE cos (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE cos (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE tan (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE tan (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE arcsin (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE arcsin (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE arccos (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE arccos (CREF<DOUBLE> obj) const = 0 ;
		virtual SINGLE arctan (CREF<SINGLE> x_ ,CREF<SINGLE> y_) const = 0 ;
		virtual DOUBLE arctan (CREF<DOUBLE> x_ ,CREF<DOUBLE> y_) const = 0 ;
		virtual SINGLE radian_angle (CREF<SINGLE> obj) const = 0 ;
		virtual DOUBLE radian_angle (CREF<DOUBLE> obj) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class MathProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<MathProc> instance () {
			return memorize ([&] () {
				MathProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1>
		imports ARG1 is_infinite (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->is_infinite (obj) ;
		}

		template <class ARG1>
		imports ARG1 sign (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->sign (obj) ;
		}

		template <class ARG1>
		imports ARG1 step (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->step (obj) ;
		}

		template <class ARG1>
		imports ARG1 abs (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->abs (obj) ;
		}

		template <class ARG1>
		imports ARG1 inverse (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->inverse (obj) ;
		}

		template <class ARG1>
		imports ARG1 min_of (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return obj ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 min_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2 ,CREF<ARG2>...obj3) {
			require (IS_SCALAR<ARG1>) ;
			if (obj1 <= obj2)
				return min_of (obj1 ,obj3...) ;
			return min_of (obj2 ,obj3...) ;
		}

		template <class ARG1>
		imports ARG1 max_of (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return obj ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 max_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2 ,CREF<ARG2>...obj3) {
			require (IS_SCALAR<ARG1>) ;
			if (obj1 >= obj2)
				return max_of (obj1 ,obj3...) ;
			return max_of (obj2 ,obj3...) ;
		}

		imports BOOL all_of () {
			return TRUE ;
		}

		template <class...ARG1>
		imports BOOL all_of (CREF<BOOL> obj1 ,CREF<ARG1>...obj2) {
			if ifnot (obj1)
				return FALSE ;
			return all_of (obj2...) ;
		}

		imports BOOL any_of () {
			return FALSE ;
		}

		template <class...ARG1>
		imports BOOL any_of (CREF<BOOL> obj1 ,CREF<ARG1>...obj2) {
			if (obj1)
				return TRUE ;
			return any_of (obj2...) ;
		}

		imports INDEX else_of () {
			return ZERO ;
		}

		template <class...ARG1>
		imports INDEX else_of (CREF<BOOL> obj1 ,CREF<ARG1>...obj2) {
			if (obj1)
				return ZERO ;
			return 1 + else_of (obj2...) ;
		}

		template <class ARG1>
		imports ARG1 sum_of (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return obj ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 sum_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2 ,CREF<ARG2>...obj3) {
			require (IS_SCALAR<ARG1>) ;
			return sum_of (obj1 + obj2 ,obj3...) ;
		}

		template <class ARG1>
		imports ARG1 acc_of (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return obj ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 acc_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2 ,CREF<ARG2>...obj3) {
			require (IS_SCALAR<ARG1>) ;
			return acc_of (obj1 * obj2 ,obj3...) ;
		}

		template <class ARG1>
		imports Array<ARG1 ,RANK1> sort_of (CREF<ARG1> obj) {
			Array<ARG1 ,RANK1> ret ;
			ret[0] = obj ;
			return move (ret) ;
		}

		template <class ARG1>
		imports Array<ARG1 ,RANK2> sort_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2) {
			Array<ARG1 ,RANK2> ret ;
			ret[0] = obj1 ;
			ret[1] = obj2 ;
			if ifswitch (TRUE) {
				if (operator_compr (ret[0] ,ret[1]) <= ZERO)
					discard ;
				swap (ret[0] ,ret[1]) ;
			}
			return move (ret) ;
		}

		template <class ARG1>
		imports Array<ARG1 ,RANK3> sort_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2 ,CREF<ARG1> obj3) {
			Array<ARG1 ,RANK3> ret ;
			ret[0] = obj1 ;
			ret[1] = obj2 ;
			ret[2] = obj3 ;
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

		template <class ARG1>
		imports ARG1 clamp (CREF<ARG1> curr ,CREF<ARG1> lb ,CREF<ARG1> rb) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->floor (curr ,lb ,rb) ;
		}

		template <class ARG1>
		imports ARG1 floor (CREF<ARG1> curr ,CREF<ARG1> base) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->floor (curr ,base) ;
		}

		template <class ARG1>
		imports ARG1 ceil (CREF<ARG1> curr ,CREF<ARG1> base) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->ceil (curr ,base) ;
		}

		template <class ARG1>
		imports ARG1 round (CREF<ARG1> curr ,CREF<ARG1> base) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->round (curr ,base) ;
		}

		template <class ARG1>
		imports ARG1 trunc (CREF<ARG1> curr ,CREF<ARG1> base) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->trunc (curr ,base) ;
		}

		template <class ARG1>
		imports ARG1 square (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->square (obj) ;
		}

		template <class ARG1>
		imports ARG1 sqrt (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->sqrt (obj) ;
		}

		template <class ARG1>
		imports ARG1 cube (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->cube (obj) ;
		}

		template <class ARG1>
		imports ARG1 cbrt (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->cbrt (obj) ;
		}

		template <class ARG1>
		imports ARG1 exp (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->exp (obj) ;
		}

		template <class ARG1>
		imports ARG1 log (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->log (obj) ;
		}

		template <class ARG1>
		imports ARG1 vlog (CREF<ARG1> curr ,CREF<ARG1> exp_) {
			require (IS_VALUE<ARG1>) ;
			return instance ().mThis->vlog (curr ,exp_) ;
		}

		template <class ARG1>
		imports ARG1 pow (CREF<ARG1> curr ,CREF<ARG1> exp_) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->pow (curr ,exp_) ;
		}

		template <class ARG1>
		imports ARG1 ncdf (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->ncdf (obj) ;
		}

		template <class ARG1>
		imports ARG1 npdf (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->npdf (obj) ;
		}

		template <class ARG1>
		imports ARG1 sin (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->sin (obj) ;
		}

		template <class ARG1>
		imports ARG1 cos (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->cos (obj) ;
		}

		template <class ARG1>
		imports ARG1 tan (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->tan (obj) ;
		}

		template <class ARG1>
		imports ARG1 arcsin (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->arcsin (obj) ;
		}

		template <class ARG1>
		imports ARG1 arccos (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->arccos (obj) ;
		}

		template <class ARG1>
		imports ARG1 arctan (CREF<ARG1> x_ ,CREF<ARG1> y_) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->arctan (x_ ,y_) ;
		}

		template <class ARG1>
		imports ARG1 radian_angle (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->radian_angle (obj) ;
		}
	} ;
} ;

using MathProc = typename MATHPROC_HELP<DEPEND ,ALWAYS>::MathProc ;

template <class...>
trait FLOATPROC_HELP ;

template <class...>
trait FLOATPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FLOATPROC_HELP<DEPEND ,ALWAYS> {
	struct NOTATION {
		FLAG mRadix ;
		BOOL mSign ;
		VAL64 mMantissa ;
		VAL64 mPrecision ;
		VAL64 mExponent ;
	} ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual DOUBLE encode (CREF<NOTATION> fexp2) const = 0 ;
		virtual NOTATION decode (CREF<DOUBLE> float_) const = 0 ;
		virtual NOTATION exp2_from_exp10 (CREF<NOTATION> fexp10) const = 0 ;
		virtual NOTATION exp10_from_exp2 (CREF<NOTATION> fexp2) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class FloatProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<FloatProc> instance () {
			return memorize ([&] () {
				FloatProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
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

template <class...>
trait BITPROC_HELP ;

template <class...>
trait BITPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BITPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual BYTE low_bit (CREF<WORD> obj) const = 0 ;
		virtual WORD low_bit (CREF<CHAR> obj) const = 0 ;
		virtual CHAR low_bit (CREF<DATA> obj) const = 0 ;
		virtual BYTE high_bit (CREF<WORD> obj) const = 0 ;
		virtual WORD high_bit (CREF<CHAR> obj) const = 0 ;
		virtual CHAR high_bit (CREF<DATA> obj) const = 0 ;
		virtual WORD up_bit (CREF<BYTE> high ,CREF<BYTE> low) const = 0 ;
		virtual CHAR up_bit (CREF<WORD> high ,CREF<WORD> low) const = 0 ;
		virtual DATA up_bit (CREF<CHAR> high ,CREF<CHAR> low) const = 0 ;
		virtual BOOL get_bit (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL get_bit (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL get_bit (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL get_bit (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual BOOL all_bit (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL all_bit (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL all_bit (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL all_bit (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual DATA nth_bit (CREF<LENGTH> nth) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class BitProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BitProc> instance () {
			return memorize ([&] () {
				BitProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports BYTE low_bit (CREF<WORD> obj) {
			return instance ().mThis->low_bit (obj) ;
		}

		imports WORD low_bit (CREF<CHAR> obj) {
			return instance ().mThis->low_bit (obj) ;
		}

		imports CHAR low_bit (CREF<DATA> obj) {
			return instance ().mThis->low_bit (obj) ;
		}

		imports BYTE high_bit (CREF<WORD> obj) {
			return instance ().mThis->high_bit (obj) ;
		}

		imports WORD high_bit (CREF<CHAR> obj) {
			return instance ().mThis->high_bit (obj) ;
		}

		imports CHAR high_bit (CREF<DATA> obj) {
			return instance ().mThis->high_bit (obj) ;
		}

		imports WORD up_bit (CREF<BYTE> high ,CREF<BYTE> low) {
			return instance ().mThis->up_bit (high ,low) ;
		}

		imports CHAR up_bit (CREF<WORD> high ,CREF<WORD> low) {
			return instance ().mThis->up_bit (high ,low) ;
		}

		imports DATA up_bit (CREF<CHAR> high ,CREF<CHAR> low) {
			return instance ().mThis->up_bit (high ,low) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL get_bit (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BIT<ARG1>) ;
			return instance ().mThis->get_bit (base ,ARG1 (mask)) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL all_bit (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BIT<ARG1>) ;
			return instance ().mThis->all_bit (base ,ARG1 (mask)) ;
		}

		imports DATA nth_bit (CREF<LENGTH> nth) {
			return instance ().mThis->nth_bit (nth) ;
		}
	} ;
} ;

using BitProc = typename BITPROC_HELP<DEPEND ,ALWAYS>::BitProc ;
} ;