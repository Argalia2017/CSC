﻿#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "begin.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <locale>
#include <random>
#include "end.h"

namespace CSC {
template <class...>
trait FUNCTION_calendar_from_timepoint_HELP ;

template <class MACRO>
trait FUNCTION_calendar_from_timepoint_HELP<MACRO ,REQUIRE<MACRO_SYSTEM_WINDOWS<MACRO>>> {
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

template <class MACRO>
trait FUNCTION_calendar_from_timepoint_HELP<MACRO ,REQUIRE<MACRO_SYSTEM_LINUX<MACRO>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_calendar_from_timepoint {
		inline std::tm operator() (CREF<std::time_t> time_) const {
			std::tm ret ;
			const auto r1x = FLAG (std::localtime ((&time_))) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<std::tm>>::expr] (unsafe_pointer (r1x))) ;
			ret = tmp ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using TimeDuration = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimeDuration ;
	using CALENDAR = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::CALENDAR ;

	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	class ImplHolder implement Holder {
	protected:
		TIMEDURATION mTimeDuration ;

	public:
		implicit ImplHolder () = default ;

		void init_now () override {
			const auto r1x = std::chrono::system_clock::now () ;
			mTimeDuration = r1x.time_since_epoch () ;
		}

		void init_epoch () override {
			mTimeDuration = TIMEDURATION (0) ;
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
			return TimeDuration (move (rax)) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		TIMEDURATION get_mTimeDuration () const leftvalue {
			return mTimeDuration ;
		}

		TIMEPOINT get_mTimePoint () const leftvalue {
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
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			const auto r1x = mTimeDuration + tmp.mTimeDuration ;
			return factory (r1x) ;
		}

		TimeDuration sub (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			const auto r1x = mTimeDuration - tmp.mTimeDuration ;
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

exports auto TIMEDURATION_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait ATOMIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ATOMIC_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		std::atomic<VAL> mAtomic ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mAtomic.store (0 ,std::memory_order::memory_order_relaxed) ;
		}

		VAL fetch () const override {
			return mHeap->mAtomic.load (std::memory_order::memory_order_relaxed) ;
		}

		void store (CREF<VAL> obj) const override {
			return mHeap->mAtomic.store (obj ,std::memory_order::memory_order_relaxed) ;
		}

		VAL exchange (CREF<VAL> obj) const override {
			return mHeap->mAtomic.exchange (obj ,std::memory_order::memory_order_relaxed) ;
		}

		void replace (CREF<VAL> expect ,CREF<VAL> right) const override {
			auto rax = expect ;
			mHeap->mAtomic.compare_exchange_strong (rax ,right ,std::memory_order::memory_order_relaxed) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> right) const override {
			const auto r1x = mHeap->mAtomic.compare_exchange_weak (expect ,right ,std::memory_order::memory_order_relaxed) ;
			if (r1x)
				return TRUE ;
			std::this_thread::yield () ;
			return FALSE ;
		}

		VAL fetch_add (CREF<VAL> obj) const override {
			return mHeap->mAtomic.fetch_add (obj ,std::memory_order::memory_order_relaxed) ;
		}

		VAL fetch_sub (CREF<VAL> obj) const override {
			return mHeap->mAtomic.fetch_sub (obj ,std::memory_order::memory_order_relaxed) ;
		}
	} ;
} ;

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
		LENGTH mCounter ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_recursive () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mRecursive = Box<std::recursive_mutex>::make () ;
		}

		void init_conditional () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mMutex = Box<std::mutex>::make () ;
			mHeap->mConditional = Box<std::condition_variable>::make () ;
			mHeap->mCounter = 0 ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		SharedRef<HEAP> conditional_lock (VREF<std::unique_lock<std::mutex>> locker) const leftvalue {
			locker = std::unique_lock<std::mutex> (mHeap->mMutex) ;
			return mHeap ;
		}

		void enter () const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->lock () ;
			}
			if ifswitch (rxx) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->lock () ;
			}
		}

		void leave () const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->unlock () ;
			}
			if ifswitch (rxx) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->unlock () ;
			}
		}
	} ;
} ;

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
		Scope<ImplHolder> mHandle ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = mutex_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap = r1x->conditional_lock (mLock) ;
			mHandle = Scope<ImplHolder> (thiz) ;
		}

		void enter () {
			noop () ;
		}

		void leave () {
			mHeap = SharedRef<HEAP> () ;
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

		void wait (CREF<TimePoint> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap->mConditional->wait_until (mLock ,r1x->get_mTimePoint ()) ;
		}

		void notify () override {
			mHeap->mConditional->notify_all () ;
		}

		void yield () override {
			mHeap->mConditional->notify_all () ;
			mLock = std::unique_lock<std::mutex> () ;
			std::this_thread::yield () ;
			mLock = std::unique_lock<std::mutex> (mHeap->mMutex) ;
		}
	} ;
} ;

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
		std::unique_lock<std::mutex> mLock ;
		Scope<ImplHolder> mHandle ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = mutex_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			mHeap = r1x->conditional_lock (mLock) ;
			mHandle = Scope<ImplHolder> (thiz) ;
		}

		void enter () {
			auto rax = move (mLock) ;
			while (TRUE) {
				if (mHeap->mCounter >= 0)
					break ;
				mHeap->mConditional->wait (rax) ;
			}
			mHeap->mCounter++ ;
		}

		void leave () {
			auto rax = std::unique_lock<std::mutex> (mHeap->mMutex) ;
			mHeap->mCounter-- ;
			mHeap->mConditional->notify_all () ;
			if ifswitch (TRUE) {
				if (mHeap->mCounter == 0)
					discard ;
				mHeap->mCounter = 0 ;
			}
			mHeap = SharedRef<HEAP> () ;
		}

		BOOL lock () const override {
			auto rax = std::unique_lock<std::mutex> (mHeap->mMutex) ;
			mHeap->mCounter-- ;
			while (TRUE) {
				if (mHeap->mCounter != 0)
					break ;
				mHeap->mConditional->wait (rax) ;
			}
			mHeap->mCounter = -1 ;
			return TRUE ;
		}
	} ;
} ;

