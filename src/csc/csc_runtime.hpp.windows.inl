#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_runtime.hpp.default.inl"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

namespace CSC {
namespace RUNTIME {
template <class...>
trait RUNTIMEPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		LENGTH thread_concurrency () const override {
			return LENGTH (std::thread::hardware_concurrency ()) ;
		}

		FLAG thread_uid () const override {
			return FLAG (GetCurrentThreadId ()) ;
		}

		void thread_sleep (CREF<TimeDuration> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			std::this_thread::sleep_for (tmp.mTimeDuration) ;
		}

		void thread_sleep (CREF<TimePoint> time_) const override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			std::this_thread::sleep_until (tmp.mTimePoint) ;
		}

		void thread_yield () const override {
			std::this_thread::yield () ;
		}

		FLAG process_uid () const override {
			return FLAG (GetCurrentProcessId ()) ;
		}

		void process_exit () const override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () const override {
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
				terminate (me.self) ;
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

		FLAG thread_uid () const override {
			unimplemented () ;
			return bad (TYPEAS<FLAG>::id) ;
		}

		void execute (RREF<Function<void>> proc) override {
			terminate (mHeap) ;
			if ifswitch (TRUE) {
				auto rax = ConditionalLock (mHeap.mThreadMutex) ;
				mHeap.mThreadProc = move (proc) ;
				rax.notify () ;
			}
		}

		void join () override {
			terminate (mHeap) ;
		}

	private:
		imports void terminate (VREF<HEAP> me) {
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

		CREF<String<STR>> path () const override {
			unimplemented () ;
			return bad (TYPEAS<CREF<String<STR>>>::id) ;
		}

		CREF<String<STR>> name () const override {
			unimplemented () ;
			return bad (TYPEAS<CREF<String<STR>>>::id) ;
		}

		void open (CREF<String<STR>> file_) override {
			const auto r1x = File (file_).name () ;
			mHeap.mHandle = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				me = GetModuleHandle ((&r1x[0])) ;
				if (me != NULL)
					return ;
				me = LoadLibrary ((&r1x[0])) ;
				if (me != NULL)
					return ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mHeap.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		void close () override {
			mHeap.mHandle = UniqueRef<HMODULE> () ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mHeap.mError ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (mHeap.mHandle.exist ()) ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (name) ;
			FLAG ret = FLAG (GetProcAddress (mHeap.mHandle ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mHeap.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

	private:
		void format_dllerror (CREF<FLAG> code) {
			if ifswitch (TRUE) {
				if (mHeap.mErrorBuffer.size () > 0)
					discard ;
				mHeap.mErrorBuffer = String<STR>::make () ;
			}
			const auto r1x = DWORD (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			const auto r3x = mHeap.mErrorBuffer.size () ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,DWORD (code) ,r1x ,(&mHeap.mErrorBuffer[0]) ,DWORD (r3x) ,NULL) ;
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