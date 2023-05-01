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

	struct NODE {
		FLAG mTag ;
		AutoRef<> mValue ;
		INDEX mNext ;
	} ;

	struct HEAP {
		VRef<BOOL> mFlag ;
		BoxBuffer<NODE ,HEAP_SIZE> mList ;
		INDEX mFirst ;
		INDEX mFree ;
	} ;

	using HEAPROOT = typename SHAREDREF_PUREHOLDER_HELP<HEAP ,ALWAYS>::PureHolder ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;
		BOOL mOwnership ;
		INDEX mIndex ;

	public:
		void initialize (CREF<FLAG> tag) override {
			mHeap = SharedRef<HEAP>::intrusive (heap_root ()) ;
			assume (mHeap.available ()) ;
			update_reserve () ;
			INDEX ix = mHeap->mFirst ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				if (mHeap->mList[ix].mTag == tag)
					break ;
				ix = mHeap->mList[ix].mNext ;
			}
			assume (ix != NONE) ;
			mOwnership = FALSE ;
			mIndex = ix ;
		}

		void initialize (CREF<FLAG> tag ,RREF<AutoRef<>> value_) override {
			assert (value_.exist ()) ;
			mHeap = SharedRef<HEAP>::intrusive (heap_root ()) ;
			assume (mHeap.available ()) ;
			update_reserve () ;
			INDEX ix = mHeap->mFree ;
			assume (ix != NONE) ;
			mHeap->mList[ix].mTag = tag ;
			mHeap->mList[ix].mValue = move (value_) ;
			mHeap->mFree = mHeap->mList[ix].mNext ;
			mHeap->mList[ix].mNext = mHeap->mFirst ;
			mHeap->mFirst = ix ;
			mOwnership = TRUE ;
			mIndex = ix ;
		}

		void finalize () override {
			if ifnot (mOwnership)
				return ;
			INDEX ix = mIndex ;
			assert (ix == mHeap->mFirst) ;
			mHeap->mFirst = mHeap->mList[ix].mNext ;
			mHeap->mList[ix].mNext = mHeap->mFree ;
			mHeap->mFree = ix ;
			mHeap->mList[ix].mTag = 0 ;
			mHeap->mList[ix].mValue = AutoRef<> () ;
		}

		CREF<AutoRef<>> invoke () const leftvalue override {
			return mHeap->mList[mIndex].mValue ;
		}

		void update_reserve () {
			if (mHeap->mFlag != NULL)
				return ;
			mHeap->mList = BoxBuffer<NODE ,HEAP_SIZE> (0) ;
			INDEX ix = NONE ;
			for (auto &&i : iter (0 ,mHeap->mList.size ())) {
				INDEX iy = mHeap->mList.size () - 1 - i ;
				mHeap->mList[iy].mNext = ix ;
				ix = iy ;
			}
			mHeap->mFirst = NONE ;
			mHeap->mFree = ix ;
			mHeap->mFlag = VRef<BOOL>::make (TRUE) ;
		}

		CRef<HEAPROOT> heap_root () const {
			return CRef<HEAPROOT>::reference (memorize ([&] () {
				Box<HEAPROOT> ret = Box<HEAPROOT>::make () ;
				ret->initialize () ;
				return move (ret) ;
			}).self) ;
		}
	} ;
} ;

template <>
exports auto LATER_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;