﻿#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_thread.hpp"

namespace CSC {
struct ThreadFlag {
	enum {
		Preparing ,
		Running ,
		Finishing ,
		Closing ,
		ETC
	} ;
} ;

struct WorkThreadImplLayout {
	Mutex mThreadMutex ;
	JustInt<ThreadFlag> mThreadFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Array<Deque<INDEX>> mThreadQueue ;
	Array<LENGTH> mThreadLoadCount ;
	Function<CREF<INDEX>> mThreadFunc ;
	Box<VFat<ThreadFriend>> mThreadFriend ;
	Deque<INDEX> mItemQueue ;
	LENGTH mItemLoadCount ;
} ;

class WorkThreadImplement implement WorkThreadImplLayout {
private:
	using THREAD_QUEUE_SIZE = ENUM<65536> ;

protected:
	using WorkThreadImplLayout::mThreadMutex ;
	using WorkThreadImplLayout::mThreadFlag ;
	using WorkThreadImplLayout::mThread ;
	using WorkThreadImplLayout::mThreadJoin ;
	using WorkThreadImplLayout::mThreadQueue ;
	using WorkThreadImplLayout::mThreadLoadCount ;
	using WorkThreadImplLayout::mThreadFunc ;
	using WorkThreadImplLayout::mThreadFriend ;
	using WorkThreadImplLayout::mItemQueue ;
	using WorkThreadImplLayout::mItemLoadCount ;

public:
	implicit WorkThreadImplement () = default ;

	implicit ~WorkThreadImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
		set_queue_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mThreadQueue = Array<Deque<INDEX>> (size_) ;
		mThreadLoadCount = Array<LENGTH> (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mItemQueue.empty ()) ;
		mItemQueue = Deque<INDEX> (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.length () > 0) ;
		mThreadLoadCount.fill (0) ;
		mItemLoadCount = 0 ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = func ;
		mThreadFriend = Box<VFat<ThreadFriend>>::make (ThreadFriendBinder<WorkThreadImplement>::create (thiz)) ;
		for (auto &&i : iter (0 ,mThread.size ())) {
			mThread[i] = Thread (Ref<ThreadFriend>::reference (mThreadFriend.self) ,i) ;
			mThread[i].start () ;
		}
	}

	void crash () {
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		try {
			while (TRUE) {
				if ifdo (TRUE) {
					if ((!mThreadQueue[slot].empty ()))
						discard ;
					poll (slot) ;
				}
				INDEX ix = mThreadQueue[slot].head () ;
				mThreadFunc (mThreadQueue[slot][ix]) ;
				mThreadQueue[slot].take () ;
			}
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void poll (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if ((!mItemQueue.empty ()))
				break ;
			if ifdo (TRUE) {
				if (mThreadJoin[slot])
					discard ;
				mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		mThreadJoin.erase (slot) ;
		if ifdo (TRUE) {
			if (mThreadQueue[slot].size () > 0)
				discard ;
			mThreadQueue[slot] = Deque<INDEX> (THREAD_QUEUE_SIZE::expr) ;
		}
		mItemLoadCount -= mThreadLoadCount[slot] ;
		const auto r1x = mItemQueue.length () + mItemLoadCount ;
		const auto r2x = inline_alignas (r1x ,mThread.size ()) / mThread.size () ;
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

	void post (CREF<INDEX> item) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mItemQueue.add (item) ;
		rax.notify () ;
	}

	void post (CREF<Array<INDEX>> item) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		for (auto &&i : item)
			mItemQueue.add (i) ;
		rax.notify () ;
	}

	void join () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mItemQueue.empty ())
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			rax.wait () ;
		}
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mItemQueue.empty () == 0)
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			auto rbx = FALSE ;
			rax = UniqueLock () ;
			predicate (rbx) ;
			rax = UniqueLock (mThreadMutex) ;
			if ((!rbx))
				return FALSE ;
			rax.wait (interval) ;
		}
		return TRUE ;
	}

	void stop () {
		crash () ;
		for (auto &&i : iter (0 ,mThread.size ()))
			mThread[i].stop () ;
		mThread = Array<Thread> () ;
		mThreadFunc = Function<CREF<INDEX>> () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadJoin = BitSet () ;
		mThreadQueue = Array<Deque<INDEX>> () ;
		mThreadLoadCount = Array<LENGTH> () ;
		mItemQueue = Deque<INDEX> () ;
	}
} ;

