#pragma once

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

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

template <>
trait THREAD_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename THREAD_HELP<ALWAYS>::Holder ;
	using THREADBLOCK = std::thread ;

	struct NODE {
		BOOL mCurrent ;
		Box<Mutex> mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Function<void> mThreadProc ;
		THREADBLOCK mThreadBlock ;
	} ;

	struct WRAP {
		VRef<NODE> mNode ;
	} ;

	class ImplHolder implement Holder {
	private:
		SharedRef<NODE> mNode ;
		UniqueRef<WRAP> mThis ;

	public:
		implicit ImplHolder () {
			mNode = SharedRef<NODE>::make () ;
			mNode->mCurrent = FALSE ;
			mNode->mThreadMutex = Box<Mutex>::make (Mutex::make_conditional_mutex ()) ;
			mThis = UniqueRef<WRAP> ([&] (VREF<WRAP> me) {
				me.mNode = VRef<NODE>::reference (mNode) ;
			} ,[] (VREF<WRAP> me) {
				wait_for_terminate (me.mNode.self) ;
			}) ;
		}

		void init_current () override {
			mNode->mCurrent = TRUE ;
		}

		void init_new () override {
			mNode->mThreadBlock = THREADBLOCK ([&] () {
				try_invoke ([&] () {
					if ifswitch (TRUE) {
						Scope<CREF<Mutex>> anonymous (mNode->mThreadMutex) ;
						mNode->mThreadFlag.self = VRef<BOOL>::make (TRUE) ;
					}
					while (TRUE) {
						auto rax = ConditionalLock::make_lock (mNode->mThreadMutex.self) ;
						if ifnot (mNode->mThreadFlag.self)
							return ;
						while (TRUE) {
							if (mNode->mThreadProc.exist ())
								break ;
							if ifnot (mNode->mThreadFlag.self)
								return ;
							rax.wait () ;
						}
						mNode->mThreadProc () ;
						mNode->mThreadProc = Function<void> () ;
						rax.notify () ;
					}
				} ,[&] () {
					noop () ;
				}) ;
			}) ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		FLAG thread_id () const override {
			FLAG ret = ZERO ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mNode->mCurrent)
					discard ;
				ret = FLAG (GetCurrentThreadId ()) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
			return move (ret) ;
		}

		void execute (RREF<Function<void>> proc) override {
			assert (ifnot (mNode->mCurrent)) ;
			wait_for_terminate (mNode) ;
			if ifswitch (TRUE) {
				Scope<CREF<Mutex>> anonymous (mNode->mThreadMutex) ;
				mNode->mThreadProc = move (proc) ;
				mNode->mThreadMutex->notify () ;
			}
		}

		void sleep (CREF<TimeDuration> time_) override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp = time_.native ().as (TYPEAS<R1X>::id).mTimeDuration ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mNode->mCurrent)
					discard ;
				std::this_thread::sleep_for (tmp) ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}

		void sleep (CREF<TimePoint> time_) override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<ALWAYS>::NODE ;
			auto &&tmp = time_.native ().as (TYPEAS<R1X>::id).mTimePoint ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mNode->mCurrent)
					discard ;
				std::this_thread::sleep_until (tmp) ;
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
		imports void wait_for_terminate (VREF<NODE> me) {
			if ifswitch (TRUE) {
				Scope<CREF<Mutex>> anonymous (me.mThreadMutex) ;
				me.mThreadFlag.self = FALSE ;
			}
			me.mThreadBlock.join () ;
			me.mThreadFlag = NULL ;
			me.mThreadProc = Function<void> () ;
			me.mThreadBlock = THREADBLOCK () ;
		}
	} ;
} ;

exports auto THREAD_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename THREAD_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

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
		NODE mNode ;

	public:
		implicit ImplHolder () {
			mNode.mCurrent = FALSE ;
		}

		void init_current () override {
			mNode.mCurrent = TRUE ;
		}

		void init_snapshot (CREF<PROCESS_SNAPSHOT> info) override {
			unimplemented () ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
		}

		FLAG process_id () const override {
			FLAG ret = ZERO ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mNode.mCurrent)
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
				if ifnot (mNode.mCurrent)
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
				if ifnot (mNode.mCurrent)
					discard ;
				unsafe_abort () ;
			}
			if ifswitch (eax) {
				unimplemented () ;
			}
		}
	} ;
} ;

exports auto PROCESS_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename PROCESS_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait MODULE_IMPLHOLDER_HELP ;

template <>
trait MODULE_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename MODULE_HELP<ALWAYS>::Holder ;
	using HMODULE = ::HMODULE ;
	using MESSAGE_SIZE = ENUMAS<VAL ,ENUMID<8192>> ;

	struct NODE {
		BOOL mCurrent ;
		UniqueRef<HMODULE> mHandle ;
		String<STR> mErrorBuffer ;
		String<STR> mError ;
	} ;

	class ImplHolder implement Holder {
	private:
		NODE mNode ;

	public:
		implicit ImplHolder () {
			mNode.mCurrent = FALSE ;
		}

		void init_current () override {
			mNode.mCurrent = TRUE ;
		}

		void init_new () override {
			noop () ;
		}

		Auto native () const override {
			return CRef<NODE>::reference (mNode) ;
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
			assert (ifnot (mNode.mCurrent)) ;
			const auto r1x = parse_file_name (file) ;
			mNode.mHandle = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				me = GetModuleHandle (r1x.raw ().self) ;
				if (me != NULL)
					return ;
				me = LoadLibrary (file.raw ().self) ;
				if (me != NULL)
					return ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mNode.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mNode.mErrorBuffer) ;
				dynamic_assert (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		void close () override {
			assert (ifnot (mNode.mCurrent)) ;
			mNode.mHandle = UniqueRef<HMODULE> () ;
		}
		
		CREF<String<STR>> error () const leftvalue override {
			return mNode.mError ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (ifnot (mNode.mCurrent)) ;
			assert (mNode.mHandle.exist ()) ;
			FLAG ret = FLAG (GetProcAddress (mNode.mHandle ,name.raw ().self)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mNode.mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mNode.mErrorBuffer) ;
				dynamic_assert (FALSE) ;
			}
			return move (ret) ;
		}

	private:
		imports String<STR> parse_file_name (CREF<String<STR>> file) {
			String<STR> ret = String<STR> (MESSAGE_SIZE::value) ;
			unimplemented () ;
			return move (ret) ;
		}

		void format_dllerror (CREF<FLAG> code) {
			if ifswitch (TRUE) {
				if (mNode.mErrorBuffer.size () > 0)
					discard ;
				mNode.mErrorBuffer = String<STR> (MESSAGE_SIZE::value) ;
			}
			const auto r3x = DWORD (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,DWORD (code) ,r3x ,mNode.mErrorBuffer.raw ().self ,DWORD (MESSAGE_SIZE::value) ,NULL) ;
		}
	} ;
} ;

exports auto MODULE_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;