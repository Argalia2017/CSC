#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <cstdio>
#include <cstdlib>
#include <signal.h>

#include <unistd.h>
#include <execinfo.h>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("discard")
#endif

namespace CSC {
class ConsoleService::Implement :public ConsoleService::Abstract {
private:
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	LENGTH mBufferSize ;
	FLAG mOptionFlag ;
	String<STR> mLogPath ;
	AutoRef<StreamLoader> mLogFileStream ;

	BOOL mTempState ;

public:
	Implement () {
		using DEFAULT_HUGESTRING_SIZE = ARGC<8388607> ;
		using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
		const auto r1x = DEFAULT_HUGESTRING_SIZE::value + 1 ;
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		mLogWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		mBufferSize = mLogWriter.size () - DEFAULT_LONGSTRING_SIZE::value ;
		modify_option (OPTION_DEFAULT) ;
		mLogPath = String<STR> () ;
	}

	LENGTH buffer_size () const override {
		return mBufferSize ;
	}

	void modify_option (FLAG option) override {
		if (option == OPTION_DEFAULT)
			mOptionFlag = OPTION_DEFAULT ;
		mOptionFlag |= option ;
	}

	void print (const Binder &msg) override {
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("%s\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("%ls\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("PRINT") ,r1x) ;
	}

	void fatal (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_FATAL) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;34m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;34m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("FATAL") ,r1x) ;
	}

	void error (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_ERROR) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;31m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;31m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("ERROR") ,r1x) ;
	}

	void warn (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_WARN) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;33m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;33m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("WARN") ,r1x) ;
	}

	void info (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_INFO) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;32m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;32m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("INFO") ,r1x) ;
	}

	void debug (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_DEBUG) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;36m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;36m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("DEBUG") ,r1x) ;
	}

	void verbose (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_VERBOSE) != 0)
			return ;
		write_con_buffer (msg) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;37m%s\033[0m\n") ,mConWriter.raw ().self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;37m%ls\033[0m\n") ,mConWriter.raw ().self) ;
#endif
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw () ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("VERBOSE") ,r1x) ;
	}

	void attach_log (const String<STR> &path) override {
		const auto r1x = _ABSOLUTEPATH_ (path) ;
		for (FOR_ONCE_DO) {
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
		log (PhanBuffer<const STR>::make (tag.self ,tag.size ()) ,ImplBinder<PhanBuffer<const STR>> (msg)) ;
	}

	void log (const PhanBuffer<const STR> &tag ,const Binder &msg) override {
		write_log_buffer (tag ,msg) ;
#ifdef __CSC_DEBUG__
		write_debugger () ;
#endif
		write_log_file () ;
	}

	void show () override {
		_STATIC_WARNING_ ("noop") ;
	}

	void hide () override {
		_STATIC_WARNING_ ("noop") ;
	}

	void flash () override {
		_STATIC_WARNING_ ("noop") ;
	}

	void pause () override {
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("press any key to continue...\n")) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("press any key to continue...\n")) ;
#endif
	}

	void clear () override {
		const auto r1x = std::system (_PCSTRA_ ("clear")) ;
		(void) r1x ;
	}

