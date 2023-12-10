#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

#include "csc_end.h"
#include <cstdlib>
#include "csc_begin.h"

namespace CSC {
class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> value ,CREF<LENGTH> size_) override {
		RefHolder::create (fake.mBuffer)->initialize (value ,size_) ;
		fake.mSize = size_ ;
		fake.mStep = fake.mBuffer.reflect (TYPE<ReflectSize>::expr)->type_size () ;
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
		if (size_ <= size ())
			return ;
		auto rax = RefLayout () ;
		RefHolder::create (rax)->initialize (RefHolder::create (fake.mBuffer)->unknown () ,size_) ;
		for (auto &&i : iter (0 ,fake.mSize)) {
			const auto r1x = fake.mBuffer.mPointer + i * fake.mStep ;
			const auto r2x = rax.mPointer + i * fake.mStep ;
			memcpy (csc_pointer_t (r2x) ,csc_pointer_t (r1x) ,fake.mStep) ;
		}
		swap (rax ,fake.mBuffer) ;
		fake.mSize = size_ ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}
} ;