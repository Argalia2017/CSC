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
namespace IMAGE {
template <class...>
trait IMAGE_ITERATOR_HELP ;

template <class RANK>
trait IMAGE_ITERATOR_HELP<RANK ,ALWAYS> {
	require (ENUM_GT_ZERO<RANK>) ;

	class Iterator {
	private:
		Array<LENGTH ,RANK> mWidth ;
		LENGTH mLength ;
		Array<INDEX ,RANK> mItem ;
		BOOL mGood ;

	public:
		implicit Iterator () = delete ;

		explicit Iterator (CREF<Array<LENGTH ,RANK>> width_ ,CREF<LENGTH> length_) {
			mWidth = width_ ;
			mLength = template_acc_of (PHX ,TYPEAS<ENUM_ZERO>::id) ;
			mItem.fill (0) ;
			mGood = BOOL (mLength > 0) ;
		}

		LENGTH length () const {
			return mLength ;
		}

		Iterator begin () const {
			return thiz ;
		}

		Iterator end () const {
			return thiz ;
		}

		BOOL good () const {
			return mGood ;
		}

		inline BOOL operator== (CREF<Iterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<Iterator>) const {
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
using ImageIterator = typename IMAGE_ITERATOR_HELP<RANK ,ALWAYS>::Iterator ;

template <class...>
trait ROWPROXY_HELP ;

template <class UNIT1 ,class UNIT2>
trait ROWPROXY_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	struct WRAP {
		VRef<UNIT1> mImage ;
		INDEX mY ;
	} ;

	struct WRAP_CRef {
		CRef<UNIT1> mImage ;
		INDEX mY ;
	} ;

	class RowProxy extend Proxy {
	private:
		WRAP mBase ;

	public:
		imports VREF<RowProxy> from (VREF<TEMP<RowProxy>> where_ ,VREF<UNIT1> image ,CREF<INDEX> y) {
			zeroize (where_) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<WRAP>>::id ,where_)) ;
			tmp.mImage = VRef<UNIT1>::reference (image) ;
			tmp.mY = y ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<RowProxy>>::id ,unsafe_deptr (tmp))) ;
		}

		imports CREF<RowProxy> from (VREF<TEMP<RowProxy>> where_ ,CREF<UNIT1> image ,CREF<INDEX> y) {
			zeroize (where_) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<WRAP_CRef>>::id ,where_)) ;
			tmp.mImage = CRef<UNIT1>::reference (image) ;
			tmp.mY = y ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<RowProxy>>::id ,unsafe_deptr (tmp))) ;
		}

		inline VREF<UNIT2> operator[] (CREF<INDEX> x) leftvalue {
			return mBase.mImage->at (x ,mBase.mY) ;
		}

		inline CREF<UNIT2> operator[] (CREF<INDEX> x) const leftvalue {
			return mBase.mImage->at (x ,mBase.mY) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using RowProxy = typename ROWPROXY_HELP<UNIT1 ,UNIT2 ,ALWAYS>::RowProxy ;

template <class...>
trait IMAGE_HELP ;

template <class ITEM ,class SIZE>
trait IMAGE_HELP<ITEM ,SIZE ,ALWAYS> {
	class Image {
	private:
		Buffer<ITEM ,SIZE> mImage ;
		INDEX mCX ;
		INDEX mCY ;
		INDEX mCW ;
		INDEX mCK ;

	public:
		implicit Image () {
			mImage = Buffer<ITEM ,SIZE> (0) ;
			reset () ;
		}

		explicit Image (CREF<ARRAY2<LENGTH>> width_) {
			const auto r1x = width_[0] ;
			const auto r2x = width_[1] ;
			mImage = Buffer<ITEM ,SIZE> (r1x * r2x) ;
			reset (r1x ,r2x ,r1x ,0) ;
		}

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
			return mCW * mCY ;
		}

		ARRAY2<LENGTH> width () const {
			if (mImage.size () == 0)
				return ARRAY2<LENGTH> ({0 ,0}) ;
			return ARRAY2<LENGTH> ({mCX ,mCY}) ;
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

		forceinline VREF<RegBuffer<ITEM>> raw () leftvalue {
			auto rax = TEMP<RegBuffer<ITEM>> () ;
			return RegBuffer<ITEM>::from (rax ,mImage ,0 ,mImage.size ()) ;
		}

		forceinline CREF<RegBuffer<ITEM>> raw () const leftvalue {
			auto rax = TEMP<RegBuffer<ITEM>> () ;
			return RegBuffer<ITEM>::from (rax ,mImage ,0 ,mImage.size ()) ;
		}

		void reset () {
			mCX = mImage.size () ;
			mCY = 1 ;
			mCW = mImage.size () ;
			mCK = 0 ;
		}

		void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cw_ ,CREF<LENGTH> ck_) {
			assert (cx_ >= 0) ;
			assert (cy_ >= 0) ;
			assert (cx_ <= cw_) ;
			assert (ck_ >= 0) ;
			assert (cy_ * cw_ + ck_ <= size ()) ;
			mCX = cx_ ;
			mCY = cy_ ;
			mCW = cw_ ;
			mCK = ck_ ;
		}

		void fill (CREF<ITEM> item) {
			fill (item ,0 ,size ()) ;
		}

		void fill (CREF<ITEM> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : CORE::iter (begin_ ,end_))
				mImage[i] = item ;
		}

		ImageIterator<RANK2> iter () const {
			return ImageIterator<RANK2> (width ()) ;
		}

		VREF<ITEM> at (CREF<ARRAY2<INDEX>> pair) leftvalue {
			return at (pair[0] ,pair[1]) ;
		}

		inline VREF<ITEM> operator[] (CREF<ARRAY2<INDEX>> pair) leftvalue {
			return at (pair) ;
		}

		VREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
			assert (vbetween (x ,0 ,mCX)) ;
			assert (vbetween (y ,0 ,mCY)) ;
			return mImage[y * mCW + x + mCK] ;
		}

		inline forceinline VREF<RowProxy<Image ,ITEM>> operator[] (CREF<INDEX> y) leftvalue {
			auto rax = TEMP<RowProxy<Image ,ITEM>> () ;
			return RowProxy<Image ,ITEM>::from (rax ,thiz ,y) ;
		}

		CREF<ITEM> at (CREF<ARRAY2<INDEX>> pair) const leftvalue {
			return at (pair[0] ,pair[1]) ;
		}

		inline CREF<ITEM> operator[] (CREF<ARRAY2<INDEX>> pair) const leftvalue {
			return at (pair) ;
		}

		CREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
			assert (vbetween (x ,0 ,mCX)) ;
			assert (vbetween (y ,0 ,mCY)) ;
			return mImage[y * mCW + x + mCK] ;
		}

		inline forceinline CREF<RowProxy<Image ,ITEM>> operator[] (CREF<INDEX> y) const leftvalue {
			auto rax = TEMP<RowProxy<Image ,ITEM>> () ;
			return RowProxy<Image ,ITEM>::from (rax ,thiz ,y) ;
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
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Image = typename IMAGE_HELP<ITEM ,SIZE ,ALWAYS>::Image ;
} ;
} ;

namespace CSC {
using namespace IMAGE ;
} ;