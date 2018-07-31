#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"
#include "csc_databases.hpp"

namespace CSC {
template <class TYPE>
struct OPERATOR_TYPENAME {
	template <class _ARG>
	inline void build_typename_cvs (TextWriter<STRU8> &writer ,const ARGV<_ARG> &) {}

	template <class _ARG>
	inline void build_typename_cvs (TextWriter<STRU8> &writer ,const ARGV<const _ARG> &) {
		writer << _PCSTRU8_ ("const ") ;
	}

	template <class _ARG>
	inline void build_typename_cvs (TextWriter<STRU8> &writer ,const ARGV<volatile _ARG> &) {
		writer << _PCSTRU8_ ("volatile ") ;
	}

	template <class _ARG>
	inline void build_typename_cvs (TextWriter<STRU8> &writer ,const ARGV<const volatile _ARG> &) {
		writer << _PCSTRU8_ ("const volatile ") ;
	}

	template <class _ARG>
	inline void build_typename_refs (TextWriter<STRU8> &writer ,const ARGV<_ARG> &) {}

	template <class _ARG>
	inline void build_typename_refs (TextWriter<STRU8> &writer ,const ARGV<_ARG &> &) {
		writer << _PCSTRU8_ (" &") ;
	}

	template <class _ARG>
	inline void build_typename_refs (TextWriter<STRU8> &writer ,const ARGV<_ARG &&> &) {
		writer << _PCSTRU8_ (" &&") ;
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_pointer<_ARG>::value>> & ,const ARGV<ARGC<6>> &) {
		writer << _PCSTRU8_ ("PTR<") ;
		build_typename_s (writer ,_NULL_<ARGV<REMOVE_POINTER_TYPE<REMOVE_CVR_TYPE<_ARG>>>> ()) ;
		writer << _PCSTRU8_ (">") ;
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_member_pointer<_ARG>::value>> & ,const ARGV<ARGC<5>> &) {
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable: 4127) //@info:warning C4127: 条件表达式是常量
#endif
		writer << _PCSTRU8_ ("DEF<") ;
		if (std::is_function<REMOVE_CVR_TYPE<REMOVE_MEMBER_POINTER_TYPE<_ARG>>>::value)
			writer << _PCSTRU8_ ("DEF<") ;
		build_typename_s (writer ,_NULL_<ARGV<REMOVE_MEMBER_POINTER_TYPE<_ARG>>> ()) ;
		if (std::is_function<REMOVE_CVR_TYPE<REMOVE_MEMBER_POINTER_TYPE<_ARG>>>::value)
			writer << _PCSTRU8_ (">") ;
		writer << _PCSTRU8_ (" ") ;
		build_typename_s (writer ,_NULL_<ARGV<MEMBEROF_TRAITS_TYPE<_ARG>>> ()) ;
		writer << _PCSTRU8_ ("::*>") ;
#pragma warning (pop)
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_same<REMOVE_CVR_TYPE<_ARG> ,ARR<REMOVE_EXTEND_TYPE<REMOVE_CVR_TYPE<_ARG>>>>::value>> & ,const ARGV<ARGC<4>> &) {
		writer << _PCSTRU8_ ("ARR<") ;
		build_typename_s (writer ,_NULL_<ARGV<REMOVE_EXTEND_TYPE<REMOVE_CVR_TYPE<_ARG>>>> ()) ;
		writer << _PCSTRU8_ (">") ;
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_array<REMOVE_CVR_TYPE<_ARG>>::value>> & ,const ARGV<ARGC<3>> &) {
		writer << _PCSTRU8_ ("DEF<") ;
		build_typename_s (writer ,_NULL_<ARGV<REMOVE_EXTEND_TYPE<REMOVE_CVR_TYPE<_ARG>>>> ()) ;
		writer << _PCSTRU8_ ("[") ;
		writer << COUNTOF_TRAITS_TYPE<_ARG>::value ;
		writer << _PCSTRU8_ ("]>") ;
	}

	inline void build_typename_params (TextWriter<STRU8> &writer ,const ARGVS<> &) {}

