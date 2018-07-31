#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : required csc_debugger.hpp"
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : required Windows.h"
#endif

#ifndef __CSC_STRICTHPPINL__
#pragma region
#include <cstdlib>
#include <crtdbg.h>
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4091) //@info:warning C4091: “typedef ”: 没有声明变量时忽略“”的左侧
#endif
#include <DbgHelp.h>
#pragma warning (pop)
#pragma endregion
#pragma comment (lib ,"dbghelp.lib")
#endif

namespace CSC {
class Console::Engine :public Interface {
public:
	static constexpr FLAG LEVEL_PRINT = 0 ;
	static constexpr FLAG LEVEL_FATAL = 1 ;
	static constexpr FLAG LEVEL_ERROR = 2 ;
	static constexpr FLAG LEVEL_WARN = 3 ;
	static constexpr FLAG LEVEL_INFO = 4 ;
	static constexpr FLAG LEVEL_DEBUG = 5 ;
	static constexpr FLAG LEVEL_VERBOSE = 6 ;

private:
	using BUFFER_SIZE = ARGC<HUGESTRING_SIZE::value + 1> ;

private:
	UniqueRef<HANDLE> mConsole ;
	TextWriter<STR> mConWriter ;
	FLAG mLevel ;

public:
	Engine () {
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			me = NULL ;
		}) ;
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (BUFFER_SIZE::value)) ;
		mLevel = LEVEL_VERBOSE ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...args) {
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...args) {
		if (mLevel < LEVEL_FATAL)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void error (const _ARGS &...args) {
		if (mLevel < LEVEL_ERROR)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_RED | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void warn (const _ARGS &...args) {
		if (mLevel < LEVEL_WARN)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void info (const _ARGS &...args) {
		if (mLevel < LEVEL_INFO)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void debug (const _ARGS &...args) {
		if (mLevel < LEVEL_DEBUG)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...args) {
		if (mLevel < LEVEL_VERBOSE)
			return ;
		write_buffer (args...) ;
		SetConsoleTextAttribute (mConsole ,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
		WriteConsole (mConsole ,mConWriter ,VARY (mConWriter.length () - 1) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	}

	void enable (FLAG level) {
		_DEBUG_ASSERT_ (level >= LEVEL_PRINT && level <= LEVEL_VERBOSE) ;
		mLevel = level ;
	}

	void start () {
		if (mConsole.exist () && mConsole != NULL)
			return ;
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			FreeConsole () ;
			me = NULL ;
		}) ;
	}

	void pause () {
		SetConsoleTextAttribute (mConsole ,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		std::system (_PCSTRA_ ("pause")) ;
	}

	void clear () {
		std::system (_PCSTRA_ ("cls")) ;
	}

	void stop () {
		mConsole = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			me = NULL ;
		}) ;
	}

private:
	template <class... _ARGS>
	void write_buffer (const _ARGS &...args) {
		mConWriter.reset () ;
		mConWriter << StreamBinder<const _ARGS...> (args...) ;
		mConWriter << _PCSTR_ ("\n") ;
		mConWriter << _EOS_ ;
	}
} ;

template <class... _ARGS>
inline export void Console::print (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->print (args...) ;
}

template <class... _ARGS>
inline export void Console::fatal (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->fatal (args...) ;
}

template <class... _ARGS>
inline export void Console::error (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->error (args...) ;
}

template <class... _ARGS>
inline export void Console::warn (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->warn (args...) ;
}

template <class... _ARGS>
inline export void Console::info (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->info (args...) ;
}

template <class... _ARGS>
inline export void Console::debug (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->debug (args...) ;
}

template <class... _ARGS>
inline export void Console::verbose (const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->verbose (args...) ;
}

inline export void Console::enable (FLAG level) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->enable (level) ;
}

inline export void Console::start () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->start () ;
}

inline export void Console::pause () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->pause () ;
}

inline export void Console::clear () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->clear () ;
}

inline export void Console::stop () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->stop () ;
}

inline export Console::Console () {
	mMutex = SharedRef<std::recursive_mutex>::make () ;
	mHolder = AnyRef<Engine>::make () ;
}

class Logger::Engine :public Interface {
private:
	using BUFFER_SIZE = ARGC<HUGESTRING_SIZE::value + 1> ;

private:
	StreamLoader mLogFile ;
	TextWriter<STR> mLogWriter ;

public:
	Engine () {
		mLogWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (BUFFER_SIZE::value)) ;
		update_log_file () ;
	}

	template <class... _ARGS>
	void write (const String<STR> &tag ,const _ARGS &...args) {
		write_buffer (tag ,args...) ;
		write_file () ;
#ifdef __CSC_DEBUG__
		write_debugger () ;
#endif
	}

private:
	void update_log_file () {
		const auto r1x = String<STR> (_PCSTR_ ("logger.log")) ;
		const auto r2x = String<STR> (_PCSTR_ ("logger.old.log")) ;
		_ERASEFILE_ (r2x) ;
		_MOVEFILE_ (r2x ,r1x) ;
		mLogFile = StreamLoader (r1x) ;
		mLogWriter.reset () ;
		mLogWriter << _BOM_ ;
		write_file () ;
	}

	template <class... _ARGS>
	void write_buffer (const String<STR> &tag ,const _ARGS &...args) {
		mLogWriter.reset () ;
		mLogWriter << _PCSTR_ ("[") ;
		mLogWriter << _BUILDHOURS_<STR> (std::chrono::system_clock ().now ()) ;
		mLogWriter << _PCSTR_ ("][") ;
		mLogWriter << tag ;
		mLogWriter << _PCSTR_ ("] : ") ;
		mLogWriter << StreamBinder<const _ARGS...> (args...) ;
		mLogWriter << _PCSTR_ ("\r\n") ;
		mLogWriter << _EOS_ ;
	}

	void write_file () {
		const auto r1x = _MAX_ (mLogWriter.length () - 1 ,VAR_ZERO) * _SIZEOF_ (STR) ;
		const auto r2x = PhanBuffer<const BYTE>::make (_LOAD_<ARR<BYTE>> (&mLogWriter.self) ,r1x) ;
		_CALL_TRY_ ([&] () {
			mLogFile.write (r2x) ;
		} ,[&] () {
			_CALL_TRY_ ([&] () {
				update_log_file () ;
				mLogFile.write (r2x) ;
			}) ;
		}) ;
	}

	void write_debugger () {
		OutputDebugString (mLogWriter.raw ()) ;
	}
} ;

