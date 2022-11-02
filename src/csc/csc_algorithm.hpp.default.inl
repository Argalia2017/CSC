#pragma once

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

		void insert_sort (CREF<Binder> binder ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb) const {
			for (auto &&i : iter (lb ,rb)) {
				INDEX ix = i + 1 ;
				INDEX iy = i ;
				const auto r1x = range_[ix] ;
				while (TRUE) {
					if (iy < lb)
						break ;
					const auto r2x = binder.friend_compare (r1x ,range_[iy]) ;
					if (r2x >= ZERO)
						break ;
					range_[ix] = range_[iy] ;
					ix = iy ;
					iy-- ;
				}
				range_[ix] = r1x ;
			}
		}

		void quick_sort (CREF<Binder> binder ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> ideal) const {
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
				quick_sort_partition (binder ,range_ ,ix ,iy ,jx) ;
				INDEX iz = jx - 1 ;
				quick_sort (binder ,range_ ,ix ,iz ,rax) ;
				ix = jx + 1 ;
			}
			if (ix >= iy)
				return ;
			insert_sort (binder ,range_ ,ix ,iy) ;
		}

		void quick_sort_partition (CREF<Binder> binder ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<INDEX> mid_one) const {
			INDEX ix = lb ;
			INDEX iy = rb ;
			const auto r1x = range_[ix] ;
			while (TRUE) {
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r2x = binder.friend_compare (range_[iy] ,r1x) ;
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

		void sort (CREF<Binder> binder ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
			const auto r1x = end_ - begin_ ;
			if (r1x <= 1)
				return ;
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			assert (vbetween (ix ,0 ,range_.size ())) ;
			assert (vbetween (iy ,0 ,range_.size ())) ;
			quick_sort (binder ,range_ ,ix ,iy ,r1x) ;
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
		LENGTH mWidth ;
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
				ret.mWidth = 0 ;
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

		void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) override {
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

		BitSet<> filter (CREF<INDEX> index ,RREF<BitSet<>> res) override {
			BitSet<> ret = move (res) ;
			const auto r1x = lead (index) ;
			for (auto &&i : mTable.iter ()) {
				if (mTable[i].mUp == NONE)
					continue ;
				INDEX ix = lead (i) ;
				if (ix != r1x)
					continue ;
				ret.add (i) ;
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
		INDEX mTo ;
		INDEX mNext ;
	} ;

	struct BINARY {
		INDEX mFirst ;
		LENGTH mLength ;
	} ;

	class ImplHolder implement Holder {
	protected:
		Allocator<NODE ,VARIABLE> mTable ;
		Array<BINARY> mBinary ;
		Array<INDEX> mJump ;

	public:
		void initialize (CREF<LENGTH> size_) override {
			mTable = Allocator<NODE ,VARIABLE> (size_) ;
			mBinary = Array<BINARY> (size_) ;
			clear () ;
		}

		void clear () override {
			mTable.clear () ;
			mJump = Array<INDEX> () ;
			for (auto &&i : mBinary) {
				i.mFirst = NONE ;
				i.mLength = 0 ;
			}
		}

		LENGTH count (CREF<INDEX> from_) const override {
			return mBinary[from_].mLength ;
		}

		void link (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
			assert (mJump.size () == 0) ;
			INDEX ix = from_ ;
			INDEX jx = find_table (mBinary[ix].mFirst ,to_) ;
			if ifswitch (TRUE) {
				if (jx != NONE)
					discard ;
				jx = mTable.alloc () ;
				mTable[jx].mTo = to_ ;
				mTable[jx].mNext = mBinary[ix].mFirst ;
				mBinary[ix].mFirst = jx ;
				mBinary[ix].mLength++ ;
			}
		}

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

		void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
			if (from_ == to_)
				return ;
			link (from_ ,to_) ;
			link (to_ ,from_) ;
		}

		BOOL get (CREF<INDEX> from_ ,CREF<INDEX> to_) const override {
			assert (mTable.size () == 0) ;
			INDEX ix = from_ ;
			INDEX iy = to_ ;
			if ifswitch (TRUE) {
				if (mBinary[ix].mLength < mBinary[iy].mLength)
					discard ;
				swap (ix ,iy) ;
			}
			const auto r1x = mBinary[ix].mFirst ;
			const auto r2x = r1x + mBinary[ix].mLength ;
			INDEX jx = find_binary (r1x ,r2x ,iy) ;
			return jx != NONE ;
		}

		INDEX find_binary (CREF<INDEX> begin_ ,CREF<INDEX> end_ ,CREF<INDEX> to_) const {
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				INDEX iz = (ix + iy) / 2 ;
				const auto r1x = operator_compr (to_ ,mJump[iz]) ;
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
			if (mJump[ix] == to_)
				return ix ;
			return NONE ;
		}

		BitSet<> filter (CREF<INDEX> from_ ,RREF<BitSet<>> res) const override {
			assert (mTable.size () == 0) ;
			BitSet<> ret = move (res) ;
			INDEX ix = from_ ;
			for (auto &&i : ret.iter ()) {
				INDEX jx = mBinary[ix].mFirst + i ;
				ret[mJump[jx]] = TRUE ;
			}
			return move (ret) ;
		}

		void optimize () override {
			mJump = Array<INDEX> (mTable.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : mBinary) {
				INDEX jx = i.mFirst ;
				i.mFirst = ix ;
				while (TRUE) {
					if (jx == NONE)
						break ;
					mJump[ix] = mTable[jx].mTo ;
					ix++ ;
					jx = mTable[jx].mNext ;
				}
			}
			mTable = Allocator<NODE ,VARIABLE> () ;
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
			set_tolerance (FLOAT (SINGLE_EPS)) ;
		}

		void set_tolerance (CREF<FLOAT> tolerance) override {
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
			mTableRange = mTable.range () ;
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