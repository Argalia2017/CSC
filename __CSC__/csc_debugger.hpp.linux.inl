#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <cstdio>

#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using std::printf ;
using std::wprintf ;
using std::getchar ;

using ::atexit ;
using ::signal ;
using ::backtrace ;
using ::free ;
using ::backtrace_symbols ;
} ;

class ConsoleService::Private::Implement
	:public ConsoleService::Abstract {
private:
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	LENGTH mBufferSize ;
	Set<EFLAG> mOptionSet ;
	UniqueRef<VAR32> mConsole ;
	String<STR> mLogPath ;
	AutoRef<StreamLoader> mLogFileStream ;

	BOOL mTempState ;

public:
	implicit Implement () {
		using R1X = U::CONSTEXPR_INCREASE<DEFAULT_HUGESTRING_SIZE> ;
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (R1X::compile ())) ;
		mLogWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (R1X::compile ())) ;
		mBufferSize = mLogWriter.size () - DEFAULT_LONGSTRING_SIZE::compile () ;
		mOptionSet = Set<EFLAG> (128) ;
		mLogPath = String<STR> () ;
	}

	LENGTH buffer_size () const override {
		return mBufferSize ;
	}

	void enable_option (const EFLAG &option) override {
		if (option == OPTION_DEFAULT)
			mOptionSet.clear () ;
		mOptionSet.add (option) ;
	}

	void disable_option (const EFLAG &option) override {
		mOptionSet.erase (option) ;
	}

#ifdef __CSC_CONFIG_STRA__
	void print (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_PRINT) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("%s\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("PRINT") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void print (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_PRINT) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("%ls\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("PRINT") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void fatal (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_FATAL) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;34m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("FATAL") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void fatal (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_FATAL) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;34m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("FATAL") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void error (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_ERROR) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;31m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("ERROR") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void error (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_ERROR) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;31m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("ERROR") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void warn (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_WARN) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;33m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("WARN") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void warn (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_WARN) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;33m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("WARN") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void info (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_INFO) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;32m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("INFO") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void info (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_INFO) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;32m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("INFO") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void debug (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_DEBUG) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;36m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("DEBUG") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void debug (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_DEBUG) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;36m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("DEBUG") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRA__
	void verbose (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_VERBOSE) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::printf (_PCSTR_ ("\033[1;37m%s\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("VERBOSE") ,r1x) ;
	}
#endif

#ifdef __CSC_CONFIG_STRW__
	void verbose (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_VERBOSE) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::wprintf (_PCSTR_ ("\033[1;37m%ls\033[0m\n") ,mConWriter.raw ().self) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("VERBOSE") ,r1x) ;
	}
#endif

	void attach_log (const String<STR> &path) override {
		const auto r1x = FileSystemProc::absolute_path (path) ;
		if switch_once (TRUE) {
			if (mLogPath == r1x)
				discard ;
			if (!mLogFileStream.exist ())
				discard ;
			mLogFileStream->flush () ;
			mLogFileStream = AutoRef<StreamLoader> () ;
		}
		mLogPath = r1x ;
	}

	void log (const Plain<STR> &tag ,const PhanBuffer<const STR> &msg) {
		struct Dependent ;
		using R1X = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplBinder<PhanBuffer<const STR>> ;
		const auto r1x = PhanBuffer<const STR>::make (tag.self ,tag.size ()) ;
		log (r1x ,R1X (msg)) ;
	}

	void log (const PhanBuffer<const STR> &tag ,const Binder &msg) override {
		write_log_buffer (tag ,msg) ;
#ifdef __CSC_DEBUG__
		write_debugger () ;
#endif
		write_log_file () ;
	}

	void show () override {
		if (mConsole.exist ())
			if (mConsole.self > 0)
				return ;
		mConsole = UniqueRef<VAR32>::make (1) ;
	}

	void hide () override {
		mConsole = UniqueRef<VAR32> () ;
	}

	void pause () override {
		if (!mConsole.exist ())
			return ;
		api::printf (_PCSTR_ ("press any key to continue...\n")) ;
		const auto r1x = api::getchar () ;
		_STATIC_UNUSED_ (r1x) ;
	}

	void clear () override {
		if (!mConsole.exist ())
			return ;
		api::printf (_PCSTR_ ("\f\f")) ;
	}

