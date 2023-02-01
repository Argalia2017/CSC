#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require 'csc_thread.hpp'"
#endif

#include "csc_thread.hpp"

namespace CSC {
template <class DEPEND>
trait WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename WORKTHREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	using THREAD_QUEUE_SIZE = ENUMAS<VAL ,65536> ;

	class ImplHolder implement Together<Holder ,Binder> {
	protected:
		Mutex mThreadMutex ;
		Mutex mThreadJoinMutex ;
		VRef<BOOL> mThreadFlag ;
		Array<Thread> mThread ;
		BitSet<> mThreadJoin ;
		Array<Deque<INDEX>> mThreadQueue ;
		Array<LENGTH> mThreadLoadCount ;
		Function<void ,TYPEAS<INDEX>> mThreadProc ;
		Deque<INDEX> mItemQueue ;
		LENGTH mItemLoadCount ;

	public:
		void initialize () override {
			mThreadMutex = ConditionalMutex::make () ;
			//@warn: two condition_variable in same mutex
			mThreadJoinMutex = ConditionalMutex::make () ;
			set_thread_size (1) ;
			set_queue_size (1) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<Thread> (size_) ;
			mThreadJoin = BitSet<> (size_) ;
			mThreadQueue = Array<Deque<INDEX>> (size_) ;
			mThreadLoadCount = Array<LENGTH> (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			assume (mItemQueue.length () == 0) ;
			mItemQueue = Deque<INDEX> (size_) ;
		}

		void start (RREF<Function<void ,TYPEAS<INDEX>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			assume (mThread.length () > 0) ;
			mThreadLoadCount.fill (0) ;
			mItemLoadCount = 0 ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThreadProc = move (proc) ;
			for (auto &&i : mThread.iter ()) {
				mThread[i] = Thread (VRef<Binder>::reference (thiz) ,i) ;
				mThread[i].start () ;
			}
		}

		void friend_execute (CREF<INDEX> slot) override {
			while (TRUE) {
				if ifswitch (TRUE) {
					if ifnot (mThreadQueue[slot].empty ())
						discard ;
					poll (slot) ;
				}
				try_invoke ([&] () {
					INDEX ix = mThreadQueue[slot].head () ;
					mThreadProc (mThreadQueue[slot][ix]) ;
					mThreadQueue[slot].take () ;
				}) ;
			}
		}

		void friend_execute (CREF<INDEX> slot) const override {
			assert (FALSE) ;
		}

		void poll (CREF<INDEX> slot) {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mItemQueue.empty ())
					break ;
				if ifswitch (TRUE) {
					if (mThreadJoin[slot])
						discard ;
					mThreadJoin.add (slot) ;
					rax.notify () ;
				}
				rax.wait () ;
			}
			assume (mThreadFlag.self) ;
			mThreadJoin.erase (slot) ;
			if ifswitch (TRUE) {
				if (mThreadQueue[slot].size () > 0)
					discard ;
				mThreadQueue[slot] = Deque<INDEX> (THREAD_QUEUE_SIZE::expr) ;
			}
			mItemLoadCount -= mThreadLoadCount[slot] ;
			const auto r1x = mItemQueue.length () + mItemLoadCount ;
			const auto r2x = valign (r1x ,mThread.size ()) / mThread.size () ;
			const auto r3x = MathProc::max_of (r2x / 2 ,LENGTH (1)) ;
			const auto r4x = MathProc::min_of (r3x ,mThreadQueue[slot].size () ,mItemQueue.length ()) ;
			mThreadLoadCount[slot] = r4x ;
			mItemLoadCount += mThreadLoadCount[slot] ;
			for (auto &&i : iter (0 ,r4x)) {
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x >= mItemQueue.size ())
					discard ;
				for (auto &&i : item)
					mItemQueue.add (i) ;
			}
			if ifswitch (act) {
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
					if (mThreadJoin.length () >= mThread.length ())
						break ;
				rax.wait () ;
			}
		}

		BOOL join (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mItemQueue.length () == 0)
					if (mThreadJoin.length () >= mThread.length ())
						break ;
				const auto r1x = predicate () ;
				if ifnot (r1x)
					return FALSE ;
				rax.wait (interval) ;
			}
			return TRUE ;
		}

		void stop () override {
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
			mThreadProc = Function<void ,TYPEAS<INDEX>> () ;
			mThreadFlag = NULL ;
			mThreadJoin = BitSet<> () ;
			mThreadQueue = Array<Deque<INDEX>> () ;
			mThreadLoadCount = Array<LENGTH> () ;
			mItemQueue = Deque<INDEX> () ;
		}
	} ;
} ;

