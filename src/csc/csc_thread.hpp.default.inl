#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require 'csc_thread.hpp'"
#endif

namespace CSC {
template <class DEPEND>
trait WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename WORKTHREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using THREAD_QUEUE_SIZE = ENUMAS<VAL ,ENUMID<131072>> ;

	class ImplHolder implement Holder {
	protected:
		ConditionalMutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Array<Thread> mThread ;
		Set<FLAG> mThreadPoll ;
		Array<Deque<INDEX>> mThreadQueue ;
		Array<LENGTH> mThreadDCount ;
		Function<void ,TYPEAS<CREF<INDEX>>> mThreadProc ;
		Deque<INDEX> mItemQueue ;
		LENGTH mItemDCount ;
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
				auto rax = UniqueLock (mThreadMutex) ;
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
			mThreadQueue = Array<Deque<INDEX>> () ;
			mThreadDCount = Array<LENGTH> () ;
			mItemQueue = Deque<INDEX> () ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<Thread> (size_) ;
			mThreadPoll = Set<FLAG> (size_) ;
			mThreadQueue = Array<Deque<INDEX>> (size_) ;
			mThreadDCount = Array<LENGTH> (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			auto rax = Deque<INDEX> (size_) ;
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
				mThreadQueue = Array<Deque<INDEX>> (r1x) ;
				mThreadDCount = Array<LENGTH> (r1x) ;
			}
			mThreadDCount.fill (0) ;
			mItemDCount = 0 ;
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
				if (mThreadQueue[slot].empty ())
					break ;
				try_invoke ([&] () {
					INDEX ix = mThreadQueue[slot].head () ;
					mThreadProc (mThreadQueue[slot][ix]) ;
					mThreadQueue[slot].take () ;
				}) ;
			}
		}

		void poll (CREF<INDEX> slot) {
			auto rax = UniqueLock (mThreadMutex) ;
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
			if ifnot (mThreadFlag.self)
				return ;
			if ifswitch (TRUE) {
				if (mThreadQueue[slot].size () > 0)
					discard ;
				mThreadQueue[slot] = Deque<INDEX> (THREAD_QUEUE_SIZE::expr) ;
			}
			mItemDCount -= mThreadDCount[slot] ;
			const auto r1x = mItemQueue.length () + mItemDCount ;
			const auto r2x = (r1x + mThread.size () - 1) / mThread.size () / 2 ;
			const auto r3x = MathProc::min_of (r2x ,mThreadQueue[slot].size () ,mItemQueue.length ()) ;
			mThreadDCount[slot] = r3x ;
			mItemDCount += mThreadDCount[slot] ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				INDEX ix = mItemQueue.head () ;
				mThreadQueue[slot].add (mItemQueue[ix]) ;
				mItemQueue.take () ;
			}
			rax.notify () ;
		}

		void post (CREF<INDEX> item) override {
			auto rax = UniqueLock (mThreadMutex) ;
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
			auto rax = UniqueLock (mThreadMutex) ;
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
			auto rax = UniqueLock (mThreadMutex) ;
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
				auto rbx = Deque<INDEX> (r1x) ;
				for (auto &&i : mItemQueue.iter ())
					rbx.add (mItemQueue[i]) ;
				for (auto &&i : item)
					rbx.add (i) ;
				mItemQueue = move (rbx) ;
			}
			rax.notify () ;
		}

		void join () override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mItemQueue.length () == 0)
					if (mThreadPoll.length () <= mThread.length ())
						break ;
				rax.wait () ;
			}
			rax.notify () ;
		}

		BOOL join (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mItemQueue.length () == 0)
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

exports auto WORKTHREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait PROMISE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PROMISE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		ConditionalMutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Thread mThread ;
		Function<Auto> mThreadProc ;
		Function<void ,TYPEAS<CREF<Generic>>> mCallbackProc ;
		VRef<Auto> mItem ;
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
				auto rax = UniqueLock (mThreadMutex) ;
				if (mThreadFlag == NULL)
					discard ;
				mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			mThread.stop () ;
			mThread = Thread () ;
			mThreadFlag = NULL ;
			mThreadProc = Function<Auto> () ;
			mCallbackProc = Function<void ,TYPEAS<CREF<Generic>>> () ;
			mItem = NULL ;
			mException = NULL ;
		}

		void start () override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThread = Thread () ;
			mThreadProc = Function<Auto> () ;
			mCallbackProc = Function<void ,TYPEAS<CREF<Generic>>> () ;
			mItem = NULL ;
			mException = NULL ;
		}

		void start (RREF<Function<Auto>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThreadProc = move (proc) ;
			mCallbackProc = Function<void ,TYPEAS<CREF<Generic>>> () ;
			mItem = NULL ;
			mException = NULL ;
			mThread = Thread (VRef<ImplHolder>::reference (thiz) ,0) ;
			mThread.start () ;
		}

		void execute (CREF<INDEX> slot) override {
			auto rax = Optional<Auto> () ;
			try {
				rax = Optional<Auto>::make (mThreadProc ()) ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			} catch (...) {
				rethrow (Exception (TYPEAS<where>::expr ,slice ("unknown C++ exception"))) ;
			}
			if ifswitch (TRUE) {
				if ifnot (rax.exist ())
					discard ;
				post (rax.poll ()) ;
			}
			signal () ;
		}

		void post (RREF<Auto> item) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			assume (mItem == NULL) ;
			assume (mException == NULL) ;
			mItem = VRef<Auto>::make (move (item)) ;
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
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			mThreadFlag.self = FALSE ;
			if ifswitch (TRUE) {
				if (mItem == NULL)
					discard ;
				if ifnot (mCallbackProc.exist ())
					discard ;
				mCallbackProc (Generic (mItem.self)) ;
			}
			rax.notify () ;
		}

		BOOL ready () override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			if (mThreadFlag == NULL)
				return TRUE ;
			if ifnot (mThreadFlag.self)
				return TRUE ;
			return FALSE ;
		}

		Auto poll () override {
			auto rax = UniqueLock (mThreadMutex) ;
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
			Auto ret = move (mItem.self) ;
			mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		Optional<Auto> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = UniqueLock (mThreadMutex) ;
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
			Optional<Auto> ret = Optional<Auto>::make (move (mItem.self)) ;
			mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		void then (RREF<Function<void ,TYPEAS<CREF<Generic>>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			mCallbackProc = move (proc) ;
			if ifswitch (TRUE) {
				if (mThreadFlag.self)
					discard ;
				if (mItem == NULL)
					discard ;
				mCallbackProc (Generic (mItem.self)) ;
			}
		}

		void stop () override {
			mHandle = Scope<ImplHolder> () ;
		}
	} ;
} ;

template <>
exports auto PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename PROMISE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;