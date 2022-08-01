#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
template <class...>
trait TIMEDURATION_HELP ;

template <class...>
trait TIMEPOINT_HELP ;

template <class...>
trait TIMEDURATION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEDURATION_HELP<DEPEND ,ALWAYS> {
	struct CALENDAR {
		LENGTH mYear ;
		LENGTH mMonth ;
		LENGTH mDay ;
		LENGTH mWDay ;
		LENGTH mYDay ;
		LENGTH mHour ;
		LENGTH mMinute ;
		LENGTH mSecond ;
	} ;

	template <class ARG1>
	using CRTP_TimePoint = typename DEPENDENT<TIMEPOINT_HELP<DEPEND ,ALWAYS> ,ARG1>::TimePoint ;

	template <class ARG1>
	using CRTP_TimeDuration = typename DEPENDENT<TIMEDURATION_HELP<DEPEND ,ALWAYS> ,ARG1>::TimeDuration ;

	struct Holder implement Interface {
		virtual void init_now () = 0 ;
		virtual void init_epoch () = 0 ;
		virtual void initialize (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) = 0 ;
		virtual void initialize (CREF<CALENDAR> calendar_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual LENGTH hours () const = 0 ;
		virtual LENGTH minutes () const = 0 ;
		virtual LENGTH seconds () const = 0 ;
		virtual LENGTH milliseconds () const = 0 ;
		virtual LENGTH microseconds () const = 0 ;
		virtual LENGTH nanoseconds () const = 0 ;
		virtual CALENDAR calendar () const = 0 ;
		virtual CRTP_TimeDuration<DEPEND> add (CREF<Holder> that) const = 0 ;
		virtual CRTP_TimeDuration<DEPEND> sub (CREF<Holder> that) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TimeDuration {
	private:
		template <class...>
		friend trait TIMEPOINT_HELP ;

	protected:
		VRef<Holder> mThis ;

	public:
		implicit TimeDuration () = default ;

		explicit TimeDuration (CREF<LENGTH> milliseconds_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (milliseconds_ ,0) ;
		}

		explicit TimeDuration (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (milliseconds_ ,nanoseconds_) ;
		}

		explicit TimeDuration (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		template <class ARG1 = DEPEND>
		explicit TimeDuration (RREF<CRTP_TimePoint<ARG1>> that) {
			mThis = move (that.mThis) ;
		}

		imports CREF<TimeDuration> zero () {
			return memorize ([&] () {
				TimeDuration ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_epoch () ;
				return move (ret) ;
			}) ;
		}

		Auto native () const leftvalue {
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
			return mThis->add (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline TimeDuration operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimeDuration sub (CREF<TimeDuration> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline TimeDuration operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}

		template <class ARG1 = DEPEND>
		CRTP_TimePoint<ARG1> add (CREF<CRTP_TimePoint<ARG1>> that) const {
			return mThis->add (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		template <class ARG1 = DEPEND>
		inline CRTP_TimePoint<ARG1> operator+ (CREF<CRTP_TimePoint<ARG1>> that) const {
			return add (that) ;
		}

		template <class ARG1 = DEPEND>
		CRTP_TimePoint<ARG1> sub (CREF<CRTP_TimePoint<ARG1>> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		template <class ARG1 = DEPEND>
		inline CRTP_TimePoint<ARG1> operator- (CREF<CRTP_TimePoint<ARG1>> that) const {
			return sub (that) ;
		}
	} ;
} ;

using TimeDuration = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimeDuration ;

template <class...>
trait TIMEPOINT_HELP ;

template <class...>
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEPOINT_HELP<DEPEND ,ALWAYS> {
	using CALENDAR = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::CALENDAR ;
	using Holder = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	template <class ARG1>
	using CRTP_TimeDuration = typename DEPENDENT<TIMEDURATION_HELP<DEPEND ,ALWAYS> ,ARG1>::TimeDuration ;

	class TimePoint {
	private:
		template <class...>
		friend trait TIMEDURATION_HELP ;

	protected:
		VRef<Holder> mThis ;

	public:
		implicit TimePoint () = default ;

		explicit TimePoint (CREF<CALENDAR> calendar_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (calendar_) ;
		}

		template <class ARG1 = DEPEND>
		explicit TimePoint (RREF<CRTP_TimeDuration<ARG1>> that) {
			mThis = move (that.mThis) ;
		}

		imports TimePoint make_now () {
			TimePoint ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->init_now () ;
			return move (ret) ;
		}

		imports CREF<TimePoint> epoch () {
			return memorize ([&] () {
				TimePoint ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_epoch () ;
				return move (ret) ;
			}) ;
		}

		Auto native () const leftvalue {
			return mThis->native () ;
		}

		TimePoint add (CREF<TimeDuration> that) const {
			return mThis->add (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline TimePoint operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimePoint sub (CREF<TimeDuration> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline TimePoint operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}

		TimeDuration sub (CREF<TimePoint> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline TimeDuration operator- (CREF<TimePoint> that) const {
			return sub (that) ;
		}

		CALENDAR calendar () const {
			return mThis->calendar () ;
		}
	} ;

	class NowTimePoint implement TimePoint {
	public:
		implicit NowTimePoint ()
			:TimePoint (TimePoint::make_now ()) {}
	} ;
} ;

using TimePoint = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::TimePoint ;
using NowTimePoint = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::NowTimePoint ;

template <class...>
trait ATOMIC_HELP ;

template <class...>
trait ATOMIC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ATOMIC_HELP<DEPEND ,ALWAYS> {
	require (IS_CLONEABLE<VAL>) ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual VAL fetch () const = 0 ;
		virtual void store (CREF<VAL> obj) const = 0 ;
		virtual VAL exchange (CREF<VAL> obj) const = 0 ;
		virtual void replace (CREF<VAL> expect ,CREF<VAL> right) const = 0 ;
		virtual BOOL change (VREF<VAL> expect ,CREF<VAL> right) const = 0 ;
		virtual VAL fetch_add (CREF<VAL> obj) const = 0 ;
		virtual VAL fetch_sub (CREF<VAL> obj) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Atomic {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Atomic () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		VAL fetch () const {
			return mThis->fetch () ;
		}

		void store (CREF<VAL> obj) const {
			return mThis->store (obj) ;
		}

		VAL exchange (CREF<VAL> obj) const {
			return mThis->exchange (obj) ;
		}

		void replace (CREF<VAL> expect ,CREF<VAL> right) const {
			return mThis->replace (expect ,right) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> right) const {
			return mThis->change (expect ,right) ;
		}

		VAL fetch_add (CREF<VAL> obj) const {
			return mThis->fetch_add (obj) ;
		}

		VAL fetch_sub (CREF<VAL> obj) const {
			return mThis->fetch_sub (obj) ;
		}

		VAL increase () const {
			return fetch_add (VAL (1)) + VAL (1) ;
		}

		VAL decrease () const {
			return fetch_sub (VAL (1)) + VAL (1) ;
		}
	} ;
} ;

using Atomic = typename ATOMIC_HELP<DEPEND ,ALWAYS>::Atomic ;

template <class...>
trait MUTEX_HELP ;

template <class...>
trait MUTEX_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MUTEX_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_recursive () = 0 ;
		virtual void init_conditional () = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual void enter () const = 0 ;
		virtual void leave () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Mutex {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Mutex () = default ;

		imports Mutex make_recursive () {
			Mutex ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->init_recursive () ;
			return move (ret) ;
		}

		imports Mutex make_conditional () {
			Mutex ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->init_conditional () ;
			return move (ret) ;
		}

		Auto native () const leftvalue {
			return mThis->native () ;
		}

		void enter () const {
			if (mThis == NULL)
				return ;
			return mThis->enter () ;
		}

		void leave () const {
			if (mThis == NULL)
				return ;
			return mThis->leave () ;
		}
	} ;

	class RecursiveMutex implement Mutex {
	public:
		implicit RecursiveMutex ()
			:Mutex (Mutex::make_recursive ()) {}
	} ;

	class ConditionalMutex implement Mutex {
	public:
		implicit ConditionalMutex ()
			:Mutex (Mutex::make_conditional ()) {}
	} ;
} ;

using Mutex = typename MUTEX_HELP<DEPEND ,ALWAYS>::Mutex ;
using RecursiveMutex = typename MUTEX_HELP<DEPEND ,ALWAYS>::RecursiveMutex ;
using ConditionalMutex = typename MUTEX_HELP<DEPEND ,ALWAYS>::ConditionalMutex ;

template <class...>
trait UNIQUELOCK_HELP ;

template <class...>
trait UNIQUELOCK_IMPLHOLDER_HELP ;

template <class DEPEND>
trait UNIQUELOCK_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<Mutex> mutex_) = 0 ;
		virtual void wait () = 0 ;
		virtual void wait (CREF<TimeDuration> time_) = 0 ;
		virtual void wait (CREF<TimePoint> time_) = 0 ;
		virtual void notify () = 0 ;
		virtual void yield () = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class UniqueLock {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit UniqueLock () = default ;

		explicit UniqueLock (CREF<Mutex> mutex_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (mutex_) ;
		}

		void wait () {
			return mThis->wait () ;
		}

		void wait (CREF<TimeDuration> time_) {
			return mThis->wait (time_) ;
		}

		void wait (CREF<TimePoint> time_) {
			return mThis->wait (time_) ;
		}

		void notify () {
			return mThis->notify () ;
		}

		void yield () {
			return mThis->yield () ;
		}
	} ;
} ;

using UniqueLock = typename UNIQUELOCK_HELP<DEPEND ,ALWAYS>::UniqueLock ;

template <class...>
trait SHAREDLOCK_HELP ;

template <class...>
trait SHAREDLOCK_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SHAREDLOCK_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<Mutex> mutex_) = 0 ;
		virtual BOOL lock () const = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class SharedLock {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit SharedLock () = default ;

		explicit SharedLock (CREF<Mutex> mutex_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (mutex_) ;
		}

		BOOL lock () const {
			return mThis->lock () ;
		}
	} ;
} ;

using SharedLock = typename SHAREDLOCK_HELP<DEPEND ,ALWAYS>::SharedLock ;

template <class...>
trait RUNTIMEPROC_HELP ;

template <class...>
trait RUNTIMEPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RUNTIMEPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual LENGTH thread_concurrency () const = 0 ;
		virtual FLAG thread_uid () const = 0 ;
		virtual void thread_sleep (CREF<TimePoint> time_) const = 0 ;
		virtual void thread_sleep (CREF<TimeDuration> time_) const = 0 ;
		virtual void thread_yield () const = 0 ;
		virtual FLAG process_uid () const = 0 ;
		virtual void process_exit () const = 0 ;
		virtual void process_abort () const = 0 ;
		virtual String<STR> working_path () const = 0 ;
		virtual String<STR> module_path () const = 0 ;
		virtual String<STR> module_name () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class RuntimeProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<RuntimeProc> instance () {
			return memorize ([&] () {
				RuntimeProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports LENGTH thread_concurrency () {
			return instance ().mThis->thread_concurrency () ;
		}

		imports FLAG thread_uid () {
			return instance ().mThis->thread_uid () ;
		}

		imports void thread_sleep (CREF<TimePoint> time_) {
			return instance ().mThis->thread_sleep (time_) ;
		}

		imports void thread_sleep (CREF<TimeDuration> time_) {
			return instance ().mThis->thread_sleep (time_) ;
		}

		imports void thread_yield () {
			return instance ().mThis->thread_yield () ;
		}

		imports FLAG process_uid () {
			return instance ().mThis->process_uid () ;
		}

		imports void process_exit () {
			return instance ().mThis->process_exit () ;
		}

		imports void process_abort () {
			return instance ().mThis->process_abort () ;
		}

		imports String<STR> working_path () {
			return instance ().mThis->working_path () ;
		}

		imports String<STR> module_path () {
			return instance ().mThis->module_path () ;
		}

		imports String<STR> module_name () {
			return instance ().mThis->module_name () ;
		}
	} ;
} ;

using RuntimeProc = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::RuntimeProc ;

template <class...>
trait THREAD_HELP ;

template <class...>
trait THREAD_IMPLHOLDER_HELP ;

template <class DEPEND>
trait THREAD_HELP<DEPEND ,ALWAYS> {
	struct Binder implement Interface {
		virtual void friend_execute (CREF<INDEX> index) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<Binder>> binder ,CREF<INDEX> index) = 0 ;
		virtual FLAG thread_uid () const = 0 ;
		virtual void start () = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Thread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Thread () = default ;

		explicit Thread (RREF<VRef<Binder>> binder ,CREF<INDEX> index) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (binder) ,index) ;
		}

		FLAG thread_uid () const {
			return mThis->thread_uid () ;
		}

		void start () {
			return mThis->start () ;
		}

		void stop () {
			return mThis->stop () ;
		}
	} ;
} ;

using Thread = typename THREAD_HELP<DEPEND ,ALWAYS>::Thread ;

template <class...>
trait PROCESS_HELP ;

template <class...>
trait PROCESS_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PROCESS_HELP<DEPEND ,ALWAYS> {
	using SNAPSHOT = ConBuffer<BYTE> ;

	struct Holder implement Interface {
		virtual void initialize (CREF<FLAG> uid) = 0 ;
		virtual void initialize (CREF<SNAPSHOT> snapshot_) = 0 ;
		virtual FLAG process_uid () const = 0 ;
		virtual SNAPSHOT snapshot () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Process {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Process () = default ;

		explicit Process (CREF<FLAG> uid) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (uid) ;
		}

		explicit Process (CREF<SNAPSHOT> snapshot_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (snapshot_) ;
		}

		FLAG process_uid () const {
			return mThis->process_uid () ;
		}

		SNAPSHOT snapshot () const {
			return mThis->snapshot () ;
		}
	} ;

	class CurrentProcess implement Process {
	public:
		implicit CurrentProcess ()
			:Process (RuntimeProc::process_uid ()) {}
	} ;
} ;

using Process = typename PROCESS_HELP<DEPEND ,ALWAYS>::Process ;
using CurrentProcess = typename PROCESS_HELP<DEPEND ,ALWAYS>::CurrentProcess ;

template <class...>
trait MODULE_HELP ;

template <class...>
trait MODULE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MODULE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<String<STR>> file) = 0 ;
		virtual CREF<String<STR>> error () const leftvalue = 0 ;
		virtual FLAG link (CREF<String<STR>> name) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Module {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Module () = default ;

		explicit Module (CREF<String<STR>> file) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (file) ;
		}

		CREF<String<STR>> error () const leftvalue {
			return mThis->error () ;
		}

		FLAG link (CREF<String<STR>> name) {
			return mThis->link (name) ;
		}
	} ;
} ;

using Module = typename MODULE_HELP<DEPEND ,ALWAYS>::Module ;

template <class...>
trait SINGLETON_HELP ;

template <class...>
trait SINGLETON_HOLDER_HELP ;

template <class...>
trait SINGLETON_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void add (CREF<Slice<STR>> name ,CREF<FLAG> addr) const = 0 ;
		virtual FLAG map (CREF<Slice<STR>> name) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class UNIT1>
trait SINGLETON_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	struct HEAP {
		VRef<Holder> mThis ;
	} ;

