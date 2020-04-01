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
class ConsoleService final
	:private Proxy {
public:
	static constexpr auto OPTION_DEFAULT = EFLAG (0) ;
	static constexpr auto OPTION_NO_PRINT = EFLAG (1) ;
	static constexpr auto OPTION_NO_FATAL = EFLAG (2) ;
	static constexpr auto OPTION_NO_ERROR = EFLAG (3) ;
	static constexpr auto OPTION_NO_WARN = EFLAG (4) ;
	static constexpr auto OPTION_NO_INFO = EFLAG (5) ;
	static constexpr auto OPTION_NO_DEBUG = EFLAG (6) ;
	static constexpr auto OPTION_NO_VERBOSE = EFLAG (7) ;
	static constexpr auto OPTION_ALWAYS_FLUSH = EFLAG (8) ;
	static constexpr auto OPTION_TESTING = EFLAG (9) ;

private:
	using Binder = typename TextWriter<STR>::Binder ;

	exports class Abstract
		:public Interface {
	public:
		virtual LENGTH buffer_size () const = 0 ;
		virtual void enable_option (EFLAG option) = 0 ;
		virtual void disable_option (EFLAG option) = 0 ;
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
	struct Detail ;
	class Implement ;
	friend Singleton<ConsoleService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	LENGTH buffer_size () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->buffer_size () ;
	}

	void enable_option (EFLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->enable_option (option) ;
	}

	void disable_option (EFLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->disable_option (option) ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->print (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->fatal (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void error (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->error (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void warn (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->warn (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void info (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->info (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void debug (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->debug (ImplBinder (msg...)) ;
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->verbose (ImplBinder (msg...)) ;
	}

	void attach_log (const String<STR> &path) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->attach_log (path) ;
	}

	template <class... _ARGS>
	void log (const String<STR> &tag ,const _ARGS &...msg) {
		struct Dependent ;
		using ImplBinder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplBinder<_ARGS...> ;
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->log (tag.raw () ,ImplBinder (msg...)) ;
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

struct ConsoleService::Detail {
	template <class... UNITS>
	class ImplBinder
		:public Binder {
	private:
		TupleBinder<const UNITS...> mBinder ;

	public:
		inline explicit ImplBinder (const UNITS &...initval)
			:mBinder (initval...) {}

		inline void friend_write (TextWriter<STR> &writer) const override {
			template_write (writer ,mBinder) ;
		}

	private:
		inline void template_write (TextWriter<STR> &writer ,const Tuple<> &binder) const {
			_STATIC_WARNING_ ("noop") ;
		}

		template <class... _ARGS>
		inline void template_write (TextWriter<STR> &writer ,const Tuple<_ARGS...> &binder) const {
			writer << binder.one () ;
			template_write (writer ,binder.rest ()) ;
		}
	} ;
} ;

class DebuggerService final
	:private Proxy {
private:
	exports class Abstract
		:public Interface {
	public:
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