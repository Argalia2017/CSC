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
		virtual BOOL is_infinite (CREF<SINGLE> a) const = 0 ;
		virtual BOOL is_infinite (CREF<DOUBLE> a) const = 0 ;
		virtual SINGLE inverse (CREF<SINGLE> a) const = 0 ;
		virtual DOUBLE inverse (CREF<DOUBLE> a) const = 0 ;
		virtual SINGLE floor (CREF<SINGLE> a ,CREF<SINGLE> b) const = 0 ;
		virtual DOUBLE floor (CREF<DOUBLE> a ,CREF<DOUBLE> b) const = 0 ;
		virtual SINGLE ceil (CREF<SINGLE> a ,CREF<SINGLE> b) const = 0 ;
		virtual DOUBLE ceil (CREF<DOUBLE> a ,CREF<DOUBLE> b) const = 0 ;
		virtual DOUBLE sqrt (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE cbrt (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE exp (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE log (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE pow (CREF<DOUBLE> a ,CREF<DOUBLE> b) const = 0 ;
		virtual DOUBLE ncdf (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE npdf (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE sin (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE cos (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE tan (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE arcsin (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE arccos (CREF<DOUBLE> a) const = 0 ;
		virtual DOUBLE arctan (CREF<DOUBLE> a ,CREF<DOUBLE> b) const = 0 ;
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
		imports ARG1 is_infinite (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->is_infinite (a) ;
		}

		template <class ARG1>
		imports ARG1 sign (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			if (a >= 0)
				return ARG1 (1) ;
			return ARG1 (-1) ;
		}

		template <class ARG1>
		imports ARG1 step (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			if (a >= 0)
				return ARG1 (1) ;
			return ARG1 (0) ;
		}

		template <class ARG1>
		imports ARG1 abs (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			if (a >= 0)
				return a ;
			return -a ;
		}

		template <class ARG1>
		imports ARG1 inverse (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->inverse (a) ;
		}

		template <class ARG1>
		imports ARG1 min_of (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3>
		imports ARG1 min_of (CREF<ARG1> a ,CREF<ARG2> b ,CREF<ARG3>...c) {
			require (IS_SCALAR<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			if (a <= b)
				return min_of (a ,c...) ;
			return min_of (b ,c...) ;
		}

		template <class ARG1>
		imports ARG1 max_of (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3>
		imports ARG1 max_of (CREF<ARG1> a ,CREF<ARG2> b ,CREF<ARG3>...c) {
			require (IS_SCALAR<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			if (a >= b)
				return max_of (a ,c...) ;
			return max_of (b ,c...) ;
		}

		imports BOOL all_of () {
			return TRUE ;
		}

		template <class...ARG1>
		imports BOOL all_of (CREF<BOOL> a ,CREF<ARG1>...b) {
			if ifnot (a)
				return FALSE ;
			return all_of (b...) ;
		}

		imports BOOL any_of () {
			return FALSE ;
		}

		template <class...ARG1>
		imports BOOL any_of (CREF<BOOL> a ,CREF<ARG1>...b) {
			if (a)
				return TRUE ;
			return any_of (b...) ;
		}

		imports INDEX else_of () {
			return ZERO ;
		}

		template <class...ARG1>
		imports INDEX else_of (CREF<BOOL> a ,CREF<ARG1>...b) {
			if (a)
				return ZERO ;
			return 1 + else_of (b...) ;
		}

		template <class ARG1>
		imports ARG1 sum_of (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3>
		imports ARG1 sum_of (CREF<ARG1> a ,CREF<ARG2> b ,CREF<ARG3>...c) {
			require (IS_SCALAR<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return sum_of (a + b ,c...) ;
		}

		template <class ARG1>
		imports ARG1 acc_of (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a ;
		}

		template <class ARG1 ,class ARG2 ,class...ARG3>
		imports ARG1 acc_of (CREF<ARG1> a ,CREF<ARG2> b ,CREF<ARG3>...c) {
			require (IS_SCALAR<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return acc_of (a * b ,c...) ;
		}

		template <class ARG1>
		imports Array<ARG1 ,RANK1> sort_of (CREF<ARG1> a) {
			Array<ARG1 ,RANK1> ret ;
			ret[0] = a ;
			return move (ret) ;
		}

		template <class ARG1 ,class ARG2>
		imports Array<ARG1 ,RANK2> sort_of (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_SAME<ARG1 ,ARG2>) ;
			Array<ARG1 ,RANK2> ret ;
			ret[0] = a ;
			ret[1] = b ;
			if ifswitch (TRUE) {
				if (operator_compr (ret[0] ,ret[1]) <= ZERO)
					discard ;
				swap (ret[0] ,ret[1]) ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class ARG2 ,class ARG3>
		imports Array<ARG1 ,RANK3> sort_of (CREF<ARG1> a ,CREF<ARG2> b ,CREF<ARG3> c) {
			require (IS_SAME<ARG1 ,ARG2>) ;
			require (IS_SAME<ARG1 ,ARG3>) ;
			Array<ARG1 ,RANK3> ret ;
			ret[0] = a ;
			ret[1] = b ;
			ret[2] = c ;
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

		template <class ARG1 ,class ARG2 ,class ARG3>
		imports ARG1 clamp (CREF<ARG1> a ,CREF<ARG2> lb ,CREF<ARG3> rb) {
			require (IS_SCALAR<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			require (IS_SAME<ARG1 ,ARG3>) ;
			if (a <= lb)
				return lb ;
			if (a >= rb)
				return rb ;
			return a ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 floor (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return instance ().mThis->floor (a ,b) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 ceil (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return instance ().mThis->ceil (a ,b) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 round (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			const auto r1x = a + b * ARG1 (MATH_INV2) ;
			return floor (r1x ,b) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 trunc (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return floor (abs (a) ,b) * sign (a) ;
		}

		template <class ARG1>
		imports ARG1 square (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a * a ;
		}

		template <class ARG1>
		imports ARG1 sqrt (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->sqrt (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 cube (CREF<ARG1> a) {
			require (IS_SCALAR<ARG1>) ;
			return a * a * a ;
		}

		template <class ARG1>
		imports ARG1 cbrt (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->cbrt (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 exp (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->exp (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 log (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->log (DOUBLE (a))) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 vlog (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_VALUE<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			LENGTH ret = 0 ;
			auto rax = a ;
			while (TRUE) {
				rax /= b ;
				if (rax == 0)
					break ;
				ret++ ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 pow (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return ARG1 (instance ().mThis->pow (DOUBLE (a) ,DOUBLE (b))) ;
		}

		template <class ARG1>
		imports ARG1 ncdf (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->ncdf (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 npdf (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->npdf (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 sin (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->sin (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 cos (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->cos (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 tan (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->tan (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 arcsin (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->arcsin (DOUBLE (a))) ;
		}

		template <class ARG1>
		imports ARG1 arccos (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (instance ().mThis->arccos (DOUBLE (a))) ;
		}

		template <class ARG1 ,class ARG2>
		imports ARG1 arctan (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_FLOAT<ARG1>) ;
			require (IS_SAME<ARG1 ,ARG2>) ;
			return ARG1 (instance ().mThis->arctan (DOUBLE (a) ,DOUBLE (b))) ;
		}

		template <class ARG1>
		imports ARG1 radian_angle (CREF<ARG1> a) {
			require (IS_FLOAT<ARG1>) ;
			return ARG1 (a / ARG1 (180) * ARG1 (MATH_PI)) ;
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
	struct Holder implement Interface {} ;

	class BitProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BitProc> instance () {
			return memorize ([&] () {
				BitProc ret ;
				ret.mThis = NULL ;
				return move (ret) ;
			}) ;
		}

		imports BYTE low_bit (CREF<WORD> obj) {
			return BYTE (obj) ;
		}

		imports BYTE high_bit (CREF<WORD> obj) {
			return BYTE (obj >> 8) ;
		}

		imports WORD up_bit (CREF<BYTE> a ,CREF<BYTE> b) {
			return (WORD (a) << 8) | WORD (b) ;
		}

		imports WORD low_bit (CREF<CHAR> obj) {
			return WORD (obj) ;
		}

		imports WORD high_bit (CREF<CHAR> obj) {
			return WORD (obj >> 16) ;
		}

		imports CHAR up_bit (CREF<WORD> a ,CREF<WORD> b) {
			return (CHAR (a) << 16) | CHAR (b) ;
		}

		imports CHAR low_bit (CREF<DATA> obj) {
			return CHAR (obj) ;
		}

		imports CHAR high_bit (CREF<DATA> obj) {
			return CHAR (obj >> 32) ;
		}

		imports DATA up_bit (CREF<CHAR> a ,CREF<CHAR> b) {
			return (DATA (a) << 32) | DATA (b) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL get_bit (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_BIT<ARG1>) ;
			return (a & ARG1 (b)) != ARG1 (0X00) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL all_bit (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_BIT<ARG1>) ;
			return (a & ARG1 (b)) == ARG1 (b) ;
		}

		template <class ARG1 ,class ARG2>
		imports void set_bit (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_BIT<ARG1>) ;
			a |= ARG1 (b) ;
		}

		template <class ARG1 ,class ARG2>
		imports void erase_bit (CREF<ARG1> a ,CREF<ARG2> b) {
			require (IS_BIT<ARG1>) ;
			a &= ~ARG1 (b) ;
		}

		imports DATA nth_bit (CREF<LENGTH> nth) {
			return DATA (0X01) << nth ;
		}
	} ;
} ;

using BitProc = typename BITPROC_HELP<DEPEND ,ALWAYS>::BitProc ;
} ;