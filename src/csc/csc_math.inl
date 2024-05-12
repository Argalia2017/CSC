#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
struct MathToolPureLayout {
	FLT64 mE ;
	FLT64 mPi ;
	FLT64 mInvSqrt2Pi ;
	FLT64 mSqrt2 ;
} ;

class MathProcImplement implement Fat<MathToolHolder ,MathToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<MathToolPureLayout>::make () ;
		fake.mThis->mE = 2.71828182845904523536 ;
		fake.mThis->mPi = 3.14159265358979323846 ;
		fake.mThis->mInvSqrt2Pi = inverse (sqrt (2 * fake.mThis->mPi)) ;
		fake.mThis->mSqrt2 = sqrt (FLT64 (2)) ;
	}

	BOOL is_inf (CREF<FLT32> a) const override {
		return std::isinf (a) ;
	}

	BOOL is_inf (CREF<FLT64> a) const override {
		return std::isinf (a) ;
	}

	VAL32 step (CREF<VAL32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	VAL64 step (CREF<VAL64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT32 step (CREF<FLT32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT64 step (CREF<FLT64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT32 sign (CREF<FLT32> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	FLT64 sign (CREF<FLT64> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	VAL32 square (CREF<VAL32> a) const override {
		return a * a ;
	}

	VAL64 square (CREF<VAL64> a) const override {
		return a * a ;
	}

	FLT32 square (CREF<FLT32> a) const override {
		return a * a ;
	}

	FLT64 square (CREF<FLT64> a) const override {
		return a * a ;
	}

	FLT32 sqrt (CREF<FLT32> a) const override {
		return std::sqrt (a) ;
	}

	FLT64 sqrt (CREF<FLT64> a) const override {
		return std::sqrt (a) ;
	}

	VAL32 abs (CREF<VAL32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	VAL64 abs (CREF<VAL64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 abs (CREF<FLT32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT64 abs (CREF<FLT64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 inverse (CREF<FLT32> a) const override {
		if (abs (a) < FLT32_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT64 inverse (CREF<FLT64> a) const override {
		if (abs (a) < FLT64_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT32 floor (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT64 floor (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT32 round (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT64 round (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT32 ceil (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	FLT64 ceil (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT32 cos (CREF<FLT32> a) const override {
		return std::cos (a) ;
	}

	FLT64 cos (CREF<FLT64> a) const override {
		return std::cos (a) ;
	}

	FLT32 sin (CREF<FLT32> a) const override {
		return std::sin (a) ;
	}

	FLT64 sin (CREF<FLT64> a) const override {
		return std::sin (a) ;
	}

	FLT32 tan (CREF<FLT32> a) const override {
		return std::tan (a) ;
	}

	FLT64 tan (CREF<FLT64> a) const override {
		return std::tan (a) ;
	}

	FLT32 arccos (CREF<FLT32> a) const override {
		return std::acos (a) ;
	}

	FLT64 arccos (CREF<FLT64> a) const override {
		return std::acos (a) ;
	}

	FLT32 arcsin (CREF<FLT32> a) const override {
		return std::asin (a) ;
	}

	FLT64 arcsin (CREF<FLT64> a) const override {
		return std::asin (a) ;
	}

	FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT32 exp (CREF<FLT32> a) const override {
		return std::exp (a) ;
	}

	FLT64 exp (CREF<FLT64> a) const override {
		return std::exp (a) ;
	}

	FLT32 log (CREF<FLT32> a) const override {
		return std::log (a) ;
	}

	FLT64 log (CREF<FLT64> a) const override {
		return std::log (a) ;
	}

	FLT32 pdf (CREF<FLT32> a) const override {
		const auto r1x = -square (a) * FLT32 (0.5) ;
		return exp (r1x) * FLT32 (fake.mThis->mInvSqrt2Pi) ;
	}

	FLT64 pdf (CREF<FLT64> a) const override {
		const auto r1x = -square (a) * FLT64 (0.5) ;
		return exp (r1x) * FLT64 (fake.mThis->mInvSqrt2Pi) ;
	}

	FLT32 cbf (CREF<FLT32> a) const override {
		const auto r1x = a * FLT32 (inverse (fake.mThis->mSqrt2)) ;
		return (1 + std::erf (r1x)) * FLT32 (0.5) ;
	}

	FLT64 cbf (CREF<FLT64> a) const override {
		const auto r1x = a * FLT64 (inverse (fake.mThis->mSqrt2)) ;
		return (1 + std::erf (r1x)) * FLT64 (0.5) ;
	}

	BOOL all_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const override {
		if ifnot (a)
			return FALSE ;
		const auto r1x = address (b) + SIZE_OF<CaptureLayout>::expr ;
		const auto r2x = r1x + b.mRank * SIZE_OF<FLAG>::expr ;
		for (auto && i : CaptureIterator<BOOL> (r1x ,r2x)) {
			if ifnot (i)
				return FALSE ;
		}
		return TRUE ;
	}

	BOOL any_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const override {
		if (a)
			return TRUE ;
		const auto r1x = address (b) + SIZE_OF<CaptureLayout>::expr ;
		const auto r2x = r1x + b.mRank * SIZE_OF<FLAG>::expr ;
		for (auto && i : CaptureIterator<BOOL> (r1x ,r2x)) {
			if (i)
				return TRUE ;
		}
		return FALSE ;
	}

	VAL32 max_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	VAL64 max_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT32 max_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT64 max_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	template <class ARG1>
	ARG1 max_of_impl (CREF<ARG1> a ,CREF<CaptureLayout> b) const {
		ARG1 ret = a ;
		const auto r1x = address (b) + SIZE_OF<CaptureLayout>::expr ;
		const auto r2x = r1x + b.mRank * SIZE_OF<FLAG>::expr ;
		for (auto && i : CaptureIterator<ARG1> (r1x ,r2x)) {
			if (ret >= i)
				continue ;
			ret = i ;
		}
		return move (ret) ;
	}

	VAL32 min_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	VAL64 min_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT32 min_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT64 min_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	template <class ARG1>
	ARG1 min_of_impl (CREF<ARG1> a ,CREF<CaptureLayout> b) const {
		ARG1 ret = a ;
		const auto r1x = address (b) + SIZE_OF<CaptureLayout>::expr ;
		const auto r2x = r1x + b.mRank * SIZE_OF<FLAG>::expr ;
		for (auto && i : CaptureIterator<ARG1> (r1x ,r2x)) {
			if (ret <= i)
				continue ;
			ret = i ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MathToolHolder> MathToolHolder::create (VREF<MathToolLayout> that) {
	return VFat<MathToolHolder> (MathProcImplement () ,Pointer::from (that)) ;
}

exports CFat<MathToolHolder> MathToolHolder::create (CREF<MathToolLayout> that) {
	return CFat<MathToolHolder> (MathProcImplement () ,Pointer::from (that)) ;
}
} ;