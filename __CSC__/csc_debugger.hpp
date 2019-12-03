#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_images.hpp"
#include "csc_geometry.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"
#include "csc_graphics.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"
#include "csc_database.hpp"

namespace CSC {
class ConsoleService final :private Interface {
public:
	static constexpr auto OPTION_DEFAULT = FLAG (0) ;
	static constexpr auto OPTION_NO_PRINT = FLAG (0X00000001) ;
	static constexpr auto OPTION_NO_FATAL = FLAG (0X00000002) ;
	static constexpr auto OPTION_NO_ERROR = FLAG (0X00000004) ;
	static constexpr auto OPTION_NO_WARN = FLAG (0X00000008) ;
	static constexpr auto OPTION_NO_INFO = FLAG (0X00000010) ;
	static constexpr auto OPTION_NO_DEBUG = FLAG (0X00000020) ;
	static constexpr auto OPTION_NO_VERBOSE = FLAG (0X00000040) ;
	static constexpr auto OPTION_ALWAYS_FLUSH = FLAG (0X00000080) ;
	static constexpr auto OPTION_TESTING = FLAG (0X00000100) ;

private:
	exports struct Binder :public Interface {
		virtual void friend_write (TextWriter<STR> &writer) const = 0 ;
	} ;

	template <class... UNITS>
	class ImplBinder :public Binder {
	private:
		TupleBinder<const UNITS...> mBinder ;

	public:
		inline explicit ImplBinder (const UNITS &...initval) :mBinder (initval...) {}

		inline void friend_write (TextWriter<STR> &writer) const override {
			template_write (writer ,mBinder) ;
		}

	public:
		inline static void template_write (TextWriter<STR> &writer ,const Tuple<> &binder) {
			_STATIC_WARNING_ ("noop") ;
		}

		template <class... _ARGS>
		inline static void template_write (TextWriter<STR> &writer ,const Tuple<_ARGS...> &binder) {
			writer << binder.one () ;
			template_write (writer ,binder.rest ()) ;
		}
	} ;

	exports struct Abstract :public Interface {
		virtual LENGTH buffer_size () const = 0 ;
		virtual void enable_option (FLAG option) = 0 ;
		virtual void disable_option (FLAG option) = 0 ;
		virtual void print (const Binder &msg) = 0 ;
		virtual void fatal (const Binder &msg) = 0 ;
		virtual void error (const Binder &msg) = 0 ;
		virtual void warn (const Binder &msg) = 0 ;
		virtual void info (const Binder &msg) = 0 ;
		virtual void debug (const Binder &msg) = 0 ;
		virtual void verbose (const Binder &msg) = 0 ;
		virtual void attach_log (const String<STR> &path) = 0 ;
		virtual void log (const PhanBuffer<const STR> &tag ,const Binder &msg) = 0 ;
		virtual void show () = 0 ;
		virtual void hide () = 0 ;
		virtual void pause () = 0 ;
		virtual void clear () = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<ConsoleService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	LENGTH buffer_size () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->buffer_size () ;
	}

	void enable_option (FLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->enable_option (option) ;
	}

	void disable_option (FLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->disable_option (option) ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->print (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->fatal (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void error (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->error (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void warn (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->warn (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void info (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->info (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void debug (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->debug (ImplBinder<_ARGS...> (msg...)) ;
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->verbose (ImplBinder<_ARGS...> (msg...)) ;
	}

	void attach_log (const String<STR> &path) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->attach_log (path) ;
	}

	template <class... _ARGS>
	void log (const String<STR> &tag ,const _ARGS &...msg) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->log (tag.raw () ,ImplBinder<_ARGS...> (msg...)) ;
	}

	void show () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->show () ;
	}

	void pause () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->pause () ;
	}

	void clear () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->clear () ;
	}

	void hide () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->hide () ;
	}

private:
	ConsoleService () ;
} ;

class DebuggerService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual void abort_once_invoked_exit (BOOL flag) = 0 ;
		virtual void output_memory_leaks_report (BOOL flag) = 0 ;
		virtual Array<LENGTH> captrue_stack_trace () popping = 0 ;
		virtual Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) popping = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<DebuggerService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	void abort_once_invoked_exit (BOOL flag) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->abort_once_invoked_exit (flag) ;
	}

	void output_memory_leaks_report (BOOL flag) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->output_memory_leaks_report (flag) ;
	}

	Array<LENGTH> captrue_stack_trace () popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->captrue_stack_trace () ;
	}

	Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->symbol_from_address (list) ;
	}

private:
	DebuggerService () ;
} ;
} ;