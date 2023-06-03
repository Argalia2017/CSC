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

#ifndef __CSC_ALGORITHM__
#error "∑(っ°Д° ;)っ : require 'csc_algorithm.hpp'"
#endif

#include "csc_algorithm.hpp"

namespace CSC {
template <class DEPEND>
trait SORTPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename SORTPROC_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename SORTPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		void insert_sort (CREF<Binder> array_ ,VREF<Array<INDEX>> result ,CREF<INDEX> lb ,CREF<INDEX> rb) const {
			for (auto &&i : iter (lb ,rb)) {
				INDEX ix = i + 1 ;
				INDEX iy = i ;
				const auto r1x = result[ix] ;
				while (TRUE) {
					if (iy < lb)
						break ;
					const auto r2x = array_.friend_compare (r1x ,result[iy]) ;
					if (r2x >= ZERO)
						break ;
					result[ix] = result[iy] ;
					ix = iy ;
					iy-- ;
				}
				result[ix] = r1x ;
			}
		}

		void quick_sort (CREF<Binder> array_ ,VREF<Array<INDEX>> result ,CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> ideal) const {
			auto rax = ideal ;
			INDEX ix = lb ;
			INDEX iy = rb ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (rax <= 0)
					break ;
				rax = rax / 2 + rax / 4 ;
				INDEX jx = NONE ;
				quick_sort_partition (array_ ,result ,ix ,iy ,jx) ;
				INDEX iz = jx - 1 ;
				quick_sort (array_ ,result ,ix ,iz ,rax) ;
				ix = jx + 1 ;
			}
			if (ix >= iy)
				return ;
			insert_sort (array_ ,result ,ix ,iy) ;
		}

		void quick_sort_partition (CREF<Binder> array_ ,VREF<Array<INDEX>> result ,CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<INDEX> mid_one) const {
			INDEX ix = lb ;
			INDEX iy = rb ;
			const auto r1x = result[ix] ;
			while (TRUE) {
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r2x = array_.friend_compare (result[iy] ,r1x) ;
					if (r2x <= ZERO)
						break ;
					iy-- ;
				}
				if (ix >= iy)
					break ;
				result[ix] = result[iy] ;
				ix++ ;
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r3x = array_.friend_compare (result[ix] ,r1x) ;
					if (r3x >= ZERO)
						break ;
					ix++ ;
				}
				if (ix >= iy)
					break ;
				result[iy] = result[ix] ;
				iy-- ;
			}
			result[ix] = r1x ;
			mid_one = ix ;
		}

		void sort (CREF<Binder> array_ ,VREF<Array<INDEX>> result ,CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
			const auto r1x = end_ - begin_ ;
			if (r1x <= 1)
				return ;
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			assert (vbetween (ix ,0 ,result.size ())) ;
			assert (vbetween (iy ,0 ,result.size ())) ;
			quick_sort (array_ ,result ,ix ,iy ,r1x) ;
		}
	} ;
} ;

