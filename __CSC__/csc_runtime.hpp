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

class Duration {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual Reference native () const = 0 ;
		virtual LENGTH hours () const = 0 ;
		virtual LENGTH minutes () const = 0 ;
		virtual LENGTH seconds () const = 0 ;
		virtual LENGTH miliseconds () const = 0 ;
		virtual LENGTH microseconds () const = 0 ;
		virtual LENGTH nanoseconds () const = 0 ;
		virtual Duration add (const Duration &that) const = 0 ;
		virtual Duration sub (const Duration &that) const = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit Duration () = default ;

	explicit Duration (const LENGTH &milliseconds_) ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Duration>>>>>
	explicit Duration (_ARG1 &&time_) ;

	implicit Duration (const Duration &that) {
		mThis = that.mThis.share () ;
	}

	inline Duration &operator= (const Duration &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Duration () ;
			new (this) Duration (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	const _RET &native () const leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<const _RET> (r1x) ;
	}

	LENGTH hours () const {
		return mThis->hours () ;
	}

	LENGTH minutes () const {
		return mThis->minutes () ;
	}

	LENGTH seconds () const {
		return mThis->seconds () ;
	}

	LENGTH miliseconds () const {
		return mThis->miliseconds () ;
	}

	LENGTH microseconds () const {
		return mThis->microseconds () ;
	}

	LENGTH nanoseconds () const {
		return mThis->nanoseconds () ;
	}

	Duration add (const Duration &that) const {
		return mThis->add (that) ;
	}

	inline Duration operator+ (const Duration &that) const {
		return add (that) ;
	}

	Duration sub (const Duration &that) const {
		return mThis->sub (that) ;
	}

	inline Duration operator- (const Duration &that) const {
		return sub (that) ;
	}
} ;

class TimePoint {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual Reference native () const = 0 ;
		virtual ARRAY8<LENGTH> calendar () const = 0 ;
		virtual TimePoint add (const Duration &that) const = 0 ;
		virtual Duration sub (const TimePoint &that) const = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit TimePoint () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,TimePoint>>>>>
	explicit TimePoint (_ARG1 &&time_) ;

	implicit TimePoint (const TimePoint &that) {
		mThis = that.mThis.share () ;
	}

	inline TimePoint &operator= (const TimePoint &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~TimePoint () ;
			new (this) TimePoint (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	const _RET &native () const leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<const _RET> (r1x) ;
	}

	ARRAY8<LENGTH> calendar () const {
		return mThis->calendar () ;
	}

	TimePoint add (const Duration &that) const {
		return mThis->add (that) ;
	}

	inline TimePoint operator+ (const Duration &that) const {
		return add (that) ;
	}

	Duration sub (const TimePoint &that) const {
		return mThis->sub (that) ;
	}

	inline Duration operator- (const TimePoint &that) const {
		return sub (that) ;
	}
} ;

class Mutex {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual Reference native () = 0 ;
		virtual Reference native () const = 0 ;
		virtual void lock () = 0 ;
		virtual BOOL try_lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit Mutex () ;

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	_RET &native () leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<_RET> (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	const _RET &native () const leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<const _RET> (r1x) ;
	}

	void lock () {
		return mThis->lock () ;
	}

	BOOL try_lock () {
		return mThis->try_lock () ;
	}

	void unlock () {
		return mThis->unlock () ;
	}
} ;

class RecursiveMutex {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual Reference native () = 0 ;
		virtual Reference native () const = 0 ;
		virtual void lock () = 0 ;
		virtual BOOL try_lock () = 0 ;
		virtual void unlock () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit RecursiveMutex () ;

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	_RET &native () leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<_RET> (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	const _RET &native () const leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<const _RET> (r1x) ;
	}

	void lock () {
		return mThis->lock () ;
	}

	BOOL try_lock () {
		return mThis->try_lock () ;
	}

