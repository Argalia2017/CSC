#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class UNIT>
class ThreadLocal {
private:
	Mutable<Mutex> mMutex ;
	ArrayList<UNIT> mValue ;
	Set<FLAG> mMappingSet ;

public:
	implicit ThreadLocal () = default ;

	UNIT &deref () leftvalue {
		ScopedGuard<Mutex> ANONYMOUS (mMutex) ;
		const auto r1x = GlobalRuntime::thread_tid () ;
		INDEX ix = mMappingSet.map (r1x) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			if switch_once (TRUE) {
				if (mValue.size () == 0)
					discard ;
				mValue = ArrayList<UNIT> (DEFAULT_RECURSIVE_SIZE::compile ()) ;
				mMappingSet = Set<FLAG> (mValue.size ()) ;
			}
			ix = mValue.insert () ;
			mMappingSet.add (r1x ,ix) ;
		}
		return mValue[ix] ;
	}
} ;

template <class ITEM>
class CalcThread {
private:
	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
	} ;

	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<AtomicVar> mThreadFlag ;
		AtomicVar mThreadCounter ;
		Function<ITEM ()> mThreadProc ;
		Set<INDEX> mThreadPendingSet ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

private:
	StrongRef<THREAD_PACK> mThis ;

public:
	implicit CalcThread () {
		mThis = StrongRef<THREAD_PACK>::make () ;
		mThis->mKeep = UniqueRef<SharedRef<THIS_PACK>> ([&] (SharedRef<THIS_PACK> &me) {
			me = SharedRef<THIS_PACK>::make () ;
		} ,[] (SharedRef<THIS_PACK> &me) {
			static_destroy (me.self) ;
		}) ;
		mThis->mThis = mThis->mKeep.self ;
		auto &r1x = mThis->mThis.self ;
		static_create (r1x) ;
	}

	LENGTH size () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mItemQueue.exist ())
			return 0 ;
		return r1x.mItemQueue->size () ;
	}

	LENGTH length () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mItemQueue.exist ())
			return 0 ;
		return r1x.mItemQueue->length () ;
	}

	ArrayList<ITEM> poll (const LENGTH &count) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		rbx.notify () ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->size () >= count)
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
			for (auto &&i : r1x.mItemQueue.self)
				rcx->add (_MOVE_ (i)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		r1x.mThreadPendingSet.clear () ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (r1x.mException.exist ())
				break ;
			if (r1x.mItemQueue->length () >= count)
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		const auto r6x = _MOVE_ (r1x.mException) ;
		if (r6x.exist ())
			r6x->raise () ;
		rbx.notify () ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ()))
			r1x.mThreadPendingSet.add (i) ;
		const auto r3x = count * _EBOOL_ (r1x.mItemQueue->length () >= count) ;
		ArrayList<ITEM> ret = ArrayList<ITEM> (r3x) ;
		for (auto &&i : _RANGE_ (0 ,r3x)) {
			_NOOP_ (i) ;
			ret.add (_MOVE_ (r1x.mItemQueue.self[r1x.mItemQueue->head ()])) ;
			r1x.mItemQueue->take () ;
		}
		return _MOVE_ (ret) ;
	}

	ArrayList<ITEM> poll (const LENGTH &count ,const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		rbx.notify () ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->size () >= count)
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
			for (auto &&i : r1x.mItemQueue.self)
				rcx->add (_MOVE_ (i)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		r1x.mThreadPendingSet.clear () ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (r1x.mException.exist ())
				break ;
			if (r1x.mItemQueue->length () >= count)
				break ;
			const auto r2x = predicate () ;
			if (!r2x)
				break ;
			rbx.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		const auto r6x = _MOVE_ (r1x.mException) ;
		if (r6x.exist ())
			r6x->raise () ;
		rbx.notify () ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ()))
			r1x.mThreadPendingSet.add (i) ;
		const auto r3x = count * _EBOOL_ (r1x.mItemQueue->length () >= count) ;
		ArrayList<ITEM> ret = ArrayList<ITEM> (r3x) ;
		for (auto &&i : _RANGE_ (0 ,r3x)) {
			_NOOP_ (i) ;
			ret.add (_MOVE_ (r1x.mItemQueue.self[r1x.mItemQueue->head ()])) ;
			r1x.mItemQueue->take () ;
		}
		return _MOVE_ (ret) ;
	}

	ArrayList<ITEM> poll_all () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		rbx.notify () ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ()))
			r1x.mThreadPendingSet.add (i) ;
		const auto r3x = r1x.mItemQueue->length () ;
		ArrayList<ITEM> ret = ArrayList<ITEM> (r3x) ;
		for (auto &&i : _RANGE_ (0 ,r3x)) {
			_NOOP_ (i) ;
			ret.add (_MOVE_ (r1x.mItemQueue.self[r1x.mItemQueue->head ()])) ;
			r1x.mItemQueue->take () ;
		}
		return _MOVE_ (ret) ;
	}

	void start (const LENGTH &count ,REMOVE_CONST_TYPE<Function<ITEM ()>> &&proc) const {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadBinder ;
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		r1x.mThreadFlag = AutoRef<AtomicVar>::make (TRUE) ;
		r1x.mThreadCounter.store (0) ;
		r1x.mThreadProc = _MOVE_ (proc) ;
		r1x.mThreadPendingSet = Set<INDEX> (count) ;
		for (auto &&i : _RANGE_ (0 ,count))
			r1x.mThreadPendingSet.add (i) ;
		r1x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = Array<AutoRef<Thread>> (count) ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			const auto r2x = StrongRef<R1X>::make (PhanRef<THIS_PACK>::make (r1x) ,i) ;
			r1x.mThreadPool[i] = AutoRef<Thread>::make (r2x) ;
		}
	}

	void suspend () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		rbx.notify () ;
		for (auto &&i : _RANGE_ (r1x.mThreadPool.length ()))
			r1x.mThreadPendingSet.add (i) ;
	}

	void resume () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		rbx.notify () ;
		r1x.mThreadPendingSet.clear () ;
	}

	void stop () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_destroy (r1x) ;
	}

