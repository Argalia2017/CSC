#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
namespace RUNTIME {
template <class...>
trait TIMEPOINT_HELP ;

template <>
trait TIMEPOINT_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_now () = 0 ;
		virtual void init_epoch () = 0 ;
		virtual Auto native () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class TimePoint {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimePoint () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		imports TimePoint make_now () {
			TimePoint ret ;
			ret.mThis->init_now () ;
			return move (ret) ;
		}

		imports CREF<TimePoint> epoch () {
			return memorize ([&] () {
				TimePoint ret ;
				ret.mThis->init_epoch () ;
				return move (ret) ;
			}) ;
		}

		Auto native () const {
			return mThis->native () ;
		}
	} ;
} ;

using TimePoint = typename TIMEPOINT_HELP<ALWAYS>::TimePoint ;

template <class...>
trait TIMEDURATION_HELP ;

template <>
trait TIMEDURATION_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_zero () = 0 ;
		virtual Auto native () const = 0 ;
		virtual LENGTH hours () const = 0 ;
		virtual LENGTH minutes () const = 0 ;
		virtual LENGTH seconds () const = 0 ;
		virtual LENGTH milliseconds () const = 0 ;
		virtual LENGTH microseconds () const = 0 ;
		virtual LENGTH nanoseconds () const = 0 ;
		virtual void add_from (CREF<Holder> a ,CREF<Holder> b) = 0 ;
		virtual void sub_from (CREF<Holder> a ,CREF<Holder> b) = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class TimeDuration {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimeDuration () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		imports CREF<TimeDuration> zero () {
			return memorize ([&] () {
				TimeDuration ret ;
				ret.mThis->init_zero () ;
				return move (ret) ;
			}) ;
		}

		Auto native () const {
			return mThis->native () ;
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

		LENGTH milliseconds () const {
			return mThis->milliseconds () ;
		}

		LENGTH microseconds () const {
			return mThis->microseconds () ;
		}

		LENGTH nanoseconds () const {
			return mThis->nanoseconds () ;
		}

		TimeDuration add (CREF<TimeDuration> that) const {
			TimeDuration ret ;
			ret.mThis->add_from (mThis ,that.mThis) ;
			return move (ret) ;
		}

		inline TimeDuration operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimeDuration sub (CREF<TimeDuration> that) const {
			TimeDuration ret ;
			ret.mThis->sub_from (mThis ,that.mThis) ;
			return move (ret) ;
		}

		inline TimeDuration operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}
	} ;
} ;

using TimeDuration = typename TIMEDURATION_HELP<ALWAYS>::TimeDuration ;

template <class...>
trait MUTEX_HELP ;

template <>
trait MUTEX_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual Auto native () const = 0 ;
		virtual void enter () const = 0 ;
		virtual BOOL try_enter () const = 0 ;
		virtual void leave () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Mutex {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Mutex () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		void enter () const {
			return mThis->enter () ;
		}

		BOOL try_enter () const {
			return mThis->try_enter () ;
		}

		void leave () const {
			return mThis->leave () ;
		}
	} ;
} ;

using Mutex = typename MUTEX_HELP<ALWAYS>::Mutex ;

template <class...>
trait CONDITIONALLOCK_HELP ;

template <>
trait CONDITIONALLOCK_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual Auto native () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class ConditionalLock {
	private:
		VRef<Holder> mThis ;

	public:
		implicit ConditionalLock () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		Auto native () const {
			return mThis->native () ;
		}
	} ;
} ;

using ConditionalLock = typename CONDITIONALLOCK_HELP<ALWAYS>::ConditionalLock ;

template <class...>
trait THREAD_HELP ;

template <>
trait THREAD_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual Auto native () const = 0 ;
		virtual FLAG thread_id () const = 0 ;
		virtual void sleep (CREF<TimePoint> time_) = 0 ;
		virtual void sleep (CREF<TimeDuration> time_) = 0 ;
		virtual void yield () = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Thread {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Thread () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		imports Thread make_current () {
			Thread ret ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		FLAG thread_id () const {
			return mThis->thread_id () ;
		}

		void sleep (CREF<TimePoint> time_) {
			return mThis->sleep (time_) ;
		}

		void sleep (CREF<TimeDuration> time_) {
			return mThis->sleep (time_) ;
		}

		void yield () {
			return mThis->yield () ;
		}
	} ;
} ;

using Thread = typename THREAD_HELP<ALWAYS>::Thread ;

using PROCESS_INFO = BoxBuffer<BYTE ,ENUMAS<VAL ,ENUMID<(128)>>> ;

template <class...>
trait PROCESS_HELP ;

template <>
trait PROCESS_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual void init_process_info (CREF<PROCESS_INFO> info) = 0 ;
		virtual Auto native () const = 0 ;
		virtual FLAG process_id () const = 0 ;
		virtual PROCESS_INFO process_info () const = 0 ;
		virtual void process_exit () = 0 ;
		virtual void process_abort () = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Process {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Process () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		imports Process make_current () {
			Process ret ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		imports Process make_process_info (CREF<PROCESS_INFO> info) {
			Process ret ;
			ret.mThis->init_process_info (info) ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		FLAG process_id () const {
			return mThis->process_id () ;
		}

		PROCESS_INFO process_info () const {
			return mThis->process_info () ;
		}

		void process_exit[[noreturn]] () {
			mThis->process_exit () ;
			dynamic_assert (FALSE) ;
		}

		void process_abort[[noreturn]] () {
			mThis->process_abort () ;
			dynamic_assert (FALSE) ;
		}
	} ;
} ;

using Process = typename PROCESS_HELP<ALWAYS>::Process ;

template <class...>
trait MODULE_HELP ;

template <>
trait MODULE_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual Auto native () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Module {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Module () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		imports Module make_current () {
			Module ret ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}
	} ;
} ;

using Module = typename MODULE_HELP<ALWAYS>::Module ;

template <class...>
trait SYSTEM_HELP ;

template <>
trait SYSTEM_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual Auto native () const = 0 ;
		virtual void set_locale (CREF<String<STR>> name) = 0 ;
		virtual void execute (CREF<String<STR>> command) = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class System {
	private:
		VRef<Holder> mThis ;

	public:
		implicit System () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		Auto native () const {
			return mThis->native () ;
		}
		
		void set_locale (CREF<String<STR>> name) {
			return mThis->set_locale (name) ;
		}

		void execute (CREF<String<STR>> command) {
			return mThis->execute (command) ;
		}
	} ;
} ;

using System = typename SYSTEM_HELP<ALWAYS>::System ;

template <class...>
trait RANDOM_HELP ;

template <>
trait RANDOM_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual Auto native () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Random {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Random () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		Auto native () const {
			return mThis->native () ;
		}
	} ;
} ;

using Random = typename RANDOM_HELP<ALWAYS>::Random ;
} ;
} ;

namespace CSC {
using namespace RUNTIME ;
} ;