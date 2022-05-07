#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
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

	class WorkThread {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit WorkThread () {
			using R1X = typename WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			mThis = VRef<R1X>::make () ;
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

template <class DEPEND>
trait WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename WORKTHREAD_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		ConditionalMutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Array<Thread> mThread ;
		Set<FLAG> mThreadPoll ;
		Array<List<INDEX>> mThreadQueue ;
		Function<void ,TYPEAS<CREF<INDEX>>> mThreadProc ;
		List<INDEX> mItemQueue ;
		Scope<ImplHolder> mHandle ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHandle = Scope<ImplHolder> (thiz) ;
		}

		void enter () {
			noop () ;
		}

		void leave () {
			if ifswitch (TRUE) {
				auto rax = ConditionalLock (mThreadMutex) ;
				if (mThreadFlag == NULL)
					discard ;
				mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			for (auto &&i : mThread)
				i.stop () ;
			mThread = Array<Thread> () ;
			mThreadProc = Function<void ,TYPEAS<CREF<INDEX>>> () ;
			mThreadFlag = NULL ;
			mThreadPoll = Set<FLAG> () ;
			mThreadQueue = Array<List<INDEX>> () ;
			mItemQueue = List<INDEX> () ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<Thread> (size_) ;
			mThreadPoll = Set<FLAG> (size_) ;
			mThreadQueue = Array<List<INDEX>> (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			auto rax = List<INDEX> (size_) ;
			for (auto &&i : mItemQueue.iter ())
				rax.add (mItemQueue[i]) ;
			mItemQueue = move (rax) ;
		}

		void start (RREF<Function<void ,TYPEAS<CREF<INDEX>>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			if ifswitch (TRUE) {
				if (mThread.size () > 0)
					discard ;
				const auto r1x = RuntimeProc::thread_concurrency () ;
				mThread = Array<Thread> (r1x) ;
				mThreadPoll = Set<FLAG> (r1x) ;
				mThreadQueue = Array<List<INDEX>> (r1x) ;
			}
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThreadProc = move (proc) ;
			for (auto &&i : mThread.iter ()) {
				mThread[i] = Thread (VRef<ImplHolder>::reference (thiz) ,i) ;
				mThread[i].start () ;
			}
		}

		void execute (CREF<INDEX> slot) override {
			while (TRUE) {
				if ifswitch (TRUE) {
					if ifnot (mThreadQueue[slot].empty ())
						discard ;
					poll (slot) ;
				}
				assume (ifnot (mThreadQueue[slot].empty ())) ;
				try_invoke ([&] () {
					INDEX ix = mThreadQueue[slot].head () ;
					mThreadProc (mThreadQueue[slot][ix]) ;
					mThreadQueue[slot].take () ;
				}) ;
			}
		}

		void poll (CREF<INDEX> slot) {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mItemQueue.empty ())
					break ;
				mThreadPoll.add (slot) ;
				rax.wait () ;
			}
			mThreadPoll.erase (slot) ;
			assume (mThreadFlag.self) ;
			const auto r1x = MathProc::min_of (mItemQueue.size () ,mThread.size ()) ;
			const auto r2x = MathProc::max_of (mItemQueue.size () / 2 ,r1x) ;
			if ifswitch (TRUE) {
				if (mThreadQueue[slot].size () > 0)
					discard ;
				const auto r3x = r2x / mThread.size () ;
				mThreadQueue[slot] = List<INDEX> (r3x) ;
			}
			const auto r4x = MathProc::max_of (r2x / mThreadPoll.length () ,LENGTH (1)) ;
			for (auto &&i : iter (0 ,r4x)) {
				noop (i) ;
				INDEX ix = mItemQueue.head () ;
				mThreadQueue[slot].add (mItemQueue[ix]) ;
				mItemQueue.take () ;
			}
			rax.notify () ;
		}

		void post (CREF<INDEX> item) override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mItemQueue.full ())
					break ;
				rax.wait () ;
			}
			assume (mThreadFlag.self) ;
			mItemQueue.add (item) ;
			rax.notify () ;
		}

		BOOL post (CREF<INDEX> item ,CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mItemQueue.full ())
					break ;
				const auto r1x = predicate () ;
				if ifnot (r1x)
					return FALSE ;
				rax.wait (interval) ;
			}
			assume (mThreadFlag.self) ;
			mItemQueue.add (item) ;
			rax.notify () ;
			return TRUE ;
		}

		void post_all (CREF<Array<INDEX>> item) override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			const auto r1x = mItemQueue.length () + item.length () ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (r1x >= mItemQueue.size ())
					discard ;
				for (auto &&i : item)
					mItemQueue.add (i) ;
			}
			if ifswitch (rxx) {
				auto rbx = List<INDEX> (r1x) ;
				for (auto &&i : mItemQueue.iter ())
					rbx.add (mItemQueue[i]) ;
				for (auto &&i : item)
					rbx.add (i) ;
				mItemQueue = move (rbx) ;
			}
			rax.notify () ;
		}

		void join () override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mThreadPoll.length () <= mThread.length ())
					break ;
				rax.wait () ;
			}
			rax.notify () ;
		}

		BOOL join (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mThreadPoll.length () <= mThread.length ())
					break ;
				const auto r1x = predicate () ;
				if ifnot (r1x)
					return FALSE ;
				rax.wait (interval) ;
			}
			rax.notify () ;
			return TRUE ;
		}

		void stop () override {
			mHandle = Scope<ImplHolder> () ;
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
trait PROMISE_IMPLHOLDER_HELP ;

template <class ITEM>
trait PROMISE_HELP<ITEM ,ALWAYS> {
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	struct Holder implement Binder {
		virtual void initialize () = 0 ;
		virtual void start () = 0 ;
		virtual void start (RREF<Function<ITEM>> proc) = 0 ;
		virtual void post (RREF<ITEM> item) = 0 ;
		virtual void rethrow (CREF<Exception> e) = 0 ;
		virtual void signal () = 0 ;
		virtual BOOL ready () = 0 ;
		virtual ITEM poll () = 0 ;
		virtual Optional<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) = 0 ;
		virtual void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) = 0 ;
		virtual void stop () = 0 ;
	} ;

	template <class ARG1>
	using MACRO_Future = typename DEPENDENT<FUTURE_HELP<ITEM ,ALWAYS> ,ARG1>::Future ;

	class Promise {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Promise () {
			using R1X = typename PROMISE_IMPLHOLDER_HELP<ITEM ,ALWAYS>::ImplHolder ;
			mThis = VRef<R1X>::make () ;
			mThis->initialize () ;
		}

		template <class ARG1 = DEPEND>
		MACRO_Future<ARG1> future () {
			return MACRO_Future<ARG1> (move (mThis)) ;
		}

		void start () {
			return mThis->start () ;
		}

		void start (RREF<Function<ITEM>> proc) {
			return mThis->start (move (proc)) ;
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
trait PROMISE_IMPLHOLDER_HELP<ITEM ,ALWAYS> {
	using Holder = typename PROMISE_HELP<ITEM ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		ConditionalMutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Thread mThread ;
		Function<ITEM> mThreadProc ;
		Function<void ,TYPEAS<VREF<ITEM>>> mCallbackProc ;
		VRef<ITEM> mItem ;
		VRef<Exception> mException ;
		Scope<ImplHolder> mHandle ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mHandle = Scope<ImplHolder> (thiz) ;
		}

		void enter () {
			noop () ;
		}

		void leave () {
			if ifswitch (TRUE) {
				auto rax = ConditionalLock (mThreadMutex) ;
				if (mThreadFlag == NULL)
					discard ;
				mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			mThread.stop () ;
			mThread = Thread () ;
			mThreadFlag = NULL ;
			mThreadProc = Function<ITEM> () ;
			mCallbackProc = Function<void ,TYPEAS<VREF<ITEM>>> () ;
			mItem = NULL ;
			mException = NULL ;
		}

		void start () override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThread = Thread () ;
			mThreadProc = Function<ITEM> () ;
			mCallbackProc = Function<void ,TYPEAS<VREF<ITEM>>> () ;
			mItem = NULL ;
			mException = NULL ;
		}

		void start (RREF<Function<ITEM>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThreadProc = move (proc) ;
			mCallbackProc = Function<void ,TYPEAS<VREF<ITEM>>> () ;
			mItem = NULL ;
			mException = NULL ;
			mThread = Thread (VRef<ImplHolder>::reference (thiz) ,0) ;
			mThread.start () ;
		}

		void execute (CREF<INDEX> slot) override {
			auto rax = Optional<ITEM> () ;
			try {
				rax = Optional<ITEM>::make (mThreadProc ()) ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			} catch (...) {
				rethrow (Exception (TYPEAS<where>::id ,slice ("unknown C++ exception"))) ;
			}
			if ifswitch (TRUE) {
				if ifnot (rax.exist ())
					discard ;
				post (rax.poll ()) ;
			}
			signal () ;
		}

		void post (RREF<ITEM> item) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			assume (mItem == NULL) ;
			assume (mException == NULL) ;
			mItem = VRef<ITEM>::make (move (item)) ;
		}

		void rethrow (CREF<Exception> e) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			assume (mException == NULL) ;
			mItem = NULL ;
			mException = VRef<Exception>::make (e) ;
		}

		void signal () override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			mThreadFlag.self = FALSE ;
			if ifswitch (TRUE) {
				if (mItem == NULL)
					discard ;
				if ifnot (mCallbackProc.exist ())
					discard ;
				mCallbackProc (mItem.self) ;
			}
			rax.notify () ;
		}

		BOOL ready () {
			Scope<Mutex> anonymous (mThreadMutex) ;
			if (mThreadFlag == NULL)
				return TRUE ;
			if ifnot (mThreadFlag.self)
				return TRUE ;
			return FALSE ;
		}

		ITEM poll () override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				rax.wait () ;
			}
			if ifswitch (TRUE) {
				if (mException == NULL)
					discard ;
				mException->raise () ;
			}
			assume (mItem != NULL) ;
			ITEM ret = move (mItem.self) ;
			mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		Optional<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = ConditionalLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				const auto r1x = predicate () ;
				if ifnot (r1x)
					return FLAG (1) ;
				rax.wait (interval) ;
			}
			if ifswitch (TRUE) {
				if (mException == NULL)
					discard ;
				mException->raise () ;
			}
			assume (mItem != NULL) ;
			Optional<ITEM> ret = Optional<ITEM>::make (move (mItem.self)) ;
			mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			mCallbackProc = move (proc) ;
			if ifswitch (TRUE) {
				if (mThreadFlag.self)
					discard ;
				if (mItem == NULL)
					discard ;
				mCallbackProc (mItem.self) ;
			}
		}

		void stop () override {
			mHandle = Scope<ImplHolder> () ;
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
			return rax.future () ;
		}

		BOOL ready () {
			return mThis->ready () ;
		}

		ITEM poll () {
			return mThis->poll () ;
		}

		Optional<ITEM> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) {
			return mThis->poll (interval ,predicate) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<ITEM>>>> proc) {
			return mThis->then (move (proc)) ;
		}
	} ;

	class AsyncFuture implement Future {
	public:
		implicit AsyncFuture () = delete ;

		explicit AsyncFuture (RREF<Function<ITEM>> proc) :Future (Future::make_async (move (proc))) {}
	} ;
} ;

template <class ITEM>
using Future = typename FUTURE_HELP<ITEM ,ALWAYS>::Future ;
template <class ITEM>
using AsyncFuture = typename FUTURE_HELP<ITEM ,ALWAYS>::AsyncFuture ;
} ;