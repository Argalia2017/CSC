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

#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef _INC_WINDOWS
#error "б╞(д├бузебу ;)д├ : require 'Windows.h'"
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace CSC {
namespace RUNTIME {
template <class...>
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <>
trait TIMEPOINT_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename TIMEPOINT_HELP<ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct NODE {
		TIMEPOINT mData ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mTimePoint ;
	
	public:
		implicit ImplHolder () = default ;

		void init_now () override {
			mTimePoint.mData = std::chrono::system_clock::now () ;
		}

		void init_epoch () override {
			const auto r1x = TIMEDURATION (0) ;
			mTimePoint.mData = TIMEPOINT (r1x) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mTimePoint) ;
		}
	} ;
} ;

exports auto TIMEPOINT_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait TIMEDURATION_IMPLHOLDER_HELP ;

template <>
trait TIMEDURATION_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename TIMEDURATION_HELP<ALWAYS>::Holder ;
	using TIMEPOINT = std::chrono::system_clock::time_point ;
	using TIMEDURATION = std::chrono::system_clock::duration ;

	struct NODE {
		TIMEDURATION mData ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mDuration ;

	public:
		implicit ImplHolder () = default ;

		void init_zero () override {
			mDuration.mData = TIMEDURATION (0) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mDuration) ;
		}

		LENGTH hours () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::hours> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH minutes () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::minutes> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH seconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH milliseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH microseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		LENGTH nanoseconds () const override {
			const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (mDuration.mData) ;
			return LENGTH (r1x.count ()) ;
		}

		void add_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = keep[TYPEAS<CREF<ImplHolder>>::id] (b) ;
			mDuration.mData = tmp_a.mDuration.mData + tmp_b.mDuration.mData ;
		}

		void sub_from (CREF<Holder> a ,CREF<Holder> b) override {
			auto &&tmp_a = keep[TYPEAS<CREF<ImplHolder>>::id] (a) ;
			auto &&tmp_b = keep[TYPEAS<CREF<ImplHolder>>::id] (b) ;
			mDuration.mData = tmp_a.mDuration.mData - tmp_b.mDuration.mData ;
		}
	} ;
} ;

exports auto TIMEDURATION_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MUTEX_IMPLHOLDER_HELP ;

template <>
trait MUTEX_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename MUTEX_HELP<ALWAYS>::Holder ;

	struct NODE {
		INDEX mIndex ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mMutex ;

	public:
		implicit ImplHolder () {
			mMutex.mIndex = NONE ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mMutex) ;
		}

		void enter () const override {
			//mMutex->lock () ;
		}

		BOOL try_enter () const override {
			//return mMutex->try_lock () ;
			return FALSE ;
		}

		void leave () const override {
			//mMutex->unlock () ;
		}
	} ;
} ;

exports auto MUTEX_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename MUTEX_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait CONDITIONALLOCK_IMPLHOLDER_HELP ;

template <>
trait CONDITIONALLOCK_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename CONDITIONALLOCK_HELP<ALWAYS>::Holder ;

	struct NODE {
		INDEX mIndex ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mConditionalLock ;

	public:
		implicit ImplHolder () {
			mConditionalLock.mIndex = NONE ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mConditionalLock) ;
		}
	} ;
} ;

exports auto CONDITIONALLOCK_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename CONDITIONALLOCK_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait FUNCTION_current_thread_id_HELP ;

