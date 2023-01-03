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
		LENGTH mSize ;
		PIXEL mPixel ;
		BOOL mGood ;

	public:
		implicit ImageIterator () = delete ;

		explicit ImageIterator (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			mWidth[0] = cx_ ;
			mWidth[1] = cy_ ;
			mSize = mWidth[0] * mWidth[1] ;
			mPixel.x = 0 ;
			mPixel.y = 0 ;
			mGood = BOOL (mSize > 0) ;
		}

		LENGTH length () const {
			return mSize ;
		}

		ImageIterator begin () const {
			return thiz ;
		}

		ImageIterator end () const {
			return thiz ;
		}

		BOOL good () const {
			return mGood ;
		}

		inline BOOL operator== (CREF<ImageIterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<ImageIterator>) const {
			return good () ;
		}

		CREF<PIXEL> peek () const leftvalue {
			return mPixel ;
		}

		inline CREF<PIXEL> operator* () const leftvalue {
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
			mGood = FALSE ;
		}

		inline void operator++ () {
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
		VRef<UNIT> mImage ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<VRef<UNIT>> image ,CREF<INDEX> y_) {
			mImage = move (image) ;
			mY = y_ ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> x_) rightvalue {
			return mImage->at (x_ ,mY) ;
		}
	} ;
} ;

