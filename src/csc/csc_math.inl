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
		if (std::isinf (a))
			return TRUE ;
		if (std::isnan (a))
			return TRUE ;
		return FALSE ;
	}

	Bool is_inf (CR<Flt64> a) const override {
		if (std::isinf (a))
			return TRUE ;
		if (std::isnan (a))
			return TRUE ;
		return FALSE ;
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
		if (a >= 0)
			return 1 ;
		return 0 ;
	}

	Val64 step (CR<Val64> a) const override {
		if (a >= 0)
			return 1 ;
		return 0 ;
	}

	Flt32 step (CR<Flt32> a) const override {
		if (a >= 0)
			return 1 ;
		return 0 ;
	}

	Flt64 step (CR<Flt64> a) const override {
		if (a >= 0)
			return 1 ;
		return 0 ;
	}

	Val32 delta (CR<Val32> a) const override {
		if (a == 0)
			return 1 ;
		return 0 ;
	}

	Val64 delta (CR<Val64> a) const override {
		if (a == 0)
			return 1 ;
		return 0 ;
	}

	Flt32 delta (CR<Flt32> a) const override {
		if (MathProc::abs (a) < FLT32_EPS)
			return 1 ;
		return 0 ;
	}

	Flt64 delta (CR<Flt64> a) const override {
		if (MathProc::abs (a) < FLT64_EPS)
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
		if (a > 0)
			return a ;
		if (a == VAL32_ABS)
			return VAL32_MAX ;
		return -a ;
	}

	Val64 abs (CR<Val64> a) const override {
		if (a > 0)
			return a ;
		if (a == VAL64_ABS)
			return VAL64_MAX ;
		return -a ;
	}

	Flt32 abs (CR<Flt32> a) const override {
		if (a > 0)
			return a ;
		return -a ;
	}

	Flt64 abs (CR<Flt64> a) const override {
		if (a > 0)
			return a ;
		return -a ;
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

	Val32 wrap (CR<Val32> a ,CR<Val32> max_) const override {
		if (max_ <= 0)
			return 0 ;
		const auto r1x = a % max_ ;
		const auto r2x = Val32 (r1x < 0) * max_ ;
		return r1x + r2x ;
	}

	Val64 wrap (CR<Val64> a ,CR<Val64> max_) const override {
		if (max_ <= 0)
			return 0 ;
		const auto r1x = a % max_ ;
		const auto r2x = Val64 (r1x < 0) * max_ ;
		return r1x + r2x ;
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
		const auto r2x = r1x - MathProc::floor (r1x ,Flt64 (2)) ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		return min_ + Val32 (round (r4x)) ;
	}

	Val64 lerp (CR<Flt64> a ,CR<Val64> min_ ,CR<Val64> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x ,Flt64 (2)) ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		return min_ + Val64 (round (r4x)) ;
	}

	Flt32 lerp (CR<Flt64> a ,CR<Flt32> min_ ,CR<Flt32> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x ,Flt64 (2)) ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		return min_ + Flt32 (r4x) ;
	}

	Flt64 lerp (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const override {
		const auto r1x = MathProc::abs (a) ;
		const auto r2x = r1x - MathProc::floor (r1x ,Flt64 (2)) ;
		const auto r3x = 1 - MathProc::abs (r2x - 1) ;
		const auto r4x = Flt64 (max_ - min_) * r3x ;
		return min_ + Flt64 (r4x) ;
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

	Flt32 atan (CR<Flt32> a) const override {
		return std::atan (a) ;
	}

	Flt64 atan (CR<Flt64> a) const override {
		return std::atan (a) ;
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

	Val64 exp2_bit (CR<Val64> a) const override {
		return Val64 (Quad (0X01) << a) ;
	}

	Val64 log2_bit (CR<Val64> a) const override {
		if (a <= 0)
			return 0 ;
		Val64 ret = 0 ;
		auto rax = Quad (a) ;
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0XFFFFFFFF00000000)))
				discard ;
			ret += 32 ;
			rax = rax >> 32 ;
		}
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0X00000000FFFF0000)))
				discard ;
			ret += 16 ;
			rax = rax >> 16 ;
		}
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0X000000000000FF00)))
				discard ;
			ret += 8 ;
			rax = rax >> 8 ;
		}
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0X00000000000000F0)))
				discard ;
			ret += 4 ;
			rax = rax >> 4 ;
		}
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0X000000000000000C)))
				discard ;
			ret += 2 ;
			rax = rax >> 2 ;
		}
		if ifdo (TRUE) {
			if (!ByteProc::any_bit (rax ,Quad (0X0000000000000002)))
				discard ;
			ret += 1 ;
			rax = rax >> 1 ;
		}
		if ifdo (TRUE) {
			if (rax == Quad (0X00))
				discard ;
			ret += 1 ;
		}
		return move (ret) ;
	}

	Val64 exp10_bit (CR<Val64> a) const override {
		assert (a >= 0) ;
		assert (a < 32) ;
		Val64 ret = 1 ;
		auto rax = a ;
		if ifdo (TRUE) {
			if (rax < 16)
				discard ;
			ret *= Val64 (10000000000000000) ;
			rax -= 16 ;
		}
		if ifdo (TRUE) {
			if (rax < 8)
				discard ;
			ret *= Val64 (100000000) ;
			rax -= 8 ;
		}
		if ifdo (TRUE) {
			if (rax < 4)
				discard ;
			ret *= Val64 (10000) ;
			rax -= 4 ;
		}
		if ifdo (TRUE) {
			if (rax < 2)
				discard ;
			ret *= Val64 (100) ;
			rax -= 2 ;
		}
		if ifdo (TRUE) {
			if (rax < 1)
				discard ;
			ret *= Val64 (10) ;
			rax -= 1 ;
		}
		return move (ret) ;
	}

	Val64 log10_bit (CR<Val64> a) const override {
		if (a <= 0)
			return 0 ;
		Val64 ret = 0 ;
		auto rax = a ;
		if ifdo (TRUE) {
			if (rax < Val64 (10000000000000000))
				discard ;
			ret += 16 ;
			rax /= Val64 (10000000000000000) ;
		}
		if ifdo (TRUE) {
			if (rax < Val64 (100000000))
				discard ;
			ret += 8 ;
			rax /= Val64 (100000000) ;
		}
		if ifdo (TRUE) {
			if (rax < Val64 (10000))
				discard ;
			ret += 4 ;
			rax /= Val64 (10000) ;
		}
		if ifdo (TRUE) {
			if (rax < Val64 (100))
				discard ;
			ret += 2 ;
			rax /= Val64 (100) ;
		}
		if ifdo (TRUE) {
			if (rax < Val64 (10))
				discard ;
			ret += 1 ;
			rax /= Val64 (10) ;
		}
		if ifdo (TRUE) {
			if (rax == Val64 (0))
				discard ;
			ret += 1 ;
		}
		return move (ret) ;
	}

	Flt32 pdf (CR<Flt32> a) const override {
		const auto r1x = -square (a) / 2 ;
		return exp (r1x) * Flt32 (MATH_PDF0) ;
	}

	Flt64 pdf (CR<Flt64> a) const override {
		const auto r1x = -square (a) / 2 ;
		return exp (r1x) * Flt64 (MATH_PDF0) ;
	}

	Flt32 cbf (CR<Flt32> a) const override {
		const auto r1x = a * Flt32 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) / 2 ;
	}

	Flt64 cbf (CR<Flt64> a) const override {
		const auto r1x = a * Flt64 (inverse (MATH_SQRT2)) ;
		return (1 + std::erf (r1x)) / 2 ;
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

	Bool mid_of (CR<Val32> a ,CR<Val32> min_ ,CR<Val32> max_) const override {
		if (a < min_)
			return FALSE ;
		if (a >= max_)
			return FALSE ;
		return TRUE ;
	}

	Bool mid_of (CR<Val64> a ,CR<Val64> min_ ,CR<Val64> max_) const override {
		if (a < min_)
			return FALSE ;
		if (a >= max_)
			return FALSE ;
		return TRUE ;
	}

	Bool mid_of (CR<Flt32> a ,CR<Flt32> min_ ,CR<Flt32> max_) const override {
		if (a < min_)
			return FALSE ;
		if (a >= max_)
			return FALSE ;
		return TRUE ;
	}

	Bool mid_of (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const override {
		if (a < min_)
			return FALSE ;
		if (a >= max_)
			return FALSE ;
		return TRUE ;
	}
} ;

exports CR<Super<UniqueRef<MathProcLayout>>> MathProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<MathProcLayout>> ret ;
		ret.mThis = UniqueRef<MathProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		MathProcHolder::hold (rax.ref)->initialize () ;
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
		const auto r3x = error - self.mAvg ;
		self.mMax = MathProc::max_of (self.mMax ,error) ;
		self.mAvg = self.mAvg + r3x * r2x ;
		const auto r4x = r1x * r2x * MathProc::square (self.mStd) + r1x * MathProc::square (r3x * r2x) ;
		self.mStd = MathProc::sqrt (r4x) ;
		const auto r5x = -self.mPro + MathProc::step (4 - error) ;
		self.mPro = self.mPro + r5x * r2x ;
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
		rax = fexp2_downflow (rax ,Quad (0XFFE0000000000000)) ;
		if ifdo (TRUE) {
			if (Val64 (rax.mDownflow) >= 0)
				discard ;
			rax.mMantissa = Quad (Val64 (rax.mMantissa) + 1) ;
			rax.mDownflow = Quad (0X00) ;
			if (!ByteProc::any_bit (rax.mMantissa ,Quad (0XFFE0000000000000)))
				discard ;
			rax.mMantissa = rax.mMantissa >> 1 ;
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent++ ;
		}
		if ifdo (TRUE) {
			const auto r1x = Length (-1074) - Length (rax.mExponent) ;
			if (r1x <= 0)
				discard ;
			rax.mMantissa = rax.mMantissa >> r1x ;
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent = -1075 ;
		}
		rax.mExponent += 1075 ;
		if ifdo (TRUE) {
			if (rax.mMantissa != Quad (0X00))
				discard ;
			rax.mExponent = 0 ;
		}
		const auto r2x = fexp2.mSign ? Quad (0X8000000000000000) : Quad (0X00) ;
		const auto r3x = (Quad (rax.mExponent) << 52) & Quad (0X7FF0000000000000) ;
		const auto r4x = rax.mMantissa & Quad (0X000FFFFFFFFFFFFF) ;
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
		ret.mMantissa = r3x ;
		ret.mDownflow = Quad (0X00) ;
		if ifdo (TRUE) {
			if (r2x == Quad (0X00))
				discard ;
			ret.mMantissa = ret.mMantissa | Quad (0X0010000000000000) ;
		}
		ret.mExponent = Val64 (r2x >> 52) ;
		ret.mExponent += Length (r2x == Quad (0X00)) ;
		ret.mExponent -= 1075 ;
		if ifdo (TRUE) {
			if (ret.mMantissa != Quad (0X00))
				discard ;
			ret.mExponent = 0 ;
		}
		if ifdo (TRUE) {
			if (ret.mMantissa == Quad (0X00))
				discard ;
			while (TRUE) {
				if (ByteProc::any_bit (ret.mMantissa ,Quad (0X0000000000000001)))
					break ;
				const auto r4x = ByteProc::shift (ret.mMantissa ,ret.mDownflow ,1) ;
				ret.mMantissa = ret.mMantissa >> 1 ;
				ret.mDownflow = r4x ;
				ret.mExponent++ ;
			}
		}
		return move (ret) ;
	}

	Notation fexp2_multiply (CR<Notation> a ,CR<Notation> b) const {
		assert (a.mRadix == 2) ;
		assert (b.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = a.mRadix ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (a.mSign ,b.mSign) ;
		const auto r1x = Length (32) ;
		const auto r2x = Quad (0X00000000FFFFFFFF) ;
		const auto r3x = Quad (0X8000000000000000) ;
		const auto r4x = fexp2_downflow (a ,r3x) ;
		const auto r5x = fexp2_downflow (b ,r3x) ;
		const auto r6x = Val64 (r4x.mMantissa >> r1x) ;
		const auto r7x = Val64 (r4x.mMantissa & r2x) ;
		const auto r8x = Val64 (r5x.mMantissa >> r1x) ;
		const auto r9x = Val64 (r5x.mMantissa & r2x) ;
		const auto r10x = Val64 (r4x.mDownflow >> r1x) ;
		const auto r11x = Val64 (r5x.mDownflow >> r1x) ;
		//@info: -1
		const auto r12x = r7x * r11x + r9x * r10x ;
		const auto r13x = Val64 (Quad (r12x) >> r1x) ;
		//@info: +0
		const auto r14x = r7x * r9x + r6x * r11x + r8x * r10x + r13x ;
		const auto r15x = Val64 (Quad (r14x) >> r1x) ;
		const auto r16x = Val64 (Quad (r14x) & r2x) ;
		//@info: +1
		const auto r17x = r7x * r8x + r6x * r9x + r15x ;
		const auto r18x = Val64 (Quad (r17x) >> r1x) ;
		const auto r19x = Val64 (Quad (r17x) & r2x) ;
		//@info: +2
		const auto r20x = r6x * r8x + r18x ;
		const auto r21x = Val64 (Quad (r19x) << r1x) + r16x ;
		ret.mMantissa = Quad (r20x) ;
		ret.mDownflow = Quad (r21x) ;
		ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
		return move (ret) ;
	}

	Notation fexp2_downflow (CR<Notation> fexp2 ,CR<Quad> mask) const {
		Notation ret = fexp2 ;
		const auto r1x = (mask & Quad (-Val64 (mask))) >> 1 ;
		if ifdo (TRUE) {
			if (ret.mMantissa == Quad (0X00))
				discard ;
			while (TRUE) {
				if (!ByteProc::any_bit (ret.mMantissa ,mask))
					break ;
				const auto r2x = ByteProc::shift (ret.mMantissa ,ret.mDownflow ,1) ;
				ret.mMantissa = ret.mMantissa >> 1 ;
				ret.mDownflow = r2x ;
				ret.mExponent++ ;
			}
			while (TRUE) {
				if (ByteProc::any_bit (ret.mMantissa ,r1x))
					break ;
				const auto r3x = ByteProc::shift (ret.mMantissa ,ret.mDownflow ,63) ;
				ret.mMantissa = r3x ;
				ret.mDownflow = ret.mDownflow << 1 ;
				ret.mExponent-- ;
			}
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
		const auto r1x = Val64 (fexp10.mDownflow << 64) / Val64 (1000000000000000000) ;
		ret.mDownflow = Quad (r1x) ;
		ret.mExponent = 0 ;
		const auto r2x = FEXP2Cache::expr[fexp10.mExponent] ;
		ret = fexp2_multiply (ret ,r2x) ;
		return move (ret) ;
	}

	Notation fexp10_multiply (CR<Notation> a ,CR<Notation> b) const {
		assert (a.mRadix == 10) ;
		assert (b.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = a.mRadix ;
		ret.mPrecision = 0 ;
		ret.mSign = MathProc::any_of (a.mSign ,b.mSign) ;
		const auto r1x = Length (9) ;
		const auto r2x = Val64 (1000000000) ;
		const auto r3x = Val64 (1000000000000000000) ;
		const auto r4x = fexp10_downflow (a ,r3x) ;
		const auto r5x = fexp10_downflow (b ,r3x) ;
		const auto r6x = Val64 (r4x.mMantissa) / r2x ;
		const auto r7x = Val64 (r5x.mMantissa) / r2x ;
		const auto r8x = Val64 (r4x.mMantissa) % r2x ;
		const auto r9x = Val64 (r5x.mMantissa) % r2x ;
		const auto r10x = Val64 (r4x.mDownflow) / r2x ;
		const auto r11x = Val64 (r5x.mDownflow) / r2x ;
		//@info: -1
		const auto r12x = r8x * r11x + r9x * r10x ;
		const auto r13x = r12x / r2x ;
		//@info: +0
		const auto r14x = r8x * r9x + r6x * r11x + r7x * r10x + r13x ;
		const auto r15x = r14x / r2x ;
		const auto r16x = r14x % r2x ;
		//@info: +1
		const auto r17x = r8x * r7x + r6x * r9x + r15x ;
		const auto r18x = r17x / r2x ;
		const auto r19x = r17x % r2x ;
		//@info: +2
		const auto r20x = r6x * r7x + r18x ;
		const auto r21x = r19x * r2x + r16x ;
		ret.mMantissa = Quad (r20x) ;
		ret.mDownflow = Quad (r21x) ;
		ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
		return move (ret) ;
	}

	Notation fexp10_downflow (CR<Notation> fexp10 ,CR<Val64> high) const {
		Notation ret = fexp10 ;
		const auto r1x = high / 10 ;
		if ifdo (TRUE) {
			if (Val64 (ret.mMantissa) >= 0)
				discard ;
			const auto r2x = Val64 (ret.mMantissa >> 1) ;
			const auto r3x = Val64 (ret.mMantissa & Quad (0X01)) ;
			const auto r4x = (r2x % 5 * 2 + r3x) * r1x + Val64 (ret.mDownflow) / 10 ;
			ret.mMantissa = Quad (r2x / 5) ;
			ret.mDownflow = Quad (r4x) ;
			ret.mExponent++ ;
		}
		if ifdo (TRUE) {
			if (ret.mMantissa == Quad (0X00))
				discard ;
			while (TRUE) {
				if (Val64 (ret.mMantissa) < high)
					break ;
				const auto r5x = Val64 (ret.mMantissa) / 10 ;
				const auto r6x = Val64 (ret.mMantissa) % 10 * r1x + Val64 (ret.mDownflow) / 10 ;
				ret.mMantissa = Quad (r5x) ;
				ret.mDownflow = Quad (r6x) ;
				ret.mExponent++ ;
			}
			while (TRUE) {
				if (Val64 (ret.mMantissa) >= r1x)
					break ;
				const auto r7x = Val64 (ret.mMantissa) * 10 + Val64 (ret.mDownflow) / r1x ;
				const auto r8x = Val64 (ret.mDownflow) % r1x * 10 ;
				ret.mMantissa = Quad (r7x) ;
				ret.mDownflow = Quad (r8x) ;
				ret.mExponent-- ;
			}
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
		const auto r1x = Val64 (fexp2.mDownflow >> 32) ;
		ret.mDownflow = Quad (r1x) >> 64 ;
		ret.mExponent = 0 ;
		const auto r2x = FEXP10Cache::expr[fexp2.mExponent] ;
		ret = fexp10_multiply (ret ,r2x) ;
		return move (ret) ;
	}
} ;

exports CR<Super<UniqueRef<FloatProcLayout>>> FloatProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<FloatProcLayout>> ret ;
		ret.mThis = UniqueRef<FloatProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		FloatProcHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FloatProcHolder> FloatProcHolder::hold (VR<FloatProcLayout> that) {
	return VFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

exports CFat<FloatProcHolder> FloatProcHolder::hold (CR<FloatProcLayout> that) {
	return CFat<FloatProcHolder> (FloatProcImplHolder () ,that) ;
}

struct FEXP2CacheLayout {} ;

exports CR<Super<UniqueRef<FEXP2CacheLayout>>> FEXP2CacheHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<FEXP2CacheLayout>> ret ;
		ret.mThis = UniqueRef<FEXP2CacheLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		FEXP2CacheHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

template class External<FEXP2CacheHolder ,FEXP2CacheLayout> ;

exports VFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (VR<FEXP2CacheLayout> that) {
	return VFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::expr ,that) ;
}

exports CFat<FEXP2CacheHolder> FEXP2CacheHolder::hold (CR<FEXP2CacheLayout> that) {
	return CFat<FEXP2CacheHolder> (External<FEXP2CacheHolder ,FEXP2CacheLayout>::expr ,that) ;
}

struct FEXP10CacheLayout {} ;

exports CR<Super<UniqueRef<FEXP10CacheLayout>>> FEXP10CacheHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<FEXP10CacheLayout>> ret ;
		ret.mThis = UniqueRef<FEXP10CacheLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		FEXP10CacheHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

template class External<FEXP10CacheHolder ,FEXP10CacheLayout> ;

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

	Byte shift (CR<Byte> high_ ,CR<Byte> low_ ,CR<Index> high_bit) const override {
		const auto r1x = high_bit ;
		const auto r2x = 8 - r1x ;
		return (high_ << r2x) | (low_ >> r1x) ;
	}

	Word shift (CR<Word> high_ ,CR<Word> low_ ,CR<Index> high_bit) const override {
		const auto r1x = high_bit ;
		const auto r2x = 16 - r1x ;
		return (high_ << r2x) | (low_ >> r1x) ;
	}

	Char shift (CR<Char> high_ ,CR<Char> low_ ,CR<Index> high_bit) const override {
		const auto r1x = high_bit ;
		const auto r2x = 32 - r1x ;
		return (high_ << r2x) | (low_ >> r1x) ;
	}

	Quad shift (CR<Quad> high_ ,CR<Quad> low_ ,CR<Index> high_bit) const override {
		const auto r1x = high_bit ;
		const auto r2x = 64 - r1x ;
		return (high_ << r2x) | (low_ >> r1x) ;
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

	Length pop_count (CR<Byte> a) const override {
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

	Length low_count (CR<Byte> a) const override {
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

exports CR<Super<UniqueRef<ByteProcLayout>>> ByteProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<ByteProcLayout>> ret ;
		ret.mThis = UniqueRef<ByteProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		ByteProcHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ByteProcHolder> ByteProcHolder::hold (VR<ByteProcLayout> that) {
	return VFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

exports CFat<ByteProcHolder> ByteProcHolder::hold (CR<ByteProcLayout> that) {
	return CFat<ByteProcHolder> (ByteProcImplHolder () ,that) ;
}

class BigRealImplHolder final implement Fat<BigRealHolder ,BigRealLayout> {
private:
	using INTEGER_MIN_SIZE = ENUM<8> ;

public:
	void initialize (CR<Length> size_) override {
		const auto r1x = inline_max (size_ ,1) ;
		const auto r2x = inline_alignas (r1x ,INTEGER_MIN_SIZE::expr) ;
		self.mReal = RefBuffer<Byte> (r2x) ;
		inline_memset (Pointer::from (self.mReal.ref) ,self.mReal.size ()) ;
		self.mWidth = r1x ;
		self.mShift = 0 ;
	}

	void initialize (CR<BigRealLayout> that) override {
		const auto r1x = inline_alignas (that.mWidth ,INTEGER_MIN_SIZE::expr) ;
		self.mReal = RefBuffer<Byte> (r1x) ;
		for (auto &&i : range (0 ,that.mWidth))
			self.mReal[i] = that.mReal[i] ;
		self.mWidth = that.mWidth ;
		self.mShift = that.mShift ;
	}

	BigRealLayout share () const {
		BigRealLayout ret ;
		BigRealHolder::hold (ret)->initialize (self) ;
		return move (ret) ;
	}

	static Byte get (CR<BigRealLayout> that ,CR<Index> index) {
		if (index < 0)
			return Byte (0X00) ;
		Index ix = MathProc::min_of (index ,that.mWidth - 1) ;
		return that.mReal[ix] ;
	}

	Length size () const override {
		if (!self.mReal.exist ())
			return 0 ;
		return self.mWidth ;
	}

	Index precision () const override {
		if (!self.mReal.exist ())
			return 0 ;
		Index ret = self.mWidth - 1 ;
		const auto r1x = self.mReal[ret] ;
		while (TRUE) {
			if (ret <= 0)
				break ;
			if (self.mReal[ret - 1] != r1x)
				break ;
			ret-- ;
		}
		return move (ret) ;
	}

	Flt64 fetch () const override {
		assert (self.mReal.exist ()) ;
		const auto r1x = get (self ,self.mWidth - 1) ;
		const auto r2x = Bool (r1x == Byte (0XFF)) ;
		auto rax = Notation () ;
		rax.mRadix = 2 ;
		rax.mSign = r2x ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x)
				discard ;
			const auto r3x = precision () - 8 ;
			const auto r4x = r3x - self.mShift ;
			rax.mMantissa = Quad (0X00) ;
			for (auto &&i : range (0 ,8)) {
				Index ix = i + r3x ;
				const auto r5x = Quad (get (self ,ix)) << Val32 (i * 8) ;
				rax.mMantissa = rax.mMantissa | r5x ;
			}
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent = r4x * 8 ;
		}
		if ifdo (act) {
			const auto r6x = sabs () ;
			const auto r7x = BigRealHolder::hold (r6x)->precision () - 8 ;
			const auto r8x = r7x - r6x.mShift ;
			rax.mMantissa = Quad (0X00) ;
			for (auto &&i : range (0 ,8)) {
				Index ix = i + r7x ;
				const auto r9x = Quad (get (r6x ,ix)) << Val32 (i * 8) ;
				rax.mMantissa = rax.mMantissa | r9x ;
			}
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent = r8x * 8 ;
		}
		return FloatProc::encode (rax) ;
	}

	void store (CR<Flt64> item) override {
		assert (self.mReal.exist ()) ;
		auto rax = FloatProc::decode (item) ;
		const auto r1x = rax.mExponent - inline_alignas (rax.mExponent - 7 ,8) ;
		for (auto &&i : range (0 ,r1x)) {
			noop (i) ;
			rax.mMantissa = rax.mMantissa << 1 ;
			rax.mExponent-- ;
		}
		const auto r2x = rax.mExponent / 8 ;
		const auto r3x = r2x >= 0 ? r2x : inline_max (-r2x - 8 ,0) ;
		const auto r4x = inline_max (r2x ,0) ;
		const auto r5x = 8 + r3x ;
		assume (self.mReal.size () >= r5x) ;
		inline_memset (Pointer::from (self.mReal.ref) ,r5x) ;
		for (auto &&i : range (0 ,8)) {
			Index ix = i + r4x ;
			const auto r6x = rax.mMantissa >> Val32 (i * 8) ;
			self.mReal[ix] = Byte (r6x) ;
		}
		self.mWidth = r5x ;
		self.mShift = r4x - r2x ;
		check_mask (self) ;
		if ifdo (TRUE) {
			if (!rax.mSign)
				discard ;
			self = minus () ;
		}
	}

	static Length aligned_width (CR<BigRealLayout> a ,CR<BigRealLayout> b) {
		const auto r1x = a.mWidth - a.mShift ;
		const auto r2x = b.mWidth - b.mShift ;
		const auto r3x = inline_max (r1x ,r2x) ;
		const auto r4x = inline_max (a.mShift ,b.mShift) ;
		return r3x + r4x ;
	}

	static Length aligned_shift (CR<BigRealLayout> a ,CR<BigRealLayout> b) {
		const auto r1x = inline_max (a.mShift ,b.mShift) ;
		return r1x ;
	}

	Bool equal (CR<BigRealLayout> that) const override {
		const auto r1x = aligned_width (self ,that) ;
		const auto r2x = aligned_shift (self ,that) ;
		const auto r3x = r1x - 1 - r2x + self.mShift ;
		const auto r4x = r1x - 1 - r2x + that.mShift ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r3x - i ;
			Index iy = r4x - i ;
			const auto r5x = inline_equal (get (self ,ix) ,get (that ,iy)) ;
			if (!r5x)
				return r5x ;
		}
		return TRUE ;
	}

	Flag compr (CR<BigRealLayout> that) const override {
		const auto r1x = aligned_width (self ,that) ;
		const auto r2x = aligned_shift (self ,that) ;
		const auto r3x = r1x - 1 - r2x + self.mShift ;
		const auto r4x = r1x - 1 - r2x + that.mShift ;
		const auto r5x = inline_compr (get (self ,r1x) ,get (that ,r1x)) ;
		if (r5x != ZERO)
			return -r5x ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r3x - i ;
			Index iy = r4x - i ;
			const auto r6x = inline_compr (get (self ,ix) ,get (that ,iy)) ;
			if (r6x != ZERO)
				return r6x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		visitor.push (Quad (self.mWidth)) ;
		visitor.push (Quad (self.mShift)) ;
		const auto r1x = self.mWidth ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = get (self ,i) ;
			visitor.push (r2x) ;
		}
		visitor.leave () ;
	}

	BigRealLayout sadd (CR<BigRealLayout> that) const override {
		BigRealLayout ret ;
		const auto r1x = aligned_width (self ,that) ;
		const auto r2x = aligned_shift (self ,that) ;
		const auto r3x = self.mShift - r2x ;
		const auto r4x = that.mShift - r2x ;
		BigRealHolder::hold (ret)->initialize (r1x) ;
		auto rax = Val32 (0) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r3x + i ;
			Index iy = r4x + i ;
			const auto r5x = Val32 (get (self ,ix)) + Val32 (get (that ,iy)) + rax ;
			rax = Val32 (Char (r5x) >> 8) ;
			ret.mReal[i] = Byte (r5x) ;
		}
		ret.mWidth = r1x ;
		ret.mShift = r2x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	BigRealLayout ssub (CR<BigRealLayout> that) const override {
		BigRealLayout ret ;
		const auto r1x = aligned_width (self ,that) ;
		const auto r2x = aligned_shift (self ,that) ;
		const auto r3x = self.mShift - r2x ;
		const auto r4x = that.mShift - r2x ;
		BigRealHolder::hold (ret)->initialize (r1x) ;
		auto rax = Val32 (0) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r3x + i ;
			Index iy = r4x + i ;
			const auto r5x = Val32 (get (self ,ix)) - Val32 (get (that ,iy)) - rax ;
			rax = Val32 (r5x < 0) ;
			const auto r6x = r5x + 256 * rax ;
			ret.mReal[i] = Byte (r6x) ;
		}
		ret.mWidth = r1x ;
		ret.mShift = r2x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	BigRealLayout smul (CR<BigRealLayout> that) const override {
		BigRealLayout ret ;
		const auto r1x = self.mWidth + that.mWidth ;
		const auto r2x = self.mShift + that.mShift ;
		BigRealHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,r1x)) {
			auto rax = Val32 (0) ;
			for (auto &&j : range (0 ,r1x)) {
				Index iy = i + j ;
				if (iy >= r1x)
					continue ;
				const auto r3x = Val32 (get (self ,i)) * Val32 (get (that ,j)) + rax ;
				const auto r4x = r3x + Val32 (ret.mReal[iy]) ;
				rax = Val32 (Char (r4x) >> 8) ;
				ret.mReal[iy] = Byte (r4x) ;
			}
		}
		ret.mWidth = r1x ;
		ret.mShift = r2x ;
		check_mask (ret) ;
		return move (ret) ;
	}

	BigRealLayout sdiv (CR<BigRealLayout> that) const override {
		BigRealLayout ret ;
		const auto r1x = BigRealHolder::hold (self)->compr (BigReal::zero ()) ;
		const auto r2x = BigRealHolder::hold (that)->compr (BigReal::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != ZERO)
				discard ;
			//@info: BigReal division by zero
			assume (FALSE) ;
		}
		const auto r3x = BigRealHolder::hold (self)->sabs () ;
		const auto r4x = BigRealHolder::hold (that)->sabs () ;
		if ifdo (act) {
			//@info: divisor raw value is 2^k, division degenerates to an exact shift
			const auto r5x = raw_exp2 (r4x) ;
			if (r5x < 0)
				discard ;
			ret = BigRealHolder::hold (r3x)->shift (8 * that.mShift - r5x) ;
		}
		if ifdo (act) {
			//@info: scale gives the quotient a fraction window of INTEGER_MIN_SIZE bytes
			auto rax = BigRealLayout () ;
			const auto r6x = INTEGER_MIN_SIZE::expr + that.mShift - self.mShift ;
			const auto r7x = inline_max (self.mWidth + inline_max (r6x ,0) ,that.mWidth + 1) + 1 ;
			BigRealHolder::hold (rax)->initialize (r7x) ;
			BigRealHolder::hold (ret)->initialize (r7x) ;
			sdiv_abs (ret ,rax ,r3x ,r4x ,r6x) ;
		}
		if ifdo (TRUE) {
			if (MathProc::sign (r1x) == MathProc::sign (r2x))
				discard ;
			ret = BigRealHolder::hold (ret)->minus () ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BigRealLayout smod (CR<BigRealLayout> that) const override {
		BigRealLayout ret ;
		const auto r1x = BigRealHolder::hold (self)->compr (BigReal::zero ()) ;
		const auto r2x = BigRealHolder::hold (that)->compr (BigReal::zero ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != ZERO)
				discard ;
			//@info: BigReal division by zero
			assume (FALSE) ;
		}
		const auto r3x = BigRealHolder::hold (self)->sabs () ;
		const auto r4x = BigRealHolder::hold (that)->sabs () ;
		if ifdo (act) {
			//@info: shift divisor raw left by a bytes so the quotient is an exact integer
			//@info: scale b keeps every dividend byte consumed
			auto rax = BigRealLayout () ;
			const auto r5x = inline_max (self.mShift - that.mShift ,0) ;
			const auto r6x = inline_max (that.mShift - self.mShift ,0) ;
			const auto r7x = BigRealHolder::hold (r4x)->shift (r5x * 8) ;
			const auto r8x = inline_max (r3x.mWidth + r6x ,r7x.mWidth + 1) + 1 ;
			BigRealHolder::hold (rax)->initialize (r8x) ;
			BigRealHolder::hold (ret)->initialize (r8x) ;
			sdiv_abs (rax ,ret ,r3x ,r7x ,r6x) ;
		}
		if ifdo (TRUE) {
			if (r1x >= ZERO)
				discard ;
			ret = BigRealHolder::hold (ret)->minus () ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	void sdiv_abs (VR<BigRealLayout> quotient ,VR<BigRealLayout> remainder ,CR<BigRealLayout> dividend ,CR<BigRealLayout> divisor ,CR<Length> scale) const {
		//@info: quotient.value = dividend.value / divisor.value, truncated below quotient byte 0
		//@info: quotient.mShift = scale + dividend.mShift - divisor.mShift
		//@info: remainder.mShift = scale + dividend.mShift
		const auto r1x = scale ;
		const auto r2x = quotient.mWidth ;
		assert (r1x + dividend.mShift >= divisor.mShift) ;
		assert (r2x >= divisor.mWidth + 1) ;
		for (auto &&i : range (0 ,r2x)) {
			Index ix = r2x - 1 - i ;
			Index iy = ix - r1x ;
			for (auto &&j : range (0 ,8)) {
				Index jx = 8 - 1 - j ;
				const auto r3x = MathProc::exp2_bit (jx) ;
				for (auto &&k : range (0 ,r2x - 1)) {
					Index kx = r2x - 1 - k ;
					remainder.mReal[kx] = ByteProc::shift (remainder.mReal[kx] ,remainder.mReal[kx - 1] ,7) ;
				}
				if ifdo (TRUE) {
					Index kx = 0 ;
					remainder.mReal[kx] = remainder.mReal[kx] << 1 ;
				}
				remainder.mReal[0] |= (get (dividend ,iy) >> jx) & Byte (0X01) ;
				auto act = TRUE ;
				if ifdo (act) {
					const auto r4x = raw_compr (remainder ,divisor) ;
					if (r4x < ZERO)
						discard ;
					auto rax = Val32 (0) ;
					for (auto &&k : range (0 ,r2x)) {
						const auto r5x = Val32 (get (remainder ,k)) - Val32 (get (divisor ,k)) - rax ;
						rax = Val32 (r5x < 0) ;
						const auto r6x = r5x + 256 * rax ;
						remainder.mReal[k] = Byte (r6x) ;
					}
					quotient.mReal[ix] |= Byte (r3x) ;
				}
				if ifdo (act) {
					quotient.mReal[ix] &= ~Byte (r3x) ;
				}
			}
		}
		quotient.mShift = r1x + dividend.mShift - divisor.mShift ;
		remainder.mShift = r1x + dividend.mShift ;
	}

	static Flag raw_compr (CR<BigRealLayout> lhs ,CR<BigRealLayout> rhs) {
		const auto r1x = inline_max (lhs.mWidth ,rhs.mWidth) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			const auto r2x = inline_compr (get (lhs ,ix) ,get (rhs ,ix)) ;
			if (r2x != ZERO)
				return r2x ;
		}
		return ZERO ;
	}

	static Length raw_exp2 (CR<BigRealLayout> that) {
		Length ret = -1 ;
		auto rax = ZERO ;
		for (auto &&i : range (0 ,that.mWidth)) {
			const auto r1x = Val32 (that.mReal[i]) ;
			for (auto &&j : range (0 ,8)) {
				if ((r1x & Val32 (MathProc::exp2_bit (j))) == 0)
					continue ;
				rax++ ;
				ret = i * 8 + j ;
			}
		}
		if (rax != 1)
			return -1 ;
		return ret ;
	}

	BigRealLayout sabs () const override {
		if (get (self ,self.mWidth - 1) == Byte (0XFF))
			return minus () ;
		return share () ;
	}

	BigRealLayout minus () const override {
		BigRealLayout ret ;
		BigRealHolder::hold (ret)->initialize (self.mWidth) ;
		for (auto &&i : range (0 ,ret.mWidth))
			ret.mReal[i] = ~get (self ,i) ;
		auto rax = Val32 (1) ;
		for (auto &&i : range (0 ,ret.mWidth)) {
			const auto r1x = Val32 (ret.mReal[i]) + rax ;
			rax = Val32 (Char (r1x) >> 8) ;
			ret.mReal[i] = Byte (r1x) ;
		}
		ret.mWidth = self.mWidth ;
		ret.mShift = self.mShift ;
		check_mask (ret) ;
		return move (ret) ;
	}

	BigRealLayout shift (CR<Length> scale) const override {
		BigRealLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (scale != 0)
				discard ;
			ret = share () ;
		}
		const auto r1x = MathProc::abs (scale) ;
		const auto r2x = r1x / 8 ;
		const auto r3x = r1x % 8 ;
		const auto r4x = self.mWidth + r2x + Val32 (r3x != 0) ;
		if ifdo (act) {
			if (scale <= 0)
				discard ;
			BigRealHolder::hold (ret)->initialize (r4x) ;
			for (auto &&i : range (0 ,r4x)) {
				Index ix = i - r2x ;
				ret.mReal[i] = get (self ,ix) ;
			}
			ret.mWidth = r4x ;
			ret.mShift = self.mShift ;
			shift_abs_l (ret ,r3x) ;
		}
		if ifdo (act) {
			if (scale >= 0)
				discard ;
			BigRealHolder::hold (ret)->initialize (r4x) ;
			for (auto &&i : range (0 ,r4x)) {
				Index ix = i - 1 ;
				ret.mReal[i] = get (self ,ix) ;
			}
			ret.mWidth = r4x ;
			ret.mShift = self.mShift + r2x + 1 ;
			shift_abs_r (ret ,r3x) ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	void shift_abs_l (VR<BigRealLayout> that ,CR<Length> scale) const {
		if (scale == 0)
			return ;
		assert (inline_mid (scale ,0 ,8)) ;
		const auto r1x = 8 - scale ;
		for (auto &&i : range (0 ,that.mWidth)) {
			Index ix = that.mWidth - 1 - i ;
			const auto r2x = get (that ,ix) ;
			const auto r3x = get (that ,ix - 1) ;
			that.mReal[ix] = ByteProc::shift (r2x ,r3x ,r1x) ;
		}
	}

	void shift_abs_r (VR<BigRealLayout> that ,CR<Length> scale) const {
		if (scale == 0)
			return ;
		assert (inline_mid (scale ,0 ,8)) ;
		const auto r1x = scale ;
		for (auto &&i : range (0 ,that.mWidth)) {
			Index ix = i ;
			const auto r2x = get (that ,ix) ;
			const auto r3x = get (that ,ix + 1) ;
			that.mReal[ix] = ByteProc::shift (r3x ,r2x ,r1x) ;
		}
	}

	BigRealLayout sround () const override {
		BigRealLayout ret = share () ;
		if ifdo (TRUE) {
			Index ix = inline_max (ret.mShift - 1 ,0) ;
			if (ByteProc::any_bit (ret.mReal[ix] ,Byte (0X80)))
				discard ;
			BigRealHolder::hold (ret)->increase () ;
		}
		for (auto &&i : range (0 ,ret.mShift)) {
			ret.mReal[i] = Byte (0X00) ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	void increase () override {
		auto rax = Val32 (1) ;
		Index ix = self.mShift ;
		while (TRUE) {
			if (ix >= self.mWidth)
				break ;
			if (rax == 0)
				break ;
			const auto r1x = Val32 (self.mReal[ix]) + rax ;
			rax = Val32 (Char (r1x) >> 8) ;
			self.mReal[ix] = Byte (r1x) ;
		}
		check_mask (self) ;
	}

	void decrease () override {
		auto rax = Val32 (1) ;
		Index ix = self.mShift ;
		while (TRUE) {
			if (ix >= self.mWidth)
				break ;
			if (rax == 0)
				break ;
			const auto r1x = Val32 (self.mReal[ix]) - rax ;
			rax = Val32 (r1x < 0) ;
			const auto r2x = r1x + 256 * rax ;
			self.mReal[ix] = Byte (r2x) ;
		}
		check_mask (self) ;
	}

	static void check_mask (VR<BigRealLayout> that) {
		const auto r1x = BigRealHolder::hold (that)->precision () ;
		Index ix = inline_max (r1x ,that.mShift) ;
		that.mWidth = ix + 1 ;
		if ifdo (TRUE) {
			Index iy = 0 ;
			while (TRUE) {
				if (iy >= that.mShift)
					break ;
				if (that.mReal[iy] != Byte (0X00))
					break ;
				iy++ ;
			}
			if (iy == 0)
				discard ;
			for (auto &&i : range (0 ,that.mWidth - iy)) {
				that.mReal[i] = that.mReal[i + iy] ;
			}
			that.mWidth -= iy ;
			that.mShift -= iy ;
		}
		ix = that.mWidth - 1 ;
		if ifdo (TRUE) {
			const auto r2x = that.mReal[ix] ;
			if (r2x == Byte (0X00))
				discard ;
			if (r2x == Byte (0XFF))
				discard ;
			const auto r3x = that.mWidth + 1 ;
			if ifdo (TRUE) {
				if (r3x <= that.mReal.size ())
					discard ;
				const auto r4x = inline_alignas (r3x ,INTEGER_MIN_SIZE::expr) ;
				that.mReal.resize (r4x) ;
			}
			ix++ ;
			that.mReal[ix] = ByteProc::binary (r2x & Byte (0X80)) ;
			that.mWidth = ix + 1 ;
		}
	}
} ;

exports VFat<BigRealHolder> BigRealHolder::hold (VR<BigRealLayout> that) {
	return VFat<BigRealHolder> (BigRealImplHolder () ,that) ;
}

exports CFat<BigRealHolder> BigRealHolder::hold (CR<BigRealLayout> that) {
	return CFat<BigRealHolder> (BigRealImplHolder () ,that) ;
}

struct JetNode ;
using JetEvalFunction = Function<VR<JetNode> ,CR<Wrapper<Flt64>>> ;

struct JetNode {
	Index mCheck ;
	Length mDepth ;
	Flt64 mFX ;
	Flt64 mEX ;
	RefBuffer<Flt64> mDX ;
	Index mSlot ;
	JetEvalFunction mEval ;
	JetIndex mP1 ;
	JetIndex mP2 ;
	Deque<JetIndex> mCompress ;
} ;

struct JetTree {
	Allocator<JetNode ,AllocatorNode> mTree ;
	Index mCheck ;
} ;

struct JetImplLayout {
	SharedRef<JetTree> mThis ;

public:
	static VR<JetImplLayout> expr_m () ;
} ;

inline VR<JetImplLayout> JetImplLayout::expr_m () {
	static auto mInstance = JetImplLayout () ;
	return mInstance ;
}

class JetImplHolder final implement Fat<JetHolder ,JetLayout> {
public:
	void initialize (CR<Length> size_ ,CR<Flt64> item) override {
		assert (size_ > 0) ;
		check_recycle (self) ;
		self.mIndex.m1st = self.mThis->mTree.alloc (Box<JetNode>::make ()) ;
		self.mIndex.m2nd = address (self.mThis.ref) ;
		ptr (self).mDepth = 1 ;
		ptr (self).mFX = item ;
		ptr (self).mEX = 0 ;
		ptr (self).mDX = RefBuffer<Flt64> (size_) ;
		const auto r1x = ptr (self).mDX.size () * SIZE_OF<Flt64>::expr ;
		inline_memset (Pointer::from (ptr (self).mDX.ref) ,r1x) ;
		ptr (self).mSlot = NONE ;
		ptr (self).mP1.m1st = NONE ;
		ptr (self).mP1.m2nd = ZERO ;
		ptr (self).mP2.m1st = NONE ;
		ptr (self).mP2.m2nd = ZERO ;
	}

	void initialize (CR<Length> size_ ,CR<Flt64> item ,CR<Index> slot) override {
		assert (inline_mid (slot ,0 ,size_)) ;
		initialize (size_ ,item) ;
		ptr (self).mSlot = slot ;
		ptr (self).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			if (node.mSlot == NONE)
				return ;
			assume (node.mSlot < params.rank ()) ;
			node.mFX = params[node.mSlot] ;
			node.mEX = 0 ;
			const auto r1x = node.mDX.size () * SIZE_OF<Flt64>::expr ;
			inline_memset (Pointer::from (node.mDX.ref) ,r1x) ;
			node.mDX[node.mSlot] = 1 ;
			check_fx (node) ;
		}) ;
	}

	void check_recycle (VR<JetLayout> root) {
		root.mThis = JetImplLayout::expr.mThis ;
		if (root.mThis.exist ())
			return ;
		root.mThis = SharedRef<JetTree>::make () ;
		root.mThis->mCheck = 0 ;
		JetImplLayout::expr.mThis = root.mThis.weak () ;
	}

	static VR<JetNode> ptr (CR<JetLayout> that) {
		return ptr (that.mIndex) ;
	}

	static VR<JetNode> ptr (CR<JetIndex> that) {
		assert (that.m2nd != ZERO) ;
		auto &&rax = keep[TYPE<JetTree>::expr] (Pointer::make (that.m2nd)) ;
		return rax.mTree[that.m1st] ;
	}

	Flt64 fx () const override {
		return ptr (self).mFX ;
	}

	Flt64 ex () const override {
		return ptr (self).mEX ;
	}

	Flt64 dx (CR<Index> slot) const override {
		return ptr (self).mDX[slot] ;
	}

	void compress () const {
		if (ptr (self).mCompress.size () > 0)
			return ;
		ptr (self).mCompress = Deque<JetIndex> (ptr (self).mDepth) ;
		auto rax = Deque<JetIndex> (ptr (self).mDepth) ;
		rax.add (self.mIndex) ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			const auto r1x = rax[rax.tail ()] ;
			rax.pop () ;
			if ifdo (TRUE) {
				if (r1x.m1st == NONE)
					discard ;
				ptr (self).mCompress.push (r1x) ;
				rax.add (ptr (r1x).mP1) ;
				rax.add (ptr (r1x).mP2) ;
			}
		}
	}

	void once (CR<Wrapper<Flt64>> params) const override {
		if (self.mIndex.m1st == NONE)
			return ;
		compress () ;
		for (auto &&i : ptr (self).mCompress) {
			ptr (i).mEval (ptr (i) ,params) ;
		}
	}

	JetLayout sadd (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout ssub (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout smul (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			node.mFX = ptr (node.mP1).mFX * ptr (node.mP2).mFX ;
			node.mEX = round_ex (ptr (node.mP1).mEX + ptr (node.mP2).mEX) ;
			const auto r1x = ptr (node.mP2).mFX ;
			const auto r2x = ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] + r2x * ptr (node.mP2).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout sdiv (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout inverse () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			const auto r1x = 1 / ptr (node.mP1).mFX ;
			node.mFX = r1x ;
			node.mEX = -ptr (node.mP1).mEX ;
			const auto r2x = -MathProc::square (r1x) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout ssqrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mFX >= 0) ;
			node.mFX = MathProc::sqrt (ptr (node.mP1).mFX) ;
			node.mEX = round_ex (ptr (node.mP1).mEX / 2) ;
			const auto r1x = 1 / (2 * node.mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout scbrt () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			node.mFX = MathProc::cbrt (ptr (node.mP1).mFX) ;
			node.mEX = round_ex (ptr (node.mP1).mEX / 3) ;
			const auto r1x = 1 / (3 * MathProc::square (node.mFX)) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout spow (CR<Val32> that) const override {
		JetLayout ret ;
		auto rax = JetLayout () ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		JetHolder::hold (rax)->initialize (ptr (self).mDX.size () ,that) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = rax.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout shypot (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout sabs () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout minus () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			copy_node (node ,ptr (node.mP1) ,-1) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout ssin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::sin (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = MathProc::cos (ptr (node.mP1).mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout scos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::cos (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = -MathProc::sin (ptr (node.mP1).mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout stan () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::tan (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = 1 + MathProc::square (node.mFX) ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout sasin () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::asin (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = MathProc::sqrt (1 - MathProc::square (ptr (node.mP1).mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout sacos () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			node.mFX = MathProc::acos (ptr (node.mP1).mFX) ;
			node.mEX = ptr (node.mP1).mEX ;
			const auto r1x = -MathProc::sqrt (1 - MathProc::square (ptr (node.mP1).mFX)) ;
			const auto r2x = 1 / r1x ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout satan (CR<JetLayout> that) const override {
		assert (ptr (self).mDX.size () == ptr (that).mDX.size ()) ;
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
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
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mP2 = that.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		ptr (ret).mDepth += ptr (that).mDepth ;
		return move (ret) ;
	}

	JetLayout sexp () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mEX == 0) ;
			const auto r1x = MathProc::exp (ptr (node.mP1).mFX) ;
			node.mFX = r1x ;
			node.mEX = ptr (node.mP1).mEX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout slog () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			assume (ptr (node.mP1).mFX >= 0) ;
			node.mFX = MathProc::log (ptr (node.mP1).mFX) ;
			const auto r1x = 1 - MathProc::delta (ptr (node.mP1).mEX) ;
			node.mEX = round_ex (r1x) ;
			const auto r2x = 1 / ptr (node.mP1).mFX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r2x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	JetLayout relu () const override {
		JetLayout ret ;
		JetHolder::hold (ret)->initialize (ptr (self).mDX.size () ,0) ;
		ptr (ret).mEval = JetEvalFunction ([] (VR<JetNode> node ,CR<Wrapper<Flt64>> params) {
			const auto r1x = MathProc::step (ptr (node.mP1).mFX) ;
			node.mFX = r1x * ptr (node.mP1).mFX ;
			node.mEX = r1x * ptr (node.mP1).mEX ;
			for (auto &&i : range (0 ,node.mDX.size ()))
				node.mDX[i] = r1x * ptr (node.mP1).mDX[i] ;
			check_fx (node) ;
		}) ;
		ptr (ret).mP1 = self.mIndex ;
		ptr (ret).mDepth += ptr (self).mDepth ;
		return move (ret) ;
	}

	static void copy_node (VR<JetNode> dst ,CR<JetNode> src ,CR<Flt64> si) {
		dst.mFX = si * src.mFX ;
		dst.mEX = src.mEX ;
		for (auto &&i : range (0 ,dst.mDX.size ()))
			dst.mDX[i] = si * src.mDX[i] ;
	}

	static Flt64 round_ex (CR<Flt64> ex) {
		return MathProc::round (ex * 100) * 0.01 ;
	}

	static void check_fx (VR<JetNode> node) {
		auto act = TRUE ;
		if ifdo (act) {
			if (!MathProc::is_low (node.mFX))
				discard ;
			node.mFX = 1 ;
			node.mEX-- ;
			const auto r1x = node.mDX.size () * SIZE_OF<Flt64>::expr ;
			inline_memset (Pointer::from (node.mDX.ref) ,r1x) ;
		}
		if ifdo (act) {
			const auto r2x = 1 / node.mFX ;
			if (!MathProc::is_low (r2x))
				discard ;
			node.mFX = 1 ;
			node.mEX++ ;
			const auto r3x = node.mDX.size () * SIZE_OF<Flt64>::expr ;
			inline_memset (Pointer::from (node.mDX.ref) ,r3x) ;
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
			0X0000 ,0X1189 ,0X2312 ,0X329B ,0X4624 ,0X57AD ,0X6536 ,0X74BF ,0X8C48 ,0X9DC1 ,0XAF5A ,0XBED3 ,0XCA6C ,0XDBE5 ,0XE97E ,0XF8F7 ,0X1081 ,0X0108 ,0X3393 ,0X221A ,0X56A5 ,0X472C ,0X75B7 ,0X643E ,0X9CC9 ,0X8D40 ,0XBFDB ,0XAE52 ,0XDAED ,0XCB64 ,0XF9FF ,0XE876 ,0X2102 ,0X308B ,0X0210 ,0X1399 ,0X6726 ,0X76AF ,0X4434 ,0X55BD ,0XAD4A ,0XBCC3 ,0X8E58 ,0X9FD1 ,0XEB6E ,0XFAE7 ,0XC87C ,0XD9F5 ,0X3183 ,0X200A ,0X1291 ,0X0318 ,0X77A7 ,0X662E ,0X54B5 ,0X453C ,0XBDCB ,0XAC42 ,0X9ED9 ,0X8F50 ,0XFBEF ,0XEA66 ,0XD8FD ,0XC974 ,0X4204 ,0X538D ,0X6116 ,0X709F ,0X0420 ,0X15A9 ,0X2732 ,0X36BB ,0XCE4C ,0XDFC5 ,0XED5E ,0XFCD7 ,0X8868 ,0X99E1 ,0XAB7A ,0XBAF3 ,0X5285 ,0X430C ,0X7197 ,0X601E ,0X14A1 ,0X0528 ,0X37B3 ,0X263A ,0XDECD ,0XCF44 ,0XFDDF ,0XEC56 ,0X98E9 ,0X8960 ,0XBBFB ,0XAA72 ,0X6306 ,0X728F ,0X4014 ,0X519D ,0X2522 ,0X34AB ,0X0630 ,0X17B9 ,0XEF4E ,0XFEC7 ,0XCC5C ,0XDDD5 ,0XA96A ,0XB8E3 ,0X8A78 ,0X9BF1 ,0X7387 ,0X620E ,0X5095 ,0X411C ,0X35A3 ,0X242A ,0X16B1 ,0X0738 ,0XFFCF ,0XEE46 ,0XDCDD ,0XCD54 ,0XB9EB ,0XA862 ,0X9AF9 ,0X8B70 ,0X8408 ,0X9581 ,0XA71A ,0XB693 ,0XC22C ,0XD3A5 ,0XE13E ,0XF0B7 ,0X0840 ,0X19C9 ,0X2B52 ,0X3ADB ,0X4E64 ,0X5FED ,0X6D76 ,0X7CFF ,0X9489 ,0X8500 ,0XB79B ,0XA612 ,0XD2AD ,0XC324 ,0XF1BF ,0XE036 ,0X18C1 ,0X0948 ,0X3BD3 ,0X2A5A ,0X5EE5 ,0X4F6C ,0X7DF7 ,0X6C7E ,0XA50A ,0XB483 ,0X8618 ,0X9791 ,0XE32E ,0XF2A7 ,0XC03C ,0XD1B5 ,0X2942 ,0X38CB ,0X0A50 ,0X1BD9 ,0X6F66 ,0X7EEF ,0X4C74 ,0X5DFD ,0XB58B ,0XA402 ,0X9699 ,0X8710 ,0XF3AF ,0XE226 ,0XD0BD ,0XC134 ,0X39C3 ,0X284A ,0X1AD1 ,0X0B58 ,0X7FE7 ,0X6E6E ,0X5CF5 ,0X4D7C ,0XC60C ,0XD785 ,0XE51E ,0XF497 ,0X8028 ,0X91A1 ,0XA33A ,0XB2B3 ,0X4A44 ,0X5BCD ,0X6956 ,0X78DF ,0X0C60 ,0X1DE9 ,0X2F72 ,0X3EFB ,0XD68D ,0XC704 ,0XF59F ,0XE416 ,0X90A9 ,0X8120 ,0XB3BB ,0XA232 ,0X5AC5 ,0X4B4C ,0X79D7 ,0X685E ,0X1CE1 ,0X0D68 ,0X3FF3 ,0X2E7A ,0XE70E ,0XF687 ,0XC41C ,0XD595 ,0XA12A ,0XB0A3 ,0X8238 ,0X93B1 ,0X6B46 ,0X7ACF ,0X4854 ,0X59DD ,0X2D62 ,0X3CEB ,0X0E70 ,0X1FF9 ,0XF78F ,0XE606 ,0XD49D ,0XC514 ,0XB1AB ,0XA022 ,0X92B9 ,0X8330 ,0X7BC7 ,0X6A4E ,0X58D5 ,0X495C ,0X3DE3 ,0X2C6A ,0X1EF1 ,0X0F78} ;
		Word ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			const auto r1x = ret ^ Word (rax[i]) ;
			const auto r2x = Index (r1x & Word (0xFF)) ;
			ret = Word (mCache[r2x]) ^ (ret >> 8) ;
		}
		return move (ret) ;
	}

	Char crchash32 (CR<Pointer> src ,CR<Length> size_) const override {
		return crchash32 (src ,size_ ,Char (0X00)) ;
	}

	Char crchash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) const override {
		static const ARR<csc_uint32_t ,ENUM<256>> mCache
		{
			0X00000000 ,0X77073096 ,0XEE0E612C ,0X990951BA ,0X076DC419 ,0X706AF48F ,0XE963A535 ,0X9E6495A3 ,0X0EDB8832 ,0X79DCB8A4 ,0XE0D5E91E ,0X97D2D988 ,0X09B64C2B ,0X7EB17CBD ,0XE7B82D07 ,0X90BF1D91 ,0X1DB71064 ,0X6AB020F2 ,0XF3B97148 ,0X84BE41DE ,0X1ADAD47D ,0X6DDDE4EB ,0XF4D4B551 ,0X83D385C7 ,0X136C9856 ,0X646BA8C0 ,0XFD62F97A ,0X8A65C9EC ,0X14015C4F ,0X63066CD9 ,0XFA0F3D63 ,0X8D080DF5 ,0X3B6E20C8 ,0X4C69105E ,0XD56041E4 ,0XA2677172 ,0X3C03E4D1 ,0X4B04D447 ,0XD20D85FD ,0XA50AB56B ,0X35B5A8FA ,0X42B2986C ,0XDBBBC9D6 ,0XACBCF940 ,0X32D86CE3 ,0X45DF5C75 ,0XDCD60DCF ,0XABD13D59 ,0X26D930AC ,0X51DE003A ,0XC8D75180 ,0XBFD06116 ,0X21B4F4B5 ,0X56B3C423 ,0XCFBA9599 ,0XB8BDA50F ,0X2802B89E ,0X5F058808 ,0XC60CD9B2 ,0XB10BE924 ,0X2F6F7C87 ,0X58684C11 ,0XC1611DAB ,0XB6662D3D ,0X76DC4190 ,0X01DB7106 ,0X98D220BC ,0XEFD5102A ,0X71B18589 ,0X06B6B51F ,0X9FBFE4A5 ,0XE8B8D433 ,0X7807C9A2 ,0X0F00F934 ,0X9609A88E ,0XE10E9818 ,0X7F6A0DBB ,0X086D3D2D ,0X91646C97 ,0XE6635C01 ,0X6B6B51F4 ,0X1C6C6162 ,0X856530D8 ,0XF262004E ,0X6C0695ED ,0X1B01A57B ,0X8208F4C1 ,0XF50FC457 ,0X65B0D9C6 ,0X12B7E950 ,0X8BBEB8EA ,0XFCB9887C ,0X62DD1DDF ,0X15DA2D49 ,0X8CD37CF3 ,0XFBD44C65 ,0X4DB26158 ,0X3AB551CE ,0XA3BC0074 ,0XD4BB30E2 ,0X4ADFA541 ,0X3DD895D7 ,0XA4D1C46D ,0XD3D6F4FB ,0X4369E96A ,0X346ED9FC ,0XAD678846 ,0XDA60B8D0 ,0X44042D73 ,0X33031DE5 ,0XAA0A4C5F ,0XDD0D7CC9 ,0X5005713C ,0X270241AA ,0XBE0B1010 ,0XC90C2086 ,0X5768B525 ,0X206F85B3 ,0XB966D409 ,0XCE61E49F ,0X5EDEF90E ,0X29D9C998 ,0XB0D09822 ,0XC7D7A8B4 ,0X59B33D17 ,0X2EB40D81 ,0XB7BD5C3B ,0XC0BA6CAD ,0XEDB88320 ,0X9ABFB3B6 ,0X03B6E20C ,0X74B1D29A ,0XEAD54739 ,0X9DD277AF ,0X04DB2615 ,0X73DC1683 ,0XE3630B12 ,0X94643B84 ,0X0D6D6A3E ,0X7A6A5AA8 ,0XE40ECF0B ,0X9309FF9D ,0X0A00AE27 ,0X7D079EB1 ,0XF00F9344 ,0X8708A3D2 ,0X1E01F268 ,0X6906C2FE ,0XF762575D ,0X806567CB ,0X196C3671 ,0X6E6B06E7 ,0XFED41B76 ,0X89D32BE0 ,0X10DA7A5A ,0X67DD4ACC ,0XF9B9DF6F ,0X8EBEEFF9 ,0X17B7BE43 ,0X60B08ED5 ,0XD6D6A3E8 ,0XA1D1937E ,0X38D8C2C4 ,0X4FDFF252 ,0XD1BB67F1 ,0XA6BC5767 ,0X3FB506DD ,0X48B2364B ,0XD80D2BDA ,0XAF0A1B4C ,0X36034AF6 ,0X41047A60 ,0XDF60EFC3 ,0XA867DF55 ,0X316E8EEF ,0X4669BE79 ,0XCB61B38C ,0XBC66831A ,0X256FD2A0 ,0X5268E236 ,0XCC0C7795 ,0XBB0B4703 ,0X220216B9 ,0X5505262F ,0XC5BA3BBE ,0XB2BD0B28 ,0X2BB45A92 ,0X5CB36A04 ,0XC2D7FFA7 ,0XB5D0CF31 ,0X2CD99E8B ,0X5BDEAE1D ,0X9B64C2B0 ,0XEC63F226 ,0X756AA39C ,0X026D930A ,0X9C0906A9 ,0XEB0E363F ,0X72076785 ,0X05005713 ,0X95BF4A82 ,0XE2B87A14 ,0X7BB12BAE ,0X0CB61B38 ,0X92D28E9B ,0XE5D5BE0D ,0X7CDCEFB7 ,0X0BDBDF21 ,0X86D3D2D4 ,0XF1D4E242 ,0X68DDB3F8 ,0X1FDA836E ,0X81BE16CD ,0XF6B9265B ,0X6FB077E1 ,0X18B74777 ,0X88085AE6 ,0XFF0F6A70 ,0X66063BCA ,0X11010B5C ,0X8F659EFF ,0XF862AE69 ,0X616BFFD3 ,0X166CCF45 ,0XA00AE278 ,0XD70DD2EE ,0X4E048354 ,0X3903B3C2 ,0XA7672661 ,0XD06016F7 ,0X4969474D ,0X3E6E77DB ,0XAED16A4A ,0XD9D65ADC ,0X40DF0B66 ,0X37D83BF0 ,0XA9BCAE53 ,0XDEBB9EC5 ,0X47B2CF7F ,0X30B5FFE9 ,0XBDBDF21C ,0XCABAC28A ,0X53B39330 ,0X24B4A3A6 ,0XBAD03605 ,0XCDD70693 ,0X54DE5729 ,0X23D967BF ,0XB3667A2E ,0XC4614AB8 ,0X5D681B02 ,0X2A6F2B94 ,0XB40BBE37 ,0XC30C8EA1 ,0X5A05DF1B ,0X2D02EF8D} ;
		Char ret = val ;
		auto &&rax = keep[TYPE<ARR<Byte>>::expr] (src) ;
		for (auto &&i : range (0 ,size_)) {
			const auto r1x = ret ^ Char (rax[i]) ;
			const auto r2x = Index (r1x & Char (0xFF)) ;
			ret = Char (mCache[r2x]) ^ (ret >> 8) ;
		}
		return move (ret) ;
	}
} ;

exports CR<Super<UniqueRef<HashProcLayout>>> HashProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<HashProcLayout>> ret ;
		ret.mThis = UniqueRef<HashProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		HashProcHolder::hold (rax.ref)->initialize () ;
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