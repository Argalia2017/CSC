#pragma once

#ifndef __CSC_IMAGE__
#define __CSC_IMAGE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
template <class A>
class RowProxy {
protected:
	A mThat ;
	INDEX mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (RREF<A> that ,CREF<INDEX> y) {
		mThat = that ;
		mY = y ;
	}

	auto operator[] (CREF<INDEX> x) rightvalue->decltype (mThat->at (x ,mY)) {
		return mThat->at (x ,mY) ;
	}
} ;

class ImageLayout {
public:
	RefBufferLayout mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mStrip ;
	LENGTH mOffset ;
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> create (VREF<ImageLayout> that) ;
	imports CFat<ImageHolder> create (CREF<ImageLayout> that) ;

	virtual void initialize (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH cx () const = 0 ;
	virtual LENGTH cy () const = 0 ;
	virtual LENGTH strip () const = 0 ;
	virtual LENGTH offset () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
} ;

template <class A>
class Image implement ImageLayout {
public:
	implicit Image () = default ;

	explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
		ImageHolder::create (thiz)->initialize (cx_ ,cy_) ;
	}

	LENGTH size () const {
		return ImageHolder::create (thiz)->size () ;
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

	inline VREF<A> operator[] (CREF<PIXEL> xy) leftvalue {
		return at (xy) ;
	}

	inline RowProxy<VPTR<Image>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VPTR<Image>> ((&thiz) ,y) ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return ImageHolder::create (thiz)->at (x ,y) ;
	}

	CREF<A> at (CREF<PIXEL> xy) const leftvalue {
		return at (xy.mX ,xy.mY) ;
	}

	inline CREF<A> operator[] (CREF<PIXEL> xy) const leftvalue {
		return at (xy) ;
	}

	inline RowProxy<CPTR<Image>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CPTR<Image>> ((&thiz) ,y) ;
	}
} ;
} ;
