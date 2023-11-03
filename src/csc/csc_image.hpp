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
template <class...>
trait ROWPROXY_HELP ;

template <class A>
trait ROWPROXY_HELP<A ,ALWAYS> {
	class RowProxy {
	protected:
		A mThat ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<A> that ,CREF<INDEX> y) {
			mThat = A::reference (that) ;
			mY = y ;
		}

		auto operator[] (CREF<INDEX> x) rightvalue->decltype (mThat->at (x ,mY)) {
			return mThat->at (x ,mY) ;
		}
	} ;
} ;

template <class A>
using RowProxy = typename ROWPROXY_HELP<A ,ALWAYS>::RowProxy ;

template <class...>
trait IMAGE_HELP ;

template <class A>
trait IMAGE_HELP<A ,ALWAYS> {
	class Image ;

	class Image {
	protected:

	public:
		implicit Image () = default ;
	} ;
} ;

template <class A>
using Image = typename IMAGE_HELP<A ,ALWAYS>::Image ;
} ;
