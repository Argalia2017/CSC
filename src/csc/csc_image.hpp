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
template <class...>
trait ROWPROXY_HELP ;

template <class A ,class ITEM ,class COND>
trait ROWPROXY_HELP<A ,ITEM ,COND ,REQUIRE<COND>> {
	class RowProxy {
	protected:
		VRef<A> mThat ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<VRef<A>> image ,CREF<INDEX> y) {
			mThat = move (image) ;
			mY = y ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> x) rightvalue {
			return mThat->at (x ,mY) ;
		}
	} ;
} ;

template <class A ,class ITEM ,class COND>
trait ROWPROXY_HELP<A ,ITEM ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	class RowProxy {
	protected:
		CRef<A> mThat ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<CRef<A>> image ,CREF<INDEX> y) {
			mThat = move (image) ;
			mY = y ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> x) rightvalue {
			return mThat->at (x ,mY) ;
		}
	} ;
} ;

template <class A ,class ITEM>
using RowProxy = typename ROWPROXY_HELP<REMOVE_REF<A> ,ITEM ,IS_VARIABLE<A> ,ALWAYS>::RowProxy ;

template <class...>
trait IMAGE_HELP ;

template <class...>
trait IMAGE_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait IMAGE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	class Image {
	protected:
		Buffer<ITEM ,SIZE> mImage ;
		INDEX mCX ;
		INDEX mCY ;
		INDEX mStrip ;
		INDEX mOffset ;

	public:
		implicit Image () noexcept {
			mCX = 0 ;
			mCY = 0 ;
			mStrip = 0 ;
			mOffset = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait IMAGE_HELP<ITEM ,SIZE ,ALWAYS> {
	using Super = typename IMAGE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Image ;

	class Image implement Super {
	protected:
		using Super::mImage ;
		using Super::mCX ;
		using Super::mCY ;
		using Super::mStrip ;
		using Super::mOffset ;

	public:
		implicit Image () = default ;

		explicit Image (CREF<ARRAY2<LENGTH>> width_)
			:Image (width_[0] ,width_[1]) {}

		explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			const auto r1x = cx_ ;
			const auto r2x = cy_ ;
			mImage = Buffer<ITEM ,SIZE> (r1x * r2x) ;
			reset (r1x ,r2x) ;
		}

		LENGTH size () const {
			return mImage.size () ;
		}

		LENGTH length () const {
			return mCY * mStrip + mOffset ;
		}

		ARRAY2<LENGTH> width () const {
			ARRAY2<LENGTH> ret ;
			ret[0] = cx () ;
			ret[1] = cy () ;
			return move (ret) ;
		}

		LENGTH cx () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCX ;
		}

		LENGTH cy () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCY ;
		}

		LENGTH strip () const {
			if (mImage.size () == 0)
				return 0 ;
			return mStrip ;
		}

