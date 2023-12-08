#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) override {
		RefHolder::create (thix.mBuffer)->initialize (value ,size_) ;
		thix.mSize = size_ ;
		auto &&rax = unsafe_cast[TYPE<Unknown>::expr] (value.mHolder) ;
		thix.mStep = rax.type_size () ;
	}

	LENGTH size () const override {
		if ifnot (RefHolder::create (thix.mBuffer)->exist ())
			return 0 ;
		return thix.mSize ;
	}

	LENGTH step () const override {
		if ifnot (RefHolder::create (thix.mBuffer)->exist ())
			return 0 ;
		return thix.mStep ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return Pointer::make (thix.mBuffer.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return Pointer::make (thix.mBuffer.mPointer) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = thix.mBuffer.mPointer + index * thix.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = thix.mBuffer.mPointer + index * thix.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		unimplemented () ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}
} ;