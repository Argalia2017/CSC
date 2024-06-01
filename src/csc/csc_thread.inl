#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_thread.hpp"

namespace CSC {
struct ThreadFlag {
	enum {
		Preparing ,
		Running ,
		Closing ,
		Finishing ,
		ETC
	} ;
} ;

class WorkThreadImplement implement ThreadFriend {
private:
	using THREAD_QUEUE_SIZE = ENUM<65536> ;

protected:
	Mutex mThreadMutex ;
	FLAG mThreadFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Array<Deque<INDEX>> mThreadQueue ;
	Array<LENGTH> mThreadLoadCount ;
	Function<CREF<INDEX>> mThreadFunc ;
	Deque<INDEX> mItemQueue ;
	LENGTH mItemLoadCount ;

public:
	implicit WorkThreadImplement () = default ;

	implicit ~WorkThreadImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		set_thread_size (1) ;
		set_queue_size (1) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mThreadQueue = Array<Deque<INDEX>> (size_) ;
		mThreadLoadCount = Array<LENGTH> (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mItemQueue.empty ()) ;
		mItemQueue = Deque<INDEX> (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.length () > 0) ;
		mThreadLoadCount.fill (0) ;
		mItemLoadCount = 0 ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = move (func) ;
		for (auto &&i : iter (0 ,mThread.size ())) {
			mThread[i] = Thread (Ref<ThreadFriend>::reference (thiz) ,i) ;
			mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) override {
		if ifdo (TRUE) {
			if (!(mThreadQueue[slot].empty ()))
				discard ;
			poll (slot) ;
		}
		INDEX ix = mThreadQueue[slot].head () ;
		mThreadFunc (mThreadQueue[slot][ix]) ;
		mThreadQueue[slot].take () ;
	}

