#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : require 'csc_debugger.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("delegate")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef delegate
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef __CSC_DEPRECATED__
#include <crtdbg.h>
#include <signal.h>
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#pragma warning (disable :4091)
#endif
#include <DbgHelp.h>
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif
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
#pragma pop_macro ("delegate")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using ::SYMBOL_INFO ;

using std::system ;

using ::atexit ;
using ::signal ;

using ::SetConsoleTextAttribute ;
using ::WriteConsole ;
using ::GetStdHandle ;
using ::AllocConsole ;
using ::FreeConsole ;
using ::GetConsoleWindow ;
using ::FlashWindow ;
using ::GetCurrentProcess ;
using ::OutputDebugString ;
using ::SymFromAddr ;
using ::SymInitialize ;
using ::SymCleanup ;
} ;

class ConsoleService::Private::Implement :
	delegate public ConsoleService::Abstract {
private:
	TextWriter<STR> mConWriter ;
	TextWriter<STR> mLogWriter ;
	LENGTH mBufferSize ;
	Set<EFLAG> mOptionSet ;
	UniqueRef<HANDLE> mConsole ;
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

	void print (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_PRINT) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("PRINT") ,r1x) ;
	}

	void fatal (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_FATAL) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("FATAL") ,r1x) ;
	}

	void error (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_ERROR) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("ERROR") ,r1x) ;
	}

	void warn (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_WARN) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("WARN") ,r1x) ;
	}

	void info (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_INFO) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_GREEN | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("INFO") ,r1x) ;
	}

	void debug (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_DEBUG) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("DEBUG") ,r1x) ;
	}

	void verbose (const Binder &msg) override {
		if (mOptionSet.find (OPTION_NO_VERBOSE) != VAR_NONE)
			return ;
		write_con_buffer (msg) ;
		attach_console () ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,mConWriter.raw ().self ,VARY (mConWriter.length () - 1) ,DEPTR[rax] ,NULL) ;
		rax = VARY (0) ;
		api::WriteConsole (mConsole ,_PCSTR_ ("\n") ,1 ,DEPTR[rax] ,NULL) ;
		if (mLogPath.empty ())
			return ;
		const auto r1x = PhanBuffer<const STR>::make (mConWriter.raw ().self ,(mConWriter.length () - 1)) ;
		log (_PCSTR_ ("VERBOSE") ,r1x) ;
	}

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
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<PhanBuffer<const STR>> ;
		const auto r1x = PhanBuffer<const STR>::make (tag.self ,tag.size ()) ;
		log (r1x ,R1X (ARGVP0 ,msg)) ;
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
			if (mConsole.self != NULL)
				return ;
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			api::AllocConsole () ;
			me = api::GetStdHandle (STD_OUTPUT_HANDLE) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			api::FreeConsole () ;
		}) ;
	}

	void hide () override {
		mConsole = UniqueRef<HANDLE> () ;
	}

	void pause () override {
		if (!mConsole.exist ())
			return ;
		const auto r1x = api::GetConsoleWindow () ;
		if (r1x == NULL)
			return ;
		api::FlashWindow (r1x ,TRUE) ;
		api::SetConsoleTextAttribute (mConsole ,(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)) ;
		const auto r2x = api::system (_PCSTRA_ ("pause")) ;
		_NOOP_ (r2x) ;
	}

	void clear () override {
		if (!mConsole.exist ())
			return ;
		const auto r1x = api::system (_PCSTRA_ ("cls")) ;
		_NOOP_ (r1x) ;
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
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = api::GetStdHandle (STD_OUTPUT_HANDLE) ;
		} ,[] (HANDLE &me) {
			_NOOP_ () ;
		}) ;
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
		api::OutputDebugString (mLogWriter.raw ().self) ;
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

exports ConsoleService::ConsoleService (const ARGVF<Singleton<ConsoleService>> &) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::Implement ;
	mThis = StrongRef<R1X>::make () ;
}

