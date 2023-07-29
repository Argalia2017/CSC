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
		forceinline std::tm operator() (CREF<std::time_t> time_) const {
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
		forceinline std::tm operator() (CREF<std::time_t> time_) const {
			std::tm ret ;
			const auto r1x = FLAG (std::localtime ((&time_))) ;
			unsafe_sync (unsafe_cast[TYPEAS<TEMP<std::tm>>::expr] (ret) ,unsafe_deref (r1x)) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::Layout ;
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

		Layout factory (CREF<TIMEDURATION> duration_) const {
			Layout ret ;
			auto rax = VRef<ImplHolder>::make () ;
			rax->mTimeDuration = duration_ ;
			ret.mThis = move (rax) ;
			return move (ret) ;
		}

		XRef native () const leftvalue override {
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

		Layout add (CREF<Layout> that) const override {
			return add (keep[TYPEAS<CREF<ImplHolder>>::expr] (that.mThis.self)) ;
		}

		Layout add (CREF<ImplHolder> that) const {
			const auto r1x = mTimeDuration + that.mTimeDuration ;
			return factory (r1x) ;
		}

		Layout sub (CREF<Layout> that) const override {
			return sub (keep[TYPEAS<CREF<ImplHolder>>::expr] (that.mThis.self)) ;
		}

		Layout sub (CREF<ImplHolder> that) const {
			const auto r1x = mTimeDuration - that.mTimeDuration ;
			return factory (r1x) ;
		}

		CALENDAR calendar () const override {
			CALENDAR ret ;
			const auto r1x = TIMEPOINT (mTimeDuration) ;
			const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
			const auto r3x = calendar_from_timepoint (r2x) ;
			ret.mYear = LENGTH (r3x.tm_year) + 1900 ;
			ret.mMonth = LENGTH (r3x.tm_mon) + 1 ;
			ret.mDay = LENGTH (r3x.tm_mday) ;
			ret.mWDay = LENGTH (r3x.tm_wday) + 1 ;
			ret.mYDay = LENGTH (r3x.tm_yday) + 1 ;
			ret.mHour = LENGTH (r3x.tm_hour) ;
			ret.mMinute = LENGTH (r3x.tm_min) ;
			ret.mSecond = LENGTH (r3x.tm_sec) ;
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
exports auto TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait ATOMIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ATOMIC_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		Box<std::atomic<VAL>> mAtomic ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize (CREF<VAL> init_) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mAtomic = Box<std::atomic<VAL>>::make () ;
			mThis->mAtomic->store (init_ ,std::memory_order::memory_order_relaxed) ;
		}

		VAL fetch () const override {
			return mThis->mAtomic->load (std::memory_order::memory_order_relaxed) ;
		}

		void store (CREF<VAL> obj) const override {
			return mThis->mAtomic->store (obj ,std::memory_order::memory_order_relaxed) ;
		}

		VAL exchange (CREF<VAL> obj) const override {
			return mThis->mAtomic->exchange (obj ,std::memory_order::memory_order_relaxed) ;
		}

		void replace (CREF<VAL> expect ,CREF<VAL> next) const override {
			auto rax = expect ;
			mThis->mAtomic->compare_exchange_strong (rax ,next ,std::memory_order::memory_order_relaxed) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> next) const override {
			const auto r1x = mThis->mAtomic->compare_exchange_weak (expect ,next ,std::memory_order::memory_order_relaxed) ;
			if (r1x)
				return TRUE ;
			std::this_thread::yield () ;
			return FALSE ;
		}

		VAL add_with (CREF<VAL> obj) const override {
			const auto r1x = mThis->mAtomic->fetch_add (obj ,std::memory_order::memory_order_relaxed) ;
			return r1x + obj ;
		}

		VAL sub_with (CREF<VAL> obj) const override {
			const auto r1x = mThis->mAtomic->fetch_sub (obj ,std::memory_order::memory_order_relaxed) ;
			return r1x - obj ;
		}
	} ;
} ;

template <>
exports auto ATOMIC_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename ATOMIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MUTEX_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		Box<std::mutex> mMutex ;
		Box<std::recursive_mutex> mRecursive ;
		Box<std::condition_variable> mConditional ;
		Atomic mShared ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize (CREF<LENGTH> cond_size) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (cond_size != 0)
					discard ;
				mThis = SharedRef<PACK>::make () ;
				mThis->mRecursive = Box<std::recursive_mutex>::make () ;
				mThis->mShared = Atomic (0) ;
			}
			if ifswitch (act) {
				if (cond_size != 1)
					discard ;
				mThis = SharedRef<PACK>::make () ;
				mThis->mMutex = Box<std::mutex>::make () ;
				mThis->mConditional = Box<std::condition_variable>::make () ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
		}

		XRef native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		SharedRef<PACK> get_mHeap () const {
			return mThis ;
		}

		void enter () override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mThis->mRecursive == NULL)
					discard ;
				mThis->mRecursive->lock () ;
			}
			if ifswitch (act) {
				if (mThis->mMutex == NULL)
					discard ;
				mThis->mMutex->lock () ;
			}
		}

		void leave () override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mThis->mRecursive == NULL)
					discard ;
				mThis->mRecursive->unlock () ;
			}
			if ifswitch (act) {
				if (mThis->mMutex == NULL)
					discard ;
				mThis->mMutex->unlock () ;
			}
		}
	} ;
} ;