template <>
exports auto WORKTHREAD_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename WORKTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait CALCTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using ITEM = typename CALCTHREAD_HELP<DEPEND ,ALWAYS>::ITEM ;
	using SOLUTION = typename CALCTHREAD_HELP<DEPEND ,ALWAYS>::SOLUTION ;
	using Holder = typename CALCTHREAD_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	class ImplHolder implement Together<Holder ,Binder> {
	protected:
		Mutex mThreadMutex ;
		Mutex mThreadJoinMutex ;
		VRef<BOOL> mThreadFlag ;
		BOOL mSuspendFlag ;
		Array<Thread> mThread ;
		BitSet<> mThreadJoin ;
		Function<SOLUTION ,TYPEAS<SOLUTION>> mThreadProc ;
		Array<SOLUTION> mThreadSolution ;
		Array<SOLUTION> mSearchSolution ;
		Array<SOLUTION> mBranchSolution ;
		SOLUTION mBestSolution ;

	public:
		void initialize () override {
			mThreadMutex = ConditionalMutex::make () ;
			//@warn: two condition_variable in same mutex
			mThreadJoinMutex = ConditionalMutex::make () ;
			set_thread_size (1) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<Thread> (size_) ;
			mThreadJoin = BitSet<> (size_) ;
			mThreadSolution = Array<SOLUTION> (size_) ;
			mSearchSolution = Array<SOLUTION> (size_) ;
			mBranchSolution = Array<SOLUTION> (size_) ;
			mBestSolution.mIndex = NONE ;
			mBestSolution.mError = DOUBLE_INF ;
			mBestSolution.mValue = ITEM::make () ;
		}

		void start (RREF<Function<SOLUTION ,TYPEAS<SOLUTION>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			assume (mThread.length () > 0) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mSuspendFlag = FALSE ;
			mThreadProc = move (proc) ;
			for (auto &&i : mThread.iter ()) {
				mThread[i] = Thread (VRef<Binder>::reference (thiz) ,i) ;
				mThread[i].start () ;
			}
		}

		void friend_execute (CREF<INDEX> slot) override {
			while (TRUE) {
				while (TRUE) {
					wait_solution (slot) ;
					while (TRUE) {
						wait_suspend (slot) ;
						if (mThreadSolution[slot].mValue == NULL)
							break ;
						mSearchSolution[slot] = mThreadProc (mThreadSolution[slot]) ;
						if (mSearchSolution[slot].mValue == NULL)
							break ;
						if (mSearchSolution[slot].mError < mThreadSolution[slot].mError)
							break ;
					}
					if (mSearchSolution[slot].mValue != NULL)
						break ;
				}
				while (TRUE) {
					accept_solution (slot) ;
					if (mBranchSolution[slot].mValue == NULL)
						break ;
					const auto r10x = bitset_xor (mThreadSolution[slot].mValue.self ,mSearchSolution[slot].mValue.self) ;
					const auto r11x = bitset_xor (mThreadSolution[slot].mValue.self ,mBranchSolution[slot].mValue.self) ;
					const auto r12x = (r10x & r11x).length () ;
					if (r12x == 0)
						break ;
					mThreadSolution[slot].mIndex = NONE ;
					mThreadSolution[slot].mError = DOUBLE_INF ;
					mThreadSolution[slot].mValue = CRef<BitSet<>>::make (bitset_xor (mBranchSolution[slot].mValue.self ,r10x)) ;
					mSearchSolution[slot] = mThreadProc (mThreadSolution[slot]) ;
					if (mSearchSolution[slot].mValue == NULL)
						break ;
					if (mSearchSolution[slot].mError >= mBranchSolution[slot].mError)
						break ;
				}
				mThreadSolution[slot] = SOLUTION () ;
			}
		}

		void friend_execute (CREF<INDEX> slot) const override {
			assert (FALSE) ;
		}

		BitSet<> bitset_xor (CREF<BitSet<>> bitset1 ,CREF<BitSet<>> bitset2) const {
			if (bitset1.size () == 0)
				return bitset2 ;
			if (bitset2.size () == 0)
				return bitset1 ;
			return bitset1 ^ bitset2 ;
		}

		void wait_solution (CREF<INDEX> slot) {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mSuspendFlag)
					break ;
				if (mThreadSolution[slot].mValue == NULL)
					break ;
				const auto r1x = address (mBestSolution.mValue.self) ;
				const auto r2x = address (mThreadSolution[slot].mValue.self) ;
				if (r1x != r2x)
					break ;
				rax.wait () ;
			}
			assume (mThreadFlag.self) ;
			mThreadSolution[slot] = mBestSolution ;
		}

		void accept_solution (CREF<INDEX> slot) {
			auto rax = UniqueLock (mThreadMutex) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (address (mBestSolution.mValue.self) != address (mThreadSolution[slot].mValue.self))
					discard ;
				mBestSolution = move (mSearchSolution[slot]) ;
				mBranchSolution[slot] = SOLUTION () ;
				rax.notify () ;
			}
			if ifswitch (act) {
				mBranchSolution[slot] = mBestSolution ;
			}
		}

		void wait_suspend (CREF<INDEX> slot) {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mSuspendFlag)
					break ;
				if ifswitch (TRUE) {
					if (mThreadJoin[slot])
						discard ;
					mThreadJoin.add (slot) ;
					rax.notify () ;
				}
				rax.wait () ;
			}
			assume (mThreadFlag.self) ;
			mThreadJoin.erase (slot) ;
		}

		SOLUTION best () override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			return mBestSolution ;
		}

		void suspend () override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			mSuspendFlag = TRUE ;
			rax.notify () ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mSuspendFlag)
					break ;
				if (mThreadJoin.length () >= mThread.length ())
					break ;
				rax.wait () ;
			}
		}

		void resume () override {
			auto rax = UniqueLock (mThreadMutex) ;
			mSuspendFlag = FALSE ;
			rax.notify () ;
		}

		void stop () override {
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
			mThreadProc = Function<SOLUTION ,TYPEAS<SOLUTION>> () ;
			mThreadFlag = NULL ;
			mThreadJoin = BitSet<> () ;
			mThreadSolution = Array<SOLUTION> () ;
			mSearchSolution = Array<SOLUTION> () ;
			mBranchSolution = Array<SOLUTION> () ;
			mBestSolution = SOLUTION () ;
		}
	} ;
} ;