private:
	imports void static_create (THIS_PACK &this_) {
		ScopedGuard<Mutex> ANONYMOUS (this_.mThreadMutex) ;
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = Array<AutoRef<Thread>> () ;
		this_.mThreadProc = Function<ITEM ()> () ;
		this_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_destroy (THIS_PACK &this_) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (!this_.mThreadFlag.exist ())
			return ;
		this_.mThreadFlag->store (FALSE) ;
		while (TRUE) {
			if (!this_.mThreadFlag.exist ())
				break ;
			if (this_.mThreadCounter.fetch () == 0)
				break ;
			rax.yield () ;
		}
		_DEBUG_ASSERT_ (this_.mThreadFlag.exist ()) ;
		for (auto &&i : this_.mThreadPool) {
			if (!i.exist ())
				continue ;
			rax = UniqueLock () ;
			i->join () ;
			rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		}
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = Array<AutoRef<Thread>> () ;
		this_.mThreadProc = Function<ITEM ()> () ;
		this_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_execute (THIS_PACK &this_ ,const INDEX &tid) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadCounter ;
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		ScopedGuard<R1X> ANONYMOUS (_CAST_ (ARGV<R1X>::ID ,this_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			static_pend (this_ ,tid) ;
			try {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				rax = this_.mThreadProc () ;
			} catch (const Exception &e) {
				static_rethrow (this_ ,e) ;
			} catch (...) {
				const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
				static_rethrow (this_ ,r1x) ;
			}
			if (rax.exist ())
				static_post (this_ ,tid ,_MOVE_ (rax.self)) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	imports void static_pend (THIS_PACK &this_ ,const INDEX &tid) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		while (TRUE) {
			if (!this_.mThreadFlag->fetch ())
				break ;
			if (this_.mThreadPendingSet.find (tid) == VAR_NONE)
				break ;
			rax.wait () ;
		}
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
	}

	imports void static_post (THIS_PACK &this_ ,const INDEX &tid ,const REMOVE_CONST_TYPE<ITEM> &item) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		while (TRUE) {
			if (!this_.mThreadFlag->fetch ())
				break ;
			if (!this_.mItemQueue->full ())
				break ;
			rax.wait () ;
		}
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		rax.notify () ;
		if (this_.mItemQueue->full ())
			this_.mItemQueue->take () ;
		this_.mItemQueue->add (_MOVE_ (item)) ;
	}

	imports void static_post (THIS_PACK &this_ ,const INDEX &tid ,REMOVE_CONST_TYPE<ITEM> &&item) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		_DEBUG_ASSERT_ (this_.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!this_.mThreadFlag->fetch ())
				break ;
			if (!this_.mItemQueue->full ())
				break ;
			rax.wait () ;
		}
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		rax.notify () ;
		if (this_.mItemQueue->full ())
			this_.mItemQueue->take () ;
		this_.mItemQueue->add (_MOVE_ (item)) ;
	}

	imports void static_rethrow (THIS_PACK &this_ ,const Exception &e) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (this_.mException.exist ())
			return ;
		rax.notify () ;
		this_.mException = AutoRef<Exception>::make (e) ;
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadBinder :
	delegate public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;
	INDEX mThreadID ;
	BOOL mTestFlag ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (REMOVE_CONST_TYPE<PhanRef<THIS_PACK>> &&this_ ,const INDEX &tid) {
		mThis = _MOVE_ (this_) ;
		mThreadID = tid ;
		mTestFlag = FALSE ;
	}

	void execute () override {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (mTestFlag)
				discard ;
			_CALL_TRY_ ([&] () {
				static_execute (mThis ,mThreadID) ;
			} ,[&] () {
				_NOOP_ () ;
			}) ;
		}
		if switch_once (fax) {
			static_execute (mThis ,mThreadID) ;
		}
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadCounter :
	delegate private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		const auto r1x = mSelf.mThreadCounter.increase () ;
		_NOOP_ (r1x) ;
	}

	void unlock () {
		const auto r1x = mSelf.mThreadCounter.decrease () ;
		_NOOP_ (r1x) ;
	}
} ;

