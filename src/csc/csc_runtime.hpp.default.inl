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
template <class DEPEND>
trait TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	class ImplHolder implement Holder {
	protected:
		TIMEDURATION mTimeDuration ;

	public:
		implicit ImplHolder () = default ;

		void init_second (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) override {
			const auto r1x = std::chrono::milliseconds (milliseconds_) ;
			const auto r2x = std::chrono::nanoseconds (milliseconds_) ;
			const auto r3x = r1x + r2x ;
			mTimeDuration = std::chrono::duration_cast<std::chrono::system_clock::duration> (r3x) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		CREF<TIMEDURATION> get_mTimeDuration () const leftvalue {
			return mTimeDuration ;
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

		void add_from (CREF<Holder> a ,CREF<Holder> b) override {
			const auto r1x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			mTimeDuration = r1x->get_mTimeDuration () + r2x->get_mTimeDuration () ;
		}

		void sub_from (CREF<Holder> a ,CREF<Holder> b) override {
			const auto r1x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			mTimeDuration = r1x->get_mTimeDuration () - r2x->get_mTimeDuration () ;
		}
	} ;
} ;

template <>
exports auto TIMEDURATION_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

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

template <class DEPEND>
trait TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::Holder ;
	using CALENDAR = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::CALENDAR ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	class ImplHolder implement Holder {
	protected:
		TIMEPOINT mTimePoint ;

	public:
		implicit ImplHolder () = default ;

		void init_now () override {
			mTimePoint = std::chrono::system_clock::now () ;
		}

		void init_epoch () override {
			const auto r1x = TIMEDURATION (0) ;
			mTimePoint = TIMEPOINT (r1x) ;
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
			mTimePoint = std::chrono::system_clock::from_time_t (r5x) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		CREF<TIMEPOINT> get_mTimePoint () const leftvalue {
			return mTimePoint ;
		}

		void add_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = b.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			mTimePoint = r1x->get_mTimePoint () + r2x->get_mTimeDuration () ;
		}

		void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = b.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			mTimePoint = r1x->get_mTimePoint () - r2x->get_mTimeDuration () ;
		}

		CALENDAR calendar () const override {
			CALENDAR ret ;
			const auto r1x = std::time_t (std::chrono::system_clock::to_time_t (mTimePoint)) ;
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

		std::tm calendar_from_timepoint (CREF<std::time_t> time_) const {
			using R1X = typename FUNCTION_calendar_from_timepoint_HELP<DEPEND ,ALWAYS>::FUNCTION_calendar_from_timepoint ;
			static constexpr auto M_INVOKE = R1X () ;
			return M_INVOKE (time_) ;
		}
	} ;
} ;

template <>
exports auto TIMEPOINT_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
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

		void init_new () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mAtomic.store (0) ;
		}

		VAL fetch () const override {
			return mHeap->mAtomic.load () ;
		}

		void store (CREF<VAL> obj) const override {
			return mHeap->mAtomic.store (obj) ;
		}

		VAL exchange (CREF<VAL> obj) const override {
			return mHeap->mAtomic.exchange (obj) ;
		}

		void replace (CREF<VAL> expect ,CREF<VAL> obj) const override {
			auto rax = expect ;
			const auto r1x = mHeap->mAtomic.compare_exchange_strong (rax ,obj) ;
			noop (r1x) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> obj) const override {
			return mHeap->mAtomic.compare_exchange_weak (expect ,obj) ;
		}

		VAL fetch_add (CREF<VAL> obj) const override {
			return mHeap->mAtomic.fetch_add (obj) ;
		}

		VAL fetch_sub (CREF<VAL> obj) const override {
			return mHeap->mAtomic.fetch_sub (obj) ;
		}
	} ;
} ;

template <>
exports auto ATOMIC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
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
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_mutex () override {
			mHeap->mMutex = Box<std::mutex>::make () ;
		}

		void init_recursive () override {
			mHeap->mRecursive = Box<std::recursive_mutex>::make () ;
		}

		void init_conditional () override {
			mHeap->mMutex = Box<std::mutex>::make () ;
			mHeap->mConditional = Box<std::condition_variable>::make () ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		CREF<SharedRef<HEAP>> get_mHeap () const leftvalue {
			return mHeap ;
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
exports auto MUTEX_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait CONDITIONALLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename CONDITIONALLOCK_HELP<DEPEND ,ALWAYS>::Holder ;
	using HEAP = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;
		std::unique_lock<std::mutex> mLock ;

	public:
		implicit ImplHolder () = default ;

		void init_lock (CREF<Mutex> mutex_) override {
			using R1X = typename MUTEX_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = mutex_.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			auto &&tmp = r1x->get_mHeap () ;
			mLock = std::unique_lock<std::mutex> (tmp->mMutex) ;
			mHeap = tmp ;
			assert (mHeap->mConditional.exist ()) ;
		}

		void wait () override {
			mHeap->mConditional->wait (mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			mHeap->mConditional->wait_for (mLock ,r1x->get_mTimeDuration ()) ;
		}

		void wait (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			mHeap->mConditional->wait_until (mLock ,r1x->get_mTimePoint ()) ;
		}

		void notify () override {
			mHeap->mConditional->notify_all () ;
		}

		void yield () override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			mHeap->mConditional->notify_all () ;
			const auto r1x = TimeDuration::zero ().native ().poll (TYPEAS<CRef<R1X>>::id) ;
			mHeap->mConditional->wait_for (mLock ,r1x->get_mTimeDuration ()) ;
		}
	} ;
} ;

template <>
exports auto CONDITIONALLOCK_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename CONDITIONALLOCK_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::id) ;
	return move (ret) ;
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

		void init_device () override {
			mLocale = Cell<String<STR>>::make (slice ("C")) ;
			mWorkingPath = RuntimeProc::working_path () ;
		}

		String<STR> get_locale () const override {
			return mLocale.fetch () ;
		}

		void set_locale (RREF<String<STR>> name) const override {
			mLocale.store (move (name)) ;
		}

		void execute (CREF<String<STR>> command) const override {
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (command) ;
			const auto r2x = std::system ((&r1x[0])) ;
			noop (r2x) ;
		}

		CREF<String<STR>> working_path () const override {
			return mWorkingPath ;
		}
	} ;
} ;

template <>
exports auto SYSTEM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
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

	public:
		implicit ImplHolder () = default ;

		void init_new () override {
			mUID = ZERO ;
			mIndex = NONE ;
		}

		FLAG thread_uid () const override {
			return mUID ;
		}

		void start (RREF<VRef<Binder>> binder ,CREF<INDEX> index) override {
			assume (mBlock != NULL) ;
			mBinder = move (binder) ;
			mIndex = index ;
			mBlock = VRef<std::thread>::make ([&] () {
				try_invoke ([&] () {
					mUID = RuntimeProc::thread_uid () ;
					mBinder->execute (mIndex) ;
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
				mBinder = NULL ;
				mUID = ZERO ;
			}
		}
	} ;
} ;

template <>
exports auto THREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
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
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_device () override {
			const auto r1x = DATA (invoke (std::random_device ())) ;
			init_seed (r1x) ;
		}

		void init_seed (CREF<DATA> seed_) override {
			mHeap->mSeed = seed_ ;
			mHeap->mRandom = std::mt19937_64 (csc_byte64_t (mHeap->mSeed)) ;
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
exports auto RANDOM_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;