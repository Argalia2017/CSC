#pragma once
#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

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

template <>
trait TIMEDURATION_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct NODE {
		TIMEDURATION mTimeDuration ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () = default ;

		void init_zero () override {
			mNode.mTimeDuration = TIMEDURATION (0) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		LENGTH hours () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::hours> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH minutes () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::minutes> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH seconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH milliseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH microseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH nanoseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (mNode.mTimeDuration) ;
			return LENGTH (r1x.count ()) ;
		}

		void add_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = keep[TYPEAS<CREF<ImplHolder>>::id] (b) ;
			mNode.mTimeDuration = tmp_a.mNode.mTimeDuration + tmp_b.mNode.mTimeDuration ;
		}

		void sub_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = keep[TYPEAS<CREF<ImplHolder>>::id] (b) ;
			mNode.mTimeDuration = tmp_a.mNode.mTimeDuration - tmp_b.mNode.mTimeDuration ;
		}
	} ;
} ;

exports auto TIMEDURATION_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <>
trait TIMEPOINT_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename TIMEPOINT_HELP<ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct NODE {
		TIMEPOINT mTimePoint ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () = default ;

		void init_now () override {
			mNode.mTimePoint = std::chrono::system_clock::now () ;
		}

		void init_epoch () override {
			const auto r1x = TIMEDURATION (0) ;
			mNode.mTimePoint = TIMEPOINT (r1x) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		void add_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mNode.mTimePoint = tmp_a.mNode.mTimePoint + tmp_b.mTimeDuration ;
		}

		void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = b.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			mNode.mTimePoint = tmp_a.mNode.mTimePoint - tmp_b.mTimeDuration ;
		}
	} ;
} ;

exports auto TIMEPOINT_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MUTEXLOCKER_HELP ;

template <>
trait MUTEXLOCKER_HELP<ALWAYS> {
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

using MutexLocker = typename MUTEXLOCKER_HELP<ALWAYS>::MutexLocker ;

template <class...>
trait MUTEX_IMPLHOLDER_HELP ;

template <>
trait MUTEX_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename MUTEX_HELP<ALWAYS>::Holder ;
	using MUTEX = std::mutex ;
	using RECURSIVEMUTEX = std::recursive_mutex ;
	using CONDITIONNALMUTEX = std::condition_variable ;

	struct NODE {
		Box<MUTEX> mMutex ;
		Box<RECURSIVEMUTEX> mRecursive ;
		Box<CONDITIONNALMUTEX> mConditional ;
	} ;

	class ImplHolder implement Holder {
	private:
		SharedRef<NODE> mNode ;

	public:
		implicit ImplHolder () {
			mNode = SharedRef<NODE>::make () ;
		}

		void init_mutex () override {
			mNode->mMutex = Box<MUTEX>::make () ;
		}

		void init_recursive_mutex () override {
			mNode->mRecursive = Box<RECURSIVEMUTEX>::make () ;
		}

		void init_conditional_mutex () override {
			mNode->mMutex = Box<MUTEX>::make () ;
			mNode->mConditional = Box<CONDITIONNALMUTEX>::make () ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		void enter () const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mNode->mMutex == NULL)
					discard ;
				mNode->mMutex->lock () ;
			}
			if ifswitch (eax) {
				if (mNode->mRecursive == NULL)
					discard ;
				mNode->mRecursive->lock () ;
			}
		}

		AnyRef<> try_enter () const leftvalue override {
			AnyRef<> ret ;
			if ifswitch (TRUE) {
				if (mNode->mRecursive == NULL)
					discard ;
				const auto r1x = mNode->mRecursive->try_lock () ;
				if ifnot (r1x)
					discard ;
				auto rax = Scope<VREF<MutexLocker>> (MutexLocker::from (mNode->mRecursive.self)) ;
				ret = AnyRef<Scope<VREF<MutexLocker>>>::make (move (rax)) ;
			}
			return move (ret) ;
		}

		void leave () const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mNode->mMutex == NULL)
					discard ;
				mNode->mMutex->unlock () ;
			}
			if ifswitch (eax) {
				if (mNode->mRecursive == NULL)
					discard ;
				mNode->mRecursive->unlock () ;
			}
		}

		void notify () const override {
			if (mNode->mConditional == NULL)
				return ;
			mNode->mConditional->notify_all () ;
		}
	} ;
} ;