	void unlock () {
		return mThis->unlock () ;
	}
} ;

class UniqueLock ;

class ConditionLock {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual Reference native () = 0 ;
		virtual Reference native () const = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit ConditionLock () ;

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	_RET &native () leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<_RET> (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Implement>>
	const _RET &native () const leftvalue {
		const auto r1x = mThis->native () ;
		return SafeReference<const _RET> (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<UniqueLock>>
	_RET watch (PhanRef<Mutex> &&mutex_) leftvalue {
		using R1X = DEPENDENT_TYPE<UniqueLock ,struct ANONYMOUS> ;
		return R1X (_MOVE_ (mutex_) ,PhanRef<ConditionLock>::make (DEREF[this])) ;
	}
} ;

class UniqueLock {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual void wait () = 0 ;
		virtual void wait (const Duration &time_) = 0 ;
		virtual void wait (const TimePoint &time_) = 0 ;
		virtual void notify () = 0 ;
		virtual void yield () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit UniqueLock () = default ;

	explicit UniqueLock (PhanRef<Mutex> &&mutex_ ,PhanRef<ConditionLock> &&condition_lock) ;

	void wait () {
		return mThis->wait () ;
	}

	void wait (const Duration &time_) {
		return mThis->wait (time_) ;
	}

	void wait (const TimePoint &time_) {
		return mThis->wait (time_) ;
	}

	void notify () {
		return mThis->notify () ;
	}

	void yield () {
		return mThis->yield () ;
	}
} ;

class Thread {
public:
	class Binder :
		delegate public Interface {
	public:
		virtual void execute () = 0 ;
	} ;

private:
	struct Private {
		class Runnable ;

		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual void join () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit Thread () = default ;

	explicit Thread (const StrongRef<Binder> &runnable) ;

	void join () {
		return mThis->join () ;
	}
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

class GlobalRuntime :
	delegate private Wrapped<> {
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

	imports FLAG system_exec (const String<STR> &cmd) ;
} ;

class TypeNameInvokeProc :
	delegate private Wrapped<> {
private:
	struct TYPENAME {
		String<STR> mName ;
	} ;

public:
	template <class _ARG1>
	imports String<STR> invoke (const ARGVF<_ARG1> &) {
		const auto r1x = Function<void (TextWriter<STR> &)> ([] (TextWriter<STR> &writer) {
			template_write_typename_x (writer ,ARGV<_ARG1>::ID) ;
		}) ;
		return String<STR>::make (r1x.self) ;
	}

private:
#ifdef __CSC_COMPILER_MSVC__
	template <class _RET>
	imports TYPENAME typeid_name_from_func () {
		const auto r1x = _PCSTR_ ("struct CSC::TypeNameInvokeProc::TYPENAME __cdecl CSC::TypeNameInvokeProc::typeid_name_from_func<") ;
		const auto r2x = _PCSTR_ (">(void)") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (PTRTOARR[M_FUNC])) ;
		const auto r3x = r1x.size () ;
		const auto r4x = ret.mName.length () - 1 - r2x.size () ;
		ret.mName = ret.mName.segment (r3x ,r4x) ;
		return _MOVE_ (ret) ;
	}
#endif

#ifdef __CSC_COMPILER_GNUC__
	template <class _RET>
	imports TYPENAME typeid_name_from_func () {
		const auto r1x = _PCSTR_ ("static CSC::TypeNameInvokeProc::TYPENAME CSC::TypeNameInvokeProc::typeid_name_from_func() [with _RET = ") ;
		const auto r2x = _PCSTR_ ("]") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (PTRTOARR[M_FUNC])) ;
		const auto r3x = r1x.size () ;
		const auto r4x = ret.mName.length () - 1 - r2x.size () ;
		ret.mName = ret.mName.segment (r3x ,r4x) ;
		return _MOVE_ (ret) ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
	template <class _RET>
	imports TYPENAME typeid_name_from_func () {
		const auto r1x = _PCSTR_ ("static CSC::TypeNameInvokeProc::TYPENAME CSC::TypeNameInvokeProc::typeid_name_from_func() [_RET = ") ;
		const auto r2x = _PCSTR_ ("]") ;
		TYPENAME ret ;
		ret.mName = StringProc::parse_strs (String<STRA> (PTRTOARR[M_FUNC])) ;
		const auto r3x = r1x.size () ;
		const auto r4x = ret.mName.length () - 1 - r2x.size () ;
		ret.mName = ret.mName.segment (r3x ,r4x) ;
		return _MOVE_ (ret) ;
}
#endif

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<IS_CONST_HELP<_ARG1> ,IS_VOLATILE_HELP<_ARG1>>>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP4)> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_CONST_HELP<_ARG1>>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_VOLATILE_HELP<_ARG1>>>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG1>
	imports void template_write_typename_cv (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_LVALUE_REFERENCE_HELP<_ARG1>>>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_RVALUE_REFERENCE_HELP<_ARG1>>>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG1>
	imports void template_write_typename_ref (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_POINTER_HELP<_ARG1>>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP8)> &) {
		writer << _PCSTR_ ("PTR<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_POINTER_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_MEMPTR_HELP<_ARG1>>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP7)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_MEMPTR_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (" ") ;
		template_write_typename_x (writer ,ARGV<MEMPTR_CLASS_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_SAME_HELP<_ARG1 ,ARR<REMOVE_ARRAY_TYPE<_ARG1>>>>>
	imports void template_write_typename_arr (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("ARR<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_ARRAY_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_ARRAY_OF_HELP<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>>>
	imports void template_write_typename_arr (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_ARRAY_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_ARRAY_HELP<_ARG1>>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP6)> &) {
		template_write_typename_arr (writer ,ARGV<_ARG1>::ID ,ARGVPX) ;
	}

	template <class _ARG1>
	imports void template_write_typename_func (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,ARGV<REMOVE_FUNCTION_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,ARGV<FUNCTION_PARAMS_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_FUNCTION_HELP<_ARG1>>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP5)> &) {
		template_write_typename_func (writer ,ARGV<REMOVE_FUNCATTR_TYPE<_ARG1>>::ID ,ARGVPX) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_TEMPLATE_HELP<_ARG1>>>
	imports void template_write_typename_claz (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP2)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("template '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'<") ;
		template_write_typename_y (writer ,ARGV<TEMPLATE_PARAMS_TYPE<_ARG1>>::ID) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	imports void template_write_typename_claz (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<IS_CLASS_HELP<_ARG1>>>
	imports void template_write_typename_id (TextWriter<STR> &writer ,const ARGVF<_ARG1> & ,const DEF<decltype (ARGVP4)> &) {
		template_write_typename_claz (writer ,ARGV<_ARG1>::ID ,ARGVPX) ;
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
		template_write_typename_cv (writer ,ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>::ID ,ARGVPX) ;
		template_write_typename_id (writer ,ARGV<REMOVE_CVR_TYPE<_ARG1>>::ID ,ARGVPX) ;
		template_write_typename_ref (writer ,ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>::ID ,ARGVPX) ;
	}

	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<ARGVS<_ARG1>> &) {
		template_write_typename_x (writer ,ARGV<_ARG1>::ID) ;
	}

	template <class _ARG1>
	imports void template_write_typename_y (TextWriter<STR> &writer ,const ARGVF<_ARG1> &) {
		using R1X = PARAMS_ONE_TYPE<_ARG1> ;
		using R2X = PARAMS_REST_TYPE<_ARG1> ;
		template_write_typename_x (writer ,ARGV<R1X>::ID) ;
		writer << _PCSTR_ (" ,") ;
		template_write_typename_y (writer ,ARGV<R2X>::ID) ;
	}
} ;

template <class>
class GlobalStatic ;

class GlobalStaticEngine :
	delegate private Wrapped<> {
private:
	struct VARXX_NODE {
		FLAG mGUID ;
		BOOL mReadOnly ;
		VAR mValue ;
	} ;

	struct CLAZZ_NODE {
		String<STR> mGUID ;
		PTR<NONE> mValue ;
	} ;

	struct THIS_PACK {
		Mutex mNodeMutex ;
		Deque<VARXX_NODE> mVarxx ;
		HashSet<FLAG> mVarxxMappingSet ;
		Deque<CLAZZ_NODE> mClazz ;
		HashSet<FLAG> mClazzMappingSet ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;

public:
	//@warn: this function should be implemented in a 'runtime.dll'
	imports PTR<NONE> unique_compare_exchange (const PTR<NONE> &expect ,const PTR<NONE> &data) ;

private:
	imports const StrongRef<THIS_PACK> &cache_unique () {
		using R1X = REMOVE_POINTER_TYPE<decltype (_NULL_ (ARGV<WeakRef>::ID).intrusive ())> ;
		return _CACHE_ ([&] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_compare_exchange (NULL ,NULL) ;
			auto rbx = StrongRef<THIS_PACK> () ;
			if switch_once (TRUE) {
				if (rax != NULL)
					discard ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = StrongRef<THIS_PACK>::make () ;
				const auto r2x = rbx.weak () ;
				const auto r3x = r2x.intrusive () ;
				rax = unique_compare_exchange (NULL ,r3x) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			const auto r4x = _POINTER_CAST_ (ARGV<R1X>::ID ,rax) ;
			return WeakRef (r4x).strong (ARGV<THIS_PACK>::ID) ;
		}) ;
	}

	imports THIS_PACK &static_unique () {
		auto rax = cache_unique ().share () ;
		_DYNAMIC_ASSERT_ (rax.exist ()) ;
		return rax.self ;
	}

	imports PTR<VARXX_NODE> static_new_node (THIS_PACK &this_ ,const FLAG &guid) {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = this_.mVarxxMappingSet.map (r1x) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = this_.mVarxx.insert () ;
			this_.mVarxxMappingSet.add (r1x ,ix) ;
			this_.mVarxx[ix].mGUID = guid ;
		}
		return DEPTR[this_.mVarxx[ix]] ;
	}

	imports FLAG node_guid_hash (const FLAG &guid) {
		return guid ;
	}

	imports PTR<VARXX_NODE> static_find_node (THIS_PACK &this_ ,const FLAG &guid) {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = this_.mVarxxMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[this_.mVarxx[ix]] ;
	}

	imports PTR<CLAZZ_NODE> static_new_node (THIS_PACK &this_ ,const String<STR> &guid) {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = this_.mClazzMappingSet.map (r1x) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = this_.mClazz.insert () ;
			this_.mClazzMappingSet.add (r1x ,ix) ;
			this_.mClazz[ix].mGUID = guid ;
		}
		return DEPTR[this_.mClazz[ix]] ;
	}

	imports FLAG node_guid_hash (const String<STR> &guid) {
		const auto r1x = guid.raw () ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		return BasicProc::mem_hash (r2x.self ,r2x.size ()) ;
	}

	imports PTR<CLAZZ_NODE> static_find_node (THIS_PACK &this_ ,const String<STR> &guid) {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = this_.mClazzMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return DEPTR[this_.mClazz[ix]] ;
	}
} ;

template <class GUID>
class GlobalStatic :
	delegate private Wrapped<> {
private:
	_STATIC_ASSERT_ (U::CONSTEXPR_COMPR_GT<GUID ,ZERO>::compile ()) ;

public:
	imports void init (const VAR &data) {
		auto &r1x = GlobalStaticEngine::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStaticEngine::static_find_node (r1x ,GUID::compile ()) ;
		if (r2x != NULL)
			return ;
		const auto r3x = GlobalStaticEngine::static_new_node (r1x ,GUID::compile ()) ;
		_DYNAMIC_ASSERT_ (r3x != NULL) ;
		DEREF[r3x].mReadOnly = TRUE ;
		DEREF[r3x].mValue = data ;
	}

	imports VAR fetch () {
		auto &r1x = GlobalStaticEngine::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStaticEngine::static_find_node (r1x ,GUID::compile ()) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return DEREF[r2x].mValue ;
	}

	imports VAR compare_exchange (const VAR &expect ,const VAR &data) {
		auto &r1x = GlobalStaticEngine::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStaticEngine::static_find_node (r1x ,GUID::compile ()) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		_DYNAMIC_ASSERT_ (!DEREF[r2x].mReadOnly) ;
		if (DEREF[r2x].mValue == expect)
			DEREF[r2x].mValue = data ;
		return DEREF[r2x].mValue ;
	}

	imports void store (const VAR &data) {
		auto &r1x = GlobalStaticEngine::static_unique () ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mNodeMutex) ;
		auto rax = GlobalStaticEngine::static_find_node (r1x ,GUID::compile ()) ;
		if switch_once (TRUE) {
			if (rax != NULL)
				discard ;
			rax = GlobalStaticEngine::static_new_node (r1x ,GUID::compile ()) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mValue = data ;
	}
} ;

template <class UNIT>
class GlobalStatic<Singleton<UNIT>> :
	delegate private Wrapped<> {
private:
	struct THIS_PACK {
		Singleton<UNIT> mValue ;
	} ;

public:
	imports Singleton<UNIT> &unique () {
		auto rax = cache_unique ().share () ;
		_DYNAMIC_ASSERT_ (rax.exist ()) ;
		return rax->mValue ;
	}

private:
	imports const StrongRef<THIS_PACK> &cache_unique () {
		using R1X = REMOVE_POINTER_TYPE<decltype (_NULL_ (ARGV<WeakRef>::ID).intrusive ())> ;
		return _CACHE_ ([&] () {
			auto &r2x = GlobalStaticEngine::static_unique () ;
			ScopedGuard<Mutex> ANONYMOUS (r2x.mNodeMutex) ;
			const auto r3x = TypeNameInvokeProc::invoke (ARGV<Singleton<UNIT>>::ID) ;
			auto rax = GlobalStaticEngine::static_find_node (r2x ,r3x) ;
			auto rbx = StrongRef<THIS_PACK> () ;
			if switch_once (TRUE) {
				if (rax != NULL)
					discard ;
				rax = GlobalStaticEngine::static_new_node (r2x ,r3x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = StrongRef<THIS_PACK>::make () ;
				const auto r4x = rbx.weak () ;
				DEREF[rax].mValue = r4x.intrusive () ;
			}
			const auto r5x = _POINTER_CAST_ (ARGV<R1X>::ID ,DEREF[rax].mValue) ;
			return WeakRef (r5x).strong (ARGV<THIS_PACK>::ID) ;
		}) ;
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

class RandomService :
	delegate private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual VAR32 entropy () const = 0 ;
		virtual void reset_seed (const VAR32 &seed_) = 0 ;
		virtual VAR32 random_value () = 0 ;
		virtual void random_skip (const LENGTH &len) = 0 ;
	} ;

private:
	friend Singleton<RandomService> ;
	Mutable<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	implicit RandomService () = delete ;

	VAR entropy () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return VAR (mThis->entropy ()) ;
	}

	void reset_seed (const VAR &seed_) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->reset_seed (VAR32 (seed_)) ;
	}

	VAR random_value (const VAR &lb ,const VAR &rb) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		const auto r1x = VAR64 (rb) - VAR64 (lb) ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x <= VAR64 (VAR32_MAX)) ;
		const auto r2x = r1x + 1 ;
		const auto r3x = VAR64 (mThis->random_value ()) ;
		return VAR (r3x % r2x + lb) ;
	}

	Array<VAR> random_value (const VAR &lb ,const VAR &rb ,const LENGTH &len) {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (lb <= rb) ;
		Array<VAR> ret = Array<VAR> (len) ;
		const auto r1x = VAR64 (rb) - VAR64 (lb) ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x <= VAR64 (VAR32_MAX)) ;
		const auto r2x = r1x + 1 ;
		for (auto &&i : _RANGE_ (0 ,ret.length ())) {
			const auto r3x = VAR64 (mThis->random_value ()) ;
			ret[i] = VAR (r3x % r2x + lb) ;
		}
		return _MOVE_ (ret) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_) {
		return random_shuffle (count ,range_ ,BitSet<> (range_)) ;
	}

	BitSet<> random_shuffle (const LENGTH &count ,const LENGTH &range_ ,BitSet<> &&res) {
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

	BitSet<> random_shuffle (const LENGTH &count ,const BitSet<> &range_) {
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

	CSC::BOOL random_draw (const VAL &possibility) {
		const auto r1x = random_value (0 ,10000) ;
		const auto r2x = VAL (r1x) * MathProc::inverse (VAL (10000)) ;
		if (r2x < possibility)
			return TRUE ;
		return FALSE ;
	}

	String<STR> random_uuid () {
		const auto r1x = _PCSTR_ ("00000000-0000-0000-000000000000") ;
		String<STR> ret = String<STR> (r1x.size ()) ;
		INDEX iw = 0 ;
		const auto r2x = random_value (0 ,35 ,28) ;
		for (auto &&i : _RANGE_ (0 ,8)) {
			INDEX ix = 0 + i ;
			ret[iw++] = index_to_hex_str (r2x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 8 + i ;
			ret[iw++] = index_to_hex_str (r2x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 12 + i ;
			ret[iw++] = index_to_hex_str (r2x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,12)) {
			INDEX ix = 16 + i ;
			ret[iw++] = index_to_hex_str (r2x[ix]) ;
		}
		if (iw < ret.size ())
			ret[iw] = 0 ;
		return _MOVE_ (ret) ;
	}

private:
	explicit RandomService (const ARGVF<Singleton<RandomService>> &) ;

private:
	imports STRU8 index_to_hex_str (const INDEX &index) {
		if (index < 10)
			return STRU8 (STRU8 ('0') + index) ;
		return STRU8 (STRU8 ('A' - 10) + index) ;
	}
} ;
} ;