	class Singleton extend Proxy {
	public:
		imports CREF<UNIT1> instance () {
			return memorize ([&] () {
				auto rax = ZERO ;
				if ifswitch (TRUE) {
					const auto r1x = unique () ;
					assert (r1x.available ()) ;
					const auto r2x = Clazz (TYPEAS<UNIT1>::expr).type_name () ;
					rax = r1x->mThis->map (r2x) ;
					if (rax != ZERO)
						discard ;
					const auto r3x = address (UNIT1::instance ()) ;
					r1x->mThis->add (r2x ,r3x) ;
					rax = r1x->mThis->map (r2x) ;
				}
				assume (rax != ZERO) ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (rax))) ;
				return CRef<UNIT1>::reference (tmp) ;
			}) ;
		}

	private:
		imports SharedRef<HEAP> unique () {
			return memorize ([&] () {
				SharedRef<HEAP> ret = SharedRef<HEAP>::make () ;
				ret->mThis = FUNCTION_extern::invoke () ;
				ret->mThis->initialize () ;
				return move (ret) ;
			}) ;
		}
	} ;
} ;

template <class UNIT1>
using Singleton = typename SINGLETON_HELP<UNIT1 ,ALWAYS>::Singleton ;

template <class...>
trait SYSTEM_HELP ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SYSTEM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual String<STR> get_locale () const = 0 ;
		virtual void set_locale (CREF<String<STR>> name) const = 0 ;
		virtual void execute (CREF<String<STR>> command) const = 0 ;
		virtual String<STR> working_path () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class System {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<System> instance () {
			return memorize ([&] () {
				System ret ;
				ret.mMutex = RecursiveMutex () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		String<STR> get_locale () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->get_locale () ;
		}

		void set_locale (CREF<String<STR>> name) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->set_locale (name) ;
		}

		void execute (CREF<String<STR>> command) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->execute (command) ;
		}

		String<STR> working_path () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->working_path () ;
		}
	} ;
} ;

