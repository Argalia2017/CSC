#pragma once

#ifndef __CSC_ALGOTIRHM__
#define __CSC_ALGOTIRHM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_geometry.hpp"

namespace CSC {
template <class...>
trait SORTPROC_HELP ;

template <class DEPEND>
trait SORTPROC_HELP<DEPEND ,ALWAYS> {
	using COMPARE = Function<BOOL ,TYPEAS<CREF<INDEX> ,CREF<INDEX>>> ;

	struct Holder implement Interface {} ;

	class SortProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<SortProc> instance () {
			return memorize ([&] () {
				SortProc ret ;
				ret.mThis = NULL ;
				return move (ret) ;
			}) ;
		}
		template <class ARG1>
		inline void operator() (CREF<ARG1> array_ ,VREF<Array<INDEX>> range_) const {
			sort (array_ ,range_ ,0 ,range_.length ()) ;
		}

		template <class ARG1>
		imports void sort (CREF<ARG1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			const auto r1x = end_ - begin_ ;
			if (r1x <= 1)
				return ;
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			assert (vbetween (ix ,0 ,range_.size ())) ;
			assert (vbetween (iy ,0 ,range_.size ())) ;
			const auto r2x = COMPARE ([&] (CREF<INDEX> a ,CREF<INDEX> b) {
				return operator_compr (array_[a] ,array_[b]) ;
			}) ;
			quick_sort (r2x ,range_ ,ix ,iy ,r1x) ;
		}

	private:
		imports void insert_sort (CREF<COMPARE> compare ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb) {
			for (auto &&i : iter (lb ,rb)) {
				INDEX ix = i + 1 ;
				INDEX iy = i ;
				const auto r1x = range_[ix] ;
				while (TRUE) {
					if (iy < lb)
						break ;
					const auto r2x = compare (r1x ,range_[iy]) ;
					if (r2x >= ZERO)
						break ;
					range_[ix] = range_[iy] ;
					ix = iy ;
					iy-- ;
				}
				range_[ix] = r1x ;
			}
		}

		imports void quick_sort_partition (CREF<COMPARE> compare ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<INDEX> mid_one) {
			INDEX ix = lb ;
			INDEX iy = rb ;
			const auto r1x = range_[ix] ;
			while (TRUE) {
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r2x = compare (range_[iy] ,r1x) ;
					if (r2x <= ZERO)
						break ;
					iy-- ;
				}
				if (ix >= iy)
					break ;
				range_[ix] = range_[iy] ;
				ix++ ;
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r3x = operator_compr (range_[ix] ,r1x) ;
					if (r3x >= ZERO)
						break ;
					ix++ ;
				}
				if (ix >= iy)
					break ;
				range_[iy] = range_[ix] ;
				iy-- ;
			}
			range_[ix] = r1x ;
			mid_one = ix ;
		}

		imports void quick_sort (CREF<COMPARE> compare ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> ideal) {
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
				quick_sort_partition (compare ,range_ ,ix ,iy ,jx) ;
				INDEX iz = jx - 1 ;
				quick_sort (compare ,range_ ,ix ,iz ,rax) ;
				ix = jx + 1 ;
			}
			if (ix >= iy)
				return ;
			insert_sort (compare ,range_ ,ix ,iy) ;
		}
	} ;
} ;

using SortProc = typename SORTPROC_HELP<DEPEND ,ALWAYS>::SortProc ;

template <class...>
trait DISJOINTTABLE_HELP ;