template <class... _ARGS>
inline export void Logger::write (const String<STR> &tag ,const _ARGS &...args) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->write (tag ,args...) ;
}

inline export Logger::Logger () {
	mMutex = SharedRef<std::recursive_mutex>::make () ;
	mHolder = AnyRef<Engine>::make () ;
}
} ;

#if defined (_CSTDLIB_) || defined (_GLIBCXX_CSTDLIB)
#if defined (_DBGHELP_) && defined (_INC_CRTDBG)
namespace CSC {
class DebuggerService::Engine :public Interface {
private:
	UniqueRef<HANDLE> mSymboalFromAdress ;

public:
	Engine () = default ;

	void abort_once_invoked_exit (BOOL flag) {
		_DEBUG_ASSERT_ (flag) ;
		std::atexit (std::abort) ;
	}

	void check_memory_leaks (BOOL flag) {
		const auto r1x = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG) ;
		const auto r2x = flag ? r1x | _CRTDBG_LEAK_CHECK_DF : r1x & ~_CRTDBG_LEAK_CHECK_DF ;
		(void) _CrtSetDbgFlag (r2x) ;
		(void) r2x ;
	}

	Array<DATA> captrue_stack_trace () {
		const auto r1x = _CALL_ ([&] () {
			PACK<AutoBuffer<PTR<VOID>> ,LENGTH> ret ;
			ret.P1 = AutoBuffer<PTR<VOID>> (RECURSIVE_SIZE::value) ;
			ret.P2 = CaptureStackBackTrace (1 ,VARY (ret.P1.size ()) ,ret.P1 ,NULL) ;
			return std::move (ret) ;
		}) ;
		Array<DATA> ret = Array<DATA> (r1x.P2) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			ret[i] = DATA (_ADRESS_ (r1x.P1[i])) ;
		return std::move (ret) ;
	}

	Array<String<STRU8>> symbol_from_address (const Array<DATA> &adress) {
		if (!mSymboalFromAdress.exist () || mSymboalFromAdress == NULL) {
			mSymboalFromAdress = UniqueRef<HANDLE> ([&] (HANDLE &me) {
				me = GetCurrentProcess () ;
				const auto r1x = SymInitialize (me ,NULL ,TRUE) ;
				if (!r1x)
					me = NULL ;
			} ,[] (HANDLE &me) {
				if (me == NULL)
					return ;
				SymCleanup (me) ;
				me = NULL ;
			}) ;
		}
		Array<String<STRU8>> ret = Array<String<STRU8>> (adress.length ()) ;
		auto &r1 = _CAST_<Array<String<STRU8>>> (ret) ;
		INDEX iw = 0 ;
		auto rax = AutoBuffer<BYTE> (_SIZEOF_ (SYMBOL_INFO) + adress.length () * (SHORTSTRING_SIZE::value + 1)) ;
		auto &r2 = _LOAD_<SYMBOL_INFO> (rax.self ,_ADRESS_ (rax.self)) ;
		r2.SizeOfStruct = _SIZEOF_ (SYMBOL_INFO) ;
		r2.MaxNameLen = SHORTSTRING_SIZE::value + 1 ;
		if (mSymboalFromAdress != NULL) {
			for (INDEX i = 0 ; i < rax.size () ; i++) {
				SymFromAddr (mSymboalFromAdress ,adress[i] ,0 ,&r2) ;
				r1[iw++] = _BUILDARGSS_<STRU8> (_PCSTRU8_ ("[") ,_BUILDHEX16S_<STRU8> (DATA (r2.Address)) ,_PCSTRU8_ ("] : ") ,_ASTOUAS_ (String<STRA> (r2.Name))) ;
			}
		} else {
			for (INDEX i = 0 ; i < rax.size () ; i++)
				r1[iw++] = _BUILDARGSS_<STRU8> (_PCSTRU8_ ("[") ,_BUILDHEX16S_<STRU8> (adress[i]) ,_PCSTRU8_ ("] : null")) ;
		}
		return std::move (ret) ;
	}
} ;

inline export void DebuggerService::abort_once_invoked_exit (BOOL flag) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->abort_once_invoked_exit (flag) ;
}

inline export void DebuggerService::check_memory_leaks (BOOL flag) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->check_memory_leaks (flag) ;
}

inline export Array<DATA> DebuggerService::captrue_stack_trace () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	return mHolder.rebind<Engine> ()->captrue_stack_trace () ;
}

inline export Array<String<STRU8>> DebuggerService::symbol_from_address (const Array<DATA> &adress) {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	return mHolder.rebind<Engine> ()->symbol_from_address (adress) ;
}

inline export DebuggerService::DebuggerService () {
	mMutex = SharedRef<std::recursive_mutex>::make () ;
	mHolder = AnyRef<Engine>::make () ;
}
} ;
#endif
#endif