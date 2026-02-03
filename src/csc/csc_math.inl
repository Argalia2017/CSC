#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
struct MathProcLayout {} ;

class MathProcImplHolder final implement Fat<MathProcHolder ,MathProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Bool is_inf (CR<Flt32> a) const override {
		return std::isinf (a) ;
	}

	Bool is_inf (CR<Flt64> a) const override {
		return std::isinf (a) ;
	}

	Bool is_low (CR<Flt32> a) const override {
		if (a < -FLT32_LOW)
			return FALSE ;
		if (a > +FLT32_LOW)
			return FALSE ;
		return TRUE ;
	}

	Bool is_low (CR<Flt64> a) const override {
		if (a < -FLT64_LOW)
			return FALSE ;
		if (a > +FLT64_LOW)
			return FALSE ;
		return TRUE ;
	}

	Val32 step (CR<Val32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	Val64 step (CR<Val64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	Flt32 step (CR<Flt32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	Flt64 step (CR<Flt64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	Val32 sign (CR<Val32> a) const override {
		if (a >= 0)
			return +1 ;
		return -1 ;
	}

	Val64 sign (CR<Val64> a) const override {
		if (a >= 0)
			return +1 ;
		return -1 ;
	}

	Flt32 sign (CR<Flt32> a) const override {
		if (a >= 0)
			return +1 ;
		return -1 ;
	}

	Flt64 sign (CR<Flt64> a) const override {
		if (a >= 0)
			return +1 ;
		return -1 ;
	}

	Val32 square (CR<Val32> a) const override {
		return a * a ;
	}

	Val64 square (CR<Val64> a) const override {
		return a * a ;
	}

	Flt32 square (CR<Flt32> a) const override {
		return a * a ;
	}

	Flt64 square (CR<Flt64> a) const override {
		return a * a ;
	}

	Flt32 sqrt (CR<Flt32> a) const override {
		return std::sqrt (a) ;
	}

	Flt64 sqrt (CR<Flt64> a) const override {
		return std::sqrt (a) ;
	}

	Val32 cubic (CR<Val32> a) const override {
		return a * a * a ;
	}

	Val64 cubic (CR<Val64> a) const override {
		return a * a * a ;
	}

	Flt32 cubic (CR<Flt32> a) const override {
		return a * a * a ;
	}

	Flt64 cubic (CR<Flt64> a) const override {
		return a * a * a ;
	}

	Flt32 cbrt (CR<Flt32> a) const override {
		return std::cbrt (a) ;
	}

	Flt64 cbrt (CR<Flt64> a) const override {
		return std::cbrt (a) ;
	}

	Flt32 pow (CR<Flt32> a ,CR<Val32> b) const override {
		return Flt32 (std::pow (a ,b)) ;
	}

	Flt64 pow (CR<Flt64> a ,CR<Val32> b) const override {
		return Flt64 (std::pow (a ,b)) ;
	}

	Flt32 hypot (CR<Flt32> a ,CR<Flt32> b) const override {
		return std::hypot (a ,b) ;
	}

	Flt64 hypot (CR<Flt64> a ,CR<Flt64> b) const override {
		return std::hypot (a ,b) ;
	}

	Val32 abs (CR<Val32> a) const override {
		if (a == VAL32_ABS)
			return 0 ;
		if (a < 0)
			return -a ;
		return a ;
	}

	Val64 abs (CR<Val64> a) const override {
		if (a == VAL64_ABS)
			return 0 ;
		if (a < 0)
			return -a ;
		return a ;
	}

	Flt32 abs (CR<Flt32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	Flt64 abs (CR<Flt64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	Flt32 inverse (CR<Flt32> a) const override {
		if (abs (a) < FLT32_EPS)
			return 0 ;
		return 1 / a ;
	}

	Flt64 inverse (CR<Flt64> a) const override {
		if (abs (a) < FLT64_EPS)
			return 0 ;
		return 1 / a ;
	}

	Flt32 floor (CR<Flt32> a ,CR<Flt32> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	Flt64 floor (CR<Flt64> a ,CR<Flt64> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	Flt32 ceil (CR<Flt32> a ,CR<Flt32> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	Flt64 ceil (CR<Flt64> a ,CR<Flt64> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	Flt32 round (CR<Flt32> a) const override {
		return std::round (a) ;
	}

	Flt64 round (CR<Flt64> a) const override {
		return std::round (a) ;
	}

	Flt32 fmod (CR<Flt32> a) const override {
		return std::fmod (a ,Flt32 (1)) ;
	}

	Flt64 fmod (CR<Flt64> a) const override {
		return std::fmod (a ,Flt64 (1)) ;
	}

	Val32 clamp (CR<Val32> a ,CR<Val32> min_ ,CR<Val32> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	Val64 clamp (CR<Val64> a ,CR<Val64> min_ ,CR<Val64> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	Flt32 clamp (CR<Flt32> a ,CR<Flt32> min_ ,CR<Flt32> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	Flt64 clamp (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const override {
		if (a <= min_)
			return min_ ;
		if (a >= max_)
			return max_ ;
		return a ;
	}

	Val32 lerp (CR<Flt64> a ,CR<Val32> min_ ,CR<Val32> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,Flt64 (1)) * 2 ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		const auto r5x = Val32 (round (r4x)) ;
		return min_ + r5x ;
	}

	Val64 lerp (CR<Flt64> a ,CR<Val64> min_ ,CR<Val64> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x / 2 ,Flt64 (1)) * 2 ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		const auto r5x = Val64 (round (r4x)) ;
		return min_ + r5x ;
	}

	Flt32 cos (CR<Flt32> a) const override {
		return std::cos (a) ;
	}

	Flt64 cos (CR<Flt64> a) const override {
		return std::cos (a) ;
	}

	Flt32 sin (CR<Flt32> a) const override {
		return std::sin (a) ;
	}

	Flt64 sin (CR<Flt64> a) const override {
		return std::sin (a) ;
	}

	Flt32 tan (CR<Flt32> a) const override {
		return std::tan (a) ;
	}

	Flt64 tan (CR<Flt64> a) const override {
		return std::tan (a) ;
	}

	Flt32 acos (CR<Flt32> a) const override {
		return std::acos (a) ;
	}

	Flt64 acos (CR<Flt64> a) const override {
		return std::acos (a) ;
	}

	Flt32 asin (CR<Flt32> a) const override {
		return std::asin (a) ;
	}

	Flt64 asin (CR<Flt64> a) const override {
		return std::asin (a) ;
	}

	Flt32 atan (CR<Flt32> y ,CR<Flt32> x) const override {
		return std::atan2 (y ,x) ;
	}

	Flt64 atan (CR<Flt64> y ,CR<Flt64> x) const override {
		return std::atan2 (y ,x) ;
	}

	Flt32 exp (CR<Flt32> a) const override {
		return std::exp (a) ;
	}

	Flt64 exp (CR<Flt64> a) const override {
		return std::exp (a) ;
	}

	Flt32 log (CR<Flt32> a) const override {
		return std::log (a) ;
	}

	Flt64 log (CR<Flt64> a) const override {
		return std::log (a) ;
	}

	Flt32 pdf (CR<Flt32> a) const override {
		const auto r1x = -square (a) * Flt32 (0.5) ;
		return exp (r1x) * Flt32 (MATH_PDF0) ;
	}

	Flt64 pdf (CR<Flt64> a) const override {
		const auto r1x = -square (a) * Flt64 (0.5) ;
		return exp (r1x) * Flt64 (MATH_PDF0) ;
	}

	Flt32 cbf (CR<Flt32> a) const override {
		const auto r1x = a * Flt32 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) * Flt32 (0.5) ;
	}

	Flt64 cbf (CR<Flt64> a) const override {
		const auto r1x = a * Flt64 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) * Flt64 (0.5) ;
	}

	Bool all_of (CR<Wrapper<Bool>> b) const override {
		for (auto &&i : range (0 ,b.rank ())) {
			if (!b[i])
				return FALSE ;
		}
		return TRUE ;
	}

	Bool any_of (CR<Wrapper<Bool>> b) const override {
		for (auto &&i : range (0 ,b.rank ())) {
			if (b[i])
				return TRUE ;
		}
		return FALSE ;
	}

	Val32 max_of (CR<Wrapper<Val32>> b) const override {
		return max_of_impl (b) ;
	}

	Val64 max_of (CR<Wrapper<Val64>> b) const override {
		return max_of_impl (b) ;
	}

	Flt32 max_of (CR<Wrapper<Flt32>> b) const override {
		return max_of_impl (b) ;
	}

	Flt64 max_of (CR<Wrapper<Flt64>> b) const override {
		return max_of_impl (b) ;
	}

	template <class ARG1>
	forceinline ARG1 max_of_impl (CR<Wrapper<ARG1>> b) const {
		assert (b.rank () > 0) ;
		ARG1 ret = b[0] ;
		for (auto &&i : range (1 ,b.rank ())) {
			if (ret >= b[i])
				continue ;
			ret = b[i] ;
		}
		return move (ret) ;
	}

	Val32 min_of (CR<Wrapper<Val32>> b) const override {
		return min_of_impl (b) ;
	}

	Val64 min_of (CR<Wrapper<Val64>> b) const override {
		return min_of_impl (b) ;
	}

	Flt32 min_of (CR<Wrapper<Flt32>> b) const override {
		return min_of_impl (b) ;
	}

	Flt64 min_of (CR<Wrapper<Flt64>> b) const override {
		return min_of_impl (b) ;
	}

	template <class ARG1>
	forceinline ARG1 min_of_impl (CR<Wrapper<ARG1>> b) const {
		assert (b.rank () > 0) ;
		ARG1 ret = b[0] ;
		for (auto &&i : range (1 ,b.rank ())) {
			if (ret <= b[i])
				continue ;
			ret = b[i] ;
		}
		return move (ret) ;
	}
} ;

exports CR<Super<Ref<MathProcLayout>>> MathProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<MathProcLayout>> ret ;
		ret.mThis = Ref<MathProcLayout>::make () ;
		MathProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<MathProcHolder> MathProcHolder::hold (VR<MathProcLayout> that) {
	return VFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::hold (CR<MathProcLayout> that) {
	return CFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

class NormalErrorImplHolder final implement Fat<NormalErrorHolder ,NormalErrorLayout> {
public:
	void concat (CR<Flt64> error) override {
		const auto r1x = Flt64 (self.mCount) ;
		const auto r2x = MathProc::inverse (r1x + 1) ;
		const auto r3x = error - self.mAvgError ;
		self.mMaxError = MathProc::max_of (self.mMaxError ,error) ;
		self.mAvgError = self.mAvgError + r3x * r2x ;
		const auto r4x = r1x * r2x * MathProc::square (self.mStdError) + r1x * MathProc::square (r3x * r2x) ;
		self.mStdError = MathProc::sqrt (r4x) ;
		self.mCount = Length (r1x + 1) ;
	}
} ;

exports VFat<NormalErrorHolder> NormalErrorHolder::hold (VR<NormalErrorLayout> that) {
	return VFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

exports CFat<NormalErrorHolder> NormalErrorHolder::hold (CR<NormalErrorLayout> that) {
	return CFat<NormalErrorHolder> (NormalErrorImplHolder () ,that) ;
}

struct FloatProcLayout {} ;

class FloatProcImplHolder final implement Fat<FloatProcHolder ,FloatProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Length value_precision () const override {
		return Length (18) ;
	}

	Length float_precision () const override {
		return Length (15) ;
	}

	Flt64 encode (CR<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		auto rax = fexp2 ;
		if ifdo (TRUE) {
			if (rax.mMantissa == 0)
				discard ;
			while (TRUE) {
				if (!ByteProc::any_bit (Quad (rax.mMantissa) ,Quad (0XFFE0000000000000)))
					break ;
				rax.mMantissa = Val64 (Quad (rax.mMantissa) >> 1) ;
				rax.mDownflow = 0 ;
				rax.mExponent++ ;
			}
			while (TRUE) {
				if (ByteProc::any_bit (Quad (rax.mMantissa) ,Quad (0XFFF0000000000000)))
					break ;
				rax.mMantissa = Val64 (Quad (rax.mMantissa) << 1) ;
				rax.mDownflow = 0 ;
				rax.mExponent-- ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = Length (-1074) - Length (rax.mExponent) ;
			if (r1x <= 0)
				discard ;
			rax.mMantissa = Val64 (Quad (rax.mMantissa) >> r1x) ;
			rax.mDownflow = 0 ;
			rax.mExponent = -1075 ;
		}
		rax.mExponent += 1075 ;
		if ifdo (TRUE) {
			if (rax.mMantissa != 0)
				discard ;
			rax.mExponent = 0 ;
		}
		const auto r2x = invoke ([&] () {
			if (!fexp2.mSign)
				return Quad (0X00) ;
			return Quad (0X8000000000000000) ;
		}) ;
		const auto r3x = (Quad (rax.mExponent) << 52) & Quad (0X7FF0000000000000) ;
		const auto r4x = Quad (rax.mMantissa) & Quad (0X000FFFFFFFFFFFFF) ;
		const auto r5x = r2x | r3x | r4x ;
		return bitwise (r5x) ;
	}

	Notation decode (CR<Flt64> float_) const override {
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		const auto r1x = Quad (bitwise (float_)) ;
		const auto r2x = r1x & Quad (0X7FF0000000000000) ;
		const auto r3x = r1x & Quad (0X000FFFFFFFFFFFFF) ;
		ret.mSign = ByteProc::any_bit (r1x ,Quad (0X8000000000000000)) ;
		ret.mMantissa = Val64 (r3x) ;
		ret.mDownflow = 0 ;
		if ifdo (TRUE) {
			if (r2x == Quad (0X00))
				discard ;
			ret.mMantissa = Val64 (Quad (ret.mMantissa) | Quad (0X0010000000000000)) ;
		}
		ret.mExponent = Val64 (r2x >> 52) ;
		ret.mExponent -= 1075 - Length (r2x == Quad (0X00)) ;
		if ifdo (TRUE) {
			if (ret.mMantissa != 0)
				discard ;
			ret.mExponent = 0 ;
		}
		if ifdo (TRUE) {
			if (ret.mMantissa == 0)
				discard ;
			while (TRUE) {
				if (ByteProc::any_bit (Quad (ret.mMantissa) ,Quad (0X0000000000000001)))
					break ;
				ret.mMantissa = Val64 (Quad (ret.mMantissa) >> 1) ;
				ret.mExponent++ ;
			}
		}
		return move (ret) ;
	}

	Notation fexp2_multiply (CR<Notation> obj1 ,CR<Notation> obj2) const {
		assert (obj1.mRadix == 2) ;
		assert (obj2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = Length (32) ;
		const auto r2x = Quad (ByteProc::pow_bit (r1x) - 1) ;
		const auto r3x = ByteProc::pow_bit (r1x - 1) ;
		const auto r4x = Val64 (Quad (obj1.mMantissa) >> r1x) ;
		const auto r5x = Val64 (Quad (obj1.mMantissa) & r2x) ;
		const auto r6x = Val64 (Quad (obj2.mMantissa) >> r1x) ;
		const auto r7x = Val64 (Quad (obj2.mMantissa) & r2x) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = r4x * r7x ;
		const auto r11x = r5x * r7x ;
		//@error: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r8x == 0)
				discard ;
			const auto r12x = Val64 (Quad (r9x) >> r1x) ;
			const auto r13x = Val64 (Quad (r9x) & r2x) ;
			const auto r14x = Val64 (Quad (r10x) >> r1x) ;
			const auto r15x = Val64 (Quad (r10x) & r2x) ;
			const auto r16x = Val64 (Quad (r11x) >> r1x) ;
			const auto r17x = Val64 (Quad (r13x + r15x + r16x + r3x) >> r1x) ;
			ret.mMantissa = r8x + r12x + r14x + r17x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r9x == 0)
				if (r10x == 0)
					discard ;
			const auto r18x = Val64 (Quad (r11x + r3x) >> r1x) ;
			ret.mMantissa = r9x + r10x + r18x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r11x ;
			ret.mDownflow = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp2_from_fexp10 (CR<Notation> fexp10) const override {
		assert (fexp10.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mPrecision = 0 ;
		ret.mSign = fexp10.mSign ;
		ret.mMantissa = fexp10.mMantissa ;
		ret.mDownflow = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP2Cache::expr[fexp10.mExponent] ;
		ret = fexp2_multiply (ret ,r1x) ;
		return move (ret) ;
	}

	Notation fexp10_downflow (CR<Notation> fexp10 ,CR<Val64> half) const {
		Notation ret = fexp10 ;
		if ifdo (TRUE) {
			if (ret.mMantissa >= 0)
				discard ;
			ret.mMantissa = Val64 (Quad (ret.mMantissa) >> 1) ;
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

	Notation fexp10_multiply (CR<Notation> obj1 ,CR<Notation> obj2) const {
		assert (obj1.mRadix == 10) ;
		assert (obj2.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = Length (9) ;
		const auto r2x = Val64 (1000000000) ;
		const auto r3x = Val64 (500000000) ;
		const auto r4x = fexp10_downflow (obj1 ,r2x) ;
		const auto r5x = fexp10_downflow (obj2 ,r2x) ;
		const auto r6x = r4x.mMantissa / r2x ;
		const auto r7x = r4x.mMantissa % r2x ;
		const auto r8x = r5x.mMantissa / r2x ;
		const auto r9x = r5x.mMantissa % r2x ;
		const auto r10x = r6x * r8x ;
		const auto r11x = r7x * r8x ;
		const auto r12x = r6x * r9x ;
		const auto r13x = r7x * r9x ;
		//@error: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r10x == 0)
				discard ;
			const auto r14x = r11x / r2x ;
			const auto r15x = r11x % r2x ;
			const auto r16x = r12x / r2x ;
			const auto r17x = r12x % r2x ;
			const auto r18x = r13x / r2x ;
			const auto r19x = (r15x + r17x + r18x + r3x) / r2x ;
			ret.mMantissa = r10x + r14x + r16x + r19x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r11x == 0)
				if (r12x == 0)
					discard ;
			const auto r20x = (r13x + r3x) / r2x ;
			ret.mMantissa = r11x + r12x + r20x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r13x ;
			ret.mDownflow = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp10_from_fexp2 (CR<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mPrecision = 0 ;
		ret.mSign = fexp2.mSign ;
		ret.mMantissa = fexp2.mMantissa ;
		ret.mDownflow = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP10Cache::expr[fexp2.mExponent] ;
		ret = fexp10_multiply (ret ,r1x) ;
		return move (ret) ;
	}
} ;

exports CR<Super<Ref<FloatProcLayout>>> FloatProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<FloatProcLayout>> ret ;
		ret.mThis = Ref<FloatProcLayout>::make () ;
		FloatProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FloatProcHolder> FloatProcHolder::hold (VR<FloatProcLayout> that) {
	return VFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

exports CFat<FloatProcHolder> FloatProcHolder::hold (CR<FloatProcLayout> that) {
	return CFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

template class External<FEXP2CacheHolder ,FEXP2CacheLayout> ;

struct FEXP2CacheLayout {} ;

exports CR<Super<UniqueRef<FEXP2CacheLayout>>> FEXP2CacheHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<FEXP2CacheLayout>> ret ;
		ret.mThis = UniqueRef<FEXP2CacheLayout>::make () ;
		FEXP2CacheHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (VR<FEXP2CacheLayout> that) {
	return VFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::expr ,that) ;
}

exports CFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (CR<FEXP2CacheLayout> that) {
	return CFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::expr ,that) ;
}

template class External<FEXP10CacheHolder ,FEXP10CacheLayout> ;

struct FEXP10CacheLayout {} ;

exports CR<Super<UniqueRef<FEXP10CacheLayout>>> FEXP10CacheHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<FEXP10CacheLayout>> ret ;
		ret.mThis = UniqueRef<FEXP10CacheLayout>::make () ;
		FEXP10CacheHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (VR<FEXP10CacheLayout> that) {
	return VFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::expr ,that) ;
}

exports CFat<FEXP10CacheHolder> FEXP10CacheHolder::hold (CR<FEXP10CacheLayout> that) {
	return CFat<FEXP10CacheHolder> (External<FEXP10CacheHolder ,FEXP10CacheLayout>::expr ,that) ;
}

struct ByteProcLayout {} ;

class ByteProcImplHolder final implement Fat<ByteProcHolder ,ByteProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Byte split_low (CR<Word> a) const override {
		return Byte (a) ;
	}

	Word split_low (CR<Char> a) const override {
		return Word (a) ;
	}

	Char split_low (CR<Quad> a) const override {
		return Char (a) ;
	}

	Byte split_high (CR<Word> a) const override {
		return Byte (a >> 8) ;
	}

	Word split_high (CR<Char> a) const override {
		return Word (a >> 16) ;
	}

	Char split_high (CR<Quad> a) const override {
		return Char (a >> 32) ;
	}

	Word merge (CR<Byte> high_ ,CR<Byte> low_) const override {
		return (Word (high_) << 8) | Word (low_) ;
	}

	Char merge (CR<Word> high_ ,CR<Word> low_) const override {
		return (Char (high_) << 16) | Char (low_) ;
	}

	Quad merge (CR<Char> high_ ,CR<Char> low_) const override {
		return (Quad (high_) << 32) | Quad (low_) ;
	}

	Byte reverse (CR<Byte> a) const override {
		return a ;
	}

	Word reverse (CR<Word> a) const override {
		Word ret = a ;
		ret = ((ret & Word (0X00FF)) << 8) | ((ret & Word (0XFF00)) >> 8) ;
		return move (ret) ;
	}

	Char reverse (CR<Char> a) const override {
		Char ret = a ;
		ret = ((ret & Char (0X00FF00FF)) << 8) | ((ret & Char (0XFF00FF00)) >> 8) ;
		ret = ((ret & Char (0X0000FFFF)) << 16) | ((ret & Char (0XFFFF0000)) >> 16) ;
		return move (ret) ;
	}

	Quad reverse (CR<Quad> a) const override {
		Quad ret = a ;
		ret = ((ret & Quad (0X00FF00FF00FF00FF)) << 8) | ((ret & Quad (0XFF00FF00FF00FF00)) >> 8) ;
		ret = ((ret & Quad (0X0000FFFF0000FFFF)) << 16) | ((ret & Quad (0XFFFF0000FFFF0000)) >> 16) ;
		ret = ((ret & Quad (0X00000000FFFFFFFF)) << 32) | ((ret & Quad (0XFFFFFFFF00000000)) >> 32) ;
		return move (ret) ;
	}

	Bool any_bit (CR<Byte> a ,CR<Byte> mask) const override {
		return (a & mask) != Byte (0X00) ;
	}

	Bool any_bit (CR<Word> a ,CR<Word> mask) const override {
		return (a & mask) != Word (0X00) ;
	}

	Bool any_bit (CR<Char> a ,CR<Char> mask) const override {
		return (a & mask) != Char (0X00) ;
	}

	Bool any_bit (CR<Quad> a ,CR<Quad> mask) const override {
		return (a & mask) != Quad (0X00) ;
	}

	Bool all_bit (CR<Byte> a ,CR<Byte> mask) const override {
		return (a & mask) == mask ;
	}

	Bool all_bit (CR<Word> a ,CR<Word> mask) const override {
		return (a & mask) == mask ;
	}

	Bool all_bit (CR<Char> a ,CR<Char> mask) const override {
		return (a & mask) == mask ;
	}

	Bool all_bit (CR<Quad> a ,CR<Quad> mask) const override {
		return (a & mask) == mask ;
	}

	Byte binary (CR<Byte> a) const override {
		if (a == Byte (0X00))
			return Byte (0X00) ;
		return ~Byte (0X00) ;
	}

	Word binary (CR<Word> a) const override {
		if (a == Word (0X00))
			return Word (0X00) ;
		return ~Word (0X00) ;
	}

	Char binary (CR<Char> a) const override {
		if (a == Char (0X00))
			return Char (0X00) ;
		return ~Char (0X00) ;
	}

	Quad binary (CR<Quad> a) const override {
		if (a == Quad (0X00))
			return Quad (0X00) ;
		return ~Quad (0X00) ;
	}

	Index pow_bit (CR<Length> nth) const override {
		return Index (Quad (0X01) << nth) ;
	}

	Length popcount (CR<Byte> a) const override {
		static const ARR<Val32 ,ENUM<256>> mCache {
			0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8} ;
		return Length (mCache[Index (a)]) ;
	}

	Length lowcount (CR<Byte> a) const override {
		static const ARR<Val32 ,ENUM<256>> mCache {
			8 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			7 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0} ;
		return Length (mCache[Index (a)]) ;
	}
} ;

exports CR<Super<Ref<ByteProcLayout>>> ByteProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<ByteProcLayout>> ret ;
		ret.mThis = Ref<ByteProcLayout>::make () ;
		ByteProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ByteProcHolder> ByteProcHolder::hold (VR<ByteProcLayout> that) {
	return VFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

exports CFat<ByteProcHolder> ByteProcHolder::hold (CR<ByteProcLayout> that) {
	return CFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

class IntegerImplHolder final implement Fat<IntegerHolder ,IntegerLayout> {
private:
	using INTEGER_MIN_SIZE = RANK8 ;

public:
	void initialize (CR<Length> size_) override {
		const auto r1x = inline_alignas (size_ ,INTEGER_MIN_SIZE::expr) + INTEGER_MIN_SIZE::expr ;
		self.mInteger = RefBuffer<Byte> (r1x) ;
		self.mWidth = size_ ;
	}

	void initialize (CR<IntegerLayout> that) override {
		self.mInteger = RefBuffer<Byte> (that.mWidth) ;
		for (auto &&i : range (0 ,that.mWidth))
			self.mInteger[i] = that.mInteger[i] ;
	}

	static Byte get (CR<IntegerLayout> that ,CR<Index> index) {
		Index ix = MathProc::min_of (index ,that.mWidth - 1) ;
		return that.mInteger[ix] ;
	}

	Length size () const override {
		if (!self.mInteger.exist ())
			return 0 ;
		return self.mWidth ;
	}

	Val64 fetch () const override {
		auto rax = Quad (0X00) ;
		const auto r1x = SIZE_OF<Val64>::expr ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Quad (get (self ,i)) << (i * 8) ;
			rax |= r2x ;
		}
		return Val64 (rax) ;
	}

	void store (CR<Val64> item) override {
		const auto r1x = SIZE_OF<Val64>::expr ;
		assert (self.mInteger.size () >= r1x) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Quad (item) >> (i * 8) ;
			self.mInteger[i] = Byte (r2x) ;
		}
		self.mWidth = r1x ;
		check_mask (self) ;
	}

	Bool equal (CR<IntegerLayout> that) const override {
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			const auto r2x = inline_equal (get (self ,ix) ,get (that ,ix)) ;
			if (!r2x)
				return r2x ;
		}
		return TRUE ;
	}

	Flag compr (CR<IntegerLayout> that) const override {
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		const auto r2x = -inline_compr (get (self ,r1x) ,get (that ,r1x)) ;
		if (r2x != ZERO)
			return r2x ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			const auto r3x = inline_compr (get (self ,ix) ,get (that ,ix)) ;
			if (r3x != ZERO)
				return r3x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = self.mWidth ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = get (self ,i) ;
			visitor.push (r2x) ;
		}
		visitor.leave () ;
	}

	IntegerLayout sadd (CR<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		auto rax = Val32 (0) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Val32 (get (self ,i)) + Val32 (get (that ,i)) + rax ;
			ret.mInteger[i] = Byte (r2x) ;
			rax = Val32 (Char (r2x) >> 8) ;
		}
		ret.mWidth = r1x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout ssub (CR<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth ,that.mWidth) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		auto rax = Val32 (0) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Val32 (get (self ,i)) - Val32 (get (that ,i)) - rax ;
			rax = Val32 (r2x < 0) ;
			const auto r3x = r2x + 256 * rax ;
			ret.mInteger[i] = Byte (r3x) ;
		}
		ret.mWidth = r1x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout smul (CR<IntegerLayout> that) const override {
		IntegerLayout ret ;
		const auto r1x = inline_max (self.mWidth + that.mWidth - 2 ,1) ;
		IntegerHolder::hold (ret)->initialize (r1x) ;
		inline_memset (Pointer::from (ret.mInteger.ref) ,r1x) ;
		for (auto &&i : range (0 ,r1x)) {
			auto rax = Val32 (0) ;
			for (auto &&j : range (0 ,r1x)) {
				Index iy = i + j ;
				if (iy >= r1x)
					continue ;
				const auto r2x = Val32 (get (self ,i)) * Val32 (get (that ,j)) + rax ;
				const auto r3x = r2x + Val32 (ret.mInteger[iy]) ;
				ret.mInteger[iy] = Byte (r3x) ;
				rax = Val32 (Char (r3x) >> 8) ;
			}
		}
		Index ix = r1x ;
		if ifdo (TRUE) {
			const auto r4x = invoke ([&] () {
				for (auto &&i : range (0 ,ix))
					if (ret.mInteger[i] != Byte (0X00))
						return i ;
				return ix ;
			}) ;
			if (r4x == ix)
				discard ;
			const auto r5x = get (self ,ix) ^ get (that ,ix) ;
			ret.mInteger[ix] = r5x ;
			ix++ ;
		}
		ret.mWidth = ix ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout sdiv (CR<IntegerLayout> that) const override {
		IntegerLayout ret ;
		auto rax = IntegerLayout () ;
		const auto r1x = IntegerHolder::hold (self)->compr (Integer::zero ()) ;
		const auto r2x = IntegerHolder::hold (that)->compr (Integer::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 0)
				discard ;
			//@info: Integer division by zero
			assume (FALSE) ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x < 0)
				discard ;
			sdiv_abs (ret ,rax ,self ,that) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x < 0)
				discard ;
			const auto r3x = IntegerHolder::hold (self)->minus () ;
			sdiv_abs (ret ,rax ,r3x ,that) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r4x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (ret ,rax ,self ,r4x) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r5x = IntegerHolder::hold (self)->minus () ;
			const auto r6x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (ret ,rax ,r5x ,r6x) ;
		}
		return move (ret) ;
	}

	IntegerLayout smod (CR<IntegerLayout> that) const override {
		IntegerLayout ret ;
		auto rax = IntegerLayout () ;
		const auto r1x = IntegerHolder::hold (self)->compr (Integer::zero ()) ;
		const auto r2x = IntegerHolder::hold (that)->compr (Integer::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 0)
				discard ;
			//@info: Integer division by zero
			assume (FALSE) ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x < 0)
				discard ;
			sdiv_abs (rax ,ret ,self ,that) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x < 0)
				discard ;
			const auto r3x = IntegerHolder::hold (self)->minus () ;
			sdiv_abs (rax ,ret ,r3x ,that) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		if ifdo (act) {
			if (r1x < 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r4x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (rax ,ret ,self ,r4x) ;
		}
		if ifdo (act) {
			if (r1x >= 0)
				discard ;
			if (r2x >= 0)
				discard ;
			const auto r5x = IntegerHolder::hold (self)->minus () ;
			const auto r6x = IntegerHolder::hold (that)->minus () ;
			sdiv_abs (rax ,ret ,r5x ,r6x) ;
			ret = IntegerHolder::hold (ret)->minus () ;
		}
		return move (ret) ;
	}

	void sdiv_abs (VR<IntegerLayout> quotient ,VR<IntegerLayout> remainder ,CR<IntegerLayout> dividend ,CR<IntegerLayout> divisor) const {
		const auto r1x = inline_max (dividend.mWidth ,divisor.mWidth) ;
		const auto r2x = r1x + 1 ;
		IntegerHolder::hold (quotient)->initialize (r2x) ;
		inline_memset (Pointer::from (quotient.mInteger.ref) ,r2x) ;
		IntegerHolder::hold (remainder)->initialize (r2x) ;
		inline_memset (Pointer::from (remainder.mInteger.ref) ,r2x) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			for (auto &&j : range (0 ,8)) {
				Index jx = 8 - 1 - j ;
				const auto r3x = ByteProc::pow_bit (jx) ;
				for (auto &&k : range (0 ,r1x - 1)) {
					Index kx = r1x - 1 - k ;
					remainder.mInteger[kx] = (remainder.mInteger[kx] << 1) | (remainder.mInteger[kx - 1] >> 7) ;
				}
				if ifdo (TRUE) {
					if (r1x == 0)
						discard ;
					Index kx = 0 ;
					remainder.mInteger[kx] = remainder.mInteger[kx] << 1 ;
				}
				remainder.mInteger[0] |= (get (dividend ,ix) >> jx) & Byte (0X01) ;
				const auto r4x = IntegerHolder::hold (remainder)->compr (Integer::zero ()) ;
				assert (r4x >= 0) ;
				auto act = TRUE ;
				if ifdo (act) {
					const auto r5x = IntegerHolder::hold (remainder)->compr (divisor) ;
					if (r5x < ZERO)
						discard ;
					auto rax = Val32 (0) ;
					for (auto &&k : range (0 ,r1x)) {
						const auto r6x = Val32 (get (remainder ,k)) - Val32 (get (divisor ,k)) - rax ;
						rax = Val32 (r6x < 0) ;
						const auto r7x = r6x + 256 * rax ;
						remainder.mInteger[k] = Byte (r7x) ;
					}
					quotient.mInteger[ix] |= Byte (r3x) ;
				}
				if ifdo (act) {
					quotient.mInteger[ix] &= ~Byte (r3x) ;
				}
			}
		}
		check_mask (quotient) ;
		check_mask (remainder) ;
	}

	IntegerLayout sabs () const override {
		if (get (self ,self.mWidth - 1) == Byte (0XFF))
			return minus () ;
		IntegerLayout ret ;
		IntegerHolder::hold (ret)->initialize (self) ;
		return move (ret) ;
	}

	IntegerLayout minus () const override {
		IntegerLayout ret ;
		IntegerHolder::hold (ret)->initialize (self.mWidth) ;
		for (auto &&i : range (0 ,self.mWidth))
			ret.mInteger[i] = ~get (self ,i) ;
		IntegerHolder::hold (ret)->increase () ;
		return move (ret) ;
	}

	IntegerLayout lshift (CR<Length> scale) const override {
		assert (scale >= 0) ;
		assert (self.mWidth > 0) ;
		IntegerLayout ret ;
		const auto r1x = scale / 8 ;
		const auto r2x = scale % 8 ;
		const auto r3x = 8 - r2x ;
		const auto r4x = self.mWidth + r1x + 1 ;
		IntegerHolder::hold (ret)->initialize (r4x) ;
		for (auto &&i : range (0 ,r4x - r1x - 1)) {
			Index ix = r4x - 1 - i ;
			const auto r5x = get (self ,ix - r1x) ;
			const auto r6x = get (self ,ix - r1x - 1) ;
			ret.mInteger[ix] = (r5x << r2x) | (r6x >> r3x) ;
		}
		const auto r7x = Byte (0X00) ;
		if ifdo (TRUE) {
			if (r4x - r1x <= 0)
				discard ;
			Index ix = r1x ;
			const auto r8x = get (self ,ix - r1x) ;
			ret.mInteger[ix] = (r8x << r2x) | (r7x >> r3x) ;
		}
		const auto r9x = inline_max (r4x - r1x ,0) ;
		for (auto &&i : range (r9x ,r4x)) {
			Index ix = r4x - 1 - i ;
			ret.mInteger[ix] = r7x ;
		}
		ret.mWidth = r4x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	IntegerLayout rshift (CR<Length> scale) const override {
		assert (scale >= 0) ;
		IntegerLayout ret ;
		const auto r1x = scale / 8 ;
		const auto r2x = scale % 8 ;
		const auto r3x = 8 - r2x ;
		const auto r4x = self.mWidth ;
		IntegerHolder::hold (ret)->initialize (r4x) ;
		for (auto &&i : range (0 ,r4x - r1x - 1)) {
			const auto r5x = get (self ,i + r1x) ;
			const auto r6x = get (self ,i + r1x + 1) ;
			ret.mInteger[i] = (r5x >> r2x) | (r6x << r3x) ;
		}
		const auto r7x = get (self ,r4x) ;
		if ifdo (TRUE) {
			if (r4x - r1x <= 0)
				discard ;
			Index ix = r4x - r1x - 1 ;
			const auto r8x = get (self ,ix + r1x) ;
			ret.mInteger[ix] = (r8x >> r2x) | (r7x << r3x) ;
		}
		const auto r9x = inline_max (r4x - r1x ,0) ;
		for (auto &&i : range (r9x ,r4x)) {
			ret.mInteger[i] = r7x ;
		}
		ret.mWidth = r4x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	void increase () override {
		const auto r1x = self.mWidth ;
		Index ix = 0 ;
		while (TRUE) {
			if (ix >= r1x)
				break ;
			const auto r2x = Val32 (get (self ,ix)) + 1 ;
			self.mInteger[ix] = Byte (r2x) ;
			if (get (self ,ix) != Byte (0X00))
				break ;
			ix++ ;
		}
		check_mask (self) ;
	}

	void decrease () override {
		const auto r1x = self.mWidth ;
		Index ix = 0 ;
		while (TRUE) {
			if (ix >= r1x)
				break ;
			const auto r2x = Val32 (get (self ,ix)) - 1 ;
			self.mInteger[ix] = Byte (r2x) ;
			if (get (self ,ix) != Byte (0XFF))
				break ;
			ix++ ;
		}
		check_mask (self) ;
	}

	static void check_mask (VR<IntegerLayout> that) {
		if ifdo (TRUE) {
			Index ix = that.mWidth - 1 ;
			const auto r1x = that.mInteger[ix] ;
			if (r1x == Byte (0X00))
				discard ;
			if (r1x == Byte (0XFF))
				discard ;
			if ifdo (TRUE) {
				if (ix + 1 < that.mInteger.size ())
					discard ;
				const auto r2x = that.mInteger.size () + INTEGER_MIN_SIZE::expr ;
				that.mInteger.resize (r2x) ;
			}
			ix++ ;
			that.mInteger[ix] = ByteProc::binary (r1x & Byte (0X80)) ;
			ix++ ;
			that.mWidth = ix ;
		}
		if ifdo (TRUE) {
			Index ix = that.mWidth - 1 ;
			const auto r3x = that.mInteger[ix] ;
			ix-- ;
			while (TRUE) {
				if (ix < 0)
					break ;
				if (that.mInteger[ix] != r3x)
					break ;
				ix-- ;
			}
			ix++ ;
			ix++ ;
			that.mWidth = ix ;
		}
	}
} ;

exports VFat<IntegerHolder> IntegerHolder::hold (VR<IntegerLayout> that) {
	return VFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

exports CFat<IntegerHolder> IntegerHolder::hold (CR<IntegerLayout> that) {
	return CFat<IntegerHolder> (IntegerImplHolder () ,that) ;
}

struct JetNode ;
using JetEvalFunction = Function<VR<JetNode> ,CR<Wrapper<Flt64>>> ;

struct JetNode {
	Flt64 mFX ;
	Flt64 mEX ;
	RefBuffer<Flt64> mDX ;
	Index mSlot ;
	JetEvalFunction mEval ;
	Index mP1 ;
	Index mP2 ;
} ;

struct JetTree {
	Allocator<JetNode ,AllocatorNode> mTree ;
} ;

class JetImplHolder final implement Fat<JetHolder ,JetLayout> {
public:
	void initialize (CR<Length> size_ ,CR<Flt64> item) override {
		assert (size_ > 0) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (root_ptr ().exist ())
				discard ;
			self.mThis = SharedRef<JetTree>::make () ;
			root_ptr () = self.mThis.weak () ;
		}
		if ifdo (act) {
			self.mThis = SharedRef<JetTree> (root_ptr ().ref) ;
		}
		self.mIndex = self.mThis->mTree.alloc (Box<JetNode>::make ()) ;
		ptr (self.mIndex).mFX = item ;
		ptr (self.mIndex).mEX = 0 ;
		ptr (self.mIndex).mDX = RefBuffer<Flt64> (size_) ;
		inline_memset (Pointer::from (ptr (self.mIndex).mDX.ref) ,ptr (self.mIndex).mDX.size () * SIZE_OF<Flt64>::expr) ;
		ptr (self.mIndex).mSlot = NONE ;
		ptr (self.mIndex).mP1 = NONE ;
		ptr (self.mIndex).mP2 = NONE ;
	}

	void initialize (CR<Length> size_ ,CR<Flt64> item ,CR<Index> slot) override {
		assert (inline_between (slot ,0 ,size_)) ;
		initialize (size_ ,item) ;
		ptr (self.mIndex).mSlot = slot ;
		ptr (self.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			if (node.mSlot == NONE)
				return ;
			assume (node.mSlot < params.rank ()) ;
			node.mFX = params[node.mSlot] ;
			node.mEX = 0 ;
			inline_memset (Pointer::from (node.mDX.ref) ,node.mDX.size () * SIZE_OF<Flt64>::expr) ;
			node.mDX[node.mSlot] = 1 ;
			check_fx (node) ;
		}) ;
	}

	static VR<JetNode> ptr (CR<Index> curr) {
		return root_ptr ()->mTree[curr] ;
	}

	static VR<SharedRef<JetTree>> root_ptr () {
		static auto mInstance = SharedRef<JetTree> () ;
		return mInstance ;
	}

	Flt64 fx () const override {
		return ptr (self.mIndex).mFX ;
	}

	Flt64 ex () const override {
		return ptr (self.mIndex).mEX ;
	}

	Flt64 dx (CR<Index> slot) const override {
		return ptr (self.mIndex).mDX[slot] ;
	}

	void once (CR<Wrapper<Flt64>> params) const override {
		once (self.mIndex ,params) ;
	}

	void once (CR<Index> curr ,CR<Wrapper<Flt64>> params) const {
		if (curr == NONE)
			return ;
		once (ptr (curr).mP1 ,params) ;
		once (ptr (curr).mP2 ,params) ;
		ptr (curr).mEval (ptr (curr) ,params) ;
	}

	JetLayout sadd (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (ptr (node.mP1).mEX != ptr (node.mP2).mEX)
					discard ;
				node.mFX = ptr (node.mP1).mFX + ptr (node.mP2).mFX ;
				node.mEX = ptr (node.mP1).mEX ;
				for (auto &&i : range (0 ,node.mDX.size ()))
					node.mDX[i] = ptr (node.mP1).mDX[i] + ptr (node.mP2).mDX[i] ;
			}
			if ifdo (act) {
				if (ptr (node.mP1).mEX < ptr (node.mP2).mEX)
					discard ;
				copy_node (node ,ptr (node.mP1) ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,ptr (node.mP2) ,+1) ;
			}
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout ssub (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (ptr (node.mP1).mEX != ptr (node.mP2).mEX)
					discard ;
				node.mFX = ptr (node.mP1).mFX - ptr (node.mP2).mFX ;
				node.mEX = ptr (node.mP1).mEX ;
				for (auto &&i : range (0 ,node.mDX.size ()))
					node.mDX[i] = ptr (node.mP1).mDX[i] - ptr (node.mP2).mDX[i] ;
			}
			if ifdo (act) {
				if (ptr (node.mP1).mEX < ptr (node.mP2).mEX)
					discard ;
				copy_node (node ,ptr (node.mP1) ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,ptr (node.mP2) ,-1) ;
			}
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout smul (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			node.mFX = ptr (node.mP1).mFX * ptr (node.mP2).mFX ;
			node.mEX = round_ex (ptr (node.mP1).mEX + ptr (node.mP2).mEX) ;
			const auto r1x = ptr (node.mP2).mFX ;
			const auto r2x = ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] + r2x * ptr (node.mP2).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout sdiv (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			const auto r1x = 1 / ptr (node.mP2).mFX ;
			node.mFX = ptr (node.mP1).mFX * r1x ;
			node.mEX = round_ex (ptr (node.mP1).mEX - ptr (node.mP2).mEX) ;
			const auto r2x = MathProc::square (r1x) ;
			const auto r3x = r2x * ptr (node.mP2).mFX ;
			const auto r4x = -r2x * ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r3x * ptr (node.mP1).mDX[i] + r4x * ptr (node.mP2).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout inverse () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			const auto r1x = 1 / ptr (node.mP1).mFX ;
			node.mFX = r1x ;
			node.mEX = -ptr (node.mP1).mEX ;
			const auto r2x = -MathProc::square (r1x) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout ssqrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mFX >= 0) ;
			node.mFX = MathProc::sqrt (ptr (node.mP1).mFX) ;
			node.mEX = round_ex (ptr (node.mP1).mEX / 2) ;
			const auto r1x = 1 / (2 * node.mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout scbrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			node.mFX = MathProc::cbrt (ptr (node.mP1).mFX) ;
			node.mEX = round_ex (ptr (node.mP1).mEX / 3) ;
			const auto r1x = 1 / (3 * MathProc::square (node.mFX)) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout spow (CR<Val32> that) const override {
		JetLayout ret ;
		auto rax = JetLayout () ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		JetHolder::hold (rax)->initialize (ptr (self.mIndex).mDX.size () ,that) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			const auto r1x = ptr (node.mP2).mFX ;
			const auto r2x = Val32 (MathProc::round (r1x - 1)) ;
			const auto r3x = MathProc::pow (ptr (node.mP1).mFX ,r2x) ;
			node.mFX = r3x * ptr (node.mP1).mFX ;
			node.mEX = round_ex (ptr (node.mP1).mEX * r1x) ;
			const auto r4x = r3x * r1x ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r4x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = rax.mIndex ;
		return move (ret) ;
	}

	JetLayout shypot (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (ptr (node.mP1).mEX != ptr (node.mP2).mEX)
					discard ;
				node.mFX = MathProc::hypot (ptr (node.mP1).mFX ,ptr (node.mP2).mFX) ;
				node.mEX = ptr (node.mP1).mEX ;
				const auto r1x = 1 / node.mFX ;
				const auto r2x = r1x * ptr (node.mP1).mFX ;
				const auto r3x = r1x * ptr (node.mP2).mFX ;
				for (auto &&i : range (0 ,node.mDX.size ()))
					node.mDX[i] = r2x * ptr (node.mP1).mDX[i] + r3x * ptr (node.mP2).mDX[i] ;
			}
			if ifdo (act) {
				if (ptr (node.mP1).mEX < ptr (node.mP2).mEX)
					discard ;
				copy_node (node ,ptr (node.mP1) ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,ptr (node.mP2) ,+1) ;
			}
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout sabs () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (ptr (node.mP1).mFX >= 0)
					discard ;
				copy_node (node ,ptr (node.mP1) ,+1) ;
			}
			if ifdo (act) {
				copy_node (node ,ptr (node.mP1) ,-1) ;
			}
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout minus () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			copy_node (node ,ptr (node.mP1) ,-1) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout ssin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::sin (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = MathProc::cos (ptr (node.mP1).mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout scos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::cos (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = -MathProc::sin (ptr (node.mP1).mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout stan () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::tan (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = 1 + MathProc::square (node.mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout sasin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::asin (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = MathProc::sqrt (1 - MathProc::square (ptr (node.mP1).mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout sacos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::acos (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = -MathProc::sqrt (1 - MathProc::square (ptr (node.mP1).mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout satan (CR<JetLayout> that) const override {
		assert (ptr (self.mIndex).mDX.size () == ptr (that.mIndex).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			assume (ptr (node.mP2).mEX == 0) ;
			node.mFX = MathProc::atan (ptr (node.mP1).mFX ,ptr (node.mP2).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = MathProc::square (ptr (node.mP1).mFX) + MathProc::square (ptr (node.mP2).mFX) ;
			const auto r2x = 1 / r1x ;
			const auto r3x = -r2x * ptr (node.mP2).mFX ;
			const auto r4x = r2x * ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r3x * ptr (node.mP1).mDX[i] + r4x * ptr (node.mP2).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		ptr (ret.mIndex).mP2 = that.mIndex ;
		return move (ret) ;
	}

	JetLayout sexp () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			const auto r1x = MathProc::exp (ptr (node.mP1).mFX) ;
			node.mFX = r1x ;
			node.mEX = ptr (node.mP1).mEX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout slog () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mFX >= 0) ;
			node.mFX = MathProc::log (ptr (node.mP1).mFX) ;
			const auto r1x = MathProc::step (MathProc::abs (ptr (node.mP1).mEX)) ;
			node.mEX = round_ex (r1x) ;
			const auto r2x = 1 / ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	JetLayout relu () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self.mIndex).mDX.size () ,0) ;
		ptr (ret.mIndex).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			auto act = TRUE ;
			if ifdo (act) {
				if (ptr (node.mP1).mEX < 0)
					discard ;
				const auto r1x = MathProc::step (ptr (node.mP1).mFX) ;
				node.mFX = r1x * ptr (node.mP1).mFX ;
				node.mEX = r1x * MathProc::step (ptr (node.mP1).mEX) ;
				for (auto &&i : range (0 ,node.mDX.size ()))
					node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			}
			if ifdo (act) {
				if (ptr (node.mP1).mEX >= 0)
					discard ;
				node.mFX = 0 ;
				node.mEX = 0 ;
				for (auto &&i : range (0 ,node.mDX.size ()))
					node.mDX[i] = 0 ;
			}
			check_fx (node) ;
		}) ;
		ptr (ret.mIndex).mP1 = self.mIndex ;
		return move (ret) ;
	}

	static void copy_node (VR<JetNode> dst ,CR<JetNode> src ,CR<Flt64> si) {
		dst.mFX = si * src.mFX ;
		dst.mEX = src.mEX ;
		for (auto &&i : range (0 ,dst.mDX.size ()))
			dst.mDX[i] = si * src.mDX[i] ;
	}

	static Flt64 round_ex (CR<Flt64> ex) {
		const auto r1x = MathProc::floor (ex + Flt64 (0.005) ,Flt64 (0.01)) ;
		const auto r2x = MathProc::inverse (ex - r1x) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	static void check_fx (VR<JetNode> node) {
		auto act = TRUE ;
		if ifdo (act) {
			if (!MathProc::is_low (node.mFX))
				discard ;
			node.mFX = 1 ;
			node.mEX-- ;
			inline_memset (Pointer::from (node.mDX.ref) ,node.mDX.size () * SIZE_OF<Flt64>::expr) ;
		}
		if ifdo (act) {
			const auto r1x = 1 / node.mFX ;
			if (!MathProc::is_low (r1x))
				discard ;
			node.mFX = 1 ;
			node.mEX++ ;
			inline_memset (Pointer::from (node.mDX.ref) ,node.mDX.size () * SIZE_OF<Flt64>::expr) ;
		}
	}
} ;

exports VFat<JetHolder> JetHolder::hold (VR<JetLayout> that) {
	return VFat<JetHolder> (JetImplHolder () ,that) ;
}

exports CFat<JetHolder> JetHolder::hold (CR<JetLayout> that) {
	return CFat<JetHolder> (JetImplHolder () ,that) ;
}

struct HashProcLayout {} ;

class HashProcImplHolder final implement Fat<HashProcHolder ,HashProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Char fnvhash32 (CR<Pointer> src ,CR<Length> size_) const override {
		return fnvhash32 (src ,size_ ,Char (2166136261UL)) ;
	}

	Char fnvhash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) const override {
		Char ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			ret ^= Char (rax[i]) ;
			ret = Char (Val32 (ret) * Val32 (16777619UL)) ;
		}
		return move (ret) ;
	}

	Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_) const override {
		return fnvhash64 (src ,size_ ,Quad (14695981039346656037ULL)) ;
	}

	Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_ ,CR<Quad> val) const override {
		Quad ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			ret ^= Quad (rax[i]) ;
			ret = Quad (Val64 (ret) * Val64 (1099511628211ULL)) ;
		}
		return move (ret) ;
	}

	Byte crchash8 (CR<Pointer> src ,CR<Length> size_) const override {
		return crchash8 (src ,size_ ,Byte (0X00)) ;
	}

	Byte crchash8 (CR<Pointer> src ,CR<Length> size_ ,CR<Byte> val) const override {
		static const ARR<csc_uint8_t ,ENUM<256>> mCache {
			0X00 ,0X5E ,0XBC ,0XE2 ,0X61 ,0X3F ,0XDD ,0X83 ,0XC2 ,0X9C ,0X7E ,0X20 ,0XA3 ,0XFD ,0X1F ,0X41 ,0X9D ,0XC3 ,0X21 ,0X7F ,0XFC ,0XA2 ,0X40 ,0X1E ,0X5F ,0X01 ,0XE3 ,0XBD ,0X3E ,0X60 ,0X82 ,0XDC ,0X23 ,0X7D ,0X9F ,0XC1 ,0X42 ,0X1C ,0XFE ,0XA0 ,0XE1 ,0XBF ,0X5D ,0X03 ,0X80 ,0XDE ,0X3C ,0X62 ,0XBE ,0XE0 ,0X02 ,0X5C ,0XDF ,0X81 ,0X63 ,0X3D ,0X7C ,0X22 ,0XC0 ,0X9E ,0X1D ,0X43 ,0XA1 ,0XFF ,0X46 ,0X18 ,0XFA ,0XA4 ,0X27 ,0X79 ,0X9B ,0XC5 ,0X84 ,0XDA ,0X38 ,0X66 ,0XE5 ,0XBB ,0X59 ,0X07 ,0XDB ,0X85 ,0X67 ,0X39 ,0XBA ,0XE4 ,0X06 ,0X58 ,0X19 ,0X47 ,0XA5 ,0XFB ,0X78 ,0X26 ,0XC4 ,0X9A ,0X65 ,0X3B ,0XD9 ,0X87 ,0X04 ,0X5A ,0XB8 ,0XE6 ,0XA7 ,0XF9 ,0X1B ,0X45 ,0XC6 ,0X98 ,0X7A ,0X24 ,0XF8 ,0XA6 ,0X44 ,0X1A ,0X99 ,0XC7 ,0X25 ,0X7B ,0X3A ,0X64 ,0X86 ,0XD8 ,0X5B ,0X05 ,0XE7 ,0XB9 ,0X8C ,0XD2 ,0X30 ,0X6E ,0XED ,0XB3 ,0X51 ,0X0F ,0X4E ,0X10 ,0XF2 ,0XAC ,0X2F ,0X71 ,0X93 ,0XCD ,0X11 ,0X4F ,0XAD ,0XF3 ,0X70 ,0X2E ,0XCC ,0X92 ,0XD3 ,0X8D ,0X6F ,0X31 ,0XB2 ,0XEC ,0X0E ,0X50 ,0XAF ,0XF1 ,0X13 ,0X4D ,0XCE ,0X90 ,0X72 ,0X2C ,0X6D ,0X33 ,0XD1 ,0X8F ,0X0C ,0X52 ,0XB0 ,0XEE ,0X32 ,0X6C ,0X8E ,0XD0 ,0X53 ,0X0D ,0XEF ,0XB1 ,0XF0 ,0XAE ,0X4C ,0X12 ,0X91 ,0XCF ,0X2D ,0X73 ,0XCA ,0X94 ,0X76 ,0X28 ,0XAB ,0XF5 ,0X17 ,0X49 ,0X08 ,0X56 ,0XB4 ,0XEA ,0X69 ,0X37 ,0XD5 ,0X8B ,0X57 ,0X09 ,0XEB ,0XB5 ,0X36 ,0X68 ,0X8A ,0XD4 ,0X95 ,0XCB ,0X29 ,0X77 ,0XF4 ,0XAA ,0X48 ,0X16 ,0XE9 ,0XB7 ,0X55 ,0X0B ,0X88 ,0XD6 ,0X34 ,0X6A ,0X2B ,0X75 ,0X97 ,0XC9 ,0X4A ,0X14 ,0XF6 ,0XA8 ,0X74 ,0X2A ,0XC8 ,0X96 ,0X15 ,0X4B ,0XA9 ,0XF7 ,0XB6 ,0XE8 ,0X0A ,0X54 ,0XD7 ,0X89 ,0X6B ,0X35} ;
		Byte ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			const auto r1x = Index (ret ^ Byte (rax[i])) ;
			ret = Byte (mCache[r1x]) ;
		}
		return move (ret) ;
	}

	Word crchash16 (CR<Pointer> src ,CR<Length> size_) const override {
		return crchash16 (src ,size_ ,Word (0X00)) ;
	}

	Word crchash16 (CR<Pointer> src ,CR<Length> size_ ,CR<Word> val) const override {
		static const ARR<csc_uint16_t ,ENUM<256>> mCache {
			0x0000 ,0x1021 ,0x2042 ,0x3063 ,0x4084 ,0x50A5 ,0x60C6 ,0x70E7 ,0x8108 ,0x9129 ,0xA14A ,0xB16B ,0xC18C ,0xD1AD ,0xE1CE ,0xF1EF ,0x1231 ,0x0210 ,0x3273 ,0x2252 ,0x52B5 ,0x4294 ,0x72F7 ,0x62D6 ,0x9339 ,0x8318 ,0xB37B ,0xA35A ,0xD3BD ,0xC39C ,0xF3FF ,0xE3DE ,0x2462 ,0x3443 ,0x0420 ,0x1401 ,0x64E6 ,0x74C7 ,0x44A4 ,0x5485 ,0xA56A ,0xB54B ,0x8528 ,0x9509 ,0xE5EE ,0xF5CF ,0xC5AC ,0xD58D ,0x3653 ,0x2672 ,0x1611 ,0x0630 ,0x76D7 ,0x66F6 ,0x5695 ,0x46B4 ,0xB75B ,0xA77A ,0x9719 ,0x8738 ,0xF7DF ,0xE7FE ,0xD79D ,0xC7BC ,0x48C4 ,0x58E5 ,0x6886 ,0x78A7 ,0x0840 ,0x1861 ,0x2802 ,0x3823 ,0xC9CC ,0xD9ED ,0xE98E ,0xF9AF ,0x8948 ,0x9969 ,0xA90A ,0xB92B ,0x5AF5 ,0x4AD4 ,0x7AB7 ,0x6A96 ,0x1A71 ,0x0A50 ,0x3A33 ,0x2A12 ,0xDBFD ,0xCBDC ,0xFBBF ,0xEB9E ,0x9B79 ,0x8B58 ,0xBB3B ,0xAB1A ,0x6CA6 ,0x7C87 ,0x4CE4 ,0x5CC5 ,0x2C22 ,0x3C03 ,0x0C60 ,0x1C41 ,0xEDAE ,0xFD8F ,0xCDEC ,0xDDCD ,0xAD2A ,0xBD0B ,0x8D68 ,0x9D49 ,0x7E97 ,0x6EB6 ,0x5ED5 ,0x4EF4 ,0x3E13 ,0x2E32 ,0x1E51 ,0x0E70 ,0xFF9F ,0xEFBE ,0xDFDD ,0xCFFC ,0xBF1B ,0xAF3A ,0x9F59 ,0x8F78 ,0x9188 ,0x81A9 ,0xB1CA ,0xA1EB ,0xD10C ,0xC12D ,0xF14E ,0xE16F ,0x1080 ,0x00A1 ,0x30C2 ,0x20E3 ,0x5004 ,0x4025 ,0x7046 ,0x6067 ,0x83B9 ,0x9398 ,0xA3FB ,0xB3DA ,0xC33D ,0xD31C ,0xE37F ,0xF35E ,0x02B1 ,0x1290 ,0x22F3 ,0x32D2 ,0x4235 ,0x5214 ,0x6277 ,0x7256 ,0xB5EA ,0xA5CB ,0x95A8 ,0x8589 ,0xF56E ,0xE54F ,0xD52C ,0xC50D ,0x34E2 ,0x24C3 ,0x14A0 ,0x0481 ,0x7466 ,0x6447 ,0x5424 ,0x4405 ,0xA7DB ,0xB7FA ,0x8799 ,0x97B8 ,0xE75F ,0xF77E ,0xC71D ,0xD73C ,0x26D3 ,0x36F2 ,0x0691 ,0x16B0 ,0x6657 ,0x7676 ,0x4615 ,0x5634 ,0xD94C ,0xC96D ,0xF90E ,0xE92F ,0x99C8 ,0x89E9 ,0xB98A ,0xA9AB ,0x5844 ,0x4865 ,0x7806 ,0x6827 ,0x18C0 ,0x08E1 ,0x3882 ,0x28A3 ,0xCB7D ,0xDB5C ,0xEB3F ,0xFB1E ,0x8BF9 ,0x9BD8 ,0xABBB ,0xBB9A ,0x4A75 ,0x5A54 ,0x6A37 ,0x7A16 ,0x0AF1 ,0x1AD0 ,0x2AB3 ,0x3A92 ,0xFD2E ,0xED0F ,0xDD6C ,0xCD4D ,0xBDAA ,0xAD8B ,0x9DE8 ,0x8DC9 ,0x7C26 ,0x6C07 ,0x5C64 ,0x4C45 ,0x3CA2 ,0x2C83 ,0x1CE0 ,0x0CC1 ,0xEF1F ,0xFF3E ,0xCF5D ,0xDF7C ,0xAF9B ,0xBFBA ,0x8FD9 ,0x9FF8 ,0x6E17 ,0x7E36 ,0x4E55 ,0x5E74 ,0x2E93 ,0x3EB2 ,0x0ED1 ,0x1EF0} ;
		Word ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			const auto r1x = (ret >> 8) ^ Word (rax[i]) ;
			const auto r2x = Index (r1x & Word (0XFF)) ;
			ret = Word (mCache[r2x]) ^ (ret << 8) ;
		}
		return move (ret) ;
	}
} ;

exports CR<Super<Ref<HashProcLayout>>> HashProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<HashProcLayout>> ret ;
		ret.mThis = Ref<HashProcLayout>::make () ;
		HashProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HashProcHolder> HashProcHolder::hold (VR<HashProcLayout> that) {
	return VFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}

exports CFat<HashProcHolder> HashProcHolder::hold (CR<HashProcLayout> that) {
	return CFat<HashProcHolder> (HashProcImplHolder () ,that) ;
}
} ;