template <>
exports auto CALCTHREAD_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename CALCTHREAD_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait PROMISE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PROMISE_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename THREAD_HELP<DEPEND ,ALWAYS>::Binder ;

	struct HEAP {
		Mutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Thread mThread ;
		Function<AutoRef<>> mThreadProc ;
		Function<void ,TYPEAS<VREF<AutoRef<>>>> mCallbackProc ;
		VRef<AutoRef<>> mItem ;
		VRef<Exception> mException ;
	} ;

	class ImplHolder implement Together<Holder ,Binder> {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize () override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mThreadMutex = ConditionalMutex::make () ;
		}

		VRef<Holder> clone () const override {
			auto rax = VRef<ImplHolder>::make () ;
			rax->mHeap = mHeap ;
			return rax.as_cast (TYPEAS<Holder>::expr) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void start () const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag == NULL) ;
			mHeap->mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mHeap->mThread = Thread () ;
			mHeap->mThreadProc = Function<AutoRef<>> () ;
			mHeap->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mHeap->mItem = NULL ;
			mHeap->mException = NULL ;
		}

		void start (RREF<Function<AutoRef<>>> proc) const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag == NULL) ;
			mHeap->mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mHeap->mThreadProc = move (proc) ;
			mHeap->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mHeap->mItem = NULL ;
			mHeap->mException = NULL ;
			mHeap->mThread = Thread (CRef<Binder>::reference (thiz) ,0) ;
			mHeap->mThread.start () ;
		}

		void friend_execute (CREF<INDEX> slot) override {
			assert (FALSE) ;
		}

		void friend_execute (CREF<INDEX> slot) const override {
			auto rax = Optional<AutoRef<>> () ;
			try {
				rax = Optional<AutoRef<>>::make (mHeap->mThreadProc ()) ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			} catch (...) {
				rethrow (Exception (slice ("unknown C++ exception"))) ;
			}
			if ifswitch (TRUE) {
				if ifnot (rax.available ())
					discard ;
				post (rax.poll ()) ;
			}
			signal () ;
		}

		void post (RREF<AutoRef<>> item) const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			assume (mHeap->mThreadFlag.self) ;
			assume (mHeap->mItem == NULL) ;
			assume (mHeap->mException == NULL) ;
			mHeap->mItem = VRef<AutoRef<>>::make (move (item)) ;
		}

		void rethrow (CREF<Exception> e) const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			assume (mHeap->mThreadFlag.self) ;
			assume (mHeap->mException == NULL) ;
			mHeap->mItem = NULL ;
			mHeap->mException = VRef<Exception>::make (e) ;
		}

		void signal () const override {
			auto rax = UniqueLock (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			assume (mHeap->mThreadFlag.self) ;
			mHeap->mThreadFlag.self = FALSE ;
			if ifswitch (TRUE) {
				if (mHeap->mItem == NULL)
					discard ;
				if ifnot (mHeap->mCallbackProc.exist ())
					discard ;
				mHeap->mCallbackProc (mHeap->mItem.self) ;
			}
			rax.notify () ;
		}

		BOOL ready () const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			if (mHeap->mThreadFlag == NULL)
				return TRUE ;
			if ifnot (mHeap->mThreadFlag.self)
				return TRUE ;
			return FALSE ;
		}

		AutoRef<> poll () const override {
			auto rax = UniqueLock (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mHeap->mThreadFlag.self)
					break ;
				rax.wait () ;
			}
			if ifswitch (TRUE) {
				if (mHeap->mException == NULL)
					discard ;
				mHeap->mException->raise () ;
			}
			assume (mHeap->mItem != NULL) ;
			AutoRef<> ret = move (mHeap->mItem.self) ;
			mHeap->mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		Optional<AutoRef<>> poll (CREF<TimeDuration> interval ,CREF<Function<BOOL>> predicate) const override {
			auto rax = UniqueLock (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mHeap->mThreadFlag.self)
					break ;
				const auto r1x = predicate () ;
				if ifnot (r1x)
					return FLAG (1) ;
				rax.wait (interval) ;
			}
			if ifswitch (TRUE) {
				if (mHeap->mException == NULL)
					discard ;
				mHeap->mException->raise () ;
			}
			assume (mHeap->mItem != NULL) ;
			Optional<AutoRef<>> ret = Optional<AutoRef<>>::make (move (mHeap->mItem.self)) ;
			mHeap->mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<AutoRef<>>>>> proc) const override {
			Scope<Mutex> anonymous (mHeap->mThreadMutex) ;
			assume (mHeap->mThreadFlag != NULL) ;
			mHeap->mCallbackProc = move (proc) ;
			if ifswitch (TRUE) {
				if (mHeap->mThreadFlag.self)
					discard ;
				if (mHeap->mItem == NULL)
					discard ;
				mHeap->mCallbackProc (mHeap->mItem.self) ;
			}
		}

		void stop () const override {
			if ifswitch (TRUE) {
				auto rax = UniqueLock (mHeap->mThreadMutex) ;
				if (mHeap->mThreadFlag == NULL)
					discard ;
				mHeap->mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			mHeap->mThread.stop () ;
			mHeap->mThread = Thread () ;
			mHeap->mThreadFlag = NULL ;
			mHeap->mThreadProc = Function<AutoRef<>> () ;
			mHeap->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mHeap->mItem = NULL ;
			mHeap->mException = NULL ;
		}
	} ;
} ;

template <>
exports auto PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename PROMISE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;