template <class UNIT ,class ITEM ,class COND>
trait ROWPROXY_HELP<UNIT ,ITEM ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	class RowProxy {
	protected:
		CRef<UNIT> mImage ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<CRef<UNIT>> image ,CREF<INDEX> y_) {
			mImage = move (image) ;
			mY = y_ ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> x_) rightvalue {
			return mImage->at (x_ ,mY) ;
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
		INDEX mCW ;
		INDEX mCK ;

	public:
		implicit Image () noexcept {
			mCX = 0 ;
			mCY = 0 ;
			mCW = 0 ;
			mCK = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait IMAGE_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_SAME<SIZE ,PROPERTY>>>> {
	using SUPER = typename IMAGE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Image ;

	class Image extend SUPER {
	protected:
		using SUPER::mImage ;
		using SUPER::mCX ;
		using SUPER::mCY ;
		using SUPER::mCW ;
		using SUPER::mCK ;

	public:
		implicit Image () = default ;

		explicit Image (CREF<ARRAY2<LENGTH>> width_)
			:Image (width_[0] ,width_[1]) {}

		explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			const auto r1x = cx_ ;
			const auto r2x = cy_ ;
			mImage = Buffer<ITEM ,SIZE> (r1x * r2x) ;
			reset (r1x ,r2x ,r1x ,0) ;
		}

		LENGTH size () const {
			return mImage.size () ;
		}

		LENGTH length () const {
			return mCY * mCW + mCK ;
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

		LENGTH cw () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCW ;
		}

		LENGTH ck () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCK ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,size () ,move (unnamed)) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,length () ,move (unnamed)) ;
		}

		void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cw_ ,CREF<LENGTH> ck_) {
			assert (cx_ >= 0) ;
			assert (cy_ >= 0) ;
			assert (cw_ >= 0) ;
			assert (ck_ >= 0) ;
			assert (cx_ <= cw_) ;
			assert (length () <= size ()) ;
			mCX = cx_ ;
			mCY = cy_ ;
			mCW = cw_ ;
			mCK = ck_ ;
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

		inline VREF<ITEM> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mCW + x_ + mCK] ;
		}

		inline RowProxy<VREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) leftvalue {
			return RowProxy<VREF<Image> ,ITEM> (VRef<Image>::reference (thiz) ,y_) ;
		}

		CREF<ITEM> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline CREF<ITEM> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) const leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mCW + x_ + mCK] ;
		}

		inline RowProxy<CREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
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
			for (auto &&i : iter ()) {
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

		inline Image operator+ (CREF<Image> that) const {
			return add (that) ;
		}

		void add_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) += that.at (i) ;
		}

		inline void operator+= (CREF<Image> that) {
			add_with (that) ;
		}

		Image sub (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) - that.at (i) ;
			return move (ret) ;
		}

		inline Image operator- (CREF<Image> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) -= that.at (i) ;
		}

		inline void operator-= (CREF<Image> that) {
			sub_with (that) ;
		}

		Image mul (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) * that.at (i) ;
			return move (ret) ;
		}

		inline Image operator* (CREF<Image> that) const {
			return mul (that) ;
		}

		void mul_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) *= that.at (i) ;
		}

		inline void operator*= (CREF<Image> that) {
			mul_with (that) ;
		}

		Image div (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) / that.at (i) ;
			return move (ret) ;
		}

		inline Image operator/ (CREF<Image> that) const {
			return div (that) ;
		}

		void div_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) /= that.at (i) ;
		}

		inline void operator/= (CREF<Image> that) {
			div_with (that) ;
		}

		Image mod (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) % that.at (i) ;
			return move (ret) ;
		}

		inline Image operator% (CREF<Image> that) const {
			return mod (that) ;
		}

		void mod_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) %= that.at (i) ;
		}

		inline void operator%= (CREF<Image> that) {
			mod_with (that) ;
		}

		Image plus () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = +at (i) ;
			return move (ret) ;
		}

		inline Image operator+ () const {
			return plus () ;
		}

		Image minus () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = -at (i) ;
			return move (ret) ;
		}

		inline Image operator- () const {
			return minus () ;
		}

		Image band (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) & that.at (i) ;
			return move (ret) ;
		}

		inline Image operator& (CREF<Image> that) const {
			return band (that) ;
		}

		void band_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) &= that.at (i) ;
		}

		inline void operator&= (CREF<Image> that) {
			band_with (that) ;
		}

		Image bor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) | that.at (i) ;
			return move (ret) ;
		}

		inline Image operator| (CREF<Image> that) const {
			return bor (that) ;
		}

		void bor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) |= that.at (i) ;
		}

		inline void operator|= (CREF<Image> that) {
			bor_with (that) ;
		}

		Image bxor (CREF<Image> that) const {
			assert (width () == that.width ()) ;
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = at (i) ^ that.at (i) ;
			return move (ret) ;
		}

		inline Image operator^ (CREF<Image> that) const {
			return bxor (that) ;
		}

		void bxor_with (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) ^= that.at (i) ;
		}

		inline void operator^= (CREF<Image> that) {
			bxor_with (that) ;
		}

		Image bnot () const {
			Image ret = Image (width ()) ;
			for (auto &&i : iter ())
				ret.at (i) = ITEM (~at (i)) ;
			return move (ret) ;
		}

		inline Image operator~ () const {
			return bnot () ;
		}

		Image transpose () const {
			Image ret = Image (mCY ,mCX) ;
			for (auto &&i : iter ())
				ret.at (i.y ,i.x) = at (i) ;
			return move (ret) ;
		}

		Image product (CREF<Image> that) const {
			Image ret = Image (that.mCX ,mCY) ;
			for (auto &&i : ret.iter ()) {
				const auto r1x = invoke ([&] () {
					ITEM ret = ITEM (0) ;
					for (auto &&j : CSC::iter (0 ,mCX))
						ret += at (j ,i.y) * that.at (i.x ,j) ;
					return move (ret) ;
				}) ;
				ret.at (i) = r1x ;
			}
			return move (ret) ;
		}

		Image convolute (CREF<Image> that) const {
			unimplemented () ;
			return bad (TYPEAS<Image>::expr) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait IMAGE_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,PROPERTY>>> {
	using SUPER = typename IMAGE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Image ;

	class Image extend SUPER {
	protected:
		using SUPER::mImage ;
		using SUPER::mCX ;
		using SUPER::mCY ;
		using SUPER::mCW ;
		using SUPER::mCK ;

	public:
		implicit Image () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Image ,ARG1>>>>>
		implicit Image (RREF<ARG1> that) {
			require (IS_CLASS<ARG1>) ;
			const auto r1x = that.cx () ;
			const auto r2x = that.cy () ;
			mImage = Buffer<ITEM ,SIZE> (move (that)) ;
			reset (r1x ,r2x ,r1x ,0) ;
		}

		Auto native () const leftvalue {
			return mImage.native () ;
		}

		LENGTH size () const {
			return mImage.size () ;
		}

		LENGTH length () const {
			return mCY * mCW + mCK ;
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

		LENGTH cw () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCW ;
		}

		LENGTH ck () const {
			if (mImage.size () == 0)
				return 0 ;
			return mCK ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,size () ,move (unnamed)) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return RegBuffer<ITEM>::from (mImage ,0 ,length () ,move (unnamed)) ;
		}

		void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cw_ ,CREF<LENGTH> ck_) {
			assert (cx_ >= 0) ;
			assert (cy_ >= 0) ;
			assert (cw_ >= 0) ;
			assert (ck_ >= 0) ;
			assert (cx_ <= cw_) ;
			assert (length () <= mImage.size ()) ;
			mCX = cx_ ;
			mCY = cy_ ;
			mCW = cw_ ;
			mCK = ck_ ;
		}

		void fill (CREF<ITEM> item) {
			for (auto &&i : CSC::iter (0 ,size ()))
				mImage[i] = item ;
		}

		ImageIterator iter () const {
			return ImageIterator (cx () ,cy ()) ;
		}

		VREF<ITEM> at (CREF<PIXEL> xy) leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline VREF<ITEM> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mCW + x_ + mCK] ;
		}

		inline RowProxy<VREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) leftvalue {
			return RowProxy<VREF<Image> ,ITEM> (VRef<Image>::reference (thiz) ,y_) ;
		}

		CREF<ITEM> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline CREF<ITEM> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) const leftvalue {
			assert (vbetween (x_ ,0 ,mCX)) ;
			assert (vbetween (y_ ,0 ,mCY)) ;
			return mImage[y_ * mCW + x_ + mCK] ;
		}

		inline RowProxy<CREF<Image> ,ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
			return RowProxy<CREF<Image> ,ITEM> (CRef<Image>::reference (thiz) ,y_) ;
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