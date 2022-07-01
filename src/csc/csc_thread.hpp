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
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	struct Holder implement Binder {
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
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	struct Holder implement Binder {
		virtual void initialize () = 0 ;
		virtual void start () = 0 ;
		virtual void start (RREF<Function<Auto>> proc) = 0 ;
		virtual void post (RREF<Auto> item) = 0 ;
		virtual void rethrow (CREF<Exception> e) = 0 ;
		virtual void signal () = 0 ;
		virtual BOOL ready () = 0 ;
		virtual Auto poll () = 0 ;
		virtual Optional<Auto> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) = 0 ;
		virtual void then (RREF<Function<void ,TYPEAS<CREF<Generic>>>> proc) = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class ITEM>
trait PROMISE_HELP<ITEM ,ALWAYS> {
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	template <class ARG1>
	using CRTP_Future = typename DEPENDENT<FUTURE_HELP<ITEM ,ALWAYS> ,ARG1>::Future ;

	class Promise {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Promise () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		template <class ARG1 = DEPEND>
		CRTP_Future<ARG1> as_future () {
			return CRTP_Future<ARG1> (move (mThis)) ;
		}

		void start () {
			return mThis->start () ;
		}

		void start (RREF<Function<ITEM>> proc) {
			return mThis->start (proc.as_generic ()) ;
		}

		void post (CREF<ITEM> item) {
			return mThis->post (AutoRef<ITEM>::make (item)) ;
		}

		void post (RREF<ITEM> item) {
			return mThis->post (AutoRef<ITEM>::make (item)) ;
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
using Promise = typename PROMISE_HELP<ITEM ,ALWAYS>::Promise ;

template <class ITEM>
trait FUTURE_HELP<ITEM ,ALWAYS> {
	using Holder = typename PROMISE_HELP<ITEM ,ALWAYS>::Holder ;

	class Future {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Future () = default ;

		explicit Future (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		imports Future make_async (RREF<Function<ITEM>> proc) {
			auto rax = Promise<ITEM> () ;
			rax.start (move (proc)) ;
			return rax.as_future () ;
		}

		BOOL ready () {
			return mThis->ready () ;
		}

		ITEM poll () {
			auto rax = mThis->poll () ;
			return rax.poll (TYPEAS<ITEM>::expr) ;
		}

		Optional<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) {
			auto rax = mThis->poll (interval ,predicate) ;
			if ifnot (rax.exist ())
				return Optional<ITEM> (rax.code ()) ;
			return Optional<ITEM>::make (rax.poll (TYPEAS<ITEM>::expr)) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) {
			return mThis->then (proc.as_generic ()) ;
		}
	} ;

	class AsyncFuture implement Future {
	public:
		implicit AsyncFuture () = delete ;

		explicit AsyncFuture (RREF<Function<ITEM>> proc)
			:Future (Future::make_async (move (proc))) {}
	} ;
} ;

template <class ITEM>
using Future = typename FUTURE_HELP<ITEM ,ALWAYS>::Future ;
template <class ITEM>
using AsyncFuture = typename FUTURE_HELP<ITEM ,ALWAYS>::AsyncFuture ;
} ;