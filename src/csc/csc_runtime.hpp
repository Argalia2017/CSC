#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
trait TIMEDURATION_HOLDER_HELP ;

template <class...>
trait TIMEDURATION_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS> {
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

	struct Layout ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void initialize (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) = 0 ;
		virtual void initialize (CREF<CALENDAR> calendar_) = 0 ;
		virtual XRef native () const leftvalue = 0 ;
		virtual LENGTH hours () const = 0 ;
		virtual LENGTH minutes () const = 0 ;
		virtual LENGTH seconds () const = 0 ;
		virtual LENGTH milliseconds () const = 0 ;
		virtual LENGTH microseconds () const = 0 ;
		virtual LENGTH nanoseconds () const = 0 ;
		virtual CALENDAR calendar () const = 0 ;
		virtual Layout add (CREF<Layout> that) const = 0 ;
		virtual Layout sub (CREF<Layout> that) const = 0 ;
	} ;

	struct Layout {
		CRef<Holder> mThis ;
	} ;
} ;

template <class DEPEND>
trait TIMEDURATION_HELP<DEPEND ,ALWAYS> {
	class TimeDuration ;
	class TimePoint ;

	using CALENDAR = typename TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::CALENDAR ;
	using Holder = typename TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename TIMEDURATION_HOLDER_HELP<DEPEND ,ALWAYS>::Layout ;

	class TimeDuration implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit TimeDuration () = default ;

		implicit TimeDuration (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit TimeDuration (CREF<LENGTH> milliseconds_)
			:TimeDuration (milliseconds_ ,0) {}

		explicit TimeDuration (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) {
			auto rax = Holder::create () ;
			rax->initialize (milliseconds_ ,nanoseconds_) ;
			mThis = move (rax) ;
		}

		XRef native () const leftvalue {
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
			return mThis->add (that) ;
		}

		inline TimeDuration operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimeDuration sub (CREF<TimeDuration> that) const {
			return mThis->sub (that) ;
		}

		inline TimeDuration operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}

		TimePoint add (CREF<TimePoint> that) const {
			return mThis->add (that) ;
		}

		inline TimePoint operator+ (CREF<TimePoint> that) const {
			return add (that) ;
		}

		TimePoint sub (CREF<TimePoint> that) const {
			return mThis->sub (that) ;
		}

		inline TimePoint operator- (CREF<TimePoint> that) const {
			return sub (that) ;
		}
	} ;

	class TimePoint implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit TimePoint () = default ;

		implicit TimePoint (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit TimePoint (CREF<BoolProxy> ok) {
			auto rax = Holder::create () ;
			rax->initialize () ;
			mThis = move (rax) ;
		}

		explicit TimePoint (CREF<CALENDAR> calendar_) {
			auto rax = Holder::create () ;
			rax->initialize (calendar_) ;
			mThis = move (rax) ;
		}

		imports CREF<TimePoint> epoch () {
			return memorize ([&] () {
				TimePoint ret ;
				auto rax = Holder::create () ;
				rax->initialize (0 ,0) ;
				ret.mThis = move (rax) ;
				return move (ret) ;
			}) ;
		}

		XRef native () const leftvalue {
			return mThis->native () ;
		}

		TimePoint add (CREF<TimeDuration> that) const {
			return mThis->add (that) ;
		}

		inline TimePoint operator+ (CREF<TimeDuration> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<TimeDuration> that) {
			thiz = add (that) ;
		}

		TimePoint sub (CREF<TimeDuration> that) const {
			return mThis->sub (that) ;
		}

		inline TimePoint operator- (CREF<TimeDuration> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<TimeDuration> that) {
			thiz = sub (that) ;
		}

		TimeDuration sub (CREF<TimePoint> that) const {
			return mThis->sub (that) ;
		}

		inline TimeDuration operator- (CREF<TimePoint> that) const {
			return sub (that) ;
		}

		CALENDAR calendar () const {
			return mThis->calendar () ;
		}
	} ;

	class NowTimePoint implement Proxy {
	public:
		imports TimePoint make () {
			return TimePoint (TRUE) ;
		}
	} ;
} ;

using TimeDuration = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimeDuration ;

using TimePoint = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::TimePoint ;
using NowTimePoint = typename TIMEDURATION_HELP<DEPEND ,ALWAYS>::NowTimePoint ;

template <class...>
trait ATOMIC_HELP ;

