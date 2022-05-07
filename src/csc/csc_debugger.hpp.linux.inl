#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "begin.h"
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include "end.h"

namespace CSC {
template <class DEPEND>
trait CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Holder ;

	using HANDLE = csc_pointer_t ;
	using CONSOLE_BUFFER_SSIZE = ENUMAS<VAL ,ENUMID<8388607>> ;

	using OPTION_DEFAULT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_DEFAULT ;
	using OPTION_NO_PRINT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_PRINT ;
	using OPTION_NO_FATAL = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_FATAL ;
	using OPTION_NO_ERROR = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_ERROR ;
	using OPTION_NO_WARN = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_WARN ;
	using OPTION_NO_INFO = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_INFO ;
	using OPTION_NO_DEBUG = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_DEBUG ;
	using OPTION_NO_VERBOSE = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_VERBOSE ;
	using OPTION_SIZE = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_SIZE ;

	struct HEAP {
		String<STR> mConBuffer ;
		TextWriter<STR> mConWriter ;
		String<STRU8> mLogBuffer ;
		TextWriter<STRU8> mLogWriter ;
		BitSet<OPTION_SIZE> mOption ;
		UniqueRef<HANDLE> mConsole ;
		String<STR> mLogFile ;
		String<STR> mOldLogFile ;
		VRef<StreamFile> mLogStreamFile ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			set_buffer_size (CONSOLE_BUFFER_SSIZE::value) ;
		}

		void set_buffer_size (CREF<LENGTH> size_) const {
			mHeap->mConBuffer = String<STR> (size_) ;
			mHeap->mConWriter = TextWriter<STR> (mHeap->mConBuffer.raw ()) ;
			mHeap->mLogBuffer = String<STRU8> (size_) ;
			mHeap->mLogWriter = TextWriter<STRU8> (mHeap->mLogBuffer.raw ()) ;
		}

