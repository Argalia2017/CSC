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
class GlobalRuntime ;

class Duration
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	friend TimePoint ;
	StrongRef<Implement> mThis ;

public:
	implicit Duration () = default ;

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

class TimePoint
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	friend GlobalRuntime ;
	StrongRef<Implement> mThis ;

public:
	implicit TimePoint () = default ;

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

class Atomic
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
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

	VAR compare_exchange (const VAR &expect ,const VAR &data) side_effects ;

	void store (const VAR &data) ;

	inline void operator= (const VAR &data) {
		store (data) ;
	}

	VAR increase () side_effects ;

	inline VAR operator++ () side_effects {
		return increase () ;
	}

	VAR decrease () side_effects ;

	inline VAR operator-- () side_effects {
		return decrease () ;
	}
} ;

class Mutex
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	Mutex () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	void lock () ;

	BOOL try_lock () side_effects ;

	void unlock () ;
} ;

class RecursiveMutex
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	RecursiveMutex () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	void lock () ;

	BOOL try_lock () side_effects ;

	void unlock () ;
} ;

class UniqueLock ;

class ConditionLock
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	ConditionLock () ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	template <class _RET = REMOVE_CVR_TYPE<UniqueLock>>
	_RET watch (Mutex &mutex_) side_effects {
		struct Dependent ;
		using UniqueLock = DEPENDENT_TYPE<UniqueLock ,Dependent> ;
		return UniqueLock (mutex_ ,DEREF[this]) ;
	}
} ;

class UniqueLock
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	implicit UniqueLock () = delete ;

	explicit UniqueLock (Mutex &mutex_ ,ConditionLock &condition) ;

	void wait () const ;

	void wait (const Duration &time_) const ;

	void wait (const TimePoint &time_) const ;

	void yield () const ;

	void notify () const ;
} ;