template <>
exports auto SORTPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SORTPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait DISJOINTTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename DISJOINTTABLE_HELP<DEPEND ,ALWAYS>::Holder ;

	struct NODE {
		INDEX mUp ;
		LENGTH mLength ;
	} ;

	class ImplHolder implement Holder {
	protected:
		Array<NODE> mTable ;

	public:
		void initialize (CREF<LENGTH> size_) override {
			mTable = Array<NODE> (size_) ;
			clear () ;
		}

		void clear () override {
			const auto r1x = invoke ([&] () {
				NODE ret ;
				ret.mUp = NONE ;
				ret.mLength = 0 ;
				return move (ret) ;
			}) ;
			mTable.fill (r1x) ;
		}

		INDEX lead (CREF<INDEX> index) override {
			INDEX ret = index ;
			if ifswitch (TRUE) {
				if (mTable[ret].mUp != NONE)
					discard ;
				mTable[ret].mUp = ret ;
				mTable[ret].mLength = 1 ;
			}
			while (TRUE) {
				if (mTable[ret].mUp == ret)
					break ;
				ret = mTable[ret].mUp ;
			}
			compress (index ,ret) ;
			return move (ret) ;
		}

		void compress (CREF<INDEX> index ,CREF<INDEX> root) {
			INDEX ix = index ;
			while (TRUE) {
				INDEX iy = ix ;
				ix = mTable[iy].mUp ;
				if (ix == root)
					break ;
				mTable[iy].mUp = root ;
				mTable[ix].mLength -= mTable[iy].mLength ;
			}
		}

		void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) override {
			INDEX ix = lead (index1) ;
			INDEX iy = lead (index2) ;
			if (ix == iy)
				return ;
			if ifswitch (TRUE) {
				if (mTable[iy].mLength <= mTable[ix].mLength)
					discard ;
				swap (ix ,iy) ;
			}
			mTable[iy].mUp = ix ;
			mTable[ix].mLength += mTable[iy].mLength ;
		}

		Array<INDEX> filter (CREF<INDEX> index) override {
			INDEX ix = lead (index) ;
			Array<INDEX> ret = Array<INDEX> (mTable[ix].mLength) ;
			INDEX jx = 0 ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp == NONE)
					continue ;
				INDEX iy = lead (i) ;
				if (iy != ix)
					continue ;
				ret[jx] = i ;
				jx++ ;
			}
			return move (ret) ;
		}

		Array<INDEX> linkage () override {
			Array<INDEX> ret = Array<INDEX> (mTable.size ()) ;
			ret.fill (NONE) ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp == NONE)
					continue ;
				INDEX ix = lead (i) ;
				if (ix == i)
					continue ;
				ret[i] = ret[ix] ;
				ret[ix] = i ;
			}
			return move (ret) ;
		}

		Array<BitSet<>> closure () override {
			const auto r1x = map_of_closure () ;
			Array<BitSet<>> ret = Array<BitSet<>> (r1x.length ()) ;
			for (auto &&i : ret)
				i = BitSet<> (mTable.size ()) ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp == NONE)
					continue ;
				INDEX ix = lead (i) ;
				INDEX iy = r1x.map (ix) ;
				ret[iy].add (i) ;
			}
			return move (ret) ;
		}

		Set<INDEX> map_of_closure () const {
			Set<INDEX> ret = Set<INDEX> (mTable.length ()) ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp != i)
					continue ;
				const auto r1x = ret.length () ;
				ret.add (i ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto DISJOINTTABLE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename DISJOINTTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BINARYTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BINARYTABLE_HELP<DEPEND ,ALWAYS>::Holder ;

	struct NODE {
		INDEX mFirst ;
		LENGTH mLength ;
	} ;

	class ImplHolder implement Holder {
	protected:
		Deque<ARRAY2<INDEX>> mTable ;
		Array<NODE> mBinary ;
		Array<INDEX> mJump ;

	public:
		void initialize (CREF<LENGTH> size_) override {
			mTable = Deque<ARRAY2<INDEX>> (size_) ;
			mBinary = Array<NODE> (size_) ;
			clear () ;
		}

		void clear () override {
			mTable.clear () ;
			mJump = Array<INDEX> () ;
			const auto r1x = invoke ([&] () {
				NODE ret ;
				ret.mFirst = NONE ;
				ret.mLength = 0 ;
				return move (ret) ;
			}) ;
			mBinary.fill (r1x) ;
		}

		void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) override {
			assert (mJump.size () == 0) ;
			const auto r1x = ARRAY2<INDEX> ({index1 ,index2}) ;
			mTable.add (r1x) ;
			if (index1 == index2)
				return ;
			const auto r2x = ARRAY2<INDEX> ({index2 ,index1}) ;
			mTable.add (r2x) ;
		}

		BOOL get (CREF<INDEX> index1 ,CREF<INDEX> index2) const override {
			assert (mTable.size () == 0) ;
			INDEX ix = index1 ;
			INDEX iy = index2 ;
			if ifswitch (TRUE) {
				if (mBinary[ix].mLength < mBinary[iy].mLength)
					discard ;
				swap (ix ,iy) ;
			}
			const auto r1x = mBinary[ix].mFirst ;
			const auto r2x = r1x + mBinary[ix].mLength ;
			const auto r3x = find_binary (r1x ,r2x ,iy) ;
			return r3x != NONE ;
		}

		INDEX find_binary (CREF<INDEX> begin_ ,CREF<INDEX> end_ ,CREF<INDEX> item) const {
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				INDEX iz = (ix + iy) / 2 ;
				const auto r1x = operator_compr (item ,mJump[iz]) ;
				auto act = TRUE ;
				if ifswitch (act) {
					if (r1x != 0)
						discard ;
					ix = iz ;
					iy = iz ;
				}
				if ifswitch (act) {
					if (r1x < 0)
						discard ;
					ix = iz + 1 ;
				}
				if ifswitch (act) {
					iy = iz - 1 ;
				}
			}
			if (mJump[ix] == item)
				return ix ;
			return NONE ;
		}

		Array<INDEX> filter (CREF<INDEX> index) const override {
			assert (mTable.size () == 0) ;
			Array<INDEX> ret = Array<INDEX> (mBinary[index].mLength) ;
			INDEX ix = 0 ;
			const auto r1x = mBinary[index].mFirst ;
			while (TRUE) {
				if (ix >= ret.size ())
					break ;
				ret[ix] = mJump[r1x + ix] ;
				ix++ ;
			}
			return move (ret) ;
		}

		void remap () override {
			mJump = Array<INDEX> (mTable.length ()) ;
			mJump.fill (NONE) ;
			INDEX ix = NONE ;
			INDEX jx = 0 ;
			const auto r1x = SortProc::sort (mTable) ;
			for (auto &&i : r1x.iter ()) {
				if ifswitch (TRUE) {
					if (ix == mTable[r1x[i]][0])
						discard ;
					ix = mTable[r1x[i]][0] ;
					mBinary[ix].mFirst = jx ;
					if (ix == 0)
						discard ;
					mBinary[ix - 1].mLength = jx - mBinary[ix - 1].mFirst ;
				}
				if ifswitch (TRUE) {
					if (jx >= 1)
						if (mJump[jx - 1] == mTable[r1x[i]][1])
							discard ;
					mJump[jx] = mTable[r1x[i]][1] ;
					jx++ ;
				}
			}
			mBinary[ix].mLength = jx - mBinary[ix].mFirst ;
			mTable = Deque<ARRAY2<INDEX>> () ;
		}
	} ;
} ;

