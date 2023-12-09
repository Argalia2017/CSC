#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
class ArrayImplHolder implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		unimplemented () ;
	}

	LENGTH size () const override {
		return RefBufferHolder::create (fake.mArray)->size () ;
	}

	LENGTH length () const override {
		return RefBufferHolder::create (fake.mArray)->size () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return RefBufferHolder::create (fake.mArray)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return RefBufferHolder::create (fake.mArray)->at (index) ;
	}

	void fill (CREF<Pointer> a) override {
		unimplemented () ;
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::create (VREF<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

exports CFat<ArrayHolder> ArrayHolder::create (CREF<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}
} ;