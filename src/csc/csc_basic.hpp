#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
class BoxBufferLayout {} ;

template <class A ,class B>
class BoxBuffer implement BoxBufferLayout {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	Tuple<ARR<A ,B>> mBuffer ;

public:
	implicit BoxBuffer () = default ;

	implicit BoxBuffer (CREF<ARR<A ,B>> that) {
		mBuffer = unsafe_cast[TYPE<Tuple<ARR<A ,B>>>::expr] (that) ;
	}

	explicit BoxBuffer (CREF<LENGTH> size_) {
		assert (size_ >= 0) ;
		assert (size_ < B::expr) ;
	}

	LENGTH size () const {
		return B::expr ;
	}

	LENGTH step () const {
		return SIZE_OF<A>::expr ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return Pointer::from (mBuffer) ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return Pointer::from (mBuffer) ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<BoxBuffer>) = delete ;

	inline BOOL operator!= (CREF<BoxBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		assert (FALSE) ;
	}
} ;

class RefBufferLayout {
public:
	RefLayout mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> create (VREF<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> create (CREF<RefBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> value ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

template <class A>
class RefBufferUnknownBinder final implement Unknown {
public:
	CREF<Interface> unknown (CREF<FLAG> uuid) const override {
		return Pointer::make (0) ;
	}
} ;

template <class A>
class RefBuffer implement RefBufferLayout {
public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->initialize (RefBufferUnknownBinder<A> () ,size_) ;
	}

	LENGTH size () const {
		return RefBufferHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return RefBufferHolder::create (thiz)->step () ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<RefBuffer>) = delete ;

	inline BOOL operator!= (CREF<RefBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return RefBufferHolder::create (thiz)->resize (size_) ;
	}
} ;
} ;