private:
	void write_con_buffer (const Binder &msg) {
		mConWriter << _CLS_ ;
		mConWriter << msg ;
		mConWriter << _EOS_ ;
	}

	void write_log_buffer (const PhanBuffer<const STR> &tag ,const Binder &msg) {
		mLogWriter << _CLS_ ;
		mLogWriter << _PCSTR_ ("[") ;
		mLogWriter << _BUILDHOURS_ (std::chrono::system_clock ().now ()) ;
		mLogWriter << _PCSTR_ ("][") ;
		mLogWriter << tag ;
		mLogWriter << _PCSTR_ ("] : ") ;
		mLogWriter << msg ;
		mLogWriter << _GAP_ ;
		mLogWriter << _EOS_ ;
	}

	void write_debugger () {
		_STATIC_WARNING_ ("noop") ;
	}

	void write_log_file () {
		if (mLogPath.empty ())
			return ;
		if (!mLogFileStream.exist ())
			attach_log_file () ;
		const auto r1x = _MAX_ ((mLogWriter.length () - 1) ,VAR_ZERO) * _SIZEOF_ (STR) ;
		const auto r2x = PhanBuffer<const BYTE>::make (_LOAD_<ARR<BYTE>> (&mLogWriter.raw ().self) ,r1x) ;
		mTempState = FALSE ;
		_CALL_TRY_ ([&] () {
			if (mTempState)
				return ;
			mLogFileStream->write (r2x) ;
			mTempState = TRUE ;
		} ,[&] () {
			mTempState = FALSE ;
		}) ;
		_CALL_TRY_ ([&] () {
			if (mTempState)
				return ;
			attach_log_file () ;
			mLogFileStream->write (r2x) ;
			mTempState = TRUE ;
		} ,[&] () {
			mTempState = FALSE ;
		}) ;
		if ((mOptionFlag & OPTION_ALWAYS_FLUSH) == 0)
			return ;
		mLogFileStream->flush () ;
	}

	void attach_log_file () {
		mLogFileStream = AutoRef<StreamLoader> () ;
		const auto r1x = mLogPath + _PCSTR_ ("console.log") ;
		const auto r2x = mLogPath + _PCSTR_ ("console.old.log") ;
		_ERASEFILE_ (r2x) ;
		_MOVEFILE_ (r2x ,r1x) ;
		mLogFileStream = AutoRef<StreamLoader>::make (r1x) ;
		const auto r3x = String<STR>::make (_XVALUE_<PTR<void (TextWriter<STR> &)>> (_BOM_)) ;
		mLogFileStream->write (PhanBuffer<const BYTE>::make (r3x.raw ())) ;
	}
} ;

inline exports ConsoleService::ConsoleService () {
	mThis = StrongRef<Implement>::make () ;
}

class DebuggerService::Implement :public DebuggerService::Abstract {
public:
	void abort_once_invoked_exit (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		std::atexit (_XVALUE_<PTR<void ()>> ([] () noexcept {
			GlobalRuntime::process_abort () ;
		})) ;
		signal (SIGFPE ,_XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		})) ;
		signal (SIGILL ,_XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		})) ;
		signal (SIGSEGV ,_XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		})) ;
	}

	void output_memory_leaks_report (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	Array<DATA> captrue_stack_trace () popping override {
		using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
		auto rax = AutoBuffer<PTR<VOID>> (DEFAULT_RECURSIVE_SIZE::value) ;
		const auto r1x = backtrace (rax.self ,VAR32 (rax.size ())) ;
		Array<DATA> ret = Array<DATA> (r1x) ;
		for (INDEX i = 0 ,ie = ret.length () ; i < ie ; i++)
			ret[i] = DATA (_ADDRESS_ (rax[i])) ;
		return std::move (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<DATA> &address) popping override {
		Array<String<STR>> ret = Array<String<STR>> (address.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : address) {
			const auto r1x = _XVALUE_<PTR<VOID>> (&_LOAD_<ARR<BYTE>> (NULL ,LENGTH (i))) ;
			const auto r2x = UniqueRef<PTR<PTR<STRA>>> ([&] (PTR<PTR<STRA>> &me) {
				me = backtrace_symbols (&r1x ,1) ;
			} ,[&] (PTR<PTR<STRA>> &me) {
				if (me == NULL)
					return ;
				free (me) ;
			}) ;
			const auto r3x = _BUILDHEX16S_ (i) ;
			const auto r4x = _PARSESTRS_ (String<STRA> (PTRTOARR[r2x.self[0]])) ;
			ret[iw++] = String<STR>::make (_PCSTR_ ("[") ,r3x ,_PCSTR_ ("] : ") ,r4x) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}
} ;

inline exports DebuggerService::DebuggerService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;