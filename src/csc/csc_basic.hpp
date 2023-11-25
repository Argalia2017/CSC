#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
template <class...>
trait BUFFER_HELP ;

template <class A ,class SIZE>
trait BUFFER_HELP<A ,SIZE ,ALWAYS> {
	class Buffer ;

	struct NODE {
		ARR<A ,SIZE> mValue ;
	} ;

	class Buffer {
	protected:
		Box<NODE> mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<ARR<A ,SIZE>> that) {
			mBuffer = Box<NODE>::make (unsafe_cast[TYPE<NODE>::expr] (that)) ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			assert (size_ <= size ()) ;
			mBuffer = Box<NODE>::make () ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return SIZE::expr ;
		}

		VREF<ARR<A>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (unsafe_cast[TYPE<NODE>::expr] (unsafe_pointer (ZERO))) ;
			return unsafe_array (mBuffer->mValue[0]) ;
		}

		inline implicit operator VREF<ARR<A>> () leftvalue {
			return self ;
		}

		CREF<ARR<A>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (unsafe_cast[TYPE<NODE>::expr] (unsafe_pointer (ZERO))) ;
			return unsafe_array (mBuffer->mValue[0]) ;
		}

		inline implicit operator CREF<ARR<A>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<A> at (CREF<INDEX> index) leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer->mValue[index] ;
		}

		inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<A> at (CREF<INDEX> index) const leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer->mValue[index] ;
		}

		inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			if (size_ == size ())
				return ;
			assume (FALSE) ;
		}
	} ;
} ;

template <class A ,class SIZE>
using Buffer = typename BUFFER_HELP<A ,SIZE ,ALWAYS>::Buffer ;

template <class A ,class SIZE ,class = REQUIRE<ENUM_COMPR_GTEQ<SIZE ,ENUM_ZERO>>>
using BoxBuffer = typename BUFFER_HELP<A ,SIZE ,ALWAYS>::Buffer ;
} ;
