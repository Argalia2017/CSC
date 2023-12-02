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
struct MathImplLayout ;

struct MathLayout implement RefBase<MathImplLayout> {} ;

struct MathHolder implement Interface {
	imports VFat<MathHolder> create (VREF<MathLayout> that) ;
	imports CFat<MathHolder> create (CREF<MathLayout> that) ;

	virtual void initialize () = 0 ;
	virtual VAL32 step (CREF<VAL32> a) const = 0 ;
	virtual VAL64 step (CREF<VAL64> a) const = 0 ;
	virtual FLT32 step (CREF<FLT32> a) const = 0 ;
	virtual FLT64 step (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sign (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sign (CREF<FLT64> a) const = 0 ;
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
	virtual VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
	virtual VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
	virtual FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const = 0 ;
	virtual FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const = 0 ;
	virtual FLT32 cos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 tan (CREF<FLT32> a) const = 0 ;
	virtual FLT64 tan (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arccos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arccos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arcsin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arcsin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const = 0 ;
	virtual FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const = 0 ;
} ;

class Math implement MathLayout {
protected:
	using MathLayout::mThis ;

public:
	imports CREF<Math> instance () {
		return memorize ([&] () {
			Math ret ;
			MathHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	template <class ARG1>
	imports ARG1 step (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathHolder::create (instance ())->step (a) ;
	}

	template <class ARG1>
	imports ARG1 sign (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->sign (a) ;
	}

	template <class ARG1>
	imports ARG1 square (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathHolder::create (instance ())->square (a) ;
	}

	template <class ARG1>
	imports ARG1 sqrt (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->sqrt (a) ;
	}

	template <class ARG1>
	imports ARG1 abs (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathHolder::create (instance ())->abs (a) ;
	}

	template <class ARG1>
	imports ARG1 inverse (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->inverse (a) ;
	}

	template <class ARG1>
	imports ARG1 clamp (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathHolder::create (instance ())->clamp (a) ;
	}

	template <class ARG1>
	imports ARG1 cos (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->cos (a) ;
	}

	template <class ARG1>
	imports ARG1 sin (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->sin (a) ;
	}

	template <class ARG1>
	imports ARG1 tan (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->tan (a) ;
	}

	template <class ARG1>
	imports ARG1 arccos (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->arccos (a) ;
	}

	template <class ARG1>
	imports ARG1 arcsin (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->arcsin (a) ;
	}

	template <class ARG1>
	imports ARG1 arctan (CREF<ARG1> y ,CREF<ARG1> x) {
		require (IS_FLOAT<ARG1>) ;
		return MathHolder::create (instance ())->arctan (y ,x) ;
	}
} ;
} ;