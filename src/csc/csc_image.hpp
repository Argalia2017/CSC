#pragma once

#ifndef __CSC_IMAGE__
#define __CSC_IMAGE__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class...>
trait IMAGEITERATOR_HELP ;

template <class RANK>
trait IMAGEITERATOR_HELP<RANK ,ALWAYS> {
	require (ENUM_GT_ZERO<RANK>) ;

	class ImageIterator {
	protected:
		Array<LENGTH ,RANK> mWidth ;
		LENGTH mSize ;
		Array<INDEX ,RANK> mItem ;
		BOOL mGood ;

	public:
		implicit ImageIterator () = delete ;

		explicit ImageIterator (CREF<Array<LENGTH ,RANK>> width_) {
			mWidth = width_ ;
			mSize = template_acc_of (PHX ,TYPEAS<ENUM_ZERO>::id) ;
			mItem.fill (0) ;
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

		CREF<Array<INDEX ,RANK>> at () const leftvalue {
			return mItem ;
		}

		inline CREF<Array<INDEX ,RANK>> operator* () const leftvalue {
			return at () ;
		}

		void next () {
			template_next (PHX ,TYPEAS<ENUM_ZERO>::id) ;
		}

		inline void operator++ () {
			next () ;
		}

	private:
		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<ENUM_SUB<RANK ,REMOVE_ALL<ARG1>>>>>
		LENGTH template_acc_of (CREF<typeof (PH2)> ,XREF<ARG1> id) const {
			using R1X = REMOVE_ALL<ARG1> ;
			return mWidth[ENUM_CHECK<R1X>::value] * template_acc_of (PHX ,TYPEAS<ENUM_INC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<ENUM_SUB<RANK ,REMOVE_ALL<ARG1>>>>>
		LENGTH template_acc_of (CREF<typeof (PH1)> ,XREF<ARG1> id) const {
			return IDEN ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<ENUM_SUB<RANK ,REMOVE_ALL<ARG1>>>>>
		void template_next (CREF<typeof (PH2)> ,XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			mItem[ENUM_CHECK<R1X>::value]++ ;
			if (mItem[ENUM_CHECK<R1X>::value] < mWidth[ENUM_CHECK<R1X>::value])
				return ;
			mItem[ENUM_CHECK<R1X>::value] = 0 ;
			template_next (PHX ,TYPEAS<ENUM_INC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<ENUM_SUB<RANK ,REMOVE_ALL<ARG1>>>>>
		void template_next (CREF<typeof (PH1)> ,XREF<ARG1> id) {
			mGood = FALSE ;
		}
	} ;
} ;

template <class RANK>
using ImageIterator = typename IMAGEITERATOR_HELP<RANK ,ALWAYS>::ImageIterator ;

template <class...>
trait ROWPROXY_HELP ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
trait ROWPROXY_HELP<UNIT1 ,UNIT2 ,UNIT3 ,REQUIRE<IS_VARIABLE<UNIT1>>> {
	class RowProxy {
	protected:
		VRef<UNIT2> mImage ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<VRef<UNIT2>> image ,CREF<INDEX> y) {
			mImage = move (image) ;
			mY = y ;
		}

		inline VREF<UNIT3> operator[] (CREF<INDEX> x) rightvalue {
			return mImage->at (x ,mY) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
trait ROWPROXY_HELP<UNIT1 ,UNIT2 ,UNIT3 ,REQUIRE<IS_CONSTANT<UNIT1>>> {
	class RowProxy {
	protected:
		CRef<UNIT2> mImage ;
		INDEX mY ;

	public:
		implicit RowProxy () = delete ;

		explicit RowProxy (RREF<CRef<UNIT2>> image ,CREF<INDEX> y) {
			mImage = move (image) ;
			mY = y ;
		}

		inline CREF<UNIT3> operator[] (CREF<INDEX> x) rightvalue {
			return mImage->at (x ,mY) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT3>
using RowProxy = typename ROWPROXY_HELP<XREF<UNIT1> ,REMOVE_REF<UNIT1> ,UNIT3 ,ALWAYS>::RowProxy ;

template <class...>
trait IMAGE_HELP ;

template <class ITEM ,class SIZE>
trait IMAGE_HELP<ITEM ,SIZE ,ALWAYS> {
	class Image {
	protected:
		Buffer<ITEM ,SIZE> mImage ;
		INDEX mCX ;
		INDEX mCY ;
		INDEX mCW ;
		INDEX mCK ;

	public:
		implicit Image () {
			mImage = Buffer<ITEM ,SIZE> (0) ;
			mCX = 0 ;
			mCY = 0 ;
			mCW = 0 ;
			mCK = 0 ;
		}

		explicit Image (CREF<ARRAY2<LENGTH>> width_) {
			const auto r1x = width_[0] ;
			const auto r2x = width_[1] ;
			mImage = Buffer<ITEM ,SIZE> (r1x * r2x) ;
			mCX = r1x ;
			mCY = r2x ;
			mCW = r1x ;
			mCK = 0 ;
		}

		explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
			const auto r1x = cx_ ;
			const auto r2x = cy_ ;
			mImage = Buffer<ITEM ,SIZE> (r1x * r2x) ;
			mCX = r1x ;
			mCY = r2x ;
			mCW = r1x ;
			mCK = 0 ;
		}

		LENGTH size () const {
			return mImage.size () ;
		}

		LENGTH length () const {
			return mCW * mCY ;
		}

		ARRAY2<LENGTH> width () const {
			ARRAY2<LENGTH> ret ;
			ret[0] = cx () ;
			ret[1] = cy () ;
			return move (ret) ;
		}

		LENGTH cx () const {
			if (mImage.size () == 0)
				return ZERO ;
			return mCX ;
		}

		LENGTH cy () const {
			if (mImage.size () == 0)
				return ZERO ;
			return mCY ;
		}

		LENGTH cw () const {
			if (mImage.size () == 0)
				return ZERO ;
			return mCW ;
		}

		LENGTH ck () const {
			if (mImage.size () == 0)
				return ZERO ;
			return mCK ;
		}

		void fill (CREF<ITEM> item) {
			fill (item ,0 ,size ()) ;
		}

		void fill (CREF<ITEM> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : CSC::iter (begin_ ,end_))
				mImage[i] = item ;
		}

		ImageIterator<RANK2> iter () const {
			return ImageIterator<RANK2> (width ()) ;
		}

		VREF<ITEM> at (CREF<ARRAY2<INDEX>> xy) leftvalue {
			return at (xy[0] ,xy[1]) ;
		}

		inline VREF<ITEM> operator[] (CREF<ARRAY2<INDEX>> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
			assert (vbetween (x ,0 ,mCX)) ;
			assert (vbetween (y ,0 ,mCY)) ;
			return mImage[y * mCW + x + mCK] ;
		}

		inline RowProxy<VREF<Image> ,ITEM> operator[] (CREF<INDEX> y) leftvalue {
			return RowProxy<VREF<Image> ,ITEM> (VRef<Image>::reference (thiz) ,y) ;
		}

		CREF<ITEM> at (CREF<ARRAY2<INDEX>> xy) const leftvalue {
			return at (xy[0] ,xy[1]) ;
		}

		inline CREF<ITEM> operator[] (CREF<ARRAY2<INDEX>> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
			assert (vbetween (x ,0 ,mCX)) ;
			assert (vbetween (y ,0 ,mCY)) ;
			return mImage[y * mCW + x + mCK] ;
		}

		inline RowProxy<CREF<Image> ,ITEM> operator[] (CREF<INDEX> y) const leftvalue {
			return RowProxy<CREF<Image> ,ITEM> (CRef<Image>::reference (thiz) ,y) ;
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

		void addto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) += that.at (i) ;
		}

		inline void operator+= (CREF<Image> that) {
			addto (that) ;
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

		void subto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) -= that.at (i) ;
		}

		inline void operator-= (CREF<Image> that) {
			subto (that) ;
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

		void multo (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) *= that.at (i) ;
		}

		inline void operator*= (CREF<Image> that) {
			multo (that) ;
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

		void divto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) /= that.at (i) ;
		}

		inline void operator/= (CREF<Image> that) {
			divto (that) ;
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

		void modto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) %= that.at (i) ;
		}

		inline void operator%= (CREF<Image> that) {
			modto (that) ;
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

		void bandto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) &= that.at (i) ;
		}

		inline void operator&= (CREF<Image> that) {
			bandto (that) ;
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

		void borto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) |= that.at (i) ;
		}

		inline void operator|= (CREF<Image> that) {
			borto (that) ;
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

		void bxorto (CREF<Image> that) {
			assert (width () == that.width ()) ;
			for (auto &&i : iter ())
				at (i) ^= that.at (i) ;
		}

		inline void operator^= (CREF<Image> that) {
			bxorto (that) ;
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
				ret.at (i[1] ,i[0]) = at (i) ;
			return move (ret) ;
		}

		Image matrix_product (CREF<Image> that) const {
			Image ret = Image (that.mCX ,mCY) ;
			for (auto &&i : ret.iter ()) {
				const auto r1x = invoke ([&] () {
					ITEM ret ;
					for (auto &&j : iter (0 ,mCX))
						ret += at (j ,i[1]) * that.at (i[0] ,j) ;
					return move (ret) ;
				}) ;
				ret.at (i) = r1x ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Image = typename IMAGE_HELP<ITEM ,SIZE ,ALWAYS>::Image ;
} ;