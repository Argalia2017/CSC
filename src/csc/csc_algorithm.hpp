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
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

	class BinaryTable {
	protected:
		Allocator<NODE ,VARIABLE> mTable ;
		Array<INDEX> mHead ;

	public:
		implicit BinaryTable () = default ;

		explicit BinaryTable (CREF<LENGTH> size_) {
			mTable = Allocator<NODE ,VARIABLE> (size_) ;
			mHead = Array<INDEX> (size_) ;
			clear () ;
		}

		void clear () {
			mTable.clear () ;
			mHead.fill (NONE) ;
		}

		void link (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			INDEX ix = find (mHead[from_] ,to_) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mTable.insert () ;
				mTable[ix].mTo = to_ ;
				mTable[ix].mWeight = 1 ;
				mTable[ix].mLeft = NONE ;
				mTable[ix].mRight = NONE ;
				update_insert (mHead[from_] ,ix) ;
			}
		}

		void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			if (from_ == to_)
				return ;
			link (from_ ,to_) ;
			link (to_ ,from_) ;
		}

		BOOL get (CREF<INDEX> from_ ,CREF<INDEX> to_) const {
			INDEX ix = find (mHead[from_] ,to_) ;
			if (ix == NONE)
				return FALSE ;
			return TRUE ;
		}

		Array<INDEX> filter (CREF<INDEX> from_) const {
			const auto r1x = node_weight (mHead[from_]) ;
			Array<INDEX> ret = Array<INDEX> (r1x) ;
			INDEX ix = 0 ;
			compute_order (mHead[from_] ,ret ,ix) ;
			assert (ix == ret.length ()) ;
			return move (ret) ;
		}

	private:
		INDEX find (CREF<INDEX> root ,CREF<INDEX> item) const {
			INDEX ret = root ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = operator_compr (item ,mTable[ret].mTo) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return mTable[ret].mLeft ;
					return mTable[ret].mRight ;
				}) ;
				ret = r2x ;
			}
			return move (ret) ;
		}

		void update_insert (VREF<INDEX> curr ,CREF<INDEX> last) {
			INDEX ix = last ;
			if ifswitch (TRUE) {
				if (curr == NONE)
					discard ;
				ix = curr ;
				mTable[ix].mWeight++ ;
				const auto r1x = operator_compr (mTable[last].mTo ,mTable[ix].mTo) ;
				assert (r1x != ZERO) ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if (r1x > ZERO)
						discard ;
					update_insert (mTable[ix].mLeft ,last) ;
					update_insert_left (ix) ;
				}
				if ifswitch (rxx) {
					update_insert (mTable[ix].mRight ,last) ;
					update_insert_right (ix) ;
				}
			}
			curr = ix ;
		}

		void update_insert_left (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mTable[ix].mLeft == NONE)
				return ;
			const auto r1x = node_weight (mTable[ix].mRight) ;
			const auto r2x = node_weight (mTable[mTable[ix].mLeft].mLeft) ;
			const auto r3x = node_weight (mTable[mTable[ix].mLeft].mRight) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_left (mTable[ix].mLeft) ;
			}
			rotate_right (ix) ;
			update_insert_left (mTable[ix].mLeft) ;
			update_insert_right (mTable[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void update_insert_right (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mTable[ix].mRight == NONE)
				return ;
			const auto r1x = node_weight (mTable[ix].mLeft) ;
			const auto r2x = node_weight (mTable[mTable[ix].mRight].mRight) ;
			const auto r3x = node_weight (mTable[mTable[ix].mRight].mLeft) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_right (mTable[ix].mRight) ;
			}
			rotate_left (ix) ;
			update_insert_left (mTable[ix].mLeft) ;
			update_insert_right (mTable[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void rotate_left (VREF<INDEX> curr) {
			INDEX ix = mTable[curr].mRight ;
			mTable[curr].mRight = mTable[ix].mLeft ;
			mTable[ix].mLeft = curr ;
			mTable[ix].mWeight = mTable[curr].mWeight ;
			const auto r1x = node_weight (mTable[curr].mLeft) ;
			const auto r2x = node_weight (mTable[curr].mRight) ;
			mTable[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		void rotate_right (VREF<INDEX> curr) {
			INDEX ix = mTable[curr].mLeft ;
			mTable[curr].mLeft = mTable[ix].mRight ;
			mTable[ix].mRight = curr ;
			mTable[ix].mWeight = mTable[curr].mWeight ;
			const auto r1x = node_weight (mTable[curr].mLeft) ;
			const auto r2x = node_weight (mTable[curr].mRight) ;
			mTable[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		LENGTH node_weight (CREF<INDEX> curr) const {
			if (curr == NONE)
				return ZERO ;
			return mTable[curr].mWeight ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> range_ ,VREF<INDEX> iw) const {
			if (curr == NONE)
				return ;
			compute_order (mTable[curr].mLeft ,range_ ,iw) ;
			range_[iw] = curr ;
			iw++ ;
			compute_order (mTable[curr].mRight ,range_ ,iw) ;
		}
	} ;
} ;

using BinaryTable = typename BINARYTABLE_HELP<DEPEND ,ALWAYS>::BinaryTable ;

template <class...>
trait SEGMENTTABLE_HELP ;

template <class DEPEND>
trait SEGMENTTABLE_HELP<DEPEND ,ALWAYS> {
	using FLOAT = SINGLE ;

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
			mTableRange = mTable.range_sort () ;
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