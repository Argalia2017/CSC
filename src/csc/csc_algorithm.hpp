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
trait DISJOINT_HELP ;

template <class DEPEND>
trait DISJOINT_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		INDEX mUp ;
		LENGTH mWidth ;
	} ;

	class Disjoint {
	protected:
		Array<NODE> mTable ;

	public:
		implicit Disjoint () = default ;

		explicit Disjoint (CREF<LENGTH> size_) {
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
			for (auto &&i : ret.iter ())
				ret[i] = BitSet<> (mTable.size ()) ;
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

using Disjoint = typename DISJOINT_HELP<DEPEND ,ALWAYS>::Disjoint ;

template <class...>
trait MSTREE_HELP ;

template <class DEPEND>
trait MSTREE_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		INDEX mTo ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

	class MSTree {
	protected:
		List<NODE> mTree ;
		Array<INDEX> mHead ;

	public:
		implicit MSTree () = default ;

		explicit MSTree (CREF<LENGTH> size_) {
			mTree = ArrayList<NODE> (size_) ;
			mHead = Array<INDEX> (size_) ;
			clear () ;
		}

		void clear () {
			mTree.clear () ;
			mHead.fill (NONE) ;
		}

		void link (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			INDEX ix = find (mHead[from_] ,to_) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mTree.insert () ;
				mTree[ix].mTo = to_ ;
				mTree[ix].mWeight = 1 ;
				mTree[ix].mLeft = NONE ;
				mTree[ix].mRight = NONE ;
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
				const auto r1x = operator_compr (item ,mTree[ret].mTo) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return mTree[ret].mLeft ;
					return mTree[ret].mRight ;
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
				mTree[ix].mWeight++ ;
				const auto r1x = operator_compr (mTree[last].mTo ,mTree[ix].mTo) ;
				assert (r1x != ZERO) ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if (r1x > ZERO)
						discard ;
					update_insert (mTree[ix].mLeft ,last) ;
					update_insert_left (ix) ;
				}
				if ifswitch (eax) {
					update_insert (mTree[ix].mRight ,last) ;
					update_insert_right (ix) ;
				}
			}
			curr = ix ;
		}

		void update_insert_left (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mTree[ix].mLeft == NONE)
				return ;
			const auto r1x = node_weight (mTree[ix].mRight) ;
			const auto r2x = node_weight (mTree[mTree[ix].mLeft].mLeft) ;
			const auto r3x = node_weight (mTree[mTree[ix].mLeft].mRight) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_left (mTree[ix].mLeft) ;
			}
			rotate_right (ix) ;
			update_insert_left (mTree[ix].mLeft) ;
			update_insert_right (mTree[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void update_insert_right (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mTree[ix].mRight == NONE)
				return ;
			const auto r1x = node_weight (mTree[ix].mLeft) ;
			const auto r2x = node_weight (mTree[mTree[ix].mRight].mRight) ;
			const auto r3x = node_weight (mTree[mTree[ix].mRight].mLeft) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_right (mTree[ix].mRight) ;
			}
			rotate_left (ix) ;
			update_insert_left (mTree[ix].mLeft) ;
			update_insert_right (mTree[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void rotate_left (VREF<INDEX> curr) {
			INDEX ix = mTree[curr].mRight ;
			mTree[curr].mRight = mTree[ix].mLeft ;
			mTree[ix].mLeft = curr ;
			mTree[ix].mWeight = mTree[curr].mWeight ;
			const auto r1x = node_weight (mTree[curr].mLeft) ;
			const auto r2x = node_weight (mTree[curr].mRight) ;
			mTree[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		void rotate_right (VREF<INDEX> curr) {
			INDEX ix = mTree[curr].mLeft ;
			mTree[curr].mLeft = mTree[ix].mRight ;
			mTree[ix].mRight = curr ;
			mTree[ix].mWeight = mTree[curr].mWeight ;
			const auto r1x = node_weight (mTree[curr].mLeft) ;
			const auto r2x = node_weight (mTree[curr].mRight) ;
			mTree[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		LENGTH node_weight (CREF<INDEX> curr) const {
			if (curr == NONE)
				return ZERO ;
			return mTree[curr].mWeight ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> range_ ,VREF<INDEX> iw) const {
			if (curr == NONE)
				return ;
			compute_order (mTree[curr].mLeft ,range_ ,iw) ;
			range_[iw++] = curr ;
			compute_order (mTree[curr].mRight ,range_ ,iw) ;
		}
	} ;
} ;

using MSTree = typename MSTREE_HELP<DEPEND ,ALWAYS>::MSTree ;

template <class...>
trait SEGMENTSET_HELP ;

template <class DEPEND>
trait SEGMENTSET_HELP<DEPEND ,ALWAYS> {
	using FLOAT = SINGLE ;

	class SegmentTable {
	protected:
		FLOAT mTolerance ;
		Set<FLOAT> mSegmentSet ;
		Array<INDEX> mSegmentSetRange ;
		Array<INDEX> mSegmentSetOrder ;
		BitSet<> mRealLeft ;
		BitSet<> mRealRight ;
		BitSet<> mReal ;

	public:
		implicit SegmentTable () {
			mTolerance = FLOAT (SINGLE_EPS) ;
		}

		void add (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mSegmentSetOrder[ix] ,mSegmentSetOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = TRUE ;
				mRealLeft[mSegmentSetRange[i]] = TRUE ;
				mRealRight[mSegmentSetRange[i + 1]] = TRUE ;
			}
		}

		void erase (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			const auto r1x = MathProc::sort_of (mSegmentSetOrder[ix] ,mSegmentSetOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				mReal[i] = FALSE ;
				mRealLeft[mSegmentSetRange[i]] = FALSE ;
				mRealRight[mSegmentSetRange[i + 1]] = FALSE ;
			}
		}

		FLOAT percent (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			INDEX ix = insert (lb) ;
			INDEX iy = insert (rb) ;
			FLOAT ret = FLOAT (0) ;
			const auto r1x = MathProc::sort_of (mSegmentSetOrder[ix] ,mSegmentSetOrder[iy]) ;
			for (auto &&i : iter (r1x[0] ,r1x[1])) {
				if ifnot (mReal[i])
					continue ;
				ret += mSegmentSet[mSegmentSetRange[i]] - mSegmentSet[mSegmentSetRange[i + 1]] ;
			}
			const auto r2x = mSegmentSet[ix] - mSegmentSet[iy] ;
			ret *= MathProc::inverse (r2x) ;
			return move (ret) ;
		}

	private:
		INDEX insert (CREF<FLOAT> point) {
			const auto r1x = MathProc::round (point ,mTolerance) ;
			INDEX ret = mSegmentSet.find (r1x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				mSegmentSet.add (r1x) ;
				update_range () ;
				ret = mSegmentSet.find (r1x) ;
			}
			return move (ret) ;
		}

		void update_range () {
			if ifswitch (TRUE) {
				if (mSegmentSetOrder.size () == mSegmentSet.size ())
					discard ;
				mSegmentSetOrder = Array<INDEX> (mSegmentSet.size ()) ;
				mSegmentSetOrder.fill (NONE) ;
			}
			mSegmentSetRange = mSegmentSet.range_sort () ;
			for (auto &&i : mSegmentSetRange.iter ())
				mSegmentSetOrder[mSegmentSetRange[i]] = i ;
			if ifswitch (TRUE) {
				if (mRealLeft.size () == mSegmentSet.size ())
					discard ;
				const auto r1x = Array<INDEX>::make (mRealLeft.iter ()) ;
				mRealLeft = BitSet<> (mSegmentSet.size () ,r1x) ;
			}
			if ifswitch (TRUE) {
				if (mRealRight.size () == mSegmentSet.size ())
					discard ;
				const auto r2x = Array<INDEX>::make (mRealRight.iter ()) ;
				mRealRight = BitSet<> (mSegmentSet.size () ,r2x) ;
			}
			mReal = BitSet<> (mSegmentSetRange.size ()) ;
			for (auto &&i : iter (0 ,mSegmentSetRange.length () - 1)) {
				if ifnot (mRealLeft[mSegmentSetRange[i]])
					if ifnot (mRealRight[mSegmentSetRange[i + 1]])
						continue ;
				mReal[i] = TRUE ;
				mRealLeft[mSegmentSetRange[i]] = TRUE ;
				mRealRight[mSegmentSetRange[i + 1]] = TRUE ;
			}
		}
	} ;
} ;
} ;