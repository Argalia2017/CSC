#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

namespace CSC {
template <class...>
trait FUNCTION_calendar_from_timepoint_HELP ;

template <class DEPEND>
trait FUNCTION_calendar_from_timepoint_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_WINDOWS<DEPEND>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_calendar_from_timepoint {
		inline std::tm operator() (CREF<std::time_t> time_) const {
			std::tm ret ;
			zeroize (ret) ;
			localtime_s ((&ret) ,(&time_)) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_calendar_from_timepoint_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_LINUX<DEPEND>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_calendar_from_timepoint {
		inline std::tm operator() (CREF<std::time_t> time_) const {
			std::tm ret ;
			const auto r1x = FLAG (std::localtime ((&time_))) ;
			unsafe_sync (unsafe_deptr (ret) ,unsafe_pointer (r1x)) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using TimeDuration = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimeDuration ;
	using CALENDAR = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::CALENDAR ;

	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	class ImplHolder implement Holder {
	protected:
		TIMEDURATION mTimeDuration ;

	public:
		void initialize () override {
			const auto r1x = std::chrono::system_clock::now () ;
			mTimeDuration = r1x.time_since_epoch () ;
		}

		void initialize (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) override {
			const auto r1x = std::chrono::milliseconds (milliseconds_) ;
			const auto r2x = std::chrono::nanoseconds (milliseconds_) ;
			const auto r3x = r1x + r2x ;
			mTimeDuration = std::chrono::duration_cast<std::chrono::system_clock::duration> (r3x) ;
		}

		void initialize (CREF<CALENDAR> calendar_) override {
			auto rax = std::tm () ;
			zeroize (rax) ;
			const auto r1x = calendar_.mYear - 1900 ;
			rax.tm_year = VAL32 (r1x * LENGTH (r1x > 0)) ;
			const auto r2x = calendar_.mMonth ;
			rax.tm_mon = VAL32 (r2x * LENGTH (r2x > 0)) ;
			rax.tm_mday = VAL32 (calendar_.mDay) ;
			const auto r3x = calendar_.mWDay - 1 ;
			rax.tm_wday = VAL32 (r3x * LENGTH (r3x > 0)) ;
			const auto r4x = calendar_.mYDay - 1 ;
			rax.tm_yday = VAL32 (r4x * LENGTH (r4x > 0)) ;
			rax.tm_hour = VAL32 (calendar_.mHour) ;
			rax.tm_min = VAL32 (calendar_.mMinute) ;
			rax.tm_sec = VAL32 (calendar_.mSecond) ;
			const auto r5x = std::mktime ((&rax)) ;
			const auto r6x = std::chrono::system_clock::from_time_t (r5x) ;
			mTimeDuration = r6x.time_since_epoch () ;
		}

		TimeDuration factory (CREF<TIMEDURATION> duration_) const {
			auto rax = VRef<ImplHolder>::make () ;
			rax->mTimeDuration = duration_ ;
			return TimeDuration (rax.as_cref ()) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		TIMEDURATION get_mTimeDuration () const {
			return mTimeDuration ;
		}

		TIMEPOINT get_mTimePoint () const {
			return TIMEPOINT (mTimeDuration) ;
		}

		LENGTH hours () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::hours> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH minutes () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::minutes> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH seconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH milliseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH microseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH nanoseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		TimeDuration add (CREF<Holder> that) const override {
			return add (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		TimeDuration add (CREF<ImplHolder> that) const {
			const auto r1x = mTimeDuration + that.mTimeDuration ;
			return factory (r1x) ;
		}

		TimeDuration sub (CREF<Holder> that) const override {
			return sub (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		TimeDuration sub (CREF<ImplHolder> that) const {
			const auto r1x = mTimeDuration - that.mTimeDuration ;
			return factory (r1x) ;
		}

		CALENDAR calendar () const override {
			CALENDAR ret ;
			const auto r1x = TIMEPOINT (mTimeDuration) ;
			const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
			const auto r3x = calendar_from_timepoint (r2x) ;
			ret.mYear = r3x.tm_year + 1900 ;
			ret.mMonth = r3x.tm_mon + 1 ;
			ret.mDay = r3x.tm_mday ;
			ret.mWDay = r3x.tm_wday + 1 ;
			ret.mYDay = r3x.tm_yday + 1 ;
			ret.mHour = r3x.tm_hour ;
			ret.mMinute = r3x.tm_min ;
			ret.mSecond = r3x.tm_sec ;
			return move (ret) ;
		}

		std::tm calendar_from_timepoint (CREF<std::time_t> time_) const {
			using R1X = typename FUNCTION_calendar_from_timepoint_HELP<DEPEND ,ALWAYS>::FUNCTION_calendar_from_timepoint ;
			const auto r1x = R1X () ;
			return r1x (time_) ;
		}
	} ;
} ;

template <>
exports auto TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait ATOMIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ATOMIC_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		Box<std::atomic<VAL>> mAtomic ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize (CREF<VAL> init_) override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mAtomic = Box<std::atomic<VAL>>::make () ;
			mHeap->mAtomic->store (init_ ,std::memory_order::memory_order_relaxed) ;
		}

		VAL fetch () const override {
			return mHeap->mAtomic->load (std::memory_order::memory_order_relaxed) ;
		}

		void store (CREF<VAL> obj) const override {
			return mHeap->mAtomic->store (obj ,std::memory_order::memory_order_relaxed) ;
		}

		VAL exchange (CREF<VAL> obj) const override {
			return mHeap->mAtomic->exchange (obj ,std::memory_order::memory_order_relaxed) ;
		}

		void replace (CREF<VAL> expect ,CREF<VAL> next) const override {
			auto rax = expect ;
			mHeap->mAtomic->compare_exchange_strong (rax ,next ,std::memory_order::memory_order_relaxed) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> next) const override {
			const auto r1x = mHeap->mAtomic->compare_exchange_weak (expect ,next ,std::memory_order::memory_order_relaxed) ;
			if (r1x)
				return TRUE ;
			std::this_thread::yield () ;
			return FALSE ;
		}

		VAL fetch_add (CREF<VAL> obj) const override {
			return mHeap->mAtomic->fetch_add (obj ,std::memory_order::memory_order_relaxed) ;
		}

		VAL fetch_sub (CREF<VAL> obj) const override {
			return mHeap->mAtomic->fetch_sub (obj ,std::memory_order::memory_order_relaxed) ;
		}
	} ;
} ;

template <>
exports auto ATOMIC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename ATOMIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MUTEX_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		Box<std::mutex> mMutex ;
		Box<std::recursive_mutex> mRecursive ;
		Box<std::condition_variable> mConditional ;
		Atomic mShared ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize (CREF<LENGTH> cond_size) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (cond_size != 0)
					discard ;
				mHeap = SharedRef<HEAP>::make () ;
				mHeap->mRecursive = Box<std::recursive_mutex>::make () ;
				mHeap->mShared = Atomic (0) ;
			}
			if ifswitch (act) {
				if (cond_size != 1)
					discard ;
				mHeap = SharedRef<HEAP>::make () ;
				mHeap->mMutex = Box<std::mutex>::make () ;
				mHeap->mConditional = Box<std::condition_variable>::make () ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		SharedRef<HEAP> get_mHeap () const {
			return mHeap ;
		}

		void enter () override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->lock () ;
			}
			if ifswitch (act) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->lock () ;
			}
		}

		void leave () override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->unlock () ;
			}
			if ifswitch (act) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->unlock () ;
			}
		}
	} ;
} ;

