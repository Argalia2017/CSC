#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class...>
trait WORKTHREAD_HELP ;

template <class...>
trait WORKTHREAD_IMPLHOLDER_HELP ;

template <class DEPEND>
trait WORKTHREAD_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
		virtual void set_queue_size (CREF<LENGTH> size_) = 0 ;
		virtual void start (RREF<Function<void ,TYPEAS<CREF<INDEX>>>> proc) = 0 ;
		virtual void post (CREF<INDEX> item) = 0 ;
		virtual BOOL post (CREF<INDEX> item ,CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) = 0 ;
		virtual void post_all (CREF<Array<INDEX>> item) = 0 ;
		virtual void join () = 0 ;
		virtual BOOL join (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class WorkThread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit WorkThread () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		void set_thread_size (CREF<LENGTH> size_) {
			return mThis->set_thread_size (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) {
			return mThis->set_queue_size (size_) ;
		}

		void start (RREF<Function<void ,TYPEAS<CREF<INDEX>>>> proc) {
			return mThis->start (move (proc)) ;
		}

		void post (CREF<INDEX> item) {
			return mThis->post (item) ;
		}

		BOOL post (CREF<INDEX> item ,CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) {
			return mThis->post (item ,interval ,predicate) ;
		}

		void post_all (CREF<Array<INDEX>> item) {
			return mThis->post_all (item) ;
		}

		void join () {
			return mThis->join () ;
		}

		BOOL join (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) {
			return mThis->join (interval ,predicate) ;
		}

		void stop () {
			return mThis->stop () ;
		}
	} ;
} ;

using WorkThread = typename WORKTHREAD_HELP<DEPEND ,ALWAYS>::WorkThread ;

template <class...>
trait CALCTHREAD_HELP ;

template <class DEPEND>
trait CALCTHREAD_HELP<DEPEND ,ALWAYS> {
	class CalcThread ;
} ;

using CalcThread = typename CALCTHREAD_HELP<DEPEND ,ALWAYS>::CalcThread ;

template <class...>
trait PROMISE_HELP ;

template <class...>
trait FUTURE_HELP ;

template <class...>
trait PROMISE_HOLDER_HELP ;

template <class...>
trait PROMISE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PROMISE_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void start () = 0 ;
		virtual void start (RREF<Function<AutoRef<>>> proc) = 0 ;
		virtual void post (RREF<AutoRef<>> item) = 0 ;
		virtual void rethrow (CREF<Exception> e) = 0 ;
		virtual void signal () = 0 ;
		virtual BOOL ready () = 0 ;
		virtual AutoRef<> poll () = 0 ;
		virtual Cell<AutoRef<>> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) = 0 ;
		virtual void then (RREF<Function<void ,TYPEAS<VREF<AutoRef<>>>>> proc) = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class ITEM>
trait PROMISE_HELP<ITEM ,ALWAYS> {
	class Promise ;

	using Holder = typename PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;
	using Future = typename FUTURE_HELP<ITEM ,ALWAYS>::Future ;

	class Promise {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Promise () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		Future as_future () {
			Future ret ;
			ret.mThis = move (mThis) ;
			return move (ret) ;
		}

		void start () {
			return mThis->start () ;
		}

		void start (RREF<Function<ITEM>> proc) {
			return mThis->start (proc.as_wrap ([] (CREF<Function<ITEM>> old) {
				return AutoRef<ITEM>::make (old ()) ;
			})) ;
		}

		void post (CREF<ITEM> item) {
			return mThis->post (move (item)) ;
		}

		void post (RREF<ITEM> item) {
			return mThis->post (move (item)) ;
		}

		void rethrow (CREF<Exception> e) {
			return mThis->rethrow (e) ;
		}

		void signal () {
			return mThis->signal () ;
		}

		void stop () {
			return mThis->stop () ;
		}
	} ;
} ;

template <class ITEM>
trait FUTURE_HELP<ITEM ,ALWAYS> {
	using Holder = typename PROMISE_HELP<ITEM ,ALWAYS>::Holder ;
	using Promise = typename PROMISE_HELP<ITEM ,ALWAYS>::Promise ;

	class Future {
	private:
		template <class...>
		friend trait PROMISE_HELP ;

	protected:
		VRef<Holder> mThis ;

	public:
		implicit Future () = default ;

		imports Future make_async (RREF<Function<ITEM>> proc) {
			auto rax = Promise () ;
			rax.start (move (proc)) ;
			return rax.as_future () ;
		}

		BOOL ready () {
			return mThis->ready () ;
		}

		ITEM poll () {
			auto rax = mThis->poll ().as_cast (TYPEAS<ITEM>::expr) ;
			return move (rax.self) ;
		}

		Cell<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) {
			auto rax = mThis->poll (interval ,predicate) ;
			if ifnot (rax.exist ())
				return rax.code () ;
			auto rbx = rax.poll ().as_cast (TYPEAS<ITEM>::expr) ;
			return Cell<ITEM>::make (move (rbx.self)) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) {
			return mThis->then (proc.as_wrap ([] (Function<void ,TYPEAS<VREF<ITEM>>> old ,VREF<AutoRef<>> item) {
				old (AutoRef<ITEM>::from (item).self) ;
			})) ;
		}
	} ;

	class AsyncFuture extend Future {
	public:
		implicit AsyncFuture () = delete ;

		explicit AsyncFuture (RREF<Function<ITEM>> proc)
			:Future (Future::make_async (move (proc))) {}
	} ;
} ;

template <class ITEM>
using Promise = typename PROMISE_HELP<ITEM ,ALWAYS>::Promise ;

template <class ITEM>
using Future = typename FUTURE_HELP<ITEM ,ALWAYS>::Future ;
template <class ITEM>
using AsyncFuture = typename FUTURE_HELP<ITEM ,ALWAYS>::AsyncFuture ;
} ;