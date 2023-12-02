#pragma once

#ifndef __CSC_IMAGE__
#define __CSC_IMAGE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class A>
class RowProxy {
protected:
	A mThat ;
	INDEX mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (XREF<A> that ,CREF<INDEX> y) :mThat (that) {
		mY = y ;
	}

	auto operator[] (CREF<INDEX> x) rightvalue->decltype (mThat->at (x ,mY)) {
		return mThat->at (x ,mY) ;
	}
} ;

struct ImageLayout {
	RefBufferLayout mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mStrip ;
	LENGTH mOffset ;

public:
	implicit ImageLayout () noexcept {
		mCX = 0 ;
		mCY = 0 ;
		mStrip = 0 ;
		mOffset = 0 ;
	}
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> create (VREF<ImageLayout> that) ;
	imports CFat<ImageHolder> create (CREF<ImageLayout> that) ;

	virtual void initialize (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH cx () const = 0 ;
	virtual LENGTH cy () const = 0 ;
	virtual LENGTH strip () const = 0 ;
	virtual LENGTH offset () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual void fill (CREF<Pointer> a) = 0 ;
} ;

template <class A>
class Image implement ImageLayout {
protected:
	require (CHECK<RefBuffer<A>>) ;
	using ImageLayout::mImage ;
	using ImageLayout::mCX ;
	using ImageLayout::mCY ;
	using ImageLayout::mStrip ;
	using ImageLayout::mOffset ;

public:
	implicit Image () = default ;

	explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
		ImageHolder::create (thiz)->initialize (cx_ ,cy_) ;
	}

	LENGTH size () const {
		return ImageHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ImageHolder::create (thiz)->step () ;
	}

	LENGTH cx () const {
		return ImageHolder::create (thiz)->cx () ;
	}

	LENGTH cy () const {
		return ImageHolder::create (thiz)->cy () ;
	}

	LENGTH strip () const {
		return ImageHolder::create (thiz)->strip () ;
	}

	LENGTH offset () const {
		return ImageHolder::create (thiz)->offset () ;
	}

	VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return ImageHolder::create (thiz)->at (x ,y) ;
	}

	VREF<A> at (CREF<PIXEL> xy) leftvalue {
		return at (xy.mX ,xy.mY) ;
	}

	forceinline VREF<A> operator[] (CREF<PIXEL> xy) leftvalue {
		return at (xy) ;
	}

	forceinline RowProxy<VPTR<Image>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VPTR<Image>> ((&thiz) ,y) ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return ImageHolder::create (thiz)->at (x ,y) ;
	}

	CREF<A> at (CREF<PIXEL> xy) const leftvalue {
		return at (xy.mX ,xy.mY) ;
	}

	forceinline CREF<A> operator[] (CREF<PIXEL> xy) const leftvalue {
		return at (xy) ;
	}

	forceinline RowProxy<CPTR<Image>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CPTR<Image>> ((&thiz) ,y) ;
	}

	PixelIterator iter () const {
		return PixelIterator (0 ,cx () ,0 ,cy ()) ;
	}

	void fill (CREF<A> a) {
		return ImageHolder::create (thiz)->fill (Pointer::from (a)) ;
	}
} ;
} ;