class Thread
	:private Proxy {
public:
	class Binder
		:public Interface {
	public:
		virtual void execute () = 0 ;
	} ;

private:
	struct Private {
		class Runnable ;

		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	implicit Thread () = delete ;

	explicit Thread (StrongRef<Binder> &&runnable) ;

	Implement &native () const leftvalue {
		return mThis ;
	}

	void join () ;
} ;

class Thread::Private::Runnable {
private:
	PhanRef<Binder> mBinder ;

public:
	implicit Runnable () = delete ;

	explicit Runnable (PhanRef<Binder> &&binder) {
		mBinder = _MOVE_ (binder) ;
	}

	inline void operator() () {
		mBinder->execute () ;
	}
} ;

class GlobalRuntime
	:private Wrapped<void> {
public:
	imports TimePoint clock_now () ;

	imports TimePoint clock_epoch () ;

	imports FLAG thread_tid () ;

	imports void thread_sleep (const Duration &time_) ;

	imports void thread_sleep (const TimePoint &time_) ;

	imports void thread_yield () ;

	imports LENGTH thread_concurrency () ;

	imports void thread_fence () ;

	imports void locale_init (const Plain<STRA> &locale_) ;

	imports FLAG process_pid () ;

	imports Buffer<BYTE ,ARGC<128>> process_info (const FLAG &pid) ;

	imports FLAG process_info_pid (const PhanBuffer<const STRU8> &info) ;

	imports void process_exit[[noreturn]] () ;

	imports void process_abort[[noreturn]] () ;

	imports FLAG system_exec (const String<STR> &cmd) side_effects ;
} ;

namespace U {
struct OPERATOR_TYPENAME {
	struct TYPENAME {
		String<STR> mName ;
	} ;

#ifdef __CSC_COMPILER_MSVC__
	template <class _RET>
	imports TYPENAME typeid_name_from_func () {
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
	imports TYPENAME typeid_name_from_func () {
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [with _RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
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

#ifdef __CSC_COMPILER_CLANG__
	template <class _RET>
	imports TYPENAME typeid_name_from_func () {
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [_RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
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

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_const<_ARG1>::value && stl::is_volatile<_ARG1>::value)>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP4)> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_const<_ARG1>::value)>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_volatile<_ARG1>::value)>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG1>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_lvalue_reference<_ARG1>::value)>>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_rvalue_reference<_ARG1>::value)>>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG1>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_pointer<_ARG1>::value)>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP8)> &) {
		writer << _PCSTR_ ("PTR<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_POINTER_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_member_pointer<_ARG1>::value)>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP7)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_MEMPTR_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (" ") ;
		template_write_typename_x (writer ,ARGV<MEMPTR_CLASS_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_same<_ARG1 ,ARR<REMOVE_ARRAY_TYPE<_ARG1>>>::value)>>
	imports void template_write_typename_arr (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("ARR<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_ARRAY_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_bounded_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value)>>
	imports void template_write_typename_arr (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_ARRAY_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_array<_ARG1>::value)>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP6)> &) {
		template_write_typename_arr (writer ,ARGV<_ARG1>::null ,ARGVPX) ;
	}

	template <class _ARG1>
	imports void template_write_typename_func (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<INVOKE_RESULT_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,ARGV<INVOKE_PARAMS_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_function<_ARG1>::value)>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP5)> &) {
		template_write_typename_func (writer ,ARGV<REMOVE_FUNCATTR_TYPE<_ARG1>>::null ,ARGVPX) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(!stl::is_template<_ARG1>::value)>>
	imports void template_write_typename_claz (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_template<_ARG1>::value)>>
	imports void template_write_typename_claz (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("template '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'<") ;
		template_write_typename_y (writer ,ARGV<TEMPLATE_PARAMS_TYPE<_ARG1>>::null) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(stl::is_class<_ARG1>::value)>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP4)> &) {
		template_write_typename_claz (writer ,ARGV<_ARG1>::null ,ARGVPX) ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<BOOL> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VAR32> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<EFLAG> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VAL32> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VAL64> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VOID> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VOID") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<NONE> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("NONE") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<decltype (NULL)> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("NULL") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<BYTE> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<WORD> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("WORD") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<CHAR> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<DATA> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("DATA") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<MEGA> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<STRA> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRA") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<STRW> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRW") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<STRU8> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<STRU16> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<STRU32> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VARX> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARX") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VARY> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARY") ;
	}

	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<VALX> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VALX") ;
	}

	template <class _ARG1>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("typename '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	imports void template_write_typename_x (TextWriter<STR> &writer ,const ARGVF<_ARG1> &) {
		template_write_typename_cv (writer ,ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>::null ,ARGVPX) ;
		template_write_typename_id (writer ,ARGV<REMOVE_CVR_TYPE<_ARG1>>::null ,ARGVPX) ;
		template_write_typename_ref (writer ,ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>::null ,ARGVPX) ;
	}

	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<ARGVS<_ARG1>> &) {
		template_write_typename_x (writer ,ARGV<_ARG1>::null) ;
	}

	template <class _ARG1>
	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		template_write_typename_x (writer ,ARGV<ONE_HINT>::null) ;
		writer << _PCSTR_ (" ,") ;
		template_write_typename_y (writer ,ARGV<REST_HINT>::null) ;
	}

	template <class _ARG1>
	imports String<STR> invoke (const ARGVF<_ARG1> &) {
		const auto r1x = Function<void (TextWriter<STR> &)> ([] (TextWriter<STR> &writer) {
			template_write_typename_x (writer ,ARGV<_ARG1>::null) ;
		}) ;
		return String<STR>::make (r1x) ;
	}
} ;
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void>
	:private Wrapped<void> {
public:
	struct Public {
		//@warn: this function should be implemented in a 'runtime.dll'
		imports DEF<PTR<NONE> (const PTR<NONE> & ,const PTR<NONE> &) side_effects> unique_atomic_address ;
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

	struct SELF_PACK {
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
	friend IntrusiveRef<SELF_PACK ,GlobalStatic> ;

private:
	imports SELF_PACK &static_unique () side_effects {
		return _CACHE_ ([&] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = Public::unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<SELF_PACK ,GlobalStatic> () ;
			if switch_once (TRUE) {
				if (rax != NULL)
					discard ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<SELF_PACK ,GlobalStatic>::make () ;
				const auto r1x = rbx.watch () ;
				auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
				auto &r3x = _LOAD_ (ARGV<NONE>::null ,DEPTR[r2x]) ;
				rax = Public::unique_atomic_address (NULL ,DEPTR[r3x]) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			auto &r4x = _LOAD_ (ARGV<SELF_PACK>::null ,rax) ;
			auto rcx = IntrusiveRef<SELF_PACK ,GlobalStatic> (DEPTR[r4x]) ;
			return rcx.watch () ;
		}) ;
	}

	imports PTR<VALUE_NODE> static_new_node (SELF_PACK &self_ ,const FLAG &guid) side_effects {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mValueList.insert () ;
			self_.mValueMappingSet.add (r1x ,ix) ;
			self_.mValueList[ix].mGUID = guid ;
		}
		return DEPTR[self_.mValueList[ix]] ;
	}

	imports FLAG node_guid_hash (const FLAG &guid) {
		return guid ;
	}

	imports PTR<VALUE_NODE> static_find_node (SELF_PACK &self_ ,const FLAG &guid) side_effects {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[self_.mValueList[ix]] ;
	}

	imports PTR<CLASS_NODE> static_new_node (SELF_PACK &self_ ,const String<STR> &guid) side_effects {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mClassList.insert () ;
			self_.mClassMappingSet.add (r1x ,ix) ;
			self_.mClassList[ix].mGUID = guid ;
		}
		return DEPTR[self_.mClassList[ix]] ;
	}

	imports FLAG node_guid_hash (const String<STR> &guid) {
		const auto r1x = guid.raw () ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		return BasicProc::mem_hash (r2x.self ,r2x.size ()) ;
	}

	imports PTR<CLASS_NODE> static_find_node (SELF_PACK &self_ ,const String<STR> &guid) side_effects {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[self_.mClassList[ix]] ;
	}

private:
	imports void friend_create (SELF_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mCounter = 0 ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	imports void friend_destroy (SELF_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	imports LENGTH friend_attach (SELF_PACK &self_) side_effects {
		return ++self_.mCounter ;
	}

	imports LENGTH friend_detach (SELF_PACK &self_) side_effects {
		return --self_.mCounter ;
	}

	imports void friend_latch (SELF_PACK &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class GUID>
class GlobalStatic
	:private Wrapped<void> {
	_STATIC_ASSERT_ (GUID::value > 0) ;

public:
	imports void init (const VAR &data) {
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

	imports VAR fetch () side_effects {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return r2x->mValue ;
	}

	imports VAR compare_exchange (const VAR &expect ,const VAR &data) side_effects {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		_DYNAMIC_ASSERT_ (!r2x->mReadOnly) ;
		if (r2x->mValue == expect)
			r2x->mValue = data ;
		return r2x->mValue ;
	}

	imports void store (const VAR &data) {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		if switch_once (TRUE) {
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
class GlobalStatic<Singleton<UNIT>>
	:private Wrapped<void> {
private:
	struct SELF_PACK {
		Atomic mCounter ;
		Singleton<UNIT> mValue ;
	} ;

private:
	friend IntrusiveRef<SELF_PACK ,GlobalStatic> ;

public:
	imports Singleton<UNIT> &unique () side_effects {
		auto &r1x = _CACHE_ ([&] () {
			auto &r2x = GlobalStatic<void>::static_unique () ;
			ScopedGuard<Mutex> ANONYMOUS (r2x.mNodeMutex) ;
			const auto r3x = U::OPERATOR_TYPENAME::invoke (ARGV<Singleton<UNIT>>::null) ;
			auto rax = GlobalStatic<void>::static_find_node (r2x ,r3x) ;
			auto rbx = IntrusiveRef<SELF_PACK ,GlobalStatic> () ;
			if switch_once (TRUE) {
				if (rax != NULL)
					discard ;
				rax = GlobalStatic<void>::static_new_node (r2x ,r3x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<SELF_PACK ,GlobalStatic>::make () ;
				const auto r4x = rbx.watch () ;
				auto &r5x = _XVALUE_ (ARGV<SELF_PACK>::null ,r4x) ;
				auto &r6x = _LOAD_ (ARGV<NONE>::null ,DEPTR[r5x]) ;
				rax->mValue = DEPTR[r6x] ;
			}
			auto &r7x = _LOAD_ (ARGV<SELF_PACK>::null ,rax->mValue) ;
			auto rcx = IntrusiveRef<SELF_PACK ,GlobalStatic> (DEPTR[r7x]) ;
			return rcx.watch () ;
		}) ;
		auto &r8x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		return r8x.mValue ;
	}

private:
	imports void friend_create (SELF_PACK &self_) {
		self_.mCounter = 0 ;
	}

	imports void friend_destroy (SELF_PACK &self_) {
		_STATIC_WARNING_ ("noop") ;
	}

	imports LENGTH friend_attach (SELF_PACK &self_) side_effects {
		return ++self_.mCounter ;
	}

	imports LENGTH friend_detach (SELF_PACK &self_) side_effects {
		return --self_.mCounter ;
	}

	imports void friend_latch (SELF_PACK &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

#ifdef __CSC_DEPRECATED__
template <class CONT>
class Coroutine {
public:
	implicit Coroutine () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif

class RandomService
	:private Proxy {
private:
	class Abstract
		:public Interface {
	public:
		virtual VAR entropy () const = 0 ;
		virtual void reset_seed (const VAR &seed_) = 0 ;
		virtual VAR random_value () side_effects = 0 ;
		virtual void random_skip (const LENGTH &len) = 0 ;
	} ;

	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
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

	VAR random_value (const VAR &min_ ,const VAR &max_) side_effects {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (min_ <= max_) ;
		const auto r1x = max_ - min_ + 1 ;
		const auto r2x = mThis->random_value () ;
		return r2x % r1x + min_ ;
	}

	Array<VAR> random_value (const VAR &min_ ,const VAR &max_ ,const LENGTH &len) side_effects {
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

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_) side_effects {
		return random_shuffle (count ,range_ ,BitSet<> (range_)) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_ ,BitSet<> &&res) side_effects {
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

	BitSet<> random_shuffle (const LENGTH &count ,const BitSet<> &range_) side_effects {
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

	String<STR> random_uuid () side_effects {
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
	imports STRU8 index_to_hex_str (const INDEX &index) {
		if (index < 10)
			return STRU8 (STRU8 ('0') + index) ;
		return STRU8 (STRU8 ('A' - 10) + index) ;
	}
} ;
} ;