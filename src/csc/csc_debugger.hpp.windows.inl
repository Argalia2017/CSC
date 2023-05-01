#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#include "csc_debugger.hpp"

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_end.h"
#include <iostream>
#include <crtdbg.h>
#include <signal.h>
#include <DbgHelp.h>
#include "csc_begin.h"

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

	using CONSOLE_BUFFER_SSIZE = ENUMAS<VAL ,8388607> ;

	using OPTION_DEFAULT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_DEFAULT ;
	using OPTION_NO_PRINT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_PRINT ;
	using OPTION_NO_FATAL = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_FATAL ;
	using OPTION_NO_ERROR = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_ERROR ;
	using OPTION_NO_WARN = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_WARN ;
	using OPTION_NO_INFO = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_INFO ;
	using OPTION_NO_DEBUG = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_DEBUG ;
	using OPTION_NO_VERBOSE = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_VERBOSE ;
	using OPTION_SIZE = ENUMAS<VAL ,32> ;

	struct PACK {
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
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			set_buffer_size (CONSOLE_BUFFER_SSIZE::expr) ;
		}

		void set_buffer_size (CREF<LENGTH> size_) const {
			mThis->mConBuffer = String<STR> (size_) ;
			mThis->mConWriter = TextWriter<STR> (mThis->mConBuffer.raw ().borrow ()) ;
			mThis->mLogBuffer = String<STRU8> (size_) ;
			mThis->mLogWriter = TextWriter<STRU8> (mThis->mLogBuffer.raw ().borrow ()) ;
		}

		void enable_option (CREF<FLAG> option) const override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (option != OPTION_DEFAULT::expr)
					discard ;
				mThis->mOption.clear () ;
			}
			if ifswitch (act) {
				mThis->mOption.add (option) ;
			}
		}

		void print (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_PRINT::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("PRINT")) ;
		}

		void fatal (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_FATAL::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("FATAL")) ;
		}

		void error (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_ERROR::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("ERROR")) ;
		}

		void warn (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_WARN::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("WARN")) ;
		}

		void info (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_INFO::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("INFO")) ;
		}

		void debug (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_DEBUG::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("DEBUG")) ;
		}

		void verbose (CREF<Binder> msg) const override {
			if (mThis->mOption[OPTION_NO_VERBOSE::expr])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
				SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
				auto rax = csc_enum_t () ;
				rax = csc_enum_t (mThis->mConWriter.length () - 1) ;
				WriteConsole (mThis->mConsole ,(&mThis->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
				rax = csc_enum_t (1) ;
				WriteConsole (mThis->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			}
			log (slice ("VERBOSE")) ;
		}

		void write_con_buffer (CREF<Binder> msg) const {
			mThis->mConWriter << CLS ;
			mThis->mConWriter << msg ;
			mThis->mConWriter << EOS ;
		}

		void open (CREF<String<STR>> dire) const override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (dire.empty ())
					discard ;
				const auto r1x = Directory (dire).absolute () ;
				mThis->mLogFile = PrintString<STR>::make (r1x ,STR ('\\') ,slice ("console.log")) ;
				mThis->mOldLogFile = PrintString<STR>::make (r1x ,STR ('\\') ,slice ("console.old.log")) ;
			}
			if ifswitch (act) {
				mThis->mLogFile = String<STR> () ;
				mThis->mOldLogFile = String<STR> () ;
			}
		}

		void log (CREF<Slice<STR>> tag) const {
			if (mThis->mLogFile.empty ())
				return ;
			write_log_buffer (tag) ;
			const auto r1x = mThis->mLogWriter.length () - 1 ;
			const auto r2x = address (mThis->mLogBuffer[0]) ;
			try_invoke ([&] () {
				if (mThis->mLogStreamFile == NULL)
					return ;
				const auto r3x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (r2x ,0 ,r1x)) ;
				assume (r3x == r1x) ;
			} ,[&] () {
				mThis->mLogStreamFile = NULL ;
			}) ;
			try_invoke ([&] () {
				if (mThis->mLogStreamFile != NULL)
					return ;
				open_log_file () ;
				const auto r4x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (r2x ,0 ,r1x)) ;
				assume (r4x == r1x) ;
			} ,[&] () {
				mThis->mLogStreamFile = NULL ;
			}) ;
			if ifswitch (TRUE) {
				if (mThis->mLogStreamFile == NULL)
					discard ;
				mThis->mLogStreamFile->flush () ;
			}
		}

		void write_log_buffer (CREF<Slice<STR>> tag) const {
			mThis->mLogWriter << CLS ;
			mThis->mLogWriter << slice ("[") ;
			const auto r1x = NowTimePoint::make () ;
			const auto r2x = r1x.calendar () ;
			mThis->mLogWriter << ValueString (r2x.mHour ,2) ;
			mThis->mLogWriter << slice (":") ;
			mThis->mLogWriter << ValueString (r2x.mMinute ,2) ;
			mThis->mLogWriter << slice (":") ;
			mThis->mLogWriter << ValueString (r2x.mSecond ,2) ;
			mThis->mLogWriter << slice ("][") ;
			mThis->mLogWriter << tag ;
			mThis->mLogWriter << slice ("] : ") ;
			const auto r3x = string_cvt[TYPEAS<STRU8 ,STR>::expr] (mThis->mConBuffer) ;
			mThis->mLogWriter << r3x ;
			mThis->mLogWriter << GAP ;
			mThis->mLogWriter << EOS ;
		}

		void open_log_file () const {
			const auto r1x = File (mThis->mLogFile) ;
			const auto r2x = File (mThis->mOldLogFile) ;
			r2x.erase () ;
			r2x.move_from (r1x) ;
			mThis->mLogStreamFile = VRef<StreamFile>::make (mThis->mLogFile) ;
			mThis->mLogStreamFile->open (TRUE ,TRUE) ;
			const auto r3x = PrintString<STRU8>::make (BOM) ;
			const auto r4x = address (r3x[0]) ;
			const auto r5x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (r4x ,0 ,r3x.length ())) ;
			assume (r5x == r3x.length ()) ;
		}

		void show () const override {
			if (is_show ())
				return ;
			mThis->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = AttachConsole (ATTACH_PARENT_PROCESS) ;
				if ifnot (r1x)
					return ;
				me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			} ,[] (VREF<HANDLE> me) {
				noop () ;
			}) ;
			if (is_show ())
				return ;
			mThis->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				AllocConsole () ;
				me = GetStdHandle (STD_OUTPUT_HANDLE) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				FreeConsole () ;
			}) ;
		}

		BOOL is_show () const {
			if ifnot (mThis->mConsole.exist ())
				return FALSE ;
			if (mThis->mConsole.self == NULL)
				return FALSE ;
			return TRUE ;
		}

		void hide () const override {
			mThis->mConsole = UniqueRef<HANDLE> () ;
		}

		void pause () const override {
			if ifnot (is_show ())
				return ;
			if ifswitch (TRUE) {
				const auto r1x = GetConsoleWindow () ;
				if (r1x == NULL)
					discard ;
				FlashWindow (r1x ,TRUE) ;
			}
			const auto r2x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mThis->mConsole ,r2x) ;
			const auto r3x = std::system ("pause") ;
			noop (r3x) ;
		}

		void clear () const override {
			if ifnot (is_show ())
				return ;
			const auto r1x = csc_byte16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mThis->mConsole ,r1x) ;
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

	using STACK_MAX_DEPTH = ENUMAS<VAL ,256> ;
	using FUNCTION_NAME_SIZE = ENUMAS<VAL ,1024> ;

	struct SYMBOL_INFO_EX {
		SYMBOL_INFO mFirst ;
		Storage<FUNCTION_NAME_SIZE> mSecond ;
	} ;

	struct PACK {
		UniqueRef<> mCrashSignal ;
		UniqueRef<HANDLE> mSymbolFromAddress ;
		String<STR> mNameBuffer ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mNameBuffer = PrintString<STR>::make () ;
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
			mThis->mCrashSignal = UniqueRef<> ([&] () {
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
			const auto r1x = LENGTH (CaptureStackBackTrace (3 ,VAL32 (rax.size ()) ,(&rax[0]) ,NULL)) ;
			Array<FLAG> ret = Array<FLAG> (r1x) ;
			for (auto &&i : ret.iter ())
				ret[i] = LENGTH (rax[i]) ;
			return move (ret) ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr) const override {
			String<STR> ret ;
			auto act = TRUE ;
			if ifswitch (act) {
				attach_symbol () ;
				if ifnot (mThis->mSymbolFromAddress.exist ())
					discard ;
				auto rax = Box<SYMBOL_INFO_EX>::make () ;
				rax->mFirst.SizeOfStruct = csc_enum_t (SIZE_OF<SYMBOL_INFO>::expr) ;
				rax->mFirst.MaxNameLen = csc_enum_t (FUNCTION_NAME_SIZE::expr) ;
				const auto r1x = maybe_SymFromAddr (mThis->mSymbolFromAddress ,DWORD64 (addr) ,NULL ,(&rax->mFirst)) ;
				if ifnot (r1x)
					discard ;
				mThis->mNameBuffer -= BufferProc<STR>::buf_slice (unsafe_array (rax->mFirst.Name[0]) ,mThis->mNameBuffer.size ()) ;
				const auto r2x = string_build[TYPEAS<STR ,DATA>::expr] (DATA (addr)) ;
				ret = PrintString<STR>::make (slice ("[") ,r2x ,slice ("] : ") ,mThis->mNameBuffer) ;
			}
			if ifswitch (act) {
				const auto r3x = string_build[TYPEAS<STR ,DATA>::expr] (DATA (addr)) ;
				ret = PrintString<STR>::make (slice ("[") ,r3x ,slice ("] : ") ,slice ("null")) ;
			}
			return move (ret) ;
		}

		void attach_symbol () const {
			if (mThis->mSymbolFromAddress.exist ())
				return ;
			if ifswitch (TRUE) {
				mThis->mSymbolFromAddress = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
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
				if (mThis->mSymbolFromAddress.self != NULL)
					discard ;
				mThis->mSymbolFromAddress = UniqueRef<HANDLE> () ;
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