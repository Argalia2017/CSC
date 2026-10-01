#pragma once

#ifndef __CSC_NUMERIC__
#define __CSC_NUMERIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"
#include "csc_matrix.hpp"

namespace CSC {
struct NumericResult {
	Flt64 mInitialCost ;
	Flt64 mFinalCost ;
	Length mIteration ;
	Bool mConverged ;
} ;

struct NumericProblemLayout ;

struct NumericProblemHolder implement Interface {
	imports Ref<NumericProblemLayout> create () ;
	imports VFat<NumericProblemHolder> hold (VR<NumericProblemLayout> that) ;
	imports CFat<NumericProblemHolder> hold (CR<NumericProblemLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual VR<Flt64> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Flt64> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index insert (CR<Length> size_) = 0 ;
	virtual void set_const (CR<Index> index) = 0 ;
	virtual void set_bound (CR<Index> index ,CR<Flt64> low ,CR<Flt64> high) = 0 ;
	virtual void add_block (RR<AutoRef<Pointer>> cost_function ,CR<Wrapper<Index>> coeff) = 0 ;
	virtual NumericResult solve () = 0 ;
} ;

class NumericProblem implement Super<Ref<NumericProblemLayout>> {
public:
	implicit NumericProblem () = default ;

	explicit NumericProblem (CR<Length> size_) {
		NumericProblemHolder::hold (thiz)->initialize (size_) ;
	}

	void clear () {
		return NumericProblemHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return NumericProblemHolder::hold (thiz)->size () ;
	}

	VR<Flt64> at (CR<Index> index) leftvalue {
		return NumericProblemHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<Flt64> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<Flt64> at (CR<Index> index) const leftvalue {
		return NumericProblemHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<Flt64> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index insert (CR<Length> size_) {
		return NumericProblemHolder::hold (thiz)->insert (size_) ;
	}

	void set_const (CR<Index> index) {
		return NumericProblemHolder::hold (thiz)->set_const (index) ;
	}

	void set_bound (CR<Index> index ,CR<Flt64> low ,CR<Flt64> high) {
		return NumericProblemHolder::hold (thiz)->set_bound (index ,low ,high) ;
	}

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_VALUE<ARG1>...>>>
	void add_block (RR<AutoRef<Pointer>> cost_function ,CR<ARG1>...coeff) {
		return NumericProblemHolder::hold (thiz)->add_block (move (cost_function) ,MakeWrapper (coeff...)) ;
	}

	NumericResult solve () {
		return NumericProblemHolder::hold (thiz)->solve () ;
	}
} ;
} ;
