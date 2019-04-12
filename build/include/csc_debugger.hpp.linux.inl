#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef import
#undef export
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("import")
#pragma pop_macro ("export")
#endif

#ifdef __CSC_DEPRECATED__
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <execinfo.h>
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("import")
#pragma push_macro ("export")
#define self to ()
#define implicit
#define popping
#define import extern
#define export
#endif

namespace CSC {
class ConsoleService::Implement final :private ConsoleService::Abstract {
private:
	friend ConsoleService ;
	friend HolderRef<Abstract> ;
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	FLAG mOptionFlag ;
	String<STR> mLogPath ;
	AutoRef<StreamLoader> mLogFileStream ;

	BOOL mTempState ;

public:
	Implement () {
		const auto r1x = _COPY_ (DEFAULT_HUGEBUFFER_SIZE::value) ;
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		mLogWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		modify_option (OPTION_DEFAULT) ;
		mLogPath = String<STR> () ;
	}

	void modify_option (FLAG option) override {
		mOptionFlag = option ;
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
		log (_PCSTR_ ("PRINT") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("FATAL") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("ERROR") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("WARN") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("INFO") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("DEBUG") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
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
		log (_PCSTR_ ("VERBOSE") ,ImplBinder<StreamBinder<const PhanBuffer<const STR>>> (r1x)) ;
	}

	void enable_log (const String<STR> &path) override {
		const auto r1x = _ABSOLUTEPATH_ (path) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (mLogPath == r1x)
				continue ;
			if (!mLogFileStream.exist ())
				continue ;
			mLogFileStream->flush () ;
			mLogFileStream = AutoRef<StreamLoader> () ;
		}
		mLogPath = r1x ;
	}

	template <LENGTH _VAL>
	void log (const DEF<STR[_VAL]> &tag ,const Binder &msg) {
		log (PhanBuffer<const STR>::make (PTRTOARR[&tag[0]] ,(_VAL - 1)) ,msg) ;
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

	void flash () override {
		_STATIC_WARNING_ ("noop") ;
	}

	void pause () override {
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("press any key to continue...\n")) ;
#else
		std::wprintf (_PCSTR_ ("press any key to continue...\n")) ;
#endif
	}

	void clear () override {
		const auto r1x = std::system (_PCSTRA_ ("clear")) ;
		(void) r1x ;
	}

	void hide () override {
		_STATIC_WARNING_ ("noop") ;
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
		mLogWriter << _BUILDHOURS_<STR> (std::chrono::system_clock ().now ()) ;
		mLogWriter << _PCSTR_ ("][") ;
		mLogWriter << tag ;
		mLogWriter << _PCSTR_ ("] : ") ;
		mLogWriter << msg ;
		mLogWriter << _GAP_ ;
		mLogWriter << _EOS_ ;
	}

	void write_debugger () {
		if (mLogPath.empty ())
			return ;
		std::fprintf (stderr ,_PCSTRA_ ("%s") ,&_BUILDSTRS_<STRA> (String<STR> (mLogWriter.raw ())).raw ().self) ;
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
			(void) mLogFileStream ;
			mTempState = FALSE ;
		}) ;
		_CALL_TRY_ ([&] () {
			if (mTempState)
				return ;
			attach_log_file () ;
			mLogFileStream->write (r2x) ;
			mTempState = TRUE ;
		} ,[&] () {
			(void) mLogFileStream ;
			mTempState = FALSE ;
		}) ;
	}

	void attach_log_file () {
		const auto r1x = mLogPath + _PCSTR_ ("logger.log") ;
		const auto r2x = mLogPath + _PCSTR_ ("logger.old.log") ;
		_MOVEFILE_ (r2x ,r1x) ;
		mLogFileStream = AutoRef<StreamLoader>::make (r1x) ;
		const auto r3x = _PRINTS_<STR> (_XVALUE_<const PTR<void (TextWriter<STR> &)> &> (&_BOM_)) ;
		mLogFileStream->write (PhanBuffer<const BYTE>::make (r3x.raw ())) ;
	}
} ;

inline export ConsoleService::ConsoleService () {
	mThis = HolderRef<Abstract> (_NULL_<const ARGV<Implement>> ()) ;
}

class DebuggerService::Implement final :private DebuggerService::Abstract {
private:
	friend DebuggerService ;
	friend HolderRef<Abstract> ;

public:
	void abort_once_invoked_exit (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		std::atexit (std::abort) ;
	}

	void output_memory_leaks_report (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		_STATIC_WARNING_ ("unimplemented") ;
	}

	Array<DATA> captrue_stack_trace () popping override {
		auto rax = AutoBuffer<PTR<VOID>> (DEFAULT_RECURSIVE_SIZE::value) ;
		const auto r1x = backtrace (rax.self ,VAR32 (rax.size ())) ;
		Array<DATA> ret = Array<DATA> (r1x) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			ret[i] = DATA (_ADDRESS_ (rax[i])) ;
		return std::move (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<DATA> &address) popping override {
		Array<String<STR>> ret = Array<String<STR>> (address.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : address) {
			const auto r1x = PTR<VOID> (i) ;
			const auto r2x = backtrace_symbols (&r1x ,1) ;
			const auto r3x = _BUILDHEX16S_<STR> (i) ;
			const auto r4x = _PARSESTRS_ (String<STRA> (PTRTOARR[&r2x[0][0]])) ;
			ret[iw++] = _PRINTS_<STR> (_PCSTR_ ("[") ,r3x ,_PCSTR_ ("] : ") ,r4x) ;
		}
		return std::move (ret) ;
	}
} ;

inline export DebuggerService::DebuggerService () {
	mThis = HolderRef<Abstract> (_NULL_<const ARGV<Implement>> ()) ;
}
} ;