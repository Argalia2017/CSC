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

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mArray)->step () ;
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

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return size () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index - 1 ;
	}

	BOOL equal (CREF<ArrayHolder> that) const override {
	
	}

	FLAG compr (CREF<ArrayHolder> that) const override {
	
	}

	void visit (CREF<Visitor> visitor) const override {
	
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