#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
class TimePoint ;

class Duration final
	:private Proxy {
private:
	friend TimePoint ;
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	Duration () = default ;

	explicit Duration (const LENGTH &milliseconds_) ;

	explicit Duration (const ARRAY6<LENGTH> &time_) ;

	explicit Duration (StrongRef<Implement> &&this_) ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	LENGTH hours () const ;

	LENGTH minutes () const ;

	LENGTH seconds () const ;

	LENGTH miliseconds () const ;

	LENGTH microseconds () const ;

	LENGTH nanoseconds () const ;

	Duration add (const Duration &that) const ;

	inline Duration operator+ (const Duration &that) const {
		return add (that) ;
	}

	Duration sub (const Duration &that) const ;

	inline Duration operator- (const Duration &that) const {
		return sub (that) ;
	}
} ;

class TimePoint final
	:private Proxy {
private:
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	TimePoint () = default ;

	explicit TimePoint (const ARRAY8<LENGTH> &time_) ;

	explicit TimePoint (StrongRef<Implement> &&this_) ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	ARRAY8<LENGTH> calendar () const ;

	TimePoint add (const Duration &that) const ;

	inline TimePoint operator+ (const Duration &that) const {
		return add (that) ;
	}

	Duration sub (const TimePoint &that) const ;

	inline Duration operator- (const TimePoint &that) const {
		return sub (that) ;
	}
} ;

class Atomic final
	:private Proxy {
private:
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	Atomic () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	VAR fetch () const ;

	inline implicit operator VAR () const {
		return fetch () ;
	}

	VAR compare_exchange (const VAR &expect ,const VAR &data) popping ;

	void store (const VAR &data) ;

	inline void operator= (const VAR &data) {
		store (data) ;
	}

	VAR increase () popping ;

	inline VAR operator++ () popping {
		return increase () ;
	}

	VAR decrease () popping ;

	inline VAR operator-- () popping {
		return decrease () ;
	}
} ;

class Mutex final
	:private Proxy {
private:
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	Mutex () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	void lock () ;

	BOOL try_lock () popping ;

	void unlock () ;
} ;

class RecursiveMutex final
	:private Proxy {
private:
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	RecursiveMutex () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	void lock () ;

	BOOL try_lock () popping ;

	void unlock () ;
} ;

class UniqueLock ;

class ConditionLock final
	:private Proxy {
private:
	friend UniqueLock ;
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	ConditionLock () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	template <class _RET = NONE>
	DEPENDENT_TYPE<UniqueLock ,_RET> watch (Mutex &mutex_) popping {
		struct Dependent ;
		return DEPENDENT_TYPE<UniqueLock ,Dependent> (mutex_ ,DEREF[this]) ;
	}
} ;

class UniqueLock final
	:private Proxy {
private:
	friend ConditionLock ;
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	UniqueLock () = delete ;

	void wait () const ;

	void wait (const Duration &time_) const ;

	void wait (const TimePoint &time_) const ;

	void yield () const ;

	void notify () const ;

private:
	explicit UniqueLock (Mutex &mutex_ ,ConditionLock &condition) ;
} ;

class Thread final
	:private Proxy {
public:
	exports class Binder
		:public Interface {
	public:
		virtual void execute () = 0 ;
	} ;

private:
	struct Detail ;
	class Implement ;
	StrongRef<Implement> mThis ;

public:
	Thread () = delete ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	explicit Thread (StrongRef<Binder> &&runnable) ;

	void join () ;
} ;

struct Thread::Detail {
	class Runnable {
	private:
		PhanRef<Binder> mBinder ;

	public:
		inline Runnable () = delete ;

		inline explicit Runnable (const PhanRef<Binder> &binder) {
			mBinder = PhanRef<Binder>::make (binder) ;
		}

		inline void operator() () {
			mBinder->execute () ;
		}
	} ;
} ;

