#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
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
		virtual void init_device () = 0 ;
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
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_device () ;
				return move (ret) ;
			}) ;
		}

		void enable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->enable_option (option) ;
		}

		void disable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->disable_option (option) ;
		}

		template <class...ARG1>
		void print (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<REMOVE_REF<ARG1>>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->print (r2x) ;
		}

		template <class...ARG1>
		void fatal (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->fatal (r2x) ;
		}

		template <class...ARG1>
		void error (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->error (r2x) ;
		}

		template <class...ARG1>
		void warn (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->warn (r2x) ;
		}

		template <class...ARG1>
		void info (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->info (r2x) ;
		}

		template <class...ARG1>
		void debug (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->debug (r2x) ;
		}

		template <class...ARG1>
		void verbose (XREF<ARG1>...msg) const {
			using R1X = Tuple<CRef<ARG1>...> ;
			using R2X = typename DEPENDENT<CONSOLE_MESSAGE_HELP<R1X ,ALWAYS> ,DEPEND>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = R1X (CRef<REMOVE_REF<ARG1>>::reference (msg)...) ;
			const auto r2x = R2X (CRef<R1X>::reference (r1x)) ;
			return mThis->verbose (r2x) ;
		}

		void link (CREF<String<STR>> dire_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->link (dire_) ;
		}

		void open () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->open () ;
		}

		void close () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->close () ;
		}

		void pause () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->pause () ;
		}

		void clear () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
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
			template_write (writer ,mMessage.self) ;
		}

	private:
		void template_write (VREF<TextWriter<STR>> writer ,CREF<Tuple<>> message) const {
			noop () ;
		}

		template <class ARG1>
		void template_write (VREF<TextWriter<STR>> writer ,XREF<ARG1> message) const {
			writer << message.one ().self ;
			template_write (writer ,message.rest ()) ;
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
		virtual void init_device () = 0 ;
		virtual void detect_memory_leaks () const = 0 ;
		virtual void detect_crash_signal () const = 0 ;
		virtual Array<FLAG> captrue_stack_trace () const = 0 ;
		virtual String<STR> symbol_from_address (CREF<FLAG> addr_) const = 0 ;
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
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_device () ;
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

		String<STR> symbol_from_address (CREF<FLAG> addr_) const {
			return mThis->symbol_from_address (addr_) ;
		}
	} ;
} ;

using Reporter = typename REPORTER_HELP<DEPEND ,ALWAYS>::Reporter ;
} ;