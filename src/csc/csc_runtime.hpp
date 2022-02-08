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
trait TIMEDURATION_HELP ;

template <class DEPEND>
trait TIMEDURATION_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_second (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) = 0 ;
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

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class TimeDuration {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimeDuration () = default ;

		explicit TimeDuration (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) {
			mThis = FUNCTION_link::invoke () ;
			mThis->init_second (milliseconds_ ,nanoseconds_) ;
		}

		imports CREF<TimeDuration> zero () {
			return memorize ([&] () {
				TimeDuration ret ;
				ret.mThis = FUNCTION_link::invoke () ;
				ret.mThis->init_second (0 ,0) ;
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
			ret.mThis = FUNCTION_link::invoke () ;
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
			ret.mThis = FUNCTION_link::invoke () ;
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
		virtual Auto native () const = 0 ;
		virtual void add_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
		virtual void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
		virtual CALENDAR calendar () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class TimePoint {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimePoint () = default ;

		explicit TimePoint (CREF<CALENDAR> calendar_) {
			mThis = FUNCTION_link::invoke () ;
			mThis->init_calendar (calendar_) ;
		}

		//@mark
		imports TimePoint make_now () {
			TimePoint ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_now () ;
			return move (ret) ;
		}

		imports CREF<TimePoint> epoch () {
			return memorize ([&] () {
				TimePoint ret ;
				ret.mThis = FUNCTION_link::invoke () ;
				ret.mThis->init_epoch () ;
				return move (ret) ;
			}) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		TimePoint add (CREF<TimeDuration> that) const {
			TimePoint ret ;
			ret.mThis = FUNCTION_link::invoke () ;
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
			ret.mThis = FUNCTION_link::invoke () ;
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
trait MUTEX_HELP ;

template <class DEPEND>
trait MUTEX_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual void init_recursive () = 0 ;
		virtual void init_conditional () = 0 ;
		virtual Auto native () const = 0 ;
		virtual void enter () const = 0 ;
		virtual Auto try_enter () const = 0 ;
		virtual void leave () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Mutex {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Mutex () = default ;

		//@mark
		imports Mutex make () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		//@mark
		imports Mutex make_recursive () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_recursive () ;
			return move (ret) ;
		}

		//@mark
		imports Mutex make_conditional () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_conditional () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		void enter () const {
			if (mThis == NULL)
				return ;
			return mThis->enter () ;
		}

		Auto try_enter () const {
			if (mThis == NULL)
				return Auto () ;
			return mThis->try_enter () ;
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

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class ConditionalLock {
	private:
		VRef<Holder> mThis ;

	public:
		implicit ConditionalLock () = default ;

		explicit ConditionalLock (CREF<Mutex> mutex_) {
			mThis = FUNCTION_link::invoke () ;
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
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class RuntimeProc {
	private:
		VRef<Holder> mThis ;

	public:
		imports CREF<RuntimeProc> instance () {
			return memorize ([&] () {
				RuntimeProc ret ;
				ret.mThis = FUNCTION_link::invoke () ;
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
	} ;
} ;

using RuntimeProc = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::RuntimeProc ;

template <class...>
trait THREAD_HELP ;

template <class DEPEND>
trait THREAD_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual FLAG thread_uid () const = 0 ;
		virtual void execute (RREF<Function<void>> proc) = 0 ;
		virtual void join () = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Thread {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Thread () = default ;

		//@mark
		imports Thread make () {
			Thread ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		FLAG thread_uid () const {
			return mThis->thread_uid () ;
		}

		void execute (RREF<Function<void>> proc) {
			return mThis->execute (move (proc)) ;
		}

		void join () {
			return mThis->join () ;
		}
	} ;
} ;

using Thread = typename THREAD_HELP<DEPEND ,ALWAYS>::Thread ;

template <class...>
trait PROCESS_HELP ;

template <class DEPEND>
trait PROCESS_HELP<DEPEND ,ALWAYS> {
	using SNAPSHOT = BoxBuffer<BYTE ,ENUMAS<VAL ,ENUMID<128>>> ;

	struct Holder implement Interface {
		virtual void init_snapshot (CREF<SNAPSHOT> info) = 0 ;
		virtual FLAG process_uid () const = 0 ;
		virtual SNAPSHOT snapshot () const = 0 ;
		virtual BOOL alive () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Process {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Process () = default ;

		explicit Process (CREF<SNAPSHOT> snapshot_) {
			mThis = FUNCTION_link::invoke () ;
			mThis->init_snapshot (snapshot_) ;
		}

		FLAG process_uid () const {
			return mThis->process_uid () ;
		}

		SNAPSHOT snapshot () const {
			return mThis->snapshot () ;
		}

		BOOL alive () const {
			return mThis->alive () ;
		}
	} ;
} ;

using Process = typename PROCESS_HELP<DEPEND ,ALWAYS>::Process ;

template <class...>
trait MODULE_HELP ;

template <class DEPEND>
trait MODULE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_new () = 0 ;
		virtual CREF<String<STR>> error () const leftvalue = 0 ;
		virtual CREF<String<STR>> path () const = 0 ;
		virtual CREF<String<STR>> name () const = 0 ;
		virtual void open (CREF<String<STR>> file_) = 0 ;
		virtual void close () = 0 ;
		virtual FLAG link (CREF<String<STR>> name) = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Module {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Module () = default ;

		//@mark
		imports Module make () {
			Module ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		CREF<String<STR>> path () const {
			return mThis->path () ;
		}

		CREF<String<STR>> name () const {
			return mThis->name () ;
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

template <class DEPEND>
trait SYSTEM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual String<STR> get_locale () const = 0 ;
		virtual void set_locale (CREF<String<STR>> name) const = 0 ;
		virtual void execute (CREF<String<STR>> command) const = 0 ;
		virtual CREF<String<STR>> working_path () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class System {
	private:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<System> instance () {
			return memorize ([&] () {
				System ret ;
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_link::invoke () ;
				return move (ret) ;
			}) ;
		}

		String<STR> get_locale () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->get_locale () ;
		}

		void set_locale (CREF<String<STR>> name) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->set_locale (name) ;
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

template <class DEPEND>
trait RANDOM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_device () = 0 ;
		virtual void init_seed (CREF<DATA> seed) = 0 ;
		virtual DATA seed () const = 0 ;
		virtual DATA random_byte () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Random {
	private:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		implicit Random () = default ;

		explicit Random (CREF<DATA> seed_) {
			mThis = FUNCTION_link::invoke () ;
			mThis->init_seed (seed_) ;
		}

		imports CREF<Random> device () {
			return memorize ([&] () {
				Random ret ;
				ret.mMutex = Mutex::make_recursive () ;
				ret.mThis = FUNCTION_link::invoke () ;
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
			assert (vbetween (count ,0 ,size_ + 1)) ;
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
			assert (vbetween (count ,0 ,size_ + 1)) ;
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
} ;

namespace CSC {
using namespace RUNTIME ;
} ;