using System = typename SYSTEM_HELP<DEPEND ,ALWAYS>::System ;

template <class...>
trait RANDOM_HELP ;

template <class...>
trait RANDOM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RANDOM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void initialize (CREF<DATA> seed) = 0 ;
		virtual DATA seed () const = 0 ;
		virtual DATA random_byte () const = 0 ;
		virtual Array<DATA> random_byte (CREF<LENGTH> size_) const = 0 ;
		virtual INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const = 0 ;
		virtual Array<INDEX> random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> size_) const = 0 ;
		virtual Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
		virtual void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> range_) const = 0 ;
		virtual BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
		virtual void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet<>> range_) const = 0 ;
		virtual BOOL random_draw (CREF<DOUBLE> possibility) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Random {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		implicit Random () = default ;

		explicit Random (CREF<DATA> seed_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (seed_) ;
		}

		imports CREF<Random> instance () {
			return memorize ([&] () {
				Random ret ;
				ret.mMutex = RecursiveMutex () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		DATA seed () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->seed () ;
		}

		DATA random_byte () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_byte () ;
		}

		Array<DATA> random_byte (CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_byte (size_) ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_value (lb ,rb) ;
		}

		Array<INDEX> random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_value (lb ,rb ,size_) ;
		}

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_shuffle (count ,size_) ;
		}

		void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> range_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_shuffle (count ,size_ ,range_) ;
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_pick (count ,size_) ;
		}

		void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet<>> range_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_pick (count ,size_ ,range_) ;
		}

		BOOL random_draw (CREF<SINGLE> possibility) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_draw (possibility) ;
		}

		BOOL random_draw (CREF<DOUBLE> possibility) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_draw (possibility) ;
		}
	} ;
} ;

using Random = typename RANDOM_HELP<DEPEND ,ALWAYS>::Random ;
} ;