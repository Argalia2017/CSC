#pragma once

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
trait CONSOLE_MESSAGE_HELP ;

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
		virtual void link (CREF<String<STR>> dire_) const = 0 ;
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
		RecursiveMutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
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
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->print (r2x) ;
		}

		template <class...ARG1>
		void fatal (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->fatal (r2x) ;
		}

		template <class...ARG1>
		void error (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->error (r2x) ;
		}

		template <class...ARG1>
		void warn (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->warn (r2x) ;
		}

		template <class...ARG1>
		void info (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->info (r2x) ;
		}

		template <class...ARG1>
		void debug (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->debug (r2x) ;
		}

		template <class...ARG1>
		void verbose (CREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X ,ALWAYS>::Message ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<ARG1>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->verbose (r2x) ;
		}

		void link (CREF<String<STR>> dire_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->link (dire_) ;
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

template <class UNIT1>
trait CONSOLE_MESSAGE_HELP<UNIT1 ,ALWAYS> {
	using Binder = typename TEXTWRITER_HELP<STR ,ALWAYS>::Binder ;

	class Message implement Binder {
	protected:
		CRef<UNIT1> mMessage ;

	public:
		implicit Message () = delete ;

		explicit Message (RREF<CRef<UNIT1>> message) {
			mMessage = move (message) ;
		}

		void friend_write (VREF<TextWriter<STR>> writer) const override {
			template_write (PHX ,writer ,mMessage.self) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,Tuple<>>>>
		void template_write (CREF<typeof (PH2)> ,VREF<TextWriter<STR>> writer ,CREF<ARG1> message) const {
			noop () ;
		}

		template <class ARG1>
		void template_write (CREF<typeof (PH1)> ,VREF<TextWriter<STR>> writer ,CREF<ARG1> message) const {
			writer << message.one ().self ;
			template_write (PHX ,writer ,message.rest ()) ;
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
		RecursiveMutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Reporter> instance () {
			return memorize ([&] () {
				Reporter ret ;
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