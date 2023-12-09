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
class MathProcLayout ;

struct MathProcHolder implement Interface {
	imports VFat<MathProcHolder> create (VREF<Ref<MathProcLayout>> that) ;
	imports CFat<MathProcHolder> create (CREF<Ref<MathProcLayout>> that) ;

	virtual void initialize () = 0 ;
	virtual VAL32 square (CREF<VAL32> a) const = 0 ;
	virtual VAL64 square (CREF<VAL64> a) const = 0 ;
	virtual FLT32 square (CREF<FLT32> a) const = 0 ;
	virtual FLT64 square (CREF<FLT64> a) const = 0 ;
	virtual FLT32 abs (CREF<FLT32> a) const = 0 ;
	virtual FLT64 abs (CREF<FLT64> a) const = 0 ;
	virtual FLT32 inverse (CREF<FLT32> a) const = 0 ;
	virtual FLT64 inverse (CREF<FLT64> a) const = 0 ;
} ;

class MathProc implement RefBase<MathProcLayout> {
public:
	imports CREF<MathProc> instance () {
		return memorize ([&] () {
			MathProc ret ;
			MathProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	template <class ARG1>
	imports ARG1 square (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->square (a) ;
	}

	template <class ARG1>
	imports ARG1 abs (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->abs (a) ;
	}

	template <class ARG1>
	imports ARG1 inverse (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->inverse (a) ;
	}
} ;
} ;