template <>
exports auto MUTEX_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait UNIQUELOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename UNIQUELOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using HEAP = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;
		std::unique_lock<std::mutex> mLock ;

	public:
		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = mutex_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap = r1x->get_mHeap () ;
			assert (mHeap->mConditional != NULL) ;
			mLock = std::unique_lock<std::mutex> (mHeap->mMutex.self) ;
		}

		void finalize () override {
			mLock = std::unique_lock<std::mutex> () ;
		}

		void wait () override {
			mHeap->mConditional->wait (mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap->mConditional->wait_for (mLock ,r1x->get_mTimeDuration ()) ;
		}

		void notify () override {
			mHeap->mConditional->notify_all () ;
		}

		void yield () override {
			mLock = std::unique_lock<std::mutex> () ;
			std::this_thread::yield () ;
			mLock = std::unique_lock<std::mutex> (mHeap->mMutex.self) ;
		}
	} ;
} ;

template <>
exports auto UNIQUELOCK_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename UNIQUELOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait SHAREDLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SHAREDLOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using HEAP = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = mutex_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap = r1x->get_mHeap () ;
			assert (mHeap->mRecursive != NULL) ;
			shared_enter () ;
		}

		void finalize () override {
			shared_leave () ;
		}

		BOOL busy () const override {
			return mHeap->mShared.fetch () != IDEN ;
		}

		void shared_enter () {
			if ifswitch (TRUE) {
				auto rax = mHeap->mShared.fetch () ;
				rax = vabs (rax) ;
				const auto r1x = mHeap->mShared.change (rax ,rax + 1) ;
				if (r1x)
					discard ;
				std::lock_guard<std::recursive_mutex> anonymous (mHeap->mRecursive.self) ;
				while (TRUE) {
					rax = vabs (rax) ;
					const auto r2x = mHeap->mShared.change (rax ,rax + 1) ;
					if (r2x)
						break ;
				}
			}
			std::atomic_thread_fence (std::memory_order::memory_order_acquire) ;
		}

		void shared_leave () {
			mHeap->mShared.decrease () ;
		}

		void enter () override {
			mHeap->mShared.decrease () ;
			mHeap->mRecursive->lock () ;
			if ifswitch (TRUE) {
				auto rax = ZERO ;
				const auto r1x = mHeap->mShared.change (rax ,NONE) ;
				if (r1x)
					discard ;
				if (rax <= NONE)
					discard ;
				while (TRUE) {
					rax = ZERO ;
					const auto r2x = mHeap->mShared.change (rax ,NONE) ;
					if (r2x)
						break ;
				}
			}
		}

		void leave () override {
			std::atomic_thread_fence (std::memory_order::memory_order_release) ;
			mHeap->mShared.replace (NONE ,ZERO) ;
			mHeap->mShared.increase () ;
			mHeap->mRecursive->unlock () ;
		}
	} ;
} ;