template <class DEPEND>
trait DISJOINTTABLE_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		INDEX mUp ;
		LENGTH mWidth ;
	} ;

	class DisjointTable {
	protected:
		Array<NODE> mTable ;

	public:
		implicit DisjointTable () = default ;

		explicit DisjointTable (CREF<LENGTH> size_) {
			mTable = Array<NODE> (size_) ;
			clear () ;
		}

		void clear () {
			const auto r1x = invoke ([&] () {
				NODE ret ;
				ret.mUp = NONE ;
				ret.mWidth = 0 ;
				return move (ret) ;
			}) ;
			mTable.fill (r1x) ;
		}

		INDEX lead (CREF<INDEX> index) {
			INDEX ret = index ;
			if ifswitch (TRUE) {
				if (mTable[ret].mUp != NONE)
					discard ;
				mTable[ret].mUp = ret ;
				mTable[ret].mWidth = 1 ;
			}
			while (TRUE) {
				if (mTable[ret].mUp == ret)
					break ;
				ret = mTable[ret].mUp ;
			}
			compress (index ,ret) ;
			return move (ret) ;
		}

		void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			INDEX ix = lead (index1) ;
			INDEX iy = lead (index2) ;
			if (ix == iy)
				return ;
			if ifswitch (TRUE) {
				if (mTable[iy].mWidth <= mTable[ix].mWidth)
					discard ;
				swap (ix ,iy) ;
			}
			mTable[iy].mUp = ix ;
			mTable[ix].mWidth += mTable[iy].mWidth ;
		}

		BitSet<> filter (CREF<INDEX> index) {
			const auto r1x = map_of_closure () ;
			const auto r2x = lead (index) ;
			BitSet<> ret = BitSet<> (mTable.size ()) ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp == NONE)
					continue ;
				INDEX ix = lead (i) ;
				if (ix != r2x)
					continue ;
				ret.add (i) ;
			}
			return move (ret) ;
		}

		Array<INDEX> linkage () {
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

		Array<BitSet<>> closure () {
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

	private:
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

		void compress (CREF<INDEX> index ,CREF<INDEX> root) {
			INDEX ix = index ;
			while (TRUE) {
				INDEX iy = ix ;
				ix = mTable[iy].mUp ;
				if (ix == root)
					break ;
				mTable[iy].mUp = root ;
				mTable[ix].mWidth -= mTable[iy].mWidth ;
			}
		}
	} ;
} ;

using DisjointTable = typename DISJOINTTABLE_HELP<DEPEND ,ALWAYS>::DisjointTable ;

template <class...>
trait BINARYTABLE_HELP ;

template <class DEPEND>
trait BINARYTABLE_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		INDEX mTo ;
		INDEX mNext ;
	} ;

	struct HEAD {
		INDEX mRoot ;
		LENGTH mLength ;
	} ;

	class BinaryTable {
	protected:
		Allocator<NODE ,VARIABLE> mTable ;
		Array<INDEX> mBinary ;
		Array<HEAD> mHead ;

	public:
		implicit BinaryTable () = default ;

		explicit BinaryTable (CREF<LENGTH> size_) {
			mTable = Allocator<NODE ,VARIABLE> (size_) ;
			mBinary = Array<INDEX> () ;
			mHead = Array<HEAD> (size_) ;
			clear () ;
		}

		void clear () {
			mTable.clear () ;
			mBinary = Array<INDEX> () ;
			for (auto &&i : mHead) {
				i.mRoot = NONE ;
				i.mLength = 0 ;
			}
		}

		void link (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			assert (mBinary.size () == 0) ;
			INDEX ix = from_ ;
			INDEX jx = find_table (mHead[ix].mRoot ,to_) ;
			if ifswitch (TRUE) {
				if (jx != NONE)
					discard ;
				jx = mTable.alloc () ;
				mTable[jx].mTo = to_ ;
				mTable[jx].mNext = mHead[ix].mRoot ;
				mHead[ix].mRoot = jx ;
				mHead[ix].mLength++ ;
			}
		}

		void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			if (from_ == to_)
				return ;
			link (from_ ,to_) ;
			link (to_ ,from_) ;
		}

		BOOL get (CREF<INDEX> from_ ,CREF<INDEX> to_) const {
			assert (mTable.size () == 0) ;
			INDEX ix = from_ ;
			INDEX iy = to_ ;
			if ifswitch (TRUE) {
				if (mHead[ix].mLength < mHead[iy].mLength)
					discard ;
				swap (ix ,iy) ;
			}
			const auto r1x = mHead[ix].mRoot ;
			const auto r2x = r1x + mHead[ix].mLength ;
			INDEX jx = find_binary (r1x ,r2x ,iy) ;
			return jx != NONE ;
		}

		Array<INDEX> filter (CREF<INDEX> from_) const {
			assert (mTable.size () == 0) ;
			INDEX ix = from_ ;
			Array<INDEX> ret = Array<INDEX> (mHead[ix].mLength) ;
			INDEX jx = mHead[ix].mRoot ;
			for (auto &&i : ret.iter ()) {
				ret[i] = jx ;
				jx = mTable[jx].mTo ;
			}
			return move (ret) ;
		}

		void optimize () {
			mBinary = Array<INDEX> (mTable.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : mHead) {
				INDEX jx = i.mRoot ;
				i.mRoot = ix ;
				while (TRUE) {
					if (jx == NONE)
						break ;
					mBinary[ix] = mTable[jx].mTo ;
					ix++ ;
					jx = mTable[jx].mNext ;
				}
			}
			mTable = Allocator<NODE ,VARIABLE> () ;
		}

	private:
		INDEX find_table (CREF<INDEX> root ,CREF<INDEX> to_) const {
			INDEX jx = root ;
			while (TRUE) {
				if (jx == NONE)
					break ;
				if (mTable[jx].mTo == to_)
					return jx ;
				jx = mTable[jx].mNext ;
			}
			return NONE ;
		}

		INDEX find_binary (CREF<INDEX> begin_ ,CREF<INDEX> end_ ,CREF<INDEX> to_) const {
			INDEX ix = begin_ ;
			INDEX iy = end_ ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				INDEX iz = (ix + iy) / 2 ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if (to_ < mBinary[iz])
						discard ;
					ix = iz ;
				}
				if ifswitch (rxx) {
					iy = iz ;
				}
			}
			if (mBinary[ix] == to_)
				return ix ;
			return NONE ;
		}
	} ;
} ;