template <>
exports auto MUTEX_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait UNIQUELOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename UNIQUELOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using PACK = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::PACK ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;
		std::unique_lock<std::mutex> mLock ;

	public:
		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = CRef<R1X> (mutex_.native ()) ;
			mThis = r1x->get_mHeap () ;
			assert (mThis->mConditional != NULL) ;
			mLock = std::unique_lock<std::mutex> (mThis->mMutex.self) ;
		}

		void finalize () override {
			mLock = std::unique_lock<std::mutex> () ;
		}

		void wait () override {
			mThis->mConditional->wait (mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = CRef<R1X> (time_.native ()) ;
			mThis->mConditional->wait_for (mLock ,r1x->get_mTimeDuration ()) ;
		}

		void notify () override {
			mThis->mConditional->notify_all () ;
		}

		void yield () override {
			mLock = std::unique_lock<std::mutex> () ;
			std::this_thread::yield () ;
			mLock = std::unique_lock<std::mutex> (mThis->mMutex.self) ;
		}
	} ;
} ;

template <>
exports auto UNIQUELOCK_HELP<DEPEND ,ALWAYS>::FakeImplHolder::create () ->Box<FakeHolder> {
	using R1X = typename UNIQUELOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait SHAREDLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SHAREDLOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using PACK = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::PACK ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = CRef<R1X> (mutex_.native ()) ;
			mThis = r1x->get_mHeap () ;
			assert (mThis->mRecursive != NULL) ;
			shared_enter () ;
		}

		void finalize () override {
			shared_leave () ;
		}

		BOOL busy () const override {
			return mThis->mShared.fetch () != IDEN ;
		}

		void shared_enter () {
			if ifswitch (TRUE) {
				auto rax = mThis->mShared.fetch () ;
				rax = MathProc::abs (rax) ;
				const auto r1x = mThis->mShared.change (rax ,rax + 1) ;
				if (r1x)
					discard ;
				std::lock_guard<std::recursive_mutex> anonymous (mThis->mRecursive.self) ;
				while (TRUE) {
					rax = MathProc::abs (rax) ;
					const auto r2x = mThis->mShared.change (rax ,rax + 1) ;
					if (r2x)
						break ;
				}
			}
			std::atomic_thread_fence (std::memory_order::memory_order_acquire) ;
		}

		void shared_leave () {
			mThis->mShared.decrease () ;
		}

		void enter () override {
			mThis->mShared.decrease () ;
			mThis->mRecursive->lock () ;
			if ifswitch (TRUE) {
				auto rax = ZERO ;
				const auto r1x = mThis->mShared.change (rax ,NONE) ;
				if (r1x)
					discard ;
				if (rax <= NONE)
					discard ;
				while (TRUE) {
					rax = ZERO ;
					const auto r2x = mThis->mShared.change (rax ,NONE) ;
					if (r2x)
						break ;
				}
			}
		}

		void leave () override {
			std::atomic_thread_fence (std::memory_order::memory_order_release) ;
			mThis->mShared.replace (NONE ,ZERO) ;
			mThis->mShared.increase () ;
			mThis->mRecursive->unlock () ;
		}
	} ;
} ;

