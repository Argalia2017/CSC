﻿#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"

namespace CSC {
template <class...>
trait CONSOLE_HELP ;

template <class...>
trait CONSOLE_IMPLHOLDER_HELP ;

template <class...>
trait CONSOLE_IMPLBINDER_HELP ;

template <class DEPEND>
trait CONSOLE_HELP<DEPEND ,ALWAYS> {
	using Binder = typename TEXTWRITER_HELP<STR ,ALWAYS>::Binder ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void enable_option (CREF<FLAG> option) const = 0 ;
		virtual void disable_option (CREF<FLAG> option) const = 0 ;
		virtual void print (CREF<Binder> msg) const = 0 ;
		virtual void fatal (CREF<Binder> msg) const = 0 ;
		virtual void error (CREF<Binder> msg) const = 0 ;
		virtual void warn (CREF<Binder> msg) const = 0 ;
		virtual void info (CREF<Binder> msg) const = 0 ;
		virtual void debug (CREF<Binder> msg) const = 0 ;
		virtual void verbose (CREF<Binder> msg) const = 0 ;
		virtual void link (CREF<String<STR>> dire) const = 0 ;
		virtual void open () const = 0 ;
		virtual void close () const = 0 ;
		virtual void pause () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	using OPTION_DEFAULT = RANK0 ;
	using OPTION_NO_PRINT = RANK1 ;
	using OPTION_NO_FATAL = RANK2 ;
	using OPTION_NO_ERROR = RANK3 ;
	using OPTION_NO_WARN = RANK4 ;
	using OPTION_NO_INFO = RANK5 ;
	using OPTION_NO_DEBUG = RANK6 ;
	using OPTION_NO_VERBOSE = RANK7 ;

	class Console {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
				ret.mMutex = RecursiveMutex () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		void enable_option (CREF<FLAG> option) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->enable_option (option) ;
		}

		void disable_option (CREF<FLAG> option) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->disable_option (option) ;
		}

		template <class...ARG1>
		void print (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->print (rax) ;
		}

		template <class...ARG1>
		void fatal (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->fatal (rax) ;
		}

		template <class...ARG1>
		void error (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->error (rax) ;
		}

		template <class...ARG1>
		void warn (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->warn (rax) ;
		}

		template <class...ARG1>
		void info (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->info (rax) ;
		}

		template <class...ARG1>
		void debug (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->debug (rax) ;
		}

		template <class...ARG1>
		void verbose (CREF<ARG1>...msg) const {
			using R1X = typename CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->verbose (rax) ;
		}

		void link (CREF<String<STR>> dire) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->link (dire) ;
		}

		void open () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->open () ;
		}

		void close () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->close () ;
		}

		void pause () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->pause () ;
		}

		void clear () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->clear () ;
		}
	} ;
} ;

template <class...UNIT>
trait CONSOLE_IMPLBINDER_HELP<TYPEAS<UNIT...> ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;

	class ImplBinder implement Binder {
	protected:
		CRef<Capture<UNIT...>> mThat ;

	public:
		implicit ImplBinder () = delete ;

		explicit ImplBinder (RREF<CRef<Capture<UNIT...>>> that) {
			mThat = move (that) ;
		}

		void friend_write (VREF<TextWriter<STR>> writer) const override {
			mThat.self ([&] (CREF<UNIT>...obj) {
				writer.prints (obj...) ;
			}) ;
		}
	} ;
} ;

using Console = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Console ;

template <class...>
trait REPORTER_HELP ;

template <class...>
trait REPORTER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait REPORTER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void detect_memory_leaks () const = 0 ;
		virtual void detect_crash_signal () const = 0 ;
		virtual Array<FLAG> captrue_stack_trace () const = 0 ;
		virtual String<STR> symbol_from_address (CREF<FLAG> addr) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Reporter {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Reporter> instance () {
			return memorize ([&] () {
				Reporter ret ;
				ret.mMutex = RecursiveMutex () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		void detect_memory_leaks () const {
			return mThis->detect_memory_leaks () ;
		}

		void detect_crash_signal () const {
			return mThis->detect_crash_signal () ;
		}

		Array<FLAG> captrue_stack_trace () const {
			return mThis->captrue_stack_trace () ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr) const {
			return mThis->symbol_from_address (addr) ;
		}
	} ;
} ;

using Reporter = typename REPORTER_HELP<DEPEND ,ALWAYS>::Reporter ;
} ;