#pragma once

#ifndef __CSC_IMAGE__
#define __CSC_IMAGE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
template <class...>
trait ARRAY_HELP ;

template <class A ,class SIZE>
trait ARRAY_HELP<A ,SIZE ,ALWAYS> {
	class Array {
	protected:
		Buffer<A ,SIZE> mArray ;

	public:
		implicit Array () = default ;

		explicit Array (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
		}

		LENGTH size () const {
			return mArray.size () ;
		}

		LENGTH length () const {
			return mArray.length () ;
		}
	} ;
} ;

template <class A ,class SIZE>
using Array = typename ARRAY_HELP<A ,SIZE ,ALWAYS>::Array ;

template <class A>
using ARRAY1 = Array<A ,RANK1> ;
template <class A>
using ARRAY2 = Array<A ,RANK2> ;
template <class A>
using ARRAY3 = Array<A ,RANK3> ;
template <class A>
using ARRAY4 = Array<A ,RANK4> ;
template <class A>
using ARRAY5 = Array<A ,RANK5> ;
template <class A>
using ARRAY6 = Array<A ,RANK6> ;
template <class A>
using ARRAY7 = Array<A ,RANK7> ;
template <class A>
using ARRAY8 = Array<A ,RANK8> ;
template <class A>
using ARRAY9 = Array<A ,RANK9> ;
} ;