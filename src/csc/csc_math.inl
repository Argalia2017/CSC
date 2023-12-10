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
struct MathProcImplLayout {} ;

class MathProcImplHolder implement Fat<MathProcHolder ,Ref<MathProcImplLayout>> {
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
} ;

exports VFat<MathProcHolder> MathProcHolder::create (VREF<Ref<MathProcImplLayout>> that) {
	return VFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::create (CREF<Ref<MathProcImplLayout>> that) {
	return CFat<MathProcHolder> (MathProcImplHolder () ,that) ;
}
} ;