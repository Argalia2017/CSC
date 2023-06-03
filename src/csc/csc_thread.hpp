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
		virtual void post_all (CREF<Array<INDEX>> item) = 0 ;
		virtual void join () = 0 ;
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

		explicit WorkThread (CREF<BoolProxy> ok) {
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

		void post_all (CREF<Array<INDEX>> item) {
			return mThis->post_all (item) ;
		}

		void join () {
			return mThis->join () ;
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
	using ITEM = SharedRef<BitSet<>> ;

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

		explicit CalcThread (CREF<BoolProxy> ok) {
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
		virtual void start () const = 0 ;
		virtual void start (RREF<Function<AutoRef<>>> proc) const = 0 ;
		virtual void post (RREF<AutoRef<>> item) const = 0 ;
		virtual void rethrow (CREF<Exception> e) const = 0 ;
		virtual void signal () const = 0 ;
		virtual BOOL ready () const = 0 ;
		virtual AutoRef<> poll () const = 0 ;
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
		CRef<Holder> mThis ;

	public:
		implicit Promise () = default ;

		explicit Promise (CREF<BoolProxy> ok) {
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize () ;
			mThis = rax.as_cref () ;
		}

		Future future () const {
			Future ret ;
			ret.mThis = mThis ;
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
			return mThis->post (AutoRef<ITEM>::make (move (item))) ;
		}

		void post (RREF<ITEM> item) const {
			return mThis->post (AutoRef<ITEM>::make (move (item))) ;
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
		CRef<Holder> mThis ;

	public:
		implicit Future () = default ;

		BOOL ready () const {
			return mThis->ready () ;
		}

		ITEM poll () const {
			auto rax = mThis->poll ().as_cast (TYPEAS<ITEM>::expr) ;
			return move (rax.self) ;
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
			auto rax = Promise (TRUE) ;
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