#pragma once

#ifndef __CSC_DEBUGGER__
#error "∑(っ°Д° ;)っ : required csc_debugger.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <cstdlib>
#include <cstdio>
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
	TextWriter<STR> mConWriter ;
	FLAG mLevel ;

public:
	Engine () {
		mConWriter = TextWriter<STR> (SharedRef<FixedBuffer<STR>>::make (BUFFER_SIZE::value)) ;
		mLevel = LEVEL_VERBOSE ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...args) {
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("%s\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("%ls\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...args) {
		if (mLevel < LEVEL_FATAL)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;34m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;34m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void error (const _ARGS &...args) {
		if (mLevel < LEVEL_ERROR)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;31m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;31m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void warn (const _ARGS &...args) {
		if (mLevel < LEVEL_WARN)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;33m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;33m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void info (const _ARGS &...args) {
		if (mLevel < LEVEL_INFO)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;32m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;32m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void debug (const _ARGS &...args) {
		if (mLevel < LEVEL_DEBUG)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;36m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;36m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...args) {
		if (mLevel < LEVEL_VERBOSE)
			return ;
		write_buffer (args...) ;
#ifdef __CSC_CONFIG_STRA__
		std::printf (_PCSTR_ ("\033[1;37m%s\033[0m\n") ,mConWriter.self) ;
#elif defined __CSC_CONFIG_STRW__
		std::wprintf (_PCSTR_ ("\033[1;37m%ls\033[0m\n") ,mConWriter.self) ;
#endif
	}

	void enable (FLAG level) {
		_DEBUG_ASSERT_ (level >= LEVEL_PRINT && level <= LEVEL_VERBOSE) ;
		mLevel = level ;
	}

	void start () {}

	void pause () {
		std::system (_PCSTRA_ ("pause")) ;
	}

	void clear () {
		std::system (_PCSTRA_ ("cls")) ;
	}

	void stop () {}

private:
	template <class... _ARGS>
	void write_buffer (const _ARGS &...args) {
		mConWriter.reset () ;
		mConWriter << StreamBinder<const _ARGS...> (args...) ;
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
		static const ARRAY2<String<STR>> mFileName = {
			_PCSTR_ ("logger.log") ,
			_PCSTR_ ("logger.old.log")} ;
		_MOVEFILE_ (mFileName[1] ,mFileName[0]) ;
		mLogFile = StreamLoader (mFileName[0]) ;
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
		std::fprintf (stderr ,_PCSTR_ ("%s") ,&_BUILDSTRS_<STRA> (String<STR> (mLogWriter.raw ())).self) ;
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

#ifdef __CSC_DEPRECATED__
class DebuggerService::Engine :public Interface {
public:
	Engine () = default ;

	void abort_once_invoked_exit (BOOL flag) {
		_DEBUG_ASSERT_ (flag) ;
		std::atexit (std::abort) ;
	}

	void check_memory_leaks (BOOL flag) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}

	Array<DATA> captrue_stack_trace () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Array<DATA>> () ;
	}

	Array<String<STRU8>> symbol_from_address (const Array<DATA> &adress) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Array<String<STRU8>>> () ;
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
#endif
} ;