template <>
exports auto SHAREDLOCK_HELP<DEPEND ,ALWAYS>::FakeImplHolder::create () ->Box<FakeHolder> {
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
		INDEX mSlot ;
		VRef<Binder> mVarBinder ;
		CRef<Binder> mConBinder ;
		Box<std::thread> mBlock ;

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
				mBlock = Box<std::thread>::make ([&] () {
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
				mBlock = Box<std::thread>::make ([&] () {
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
exports auto THREAD_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait FUNCTION_string_cvt_locale_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_locale_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRA<DEPEND>>> {
#ifdef __CSC_CONFIG_STRA__
	struct FUNCTION_string_cvt_locale {
		forceinline String<STR> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_string_cvt_locale_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_STRW<DEPEND>>> {
#ifdef __CSC_CONFIG_STRW__
	struct FUNCTION_string_cvt_locale {
		forceinline String<STR> operator() (CREF<String<STRA>> obj) const {
			return StringProc::string_cvt_w_from_ansi (obj) ;
		}

		forceinline String<STRA> operator() (CREF<String<STR>> obj) const {
			return StringProc::string_cvt_ansi_from_w (obj) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SYSTEM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		String<STRA> mLocale ;
		String<STRA> mOldLocale ;
		String<STR> mWorkingPath ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mLocale = PrintString<STRA>::make (slice ("C")) ;
			mThis->mWorkingPath = RuntimeProc::working_path () ;
		}

		String<STR> get_locale () const override {
			using R1X = typename FUNCTION_string_cvt_locale_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt_locale ;
			const auto r1x = R1X () ;
			return r1x (mThis->mLocale) ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			using R1X = typename FUNCTION_string_cvt_locale_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt_locale ;
			const auto r1x = R1X () ;
			mThis->mOldLocale = move (mThis->mLocale) ;
			mThis->mLocale = r1x (name) ;
			if ifswitch (TRUE) {
				if ifnot (mThis->mLocale.empty ())
					discard ;
				mThis->mLocale = String<STRA>::zero () ;
			}
			std::setlocale (LC_CTYPE ,(&mThis->mLocale[0])) ;
		}

		FLAG execute (CREF<String<STR>> command) const override {
			if (command.empty ())
				return NONE ;
			const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (command) ;
			const auto r2x = std::system ((&r1x[0])) ;
			return FLAG (r2x) ;
		}

		String<STR> working_path () const override {
			return mThis->mWorkingPath ;
		}
	} ;
} ;

template <>
exports auto SYSTEM_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RANDOM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		DATA mSeed ;
		Box<std::mt19937_64> mRandom ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mSeed = DATA (std::random_device () ()) ;
			mThis->mRandom = Box<std::mt19937_64>::make (csc_byte64_t (mThis->mSeed)) ;
		}

		void initialize (CREF<DATA> seed_) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mSeed = seed_ ;
			mThis->mRandom = Box<std::mt19937_64>::make (csc_byte64_t (mThis->mSeed)) ;
		}

		DATA seed () const override {
			return mThis->mSeed ;
		}

		DATA random_byte () const override {
			return DATA (mThis->mRandom.self ()) ;
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
exports auto RANDOM_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait GLOBAL_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename GLOBAL_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		BOOL mThreadFlag ;
		List<AutoRef<>> mGlobal ;
		Set<Slice<STR>> mGlobalSet ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mThreadFlag = FALSE ;
		}

		void startup () const override {
			if (mThis->mThreadFlag)
				return ;
			mThis->mThreadFlag = TRUE ;
			mThis->mGlobal = List<AutoRef<>> () ;
			mThis->mGlobalSet = Set<Slice<STR>> () ;
		}

		VREF<AutoRef<>> unique (CREF<Slice<STR>> name) const leftvalue override {
			assume (mThis->mThreadFlag) ;
			INDEX ix = mThis->mGlobalSet.map (name) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mThis->mGlobal.insert () ;
				mThis->mGlobalSet.add (name ,ix) ;
			}
			return mThis->mGlobal[ix] ;
		}

		void shutdown () const override {
			if ifnot (mThis->mThreadFlag)
				return ;
			mThis->mThreadFlag = FALSE ;
			mThis->mGlobal = List<AutoRef<>> () ;
			mThis->mGlobalSet = Set<Slice<STR>> () ;
		}
	} ;
} ;

template <>
exports auto GLOBAL_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename GLOBAL_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;