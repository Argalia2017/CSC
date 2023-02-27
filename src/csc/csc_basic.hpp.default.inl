#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require 'csc_basic.hpp'"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
template <class DEPEND>
trait BUFFERPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BUFFERPROC_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}
	} ;
} ;

template <>
exports auto BUFFERPROC_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BUFFERPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait PINMUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PINMUTEX_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<std::mutex> mMutex ;

	public:
		void initialize () override {
			mMutex = Box<std::mutex>::make () ;
		}

		void enter () override {
			mMutex->lock () ;
		}

		void leave () override {
			mMutex->unlock () ;
		}
	} ;
} ;

template <>
exports auto PINMUTEX_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename PINMUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	struct HEAP ;

	using Holder = typename LATER_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using HEAP_SIZE = ENUMAS<VAL ,256> ;

	struct OWNERSHIP {
		VRef<HEAP> mHeap ;
		INDEX mIndex ;
	} ;

	struct NODE {
		FLAG mTag ;
		Function<Auto> mExpr ;
		SharedRef<UniqueRef<OWNERSHIP>> mWeak ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	struct HEAP {
		BoxBuffer<NODE ,HEAP_SIZE> mList ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mFree ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<UniqueRef<OWNERSHIP>> mLater ;

	public:
		void initialize (CREF<FLAG> tag) override {
			const auto r1x = unique () ;
			assert (r1x.exist ()) ;
			INDEX ix = r1x->mLast ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				if (r1x->mList[ix].mTag == tag)
					break ;
				ix = r1x->mList[ix].mPrev ;
			}
			if (ix == NONE)
				return ;
			assert (r1x->mList[ix].mPrev != USED) ;
			mLater = r1x->mList[ix].mWeak ;
			assume (mLater.available ()) ;
		}

		void initialize (CREF<FLAG> tag ,RREF<Function<Auto>> expr_) override {
			assert (expr_.exist ()) ;
			const auto r1x = unique () ;
			assert (r1x.exist ()) ;
			INDEX ix = r1x->mFree ;
			assume (ix != NONE) ;
			auto rax = UniqueRef<OWNERSHIP> ([&] (VREF<OWNERSHIP> me) {
				me.mHeap = VRef<HEAP>::reference (r1x.self) ;
				me.mHeap->mList[ix].mTag = tag ;
				me.mHeap->mList[ix].mExpr = move (expr_) ;
				me.mHeap->mFree = me.mHeap->mList[ix].mNext ;
				me.mHeap->mList[ix].mPrev = me.mHeap->mLast ;
				me.mHeap->mList[ix].mNext = NONE ;
				curr_next (me.mHeap.self ,me.mHeap->mLast ,ix) ;
				me.mHeap->mLast = ix ;
				me.mIndex = ix ;
			} ,[] (VREF<OWNERSHIP> me) {
				INDEX iy = me.mIndex ;
				if (me.mHeap->mList[iy].mPrev == USED)
					return ;
				curr_next (me.mHeap.self ,me.mHeap->mList[iy].mPrev ,me.mHeap->mList[iy].mNext) ;
				curr_prev (me.mHeap.self ,me.mHeap->mList[iy].mNext ,me.mHeap->mList[iy].mPrev) ;
				me.mHeap->mList[iy].mPrev = USED ;
				me.mHeap->mList[iy].mNext = me.mHeap->mFree ;
				me.mHeap->mFree = iy ;
				me.mHeap->mList[iy].mTag = 0 ;
				me.mHeap->mList[iy].mExpr = Function<Auto> () ;
			}) ;
			mLater = SharedRef<UniqueRef<OWNERSHIP>>::make (move (rax)) ;
			r1x->mList[ix].mWeak = mLater.weak () ;
		}

		BOOL available () const override {
			if ifnot (mLater.exist ())
				return FALSE ;
			if ifnot (mLater->available ())
				return FALSE ;
			return TRUE ;
		}

		Auto invoke () const override {
			return mLater->self.mHeap->mList[mLater->self.mIndex].mExpr () ;
		}

		imports SharedRef<HEAP> unique () {
			return memorize ([&] () {
				SharedRef<HEAP> ret = SharedRef<HEAP>::make () ;
				ret->mList = BoxBuffer<NODE ,HEAP_SIZE> (0) ;
				update_reserve (ret.self) ;
				return move (ret) ;
			}) ;
		}

		imports void update_reserve (VREF<HEAP> heap) {
			INDEX ix = NONE ;
			for (auto &&i : iter (0 ,heap.mList.size ())) {
				INDEX iy = heap.mList.size () - 1 - i ;
				heap.mList[iy].mPrev = USED ;
				heap.mList[iy].mNext = ix ;
				ix = iy ;
			}
			heap.mFirst = NONE ;
			heap.mLast = NONE ;
			heap.mFree = ix ;
		}

		imports void curr_next (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> next) {
			auto act = TRUE ;
			if ifswitch (act) {
				if (curr == NONE)
					discard ;
				heap.mList[curr].mNext = next ;
			}
			if ifswitch (act) {
				heap.mFirst = next ;
			}
		}

		imports void curr_prev (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> prev) {
			auto act = TRUE ;
			if ifswitch (act) {
				if (curr == NONE)
					discard ;
				heap.mList[curr].mPrev = prev ;
			}
			if ifswitch (act) {
				heap.mLast = prev ;
			}
		}
	} ;
} ;

template <>
exports auto LATER_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;