	void poll (CREF<INDEX> slot) {
		auto rax = UniqueLock (mThreadMutex) ;
		while (TRUE) {
			assume (mThreadFlag == ThreadFlag::Running) ;
			if (!(mItemQueue.empty ()))
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
		if (mThreadFlag != ThreadFlag::Running)
			return ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Running)
				break ;
			if (mItemQueue.empty ())
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			rax.wait () ;
		}
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) {
		auto rax = UniqueLock (mThreadMutex) ;
		if (mThreadFlag != ThreadFlag::Running)
			return TRUE ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Running)
				break ;
			if (mItemQueue.empty () == 0)
				if (mThreadJoin.length () >= mThread.length ())
					break ;
			auto rbx = FALSE ;
			predicate (rbx) ;
			if (!(rbx))
				return FALSE ;
			rax.wait (interval) ;
		}
		return TRUE ;
	}

	void stop () {
		if ifdo (TRUE) {
			auto rax = UniqueLock (mThreadMutex) ;
			if (mThreadFlag == ThreadFlag::Preparing)
				discard ;
			mThreadFlag = ThreadFlag::Closing ;
			rax.notify () ;
		}
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
		fake.mThis->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return fake.mThis->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const override {
		return fake.mThis->set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const override {
		return fake.mThis->start (move (func)) ;
	}

	void post (CREF<INDEX> item) const override {
		return fake.mThis->post (item) ;
	}

	void post (CREF<Array<INDEX>> item) const override {
		return fake.mThis->post (item) ;
	}

	void join () const override {
		return fake.mThis->join () ;
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) const override {
		return fake.mThis->join (interval ,predicate) ;
	}

	void stop () const override {
		return fake.mThis->stop () ;
	}
} ;

exports VFat<WorkThreadHolder> WorkThreadHolder::create (VREF<WorkThreadLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::create (CREF<WorkThreadLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

class CalcThreadImplement implement ThreadFriend {
protected:
	Mutex mThreadMutex ;
	FLAG mThreadFlag ;
	BOOL mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<CREF<CalcSolution> ,VREF<CalcSolution>> mThreadFunc ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	CalcSolution mBestSolution ;

public:
	implicit CalcThreadImplement () = default ;

	implicit ~CalcThreadImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
		set_thread_size (1) ;
	}

	void set_thread_size (CREF<LENGTH> size_) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThread = Array<Thread> (size_) ;
		mThreadJoin = BitSet (size_) ;
		mThreadSolution = Array<CalcSolution> (size_) ;
		mBestSolution.mIndex = NONE ;
		mBestSolution.mError = FLT64_INF ;
		mBestSolution.mInput = BitSet () ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		assume (mThread.length () > 0) ;
		mThreadFlag = ThreadFlag::Running ;
		mSuspendFlag = FALSE ;
		mThreadFunc = move (func) ;
		for (auto &&i : iter (0 ,mThread.size ())) {
			mThread[i] = Thread (Ref<ThreadFriend>::reference (thiz) ,i) ;
			mThread[i].start () ;
		}
	}

	void friend_execute (CREF<INDEX> slot) override {
		search_new (slot) ;
		search_xor (slot) ;
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
			return bitset2 ;
		if (bitset2.size () == 0)
			return bitset1 ;
		return bitset1 ^ bitset2 ;
	}

	void wait_solution (CREF<INDEX> slot) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		if (mThreadSolution[slot].mIndex == mBestSolution.mIndex)
			return ;
		mThreadSolution[slot] = mBestSolution ;
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
			mBestSolution.mInput = mSearchSolution[slot].mInput ;
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
			if (!(mSuspendFlag))
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
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		return mBestSolution ;
	}

	void suspend () {
		auto rax = UniqueLock (mThreadMutex) ;
		if (mThreadFlag != ThreadFlag::Running)
			return ;
		mSuspendFlag = TRUE ;
		rax.notify () ;
		while (TRUE) {
			if (mThreadFlag != ThreadFlag::Running)
				break ;
			if (!(mSuspendFlag))
				break ;
			if (mThreadJoin.length () >= mThread.length ())
				break ;
			rax.wait () ;
		}
	}

	void resume () {
		auto rax = UniqueLock (mThreadMutex) ;
		mSuspendFlag = FALSE ;
		rax.notify () ;
	}

	void stop () {
		if ifdo (TRUE) {
			auto rax = UniqueLock (mThreadMutex) ;
			if (mThreadFlag == ThreadFlag::Preparing)
				discard ;
			mThreadFlag = ThreadFlag::Closing ;
			rax.notify () ;
		}
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
		fake.mThis->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const override {
		return fake.mThis->set_thread_size (size_) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const override {
		return fake.mThis->start (move (func)) ;
	}

	CalcSolution best () const override {
		return fake.mThis->best () ;
	}

	void suspend () const override {
		return fake.mThis->suspend () ;
	}

	void resume () const override {
		return fake.mThis->resume () ;
	}

	void stop () const override {
		return fake.mThis->stop () ;
	}
} ;

exports VFat<CalcThreadHolder> CalcThreadHolder::create (VREF<CalcThreadLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::create (CREF<CalcThreadLayout> that) {
	return CFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

class PromiseImplement implement ThreadFriend {
protected:
	Mutex mThreadMutex ;
	FLAG mThreadFlag ;
	Thread mThread ;
	Function<VREF<AutoRef<Pointer>>> mThreadFunc ;
	Box<AutoRef<Pointer>> mItem ;
	Box<Exception> mException ;
	SharedRef<PromiseImplement> mPrevious ;

public:
	implicit PromiseImplement () = default ;

	implicit ~PromiseImplement () noexcept {
		stop () ;
	}

	void initialize () {
		mThreadMutex = UniqueMutex () ;
		mThreadFlag = ThreadFlag::Preparing ;
	}

	void start () {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThreadFlag = ThreadFlag::Running ;
		mThread = Thread () ;
		mThreadFunc = Function<VREF<AutoRef<Pointer>>> () ;
		mItem = NULL ;
		mException = NULL ;
	}

	void start (CREF<Function<VREF<AutoRef<Pointer>>>> func) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Preparing) ;
		mThreadFlag = ThreadFlag::Running ;
		mThreadFunc = move (func) ;
		mItem = NULL ;
		mException = NULL ;
		mThread = Thread (Ref<ThreadFriend>::reference (thiz) ,0) ;
		mThread.start () ;
	}

	void friend_execute (CREF<INDEX> slot) override {
		auto rax = AutoRef<Pointer> () ;
		try {
			mThreadFunc (rax) ;
		} catch (CREF<Exception> e) {
			rethrow (e) ;
		} catch (...) {
			rethrow (Exception (slice ("unknown C++ exception"))) ;
		}
		post (move (rax)) ;
		signal () ;
		assume (FALSE) ;
	}

	void post (RREF<AutoRef<Pointer>> item) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		assume (mItem == NULL) ;
		assume (mException == NULL) ;
		mItem = Box<AutoRef<Pointer>>::make (move (item)) ;
	}

	void rethrow (CREF<Exception> e) {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		assume (mException == NULL) ;
		mItem = NULL ;
		mException = Box<Exception>::make (e) ;
	}

	void signal () {
		auto rax = UniqueLock (mThreadMutex) ;
		assume (mThreadFlag == ThreadFlag::Running) ;
		mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	BOOL ready () {
		Scope<CREF<Mutex>> anonymous (mThreadMutex) ;
		if (mThreadFlag == ThreadFlag::Closing)
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
		if ifdo (TRUE) {
			if (mException == NULL)
				discard ;
			mException->raise () ;
		}
		assume (mItem != NULL) ;
		AutoRef<Pointer> ret = move (mItem.self) ;
		mItem = NULL ;
		rax.notify () ;
		return move (ret) ;
	}

	void stop () {
		if ifdo (TRUE) {
			auto rax = UniqueLock (mThreadMutex) ;
			if (mThreadFlag == ThreadFlag::Preparing)
				discard ;
			mThreadFlag = ThreadFlag::Closing ;
			rax.notify () ;
		}
		mThread.stop () ;
		mThread = Thread () ;
		mThreadFlag = ThreadFlag::Preparing ;
		mThreadFunc = Function<VREF<AutoRef<Pointer>>> () ;
		mItem = NULL ;
		mException = NULL ;
	}
} ;

class PromiseImplHolder implement Fat<PromiseHolder ,PromiseLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<PromiseImplement>::make () ;
		fake.mThis->initialize () ;
	}

	void start () const override {
		return fake.mThis->start () ;
	}

	void start (RREF<FunctionLayout> func) const override {
		return fake.mThis->start (move (keep[TYPE<Function<VREF<AutoRef<Pointer>>>>::expr] (func))) ;
	}

	void post (RREF<AutoRef<Pointer>> item) const override {
		return fake.mThis->post (move (item)) ;
	}

	void rethrow (CREF<Exception> e) const override {
		return fake.mThis->rethrow (e) ;
	}

	void signal () const override {
		return fake.mThis->signal () ;
	}

	BOOL ready () const override {
		return fake.mThis->ready () ;
	}

	AutoRef<Pointer> poll () const override {
		return fake.mThis->poll () ;
	}

	void stop () const override {
		return fake.mThis->stop () ;
	}
} ;

exports VFat<PromiseHolder> PromiseHolder::create (VREF<PromiseLayout> that) {
	return VFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

exports CFat<PromiseHolder> PromiseHolder::create (CREF<PromiseLayout> that) {
	return CFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}
} ;