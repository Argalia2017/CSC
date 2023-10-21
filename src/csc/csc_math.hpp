#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
static constexpr auto MATH_INV2 = FLT64 (0.5) ;
static constexpr auto MATH_INV3 = FLT64 (0.33333333333333333333) ;
static constexpr auto MATH_E = FLT64 (2.71828182845904523536) ;
static constexpr auto MATH_PI = FLT64 (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = FLT64 (1.41421356237309504880) ;
static constexpr auto MATH_SQRT3 = FLT64 (1.73205080756887729353) ;
static constexpr auto MATH_LN2 = FLT64 (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = FLT64 (2.30258509299404568402) ;
static constexpr auto MATH_INVSQRT2PI = FLT64 (0.39894228040143267794) ;

template <class...>
trait MATHPROC_HELP ;

template <class...>
trait MATHPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MATHPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual BOOL is_inf (CREF<FLT32> obj) const = 0 ;
		virtual BOOL is_inf (CREF<FLT64> obj) const = 0 ;
		virtual VAL32 sign (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 sign (CREF<VAL64> obj) const = 0 ;
		virtual FLT32 sign (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 sign (CREF<FLT64> obj) const = 0 ;
		virtual VAL32 abs (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 abs (CREF<VAL64> obj) const = 0 ;
		virtual FLT32 abs (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 abs (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 inverse (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 inverse (CREF<FLT64> obj) const = 0 ;
		virtual VAL32 min_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const = 0 ;
		virtual VAL64 min_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const = 0 ;
		virtual FLT32 min_of (CREF<FLT32> obj1 ,CREF<SpanIterator<FLT32>> obj2) const = 0 ;
		virtual FLT64 min_of (CREF<FLT64> obj1 ,CREF<SpanIterator<FLT64>> obj2) const = 0 ;
		virtual VAL32 max_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const = 0 ;
		virtual VAL64 max_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const = 0 ;
		virtual FLT32 max_of (CREF<FLT32> obj1 ,CREF<SpanIterator<FLT32>> obj2) const = 0 ;
		virtual FLT64 max_of (CREF<FLT64> obj1 ,CREF<SpanIterator<FLT64>> obj2) const = 0 ;
		virtual BOOL all_of (CREF<SpanIterator<BOOL>> obj1) const = 0 ;
		virtual BOOL any_of (CREF<SpanIterator<BOOL>> obj1) const = 0 ;
		virtual INDEX else_of (CREF<SpanIterator<BOOL>> obj1) const = 0 ;
		virtual VAL32 sum_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const = 0 ;
		virtual VAL64 sum_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const = 0 ;
		virtual FLT32 sum_of (CREF<FLT32> obj1 ,CREF<SpanIterator<FLT32>> obj2) const = 0 ;
		virtual FLT64 sum_of (CREF<FLT64> obj1 ,CREF<SpanIterator<FLT64>> obj2) const = 0 ;
		virtual VAL32 acc_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const = 0 ;
		virtual VAL64 acc_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const = 0 ;
		virtual FLT32 acc_of (CREF<FLT32> obj1 ,CREF<SpanIterator<FLT32>> obj2) const = 0 ;
		virtual FLT64 acc_of (CREF<FLT64> obj1 ,CREF<SpanIterator<FLT64>> obj2) const = 0 ;
		virtual void sort_of (VREF<RegBuffer<VAL32>> result) const = 0 ;
		virtual void sort_of (VREF<RegBuffer<VAL64>> result) const = 0 ;
		virtual void sort_of (VREF<RegBuffer<FLT32>> result) const = 0 ;
		virtual void sort_of (VREF<RegBuffer<FLT64>> result) const = 0 ;
		virtual VAL32 clamp (CREF<VAL32> curr ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
		virtual VAL64 clamp (CREF<VAL64> curr ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
		virtual FLT32 clamp (CREF<FLT32> curr ,CREF<FLT32> lb ,CREF<FLT32> rb) const = 0 ;
		virtual FLT64 clamp (CREF<FLT64> curr ,CREF<FLT64> lb ,CREF<FLT64> rb) const = 0 ;
		virtual FLT32 floor (CREF<FLT32> curr ,CREF<FLT32> base) const = 0 ;
		virtual FLT64 floor (CREF<FLT64> curr ,CREF<FLT64> base) const = 0 ;
		virtual FLT32 ceil (CREF<FLT32> curr ,CREF<FLT32> base) const = 0 ;
		virtual FLT64 ceil (CREF<FLT64> curr ,CREF<FLT64> base) const = 0 ;
		virtual FLT32 round (CREF<FLT32> curr ,CREF<FLT32> base) const = 0 ;
		virtual FLT64 round (CREF<FLT64> curr ,CREF<FLT64> base) const = 0 ;
		virtual FLT32 trunc (CREF<FLT32> curr ,CREF<FLT32> base) const = 0 ;
		virtual FLT64 trunc (CREF<FLT64> curr ,CREF<FLT64> base) const = 0 ;
		virtual VAL32 square (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 square (CREF<VAL64> obj) const = 0 ;
		virtual FLT32 square (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 square (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 sqrt (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 sqrt (CREF<FLT64> obj) const = 0 ;
		virtual VAL32 cubic (CREF<VAL32> obj) const = 0 ;
		virtual VAL64 cubic (CREF<VAL64> obj) const = 0 ;
		virtual FLT32 cubic (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 cubic (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 cbrt (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 cbrt (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 exp (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 exp (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 log (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 log (CREF<FLT64> obj) const = 0 ;
		virtual VAL32 vlog (CREF<VAL32> curr ,CREF<VAL32> base) const = 0 ;
		virtual VAL64 vlog (CREF<VAL64> curr ,CREF<VAL64> base) const = 0 ;
		virtual FLT32 pow (CREF<FLT32> base ,CREF<FLT32> exponent) const = 0 ;
		virtual FLT64 pow (CREF<FLT64> base ,CREF<FLT64> exponent) const = 0 ;
		virtual FLT32 ncdf (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 ncdf (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 npdf (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 npdf (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 sin (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 sin (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 cos (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 cos (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 tan (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 tan (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 arcsin (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 arcsin (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 arccos (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 arccos (CREF<FLT64> obj) const = 0 ;
		virtual FLT32 arctan (CREF<FLT32> fy ,CREF<FLT32> fx) const = 0 ;
		virtual FLT64 arctan (CREF<FLT64> fy ,CREF<FLT64> fx) const = 0 ;
		virtual FLT32 radian_angle (CREF<FLT32> obj) const = 0 ;
		virtual FLT64 radian_angle (CREF<FLT64> obj) const = 0 ;
	} ;

	class MathProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<MathProc> instance () {
			return memorize ([&] () {
				MathProc ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1>
		imports BOOL is_inf (CREF<ARG1> obj) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->is_inf (obj) ;
		}

		template <class ARG1>
		imports ARG1 sign (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->sign (obj) ;
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

		template <class ARG1 ,class...ARG2>
		imports Array<ARG1 ,COUNT_OF<TYPEAS<ARG1 ,ARG2...>>> sort_of (CREF<ARG1> obj1 ,CREF<ARG2>...obj2) {
			require (IS_SCALAR<ARG1>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
			using R1X = COUNT_OF<TYPEAS<ARG1 ,ARG2...>> ;
			const auto r1x = SpanIterator<ARG1> (capture (obj1 ,obj2...)) ;
			Array<ARG1 ,R1X> ret = Array<ARG1 ,R1X> (r1x) ;
			instance ().mThis->sort_of (ret.raw ()) ;
			return move (ret) ;
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
		imports ARG1 cubic (CREF<ARG1> obj) {
			require (IS_SCALAR<ARG1>) ;
			return instance ().mThis->cubic (obj) ;
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
		imports ARG1 arctan (CREF<ARG1> fy ,CREF<ARG1> fx) {
			require (IS_FLOAT<ARG1>) ;
			return instance ().mThis->arctan (fy ,fx) ;
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
		LENGTH mPrecision ;
		VAL64 mMantissa ;
		VAL64 mDownflow ;
		VAL64 mExponent ;
	} ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual FLT64 encode (CREF<NOTATION> fexp2) const = 0 ;
		virtual NOTATION decode (CREF<FLT64> float_) const = 0 ;
		virtual NOTATION fexp2_from_fexp10 (CREF<NOTATION> fexp10) const = 0 ;
		virtual NOTATION fexp10_from_fexp2 (CREF<NOTATION> fexp2) const = 0 ;
	} ;

	class FloatProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<FloatProc> instance () {
			return memorize ([&] () {
				FloatProc ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports FLT64 encode (CREF<NOTATION> fexp2) {
			return instance ().mThis->encode (fexp2) ;
		}

		imports NOTATION decode (CREF<FLT64> float_) {
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
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual BYTE bit_low (CREF<WORD> obj) const = 0 ;
		virtual WORD bit_low (CREF<CHAR> obj) const = 0 ;
		virtual CHAR bit_low (CREF<DATA> obj) const = 0 ;
		virtual BYTE bit_high (CREF<WORD> obj) const = 0 ;
		virtual WORD bit_high (CREF<CHAR> obj) const = 0 ;
		virtual CHAR bit_high (CREF<DATA> obj) const = 0 ;
		virtual WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const = 0 ;
		virtual CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const = 0 ;
		virtual DATA bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const = 0 ;
		virtual BOOL bit_any (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL bit_any (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL bit_any (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL bit_any (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual BOOL bit_all (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
		virtual BOOL bit_all (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
		virtual BOOL bit_all (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
		virtual BOOL bit_all (CREF<DATA> base ,CREF<DATA> mask) const = 0 ;
		virtual DATA bit_single (CREF<LENGTH> nth) const = 0 ;
		virtual BYTE bit_reverse (CREF<BYTE> obj) const = 0 ;
		virtual WORD bit_reverse (CREF<WORD> obj) const = 0 ;
		virtual CHAR bit_reverse (CREF<CHAR> obj) const = 0 ;
		virtual DATA bit_reverse (CREF<DATA> obj) const = 0 ;
		virtual INDEX bit_find (CREF<BYTE> obj) const = 0 ;
		virtual INDEX bit_find (CREF<WORD> obj) const = 0 ;
		virtual INDEX bit_find (CREF<CHAR> obj) const = 0 ;
		virtual INDEX bit_find (CREF<DATA> obj) const = 0 ;
	} ;

	class BitProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BitProc> instance () {
			return memorize ([&] () {
				BitProc ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports BYTE bit_low (CREF<WORD> obj) {
			return instance ().mThis->bit_low (obj) ;
		}

		imports WORD bit_low (CREF<CHAR> obj) {
			return instance ().mThis->bit_low (obj) ;
		}

		imports CHAR bit_low (CREF<DATA> obj) {
			return instance ().mThis->bit_low (obj) ;
		}

		imports BYTE bit_high (CREF<WORD> obj) {
			return instance ().mThis->bit_high (obj) ;
		}

		imports WORD bit_high (CREF<CHAR> obj) {
			return instance ().mThis->bit_high (obj) ;
		}

		imports CHAR bit_high (CREF<DATA> obj) {
			return instance ().mThis->bit_high (obj) ;
		}

		imports WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) {
			return instance ().mThis->bit_merge (high ,low) ;
		}

		imports CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) {
			return instance ().mThis->bit_merge (high ,low) ;
		}

		imports DATA bit_merge (CREF<CHAR> high ,CREF<CHAR> low) {
			return instance ().mThis->bit_merge (high ,low) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL bit_any (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BYTE<ARG1>) ;
			return instance ().mThis->bit_any (base ,ARG1 (mask)) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL bit_all (CREF<ARG1> base ,CREF<ARG2> mask) {
			require (IS_BYTE<ARG1>) ;
			return instance ().mThis->bit_all (base ,ARG1 (mask)) ;
		}

		imports DATA bit_single (CREF<LENGTH> nth) {
			return instance ().mThis->bit_single (nth) ;
		}

		imports BYTE bit_reverse (CREF<BYTE> obj) {
			return instance ().mThis->bit_reverse (obj) ;
		}

		imports WORD bit_reverse (CREF<WORD> obj) {
			return instance ().mThis->bit_reverse (obj) ;
		}

		imports CHAR bit_reverse (CREF<CHAR> obj) {
			return instance ().mThis->bit_reverse (obj) ;
		}

		imports DATA bit_reverse (CREF<DATA> obj) {
			return instance ().mThis->bit_reverse (obj) ;
		}

		template <class ARG1>
		imports INDEX bit_find (CREF<ARG1> obj) {
			require (IS_BYTE<ARG1>) ;
			return instance ().mThis->bit_find (obj) ;
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
	struct Layout ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<LENGTH> size_ ,CREF<VAL64> value_) = 0 ;
		virtual Layout clone () const = 0 ;
		virtual LENGTH precision () const = 0 ;
		virtual VAL64 get () const = 0 ;
		virtual void set (CREF<VAL64> value_) = 0 ;
		virtual BOOL equal (CREF<Layout> that) const = 0 ;
		virtual FLAG compr (CREF<Layout> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
		virtual Layout add (CREF<Layout> that) const = 0 ;
		virtual Layout sub (CREF<Layout> that) const = 0 ;
		virtual Layout mul (CREF<Layout> that) const = 0 ;
		virtual Layout mul (CREF<VAL64> scale) const = 0 ;
		virtual Layout div (CREF<VAL64> scale) const = 0 ;
		virtual Layout mod (CREF<VAL64> scale) const = 0 ;
		virtual Layout minus () const = 0 ;
		virtual void increase () = 0 ;
		virtual void decrease () = 0 ;
	} ;

	struct Layout {
		VRef<Holder> mThis ;
	} ;

	class Integer implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit Integer () = default ;

		implicit Integer (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit Integer (CREF<LENGTH> size_ ,CREF<VAL64> value_) {
			mThis = Holder::create () ;
			mThis->initialize (size_ ,value_) ;
		}

		implicit Integer (CREF<Integer> that) {
			if (that.mThis == NULL)
				return ;
			thiz = that.mThis->clone () ;
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
			return mThis->equal (that) ;
		}

		inline BOOL operator== (CREF<Integer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Integer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Integer> that) const {
			return mThis->compr (that) ;
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
			return mThis->add (that) ;
		}

		inline Integer operator+ (CREF<Integer> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<Integer> that) {
			thiz = add (that) ;
		}

		Integer sub (CREF<Integer> that) const {
			return mThis->sub (that) ;
		}

		inline Integer operator- (CREF<Integer> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<Integer> that) {
			thiz = sub (that) ;
		}

		Integer mul (CREF<Integer> that) const {
			return mThis->mul (that) ;
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