template <>
exports auto BINARYTABLE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BINARYTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait SEGMENTTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SEGMENTTABLE_HELP<DEPEND ,ALWAYS>::Holder ;
	using FLOAT = typename SEGMENTTABLE_HELP<DEPEND ,ALWAYS>::FLOAT ;

	class ImplHolder implement Holder {
	protected:
		FLOAT mTolerance ;
		Set<FLOAT> mTable ;
		Array<INDEX> mTableRange ;
		Array<INDEX> mTableOrder ;
		BitSet<> mRealLeft ;
		BitSet<> mRealRight ;
		BitSet<> mReal ;

	public:
		void initialize () override {
			set_tolerance (FLOAT (1)) ;
		}

		void set_tolerance (CREF<FLOAT> tolerance) override {
			assume (mTable.length () == 0) ;
			mTolerance = tolerance ;
		}

		void add (CREF<FLOAT> lb ,CREF<FLOAT> rb) override {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mTableOrder[ix] ,mTableOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = TRUE ;
				mRealLeft[mTableRange[i]] = TRUE ;
				mRealRight[mTableRange[i + 1]] = TRUE ;
			}
		}

		INDEX insert (CREF<FLOAT> point) {
			const auto r1x = MathProc::round (point ,mTolerance) ;
			INDEX ret = mTable.find (r1x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				mTable.add (r1x) ;
				update_range () ;
				ret = mTable.find (r1x) ;
			}
			return move (ret) ;
		}

		void update_range () {
			if ifswitch (TRUE) {
				if (mTableOrder.size () == mTable.size ())
					discard ;
				mTableOrder = Array<INDEX> (mTable.size ()) ;
				mTableOrder.fill (NONE) ;
			}
			mTableRange = mTable.esort () ;
			for (auto &&i : mTableRange.iter ())
				mTableOrder[mTableRange[i]] = i ;
			if ifswitch (TRUE) {
				if (mRealLeft.size () == mTable.size ())
					discard ;
				const auto r1x = IterArray<INDEX>::make (mRealLeft.iter ()) ;
				mRealLeft = BitSet<> (mTable.size ()) ;
				for (auto &&i : r1x)
					mRealLeft[i] = TRUE ;
			}
			if ifswitch (TRUE) {
				if (mRealRight.size () == mTable.size ())
					discard ;
				const auto r2x = IterArray<INDEX>::make (mRealRight.iter ()) ;
				mRealRight = BitSet<> (mTable.size ()) ;
				for (auto &&i : r2x)
					mRealRight[i] = TRUE ;
			}
			mReal = BitSet<> (mTableRange.size ()) ;
			for (auto &&i : iter (0 ,mTableRange.length () - 1)) {
				if ifnot (mRealLeft[mTableRange[i]])
					if ifnot (mRealRight[mTableRange[i + 1]])
						continue ;
				mReal[i] = TRUE ;
				mRealLeft[mTableRange[i]] = TRUE ;
				mRealRight[mTableRange[i + 1]] = TRUE ;
			}
		}

		void erase (CREF<FLOAT> lb ,CREF<FLOAT> rb) override {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mTableOrder[ix] ,mTableOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = FALSE ;
				mRealLeft[mTableRange[i]] = FALSE ;
				mRealRight[mTableRange[i + 1]] = FALSE ;
			}
		}

		FLOAT percent (CREF<FLOAT> lb ,CREF<FLOAT> rb) override {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			FLOAT ret = FLOAT (0) ;
			const auto r1x = MathProc::sort_of (mTableOrder[ix] ,mTableOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				if ifnot (mReal[i])
					continue ;
				ret += mTable[mTableRange[i]] - mTable[mTableRange[i + 1]] ;
			}
			const auto r2x = mTable[ix] - mTable[iy] ;
			ret *= MathProc::inverse (r2x) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto SEGMENTTABLE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SEGMENTTABLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;