template <>
exports auto SHAREDLOCK_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename SHAREDLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait STATICPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STATICPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	struct NODE {
		FLAG mGroup ;
		FLAG mCabi ;
		CRef<Proxy> mAddr ;
		INDEX mNext ;
		BOOL mGood ;
		INDEX mGoodNext ;
	} ;

	struct HEAP {
		Mutex mMutex ;
		BOOL mFinalizing ;
		VarBuffer<INDEX> mRange ;
		LENGTH mSize ;
		INDEX mRead ;
		FLAG mMinCabi ;
		FLAG mMaxCabi ;
		Allocator<NODE ,VARIABLE> mList ;
		INDEX mGoodFirst ;
		LENGTH mGoodLength ;
	} ;

	using HEADER_SIZE = ENUMAS<VAL ,65536> ;

	class ImplHolder implement Holder {
	protected:
		FLAG mPointer ;

	public:
		void initialize () override {
			mPointer = address (unique ().self) ;
			fake.mMutex = RecursiveMutex::make () ;
			fake.mFinalizing = FALSE ;
			fake.mSize = 0 ;
			fake.mRead = 0 ;
			fake.mMinCabi = ZERO ;
			fake.mMaxCabi = ZERO ;
			fake.mGoodFirst = NONE ;
			fake.mGoodLength = 0 ;
		}

		void finalize () override {
			auto rax = SharedLock (fake.mMutex) ;
			fake.mFinalizing = TRUE ;
			auto rbx = CRef<Proxy> () ;
			while (TRUE) {
				if (fake.mGoodFirst == NONE)
					break ;
				if ifswitch (TRUE) {
					Scope<SharedLock> anonymous (rax) ;
					if (fake.mGoodFirst == NONE)
						discard ;
					INDEX ix = fake.mGoodFirst ;
					rbx = move (fake.mList[ix].mAddr) ;
					fake.mList[ix].mGood = FALSE ;
					fake.mGoodFirst = fake.mList[ix].mGoodNext ;
					fake.mGoodLength-- ;
				}
				rbx = NULL ;
			}
			fake.mList.clear () ;
			fake.mList = Allocator<NODE ,VARIABLE> () ;
			fake.mRange = VarBuffer<INDEX> () ;
		}

		CRef<Proxy> link (CREF<FLAG> group ,CREF<FLAG> cabi) const override {
			auto rax = SharedLock (fake.mMutex) ;
			const auto r1x = group ;
			const auto r2x = cabi ;
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (r2x < fake.mMinCabi)
					discard ;
				if (r2x > fake.mMaxCabi)
					discard ;
				if (fake.mSize == 0)
					discard ;
				const auto r3x = (fake.mRead + r2x - fake.mMinCabi) % fake.mSize ;
				ix = fake.mRange[r3x] ;
				while (TRUE) {
					if (ix == NONE)
						break ;
					if (fake.mList[ix].mGroup == r1x)
						break ;
					ix = fake.mList[ix].mNext ;
				}
			}
			if (ix == NONE)
				return NULL ;
			return CRef<Proxy>::reference (fake.mList[ix].mAddr.self) ;
		}

		void regi (CREF<FLAG> group ,CREF<FLAG> cabi ,VREF<CRef<Proxy>> addr) const override {
			assert (addr != NULL) ;
			auto rax = SharedLock (fake.mMutex) ;
			if (fake.mFinalizing)
				return ;
			const auto r1x = group ;
			const auto r2x = cabi ;
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (r2x < fake.mMinCabi)
					discard ;
				if (r2x > fake.mMaxCabi)
					discard ;
				if (fake.mSize == 0)
					discard ;
				const auto r3x = (fake.mRead + r2x - fake.mMinCabi) % fake.mSize ;
				ix = fake.mRange[r3x] ;
				while (TRUE) {
					if (ix == NONE)
						break ;
					if (fake.mList[ix].mGroup == r1x)
						break ;
					ix = fake.mList[ix].mNext ;
				}
			}
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				Scope<SharedLock> anonymous (rax) ;
				update_resize (r2x) ;
				ix = fake.mList.alloc () ;
				const auto r4x = (fake.mRead + r2x - fake.mMinCabi) % fake.mSize ;
				fake.mList[ix].mGroup = r1x ;
				fake.mList[ix].mCabi = r2x ;
				fake.mList[ix].mAddr = NULL ;
				fake.mList[ix].mNext = fake.mRange[r4x] ;
				fake.mList[ix].mGood = FALSE ;
				fake.mList[ix].mGoodNext = NONE ;
				fake.mRange[r4x] = ix ;
			}
			if ifswitch (TRUE) {
				if (fake.mList[ix].mGood)
					discard ;
				Scope<SharedLock> anonymous (rax) ;
				fake.mList[ix].mAddr = move (addr) ;
				fake.mList[ix].mGood = TRUE ;
				fake.mList[ix].mGoodNext = fake.mGoodFirst ;
				fake.mGoodFirst = ix ;
				fake.mGoodLength++ ;
			}
			addr = CRef<Proxy>::reference (fake.mList[ix].mAddr.self) ;
		}

		void update_resize (CREF<FLAG> cabi) const {
			auto act = TRUE ;
			if ifswitch (act) {
				if (fake.mRange.size () > 0)
					discard ;
				const auto r1x = HEADER_SIZE::expr ;
				auto rax = VarBuffer<INDEX> (r1x) ;
				if (rax.size () == 0)
					discard ;
				BufferProc<INDEX>::buf_fill (rax ,NONE ,0 ,rax.size ()) ;
				fake.mRange = move (rax) ;
				fake.mSize = r1x ;
				fake.mRead = 0 ;
				fake.mMinCabi = cabi ;
				fake.mMaxCabi = cabi ;
			}
			if ifswitch (act) {
				if (cabi >= fake.mMinCabi)
					discard ;
				const auto r2x = fake.mMinCabi - cabi ;
				const auto r3x = fake.mSize - (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				if (r2x > r3x)
					discard ;
				fake.mRead = (fake.mRead - r2x + fake.mSize) % fake.mSize ;
				fake.mMinCabi = cabi ;
			}
			if ifswitch (act) {
				if (cabi <= fake.mMaxCabi)
					discard ;
				const auto r4x = cabi - fake.mMaxCabi ;
				const auto r5x = fake.mSize - (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				if (r4x > r5x)
					discard ;
				fake.mMaxCabi = cabi ;
			}
			if ifswitch (act) {
				if (vbetween (cabi ,fake.mMinCabi ,fake.mMaxCabi + 1))
					discard ;
				const auto r6x = fake.mSize + vmax (cabi - fake.mMaxCabi ,fake.mMinCabi - cabi) ;
				auto rax = VarBuffer<INDEX> (r6x) ;
				if (rax.size () == 0)
					discard ;
				const auto r7x = fake.mRead + (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				const auto r8x = vmin (r7x ,fake.mSize) - fake.mRead ;
				BufferProc<INDEX>::buf_copy (rax ,unsafe_array (fake.mRange[fake.mRead]) ,0 ,r8x) ;
				const auto r9x = vmax (r7x - fake.mSize ,0) ;
				BufferProc<INDEX>::buf_copy (unsafe_array (rax[r8x]) ,fake.mRange ,0 ,r9x) ;
				const auto r10x = vmax (fake.mMinCabi - cabi ,0) ;
				fake.mRange = move (rax) ;
				fake.mSize = r6x ;
				fake.mRead = (fake.mRead - r10x + fake.mSize) % fake.mSize ;
				fake.mMinCabi = vmin (fake.mMinCabi ,cabi) ;
				fake.mMaxCabi = vmax (fake.mMaxCabi ,cabi) ;
			}
		}

		imports CREF<Box<HEAP>> unique () {
			return memorize ([&] () {
				return Box<HEAP>::make () ;
			}) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <>
exports auto STATICPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename STATICPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename THREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mUID ;
		INDEX mSlot ;
		VRef<Binder> mVarBinder ;
		CRef<Binder> mConBinder ;
		VRef<std::thread> mBlock ;

	public:
		void initialize (RREF<VRef<Binder>> binder ,CREF<INDEX> slot) override {
			mUID = ZERO ;
			mSlot = slot ;
			mVarBinder = move (binder) ;
		}

		void initialize (RREF<CRef<Binder>> binder ,CREF<INDEX> slot) override {
			mUID = ZERO ;
			mSlot = slot ;
			mConBinder = move (binder) ;
		}

		void finalize () override {
			assert (mBlock == NULL) ;
		}

		FLAG thread_uid () const override {
			return mUID ;
		}

		void start () override {
			assume (mBlock == NULL) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (mVarBinder == NULL)
					discard ;
				mBlock = VRef<std::thread>::make ([&] () {
					try_invoke ([&] () {
						mUID = RuntimeProc::thread_uid () ;
						mVarBinder->friend_execute (mSlot) ;
					} ,[&] () {
						noop () ;
					}) ;
				}) ;
			}
			if ifswitch (act) {
				if (mConBinder == NULL)
					discard ;
				mBlock = VRef<std::thread>::make ([&] () {
					try_invoke ([&] () {
						mUID = RuntimeProc::thread_uid () ;
						mConBinder->friend_execute (mSlot) ;
					} ,[&] () {
						noop () ;
					}) ;
				}) ;
			}
		}

		void stop () override {
			if ifswitch (TRUE) {
				if (mBlock == NULL)
					discard ;
				mBlock->join () ;
				mBlock = NULL ;
				mUID = ZERO ;
			}
		}
	} ;
} ;

template <>
exports auto THREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait FUNCTION_string_cvt_locale_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_locale_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRA<DEPEND>>> {
#ifdef __CSC_CONFIG_STRA__
	struct FUNCTION_string_cvt_locale {
		inline String<STR> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_string_cvt_locale_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRW<DEPEND>>> {
#ifdef __CSC_CONFIG_STRW__
	struct FUNCTION_string_cvt_locale {
		inline String<STR> operator() (CREF<String<STRA>> obj) const {
			return StringProc::string_cvt_w_from_ansi (obj) ;
		}

		inline String<STRA> operator() (CREF<String<STR>> obj) const {
			return StringProc::string_cvt_ansi_from_w (obj) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SYSTEM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		String<STRA> mLocale ;
		String<STRA> mOldLocale ;
		String<STR> mWorkingPath ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mLocale = PrintString<STRA>::make (slice ("C")) ;
			mHeap->mWorkingPath = RuntimeProc::working_path () ;
		}

		String<STR> get_locale () const override {
			using R1X = typename FUNCTION_string_cvt_locale_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt_locale ;
			const auto r1x = R1X () ;
			return r1x (mHeap->mLocale) ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			using R1X = typename FUNCTION_string_cvt_locale_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt_locale ;
			const auto r1x = R1X () ;
			mHeap->mOldLocale = move (mHeap->mLocale) ;
			mHeap->mLocale = r1x (name) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mLocale.empty ())
					discard ;
				mHeap->mLocale = String<STRA>::zero () ;
			}
			std::setlocale (LC_CTYPE ,(&mHeap->mLocale[0])) ;
		}

		FLAG execute (CREF<String<STR>> command) const override {
			if (command.empty ())
				return NONE ;
			const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (command) ;
			const auto r2x = std::system ((&r1x[0])) ;
			return FLAG (r2x) ;
		}

		String<STR> working_path () const override {
			return mHeap->mWorkingPath ;
		}
	} ;
} ;

template <>
exports auto SYSTEM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RANDOM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		DATA mSeed ;
		Box<std::mt19937_64> mRandom ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mSeed = DATA (std::random_device () ()) ;
			mHeap->mRandom = Box<std::mt19937_64>::make (csc_byte64_t (mHeap->mSeed)) ;
		}

		void initialize (CREF<DATA> seed_) override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mSeed = seed_ ;
			mHeap->mRandom = Box<std::mt19937_64>::make (csc_byte64_t (mHeap->mSeed)) ;
		}

		DATA seed () const override {
			return mHeap->mSeed ;
		}

		DATA random_byte () const override {
			return DATA (mHeap->mRandom.self ()) ;
		}

		void random_byte (VREF<Array<DATA>> result) const override {
			for (auto &&i : result)
				i = random_byte () ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const override {
			assert (lb <= rb) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) + 1 ;
			const auto r2x = VAL64 (random_byte () & DATA (VAL64_MAX)) ;
			return INDEX (r2x % r1x + lb) ;
		}

		void random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<Array<INDEX>> result) const override {
			for (auto &&i : result)
				i = random_value (lb ,rb) ;
		}

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
			Array<INDEX> ret = IterArray<INDEX>::make (iter (0 ,size_)) ;
			random_shuffle (count ,ret) ;
			return move (ret) ;
		}

		void random_shuffle (CREF<LENGTH> count ,VREF<Array<INDEX>> result) const override {
			assert (count >= 0) ;
			const auto r1x = result.size () - 1 ;
			const auto r2x = vmin (count ,r1x) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= r2x)
					break ;
				INDEX iy = random_value (ix ,r1x) ;
				swap (result[ix] ,result[iy]) ;
				ix++ ;
			}
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
			BitSet<> ret = BitSet<> (size_) ;
			random_pick (count ,ret) ;
			return move (ret) ;
		}

		void random_pick (CREF<LENGTH> count ,VREF<BitSet<>> result) const override {
			assert (count >= 0) ;
			assert (count <= result.size ()) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (count >= result.size () / 2)
					discard ;
				const auto r1x = random_shuffle (count ,result.size ()) ;
				for (auto &&i : iter (0 ,count))
					result.add (r1x[i]) ;
			}
			if ifswitch (act) {
				const auto r2x = result.size () - count ;
				const auto r3x = random_shuffle (r2x ,result.size ()) ;
				for (auto &&i : iter (0 ,count))
					result.add (r3x[r2x + i]) ;
			}
		}

		BOOL random_draw (CREF<DOUBLE> possibility) const override {
			const auto r1x = random_value (0 ,10000) ;
			const auto r2x = DOUBLE (r1x) * MathProc::inverse (DOUBLE (10000)) ;
			if (r2x < possibility)
				return TRUE ;
			return FALSE ;
		}

		void random_draw (CREF<DOUBLE> possibility ,VREF<Array<BOOL>> result) const override {
			for (auto &&i : result)
				i = random_draw (possibility) ;
		}
	} ;
} ;