template <class UNIT1>
trait FUNCTION_current_thread_id_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_WINDOWS<UNIT1>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_thread_id {
		inline FLAG operator() () const {
			return FLAG (GetCurrentThreadId ()) ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_current_thread_id_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_LINUX<UNIT1>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_thread_id {
		inline FLAG operator() () const {
			return FLAG (syscall (SYS_gettid)) ;
		}
	} ;
#endif
} ;

struct FUNCTION_current_thread_id {
	inline LENGTH operator() () const {
		using R1X = typename FUNCTION_current_thread_id_HELP<void ,ALWAYS>::FUNCTION_current_thread_id ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto current_thread_id = FUNCTION_current_thread_id () ;

template <class...>
trait THREAD_IMPLHOLDER_HELP ;

template <>
trait THREAD_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename THREAD_HELP<ALWAYS>::Holder ;

	struct NODE {
		BOOL mCurrent ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mThread ;

	public:
		implicit ImplHolder () {
			mThread.mCurrent = FALSE ;
		}

		void init_current () override {
			mThread.mCurrent = TRUE ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mThread) ;
		}

		FLAG thread_id () const override {
			FLAG ret = ZERO ;
			if ifswitch (TRUE) {
				if ifnot (mThread.mCurrent)
					discard ;
				ret = current_thread_id () ;
			}
			return move (ret) ;
		}

		void sleep (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			const auto r1x = time_.native ().as (TYPEAS<VRef<R1X>>::id) ;
			std::this_thread::sleep_until (r1x->mData) ;
		}

		void sleep (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			const auto r1x = time_.native ().as (TYPEAS<VRef<R1X>>::id) ;
			std::this_thread::sleep_for (r1x->mData) ;
		}

		void yield () override {
			std::this_thread::yield () ;
		}
	} ;
} ;

exports auto THREAD_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait FUNCTION_current_process_id_HELP ;

template <class UNIT1>
trait FUNCTION_current_process_id_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_WINDOWS<UNIT1>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_process_id {
		inline FLAG operator() () const {
			return FLAG (GetCurrentProcessId ()) ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_current_process_id_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_LINUX<UNIT1>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_process_id {
		inline FLAG operator() () const {
			return FLAG (syscall (SYS_getpid)) ;
		}
	} ;
#endif
} ;

struct FUNCTION_current_process_id {
	inline LENGTH operator() () const {
		using R1X = typename FUNCTION_current_process_id_HELP<void ,ALWAYS>::FUNCTION_current_process_id ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	}
} ;

static constexpr auto current_process_id = FUNCTION_current_process_id () ;

template <class...>
trait PROCESS_IMPLHOLDER_HELP ;

template <>
trait PROCESS_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename PROCESS_HELP<ALWAYS>::Holder ;

	struct NODE {
		BOOL mCurrent ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mProcess ;

	public:
		implicit ImplHolder () {
			mProcess.mCurrent = FALSE ;
		}

		void init_current () override {
			mProcess.mCurrent = TRUE ;
		}

		void init_process_info (CREF<PROCESS_INFO> info) override {
			mProcess.mCurrent = TRUE ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mProcess) ;
		}

		FLAG process_id () const override {
			FLAG ret = ZERO ;
			if ifswitch (TRUE) {
				if ifnot (mProcess.mCurrent)
					discard ;
				ret = current_process_id () ;
			}
			return move (ret) ;
		}

		PROCESS_INFO process_info () const override {
			PROCESS_INFO ret ;
			//
			return move (ret) ;
		}

		void process_exit () override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () override {
			std::terminate () ;
		}
	} ;
} ;

exports auto PROCESS_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename PROCESS_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MODULE_IMPLHOLDER_HELP ;

template <>
trait MODULE_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename MODULE_HELP<ALWAYS>::Holder ;

	struct NODE {
		BOOL mCurrent ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mModule ;

	public:
		implicit ImplHolder () {
			mModule.mCurrent = FALSE ;
		}

		void init_current () override {
			mModule.mCurrent = TRUE ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mModule) ;
		}
	} ;
} ;

exports auto MODULE_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <>
trait SYSTEM_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename SYSTEM_HELP<ALWAYS>::Holder ;

	struct NODE {
		String<STR> mLocale ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mSystem ;

	public:
		implicit ImplHolder () {
			mSystem.mLocale = slice ("C") ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mSystem) ;
		}

		void set_locale (CREF<String<STR>> name) override {
			mSystem.mLocale = name ;
		}

		void execute (CREF<String<STR>> command) override {
			//std::system () ;
		}
	} ;
} ;

exports auto SYSTEM_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename SYSTEM_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait RANDOM_IMPLHOLDER_HELP ;

template <>
trait RANDOM_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename RANDOM_HELP<ALWAYS>::Holder ;

	struct NODE {
		std::mt19937_64 mData ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mRandom ;

	public:
		implicit ImplHolder () {
			mRandom.mData = std::mt19937_64 () ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mRandom) ;
		}
	} ;
} ;

exports auto RANDOM_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> VRef<Holder> {
	using R1X = typename RANDOM_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;