template <class ITEM>
class WorkThread {
private:
	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
	} ;

	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<AtomicVar> mThreadFlag ;
		AtomicVar mThreadCounter ;
		Function<void (const ITEM &)> mThreadProc ;
		Set<INDEX> mThreadPendingSet ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

private:
	StrongRef<THREAD_PACK> mThis ;

public:
	implicit WorkThread () {
		mThis = StrongRef<THREAD_PACK>::make () ;
		mThis->mKeep = UniqueRef<SharedRef<THIS_PACK>> ([&] (SharedRef<THIS_PACK> &me) {
			me = SharedRef<THIS_PACK>::make () ;
		} ,[] (SharedRef<THIS_PACK> &me) {
			static_destroy (me.self) ;
		}) ;
		mThis->mThis = mThis->mKeep.self ;
		auto &r1x = mThis->mThis.self ;
		static_create (r1x) ;
	}

	LENGTH size () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mItemQueue.exist ())
			return 0 ;
		return r1x.mItemQueue->size () ;
	}

	LENGTH length () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mItemQueue.exist ())
			return 0 ;
		return r1x.mItemQueue->length () ;
	}

	void post (const REMOVE_CONST_TYPE<ITEM> &item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		if (r1x.mItemQueue->full ())
			return ;
		rbx.notify () ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
	}

	void post (REMOVE_CONST_TYPE<ITEM> &&item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		if (r1x.mItemQueue->full ())
			return ;
		rbx.notify () ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
	}

	void post (const REMOVE_CONST_TYPE<ITEM> &item ,const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			const auto r2x = predicate () ;
			if (!r2x)
				break ;
			rbx.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		if (r1x.mItemQueue->full ())
			return ;
		rbx.notify () ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
	}

	void post (REMOVE_CONST_TYPE<ITEM> &&item ,const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			const auto r2x = predicate () ;
			if (!r2x)
				break ;
			rbx.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		if (r1x.mItemQueue->full ())
			return ;
		rbx.notify () ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
	}

	void post_all (const Array<REMOVE_CVR_TYPE<ITEM>> &item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		rbx.notify () ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->length () + item.length () <= r1x.mItemQueue->size ())
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (item.length ()) ;
			for (auto &&i : r1x.mItemQueue.self)
				rcx->add (_MOVE_ (i)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		for (auto &&i : _RANGE_ (0 ,item.length ()))
			r1x.mItemQueue->add (_MOVE_ (item[i])) ;
	}

	void post_all (Array<REMOVE_CVR_TYPE<ITEM>> &&item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DEBUG_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		rbx.notify () ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->length () + item.length () <= r1x.mItemQueue->size ())
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (item.length ()) ;
			for (auto &&i : r1x.mItemQueue.self)
				rcx->add (_MOVE_ (i)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		for (auto &&i : _RANGE_ (0 ,item.length ()))
			r1x.mItemQueue->add (_MOVE_ (item[i])) ;
	}

	void start (const LENGTH &count ,Function<void (const ITEM &)> &&proc) const {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadBinder ;
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		r1x.mThreadFlag = AutoRef<AtomicVar>::make (TRUE) ;
		r1x.mThreadCounter.store (0) ;
		r1x.mThreadProc = _MOVE_ (proc) ;
		r1x.mThreadPendingSet = Set<INDEX> (count) ;
		for (auto &&i : _RANGE_ (0 ,count))
			r1x.mThreadPendingSet.add (i) ;
		r1x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = Array<AutoRef<Thread>> (count) ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			const auto r2x = StrongRef<R1X>::make (PhanRef<THIS_PACK>::make (r1x) ,i) ;
			r1x.mThreadPool[i] = AutoRef<Thread>::make (r2x) ;
		}
	}

	void join () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (r1x.mException.exist ())
				break ;
			if (r1x.mThreadPendingSet.length () >= r1x.mThreadPool.length ())
				if (r1x.mItemQueue->empty ())
					break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		const auto r3x = _MOVE_ (r1x.mException) ;
		if (!r3x.exist ())
			return ;
		r3x->raise () ;
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag->fetch ())
				break ;
			if (r1x.mException.exist ())
				break ;
			if (r1x.mThreadPendingSet.length () >= r1x.mThreadPool.length ())
				if (r1x.mItemQueue->empty ())
					break ;
			const auto r2x = predicate () ;
			if (!r2x)
				break ;
			rbx.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag->fetch ()) ;
		const auto r3x = _MOVE_ (r1x.mException) ;
		if (!r3x.exist ())
			return ;
		r3x->raise () ;
	}

	void stop () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_destroy (r1x) ;
	}

