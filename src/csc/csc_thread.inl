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
		Finishing ,
		Closing ,
		ETC
	} ;
} ;

struct WorkThreadLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Array<IndexIterator> mThreadQueue ;
	Array<Length> mThreadLoadLength ;
	Function<CR<Index>> mThreadFunc ;
	Deque<IndexIterator> mItemQueue ;
	Length mItemLoadLength ;

public:
	implicit WorkThreadLayout () = default ;

	implicit ~WorkThreadLayout () noexcept {
		WorkThreadHolder::hold (thiz)->stop () ;
	}
} ;

class WorkThreadThreadProcBinder final implement Fat<ThreadProcHolder ,WorkThreadLayout> {
public:
	void friend_proc (CR<Index> slot) override {
		return WorkThreadHolder::hold (self)->friend_proc (slot) ;
	}
} ;

class WorkThreadImplHolder final implement Fat<WorkThreadHolder ,WorkThreadLayout> {
public:
	void initialize () override {
		self.mThreadMutex = UniqueMutex () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
		set_queue_size (r1x) ;
	}

	void set_thread_size (CR<Length> size_) override {
		Scope anonymous (self.mThreadMutex) ;
		assert (self.mThreadFlag == ThreadFlag::Preparing) ;
		const auto r1x = MathProc::max_of (size_ ,Length (1)) ;
		self.mThread = Array<Thread> (r1x) ;
		self.mThreadJoin = BitSet (r1x) ;
		self.mThreadQueue = Array<IndexIterator> (r1x) ;
		self.mThreadLoadLength = Array<Length> (r1x) ;
	}

	void set_queue_size (CR<Length> size_) override {
		assert (size_ > 0) ;
		Scope anonymous (self.mThreadMutex) ;
		assert (self.mThreadFlag == ThreadFlag::Preparing) ;
		self.mItemQueue = Deque<IndexIterator> (size_) ;
	}

	void start (CR<Function<CR<Index>>> func) override {
		Scope anonymous (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Preparing) ;
		assume (self.mThread.size () > 0) ;
		self.mThreadLoadLength.fill (0) ;
		self.mItemLoadLength = 0 ;
		self.mThreadFlag = ThreadFlag::Running ;
		self.mThreadFunc = func ;
		const auto r1x = VFat<ThreadProcHolder> (WorkThreadThreadProcBinder () ,self) ;
		for (auto &&i : self.mThread.iter ()) {
			self.mThread[i] = Thread (Ref<ThreadProc>::make (r1x) ,i) ;
			self.mThread[i].start () ;
		}
	}

	void friend_proc (CR<Index> slot) override {
		try {
			while (TRUE) {
				pull (slot) ;
				for (auto &&i : self.mThreadQueue[slot]) {
					self.mThreadFunc (i) ;
				}
			}
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (self.mThreadMutex) ;
		self.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void pull (CR<Index> slot) {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!self.mItemQueue.empty ())
				break ;
			if ifdo (TRUE) {
				if (self.mThreadJoin[slot])
					discard ;
				self.mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		self.mThreadJoin.erase (slot) ;
		self.mItemLoadLength -= self.mThreadLoadLength[slot] ;
		Index ix = self.mItemQueue.head () ;
		const auto r1x = inline_alignas (self.mItemLoadLength ,self.mThread.size ()) / self.mThread.size () ;
		const auto r2x = MathProc::max_of (r1x / 2 ,IDEN) ;
		const auto r3x = MathProc::min_of (r2x ,self.mItemQueue[ix].length ()) ;
		self.mThreadLoadLength[slot] = r3x ;
		auto &&rbx = keep[TYPE<IndexIteratorLayout>::expr] (self.mItemQueue[ix]) ;
		self.mThreadQueue[slot] = IndexIterator (rbx.mBegin ,rbx.mBegin + r3x) ;
		self.mItemQueue[ix] = IndexIterator (rbx.mBegin + r3x ,rbx.mEnd) ;
		if ifdo (TRUE) {
			if (self.mItemQueue[ix].good ())
				discard ;
			self.mItemQueue.take () ;
		}
		rax.notify () ;
	}

	void post (CR<Index> begin_ ,CR<Index> end_) override {
		if (begin_ >= end_)
			return ;
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!self.mItemQueue.full ())
				break ;
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		const auto r1x = IndexIterator (begin_ ,end_) ;
		self.mItemQueue.add (r1x) ;
		self.mItemLoadLength += r1x.length () ;
		rax.notify () ;
	}

	void join () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (self.mItemQueue.empty ())
				if (self.mThreadJoin.length () >= self.mThread.length ())
					break ;
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
	}

