#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
template <class A>
class ArrayIterator {
protected:
	A mThat ;
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (RREF<A> that) {
		mThat = that ;
		mBegin = mThat->ibegin () ;
		mEnd = mThat->iend () ;
		mPeek = mBegin ;
	}

	ArrayIterator begin () const {
		return thiz ;
	}

	ArrayIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return mThat->length () ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	inline BOOL operator== (CREF<ArrayIterator>) const {
		return ifnot (good ()) ;
	}

	inline BOOL operator!= (CREF<ArrayIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	inline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek = mThat->index (mPeek) ;
	}

	inline void operator++ () {
		next () ;
	}
} ;

class ArrayLayout {
public:
	RefBufferLayout mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<ArrayHolder> that) const = 0 ;
	virtual FLAG compr (CREF<ArrayHolder> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
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
		return ArrayHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ArrayHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return ArrayHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const override {
		return ArrayHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const override {
		return ArrayHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return ArrayHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Array>> range () const leftvalue {
		return ArrayIterator<CPTR<Array>> ((&thiz)) ;
	}

	BOOL equal (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->at (that) ;
	}

	inline BOOL operator== (CREF<Array> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Array> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->at (that) ;
	}

	inline BOOL operator< (CREF<Array> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Array> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Array> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Array> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return ArrayHolder::create (thiz)->visit (visitor) ;
	}

	void fill (CREF<A> a) {
		return ArrayHolder::create (thiz)->fill (Pointer::from (a)) ;
	}
} ;

class StringLayout {
public:
	RefBufferLayout mString ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> create (VREF<StringLayout> that) ;
	imports CFat<StringHolder> create (CREF<StringLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<StringHolder> that) const = 0 ;
	virtual FLAG compr (CREF<StringHolder> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<Pointer> a) = 0 ;
} ;

class String implement StringLayout {
public:
	implicit String () = default ;
} ;
} ;