template <class...>
trait ATOMIC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ATOMIC_HELP<DEPEND ,ALWAYS> {
	require (IS_CLONEABLE<VAL>) ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<VAL> init_) = 0 ;
		virtual VAL fetch () const = 0 ;
		virtual void store (CREF<VAL> obj) const = 0 ;
		virtual VAL exchange (CREF<VAL> obj) const = 0 ;
		virtual void replace (CREF<VAL> expect ,CREF<VAL> next) const = 0 ;
		virtual BOOL change (VREF<VAL> expect ,CREF<VAL> next) const = 0 ;
		virtual VAL add_with (CREF<VAL> obj) const = 0 ;
		virtual VAL sub_with (CREF<VAL> obj) const = 0 ;
	} ;

	class Atomic {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Atomic () = default ;

		explicit Atomic (CREF<VAL> init_) {
			mThis = Holder::create () ;
			mThis->initialize (init_) ;
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

		void replace (CREF<VAL> expect ,CREF<VAL> next) const {
			return mThis->replace (expect ,next) ;
		}

		BOOL change (VREF<VAL> expect ,CREF<VAL> next) const {
			return mThis->change (expect ,next) ;
		}

		VAL add_with (CREF<VAL> obj) const {
			return mThis->add_with (obj) ;
		}

		VAL sub_with (CREF<VAL> obj) const {
			return mThis->sub_with (obj) ;
		}

		VAL increase () const {
			return add_with (VAL (1)) ;
		}

		VAL decrease () const {
			return sub_with (VAL (1)) ;
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
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<LENGTH> cond_size) = 0 ;
		virtual XRef native () const leftvalue = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	class Mutex {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Mutex () = default ;

		explicit Mutex (CREF<LENGTH> cond_size) {
			mThis = Holder::create () ;
			mThis->initialize (cond_size) ;
		}

		XRef native () const leftvalue {
			return mThis->native () ;
		}

		void enter () {
			if (mThis == NULL)
				return ;
			return mThis->enter () ;
		}

		void leave () {
			if (mThis == NULL)
				return ;
			return mThis->leave () ;
		}
	} ;

	class RecursiveMutex implement Proxy {
	public:
		imports Mutex make () {
			return Mutex (0) ;
		}
	} ;

	class ConditionalMutex implement Proxy {
	public:
		imports Mutex make () {
			return Mutex (1) ;
		}
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
		virtual void notify () = 0 ;
		virtual void yield () = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,128> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FakeImplHolder {
		imports Box<FakeHolder> create () ;
	} ;

	class UniqueLock {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit UniqueLock () = default ;

		explicit UniqueLock (CREF<Mutex> mutex_) {
			mThis = FakeImplHolder::create () ;
			mThis->initialize (mutex_) ;
		}

		void wait () {
			return mThis->wait () ;
		}

		void wait (CREF<TimeDuration> time_) {
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
		virtual BOOL busy () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUMAS<VAL ,128> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FakeImplHolder {
		imports Box<FakeHolder> create () ;
	} ;

	class SharedLock {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit SharedLock () = default ;

		explicit SharedLock (CREF<Mutex> mutex_) {
			mThis = FakeImplHolder::create () ;
			mThis->initialize (mutex_) ;
		}

		BOOL busy () const {
			return mThis->busy () ;
		}

		void enter () {
			return mThis->enter () ;
		}

		void leave () {
			return mThis->leave () ;
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
		imports VRef<Holder> create () ;

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
		virtual CREF<String<STR>> module_path () const = 0 ;
		virtual CREF<String<STR>> module_name () const = 0 ;
	} ;

	class RuntimeProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<RuntimeProc> instance () {
			return memorize ([&] () {
				RuntimeProc ret ;
				ret.mThis = Holder::create () ;
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

		imports CREF<String<STR>> module_path () {
			return instance ().mThis->module_path () ;
		}

		imports CREF<String<STR>> module_name () {
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
		virtual void friend_execute (CREF<INDEX> slot) {
			return keep[TYPEAS<CREF<Binder>>::expr] (thiz).friend_execute (slot) ;
		}

		virtual void friend_execute (CREF<INDEX> slot) const = 0 ;
	} ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (RREF<VRef<Binder>> binder ,CREF<INDEX> slot) = 0 ;
		virtual void initialize (RREF<CRef<Binder>> binder ,CREF<INDEX> slot) = 0 ;
		virtual FLAG thread_uid () const = 0 ;
		virtual void start () = 0 ;
		virtual void stop () = 0 ;
	} ;

	class Thread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Thread () = default ;

		explicit Thread (RREF<VRef<Binder>> binder ,CREF<INDEX> slot) {
			mThis = Holder::create () ;
			mThis->initialize (move (binder) ,slot) ;
		}

		explicit Thread (RREF<CRef<Binder>> binder ,CREF<INDEX> slot) {
			mThis = Holder::create () ;
			mThis->initialize (move (binder) ,slot) ;
		}

		FLAG thread_uid () const {
			return mThis->thread_uid () ;
		}

		void start () {
			if (mThis == NULL)
				return ;
			return mThis->start () ;
		}

		void stop () {
			if (mThis == NULL)
				return ;
			return mThis->stop () ;
		}
	} ;
} ;

using Thread = typename THREAD_HELP<DEPEND ,ALWAYS>::Thread ;

template <class...>
trait THREADLOCAL_HELP ;

template <class...>
trait THREADLOCAL_IMPLHOLDER_HELP ;

template <class DEPEND>
trait THREADLOCAL_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual INDEX local () const = 0 ;
	} ;

	class ThreadLocal {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit ThreadLocal () = default ;

		explicit ThreadLocal (CREF<BoolProxy> ok) {
			mThis = Holder::create () ;
			mThis->initialize () ;
		}

		INDEX local () const {
			return mThis->local () ;
		}
	} ;
} ;

using ThreadLocal = typename THREADLOCAL_HELP<DEPEND ,ALWAYS>::ThreadLocal ;

template <class...>
trait PROCESS_HELP ;

template <class...>
trait PROCESS_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PROCESS_HELP<DEPEND ,ALWAYS> {
	using SNAPSHOT = ConBuffer<BYTE> ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<FLAG> uid) = 0 ;
		virtual void initialize (CREF<SNAPSHOT> snapshot_) = 0 ;
		virtual FLAG process_uid () const = 0 ;
		virtual SNAPSHOT snapshot () const = 0 ;
	} ;

	class Process {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Process () = default ;

		explicit Process (CREF<FLAG> uid) {
			mThis = Holder::create () ;
			mThis->initialize (uid) ;
		}

		explicit Process (CREF<SNAPSHOT> snapshot_) {
			mThis = Holder::create () ;
			mThis->initialize (snapshot_) ;
		}

		FLAG process_uid () const {
			return mThis->process_uid () ;
		}

		SNAPSHOT snapshot () const {
			return mThis->snapshot () ;
		}
	} ;

	class CurrentProcess implement Proxy {
	public:
		imports Process make () {
			return Process (RuntimeProc::process_uid ()) ;
		}
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
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void initialize (CREF<String<STR>> file) = 0 ;
		virtual CREF<String<STR>> error () const leftvalue = 0 ;
		virtual FLAG link (CREF<String<STR>> name) = 0 ;
	} ;

	class Module {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Module () = default ;

		explicit Module (CREF<BoolProxy> ok) {
			mThis = Holder::create () ;
			mThis->initialize () ;
		}

		explicit Module (CREF<String<STR>> file) {
			mThis = Holder::create () ;
			mThis->initialize (file) ;
		}

		CREF<String<STR>> error () const leftvalue {
			return mThis->error () ;
		}

		FLAG link (CREF<String<STR>> name) {
			return mThis->link (name) ;
		}
	} ;

	class CurrentModule implement Proxy {
	public:
		imports Module make () {
			return Module (TRUE) ;
		}
	} ;
} ;

using Module = typename MODULE_HELP<DEPEND ,ALWAYS>::Module ;
using CurrentModule = typename MODULE_HELP<DEPEND ,ALWAYS>::CurrentModule ;

template <class...>
trait SYSTEM_HELP ;

template <class...>
trait SYSTEM_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SYSTEM_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual String<STR> get_locale () const = 0 ;
		virtual void set_locale (CREF<String<STR>> name) const = 0 ;
		virtual FLAG execute (CREF<String<STR>> command) const = 0 ;
		virtual String<STR> working_path () const = 0 ;
	} ;

	class System {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<System> instance () {
			return memorize ([&] () {
				System ret ;
				ret.mMutex = RecursiveMutex::make () ;
				ret.mThis = Holder::create () ;
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

		FLAG execute (CREF<String<STR>> command) const {
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
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void initialize (CREF<DATA> seed) = 0 ;
		virtual DATA seed () const = 0 ;
		virtual DATA random_byte () const = 0 ;
		virtual void random_byte (VREF<Array<DATA>> result) const = 0 ;
		virtual INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const = 0 ;
		virtual void random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<Array<INDEX>> result) const = 0 ;
		virtual Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
		virtual void random_shuffle (CREF<LENGTH> count ,VREF<Array<INDEX>> result) const = 0 ;
		virtual BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
		virtual void random_pick (CREF<LENGTH> count ,VREF<BitSet<>> result) const = 0 ;
		virtual BOOL random_draw (CREF<FLT64> possibility) const = 0 ;
		virtual void random_draw (CREF<FLT64> possibility ,VREF<Array<BOOL>> result) const = 0 ;
	} ;

	class Random {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		implicit Random () = default ;

		explicit Random (CREF<DATA> seed_) {
			mThis = Holder::create () ;
			mThis->initialize (seed_) ;
		}

		imports CREF<Random> instance () {
			return memorize ([&] () {
				Random ret ;
				ret.mMutex = RecursiveMutex::make () ;
				ret.mThis = Holder::create () ;
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

		void random_byte (VREF<Array<DATA>> result) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_byte (result) ;
		}

		INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_value (lb ,rb) ;
		}

		void random_value (CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<Array<INDEX>> result) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_value (lb ,rb ,result) ;
		}

		Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_shuffle (count ,size_) ;
		}

		void random_shuffle (CREF<LENGTH> count ,VREF<Array<INDEX>> result) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_shuffle (count ,result) ;
		}

		BitSet<> random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_pick (count ,size_) ;
		}

		void random_pick (CREF<LENGTH> count ,VREF<BitSet<>> result) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_pick (count ,result) ;
		}

		BOOL random_draw (CREF<FLT64> possibility) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_draw (possibility) ;
		}

		void random_draw (CREF<FLT64> possibility ,VREF<Array<BOOL>> result) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->random_draw (possibility ,result) ;
		}
	} ;
} ;

