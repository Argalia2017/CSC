#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
class ArrayLayout {
public:
	RefBufferLayout mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void fill (CREF<Pointer> a) = 0 ;
} ;

template <class A>
class Array implement ArrayLayout {
public:
	implicit Array () = default ;

	explicit Array (CREF<LENGTH> size_) {
		ArrayHolder::create (thiz)->initialize (size_) ;
	}

	LENGTH size () const {
		ArrayHolder::create (thiz)->size () ;
	}

	LENGTH length () const {
		ArrayHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	void fill (CREF<A> a) {
		return ArrayHolder::create (thiz)->fill (Pointer::from (a)) ;
	}
} ;
} ;