class WorkThreadImplHolder implement Fat<WorkThreadHolder ,WorkThreadLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<WorkThreadImplement>::make () ;
		return ptr (fake).initialize () ;
	}

	imports VREF<WorkThreadImplement> ptr (CREF<WorkThreadLayout> layout) {
		return keep[TYPE<WorkThreadImplement>::expr] (layout.mThis.self) ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const override {
		return ptr (fake).start (func) ;
	}

	void post (CREF<INDEX> item) const override {
		return ptr (fake).post (item) ;
	}

	void post (CREF<Array<INDEX>> item) const override {
		return ptr (fake).post (item) ;
	}

	void join () const override {
		return ptr (fake).join () ;
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) const override {
		return ptr (fake).join (interval ,predicate) ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<WorkThreadHolder> WorkThreadHolder::create (VREF<WorkThreadLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::create (CREF<WorkThreadLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

struct CalcThreadImplLayout {
	Mutex mThreadMutex ;
	JustInt<ThreadFlag> mThreadFlag ;
	BOOL mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<CREF<CalcSolution> ,VREF<CalcSolution>> mThreadFunc ;
	Box<VFat<ThreadFriend>> mThreadFriend ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	CalcSolution mBestSolution ;
	BOOL mHasBestSolution ;
} ;

class CalcThreadImplement implement CalcThreadImplLayout {
protected:
	using CalcThreadImplLayout::mThreadMutex ;
	using CalcThreadImplLayout::mThreadFlag ;
	using CalcThreadImplLayout::mSuspendFlag ;
	using CalcThreadImplLayout::mThread ;
	using CalcThreadImplLayout::mThreadJoin ;
	using CalcThreadImplLayout::mThreadFunc ;
	using CalcThreadImplLayout::mThreadFriend ;
	using CalcThreadImplLayout::mThreadSolution ;
	using CalcThreadImplLayout::mSearchSolution ;
	using CalcThreadImplLayout::mBestSolution ;
	using CalcThreadImplLayout::mHasBestSolution ;

public:
	implicit CalcThreadImplement () = default ;

	implicit ~CalcThreadImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mThreadSolution = Array<CalcSolution> (size_) ;
		mSearchSolution = Array<CalcSolution> (size_) ;
		mBestSolution.mIndex = NONE ;
		mBestSolution.mError = FLT64_INF ;
		mBestSolution.mInput = BitSet () ;
		mHasBestSolution = FALSE ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.length () > 0) ;
		mThreadFlag = ThreadFlag::Running ;
		mSuspendFlag = FALSE ;
		mThreadFunc = func ;
		mThreadFriend = Box<VFat<ThreadFriend>>::make (ThreadFriendBinder<CalcThreadImplement>::create (thiz)) ;
		for (auto &&i : iter (0 ,mThread.size ())) {
			mThread[i] = Thread (Ref<ThreadFriend>::reference (mThreadFriend.self) ,i) ;
			mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) {
		try {
			while (TRUE) {
				search_new (slot) ;
				search_xor (slot) ;
			}
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void search_new (CREF<INDEX> slot) {
		while (TRUE) {
			wait_solution (slot) ;
			while (TRUE) {
				wait_suspend (slot) ;
				mThreadFunc (mThreadSolution[slot] ,mSearchSolution[slot]) ;
				if (mSearchSolution[slot].mIndex == NONE)
					break ;
				if (mSearchSolution[slot].mError < mThreadSolution[slot].mError)
					break ;
			}
			if (mSearchSolution[slot].mIndex != NONE)
				break ;
		}
	}

	void search_xor (CREF<INDEX> slot) {
		while (TRUE) {
			const auto r1x = accept_solution (slot) ;
			if (r1x)
				break ;
			mThreadFunc (mThreadSolution[slot] ,mSearchSolution[slot]) ;
			if (mSearchSolution[slot].mIndex == NONE)
				break ;
			if (mSearchSolution[slot].mError >= mThreadSolution[slot].mError)
				break ;
		}
	}

	BitSet bitset_xor (CREF<BitSet> bitset1 ,CREF<BitSet> bitset2) const {
		if (bitset1.size () == 0)
			return bitset2.clone () ;
		if (bitset2.size () == 0)
			return bitset1.clone () ;
		return bitset1 ^ bitset2 ;
	}

	void wait_solution (CREF<INDEX> slot) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadSolution[slot].mIndex == mBestSolution.mIndex)
			return ;
		mThreadSolution[slot] = mBestSolution.clone () ;
	}

	BOOL accept_solution (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mThreadSolution[slot].mIndex != mBestSolution.mIndex)
				if (mSearchSolution[slot].mError >= mBestSolution.mError)
					discard ;
			mBestSolution.mIndex++ ;
			mBestSolution.mError = mSearchSolution[slot].mError ;
			mBestSolution.mInput = mSearchSolution[slot].mInput.clone () ;
			mHasBestSolution = TRUE ;
			rax.notify () ;
		}
		if ifdo (act) {
			assume (mBestSolution.mIndex != NONE) ;
			const auto r1x = bitset_xor (mThreadSolution[slot].mInput ,mSearchSolution[slot].mInput) ;
			const auto r2x = bitset_xor (mThreadSolution[slot].mInput ,mBestSolution.mInput) ;
			const auto r3x = (r1x & r2x).length () ;
			if (r3x == 0)
				discard ;
			mThreadSolution[slot].mIndex = mBestSolution.mIndex ;
			mThreadSolution[slot].mError = mBestSolution.mError ;
			mThreadSolution[slot].mInput = bitset_xor (mBestSolution.mInput ,r1x) ;
			return TRUE ;
		}
		return FALSE ;
	}

	void wait_suspend (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if ((!mSuspendFlag))
				break ;
			if ifdo (TRUE) {
				if (mThreadJoin[slot])
					discard ;
				mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		mThreadJoin.erase (slot) ;
	}

	CalcSolution best () {
		Scope<Mutex> anonymous (mThreadMutex) ;
		return mBestSolution.clone () ;
	}

	void join () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (mHasBestSolution)
				break ;
			rax.wait () ;
		}
		mHasBestSolution = FALSE ;
	}

	void suspend () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mSuspendFlag = TRUE ;
		rax.notify () ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if ((!mSuspendFlag))
				break ;
			if (mThreadJoin.length () >= mThread.length ())
				break ;
			rax.wait () ;
		}
	}

	void resume () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mSuspendFlag = FALSE ;
		rax.notify () ;
	}

	void stop () {
		crash () ;
		for (auto &&i : iter (0 ,mThread.size ()))
			mThread[i].stop () ;
		mThread = Array<Thread> () ;
		mThreadFunc = Function<CREF<CalcSolution> ,VREF<CalcSolution>> () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadJoin = BitSet () ;
		mThreadSolution = Array<CalcSolution> () ;
		mSearchSolution = Array<CalcSolution> () ;
		mBestSolution = CalcSolution () ;
	}
} ;

