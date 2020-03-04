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
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef switch_case
#undef discard
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef __CSC_DEPRECATED__
#pragma region
#include <crtdbg.h>
#include <signal.h>
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4091)
#endif
#include <DbgHelp.h>
#pragma warning (pop)
#pragma endregion
#endif

#ifndef _DBGHELP_
#error "∑(っ°Д° ;)っ : require 'DbgHelp.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_dbghelp
#define use_comment_lib_dbghelp "dbghelp.lib"
#endif
#pragma comment (lib ,use_comment_lib_dbghelp)
#undef use_comment_lib_dbghelp
#endif
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
class ConsoleService::Implement :public ConsoleService::Abstract {
private:
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	LENGTH mBufferSize ;
	FLAG mOptionFlag ;
	UniqueRef<HANDLE> mConsole ;
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
		mOptionFlag = OPTION_DEFAULT ;
		mLogPath = String<STR> () ;
	}

	LENGTH buffer_size () const override {
		return mBufferSize ;
	}

	void enable_option (FLAG option) override {
		mOptionFlag |= option ;
	}

	void disable_option (FLAG option) override {
		mOptionFlag &= ~option ;
	}

	void print (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_PRINT) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("PRINT") ,r1x) ;
	}

	void fatal (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_FATAL) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("FATAL") ,r1x) ;
	}

	void error (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_ERROR) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("ERROR") ,r1x) ;
	}

	void warn (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_WARN) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("WARN") ,r1x) ;
	}

	void info (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_INFO) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_GREEN | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("INFO") ,r1x) ;
	}

	void debug (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_DEBUG) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("DEBUG") ,r1x) ;
	}

	void verbose (const Binder &msg) override {
		if ((mOptionFlag & OPTION_NO_VERBOSE) != 0)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,&rax ,NULL) ;
		rax = VARY (0) ;
		WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,&rax ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("VERBOSE") ,r1x) ;
	}

	void attach_log (const String<STR> &path) override {
		const auto r1x = _ABSOLUTEPATH_ (path) ;
		if switch_case (TRUE) {
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
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			FreeConsole () ;
		}) ;
	}

	void hide () override {
		mConsole = UniqueRef<HANDLE> () ;
	}

	void pause () override {
		if (!mConsole.exist ())
			return ;
		const auto r1x = GetConsoleWindow () ;
		if (r1x == NULL)
			return ;
		FlashWindow (r1x ,TRUE) ;
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)) ;
		const auto r2x = std::system (_PCSTRA_ ("pause")) ;
		(void) r2x ;
	}

	void clear () override {
		if (!mConsole.exist ())
			return ;
		const auto r1x = std::system (_PCSTRA_ ("cls")) ;
		(void) r1x ;
	}

private:
	void write_con_buffer (const Binder &msg) {
		mConWriter << _CLS_ ;
		mConWriter << msg ;
		mConWriter << _EOS_ ;
	}

	void attach_console () {
		if (mConsole.exist ())
			return ;
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
		} ,[] (HANDLE &me) {
			_STATIC_WARNING_ ("noop") ;
		}) ;
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
		OutputDebugString (mLogWriter.raw ().self) ;
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
		if ((mOptionFlag & OPTION_ALWAYS_FLUSH) == 0)
			return ;
		if (!mLogFileStream.exist ())
			return ;
		mLogFileStream->flush () ;
	}

	void attach_log_file () {
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
private:
	UniqueRef<HANDLE> mSymbolFromAddress ;

public:
	void abort_once_invoked_exit (BOOL flag) override {
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039)
#endif
		_DEBUG_ASSERT_ (flag) ;
		const auto r1x = _XVALUE_<PTR<void ()>> ([] () noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r2x = _XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r3x = _XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		const auto r4x = _XVALUE_<PTR<void (VAR32)>> ([] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		std::atexit (r1x) ;
		signal (SIGFPE ,r2x) ;
		signal (SIGILL ,r3x) ;
		signal (SIGSEGV ,r4x) ;
#pragma warning (pop)
#pragma endregion
	}

	void output_memory_leaks_report (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
		const auto r2x = VAR32 (r1x | _CRTDBG_LEAK_CHECK_DF) ;
		const auto r3x = _CrtSetDbgFlag (r2x) ;
		(void) r1x ;
		(void) r2x ;
		(void) r3x ;
	}

	Array<LENGTH> captrue_stack_trace () popping override {
		using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
		auto rax = AutoBuffer<PTR<VOID>> (DEFAULT_RECURSIVE_SIZE::value) ;
		const auto r1x = CaptureStackBackTrace (3 ,VARY (rax.size ()) ,rax.self ,NULL) ;
		Array<LENGTH> ret = Array<LENGTH> (r1x) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ()))
			ret[i] = _ADDRESS_ (rax[i]) ;
		return std::move (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) popping override {
		_DEBUG_ASSERT_ (list.length () < VAR32_MAX) ;
		using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
		attach_symbol_info () ;
		Array<String<STR>> ret = Array<String<STR>> (list.length ()) ;
		INDEX iw = 0 ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!mSymbolFromAddress.exist ())
				discard ;
			const auto r1x = _ALIGNOF_ (SYMBOL_INFO) - 1 + _SIZEOF_ (SYMBOL_INFO) + list.length () * DEFAULT_SHORTSTRING_SIZE::value ;
			auto rax = AutoBuffer<BYTE> (r1x) ;
			const auto r2x = _ALIGNAS_ (_ADDRESS_ (&rax.self) ,_ALIGNOF_ (SYMBOL_INFO)) ;
			auto &r3y = _LOAD_<SYMBOL_INFO> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r2x)) ;
			r3y.SizeOfStruct = _SIZEOF_ (SYMBOL_INFO) ;
			r3y.MaxNameLen = DEFAULT_SHORTSTRING_SIZE::value ;
			for (auto &&i : list) {
				SymFromAddr (mSymbolFromAddress ,DATA (i) ,NULL ,&r3y) ;
				const auto r4x = _BUILDHEX16S_ (DATA (r3y.Address)) ;
				const auto r5x = _PARSESTRS_ (String<STRA> (PTRTOARR[r3y.Name])) ;
				ret[iw++] = String<STR>::make (_PCSTR_ ("[") ,r4x ,_PCSTR_ ("] : ") ,r5x) ;
			}
		}
		if switch_case (fax) {
			for (auto &&i : list) {
				const auto r6x = _BUILDHEX16S_ (DATA (i)) ;
				ret[iw++] = String<STR>::make (_PCSTR_ ("[") ,r6x ,_PCSTR_ ("] : null")) ;
			}
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

private:
	void attach_symbol_info () {
		if (mSymbolFromAddress.exist ())
			return ;
		mSymbolFromAddress = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = GetCurrentProcess () ;
			const auto r1x = SymInitialize (me ,NULL ,TRUE) ;
			if (r1x)
				return ;
			me = NULL ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			SymCleanup (me) ;
		}) ;
		if (mSymbolFromAddress.self != NULL)
			return ;
		mSymbolFromAddress = UniqueRef<HANDLE> () ;
	}
} ;

inline exports DebuggerService::DebuggerService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;