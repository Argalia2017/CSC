#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#include "csc_debugger.hpp"

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "begin.h"
#include <iostream>
#include <crtdbg.h>
#include <signal.h>
#include <DbgHelp.h>
#include "end.h"

#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_dbghelp
#define use_comment_lib_dbghelp "dbghelp.lib"
#endif
#pragma comment (lib ,use_comment_lib_dbghelp)
#endif

#ifdef __CSC_COMPILER_GNUC__
#ifndef use_comment_lib_dbghelp
#define maybe_SymFromAddr(...) CSC::FALSE
#define maybe_SymInitialize(...) CSC::FALSE
#define maybe_SymCleanup(...)
#endif
#endif

#ifdef __CSC_COMPILER_CLANG__
#ifndef use_comment_lib_dbghelp
#define use_comment_lib_dbghelp "dbghelp.lib"
#endif
#pragma comment (lib ,use_comment_lib_dbghelp)
#endif

#ifndef maybe_SymFromAddr
#define maybe_SymFromAddr(...) SymFromAddr (__VA_ARGS__)
#endif

#ifndef maybe_SymInitialize
#define maybe_SymInitialize(...) SymInitialize (__VA_ARGS__)
#endif

#ifndef maybe_SymCleanup
#define maybe_SymCleanup(...) SymCleanup (__VA_ARGS__)
#endif

namespace CSC {
template <class DEPEND>
trait CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Holder ;

	using CONSOLE_BUFFER_SSIZE = ENUMAS<VAL ,ENUMID<8388607>> ;

	using OPTION_DEFAULT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_DEFAULT ;
	using OPTION_NO_PRINT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_PRINT ;
	using OPTION_NO_FATAL = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_FATAL ;
	using OPTION_NO_ERROR = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_ERROR ;
	using OPTION_NO_WARN = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_WARN ;
	using OPTION_NO_INFO = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_INFO ;
	using OPTION_NO_DEBUG = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_DEBUG ;
	using OPTION_NO_VERBOSE = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_VERBOSE ;
	using OPTION_SIZE = ENUMAS<VAL ,ENUMID<32>> ;

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
			set_buffer_size (CONSOLE_BUFFER_SSIZE::expr) ;
		}

		void set_buffer_size (CREF<LENGTH> size_) const {
			mHeap->mConBuffer = String<STR> (size_) ;
			mHeap->mConWriter = TextWriter<STR> (mHeap->mConBuffer.raw ().ref ()) ;
			mHeap->mLogBuffer = String<STRU8> (size_) ;
			mHeap->mLogWriter = TextWriter<STRU8> (mHeap->mLogBuffer.raw ().ref ()) ;
		}

		void enable_option (CREF<FLAG> option) const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (option != OPTION_DEFAULT::expr)
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
			if (mHeap->mOption[OPTION_NO_PRINT::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("PRINT")) ;
		}

		void fatal (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_FATAL::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("FATAL")) ;
		}

		void error (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_ERROR::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("ERROR")) ;
		}

		void warn (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_WARN::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("WARN")) ;
		}

		void info (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_INFO::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("INFO")) ;
		}

		void debug (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_DEBUG::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("DEBUG")) ;
		}

		void verbose (CREF<Binder> msg) const override {
			if (mHeap->mOption[OPTION_NO_VERBOSE::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (mHeap->mConsole.exist ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
				auto rax = DWORD () ;
				rax = DWORD (mHeap->mConWriter.length () - 1) ;
				WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = DWORD (1) ;
				WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("VERBOSE")) ;
		}

		void write_con_buffer (CREF<Binder> msg) const {
			mHeap->mConWriter << TextWriter<STR>::CLS ;
			mHeap->mConWriter << msg ;
			mHeap->mConWriter << TextWriter<STR>::EOS ;
		}

		void link (CREF<String<STR>> dire) const override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (dire.empty ())
					discard ;
				const auto r1x = Directory (dire).absolute () ;
				mHeap->mLogFile = String<STR>::make (r1x ,STR ('\\') ,slice ("console.log")) ;
				mHeap->mOldLogFile = String<STR>::make (r1x ,STR ('\\') ,slice ("console.old.log")) ;
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
			const auto r1x = mHeap->mLogWriter.length () - 1 ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (mHeap->mLogBuffer[0])) ;
			try_invoke ([&] () {
				if (mHeap->mLogStreamFile == NULL)
					return ;
				const auto r2x = mHeap->mLogStreamFile->write (RegBuffer<BYTE>::make (tmp ,0 ,r1x)) ;
				assume (r2x == r1x) ;
			} ,[&] () {
				mHeap->mLogStreamFile = NULL ;
			}) ;
			try_invoke ([&] () {
				if (mHeap->mLogStreamFile != NULL)
					return ;
				attach_log_file () ;
				const auto r3x = mHeap->mLogStreamFile->write (RegBuffer<BYTE>::make (tmp ,0 ,r1x)) ;
				assume (r3x == r1x) ;
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
			const auto r3x = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::expr] (mHeap->mConBuffer) ;
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
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (r4x[0])) ;
			const auto r5x = mHeap->mLogStreamFile->write (RegBuffer<BYTE>::make (tmp ,0 ,r4x.length ())) ;
			assume (r5x == r4x.length ()) ;
		}

		void open () const override {
			if (is_open ())
				return ;
			mHeap->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				AllocConsole () ;
				me = GetStdHandle (STD_OUTPUT_HANDLE) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				FreeConsole () ;
			}) ;
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
			if ifswitch (TRUE) {
				const auto r1x = GetConsoleWindow () ;
				if (r1x == NULL)
					discard ;
				FlashWindow (r1x ,TRUE) ;
			}
			const auto r2x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mHeap->mConsole ,r2x) ;
			const auto r3x = std::system ("pause") ;
			noop (r3x) ;
		}

		void clear () const override {
			if ifnot (is_open ())
				return ;
			const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mHeap->mConsole ,r1x) ;
			const auto r2x = std::system ("cls") ;
			noop (r2x) ;
		}
	} ;
} ;

