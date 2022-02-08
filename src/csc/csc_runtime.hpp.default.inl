#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "begin.h"
#include <cstdio>
#include <cstdlib>
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
namespace RUNTIME {
template <class...>
trait TIMEDURATION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct HEAP {
		TIMEDURATION mTimeDuration ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_second (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) override {
			const auto r1x = std::chrono::milliseconds (milliseconds_) ;
			const auto r2x = std::chrono::nanoseconds (milliseconds_) ;
			const auto r3x = r1x + r2x ;
			mHeap.mTimeDuration = std::chrono::duration_cast<std::chrono::system_clock::duration> (r3x) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		LENGTH hours () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::hours> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH minutes () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::minutes> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH seconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH milliseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH microseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH nanoseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (mHeap.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		void add_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = a.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			mHeap.mTimeDuration = tmp_a.mTimeDuration + tmp_b.mTimeDuration ;
		}

		void sub_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = a.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			mHeap.mTimeDuration = tmp_a.mTimeDuration - tmp_b.mTimeDuration ;
		}
	} ;
} ;

template <>
exports auto TIMEDURATION_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

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
			const auto r1x = std::localtime ((&time_)) ;
			ret = (*r1x) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time_) const {
		using R1X = typename FUNCTION_calendar_from_timepoint_HELP<DEPEND ,ALWAYS>::FUNCTION_calendar_from_timepoint ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE (time_) ;
	}
} ;

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

template <class...>
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::Holder ;
	using CALENDAR = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::CALENDAR ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct HEAP {
		TIMEPOINT mTimePoint ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_now () override {
			mHeap.mTimePoint = std::chrono::system_clock::now () ;
		}

		void init_epoch () override {
			const auto r1x = TIMEDURATION (0) ;
			mHeap.mTimePoint = TIMEPOINT (r1x) ;
		}

		void init_calendar (CREF<CALENDAR> calendar_) override {
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
			mHeap.mTimePoint = std::chrono::system_clock::from_time_t (r5x) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		void add_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp_a = b.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mTimePoint = tmp_a.mTimePoint + tmp_b.mTimeDuration ;
		}

		void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp_a = b.native ().as (TYPEAS<CRef<HEAP>>::id).self ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mTimePoint = tmp_a.mTimePoint - tmp_b.mTimeDuration ;
		}

		CALENDAR calendar () const override {
			CALENDAR ret ;
			const auto r1x = std::time_t (std::chrono::system_clock::to_time_t (mHeap.mTimePoint)) ;
			const auto r2x = calendar_from_timepoint (r1x) ;
			ret.mYear = r2x.tm_year + 1900 ;
			ret.mMonth = r2x.tm_mon + 1 ;
			ret.mDay = r2x.tm_mday ;
			ret.mWDay = r2x.tm_wday + 1 ;
			ret.mYDay = r2x.tm_yday + 1 ;
			ret.mHour = r2x.tm_hour ;
			ret.mMinute = r2x.tm_min ;
			ret.mSecond = r2x.tm_sec ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto TIMEPOINT_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MUTEXLOCKER_HELP ;

template <class DEPEND>
trait MUTEXLOCKER_HELP<DEPEND ,ALWAYS> {
	using RECURSIVEMUTEX = std::recursive_mutex ;

	class MutexLocker extend Proxy {
	private:
		RECURSIVEMUTEX mBase ;

	public:
		imports VREF<MutexLocker> from (VREF<RECURSIVEMUTEX> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<MutexLocker>>::id] (unsafe_deptr (that))) ;
		}

		void enter () {
			noop () ;
		}

		void leave () {
			mBase.unlock () ;
		}
	} ;
} ;

using MutexLocker = typename MUTEXLOCKER_HELP<DEPEND ,ALWAYS>::MutexLocker ;

