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

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class TimeDuration {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimeDuration () = default ;

		imports CREF<TimeDuration> zero () {
			return memorize ([&] () {
				TimeDuration ret ;
				ret.mThis = FUNCTION_link::invoke () ;
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
	struct Holder implement Interface {
		virtual void init_now () = 0 ;
		virtual void init_epoch () = 0 ;
		virtual Auto native () const = 0 ;
		virtual void add_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
		virtual void sub_from (CREF<Holder> a ,CREF<TimeDuration> b) = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class TimePoint {
	private:
		VRef<Holder> mThis ;

	public:
		implicit TimePoint () = default ;

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
	} ;
} ;

using TimePoint = typename TIMEPOINT_HELP<DEPEND ,ALWAYS>::TimePoint ;

template <class...>
trait MUTEX_HELP ;

template <class DEPEND>
trait MUTEX_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_mutex () = 0 ;
		virtual void init_recursive_mutex () = 0 ;
		virtual void init_conditional_mutex () = 0 ;
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

		imports Mutex make_mutex () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_mutex () ;
			return move (ret) ;
		}

		imports Mutex make_recursive_mutex () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_recursive_mutex () ;
			return move (ret) ;
		}

		imports Mutex make_conditional_mutex () {
			Mutex ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_conditional_mutex () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		void enter () const {
			return mThis->enter () ;
		}

		Auto try_enter () const {
			return mThis->try_enter () ;
		}

		void leave () const {
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
		virtual Auto native () const = 0 ;
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

		imports ConditionalLock make_lock (CREF<Mutex> mutex_) {
			ConditionalLock ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_lock (mutex_) ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
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
trait THREAD_HELP ;

template <class DEPEND>
trait THREAD_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual void init_new () = 0 ;
		virtual Auto native () const = 0 ;
		virtual FLAG thread_id () const = 0 ;
		virtual void execute (RREF<Function<void>> proc) = 0 ;
		virtual void sleep (CREF<TimePoint> time_) = 0 ;
		virtual void sleep (CREF<TimeDuration> time_) = 0 ;
		virtual void yield () = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Thread {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Thread () = default ;

		imports Thread make_current () {
			Thread ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		imports Thread make_new () {
			Thread ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		FLAG thread_id () const {
			return mThis->thread_id () ;
		}

		void execute (RREF<Function<void>> proc) {
			return mThis->execute (move (proc)) ;
		}

		void sleep (CREF<TimeDuration> time_) {
			return mThis->sleep (time_) ;
		}

		void sleep (CREF<TimePoint> time_) {
			return mThis->sleep (time_) ;
		}

		void yield () {
			return mThis->yield () ;
		}
	} ;
} ;

using Thread = typename THREAD_HELP<DEPEND ,ALWAYS>::Thread ;

using PROCESS_SNAPSHOT = BoxBuffer<BYTE ,ENUMAS<VAL ,ENUMID<128>>> ;

template <class...>
trait PROCESS_HELP ;

template <class DEPEND>
trait PROCESS_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual void init_snapshot (CREF<PROCESS_SNAPSHOT> info) = 0 ;
		virtual Auto native () const = 0 ;
		virtual FLAG process_id () const = 0 ;
		virtual PROCESS_SNAPSHOT process_snapshot () const = 0 ;
		virtual void process_exit () = 0 ;
		virtual void process_abort () = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Process {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Process () = default ;

		imports Process make_current () {
			Process ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		imports Process make_snapshot (CREF<PROCESS_SNAPSHOT> info) {
			Process ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_snapshot (info) ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		FLAG process_id () const {
			return mThis->process_id () ;
		}

		PROCESS_SNAPSHOT process_snapshot () const {
			return mThis->process_snapshot () ;
		}

		void process_exit () {
			mThis->process_exit () ;
		}

		void process_abort () {
			mThis->process_abort () ;
		}
	} ;
} ;

using Process = typename PROCESS_HELP<DEPEND ,ALWAYS>::Process ;

template <class...>
trait MODULE_HELP ;

template <class DEPEND>
trait MODULE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void init_current () = 0 ;
		virtual void init_new () = 0 ;
		virtual Auto native () const = 0 ;
		virtual CREF<String<STR>> error () const leftvalue = 0 ;
		virtual CREF<String<STR>> path () const = 0 ;
		virtual CREF<String<STR>> name () const = 0 ;
		virtual void open (CREF<String<STR>> file) = 0 ;
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

		imports Module make_current () {
			Module ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_current () ;
			return move (ret) ;
		}

		imports Module make_new () {
			Module ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		CREF<String<STR>> path () const {
			return mThis->path () ;
		}

		CREF<String<STR>> name () const {
			return mThis->name () ;
		}

		void open (CREF<String<STR>> file) {
			return mThis->open (file) ;
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
				ret.mMutex = Mutex::make_recursive_mutex () ;
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
		virtual void init_new () = 0 ;
		virtual Auto native () const = 0 ;
		virtual void reset_seed (CREF<DATA> seed) = 0 ;
		virtual DATA random_byte () = 0 ;
		virtual void random_skip (CREF<LENGTH> size_) = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Random {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Random () = default ;

		imports Random make_new () {
			Random ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_new () ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		void reset_seed (CREF<DATA> seed) {
			return mThis->reset_seed (seed) ;
		}

		DATA random_byte () {
			return mThis->random_byte () ;
		}

		Array<DATA> random_byte (CREF<LENGTH> size_) {
			Array<DATA> ret = Array<DATA> (size_) ;
			for (auto &&i : ret.iter ())
				ret[i] = mThis->random_byte () ;
			return move (ret) ;
		}

		void random_skip (CREF<LENGTH> size_) {
			return mThis->random_skip (size_) ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) {
			assert (lb <= rb) ;
			const auto r1x = VAL64 (rb) - VAL64 (lb) ;
			const auto r2x = r1x + 1 ;
			const auto r3x = VAL64 (mThis->random_byte ()) ;
			return INDEX (r3x % r2x + lb) ;
		}

		Array<INDEX> random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> size_) {
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

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) {
			Array<INDEX> ret = Array<INDEX> (size_) ;
			for (auto &&i : ret.iter ())
				ret[i] = i ;
			random_shuffle (count ,size_ ,ret) ;
			return move (ret) ;
		}

		void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> ret_) {
			assert (vbetween (count ,0 ,size_ + 1)) ;
			assert (ret_.size () == size_) ;
			const auto r1x = ret_.size () - 1 ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= count)
					break ;
				INDEX iy = random_value (ix ,r1x) ;
				swap (ret_[ix] ,ret_[iy]) ;
				ix++ ;
			}
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) {
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
				const auto r1x = random_shuffle (count ,size_) ;
				for (auto &&i : iter (size_ - count ,size_))
					ret.add (r1x[i]) ;
			}
			return move (ret) ;
		}

		BOOL random_draw (CREF<SINGLE> possibility) {
			const auto r1x = random_value (0 ,10000) ;
			const auto r2x = SINGLE (r1x) * MathProc::inverse (SINGLE (10000)) ;
			if (r2x < possibility)
				return TRUE ;
			return FALSE ;
		}

		BOOL random_draw (CREF<DOUBLE> possibility) {
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