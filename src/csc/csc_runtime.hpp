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
template <class...>
trait TIMEDURATION_HELP ;

template <class...>
trait TIMEDURATION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEDURATION_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_second (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual LENGTH hours () const = 0 ;
		virtual LENGTH minutes () const = 0 ;
		virtual LENGTH seconds () const = 0 ;
		virtual LENGTH milliseconds () const = 0 ;
		virtual LENGTH microseconds () const = 0 ;
		virtual LENGTH nanoseconds () const = 0 ;
		virtual void add_from (CREF<Holder> a ,CREF<Holder> b) = 0 ;
		virtual void sub_from (CREF<Holder> a ,CREF<Holder> b) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TimeDuration {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit TimeDuration () = default ;

		explicit TimeDuration (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_second (milliseconds_ ,nanoseconds_) ;
		}

		imports CREF<TimeDuration> zero () {
			return memorize ([&] () {
				TimeDuration ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_second (0 ,0) ;
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
			TimeDuration ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
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
			ret.mThis = FUNCTION_extern::invoke () ;
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

using TimeDuration = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimeDuration ;

template <class...>
trait TIMEPOINT_HELP ;

template <class...>
trait TIMEPOINT_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEPOINT_HELP<DEPEND ,ALWAYS> {
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

	struct Holder implement Interface {
		virtual void init_now () = 0 ;
		virtual void init_epoch () = 0 ;
		virtual void init_calendar (CREF<CALENDAR> calendar_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual void add_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
		virtual void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
		virtual CALENDAR calendar () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TimePoint {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit TimePoint () = default ;

		explicit TimePoint (CREF<CALENDAR> calendar_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_calendar (calendar_) ;
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
			TimePoint ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->add_from (mThis ,that) ;
			return move (ret) ;
		}

		inline TimePoint operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimePoint sub (CREF<TimeDuration> that) const {
			TimePoint ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->sub_from (mThis ,that) ;
			return move (ret) ;
		}

		inline TimePoint operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}

		CALENDAR calendar () const {
			return mThis->calendar () ;
		}
	} ;
} ;

using TimePoint = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::TimePoint ;

template <class...>
trait ATOMIC_HELP ;

template <class...>
trait ATOMIC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ATOMIC_HELP<DEPEND ,ALWAYS> {
	require (IS_CLONEABLE<VAL>) ;

	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual VAL fetch () const = 0 ;
		virtual void store (CREF<VAL> obj) const = 0 ;
		virtual VAL exchange (CREF<VAL> obj) const = 0 ;
		virtual void replace (CREF<VAL> expect ,CREF<VAL> obj) const = 0 ;
		virtual BOOL change (VREF<VAL> expect ,CREF<VAL> obj) const = 0 ;
		virtual VAL fetch_add (CREF<VAL> obj) const = 0 ;
		virtual VAL fetch_sub (CREF<VAL> obj) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Atomic final {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Atomic () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_new () ;
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

		void replace (CREF<VAL> expect ,CREF<VAL> obj) const {
			return mThis->replace (expect ,obj) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> obj) const {
			return mThis->change (expect ,obj) ;
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
		virtual void init_mutex () = 0 ;
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

		imports Mutex make_mutex () {
			Mutex ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->init_mutex () ;
			return move (ret) ;
		}

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
} ;

using Mutex = typename MUTEX_HELP<DEPEND ,ALWAYS>::Mutex ;

template <class...>
trait CONDITIONALLOCK_HELP ;

template <class...>
trait CONDITIONALLOCK_IMPLHOLDER_HELP ;

template <class DEPEND>
trait CONDITIONALLOCK_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_lock (CREF<Mutex> mutex_) = 0 ;
		virtual void wait () = 0 ;
		virtual void wait (CREF<TimeDuration> time_) = 0 ;
		virtual void wait (CREF<TimePoint> time_) = 0 ;
		virtual void notify () = 0 ;
		virtual void yield () = 0 ;
	} ;

	using CONDITIONALLOCK_MAX_SIZE = ENUMAS<VAL ,ENUMID<64>> ;
	using CONDITIONALLOCK_MAX_ALIGN = ALIGN_OF<DATA> ;

	class FakeHolder implement Holder {
	protected:
		Storage<CONDITIONALLOCK_MAX_SIZE ,CONDITIONALLOCK_MAX_ALIGN> mStorage ;

	public:
		void init_lock (CREF<Mutex> mutex_) override ;
		void wait () override ;
		void wait (CREF<TimeDuration> time_) override ;
		void wait (CREF<TimePoint> time_) override ;
		void notify () override ;
		void yield () override ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class ConditionalLock {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit ConditionalLock () = default ;

		explicit ConditionalLock (CREF<Mutex> mutex_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_lock (mutex_) ;
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

using ConditionalLock = typename CONDITIONALLOCK_HELP<DEPEND ,ALWAYS>::ConditionalLock ;

template <class...>
trait RUNTIMEPROC_HELP ;

template <class...>
trait RUNTIMEPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait RUNTIMEPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
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
				return move (ret) ;
			}) ;
		}

		imports LENGTH thread_concurrency () {
			return instance ().mThis->thread_concurrency () ;
		}

		imports FLAG thread_uid () {
			return instance ().mThis->thread_uid () ;
		}

		imports FLAG thread_sleep (CREF<TimePoint> time_) {
			return instance ().mThis->thread_sleep (time_) ;
		}

		imports FLAG thread_sleep (CREF<TimeDuration> time_) {
			return instance ().mThis->thread_sleep (time_) ;
		}

		imports FLAG thread_yield () {
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
		virtual void execute (CREF<INDEX> index) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual FLAG thread_uid () const = 0 ;
		virtual void start (RREF<VRef<Binder>> binder ,CREF<INDEX> index) = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Thread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Thread () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_new () ;
		}

		FLAG thread_uid () const {
			return mThis->thread_uid () ;
		}

		void start (RREF<VRef<Binder>> binder ,CREF<INDEX> index) {
			return mThis->execute (move (binder) ,index) ;
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
	using SNAPSHOT = BoxBuffer<BYTE ,ENUMAS<VAL ,ENUMID<128>>> ;

	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual void init_snapshot (CREF<SNAPSHOT> snapshot_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual FLAG process_uid () const = 0 ;
		virtual CREF<SNAPSHOT> snapshot () const = 0 ;
		virtual BOOL equal (CREF<Holder> a) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Process {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Process () = default ;

		explicit Process (CREF<SNAPSHOT> snapshot_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_snapshot (snapshot_) ;
		}

		imports Process make_current () {
			Process ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		Auto native () const leftvalue {
			return mThis->native () ;
		}

		FLAG process_uid () const {
			return mThis->process_uid () ;
		}

		CREF<SNAPSHOT> snapshot () const {
			return mThis->snapshot () ;
		}

		BOOL equal (CREF<Process> that) const {
			return mThis->equal (that) ;
		}

		inline BOOL operator== (CREF<Process> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Process> that) const {
			return ifnot (equal (that)) ;
		}
	} ;
} ;

using Process = typename PROCESS_HELP<DEPEND ,ALWAYS>::Process ;

template <class...>
trait MODULE_HELP ;

template <class...>
trait MODULE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait MODULE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual CREF<String<STR>> error () const leftvalue = 0 ;
		virtual void open (CREF<String<STR>> file_) = 0 ;
		virtual void close () = 0 ;
		virtual FLAG link (CREF<String<STR>> name) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Module {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Module () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->init_new () ;
		}

		void open (CREF<String<STR>> file_) {
			return mThis->open (file_) ;
		}

		void close () {
			return mThis->close () ;
		}

		FLAG link (CREF<String<STR>> name) {
			return mThis->link (name) ;
		}
	} ;
} ;

using Module = typename MODULE_HELP<DEPEND ,ALWAYS>::Module ;

template <class...>
trait SYSTEM_HELP ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SYSTEM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_device () = 0 ;
		virtual String<STR> get_locale () const = 0 ;
		virtual void set_locale (RREF<String<STR>> name) const = 0 ;
		virtual void execute (CREF<String<STR>> command) const = 0 ;
		virtual CREF<String<STR>> working_path () const = 0 ;
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
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_device () ;
				return move (ret) ;
			}) ;
		}

		String<STR> get_locale () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->get_locale () ;
		}

		void set_locale (CREF<String<STR>> name) const {
			set_locale (move (name)) ;
		}

		void set_locale (RREF<String<STR>> name) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->set_locale (move (name)) ;
		}

		void execute (CREF<String<STR>> command) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->execute (command) ;
		}

		CREF<String<STR>> working_path () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
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
		virtual void init_device () = 0 ;
		virtual void init_seed (CREF<DATA> seed) = 0 ;
		virtual DATA seed () const = 0 ;
		virtual DATA random_byte () const = 0 ;
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
			mThis->init_seed (seed_) ;
		}

		imports CREF<Random> instance () {
			return memorize ([&] () {
				Random ret ;
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->init_device () ;
				return move (ret) ;
			}) ;
		}

		DATA seed () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->seed () ;
		}

		DATA random_byte () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->random_byte () ;
		}

		Array<DATA> random_byte (CREF<LENGTH> size_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			Array<DATA> ret = Array<DATA> (size_) ;
			for (auto &&i : ret.iter ())
				ret[i] = mThis->random_byte () ;
			return move (ret) ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			assert (lb <= rb) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) ;
			const auto r2x = r1x + 1 ;
			const auto r3x = VAL64 (mThis->random_byte ()) ;
			return INDEX (r3x % r2x + lb) ;
		}

		Array<INDEX> random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> size_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			assert (lb <= rb) ;
			Array<INDEX> ret = Array<INDEX> (size_) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) ;
			const auto r2x = r1x + 1 ;
			for (auto &&i : ret.iter ()) {
				const auto r3x = VAL64 (mThis->random_byte ()) ;
				ret[i] = INDEX (r3x % r2x + lb) ;
			}
			return move (ret) ;
		}

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			Array<INDEX> ret = Array<INDEX> (size_) ;
			for (auto &&i : ret.iter ())
				ret[i] = i ;
			random_shuffle (count ,size_ ,ret) ;
			return move (ret) ;
		}

		void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> range) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			assert (count >= 0) ;
			assert (count <= size_) ;
			assert (range.size () == size_) ;
			const auto r1x = range.size () - 1 ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= count)
					break ;
				INDEX iy = random_value (ix ,r1x) ;
				swap (range[ix] ,range[iy]) ;
				ix++ ;
			}
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			assert (count >= 0) ;
			assert (count <= size_) ;
			BitSet<> ret = BitSet<> (size_) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (count >= size_ / 2)
					discard ;
				const auto r1x = random_shuffle (count ,size_) ;
				for (auto &&i : iter (0 ,count))
					ret.add (r1x[i]) ;
			}
			if ifswitch (eax) {
				const auto r2x = random_shuffle (size_ - count ,size_) ;
				for (auto &&i : iter (size_ - count ,size_))
					ret.add (r2x[i]) ;
			}
			return move (ret) ;
		}

		BOOL random_draw (CREF<SINGLE> possibility) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			const auto r1x = random_value (0 ,10000) ;
			const auto r2x = SINGLE (r1x) * MathProc::inverse (SINGLE (10000)) ;
			if (r2x < possibility)
				return TRUE ;
			return FALSE ;
		}

		BOOL random_draw (CREF<DOUBLE> possibility) const {
			return random_draw (SINGLE (possibility)) ;
		}
	} ;
} ;

using Random = typename RANDOM_HELP<DEPEND ,ALWAYS>::Random ;
} ;