exports auto MUTEX_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait CONDITIONALLOCK_IMPLHOLDER_HELP ;

template <>
trait CONDITIONALLOCK_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename CONDITIONALLOCK_HELP<ALWAYS>::Holder ;
	using MUTEX = typename MUTEX_IMPLHOLDER_HELP<ALWAYS>::MUTEX ;
	using MUTEXNODE = typename MUTEX_IMPLHOLDER_HELP<ALWAYS>::NODE ;
	using CONDITIONALLOCK = std::unique_lock<MUTEX> ;

	struct NODE {
		SharedRef<MUTEXNODE> mMutexNode ;
		CONDITIONALLOCK mLock ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () = default ;

		void init_lock (CREF<Mutex> mutex_) {
			//@mark
			mNode.mMutexNode = mutex_.native ().as (TYPEAS<SharedRef<MUTEXNODE>>::id) ;
			assert (mNode.mMutexNode.exist ()) ;
			mNode.mLock = CONDITIONALLOCK (mNode.mMutexNode->mMutex) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		void wait () override {
			mNode.mMutexNode->mConditional->wait (mNode.mLock) ;
		}

		void wait (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp = time_.native ().as (TYPEAS<R1X>::id).mTimeDuration ;
			mNode.mMutexNode->mConditional->wait_for (mNode.mLock ,tmp) ;
		}

		void wait (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp = time_.native ().as (TYPEAS<R1X>::id).mTimePoint ;
			mNode.mMutexNode->mConditional->wait_until (mNode.mLock ,tmp) ;
		}

		void notify () override {
			mNode.mMutexNode->mConditional->notify_all () ;
		}

		void yield () override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			mNode.mMutexNode->mConditional->notify_all () ;
			auto &&tmp = TimeDuration::zero ().native ().as (TYPEAS<R1X>::id).mTimeDuration ;
			mNode.mMutexNode->mConditional->wait_for (mNode.mLock ,tmp) ;
		}
	} ;
} ;

exports auto CONDITIONALLOCK_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename CONDITIONALLOCK_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <>
trait SYSTEM_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename SYSTEM_HELP<ALWAYS>::Holder ;

	struct NODE {
		Cell<String<STR>> mLocale ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () = default ;

		String<STR> get_locale () const {
			return mNode.mLocale.fetch () ;
		}

		void set_locale (CREF<String<STR>> name) const override {
			mNode.mLocale.store (name) ;
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

exports auto SYSTEM_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait RANDOM_IMPLHOLDER_HELP ;

template <>
trait RANDOM_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename RANDOM_HELP<ALWAYS>::Holder ;
	using RANDOM = std::mt19937_64 ;

	struct NODE {
		DATA mSeed ;
		RANDOM mRandom ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () = default ;

		void init_new () override {
			const auto r1x = DATA (invoke (std::random_device ())) ;
			reset_seed (r1x) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		void reset_seed (CREF<DATA> seed) override {
			mNode.mSeed = seed ;
			mNode.mRandom = RANDOM (csc_byte64_t (mNode.mSeed)) ;
		}

		DATA random_byte () override {
			return DATA (mNode.mRandom ()) ;
		}

		void random_skip (CREF<LENGTH> size_) override {
			for (auto &&i : iter (0 ,size_)) {
				noop (i) ;
				const auto r1x = DATA (mNode.mRandom ()) ;
				noop () ;
			}
		}
	} ;
} ;

exports auto RANDOM_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;