	template <class _ARG>
	inline void build_typename_params (TextWriter<STRU8> &writer ,const ARGVS<_ARG> &) {
		build_typename_s (writer ,_NULL_<ARGV<_ARG>> ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline void build_typename_params (TextWriter<STRU8> &writer ,const ARGVS<_ARG1 ,_ARGS...> &) {
		build_typename_s (writer ,_NULL_<ARGV<_ARG1>> ()) ;
		writer << _PCSTRU8_ (" ,") ;
		build_typename_params (writer ,_NULL_<ARGVS<_ARGS...>> ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline void build_typename_funcs (TextWriter<STRU8> &writer ,const ARGV<_ARG1 (_ARGS...)> &) {
		build_typename_s (writer ,_NULL_<ARGV<_ARG1>> ()) ;
		writer << _PCSTRU8_ (" (") ;
		build_typename_params (writer ,_NULL_<ARGVS<_ARGS...>> ()) ;
		writer << _PCSTRU8_ (")") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline void build_typename_funcs (TextWriter<STRU8> &writer ,const ARGV<_ARG1 (_ARGS...) const> &) {
		build_typename_s (writer ,_NULL_<ARGV<_ARG1>> ()) ;
		writer << _PCSTRU8_ (" (") ;
		build_typename_params (writer ,_NULL_<ARGVS<_ARGS...>> ()) ;
		writer << _PCSTRU8_ (") const") ;
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_function<REMOVE_CVR_TYPE<_ARG>>::value>> & ,const ARGV<ARGC<2>> &) {
		build_typename_funcs (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<_ARG>>> ()) ;
	}

	template <class _ARG>
	inline void build_typename_s (TextWriter<STRU8> &writer ,const ARGV<_ARG> & ,const ARGV<VOID> & ,const ARGV<ARGC<1>> &) {
		const auto r1x = &_LOAD_<ARR<STRA>> (typeid (REMOVE_CVR_TYPE<_ARG>).name ()) ;
		writer << String<STRU8> (_LOAD_<ARR<STRU8>> (r1x)) ;
	}

	inline void invoke (TextWriter<STRU8> &writer) {
		build_typename_cvs (writer ,_NULL_<ARGV<TYPE>> ()) ;
		build_typename_s (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<TYPE>>> () ,ARGV_VOID ,ARGV9) ;
		build_typename_refs (writer ,_NULL_<ARGV<TYPE>> ()) ;
	}

	inline String<STRU8> invoke () {
		String<STRU8> ret = String<STRU8> (LONGSTRING_SIZE::value) ;
		auto writer = TextWriter<STRU8> (ret.raw ()) ;
		invoke (writer) ;
		writer << _EOS_ ;
		return std::move (ret) ;
	}
} ;

class Console {
private:
	class Engine ;

private:
	friend Singleton<Console> ;
	SharedRef<std::recursive_mutex> mMutex ;
	AnyRef<void> mHolder ;

public:
	template <class... _ARGS>
	void print (const _ARGS &...args) ;

	template <class... _ARGS>
	void fatal (const _ARGS &...args) ;

	template <class... _ARGS>
	void error (const _ARGS &...args) ;

	template <class... _ARGS>
	void warn (const _ARGS &...args) ;

	template <class... _ARGS>
	void info (const _ARGS &...args) ;

	template <class... _ARGS>
	void debug (const _ARGS &...args) ;

	template <class... _ARGS>
	void verbose (const _ARGS &...args) ;

	void enable (FLAG level) ;

	void start () ;

	void pause () ;

	void clear () ;

	void stop () ;

private:
	Console () ;

	~Console () noexcept {}
} ;

class Logger {
private:
	class Engine ;

private:
	friend Singleton<Logger> ;
	SharedRef<std::recursive_mutex> mMutex ;
	AnyRef<void> mHolder ;

public:
	template <class... _ARGS>
	void write (const String<STR> &tag ,const _ARGS &...args) ;

private:
	Logger () ;

	~Logger () noexcept {}
} ;

class DebuggerService {
private:
	class Engine ;

private:
	friend Singleton<DebuggerService> ;
	SharedRef<std::recursive_mutex> mMutex ;
	AnyRef<void> mHolder ;

public:
	void abort_once_invoked_exit (BOOL flag) ;

	void check_memory_leaks (BOOL flag) ;

	Array<DATA> captrue_stack_trace () ;

	Array<String<STRU8>> symbol_from_address (const Array<DATA> &adress) ;

private:
	DebuggerService () ;

	~DebuggerService () noexcept {}
} ;
} ;