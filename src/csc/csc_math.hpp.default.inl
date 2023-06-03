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
#error "∑(っ°Д° ;)っ : require 'csc_math.hpp'"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
template <class DEPEND>
trait MATHPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MATHPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		BOOL is_inf (CREF<SINGLE> obj) const override {
			const auto r1x = bitwise (obj) ;
			if ifnot (BitProc::bit_all (r1x ,CHAR (0X7F800000)))
				return FALSE ;
			//@warn: treat nan as inf
			if (BitProc::bit_any (r1x ,CHAR (0X007FFFFF)))
				return TRUE ;
			return TRUE ;
		}

		BOOL is_inf (CREF<DOUBLE> obj) const override {
			const auto r1x = bitwise (obj) ;
			if ifnot (BitProc::bit_all (r1x ,DATA (0X7FF0000000000000)))
				return FALSE ;
			//@warn: treat nan as inf
			if (BitProc::bit_any (r1x ,DATA (0X000FFFFFFFFFFFFF)))
				return TRUE ;
			return TRUE ;
		}

		VAL32 sign (CREF<VAL32> obj) const override {
			if (obj >= 0)
				return VAL32 (1) ;
			return VAL32 (-1) ;
		}

		VAL64 sign (CREF<VAL64> obj) const override {
			if (obj >= 0)
				return VAL64 (1) ;
			return VAL64 (-1) ;
		}

		SINGLE sign (CREF<SINGLE> obj) const override {
			if (obj >= 0)
				return SINGLE (1) ;
			return SINGLE (-1) ;
		}

		DOUBLE sign (CREF<DOUBLE> obj) const override {
			if (obj >= 0)
				return DOUBLE (1) ;
			return DOUBLE (-1) ;
		}

		VAL32 step (CREF<VAL32> obj) const override {
			if (obj >= 0)
				return VAL32 (1) ;
			return VAL32 (0) ;
		}

		VAL64 step (CREF<VAL64> obj) const override {
			if (obj >= 0)
				return VAL64 (1) ;
			return VAL64 (0) ;
		}

		SINGLE step (CREF<SINGLE> obj) const override {
			if (obj >= 0)
				return SINGLE (1) ;
			return SINGLE (0) ;
		}

		DOUBLE step (CREF<DOUBLE> obj) const override {
			if (obj >= 0)
				return DOUBLE (1) ;
			return DOUBLE (0) ;
		}

		VAL32 abs (CREF<VAL32> obj) const override {
			//@warn: treat abs as 0
			if (obj == VAL32_ABS)
				return 0 ;
			if (obj >= 0)
				return obj ;
			return -obj ;
		}

		VAL64 abs (CREF<VAL64> obj) const override {
			//@warn: treat abs as 0
			if (obj == VAL64_ABS)
				return 0 ;
			if (obj >= 0)
				return obj ;
			return -obj ;
		}

		SINGLE abs (CREF<SINGLE> obj) const override {
			if (obj >= 0)
				return obj ;
			return -obj ;
		}

		DOUBLE abs (CREF<DOUBLE> obj) const override {
			if (obj >= 0)
				return obj ;
			return -obj ;
		}

		SINGLE inverse (CREF<SINGLE> obj) const override {
			if (MathProc::abs (obj) < SINGLE_EPS)
				return SINGLE (0) ;
			return SINGLE (1) / obj ;
		}

		DOUBLE inverse (CREF<DOUBLE> obj) const override {
			if (MathProc::abs (obj) < DOUBLE_EPS)
				return DOUBLE (0) ;
			return DOUBLE (1) / obj ;
		}

		VAL32 min_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const override {
			VAL32 ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret <= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		VAL64 min_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const override {
			VAL64 ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret <= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		SINGLE min_of (CREF<SINGLE> obj1 ,CREF<SpanIterator<SINGLE>> obj2) const override {
			SINGLE ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret <= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		DOUBLE min_of (CREF<DOUBLE> obj1 ,CREF<SpanIterator<DOUBLE>> obj2) const override {
			DOUBLE ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret <= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		VAL32 max_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const override {
			VAL32 ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret >= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		VAL64 max_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const override {
			VAL64 ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret >= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		SINGLE max_of (CREF<SINGLE> obj1 ,CREF<SpanIterator<SINGLE>> obj2) const override {
			SINGLE ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret >= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		DOUBLE max_of (CREF<DOUBLE> obj1 ,CREF<SpanIterator<DOUBLE>> obj2) const override {
			DOUBLE ret = obj1 ;
			for (auto &&i : obj2) {
				if (ret >= i)
					continue ;
				ret = i ;
			}
			return move (ret) ;
		}

		BOOL all_of (CREF<SpanIterator<BOOL>> obj1) const override {
			for (auto &&i : obj1) {
				if ifnot (i)
					return FALSE ;
			}
			return TRUE ;
		}

		BOOL any_of (CREF<SpanIterator<BOOL>> obj1) const override {
			for (auto &&i : obj1) {
				if (i)
					return TRUE ;
			}
			return FALSE ;
		}

		INDEX else_of (CREF<SpanIterator<BOOL>> obj1) const override {
			INDEX ret = 0 ;
			for (auto &&i : obj1) {
				if (i)
					break ;
				ret++ ;
			}
			return move (ret) ;
		}

		VAL32 sum_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const override {
			VAL32 ret = obj1 ;
			for (auto &&i : obj2)
				ret += i ;
			return move (ret) ;
		}

		VAL64 sum_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const override {
			VAL64 ret = obj1 ;
			for (auto &&i : obj2)
				ret += i ;
			return move (ret) ;
		}

		SINGLE sum_of (CREF<SINGLE> obj1 ,CREF<SpanIterator<SINGLE>> obj2) const override {
			SINGLE ret = obj1 ;
			for (auto &&i : obj2)
				ret += i ;
			return move (ret) ;
		}

		DOUBLE sum_of (CREF<DOUBLE> obj1 ,CREF<SpanIterator<DOUBLE>> obj2) const override {
			DOUBLE ret = obj1 ;
			for (auto &&i : obj2)
				ret += i ;
			return move (ret) ;
		}

		VAL32 acc_of (CREF<VAL32> obj1 ,CREF<SpanIterator<VAL32>> obj2) const override {
			VAL32 ret = obj1 ;
			for (auto &&i : obj2)
				ret *= i ;
			return move (ret) ;
		}

		VAL64 acc_of (CREF<VAL64> obj1 ,CREF<SpanIterator<VAL64>> obj2) const override {
			VAL64 ret = obj1 ;
			for (auto &&i : obj2)
				ret *= i ;
			return move (ret) ;
		}

		SINGLE acc_of (CREF<SINGLE> obj1 ,CREF<SpanIterator<SINGLE>> obj2) const override {
			SINGLE ret = obj1 ;
			for (auto &&i : obj2)
				ret *= i ;
			return move (ret) ;
		}

		DOUBLE acc_of (CREF<DOUBLE> obj1 ,CREF<SpanIterator<DOUBLE>> obj2) const override {
			DOUBLE ret = obj1 ;
			for (auto &&i : obj2)
				ret *= i ;
			return move (ret) ;
		}

		void sort_of (VREF<RegBuffer<VAL32>> result) const override {
			for (auto &&i : iter (0 ,result.size ())) {
				INDEX iy = i ;
				for (auto &&j : iter (i + 1 ,result.size ())) {
					if (result[iy] <= result[j])
						continue ;
					iy = j ;
				}
				if (iy == i)
					continue ;
				swap (result[i] ,result[iy]) ;
			}
		}

		void sort_of (VREF<RegBuffer<VAL64>> result) const override {
			for (auto &&i : iter (0 ,result.size ())) {
				INDEX iy = i ;
				for (auto &&j : iter (i + 1 ,result.size ())) {
					if (result[iy] <= result[j])
						continue ;
					iy = j ;
				}
				if (iy == i)
					continue ;
				swap (result[i] ,result[iy]) ;
			}
		}

		void sort_of (VREF<RegBuffer<SINGLE>> result) const override {
			for (auto &&i : iter (0 ,result.size ())) {
				INDEX iy = i ;
				for (auto &&j : iter (i + 1 ,result.size ())) {
					if (result[iy] <= result[j])
						continue ;
					iy = j ;
				}
				if (iy == i)
					continue ;
				swap (result[i] ,result[iy]) ;
			}
		}

		void sort_of (VREF<RegBuffer<DOUBLE>> result) const override {
			for (auto &&i : iter (0 ,result.size ())) {
				INDEX iy = i ;
				for (auto &&j : iter (i + 1 ,result.size ())) {
					if (result[iy] <= result[j])
						continue ;
					iy = j ;
				}
				if (iy == i)
					continue ;
				swap (result[i] ,result[iy]) ;
			}
		}

		VAL32 clamp (CREF<VAL32> curr ,CREF<VAL32> lb ,CREF<VAL32> rb) const override {
			if (curr <= lb)
				return lb ;
			if (curr >= rb)
				return rb ;
			return curr ;
		}

		VAL64 clamp (CREF<VAL64> curr ,CREF<VAL64> lb ,CREF<VAL64> rb) const override {
			if (curr <= lb)
				return lb ;
			if (curr >= rb)
				return rb ;
			return curr ;
		}

		SINGLE clamp (CREF<SINGLE> curr ,CREF<SINGLE> lb ,CREF<SINGLE> rb) const override {
			if (curr <= lb)
				return lb ;
			if (curr >= rb)
				return rb ;
			return curr ;
		}

		DOUBLE clamp (CREF<DOUBLE> curr ,CREF<DOUBLE> lb ,CREF<DOUBLE> rb) const override {
			if (curr <= lb)
				return lb ;
			if (curr >= rb)
				return rb ;
			return curr ;
		}

		SINGLE floor (CREF<SINGLE> curr ,CREF<SINGLE> base) const override {
			assert (base > 0) ;
			const auto r1x = VAL64 (curr * MathProc::inverse (base)) ;
			const auto r2x = base * SINGLE (r1x) ;
			if (curr >= 0)
				return r2x ;
			if (curr >= r2x)
				return r2x ;
			return base * SINGLE (r1x - 1) ;
		}

		DOUBLE floor (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const override {
			assert (base > 0) ;
			const auto r1x = VAL64 (curr * MathProc::inverse (base)) ;
			const auto r2x = base * DOUBLE (r1x) ;
			if (curr >= 0)
				return r2x ;
			if (curr >= r2x)
				return r2x ;
			return base * DOUBLE (r1x - 1) ;
		}

		SINGLE ceil (CREF<SINGLE> curr ,CREF<SINGLE> base) const override {
			assert (base > 0) ;
			const auto r1x = VAL64 (curr * MathProc::inverse (base)) ;
			const auto r2x = base * SINGLE (r1x) ;
			if (curr <= 0)
				return r2x ;
			if (curr <= r2x)
				return r2x ;
			return base * SINGLE (r1x + 1) ;
		}

		DOUBLE ceil (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const override {
			assert (base > 0) ;
			const auto r1x = VAL64 (curr * MathProc::inverse (base)) ;
			const auto r2x = base * DOUBLE (r1x) ;
			if (curr <= 0)
				return r2x ;
			if (curr <= r2x)
				return r2x ;
			return base * DOUBLE (r1x + 1) ;
		}

		SINGLE round (CREF<SINGLE> curr ,CREF<SINGLE> base) const override {
			const auto r1x = curr + base * SINGLE (MATH_INV2) ;
			return floor (r1x ,base) ;
		}

		DOUBLE round (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const override {
			const auto r1x = curr + base * DOUBLE (MATH_INV2) ;
			return floor (r1x ,base) ;
		}

		SINGLE trunc (CREF<SINGLE> curr ,CREF<SINGLE> base) const override {
			return floor (abs (curr) ,base) * sign (curr) ;
		}

		DOUBLE trunc (CREF<DOUBLE> curr ,CREF<DOUBLE> base) const override {
			return floor (abs (curr) ,base) * sign (curr) ;
		}

		VAL32 square (CREF<VAL32> obj) const override {
			return obj * obj ;
		}

		VAL64 square (CREF<VAL64> obj) const override {
			return obj * obj ;
		}

		SINGLE square (CREF<SINGLE> obj) const override {
			return obj * obj ;
		}

		DOUBLE square (CREF<DOUBLE> obj) const override {
			return obj * obj ;
		}

		SINGLE sqrt (CREF<SINGLE> obj) const override {
			return std::sqrt (obj) ;
		}

		DOUBLE sqrt (CREF<DOUBLE> obj) const override {
			return std::sqrt (obj) ;
		}

		VAL32 cube (CREF<VAL32> obj) const override {
			return obj * obj * obj ;
		}

		VAL64 cube (CREF<VAL64> obj) const override {
			return obj * obj * obj ;
		}

		SINGLE cube (CREF<SINGLE> obj) const override {
			return obj * obj * obj ;
		}

		DOUBLE cube (CREF<DOUBLE> obj) const override {
			return obj * obj * obj ;
		}

		SINGLE cbrt (CREF<SINGLE> obj) const override {
			return std::cbrt (obj) ;
		}

		DOUBLE cbrt (CREF<DOUBLE> obj) const override {
			return std::cbrt (obj) ;
		}

		SINGLE exp (CREF<SINGLE> obj) const override {
			return std::exp (obj) ;
		}

		DOUBLE exp (CREF<DOUBLE> obj) const override {
			return std::exp (obj) ;
		}

		SINGLE log (CREF<SINGLE> obj) const override {
			return std::log (obj) ;
		}

		DOUBLE log (CREF<DOUBLE> obj) const override {
			return std::log (obj) ;
		}

		VAL32 vlog (CREF<VAL32> curr ,CREF<VAL32> base) const override {
			VAL32 ret = 0 ;
			auto rax = curr ;
			while (TRUE) {
				rax /= base ;
				if (rax == 0)
					break ;
				ret++ ;
			}
			return move (ret) ;
		}

		VAL64 vlog (CREF<VAL64> curr ,CREF<VAL64> base) const override {
			VAL64 ret = 0 ;
			auto rax = curr ;
			while (TRUE) {
				rax /= base ;
				if (rax == 0)
					break ;
				ret++ ;
			}
			return move (ret) ;
		}

		SINGLE pow (CREF<SINGLE> base ,CREF<SINGLE> exponent) const override {
			return std::pow (base ,exponent) ;
		}

		DOUBLE pow (CREF<DOUBLE> base ,CREF<DOUBLE> exponent) const override {
			return std::pow (base ,exponent) ;
		}

		SINGLE ncdf (CREF<SINGLE> obj) const override {
			const auto r1x = obj * MathProc::inverse (SINGLE (MATH_SQRT2)) ;
			return std::erf (r1x) * SINGLE (MATH_INV2) + SINGLE (MATH_INV2) ;
		}

		DOUBLE ncdf (CREF<DOUBLE> obj) const override {
			const auto r1x = obj * MathProc::inverse (DOUBLE (MATH_SQRT2)) ;
			return std::erf (r1x) * DOUBLE (MATH_INV2) + DOUBLE (MATH_INV2) ;
		}

		SINGLE npdf (CREF<SINGLE> obj) const override {
			const auto r1x = -MathProc::square (obj) * SINGLE (MATH_INV2) ;
			return exp (r1x) * SINGLE (MATH_INVSQRT2PI) ;
		}

		DOUBLE npdf (CREF<DOUBLE> obj) const override {
			const auto r1x = -MathProc::square (obj) * DOUBLE (MATH_INV2) ;
			return exp (r1x) * DOUBLE (MATH_INVSQRT2PI) ;
		}

		SINGLE sin (CREF<SINGLE> obj) const override {
			return std::sin (obj) ;
		}

		DOUBLE sin (CREF<DOUBLE> obj) const override {
			return std::sin (obj) ;
		}

		SINGLE cos (CREF<SINGLE> obj) const override {
			return std::cos (obj) ;
		}

		DOUBLE cos (CREF<DOUBLE> obj) const override {
			return std::cos (obj) ;
		}

		SINGLE tan (CREF<SINGLE> obj) const override {
			return std::tan (obj) ;
		}

		DOUBLE tan (CREF<DOUBLE> obj) const override {
			return std::tan (obj) ;
		}

		SINGLE arcsin (CREF<SINGLE> obj) const override {
			return std::asin (obj) ;
		}

		DOUBLE arcsin (CREF<DOUBLE> obj) const override {
			return std::asin (obj) ;
		}

		SINGLE arccos (CREF<SINGLE> obj) const override {
			return std::acos (obj) ;
		}

		DOUBLE arccos (CREF<DOUBLE> obj) const override {
			return std::acos (obj) ;
		}

		SINGLE arctan (CREF<SINGLE> fy ,CREF<SINGLE> fx) const override {
			return std::atan2 (fy ,fx) ;
		}

		DOUBLE arctan (CREF<DOUBLE> fy ,CREF<DOUBLE> fx) const override {
			return std::atan2 (fy ,fx) ;
		}

		SINGLE radian_angle (CREF<SINGLE> obj) const override {
			return obj / SINGLE (180) * SINGLE (MATH_PI) ;
		}

		DOUBLE radian_angle (CREF<DOUBLE> obj) const override {
			return obj / DOUBLE (180) * DOUBLE (MATH_PI) ;
		}
	} ;
} ;

template <>
exports auto MATHPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename MATHPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait FLOATPROC_FEXP2CACHE_HELP ;

template <class...>
trait FLOATPROC_FEXP10CACHE_HELP ;

template <class DEPEND>
trait FLOATPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using Holder = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		DOUBLE encode (CREF<NOTATION> fexp2) const override {
			assert (fexp2.mRadix == 2) ;
			auto rax = fexp2 ;
			if ifswitch (TRUE) {
				if (rax.mMantissa == 0)
					discard ;
				while (TRUE) {
					if ifnot (BitProc::bit_any (DATA (rax.mMantissa) ,DATA (0XFFE0000000000000)))
						break ;
					rax.mMantissa = VAL64 (DATA (rax.mMantissa) >> 1) ;
					rax.mExponent++ ;
				}
				while (TRUE) {
					if (BitProc::bit_any (DATA (rax.mMantissa) ,DATA (0XFFF0000000000000)))
						break ;
					rax.mMantissa = VAL64 (DATA (rax.mMantissa) << 1) ;
					rax.mExponent-- ;
				}
			}
			if ifswitch (TRUE) {
				const auto r1x = VAL64 (-1074) - rax.mExponent ;
				if (r1x <= 0)
					discard ;
				rax.mMantissa = VAL64 (DATA (rax.mMantissa) >> r1x) ;
				rax.mExponent = -1075 ;
			}
			rax.mExponent += 1075 ;
			if ifswitch (TRUE) {
				if (rax.mMantissa != 0)
					discard ;
				rax.mExponent = 0 ;
			}
			const auto r2x = invoke ([&] () {
				if ifnot (fexp2.mSign)
					return DATA (0X00) ;
				return DATA (0X8000000000000000) ;
			}) ;
			const auto r3x = (DATA (rax.mExponent) << 52) & DATA (0X7FF0000000000000) ;
			const auto r4x = DATA (rax.mMantissa) & DATA (0X000FFFFFFFFFFFFF) ;
			const auto r5x = r2x | r3x | r4x ;
			return bitwise[TYPEAS<DOUBLE>::expr] (r5x) ;
		}

		NOTATION decode (CREF<DOUBLE> obj) const override {
			NOTATION ret ;
			ret.mRadix = 2 ;
			const auto r1x = bitwise (obj) ;
			const auto r2x = r1x & DATA (0X7FF0000000000000) ;
			const auto r3x = r1x & DATA (0X000FFFFFFFFFFFFF) ;
			ret.mSign = BitProc::bit_any (r1x ,DATA (0X8000000000000000)) ;
			ret.mMantissa = VAL64 (r3x) ;
			ret.mPrecision = 0 ;
			if ifswitch (TRUE) {
				if (r2x == DATA (0X00))
					discard ;
				ret.mMantissa = VAL64 (DATA (ret.mMantissa) | DATA (0X0010000000000000)) ;
			}
			ret.mExponent = VAL64 (r2x >> 52) ;
			ret.mExponent -= 1075 - LENGTH (r2x == DATA (0X00)) ;
			if ifswitch (TRUE) {
				if (ret.mMantissa != 0)
					discard ;
				ret.mExponent = 0 ;
			}
			if ifswitch (TRUE) {
				if (ret.mMantissa == 0)
					discard ;
				while (TRUE) {
					if (BitProc::bit_any (DATA (ret.mMantissa) ,DATA (0X0000000000000001)))
						break ;
					ret.mMantissa = VAL64 (DATA (ret.mMantissa) >> 1) ;
					ret.mExponent++ ;
				}
			}
			return move (ret) ;
		}

		NOTATION fexp2_multiply (CREF<NOTATION> obj1 ,CREF<NOTATION> obj2) const {
			assert (obj1.mRadix == 2) ;
			assert (obj2.mRadix == 2) ;
			NOTATION ret ;
			ret.mRadix = 2 ;
			ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
			const auto r1x = LENGTH (32) ;
			const auto r2x = DATA (VAL64 (BitProc::bit_single (r1x)) - 1) ;
			const auto r3x = VAL64 (BitProc::bit_single (r1x - 1)) ;
			const auto r4x = VAL64 (DATA (obj1.mMantissa) >> r1x) ;
			const auto r5x = VAL64 (DATA (obj1.mMantissa) & r2x) ;
			const auto r6x = VAL64 (DATA (obj2.mMantissa) >> r1x) ;
			const auto r7x = VAL64 (DATA (obj2.mMantissa) & r2x) ;
			const auto r8x = r4x * r6x ;
			const auto r9x = r5x * r6x ;
			const auto r10x = r4x * r7x ;
			const auto r11x = r5x * r7x ;
			//@warn: lose a part of precision
			auto act = TRUE ;
			if ifswitch (act) {
				if (r8x == 0)
					discard ;
				const auto r12x = VAL64 (DATA (r9x) >> r1x) ;
				const auto r13x = VAL64 (DATA (r9x) & r2x) ;
				const auto r14x = VAL64 (DATA (r10x) >> r1x) ;
				const auto r15x = VAL64 (DATA (r10x) & r2x) ;
				const auto r16x = VAL64 (DATA (r11x) >> r1x) ;
				const auto r17x = VAL64 (DATA (r13x + r15x + r16x + r3x) >> r1x) ;
				ret.mMantissa = r8x + r12x + r14x + r17x ;
				ret.mPrecision = 0 ;
				ret.mExponent = obj1.mExponent + obj2.mExponent + r1x * 2 ;
			}
			if ifswitch (act) {
				if (r9x == 0)
					if (r10x == 0)
						discard ;
				const auto r18x = VAL64 (DATA (r11x + r3x) >> r1x) ;
				ret.mMantissa = r9x + r10x + r18x ;
				ret.mPrecision = 0 ;
				ret.mExponent = obj1.mExponent + obj2.mExponent + r1x ;
			}
			if ifswitch (act) {
				ret.mMantissa = r11x ;
				ret.mPrecision = 0 ;
				ret.mExponent = obj1.mExponent + obj2.mExponent ;
			}
			return move (ret) ;
		}

		NOTATION fexp2_from_fexp10 (CREF<NOTATION> fexp10) const override {
			using R1X = typename DEPENDENT<FLOATPROC_FEXP2CACHE_HELP<DEPEND ,ALWAYS> ,DEPEND>::FEXP2Cache ;
			assert (fexp10.mRadix == 10) ;
			const auto r1x = invoke ([&] () {
				NOTATION ret ;
				ret.mRadix = 2 ;
				ret.mSign = fexp10.mSign ;
				ret.mMantissa = fexp10.mMantissa ;
				ret.mPrecision = 0 ;
				ret.mExponent = 0 ;
				return move (ret) ;
			}) ;
			const auto r2x = R1X::instance ()[fexp10.mExponent] ;
			const auto r3x = fexp2_multiply (r1x ,r2x) ;
			return r3x ;
		}

		NOTATION fexp10_overflow (CREF<NOTATION> fexp10 ,CREF<VAL64> half) const {
			NOTATION ret = fexp10 ;
			if ifswitch (TRUE) {
				if (ret.mMantissa >= 0)
					discard ;
				ret.mMantissa = VAL64 (DATA (ret.mMantissa) >> 1) ;
				ret.mMantissa /= 5 ;
				ret.mExponent++ ;
			}
			const auto r1x = MathProc::square (half) ;
			while (TRUE) {
				if (ret.mMantissa < r1x)
					break ;
				ret.mMantissa /= 10 ;
				ret.mExponent++ ;
			}
			return move (ret) ;
		}

		NOTATION fexp10_multiply (CREF<NOTATION> obj1 ,CREF<NOTATION> obj2) const {
			assert (obj1.mRadix == 10) ;
			assert (obj2.mRadix == 10) ;
			NOTATION ret ;
			ret.mRadix = 10 ;
			ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
			const auto r1x = LENGTH (9) ;
			const auto r2x = VAL64 (1000000000) ;
			const auto r3x = VAL64 (500000000) ;
			const auto r4x = fexp10_overflow (obj1 ,r2x) ;
			const auto r5x = fexp10_overflow (obj2 ,r2x) ;
			const auto r6x = r4x.mMantissa / r2x ;
			const auto r7x = r4x.mMantissa % r2x ;
			const auto r8x = r5x.mMantissa / r2x ;
			const auto r9x = r5x.mMantissa % r2x ;
			const auto r10x = r6x * r8x ;
			const auto r11x = r7x * r8x ;
			const auto r12x = r6x * r9x ;
			const auto r13x = r7x * r9x ;
			//@warn: lose a part of precision
			auto act = TRUE ;
			if ifswitch (act) {
				if (r10x == 0)
					discard ;
				const auto r14x = r11x / r2x ;
				const auto r15x = r11x % r2x ;
				const auto r16x = r12x / r2x ;
				const auto r17x = r12x % r2x ;
				const auto r18x = r13x / r2x ;
				const auto r19x = (r15x + r17x + r18x + r3x) / r2x ;
				ret.mMantissa = r10x + r14x + r16x + r19x ;
				ret.mPrecision = 0 ;
				ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
			}
			if ifswitch (act) {
				if (r11x == 0)
					if (r12x == 0)
						discard ;
				const auto r20x = (r13x + r3x) / r2x ;
				ret.mMantissa = r11x + r12x + r20x ;
				ret.mPrecision = 0 ;
				ret.mExponent = r4x.mExponent + r5x.mExponent + r1x ;
			}
			if ifswitch (act) {
				ret.mMantissa = r13x ;
				ret.mPrecision = 0 ;
				ret.mExponent = r4x.mExponent + r5x.mExponent ;
			}
			return move (ret) ;
		}

		NOTATION fexp10_from_fexp2 (CREF<NOTATION> fexp2) const override {
			using R1X = typename DEPENDENT<FLOATPROC_FEXP10CACHE_HELP<DEPEND ,ALWAYS> ,DEPEND>::FEXP10Cache ;
			assert (fexp2.mRadix == 2) ;
			const auto r1x = invoke ([&] () {
				NOTATION ret ;
				ret.mRadix = 10 ;
				ret.mSign = fexp2.mSign ;
				ret.mMantissa = fexp2.mMantissa ;
				ret.mPrecision = 0 ;
				ret.mExponent = 0 ;
				return move (ret) ;
			}) ;
			const auto r2x = R1X::instance ()[fexp2.mExponent] ;
			const auto r3x = fexp10_multiply (r1x ,r2x) ;
			return r3x ;
		}
	} ;
} ;

template <class DEPEND>
trait FLOATPROC_FEXP2CACHE_HELP<DEPEND ,ALWAYS> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;

	using SIZE = ENUMAS<VAL ,693> ;

	class FEXP2Cache {
	protected:
		BoxBuffer<DATA ,SIZE> mMCache ;
		BoxBuffer<LENGTH ,SIZE> mECache ;

	public:
		imports CREF<FEXP2Cache> instance () {
			static const DATA M_MCACHE[] {
				DATA (0XC3C05EE50655E1FA) ,DATA (0XF4B0769E47EB5A78) ,DATA (0X98EE4A22ECF3188B) ,DATA (0XBF29DCABA82FDEAE) ,DATA (0XEEF453D6923BD65A) ,DATA (0X9558B4661B6565F8) ,DATA (0XBAAEE17FA23EBF76) ,DATA (0XE95A99DF8ACE6F53) ,DATA (0X91D8A02BB6C10594) ,DATA (0XB64EC836A47146F9) ,DATA (0XE3E27A444D8D98B7) ,DATA (0X8E6D8C6AB0787F72) ,DATA (0XB208EF855C969F4F) ,DATA (0XDE8B2B66B3BC4723) ,DATA (0X8B16FB203055AC76) ,DATA (0XADDCB9E83C6B1793) ,DATA (0XD953E8624B85DD78) ,DATA (0X87D4713D6F33AA6B) ,DATA (0XA9C98D8CCB009506) ,DATA (0XD43BF0EFFDC0BA48) ,DATA (0X84A57695FE98746D) ,DATA (0XA5CED43B7E3E9188) ,DATA (0XCF42894A5DCE35EA) ,DATA (0X818995CE7AA0E1B2) ,DATA (0XA1EBFB4219491A1F) ,DATA (0XCA66FA129F9B60A6) ,DATA (0XFD00B897478238D0) ,DATA (0X9E20735E8CB16382) ,DATA (0XC5A890362FDDBC62) ,DATA (0XF712B443BBD52B7B) ,DATA (0X9A6BB0AA55653B2D) ,DATA (0XC1069CD4EABE89F8) ,DATA (0XF148440A256E2C76) ,DATA (0X96CD2A865764DBCA) ,DATA (0XBC807527ED3E12BC) ,DATA (0XEBA09271E88D976B) ,DATA (0X93445B8731587EA3) ,DATA (0XB8157268FDAE9E4C) ,DATA (0XE61ACF033D1A45DF) ,DATA (0X8FD0C16206306BAB) ,DATA (0XB3C4F1BA87BC8696) ,DATA (0XE0B62E2929ABA83C) ,DATA (0X8C71DCD9BA0B4925) ,DATA (0XAF8E5410288E1B6F) ,DATA (0XDB71E91432B1A24A) ,DATA (0X892731AC9FAF056E) ,DATA (0XAB70FE17C79AC6CA) ,DATA (0XD64D3D9DB981787D) ,DATA (0X85F0468293F0EB4E) ,DATA (0XA76C582338ED2621) ,DATA (0XD1476E2C07286FAA) ,DATA (0X82CCA4DB847945CA) ,DATA (0XA37FCE126597973C) ,DATA (0XCC5FC196FEFD7D0C) ,DATA (0XFF77B1FCBEBCDC4F) ,DATA (0X9FAACF3DF73609B1) ,DATA (0XC795830D75038C1D) ,DATA (0XF97AE3D0D2446F25) ,DATA (0X9BECCE62836AC577) ,DATA (0XC2E801FB244576D5) ,DATA (0XF3A20279ED56D48A) ,DATA (0X9845418C345644D6) ,DATA (0XBE5691EF416BD60C) ,DATA (0XEDEC366B11C6CB8F) ,DATA (0X94B3A202EB1C3F39) ,DATA (0XB9E08A83A5E34F07) ,DATA (0XE858AD248F5C22C9) ,DATA (0X91376C36D99995BE) ,DATA (0XB58547448FFFFB2D) ,DATA (0XE2E69915B3FFF9F9) ,DATA (0X8DD01FAD907FFC3B) ,DATA (0XB1442798F49FFB4A) ,DATA (0XDD95317F31C7FA1D) ,DATA (0X8A7D3EEF7F1CFC52) ,DATA (0XAD1C8EAB5EE43B66) ,DATA (0XD863B256369D4A40) ,DATA (0X873E4F75E2224E68) ,DATA (0XA90DE3535AAAE202) ,DATA (0XD3515C2831559A83) ,DATA (0X8412D9991ED58091) ,DATA (0XA5178FFF668AE0B6) ,DATA (0XCE5D73FF402D98E3) ,DATA (0X80FA687F881C7F8E) ,DATA (0XA139029F6A239F72) ,DATA (0XC987434744AC874E) ,DATA (0XFBE9141915D7A922) ,DATA (0X9D71AC8FADA6C9B5) ,DATA (0XC4CE17B399107C22) ,DATA (0XF6019DA07F549B2B) ,DATA (0X99C102844F94E0FB) ,DATA (0XC0314325637A1939) ,DATA (0XF03D93EEBC589F88) ,DATA (0X96267C7535B763B5) ,DATA (0XBBB01B9283253CA2) ,DATA (0XEA9C227723EE8BCB) ,DATA (0X92A1958A7675175F) ,DATA (0XB749FAED14125D36) ,DATA (0XE51C79A85916F484) ,DATA (0X8F31CC0937AE58D2) ,DATA (0XB2FE3F0B8599EF07) ,DATA (0XDFBDCECE67006AC9) ,DATA (0X8BD6A141006042BD) ,DATA (0XAECC49914078536D) ,DATA (0XDA7F5BF590966848) ,DATA (0X888F99797A5E012D) ,DATA (0XAAB37FD7D8F58178) ,DATA (0XD5605FCDCF32E1D6) ,DATA (0X855C3BE0A17FCD26) ,DATA (0XA6B34AD8C9DFC06F) ,DATA (0XD0601D8EFC57B08B) ,DATA (0X823C12795DB6CE57) ,DATA (0XA2CB1717B52481ED) ,DATA (0XCB7DDCDDA26DA268) ,DATA (0XFE5D54150B090B02) ,DATA (0X9EFA548D26E5A6E1) ,DATA (0XC6B8E9B0709F109A) ,DATA (0XF867241C8CC6D4C0) ,DATA (0X9B407691D7FC44F8) ,DATA (0XC21094364DFB5636) ,DATA (0XF294B943E17A2BC4) ,DATA (0X979CF3CA6CEC5B5A) ,DATA (0XBD8430BD08277231) ,DATA (0XECE53CEC4A314EBD) ,DATA (0X940F4613AE5ED136) ,DATA (0XB913179899F68584) ,DATA (0XE757DD7EC07426E5) ,DATA (0X9096EA6F3848984F) ,DATA (0XB4BCA50B065ABE63) ,DATA (0XE1EBCE4DC7F16DFB) ,DATA (0X8D3360F09CF6E4BD) ,DATA (0XB080392CC4349DEC) ,DATA (0XDCA04777F541C567) ,DATA (0X89E42CAAF9491B60) ,DATA (0XAC5D37D5B79B6239) ,DATA (0XD77485CB25823AC7) ,DATA (0X86A8D39EF77164BC) ,DATA (0XA8530886B54DBDEB) ,DATA (0XD267CAA862A12D66) ,DATA (0X8380DEA93DA4BC60) ,DATA (0XA46116538D0DEB78) ,DATA (0XCD795BE870516656) ,DATA (0X806BD9714632DFF6) ,DATA (0XA086CFCD97BF97F3) ,DATA (0XC8A883C0FDAF7DF0) ,DATA (0XFAD2A4B13D1B5D6C) ,DATA (0X9CC3A6EEC6311A63) ,DATA (0XC3F490AA77BD60FC) ,DATA (0XF4F1B4D515ACB93B) ,DATA (0X991711052D8BF3C5) ,DATA (0XBF5CD54678EEF0B6) ,DATA (0XEF340A98172AACE4) ,DATA (0X9580869F0E7AAC0E) ,DATA (0XBAE0A846D2195712) ,DATA (0XE998D258869FACD7) ,DATA (0X91FF83775423CC06) ,DATA (0XB67F6455292CBF08) ,DATA (0XE41F3D6A7377EECA) ,DATA (0X8E938662882AF53E) ,DATA (0XB23867FB2A35B28D) ,DATA (0XDEC681F9F4C31F31) ,DATA (0X8B3C113C38F9F37E) ,DATA (0XAE0B158B4738705E) ,DATA (0XD98DDAEE19068C76) ,DATA (0X87F8A8D4CFA417C9) ,DATA (0XA9F6D30A038D1DBC) ,DATA (0XD47487CC8470652B) ,DATA (0X84C8D4DFD2C63F3B) ,DATA (0XA5FB0A17C777CF09) ,DATA (0XCF79CC9DB955C2CC) ,DATA (0X81AC1FE293D599BF) ,DATA (0XA21727DB38CB002F) ,DATA (0XCA9CF1D206FDC03B) ,DATA (0XFD442E4688BD304A) ,DATA (0X9E4A9CEC15763E2E) ,DATA (0XC5DD44271AD3CDBA) ,DATA (0XF7549530E188C128) ,DATA (0X9A94DD3E8CF578B9) ,DATA (0XC13A148E3032D6E7) ,DATA (0XF18899B1BC3F8CA1) ,DATA (0X96F5600F15A7B7E5) ,DATA (0XBCB2B812DB11A5DE) ,DATA (0XEBDF661791D60F56) ,DATA (0X936B9FCEBB25C995) ,DATA (0XB84687C269EF3BFB) ,DATA (0XE65829B3046B0AFA) ,DATA (0X8FF71A0FE2C2E6DC) ,DATA (0XB3F4E093DB73A093) ,DATA (0XE0F218B8D25088B8) ,DATA (0X8C974F7383725573) ,DATA (0XAFBD2350644EEACF) ,DATA (0XDBAC6C247D62A583) ,DATA (0X894BC396CE5DA772) ,DATA (0XAB9EB47C81F5114F) ,DATA (0XD686619BA27255A2) ,DATA (0X8613FD0145877585) ,DATA (0XA798FC4196E952E7) ,DATA (0XD17F3B51FCA3A7A0) ,DATA (0X82EF85133DE648C4) ,DATA (0XA3AB66580D5FDAF5) ,DATA (0XCC963FEE10B7D1B3) ,DATA (0XFFBBCFE994E5C61F) ,DATA (0X9FD561F1FD0F9BD3) ,DATA (0XC7CABA6E7C5382C8) ,DATA (0XF9BD690A1B68637B) ,DATA (0X9C1661A651213E2D) ,DATA (0XC31BFA0FE5698DB8) ,DATA (0XF3E2F893DEC3F126) ,DATA (0X986DDB5C6B3A76B7) ,DATA (0XBE89523386091465) ,DATA (0XEE2BA6C0678B597F) ,DATA (0X94DB483840B717EF) ,DATA (0XBA121A4650E4DDEB) ,DATA (0XE896A0D7E51E1566) ,DATA (0X915E2486EF32CD60) ,DATA (0XB5B5ADA8AAFF80B8) ,DATA (0XE3231912D5BF60E6) ,DATA (0X8DF5EFABC5979C8F) ,DATA (0XB1736B96B6FD83B3) ,DATA (0XDDD0467C64BCE4A0) ,DATA (0X8AA22C0DBEF60EE4) ,DATA (0XAD4AB7112EB3929D) ,DATA (0XD89D64D57A607744) ,DATA (0X87625F056C7C4A8B) ,DATA (0XA93AF6C6C79B5D2D) ,DATA (0XD389B47879823479) ,DATA (0X843610CB4BF160CB) ,DATA (0XA54394FE1EEDB8FE) ,DATA (0XCE947A3DA6A9273E) ,DATA (0X811CCC668829B887) ,DATA (0XA163FF802A3426A8) ,DATA (0XC9BCFF6034C13052) ,DATA (0XFC2C3F3841F17C67) ,DATA (0X9D9BA7832936EDC0) ,DATA (0XC5029163F384A931) ,DATA (0XF64335BCF065D37D) ,DATA (0X99EA0196163FA42E) ,DATA (0XC06481FB9BCF8D39) ,DATA (0XF07DA27A82C37088) ,DATA (0X964E858C91BA2655) ,DATA (0XBBE226EFB628AFEA) ,DATA (0XEADAB0ABA3B2DBE5) ,DATA (0X92C8AE6B464FC96F) ,DATA (0XB77ADA0617E3BBCB) ,DATA (0XE55990879DDCAABD) ,DATA (0X8F57FA54C2A9EAB6) ,DATA (0XB32DF8E9F3546564) ,DATA (0XDFF9772470297EBD) ,DATA (0X8BFBEA76C619EF36) ,DATA (0XAEFAE51477A06B03) ,DATA (0XDAB99E59958885C4) ,DATA (0X88B402F7FD75539B) ,DATA (0XAAE103B5FCD2A881) ,DATA (0XD59944A37C0752A2) ,DATA (0X857FCAE62D8493A5) ,DATA (0XA6DFBD9FB8E5B88E) ,DATA (0XD097AD07A71F26B2) ,DATA (0X825ECC24C873782F) ,DATA (0XA2F67F2DFA90563B) ,DATA (0XCBB41EF979346BCA) ,DATA (0XFEA126B7D78186BC) ,DATA (0X9F24B832E6B0F436) ,DATA (0XC6EDE63FA05D3143) ,DATA (0XF8A95FCF88747D94) ,DATA (0X9B69DBE1B548CE7C) ,DATA (0XC24452DA229B021B) ,DATA (0XF2D56790AB41C2A2) ,DATA (0X97C560BA6B0919A5) ,DATA (0XBDB6B8E905CB600F) ,DATA (0XED246723473E3813) ,DATA (0X9436C0760C86E30B) ,DATA (0XB94470938FA89BCE) ,DATA (0XE7958CB87392C2C2) ,DATA (0X90BD77F3483BB9B9) ,DATA (0XB4ECD5F01A4AA828) ,DATA (0XE2280B6C20DD5232) ,DATA (0X8D590723948A535F) ,DATA (0XB0AF48EC79ACE837) ,DATA (0XDCDB1B2798182244) ,DATA (0X8A08F0F8BF0F156B) ,DATA (0XAC8B2D36EED2DAC5) ,DATA (0XD7ADF884AA879177) ,DATA (0X86CCBB52EA94BAEA) ,DATA (0XA87FEA27A539E9A5) ,DATA (0XD29FE4B18E88640E) ,DATA (0X83A3EEEEF9153E89) ,DATA (0XA48CEAAAB75A8E2B) ,DATA (0XCDB02555653131B6) ,DATA (0X808E17555F3EBF11) ,DATA (0XA0B19D2AB70E6ED6) ,DATA (0XC8DE047564D20A8B) ,DATA (0XFB158592BE068D2E) ,DATA (0X9CED737BB6C4183D) ,DATA (0XC428D05AA4751E4C) ,DATA (0XF53304714D9265DF) ,DATA (0X993FE2C6D07B7FAB) ,DATA (0XBF8FDB78849A5F96) ,DATA (0XEF73D256A5C0F77C) ,DATA (0X95A8637627989AAD) ,DATA (0XBB127C53B17EC159) ,DATA (0XE9D71B689DDE71AF) ,DATA (0X9226712162AB070D) ,DATA (0XB6B00D69BB55C8D1) ,DATA (0XE45C10C42A2B3B05) ,DATA (0X8EB98A7A9A5B04E3) ,DATA (0XB267ED1940F1C61C) ,DATA (0XDF01E85F912E37A3) ,DATA (0X8B61313BBABCE2C6) ,DATA (0XAE397D8AA96C1B77) ,DATA (0XD9C7DCED53C72255) ,DATA (0X881CEA14545C7575) ,DATA (0XAA242499697392D2) ,DATA (0XD4AD2DBFC3D07787) ,DATA (0X84EC3C97DA624AB4) ,DATA (0XA6274BBDD0FADD61) ,DATA (0XCFB11EAD453994BA) ,DATA (0X81CEB32C4B43FCF4) ,DATA (0XA2425FF75E14FC31) ,DATA (0XCAD2F7F5359A3B3E) ,DATA (0XFD87B5F28300CA0D) ,DATA (0X9E74D1B791E07E48) ,DATA (0XC612062576589DDA) ,DATA (0XF79687AED3EEC551) ,DATA (0X9ABE14CD44753B52) ,DATA (0XC16D9A0095928A27) ,DATA (0XF1C90080BAF72CB1) ,DATA (0X971DA05074DA7BEE) ,DATA (0XBCE5086492111AEA) ,DATA (0XEC1E4A7DB69561A5) ,DATA (0X9392EE8E921D5D07) ,DATA (0XB877AA3236A4B449) ,DATA (0XE69594BEC44DE15B) ,DATA (0X901D7CF73AB0ACD9) ,DATA (0XB424DC35095CD80F) ,DATA (0XE12E13424BB40E13) ,DATA (0X8CBCCC096F5088CB) ,DATA (0XAFEBFF0BCB24AAFE) ,DATA (0XDBE6FECEBDEDD5BE) ,DATA (0X89705F4136B4A597) ,DATA (0XABCC77118461CEFC) ,DATA (0XD6BF94D5E57A42BC) ,DATA (0X8637BD05AF6C69B5) ,DATA (0XA7C5AC471B478423) ,DATA (0XD1B71758E219652B) ,DATA (0X83126E978D4FDF3B) ,DATA (0XA3D70A3D70A3D70A) ,DATA (0XCCCCCCCCCCCCCCCC) ,DATA (0X0000000000000001) ,DATA (0X000000000000000A) ,DATA (0X0000000000000064) ,DATA (0X00000000000003E8) ,DATA (0X0000000000002710) ,DATA (0X00000000000186A0) ,DATA (0X00000000000F4240) ,DATA (0X0000000000989680) ,DATA (0X0000000005F5E100) ,DATA (0X000000003B9ACA00) ,DATA (0X00000002540BE400) ,DATA (0X000000174876E800) ,DATA (0X000000E8D4A51000) ,DATA (0X000009184E72A000) ,DATA (0X00005AF3107A4000) ,DATA (0X00038D7EA4C68000) ,DATA (0X002386F26FC10000) ,DATA (0X016345785D8A0000) ,DATA (0X0DE0B6B3A7640000) ,DATA (0X8AC7230489E80000) ,DATA (0XAD78EBC5AC620000) ,DATA (0XD8D726B7177A8000) ,DATA (0X878678326EAC9000) ,DATA (0XA968163F0A57B400) ,DATA (0XD3C21BCECCEDA100) ,DATA (0X84595161401484A0) ,DATA (0XA56FA5B99019A5C8) ,DATA (0XCECB8F27F4200F3A) ,DATA (0X813F3978F8940984) ,DATA (0XA18F07D736B90BE5) ,DATA (0XC9F2C9CD04674EDE) ,DATA (0XFC6F7C4045812296) ,DATA (0X9DC5ADA82B70B59D) ,DATA (0XC5371912364CE305) ,DATA (0XF684DF56C3E01BC6) ,DATA (0X9A130B963A6C115C) ,DATA (0XC097CE7BC90715B3) ,DATA (0XF0BDC21ABB48DB20) ,DATA (0X96769950B50D88F4) ,DATA (0XBC143FA4E250EB31) ,DATA (0XEB194F8E1AE525FD) ,DATA (0X92EFD1B8D0CF37BE) ,DATA (0XB7ABC627050305AD) ,DATA (0XE596B7B0C643C719) ,DATA (0X8F7E32CE7BEA5C6F) ,DATA (0XB35DBF821AE4F38B) ,DATA (0XE0352F62A19E306E) ,DATA (0X8C213D9DA502DE45) ,DATA (0XAF298D050E4395D6) ,DATA (0XDAF3F04651D47B4C) ,DATA (0X88D8762BF324CD0F) ,DATA (0XAB0E93B6EFEE0053) ,DATA (0XD5D238A4ABE98068) ,DATA (0X85A36366EB71F041) ,DATA (0XA70C3C40A64E6C51) ,DATA (0XD0CF4B50CFE20765) ,DATA (0X82818F1281ED449F) ,DATA (0XA321F2D7226895C7) ,DATA (0XCBEA6F8CEB02BB39) ,DATA (0XFEE50B7025C36A08) ,DATA (0X9F4F2726179A2245) ,DATA (0XC722F0EF9D80AAD6) ,DATA (0XF8EBAD2B84E0D58B) ,DATA (0X9B934C3B330C8577) ,DATA (0XC2781F49FFCFA6D5) ,DATA (0XF316271C7FC3908A) ,DATA (0X97EDD871CFDA3A56) ,DATA (0XBDE94E8E43D0C8EC) ,DATA (0XED63A231D4C4FB27) ,DATA (0X945E455F24FB1CF8) ,DATA (0XB975D6B6EE39E436) ,DATA (0XE7D34C64A9C85D44) ,DATA (0X90E40FBEEA1D3A4A) ,DATA (0XB51D13AEA4A488DD) ,DATA (0XE264589A4DCDAB14) ,DATA (0X8D7EB76070A08AEC) ,DATA (0XB0DE65388CC8ADA8) ,DATA (0XDD15FE86AFFAD912) ,DATA (0X8A2DBF142DFCC7AB) ,DATA (0XACB92ED9397BF996) ,DATA (0XD7E77A8F87DAF7FB) ,DATA (0X86F0AC99B4E8DAFD) ,DATA (0XA8ACD7C0222311BC) ,DATA (0XD2D80DB02AABD62B) ,DATA (0X83C7088E1AAB65DB) ,DATA (0XA4B8CAB1A1563F52) ,DATA (0XCDE6FD5E09ABCF26) ,DATA (0X80B05E5AC60B6178) ,DATA (0XA0DC75F1778E39D6) ,DATA (0XC913936DD571C84C) ,DATA (0XFB5878494ACE3A5F) ,DATA (0X9D174B2DCEC0E47B) ,DATA (0XC45D1DF942711D9A) ,DATA (0XF5746577930D6500) ,DATA (0X9968BF6ABBE85F20) ,DATA (0XBFC2EF456AE276E8) ,DATA (0XEFB3AB16C59B14A2) ,DATA (0X95D04AEE3B80ECE5) ,DATA (0XBB445DA9CA61281F) ,DATA (0XEA1575143CF97226) ,DATA (0X924D692CA61BE758) ,DATA (0XB6E0C377CFA2E12E) ,DATA (0XE498F455C38B997A) ,DATA (0X8EDF98B59A373FEC) ,DATA (0XB2977EE300C50FE7) ,DATA (0XDF3D5E9BC0F653E1) ,DATA (0X8B865B215899F46C) ,DATA (0XAE67F1E9AEC07187) ,DATA (0XDA01EE641A708DE9) ,DATA (0X884134FE908658B2) ,DATA (0XAA51823E34A7EEDE) ,DATA (0XD4E5E2CDC1D1EA96) ,DATA (0X850FADC09923329E) ,DATA (0XA6539930BF6BFF45) ,DATA (0XCFE87F7CEF46FF16) ,DATA (0X81F14FAE158C5F6E) ,DATA (0XA26DA3999AEF7749) ,DATA (0XCB090C8001AB551C) ,DATA (0XFDCB4FA002162A63) ,DATA (0X9E9F11C4014DDA7E) ,DATA (0XC646D63501A1511D) ,DATA (0XF7D88BC24209A565) ,DATA (0X9AE757596946075F) ,DATA (0XC1A12D2FC3978937) ,DATA (0XF209787BB47D6B84) ,DATA (0X9745EB4D50CE6332) ,DATA (0XBD176620A501FBFF) ,DATA (0XEC5D3FA8CE427AFF) ,DATA (0X93BA47C980E98CDF) ,DATA (0XB8A8D9BBE123F017) ,DATA (0XE6D3102AD96CEC1D) ,DATA (0X9043EA1AC7E41392) ,DATA (0XB454E4A179DD1877) ,DATA (0XE16A1DC9D8545E94) ,DATA (0X8CE2529E2734BB1D) ,DATA (0XB01AE745B101E9E4) ,DATA (0XDC21A1171D42645D) ,DATA (0X899504AE72497EBA) ,DATA (0XABFA45DA0EDBDE69) ,DATA (0XD6F8D7509292D603) ,DATA (0X865B86925B9BC5C2) ,DATA (0XA7F26836F282B732) ,DATA (0XD1EF0244AF2364FF) ,DATA (0X8335616AED761F1F) ,DATA (0XA402B9C5A8D3A6E7) ,DATA (0XCD036837130890A1) ,DATA (0X802221226BE55A64) ,DATA (0XA02AA96B06DEB0FD) ,DATA (0XC83553C5C8965D3D) ,DATA (0XFA42A8B73ABBF48C) ,DATA (0X9C69A97284B578D7) ,DATA (0XC38413CF25E2D70D) ,DATA (0XF46518C2EF5B8CD1) ,DATA (0X98BF2F79D5993802) ,DATA (0XBEEEFB584AFF8603) ,DATA (0XEEAABA2E5DBF6784) ,DATA (0X952AB45CFA97A0B2) ,DATA (0XBA756174393D88DF) ,DATA (0XE912B9D1478CEB17) ,DATA (0X91ABB422CCB812EE) ,DATA (0XB616A12B7FE617AA) ,DATA (0XE39C49765FDF9D94) ,DATA (0X8E41ADE9FBEBC27D) ,DATA (0XB1D219647AE6B31C) ,DATA (0XDE469FBD99A05FE3) ,DATA (0X8AEC23D680043BEE) ,DATA (0XADA72CCC20054AE9) ,DATA (0XD910F7FF28069DA4) ,DATA (0X87AA9AFF79042286) ,DATA (0XA99541BF57452B28) ,DATA (0XD3FA922F2D1675F2) ,DATA (0X847C9B5D7C2E09B7) ,DATA (0XA59BC234DB398C25) ,DATA (0XCF02B2C21207EF2E) ,DATA (0X8161AFB94B44F57D) ,DATA (0XA1BA1BA79E1632DC) ,DATA (0XCA28A291859BBF93) ,DATA (0XFCB2CB35E702AF78) ,DATA (0X9DEFBF01B061ADAB) ,DATA (0XC56BAEC21C7A1916) ,DATA (0XF6C69A72A3989F5B) ,DATA (0X9A3C2087A63F6399) ,DATA (0XC0CB28A98FCF3C7F) ,DATA (0XF0FDF2D3F3C30B9F) ,DATA (0X969EB7C47859E743) ,DATA (0XBC4665B596706114) ,DATA (0XEB57FF22FC0C7959) ,DATA (0X9316FF75DD87CBD8) ,DATA (0XB7DCBF5354E9BECE) ,DATA (0XE5D3EF282A242E81) ,DATA (0X8FA475791A569D10) ,DATA (0XB38D92D760EC4455) ,DATA (0XE070F78D3927556A) ,DATA (0X8C469AB843B89562) ,DATA (0XAF58416654A6BABB) ,DATA (0XDB2E51BFE9D0696A) ,DATA (0X88FCF317F22241E2) ,DATA (0XAB3C2FDDEEAAD25A) ,DATA (0XD60B3BD56A5586F1) ,DATA (0X85C7056562757456) ,DATA (0XA738C6BEBB12D16C) ,DATA (0XD106F86E69D785C7) ,DATA (0X82A45B450226B39C) ,DATA (0XA34D721642B06084) ,DATA (0XCC20CE9BD35C78A5) ,DATA (0XFF290242C83396CE) ,DATA (0X9F79A169BD203E41) ,DATA (0XC75809C42C684DD1) ,DATA (0XF92E0C3537826145) ,DATA (0X9BBCC7A142B17CCB) ,DATA (0XC2ABF989935DDBFE) ,DATA (0XF356F7EBF83552FE) ,DATA (0X98165AF37B2153DE) ,DATA (0XBE1BF1B059E9A8D6) ,DATA (0XEDA2EE1C7064130C) ,DATA (0X9485D4D1C63E8BE7) ,DATA (0XB9A74A0637CE2EE1) ,DATA (0XE8111C87C5C1BA99) ,DATA (0X910AB1D4DB9914A0) ,DATA (0XB54D5E4A127F59C8) ,DATA (0XE2A0B5DC971F303A) ,DATA (0X8DA471A9DE737E24) ,DATA (0XB10D8E1456105DAD) ,DATA (0XDD50F1996B947518) ,DATA (0X8A5296FFE33CC92F) ,DATA (0XACE73CBFDC0BFB7B) ,DATA (0XD8210BEFD30EFA5A) ,DATA (0X8714A775E3E95C78) ,DATA (0XA8D9D1535CE3B396) ,DATA (0XD31045A8341CA07C) ,DATA (0X83EA2B892091E44D) ,DATA (0XA4E4B66B68B65D60) ,DATA (0XCE1DE40642E3F4B9) ,DATA (0X80D2AE83E9CE78F3) ,DATA (0XA1075A24E4421730) ,DATA (0XC94930AE1D529CFC) ,DATA (0XFB9B7CD9A4A7443C) ,DATA (0X9D412E0806E88AA5) ,DATA (0XC491798A08A2AD4E) ,DATA (0XF5B5D7EC8ACB58A2) ,DATA (0X9991A6F3D6BF1765) ,DATA (0XBFF610B0CC6EDD3F) ,DATA (0XEFF394DCFF8A948E) ,DATA (0X95F83D0A1FB69CD9) ,DATA (0XBB764C4CA7A4440F) ,DATA (0XEA53DF5FD18D5513) ,DATA (0X92746B9BE2F8552C) ,DATA (0XB7118682DBB66A77) ,DATA (0XE4D5E82392A40515) ,DATA (0X8F05B1163BA6832D) ,DATA (0XB2C71D5BCA9023F8) ,DATA (0XDF78E4B2BD342CF6) ,DATA (0X8BAB8EEFB6409C1A) ,DATA (0XAE9672ABA3D0C320) ,DATA (0XDA3C0F568CC4F3E8) ,DATA (0X8865899617FB1871) ,DATA (0XAA7EEBFB9DF9DE8D) ,DATA (0XD51EA6FA85785631) ,DATA (0X8533285C936B35DE) ,DATA (0XA67FF273B8460356) ,DATA (0XD01FEF10A657842C) ,DATA (0X8213F56A67F6B29B) ,DATA (0XA298F2C501F45F42) ,DATA (0XCB3F2F7642717713) ,DATA (0XFE0EFB53D30DD4D7) ,DATA (0X9EC95D1463E8A506) ,DATA (0XC67BB4597CE2CE48) ,DATA (0XF81AA16FDC1B81DA) ,DATA (0X9B10A4E5E9913128) ,DATA (0XC1D4CE1F63F57D72) ,DATA (0XF24A01A73CF2DCCF) ,DATA (0X976E41088617CA01) ,DATA (0XBD49D14AA79DBC82) ,DATA (0XEC9C459D51852BA2) ,DATA (0X93E1AB8252F33B45) ,DATA (0XB8DA1662E7B00A17) ,DATA (0XE7109BFBA19C0C9D) ,DATA (0X906A617D450187E2) ,DATA (0XB484F9DC9641E9DA) ,DATA (0XE1A63853BBD26451) ,DATA (0X8D07E33455637EB2) ,DATA (0XB049DC016ABC5E5F) ,DATA (0XDC5C5301C56B75F7) ,DATA (0X89B9B3E11B6329BA) ,DATA (0XAC2820D9623BF429) ,DATA (0XD732290FBACAF133) ,DATA (0X867F59A9D4BED6C0) ,DATA (0XA81F301449EE8C70) ,DATA (0XD226FC195C6A2F8C) ,DATA (0X83585D8FD9C25DB7) ,DATA (0XA42E74F3D032F525) ,DATA (0XCD3A1230C43FB26F) ,DATA (0X80444B5E7AA7CF85) ,DATA (0XA0555E361951C366) ,DATA (0XC86AB5C39FA63440) ,DATA (0XFA856334878FC150) ,DATA (0X9C935E00D4B9D8D2) ,DATA (0XC3B8358109E84F07) ,DATA (0XF4A642E14C6262C8) ,DATA (0X98E7E9CCCFBD7DBD) ,DATA (0XBF21E44003ACDD2C) ,DATA (0XEEEA5D5004981478) ,DATA (0X95527A5202DF0CCB) ,DATA (0XBAA718E68396CFFD) ,DATA (0XE950DF20247C83FD) ,DATA (0X91D28B7416CDD27E) ,DATA (0XB6472E511C81471D) ,DATA (0XE3D8F9E563A198E5) ,DATA (0X8E679C2F5E44FF8F) ,DATA (0XB201833B35D63F73) ,DATA (0XDE81E40A034BCF4F) ,DATA (0X8B112E86420F6191) ,DATA (0XADD57A27D29339F6) ,DATA (0XD94AD8B1C7380874) ,DATA (0X87CEC76F1C830548) ,DATA (0XA9C2794AE3A3C69A) ,DATA (0XD433179D9C8CB841) ,DATA (0X849FEEC281D7F328) ,DATA (0XA5C7EA73224DEFF3) ,DATA (0XCF39E50FEAE16BEF) ,DATA (0X81842F29F2CCE375) ,DATA (0XA1E53AF46F801C53) ,DATA (0XCA5E89B18B602368) ,DATA (0XFCF62C1DEE382C42) ,DATA (0X9E19DB92B4E31BA9) ,DATA (0XC5A05277621BE293) ,DATA (0XF70867153AA2DB38) ,DATA (0X9A65406D44A5C903) ,DATA (0XC0FE908895CF3B44) ,DATA (0XF13E34AABB430A15) ,DATA (0X96C6E0EAB509E64D) ,DATA (0XBC789925624C5FE0) ,DATA (0XEB96BF6EBADF77D8) ,DATA (0X933E37A534CBAAE7) ,DATA (0XB80DC58E81FE95A1) ,DATA (0XE61136F2227E3B09) ,DATA (0X8FCAC257558EE4E6) ,DATA (0XB3BD72ED2AF29E1F) ,DATA (0XE0ACCFA875AF45A7) ,DATA (0X8C6C01C9498D8B88) ,DATA (0XAF87023B9BF0EE6A) ,DATA (0XDB68C2CA82ED2A05) ,DATA (0X892179BE91D43A43) ,DATA (0XAB69D82E364948D4) ,DATA (0XD6444E39C3DB9B09) ,DATA (0X85EAB0E41A6940E5) ,DATA (0XA7655D1D2103911F)} ;
			static const LENGTH M_ECACHE[] {
				-1213 ,-1210 ,-1206 ,-1203 ,-1200 ,-1196 ,-1193 ,-1190 ,-1186 ,-1183 ,-1180 ,-1176 ,-1173 ,-1170 ,-1166 ,-1163 ,-1160 ,-1156 ,-1153 ,-1150 ,-1146 ,-1143 ,-1140 ,-1136 ,-1133 ,-1130 ,-1127 ,-1123 ,-1120 ,-1117 ,-1113 ,-1110 ,-1107 ,-1103 ,-1100 ,-1097 ,-1093 ,-1090 ,-1087 ,-1083 ,-1080 ,-1077 ,-1073 ,-1070 ,-1067 ,-1063 ,-1060 ,-1057 ,-1053 ,-1050 ,-1047 ,-1043 ,-1040 ,-1037 ,-1034 ,-1030 ,-1027 ,-1024 ,-1020 ,-1017 ,-1014 ,-1010 ,-1007 ,-1004 ,-1000 ,-997 ,-994 ,-990 ,-987 ,-984 ,-980 ,-977 ,-974 ,-970 ,-967 ,-964 ,-960 ,-957 ,-954 ,-950 ,-947 ,-944 ,-940 ,-937 ,-934 ,-931 ,-927 ,-924 ,-921 ,-917 ,-914 ,-911 ,-907 ,-904 ,-901 ,-897 ,-894 ,-891 ,-887 ,-884 ,-881 ,-877 ,-874 ,-871 ,-867 ,-864 ,-861 ,-857 ,-854 ,-851 ,-847 ,-844 ,-841 ,-838 ,-834 ,-831 ,-828 ,-824 ,-821 ,-818 ,-814 ,-811 ,-808 ,-804 ,-801 ,-798 ,-794 ,-791 ,-788 ,-784 ,-781 ,-778 ,-774 ,-771 ,-768 ,-764 ,-761 ,-758 ,-754 ,-751 ,-748 ,-744 ,-741 ,-738 ,-735 ,-731 ,-728 ,-725 ,-721 ,-718 ,-715 ,-711 ,-708 ,-705 ,-701 ,-698 ,-695 ,-691 ,-688 ,-685 ,-681 ,-678 ,-675 ,-671 ,-668 ,-665 ,-661 ,-658 ,-655 ,-651 ,-648 ,-645 ,-642 ,-638 ,-635 ,-632 ,-628 ,-625 ,-622 ,-618 ,-615 ,-612 ,-608 ,-605 ,-602 ,-598 ,-595 ,-592 ,-588 ,-585 ,-582 ,-578 ,-575 ,-572 ,-568 ,-565 ,-562 ,-558 ,-555 ,-552 ,-549 ,-545 ,-542 ,-539 ,-535 ,-532 ,-529 ,-525 ,-522 ,-519 ,-515 ,-512 ,-509 ,-505 ,-502 ,-499 ,-495 ,-492 ,-489 ,-485 ,-482 ,-479 ,-475 ,-472 ,-469 ,-465 ,-462 ,-459 ,-455 ,-452 ,-449 ,-446 ,-442 ,-439 ,-436 ,-432 ,-429 ,-426 ,-422 ,-419 ,-416 ,-412 ,-409 ,-406 ,-402 ,-399 ,-396 ,-392 ,-389 ,-386 ,-382 ,-379 ,-376 ,-372 ,-369 ,-366 ,-362 ,-359 ,-356 ,-353 ,-349 ,-346 ,-343 ,-339 ,-336 ,-333 ,-329 ,-326 ,-323 ,-319 ,-316 ,-313 ,-309 ,-306 ,-303 ,-299 ,-296 ,-293 ,-289 ,-286 ,-283 ,-279 ,-276 ,-273 ,-269 ,-266 ,-263 ,-259 ,-256 ,-253 ,-250 ,-246 ,-243 ,-240 ,-236 ,-233 ,-230 ,-226 ,-223 ,-220 ,-216 ,-213 ,-210 ,-206 ,-203 ,-200 ,-196 ,-193 ,-190 ,-186 ,-183 ,-180 ,-176 ,-173 ,-170 ,-166 ,-163 ,-160 ,-157 ,-153 ,-150 ,-147 ,-143 ,-140 ,-137 ,-133 ,-130 ,-127 ,-123 ,-120 ,-117 ,-113 ,-110 ,-107 ,-103 ,-100 ,-97 ,-93 ,-90 ,-87 ,-83 ,-80 ,-77 ,-73 ,-70 ,-67 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,3 ,6 ,10 ,13 ,16 ,20 ,23 ,26 ,30 ,33 ,36 ,39 ,43 ,46 ,49 ,53 ,56 ,59 ,63 ,66 ,69 ,73 ,76 ,79 ,83 ,86 ,89 ,93 ,96 ,99 ,103 ,106 ,109 ,113 ,116 ,119 ,123 ,126 ,129 ,132 ,136 ,139 ,142 ,146 ,149 ,152 ,156 ,159 ,162 ,166 ,169 ,172 ,176 ,179 ,182 ,186 ,189 ,192 ,196 ,199 ,202 ,206 ,209 ,212 ,216 ,219 ,222 ,226 ,229 ,232 ,235 ,239 ,242 ,245 ,249 ,252 ,255 ,259 ,262 ,265 ,269 ,272 ,275 ,279 ,282 ,285 ,289 ,292 ,295 ,299 ,302 ,305 ,309 ,312 ,315 ,319 ,322 ,325 ,328 ,332 ,335 ,338 ,342 ,345 ,348 ,352 ,355 ,358 ,362 ,365 ,368 ,372 ,375 ,378 ,382 ,385 ,388 ,392 ,395 ,398 ,402 ,405 ,408 ,412 ,415 ,418 ,422 ,425 ,428 ,431 ,435 ,438 ,441 ,445 ,448 ,451 ,455 ,458 ,461 ,465 ,468 ,471 ,475 ,478 ,481 ,485 ,488 ,491 ,495 ,498 ,501 ,505 ,508 ,511 ,515 ,518 ,521 ,524 ,528 ,531 ,534 ,538 ,541 ,544 ,548 ,551 ,554 ,558 ,561 ,564 ,568 ,571 ,574 ,578 ,581 ,584 ,588 ,591 ,594 ,598 ,601 ,604 ,608 ,611 ,614 ,617 ,621 ,624 ,627 ,631 ,634 ,637 ,641 ,644 ,647 ,651 ,654 ,657 ,661 ,664 ,667 ,671 ,674 ,677 ,681 ,684 ,687 ,691 ,694 ,697 ,701 ,704 ,707 ,711 ,714 ,717 ,720 ,724 ,727 ,730 ,734 ,737 ,740 ,744 ,747 ,750 ,754 ,757 ,760 ,764 ,767 ,770 ,774 ,777 ,780 ,784 ,787 ,790 ,794 ,797 ,800 ,804 ,807 ,810 ,813 ,817 ,820 ,823 ,827 ,830 ,833 ,837 ,840 ,843 ,847 ,850 ,853 ,857 ,860 ,863 ,867 ,870 ,873 ,877 ,880 ,883 ,887 ,890 ,893 ,897 ,900 ,903 ,907 ,910 ,913 ,916 ,920 ,923 ,926 ,930 ,933 ,936 ,940 ,943 ,946 ,950 ,953 ,956 ,960 ,963 ,966 ,970 ,973 ,976 ,980 ,983 ,986 ,990 ,993 ,996 ,1000 ,1003 ,1006 ,1009 ,1013 ,1016 ,1019 ,1023 ,1026 ,1029 ,1033 ,1036 ,1039 ,1043 ,1046 ,1049 ,1053 ,1056 ,1059 ,1063 ,1066 ,1069 ,1073 ,1076 ,1079 ,1083 ,1086} ;
			return memorize ([&] () {
				FEXP2Cache ret ;
				ret.mMCache = BoxBuffer<DATA ,SIZE> (M_MCACHE) ;
				ret.mECache = BoxBuffer<LENGTH ,SIZE> (M_ECACHE) ;
				return move (ret) ;
			}) ;
		}

		inline NOTATION operator[] (CREF<VAL64> k) const {
			NOTATION ret ;
			ret.mRadix = 2 ;
			ret.mSign = FALSE ;
			const auto r1x = SIZE::expr / 2 + INDEX (k) ;
			assume (vbetween (r1x ,0 ,SIZE::expr)) ;
			ret.mMantissa = VAL64 (mMCache[r1x]) ;
			ret.mPrecision = 0 ;
			ret.mExponent = VAL64 (mECache[r1x]) ;
			return move (ret) ;
		}
	} ;
} ;

template <class DEPEND>
trait FLOATPROC_FEXP10CACHE_HELP<DEPEND ,ALWAYS> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;

	using SIZE = ENUMAS<VAL ,2175> ;

	class FEXP10Cache {
	protected:
		BoxBuffer<DATA ,SIZE> mMCache ;
		BoxBuffer<LENGTH ,SIZE> mECache ;

	public:
		imports CREF<FEXP10Cache> instance () {
			static const DATA M_MCACHE[] {
				DATA (0X53B2AE8E9081C88F) ,DATA (0XA7655D1D2103911F) ,DATA (0X217AAC39069A5039) ,DATA (0X42F558720D34A072) ,DATA (0X85EAB0E41A6940E5) ,DATA (0X1AC889C7387B7361) ,DATA (0X3591138E70F6E6C2) ,DATA (0X6B22271CE1EDCD84) ,DATA (0XD6444E39C3DB9B09) ,DATA (0X2ADA760B8D925235) ,DATA (0X55B4EC171B24A46A) ,DATA (0XAB69D82E364948D4) ,DATA (0X22485E6FA4750E90) ,DATA (0X4490BCDF48EA1D21) ,DATA (0X892179BE91D43A43) ,DATA (0X1B6D1859505DA540) ,DATA (0X36DA30B2A0BB4A81) ,DATA (0X6DB4616541769502) ,DATA (0XDB68C2CA82ED2A05) ,DATA (0X2BE1C08EE6FC3B9A) ,DATA (0X57C3811DCDF87735) ,DATA (0XAF87023B9BF0EE6A) ,DATA (0X231B0072526362E2) ,DATA (0X463600E4A4C6C5C4) ,DATA (0X8C6C01C9498D8B88) ,DATA (0X1C1599F50EB5E8B4) ,DATA (0X382B33EA1D6BD169) ,DATA (0X705667D43AD7A2D3) ,DATA (0XE0ACCFA875AF45A7) ,DATA (0X2CEF5CBB4ABCA787) ,DATA (0X59DEB97695794F0F) ,DATA (0XB3BD72ED2AF29E1F) ,DATA (0X23F2B095D563B939) ,DATA (0X47E5612BAAC77273) ,DATA (0X8FCAC257558EE4E6) ,DATA (0X1CC226DE444FC761) ,DATA (0X39844DBC889F8EC2) ,DATA (0X73089B79113F1D84) ,DATA (0XE61136F2227E3B09) ,DATA (0X2E037163A07FA568) ,DATA (0X5C06E2C740FF4AD0) ,DATA (0XB80DC58E81FE95A1) ,DATA (0X24CF8DE94D32EAB9) ,DATA (0X499F1BD29A65D573) ,DATA (0X933E37A534CBAAE7) ,DATA (0X1D72D7EDD75BEEFB) ,DATA (0X3AE5AFDBAEB7DDF6) ,DATA (0X75CB5FB75D6FBBEC) ,DATA (0XEB96BF6EBADF77D8) ,DATA (0X2F1E2649589317F8) ,DATA (0X5E3C4C92B1262FF0) ,DATA (0XBC789925624C5FE0) ,DATA (0X25B1B83AAD427993) ,DATA (0X4B6370755A84F326) ,DATA (0X96C6E0EAB509E64D) ,DATA (0X1E27C69557686142) ,DATA (0X3C4F8D2AAED0C285) ,DATA (0X789F1A555DA1850A) ,DATA (0XF13E34AABB430A15) ,DATA (0X303FA4222573CED1) ,DATA (0X607F48444AE79DA2) ,DATA (0XC0FE908895CF3B44) ,DATA (0X2699501B51297240) ,DATA (0X4D32A036A252E481) ,DATA (0X9A65406D44A5C903) ,DATA (0X1EE10CE2A7545B67) ,DATA (0X3DC219C54EA8B6CE) ,DATA (0X7B84338A9D516D9C) ,DATA (0XF70867153AA2DB38) ,DATA (0X3168149DD886F8A4) ,DATA (0X62D0293BB10DF149) ,DATA (0XC5A05277621BE293) ,DATA (0X278676E4AD38C6EA) ,DATA (0X4F0CEDC95A718DD4) ,DATA (0X9E19DB92B4E31BA9) ,DATA (0X1F9EC583BDC70588) ,DATA (0X3F3D8B077B8E0B10) ,DATA (0X7E7B160EF71C1621) ,DATA (0XFCF62C1DEE382C42) ,DATA (0X3297A26C62D808DA) ,DATA (0X652F44D8C5B011B4) ,DATA (0XCA5E89B18B602368) ,DATA (0X28794EBD1BE00714) ,DATA (0X50F29D7A37C00E29) ,DATA (0XA1E53AF46F801C53) ,DATA (0X20610BCA7CB338DD) ,DATA (0X40C21794F96671BA) ,DATA (0X81842F29F2CCE375) ,DATA (0X19E73CA1FD5C2D7D) ,DATA (0X33CE7943FAB85AFB) ,DATA (0X679CF287F570B5F7) ,DATA (0XCF39E50FEAE16BEF) ,DATA (0X2971FA9CC8937BFC) ,DATA (0X52E3F5399126F7F9) ,DATA (0XA5C7EA73224DEFF3) ,DATA (0X2127FBB0A075FCCA) ,DATA (0X424FF76140EBF994) ,DATA (0X849FEEC281D7F328) ,DATA (0X1A8662F3B3919708) ,DATA (0X350CC5E767232E10) ,DATA (0X6A198BCECE465C20) ,DATA (0XD433179D9C8CB841) ,DATA (0X2A709E52B8E8F1A6) ,DATA (0X54E13CA571D1E34D) ,DATA (0XA9C2794AE3A3C69A) ,DATA (0X21F3B1DBC720C152) ,DATA (0X43E763B78E4182A4) ,DATA (0X87CEC76F1C830548) ,DATA (0X1B295B1638E7010E) ,DATA (0X3652B62C71CE021D) ,DATA (0X6CA56C58E39C043A) ,DATA (0XD94AD8B1C7380874) ,DATA (0X2B755E89F4A4CE7D) ,DATA (0X56EABD13E9499CFB) ,DATA (0XADD57A27D29339F6) ,DATA (0X22C44BA19083D864) ,DATA (0X458897432107B0C8) ,DATA (0X8B112E86420F6191) ,DATA (0X1BD03C81406979E9) ,DATA (0X37A0790280D2F3D3) ,DATA (0X6F40F20501A5E7A7) ,DATA (0XDE81E40A034BCF4F) ,DATA (0X2C8060CECD758FDC) ,DATA (0X5900C19D9AEB1FB9) ,DATA (0XB201833B35D63F73) ,DATA (0X2399E70BD7913FE3) ,DATA (0X4733CE17AF227FC7) ,DATA (0X8E679C2F5E44FF8F) ,DATA (0X1C7B1F3CAC74331C) ,DATA (0X38F63E7958E86639) ,DATA (0X71EC7CF2B1D0CC72) ,DATA (0XE3D8F9E563A198E5) ,DATA (0X2D91CB94472051C7) ,DATA (0X5B2397288E40A38E) ,DATA (0XB6472E511C81471D) ,DATA (0X2474A2DD05B3749F) ,DATA (0X48E945BA0B66E93F) ,DATA (0X91D28B7416CDD27E) ,DATA (0X1D2A1BE4048F907F) ,DATA (0X3A5437C8091F20FF) ,DATA (0X74A86F90123E41FE) ,DATA (0XE950DF20247C83FD) ,DATA (0X2EA9C639A0E5B3FF) ,DATA (0X5D538C7341CB67FE) ,DATA (0XBAA718E68396CFFD) ,DATA (0X25549E9480B7C332) ,DATA (0X4AA93D29016F8665) ,DATA (0X95527A5202DF0CCB) ,DATA (0X1DDD4BAA0093028F) ,DATA (0X3BBA97540126051E) ,DATA (0X77752EA8024C0A3C) ,DATA (0XEEEA5D5004981478) ,DATA (0X2FC8791000EB374B) ,DATA (0X5F90F22001D66E96) ,DATA (0XBF21E44003ACDD2C) ,DATA (0X2639FA7333EF5F6F) ,DATA (0X4C73F4E667DEBEDE) ,DATA (0X98E7E9CCCFBD7DBD) ,DATA (0X1E94C85C298C4C59) ,DATA (0X3D2990B8531898B2) ,DATA (0X7A532170A6313164) ,DATA (0XF4A642E14C6262C8) ,DATA (0X30EE0D60427A13C1) ,DATA (0X61DC1AC084F42783) ,DATA (0XC3B8358109E84F07) ,DATA (0X2724D780352E7634) ,DATA (0X4E49AF006A5CEC69) ,DATA (0X9C935E00D4B9D8D2) ,DATA (0X1F50AC6690F1F82A) ,DATA (0X3EA158CD21E3F054) ,DATA (0X7D42B19A43C7E0A8) ,DATA (0XFA856334878FC150) ,DATA (0X321AAD70E7E98D10) ,DATA (0X64355AE1CFD31A20) ,DATA (0XC86AB5C39FA63440) ,DATA (0X2815578D865470D9) ,DATA (0X502AAF1B0CA8E1B3) ,DATA (0XA0555E361951C366) ,DATA (0X201112D79EA9F3E1) ,DATA (0X402225AF3D53E7C2) ,DATA (0X80444B5E7AA7CF85) ,DATA (0X19A742461887F64D) ,DATA (0X334E848C310FEC9B) ,DATA (0X669D0918621FD937) ,DATA (0XCD3A1230C43FB26F) ,DATA (0X290B9D3CF40CBD49) ,DATA (0X52173A79E8197A92) ,DATA (0XA42E74F3D032F525) ,DATA (0X20D61763F670976D) ,DATA (0X41AC2EC7ECE12EDB) ,DATA (0X83585D8FD9C25DB7) ,DATA (0X1A44DF832B8D45F1) ,DATA (0X3489BF06571A8BE3) ,DATA (0X69137E0CAE3517C6) ,DATA (0XD226FC195C6A2F8C) ,DATA (0X2A07CC05127BA31C) ,DATA (0X540F980A24F74638) ,DATA (0XA81F301449EE8C70) ,DATA (0X219FD66A752FB5B0) ,DATA (0X433FACD4EA5F6B60) ,DATA (0X867F59A9D4BED6C0) ,DATA (0X1AE64521F7595E26) ,DATA (0X35CC8A43EEB2BC4C) ,DATA (0X6B991487DD657899) ,DATA (0XD732290FBACAF133) ,DATA (0X2B0A0836588EFD0A) ,DATA (0X5614106CB11DFA14) ,DATA (0XAC2820D9623BF429) ,DATA (0X226E6CF846D8CA6E) ,DATA (0X44DCD9F08DB194DD) ,DATA (0X89B9B3E11B6329BA) ,DATA (0X1B8B8A6038AD6EBE) ,DATA (0X371714C0715ADD7D) ,DATA (0X6E2E2980E2B5BAFB) ,DATA (0XDC5C5301C56B75F7) ,DATA (0X2C1277005AAF1797) ,DATA (0X5824EE00B55E2F2F) ,DATA (0XB049DC016ABC5E5F) ,DATA (0X2341F8CD1558DFAC) ,DATA (0X4683F19A2AB1BF59) ,DATA (0X8D07E33455637EB2) ,DATA (0X1C34C70A777A4C8A) ,DATA (0X38698E14EEF49914) ,DATA (0X70D31C29DDE93228) ,DATA (0XE1A63853BBD26451) ,DATA (0X2D213E7725907A76) ,DATA (0X5A427CEE4B20F4ED) ,DATA (0XB484F9DC9641E9DA) ,DATA (0X241A985F514061F8) ,DATA (0X483530BEA280C3F1) ,DATA (0X906A617D450187E2) ,DATA (0X1CE2137F74338193) ,DATA (0X39C426FEE8670327) ,DATA (0X73884DFDD0CE064E) ,DATA (0XE7109BFBA19C0C9D) ,DATA (0X2E368598B9EC0285) ,DATA (0X5C6D0B3173D8050B) ,DATA (0XB8DA1662E7B00A17) ,DATA (0X24F86AE094BCCED1) ,DATA (0X49F0D5C129799DA2) ,DATA (0X93E1AB8252F33B45) ,DATA (0X1D9388B3AA30A574) ,DATA (0X3B27116754614AE8) ,DATA (0X764E22CEA8C295D1) ,DATA (0XEC9C459D51852BA2) ,DATA (0X2F527452A9E76F20) ,DATA (0X5EA4E8A553CEDE41) ,DATA (0XBD49D14AA79DBC82) ,DATA (0X25DB90422185F280) ,DATA (0X4BB72084430BE500) ,DATA (0X976E41088617CA01) ,DATA (0X1E494034E79E5B99) ,DATA (0X3C928069CF3CB733) ,DATA (0X792500D39E796E67) ,DATA (0XF24A01A73CF2DCCF) ,DATA (0X30753387D8FD5F5C) ,DATA (0X60EA670FB1FABEB9) ,DATA (0XC1D4CE1F63F57D72) ,DATA (0X26C429397A644C4A) ,DATA (0X4D885272F4C89894) ,DATA (0X9B10A4E5E9913128) ,DATA (0X1F03542DFB83703B) ,DATA (0X3E06A85BF706E076) ,DATA (0X7C0D50B7EE0DC0ED) ,DATA (0XF81AA16FDC1B81DA) ,DATA (0X319EED165F38B392) ,DATA (0X633DDA2CBE716724) ,DATA (0XC67BB4597CE2CE48) ,DATA (0X27B2574518FA2941) ,DATA (0X4F64AE8A31F45283) ,DATA (0X9EC95D1463E8A506) ,DATA (0X1FC1DF6A7A61BA9A) ,DATA (0X3F83BED4F4C37535) ,DATA (0X7F077DA9E986EA6B) ,DATA (0XFE0EFB53D30DD4D7) ,DATA (0X32CFCBDD909C5DC4) ,DATA (0X659F97BB2138BB89) ,DATA (0XCB3F2F7642717713) ,DATA (0X28A63CB1407D17D0) ,DATA (0X514C796280FA2FA1) ,DATA (0XA298F2C501F45F42) ,DATA (0X2084FD5A99FDACA6) ,DATA (0X4109FAB533FB594D) ,DATA (0X8213F56A67F6B29B) ,DATA (0X1A03FDE214CAF085) ,DATA (0X3407FBC42995E10B) ,DATA (0X680FF788532BC216) ,DATA (0XD01FEF10A657842C) ,DATA (0X299FFC9CEE1180D5) ,DATA (0X533FF939DC2301AB) ,DATA (0XA67FF273B8460356) ,DATA (0X214CCA1724DACD77) ,DATA (0X4299942E49B59AEF) ,DATA (0X8533285C936B35DE) ,DATA (0X1AA3D4DF50AF0AC6) ,DATA (0X3547A9BEA15E158C) ,DATA (0X6A8F537D42BC2B18) ,DATA (0XD51EA6FA85785631) ,DATA (0X2A9FBAFEE77E77A3) ,DATA (0X553F75FDCEFCEF46) ,DATA (0XAA7EEBFB9DF9DE8D) ,DATA (0X2219626585FEC61C) ,DATA (0X4432C4CB0BFD8C38) ,DATA (0X8865899617FB1871) ,DATA (0X1B4781EAD1989E7D) ,DATA (0X368F03D5A3313CFA) ,DATA (0X6D1E07AB466279F4) ,DATA (0XDA3C0F568CC4F3E8) ,DATA (0X2BA59CAAE8F430C8) ,DATA (0X574B3955D1E86190) ,DATA (0XAE9672ABA3D0C320) ,DATA (0X22EAE3BBED902706) ,DATA (0X45D5C777DB204E0D) ,DATA (0X8BAB8EEFB6409C1A) ,DATA (0X1BEF1C9657A6859E) ,DATA (0X37DE392CAF4D0B3D) ,DATA (0X6FBC72595E9A167B) ,DATA (0XDF78E4B2BD342CF6) ,DATA (0X2CB1C756F2A408FE) ,DATA (0X59638EADE54811FC) ,DATA (0XB2C71D5BCA9023F8) ,DATA (0X23C16C458EE9A0CB) ,DATA (0X4782D88B1DD34196) ,DATA (0X8F05B1163BA6832D) ,DATA (0X1C9ABD04725480A2) ,DATA (0X39357A08E4A90145) ,DATA (0X726AF411C952028A) ,DATA (0XE4D5E82392A40515) ,DATA (0X2DC461A0B6ED9A9D) ,DATA (0X5B88C3416DDB353B) ,DATA (0XB7118682DBB66A77) ,DATA (0X249D1AE6F8BE154B) ,DATA (0X493A35CDF17C2A96) ,DATA (0X92746B9BE2F8552C) ,DATA (0X1D4A7BEBFA31AAA2) ,DATA (0X3A94F7D7F4635544) ,DATA (0X7529EFAFE8C6AA89) ,DATA (0XEA53DF5FD18D5513) ,DATA (0X2EDD931329E91103) ,DATA (0X5DBB262653D22207) ,DATA (0XBB764C4CA7A4440F) ,DATA (0X257E0F4287EDA736) ,DATA (0X4AFC1E850FDB4E6C) ,DATA (0X95F83D0A1FB69CD9) ,DATA (0X1DFE729B9FF15291) ,DATA (0X3BFCE5373FE2A523) ,DATA (0X77F9CA6E7FC54A47) ,DATA (0XEFF394DCFF8A948E) ,DATA (0X2FFD842C331BB74F) ,DATA (0X5FFB085866376E9F) ,DATA (0XBFF610B0CC6EDD3F) ,DATA (0X266469BCF5AFC5D9) ,DATA (0X4CC8D379EB5F8BB2) ,DATA (0X9991A6F3D6BF1765) ,DATA (0X1EB6BAFD91596B14) ,DATA (0X3D6D75FB22B2D628) ,DATA (0X7ADAEBF64565AC51) ,DATA (0XF5B5D7EC8ACB58A2) ,DATA (0X31245E628228AB53) ,DATA (0X6248BCC5045156A7) ,DATA (0XC491798A08A2AD4E) ,DATA (0X27504B8201BA22A9) ,DATA (0X4EA0970403744552) ,DATA (0X9D412E0806E88AA5) ,DATA (0X1F736F9B3494E887) ,DATA (0X3EE6DF366929D10F) ,DATA (0X7DCDBE6CD253A21E) ,DATA (0XFB9B7CD9A4A7443C) ,DATA (0X32524C2B8754A73F) ,DATA (0X64A498570EA94E7E) ,DATA (0XC94930AE1D529CFC) ,DATA (0X2841D689391085CC) ,DATA (0X5083AD1272210B98) ,DATA (0XA1075A24E4421730) ,DATA (0X2034ABA0FA739E3C) ,DATA (0X40695741F4E73C79) ,DATA (0X80D2AE83E9CE78F3) ,DATA (0X19C3BC80C85C7E97) ,DATA (0X3387790190B8FD2E) ,DATA (0X670EF2032171FA5C) ,DATA (0XCE1DE40642E3F4B9) ,DATA (0X29392D9ADA2D9758) ,DATA (0X52725B35B45B2EB0) ,DATA (0XA4E4B66B68B65D60) ,DATA (0X20FA8AE248247913) ,DATA (0X41F515C49048F226) ,DATA (0X83EA2B892091E44D) ,DATA (0X1A6208B50683940F) ,DATA (0X34C4116A0D07281F) ,DATA (0X698822D41A0E503E) ,DATA (0XD31045A8341CA07C) ,DATA (0X2A367454D738ECE5) ,DATA (0X546CE8A9AE71D9CB) ,DATA (0XA8D9D1535CE3B396) ,DATA (0X21C529DD78FA571E) ,DATA (0X438A53BAF1F4AE3C) ,DATA (0X8714A775E3E95C78) ,DATA (0X1B04217DFA61DF4B) ,DATA (0X360842FBF4C3BE96) ,DATA (0X6C1085F7E9877D2D) ,DATA (0XD8210BEFD30EFA5A) ,DATA (0X2B39CF2FF702FEDE) ,DATA (0X56739E5FEE05FDBD) ,DATA (0XACE73CBFDC0BFB7B) ,DATA (0X2294A5BFF8CF324B) ,DATA (0X45294B7FF19E6497) ,DATA (0X8A5296FFE33CC92F) ,DATA (0X1BAA1E332D728EA3) ,DATA (0X37543C665AE51D46) ,DATA (0X6EA878CCB5CA3A8C) ,DATA (0XDD50F1996B947518) ,DATA (0X2C4363851584176B) ,DATA (0X5886C70A2B082ED6) ,DATA (0XB10D8E1456105DAD) ,DATA (0X23691C6A779CDF89) ,DATA (0X46D238D4EF39BF12) ,DATA (0X8DA471A9DE737E24) ,DATA (0X1C5416BB92E3E607) ,DATA (0X38A82D7725C7CC0E) ,DATA (0X71505AEE4B8F981D) ,DATA (0XE2A0B5DC971F303A) ,DATA (0X2D535792849FD672) ,DATA (0X5AA6AF25093FACE4) ,DATA (0XB54D5E4A127F59C8) ,DATA (0X2442AC7536E64528) ,DATA (0X488558EA6DCC8A50) ,DATA (0X910AB1D4DB9914A0) ,DATA (0X1D022390F8B83753) ,DATA (0X3A044721F1706EA6) ,DATA (0X74088E43E2E0DD4C) ,DATA (0XE8111C87C5C1BA99) ,DATA (0X2E69D2818DF38BB8) ,DATA (0X5CD3A5031BE71770) ,DATA (0XB9A74A0637CE2EE1) ,DATA (0X25217534718FA2F9) ,DATA (0X4A42EA68E31F45F3) ,DATA (0X9485D4D1C63E8BE7) ,DATA (0X1DB45DC38E0C8261) ,DATA (0X3B68BB871C1904C3) ,DATA (0X76D1770E38320986) ,DATA (0XEDA2EE1C7064130C) ,DATA (0X2F86FC6C167A6A35) ,DATA (0X5F0DF8D82CF4D46B) ,DATA (0XBE1BF1B059E9A8D6) ,DATA (0X260596BCDEC854F7) ,DATA (0X4C0B2D79BD90A9EF) ,DATA (0X98165AF37B2153DE) ,DATA (0X1E6ADEFD7F06AA5F) ,DATA (0X3CD5BDFAFE0D54BF) ,DATA (0X79AB7BF5FC1AA97F) ,DATA (0XF356F7EBF83552FE) ,DATA (0X30AAFE6264D776FF) ,DATA (0X6155FCC4C9AEEDFF) ,DATA (0XC2ABF989935DDBFE) ,DATA (0X26EF31E850AC5F32) ,DATA (0X4DDE63D0A158BE65) ,DATA (0X9BBCC7A142B17CCB) ,DATA (0X1F25C186A6F04C28) ,DATA (0X3E4B830D4DE09851) ,DATA (0X7C97061A9BC130A2) ,DATA (0XF92E0C3537826145) ,DATA (0X31D602710B1A1374) ,DATA (0X63AC04E2163426E8) ,DATA (0XC75809C42C684DD1) ,DATA (0X27DE685A6F480F90) ,DATA (0X4FBCD0B4DE901F20) ,DATA (0X9F79A169BD203E41) ,DATA (0X1FE52048590672D9) ,DATA (0X3FCA4090B20CE5B3) ,DATA (0X7F9481216419CB67) ,DATA (0XFF290242C83396CE) ,DATA (0X330833A6F4D71E29) ,DATA (0X6610674DE9AE3C52) ,DATA (0XCC20CE9BD35C78A5) ,DATA (0X28D35C8590AC1821) ,DATA (0X51A6B90B21583042) ,DATA (0XA34D721642B06084) ,DATA (0X20A916D14089ACE7) ,DATA (0X41522DA2811359CE) ,DATA (0X82A45B450226B39C) ,DATA (0X1A20DF0DCD3AF0B8) ,DATA (0X3441BE1B9A75E171) ,DATA (0X68837C3734EBC2E3) ,DATA (0XD106F86E69D785C7) ,DATA (0X29CE31AFAEC4B45B) ,DATA (0X539C635F5D8968B6) ,DATA (0XA738C6BEBB12D16C) ,DATA (0X2171C159589D5D15) ,DATA (0X42E382B2B13ABA2B) ,DATA (0X85C7056562757456) ,DATA (0X1AC1677AAD4AB0DE) ,DATA (0X3582CEF55A9561BC) ,DATA (0X6B059DEAB52AC378) ,DATA (0XD60B3BD56A5586F1) ,DATA (0X2ACF0BF77BAAB496) ,DATA (0X559E17EEF755692D) ,DATA (0XAB3C2FDDEEAAD25A) ,DATA (0X223F3CC5FC889078) ,DATA (0X447E798BF91120F1) ,DATA (0X88FCF317F22241E2) ,DATA (0X1B65CA37FD3A0D2D) ,DATA (0X36CB946FFA741A5A) ,DATA (0X6D9728DFF4E834B5) ,DATA (0XDB2E51BFE9D0696A) ,DATA (0X2BD610599529AEAE) ,DATA (0X57AC20B32A535D5D) ,DATA (0XAF58416654A6BABB) ,DATA (0X2311A6AE10EE2558) ,DATA (0X46234D5C21DC4AB1) ,DATA (0X8C469AB843B89562) ,DATA (0X1C0E1EF1A724EAAD) ,DATA (0X381C3DE34E49D55A) ,DATA (0X70387BC69C93AAB5) ,DATA (0XE070F78D3927556A) ,DATA (0X2CE364B5D83B1115) ,DATA (0X59C6C96BB076222A) ,DATA (0XB38D92D760EC4455) ,DATA (0X23E91D5E4695A744) ,DATA (0X47D23ABC8D2B4E88) ,DATA (0X8FA475791A569D10) ,DATA (0X1CBA7DE5054485D0) ,DATA (0X3974FBCA0A890BA0) ,DATA (0X72E9F79415121740) ,DATA (0XE5D3EF282A242E81) ,DATA (0X2DF72FD4D53A6FB3) ,DATA (0X5BEE5FA9AA74DF67) ,DATA (0XB7DCBF5354E9BECE) ,DATA (0X24C5BFDD7761F2F6) ,DATA (0X498B7FBAEEC3E5EC) ,DATA (0X9316FF75DD87CBD8) ,DATA (0X1D6AFFE45F818F2B) ,DATA (0X3AD5FFC8BF031E56) ,DATA (0X75ABFF917E063CAC) ,DATA (0XEB57FF22FC0C7959) ,DATA (0X2F11996D659C1845) ,DATA (0X5E2332DACB38308A) ,DATA (0XBC4665B596706114) ,DATA (0X25A7ADF11E1679D0) ,DATA (0X4B4F5BE23C2CF3A1) ,DATA (0X969EB7C47859E743) ,DATA (0X1E1FBE5A7E786173) ,DATA (0X3C3F7CB4FCF0C2E7) ,DATA (0X787EF969F9E185CF) ,DATA (0XF0FDF2D3F3C30B9F) ,DATA (0X3032CA2A63F3CF1F) ,DATA (0X60659454C7E79E3F) ,DATA (0XC0CB28A98FCF3C7F) ,DATA (0X268F0821E98FD8E6) ,DATA (0X4D1E1043D31FB1CC) ,DATA (0X9A3C2087A63F6399) ,DATA (0X1ED8D34E547313EB) ,DATA (0X3DB1A69CA8E627D6) ,DATA (0X7B634D3951CC4FAD) ,DATA (0XF6C69A72A3989F5B) ,DATA (0X315AEBB0871E8645) ,DATA (0X62B5D7610E3D0C8B) ,DATA (0XC56BAEC21C7A1916) ,DATA (0X277BEFC06C186B6A) ,DATA (0X4EF7DF80D830D6D5) ,DATA (0X9DEFBF01B061ADAB) ,DATA (0X1F965966BCE055EF) ,DATA (0X3F2CB2CD79C0ABDE) ,DATA (0X7E59659AF38157BC) ,DATA (0XFCB2CB35E702AF78) ,DATA (0X328A28A46166EFE4) ,DATA (0X65145148C2CDDFC9) ,DATA (0XCA28A291859BBF93) ,DATA (0X286E86E9E7858CB7) ,DATA (0X50DD0DD3CF0B196E) ,DATA (0XA1BA1BA79E1632DC) ,DATA (0X20586BEE52D13D5F) ,DATA (0X40B0D7DCA5A27ABE) ,DATA (0X8161AFB94B44F57D) ,DATA (0X19E056584240FDE5) ,DATA (0X33C0ACB08481FBCB) ,DATA (0X678159610903F797) ,DATA (0XCF02B2C21207EF2E) ,DATA (0X2966F08D36CE6309) ,DATA (0X52CDE11A6D9CC612) ,DATA (0XA59BC234DB398C25) ,DATA (0X211F26D75F0B826D) ,DATA (0X423E4DAEBE1704DB) ,DATA (0X847C9B5D7C2E09B7) ,DATA (0X1A7F5245E5A2CEBE) ,DATA (0X34FEA48BCB459D7C) ,DATA (0X69FD4917968B3AF9) ,DATA (0XD3FA922F2D1675F2) ,DATA (0X2A65506FD5D14ACA) ,DATA (0X54CAA0DFABA29594) ,DATA (0XA99541BF57452B28) ,DATA (0X21EAA6BFDE4108A1) ,DATA (0X43D54D7FBC821143) ,DATA (0X87AA9AFF79042286) ,DATA (0X1B221EFFE500D3B4) ,DATA (0X36443DFFCA01A769) ,DATA (0X6C887BFF94034ED2) ,DATA (0XD910F7FF28069DA4) ,DATA (0X2B69CB33080152BA) ,DATA (0X56D396661002A574) ,DATA (0XADA72CCC20054AE9) ,DATA (0X22BB08F5A0010EFB) ,DATA (0X457611EB40021DF7) ,DATA (0X8AEC23D680043BEE) ,DATA (0X1BC8D3F7B3340BFC) ,DATA (0X3791A7EF666817F8) ,DATA (0X6F234FDECCD02FF1) ,DATA (0XDE469FBD99A05FE3) ,DATA (0X2C7486591EB9ACC7) ,DATA (0X58E90CB23D73598E) ,DATA (0XB1D219647AE6B31C) ,DATA (0X23906B7A7EFAF09F) ,DATA (0X4720D6F4FDF5E13E) ,DATA (0X8E41ADE9FBEBC27D) ,DATA (0X1C73892ECBFBF3B2) ,DATA (0X38E7125D97F7E765) ,DATA (0X71CE24BB2FEFCECA) ,DATA (0XE39C49765FDF9D94) ,DATA (0X2D85A84ADFF985EA) ,DATA (0X5B0B5095BFF30BD5) ,DATA (0XB616A12B7FE617AA) ,DATA (0X246AED08B32E04BB) ,DATA (0X48D5DA11665C0977) ,DATA (0X91ABB422CCB812EE) ,DATA (0X1D22573A28F19D62) ,DATA (0X3A44AE7451E33AC5) ,DATA (0X74895CE8A3C6758B) ,DATA (0XE912B9D1478CEB17) ,DATA (0X2E9D585D0E4F6237) ,DATA (0X5D3AB0BA1C9EC46F) ,DATA (0XBA756174393D88DF) ,DATA (0X254AAD173EA5E82C) ,DATA (0X4A955A2E7D4BD059) ,DATA (0X952AB45CFA97A0B2) ,DATA (0X1DD55745CBB7ECF0) ,DATA (0X3BAAAE8B976FD9E1) ,DATA (0X77555D172EDFB3C2) ,DATA (0XEEAABA2E5DBF6784) ,DATA (0X2FBBBED612BFE180) ,DATA (0X5F777DAC257FC301) ,DATA (0XBEEEFB584AFF8603) ,DATA (0X262FCBDE75664E00) ,DATA (0X4C5F97BCEACC9C01) ,DATA (0X98BF2F79D5993802) ,DATA (0X1E8CA3185DEB719A) ,DATA (0X3D194630BBD6E334) ,DATA (0X7A328C6177ADC668) ,DATA (0XF46518C2EF5B8CD1) ,DATA (0X30E104F3C978B5C3) ,DATA (0X61C209E792F16B86) ,DATA (0XC38413CF25E2D70D) ,DATA (0X271A6A5CA12D5E35) ,DATA (0X4E34D4B9425ABC6B) ,DATA (0X9C69A97284B578D7) ,DATA (0X1F485516E7577E91) ,DATA (0X3E90AA2DCEAEFD23) ,DATA (0X7D21545B9D5DFA46) ,DATA (0XFA42A8B73ABBF48C) ,DATA (0X320D54F17225974F) ,DATA (0X641AA9E2E44B2E9E) ,DATA (0XC83553C5C8965D3D) ,DATA (0X280AAA5AC1B7AC3F) ,DATA (0X501554B5836F587E) ,DATA (0XA02AA96B06DEB0FD) ,DATA (0X200888489AF95699) ,DATA (0X4011109135F2AD32) ,DATA (0X802221226BE55A64) ,DATA (0X19A06D06E2611214) ,DATA (0X3340DA0DC4C22428) ,DATA (0X6681B41B89844850) ,DATA (0XCD036837130890A1) ,DATA (0X2900AE716A34E9B9) ,DATA (0X52015CE2D469D373) ,DATA (0XA402B9C5A8D3A6E7) ,DATA (0X20CD585ABB5D87C7) ,DATA (0X419AB0B576BB0F8F) ,DATA (0X8335616AED761F1F) ,DATA (0X1A3DE04895E46C9F) ,DATA (0X347BC0912BC8D93F) ,DATA (0X68F781225791B27F) ,DATA (0XD1EF0244AF2364FF) ,DATA (0X29FC9A0DBCA0ADCC) ,DATA (0X53F9341B79415B99) ,DATA (0XA7F26836F282B732) ,DATA (0X2196E1A496E6F170) ,DATA (0X432DC3492DCDE2E1) ,DATA (0X865B86925B9BC5C2) ,DATA (0X1ADF1AEA12525AC0) ,DATA (0X35BE35D424A4B580) ,DATA (0X6B7C6BA849496B01) ,DATA (0XD6F8D7509292D603) ,DATA (0X2AFE917683B6F79A) ,DATA (0X55FD22ED076DEF34) ,DATA (0XABFA45DA0EDBDE69) ,DATA (0X2265412B9C925FAE) ,DATA (0X44CA82573924BF5D) ,DATA (0X899504AE72497EBA) ,DATA (0X1B843422E3A84C8B) ,DATA (0X37086845C7509917) ,DATA (0X6E10D08B8EA1322E) ,DATA (0XDC21A1171D42645D) ,DATA (0X2C06B9D16C407A79) ,DATA (0X580D73A2D880F4F2) ,DATA (0XB01AE745B101E9E4) ,DATA (0X233894A789CD2EC7) ,DATA (0X4671294F139A5D8E) ,DATA (0X8CE2529E2734BB1D) ,DATA (0X1C2D43B93B0A8BD2) ,DATA (0X385A8772761517A5) ,DATA (0X70B50EE4EC2A2F4A) ,DATA (0XE16A1DC9D8545E94) ,DATA (0X2D1539285E77461D) ,DATA (0X5A2A7250BCEE8C3B) ,DATA (0XB454E4A179DD1877) ,DATA (0X2410FA86B1F904E4) ,DATA (0X4821F50D63F209C9) ,DATA (0X9043EA1AC7E41392) ,DATA (0X1CDA62055B2D9D83) ,DATA (0X39B4C40AB65B3B07) ,DATA (0X736988156CB6760E) ,DATA (0XE6D3102AD96CEC1D) ,DATA (0X2E2A366EF848FC05) ,DATA (0X5C546CDDF091F80B) ,DATA (0XB8A8D9BBE123F017) ,DATA (0X24EE91F2603A6337) ,DATA (0X49DD23E4C074C66F) ,DATA (0X93BA47C980E98CDF) ,DATA (0X1D8BA7F519C84F5F) ,DATA (0X3B174FEA33909EBF) ,DATA (0X762E9FD467213D7F) ,DATA (0XEC5D3FA8CE427AFF) ,DATA (0X2F45D98829407EFF) ,DATA (0X5E8BB3105280FDFF) ,DATA (0XBD176620A501FBFF) ,DATA (0X25D17AD3543398CC) ,DATA (0X4BA2F5A6A8673199) ,DATA (0X9745EB4D50CE6332) ,DATA (0X1E412F0F768FAD70) ,DATA (0X3C825E1EED1F5AE1) ,DATA (0X7904BC3DDA3EB5C2) ,DATA (0XF209787BB47D6B84) ,DATA (0X30684B4BF0E5E24D) ,DATA (0X60D09697E1CBC49B) ,DATA (0XC1A12D2FC3978937) ,DATA (0X26B9D5D65A5181D7) ,DATA (0X4D73ABACB4A303AF) ,DATA (0X9AE757596946075F) ,DATA (0X1EFB1178484134AC) ,DATA (0X3DF622F090826959) ,DATA (0X7BEC45E12104D2B2) ,DATA (0XF7D88BC24209A565) ,DATA (0X3191B58D40685447) ,DATA (0X63236B1A80D0A88E) ,DATA (0XC646D63501A1511D) ,DATA (0X27A7C4710053769F) ,DATA (0X4F4F88E200A6ED3F) ,DATA (0X9E9F11C4014DDA7E) ,DATA (0X1FB969F40042C54C) ,DATA (0X3F72D3E800858A98) ,DATA (0X7EE5A7D0010B1531) ,DATA (0XFDCB4FA002162A63) ,DATA (0X32C24320006AD547) ,DATA (0X6584864000D5AA8E) ,DATA (0XCB090C8001AB551C) ,DATA (0X289B68E666BBDDD2) ,DATA (0X5136D1CCCD77BBA4) ,DATA (0XA26DA3999AEF7749) ,DATA (0X207C53EB856317DB) ,DATA (0X40F8A7D70AC62FB7) ,DATA (0X81F14FAE158C5F6E) ,DATA (0X19FD0FEF9DE8DFE2) ,DATA (0X33FA1FDF3BD1BFC5) ,DATA (0X67F43FBE77A37F8B) ,DATA (0XCFE87F7CEF46FF16) ,DATA (0X2994E64C2FDAFFD1) ,DATA (0X5329CC985FB5FFA2) ,DATA (0XA6539930BF6BFF45) ,DATA (0X2143EB702648CCA7) ,DATA (0X4287D6E04C91994F) ,DATA (0X850FADC09923329E) ,DATA (0X1A9CBC59B83A3D52) ,DATA (0X353978B370747AA5) ,DATA (0X6A72F166E0E8F54B) ,DATA (0XD4E5E2CDC1D1EA96) ,DATA (0X2A94608F8D29FBB7) ,DATA (0X5528C11F1A53F76F) ,DATA (0XAA51823E34A7EEDE) ,DATA (0X22104D3FA421962C) ,DATA (0X44209A7F48432C59) ,DATA (0X884134FE908658B2) ,DATA (0X1B403DCC834E11BD) ,DATA (0X36807B99069C237A) ,DATA (0X6D00F7320D3846F4) ,DATA (0XDA01EE641A708DE9) ,DATA (0X2B99FC7A6BB01C61) ,DATA (0X5733F8F4D76038C3) ,DATA (0XAE67F1E9AEC07187) ,DATA (0X22E196C856267D1B) ,DATA (0X45C32D90AC4CFA36) ,DATA (0X8B865B215899F46C) ,DATA (0X1BE7ABD3781ECA7C) ,DATA (0X37CF57A6F03D94F8) ,DATA (0X6F9EAF4DE07B29F0) ,DATA (0XDF3D5E9BC0F653E1) ,DATA (0X2CA5DFB8C03143F9) ,DATA (0X594BBF71806287F3) ,DATA (0XB2977EE300C50FE7) ,DATA (0X23B7E62D668DCFFB) ,DATA (0X476FCC5ACD1B9FF6) ,DATA (0X8EDF98B59A373FEC) ,DATA (0X1C931E8AB871732F) ,DATA (0X39263D1570E2E65E) ,DATA (0X724C7A2AE1C5CCBD) ,DATA (0XE498F455C38B997A) ,DATA (0X2DB830DDF3E8B84B) ,DATA (0X5B7061BBE7D17097) ,DATA (0XB6E0C377CFA2E12E) ,DATA (0X24935A4B2986F9D6) ,DATA (0X4926B496530DF3AC) ,DATA (0X924D692CA61BE758) ,DATA (0X1D42AEA2879F2E44) ,DATA (0X3A855D450F3E5C89) ,DATA (0X750ABA8A1E7CB913) ,DATA (0XEA1575143CF97226) ,DATA (0X2ED1176A72984A07) ,DATA (0X5DA22ED4E530940F) ,DATA (0XBB445DA9CA61281F) ,DATA (0X257412BB8EE03B39) ,DATA (0X4AE825771DC07672) ,DATA (0X95D04AEE3B80ECE5) ,DATA (0X1DF67562D8B36294) ,DATA (0X3BECEAC5B166C528) ,DATA (0X77D9D58B62CD8A51) ,DATA (0XEFB3AB16C59B14A2) ,DATA (0X2FF0BBD15AB89DBA) ,DATA (0X5FE177A2B5713B74) ,DATA (0XBFC2EF456AE276E8) ,DATA (0X265A2FDAAEFA17C8) ,DATA (0X4CB45FB55DF42F90) ,DATA (0X9968BF6ABBE85F20) ,DATA (0X1EAE8CAEF261ACA0) ,DATA (0X3D5D195DE4C35940) ,DATA (0X7ABA32BBC986B280) ,DATA (0XF5746577930D6500) ,DATA (0X3117477E509C4766) ,DATA (0X622E8EFCA1388ECD) ,DATA (0XC45D1DF942711D9A) ,DATA (0X2745D2CB73B0391E) ,DATA (0X4E8BA596E760723D) ,DATA (0X9D174B2DCEC0E47B) ,DATA (0X1F6B0F092959C74B) ,DATA (0X3ED61E1252B38E97) ,DATA (0X7DAC3C24A5671D2F) ,DATA (0XFB5878494ACE3A5F) ,DATA (0X3244E4DB755C7213) ,DATA (0X6489C9B6EAB8E426) ,DATA (0XC913936DD571C84C) ,DATA (0X28371D7C5DE38E75) ,DATA (0X506E3AF8BBC71CEB) ,DATA (0XA0DC75F1778E39D6) ,DATA (0X202C1796B182D85E) ,DATA (0X40582F2D6305B0BC) ,DATA (0X80B05E5AC60B6178) ,DATA (0X19BCDFABC13579E4) ,DATA (0X3379BF57826AF3C9) ,DATA (0X66F37EAF04D5E793) ,DATA (0XCDE6FD5E09ABCF26) ,DATA (0X292E32AC68558FD4) ,DATA (0X525C6558D0AB1FA9) ,DATA (0XA4B8CAB1A1563F52) ,DATA (0X20F1C22386AAD976) ,DATA (0X41E384470D55B2ED) ,DATA (0X83C7088E1AAB65DB) ,DATA (0X1A5B01B605557AC5) ,DATA (0X34B6036C0AAAF58A) ,DATA (0X696C06D81555EB15) ,DATA (0XD2D80DB02AABD62B) ,DATA (0X2A2B35F00888C46F) ,DATA (0X54566BE0111188DE) ,DATA (0XA8ACD7C0222311BC) ,DATA (0X21BC2B266D3A36BF) ,DATA (0X4378564CDA746D7E) ,DATA (0X86F0AC99B4E8DAFD) ,DATA (0X1AFCEF51F0FB5EFF) ,DATA (0X35F9DEA3E1F6BDFE) ,DATA (0X6BF3BD47C3ED7BFD) ,DATA (0XD7E77A8F87DAF7FB) ,DATA (0X2B2E4BB64E5EFE65) ,DATA (0X565C976C9CBDFCCB) ,DATA (0XACB92ED9397BF996) ,DATA (0X228B6FC50B7F31EA) ,DATA (0X4516DF8A16FE63D5) ,DATA (0X8A2DBF142DFCC7AB) ,DATA (0X1BA2BFD0D5FF5B22) ,DATA (0X37457FA1ABFEB644) ,DATA (0X6E8AFF4357FD6C89) ,DATA (0XDD15FE86AFFAD912) ,DATA (0X2C37994E23322B6A) ,DATA (0X586F329C466456D4) ,DATA (0XB0DE65388CC8ADA8) ,DATA (0X235FADD81C2822BB) ,DATA (0X46BF5BB038504576) ,DATA (0X8D7EB76070A08AEC) ,DATA (0X1C4C8B1349B9B562) ,DATA (0X3899162693736AC5) ,DATA (0X71322C4D26E6D58A) ,DATA (0XE264589A4DCDAB14) ,DATA (0X2D4744EBA9292237) ,DATA (0X5A8E89D75252446E) ,DATA (0XB51D13AEA4A488DD) ,DATA (0X243903EFBA874E92) ,DATA (0X487207DF750E9D25) ,DATA (0X90E40FBEEA1D3A4A) ,DATA (0X1CFA698C95390BA8) ,DATA (0X39F4D3192A721751) ,DATA (0X73E9A63254E42EA2) ,DATA (0XE7D34C64A9C85D44) ,DATA (0X2E5D75ADBB8E790D) ,DATA (0X5CBAEB5B771CF21B) ,DATA (0XB975D6B6EE39E436) ,DATA (0X25179157C93EC73E) ,DATA (0X4A2F22AF927D8E7C) ,DATA (0X945E455F24FB1CF8) ,DATA (0X1DAC74463A989F64) ,DATA (0X3B58E88C75313EC9) ,DATA (0X76B1D118EA627D93) ,DATA (0XED63A231D4C4FB27) ,DATA (0X2F7A53A390F4323B) ,DATA (0X5EF4A74721E86476) ,DATA (0XBDE94E8E43D0C8EC) ,DATA (0X25FB761C73F68E95) ,DATA (0X4BF6EC38E7ED1D2B) ,DATA (0X97EDD871CFDA3A56) ,DATA (0X1E62C4E38FF87211) ,DATA (0X3CC589C71FF0E422) ,DATA (0X798B138E3FE1C845) ,DATA (0XF316271C7FC3908A) ,DATA (0X309E07D27FF3E9B5) ,DATA (0X613C0FA4FFE7D36A) ,DATA (0XC2781F49FFCFA6D5) ,DATA (0X26E4D30ECCC3215D) ,DATA (0X4DC9A61D998642BB) ,DATA (0X9B934C3B330C8577) ,DATA (0X1F1D75A5709C1AB1) ,DATA (0X3E3AEB4AE1383562) ,DATA (0X7C75D695C2706AC5) ,DATA (0XF8EBAD2B84E0D58B) ,DATA (0X31C8BC3BE7602AB5) ,DATA (0X63917877CEC0556B) ,DATA (0XC722F0EF9D80AAD6) ,DATA (0X27D3C9C985E68891) ,DATA (0X4FA793930BCD1122) ,DATA (0X9F4F2726179A2245) ,DATA (0X1FDCA16E04B86D41) ,DATA (0X3FB942DC0970DA82) ,DATA (0X7F7285B812E1B504) ,DATA (0XFEE50B7025C36A08) ,DATA (0X32FA9BE33AC0AECE) ,DATA (0X65F537C675815D9C) ,DATA (0XCBEA6F8CEB02BB39) ,DATA (0X28C87CB5C89A2571) ,DATA (0X5190F96B91344AE3) ,DATA (0XA321F2D7226895C7) ,DATA (0X20A063C4A07B5127) ,DATA (0X4140C78940F6A24F) ,DATA (0X82818F1281ED449F) ,DATA (0X1A19E96A19FC40EC) ,DATA (0X3433D2D433F881D9) ,DATA (0X6867A5A867F103B2) ,DATA (0XD0CF4B50CFE20765) ,DATA (0X29C30F1029939B14) ,DATA (0X53861E2053273628) ,DATA (0XA70C3C40A64E6C51) ,DATA (0X2168D8D9BADC7C10) ,DATA (0X42D1B1B375B8F820) ,DATA (0X85A36366EB71F041) ,DATA (0X1ABA4714957D300D) ,DATA (0X35748E292AFA601A) ,DATA (0X6AE91C5255F4C034) ,DATA (0XD5D238A4ABE98068) ,DATA (0X2AC3A4EDBBFB8014) ,DATA (0X558749DB77F70029) ,DATA (0XAB0E93B6EFEE0053) ,DATA (0X22361D8AFCC93343) ,DATA (0X446C3B15F9926687) ,DATA (0X88D8762BF324CD0F) ,DATA (0X1B5E7E08CA3A8F69) ,DATA (0X36BCFC1194751ED3) ,DATA (0X6D79F82328EA3DA6) ,DATA (0XDAF3F04651D47B4C) ,DATA (0X2BCA63414390E575) ,DATA (0X5794C6828721CAEB) ,DATA (0XAF298D050E4395D6) ,DATA (0X23084F676940B791) ,DATA (0X46109ECED2816F22) ,DATA (0X8C213D9DA502DE45) ,DATA (0X1C06A5EC5433C60D) ,DATA (0X380D4BD8A8678C1B) ,DATA (0X701A97B150CF1837) ,DATA (0XE0352F62A19E306E) ,DATA (0X2CD76FE086B93CE2) ,DATA (0X59AEDFC10D7279C5) ,DATA (0XB35DBF821AE4F38B) ,DATA (0X23DF8CB39EFA971B) ,DATA (0X47BF19673DF52E37) ,DATA (0X8F7E32CE7BEA5C6F) ,DATA (0X1CB2D6F618C878E3) ,DATA (0X3965ADEC3190F1C6) ,DATA (0X72CB5BD86321E38C) ,DATA (0XE596B7B0C643C719) ,DATA (0X2DEAF189C140C16B) ,DATA (0X5BD5E313828182D6) ,DATA (0XB7ABC627050305AD) ,DATA (0X24BBF46E3433CDEF) ,DATA (0X4977E8DC68679BDF) ,DATA (0X92EFD1B8D0CF37BE) ,DATA (0X1D6329F1C35CA4BF) ,DATA (0X3AC653E386B9497F) ,DATA (0X758CA7C70D7292FE) ,DATA (0XEB194F8E1AE525FD) ,DATA (0X2F050FE938943ACC) ,DATA (0X5E0A1FD271287598) ,DATA (0XBC143FA4E250EB31) ,DATA (0X259DA6542D43623D) ,DATA (0X4B3B4CA85A86C47A) ,DATA (0X96769950B50D88F4) ,DATA (0X1E17B84357691B64) ,DATA (0X3C2F7086AED236C8) ,DATA (0X785EE10D5DA46D90) ,DATA (0XF0BDC21ABB48DB20) ,DATA (0X3025F39EF241C56C) ,DATA (0X604BE73DE4838AD9) ,DATA (0XC097CE7BC90715B3) ,DATA (0X2684C2E58E9B0457) ,DATA (0X4D0985CB1D3608AE) ,DATA (0X9A130B963A6C115C) ,DATA (0X1ED09BEAD87C0378) ,DATA (0X3DA137D5B0F806F1) ,DATA (0X7B426FAB61F00DE3) ,DATA (0XF684DF56C3E01BC6) ,DATA (0X314DC6448D9338C1) ,DATA (0X629B8C891B267182) ,DATA (0XC5371912364CE305) ,DATA (0X27716B6A0ADC2D67) ,DATA (0X4EE2D6D415B85ACE) ,DATA (0X9DC5ADA82B70B59D) ,DATA (0X1F8DEF8808B02452) ,DATA (0X3F1BDF10116048A5) ,DATA (0X7E37BE2022C0914B) ,DATA (0XFC6F7C4045812296) ,DATA (0X327CB2734119D3B7) ,DATA (0X64F964E68233A76F) ,DATA (0XC9F2C9CD04674EDE) ,DATA (0X2863C1F5CDAE42F9) ,DATA (0X50C783EB9B5C85F2) ,DATA (0XA18F07D736B90BE5) ,DATA (0X204FCE5E3E250261) ,DATA (0X409F9CBC7C4A04C2) ,DATA (0X813F3978F8940984) ,DATA (0X19D971E4FE8401E7) ,DATA (0X33B2E3C9FD0803CE) ,DATA (0X6765C793FA10079D) ,DATA (0X14ADF4B7320334B9) ,DATA (0X0422CA8B0A00A425) ,DATA (0X00D3C21BCECCEDA1) ,DATA (0X002A5A058FC295ED) ,DATA (0X000878678326EAC9) ,DATA (0X0001B1AE4D6E2EF5) ,DATA (0X000056BC75E2D631) ,DATA (0X00001158E460913D) ,DATA (0X000003782DACE9D9) ,DATA (0X000000B1A2BC2EC5) ,DATA (0X0000002386F26FC1) ,DATA (0X000000071AFD498D) ,DATA (0X000000016BCC41E9) ,DATA (0X0000000048C27395) ,DATA (0X000000000E8D4A51) ,DATA (0X0000000002E90EDD) ,DATA (0X00000000009502F9) ,DATA (0X00000000001DCD65) ,DATA (0X000000000005F5E1) ,DATA (0X000000000001312D) ,DATA (0X0000000000003D09) ,DATA (0X0000000000000C35) ,DATA (0X0000000000000271) ,DATA (0X000000000000007D) ,DATA (0X0000000000000019) ,DATA (0X0000000000000005) ,DATA (0X0000000000000001) ,DATA (0X0000000000000002) ,DATA (0X0000000000000004) ,DATA (0X0000000000000008) ,DATA (0X0000000000000010) ,DATA (0X0000000000000020) ,DATA (0X0000000000000040) ,DATA (0X0000000000000080) ,DATA (0X0000000000000100) ,DATA (0X0000000000000200) ,DATA (0X0000000000000400) ,DATA (0X0000000000000800) ,DATA (0X0000000000001000) ,DATA (0X0000000000002000) ,DATA (0X0000000000004000) ,DATA (0X0000000000008000) ,DATA (0X0000000000010000) ,DATA (0X0000000000020000) ,DATA (0X0000000000040000) ,DATA (0X0000000000080000) ,DATA (0X0000000000100000) ,DATA (0X0000000000200000) ,DATA (0X0000000000400000) ,DATA (0X0000000000800000) ,DATA (0X0000000001000000) ,DATA (0X0000000002000000) ,DATA (0X0000000004000000) ,DATA (0X0000000008000000) ,DATA (0X0000000010000000) ,DATA (0X0000000020000000) ,DATA (0X0000000040000000) ,DATA (0X0000000080000000) ,DATA (0X0000000100000000) ,DATA (0X0000000200000000) ,DATA (0X0000000400000000) ,DATA (0X0000000800000000) ,DATA (0X0000001000000000) ,DATA (0X0000002000000000) ,DATA (0X0000004000000000) ,DATA (0X0000008000000000) ,DATA (0X0000010000000000) ,DATA (0X0000020000000000) ,DATA (0X0000040000000000) ,DATA (0X0000080000000000) ,DATA (0X0000100000000000) ,DATA (0X0000200000000000) ,DATA (0X0000400000000000) ,DATA (0X0000800000000000) ,DATA (0X0001000000000000) ,DATA (0X0002000000000000) ,DATA (0X0004000000000000) ,DATA (0X0008000000000000) ,DATA (0X0010000000000000) ,DATA (0X0020000000000000) ,DATA (0X0040000000000000) ,DATA (0X0080000000000000) ,DATA (0X0100000000000000) ,DATA (0X0200000000000000) ,DATA (0X0400000000000000) ,DATA (0X0800000000000000) ,DATA (0X1000000000000000) ,DATA (0X2000000000000000) ,DATA (0X4000000000000000) ,DATA (0X8000000000000000) ,DATA (0X1999999999999999) ,DATA (0X3333333333333333) ,DATA (0X6666666666666666) ,DATA (0XCCCCCCCCCCCCCCCC) ,DATA (0X28F5C28F5C28F5C2) ,DATA (0X51EB851EB851EB85) ,DATA (0XA3D70A3D70A3D70A) ,DATA (0X20C49BA5E353F7CE) ,DATA (0X4189374BC6A7EF9D) ,DATA (0X83126E978D4FDF3B) ,DATA (0X1A36E2EB1C432CA5) ,DATA (0X346DC5D63886594A) ,DATA (0X68DB8BAC710CB295) ,DATA (0XD1B71758E219652B) ,DATA (0X29F16B11C6D1E108) ,DATA (0X53E2D6238DA3C211) ,DATA (0XA7C5AC471B478423) ,DATA (0X218DEF416BDB1A6D) ,DATA (0X431BDE82D7B634DA) ,DATA (0X8637BD05AF6C69B5) ,DATA (0X1AD7F29ABCAF4857) ,DATA (0X35AFE535795E90AF) ,DATA (0X6B5FCA6AF2BD215E) ,DATA (0XD6BF94D5E57A42BC) ,DATA (0X2AF31DC4611873BF) ,DATA (0X55E63B88C230E77E) ,DATA (0XABCC77118461CEFC) ,DATA (0X225C17D04DAD2965) ,DATA (0X44B82FA09B5A52CB) ,DATA (0X89705F4136B4A597) ,DATA (0X1B7CDFD9D7BDBAB7) ,DATA (0X36F9BFB3AF7B756F) ,DATA (0X6DF37F675EF6EADF) ,DATA (0XDBE6FECEBDEDD5BE) ,DATA (0X2BFAFFC2F2C92ABF) ,DATA (0X57F5FF85E592557F) ,DATA (0XAFEBFF0BCB24AAFE) ,DATA (0X232F33025BD42232) ,DATA (0X465E6604B7A84465) ,DATA (0X8CBCCC096F5088CB) ,DATA (0X1C25C268497681C2) ,DATA (0X384B84D092ED0384) ,DATA (0X709709A125DA0709) ,DATA (0XE12E13424BB40E13) ,DATA (0X2D09370D42573603) ,DATA (0X5A126E1A84AE6C07) ,DATA (0XB424DC35095CD80F) ,DATA (0X24075F3DCEAC2B36) ,DATA (0X480EBE7B9D58566C) ,DATA (0X901D7CF73AB0ACD9) ,DATA (0X1CD2B297D889BC2B) ,DATA (0X39A5652FB1137856) ,DATA (0X734ACA5F6226F0AD) ,DATA (0XE69594BEC44DE15B) ,DATA (0X2E1DEA8C8DA92D12) ,DATA (0X5C3BD5191B525A24) ,DATA (0XB877AA3236A4B449) ,DATA (0X24E4BBA3A4875741) ,DATA (0X49C97747490EAE83) ,DATA (0X9392EE8E921D5D07) ,DATA (0X1D83C94FB6D2AC34) ,DATA (0X3B07929F6DA55869) ,DATA (0X760F253EDB4AB0D2) ,DATA (0XEC1E4A7DB69561A5) ,DATA (0X2F394219248446BA) ,DATA (0X5E72843249088D75) ,DATA (0XBCE5086492111AEA) ,DATA (0X25C768141D369EFB) ,DATA (0X4B8ED0283A6D3DF7) ,DATA (0X971DA05074DA7BEE) ,DATA (0X1E392010175EE596) ,DATA (0X3C7240202EBDCB2C) ,DATA (0X78E480405D7B9658) ,DATA (0XF1C90080BAF72CB1) ,DATA (0X305B66802564A289) ,DATA (0X60B6CD004AC94513) ,DATA (0XC16D9A0095928A27) ,DATA (0X26AF8533511D4ED4) ,DATA (0X4D5F0A66A23A9DA9) ,DATA (0X9ABE14CD44753B52) ,DATA (0X1EF2D0F5DA7DD8AA) ,DATA (0X3DE5A1EBB4FBB154) ,DATA (0X7BCB43D769F762A8) ,DATA (0XF79687AED3EEC551) ,DATA (0X318481895D962776) ,DATA (0X63090312BB2C4EED) ,DATA (0XC612062576589DDA) ,DATA (0X279D346DE4781F92) ,DATA (0X4F3A68DBC8F03F24) ,DATA (0X9E74D1B791E07E48) ,DATA (0X1FB0F6BE50601941) ,DATA (0X3F61ED7CA0C03283) ,DATA (0X7EC3DAF941806506) ,DATA (0XFD87B5F28300CA0D) ,DATA (0X32B4BDFD4D668ECF) ,DATA (0X65697BFA9ACD1D9F) ,DATA (0XCAD2F7F5359A3B3E) ,DATA (0X289097FDD7853F0C) ,DATA (0X51212FFBAF0A7E18) ,DATA (0XA2425FF75E14FC31) ,DATA (0X2073ACCB12D0FF3D) ,DATA (0X40E7599625A1FE7A) ,DATA (0X81CEB32C4B43FCF4) ,DATA (0X19F623D5A8A73297) ,DATA (0X33EC47AB514E652E) ,DATA (0X67D88F56A29CCA5D) ,DATA (0XCFB11EAD453994BA) ,DATA (0X2989D2EF743EB758) ,DATA (0X5313A5DEE87D6EB0) ,DATA (0XA6274BBDD0FADD61) ,DATA (0X213B0F25F69892AD) ,DATA (0X42761E4BED31255A) ,DATA (0X84EC3C97DA624AB4) ,DATA (0X1A95A5B7F87A0EF0) ,DATA (0X352B4B6FF0F41DE1) ,DATA (0X6A5696DFE1E83BC3) ,DATA (0XD4AD2DBFC3D07787) ,DATA (0X2A8909265A5CE4B4) ,DATA (0X5512124CB4B9C969) ,DATA (0XAA242499697392D2) ,DATA (0X22073A8515171D5D) ,DATA (0X440E750A2A2E3ABA) ,DATA (0X881CEA14545C7575) ,DATA (0X1B38FB9DAA78E44A) ,DATA (0X3671F73B54F1C895) ,DATA (0X6CE3EE76A9E3912A) ,DATA (0XD9C7DCED53C72255) ,DATA (0X2B8E5F62AA5B06DD) ,DATA (0X571CBEC554B60DBB) ,DATA (0XAE397D8AA96C1B77) ,DATA (0X22D84C4EEEAF38B1) ,DATA (0X45B0989DDD5E7163) ,DATA (0X8B61313BBABCE2C6) ,DATA (0X1BE03D0BF225C6F4) ,DATA (0X37C07A17E44B8DE8) ,DATA (0X6F80F42FC8971BD1) ,DATA (0XDF01E85F912E37A3) ,DATA (0X2C99FB46503C7187) ,DATA (0X5933F68CA078E30E) ,DATA (0XB267ED1940F1C61C) ,DATA (0X23AE629EA696C138) ,DATA (0X475CC53D4D2D8271) ,DATA (0X8EB98A7A9A5B04E3) ,DATA (0X1C8B821885456760) ,DATA (0X391704310A8ACEC1) ,DATA (0X722E086215159D82) ,DATA (0XE45C10C42A2B3B05) ,DATA (0X2DAC035A6ED57234) ,DATA (0X5B5806B4DDAAE468) ,DATA (0XB6B00D69BB55C8D1) ,DATA (0X24899C4858AAC1C3) ,DATA (0X49133890B1558386) ,DATA (0X9226712162AB070D) ,DATA (0X1D3AE36D13BBCE35) ,DATA (0X3A75C6DA27779C6B) ,DATA (0X74EB8DB44EEF38D7) ,DATA (0XE9D71B689DDE71AF) ,DATA (0X2EC49F14EC5FB056) ,DATA (0X5D893E29D8BF60AC) ,DATA (0XBB127C53B17EC159) ,DATA (0X256A18DD89E626AB) ,DATA (0X4AD431BB13CC4D56) ,DATA (0X95A8637627989AAD) ,DATA (0X1DEE7A4AD4B81EEF) ,DATA (0X3BDCF495A9703DDF) ,DATA (0X77B9E92B52E07BBE) ,DATA (0XEF73D256A5C0F77C) ,DATA (0X2FE3F6DE212697E5) ,DATA (0X5FC7EDBC424D2FCB) ,DATA (0XBF8FDB78849A5F96) ,DATA (0X264FF8B1B41EDFEA) ,DATA (0X4C9FF163683DBFD5) ,DATA (0X993FE2C6D07B7FAB) ,DATA (0X1EA6608E29B24CBB) ,DATA (0X3D4CC11C53649977) ,DATA (0X7A998238A6C932EF) ,DATA (0XF53304714D9265DF) ,DATA (0X310A3416A91D4793) ,DATA (0X6214682D523A8F26) ,DATA (0XC428D05AA4751E4C) ,DATA (0X273B5CDEEDB1060F) ,DATA (0X4E76B9BDDB620C1E) ,DATA (0X9CED737BB6C4183D) ,DATA (0X1F62B0B257C0D1A5) ,DATA (0X3EC56164AF81A34B) ,DATA (0X7D8AC2C95F034697) ,DATA (0XFB158592BE068D2E) ,DATA (0X3237811D593482A2) ,DATA (0X646F023AB2690545) ,DATA (0XC8DE047564D20A8B) ,DATA (0X282C674AADC39BB5) ,DATA (0X5058CE955B87376B) ,DATA (0XA0B19D2AB70E6ED6) ,DATA (0X202385D557CFAFC4) ,DATA (0X40470BAAAF9F5F88) ,DATA (0X808E17555F3EBF11) ,DATA (0X19B604AAACA62636) ,DATA (0X336C0955594C4C6D) ,DATA (0X66D812AAB29898DB) ,DATA (0XCDB02555653131B6) ,DATA (0X29233AAAADD6A38A) ,DATA (0X524675555BAD4715) ,DATA (0XA48CEAAAB75A8E2B) ,DATA (0X20E8FBBBBE454FA2) ,DATA (0X41D1F7777C8A9F44) ,DATA (0X83A3EEEEF9153E89) ,DATA (0X1A53FC9631D10C81) ,DATA (0X34A7F92C63A21903) ,DATA (0X694FF258C7443207) ,DATA (0XD29FE4B18E88640E) ,DATA (0X2A1FFA89E94E7A69) ,DATA (0X543FF513D29CF4D2) ,DATA (0XA87FEA27A539E9A5) ,DATA (0X21B32ED4BAA52EBA) ,DATA (0X43665DA9754A5D75) ,DATA (0X86CCBB52EA94BAEA) ,DATA (0X1AF5BF109550F22E) ,DATA (0X35EB7E212AA1E45D) ,DATA (0X6BD6FC425543C8BB) ,DATA (0XD7ADF884AA879177) ,DATA (0X2B22CB4DBBB4B6B1) ,DATA (0X5645969B77696D62) ,DATA (0XAC8B2D36EED2DAC5) ,DATA (0X22823C3E2FC3C55A) ,DATA (0X4504787C5F878AB5) ,DATA (0X8A08F0F8BF0F156B) ,DATA (0X1B9B6364F3030448) ,DATA (0X3736C6C9E6060891) ,DATA (0X6E6D8D93CC0C1122) ,DATA (0XDCDB1B2798182244) ,DATA (0X2C2BD23B1E6B3A0D) ,DATA (0X5857A4763CD6741B) ,DATA (0XB0AF48EC79ACE837) ,DATA (0X235641C8E52294D7) ,DATA (0X46AC8391CA4529AF) ,DATA (0X8D590723948A535F) ,DATA (0X1C45016D841BAA46) ,DATA (0X388A02DB0837548C) ,DATA (0X711405B6106EA919) ,DATA (0XE2280B6C20DD5232) ,DATA (0X2D3B357C0692AA0A) ,DATA (0X5A766AF80D255414) ,DATA (0XB4ECD5F01A4AA828) ,DATA (0X242F5DFCD20EEE6E) ,DATA (0X485EBBF9A41DDCDC) ,DATA (0X90BD77F3483BB9B9) ,DATA (0X1CF2B1970E725858) ,DATA (0X39E5632E1CE4B0B0) ,DATA (0X73CAC65C39C96161) ,DATA (0XE7958CB87392C2C2) ,DATA (0X2E511C24E3EA26F3) ,DATA (0X5CA23849C7D44DE7) ,DATA (0XB94470938FA89BCE) ,DATA (0X250DB01D8321B8C2) ,DATA (0X4A1B603B06437185) ,DATA (0X9436C0760C86E30B) ,DATA (0X1DA48CE468E7C702) ,DATA (0X3B4919C8D1CF8E04) ,DATA (0X76923391A39F1C09) ,DATA (0XED246723473E3813) ,DATA (0X2F6DAE3A4172D803) ,DATA (0X5EDB5C7482E5B007) ,DATA (0XBDB6B8E905CB600F) ,DATA (0X25F1582E9AC24669) ,DATA (0X4BE2B05D35848CD2) ,DATA (0X97C560BA6B0919A5) ,DATA (0X1E5AACF215683854) ,DATA (0X3CB559E42AD070A8) ,DATA (0X796AB3C855A0E151) ,DATA (0XF2D56790AB41C2A2) ,DATA (0X309114B688A6C086) ,DATA (0X6122296D114D810D) ,DATA (0XC24452DA229B021B) ,DATA (0X26DA76F86D52339F) ,DATA (0X4DB4EDF0DAA4673E) ,DATA (0X9B69DBE1B548CE7C) ,DATA (0X1F152BF9F10E8FB2) ,DATA (0X3E2A57F3E21D1F65) ,DATA (0X7C54AFE7C43A3ECA) ,DATA (0XF8A95FCF88747D94) ,DATA (0X31BB798FE8174C50) ,DATA (0X6376F31FD02E98A1) ,DATA (0XC6EDE63FA05D3143) ,DATA (0X27C92E0CB9AC3D0D) ,DATA (0X4F925C1973587A1B) ,DATA (0X9F24B832E6B0F436) ,DATA (0X1FD424D6FAF030D7) ,DATA (0X3FA849ADF5E061AF) ,DATA (0X7F50935BEBC0C35E) ,DATA (0XFEA126B7D78186BC) ,DATA (0X32ED07BE5E4D1AF2) ,DATA (0X65DA0F7CBC9A35E5) ,DATA (0XCBB41EF979346BCA) ,DATA (0X28BD9FCB7EA4158E) ,DATA (0X517B3F96FD482B1D) ,DATA (0XA2F67F2DFA90563B) ,DATA (0X2097B309321CDE0B) ,DATA (0X412F66126439BC17) ,DATA (0X825ECC24C873782F) ,DATA (0X1A12F5A0F4E3E4D6) ,DATA (0X3425EB41E9C7C9AC) ,DATA (0X684BD683D38F9359) ,DATA (0XD097AD07A71F26B2) ,DATA (0X29B7EF67EE396E23) ,DATA (0X536FDECFDC72DC47) ,DATA (0XA6DFBD9FB8E5B88E) ,DATA (0X215FF2B98B6124E9) ,DATA (0X42BFE57316C249D2) ,DATA (0X857FCAE62D8493A5) ,DATA (0X1AB328946F80EA54) ,DATA (0X35665128DF01D4A8) ,DATA (0X6ACCA251BE03A951) ,DATA (0XD59944A37C0752A2) ,DATA (0X2AB840ED7F34AA20) ,DATA (0X557081DAFE695440) ,DATA (0XAAE103B5FCD2A881) ,DATA (0X222D00BDFF5D54E6) ,DATA (0X445A017BFEBAA9CD) ,DATA (0X88B402F7FD75539B) ,DATA (0X1B5733CB32B110B8) ,DATA (0X36AE679665622171) ,DATA (0X6D5CCF2CCAC442E2) ,DATA (0XDAB99E59958885C4) ,DATA (0X2BBEB9451DE81AC0) ,DATA (0X577D728A3BD03581) ,DATA (0XAEFAE51477A06B03) ,DATA (0X22FEFA9DB1867BCD) ,DATA (0X45FDF53B630CF79B) ,DATA (0X8BFBEA76C619EF36) ,DATA (0X1BFF2EE48E052FD7) ,DATA (0X37FE5DC91C0A5FAF) ,DATA (0X6FFCBB923814BF5E) ,DATA (0XDFF9772470297EBD) ,DATA (0X2CCB7E3A7CD51959) ,DATA (0X5996FC74F9AA32B2) ,DATA (0XB32DF8E9F3546564) ,DATA (0X23D5FE9530AA7AAD) ,DATA (0X47ABFD2A6154F55B) ,DATA (0X8F57FA54C2A9EAB6) ,DATA (0X1CAB3210F3BB9557) ,DATA (0X39566421E7772AAF) ,DATA (0X72ACC843CEEE555E) ,DATA (0XE55990879DDCAABD) ,DATA (0X2DDEB68185F8EEF2) ,DATA (0X5BBD6D030BF1DDE5) ,DATA (0XB77ADA0617E3BBCB) ,DATA (0X24B22B9AD193F25B) ,DATA (0X49645735A327E4B7) ,DATA (0X92C8AE6B464FC96F) ,DATA (0X1D5B561574765B7C) ,DATA (0X3AB6AC2AE8ECB6F9) ,DATA (0X756D5855D1D96DF2) ,DATA (0XEADAB0ABA3B2DBE5) ,DATA (0X2EF889BBED8A2BFA) ,DATA (0X5DF11377DB1457F5) ,DATA (0XBBE226EFB628AFEA) ,DATA (0X2593A163246E8995) ,DATA (0X4B2742C648DD132A) ,DATA (0X964E858C91BA2655) ,DATA (0X1E0FB44F50586E11) ,DATA (0X3C1F689EA0B0DC22) ,DATA (0X783ED13D4161B844) ,DATA (0XF07DA27A82C37088) ,DATA (0X3019207EE6F3E34E) ,DATA (0X603240FDCDE7C69C) ,DATA (0XC06481FB9BCF8D39) ,DATA (0X267A8065858FE90B) ,DATA (0X4CF500CB0B1FD217) ,DATA (0X99EA0196163FA42E) ,DATA (0X1EC866B79E0CBA6F) ,DATA (0X3D90CD6F3C1974DF) ,DATA (0X7B219ADE7832E9BE) ,DATA (0XF64335BCF065D37D) ,DATA (0X3140A458FCE12A4C) ,DATA (0X628148B1F9C25498) ,DATA (0XC5029163F384A931) ,DATA (0X2766E9E0CA4DBB70) ,DATA (0X4ECDD3C1949B76E0) ,DATA (0X9D9BA7832936EDC0) ,DATA (0X1F8587E7083E2F8C) ,DATA (0X3F0B0FCE107C5F19) ,DATA (0X7E161F9C20F8BE33) ,DATA (0XFC2C3F3841F17C67) ,DATA (0X326F3FD80D304C14) ,DATA (0X64DE7FB01A609829) ,DATA (0XC9BCFF6034C13052) ,DATA (0X2858FFE00A8D09AA) ,DATA (0X50B1FFC0151A1354) ,DATA (0XA163FF802A3426A8) ,DATA (0X20473319A20A6E21) ,DATA (0X408E66334414DC43) ,DATA (0X811CCC668829B887) ,DATA (0X19D28F47B4D524E7) ,DATA (0X33A51E8F69AA49CF) ,DATA (0X674A3D1ED354939F) ,DATA (0XCE947A3DA6A9273E) ,DATA (0X2950E53F87BB6E3F) ,DATA (0X52A1CA7F0F76DC7F) ,DATA (0XA54394FE1EEDB8FE) ,DATA (0X210D8432D2FC5832) ,DATA (0X421B0865A5F8B065) ,DATA (0X843610CB4BF160CB) ,DATA (0X1A71368F0F30468F) ,DATA (0X34E26D1E1E608D1E) ,DATA (0X69C4DA3C3CC11A3C) ,DATA (0XD389B47879823479) ,DATA (0X2A4EBDB1B1E6D74B) ,DATA (0X549D7B6363CDAE96) ,DATA (0XA93AF6C6C79B5D2D) ,DATA (0X21D897C15B1F12A2) ,DATA (0X43B12F82B63E2545) ,DATA (0X87625F056C7C4A8B) ,DATA (0X1B13AC9AAF4C0EE8) ,DATA (0X362759355E981DD1) ,DATA (0X6C4EB26ABD303BA2) ,DATA (0XD89D64D57A607744) ,DATA (0X2B52ADC44BACE4A7) ,DATA (0X56A55B889759C94E) ,DATA (0XAD4AB7112EB3929D) ,DATA (0X22A88B036FBD83B9) ,DATA (0X45511606DF7B0772) ,DATA (0X8AA22C0DBEF60EE4) ,DATA (0X1BBA08CF8C979C94) ,DATA (0X3774119F192F3928) ,DATA (0X6EE8233E325E7250) ,DATA (0XDDD0467C64BCE4A0) ,DATA (0X2C5CDAE5ADBF60EC) ,DATA (0X58B9B5CB5B7EC1D9) ,DATA (0XB1736B96B6FD83B3) ,DATA (0X237D7BEAF165E723) ,DATA (0X46FAF7D5E2CBCE47) ,DATA (0X8DF5EFABC5979C8F) ,DATA (0X1C6463225AB7EC1C) ,DATA (0X38C8C644B56FD839) ,DATA (0X71918C896ADFB073) ,DATA (0XE3231912D5BF60E6) ,DATA (0X2D6D6B6A2ABFE02E) ,DATA (0X5ADAD6D4557FC05C) ,DATA (0XB5B5ADA8AAFF80B8) ,DATA (0X24578921BBCCB358) ,DATA (0X48AF1243779966B0) ,DATA (0X915E2486EF32CD60) ,DATA (0X1D12D41AFCA3C2AC) ,DATA (0X3A25A835F9478559) ,DATA (0X744B506BF28F0AB3) ,DATA (0XE896A0D7E51E1566) ,DATA (0X2E8486919439377A) ,DATA (0X5D090D2328726EF5) ,DATA (0XBA121A4650E4DDEB) ,DATA (0X2536D20E102DC5FB) ,DATA (0X4A6DA41C205B8BF7) ,DATA (0X94DB483840B717EF) ,DATA (0X1DC574D80CF16B2F) ,DATA (0X3B8AE9B019E2D65F) ,DATA (0X7715D36033C5ACBF) ,DATA (0XEE2BA6C0678B597F) ,DATA (0X2FA2548CE1824519) ,DATA (0X5F44A919C3048A32) ,DATA (0XBE89523386091465) ,DATA (0X261B76D71ACE9DAD) ,DATA (0X4C36EDAE359D3B5B) ,DATA (0X986DDB5C6B3A76B7) ,DATA (0X1E7C5F127BD87E24) ,DATA (0X3CF8BE24F7B0FC49) ,DATA (0X79F17C49EF61F893) ,DATA (0XF3E2F893DEC3F126) ,DATA (0X30C6FE83F95A636E) ,DATA (0X618DFD07F2B4C6DC) ,DATA (0XC31BFA0FE5698DB8) ,DATA (0X2705986994484F8B) ,DATA (0X4E0B30D328909F16) ,DATA (0X9C1661A651213E2D) ,DATA (0X1F37AD21436D0C6F) ,DATA (0X3E6F5A4286DA18DE) ,DATA (0X7CDEB4850DB431BD) ,DATA (0XF9BD690A1B68637B) ,DATA (0X31F2AE9B9F14E0B2) ,DATA (0X63E55D373E29C164) ,DATA (0XC7CABA6E7C5382C8) ,DATA (0X27F5587C7F43E6F4) ,DATA (0X4FEAB0F8FE87CDE9) ,DATA (0X9FD561F1FD0F9BD3) ,DATA (0X1FF779FD329CB8C3) ,DATA (0X3FEEF3FA65397187) ,DATA (0X7FDDE7F4CA72E30F) ,DATA (0XFFBBCFE994E5C61F) ,DATA (0X33258FFB842DF46C) ,DATA (0X664B1FF7085BE8D9) ,DATA (0XCC963FEE10B7D1B3) ,DATA (0X28EAD9960357F6BD) ,DATA (0X51D5B32C06AFED7A) ,DATA (0XA3AB66580D5FDAF5) ,DATA (0X20BBE144CF799231) ,DATA (0X4177C2899EF32462) ,DATA (0X82EF85133DE648C4) ,DATA (0X1A2FE76A3F9474F4) ,DATA (0X345FCED47F28E9E8) ,DATA (0X68BF9DA8FE51D3D0) ,DATA (0XD17F3B51FCA3A7A0) ,DATA (0X29E63F1065BA54B9) ,DATA (0X53CC7E20CB74A973) ,DATA (0XA798FC4196E952E7) ,DATA (0X2184FF405161DD61) ,DATA (0X4309FE80A2C3BAC2) ,DATA (0X8613FD0145877585) ,DATA (0X1AD0CC33744E4AB4) ,DATA (0X35A19866E89C9568) ,DATA (0X6B4330CDD1392AD1) ,DATA (0XD686619BA27255A2) ,DATA (0X2AE7AD1F207D4453) ,DATA (0X55CF5A3E40FA88A7) ,DATA (0XAB9EB47C81F5114F) ,DATA (0X2252F0E5B39769DC) ,DATA (0X44A5E1CB672ED3B9) ,DATA (0X894BC396CE5DA772) ,DATA (0X1B758D848FAC54B0) ,DATA (0X36EB1B091F58A960) ,DATA (0X6DD636123EB152C1) ,DATA (0XDBAC6C247D62A583) ,DATA (0X2BEF48D41913BAB3) ,DATA (0X57DE91A832277567) ,DATA (0XAFBD2350644EEACF) ,DATA (0X2325D3DCE0DC955C) ,DATA (0X464BA7B9C1B92AB9) ,DATA (0X8C974F7383725573) ,DATA (0X1C1E43171A4A1117) ,DATA (0X383C862E3494222E) ,DATA (0X70790C5C6928445C) ,DATA (0XE0F218B8D25088B8) ,DATA (0X2CFD3824F6DCE824) ,DATA (0X59FA7049EDB9D049) ,DATA (0XB3F4E093DB73A093) ,DATA (0X23FDC683F8B0B9B7) ,DATA (0X47FB8D07F161736E) ,DATA (0X8FF71A0FE2C2E6DC) ,DATA (0X1CCB0536608D615F) ,DATA (0X39960A6CC11AC2BE) ,DATA (0X732C14D98235857D) ,DATA (0XE65829B3046B0AFA) ,DATA (0X2E11A1F09A7BCEFE) ,DATA (0X5C2343E134F79DFD) ,DATA (0XB84687C269EF3BFB) ,DATA (0X24DAE7F3AEC97265) ,DATA (0X49B5CFE75D92E4CA) ,DATA (0X936B9FCEBB25C995) ,DATA (0X1D7BECC2F23AC1EA) ,DATA (0X3AF7D985E47583D5) ,DATA (0X75EFB30BC8EB07AB) ,DATA (0XEBDF661791D60F56) ,DATA (0X2F2CAE04B6C46977) ,DATA (0X5E595C096D88D2EF) ,DATA (0XBCB2B812DB11A5DE) ,DATA (0X25BD5803C569EDF9) ,DATA (0X4B7AB0078AD3DBF2) ,DATA (0X96F5600F15A7B7E5) ,DATA (0X1E3113363787F194) ,DATA (0X3C62266C6F0FE328) ,DATA (0X78C44CD8DE1FC650) ,DATA (0XF18899B1BC3F8CA1) ,DATA (0X304E85238C0CB5B9) ,DATA (0X609D0A4718196B73) ,DATA (0XC13A148E3032D6E7) ,DATA (0X26A5374FA33D5E2E) ,DATA (0X4D4A6E9F467ABC5C) ,DATA (0X9A94DD3E8CF578B9) ,DATA (0X1EEA92A61C311825) ,DATA (0X3DD5254C3862304A) ,DATA (0X7BAA4A9870C46094) ,DATA (0XF7549530E188C128) ,DATA (0X31775109C6B4F36E) ,DATA (0X62EEA2138D69E6DD) ,DATA (0XC5DD44271AD3CDBA) ,DATA (0X2792A73B055D8F8B) ,DATA (0X4F254E760ABB1F17) ,DATA (0X9E4A9CEC15763E2E) ,DATA (0X1FA885C8D117A609) ,DATA (0X3F510B91A22F4C12) ,DATA (0X7EA21723445E9825) ,DATA (0XFD442E4688BD304A) ,DATA (0X32A73C7481BF700E) ,DATA (0X654E78E9037EE01D) ,DATA (0XCA9CF1D206FDC03B) ,DATA (0X2885C9F6CE32C00B) ,DATA (0X510B93ED9C658017) ,DATA (0XA21727DB38CB002F) ,DATA (0X206B07F8A4F5666F) ,DATA (0X40D60FF149EACCDF) ,DATA (0X81AC1FE293D599BF) ,DATA (0X19EF3993B72AB859) ,DATA (0X33DE73276E5570B3) ,DATA (0X67BCE64EDCAAE166) ,DATA (0XCF79CC9DB955C2CC) ,DATA (0X297EC285F1DDF3C2) ,DATA (0X52FD850BE3BBE784) ,DATA (0XA5FB0A17C777CF09) ,DATA (0X21323537F4B18FCE) ,DATA (0X42646A6FE9631F9D) ,DATA (0X84C8D4DFD2C63F3B) ,DATA (0X1A8E90F9908E0CA5) ,DATA (0X351D21F3211C194A) ,DATA (0X6A3A43E642383295) ,DATA (0XD47487CC8470652B) ,DATA (0X2A7DB4C280E3476F) ,DATA (0X54FB698501C68EDE) ,DATA (0XA9F6D30A038D1DBC) ,DATA (0X21FE2A3533E905F2) ,DATA (0X43FC546A67D20BE4) ,DATA (0X87F8A8D4CFA417C9) ,DATA (0X1B31BB5DC320D18E) ,DATA (0X366376BB8641A31D) ,DATA (0X6CC6ED770C83463B) ,DATA (0XD98DDAEE19068C76) ,DATA (0X2B82C562D1CE1C17) ,DATA (0X57058AC5A39C382F) ,DATA (0XAE0B158B4738705E) ,DATA (0X22CF044F0E3E7CDF) ,DATA (0X459E089E1C7CF9BF) ,DATA (0X8B3C113C38F9F37E) ,DATA (0X1BD8D03F3E9863E6) ,DATA (0X37B1A07E7D30C7CC) ,DATA (0X6F6340FCFA618F98) ,DATA (0XDEC681F9F4C31F31) ,DATA (0X2C8E19FECA8D6CA3) ,DATA (0X591C33FD951AD946) ,DATA (0XB23867FB2A35B28D) ,DATA (0X23A4E198A20ABD4F) ,DATA (0X4749C33144157A9F) ,DATA (0X8E938662882AF53E) ,DATA (0X1C83E7AD4E6EFDD9) ,DATA (0X3907CF5A9CDDFBB2) ,DATA (0X720F9EB539BBF765) ,DATA (0XE41F3D6A7377EECA) ,DATA (0X2D9FD9154A4B2FC2) ,DATA (0X5B3FB22A94965F84) ,DATA (0XB67F6455292CBF08) ,DATA (0X247FE0DDD508F301) ,DATA (0X48FFC1BBAA11E603) ,DATA (0X91FF83775423CC06) ,DATA (0X1D331A4B10D3F59A) ,DATA (0X3A66349621A7EB35) ,DATA (0X74CC692C434FD66B) ,DATA (0XE998D258869FACD7) ,DATA (0X2EB82A11B48655C4) ,DATA (0X5D705423690CAB89) ,DATA (0XBAE0A846D2195712) ,DATA (0X256021A7C39EAB03) ,DATA (0X4AC0434F873D5607) ,DATA (0X9580869F0E7AAC0E) ,DATA (0X1DE6815302E5559C) ,DATA (0X3BCD02A605CAAB39) ,DATA (0X779A054C0B955672) ,DATA (0XEF340A98172AACE4) ,DATA (0X2FD735519E3BBC2D) ,DATA (0X5FAE6AA33C77785B) ,DATA (0XBF5CD54678EEF0B6) ,DATA (0X2645C4414B62FCF1) ,DATA (0X4C8B888296C5F9E2) ,DATA (0X991711052D8BF3C5) ,DATA (0X1E9E369AA2B59727) ,DATA (0X3D3C6D35456B2E4E) ,DATA (0X7A78DA6A8AD65C9D) ,DATA (0XF4F1B4D515ACB93B) ,DATA (0X30FD242A9DEF583F) ,DATA (0X61FA48553BDEB07E) ,DATA (0XC3F490AA77BD60FC) ,DATA (0X2730E9BBB18C4698) ,DATA (0X4E61D37763188D31) ,DATA (0X9CC3A6EEC6311A63) ,DATA (0X1F5A549627A36BAD) ,DATA (0X3EB4A92C4F46D75B) ,DATA (0X7D6952589E8DAEB6) ,DATA (0XFAD2A4B13D1B5D6C) ,DATA (0X322A20F03F6BDF7C) ,DATA (0X645441E07ED7BEF8) ,DATA (0XC8A883C0FDAF7DF0) ,DATA (0X2821B3F365EFE5FC) ,DATA (0X504367E6CBDFCBF9) ,DATA (0XA086CFCD97BF97F3) ,DATA (0X201AF65C518CB7FD) ,DATA (0X4035ECB8A3196FFB) ,DATA (0X806BD9714632DFF6) ,DATA (0X19AF2B7D0E0A2CCA) ,DATA (0X335E56FA1C145995) ,DATA (0X66BCADF43828B32B) ,DATA (0XCD795BE870516656) ,DATA (0X29184594E3437ADE) ,DATA (0X52308B29C686F5BC) ,DATA (0XA46116538D0DEB78) ,DATA (0X20E037AA4F692F18) ,DATA (0X41C06F549ED25E30) ,DATA (0X8380DEA93DA4BC60) ,DATA (0X1A4CF9550C5425AC) ,DATA (0X3499F2AA18A84B59) ,DATA (0X6933E554315096B3) ,DATA (0XD267CAA862A12D66) ,DATA (0X2A14C221AD536F7A) ,DATA (0X542984435AA6DEF5) ,DATA (0XA8530886B54DBDEB) ,DATA (0X21AA34E7BDDC592F) ,DATA (0X435469CF7BB8B25E) ,DATA (0X86A8D39EF77164BC) ,DATA (0X1AEE90B964B04758) ,DATA (0X35DD2172C9608EB1) ,DATA (0X6BBA42E592C11D63) ,DATA (0XD77485CB25823AC7) ,DATA (0X2B174DF56DE6D88E) ,DATA (0X562E9BEADBCDB11C) ,DATA (0XAC5D37D5B79B6239) ,DATA (0X22790B2ABE5246D8) ,DATA (0X44F216557CA48DB0) ,DATA (0X89E42CAAF9491B60) ,DATA (0X1B9408EEFEA838AC) ,DATA (0X372811DDFD507159) ,DATA (0X6E5023BBFAA0E2B3) ,DATA (0XDCA04777F541C567) ,DATA (0X2C200E4B310D277B) ,DATA (0X58401C96621A4EF6) ,DATA (0XB080392CC4349DEC) ,DATA (0X234CD83C273DB92F) ,DATA (0X4699B0784E7B725E) ,DATA (0X8D3360F09CF6E4BD) ,DATA (0X1C3D79C9B8FE2DBF) ,DATA (0X387AF39371FC5B7E) ,DATA (0X70F5E726E3F8B6FD) ,DATA (0XE1EBCE4DC7F16DFB) ,DATA (0X2D2F2942C196AF98) ,DATA (0X5A5E5285832D5F31) ,DATA (0XB4BCA50B065ABE63) ,DATA (0X2425BA9BCE122613) ,DATA (0X484B75379C244C27) ,DATA (0X9096EA6F3848984F) ,DATA (0X1CEAFBAFD80E84DC) ,DATA (0X39D5F75FB01D09B9) ,DATA (0X73ABEEBF603A1372) ,DATA (0XE757DD7EC07426E5) ,DATA (0X2E44C5E6267DA161) ,DATA (0X5C898BCC4CFB42C2) ,DATA (0XB913179899F68584) ,DATA (0X2503D184EB97B44D) ,DATA (0X4A07A309D72F689B) ,DATA (0X940F4613AE5ED136) ,DATA (0X1D9CA79D894629D7) ,DATA (0X3B394F3B128C53AF) ,DATA (0X76729E762518A75E) ,DATA (0XECE53CEC4A314EBD) ,DATA (0X2F610C2F4209DC8C) ,DATA (0X5EC2185E8413B918) ,DATA (0XBD8430BD08277231) ,DATA (0X25E73CF29B3B16D6) ,DATA (0X4BCE79E536762DAD) ,DATA (0X979CF3CA6CEC5B5A) ,DATA (0X1E5297287C2F4578) ,DATA (0X3CA52E50F85E8AF1) ,DATA (0X794A5CA1F0BD15E2) ,DATA (0XF294B943E17A2BC4) ,DATA (0X3084250D937ED58D) ,DATA (0X61084A1B26FDAB1B) ,DATA (0XC21094364DFB5636) ,DATA (0X26D01DA475FF113E) ,DATA (0X4DA03B48EBFE227C) ,DATA (0X9B407691D7FC44F8) ,DATA (0X1F0CE4839198DA98) ,DATA (0X3E19C9072331B530) ,DATA (0X7C33920E46636A60) ,DATA (0XF867241C8CC6D4C0) ,DATA (0X31AE3A6C1C27C426) ,DATA (0X635C74D8384F884D) ,DATA (0XC6B8E9B0709F109A) ,DATA (0X27BE952349B969B8) ,DATA (0X4F7D2A469372D370) ,DATA (0X9EFA548D26E5A6E1) ,DATA (0X1FCBAA82A1612160) ,DATA (0X3F97550542C242C0) ,DATA (0X7F2EAA0A85848581) ,DATA (0XFE5D54150B090B02) ,DATA (0X32DF7737689B689A) ,DATA (0X65BEEE6ED136D134) ,DATA (0XCB7DDCDDA26DA268) ,DATA (0X28B2C5C5ED49207B) ,DATA (0X51658B8BDA9240F6) ,DATA (0XA2CB1717B52481ED) ,DATA (0X208F049E576DB395) ,DATA (0X411E093CAEDB672B) ,DATA (0X823C12795DB6CE57) ,DATA (0X1A0C03B1DF8AF611) ,DATA (0X34180763BF15EC22) ,DATA (0X68300EC77E2BD845) ,DATA (0XD0601D8EFC57B08B) ,DATA (0X29ACD2B63277F01B) ,DATA (0X5359A56C64EFE037) ,DATA (0XA6B34AD8C9DFC06F) ,DATA (0X21570EF8285FF349) ,DATA (0X42AE1DF050BFE693) ,DATA (0X855C3BE0A17FCD26) ,DATA (0X1AAC0BF9B9E65C3A) ,DATA (0X355817F373CCB875) ,DATA (0X6AB02FE6E79970EB) ,DATA (0XD5605FCDCF32E1D6) ,DATA (0X2AACDFF5F63D605E) ,DATA (0X5559BFEBEC7AC0BC) ,DATA (0XAAB37FD7D8F58178) ,DATA (0X2223E65E5E97804B) ,DATA (0X4447CCBCBD2F0096) ,DATA (0X888F99797A5E012D) ,DATA (0X1B4FEB7EB212CD09) ,DATA (0X369FD6FD64259A12) ,DATA (0X6D3FADFAC84B3424) ,DATA (0XDA7F5BF590966848) ,DATA (0X2BB31264501E14DB) ,DATA (0X576624C8A03C29B6) ,DATA (0XAECC49914078536D) ,DATA (0X22F5A850401810AF) ,DATA (0X45EB50A08030215E) ,DATA (0X8BD6A141006042BD) ,DATA (0X1BF7B9D9CCE00D59) ,DATA (0X37EF73B399C01AB2) ,DATA (0X6FDEE76733803564) ,DATA (0XDFBDCECE67006AC9) ,DATA (0X2CBF8FC2E1667BC1) ,DATA (0X597F1F85C2CCF783) ,DATA (0XB2FE3F0B8599EF07) ,DATA (0X23CC73024DEB9634) ,DATA (0X4798E6049BD72C69) ,DATA (0X8F31CC0937AE58D2) ,DATA (0X1CA38F350B22DE90) ,DATA (0X39471E6A1645BD21) ,DATA (0X728E3CD42C8B7A42) ,DATA (0XE51C79A85916F484) ,DATA (0X2DD27EBB4504974D) ,DATA (0X5BA4FD768A092E9B) ,DATA (0XB749FAED14125D36) ,DATA (0X24A865629D9D45D7) ,DATA (0X4950CAC53B3A8BAF) ,DATA (0X92A1958A7675175F) ,DATA (0X1D53844EE47DD179) ,DATA (0X3AA7089DC8FBA2F2) ,DATA (0X754E113B91F745E5) ,DATA (0XEA9C227723EE8BCB) ,DATA (0X2EEC06E4A0C94F28) ,DATA (0X5DD80DC941929E51) ,DATA (0XBBB01B9283253CA2) ,DATA (0X25899F1D4D6DD8ED) ,DATA (0X4B133E3A9ADBB1DA) ,DATA (0X96267C7535B763B5) ,DATA (0X1E07B27DD78B13F1) ,DATA (0X3C0F64FBAF1627E2) ,DATA (0X781EC9F75E2C4FC4) ,DATA (0XF03D93EEBC589F88) ,DATA (0X300C50C958DE864E) ,DATA (0X6018A192B1BD0C9C) ,DATA (0XC0314325637A1939) ,DATA (0X267040A113E5383E) ,DATA (0X4CE0814227CA707D) ,DATA (0X99C102844F94E0FB) ,DATA (0X1EC033B40FEA9365) ,DATA (0X3D8067681FD526CA) ,DATA (0X7B00CED03FAA4D95) ,DATA (0XF6019DA07F549B2B) ,DATA (0X313385ECE6441F08) ,DATA (0X62670BD9CC883E11) ,DATA (0XC4CE17B399107C22) ,DATA (0X275C6B23EB69B26D) ,DATA (0X4EB8D647D6D364DA) ,DATA (0X9D71AC8FADA6C9B5) ,DATA (0X1F7D228322BAF524) ,DATA (0X3EFA45064575EA48) ,DATA (0X7DF48A0C8AEBD491) ,DATA (0XFBE9141915D7A922) ,DATA (0X3261D0D1D12B21D3) ,DATA (0X64C3A1A3A25643A7) ,DATA (0XC987434744AC874E) ,DATA (0X284E40A7DA88E7DC) ,DATA (0X509C814FB511CFB9) ,DATA (0XA139029F6A239F72) ,DATA (0X203E9A1FE2071FE3) ,DATA (0X407D343FC40E3FC7) ,DATA (0X80FA687F881C7F8E) ,DATA (0X19CBAE7FE805B31C) ,DATA (0X33975CFFD00B6638) ,DATA (0X672EB9FFA016CC71) ,DATA (0XCE5D73FF402D98E3) ,DATA (0X2945E3FFD9A2B82D) ,DATA (0X528BC7FFB345705B) ,DATA (0XA5178FFF668AE0B6) ,DATA (0X2104B66647B56024) ,DATA (0X42096CCC8F6AC048) ,DATA (0X8412D9991ED58091) ,DATA (0X1A6A2B85062AB350) ,DATA (0X34D4570A0C5566A0) ,DATA (0X69A8AE1418AACD41) ,DATA (0XD3515C2831559A83) ,DATA (0X2A4378D4D6AAB880) ,DATA (0X5486F1A9AD557101) ,DATA (0XA90DE3535AAAE202) ,DATA (0X21CF93DD7888939A) ,DATA (0X439F27BAF1112734) ,DATA (0X873E4F75E2224E68) ,DATA (0X1B0C764AC6D3A948) ,DATA (0X3618EC958DA75290) ,DATA (0X6C31D92B1B4EA520) ,DATA (0XD863B256369D4A40) ,DATA (0X2B4723AAD7B90ED9) ,DATA (0X568E4755AF721DB3) ,DATA (0XAD1C8EAB5EE43B66) ,DATA (0X229F4FBBDFC73F14) ,DATA (0X453E9F77BF8E7E29) ,DATA (0X8A7D3EEF7F1CFC52) ,DATA (0X1BB2A62FE638FF43) ,DATA (0X37654C5FCC71FE87) ,DATA (0X6ECA98BF98E3FD0E) ,DATA (0XDD95317F31C7FA1D) ,DATA (0X2C5109E63D27FED2) ,DATA (0X58A213CC7A4FFDA5) ,DATA (0XB1442798F49FFB4A) ,DATA (0X237407EB641FFF0E) ,DATA (0X46E80FD6C83FFE1D) ,DATA (0X8DD01FAD907FFC3B) ,DATA (0X1C5CD322B67FFF3F) ,DATA (0X38B9A6456CFFFE7E) ,DATA (0X71734C8AD9FFFCFC) ,DATA (0XE2E69915B3FFF9F9) ,DATA (0X2D6151D123FFFECB) ,DATA (0X5AC2A3A247FFFD96) ,DATA (0XB58547448FFFFB2D) ,DATA (0X244DDB0DB666656F) ,DATA (0X489BB61B6CCCCADF) ,DATA (0X91376C36D99995BE) ,DATA (0X1D0B15A491EB8459) ,DATA (0X3A162B4923D708B2) ,DATA (0X742C569247AE1164) ,DATA (0XE858AD248F5C22C9) ,DATA (0X2E7822A0E978D3C1) ,DATA (0X5CF04541D2F1A783) ,DATA (0XB9E08A83A5E34F07) ,DATA (0X252CE880BAC70FCE) ,DATA (0X4A59D101758E1F9C) ,DATA (0X94B3A202EB1C3F39) ,DATA (0X1DBD86CD6238D971) ,DATA (0X3B7B0D9AC471B2E3) ,DATA (0X76F61B3588E365C7) ,DATA (0XEDEC366B11C6CB8F) ,DATA (0X2F95A47BD05AF583) ,DATA (0X5F2B48F7A0B5EB06) ,DATA (0XBE5691EF416BD60C) ,DATA (0X261150630D159135) ,DATA (0X4C22A0C61A2B226B) ,DATA (0X9845418C345644D6) ,DATA (0X1E74404F3DAADA91) ,DATA (0X3CE8809E7B55B522) ,DATA (0X79D1013CF6AB6A45) ,DATA (0XF3A20279ED56D48A) ,DATA (0X30BA007EC9115DB5) ,DATA (0X617400FD9222BB6A) ,DATA (0XC2E801FB244576D5) ,DATA (0X26FB3398A0DAB15D) ,DATA (0X4DF6673141B562BB) ,DATA (0X9BECCE62836AC577) ,DATA (0X1F2F5C7A1A488DE4) ,DATA (0X3E5EB8F434911BC9) ,DATA (0X7CBD71E869223792) ,DATA (0XF97AE3D0D2446F25) ,DATA (0X31E560C35D40E307) ,DATA (0X63CAC186BA81C60E) ,DATA (0XC795830D75038C1D) ,DATA (0X27EAB3CF7DCD826C) ,DATA (0X4FD5679EFB9B04D8) ,DATA (0X9FAACF3DF73609B1) ,DATA (0X1FEEF63F97D79B89) ,DATA (0X3FDDEC7F2FAF3713) ,DATA (0X7FBBD8FE5F5E6E27) ,DATA (0XFF77B1FCBEBCDC4F) ,DATA (0X3317F065BFBF5F43) ,DATA (0X662FE0CB7F7EBE86) ,DATA (0XCC5FC196FEFD7D0C) ,DATA (0X28DFF3849965E5CF) ,DATA (0X51BFE70932CBCB9E) ,DATA (0XA37FCE126597973C) ,DATA (0X20B32936E11E5172) ,DATA (0X4166526DC23CA2E5) ,DATA (0X82CCA4DB847945CA) ,DATA (0X1A28EDC580E50DF5) ,DATA (0X3451DB8B01CA1BEA) ,DATA (0X68A3B716039437D5) ,DATA (0XD1476E2C07286FAA) ,DATA (0X29DB1608CE3B4988) ,DATA (0X53B62C119C769310) ,DATA (0XA76C582338ED2621) ,DATA (0X217C11A0A4FC3AD3) ,DATA (0X42F8234149F875A7) ,DATA (0X85F0468293F0EB4E) ,DATA (0X1AC9A7B3B7302F0F) ,DATA (0X35934F676E605E1F) ,DATA (0X6B269ECEDCC0BC3E) ,DATA (0XD64D3D9DB981787D) ,DATA (0X2ADC3F85F1E6B1B2) ,DATA (0X55B87F0BE3CD6365) ,DATA (0XAB70FE17C79AC6CA) ,DATA (0X2249CC6B27EBC15B) ,DATA (0X449398D64FD782B7) ,DATA (0X892731AC9FAF056E) ,DATA (0X1B6E3D2286563449) ,DATA (0X36DC7A450CAC6892) ,DATA (0X6DB8F48A1958D125) ,DATA (0XDB71E91432B1A24A) ,DATA (0X2BE395040A2386DB) ,DATA (0X57C72A0814470DB7) ,DATA (0XAF8E5410288E1B6F) ,DATA (0X231C77366E82D249) ,DATA (0X4638EE6CDD05A492) ,DATA (0X8C71DCD9BA0B4925) ,DATA (0X1C16C5C525357507) ,DATA (0X382D8B8A4A6AEA0F) ,DATA (0X705B171494D5D41E) ,DATA (0XE0B62E2929ABA83C) ,DATA (0X2CF13C6EA1EF21A5) ,DATA (0X59E278DD43DE434B) ,DATA (0XB3C4F1BA87BC8696) ,DATA (0X23F43058818C1AEA) ,DATA (0X47E860B1031835D5) ,DATA (0X8FD0C16206306BAB) ,DATA (0X1CC359E067A348BB) ,DATA (0X3986B3C0CF469177) ,DATA (0X730D67819E8D22EF) ,DATA (0XE61ACF033D1A45DF)} ;
			static const LENGTH M_ECACHE[] {
				-346 ,-346 ,-345 ,-345 ,-345 ,-344 ,-344 ,-344 ,-344 ,-343 ,-343 ,-343 ,-342 ,-342 ,-342 ,-341 ,-341 ,-341 ,-341 ,-340 ,-340 ,-340 ,-339 ,-339 ,-339 ,-338 ,-338 ,-338 ,-338 ,-337 ,-337 ,-337 ,-336 ,-336 ,-336 ,-335 ,-335 ,-335 ,-335 ,-334 ,-334 ,-334 ,-333 ,-333 ,-333 ,-332 ,-332 ,-332 ,-332 ,-331 ,-331 ,-331 ,-330 ,-330 ,-330 ,-329 ,-329 ,-329 ,-329 ,-328 ,-328 ,-328 ,-327 ,-327 ,-327 ,-326 ,-326 ,-326 ,-326 ,-325 ,-325 ,-325 ,-324 ,-324 ,-324 ,-323 ,-323 ,-323 ,-323 ,-322 ,-322 ,-322 ,-321 ,-321 ,-321 ,-320 ,-320 ,-320 ,-319 ,-319 ,-319 ,-319 ,-318 ,-318 ,-318 ,-317 ,-317 ,-317 ,-316 ,-316 ,-316 ,-316 ,-315 ,-315 ,-315 ,-314 ,-314 ,-314 ,-313 ,-313 ,-313 ,-313 ,-312 ,-312 ,-312 ,-311 ,-311 ,-311 ,-310 ,-310 ,-310 ,-310 ,-309 ,-309 ,-309 ,-308 ,-308 ,-308 ,-307 ,-307 ,-307 ,-307 ,-306 ,-306 ,-306 ,-305 ,-305 ,-305 ,-304 ,-304 ,-304 ,-304 ,-303 ,-303 ,-303 ,-302 ,-302 ,-302 ,-301 ,-301 ,-301 ,-301 ,-300 ,-300 ,-300 ,-299 ,-299 ,-299 ,-298 ,-298 ,-298 ,-298 ,-297 ,-297 ,-297 ,-296 ,-296 ,-296 ,-295 ,-295 ,-295 ,-295 ,-294 ,-294 ,-294 ,-293 ,-293 ,-293 ,-292 ,-292 ,-292 ,-291 ,-291 ,-291 ,-291 ,-290 ,-290 ,-290 ,-289 ,-289 ,-289 ,-288 ,-288 ,-288 ,-288 ,-287 ,-287 ,-287 ,-286 ,-286 ,-286 ,-285 ,-285 ,-285 ,-285 ,-284 ,-284 ,-284 ,-283 ,-283 ,-283 ,-282 ,-282 ,-282 ,-282 ,-281 ,-281 ,-281 ,-280 ,-280 ,-280 ,-279 ,-279 ,-279 ,-279 ,-278 ,-278 ,-278 ,-277 ,-277 ,-277 ,-276 ,-276 ,-276 ,-276 ,-275 ,-275 ,-275 ,-274 ,-274 ,-274 ,-273 ,-273 ,-273 ,-273 ,-272 ,-272 ,-272 ,-271 ,-271 ,-271 ,-270 ,-270 ,-270 ,-270 ,-269 ,-269 ,-269 ,-268 ,-268 ,-268 ,-267 ,-267 ,-267 ,-267 ,-266 ,-266 ,-266 ,-265 ,-265 ,-265 ,-264 ,-264 ,-264 ,-264 ,-263 ,-263 ,-263 ,-262 ,-262 ,-262 ,-261 ,-261 ,-261 ,-260 ,-260 ,-260 ,-260 ,-259 ,-259 ,-259 ,-258 ,-258 ,-258 ,-257 ,-257 ,-257 ,-257 ,-256 ,-256 ,-256 ,-255 ,-255 ,-255 ,-254 ,-254 ,-254 ,-254 ,-253 ,-253 ,-253 ,-252 ,-252 ,-252 ,-251 ,-251 ,-251 ,-251 ,-250 ,-250 ,-250 ,-249 ,-249 ,-249 ,-248 ,-248 ,-248 ,-248 ,-247 ,-247 ,-247 ,-246 ,-246 ,-246 ,-245 ,-245 ,-245 ,-245 ,-244 ,-244 ,-244 ,-243 ,-243 ,-243 ,-242 ,-242 ,-242 ,-242 ,-241 ,-241 ,-241 ,-240 ,-240 ,-240 ,-239 ,-239 ,-239 ,-239 ,-238 ,-238 ,-238 ,-237 ,-237 ,-237 ,-236 ,-236 ,-236 ,-236 ,-235 ,-235 ,-235 ,-234 ,-234 ,-234 ,-233 ,-233 ,-233 ,-232 ,-232 ,-232 ,-232 ,-231 ,-231 ,-231 ,-230 ,-230 ,-230 ,-229 ,-229 ,-229 ,-229 ,-228 ,-228 ,-228 ,-227 ,-227 ,-227 ,-226 ,-226 ,-226 ,-226 ,-225 ,-225 ,-225 ,-224 ,-224 ,-224 ,-223 ,-223 ,-223 ,-223 ,-222 ,-222 ,-222 ,-221 ,-221 ,-221 ,-220 ,-220 ,-220 ,-220 ,-219 ,-219 ,-219 ,-218 ,-218 ,-218 ,-217 ,-217 ,-217 ,-217 ,-216 ,-216 ,-216 ,-215 ,-215 ,-215 ,-214 ,-214 ,-214 ,-214 ,-213 ,-213 ,-213 ,-212 ,-212 ,-212 ,-211 ,-211 ,-211 ,-211 ,-210 ,-210 ,-210 ,-209 ,-209 ,-209 ,-208 ,-208 ,-208 ,-208 ,-207 ,-207 ,-207 ,-206 ,-206 ,-206 ,-205 ,-205 ,-205 ,-205 ,-204 ,-204 ,-204 ,-203 ,-203 ,-203 ,-202 ,-202 ,-202 ,-201 ,-201 ,-201 ,-201 ,-200 ,-200 ,-200 ,-199 ,-199 ,-199 ,-198 ,-198 ,-198 ,-198 ,-197 ,-197 ,-197 ,-196 ,-196 ,-196 ,-195 ,-195 ,-195 ,-195 ,-194 ,-194 ,-194 ,-193 ,-193 ,-193 ,-192 ,-192 ,-192 ,-192 ,-191 ,-191 ,-191 ,-190 ,-190 ,-190 ,-189 ,-189 ,-189 ,-189 ,-188 ,-188 ,-188 ,-187 ,-187 ,-187 ,-186 ,-186 ,-186 ,-186 ,-185 ,-185 ,-185 ,-184 ,-184 ,-184 ,-183 ,-183 ,-183 ,-183 ,-182 ,-182 ,-182 ,-181 ,-181 ,-181 ,-180 ,-180 ,-180 ,-180 ,-179 ,-179 ,-179 ,-178 ,-178 ,-178 ,-177 ,-177 ,-177 ,-177 ,-176 ,-176 ,-176 ,-175 ,-175 ,-175 ,-174 ,-174 ,-174 ,-173 ,-173 ,-173 ,-173 ,-172 ,-172 ,-172 ,-171 ,-171 ,-171 ,-170 ,-170 ,-170 ,-170 ,-169 ,-169 ,-169 ,-168 ,-168 ,-168 ,-167 ,-167 ,-167 ,-167 ,-166 ,-166 ,-166 ,-165 ,-165 ,-165 ,-164 ,-164 ,-164 ,-164 ,-163 ,-163 ,-163 ,-162 ,-162 ,-162 ,-161 ,-161 ,-161 ,-161 ,-160 ,-160 ,-160 ,-159 ,-159 ,-159 ,-158 ,-158 ,-158 ,-158 ,-157 ,-157 ,-157 ,-156 ,-156 ,-156 ,-155 ,-155 ,-155 ,-155 ,-154 ,-154 ,-154 ,-153 ,-153 ,-153 ,-152 ,-152 ,-152 ,-152 ,-151 ,-151 ,-151 ,-150 ,-150 ,-150 ,-149 ,-149 ,-149 ,-149 ,-148 ,-148 ,-148 ,-147 ,-147 ,-147 ,-146 ,-146 ,-146 ,-145 ,-145 ,-145 ,-145 ,-144 ,-144 ,-144 ,-143 ,-143 ,-143 ,-142 ,-142 ,-142 ,-142 ,-141 ,-141 ,-141 ,-140 ,-140 ,-140 ,-139 ,-139 ,-139 ,-139 ,-138 ,-138 ,-138 ,-137 ,-137 ,-137 ,-136 ,-136 ,-136 ,-136 ,-135 ,-135 ,-135 ,-134 ,-134 ,-134 ,-133 ,-133 ,-133 ,-133 ,-132 ,-132 ,-132 ,-131 ,-131 ,-131 ,-130 ,-130 ,-130 ,-130 ,-129 ,-129 ,-129 ,-128 ,-128 ,-128 ,-127 ,-127 ,-127 ,-127 ,-126 ,-126 ,-126 ,-125 ,-125 ,-125 ,-124 ,-124 ,-124 ,-124 ,-123 ,-123 ,-123 ,-122 ,-122 ,-122 ,-121 ,-121 ,-121 ,-121 ,-120 ,-120 ,-120 ,-119 ,-119 ,-119 ,-118 ,-118 ,-118 ,-118 ,-117 ,-117 ,-117 ,-116 ,-116 ,-116 ,-115 ,-115 ,-115 ,-114 ,-114 ,-114 ,-114 ,-113 ,-113 ,-113 ,-112 ,-112 ,-112 ,-111 ,-111 ,-111 ,-111 ,-110 ,-110 ,-110 ,-109 ,-109 ,-109 ,-108 ,-108 ,-108 ,-108 ,-107 ,-107 ,-107 ,-106 ,-106 ,-106 ,-105 ,-105 ,-105 ,-105 ,-104 ,-104 ,-104 ,-103 ,-103 ,-103 ,-102 ,-102 ,-102 ,-102 ,-101 ,-101 ,-101 ,-100 ,-100 ,-100 ,-99 ,-99 ,-99 ,-99 ,-98 ,-98 ,-98 ,-97 ,-97 ,-97 ,-96 ,-96 ,-96 ,-96 ,-95 ,-95 ,-95 ,-94 ,-94 ,-94 ,-93 ,-93 ,-93 ,-93 ,-92 ,-92 ,-92 ,-91 ,-91 ,-91 ,-90 ,-90 ,-90 ,-90 ,-89 ,-89 ,-89 ,-88 ,-88 ,-88 ,-87 ,-87 ,-87 ,-86 ,-86 ,-86 ,-86 ,-85 ,-85 ,-85 ,-84 ,-84 ,-84 ,-83 ,-83 ,-83 ,-83 ,-82 ,-82 ,-82 ,-81 ,-81 ,-81 ,-80 ,-80 ,-80 ,-80 ,-79 ,-79 ,-79 ,-78 ,-78 ,-78 ,-77 ,-77 ,-77 ,-77 ,-76 ,-76 ,-76 ,-75 ,-75 ,-75 ,-74 ,-74 ,-74 ,-74 ,-73 ,-73 ,-73 ,-72 ,-72 ,-72 ,-71 ,-71 ,-71 ,-71 ,-70 ,-70 ,-70 ,-69 ,-69 ,-69 ,-68 ,-68 ,-68 ,-68 ,-67 ,-67 ,-67 ,-66 ,-66 ,-66 ,-65 ,-65 ,-65 ,-65 ,-64 ,-64 ,-64 ,-63 ,-63 ,-63 ,-62 ,-62 ,-62 ,-62 ,-61 ,-61 ,-61 ,-60 ,-60 ,-60 ,-59 ,-59 ,-59 ,-59 ,-58 ,-58 ,-58 ,-57 ,-57 ,-57 ,-56 ,-56 ,-56 ,-55 ,-55 ,-55 ,-55 ,-54 ,-54 ,-54 ,-53 ,-53 ,-53 ,-52 ,-52 ,-52 ,-52 ,-51 ,-51 ,-51 ,-50 ,-50 ,-50 ,-49 ,-49 ,-49 ,-49 ,-48 ,-48 ,-48 ,-47 ,-47 ,-47 ,-46 ,-46 ,-46 ,-46 ,-45 ,-45 ,-45 ,-44 ,-44 ,-44 ,-43 ,-43 ,-43 ,-43 ,-42 ,-42 ,-42 ,-41 ,-41 ,-41 ,-40 ,-40 ,-40 ,-40 ,-39 ,-39 ,-39 ,-38 ,-38 ,-38 ,-37 ,-37 ,-37 ,-37 ,-36 ,-36 ,-36 ,-35 ,-35 ,-35 ,-34 ,-34 ,-34 ,-34 ,-33 ,-33 ,-33 ,-32 ,-32 ,-32 ,-31 ,-31 ,-31 ,-31 ,-30 ,-30 ,-30 ,-29 ,-29 ,-29 ,-28 ,-28 ,-28 ,-27 ,-27 ,-27 ,-26 ,-25 ,-24 ,-23 ,-22 ,-21 ,-20 ,-19 ,-18 ,-17 ,-16 ,-15 ,-14 ,-13 ,-12 ,-11 ,-10 ,-9 ,-8 ,-7 ,-6 ,-5 ,-4 ,-3 ,-2 ,-1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,5 ,5 ,5 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,12 ,12 ,12 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,17 ,17 ,17 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,24 ,24 ,24 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,29 ,29 ,29 ,30 ,30 ,30 ,31 ,31 ,31 ,32 ,32 ,32 ,32 ,33 ,33 ,33 ,34 ,34 ,34 ,35 ,35 ,35 ,35 ,36 ,36 ,36 ,37 ,37 ,37 ,38 ,38 ,38 ,38 ,39 ,39 ,39 ,40 ,40 ,40 ,41 ,41 ,41 ,41 ,42 ,42 ,42 ,43 ,43 ,43 ,44 ,44 ,44 ,44 ,45 ,45 ,45 ,46 ,46 ,46 ,47 ,47 ,47 ,47 ,48 ,48 ,48 ,49 ,49 ,49 ,50 ,50 ,50 ,50 ,51 ,51 ,51 ,52 ,52 ,52 ,53 ,53 ,53 ,53 ,54 ,54 ,54 ,55 ,55 ,55 ,56 ,56 ,56 ,56 ,57 ,57 ,57 ,58 ,58 ,58 ,59 ,59 ,59 ,60 ,60 ,60 ,60 ,61 ,61 ,61 ,62 ,62 ,62 ,63 ,63 ,63 ,63 ,64 ,64 ,64 ,65 ,65 ,65 ,66 ,66 ,66 ,66 ,67 ,67 ,67 ,68 ,68 ,68 ,69 ,69 ,69 ,69 ,70 ,70 ,70 ,71 ,71 ,71 ,72 ,72 ,72 ,72 ,73 ,73 ,73 ,74 ,74 ,74 ,75 ,75 ,75 ,75 ,76 ,76 ,76 ,77 ,77 ,77 ,78 ,78 ,78 ,78 ,79 ,79 ,79 ,80 ,80 ,80 ,81 ,81 ,81 ,81 ,82 ,82 ,82 ,83 ,83 ,83 ,84 ,84 ,84 ,84 ,85 ,85 ,85 ,86 ,86 ,86 ,87 ,87 ,87 ,87 ,88 ,88 ,88 ,89 ,89 ,89 ,90 ,90 ,90 ,91 ,91 ,91 ,91 ,92 ,92 ,92 ,93 ,93 ,93 ,94 ,94 ,94 ,94 ,95 ,95 ,95 ,96 ,96 ,96 ,97 ,97 ,97 ,97 ,98 ,98 ,98 ,99 ,99 ,99 ,100 ,100 ,100 ,100 ,101 ,101 ,101 ,102 ,102 ,102 ,103 ,103 ,103 ,103 ,104 ,104 ,104 ,105 ,105 ,105 ,106 ,106 ,106 ,106 ,107 ,107 ,107 ,108 ,108 ,108 ,109 ,109 ,109 ,109 ,110 ,110 ,110 ,111 ,111 ,111 ,112 ,112 ,112 ,112 ,113 ,113 ,113 ,114 ,114 ,114 ,115 ,115 ,115 ,115 ,116 ,116 ,116 ,117 ,117 ,117 ,118 ,118 ,118 ,119 ,119 ,119 ,119 ,120 ,120 ,120 ,121 ,121 ,121 ,122 ,122 ,122 ,122 ,123 ,123 ,123 ,124 ,124 ,124 ,125 ,125 ,125 ,125 ,126 ,126 ,126 ,127 ,127 ,127 ,128 ,128 ,128 ,128 ,129 ,129 ,129 ,130 ,130 ,130 ,131 ,131 ,131 ,131 ,132 ,132 ,132 ,133 ,133 ,133 ,134 ,134 ,134 ,134 ,135 ,135 ,135 ,136 ,136 ,136 ,137 ,137 ,137 ,137 ,138 ,138 ,138 ,139 ,139 ,139 ,140 ,140 ,140 ,140 ,141 ,141 ,141 ,142 ,142 ,142 ,143 ,143 ,143 ,143 ,144 ,144 ,144 ,145 ,145 ,145 ,146 ,146 ,146 ,146 ,147 ,147 ,147 ,148 ,148 ,148 ,149 ,149 ,149 ,150 ,150 ,150 ,150 ,151 ,151 ,151 ,152 ,152 ,152 ,153 ,153 ,153 ,153 ,154 ,154 ,154 ,155 ,155 ,155 ,156 ,156 ,156 ,156 ,157 ,157 ,157 ,158 ,158 ,158 ,159 ,159 ,159 ,159 ,160 ,160 ,160 ,161 ,161 ,161 ,162 ,162 ,162 ,162 ,163 ,163 ,163 ,164 ,164 ,164 ,165 ,165 ,165 ,165 ,166 ,166 ,166 ,167 ,167 ,167 ,168 ,168 ,168 ,168 ,169 ,169 ,169 ,170 ,170 ,170 ,171 ,171 ,171 ,171 ,172 ,172 ,172 ,173 ,173 ,173 ,174 ,174 ,174 ,174 ,175 ,175 ,175 ,176 ,176 ,176 ,177 ,177 ,177 ,178 ,178 ,178 ,178 ,179 ,179 ,179 ,180 ,180 ,180 ,181 ,181 ,181 ,181 ,182 ,182 ,182 ,183 ,183 ,183 ,184 ,184 ,184 ,184 ,185 ,185 ,185 ,186 ,186 ,186 ,187 ,187 ,187 ,187 ,188 ,188 ,188 ,189 ,189 ,189 ,190 ,190 ,190 ,190 ,191 ,191 ,191 ,192 ,192 ,192 ,193 ,193 ,193 ,193 ,194 ,194 ,194 ,195 ,195 ,195 ,196 ,196 ,196 ,196 ,197 ,197 ,197 ,198 ,198 ,198 ,199 ,199 ,199 ,199 ,200 ,200 ,200 ,201 ,201 ,201 ,202 ,202 ,202 ,202 ,203 ,203 ,203 ,204 ,204 ,204 ,205 ,205 ,205 ,206 ,206 ,206 ,206 ,207 ,207 ,207 ,208 ,208 ,208 ,209 ,209 ,209 ,209 ,210 ,210 ,210 ,211 ,211 ,211 ,212 ,212 ,212 ,212 ,213 ,213 ,213 ,214 ,214 ,214 ,215 ,215 ,215 ,215 ,216 ,216 ,216 ,217 ,217 ,217 ,218 ,218 ,218 ,218 ,219 ,219 ,219 ,220 ,220 ,220 ,221 ,221 ,221 ,221 ,222 ,222 ,222 ,223 ,223 ,223 ,224 ,224 ,224 ,224 ,225 ,225 ,225 ,226 ,226 ,226 ,227 ,227 ,227 ,227 ,228 ,228 ,228 ,229 ,229 ,229 ,230 ,230 ,230 ,230 ,231 ,231 ,231 ,232 ,232 ,232 ,233 ,233 ,233 ,233 ,234 ,234 ,234 ,235 ,235 ,235 ,236 ,236 ,236 ,237 ,237 ,237 ,237 ,238 ,238 ,238 ,239 ,239 ,239 ,240 ,240 ,240 ,240 ,241 ,241 ,241 ,242 ,242 ,242 ,243 ,243 ,243 ,243 ,244 ,244 ,244 ,245 ,245 ,245 ,246 ,246 ,246 ,246 ,247 ,247 ,247 ,248 ,248 ,248 ,249 ,249 ,249 ,249 ,250 ,250 ,250 ,251 ,251 ,251 ,252 ,252 ,252 ,252 ,253 ,253 ,253 ,254 ,254 ,254 ,255 ,255 ,255 ,255 ,256 ,256 ,256 ,257 ,257 ,257 ,258 ,258 ,258 ,258 ,259 ,259 ,259 ,260 ,260 ,260 ,261 ,261 ,261 ,261 ,262 ,262 ,262 ,263 ,263 ,263 ,264 ,264 ,264 ,265 ,265 ,265 ,265 ,266 ,266 ,266 ,267 ,267 ,267 ,268 ,268 ,268 ,268 ,269 ,269 ,269 ,270 ,270 ,270 ,271 ,271 ,271 ,271 ,272 ,272 ,272 ,273 ,273 ,273 ,274 ,274 ,274 ,274 ,275 ,275 ,275 ,276 ,276 ,276 ,277 ,277 ,277 ,277 ,278 ,278 ,278 ,279 ,279 ,279 ,280 ,280 ,280 ,280 ,281 ,281 ,281 ,282 ,282 ,282 ,283 ,283 ,283 ,283 ,284 ,284 ,284 ,285 ,285 ,285 ,286 ,286 ,286 ,286 ,287 ,287 ,287 ,288 ,288 ,288 ,289 ,289 ,289 ,289 ,290 ,290 ,290 ,291 ,291 ,291 ,292 ,292 ,292 ,292 ,293 ,293 ,293 ,294 ,294 ,294 ,295 ,295 ,295 ,296 ,296 ,296 ,296 ,297 ,297 ,297 ,298 ,298 ,298 ,299 ,299 ,299 ,299 ,300 ,300 ,300 ,301 ,301 ,301 ,302 ,302 ,302 ,302 ,303 ,303 ,303 ,304 ,304 ,304 ,305 ,305 ,305 ,305 ,306 ,306 ,306 ,307 ,307 ,307 ,308 ,308 ,308 ,308} ;
			return memorize ([&] () {
				FEXP10Cache ret ;
				ret.mMCache = BoxBuffer<DATA ,SIZE> (M_MCACHE) ;
				ret.mECache = BoxBuffer<LENGTH ,SIZE> (M_ECACHE) ;
				return move (ret) ;
			}) ;
		}

		inline NOTATION operator[] (CREF<VAL64> k) const {
			NOTATION ret ;
			ret.mRadix = 10 ;
			ret.mSign = FALSE ;
			const auto r1x = SIZE::expr / 2 + INDEX (k) ;
			assume (vbetween (r1x ,0 ,SIZE::expr)) ;
			ret.mMantissa = VAL64 (mMCache[r1x]) ;
			ret.mPrecision = 0 ;
			ret.mExponent = VAL64 (mECache[r1x]) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto FLOATPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename FLOATPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BITPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BITPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		BYTE bit_low (CREF<WORD> obj) const override {
			return BYTE (obj) ;
		}

		WORD bit_low (CREF<CHAR> obj) const override {
			return WORD (obj) ;
		}

		CHAR bit_low (CREF<DATA> obj) const override {
			return CHAR (obj) ;
		}

		BYTE bit_high (CREF<WORD> obj) const override {
			return BYTE (obj >> 8) ;
		}

		WORD bit_high (CREF<CHAR> obj) const override {
			return WORD (obj >> 16) ;
		}

		CHAR bit_high (CREF<DATA> obj) const override {
			return CHAR (obj >> 32) ;
		}

		WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const override {
			return (WORD (high) << 8) | WORD (low) ;
		}

		CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const override {
			return (CHAR (high) << 16) | CHAR (low) ;
		}

		DATA bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const override {
			return (DATA (high) << 32) | DATA (low) ;
		}

		BOOL bit_any (CREF<BYTE> base ,CREF<BYTE> mask) const override {
			return (base & mask) != BYTE (0X00) ;
		}

		BOOL bit_any (CREF<WORD> base ,CREF<WORD> mask) const override {
			return (base & mask) != WORD (0X00) ;
		}

		BOOL bit_any (CREF<CHAR> base ,CREF<CHAR> mask) const override {
			return (base & mask) != CHAR (0X00) ;
		}

		BOOL bit_any (CREF<DATA> base ,CREF<DATA> mask) const override {
			return (base & mask) != DATA (0X00) ;
		}

		BOOL bit_all (CREF<BYTE> base ,CREF<BYTE> mask) const override {
			return (base & mask) == mask ;
		}

		BOOL bit_all (CREF<WORD> base ,CREF<WORD> mask) const override {
			return (base & mask) == mask ;
		}

		BOOL bit_all (CREF<CHAR> base ,CREF<CHAR> mask) const override {
			return (base & mask) == mask ;
		}

		BOOL bit_all (CREF<DATA> base ,CREF<DATA> mask) const override {
			return (base & mask) == mask ;
		}

		DATA bit_single (CREF<LENGTH> nth) const override {
			return DATA (0X01) << nth ;
		}

		BYTE bit_reverse (CREF<BYTE> obj) const override {
			return obj ;
		}

		WORD bit_reverse (CREF<WORD> obj) const override {
			auto rax = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (obj) ;
			swap (rax[0] ,rax[1]) ;
			return bitwise[TYPEAS<WORD>::expr] (rax) ;
		}

		CHAR bit_reverse (CREF<CHAR> obj) const override {
			auto rax = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (obj) ;
			swap (rax[0] ,rax[3]) ;
			swap (rax[1] ,rax[2]) ;
			return bitwise[TYPEAS<CHAR>::expr] (rax) ;
		}

		DATA bit_reverse (CREF<DATA> obj) const override {
			auto rax = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<DATA>>>::expr] (obj) ;
			swap (rax[0] ,rax[7]) ;
			swap (rax[1] ,rax[6]) ;
			swap (rax[2] ,rax[5]) ;
			swap (rax[3] ,rax[4]) ;
			return bitwise[TYPEAS<DATA>::expr] (rax) ;
		}

		INDEX bit_find (CREF<BYTE> obj) const override {
			INDEX ret = NONE ;
			auto rax = obj ;
			while (TRUE) {
				if (rax == BYTE (0X00))
					break ;
				rax = rax >> 1 ;
				ret++ ;
			}
			return move (ret) ;
		}

		INDEX bit_find (CREF<WORD> obj) const override {
			INDEX ret = NONE ;
			auto rax = obj ;
			while (TRUE) {
				if (rax == WORD (0X00))
					break ;
				rax = rax >> 1 ;
				ret++ ;
			}
			return move (ret) ;
		}

		INDEX bit_find (CREF<CHAR> obj) const override {
			INDEX ret = NONE ;
			auto rax = obj ;
			while (TRUE) {
				if (rax == CHAR (0X00))
					break ;
				rax = rax >> 1 ;
				ret++ ;
			}
			return move (ret) ;
		}

		INDEX bit_find (CREF<DATA> obj) const override {
			INDEX ret = NONE ;
			auto rax = obj ;
			while (TRUE) {
				if (rax == DATA (0X00))
					break ;
				rax = rax >> 1 ;
				ret++ ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto BITPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BITPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait INTEGER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename INTEGER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FakeHolder = typename INTEGER_HELP<DEPEND ,ALWAYS>::FakeHolder ;

	class ImplHolder implement Holder {
	protected:
		VarBuffer<BYTE> mInteger ;

	public:
		void initialize (CREF<LENGTH> size_ ,CREF<VAL64> value_) override {
			mInteger = VarBuffer<BYTE> (size_) ;
			set (value_) ;
		}

		Integer clone () const override {
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			for (auto &&i : iter (0 ,mInteger.size ()))
				rbx[i] = mInteger[i] ;
			return factory (move (rbx)) ;
		}

		Integer factory (RREF<VarBuffer<BYTE>> that) const {
			auto rax = Box<FakeHolder> () ;
			rax.acquire (TYPEAS<ImplHolder>::expr) ;
			auto &&tmp = keep[TYPEAS<VREF<ImplHolder>>::expr] (keep[TYPEAS<VREF<Holder>>::expr] (rax.self)) ;
			tmp.mInteger = move (that) ;
			return Integer (move (rax)) ;
		}

		INDEX find_highest () const {
			for (auto &&i : iter (0 ,mInteger.size ())) {
				INDEX ix = mInteger.size () - 1 - i ;
				if (mInteger[ix] != BYTE (0X00))
					return ix ;
			}
			return NONE ;
		}

		LENGTH precision () const override {
			const auto r1x = find_highest () ;
			if (r1x == NONE)
				return 0 ;
			const auto r2x = DATA (mInteger[r1x]) ;
			const auto r3x = BitProc::bit_find (r2x) + 1 ;
			const auto r4x = r1x * 8 + r3x ;
			return r4x ;
		}

		VAL64 get () const override {
			VAL64 ret = 0 ;
			const auto r1x = vmin (mInteger.size () ,SIZE_OF<VAL64>::expr) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = DATA (mInteger[i]) << (i * 8) ;
				ret = VAL64 (DATA (ret) | r2x) ;
			}
			return move (ret) ;
		}

		void set (CREF<VAL64> value_) override {
			const auto r1x = vmin (mInteger.size () ,SIZE_OF<VAL64>::expr) ;
			const auto r2x = DATA (value_) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r3x = BYTE (r2x >> (i * 8)) ;
				mInteger[i] = r3x ;
			}
			const auto r4x = invoke ([&] () {
				if (value_ >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			BufferProc<BYTE>::buf_fill (mInteger ,r4x ,r1x ,mInteger.size ()) ;
		}

		BOOL equal (CREF<Holder> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			assert (mInteger.size () == that.mInteger.size ()) ;
			return BufferProc<BYTE>::buf_equal (mInteger ,that.mInteger ,0 ,mInteger.size ()) ;
		}

		FLAG compr (CREF<Holder> that) const override {
			return compr (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		FLAG compr (CREF<ImplHolder> that) const {
			assert (mInteger.size () == that.mInteger.size ()) ;
			return BufferProc<BYTE>::buf_compr (mInteger ,that.mInteger ,0 ,mInteger.size ()) ;
		}

		FLAG hash () const override {
			return BufferProc<BYTE>::buf_hash (mInteger ,0 ,mInteger.size ()) ;
		}

		Integer add (CREF<Holder> that) const override {
			return add (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		Integer add (CREF<ImplHolder> that) const {
			assert (mInteger.size () == that.mInteger.size ()) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) + VAL64 (that.mInteger[i]) + rax ;
				const auto r2x = DATA (r1x) ;
				rbx[i] = BYTE (r2x) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return factory (move (rbx)) ;
		}

		Integer sub (CREF<Holder> that) const override {
			return sub (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		Integer sub (CREF<ImplHolder> that) const {
			assert (mInteger.size () == that.mInteger.size ()) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) - VAL64 (that.mInteger[i]) - rax ;
				const auto r2x = VAL64 (r1x < 0) ;
				const auto r3x = r1x + 256 * r2x ;
				const auto r4x = DATA (r3x) ;
				rbx[i] = BYTE (r4x) ;
				rax = r2x ;
			}
			return factory (move (rbx)) ;
		}

		Integer mul (CREF<Holder> that) const override {
			return mul (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		Integer mul (CREF<ImplHolder> that) const {
			assert (mInteger.size () == that.mInteger.size ()) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				for (auto &&j : iter (0 ,i + 1)) {
					const auto r1x = VAL64 (mInteger[j]) * VAL64 (that.mInteger[i - j]) ;
					rax += r1x ;
				}
				const auto r2x = DATA (rax) ;
				rbx[i] = BYTE (r2x) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return factory (move (rbx)) ;
		}

		Integer mul (CREF<VAL64> that) const override {
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			const auto r1x = find_highest () + 1 ;
			BufferProc<BYTE>::buf_fill (rbx ,BYTE (0X00) ,r1x ,mInteger.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = VAL64 (mInteger[i]) * that + rax ;
				const auto r3x = DATA (r2x) ;
				rbx[i] = BYTE (r3x) ;
				rax = VAL64 (r3x >> 8) ;
			}
			INDEX ix = r1x ;
			while (TRUE) {
				if (rax == 0)
					break ;
				const auto r4x = DATA (rax) ;
				rbx[ix] = BYTE (r4x) ;
				rax = VAL64 (r4x >> 8) ;
				ix++ ;
			}
			return factory (move (rbx)) ;
		}

		Integer div (CREF<VAL64> that) const override {
			assert (that != ZERO) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			const auto r1x = find_highest () + 1 ;
			BufferProc<BYTE>::buf_fill (rbx ,BYTE (0X00) ,r1x ,mInteger.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				INDEX ix = r1x - 1 - i ;
				const auto r2x = VAL64 (mInteger[ix]) + rax ;
				const auto r3x = r2x / that ;
				const auto r4x = DATA (r3x) ;
				rbx[ix] = BYTE (r4x) ;
				const auto r5x = r2x - r3x * that ;
				rax = VAL64 (DATA (r5x) << 8) ;
			}
			return factory (move (rbx)) ;
		}

		Integer mod (CREF<VAL64> that) const override {
			assert (that != ZERO) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			const auto r1x = find_highest () + 1 ;
			BufferProc<BYTE>::buf_fill (rbx ,BYTE (0X00) ,r1x ,mInteger.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				INDEX ix = r1x - 1 - i ;
				const auto r2x = VAL64 (mInteger[ix]) + rax ;
				const auto r3x = r2x / that ;
				const auto r4x = DATA (r3x) ;
				rbx[ix] = BYTE (r4x) ;
				const auto r5x = r2x - r3x * that ;
				rax = VAL64 (DATA (r5x) << 8) ;
			}
			return Integer (rax ,mInteger.size ()) ;
		}

		Integer minus () const override {
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			for (auto &&i : iter (0 ,mInteger.size ()))
				rbx[i] = ~mInteger[i] ;
			Integer ret = factory (move (rbx)) ;
			ret++ ;
			return move (ret) ;
		}

		void increase () override {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mInteger.size ())
					break ;
				mInteger[ix] = BYTE (LENGTH (mInteger[ix]) + 1) ;
				if (mInteger[ix] != BYTE (0X00))
					break ;
				ix++ ;
			}
		}

		void decrease () override {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mInteger.size ())
					break ;
				mInteger[ix] = BYTE (LENGTH (mInteger[ix]) - 1) ;
				if (mInteger[ix] != BYTE (0XFF))
					break ;
				ix++ ;
			}
		}
	} ;
} ;

template <>
exports auto INTEGER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename INTEGER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}
} ;