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

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef __CSC_DEPRECATED__
#pragma region
#include <cstdlib>
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
#pragma pop_macro ("discard")
#endif

namespace CSC {
class ConsoleService::Implement :public ConsoleService::Abstract {
private:
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	FLAG mOptionFlag ;
	UniqueRef<HANDLE> mConsole ;
	String<STR> mLogPath ;
	AutoRef<StreamLoader> mLogFileStream ;

	BOOL mTempState ;

public:
	Implement () {
		const auto r1x = _COPY_ (DEFAULT_HUGEBUFFER_SIZE::value) ;
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		mLogWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (r1x)) ;
		attach_console () ;
		modify_option (OPTION_DEFAULT) ;
		mLogPath = String<STR> () ;
	}

	void modify_option (FLAG option) override {
		if (option == OPTION_DEFAULT)
			mOptionFlag = OPTION_DEFAULT ;
		mOptionFlag |= option ;
	}

	void print (const Binder &msg) override {
		write_con_buffer (msg) ;
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
		for (FOR_ONCE_DO_WHILE) {
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
		if (mConsole.exist () && mConsole != NULL)
			return ;
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeConsole () ;
		}) ;
	}

	void flash () override {
		if (!mConsole.exist ())
			return ;
		if (mConsole == NULL)
			return ;
		const auto r1x = GetConsoleWindow () ;
		if (r1x == NULL)
			return ;
		FlashWindow (r1x ,TRUE) ;
	}

	void pause () override {
		SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)) ;
		const auto r1x = std::system (_PCSTRA_ ("pause")) ;
		(void) r1x ;
	}

	void clear () override {
		const auto r1x = std::system (_PCSTRA_ ("cls")) ;
		(void) r1x ;
	}

	void hide () override {
		attach_console () ;
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
		mLogWriter << _BUILDHOURS_<STR> (std::chrono::system_clock ().now ()) ;
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

#if defined (_CSTDLIB_) || defined (_GLIBCXX_CSTDLIB)
#ifdef _DBGHELP_
#ifdef _INC_CRTDBG
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
#pragma warning (pop)
#pragma endregion
	}

	void output_memory_leaks_report (BOOL flag) override {
		_DEBUG_ASSERT_ (flag) ;
		const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
		const auto r2x = _CrtSetDbgFlag ((r1x | _CRTDBG_LEAK_CHECK_DF)) ;
		(void) r1x ;
		(void) r2x ;
	}

	Array<DATA> captrue_stack_trace () popping override {
		auto rax = AutoBuffer<PTR<VOID>> (DEFAULT_RECURSIVE_SIZE::value) ;
		const auto r1x = CaptureStackBackTrace (3 ,VARY (rax.size ()) ,rax.self ,NULL) ;
		Array<DATA> ret = Array<DATA> (r1x) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			ret[i] = DATA (_ADDRESS_ (rax[i])) ;
		return std::move (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<DATA> &address) popping override {
		attach_symbol_info () ;
		Array<String<STR>> ret = Array<String<STR>> (address.length ()) ;
		INDEX iw = 0 ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mSymbolFromAddress.exist ()) ;
			const auto r1x = _SIZEOF_ (SYMBOL_INFO) + address.length () * (DEFAULT_SHORTSTRING_SIZE::value) ;
			auto rax = AutoBuffer<BYTE> (r1x) ;
			const auto r4x = &_LOAD_<SYMBOL_INFO> (NULL ,_ADDRESS_ (&rax.self)) ;
			r4x->SizeOfStruct = _SIZEOF_ (SYMBOL_INFO) ;
			r4x->MaxNameLen = DEFAULT_SHORTSTRING_SIZE::value ;
			for (auto &&i : address) {
				SymFromAddr (mSymbolFromAddress ,i ,NULL ,r4x) ;
				const auto r2x = _BUILDHEX16S_<STR> (DATA (r4x->Address)) ;
				const auto r3x = _PARSESTRS_ (String<STRA> (PTRTOARR[r4x->Name])) ;
				ret[iw++] = String<STR>::make (_PCSTR_ ("[") ,r2x ,_PCSTR_ ("] : ") ,r3x) ;
			}
		} ,[&] (BOOL &_case_req) {
			for (auto &&i : address)
				ret[iw++] = String<STR>::make (_PCSTR_ ("[") ,_BUILDHEX16S_<STR> (i) ,_PCSTR_ ("] : null")) ;
		}) ;
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
		if (mSymbolFromAddress != NULL)
			return ;
		mSymbolFromAddress = UniqueRef<HANDLE> () ;
	}
} ;

inline exports DebuggerService::DebuggerService () {
	mThis = StrongRef<Implement>::make () ;
}
#endif
#endif
#endif
} ;