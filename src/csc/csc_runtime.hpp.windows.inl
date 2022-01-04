#pragma once

#ifndef __CSC_RUNTIME__
#error "б╞(д├бузебу ;)д├ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "б╞(д├бузебу ;)д├
#endif

#include "csc_runtime.hpp.default.inl"

#ifndef _INC_WINDOWS
#error "б╞(д├бузебу ;)д├ : require 'Windows.h'"
#endif

namespace CSC {
namespace RUNTIME {
template <class...>
trait THREAD_IMPLHOLDER_HELP ;

template <class DEPEND>
trait THREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename THREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using THREADBLOCK = std::thread ;

	struct HEAP {
		BOOL mCurrent ;
		Mutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Function<void> mThreadProc ;
		THREADBLOCK mThreadBlock ;
	} ;

	struct WRAP {
		SharedRef<HEAP> mHeap ;
	} ;

	class ImplHolder implement Holder {
	private:
		SharedRef<HEAP> mHeap ;
		UniqueRef<WRAP> mThis ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mCurrent = FALSE ;
			mHeap->mThreadMutex = Mutex::make_conditional_mutex () ;
			mThis = UniqueRef<WRAP> ([&] (VREF<WRAP> me) {
				me.mHeap = mHeap ;
			} ,[] (VREF<WRAP> me) {
				wait_for_terminate (me.mHeap.self) ;
			}) ;
		}

		void init_current () override {
			mHeap->mCurrent = TRUE ;
		}

		void init_new () override {
			mHeap->mThreadBlock = THREADBLOCK ([&] () {
				try_invoke ([&] () {
					auto rax = ConditionalLock::make_lock (mHeap->mThreadMutex) ;
					mHeap->mThreadFlag.self = VRef<BOOL>::make (TRUE) ;
					while (TRUE) {
						while (TRUE) {
							if (mHeap->mThreadProc.exist ())
								break ;
							if ifnot (mHeap->mThreadFlag.self)
								break ;
							rax.wait () ;
						}
						if ifnot (mHeap->mThreadFlag.self)
							break ;
						mHeap->mThreadProc () ;
						mHeap->mThreadProc = Function<void> () ;
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

		FLAG thread_id () const override {
			FLAG ret = ZERO ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap->mCurrent)
					discard ;
				ret = FLAG (GetCurrentThreadId ()) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
			return move (ret) ;
		}

		void execute (RREF<Function<void>> proc) override {
			assert (ifnot (mHeap->mCurrent)) ;
			wait_for_terminate (mHeap) ;
			if ifswitch (TRUE) {
				auto rax = ConditionalLock::make_lock (mHeap->mThreadMutex) ;
				mHeap->mThreadProc = move (proc) ;
				rax.notify () ;
			}
		}

		void sleep (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap->mCurrent)
					discard ;
				std::this_thread::sleep_for (tmp.mTimeDuration) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}

		void sleep (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = time_.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap->mCurrent)
					discard ;
				std::this_thread::sleep_until (tmp.mTimePoint) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}

		void yield () override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				std::this_thread::yield () ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}

	private:
		imports void wait_for_terminate (VREF<HEAP> me) {
			if ifswitch (TRUE) {
				auto rax = ConditionalLock::make_lock (me.mThreadMutex) ;
				me.mThreadFlag.self = FALSE ;
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

	struct HEAP {
		BOOL mCurrent ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () {
			mHeap.mCurrent = FALSE ;
		}

		void init_current () override {
			mHeap.mCurrent = TRUE ;
		}

		void init_snapshot (CREF<PROCESS_SNAPSHOT> info) override {
			unimplemented () ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		FLAG process_id () const override {
			FLAG ret = ZERO ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap.mCurrent)
					discard ;
				ret = FLAG (GetCurrentProcessId ()) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
			return move (ret) ;
		}

		PROCESS_SNAPSHOT process_snapshot () const override {
			PROCESS_SNAPSHOT ret ;
			unimplemented () ;
			return move (ret) ;
		}

		void process_exit () override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap.mCurrent)
					discard ;
				std::exit (EXIT_FAILURE) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}

		void process_abort () override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mHeap.mCurrent)
					discard ;
				unsafe_abort () ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
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
	using HMODULE = ::HMODULE ;

	struct HEAP {
		BOOL mCurrent ;
		UniqueRef<HMODULE> mHandle ;
		String<STR> mErrorBuffer ;
		String<STR> mError ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		implicit ImplHolder () {
			mHeap.mCurrent = FALSE ;
		}

		void init_current () override {
			mHeap.mCurrent = TRUE ;
		}

		void init_new () override {
			noop () ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		CREF<String<STR>> path () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		CREF<String<STR>> name () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		void open (CREF<String<STR>> file) override {
			assert (ifnot (mHeap.mCurrent)) ;
			const auto r1x = parse_file_name (file) ;
			mHeap.mHandle = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				me = GetModuleHandle (r1x.raw ().self) ;
				if (me != NULL)
					return ;
				me = LoadLibrary (file.raw ().self) ;
				if (me != NULL)
					return ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mHeap.mError = String<STR>::make_print (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		void close () override {
			assert (ifnot (mHeap.mCurrent)) ;
			mHeap.mHandle = UniqueRef<HMODULE> () ;
		}
		
		CREF<String<STR>> error () const leftvalue override {
			return mHeap.mError ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (ifnot (mHeap.mCurrent)) ;
			assert (mHeap.mHandle.exist ()) ;
			FLAG ret = FLAG (GetProcAddress (mHeap.mHandle ,name.raw ().self)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mHeap.mError = String<STR>::make_print (slice ("Error = ") ,r2x ,slice (" : ") ,mHeap.mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

	private:
		imports String<STR> parse_file_name (CREF<String<STR>> file) {
			String<STR> ret = String<STR>::make_print () ;
			unimplemented () ;
			return move (ret) ;
		}

		void format_dllerror (CREF<FLAG> code) {
			if ifswitch (TRUE) {
				if (mHeap.mErrorBuffer.size () > 0)
					discard ;
				mHeap.mErrorBuffer = String<STR>::make_print () ;
			}
			const auto r3x = DWORD (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,DWORD (code) ,r3x ,mHeap.mErrorBuffer.raw ().self ,DWORD (mHeap.mErrorBuffer.size ()) ,NULL) ;
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