class DebuggerService::Private::Implement :
	delegate public DebuggerService::Abstract {
private:
	UniqueRef<HANDLE> mSymbolFromAddress ;

public:
	implicit Implement () = default ;

	void abort_once_invoked_exit (const BOOL &flag) override {
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#pragma warning (disable :5039)
#endif
		_DEBUG_ASSERT_ (flag) ;
		api::atexit ([] () noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		api::signal (SIGFPE ,[] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		api::signal (SIGILL ,[] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
		api::signal (SIGSEGV ,[] (VAR32) noexcept {
			GlobalRuntime::process_abort () ;
		}) ;
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif
	}

	void output_memory_leaks_report (const BOOL &flag) override {
		_DEBUG_ASSERT_ (flag) ;
		const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
		_NOOP_ (r1x) ;
		const auto r2x = VAR32 (r1x | _CRTDBG_LEAK_CHECK_DF) ;
		_NOOP_ (r2x) ;
		const auto r3x = _CrtSetDbgFlag (r2x) ;
		_NOOP_ (r3x) ;
	}

	Array<LENGTH> captrue_stack_trace () override {
		auto rax = AutoBuffer<PTR<NONE>> (DEFAULT_RECURSIVE_SIZE::compile ()) ;
		const auto r1x = CaptureStackBackTrace (3 ,VARY (rax.size ()) ,rax.self ,NULL) ;
		Array<LENGTH> ret = Array<LENGTH> (r1x) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ()))
			ret[i] = _ADDRESS_ (rax[i]) ;
		return _MOVE_ (ret) ;
	}

	Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) override {
		_DEBUG_ASSERT_ (list.length () < VAR32_MAX) ;
		attach_symbol_info () ;
		Array<String<STR>> ret = Array<String<STR>> (list.size ()) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!mSymbolFromAddress.exist ())
				discard ;
			const auto r1x = _ALIGNOF_ (api::SYMBOL_INFO) - 1 + _SIZEOF_ (api::SYMBOL_INFO) + list.length () * DEFAULT_FILEPATH_SIZE::compile () ;
			auto rax = AutoBuffer<BYTE> (r1x) ;
			const auto r2x = _ADDRESS_ (DEPTR[rax.self]) ;
			const auto r3x = _ALIGNAS_ (r2x ,_ALIGNOF_ (api::SYMBOL_INFO)) ;
			const auto r4x = _POINTER_CAST_ (ARGV<api::SYMBOL_INFO>::ID ,_UNSAFE_POINTER_ (r3x)) ;
			DEREF[r4x].SizeOfStruct = VARY (_SIZEOF_ (api::SYMBOL_INFO)) ;
			DEREF[r4x].MaxNameLen = VARY (DEFAULT_FILEPATH_SIZE::compile ()) ;
			for (auto &&i : _RANGE_ (0 ,list.length ())) {
				api::SymFromAddr (mSymbolFromAddress ,DATA (list[i]) ,NULL ,r4x) ;
				const auto r5x = StringProc::build_hexs (ARGV<STR>::ID ,DATA (DEREF[r4x].Address)) ;
				const auto r6x = StringProc::parse_strs (String<STRA> (PTRTOARR[DEREF[r4x].Name])) ;
				ret[i] = String<STR>::make (_PCSTR_ ("[") ,r5x ,_PCSTR_ ("] : ") ,r6x) ;
			}
		}
		if switch_once (fax) {
			for (auto &&i : _RANGE_ (0 ,list.length ())) {
				const auto r7x = StringProc::build_hexs (ARGV<STR>::ID ,DATA (list[i])) ;
				ret[i] = String<STR>::make (_PCSTR_ ("[") ,r7x ,_PCSTR_ ("] : null")) ;
			}
		}
		return _MOVE_ (ret) ;
	}

private:
	void attach_symbol_info () {
		if (mSymbolFromAddress.exist ())
			return ;
		mSymbolFromAddress = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = api::GetCurrentProcess () ;
			const auto r1x = api::SymInitialize (me ,NULL ,TRUE) ;
			if (r1x)
				return ;
			me = NULL ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			api::SymCleanup (me) ;
		}) ;
		if (mSymbolFromAddress.self != NULL)
			return ;
		mSymbolFromAddress = UniqueRef<HANDLE> () ;
	}
} ;

exports DebuggerService::DebuggerService (const ARGVF<Singleton<DebuggerService>> &) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::Implement ;
	mThis = StrongRef<R1X>::make () ;
}
} ;