		void enable_option (CREF<FLAG> option) const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (option != OPTION_DEFAULT::value)
					discard ;
				mHeap->mOption.clear () ;
			}
			if ifswitch (rxx) {
				mHeap->mOption.add (option) ;
			}
		}

		void disable_option (CREF<FLAG> option) const override {
			mHeap->mOption.erase (option) ;
		}

		void print (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_PRINT::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("%s\n" ,(&r1x[0])) ;
			log (slice ("PRINT")) ;
		}

		void fatal (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_FATAL::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;34m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("FATAL")) ;
		}

		void error (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_ERROR::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;31m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("ERROR")) ;
		}

		void warn (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_WARN::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;33m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("WARN")) ;
		}

		void info (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_INFO::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;32m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("INFO")) ;
		}

		void debug (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_DEBUG::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;36m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("DEBUG")) ;
		}

		void verbose (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_VERBOSE::value])
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (mHeap->mConBuffer) ;
			printf ("\033[1;37m%s\033[0m\n" ,(&r1x[0])) ;
			log (slice ("VERBOSE")) ;
		}

		void write_con_buffer (CREF<Binder> msg) const {
			mHeap->mConWriter << TextWriter<STR>::CLS ;
			mHeap->mConWriter << msg ;
			mHeap->mConWriter << TextWriter<STR>::EOS ;
		}

		void attach_console () const {
			if (mHeap->mConsole.exist ())
				return ;
			mHeap->mConsole = UniqueRef<HANDLE>::make (NULL) ;
		}

		void link (CREF<String<STR>> dire_) const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (dire_.empty ())
					discard ;
				const auto r1x = Directory (dire_).absolute () ;
				mHeap->mLogFile = String<STR>::make (r1x ,STR ('/') ,slice ("console.log")) ;
				mHeap->mOldLogFile = String<STR>::make (r1x ,STR ('/') ,slice ("console.old.log")) ;
			}
			if ifswitch (rxx) {
				mHeap->mLogFile = String<STR> () ;
				mHeap->mOldLogFile = String<STR> () ;
			}
		}

		void log (CREF<Slice<STR>> tag) const {
			if (mHeap->mLogFile.empty ())
				return ;
			write_log_buffer (tag) ;
			auto rax = RegBuffer<BYTE>::from (unsafe_array (mHeap->mLogBuffer[0]) ,0 ,mHeap->mLogWriter.length () - 1) ;
			try_invoke ([&] () {
				if (mHeap->mLogStreamFile == NULL)
					return ;
				const auto r1x = mHeap->mLogStreamFile->write (rax) ;
				assume (r1x == rax->size ()) ;
			} ,[&] () {
				mHeap->mLogStreamFile = NULL ;
			}) ;
			try_invoke ([&] () {
				if (mHeap->mLogStreamFile != NULL)
					return ;
				attach_log_file () ;
				const auto r2x = mHeap->mLogStreamFile->write (rax) ;
				assume (r2x == rax->size ()) ;
			} ,[&] () {
				mHeap->mLogStreamFile = NULL ;
			}) ;
			if ifswitch (TRUE) {
				if (mHeap->mLogStreamFile == NULL)
					discard ;
				mHeap->mLogStreamFile->flush () ;
			}
		}

		void write_log_buffer (CREF<Slice<STR>> tag) const {
			mHeap->mLogWriter << TextWriter<STRU8>::CLS ;
			mHeap->mLogWriter << slice ("[") ;
			const auto r1x = NowTimePoint () ;
			const auto r2x = r1x.calendar () ;
			mHeap->mLogWriter << AlignedString (r2x.mHour ,2) ;
			mHeap->mLogWriter << slice (":") ;
			mHeap->mLogWriter << AlignedString (r2x.mMinute ,2) ;
			mHeap->mLogWriter << slice (":") ;
			mHeap->mLogWriter << AlignedString (r2x.mSecond ,2) ;
			mHeap->mLogWriter << slice ("][") ;
			mHeap->mLogWriter << tag ;
			mHeap->mLogWriter << slice ("] : ") ;
			const auto r3x = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::id] (mHeap->mConBuffer) ;
			mHeap->mLogWriter << r3x ;
			mHeap->mLogWriter << TextWriter<STRU8>::GAP ;
			mHeap->mLogWriter << TextWriter<STRU8>::EOS ;
		}

		void attach_log_file () const {
			const auto r1x = File (mHeap->mLogFile) ;
			const auto r2x = File (mHeap->mOldLogFile) ;
			r2x.erase () ;
			r2x.move_from (r1x) ;
			mHeap->mLogStreamFile = VRef<StreamFile>::make (mHeap->mLogFile) ;
			const auto r3x = mHeap->mLogStreamFile->link (TRUE ,TRUE) ;
			assume (r3x) ;
			const auto r4x = String<STRU8>::make (TextWriter<STRU8>::BOM) ;
			auto rax = RegBuffer<BYTE>::from (unsafe_array (r4x[0]) ,0 ,r4x.length ()) ;
			const auto r5x = mHeap->mLogStreamFile->write (rax) ;
			assume (r5x == rax->size ()) ;
		}

		void open () const override {
			if (is_open ())
				return ;
			mHeap->mConsole = UniqueRef<HANDLE>::make (stdout) ;
		}

		BOOL is_open () const {
			if ifnot (mHeap->mConsole.exist ())
				return FALSE ;
			if (mHeap->mConsole.self == NULL)
				return FALSE ;
			return TRUE ;
		}

		void close () const override {
			mHeap->mConsole = UniqueRef<HANDLE> () ;
		}

		void pause () const override {
			if ifnot (is_open ())
				return ;
			printf ("press any key to continue...\n") ;
			const auto r1x = getchar () ;
			noop (r1x) ;
		}

		void clear () const override {
			if ifnot (is_open ())
				return ;
			const auto r1x = std::system ("clear") ;
			noop (r1x) ;
		}
	} ;
} ;

