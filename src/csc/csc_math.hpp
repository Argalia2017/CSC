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
struct MathProcImplLayout ;

struct MathProcHolder implement Interface {
	imports VFat<MathProcHolder> create (VREF<Ref<MathProcImplLayout>> that) ;
	imports CFat<MathProcHolder> create (CREF<Ref<MathProcImplLayout>> that) ;

	virtual void initialize () = 0 ;
	virtual VAL32 square (CREF<VAL32> a) const = 0 ;
	virtual VAL64 square (CREF<VAL64> a) const = 0 ;
	virtual FLT32 square (CREF<FLT32> a) const = 0 ;
	virtual FLT64 square (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sqrt (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sqrt (CREF<FLT64> a) const = 0 ;
	virtual VAL32 abs (CREF<VAL32> a) const = 0 ;
	virtual VAL64 abs (CREF<VAL64> a) const = 0 ;
	virtual FLT32 abs (CREF<FLT32> a) const = 0 ;
	virtual FLT64 abs (CREF<FLT64> a) const = 0 ;
	virtual FLT32 inverse (CREF<FLT32> a) const = 0 ;
	virtual FLT64 inverse (CREF<FLT64> a) const = 0 ;
	virtual FLT32 cos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 tan (CREF<FLT32> a) const = 0 ;
	virtual FLT64 tan (CREF<FLT64> a) const = 0 ;
} ;

class MathProc implement RefBase<MathProcImplLayout> {
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
	imports ARG1 sqrt (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->sqrt (a) ;
	}

	template <class ARG1>
	imports ARG1 abs (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->abs (a) ;
	}

	template <class ARG1>
	imports ARG1 inverse (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->inverse (a) ;
	}

	template <class ARG1>
	imports ARG1 cos (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->cos (a) ;
	}

	template <class ARG1>
	imports ARG1 sin (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->sin (a) ;
	}

	template <class ARG1>
	imports ARG1 tan (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->tan (a) ;
	}
} ;
} ;