private:
	imports void static_create (THIS_PACK &this_) {
		ScopedGuard<Mutex> ANONYMOUS (this_.mThreadMutex) ;
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = Array<AutoRef<Thread>> () ;
		this_.mThreadProc = Function<void (const ITEM &)> () ;
		this_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_destroy (THIS_PACK &this_) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (!this_.mThreadFlag.exist ())
			return ;
		this_.mThreadFlag->store (FALSE) ;
		while (TRUE) {
			if (!this_.mThreadFlag.exist ())
				break ;
			if (this_.mThreadCounter.fetch () == 0)
				break ;
			rax.yield () ;
		}
		_DEBUG_ASSERT_ (this_.mThreadFlag.exist ()) ;
		for (auto &&i : this_.mThreadPool) {
			if (!i.exist ())
				continue ;
			rax = UniqueLock () ;
			i->join () ;
			rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		}
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = Array<AutoRef<Thread>> () ;
		this_.mThreadProc = Function<void (const ITEM &)> () ;
		this_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_execute (THIS_PACK &this_ ,const INDEX &tid) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadCounter ;
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		ScopedGuard<R1X> ANONYMOUS (_CAST_ (ARGV<R1X>::ID ,this_)) ;
		auto rax = List<ITEM> () ;
		while (TRUE) {
			if (rax.empty ())
				static_poll (this_ ,tid ,rax) ;
			INDEX ix = rax.head () ;
			try {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				this_.mThreadProc (rax[ix]) ;
			} catch (const Exception &e) {
				static_rethrow (this_ ,e) ;
			} catch (...) {
				const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
				static_rethrow (this_ ,r1x) ;
			}
			rax.remove (ix) ;
		}
	}

	imports void static_poll (THIS_PACK &this_ ,const INDEX &tid ,List<ITEM> &list) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		rax.notify () ;
		this_.mThreadPendingSet.add (tid) ;
		while (TRUE) {
			if (!this_.mThreadFlag->fetch ())
				break ;
			if (!this_.mItemQueue->empty ())
				break ;
			rax.wait () ;
		}
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		rax.notify () ;
		const auto r1x = this_.mThreadPendingSet.length () + this_.mThreadPendingSet.size () ;
		const auto r2x = (this_.mItemQueue->length () + r1x - 1) / r1x ;
		for (auto &&i : _RANGE_ (0 ,r2x)) {
			_NOOP_ (i) ;
			list.add (_MOVE_ (this_.mItemQueue.self[this_.mItemQueue->head ()])) ;
			this_.mItemQueue->take () ;
		}
		this_.mThreadPendingSet.erase (tid) ;
	}

	imports void static_rethrow (THIS_PACK &this_ ,const Exception &e) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (this_.mException.exist ())
			return ;
		rax.notify () ;
		this_.mException = AutoRef<Exception>::make (e) ;
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::ThreadBinder :
	delegate public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;
	INDEX mThreadID ;
	BOOL mTestFlag ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (REMOVE_CONST_TYPE<PhanRef<THIS_PACK>> &&this_ ,const INDEX &tid) {
		mThis = _MOVE_ (this_) ;
		mThreadID = tid ;
		mTestFlag = FALSE ;
	}

	void execute () override {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (mTestFlag)
				discard ;
			_CALL_TRY_ ([&] () {
				static_execute (mThis ,mThreadID) ;
			} ,[&] () {
				_NOOP_ () ;
			}) ;
		}
		if switch_once (fax) {
			static_execute (mThis ,mThreadID) ;
		}
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::ThreadCounter :
	delegate private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		const auto r1x = mSelf.mThreadCounter.increase () ;
		_NOOP_ (r1x) ;
	}

	void unlock () {
		const auto r1x = mSelf.mThreadCounter.decrease () ;
		_NOOP_ (r1x) ;
	}
} ;