	Bool join (CR<Time> interval) override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		auto rbx = TRUE ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (self.mItemQueue.empty ())
				if (self.mThreadJoin.length () >= self.mThread.length ())
					break ;
			if (!rbx)
				return FALSE ;
			rbx = FALSE ;
			rax.wait (interval) ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		return TRUE ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : self.mThread.iter ())
			self.mThread[i].stop () ;
		self.mThreadFunc = Function<CR<Index>> () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
	}
} ;

exports Ref<WorkThreadLayout> WorkThreadHolder::create () {
	return Ref<WorkThreadLayout>::make () ;
}

exports VFat<WorkThreadHolder> WorkThreadHolder::hold (VR<WorkThreadLayout> that) {
	return VFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

exports CFat<WorkThreadHolder> WorkThreadHolder::hold (CR<WorkThreadLayout> that) {
	return CFat<WorkThreadHolder> (WorkThreadImplHolder () ,that) ;
}

struct CalcThreadLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	Bool mSuspendFlag ;
	Array<Thread> mThread ;
	BitSet mThreadJoin ;
	Function<CR<CalcSolution> ,VR<CalcSolution>> mThreadFunc ;
	Array<Flt64> mConfidence ;
	Flt64 mConfidencePow ;
	Flt64 mConfidenceFator ;
	Array<CalcSolution> mThreadSolution ;
	Array<CalcSolution> mSearchSolution ;
	CalcSolution mBestSolution ;
	Bool mNewSolution ;

public:
	implicit CalcThreadLayout () = default ;

	implicit ~CalcThreadLayout () noexcept {
		CalcThreadHolder::hold (thiz)->stop () ;
	}
} ;

class CalcThreadThreadProcBinder final implement Fat<ThreadProcHolder ,CalcThreadLayout> {
public:
	void friend_proc (CR<Index> slot) override {
		return CalcThreadHolder::hold (self)->friend_proc (slot) ;
	}
} ;

class CalcThreadImplHolder final implement Fat<CalcThreadHolder ,CalcThreadLayout> {
public:
	void initialize () override {
		self.mThreadMutex = UniqueMutex () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		const auto r1x = RuntimeProc::thread_concurrency () ;
		set_thread_size (r1x) ;
	}

	void set_thread_size (CR<Length> size_) override {
		Scope anonymous (self.mThreadMutex) ;
		assert (self.mThreadFlag == ThreadFlag::Preparing) ;
		const auto r1x = MathProc::max_of (size_ ,Length (1)) ;
		self.mThread = Array<Thread> (r1x) ;
		self.mThreadJoin = BitSet (r1x) ;
		self.mSearchSolution = Array<CalcSolution> (r1x) ;
		self.mThreadSolution = Array<CalcSolution> (r1x) ;
		self.mNewSolution = FALSE ;
	}

	void set_start_input (CR<BitSet> input ,CR<Flt64> factor) override {
		Scope anonymous (self.mThreadMutex) ;
		assert (self.mThreadFlag == ThreadFlag::Preparing) ;
		assert (self.mThread.size () > 0) ;
		self.mBestSolution.mIteration = ZERO ;
		self.mBestSolution.mCost.mMax = infinity ;
		self.mBestSolution.mCost.mAvg = infinity ;
		self.mBestSolution.mCost.mStd = 0 ;
		self.mBestSolution.mInput = input ;
		for (auto &&i : self.mThreadSolution.iter ())
			self.mThreadSolution[i].mIteration = NONE ;
		self.mConfidence = Array<Flt64> (1000) ;
		self.mConfidencePow = 0.9545 ;
		self.mConfidenceFator = MathProc::clamp (factor ,Flt64 (0) ,Flt64 (1)) ;
		self.mConfidence[0] = 1 ;
		for (auto &&i : range (1 ,self.mConfidence.length ())) {
			self.mConfidence[i] = self.mConfidencePow * self.mConfidence[i - 1] ;
		}
	}

