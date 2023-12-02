#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
struct MathImplLayout {
	FLT64 mE ;
	FLT64 mPi ;
	FLT64 mInvSqrt2Pi ;
} ;

class MathImplHolder implement Fat<MathHolder ,MathLayout> {
public:
	void initialize () override {
		fake.mThis->mE = 2.71828182845904523536 ;
		fake.mThis->mPi = 3.14159265358979323846 ;
		fake.mThis->mInvSqrt2Pi = inverse (sqrt (2 * fake.mThis->mPi)) ;
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
} ;

exports VFat<MathHolder> MathHolder::create (VREF<MathLayout> that) {
	return VFat<MathHolder> (MathImplHolder () ,that) ;
}

exports CFat<MathHolder> MathHolder::create (CREF<MathLayout> that) {
	return CFat<MathHolder> (MathImplHolder () ,that) ;
}
} ;