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

	explicit ArrayIterator (XREF<A> that) :mThat (that) {
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

	virtual void initialize (CREF<Unknown> value ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<ArrayLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ArrayLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
} ;

template <class A>
class ArrayUnknownBinder final implement Unknown {
public:
	FLAG unknown (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSize::uuid ())
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreate::uuid ())
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroy::uuid ())
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectClone::uuid ())
			return unsafe_hold (ReflectCloneBinder<A> ()) ;
		if (uuid == ReflectEqual::uuid ())
			return unsafe_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectCompr::uuid ())
			return unsafe_hold (ReflectComprBinder<A> ()) ;
		assert (FALSE) ;
		return 0 ;
	}
} ;

template <class A>
class Array implement ArrayLayout {
public:
	implicit Array () = default ;

	explicit Array (CREF<LENGTH> size_) {
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
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

	INDEX ibegin () const {
		return ArrayHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ArrayHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Array>> range () const leftvalue {
		return ArrayIterator<CPTR<Array>> ((&thiz)) ;
	}

	BOOL equal (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Array> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Array> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->compr (that) ;
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

	void fill (CREF<A> item) {
		return ArrayHolder::create (thiz)->fill (Pointer::from (item)) ;
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
	virtual void initialize (CREF<RefBase<SliceLayout>> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<STRU32> item) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<StringLayout> that) const = 0 ;
	virtual FLAG compr (CREF<StringLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<STRU32> item) = 0 ;
} ;

class String implement StringLayout {
public:
	implicit String () = default ;

	explicit String (CREF<LENGTH> size_) {
		StringHolder::create (thiz)->initialize (size_) ;
	}

	explicit String (CREF<RefBase<SliceLayout>> text) {
		StringHolder::create (thiz)->initialize (text) ;
	}

	LENGTH size () const {
		return StringHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return StringHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return StringHolder::create (thiz)->length () ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		return StringHolder::create (thiz)->get (index ,item) ;
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) {
		return StringHolder::create (thiz)->set (index ,item) ;
	}

	STRU32 at (CREF<INDEX> index) const {
		STRU32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	inline SetProxy<VPTR<String> ,STRU32> operator[] (CREF<INDEX> index) leftvalue {
		return SetProxy<VPTR<String> ,STRU32> ((&thiz) ,index) ;
	}

	inline SetProxy<CPTR<String> ,STRU32> operator[] (CREF<INDEX> index) const leftvalue {
		return SetProxy<CPTR<String> ,STRU32> ((&thiz) ,index) ;
	}

	INDEX ibegin () const {
		return StringHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return StringHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return StringHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<String>> range () const leftvalue {
		return ArrayIterator<CPTR<String>> ((&thiz)) ;
	}

	BOOL equal (CREF<String> that) const {
		return StringHolder::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<String> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<String> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<String> that) const {
		return StringHolder::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<String> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<String> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<String> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<String> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return StringHolder::create (thiz)->visit (visitor) ;
	}

	void fill (CREF<STRU32> item) {
		return StringHolder::create (thiz)->fill (item) ;
	}
} ;
} ;