exports auto SHAREDLOCK_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename SHAREDLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
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
		INDEX mIndex ;
		VRef<Binder> mBinder ;
		VRef<std::thread> mBlock ;
		Scope<ImplHolder> mHandle ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<VRef<Binder>> binder ,CREF<INDEX> index) override {
			mUID = ZERO ;
			mIndex = index ;
			mBinder = move (binder) ;
			mHandle = Scope<ImplHolder> (thiz) ;
		}

		void enter () {
			noop () ;
		}

		void leave () {
			assume (mBlock == NULL) ;
		}

		FLAG thread_uid () const override {
			return mUID ;
		}

		void start () override {
			assume (mBlock == NULL) ;
			mBlock = VRef<std::thread>::make ([&] () {
				try_invoke ([&] () {
					mUID = RuntimeProc::thread_uid () ;
					mBinder->friend_execute (mIndex) ;
				} ,[&] () {
					noop () ;
				}) ;
			}) ;
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

exports auto THREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SYSTEM_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Cell<String<STR>> mLocale ;
		String<STR> mWorkingPath ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mLocale = Cell<String<STR>>::make (slice ("C")) ;
			mWorkingPath = RuntimeProc::working_path () ;
		}

		String<STR> get_locale () const override {
			return mLocale.fetch () ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			mLocale.store (name) ;
		}

		FLAG execute (CREF<String<STR>> command) const override {
			if (command.empty ())
				return NONE ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (command) ;
			const auto r2x = std::system ((&r1x[0])) ;
			return FLAG (r2x) ;
		}

		String<STR> working_path () const override {
			return mWorkingPath ;
		}
	} ;
} ;

exports auto SYSTEM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RANDOM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		DATA mSeed ;
		std::mt19937_64 mRandom ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mSeed = DATA (std::random_device () ()) ;
			mHeap->mRandom = std::mt19937_64 (csc_byte64_t (mHeap->mSeed)) ;
		}

		void initialize (CREF<DATA> seed_) override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mSeed = seed_ ;
			mHeap->mRandom = std::mt19937_64 (csc_byte64_t (mHeap->mSeed)) ;
		}

		DATA seed () const override {
			return mHeap->mSeed ;
		}

		DATA random_byte () const override {
			return DATA (mHeap->mRandom ()) ;
		}

		Array<DATA> random_byte (CREF<LENGTH> size_) const override {
			Array<DATA> ret = Array<DATA> (size_) ;
			for (auto &&i : ret)
				i = random_byte () ;
			return move (ret) ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const override {
			assert (lb <= rb) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) + 1 ;
			const auto r2x = VAL64 (random_byte () & DATA (VAL64_MAX)) ;
			return INDEX (r2x % r1x + lb) ;
		}

		Array<INDEX> random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> size_) const override {
			assert (lb <= rb) ;
			Array<INDEX> ret = Array<INDEX> (size_) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) + 1 ;
			for (auto &&i : ret) {
				const auto r2x = VAL64 (random_byte () & DATA (VAL64_MAX)) ;
				i = INDEX (r2x % r1x + lb) ;
			}
			return move (ret) ;
		}

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
			Array<INDEX> ret = Array<INDEX>::make (iter (0 ,size_)) ;
			random_shuffle (count ,size_ ,ret) ;
			return move (ret) ;
		}

		void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> range_) const override {
			assert (count >= 0) ;
			assert (count <= size_) ;
			assert (range_.size () == size_) ;
			const auto r1x = range_.size () - 1 ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= count)
					break ;
				INDEX iy = random_value (ix ,r1x) ;
				swap (range_[ix] ,range_[iy]) ;
				ix++ ;
			}
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
			BitSet<> ret = BitSet<> (size_) ;
			random_pick (count ,size_ ,ret) ;
			return move (ret) ;
		}

		void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet<>> range_) const override {
			assert (count >= 0) ;
			assert (count <= size_) ;
			assert (range_.size () == size_) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (count >= size_ / 2)
					discard ;
				const auto r1x = random_shuffle (count ,size_) ;
				for (auto &&i : iter (0 ,count))
					range_.add (r1x[i]) ;
			}
			if ifswitch (rxx) {
				const auto r2x = random_shuffle (size_ - count ,size_) ;
				for (auto &&i : iter (size_ - count ,size_))
					range_.add (r2x[i]) ;
			}
		}

		BOOL random_draw (CREF<DOUBLE> possibility) const override {
			const auto r1x = random_value (0 ,10000) ;
			const auto r2x = DOUBLE (r1x) * MathProc::inverse (DOUBLE (10000)) ;
			if (r2x < possibility)
				return TRUE ;
			return FALSE ;
		}
	} ;
} ;

exports auto RANDOM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;