#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> value ,CREF<LENGTH> size_) override {
		RefHolder::create (fake.mBuffer)->initialize (value ,size_) ;
		fake.mSize = size_ ;
		fake.mStep = fake.mBuffer.unknown (TYPE<ReflectSize>::expr)->type_size () ;
	}

	LENGTH size () const override {
		if ifnot (RefHolder::create (fake.mBuffer)->exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if ifnot (RefHolder::create (fake.mBuffer)->exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return RefHolder::create (fake.mBuffer)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefHolder::create (fake.mBuffer)->self ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer.mPointer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer.mPointer + index * fake.mStep ;
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