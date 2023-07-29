#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
trait IMAGEITERATOR_HELP ;

template <class DEPEND>
trait IMAGEITERATOR_HELP<DEPEND ,ALWAYS> {
	struct PIXEL {
		INDEX x ;
		INDEX y ;
	} ;

	class ImageIterator {
	protected:
		Array<LENGTH ,RANK2> mWidth ;
		PIXEL mPixel ;
		BOOL mBad ;

	public:
		implicit ImageIterator () = delete ;

		explicit ImageIterator (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			mWidth[0] = cx_ ;
			mWidth[1] = cy_ ;
			mPixel.x = 0 ;
			mPixel.y = 0 ;
			mBad = BOOL (rank () <= 0) ;
		}

		ImageIterator begin () const {
			return thiz ;
		}

		ImageIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mWidth[0] * mWidth[1] ;
		}

		BOOL bad () const {
			return mBad ;
		}

		forceinline BOOL operator== (CREF<ImageIterator>) const {
			return bad () ;
		}

		forceinline BOOL operator!= (CREF<ImageIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<PIXEL> peek () const leftvalue {
			return mPixel ;
		}

		forceinline CREF<PIXEL> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mPixel.x++ ;
			if (mPixel.x < mWidth[0])
				return ;
			mPixel.x = 0 ;
			mPixel.y++ ;
			if (mPixel.y < mWidth[1])
				return ;
			mPixel.y = 0 ;
			mBad = TRUE ;
		}

		forceinline void operator++ () {
			next () ;
		}
	} ;
} ;

using PIXEL = typename IMAGEITERATOR_HELP<DEPEND ,ALWAYS>::PIXEL ;
using ImageIterator = typename IMAGEITERATOR_HELP<DEPEND ,ALWAYS>::ImageIterator ;

template <class...>
trait ROWPROXY_HELP ;

template <class UNIT ,class ITEM ,class COND>
trait ROWPROXY_HELP<UNIT ,ITEM ,COND ,REQUIRE<COND>> {
	class RowProxy {
	protected:
		VRef<UNIT> mThat ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<VRef<UNIT>> image ,CREF<INDEX> y_) {
			mThat = move (image) ;
			mY = y_ ;
		}

		forceinline VREF<ITEM> operator[] (CREF<INDEX> x_) rightvalue {
			return mThat->at (x_ ,mY) ;
		}
	} ;
} ;

template <class UNIT ,class ITEM ,class COND>
trait ROWPROXY_HELP<UNIT ,ITEM ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	class RowProxy {
	protected:
		CRef<UNIT> mThat ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<CRef<UNIT>> image ,CREF<INDEX> y_) {
			mThat = move (image) ;
			mY = y_ ;
		}

		forceinline CREF<ITEM> operator[] (CREF<INDEX> x_) rightvalue {
			return mThat->at (x_ ,mY) ;
		}
	} ;
} ;

template <class UNIT ,class ITEM>
using RowProxy = typename ROWPROXY_HELP<REMOVE_REF<UNIT> ,ITEM ,IS_VARIABLE<UNIT> ,ALWAYS>::RowProxy ;

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
trait IMAGE_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_SAME<SIZE ,ORDINARY>>>> {
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

		ImageIterator iter () const {
			return ImageIterator (cx () ,cy ()) ;
		}

		VREF<ITEM> at (CREF<PIXEL> xy) leftvalue {
			return at (xy.x ,xy.y) ;
		}

		forceinline VREF<ITEM> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mStrip + x_ + mOffset] ;
		}

		forceinline RowProxy<VREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) leftvalue {
			return RowProxy<VREF<Image> ,ITEM> (VRef<Image>::reference (thiz) ,y_) ;
		}

		CREF<ITEM> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		forceinline CREF<ITEM> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) const leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mStrip + x_ + mOffset] ;
		}

		forceinline RowProxy<CREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
			return RowProxy<CREF<Image> ,ITEM> (CRef<Image>::reference (thiz) ,y_) ;
		}

		BOOL equal (CREF<Image> that) const {
			if (width () != that.width ())
				return FALSE ;
			for (auto &&i : iter ()) {
				if ifnot (operator_equal (at (i) ,that.at (i)))
					return FALSE ;
			}
			return TRUE ;
		}

		forceinline BOOL operator== (CREF<Image> that) const {
			return equal (that) ;
		}

		forceinline BOOL operator!= (CREF<Image> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Image> that) const {
			const auto r1x = operator_compr (width () ,that.width ()) ;
			if (r1x != ZERO)
				return r1x ;
			for (auto &&i : iter ()) {
				const auto r2x = operator_compr (at (i) ,that.at (i)) ;
				if (r2x != ZERO)
					return r2x ;
			}
			return ZERO ;
		}

		forceinline BOOL operator< (CREF<Image> that) const {
			return compr (that) < ZERO ;
		}

		forceinline BOOL operator<= (CREF<Image> that) const {
			return compr (that) <= ZERO ;
		}

		forceinline BOOL operator> (CREF<Image> that) const {
			return compr (that) > ZERO ;
		}

		forceinline BOOL operator>= (CREF<Image> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			FLAG ret = hashcode () ;
			for (auto &&i : iter ()) {
				const auto r1x = operator_hash (at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		Image add (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) + that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator+ (CREF<Image> that) const {
			return add (that) ;
		}

		void add_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) += that.at (i) ;
		}

		forceinline void operator+= (CREF<Image> that) {
			add_with (that) ;
		}

		Image sub (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) - that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator- (CREF<Image> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) -= that.at (i) ;
		}

		forceinline void operator-= (CREF<Image> that) {
			sub_with (that) ;
		}

		Image mul (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) * that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator* (CREF<Image> that) const {
			return mul (that) ;
		}

		void mul_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) *= that.at (i) ;
		}

		forceinline void operator*= (CREF<Image> that) {
			mul_with (that) ;
		}

		Image div (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) / that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator/ (CREF<Image> that) const {
			return div (that) ;
		}

		void div_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) /= that.at (i) ;
		}

		forceinline void operator/= (CREF<Image> that) {
			div_with (that) ;
		}

		Image mod (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) % that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator% (CREF<Image> that) const {
			return mod (that) ;
		}

		void mod_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) %= that.at (i) ;
		}

		forceinline void operator%= (CREF<Image> that) {
			mod_with (that) ;
		}

		Image plus () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = +at (i) ;
			return move (ret) ;
		}

		forceinline Image operator+ () const {
			return plus () ;
		}

		Image minus () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = -at (i) ;
			return move (ret) ;
		}

		forceinline Image operator- () const {
			return minus () ;
		}

		Image band (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) & that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator& (CREF<Image> that) const {
			return band (that) ;
		}

		void band_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) &= that.at (i) ;
		}

		forceinline void operator&= (CREF<Image> that) {
			band_with (that) ;
		}

		Image bor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) | that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator| (CREF<Image> that) const {
			return bor (that) ;
		}

		void bor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) |= that.at (i) ;
		}

		forceinline void operator|= (CREF<Image> that) {
			bor_with (that) ;
		}

		Image bxor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) ^ that.at (i) ;
			return move (ret) ;
		}

		forceinline Image operator^ (CREF<Image> that) const {
			return bxor (that) ;
		}

		void bxor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) ^= that.at (i) ;
		}

		forceinline void operator^= (CREF<Image> that) {
			bxor_with (that) ;
		}

		Image bnot () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = ITEM (~at (i)) ;
			return move (ret) ;
		}

		forceinline Image operator~ () const {
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