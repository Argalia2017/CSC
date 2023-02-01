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
		virtual void start (RREF<Function<void ,TYPEAS<INDEX>>> proc) = 0 ;
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
		implicit WorkThread () = default ;

		explicit WorkThread (CREF<typeof (PH0)>) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		void set_thread_size (CREF<LENGTH> size_) {
			return mThis->set_thread_size (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) {
			return mThis->set_queue_size (size_) ;
		}

		void start (RREF<Function<void ,TYPEAS<INDEX>>> proc) {
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

template <class...>
trait CALCTHREAD_HOLDER_HELP ;

template <class...>
trait CALCTHREAD_IMPLHOLDER_HELP ;

template <class DEPEND>
trait CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS> {
	using ITEM = CRef<BitSet<>> ;

	struct SOLUTION {
		INDEX mIndex ;
		DOUBLE mError ;
		ITEM mValue ;
	} ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
		virtual void start (RREF<Function<SOLUTION ,TYPEAS<SOLUTION>>> proc) = 0 ;
		virtual SOLUTION best () = 0 ;
		virtual void suspend () = 0 ;
		virtual void resume () = 0 ;
		virtual void stop () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class DEPEND>
trait CALCTHREAD_HELP<DEPEND ,ALWAYS> {
	using ITEM = typename CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS>::ITEM ;
	using SOLUTION = typename CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS>::SOLUTION ;
	using Holder = typename CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	class CalcThread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit CalcThread () = default ;

		explicit CalcThread (CREF<typeof (PH0)>) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		void set_thread_size (CREF<LENGTH> size_) {
			return mThis->set_thread_size (size_) ;
		}

		void start (RREF<Function<SOLUTION ,TYPEAS<SOLUTION>>> proc) {
			return mThis->start (move (proc)) ;
		}

		SOLUTION best () {
			return mThis->best () ;
		}

		void suspend () {
			return mThis->suspend () ;
		}

		void resume () {
			return mThis->resume () ;
		}

		void stop () {
			return mThis->stop () ;
		}
	} ;
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
		virtual VRef<Holder> clone () const = 0 ;
		virtual void start () const = 0 ;
		virtual void start (RREF<Function<AutoRef<>>> proc) const = 0 ;
		virtual void post (RREF<AutoRef<>> item) const = 0 ;
		virtual void rethrow (CREF<Exception> e) const = 0 ;
		virtual void signal () const = 0 ;
		virtual BOOL ready () const = 0 ;
		virtual AutoRef<> poll () const = 0 ;
		virtual Optional<AutoRef<>> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) const = 0 ;
		virtual void then (RREF<Function<void ,TYPEAS<VREF<AutoRef<>>>>> proc) const = 0 ;
		virtual void stop () const = 0 ;
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
		implicit Promise () = default ;

		explicit Promise (CREF<typeof (PH0)>) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		implicit Promise (CREF<Promise> that) {
			if (that.mThis == NULL)
				return ;
			mThis = that.mThis->clone () ;
		}

		inline VREF<Promise> operator= (CREF<Promise> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Promise (RREF<Promise> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Promise> operator= (RREF<Promise> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		Future future () const {
			Future ret ;
			ret.mThis = mThis->clone () ;
			return move (ret) ;
		}

		BOOL ready () const {
			return mThis->ready () ;
		}

		void start () const {
			return mThis->start () ;
		}

		void start (RREF<Function<ITEM>> proc) const {
			return mThis->start (proc.as_wrap ([] (CREF<Function<ITEM>> old) {
				return AutoRef<ITEM>::make (old ()).as_cast (TYPEAS<void>::expr) ;
			})) ;
		}

		void post (CREF<ITEM> item) const {
			return mThis->post (move (item)) ;
		}

		void post (RREF<ITEM> item) const {
			return mThis->post (move (item)) ;
		}

		void rethrow (CREF<Exception> e) const {
			return mThis->rethrow (e) ;
		}

		void signal () const {
			return mThis->signal () ;
		}

		void stop () const {
			return mThis->stop () ;
		}
	} ;
} ;

template <class ITEM>
trait FUTURE_HELP<ITEM ,ALWAYS> {
	class Future ;

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

		implicit Future (CREF<Future> that) {
			if (that.mThis == NULL)
				return ;
			mThis = that.mThis->clone () ;
		}

		inline VREF<Future> operator= (CREF<Future> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Future (RREF<Future> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Future> operator= (RREF<Future> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL ready () const {
			return mThis->ready () ;
		}

		ITEM poll () const {
			auto rax = mThis->poll ().as_cast (TYPEAS<ITEM>::expr) ;
			return move (rax.self) ;
		}

		Optional<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) const {
			auto rax = mThis->poll (interval ,predicate) ;
			if ifnot (rax.exist ())
				return rax.code () ;
			auto rbx = rax.poll ().as_cast (TYPEAS<ITEM>::expr) ;
			return Optional<ITEM>::make (move (rbx.self)) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) const {
			return mThis->then (proc.as_wrap ([] (Function<void ,TYPEAS<VREF<ITEM>>> old ,VREF<AutoRef<>> item) {
				old (AutoRef<ITEM>::from (item).self) ;
			})) ;
		}
	} ;

	class AsyncFuture extend Proxy {
	public:
		imports Future make (RREF<Function<ITEM>> proc) {
			auto rax = Promise (PH0) ;
			rax.start (move (proc)) ;
			return rax.future () ;
		}
	} ;
} ;

template <class ITEM>
using Promise = typename PROMISE_HELP<ITEM ,ALWAYS>::Promise ;

template <class ITEM>
using Future = typename FUTURE_HELP<ITEM ,ALWAYS>::Future ;
template <class ITEM>
using AsyncFuture = typename FUTURE_HELP<ITEM ,ALWAYS>::AsyncFuture ;
} ;