using Random = typename RANDOM_HELP<DEPEND ,ALWAYS>::Random ;

template <class...>
trait GLOBAL_HELP ;

template <class...>
trait GLOBAL_IMPLHOLDER_HELP ;

template <class DEPEND>
trait GLOBAL_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void startup () const = 0 ;
		virtual VREF<AutoRef<>> unique (CREF<Slice<STR>> name) const leftvalue = 0 ;
		virtual void shutdown () const = 0 ;
	} ;

	class Global {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Global> instance () {
			return memorize ([&] () {
				Global ret ;
				ret.mMutex = RecursiveMutex::make () ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		void startup () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->startup () ;
		}

		VREF<AutoRef<>> unique (CREF<Slice<STR>> name) const leftvalue {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->unique (name) ;
		}

		template <class ARG1>
		CREF<ARG1> unique (CREF<Slice<STR>> name ,TYPEID<ARG1> id) const leftvalue {
			Scope<Mutex> anonymous (mMutex) ;
			auto &&tmp = mThis->unique (name) ;
			assume (tmp.exist ()) ;
			return AutoRef<ARG1>::from (tmp).self ;
		}

		void shutdown () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->shutdown () ;
		}
	} ;
} ;

using Global = typename GLOBAL_HELP<DEPEND ,ALWAYS>::Global ;