class CalcThreadImplHolder implement Fat<CalcThreadHolder ,CalcThreadLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<CalcThreadImplement>::make () ;
		return ptr (fake).initialize () ;
	}

	imports VREF<CalcThreadImplement> ptr (CREF<CalcThreadLayout> layout) {
		return keep[TYPE<CalcThreadImplement>::expr] (layout.mThis.self) ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return ptr (fake).set_thread_size (size_) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const override {
		return ptr (fake).start (func) ;
	}

	CalcSolution best () const override {
		return ptr (fake).best () ;
	}

	void join () const override {
		return ptr (fake).join () ;
	}

	void suspend () const override {
		return ptr (fake).suspend () ;
	}

	void resume () const override {
		return ptr (fake).resume () ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<CalcThreadHolder> CalcThreadHolder::create (VREF<CalcThreadLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::create (CREF<CalcThreadLayout> that) {
	return CFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

struct PromiseImplLayout {
	Mutex mThreadMutex ;
	JustInt<ThreadFlag> mThreadFlag ;
	Ref<Thread> mThread ;
	Function<> mThreadFunc ;
	Function<> mRunningFunc ;
	Box<VFat<ThreadFriend>> mThreadFriend ;
	Box<AutoRef<Pointer>> mItem ;
	Box<Exception> mException ;
	BOOL mRetryFlag ;
} ;

class PromiseImplement implement PromiseImplLayout {
protected:
	using PromiseImplLayout::mThreadMutex ;
	using PromiseImplLayout::mThreadFlag ;
	using PromiseImplLayout::mThread ;
	using PromiseImplLayout::mThreadFunc ;
	using PromiseImplLayout::mRunningFunc ;
	using PromiseImplLayout::mThreadFriend ;
	using PromiseImplLayout::mItem ;
	using PromiseImplLayout::mException ;
	using PromiseImplLayout::mRetryFlag ;

public:
	implicit PromiseImplement () = default ;

	implicit ~PromiseImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		set_retry (FALSE) ;
	}

	void set_retry (CREF<BOOL> flag) {
		Scope<Mutex> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mRetryFlag = flag ;
	}

	void start () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread == NULL) ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = Function<> () ;
		mItem = NULL ;
		mException = NULL ;
		rax.notify () ;
	}

	void start (CREF<Function<>> func) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = func ;
		mItem = NULL ;
		mException = NULL ;
		if ifdo (TRUE) {
			if (mThread != NULL)
				discard ;
			mThreadFriend = Box<VFat<ThreadFriend>>::make (ThreadFriendBinder<PromiseImplement>::create (thiz)) ;
			mThread = Ref<Thread>::make (Ref<ThreadFriend>::reference (mThreadFriend.self) ,0) ;
			mThread->start () ;
		}
		rax.notify () ;
	}

	void friend_execute (CREF<INDEX> slot) {
		while (TRUE) {
			try {
				mRunningFunc = mThreadFunc ;
				mRunningFunc () ;
			} catch (CREF<Exception> e) {
				rethrow (e) ;
			}
			const auto r2x = wait_future () ;
			if ((!r2x))
				break ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (mThreadMutex) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	BOOL wait_future () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Finishing)
				break ;
			rax.wait () ;
		}
		if (mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		if ifdo (TRUE) {
			if (mRetryFlag)
				discard ;
			mThreadFlag = ThreadFlag::Preparing ;
			rax.notify () ;
		}
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Preparing)
				break ;
			rax.wait () ;
		}
		if (mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		return TRUE ;
	}

	void post (RREF<AutoRef<Pointer>> item) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		assume (mItem == NULL) ;
		mItem = Box<AutoRef<Pointer>>::make (move (item)) ;
		mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	void rethrow (CREF<Exception> e) {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		assume (mException == NULL) ;
		mException = Box<Exception>::make (e) ;
		mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	BOOL ready () {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	BOOL running () {
		Scope<Mutex> anonymous (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Running)
			return TRUE ;
		if (mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	AutoRef<Pointer> poll () {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Running)
				break ;
			rax.wait () ;
		}
		assume (mThreadFlag != ThreadFlag::Closing) ;
		if ifdo (TRUE) {
			if (mException == NULL)
				discard ;
			mException->raise () ;
		}
		AutoRef<Pointer> ret ;
		if ifdo (TRUE) {
			if (mItem == NULL)
				discard ;
			ret = move (mItem.self) ;
			mItem = NULL ;
		}
		mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
		return move (ret) ;
	}

	void signal () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Finishing) ;
		mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
	}

	void stop () {
		crash () ;
		if ifdo (TRUE) {
			if (mThread == NULL)
				discard ;
			mThread->stop () ;
			mThread = NULL ;
		}
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadFunc = Function<> () ;
		mItem = NULL ;
		mException = NULL ;
	}
} ;