		LENGTH offset () const {
			if (mImage.size () == 0)
				return 0 ;
			return mOffset ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,size () ,move (unnamed)) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,length () ,move (unnamed)) ;
		}

		void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			reset (cx_ ,cy_ ,cx_ ,0) ;
		}

		void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> strip_ ,CREF<LENGTH> offset_) {
			assert (cx_ >= 0) ;
			assert (cy_ >= 0) ;
			assert (strip_ >= 0) ;
			assert (offset_ >= 0) ;
			assert (cx_ <= strip_) ;
			assert (length () <= size ()) ;
			mCX = cx_ ;
			mCY = cy_ ;
			mStrip = strip_ ;
			mOffset = offset_ ;
		}

		void fill (CREF<ITEM> item) {
			BufferProc<ITEM>::buf_fill (mImage ,item ,0 ,size ()) ;
		}

		PixelIterator iter () const {
			return PixelIterator (0 ,cx () ,0 ,cy ()) ;
		}

		VREF<ITEM> at (CREF<PIXEL> xy) leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline VREF<ITEM> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
			assert (operator_between (x ,0 ,mCX)) ;
			assert (operator_between (y ,0 ,mCY)) ;
			return mImage[y * mStrip + x + mOffset] ;
		}

		inline RowProxy<VREF<Image> ,ITEM> operator[] (CREF<INDEX> y) leftvalue {
			return RowProxy<VREF<Image> ,ITEM> (VRef<Image>::reference (thiz) ,y) ;
		}

		CREF<ITEM> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline CREF<ITEM> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
			assert (operator_between (x ,0 ,mCX)) ;
			assert (operator_between (y ,0 ,mCY)) ;
			return mImage[y * mStrip + x + mOffset] ;
		}

		inline RowProxy<CREF<Image> ,ITEM> operator[] (CREF<INDEX> y) const leftvalue {
			return RowProxy<CREF<Image> ,ITEM> (CRef<Image>::reference (thiz) ,y) ;
		}

		BOOL equal (CREF<Image> that) const {
			if (width () != that.width ())
				return FALSE ;
			for (auto&& i : iter ()) {
				if ifnot (operator_equal (at (i) ,that.at (i)))
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Image> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Image> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Image> that) const {
			const auto r1x = operator_compr (width () ,that.width ()) ;
			if (r1x != ZERO)
				return r1x ;
			for (auto&& i : iter ()) {
				const auto r2x = operator_compr (at (i) ,that.at (i)) ;
				if (r2x != ZERO)
					return r2x ;
			}
			return ZERO ;
		}

		inline BOOL operator< (CREF<Image> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Image> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Image> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Image> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			FLAG ret = hashcode () ;
			for (auto&& i : iter ()) {
				const auto r1x = operator_hash (at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		Image add (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) + that.at (i) ;
			return move (ret) ;
		}

		inline Image operator+ (CREF<Image> that) const {
			return add (that) ;
		}

		void add_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) += that.at (i) ;
		}

		inline void operator+= (CREF<Image> that) {
			add_with (that) ;
		}

		Image sub (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) - that.at (i) ;
			return move (ret) ;
		}

		inline Image operator- (CREF<Image> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) -= that.at (i) ;
		}

		inline void operator-= (CREF<Image> that) {
			sub_with (that) ;
		}

		Image mul (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) * that.at (i) ;
			return move (ret) ;
		}

		inline Image operator* (CREF<Image> that) const {
			return mul (that) ;
		}

		void mul_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) *= that.at (i) ;
		}

		inline void operator*= (CREF<Image> that) {
			mul_with (that) ;
		}

		Image div (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) / that.at (i) ;
			return move (ret) ;
		}

		inline Image operator/ (CREF<Image> that) const {
			return div (that) ;
		}

		void div_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) /= that.at (i) ;
		}

		inline void operator/= (CREF<Image> that) {
			div_with (that) ;
		}

		Image mod (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) % that.at (i) ;
			return move (ret) ;
		}

		inline Image operator% (CREF<Image> that) const {
			return mod (that) ;
		}

		void mod_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) %= that.at (i) ;
		}

		inline void operator%= (CREF<Image> that) {
			mod_with (that) ;
		}

		Image plus () const {
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = +at (i) ;
			return move (ret) ;
		}

		inline Image operator+ () const {
			return plus () ;
		}

		Image minus () const {
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = -at (i) ;
			return move (ret) ;
		}

		inline Image operator- () const {
			return minus () ;
		}

		Image band (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) & that.at (i) ;
			return move (ret) ;
		}

		inline Image operator& (CREF<Image> that) const {
			return band (that) ;
		}

		void band_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) &= that.at (i) ;
		}

		inline void operator&= (CREF<Image> that) {
			band_with (that) ;
		}

		Image bor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) | that.at (i) ;
			return move (ret) ;
		}

		inline Image operator| (CREF<Image> that) const {
			return bor (that) ;
		}

		void bor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) |= that.at (i) ;
		}

		inline void operator|= (CREF<Image> that) {
			bor_with (that) ;
		}

		Image bxor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = at (i) ^ that.at (i) ;
			return move (ret) ;
		}

		inline Image operator^ (CREF<Image> that) const {
			return bxor (that) ;
		}

		void bxor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto&& i : iter ())
				at (i) ^= that.at (i) ;
		}

		inline void operator^= (CREF<Image> that) {
			bxor_with (that) ;
		}

		Image bnot () const {
			Image ret = Image (width ()) ;
			for (auto&& i : iter ())
				ret.at (i) = ITEM (~at (i)) ;
			return move (ret) ;
		}

		inline Image operator~ () const {
			return bnot () ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Image = typename IMAGE_HELP<ITEM ,SIZE ,ALWAYS>::Image ;

template <class...>
trait SPARSE_HELP ;

template <class ITEM ,class SIZE>
trait SPARSE_HELP<ITEM ,SIZE ,ALWAYS> {
	class Sparse ;
} ;
} ;