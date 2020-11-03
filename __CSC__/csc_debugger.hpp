#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
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

class ConsoleService :
	delegate private Proxy {
private:
	struct Private {
		template <class...>
		class ImplBinder ;

		class Implement ;
	} ;

	using Binder = typename TextWriter<STR>::Binder ;

	class Abstract :
		delegate public Interface {
	public:
		virtual LENGTH buffer_size () const = 0 ;
		virtual void enable_option (const EFLAG &option) = 0 ;
		virtual void disable_option (const EFLAG &option) = 0 ;
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
	friend Singleton<ConsoleService> ;
	Mutable<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	implicit ConsoleService () = delete ;

	LENGTH buffer_size () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->buffer_size () ;
	}

	void enable_option (const EFLAG &option) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->enable_option (option) ;
	}

	void disable_option (const EFLAG &option) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->disable_option (option) ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->print (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->fatal (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void error (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->error (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void warn (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->warn (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void info (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->info (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void debug (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->debug (R1X (ARGVP0 ,msg...)) ;
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->verbose (R1X (ARGVP0 ,msg...)) ;
	}

	void attach_log (const String<STR> &path) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->attach_log (path) ;
	}

	template <class... _ARGS>
	void log (const String<STR> &tag ,const _ARGS &...msg) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->log (tag.raw () ,R1X (ARGVP0 ,msg...)) ;
	}

	void show () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->show () ;
	}

	void pause () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->pause () ;
	}

	void clear () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->clear () ;
	}

	void hide () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->hide () ;
	}

private:
	explicit ConsoleService (const ARGVF<Singleton<ConsoleService>> &) ;
} ;

template <class... UNITS>
class ConsoleService::Private::ImplBinder :
	delegate public Binder {
private:
	TupleBinder<const UNITS...> mBinder ;

public:
	implicit ImplBinder () = delete ;

	explicit ImplBinder (const DEF<decltype (ARGVP0)> & ,const UNITS &...initval) :
		delegate mBinder (initval...) {}

	void friend_write (TextWriter<STR> &writer) const override {
		template_write (writer ,mBinder) ;
	}

private:
	void template_write (TextWriter<STR> &writer ,const Tuple<> &binder) const {
		_NOOP_ () ;
	}

	template <class... _ARGS>
	void template_write (TextWriter<STR> &writer ,const Tuple<_ARGS...> &binder) const {
		writer << binder.one () ;
		template_write (writer ,binder.rest ()) ;
	}
} ;

class DebuggerService :
	delegate private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual void abort_once_invoked_exit (const BOOL &flag) = 0 ;
		virtual void output_memory_leaks_report (const BOOL &flag) = 0 ;
		virtual Array<LENGTH> captrue_stack_trace () = 0 ;
		virtual Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) = 0 ;
	} ;

private:
	friend Singleton<DebuggerService> ;
	Mutable<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	implicit DebuggerService () = delete ;

	void abort_once_invoked_exit (const BOOL &flag) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->abort_once_invoked_exit (flag) ;
	}

	void output_memory_leaks_report (const BOOL &flag) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->output_memory_leaks_report (flag) ;
	}

	Array<LENGTH> captrue_stack_trace () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->captrue_stack_trace () ;
	}

	Array<String<STR>> symbol_from_address (const Array<LENGTH> &list) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->symbol_from_address (list) ;
	}

private:
	explicit DebuggerService (const ARGVF<Singleton<DebuggerService>> &) ;
} ;
} ;