using BinaryTable = typename BINARYTABLE_HELP<DEPEND ,ALWAYS>::BinaryTable ;

template <class...>
trait SEGMENTTABLE_HELP ;

template <class DEPEND>
trait SEGMENTTABLE_HELP<DEPEND ,ALWAYS> {
	using FLOAT = DOUBLE ;

	class SegmentTable {
	protected:
		FLOAT mTolerance ;
		Set<FLOAT> mTable ;
		Array<INDEX> mTableRange ;
		Array<INDEX> mTableOrder ;
		BitSet<> mRealLeft ;
		BitSet<> mRealRight ;
		BitSet<> mReal ;

	public:
		implicit SegmentTable () {
			set_tolerance (FLOAT (SINGLE_EPS)) ;
		}

		void set_tolerance (CREF<FLOAT> tolerance) {
			mTolerance = tolerance ;
		}

		void add (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mTableOrder[ix] ,mTableOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = TRUE ;
				mRealLeft[mTableRange[i]] = TRUE ;
				mRealRight[mTableRange[i + 1]] = TRUE ;
			}
		}

		void erase (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mTableOrder[ix] ,mTableOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = FALSE ;
				mRealLeft[mTableRange[i]] = FALSE ;
				mRealRight[mTableRange[i + 1]] = FALSE ;
			}
		}

		FLOAT percent (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
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

	private:
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
			mTableRange = mTable.range (SortProc::instance ()) ;
			for (auto &&i : mTableRange.iter ())
				mTableOrder[mTableRange[i]] = i ;
			if ifswitch (TRUE) {
				if (mRealLeft.size () == mTable.size ())
					discard ;
				const auto r1x = Array<INDEX>::make (mRealLeft.iter ()) ;
				mRealLeft = BitSet<> (mTable.size ()) ;
				for (auto &&i : r1x)
					mRealLeft[i] = TRUE ;
			}
			if ifswitch (TRUE) {
				if (mRealRight.size () == mTable.size ())
					discard ;
				const auto r2x = Array<INDEX>::make (mRealRight.iter ()) ;
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
	} ;
} ;

using SegmentTable = typename SEGMENTTABLE_HELP<DEPEND ,ALWAYS>::SegmentTable ;
} ;