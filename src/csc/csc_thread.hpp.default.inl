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
		Array<VRef<Thread>> mThread ;
		BitSet<> mThreadJoin ;
		Function<void ,TYPEAS<INDEX>> mThreadProc ;
		Deque<INDEX> mWaitQueue ;
		Array<INDEX> mLoadQueue ;
		Atomic mLoadRead ;
		Atomic mLoadCheck ;
		Atomic mLoadWrite ;

	public:
		void initialize () override {
			mThreadMutex = ConditionalMutex::make () ;
			//@warn: two condition_variable in same mutex
			mThreadJoinMutex = ConditionalMutex::make () ;
			set_thread_size (RuntimeProc::thread_concurrency ()) ;
			set_queue_size (RuntimeProc::thread_concurrency ()) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<VRef<Thread>> (size_) ;
			mThreadJoin = BitSet<> (size_) ;
		}

		void set_queue_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			assume (mWaitQueue.empty ()) ;
			mWaitQueue = Deque<INDEX> (size_) ;
			mLoadQueue = Array<INDEX> (THREAD_QUEUE_SIZE::expr) ;
			mLoadRead = Atomic (0) ;
			mLoadCheck = Atomic (0) ;
			mLoadWrite = Atomic (0) ;
		}

		void start (RREF<Function<void ,TYPEAS<INDEX>>> proc) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			assume (mThread.length () > 0) ;
			mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThreadProc = move (proc) ;
			for (auto &&i : mThread.iter ()) {
				mThread[i] = VRef<Thread>::make (VRef<Binder>::reference (thiz) ,i) ;
				mThread[i]->start () ;
			}
		}

		void friend_execute (CREF<INDEX> slot) override {
			while (TRUE) {
				auto rax = NONE ;
				while (TRUE) {
					INDEX ix = mLoadRead.increase () - 1 ;
					if (ix >= mLoadWrite.fetch ())
						break ;
					rax = mLoadQueue[ix] ;
					mLoadCheck.increase () ;
					try_invoke ([&] () {
						mThreadProc (rax) ;
					}) ;
				}
				poll (slot) ;
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
				if ifnot (mWaitQueue.empty ())
					break ;
				if (mLoadRead.fetch () < mLoadWrite.fetch ())
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
			if (mLoadRead.fetch () < mLoadWrite.fetch ())
				return ;
			//@warn: should not be dead lock
			while (TRUE) {
				if (mLoadCheck.fetch () == mLoadWrite.fetch ())
					break ;
				RuntimeProc::thread_yield () ;
			}
			const auto r1x = MathProc::min_of (mWaitQueue.length () ,mLoadQueue.length ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				INDEX ix = mWaitQueue.head () ;
				mLoadQueue[i] = mWaitQueue[ix] ;
				mWaitQueue.take () ;
			}
			mLoadRead.store (0) ;
			mLoadCheck.store (0) ;
			mLoadWrite.store (r1x) ;
			rax.notify () ;
		}

		void post (CREF<INDEX> item) override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if ifnot (mWaitQueue.full ())
					break ;
				rax.wait () ;
			}
			assume (mThreadFlag.self) ;
			mWaitQueue.add (item) ;
			rax.notify () ;
		}

		void post_all (CREF<Array<INDEX>> item) override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			assume (mThreadFlag.self) ;
			const auto r1x = mWaitQueue.length () + item.length () ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x >= mWaitQueue.size ())
					discard ;
				for (auto &&i : item)
					mWaitQueue.add (i) ;
			}
			if ifswitch (act) {
				auto rbx = Deque<INDEX> (r1x) ;
				for (auto &&i : mWaitQueue.iter ())
					rbx.add (mWaitQueue[i]) ;
				for (auto &&i : item)
					rbx.add (i) ;
				mWaitQueue = move (rbx) ;
			}
			rax.notify () ;
		}

		void join () override {
			auto rax = UniqueLock (mThreadMutex) ;
			assume (mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThreadFlag.self)
					break ;
				if (mWaitQueue.empty ())
					if (mThreadJoin.length () >= mThread.length ())
						break ;
				rax.wait () ;
			}
		}

		void stop () override {
			if ifswitch (TRUE) {
				auto rax = UniqueLock (mThreadMutex) ;
				if (mThreadFlag == NULL)
					discard ;
				mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			for (auto &&i : mThread) {
				if (i == NULL)
					continue ;
				i->stop () ;
			}
			mThread = Array<VRef<Thread>> () ;
			mThreadProc = Function<void ,TYPEAS<INDEX>> () ;
			mThreadFlag = NULL ;
			mThreadJoin = BitSet<> () ;
			mWaitQueue.clear () ;
			mLoadRead.store (0) ;
			mLoadCheck.store (0) ;
			mLoadWrite.store (0) ;
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
		Array<VRef<Thread>> mThread ;
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
			set_thread_size (RuntimeProc::thread_concurrency ()) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void set_thread_size (CREF<LENGTH> size_) override {
			Scope<Mutex> anonymous (mThreadMutex) ;
			assume (mThreadFlag == NULL) ;
			mThread = Array<VRef<Thread>> (size_) ;
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
				mThread[i] = VRef<Thread>::make (VRef<Binder>::reference (thiz) ,i) ;
				mThread[i]->start () ;
			}
		}

		void friend_execute (CREF<INDEX> slot) override {
			while (TRUE) {
				mThreadSolution[slot] = SOLUTION () ;
				search_new (slot) ;
				search_xor (slot) ;
			}
		}

		void friend_execute (CREF<INDEX> slot) const override {
			assert (FALSE) ;
		}

		void search_new (CREF<INDEX> slot) {
			while (TRUE) {
				wait_solution (slot) ;
				while (TRUE) {
					wait_suspend (slot) ;
					if ifnot (mThreadSolution[slot].mValue.exist ())
						break ;
					mSearchSolution[slot] = mThreadProc (mThreadSolution[slot]) ;
					if ifnot (mSearchSolution[slot].mValue.exist ())
						break ;
					if (mSearchSolution[slot].mError < mThreadSolution[slot].mError)
						break ;
				}
				if (mSearchSolution[slot].mValue.exist ())
					break ;
			}
		}

		void search_xor (CREF<INDEX> slot) {
			while (TRUE) {
				accept_solution (slot) ;
				if ifnot (mBranchSolution[slot].mValue.exist ())
					break ;
				const auto r1x = bitset_xor (mThreadSolution[slot].mValue.self ,mSearchSolution[slot].mValue.self) ;
				const auto r2x = bitset_xor (mThreadSolution[slot].mValue.self ,mBranchSolution[slot].mValue.self) ;
				const auto r3x = (r1x & r2x).length () ;
				if (r3x == 0)
					break ;
				mThreadSolution[slot].mIndex = NONE ;
				mThreadSolution[slot].mError = DOUBLE_INF ;
				mThreadSolution[slot].mValue = ITEM::make (bitset_xor (mBranchSolution[slot].mValue.self ,r1x)) ;
				mSearchSolution[slot] = mThreadProc (mThreadSolution[slot]) ;
				if ifnot (mSearchSolution[slot].mValue.exist ())
					break ;
				if (mSearchSolution[slot].mError >= mBranchSolution[slot].mError)
					break ;
			}
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
				if ifnot (mThreadSolution[slot].mValue.exist ())
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
			for (auto &&i : mThread) {
				if (i == NULL)
					continue ;
				i->stop () ;
			}
			mThread = Array<VRef<Thread>> () ;
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

	struct PACK {
		Mutex mThreadMutex ;
		VRef<BOOL> mThreadFlag ;
		Atomic mThreadReady ;
		Thread mThread ;
		Function<AutoRef<>> mThreadProc ;
		Function<void ,TYPEAS<VREF<AutoRef<>>>> mCallbackProc ;
		VRef<AutoRef<>> mItem ;
		VRef<Exception> mException ;
	} ;

	class ImplHolder implement Together<Holder ,Binder> {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize () override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mThreadMutex = ConditionalMutex::make () ;
			mThis->mThreadReady = Atomic (TRUE) ;
		}

		void finalize () override {
			try_invoke ([&] () {
				stop () ;
			}) ;
		}

		void start () const override {
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag == NULL) ;
			mThis->mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThis->mThreadReady.store (FALSE) ;
			mThis->mThread = Thread () ;
			mThis->mThreadProc = Function<AutoRef<>> () ;
			mThis->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mThis->mItem = NULL ;
			mThis->mException = NULL ;
		}

		void start (RREF<Function<AutoRef<>>> proc) const override {
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag == NULL) ;
			mThis->mThreadFlag = VRef<BOOL>::make (TRUE) ;
			mThis->mThreadReady.store (FALSE) ;
			mThis->mThreadProc = move (proc) ;
			mThis->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mThis->mItem = NULL ;
			mThis->mException = NULL ;
			mThis->mThread = Thread (CRef<Binder>::reference (thiz) ,0) ;
			mThis->mThread.start () ;
		}

		void friend_execute (CREF<INDEX> slot) override {
			assert (FALSE) ;
		}

		void friend_execute (CREF<INDEX> slot) const override {
			auto rax = Optional<AutoRef<>> () ;
			try {
				rax = Optional<AutoRef<>>::make (mThis->mThreadProc ()) ;
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
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag != NULL) ;
			assume (mThis->mThreadFlag.self) ;
			assume (mThis->mItem == NULL) ;
			assume (mThis->mException == NULL) ;
			mThis->mItem = VRef<AutoRef<>>::make (move (item)) ;
		}

		void rethrow (CREF<Exception> e) const override {
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag != NULL) ;
			assume (mThis->mThreadFlag.self) ;
			assume (mThis->mException == NULL) ;
			mThis->mItem = NULL ;
			mThis->mException = VRef<Exception>::make (e) ;
		}

		void signal () const override {
			auto rax = UniqueLock (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag != NULL) ;
			assume (mThis->mThreadFlag.self) ;
			mThis->mThreadFlag.self = FALSE ;
			mThis->mThreadReady.store (TRUE) ;
			if ifswitch (TRUE) {
				if (mThis->mItem == NULL)
					discard ;
				if ifnot (mThis->mCallbackProc.exist ())
					discard ;
				mThis->mCallbackProc (mThis->mItem.self) ;
			}
			rax.notify () ;
		}

		BOOL ready () const override {
			if ifnot (mThis->mThreadReady.fetch ())
				return FALSE ;
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			if (mThis->mThreadFlag == NULL)
				return TRUE ;
			if ifnot (mThis->mThreadFlag.self)
				return TRUE ;
			return FALSE ;
		}

		AutoRef<> poll () const override {
			auto rax = UniqueLock (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag != NULL) ;
			while (TRUE) {
				if ifnot (mThis->mThreadFlag.self)
					break ;
				rax.wait () ;
			}
			if ifswitch (TRUE) {
				if (mThis->mException == NULL)
					discard ;
				mThis->mException->raise () ;
			}
			assume (mThis->mItem != NULL) ;
			AutoRef<> ret = move (mThis->mItem.self) ;
			mThis->mItem = NULL ;
			rax.notify () ;
			return move (ret) ;
		}

		void then (RREF<Function<void ,TYPEAS<VREF<AutoRef<>>>>> proc) const override {
			Scope<Mutex> anonymous (mThis->mThreadMutex) ;
			assume (mThis->mThreadFlag != NULL) ;
			mThis->mCallbackProc = move (proc) ;
			if ifswitch (TRUE) {
				if (mThis->mThreadFlag.self)
					discard ;
				if (mThis->mItem == NULL)
					discard ;
				mThis->mCallbackProc (mThis->mItem.self) ;
			}
		}

		void stop () const override {
			if ifswitch (TRUE) {
				auto rax = UniqueLock (mThis->mThreadMutex) ;
				if (mThis->mThreadFlag == NULL)
					discard ;
				mThis->mThreadFlag.self = FALSE ;
				rax.notify () ;
			}
			mThis->mThread.stop () ;
			mThis->mThread = Thread () ;
			mThis->mThreadFlag = NULL ;
			mThis->mThreadProc = Function<AutoRef<>> () ;
			mThis->mCallbackProc = Function<void ,TYPEAS<VREF<AutoRef<>>>> () ;
			mThis->mItem = NULL ;
			mThis->mException = NULL ;
		}
	} ;
} ;

template <>
exports auto PROMISE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename PROMISE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;