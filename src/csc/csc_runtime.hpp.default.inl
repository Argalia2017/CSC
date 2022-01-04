#pragma once

#ifndef __CSC_RUNTIME__
#error "б╞(д├бузебу ;)д├ : require 'csc_runtime.hpp'"
#endif

#include "begin.h"
#include <cstdio>
#include <cstdlib>
#include <exception>
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

		void init_zero () override {
			mHeap.mTimeDuration = TIMEDURATION (0) ;
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
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::Holder ;
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
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<MutexLocker>>::id ,unsafe_deptr (that))) ;
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

		void init_mutex () override {
			mHeap->mMutex = Box<MUTEX>::make () ;
		}

		void init_recursive_mutex () override {
			mHeap->mRecursive = Box<RECURSIVEMUTEX>::make () ;
		}

		void init_conditional_mutex () override {
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
		SharedRef<MUTEXHEAP> mMutexNode ;
		CONDITIONALLOCK mLock ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_lock (CREF<Mutex> mutex_) override {
			//@mark
			mHeap.mMutexNode = mutex_.native ().as (TYPEAS<CRef<SharedRef<HEAP>>>::id).self ;
			assert (mHeap.mMutexNode.exist ()) ;
			assert (mHeap.mMutexNode->mConditional.exist ()) ;
			mHeap.mLock = CONDITIONALLOCK (mHeap.mMutexNode->mMutex) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		void wait () override {
			mHeap.mMutexNode->mConditional->wait (mHeap.mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutexNode->mConditional->wait_for (mHeap.mLock ,tmp.mTimeDuration) ;
		}

		void wait (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutexNode->mConditional->wait_until (mHeap.mLock ,tmp.mTimePoint) ;
		}

		void notify () override {
			mHeap.mMutexNode->mConditional->notify_all () ;
		}

		void yield () override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			mHeap.mMutexNode->mConditional->notify_all () ;
			auto &&tmp = TimeDuration::zero ().native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mHeap.mMutexNode->mConditional->wait_for (mHeap.mLock ,tmp.mTimeDuration) ;
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
			mHeap.mLocale.initialize (slice ("C")) ;
		}

		String<STR> get_locale () const override {
			return mHeap.mLocale.fetch () ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			mHeap.mLocale.store (name) ;
		}

		void execute (CREF<String<STR>> command) const override {
			unimplemented () ;
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
		HEAP mHeap ;

	public:
		implicit ImplHolder () = default ;

		void init_new () override {
			const auto r1x = DATA (invoke (std::random_device ())) ;
			reset_seed (r1x) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		void reset_seed (CREF<DATA> seed) override {
			mHeap.mSeed = seed ;
			mHeap.mRandom = RANDOM (csc_byte64_t (mHeap.mSeed)) ;
		}

		DATA random_byte () override {
			return DATA (mHeap.mRandom ()) ;
		}

		void random_skip (CREF<LENGTH> size_) override {
			for (auto &&i : iter (0 ,size_)) {
				noop (i) ;
				const auto r1x = DATA (mHeap.mRandom ()) ;
				noop (r1x) ;
			}
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