template <class>
class Future ;

template <class ITEM>
class Promise {
private:
	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
	} ;

	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<AtomicVar> mThreadFlag ;
		AtomicVar mThreadCounter ;
		Function<ITEM ()> mThreadProc ;
		Function<void (ITEM &)> mCallbackProc ;
		AutoRef<Thread> mThreadPool ;
		AutoRef<ITEM> mItem ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

private:
	StrongRef<THREAD_PACK> mThis ;

public:
	implicit Promise () {
		mThis = StrongRef<THREAD_PACK>::make () ;
		mThis->mKeep = UniqueRef<SharedRef<THIS_PACK>> ([&] (SharedRef<THIS_PACK> &me) {
			me = SharedRef<THIS_PACK>::make () ;
		} ,[] (SharedRef<THIS_PACK> &me) {
			static_destroy (me.self) ;
		}) ;
		mThis->mThis = mThis->mKeep.self ;
		auto &r1x = mThis->mThis.self ;
		static_create (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Future<ITEM>>>
	_RET future () const {
		using R1X = DEPENDENT_TYPE<Future<ITEM> ,struct ANONYMOUS> ;
		return R1X (mThis) ;
	}

	void push (const REMOVE_CONST_TYPE<ITEM> &item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_push (r1x ,_MOVE_ (item)) ;
	}

	void push (REMOVE_CONST_TYPE<ITEM> &&item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_push (r1x ,_MOVE_ (item)) ;
	}

	void rethrow (const Exception &e) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_rethrow (r1x) ;
	}

	void start () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		r1x.mThreadFlag = AutoRef<AtomicVar>::make (TRUE) ;
		r1x.mThreadCounter.store (0) ;
		r1x.mThreadProc = Function<ITEM ()> () ;
		r1x.mCallbackProc = Function<void (ITEM &)> () ;
		r1x.mItem = AutoRef<ITEM> () ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = AutoRef<Thread> () ;
	}

	void start (REMOVE_CONST_TYPE<Function<ITEM ()>> &&proc) const {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadBinder ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		r1x.mThreadFlag = AutoRef<AtomicVar>::make (TRUE) ;
		r1x.mThreadCounter.store (0) ;
		r1x.mThreadProc = _MOVE_ (proc) ;
		r1x.mCallbackProc = Function<void (ITEM &)> () ;
		r1x.mItem = AutoRef<ITEM> () ;
		r1x.mException = AutoRef<Exception> () ;
		//@warn: forward object having captured context
		const auto r2x = StrongRef<R1X>::make (PhanRef<THIS_PACK>::make (r1x)) ;
		r1x.mThreadPool = AutoRef<Thread>::make (r2x) ;
	}

	void signal () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_signal (r1x) ;
	}

	void stop () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_destroy (r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Future<ITEM>>>
	imports _RET async (REMOVE_CONST_TYPE<Function<ITEM ()>> &&proc) {
		auto rax = Promise<ITEM> () ;
		rax.start (_MOVE_ (proc)) ;
		return rax.future () ;
	}

private:
	imports void static_create (THIS_PACK &this_) {
		ScopedGuard<Mutex> ANONYMOUS (this_.mThreadMutex) ;
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = AutoRef<Thread> () ;
		this_.mThreadProc = Function<ITEM ()> () ;
		this_.mCallbackProc = Function<void (ITEM &)> () ;
	}

	imports void static_destroy (THIS_PACK &this_) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (!this_.mThreadFlag.exist ())
			return ;
		this_.mThreadFlag->store (FALSE) ;
		while (TRUE) {
			if (!this_.mThreadFlag.exist ())
				break ;
			if (this_.mThreadCounter.fetch () == 0)
				break ;
			rax.yield () ;
		}
		_DEBUG_ASSERT_ (this_.mThreadFlag.exist ()) ;
		if switch_once (TRUE) {
			if (!this_.mThreadPool.exist ())
				discard ;
			rax = UniqueLock () ;
			this_.mThreadPool->join () ;
			rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		}
		this_.mThreadFlag = AutoRef<AtomicVar> () ;
		this_.mThreadCounter.store (0) ;
		this_.mThreadPool = AutoRef<Thread> () ;
		this_.mThreadProc = Function<ITEM ()> () ;
		this_.mCallbackProc = Function<void (ITEM &)> () ;
	}

	imports void static_execute (THIS_PACK &this_) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::ThreadCounter ;
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		ScopedGuard<R1X> ANONYMOUS (_CAST_ (ARGV<R1X>::ID ,this_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		try {
			//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
			rax = this_.mThreadProc () ;
		} catch (const Exception &e) {
			static_rethrow (this_ ,e) ;
		} catch (...) {
			const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
			static_rethrow (this_ ,r1x) ;
		}
		if (rax.exist ())
			static_push (this_ ,_MOVE_ (rax.self)) ;
		static_signal (this_) ;
	}

	imports void static_push (THIS_PACK &this_ ,const REMOVE_CONST_TYPE<ITEM> &item) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		_DYNAMIC_ASSERT_ (!this_.mException.exist ()) ;
		rax.notify () ;
		this_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_push (THIS_PACK &this_ ,REMOVE_CONST_TYPE<ITEM> &&item) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (this_.mThreadFlag->fetch ()) ;
		_DYNAMIC_ASSERT_ (!this_.mException.exist ()) ;
		rax.notify () ;
		this_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_rethrow (THIS_PACK &this_ ,const Exception &e) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		if (this_.mException.exist ())
			return ;
		rax.notify () ;
		this_.mItem = AutoRef<ITEM> () ;
		this_.mException = AutoRef<Exception>::make (e) ;
	}

	imports void static_signal (THIS_PACK &this_) {
		auto rax = this_.mThreadConditionLock.watch (PhanRef<Mutex>::make (this_.mThreadMutex)) ;
		this_.mThreadFlag->store (FALSE) ;
		rax.notify () ;
		if switch_once (TRUE) {
			if (!this_.mItem.exist ())
				discard ;
			if (!this_.mCallbackProc.exist ())
				discard ;
			this_.mCallbackProc (this_.mItem) ;
		}
		this_.mCallbackProc = Function<void (ITEM &)> () ;
	}
} ;

