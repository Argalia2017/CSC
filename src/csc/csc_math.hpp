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
		virtual BOOL is_inf (CREF<SINGLE> obj) const = 0 ;
		virtual BOOL is_inf (CREF<DOUBLE> obj) const = 0 ;
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
		virtual VAL32 min_of (CREF<VAL32> obj1 ,CREF<Variadic<VAL32>> obj2) const = 0 ;
		virtual VAL64 min_of (CREF<VAL64> obj1 ,CREF<Variadic<VAL64>> obj2) const = 0 ;
		virtual SINGLE min_of (CREF<SINGLE> obj1 ,CREF<Variadic<SINGLE>> obj2) const = 0 ;
		virtual DOUBLE min_of (CREF<DOUBLE> obj1 ,CREF<Variadic<DOUBLE>> obj2) const = 0 ;
		virtual VAL32 max_of (CREF<VAL32> obj1 ,CREF<Variadic<VAL32>> obj2) const = 0 ;
		virtual VAL64 max_of (CREF<VAL64> obj1 ,CREF<Variadic<VAL64>> obj2) const = 0 ;
		virtual SINGLE max_of (CREF<SINGLE> obj1 ,CREF<Variadic<SINGLE>> obj2) const = 0 ;
		virtual DOUBLE max_of (CREF<DOUBLE> obj1 ,CREF<Variadic<DOUBLE>> obj2) const = 0 ;
		virtual BOOL all_of (CREF<Variadic<BOOL>> obj1) const = 0 ;
		virtual BOOL any_of (CREF<Variadic<BOOL>> obj1) const = 0 ;
		virtual INDEX else_of (CREF<Variadic<BOOL>> obj1) const = 0 ;
		virtual VAL32 sum_of (CREF<VAL32> obj1 ,CREF<Variadic<VAL32>> obj2) const = 0 ;
		virtual VAL64 sum_of (CREF<VAL64> obj1 ,CREF<Variadic<VAL64>> obj2) const = 0 ;
		virtual SINGLE sum_of (CREF<SINGLE> obj1 ,CREF<Variadic<SINGLE>> obj2) const = 0 ;
		virtual DOUBLE sum_of (CREF<DOUBLE> obj1 ,CREF<Variadic<DOUBLE>> obj2) const = 0 ;
		virtual VAL32 acc_of (CREF<VAL32> obj1 ,CREF<Variadic<VAL32>> obj2) const = 0 ;
		virtual VAL64 acc_of (CREF<VAL64> obj1 ,CREF<Variadic<VAL64>> obj2) const = 0 ;
		virtual SINGLE acc_of (CREF<SINGLE> obj1 ,CREF<Variadic<SINGLE>> obj2) const = 0 ;
		virtual DOUBLE acc_of (CREF<DOUBLE> obj1 ,CREF<Variadic<DOUBLE>> obj2) const = 0 ;
		virtual Array<VAL32 ,RANK2> sort_of (CREF<VAL32> obj1 ,CREF<VAL32> obj2) const = 0 ;
		virtual Array<VAL64 ,RANK2> sort_of (CREF<VAL64> obj1 ,CREF<VAL64> obj2) const = 0 ;
		virtual Array<SINGLE ,RANK2> sort_of (CREF<SINGLE> obj1 ,CREF<SINGLE> obj2) const = 0 ;
		virtual Array<DOUBLE ,RANK2> sort_of (CREF<DOUBLE> obj1 ,CREF<DOUBLE> obj2) const = 0 ;
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
		virtual SINGLE pow (CREF<SINGLE> base ,CREF<SINGLE> exponent) const = 0 ;
		virtual DOUBLE pow (CREF<DOUBLE> base ,CREF<DOUBLE> exponent) const = 0 ;
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
		virtual SINGLE arctan (CREF<SINGLE> fx ,CREF<SINGLE> fy) const = 0 ;
		virtual DOUBLE arctan (CREF<DOUBLE> fx ,CREF<DOUBLE> fy) const = 0 ;
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
		imports ARG1 is_inf (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->is_inf (obj) ;
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

		template <class ARG1 ,class...ARG2>
		imports ARG1 min_of (CREF<ARG1> obj1 ,CREF<ARG2>...obj2) {
			require (IS_SCALAR<ARG1>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
			return instance ().mThis->min_of (obj1 ,capture (obj2...)) ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 max_of (CREF<ARG1> obj1 ,CREF<ARG2>...obj2) {
			require (IS_SCALAR<ARG1>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
			return instance ().mThis->max_of (obj1 ,capture (obj2...)) ;
		}

		template <class...ARG1>
		imports BOOL all_of (CREF<ARG1>...obj1) {
			require (ENUM_ALL<IS_SAME<BOOL ,ARG1>...>) ;
			return instance ().mThis->all_of (capture (obj1...)) ;
		}

		template <class...ARG1>
		imports BOOL any_of (CREF<ARG1>...obj1) {
			require (ENUM_ALL<IS_SAME<BOOL ,ARG1>...>) ;
			return instance ().mThis->any_of (capture (obj1...)) ;
		}

		template <class...ARG1>
		imports INDEX else_of (CREF<ARG1>...obj1) {
			require (ENUM_ALL<IS_SAME<BOOL ,ARG1>...>) ;
			return instance ().mThis->else_of (capture (obj1...)) ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 sum_of (CREF<ARG1> obj1 ,CREF<ARG2>...obj2) {
			require (IS_SCALAR<ARG1>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
			return instance ().mThis->sum_of (obj1 ,capture (obj2...)) ;
		}

		template <class ARG1 ,class...ARG2>
		imports ARG1 acc_of (CREF<ARG1> obj1 ,CREF<ARG2>...obj2) {
			require (IS_SCALAR<ARG1>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
			return instance ().mThis->sum_of (obj1 ,capture (obj2...)) ;
		}

		template <class ARG1>
		imports Array<ARG1 ,RANK2> sort_of (CREF<ARG1> obj1 ,CREF<ARG1> obj2) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->sort_of (obj1 ,obj2) ;
		}

		template <class ARG1>
		imports ARG1 clamp (CREF<ARG1> curr ,CREF<ARG1> lb ,CREF<ARG1> rb) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->clamp (curr ,lb ,rb) ;
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
		imports ARG1 vlog (CREF<ARG1> curr ,CREF<ARG1> base) {
			require (IS_VALUE<ARG1>) ;
			return instance ().mThis->vlog (curr ,base) ;
		}

		template <class ARG1>
		imports ARG1 pow (CREF<ARG1> base ,CREF<ARG1> exponent) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->pow (base ,exponent) ;
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
		imports ARG1 arctan (CREF<ARG1> fx ,CREF<ARG1> fy) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->arctan (fx ,fy) ;
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
		virtual NOTATION fexp2_from_fexp10 (CREF<NOTATION> fexp10) const = 0 ;
		virtual NOTATION fexp10_from_fexp2 (CREF<NOTATION> fexp2) const = 0 ;
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

		imports NOTATION fexp2_from_fexp10 (CREF<NOTATION> fexp10) {
			return instance ().mThis->fexp2_from_fexp10 (fexp10) ;
		}

		imports NOTATION fexp10_from_fexp2 (CREF<NOTATION> fexp2) {
			return instance ().mThis->fexp10_from_fexp2 (fexp2) ;
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
		virtual WORD merge_bit (CREF<BYTE> high ,CREF<BYTE> low) const = 0 ;
		virtual CHAR merge_bit (CREF<WORD> high ,CREF<WORD> low) const = 0 ;
		virtual DATA merge_bit (CREF<CHAR> high ,CREF<CHAR> low) const = 0 ;
		virtual BOOL any_bit (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL any_bit (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL any_bit (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL any_bit (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual BOOL all_bit (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL all_bit (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL all_bit (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL all_bit (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual DATA single_bit (CREF<LENGTH> nth) const = 0 ;
		virtual INDEX find_bit (CREF<BYTE> obj) const = 0 ;
		virtual INDEX find_bit (CREF<WORD> obj) const = 0 ;
		virtual INDEX find_bit (CREF<CHAR> obj) const = 0 ;
		virtual INDEX find_bit (CREF<DATA> obj) const = 0 ;
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

		imports WORD merge_bit (CREF<BYTE> high ,CREF<BYTE> low) {
			return instance ().mThis->merge_bit (high ,low) ;
		}

		imports CHAR merge_bit (CREF<WORD> high ,CREF<WORD> low) {
			return instance ().mThis->merge_bit (high ,low) ;
		}

		imports DATA merge_bit (CREF<CHAR> high ,CREF<CHAR> low) {
			return instance ().mThis->merge_bit (high ,low) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL any_bit (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BIT<ARG1>) ;
			return instance ().mThis->any_bit (base ,ARG1 (mask)) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL all_bit (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BIT<ARG1>) ;
			return instance ().mThis->all_bit (base ,ARG1 (mask)) ;
		}

		imports DATA single_bit (CREF<LENGTH> nth) {
			return instance ().mThis->single_bit (nth) ;
		}

		template <class ARG1>
		imports INDEX find_bit (CREF<ARG1> obj) {
			require (IS_BIT<ARG1>) ;
			return instance ().mThis->find_bit (obj) ;
		}
	} ;
} ;

using BitProc = typename BITPROC_HELP<DEPEND ,ALWAYS>::BitProc ;

template <class...>
trait INTEGER_HELP ;

template <class...>
trait INTEGER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait INTEGER_HELP<DEPEND ,ALWAYS> {
	class Integer ;

	struct Holder implement Interface {
		virtual void initialize (CREF<LENGTH> size_ ,CREF<VAL64> value_) = 0 ;
		virtual LENGTH precision () const = 0 ;
		virtual VAL64 get () const = 0 ;
		virtual void set (CREF<VAL64> value_) = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
		virtual Integer add (CREF<Holder> that) const = 0 ;
		virtual Integer sub (CREF<Holder> that) const = 0 ;
		virtual Integer mul (CREF<Holder> that) const = 0 ;
		virtual Integer mul (CREF<VAL64> scale) const = 0 ;
		virtual Integer div (CREF<VAL64> scale) const = 0 ;
		virtual Integer mod (CREF<VAL64> scale) const = 0 ;
		virtual void friend_clone (VREF<Integer> that) const = 0 ;
		virtual Integer minus () const = 0 ;
		virtual void increase () = 0 ;
		virtual void decrease () = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		VarBuffer<BYTE> mInteger ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class Integer {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit Integer () = default ;

		explicit Integer (CREF<LENGTH> size_ ,CREF<VAL64> value_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (size_ ,value_) ;
		}

		explicit Integer (RREF<Box<FakeHolder>> that) {
			mThis = move (that) ;
		}

		implicit Integer (CREF<Integer> that) {
			that.mThis->friend_clone (thiz) ;
		}

		inline VREF<Integer> operator= (CREF<Integer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Integer (RREF<Integer> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Integer> operator= (RREF<Integer> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		LENGTH precision () const {
			return mThis->precision () ;
		}

		VAL64 get () const {
			return mThis->get () ;
		}

		void set (CREF<VAL64> value_) {
			return mThis->set (value_) ;
		}

		BOOL equal (CREF<Integer> that) const {
			return mThis->equal (that.mThis.self) ;
		}

		inline BOOL operator== (CREF<Integer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Integer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Integer> that) const {
			return mThis->compr (that.mThis.self) ;
		}

		inline BOOL operator< (CREF<Integer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Integer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Integer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Integer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return mThis->hash () ;
		}

		Integer add (CREF<Integer> that) const {
			return mThis->add (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis.self)) ;
		}

		inline Integer operator+ (CREF<Integer> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<Integer> that) {
			thiz = add (that) ;
		}

		Integer sub (CREF<Integer> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis.self)) ;
		}

		inline Integer operator- (CREF<Integer> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<Integer> that) {
			thiz = sub (that) ;
		}

		Integer mul (CREF<Integer> that) const {
			return mThis->mul (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis.self)) ;
		}

		inline Integer operator* (CREF<Integer> that) const {
			return mul (that) ;
		}

		inline void operator*= (CREF<Integer> that) {
			thiz = mul (that) ;
		}

		Integer mul (CREF<VAL64> scale) const {
			return mThis->mul (scale) ;
		}

		inline Integer operator* (CREF<VAL64> scale) const {
			return mul (scale) ;
		}

		inline void operator*= (CREF<VAL64> scale) {
			thiz = mul (scale) ;
		}

		Integer div (CREF<VAL64> scale) const {
			return mThis->div (scale) ;
		}

		inline Integer operator/ (CREF<VAL64> scale) const {
			return div (scale) ;
		}

		inline void operator/= (CREF<VAL64> scale) {
			thiz = div (scale) ;
		}

		Integer mod (CREF<VAL64> scale) const {
			return mThis->mod (scale) ;
		}

		inline Integer operator% (CREF<VAL64> scale) const {
			return mod (scale) ;
		}

		inline void operator%= (CREF<VAL64> scale) {
			thiz = mod (scale) ;
		}

		inline Integer operator+ () const {
			return thiz ;
		}

		Integer minus () const {
			return mThis->minus () ;
		}

		inline Integer operator- () const {
			return minus () ;
		}

		void increase () {
			return mThis->increase () ;
		}

		inline void operator++ (VAL32) {
			increase () ;
		}

		void decrease () {
			return mThis->decrease () ;
		}

		inline void operator-- (VAL32) {
			decrease () ;
		}
	} ;
} ;

using Integer = typename INTEGER_HELP<DEPEND ,ALWAYS>::Integer ;
} ;