template <>
exports auto CONSOLE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename REPORTER_HELP<DEPEND ,ALWAYS>::Holder ;

	using HSYMB = DEF<STRA **> ;
	using STACK_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;
	using FUNCTION_NAME_MAX_SIZE = ENUMAS<VAL ,ENUMID<1024>> ;

	struct HEAP {
		UniqueRef<> mCrashSignal ;
		String<STR> mNameBuffer ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mNameBuffer = String<STR>::make () ;
		}

		void detect_memory_leaks () const override {
			unimplemented () ;
		}

		void detect_crash_signal () const override {
			mHeap->mCrashSignal = UniqueRef<> ([&] () {
				const auto r1x = signal (SIGINT ,crash_handle_sigint) ;
				noop (r1x) ;
				const auto r2x = signal (SIGILL ,crash_handle_sigill) ;
				noop (r2x) ;
				const auto r3x = signal (SIGABRT ,crash_handle_sigabrt) ;
				noop (r3x) ;
				const auto r4x = signal (SIGFPE ,crash_handle_sigfpe) ;
				noop (r4x) ;
				const auto r5x = signal (SIGSEGV ,crash_handle_sigsegv) ;
				noop (r5x) ;
				const auto r6x = signal (SIGTERM ,crash_handle_sigterm) ;
				noop (r6x) ;
				const auto r7x = signal (SIGTERM ,crash_handle_sigbus) ;
				noop (r7x) ;
				const auto r8x = signal (SIGTERM ,crash_handle_sigsys) ;
				noop (r8x) ;
			} ,[] () {
				signal (SIGINT ,SIG_DFL) ;
				signal (SIGILL ,SIG_DFL) ;
				signal (SIGABRT ,SIG_DFL) ;
				signal (SIGFPE ,SIG_DFL) ;
				signal (SIGSEGV ,SIG_DFL) ;
				signal (SIGTERM ,SIG_DFL) ;
				//@info: Historical signals specified by POSIX
				signal (SIGBUS ,SIG_DFL) ;
				signal (SIGSYS ,SIG_DFL) ;
			}) ;
		}

		imports void crash_handle_sigint (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Interrupt Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGINT ,SIG_DFL) ;
			raise (SIGINT) ;
		}

		imports void crash_handle_sigill (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Illegal Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGILL ,SIG_DFL) ;
			raise (SIGILL) ;
		}

		imports void crash_handle_sigabrt (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Abort Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGABRT ,SIG_DFL) ;
			raise (SIGABRT) ;
		}

		imports void crash_handle_sigfpe (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Float Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGFPE ,SIG_DFL) ;
			raise (SIGFPE) ;
		}

		imports void crash_handle_sigsegv (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Segmentation Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGSEGV ,SIG_DFL) ;
			raise (SIGSEGV) ;
		}

		imports void crash_handle_sigterm (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Termination Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGTERM ,SIG_DFL) ;
			raise (SIGTERM) ;
		}

		imports void crash_handle_sigbus (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Bus Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGBUS ,SIG_DFL) ;
			raise (SIGBUS) ;
		}

		imports void crash_handle_sigsys (VAL32) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("System Call Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGSYS ,SIG_DFL) ;
			raise (SIGSYS) ;
		}

		imports void crash_stack_trace () {
			Singleton<Console>::instance ().print () ;
			const auto r1x = Singleton<Reporter>::instance ().captrue_stack_trace () ;
			Singleton<Console>::instance ().fatal (slice ("stack trace")) ;
			for (auto &&i : r1x.iter ()) {
				const auto r2x = Singleton<Reporter>::instance ().symbol_from_address (r1x[i]) ;
				Singleton<Console>::instance ().debug (slice ("#[") ,i ,slice ("] = ") ,r2x) ;
			}
			Singleton<Console>::instance ().print () ;
		}

		Array<FLAG> captrue_stack_trace () const override {
			auto rax = Array<csc_pointer_t> (STACK_MAX_DEPTH::value) ;
			const auto r1x = LENGTH (backtrace ((&rax[0]) ,VAL32 (rax.size ()))) - 3 ;
			Array<FLAG> ret = Array<FLAG> (r1x) ;
			for (auto &&i : ret.iter ())
				ret[i] = LENGTH (rax[3 + i]) ;
			return move (ret) ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr_) const override {
			String<STR> ret ;
			const auto r1x = csc_pointer_t (addr_) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				const auto r2x = UniqueRef<HSYMB> ([&] (VREF<HSYMB> me) {
					me = backtrace_symbols ((&r1x) ,VAL32 (1)) ;
				} ,[] (VREF<HSYMB> me) {
					if (me == NULL)
						return ;
					free (me) ;
				}) ;
				if (r2x.self == NULL)
					discard ;
				auto &&tmp = unsafe_array ((**r2x.self)) ;
				BufferProc::buf_slice (mHeap->mNameBuffer ,tmp ,mHeap->mNameBuffer.size ()) ;
				const auto r3x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::id] (DATA (addr_)) ;
				ret = String<STR>::make (slice ("[") ,r3x ,slice ("] : ") ,mHeap->mNameBuffer) ;
			}
			if ifswitch (rxx) {
				const auto r4x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::id] (DATA (addr_)) ;
				ret = String<STR>::make (slice ("[") ,r4x ,slice ("] : ") ,slice ("null")) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto REPORTER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;