template <class...>
trait MUTEX_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MUTEX_HELP<DEPEND ,ALWAYS>::Holder ;
	using MUTEX = std::mutex ;
	using RECURSIVEMUTEX = std::recursive_mutex ;
	using CONDITIONNALMUTEX = std::condition_variable ;

	struct HEAP {
		Box<MUTEX> mMutex ;
		Box<RECURSIVEMUTEX> mRecursive ;
		Box<CONDITIONNALMUTEX> mConditional ;
	} ;

	class ImplHolder implement Holder {
	private:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_new () override {
			mHeap->mMutex = Box<MUTEX>::make () ;
		}

		void init_recursive () override {
			mHeap->mRecursive = Box<RECURSIVEMUTEX>::make () ;
		}

		void init_conditional () override {
			mHeap->mMutex = Box<MUTEX>::make () ;
			mHeap->mConditional = Box<CONDITIONNALMUTEX>::make () ;
		}

		Auto native () const override {
			return CRef<SharedRef<HEAP>>::reference (mHeap) ;
		}

		void enter () const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->lock () ;
			}
			if ifswitch (eax) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->lock () ;
			}
		}

		Auto try_enter () const override {
			if ifswitch (TRUE) {
				if (mHeap->mRecursive == NULL)
					discard ;
				const auto r1x = mHeap->mRecursive->try_lock () ;
				if ifnot (r1x)
					discard ;
				auto rax = Scope<VREF<MutexLocker>> (MutexLocker::from (mHeap->mRecursive.self)) ;
				return Auto (move (rax)) ;
			}
			return Auto () ;
		}

		void leave () const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mHeap->mMutex == NULL)
					discard ;
				mHeap->mMutex->unlock () ;
			}
			if ifswitch (eax) {
				if (mHeap->mRecursive == NULL)
					discard ;
				mHeap->mRecursive->unlock () ;
			}
		}
	} ;
} ;

template <>
exports auto MUTEX_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait CONDITIONALLOCK_IMPLHOLDER_HELP ;

template <class DEPEND>
trait CONDITIONALLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename CONDITIONALLOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using MUTEX = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::MUTEX ;
	using MUTEXHEAP = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using CONDITIONALLOCK = std::unique_lock<MUTEX> ;

	struct HEAP {
		SharedRef<MUTEXHEAP> mMutex ;
		CONDITIONALLOCK mLock ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_lock (CREF<Mutex> mutex_) override {
			//@mark
			mHeap.mMutex = mutex_.native ().as (TYPEAS<SharedRef<MUTEXHEAP>>::id) ;
			assert (mHeap.mMutex.exist ()) ;
			assert (mHeap.mMutex->mConditional.exist ()) ;
			mHeap.mLock = CONDITIONALLOCK (mHeap.mMutex->mMutex) ;
		}

		void wait () override {
			mHeap.mMutex->mConditional->wait (mHeap.mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutex->mConditional->wait_for (mHeap.mLock ,tmp.mTimeDuration) ;
		}

		void wait (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutex->mConditional->wait_until (mHeap.mLock ,tmp.mTimePoint) ;
		}

		void notify () override {
			mHeap.mMutex->mConditional->notify_all () ;
		}

		void yield () override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			mHeap.mMutex->mConditional->notify_all () ;
			auto &&tmp = TimeDuration::zero ().native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutex->mConditional->wait_for (mHeap.mLock ,tmp.mTimeDuration) ;
		}
	} ;
} ;

template <>
exports auto CONDITIONALLOCK_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename CONDITIONALLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SYSTEM_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		Cell<String<STR>> mLocale ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () {
			mHeap.mLocale.init (slice ("C")) ;
		}

		String<STR> get_locale () const override {
			return mHeap.mLocale.fetch () ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			mHeap.mLocale.store (name) ;
		}

		void execute (CREF<String<STR>> command) const override {
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (command) ;
			std::system ((&r1x[0])) ;
		}

		CREF<String<STR>> working_path () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}
	} ;
} ;

template <>
exports auto SYSTEM_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait RANDOM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RANDOM_HELP<DEPEND ,ALWAYS>::Holder ;
	using RANDOM = std::mt19937_64 ;

	struct HEAP {
		DATA mSeed ;
		RANDOM mRandom ;
	} ;

	class ImplHolder implement Holder {
	private:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_device () override {
			const auto r1x = DATA (invoke (std::random_device ())) ;
			init_seed (r1x) ;
		}

		void init_seed (CREF<DATA> seed_) override {
			mHeap->mSeed = seed_ ;
			mHeap->mRandom = RANDOM (csc_byte64_t (mHeap->mSeed)) ;
		}

		DATA seed () const override {
			return mHeap->mSeed ;
		}

		DATA random_byte () const override {
			return DATA (mHeap->mRandom ()) ;
		}
	} ;
} ;

template <>
exports auto RANDOM_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;