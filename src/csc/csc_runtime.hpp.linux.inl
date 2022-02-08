#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_runtime.hpp.default.inl"

#include "begin.h"
#include <unistd.h>
#include <sys/syscall.h>
#include "end.h"

namespace CSC {
namespace RUNTIME {
template <class...>
trait RUNTIMEPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		FLAG thread_uid () {
			unimplemented () ;
			return bad (TYPEAS<FLAG>::id) ;
		}

		void thread_sleep (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			std::this_thread::sleep_for (tmp.mTimeDuration) ;
		}

		void thread_sleep (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			std::this_thread::sleep_until (tmp.mTimePoint) ;
		}

		void thread_yield () override {
			std::this_thread::yield () ;
		}

		FLAG process_uid () override {
			unimplemented () ;
			return bad (TYPEAS<FLAG>::id) ;
		}

		void process_exit () override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () override {
			unsafe_abort () ;
		}
	} ;
} ;

template <>
exports auto RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait THREAD_IMPLHOLDER_HELP ;

template <class DEPEND>
trait THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename THREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using THREADBLOCK = std::thread ;

	struct HEAP {
		Mutex mThreadMutex ;
		Cell<BOOL> mThreadFlag ;
		Function<void> mThreadProc ;
		THREADBLOCK mThreadBlock ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;
		UniqueRef<VRef<HEAP>> mCleaner ;

	public:
		implicit ImplHolder () {
			mHeap.mThreadMutex = Mutex::make_conditional () ;
			mCleaner = UniqueRef<VRef<HEAP>> ([&] (VREF<VRef<HEAP>> me) {
				me = VRef<HEAP>::reference (mHeap) ;
			} ,[] (VREF<VRef<HEAP>> me) {
				wait_for_terminate (me.self) ;
			}) ;
		}

		void init_new () override {
			mHeap.mThreadBlock = THREADBLOCK ([&] () {
				try_invoke ([&] () {
					auto rax = ConditionalLock (mHeap.mThreadMutex) ;
					mHeap.mThreadFlag = Cell<BOOL>::make (TRUE) ;
					while (TRUE) {
						while (TRUE) {
							if (mHeap.mThreadProc.exist ())
								break ;
							if ifnot (mHeap.mThreadFlag.fetch ())
								break ;
							rax.wait () ;
						}
						if ifnot (mHeap.mThreadFlag.fetch ())
							break ;
						mHeap.mThreadProc () ;
						mHeap.mThreadProc = Function<void> () ;
						rax.notify () ;
					}
				} ,[&] () {
					noop () ;
				}) ;
			}) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		FLAG thread_uid () const override {
			unimplemented () ;
			return bad (TYPEAS<FLAG>::id) ;
		}

		void execute (RREF<Function<void>> proc) override {
			wait_for_terminate (mHeap) ;
			if ifswitch (TRUE) {
				auto rax = ConditionalLock (mHeap.mThreadMutex) ;
				mHeap.mThreadProc = move (proc) ;
				rax.notify () ;
			}
		}

	private:
		imports void wait_for_terminate (VREF<HEAP> me) {
			if ifswitch (TRUE) {
				auto rax = ConditionalLock (me.mThreadMutex) ;
				me.mThreadFlag.store (FALSE) ;
				rax.notify () ;
			}
			me.mThreadBlock.join () ;
			me.mThreadFlag = NULL ;
			me.mThreadProc = Function<void> () ;
			me.mThreadBlock = THREADBLOCK () ;
		}
	} ;
} ;

template <>
exports auto THREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait PROCESS_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PROCESS_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PROCESS_HELP<DEPEND ,ALWAYS>::Holder ;
	using SNAPSHOT = typename PROCESS_HELP<DEPEND ,ALWAYS>::SNAPSHOT ;

	struct HEAP {

	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () {
			noop () ;
		}

		void init_snapshot (CREF<SNAPSHOT> snapshot_) override {
			unimplemented () ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		FLAG process_uid () const override {
			unimplemented () ;
			return bad (TYPEAS<FLAG>::id) ;
		}

		SNAPSHOT snapshot () const override {
			SNAPSHOT ret ;
			unimplemented () ;
			return move (ret) ;
		}

		BOOL alive () const override {
			unimplemented () ;
			return TRUE ;
		}
	} ;
} ;

template <>
exports auto PROCESS_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename PROCESS_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MODULE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MODULE_HELP<DEPEND ,ALWAYS>::Holder ;
	using HMODULE = csc_pointer_t ;

	struct HEAP {
		UniqueRef<HMODULE> mHandle ;
		String<STR> mErrorBuffer ;
		String<STR> mError ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () {
			noop () ;
		}

		void init_new () override {
			noop () ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mHeap.mError ;
		}

		CREF<String<STR>> path () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		CREF<String<STR>> name () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		void open (CREF<String<STR>> file_) override {
			const auto r1x = string_cvt[TYPEAS<STRA ,STR>::id] (file_) ;
			mHeap.mHandle = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				const auto r3x = VAL32 (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
				me = dlopen ((&r1x[0]) ,RTLD_NOLOAD | r3x) ;
				if (me != NULL)
					return ;
				me = dlopen ((&r1x[0]) ,r3x) ;
				if (me != NULL)
					return ;
				const auto r2x = FLAG (errno) ;
				format_dllerror () ;
				mHeap.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		void close () override {
			mHeap.mHandle = UniqueRef<HMODULE> () ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assume (mHeap.mHandle.exist ()) ;
			const auto r1x = string_cvt[TYPEAS<STRA ,STR>::id] (name) ;
			FLAG ret = FLAG (dlsym (mHeap.mHandle ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (errno) ;
				format_dllerror () ;
				mHeap.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

	private:
		void format_dllerror () {
			if ifswitch (TRUE) {
				if (mHeap.mErrorBuffer.size () > 0)
					discard ;
				mHeap.mErrorBuffer = String<STR>::make () ;
			}
			const auto r1x = dlerror () ;
			assume (r1x != NULL) ;
			auto &&tmp = unsafe_array ((*r1x)) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mHeap.mErrorBuffer.size ())
					break ;
				if (tmp[ix] == 0)
					break ;
				mHeap.mErrorBuffer[ix] = STR (tmp[ix]) ;
				ix++ ;
			}
			mHeap.mErrorBuffer[ix] = 0 ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;