class GlobalRuntime final
	:private Wrapped<void> {
public:
	inline imports_static TimePoint clock_now () ;

	inline imports_static TimePoint clock_epoch () ;

	inline imports_static FLAG thread_tid () ;

	inline imports_static void thread_sleep (const Duration &time_) ;

	inline imports_static void thread_sleep (const TimePoint &time_) ;

	inline imports_static void thread_yield () ;

	inline imports_static LENGTH thread_concurrency () ;

	inline imports_static void thread_fence () ;

	inline imports_static void locale_init (const Plain<STRA> &locale_) ;

	inline imports_static FLAG process_pid () ;

	inline imports_static Buffer<BYTE ,ARGC<128>> process_info (const FLAG &pid) ;

	inline imports_static FLAG process_info_pid (const PhanBuffer<const STRU8> &info) ;

	inline imports_static void process_exit[[noreturn]] () ;

	inline imports_static void process_abort[[noreturn]] () ;

	inline imports_static FLAG system_exec (const String<STR> &cmd) popping ;
} ;

namespace U {
struct OPERATOR_TYPENAME {
	struct TYPENAME {
		String<STR> mName ;
	} ;

#ifdef __CSC_COMPILER_MSVC__
	template <class _RET>
	inline static TYPENAME typeid_name_from_func () {
		static constexpr auto M_PREFIX = _PCSTR_ ("struct CSC::U::OPERATOR_TYPENAME::TYPENAME __cdecl CSC::U::OPERATOR_TYPENAME::typeid_name_from_func<") ;
		static constexpr auto M_SUFFIX = _PCSTR_ (">(void)") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (M_FUNC)) ;
		const auto r1x = M_PREFIX.size () ;
		const auto r2x = M_SUFFIX.size () ;
		const auto r3x = ret.mName.length () - r1x - r2x ;
		_DYNAMIC_ASSERT_ (r3x > 0) ;
		ret.mName = ret.mName.segment (r1x ,r3x) ;
		return _MOVE_ (ret) ;
	}
#endif

#ifdef __CSC_COMPILER_GNUC__
	template <class _RET>
	inline static TYPENAME typeid_name_from_func () {
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [with _RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (M_FUNC)) ;
		const auto r4x = M_PREFIX.size () ;
		const auto r5x = M_SUFFIX.size () ;
		const auto r6x = ret.mName.length () - r4x - r5x ;
		_DYNAMIC_ASSERT_ (r6x > 0) ;
		ret.mName = ret.mName.segment (r4x ,r6x) ;
		return _MOVE_ (ret) ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
	template <class _RET>
	inline static TYPENAME typeid_name_from_func () {
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [_RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (M_FUNC)) ;
		const auto r7x = M_PREFIX.size () ;
		const auto r8x = M_SUFFIX.size () ;
		const auto r9x = ret.mName.length () - r7x - r8x ;
		_DYNAMIC_ASSERT_ (r9x > 0) ;
		ret.mName = ret.mName.segment (r7x ,r9x) ;
		return _MOVE_ (ret) ;
}
#endif

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<const _ARG1> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<volatile _ARG1> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<const volatile _ARG1> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1 &> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1 &&> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP8)> &) {
		writer << _PCSTR_ ("PTR<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_POINTER_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_member_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP7)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_MEMPTR_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" ") ;
		template_write_typename_x (writer ,_NULL_<ARGV<MEMPTR_CLASS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_arr (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,ARR<REMOVE_ARRAY_TYPE<_ARG1>>>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("ARR<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_arr (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_bounded_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_array<_ARG1>::value>> & ,const DEF<decltype (ARGVP6)> &) {
		template_write_typename_arr (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_func (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<VOID> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_function<_ARG1>::value>> & ,const DEF<decltype (ARGVP5)> &) {
		template_write_typename_func (writer ,_NULL_<ARGV<REMOVE_FUNCATTR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_claz (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<!stl::is_template<_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_claz (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_template<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("template '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'<") ;
		template_write_typename_y (writer ,_NULL_<ARGV<TEMPLATE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_class<_ARG1>::value>> & ,const DEF<decltype (ARGVP4)> &) {
		template_write_typename_claz (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,STRU8>::value && !stl::is_same<_ARG1 ,BYTE>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,STRU16>::value && !stl::is_same<_ARG1 ,WORD>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,STRU32>::value && !stl::is_same<_ARG1 ,CHAR>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,VARX>::value && !stl::is_same<_ARG1 ,VAR32>::value && !stl::is_same<_ARG1 ,VAR64>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VARX") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,VARY>::value && !stl::is_same<_ARG1 ,CHAR>::value && !stl::is_same<_ARG1 ,DATA>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VARY") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_same<_ARG1 ,VALX>::value && !stl::is_same<_ARG1 ,VAL32>::value && !stl::is_same<_ARG1 ,VAL64>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VALX") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BOOL> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAR32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<EFLAG> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL64> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VOID> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VOID") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<NONE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("NONE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<decltype (NULL)> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("NULL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BYTE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<WORD> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("WORD") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<CHAR> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<DATA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("DATA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<MEGA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRW> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRW") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("typename '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_x (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		template_write_typename_cv (writer ,_NULL_<ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>> ()) ;
		template_write_typename_id (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
		template_write_typename_ref (writer ,_NULL_<ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>> ()) ;
	}

	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1>> &) {
		template_write_typename_x (writer ,_NULL_<ARGV<_ARG1>> ()) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		template_write_typename_x (writer ,_NULL_<ARGV<ONE_HINT>> ()) ;
		writer << _PCSTR_ (" ,") ;
		template_write_typename_y (writer ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	template <class _RET>
	inline static String<STR> invoke () {
		const auto r1x = PTR<void (TextWriter<STR> &)> ([] (TextWriter<STR> &writer) {
			template_write_typename_x (writer ,_NULL_<ARGV<_RET>> ()) ;
		}) ;
		return String<STR>::make (r1x) ;
	}
} ;
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> final
	:private Wrapped<void> {
public:
	struct Extern {
		//@warn: this function should be implemented in a 'runtime.dll'
		imports_static DEF<PTR<NONE> (const PTR<NONE> & ,const PTR<NONE> &) popping> unique_atomic_address ;
	} ;

private:
	struct VALUE_NODE {
		FLAG mGUID ;
		BOOL mReadOnly ;
		VAR mValue ;
	} ;

	struct CLASS_NODE {
		String<STR> mGUID ;
		PTR<NONE> mValue ;
	} ;

	class Pack {
	private:
		template <class>
		friend class GlobalStatic ;
		Atomic mCounter ;
		Mutex mNodeMutex ;
		Deque<VALUE_NODE> mValueList ;
		HashSet<FLAG> mValueMappingSet ;
		Deque<CLASS_NODE> mClassList ;
		HashSet<FLAG> mClassMappingSet ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Pack ,GlobalStatic> ;

private:
	imports_static PTR<NONE> unique_atomic_address (const PTR<NONE> &expect ,const PTR<NONE> &data) popping {
		return Extern::unique_atomic_address (expect ,data) ;
	}

	imports_static Pack &static_unique () popping {
		return _CACHE_ ([&] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Pack ,GlobalStatic> () ;
			if switch_case (TRUE) {
				if (rax != NULL)
					discard ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack ,GlobalStatic>::make () ;
				const auto r1x = rbx.watch () ;
				auto &r2x = _FORWARD_<Pack &> (r1x) ;
				auto &r3x = _LOAD_<NONE> (DEPTR[r2x]) ;
				rax = unique_atomic_address (NULL ,DEPTR[r3x]) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			auto &r4x = _LOAD_<Pack> (rax) ;
			auto rcx = IntrusiveRef<Pack ,GlobalStatic> (DEPTR[r4x]) ;
			return rcx.watch () ;
		}) ;
	}

	imports_static PTR<VALUE_NODE> static_new_node (Pack &self_ ,const FLAG &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mValueList.insert () ;
			self_.mValueMappingSet.add (r1x ,ix) ;
			self_.mValueList[ix].mGUID = guid ;
		}
		return DEPTR[self_.mValueList[ix]] ;
	}

	imports_static FLAG node_guid_hash (const FLAG &guid) {
		return guid ;
	}

	imports_static PTR<VALUE_NODE> static_find_node (Pack &self_ ,const FLAG &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[self_.mValueList[ix]] ;
	}

	imports_static PTR<CLASS_NODE> static_new_node (Pack &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mClassList.insert () ;
			self_.mClassMappingSet.add (r1x ,ix) ;
			self_.mClassList[ix].mGUID = guid ;
		}
		return DEPTR[self_.mClassList[ix]] ;
	}

	imports_static FLAG node_guid_hash (const String<STR> &guid) {
		const auto r1x = guid.raw () ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		return BasicProc::mem_hash (r2x.self ,r2x.size ()) ;
	}

	imports_static PTR<CLASS_NODE> static_find_node (Pack &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[self_.mClassList[ix]] ;
	}

private:
	static void friend_create (Pack &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mCounter = 0 ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	static void friend_destroy (Pack &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class GUID>
class GlobalStatic final
	:private Wrapped<void> {
	_STATIC_ASSERT_ (GUID::value > 0) ;

public:
	imports_static void init (const VAR &data) {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		if (r2x != NULL)
			return ;
		const auto r3x = GlobalStatic<void>::static_new_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r3x != NULL) ;
		r3x->mReadOnly = TRUE ;
		r3x->mValue = data ;
	}

	imports_static VAR fetch () popping {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return r2x->mValue ;
	}

	imports_static VAR compare_exchange (const VAR &expect ,const VAR &data) popping {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		_DYNAMIC_ASSERT_ (!r2x->mReadOnly) ;
		if (r2x->mValue == expect)
			r2x->mValue = data ;
		return r2x->mValue ;
	}

	imports_static void store (const VAR &data) {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		if switch_case (TRUE) {
			if (rax != NULL)
				discard ;
			rax = GlobalStatic<void>::static_new_node (r1x ,GUID::value) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mValue = data ;
	}
} ;

template <class UNIT>
class GlobalStatic<Singleton<UNIT>> final
	:private Wrapped<void> {
private:
	class Pack {
	private:
		friend GlobalStatic ;
		Atomic mCounter ;
		Singleton<UNIT> mValue ;
	} ;

private:
	friend IntrusiveRef<Pack ,GlobalStatic> ;

public:
	static Singleton<UNIT> &unique () popping {
		auto &r1x = _CACHE_ ([&] () {
			auto &r2x = GlobalStatic<void>::static_unique () ;
			ScopedGuard<Mutex> ANONYMOUS (r2x.mNodeMutex) ;
			const auto r3x = U::OPERATOR_TYPENAME::invoke<Singleton<UNIT>> () ;
			auto rax = GlobalStatic<void>::static_find_node (r2x ,r3x) ;
			auto rbx = IntrusiveRef<Pack ,GlobalStatic> () ;
			if switch_case (TRUE) {
				if (rax != NULL)
					discard ;
				rax = GlobalStatic<void>::static_new_node (r2x ,r3x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack ,GlobalStatic>::make () ;
				const auto r4x = rbx.watch () ;
				auto &r5x = _FORWARD_<Pack &> (r4x) ;
				auto &r6x = _LOAD_<NONE> (DEPTR[r5x]) ;
				rax->mValue = DEPTR[r6x] ;
			}
			auto &r7x = _LOAD_<Pack> (rax->mValue) ;
			auto rcx = IntrusiveRef<Pack ,GlobalStatic> (DEPTR[r7x]) ;
			return rcx.watch () ;
		}) ;
		auto &r8x = _FORWARD_<Pack &> (r1x) ;
		return r8x.mValue ;
	}

private:
	static void friend_create (Pack &self_) {
		self_.mCounter = 0 ;
	}

	static void friend_destroy (Pack &self_) {
		_STATIC_WARNING_ ("noop") ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

#ifdef __CSC_DEPRECATED__
template <class CONT>
class Coroutine {
public:
	Coroutine () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif

class RandomService final
	:private Proxy {
private:
	exports class Abstract
		:public Interface {
	public:
		virtual VAR entropy () const = 0 ;
		virtual void reset_seed (const VAR &seed_) = 0 ;
		virtual VAR random_value () popping = 0 ;
		virtual void random_skip (const LENGTH &len) = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<RandomService> ;
	Monostate<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	VAR entropy () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->entropy () ;
	}

	void reset_seed (const VAR &seed_) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		mThis->reset_seed (seed_) ;
	}

	VAR random_value (const VAR &min_ ,const VAR &max_) popping {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (min_ <= max_) ;
		const auto r1x = max_ - min_ + 1 ;
		const auto r2x = mThis->random_value () ;
		return r2x % r1x + min_ ;
	}

	Array<VAR> random_value (const VAR &min_ ,const VAR &max_ ,const LENGTH &len) popping {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (min_ <= max_) ;
		Array<VAR> ret = Array<VAR> (len) ;
		const auto r1x = max_ - min_ + 1 ;
		for (auto &&i : _RANGE_ (0 ,ret.length ())) {
			const auto r2x = mThis->random_value () ;
			ret[i] = r2x % r1x + min_ ;
		}
		return _MOVE_ (ret) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_) popping {
		return random_shuffle (count ,range_ ,BitSet<> (range_)) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_ ,BitSet<> &&res) popping {
		_DEBUG_ASSERT_ (count >= 0 && count < range_) ;
		_DEBUG_ASSERT_ (res.size () == range_) ;
		BitSet<> ret = _MOVE_ (res) ;
		ret.clear () ;
		while (TRUE) {
			if (ret.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range_ - 1)) ;
			ret[ix] = TRUE ;
		}
		return _MOVE_ (ret) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const BitSet<> &range_) popping {
		BitSet<> ret = BitSet<> (range_.size ()) ;
		compute_random_shuffle (count ,range_ ,ret) ;
		return _MOVE_ (ret) ;
	}

	void compute_random_shuffle (const LENGTH &count ,const BitSet<> &range_ ,BitSet<> &chosen) {
		_DEBUG_ASSERT_ (count >= 0 && count < range_.size ()) ;
		_DEBUG_ASSERT_ (chosen.size () == range_.size ()) ;
		chosen.clear () ;
		while (TRUE) {
			if (chosen.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range_.size () - 1)) ;
			chosen[ix] = BOOL (range_[ix]) ;
		}
	}

	String<STR> random_uuid () popping {
		static constexpr auto M_UUID = _PCSTR_ ("00000000-0000-0000-000000000000") ;
		String<STR> ret = String<STR> (M_UUID.size ()) ;
		INDEX iw = 0 ;
		const auto r1x = random_value (0 ,36 ,28) ;
		for (auto &&i : _RANGE_ (0 ,8)) {
			INDEX ix = 0 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 8 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 12 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,12)) {
			INDEX ix = 16 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		if (iw < ret.size ())
			ret[iw] = 0 ;
		return _MOVE_ (ret) ;
	}

	void random_skip (const LENGTH &len) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->random_skip (len) ;
	}

private:
	RandomService () ;

private:
	inline static STRU8 index_to_hex_str (const INDEX &index) {
		if (index < 10)
			return STRU8 (STRU8 ('0') + index) ;
		return STRU8 (STRU8 ('A' - 10) + index) ;
	}
} ;
} ;