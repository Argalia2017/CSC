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
#include "csc_filesystem.hpp"

namespace CSC {
namespace DEBUGGER {
template <class...>
trait CONSOLE_HELP ;

template <class...>
trait CONSOLE_MESSAGE_HELP ;

template <class UNIT1>
trait CONSOLE_MESSAGE_HELP<UNIT1 ,ALWAYS> {
	using Binder = typename TEXTWRITER_BINDER_HELP<STR ,ALWAYS>::Binder ;

	class Message implement Binder {
	private:
		VRef<UNIT1> mMessage ;

	public:
		implicit Message () = delete ;

		explicit Message (RREF<VRef<UNIT1>> message) {
			mMessage = move (message) ;
		}

		void friend_write (VREF<TextWriter<STR>> writer) const override {
			template_write (writer ,mMessage) ;
		}

	private:
		imports void template_write (VREF<TextWriter<STR>> writer ,CREF<Tuple<>> message) {
			noop () ;
		}

		template <class ARG1>
		imports void template_write (VREF<TextWriter<STR>> writer ,XREF<ARG1> message) {
			writer << message.first_one () ;
			template_write (writer ,message.first_rest ()) ;
		}
	} ;
} ;

template <class DEPEND>
trait CONSOLE_HELP<DEPEND ,ALWAYS> {
	using Binder = typename TEXTWRITER_BINDER_HELP<STR ,ALWAYS>::Binder ;

	struct Holder implement Interface {
		virtual LENGTH buffer_size () const = 0 ;
		virtual void enable_option (CREF<FLAG> option) const = 0 ;
		virtual void disable_option (CREF<FLAG> option) const = 0 ;
		virtual void print (CREF<Binder> msg) const = 0 ;
		virtual void fatal (CREF<Binder> msg) const = 0 ;
		virtual void error (CREF<Binder> msg) const = 0 ;
		virtual void warn (CREF<Binder> msg) const = 0 ;
		virtual void info (CREF<Binder> msg) const = 0 ;
		virtual void debug (CREF<Binder> msg) const = 0 ;
		virtual void verbose (CREF<Binder> msg) const = 0 ;
		virtual void link (CREF<String<STR>> file_) const = 0 ;
		virtual void show () const = 0 ;
		virtual void hide () const = 0 ;
		virtual void pause () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Console {
	private:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_link::invoke () ;
				return move (ret) ;
			}) ;
		}

		LENGTH buffer_size () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->buffer_size () ;
		}

		void enable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->enable_option (option) ;
		}

		void disable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->disable_option (option) ;
		}

		template <class...ARGS>
		void print (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			print (r1x) ;
		}

		template <class...ARGS>
		void fatal (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			fatal (r1x) ;
		}

		template <class...ARGS>
		void error (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			error (r1x) ;
		}

		template <class...ARGS>
		void warn (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			warn (r1x) ;
		}

		template <class...ARGS>
		void info (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			info (r1x) ;
		}

		template <class...ARGS>
		void debug (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			debug (r1x) ;
		}

		template <class...ARGS>
		void verbose (XREF<ARGS>...msg) const {
			using R1X = TupleBinder<XREF<ARGS>...> ;
			using R2X = typename CONSOLE_MESSAGE_HELP<R1X>::Message ;
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			auto rax = TEMP<R1X> () ;
			const auto r1x = R2X (R1X::from (msg...)) ;
			verbose (r1x) ;
		}

		void link (CREF<String<STR>> file_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->link (file_) ;
		}

		void show () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->show () ;
		}

		void pause () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->pause () ;
		}

		void clear () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->clear () ;
		}

		void hide () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->hide () ;
		}
	} ;
} ;

using Console = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Console ;
} ;
} ;

namespace CSC {
using namespace DEBUGGER ;
} ;