	void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) override {
		Scope anonymous (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Preparing) ;
		assume (self.mThread.size () > 0) ;
		self.mThreadFlag = ThreadFlag::Running ;
		self.mSuspendFlag = FALSE ;
		self.mThreadFunc = func ;
		const auto r1x = VFat<ThreadProcHolder> (CalcThreadThreadProcBinder () ,self) ;
		for (auto &&i : self.mThread.iter ()) {
			self.mThread[i] = Thread (Ref<ThreadProc>::make (r1x) ,i) ;
			self.mThread[i].start () ;
		}
	}

	void friend_proc (CR<Index> slot) override {
		try {
			while (TRUE) {
				search_new (slot) ;
				search_xor (slot) ;
			}
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (self.mThreadMutex) ;
		self.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	void search_new (CR<Index> slot) {
		while (TRUE) {
			wait_solution (slot) ;
			while (TRUE) {
				wait_suspend (slot) ;
				self.mSearchSolution[slot].mIteration = NONE ;
				self.mThreadFunc (self.mThreadSolution[slot] ,self.mSearchSolution[slot]) ;
				if (self.mSearchSolution[slot].mIteration == NONE)
					break ;
				if (is_better (self.mSearchSolution[slot] ,self.mThreadSolution[slot]))
					break ;
			}
			if (self.mSearchSolution[slot].mIteration != NONE)
				break ;
		}
	}

	void search_xor (CR<Index> slot) {
		while (TRUE) {
			const auto r1x = accept_solution (slot) ;
			if (r1x)
				break ;
			self.mSearchSolution[slot].mIteration = self.mThreadSolution[slot].mIteration ;
			self.mThreadFunc (self.mThreadSolution[slot] ,self.mSearchSolution[slot]) ;
			if (self.mSearchSolution[slot].mIteration == NONE)
				break ;
			if (!is_better (self.mSearchSolution[slot] ,self.mThreadSolution[slot]))
				break ;
		}
	}

	Bool is_better (CR<CalcSolution> a ,CR<CalcSolution> b) const {
		const auto r1x = Index (Flt64 (a.mIteration) * self.mConfidenceFator) ;
		Index ix = MathProc::clamp (r1x ,ZERO ,self.mConfidence.length ()) ;
		const auto r2x = a.mCost.mAvg - a.mCost.mStd * self.mConfidence[ix] ;
		const auto r3x = b.mCost.mAvg - b.mCost.mStd * self.mConfidence[ix] ;
		if (r2x < r3x)
			return TRUE ;
		return FALSE ;
	}

	void wait_solution (CR<Index> slot) {
		Scope anonymous (self.mThreadMutex) ;
		if (self.mBestSolution.mIteration != NONE)
			if (self.mThreadSolution[slot].mIteration == self.mBestSolution.mIteration)
				return ;
		self.mThreadSolution[slot] = self.mBestSolution ;
	}

	Bool accept_solution (CR<Index> slot) {
		auto rax = UniqueLock (self.mThreadMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mThreadSolution[slot].mIteration != self.mBestSolution.mIteration)
				if (is_better (self.mSearchSolution[slot] ,self.mBestSolution))
					discard ;
			assert (self.mSearchSolution[slot].mInput.size () > 0) ;
			self.mBestSolution = move (self.mSearchSolution[slot]) ;
			self.mNewSolution = TRUE ;
			rax.notify () ;
			return TRUE ;
		}
		if ifdo (act) {
			assume (self.mBestSolution.mIteration != NONE) ;
			const auto r1x = bitset_xor (self.mThreadSolution[slot].mInput ,self.mSearchSolution[slot].mInput) ;
			self.mThreadSolution[slot] = self.mBestSolution ;
			self.mSearchSolution[slot].mCost.mMax = infinity ;
			self.mSearchSolution[slot].mCost.mAvg = infinity ;
			self.mSearchSolution[slot].mCost.mStd = 0 ;
			self.mSearchSolution[slot].mInput = bitset_xor (self.mBestSolution.mInput ,r1x) ;
		}
		return FALSE ;
	}

	BitSet bitset_xor (CR<BitSet> bitset1 ,CR<BitSet> bitset2) const {
		if (bitset1.size () == 0)
			return bitset2 ;
		if (bitset2.size () == 0)
			return bitset1 ;
		return bitset1 ^ bitset2 ;
	}

	void wait_suspend (CR<Index> slot) {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!self.mSuspendFlag)
				break ;
			if ifdo (TRUE) {
				if (self.mThreadJoin[slot])
					discard ;
				self.mThreadJoin.add (slot) ;
				rax.notify () ;
			}
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		self.mThreadJoin.erase (slot) ;
	}

	Bool ready () const override {
		Scope anonymous (self.mThreadMutex) ;
		return self.mNewSolution ;
	}

	CalcSolution pull () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (self.mNewSolution)
				break ;
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		self.mNewSolution = FALSE ;
		return self.mBestSolution ;
	}

	void suspend () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		self.mSuspendFlag = TRUE ;
		rax.notify () ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			if (!self.mSuspendFlag)
				break ;
			if (self.mThreadJoin.length () >= self.mThread.length ())
				break ;
			rax.wait () ;
		}
		assume (self.mThreadFlag == ThreadFlag::Running) ;
	}

	void resume () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		self.mSuspendFlag = FALSE ;
		rax.notify () ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : self.mThread.iter ())
			self.mThread[i].stop () ;
		self.mThreadFunc = Function<CR<CalcSolution> ,VR<CalcSolution>> () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		self.mBestSolution = CalcSolution () ;
	}
} ;

