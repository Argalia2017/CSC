#pragma once

#ifndef __CSC_THREAD__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_thread.hpp"

namespace CSC {
struct ThreadStatus {
	enum {
		Preparing ,
		Running ,
		Closing ,
		Finishing ,
		ETC
	} ;
} ;

struct WorkThreadPureLayout {
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
	implicit WorkThreadPureLayout () = default ;

	implicit ~WorkThreadPureLayout () noexcept ;
} ;

class WorkThreadImplement implement Fat<WorkThreadHolder ,WorkThreadPureLayout> {
private:
	using THREAD_QUEUE_SIZE = ENUM<65536> ;

public:
	void initialize () override {
		fake.mThreadMutex = UniqueMutex::make () ;
		set_thread_size (1) ;
		set_queue_size (1) ;
	}

	void set_thread_size (CREF<LENGTH> size_) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadStatus::Preparing) ;
		fake.mThread = Array<Thread> (size_) ;
		fake.mThreadJoin = BitSet (size_) ;
		fake.mThreadQueue = Array<Deque<INDEX>> (size_) ;
		fake.mThreadLoadCount = Array<LENGTH> (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadStatus::Preparing) ;
		assume (fake.mItemQueue.length () == 0) ;
		fake.mItemQueue = Deque<INDEX> (size_) ;
	}

	void start (RREF<Function<CREF<INDEX>>> func) override {
		Scope<Mutex> anonymous (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadStatus::Preparing) ;
		assume (fake.mThread.length () > 0) ;
		fake.mThreadLoadCount.fill (0) ;
		fake.mItemLoadCount = 0 ;
		fake.mThreadFlag = ThreadStatus::Running ;
		fake.mThreadFunc = move (func) ;
		const auto r1x = Ref<WorkThreadPureLayout>::reference (fake) ;
		const auto r2x = SharedRef<ThreadExecutable> (SharedRef<ThreadExecutableBinder<WorkThreadPureLayout>>::make (r1x)) ;
		for (auto &&i : iter (0 ,fake.mThread.size ())) {
			fake.mThread[i] = Thread (r2x ,i) ;
			fake.mThread[i].start () ;
		}
	}

	void execute (CREF<INDEX> slot) {
		unimplemented () ;
	}

	void poll (CREF<INDEX> slot) {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag != ThreadStatus::Preparing) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadStatus::Running)
				break ;
			if ifnot (fake.mItemQueue.empty ())
				break ;
			if ifdo (TRUE) {
				if (fake.mThreadJoin[slot])
					discard ;
				fake.mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		assume (fake.mThreadFlag == ThreadStatus::Running) ;
		fake.mThreadJoin.erase (slot) ;
		if ifdo (TRUE) {
			if (fake.mThreadQueue[slot].size () > 0)
				discard ;
			fake.mThreadQueue[slot] = Deque<INDEX> (THREAD_QUEUE_SIZE::expr) ;
		}
		fake.mItemLoadCount -= fake.mThreadLoadCount[slot] ;
		const auto r1x = fake.mItemQueue.length () + fake.mItemLoadCount ;
		const auto r2x = inline_alignas (r1x ,fake.mThread.size ()) / fake.mThread.size () ;
		const auto r3x = MathTool::max_of (r2x / 2 ,LENGTH (1)) ;
		const auto r4x = MathTool::min_of (r3x ,fake.mThreadQueue[slot].size () ,fake.mItemQueue.length ()) ;
		fake.mThreadLoadCount[slot] = r4x ;
		fake.mItemLoadCount += fake.mThreadLoadCount[slot] ;
		for (auto &&i : iter (0 ,r4x)) {
			noop (i) ;
			INDEX ix = fake.mItemQueue.head () ;
			fake.mThreadQueue[slot].add (fake.mItemQueue[ix]) ;
			fake.mItemQueue.take () ;
		}
		rax.notify () ;
	}

	void post (CREF<INDEX> item) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadStatus::Running) ;
		fake.mItemQueue.add (item) ;
		rax.notify () ;
	}

	void post (CREF<Array<INDEX>> item) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag == ThreadStatus::Running) ;
		for (auto &&i : item)
			fake.mItemQueue.add (i) ;
		rax.notify () ;
	}

	void join () override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag != ThreadStatus::Preparing) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadStatus::Running)
				break ;
			if (fake.mItemQueue.empty ())
				if (fake.mThreadJoin.length () >= fake.mThread.length ())
					break ;
			rax.wait () ;
		}
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) override {
		auto rax = UniqueLock (fake.mThreadMutex) ;
		assume (fake.mThreadFlag != ThreadStatus::Preparing) ;
		while (TRUE) {
			if (fake.mThreadFlag != ThreadStatus::Running)
				break ;
			if (fake.mItemQueue.empty () == 0)
				if (fake.mThreadJoin.length () >= fake.mThread.length ())
					break ;
			auto rbx = FALSE ;
			predicate (rbx) ;
			if ifnot (rbx)
				return FALSE ;
			rax.wait (interval) ;
		}
		return TRUE ;
	}

	void stop () override {
		if ifdo (TRUE) {
			auto rax = UniqueLock (fake.mThreadMutex) ;
			if (fake.mThreadFlag == ThreadStatus::Preparing)
				discard ;
			fake.mThreadFlag = ThreadStatus::Closing ;
			rax.notify () ;
		}
		for (auto &&i : iter (0 ,fake.mThread.size ()))
			fake.mThread[i].stop () ;
		fake.mThread = Array<Thread> () ;
		fake.mThreadFunc = Function<CREF<INDEX>> () ;
		fake.mThreadFlag = ThreadStatus::Preparing ;
		fake.mThreadJoin = BitSet () ;
		fake.mThreadQueue = Array<Deque<INDEX>> () ;
		fake.mThreadLoadCount = Array<LENGTH> () ;
		fake.mItemQueue = Deque<INDEX> () ;
	}
} ;