private:
	void write_con_buffer (const Binder &msg) {
		mConWriter << TextWriter<STR>::CLS ;
		mConWriter << msg ;
		mConWriter << TextWriter<STR>::EOS ;
	}

	void attach_console () {
		if (mConsole.exist ())
			return ;
		mConsole = UniqueRef<VAR32>::make (0) ;
	}

	void write_log_buffer (const PhanBuffer<const STR> &tag ,const Binder &msg) {
		mLogWriter << TextWriter<STR>::CLS ;
		mLogWriter << _PCSTR_ ("[") ;
		const auto r1x = GlobalRuntime::clock_now () ;
		mLogWriter << StringProc::build_hours (ARGV<STR>::ID ,r1x) ;
		mLogWriter << _PCSTR_ ("][") ;
		mLogWriter << tag ;
		mLogWriter << _PCSTR_ ("] : ") ;
		mLogWriter << msg ;
		mLogWriter << TextWriter<STR>::GAP ;
		mLogWriter << TextWriter<STR>::EOS ;
	}

	void write_debugger () {
		_STATIC_WARNING_ ("noop") ;
	}

	void write_log_file () {
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mLogWriter.raw ().self ,(mLogWriter.length () - 1)) ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		mTempState = FALSE ;
		_CALL_TRY_ ([&] () {
			if (mTempState)
				return ;
			if (!mLogFileStream.exist ())
				return ;
			mLogFileStream->write (r2x) ;
			mTempState = TRUE ;
		} ,[&] () {
			mLogFileStream = AutoRef<StreamLoader> () ;
			mTempState = FALSE ;
		}) ;
		_CALL_TRY_ ([&] () {
			if (mTempState)
				return ;
			attach_log_file () ;
			mLogFileStream->write (r2x) ;
			mTempState = TRUE ;
		} ,[&] () {
			mLogFileStream = AutoRef<StreamLoader> () ;
			mTempState = FALSE ;
		}) ;
		if (mOptionSet.find (OPTION_ALWAYS_FLUSH) == VAR_NONE)
			return ;
		if (!mLogFileStream.exist ())
			return ;
		mLogFileStream->flush () ;
	}

	void attach_log_file () {
		const auto r1x = mLogPath + _PCSTR_ ("console.log") ;
		const auto r2x = mLogPath + _PCSTR_ ("console.old.log") ;
		FileSystemProc::erase_file (r2x) ;
		FileSystemProc::move_file (r2x ,r1x) ;
		mLogFileStream = AutoRef<StreamLoader>::make (r1x) ;
		const auto r3x = String<STR>::make (TextWriter<STR>::BOM) ;
		mLogFileStream->write (PhanBuffer<const BYTE>::make (r3x.raw ())) ;
	}
} ;

inline exports ConsoleService::ConsoleService (const ARGVF<Singleton<ConsoleService>> &) {
	using R1X = typename Private::Implement ;
	mThis = StrongRef<R1X>::make () ;
}

class DebuggerService::Private::Implement
	:public DebuggerService::Abstract {
public:
	implicit Implement () = default ;

	void abort_once_invoked_exit (const BOOL &flag) override {
		_DEBUG_ASSERT_ (flag) ;
		const auto r1x = Function<void ()> ([] () noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r2x = Function<void (VAR32)> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r3x = Function<void (VAR32)> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r4x = Function<void (VAR32)> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		api::atexit (DEPTR[r1x.self]) ;
		api::signal (SIGFPE ,DEPTR[r2x.self]) ;
		api::signal (SIGILL ,DEPTR[r3x.self]) ;
		api::signal (SIGSEGV ,DEPTR[r4x.self]) ;
	}

	void output_memory_leaks_report (const BOOL &flag) override {
		_DEBUG_ASSERT_ (flag) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	Array<LENGTH> captrue_stack_trace () override {
		auto rax = AutoBuffer<PTR<NONE>> (DEFAULT_RECURSIVE_SIZE::compile ()) ;
		const auto r1x = api::backtrace (rax.self ,VAR32 (rax.size ())) ;
		Array<LENGTH> ret = Array<LENGTH> (r1x) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ()))
			ret[i] = _ADDRESS_ (rax[i]) ;
		return _MOVE_ (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) override {
		_DEBUG_ASSERT_ (list.length () < VAR32_MAX) ;
		const auto r1x = _CALL_ ([&] () {
			Array<PTR<NONE>> ret = Array<PTR<NONE>> (list.length ()) ;
			for (auto &&i : _RANGE_ (0 ,ret.length ())) {
				const auto r2x = _UNSAFE_POINTER_CAST_ (ARGV<NONE>::ID ,list[i]) ;
				ret[i] = r2x ;
			}
			return _MOVE_ (ret) ;
		}) ;
		const auto r3x = UniqueRef<PTR<PTR<STRA>>> ([&] (PTR<PTR<STRA>> &me) {
			me = api::backtrace_symbols (r1x.raw ().self ,VAR32 (r1x.length ())) ;
		} ,[&] (PTR<PTR<STRA>> &me) {
			if (me == NULL)
				return ;
			api::free (me) ;
		}) ;
		Array<String<STR>> ret = Array<String<STR>> (list.size ()) ;
		for (auto &&i : _RANGE_ (0 ,list.length ())) {
			const auto r4x = StringProc::build_hexs (ARGV<STR>::ID ,DATA (list[i])) ;
			const auto r5x = StringProc::parse_strs (String<STRA> (PTRTOARR[PTRTOARR[r3x.self][i]])) ;
			ret[i] = String<STR>::make (_PCSTR_ ("[") ,r4x ,_PCSTR_ ("] : ") ,r5x) ;
		}
		return _MOVE_ (ret) ;
	}
} ;

inline exports DebuggerService::DebuggerService (const ARGVF<Singleton<DebuggerService>> &) {
	using R1X = typename Private::Implement ;
	mThis = StrongRef<R1X>::make () ;
}
} ;