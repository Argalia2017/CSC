#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
class ArrayImplHolder implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<Unknown> value ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mArray)->initialize (value ,size_) ;
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

	BOOL equal (CREF<ArrayLayout> that) const override {
		const auto r1x = ReflectEqual::from (fake.mArray.mBuffer.mHolder) ;
		for (auto &&i : iter (0 ,size ())) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			auto &&rbx = RefBufferHolder::create (that.mArray)->at (i) ;
			if ifnot (r1x->equal (rax ,rbx))
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<ArrayLayout> that) const override {
		const auto r1x = ReflectCompr::from (fake.mArray.mBuffer.mHolder) ;
		for (auto &&i : iter (0 ,size ())) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			auto &&rbx = RefBufferHolder::create (that.mArray)->at (i) ;
			const auto r2x = r1x->compr (rax ,rbx) ;
			if (r2x != ZERO)
				return r2x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		unimplemented () ;
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = ReflectAssign::from (fake.mArray.mBuffer.mHolder) ;
		for (auto &&i : iter (0 ,size ())) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			r1x->assign (rax ,item) ;
		}
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::create (VREF<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

exports CFat<ArrayHolder> ArrayHolder::create (CREF<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}
} ;