inline WorkThreadPureLayout::~WorkThreadPureLayout () noexcept {
	WorkThreadHolder::create (thiz)->stop () ;
}

exports VFat<WorkThreadHolder> WorkThreadHolder::create (VREF<WorkThreadPureLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplement () ,Pointer::from (that)) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::create (CREF<WorkThreadPureLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplement () ,Pointer::from (that)) ;
}

struct CalcThreadImplLayout {
	Mutex mThreadMutex ;
	FLAG mThreadFlag ;
	BOOL mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<VREF<CalcSolution>> mThreadFunc ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	Array<CalcSolution> mBranchSolution ;
	CalcSolution mBestSolution ;
} ;

class CalcThreadImplement implement Fat<CalcThreadHolder ,CalcThreadPureLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	void set_thread_size (CREF<LENGTH> size_) override {
		unimplemented () ;
	}

	void start (RREF<Function<VREF<CalcSolution>>> func) override {
		unimplemented () ;
	}

	CalcSolution best () override {
		unimplemented () ;
		return CalcSolution () ;
	}

	void suspend () override {
		unimplemented () ;
	}

	void resume () override {
		unimplemented () ;
	}

	void stop () override {
		unimplemented () ;
	}
} ;

exports VFat<CalcThreadHolder> CalcThreadHolder::create (VREF<CalcThreadPureLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplement () ,Pointer::from (that)) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::create (CREF<CalcThreadPureLayout> that) {
	return CFat<CalcThreadHolder> (CalcThreadImplement () ,Pointer::from (that)) ;
}

struct PromiseImplLayout {
	Mutex mThreadMutex ;
	FLAG mThreadFlag ;
	Thread mThread ;
	Function<VREF<AutoRef<Pointer>>> mThreadFunc ;
	SharedRef<PromiseImplLayout> mPrevious ;
	Ref<AutoRef<Pointer>> mItem ;
	Ref<Exception> mException ;
} ;

class PromiseImplement implement Fat<PromiseHolder ,PromiseLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	void start () const override {
		unimplemented () ;
	}

	void start (RREF<Function<VREF<AutoRef<Pointer>>>> func) const override {
		unimplemented () ;
	}

	void post (RREF<AutoRef<Pointer>> item) const override {
		unimplemented () ;
	}

	void rethrow (CREF<Exception> e) const override {
		unimplemented () ;
	}

	void signal () const override {
		unimplemented () ;
	}

	BOOL ready () const override {
		unimplemented () ;
		return FLAG () ;
	}

	AutoRef<Pointer> poll () const override {
		unimplemented () ;
		return AutoRef<Pointer> () ;
	}

	PromiseLayout then (RREF<Function<VREF<AutoRef<Pointer>>>> func) const override {
		unimplemented () ;
		return PromiseLayout () ;
	}

	void stop () const override {
		unimplemented () ;
	}
} ;

exports VFat<PromiseHolder> PromiseHolder::create (VREF<PromiseLayout> that) {
	return VFat<PromiseHolder> (PromiseImplement () ,Pointer::from (that)) ;
}

exports CFat<PromiseHolder> PromiseHolder::create (CREF<PromiseLayout> that) {
	return CFat<PromiseHolder> (PromiseImplement () ,Pointer::from (that)) ;
}
} ;