template <class ITEM>
class Promise<ITEM>::Private::ThreadBinder :
	delegate public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;
	BOOL mTestFlag ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (REMOVE_CONST_TYPE<PhanRef<THIS_PACK>> &&this_) {
		mThis = _MOVE_ (this_) ;
		mTestFlag = FALSE ;
	}

	void execute () override {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (mTestFlag)
				discard ;
			_CALL_TRY_ ([&] () {
				static_execute (mThis) ;
			} ,[&] () {
				_NOOP_ () ;
			}) ;
		}
		if switch_once (fax) {
			static_execute (mThis) ;
		}
	}
} ;

template <class ITEM>
class Promise<ITEM>::Private::ThreadCounter :
	delegate private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		const auto r1x = mSelf.mThreadCounter.increase () ;
		_NOOP_ (r1x) ;
	}

	void unlock () {
		const auto r1x = mSelf.mThreadCounter.decrease () ;
		_NOOP_ (r1x) ;
	}
} ;

template <class ITEM>
class Future {
private:
	using THREAD_PACK = typename Promise<ITEM>::THREAD_PACK ;

private:
	StrongRef<THREAD_PACK> mThis ;

public:
	implicit Future () = default ;

	explicit Future (const StrongRef<THREAD_PACK> &this_) {
		mThis = this_ ;
	}