template <>
exports auto CONSOLE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename REPORTER_HELP<DEPEND ,ALWAYS>::Holder ;

	using STACK_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;
	using FUNCTION_NAME_SSIZE = ENUMAS<VAL ,ENUMID<1023>> ;

	struct REAL_SYMBOL_INFO {
		SYMBOL_INFO mFirst ;
		ARR<STRA ,FUNCTION_NAME_SSIZE> mSecond ;
	} ;

	struct HEAP {
		UniqueRef<> mCrashSignal ;
		UniqueRef<HANDLE> mSymbolFromAddress ;
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
			const auto r1x = csc_byte32_t (_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG)) ;
			noop (r1x) ;
			const auto r2x = csc_byte32_t (r1x | _CRTDBG_LEAK_CHECK_DF) ;
			noop (r2x) ;
			const auto r3x = csc_byte32_t (_CrtSetDbgFlag (r2x)) ;
			noop (r3x) ;
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
			} ,[] () {
				signal (SIGINT ,SIG_DFL) ;
				signal (SIGILL ,SIG_DFL) ;
				signal (SIGABRT ,SIG_DFL) ;
				signal (SIGFPE ,SIG_DFL) ;
				signal (SIGSEGV ,SIG_DFL) ;
				signal (SIGTERM ,SIG_DFL) ;
			}) ;
		}

		imports void crash_handle_sigint (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Interrupt Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGINT ,SIG_DFL) ;
			raise (SIGINT) ;
		}

		imports void crash_handle_sigill (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Illegal Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGILL ,SIG_DFL) ;
			raise (SIGILL) ;
		}

		imports void crash_handle_sigabrt (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Abort Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGABRT ,SIG_DFL) ;
			raise (SIGABRT) ;
		}

		imports void crash_handle_sigfpe (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Float Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGFPE ,SIG_DFL) ;
			raise (SIGFPE) ;
		}

		imports void crash_handle_sigsegv (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Segmentation Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGSEGV ,SIG_DFL) ;
			raise (SIGSEGV) ;
		}

		imports void crash_handle_sigterm (csc_int32_t) {
			Singleton<Console>::instance ().print () ;
			Singleton<Console>::instance ().error (slice ("Termination Fault")) ;
			Singleton<Console>::instance ().error (slice ("errno = ") ,FLAG (errno)) ;
			crash_stack_trace () ;
			signal (SIGTERM ,SIG_DFL) ;
			raise (SIGTERM) ;
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
			auto rax = Array<csc_pointer_t> (STACK_MAX_DEPTH::expr) ;
			const auto r1x = LENGTH (CaptureStackBackTrace (3 ,DWORD (rax.size ()) ,(&rax[0]) ,NULL)) ;
			Array<FLAG> ret = Array<FLAG> (r1x) ;
			for (auto &&i : ret.iter ())
				ret[i] = LENGTH (rax[i]) ;
			return move (ret) ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr) const override {
			String<STR> ret ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				attach_symbol () ;
				if ifnot (mHeap->mSymbolFromAddress.exist ())
					discard ;
				auto rax = Box<REAL_SYMBOL_INFO>::make () ;
				rax->mFirst.SizeOfStruct = DWORD (SIZE_OF<SYMBOL_INFO>::expr) ;
				rax->mFirst.MaxNameLen = DWORD (FUNCTION_NAME_SSIZE::expr) ;
				const auto r1x = maybe_SymFromAddr (mHeap->mSymbolFromAddress ,DWORD64 (addr) ,NULL ,(&rax->mFirst)) ;
				if ifnot (r1x)
					discard ;
				const auto r2x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::expr] (DATA (addr)) ;
				BufferProc::buf_slice (mHeap->mNameBuffer ,rax->mFirst.Name ,mHeap->mNameBuffer.size ()) ;
				ret = String<STR>::make (slice ("[") ,r2x ,slice ("] : ") ,mHeap->mNameBuffer) ;
			}
			if ifswitch (rxx) {
				const auto r3x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::expr] (DATA (addr)) ;
				ret = String<STR>::make (slice ("[") ,r3x ,slice ("] : ") ,slice ("null")) ;
			}
			return move (ret) ;
		}

		void attach_symbol () const {
			if (mHeap->mSymbolFromAddress.exist ())
				return ;
			if ifswitch (TRUE) {
				mHeap->mSymbolFromAddress = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
					me = GetCurrentProcess () ;
					const auto r1x = maybe_SymInitialize (me ,NULL ,TRUE) ;
					if (r1x)
						return ;
					me = NULL ;
				} ,[] (VREF<HANDLE> me) {
					if (me == NULL)
						return ;
					maybe_SymCleanup (me) ;
				}) ;
				if (mHeap->mSymbolFromAddress.self != NULL)
					discard ;
				mHeap->mSymbolFromAddress = UniqueRef<HANDLE> () ;
			}
		}
	} ;
} ;

template <>
exports auto REPORTER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;