class PromiseImplHolder implement Fat<PromiseHolder ,PromiseLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<PromiseImplement>::make () ;
		return ptr (fake).initialize () ;
	}

	imports VREF<PromiseImplement> ptr (CREF<PromiseLayout> layout) {
		return keep[TYPE<PromiseImplement>::expr] (layout.mThis.self) ;
	}

	void set_retry (CREF<BOOL> flag) const override {
		return ptr (fake).set_retry (flag) ;
	}

	void start () const override {
		return ptr (fake).start () ;
	}

	void start (CREF<Function<>> func) const override {
		return ptr (fake).start (func) ;
	}

	void post (RREF<AutoRef<Pointer>> item) const override {
		return ptr (fake).post (move (item)) ;
	}

	void rethrow (CREF<Exception> e) const override {
		return ptr (fake).rethrow (e) ;
	}

	BOOL ready () const override {
		return ptr (fake).ready () ;
	}

	BOOL running () const override {
		return ptr (fake).running () ;
	}

	AutoRef<Pointer> poll () const override {
		return ptr (fake).poll () ;
	}

	void signal () const override {
		return ptr (fake).signal () ;
	}

	void stop () const override {
		return ptr (fake).stop () ;
	}
} ;

exports VFat<PromiseHolder> PromiseHolder::create (VREF<PromiseLayout> that) {
	return VFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

exports CFat<PromiseHolder> PromiseHolder::create (CREF<PromiseLayout> that) {
	return CFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

struct ExpressionNode {
	FunctionLayout mOperator ;
	Deque<INDEX> mOperand ;
	AutoRef<Pointer> mValue ;
} ;

struct ExpressionImplLayout {
	List<ExpressionNode> mList ;
} ;

class ExpressionImplHolder implement Fat<ExpressionHolder ,ExpressionLayout> {
public:
	void initialize (RREF<AutoRef<Pointer>> item) override {
		fake.mThis = SharedRef<ExpressionImplLayout>::make () ;
		fake.mIndex = fake.mThis->mList.insert () ;
		fake.mThis->mList[fake.mIndex].mValue = move (item) ;
	}

	LENGTH rank () const override {
		const auto r1x = FunctionHolder::create (fake.mThis->mList[fake.mIndex].mOperator)->rank () ;
		const auto r2x = fake.mThis->mList[fake.mIndex].mOperand.length () ;
		return r1x - r2x ;
	}

	ExpressionLayout add (CREF<ExpressionLayout> that) const override {
		ExpressionLayout ret ;
		unimplemented () ;
		return move (ret) ;
	}

	ExpressionLayout sub (CREF<ExpressionLayout> that) const override {
		ExpressionLayout ret ;
		unimplemented () ;
		return move (ret) ;
	}

	CREF<AutoRef<Pointer>> eval () const leftvalue override {
		return fake.mThis->mList[fake.mIndex].mValue ;
	}
} ;

exports VFat<ExpressionHolder> ExpressionHolder::create (VREF<ExpressionLayout> that) {
	return VFat<ExpressionHolder> (ExpressionImplHolder () ,that) ;
}

exports CFat<ExpressionHolder> ExpressionHolder::create (CREF<ExpressionLayout> that) {
	return CFat<ExpressionHolder> (ExpressionImplHolder () ,that) ;
}
} ;