exports Ref<CalcThreadLayout> CalcThreadHolder::create () {
	return Ref<CalcThreadLayout>::make () ;
}

exports VFat<CalcThreadHolder> CalcThreadHolder::hold (VR<CalcThreadLayout> that) {
	return VFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

exports CFat<CalcThreadHolder> CalcThreadHolder::hold (CR<CalcThreadLayout> that) {
	return CFat<CalcThreadHolder> (CalcThreadImplHolder () ,that) ;
}

struct PromiseLayout {
	Mutex mThreadMutex ;
	Just<ThreadFlag> mThreadFlag ;
	Array<Thread> mThread ;
	Function<> mThreadFunc ;
	Function<> mRunningFunc ;
	Box<AutoRef<Pointer>> mItem ;
	Box<Exception> mException ;
	Bool mRetryFlag ;

public:
	implicit PromiseLayout () = default ;

	implicit ~PromiseLayout () noexcept {
		PromiseHolder::hold (thiz)->stop () ;
	}
} ;

class PromiseThreadProcBinder final implement Fat<ThreadProcHolder ,PromiseLayout> {
public:
	void friend_proc (CR<Index> slot) override {
		return PromiseHolder::hold (self)->friend_proc (slot) ;
	}
} ;

class PromiseImplHolder final implement Fat<PromiseHolder ,PromiseLayout> {
public:
	void initialize () override {
		self.mThreadMutex = UniqueMutex () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		set_retry (FALSE) ;
	}

	void set_retry (CR<Bool> flag) override {
		Scope anonymous (self.mThreadMutex) ;
		assert (self.mThreadFlag == ThreadFlag::Preparing) ;
		self.mRetryFlag = flag ;
	}

	void start () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Preparing) ;
		assume (self.mThread.size () == 0) ;
		self.mThreadFlag = ThreadFlag::Running ;
		self.mThreadFunc = Function<> () ;
		self.mItem = NULL ;
		self.mException = NULL ;
		rax.notify () ;
	}

	void start (CR<Function<>> func) override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Preparing) ;
		self.mThreadFlag = ThreadFlag::Running ;
		self.mThreadFunc = func ;
		self.mItem = NULL ;
		self.mException = NULL ;
		if ifdo (TRUE) {
			if (self.mThread.size () > 0)
				discard ;
			self.mThread = Array<Thread> (1) ;
			const auto r1x = VFat<ThreadProcHolder> (PromiseThreadProcBinder () ,self) ;
			for (auto &&i : self.mThread.iter ()) {
				self.mThread[i] = Thread (Ref<ThreadProc>::make (r1x) ,0) ;
				self.mThread[i].start () ;
			}
		}
		rax.notify () ;
	}

	void friend_proc (CR<Index> slot) override {
		while (TRUE) {
			try {
				self.mRunningFunc = self.mThreadFunc ;
				self.mRunningFunc () ;
			} catch (CR<Exception> e) {
				rethrow (e) ;
			}
			const auto r1x = wait_future () ;
			if (!r1x)
				break ;
		}
		crash () ;
	}

	void crash () {
		auto rax = UniqueLock (self.mThreadMutex) ;
		self.mThreadFlag = ThreadFlag::Closing ;
		rax.notify () ;
	}

	Bool wait_future () {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Finishing)
				break ;
			rax.wait () ;
		}
		if (self.mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		if ifdo (TRUE) {
			if (self.mRetryFlag)
				discard ;
			self.mThreadFlag = ThreadFlag::Preparing ;
			rax.notify () ;
		}
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Preparing)
				break ;
			rax.wait () ;
		}
		if (self.mThreadFlag == ThreadFlag::Closing)
			return FALSE ;
		return TRUE ;
	}

	void post (RR<AutoRef<Pointer>> item) override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		assume (self.mThreadFlag == ThreadFlag::Running) ;
		assume (self.mItem == NULL) ;
		self.mItem = Box<AutoRef<Pointer>>::make (move (item)) ;
		self.mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	void rethrow (CR<Exception> e) override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		if (self.mThreadFlag != ThreadFlag::Running)
			return ;
		if (self.mException != NULL)
			return ;
		self.mException = Box<Exception>::make (e) ;
		self.mThreadFlag = ThreadFlag::Finishing ;
		rax.notify () ;
	}

	Bool ready () const override {
		Scope anonymous (self.mThreadMutex) ;
		if (self.mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	Bool running () const override {
		Scope anonymous (self.mThreadMutex) ;
		if (self.mThreadFlag == ThreadFlag::Running)
			return TRUE ;
		if (self.mThreadFlag == ThreadFlag::Finishing)
			return TRUE ;
		return FALSE ;
	}

	AutoRef<Pointer> pull () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		while (TRUE) {
			if (self.mThreadFlag != ThreadFlag::Running)
				break ;
			rax.wait () ;
		}
		assume (self.mThreadFlag != ThreadFlag::Closing) ;
		if ifdo (TRUE) {
			if (self.mException == NULL)
				discard ;
			self.mException->raise () ;
		}
		AutoRef<Pointer> ret ;
		if ifdo (TRUE) {
			if (self.mItem == NULL)
				discard ;
			ret = move (self.mItem.ref) ;
			self.mItem = NULL ;
		}
		self.mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
		return move (ret) ;
	}

	void future () override {
		auto rax = UniqueLock (self.mThreadMutex) ;
		if (self.mThreadFlag == ThreadFlag::Preparing)
			return ;
		assume (self.mThreadFlag == ThreadFlag::Finishing) ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		rax.notify () ;
	}

	void stop () override {
		crash () ;
		for (auto &&i : self.mThread.iter ())
			self.mThread[i].stop () ;
		self.mThread = Array<Thread> () ;
		self.mThreadFlag = ThreadFlag::Preparing ;
		self.mThreadFunc = Function<> () ;
		self.mItem = NULL ;
		self.mException = NULL ;
	}
} ;

