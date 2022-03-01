#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

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
#undef use_comment_lib_dbghelp
#endif

#ifdef __CSC_COMPILER_CLANG__
#ifndef use_comment_lib_dbghelp
#define use_comment_lib_dbghelp "dbghelp.lib"
#endif
#pragma comment (lib ,use_comment_lib_dbghelp)
#undef use_comment_lib_dbghelp
#endif

namespace CSC {
template <class DEPEND>
trait CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Holder ;

	using CONSOLE_BUFFER_DEF_SIZE = ENUMAS<VAL ,ENUMID<8388608>> ;

	using OPTION_DEFAULT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_DEFAULT ;
	using OPTION_NO_PRINT = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_PRINT ;
	using OPTION_NO_FATAL = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_FATAL ;
	using OPTION_NO_ERROR = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_ERROR ;
	using OPTION_NO_WARN = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_WARN ;
	using OPTION_NO_INFO = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_INFO ;
	using OPTION_NO_DEBUG = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_DEBUG ;
	using OPTION_NO_VERBOSE = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION_NO_VERBOSE ;

	struct HEAP {
		VarBuffer<STR> mConBuffer ;
		TextWriter<STR> mConWriter ;
		VarBuffer<STR> mLogBuffer ;
		TextWriter<STR> mLogWriter ;
		Set<FLAG> mOption ;
		UniqueRef<HANDLE> mConsole ;
		String<STR> mLogFile ;
		String<STR> mOldLogFile ;
		VRef<StreamFile> mLogStreamFile ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_device () override {
			set_buffer_size (CONSOLE_BUFFER_DEF_SIZE::value) ;
		}

		void set_buffer_size (CREF<LENGTH> size_) const {
			mHeap->mConBuffer = VarBuffer<STR> (size_) ;
			mHeap->mConWriter = TextWriter<STR> (RegBuffer<STR>::from (mHeap->mConBuffer ,0 ,mHeap->mConBuffer.size ())) ;
			mHeap->mLogBuffer = VarBuffer<STR> (size_) ;
			mHeap->mLogWriter = TextWriter<STR> (RegBuffer<STR>::from (mHeap->mLogBuffer ,0 ,mHeap->mLogBuffer.size ())) ;
		}

