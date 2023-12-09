#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
class MathProcImplHolder implement Fat<MathProcHolder ,Ref<MathProcLayout>> {
public:
	void initialize () override {
	
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
} ;

exports VFat<MathProcHolder> MathProcHolder::create (VREF<Ref<MathProcLayout>> that) {
	return VFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::create (CREF<Ref<MathProcLayout>> that) {
	return CFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}
} ;