	BOOL ready () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mThreadFlag.exist ())
			return TRUE ;
		if (r1x.mThreadFlag->fetch ())
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () const {
		_STATIC_ASSERT_ (IS_COPY_CONSTRUCTIBLE_HELP<ITEM>::compile ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mThreadFlag->fetch ())
				break ;
			rbx.wait () ;
		}
		rbx.notify () ;
		if (r1x.mException.exist ())
			r1x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r1x.mItem.exist ()) ;
		return r1x.mItem.self ;
	}

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		_STATIC_ASSERT_ (IS_COPY_CONSTRUCTIBLE_HELP<ITEM>::compile ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mThreadFlag->fetch ())
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			rbx.wait (interval) ;
		}
		rbx.notify () ;
		if (r1x.mException.exist ())
			r1x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r1x.mItem.exist ()) ;
		return r1x.mItem.self ;
	}

	ITEM value (const ITEM &def) const {
		_STATIC_ASSERT_ (IS_COPY_CONSTRUCTIBLE_HELP<ITEM>::compile ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mThreadFlag.exist ())
			return def ;
		if (r1x.mThreadFlag->fetch ())
			return def ;
		if (!r1x.mItem.exist ())
			return def ;
		return r1x.mItem.self ;
	}

	void then (Function<void (ITEM &)> &&proc) const {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r1x.mCallbackProc.exist ()) ;
		r1x.mCallbackProc = _MOVE_ (proc) ;
		if (r1x.mThreadFlag->fetch ())
			return ;
		if (!r1x.mItem.exist ())
			return ;
		r1x.mCallbackProc (r1x.mItem.self) ;
		r1x.mCallbackProc = Function<void (ITEM &)> () ;
	}

	void stop () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_destroy (r1x) ;
	}
} ;
} ;