		void enable_option (CREF<FLAG> option) const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (option != OPTION_DEFAULT::value)
					discard ;
				mHeap->mOption.clear () ;
			}
			if ifswitch (eax) {
				mHeap->mOption.add (option) ;
			}
		}

		void disable_option (CREF<FLAG> option) const override {
			mHeap->mOption.erase (option) ;
		}

		void print (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_PRINT::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("PRINT")) ;
		}

		void fatal (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_FATAL::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("FATAL")) ;
		}

		void error (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_ERROR::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("ERROR")) ;
		}

		void warn (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_WARN::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("WARN")) ;
		}

		void info (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_INFO::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("INFO")) ;
		}

		void debug (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_DEBUG::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
			log (slice ("DEBUG")) ;
		}

		void verbose (CREF<Binder> msg) const override {
			if (mHeap->mOption.find (OPTION_NO_VERBOSE::value) != NONE)
				return ;
			write_con_buffer (msg) ;
			attach_console () ;
			const auto r1x = VAL32 (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			auto rax = DWORD () ;
			rax = DWORD (mHeap->mConWriter.length () - 1) ;
			WriteConsole (mHeap->mConsole ,(&mHeap->mConBuffer[0]) ,rax ,(&rax) ,NULL) ;
			rax = DWORD (1) ;
			WriteConsole (mHeap->mConsole ,TEXT ("\n") ,rax ,(&rax) ,NULL) ;
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
			mHeap->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			} ,[] (VREF<HANDLE> me) {
				noop () ;
			}) ;
		}

		void link (CREF<String<STR>> dire_) const override {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (dire_.empty ())
					discard ;
				const auto r1x = Directory (dire_).absolute () ;
				mHeap->mLogFile = String<STR>::make (r1x ,slice ("console.log")) ;
				mHeap->mOldLogFile = String<STR>::make (r1x ,slice ("console.old.log")) ;
			}
			if ifswitch (eax) {
				mHeap->mLogFile = String<STR> () ;
				mHeap->mOldLogFile = String<STR> () ;
			}
		}

		void log (CREF<Slice<STR>> tag) const {
			if (mHeap->mLogFile.empty ())
				return ;
			//write_log_buffer (tag) ;
			//const auto r1x = RegBuffer<BYTE_BASE<STR>>::from (mHeap->mLogBuffer ,0 ,mHeap->mLogWriter.length () - 1) ;
			//try_invoke ([&] () {
			//	if (mHeap->mLogStreamFile == NULL)
			//		return ;
			//	mHeap->mLogStreamFile->write (r1x) ;
			//} ,[&] () {
			//	mHeap->mLogStreamFile = NULL ;
			//}) ;
			//try_invoke ([&] () {
			//	if (mHeap->mLogStreamFile != NULL)
			//		return ;
			//	attach_log_file () ;
			//	mHeap->mLogStreamFile->write (r1x) ;
			//} ,[&] () {
			//	mHeap->mLogStreamFile = NULL ;
			//}) ;
			//if ifswitch (TRUE) {
			//	if (mHeap->mLogStreamFile == NULL)
			//		discard ;
			//	mHeap->mLogStreamFile->flush () ;
			//}
		}

		void write_log_buffer (CREF<Slice<STR>> tag) const {
			mHeap->mLogWriter << TextWriter<STR>::CLS ;
			mHeap->mLogWriter << slice ("[") ;
			const auto r1x = TimePoint::make_now () ;
			const auto r2x = r1x.calendar () ;
			write_log_buffer_time (r2x.mHour ,2) ;
			mHeap->mLogWriter << slice (":") ;
			write_log_buffer_time (r2x.mMinute ,2) ;
			mHeap->mLogWriter << slice (":") ;
			write_log_buffer_time (r2x.mSecond ,2) ;
			mHeap->mLogWriter << slice ("][") ;
			mHeap->mLogWriter << tag ;
			mHeap->mLogWriter << slice ("] : ") ;
			const auto r3x = RegBuffer<STR>::from (mHeap->mConBuffer ,0 ,mHeap->mConWriter.length () - 1) ;
			for (auto &&i : iter (0 ,r3x->size ()))
				mHeap->mLogWriter << r3x.self[i] ;
			mHeap->mLogWriter << TextWriter<STR>::GAP ;
			mHeap->mLogWriter << TextWriter<STR>::EOS ;
		}

		void write_log_buffer_time (CREF<LENGTH> count ,CREF<LENGTH> align_) const {
			const auto r1x = align_ - MathProc::log10v (count) ;
			assume (r1x >= 0) ;
			for (auto &&i : iter (0 ,r1x)) {
				noop (i) ;
				mHeap->mLogWriter << slice ("0") ;
			}
			mHeap->mLogWriter << count ;
		}

		void attach_log_file () const {
			const auto r1x = File (mHeap->mLogFile) ;
			const auto r2x = File (mHeap->mOldLogFile) ;
			r2x.erase () ;
			r2x.move_from (r1x) ;
			//mHeap->mLogStreamFile = VRef<StreamFile>::make (mHeap->mLogFile ,FALSE) ;
			//const auto r3x = String<STR>::make (TextWriter<STR>::BOM) ;
			//const auto r4x = RegBuffer<BYTE_BASE<STR>>::from (unsafe_array (r3x[0]) ,0 ,r3x.length ()) ;
			//mHeap->mLogStreamFile->write (r4x) ;
		}

		void open () const override {
			if (mHeap->mConsole.exist ())
				if (mHeap->mConsole.self != NULL)
					return ;
			mHeap->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				AllocConsole () ;
				me = GetStdHandle (STD_OUTPUT_HANDLE) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				FreeConsole () ;
			}) ;
		}

		void close () const override {
			mHeap->mConsole = UniqueRef<HANDLE> () ;
		}

		void pause () const override {
			if (mHeap->mConsole.exist ())
				return ;
			if (mHeap->mConsole.self == NULL)
				return ;
			if ifswitch (TRUE) {
				const auto r1x = GetConsoleWindow () ;
				if (r1x == NULL)
					discard ;
				FlashWindow (r1x ,TRUE) ;
			}
			const auto r2x = VAL32 (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r2x)) ;
			const auto r3x = std::system ("pause") ;
			noop (r3x) ;
		}

		void clear () const override {
			if (mHeap->mConsole.exist ())
				return ;
			if (mHeap->mConsole.self == NULL)
				return ;
			const auto r1x = VAL32 (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (mHeap->mConsole ,csc_byte16_t (r1x)) ;
			const auto r2x = std::system ("cls") ;
			noop (r2x) ;
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

	using STACK_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;
	using FUNCTION_NAME_MAX_SIZE = ENUMAS<VAL ,ENUMID<1024>> ;

	struct REAL_SYMBOL_INFO {
		SYMBOL_INFO mFirst ;
		ARR<STRA ,FUNCTION_NAME_MAX_SIZE> mSecond ;
	} ;

	struct HEAP {
		UniqueRef<> mCrashSignal ;
		UniqueRef<HANDLE> mSymbolFromAddress ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () {
			mHeap = SharedRef<HEAP>::make () ;
		}

		void init_device () override {
			noop () ;
		}

		void detect_memory_leaks () const override {
			const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
			noop (r1x) ;
			const auto r2x = CSC::VAL32 (r1x | _CRTDBG_LEAK_CHECK_DF) ;
			noop (r2x) ;
			const auto r3x = _CrtSetDbgFlag (r2x) ;
			noop (r3x) ;
		}

		void detect_crash_signal () const override {
			mHeap->mCrashSignal = UniqueRef<> ([&] () {
				const auto r1x = signal (SIGINT ,crash_handle_sigint) ;
				assume (r1x == SIG_DFL) ;
				const auto r2x = signal (SIGILL ,crash_handle_sigill) ;
				assume (r2x == SIG_DFL) ;
				const auto r3x = signal (SIGABRT ,crash_handle_sigabrt) ;
				assume (r3x == SIG_DFL) ;
				const auto r4x = signal (SIGFPE ,crash_handle_sigfpe) ;
				assume (r4x == SIG_DFL) ;
				const auto r5x = signal (SIGSEGV ,crash_handle_sigsegv) ;
				assume (r5x == SIG_DFL) ;
				const auto r6x = signal (SIGTERM ,crash_handle_sigterm) ;
				assume (r6x == SIG_DFL) ;
			} ,[] () {
				signal (SIGINT ,SIG_DFL) ;
				signal (SIGILL ,SIG_DFL) ;
				signal (SIGABRT ,SIG_DFL) ;
				signal (SIGFPE ,SIG_DFL) ;
				signal (SIGSEGV ,SIG_DFL) ;
				signal (SIGTERM ,SIG_DFL) ;
			}) ;
		}

		imports void crash_handle_sigint (int) {
			Console::instance ().error (slice ("Interrupt Fault")) ;
			signal (SIGINT ,SIG_DFL) ;
			raise (SIGINT) ;
		}

		imports void crash_handle_sigill (int) {
			Console::instance ().error (slice ("Illegal Fault")) ;
			signal (SIGILL ,SIG_DFL) ;
			raise (SIGILL) ;
		}

		imports void crash_handle_sigabrt (int) {
			Console::instance ().error (slice ("Abort Fault")) ;
			signal (SIGABRT ,SIG_DFL) ;
			raise (SIGABRT) ;
		}

		imports void crash_handle_sigfpe (int) {
			Console::instance ().error (slice ("Float Fault")) ;
			signal (SIGFPE ,SIG_DFL) ;
			raise (SIGFPE) ;
		}

		imports void crash_handle_sigsegv (int) {
			Console::instance ().error (slice ("Segmentation Fault")) ;
			signal (SIGSEGV ,SIG_DFL) ;
			raise (SIGSEGV) ;
		}

		imports void crash_handle_sigterm (int) {
			Console::instance ().error (slice ("Termination Fault")) ;
			signal (SIGTERM ,SIG_DFL) ;
			raise (SIGTERM) ;
		}

		Array<FLAG> captrue_stack_trace () const override {
			auto rax = Array<csc_pointer_t> (STACK_MAX_DEPTH::value) ;
			const auto r1x = LENGTH (CaptureStackBackTrace (3 ,DWORD (rax.size ()) ,(&rax[0]) ,NULL)) ;
			Array<FLAG> ret = Array<FLAG> (r1x) ;
			for (auto &&i : ret.iter ())
				ret[i] = LENGTH (rax[i]) ;
			return move (ret) ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr_) const override {
			String<STR> ret ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				attach_symbol () ;
				if ifnot (mHeap->mSymbolFromAddress.exist ())
					discard ;
				auto rax = Box<REAL_SYMBOL_INFO>::make () ;
				rax->mFirst.SizeOfStruct = DWORD (SIZE_OF<SYMBOL_INFO>::value) ;
				rax->mFirst.MaxNameLen = DWORD (FUNCTION_NAME_MAX_SIZE::value) ;
				SymFromAddr (mHeap->mSymbolFromAddress ,DWORD64 (addr_) ,NULL ,(&rax->mFirst)) ;
				const auto r1x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::id] (DATA (addr_)) ;
				const auto r2x = invoke ([&] () {
					String<STR> ret = String<STR>::make () ;
					INDEX ix = 0 ;
					while (TRUE) {
						if (rax->mFirst.Name[ix] == 0)
							break ;
						ret[ix] = STR (rax->mFirst.Name[ix]) ;
						ix++ ;
					}
					return move (ret) ;
				}) ;
				ret = String<STR>::make (slice ("[") ,r1x ,slice ("] : ") ,r2x) ;
			}
			if ifswitch (eax) {
				const auto r3x = string_build[TYPEAS<TYPEAS<STR ,DATA>>::id] (DATA (addr_)) ;
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
					const auto r1x = SymInitialize (me ,NULL ,TRUE) ;
					if (r1x)
						return ;
					me = NULL ;
				} ,[] (VREF<HANDLE> me) {
					if (me == NULL)
						return ;
					SymCleanup (me) ;
				}) ;
				if (mHeap->mSymbolFromAddress.self != NULL)
					discard ;
				mHeap->mSymbolFromAddress = UniqueRef<HANDLE> () ;
			}
		}
	} ;
} ;

template <>
exports auto REPORTER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;