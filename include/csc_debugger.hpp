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
#ifdef __CSC_DEPRECATED__
namespace U {
struct OPERATOR_TYPENAME {
	struct TYPENAME {
		String<STR> mSelf ;
	} ;

	template <class _RET>
	inline static TYPENAME type_name_from_func () {
		TYPENAME ret ;
		ret.mSelf = _PARSESTRS_ (String<STRA> (M_FUNC)) ;
#ifdef __CSC_COMPILER_MSVC__
		static constexpr auto M_PREFIX = _PCSTR_ ("struct CSC::U::OPERATOR_TYPENAME::TYPENAME __cdecl CSC::U::OPERATOR_TYPENAME::type_name_from_func<") ;
		static constexpr auto M_SUFFIX = _PCSTR_ (">(void)") ;
		const auto r1x = M_PREFIX.size () ;
		const auto r2x = M_SUFFIX.size () ;
		const auto r3x = ret.mSelf.length () - r1x - r2x ;
		_DYNAMIC_ASSERT_ (r3x > 0) ;
		ret.mSelf = ret.mSelf.segment (r1x ,r3x) ;
#elif defined __CSC_COMPILER_GNUC__
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::type_name_from_func() [with _RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		const auto r4x = M_PREFIX.size () ;
		const auto r5x = M_SUFFIX.size () ;
		const auto r6x = ret.mSelf.length () - r4x - r5x ;
		_DYNAMIC_ASSERT_ (r6x > 0) ;
		ret.mSelf = ret.mSelf.segment (r4x ,r6x) ;
#elif defined __CSC_COMPILER_CLANG__
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::type_name_from_func() [_RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		const auto r7x = M_PREFIX.size () ;
		const auto r8x = M_SUFFIX.size () ;
		const auto r9x = ret.mSelf.length () - r7x - r8x ;
		_DYNAMIC_ASSERT_ (r9x > 0) ;
		ret.mSelf = ret.mSelf.segment (r7x ,r9x) ;
#else
		ret.mSelf = _BUILDVAR64S_ (_TYPEUID_<_RET> ()) ;
#endif
		return std::move (ret) ;
	}

	template <class _ARG1>
	inline static void static_write_typename_cvs (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_cvs (TextWriter<STR> &writer ,const ARGV<const _ARG1> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_cvs (TextWriter<STR> &writer ,const ARGV<volatile _ARG1> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_cvs (TextWriter<STR> &writer ,const ARGV<const volatile _ARG1> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG1 &> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_refs (TextWriter<STR> &writer ,const ARGV<_ARG1 &&> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP8)> &) {
		writer << _PCSTR_ ("PTR<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<REMOVE_POINTER_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_member_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP7)> &) {
		writer << _PCSTR_ ("DEF<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<REMOVE_MEMBER_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" ") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<MEMBER_OF_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_arrs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,ARR<REMOVE_ARRAY_TYPE<_ARG1>>>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("ARR<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_arrs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_full_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_array<_ARG1>::value>> & ,const DEF<decltype (ARGVP5)> &) {
		static_write_typename_arrs (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	inline static void static_write_typename_ys (TextWriter<STR> &writer ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ys (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1>> &) {
		static_write_typename_xs (writer ,_NULL_<ARGV<_ARG1>> ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static void static_write_typename_ys (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1 ,_ARG2 ,_ARGS...>> &) {
		static_write_typename_xs (writer ,_NULL_<ARGV<_ARG1>> ()) ;
		writer << _PCSTR_ (" ,") ;
		static_write_typename_ys (writer ,_NULL_<ARGV<ARGVS<_ARG2 ,_ARGS...>>> ()) ;
	}

	template <class _ARG1>
	inline static void static_write_typename_funcs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,REMOVE_MEMFUNC_TYPE<_ARG1>>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("DEF<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		static_write_typename_ys (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_funcs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<!std::is_same<_ARG1 ,REMOVE_MEMFUNC_TYPE<_ARG1>>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		static_write_typename_xs (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		static_write_typename_ys (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (") MF>") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_function<_ARG1>::value>> & ,const DEF<decltype (ARGVP4)> &) {
		static_write_typename_funcs (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void static_write_typename_clazs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<!stl::is_template_type<_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		const auto r1x = type_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mSelf ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_clazs (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_template_type<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = type_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mSelf ;
		writer << _PCSTR_ ("'<") ;
		static_write_typename_ys (writer ,_NULL_<ARGV<TEMPLATE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_class<_ARG1>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		static_write_typename_clazs (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU8>::value && !std::is_same<_ARG1 ,BYTE>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU16>::value && !std::is_same<_ARG1 ,WORD>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU32>::value && !std::is_same<_ARG1 ,CHAR>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARX>::value && !std::is_same<_ARG1 ,VAR32>::value && !std::is_same<_ARG1 ,VAR64>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARX") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARY>::value && !std::is_same<_ARG1 ,CHAR>::value && !std::is_same<_ARG1 ,DATA>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARY") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VALX>::value && !std::is_same<_ARG1 ,VAL32>::value && !std::is_same<_ARG1 ,VAL64>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VALX") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("typename '") ;
		writer << _TYPEUID_<_ARG1> () ;
		writer << _PCSTR_ ("'") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<BOOL> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<VAR32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<EFLAG> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<VAL32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<VAL64> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<VOID> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VOID") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<NONE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("NONE") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<decltype (NULL)> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("NULL") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<BYTE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<WORD> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("WORD") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<CHAR> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<DATA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DATA") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<MEGA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<STRA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("STRA") ;
	}

	inline static void static_write_typename_ids (TextWriter<STR> &writer ,const ARGV<STRW> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("STRW") ;
	}

	template <class _ARG1>
	inline static void static_write_typename_xs (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		static_write_typename_cvs (writer ,_NULL_<ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>> ()) ;
		static_write_typename_ids (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
		static_write_typename_refs (writer ,_NULL_<ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>> ()) ;
	}

	template <class _RET>
	inline static String<STR> invoke () {
		const auto r1x = _XVALUE_<PTR<void (TextWriter<STR> &)>> ([] (TextWriter<STR> &writer) {
			static_write_typename_xs (writer ,_NULL_<ARGV<_RET>> ()) ;
		}) ;
		return String<STR>::make (r1x) ;
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
	static constexpr auto OPTION_ALWAYS_FLUSH = FLAG (0X00000080) ;
	static constexpr auto OPTION_TESTING = FLAG (0X00000100) ;

private:
	struct Binder :public Interface {
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
		virtual void modify_option (FLAG option) = 0 ;
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
		virtual void flash () = 0 ;
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

	void modify_option (FLAG option) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->modify_option (option) ;
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