template <class...>
trait FLT32TON_HELP ;

template <class...>
trait FLT32TON_HOLDER_HELP ;

template <class...>
trait FLT32TON_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FLT32TON_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void regi (CREF<Slice<STR>> name ,CREF<FLAG> addr) const = 0 ;
		virtual FLAG link (CREF<Slice<STR>> name) const = 0 ;
	} ;

	class Singleton {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<Singleton> instance () {
			return memorize ([&] () {
				Singleton ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}
		
		void regi (CREF<Slice<STR>> name ,CREF<FLAG> addr) const {
			return mThis->regi (name ,addr) ;
		}
		
		FLAG link (CREF<Slice<STR>> name) const {
			return mThis->link (name) ;
		}
	} ;
} ;

template <class UNIT>
trait FLT32TON_HELP<UNIT ,ALWAYS> {
	using Holder = typename FLT32TON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename FLT32TON_HOLDER_HELP<DEPEND ,ALWAYS>::Singleton ;

	class Singleton implement Proxy {
	public:
		imports CREF<UNIT> instance () {
			return memorize ([&] () {
				auto rax = ZERO ;
				if ifswitch (TRUE) {
					auto &&tmp = Super::instance () ;
					const auto r2x = Clazz (TYPEAS<UNIT>::expr).type_name () ;
					rax = tmp.link (r2x) ;
					if (rax != ZERO)
						discard ;
					const auto r3x = address (UNIT::instance ()) ;
					tmp.regi (r2x ,r3x) ;
					rax = tmp.link (r2x) ;
				}
				assume (rax != ZERO) ;
				auto &&tmp_2 = unsafe_cast[TYPEAS<UNIT>::expr] (unsafe_deref (rax)) ;
				return CRef<UNIT>::reference (tmp_2) ;
			}).self ;
		}
	} ;
} ;

template <class UNIT>
using Singleton = typename FLT32TON_HELP<UNIT ,ALWAYS>::Singleton ;
} ;