template <>
exports auto RANDOM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait SINGLETON_HACKSHAREDREF_HELP ;

template <class UNIT>
trait SINGLETON_HACKSHAREDREF_HELP<UNIT ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<UNIT ,ALWAYS>::Holder ;
	using PureHolder = typename SHAREDREF_PUREHOLDER_HELP<UNIT ,ALWAYS>::PureHolder ;
	using SUPER = SharedRef<UNIT> ;

	class HackSharedRef extend SUPER {
	public:
		imports SUPER make () {
			HackSharedRef ret ;
			auto rax = CRef<Holder>::reference (unique ().self) ;
			Scope<PinMutex> anonymous (rax->pin_mutex ()) ;
			ret.mThis = move (rax) ;
			ret.mPointer = ret.mThis->pointer () ;
			if ifswitch (TRUE) {
				if (ret.mThis->counter () > 0)
					discard ;
				const auto r1x = ret.mThis->pointer () ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<Box<UNIT>>>::expr] (unsafe_pointer (r1x))) ;
				tmp.acquire (TYPEAS<UNIT>::expr) ;
			}
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		imports CREF<Box<PureHolder>> unique () {
			return memorize ([&] () {
				Box<PureHolder> ret = Box<PureHolder>::make () ;
				ret->initialize () ;
				return move (ret) ;
			}) ;
		}
	} ;
} ;
} ;