exports Ref<PromiseLayout> PromiseHolder::create () {
	return Ref<PromiseLayout>::make () ;
}

exports VFat<PromiseHolder> PromiseHolder::hold (VR<PromiseLayout> that) {
	return VFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

exports CFat<PromiseHolder> PromiseHolder::hold (CR<PromiseLayout> that) {
	return CFat<PromiseHolder> (PromiseImplHolder () ,that) ;
}

struct ExecutionNode {
	Index mFlowId ;
	String<Str> mName ;
	AutoRef<Pointer> mValue ;
	Set<Index> mParent ;
	Set<Index> mChild ;
	Length mDepth ;
	Function<> mOnceActor ;
	Function<> mThenActor ;
	Bool mPlaying ;
	Bool mOncePlayed ;
	Bool mThenPlayed ;
} ;

struct ExecutionFlow {
	String<Str> mName ;
	Set<String<Str>> mNodeName ;
	ArrayList<Flag> mMonad ;
	Set<String<Str>> mMonadNode ;
} ;

struct ExecutionLayout {
	Super<Ref<ExecutionLayout>> mWeak ;
	Path mWorkingPath ;
	List<ExecutionNode> mNode ;
	List<ExecutionFlow> mFlow ;
	Set<String<Str>> mFlowName ;
	Set<Tuple<Index ,Index>> mNodeFlowSet ;
	Deque<Index> mStackNodeId ;
	Index mRootNodeId ;
	Priority<IndexPair<Length>> mPlayQueue ;
	Deque<Index> mScanStack ;
} ;

class ExecutionImplHolder final implement Fat<ExecutionHolder ,ExecutionLayout> {
public:
	void initialize (CR<String<Str>> dire) override {
		self.mWeak.mThis = Ref<ExecutionLayout>::reference (self) ;
		self.mWorkingPath = Path (dire) ;
		const auto r1x = alloc_node (slice ("") ,slice ("")) ;
		self.mRootNodeId = r1x.m2nd ;
		self.mStackNodeId.add (self.mRootNodeId) ;
	}

	Path local_path () const override {
		return self.mWorkingPath ;
	}

	String<Str> local_flow () const override {
		Index ix = self.mStackNodeId[self.mStackNodeId.tail ()] ;
		return self.mFlow[self.mNode[ix].mFlowId].mName ;
	}

	String<Str> local_node () const override {
		Index ix = self.mStackNodeId[self.mStackNodeId.tail ()] ;
		return self.mNode[ix].mName ;
	}

	CR<Pointer> stack (CR<String<Str>> flow ,CR<String<Str>> node) leftvalue override {
		const auto r1x = alloc_node (flow ,node) ;
		Index ix = r1x.m2nd ;
		Index iy = self.mStackNodeId[self.mStackNodeId.tail ()] ;
		assume (iy != self.mRootNodeId) ;
		if ifdo (TRUE) {
			if (ix == iy)
				discard ;
			self.mNode[ix].mParent.add (iy) ;
			self.mNode[iy].mChild.add (ix) ;
			const auto r2x = self.mNode[iy].mDepth + 1 ;
			self.mNode[ix].mDepth = MathProc::max_of (self.mNode[ix].mDepth ,r2x) ;
		}
		compile_node (ix) ;
		return self.mNode[ix].mValue.ref ;
	}

	Tuple<Index ,Index> alloc_node (CR<String<Str>> flow ,CR<String<Str>> node) {
		Index jx = self.mFlowName.map (flow) ;
		if ifdo (TRUE) {
			if (jx != NONE)
				discard ;
			jx = self.mFlow.insert () ;
			self.mFlow[jx].mName = flow ;
			self.mFlowName.add (self.mFlow[jx].mName ,jx) ;
			self.mFlow[jx].mNodeName = Set<String<Str>> (32) ;
			self.mFlow[jx].mMonadNode = Set<String<Str>> (32) ;
		}
		Index ix = self.mFlow[jx].mNodeName.map (node) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mNode.insert () ;
			self.mNode[ix].mFlowId = jx ;
			self.mNode[ix].mName = node ;
			self.mFlow[jx].mNodeName.add (self.mNode[ix].mName ,ix) ;
			self.mNode[ix].mParent = Set<Index> (32) ;
			self.mNode[ix].mChild = Set<Index> (32) ;
			self.mNode[ix].mPlaying = FALSE ;
			self.mNodeFlowSet.add ({jx ,ix}) ;
		}
		return Tuple<Index ,Index> ({jx ,ix}) ;
	}

	void compile_node (CR<Index> curr) {
		Index ix = curr ;
		if (self.mNode[ix].mValue.exist ())
			return ;
		Index jx = self.mNode[self.mRootNodeId].mFlowId ;
		Index kx = self.mFlow[jx].mMonadNode.map (self.mNode[ix].mName) ;
		assume (kx != NONE) ;
		const auto r1x = Unknown (self.mFlow[jx].mMonad[kx]) ;
		self.mNode[ix].mValue = AutoRef<Pointer> (r1x) ;
		self.mStackNodeId.add (ix) ;
		const auto r2x = RFat<ReflectCompile> (r1x) ;
		r2x->compile (self.mNode[ix].mValue.ref ,keep[TYPE<Execution>::expr] (self.mWeak)) ;
		self.mStackNodeId.pop () ;
	}

	void once (CR<Function<>> actor) override {
		Index ix = self.mStackNodeId[self.mStackNodeId.tail ()] ;
		self.mNode[ix].mOnceActor = actor ;
		self.mNode[ix].mPlaying = FALSE ;
		self.mNode[ix].mOncePlayed = FALSE ;
	}

	void then (CR<Function<>> actor) override {
		Index ix = self.mStackNodeId[self.mStackNodeId.tail ()] ;
		self.mNode[ix].mThenActor = actor ;
		self.mNode[ix].mPlaying = FALSE ;
		self.mNode[ix].mThenPlayed = FALSE ;
	}

	void monad (CR<Unknown> holder ,CR<String<Str>> node) override {
		Index jx = self.mNode[self.mRootNodeId].mFlowId ;
		Index kx = self.mFlow[jx].mMonadNode.map (node) ;
		if ifdo (TRUE) {
			if (kx != NONE)
				discard ;
			kx = self.mFlow[jx].mMonad.insert () ;
			self.mFlow[jx].mMonadNode.add (node ,kx) ;
		}
		self.mFlow[jx].mMonad[kx] = inline_vptr (holder) ;
	}

	void undo (CR<String<Str>> flow ,CR<String<Str>> node) override {
		const auto r1x = alloc_node (flow ,node) ;
		Index ix = r1x.m2nd ;
		self.mScanStack.clear () ;
		self.mScanStack.add (ix) ;
		while (TRUE) {
			if (self.mScanStack.empty ())
				break ;
			self.mScanStack.pop (ix) ;
			self.mNode[ix].mOncePlayed = FALSE ;
			self.mNode[ix].mThenPlayed = FALSE ;
			for (auto &&i : self.mNode[ix].mParent)
				self.mScanStack.add (i) ;
		}
	}

	void execute () override {
		self.mPlayQueue.clear () ;
		for (auto &&i : self.mNodeFlowSet) {
			Index ix = i.m2nd ;
			if (self.mNode[ix].mThenPlayed)
				continue ;
			const auto r1x = self.mNode[ix].mDepth ;
			self.mPlayQueue.add ({r1x ,ix}) ;
			self.mNode[ix].mPlaying = TRUE ;
		}
		while (TRUE) {
			if (self.mPlayQueue.empty ())
				break ;
			Index ix = self.mPlayQueue[self.mPlayQueue.head ()].m2nd ;
			self.mPlayQueue.take () ;
			play_node (ix) ;
			for (auto &&j : self.mNode[ix].mParent) {
				if (self.mNode[j].mPlaying)
					continue ;
				const auto r2x = self.mNode[j].mDepth ;
				self.mPlayQueue.add ({r2x ,j}) ;
				self.mNode[j].mPlaying = TRUE ;
			}
		}
	}

	void play_node (CR<Index> curr) {
		Index ix = curr ;
		if ifdo (FALSE) {
			if (self.mNode[ix].mOncePlayed)
				discard ;
			self.mNode[ix].mOnceActor () ;
			self.mNode[ix].mOncePlayed = TRUE ;
		}
		if ifdo (TRUE) {
			if (self.mNode[ix].mThenPlayed)
				discard ;
			self.mNode[ix].mThenActor () ;
			self.mNode[ix].mThenPlayed = TRUE ;
			self.mNode[ix].mPlaying = FALSE ;
		}
	}
} ;

exports Ref<ExecutionLayout> ExecutionHolder::create () {
	return Ref<ExecutionLayout>::make () ;
}

exports VFat<ExecutionHolder> ExecutionHolder::hold (VR<ExecutionLayout> that) {
	return VFat<ExecutionHolder> (ExecutionImplHolder () ,that) ;
}

exports CFat<ExecutionHolder> ExecutionHolder::hold (CR<ExecutionLayout> that) {
	return CFat<ExecutionHolder> (ExecutionImplHolder () ,that) ;
}
} ;