﻿#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#include "csc_debugger.hpp"

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_end.h"
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include "csc_begin.h"

#define PRINT_SP(type ,color) "\033[" type ";" color "m%s\033[0m\n"
#define PRINT_BLACK PRINT_SP ("1" ,"30")
#define PRINT_RED PRINT_SP ("1" ,"31")
#define PRINT_GREEN PRINT_SP ("1" ,"32")
#define PRINT_YELLOW PRINT_SP ("1" ,"33")
#define PRINT_BLUE PRINT_SP ("1" ,"34")
#define PRINT_PURPLE PRINT_SP ("1" ,"34")
#define PRINT_CYAN PRINT_SP ("1" ,"36")
#define PRINT_WHITE PRINT_SP ("1" ,"37")

namespace CSC {
template <class DEPEND>
trait CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Holder ;
	using OPTION = typename CONSOLE_HELP<DEPEND ,ALWAYS>::OPTION ;

	using HANDLE = csc_pointer_t ;
	using CONSOLE_BUFFER_SSIZE = ENUMAS<VAL ,8388607> ;

	struct PACK {
		String<STR> mConBuffer ;
		TextWriter<STR> mConWriter ;
		String<STRU8> mLogBuffer ;
		TextWriter<STRU8> mLogWriter ;
		BitSet<> mOption ;
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
			mThis->mOption = BitSet<> (LENGTH (OPTION_CLAZZ::EnumSize)) ;
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
				if (option != OPTION_CLAZZ::Default)
					discard ;
				mThis->mOption.clear () ;
			}
			if ifswitch (act) {
				mThis->mOption.add (option) ;
			}
		}

		void print (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoPrint)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf ("%s\n" ,(&r1x[0])) ;
			}
			log (slice ("PRINT")) ;
		}

		void fatal (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoFatal)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_BLUE ,(&r1x[0])) ;
			}
			log (slice ("FATAL")) ;
		}

		void error (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoError)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_RED ,(&r1x[0])) ;
			}
			log (slice ("ERROR")) ;
		}

		void warn (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoWarn)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_YELLOW ,(&r1x[0])) ;
			}
			log (slice ("WARN")) ;
		}

		void info (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoInfo)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_GREEN ,(&r1x[0])) ;
			}
			log (slice ("INFO")) ;
		}

		void debug (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoDebug)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_CYAN ,(&r1x[0])) ;
			}
			log (slice ("DEBUG")) ;
		}

		void verbose (CREF<Binder> msg) const override {
			if (mThis->mOption[FLAG (OPTION_CLAZZ::NoVerbose)])
				return ;
			write_con_buffer (msg) ;
			if ifswitch (TRUE) {
				if ifnot (is_show ())
					discard ;
				const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mConBuffer) ;
				printf (PRINT_WHITE ,(&r1x[0])) ;
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
				mThis->mLogFile = PrintString<STR>::make (r1x ,STR ('/') ,slice ("console.log")) ;
				mThis->mOldLogFile = PrintString<STR>::make (r1x ,STR ('/') ,slice ("console.old.log")) ;
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
			try_invoke ([&] () {
				if (mThis->mLogStreamFile == NULL)
					return ;
				const auto r3x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (unsafe_deptr (mThis->mLogBuffer[0]) ,0 ,r1x)) ;
				assume (r3x == r1x) ;
			} ,[&] () {
				mThis->mLogStreamFile = NULL ;
			}) ;
			try_invoke ([&] () {
				if (mThis->mLogStreamFile != NULL)
					return ;
				open_log_file () ;
				const auto r4x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (unsafe_deptr (mThis->mLogBuffer[0]) ,0 ,r1x)) ;
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
			const auto r5x = mThis->mLogStreamFile->write (RegBuffer<BYTE>::from (unsafe_deptr (r3x[0]) ,0 ,r3x.length ())) ;
			assume (r5x == r3x.length ()) ;
		}

		void show () const override {
			if (is_show ())
				return ;
			mThis->mConsole = UniqueRef<HANDLE>::make (stdout) ;
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
			printf ("press any key to continue...\n") ;
			const auto r1x = getchar () ;
			noop (r1x) ;
		}

		void clear () const override {
			if ifnot (is_show ())
				return ;
			const auto r1x = std::system ("clear") ;
			noop (r1x) ;
		}
	} ;
} ;

template <>
exports auto CONSOLE_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename CONSOLE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename REPORTER_HELP<DEPEND ,ALWAYS>::Holder ;

	using HSYMB = DEF<STRA **> ;
	using STACK_MAX_DEPTH = ENUMAS<VAL ,256> ;
	using FUNCTION_NAME_MAX_SIZE = ENUMAS<VAL ,1024> ;

	struct PACK {
		UniqueRef<> mCrashSignal ;
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
			unimplemented () ;
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
			auto rax = Array<csc_pointer_t> (STACK_MAX_DEPTH::expr) ;
			const auto r1x = LENGTH (backtrace ((&rax[0]) ,VAL32 (rax.size ()))) - 3 ;
			Array<FLAG> ret = Array<FLAG> (r1x) ;
			for (auto &&i : ret.iter ())
				ret[i] = LENGTH (rax[3 + i]) ;
			return move (ret) ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr) const override {
			String<STR> ret ;
			auto act = TRUE ;
			if ifswitch (act) {
				const auto r1x = csc_pointer_t (addr) ;
				const auto r2x = UniqueRef<HSYMB> ([&] (VREF<HSYMB> me) {
					me = backtrace_symbols ((&r1x) ,VAL32 (1)) ;
				} ,[] (VREF<HSYMB> me) {
					if (me == NULL)
						return ;
					free (me) ;
				}) ;
				if (r2x.self == NULL)
					discard ;
				const auto r3x = address ((**r2x.self)) ;
				auto &&tmp = unsafe_cast[TYPEAS<ARR<STRA>>::expr] (unsafe_deref (r3x)) ;
				mThis->mNameBuffer -= BufferProc<STR>::buf_slice (tmp ,mThis->mNameBuffer.size ()) ;
				const auto r4x = string_build[TYPEAS<STR ,DATA>::expr] (DATA (addr)) ;
				ret = PrintString<STR>::make (slice ("[") ,r4x ,slice ("] : ") ,mThis->mNameBuffer) ;
			}
			if ifswitch (act) {
				const auto r5x = string_build[TYPEAS<STR ,DATA>::expr] (DATA (addr)) ;
				ret = PrintString<STR>::make (slice ("[") ,r5x ,slice ("] : ") ,slice ("null")) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto REPORTER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename REPORTER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;