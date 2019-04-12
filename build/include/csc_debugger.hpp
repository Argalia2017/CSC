#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"
#include "csc_database.hpp"

namespace CSC {
#ifdef __CSC_DEPRECATED__
namespace U {
template <class TYPE>
struct OPERATOR_TYPENAME {
	template <class _ARG>
	inline static void build_typename_cvs (TextWriter<STR> &writer ,const ARGV<_ARG> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline static void build_typename_cvs (TextWriter<STR> &writer ,const ARGV<const _ARG> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG>
	inline static void build_typename_cvs (TextWriter<STR> &writer ,const ARGV<volatile _ARG> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG>
	inline static void build_typename_cvs (TextWriter<STR> &writer ,const ARGV<const volatile _ARG> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG>
	inline static void build_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline static void build_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG &> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG>
	inline static void build_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG &&> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_pointer<_ARG>::value>> & ,const decltype (ARGVP8) &) {
		writer << _PCSTR_ ("PTR<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<REMOVE_POINTER_TYPE<_ARG>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_member_pointer<_ARG>::value>> & ,const decltype (ARGVP7) &) {
		writer << _PCSTR_ ("DEF<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<REMOVE_MEMBER_POINTER_TYPE<_ARG>>> ()) ;
		writer << _PCSTR_ (" ") ;
		build_typename_xs (writer ,_NULL_<const ARGV<MEMBEROF_TRAITS_TYPE<_ARG>>> ()) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,ARR<REMOVE_ARRAY_TYPE<_ARG>>>::value>> & ,const decltype (ARGVP6) &) {
		writer << _PCSTR_ ("ARR<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<REMOVE_ARRAY_TYPE<_ARG>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_array<_ARG>::value>> & ,const decltype (ARGVP5) &) {
		writer << _PCSTR_ ("DEF<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<REMOVE_ARRAY_TYPE<_ARG>>> ()) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG) ;
		writer << _PCSTR_ ("]>") ;
	}

	inline static void build_typename_params (TextWriter<STR> &writer ,const ARGVS<> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline static void build_typename_params (TextWriter<STR> &writer ,const ARGVS<_ARG> &) {
		build_typename_xs (writer ,_NULL_<const ARGV<_ARG>> ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static void build_typename_params (TextWriter<STR> &writer ,const ARGVS<_ARG1 ,_ARG2 ,_ARGS...> &) {
		build_typename_xs (writer ,_NULL_<const ARGV<_ARG1>> ()) ;
		writer << _PCSTR_ (" ,") ;
		build_typename_params (writer ,_NULL_<const ARGVS<_ARG2 ,_ARGS...>> ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void build_typename_funcs (TextWriter<STR> &writer ,const ARGV<_ARG1 (_ARGS...)> &) {
		writer << _PCSTR_ ("DEF<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<_ARG1>> ()) ;
		writer << _PCSTR_ (" (") ;
		build_typename_params (writer ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void build_typename_funcs (TextWriter<STR> &writer ,const ARGV<_ARG1 (_ARGS...) const> &) {
		writer << _PCSTR_ ("DEF<") ;
		build_typename_xs (writer ,_NULL_<const ARGV<_ARG1>> ()) ;
		writer << _PCSTR_ (" (") ;
		build_typename_params (writer ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		writer << _PCSTR_ (") const>") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_function<_ARG>::value>> & ,const decltype (ARGVP4) &) {
		build_typename_funcs (writer ,_NULL_<const ARGV<_ARG>> ()) ;
	}

	template <class _ARG>
	inline static void build_typename_clazzs (TextWriter<STR> &writer ,const ARGV<_ARG> &) {
		writer << _PCSTR_ ("class '") ;
		writer << _TYPEID_<_ARG> () ;
		writer << _PCSTR_ ("'") ;
	}

	template <template <class...> class _ARGT ,class..._ARGS>
	inline static void build_typename_clazzs (TextWriter<STR> &writer ,const ARGV<_ARGT<_ARGS...>> &) {
		writer << _PCSTR_ ("class '") ;
		writer << _TYPEID_<_ARGT<_ARGS...>> () ;
		writer << _PCSTR_ ("'<") ;
		build_typename_params (writer ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_class<_ARG>::value>> & ,const decltype (ARGVP3) &) {
		build_typename_clazzs (writer ,_NULL_<const ARGV<_ARG>> ()) ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,STRU8>::value && !std::is_same<_ARG ,BYTE>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,STRU16>::value && !std::is_same<_ARG ,WORD>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,STRU32>::value && !std::is_same<_ARG ,CHAR>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,VARX>::value && !std::is_same<_ARG ,VAR32>::value && !std::is_same<_ARG ,VAR64>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("VARX") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,VARY>::value && !std::is_same<_ARG ,CHAR>::value && !std::is_same<_ARG ,DATA>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("VARY") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG ,VALX>::value && !std::is_same<_ARG ,VAL32>::value && !std::is_same<_ARG ,VAL64>::value>> & ,const decltype (ARGVP2) &) {
		writer << _PCSTR_ ("VALX") ;
	}

	template <class _ARG>
	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("typename '") ;
		writer << _TYPEID_<_ARG> () ;
		writer << _PCSTR_ ("'") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<BOOL> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<VAR32> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<EFLAG> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<VAL32> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<VAL64> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<VOID> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("VOID") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<decltype (NULL)> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("NULL") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<NONE> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("NONE") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<BYTE> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<WORD> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("WORD") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<CHAR> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<DATA> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("DATA") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<MEGA> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<STRA> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("STRA") ;
	}

	inline static void build_typename_ids (TextWriter<STR> &writer ,const ARGV<STRW> & ,const ARGV<VOID> & ,const decltype (ARGVP1) &) {
		writer << _PCSTR_ ("STRW") ;
	}

	template <class _ARG>
	inline static void build_typename_xs (TextWriter<STR> &writer ,const ARGV<_ARG> &) {
		build_typename_cvs (writer ,_NULL_<const ARGV<REMOVE_REFERENCE_TYPE<_ARG>>> ()) ;
		build_typename_ids (writer ,_NULL_<const ARGV<REMOVE_CVR_TYPE<_ARG>>> () ,ARGV_VOID ,ARGVP9) ;
		build_typename_refs (writer ,_NULL_<const ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG>>>> ()) ;
	}

	inline static void invoke (TextWriter<STR> &writer) {
		build_typename_xs (writer ,_NULL_<const ARGV<TYPE>> ()) ;
	}

	inline static String<STR> invoke () {
		return _PRINTS_<STR> (_XVALUE_<const PTR<void (TextWriter<STR> &)> &> (&OPERATOR_TYPENAME::invoke)) ;
	}
} ;
} ;
#endif

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

private:
	class Implement ;

	struct Binder :public Interface {
		virtual void friend_write (TextWriter<STR> &writer) const = 0 ;
	} ;

	template <class TYPE>
	class ImplBinder :public Binder {
	private:
		TYPE mBinder ;

	public:
		template <class... _ARGS>
		inline explicit ImplBinder (_ARGS &&...args) :mBinder (std::forward<_ARGS> (args)...) {}

		inline void friend_write (TextWriter<STR> &writer) const popping override {
			mBinder.friend_write (writer) ;
		}
	} ;

	export struct Abstract :public Interface {
		virtual void modify_option (FLAG option) = 0 ;
		virtual void print (const Binder &msg) = 0 ;
		virtual void fatal (const Binder &msg) = 0 ;
		virtual void error (const Binder &msg) = 0 ;
		virtual void warn (const Binder &msg) = 0 ;
		virtual void info (const Binder &msg) = 0 ;
		virtual void debug (const Binder &msg) = 0 ;
		virtual void verbose (const Binder &msg) = 0 ;
		virtual void enable_log (const String<STR> &path) = 0 ;
		virtual void log (const PhanBuffer<const STR> &tag ,const Binder &msg) = 0 ;
		virtual void show () = 0 ;
		virtual void flash () = 0 ;
		virtual void pause () = 0 ;
		virtual void clear () = 0 ;
		virtual void hide () = 0 ;
	} ;

private:
	friend Singleton<ConsoleService> ;
	Monostate<std::recursive_mutex> mMutex ;
	HolderRef<Abstract> mThis ;

public:
	void modify_option (FLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->modify_option (option) ;
	}

	template <class... _ARGS>
	void print (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->print (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void fatal (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->fatal (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void error (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->error (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void warn (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->warn (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void info (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->info (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void debug (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->debug (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	template <class... _ARGS>
	void verbose (const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->verbose (_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	void enable_log (const String<STR> &path) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->enable_log (path) ;
	}

	template <class... _ARGS>
	void log (const String<STR> &tag ,const _ARGS &...args) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->log (tag.raw () ,_XVALUE_<const Binder &> (ImplBinder<StreamBinder<const _ARGS...>> (args...))) ;
	}

	void show () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->show () ;
	}

	void flash () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->flash () ;
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
	class Implement ;

	export struct Abstract :public Interface {
		virtual void abort_once_invoked_exit (BOOL flag) = 0 ;
		virtual void output_memory_leaks_report (BOOL flag) = 0 ;
		virtual Array<DATA> captrue_stack_trace () popping = 0 ;
		virtual Array<String<STR>> symbol_from_address (const Array<DATA> &address) popping = 0 ;
	} ;

private:
	friend Singleton<DebuggerService> ;
	Monostate<std::recursive_mutex> mMutex ;
	HolderRef<Abstract> mThis ;

public:
	void abort_once_invoked_exit (BOOL flag) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->abort_once_invoked_exit (flag) ;
	}

	void output_memory_leaks_report (BOOL flag) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->output_memory_leaks_report (flag) ;
	}

	Array<DATA> captrue_stack_trace () popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->captrue_stack_trace () ;
	}

	Array<String<STR>> symbol_from_address (const Array<DATA> &address) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->symbol_from_address (address) ;
	}

private:
	DebuggerService () ;
} ;
} ;