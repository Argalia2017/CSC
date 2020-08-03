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
template <class ITEM>
class CalcThread {
private:
	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<BOOL> mThreadFlag ;
		LENGTH mThreadCounter ;
		Array<Function<MEMPTR<ITEM ()>>> mThreadProc ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
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

	void reserve (const LENGTH &post_len) const {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (r1x.mItemQueue.exist ())
			if (r1x.mItemQueue->length () + post_len <= r1x.mItemQueue->size ())
				return ;
		auto rbx = AutoRef<List<ITEM ,SFIXED>>::make (post_len) ;
		rbx->appand (_MOVE_ (r1x.mItemQueue.self)) ;
		r1x.mItemQueue = _MOVE_ (rbx) ;
	}

	ITEM poll () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->empty ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		ITEM ret = _MOVE_ (r1x.mItemQueue.self[r1x.mItemQueue->head ()]) ;
		r1x.mItemQueue->take () ;
		return _MOVE_ (ret) ;
	}

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->empty ())
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			rbx.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		ITEM ret = _MOVE_ (r1x.mItemQueue.self[r1x.mItemQueue->head ()]) ;
		r1x.mItemQueue->take () ;
		return _MOVE_ (ret) ;
	}

	void start (Array<Function<MEMPTR<ITEM ()>>> &&proc) const {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1x.mThreadCounter == 0) ;
		r1x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1x.mThreadCounter = 0 ;
		r1x.mThreadProc = Array<Function<MEMPTR<ITEM ()>>> (proc.length ()) ;
		for (auto &&i : _RANGE_ (0 ,proc.length ())) {
			_DEBUG_ASSERT_ (proc[i].exist ()) ;
			r1x.mThreadProc[i] = _MOVE_ (proc[i]) ;
		}
		if (!r1x.mItemQueue.exist ())
			r1x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (proc.length ()) ;
		r1x.mItemQueue->clear () ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = Array<AutoRef<Thread>> (proc.size ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			const auto r2x = StrongRef<ThreadBinder>::make (PhanRef<THIS_PACK>::make (r1x) ,i) ;
			r1x.mThreadPool[i] = AutoRef<Thread>::make (r2x) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
			if (r1x.mException.exist ())
				break ;
			if (r1x.mItemQueue->full ())
				break ;
			const auto r2x = predicate () ;
			if (!r2x)
				break ;
			rbx.wait (interval) ;
		}
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
	imports void static_create (THIS_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Array<Function<MEMPTR<ITEM ()>>> () ;
	}

	imports void static_destroy (THIS_PACK &self_) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		rax.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			rax.yield () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
		for (auto &&i : self_.mThreadPool) {
			if (!i.exist ())
				continue ;
			i->join () ;
		}
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Array<Function<MEMPTR<ITEM ()>>> () ;
	}

	imports void static_execute (THIS_PACK &self_ ,const INDEX &tid) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			try {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				rax = self_.mThreadProc[tid] () ;
			} catch (const Exception &e) {
				static_rethrow (self_ ,e) ;
			} catch (...) {
				const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
				static_rethrow (self_ ,r1x) ;
			}
			if (rax.exist ())
				static_push (self_ ,_MOVE_ (rax.self)) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	imports void static_push (THIS_PACK &self_ ,const REMOVE_CONST_TYPE<ITEM> &item) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_once (TRUE) {
			if (!self_.mItemQueue->full ())
				discard ;
			rax.yield () ;
			if (!self_.mItemQueue->full ())
				discard ;
			self_.mItemQueue->take () ;
		}
		self_.mItemQueue->add (_MOVE_ (item)) ;
		rax.notify () ;
	}

	imports void static_push (THIS_PACK &self_ ,REMOVE_CONST_TYPE<ITEM> &&item) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_once (TRUE) {
			if (!self_.mItemQueue->full ())
				discard ;
			rax.yield () ;
			if (!self_.mItemQueue->full ())
				discard ;
			self_.mItemQueue->take () ;
		}
		self_.mItemQueue->add (_MOVE_ (item)) ;
		rax.notify () ;
	}

	imports void static_rethrow (THIS_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;
	INDEX mThreadID ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<THIS_PACK> &&this_ ,const INDEX &tid) {
		mThis = _MOVE_ (this_) ;
		mThreadID = tid ;
	}

	void execute () override {
		_CALL_TRY_ ([&] () {
			static_execute (mThis ,mThreadID) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadCounter
	:private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter-- ;
	}
} ;

template <class ITEM>
class WorkThread {
private:
	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		LENGTH mThreadCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<MEMPTR<void (const ITEM &)>> mThreadProc ;
		Set<INDEX> mThreadPendingSet ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
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
		_DYNAMIC_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
		rbx.notify () ;
	}

	void post (REMOVE_CONST_TYPE<ITEM> &&item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		r1x.mItemQueue->add (_MOVE_ (item)) ;
		rbx.notify () ;
	}

	void post (const Array<REMOVE_CVR_TYPE<ITEM>> &item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->length () + item.length () <= r1x.mItemQueue->size ())
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (item.length ()) ;
			rcx->appand (_MOVE_ (r1x.mItemQueue.self)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		for (auto &&i : _RANGE_ (0 ,item.length ()))
			r1x.mItemQueue->add (_MOVE_ (item[i])) ;
		rbx.notify () ;
	}

	void post (Array<REMOVE_CVR_TYPE<ITEM>> &&item) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		_DYNAMIC_ASSERT_ (r1x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mItemQueue->full ())
				break ;
			rbx.wait () ;
		}
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		if switch_once (TRUE) {
			if (r1x.mItemQueue->length () + item.length () <= r1x.mItemQueue->size ())
				discard ;
			auto rcx = AutoRef<List<ITEM ,SFIXED>>::make (item.length ()) ;
			rcx->appand (_MOVE_ (r1x.mItemQueue.self)) ;
			r1x.mItemQueue = _MOVE_ (rcx) ;
		}
		for (auto &&i : _RANGE_ (0 ,item.length ()))
			r1x.mItemQueue->add (_MOVE_ (item[i])) ;
		rbx.notify () ;
	}

	void start (const LENGTH &count ,Function<MEMPTR<void (const ITEM &)>> &&proc) const {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1x.mThreadCounter == 0) ;
		r1x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1x.mThreadCounter = 0 ;
		r1x.mThreadProc = _MOVE_ (proc) ;
		r1x.mThreadPendingSet = Set<INDEX> (count) ;
		for (auto &&i : _RANGE_ (0 ,count))
			r1x.mThreadPendingSet.add (i) ;
		if (!r1x.mItemQueue.exist ())
			r1x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
		r1x.mItemQueue->clear () ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = Array<AutoRef<Thread>> (count) ;
		for (auto &&i : _RANGE_ (0 ,r1x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			const auto r2x = StrongRef<ThreadBinder>::make (PhanRef<THIS_PACK>::make (r1x) ,i) ;
			r1x.mThreadPool[i] = AutoRef<Thread>::make (r2x) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
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
	imports void static_create (THIS_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Function<MEMPTR<void (const ITEM &)>> () ;
		self_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_destroy (THIS_PACK &self_) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		rax.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			rax.yield () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
		for (auto &&i : self_.mThreadPool) {
			if (!i.exist ())
				continue ;
			i->join () ;
		}
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Function<MEMPTR<void (const ITEM &)>> () ;
		self_.mThreadPendingSet = Set<INDEX> () ;
	}

	imports void static_execute (THIS_PACK &self_ ,const INDEX &tid) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = List<ITEM> () ;
		while (TRUE) {
			if (rax.empty ())
				static_poll (self_ ,tid ,rax) ;
			INDEX ix = rax.head () ;
			try {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				self_.mThreadProc (rax[ix]) ;
			} catch (const Exception &e) {
				static_rethrow (self_ ,e) ;
			} catch (...) {
				const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
				static_rethrow (self_ ,r1x) ;
			}
			rax.remove (ix) ;
		}
	}

	imports void static_poll (THIS_PACK &self_ ,const INDEX &tid ,List<ITEM> &list) {
		struct Dependent ;
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		self_.mThreadPendingSet.add (tid) ;
		while (TRUE) {
			if (!self_.mThreadFlag.self)
				break ;
			if (!self_.mItemQueue->empty ())
				break ;
			rax.wait () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		const auto r1x = self_.mThreadPendingSet.length () + self_.mThreadPendingSet.size () ;
		const auto r2x = (self_.mItemQueue->length () + r1x - 1) / r1x ;
		for (auto &&i : _RANGE_ (0 ,r2x)) {
			list.add (_MOVE_ (self_.mItemQueue.self[self_.mItemQueue->head ()])) ;
			self_.mItemQueue->take () ;
		}
		self_.mThreadPendingSet.erase (tid) ;
	}

	imports void static_rethrow (THIS_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;
	INDEX mThreadID ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<THIS_PACK> &&this_ ,const INDEX &tid) {
		mThis = _MOVE_ (this_) ;
		mThreadID = tid ;
	}

	void execute () override {
		_CALL_TRY_ ([&] () {
			static_execute (mThis ,mThreadID) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::ThreadCounter
	:private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter-- ;
	}
} ;

template <class>
class Future ;

template <class ITEM>
class Promise {
private:
	struct THIS_PACK {
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		LENGTH mThreadCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<MEMPTR<ITEM ()>> mThreadProc ;
		Function<MEMPTR<void (ITEM &)>> mCallbackProc ;
		AutoRef<Thread> mThreadPool ;
		AutoRef<ITEM> mItem ;
		AutoRef<Exception> mException ;
	} ;

	struct THREAD_PACK {
		SharedRef<THIS_PACK> mThis ;
		UniqueRef<SharedRef<THIS_PACK>> mKeep ;
	} ;

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
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
		struct Dependent ;
		using Future = DEPENDENT_TYPE<Future<ITEM> ,Dependent> ;
		return Future (mThis) ;
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
		_DEBUG_ASSERT_ (r1x.mThreadCounter == 0) ;
		r1x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1x.mThreadCounter = 0 ;
		r1x.mThreadProc = Function<MEMPTR<ITEM ()>> () ;
		r1x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
		r1x.mItem = AutoRef<ITEM> () ;
		r1x.mException = AutoRef<Exception> () ;
		r1x.mThreadPool = AutoRef<Thread> () ;
	}

	void start (Function<MEMPTR<ITEM ()>> &&proc) const {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1x.mThreadCounter == 0) ;
		r1x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1x.mThreadCounter = 0 ;
		r1x.mThreadProc = _MOVE_ (proc) ;
		r1x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
		r1x.mItem = AutoRef<ITEM> () ;
		r1x.mException = AutoRef<Exception> () ;
		//@warn: forward object having captured context
		const auto r2x = StrongRef<ThreadBinder>::make (PhanRef<THIS_PACK>::make (r1x)) ;
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
	imports _RET async (Function<MEMPTR<ITEM ()>> &&proc) {
		auto rax = Promise<ITEM> () ;
		rax.start (_MOVE_ (proc)) ;
		return rax.future () ;
	}

private:
	imports void static_create (THIS_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = AutoRef<Thread> () ;
		self_.mThreadProc = Function<MEMPTR<ITEM ()>> () ;
		self_.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	imports void static_destroy (THIS_PACK &self_) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		rax.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			rax.yield () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mThreadPool.exist ())
			self_.mThreadPool->join () ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = AutoRef<Thread> () ;
		self_.mThreadProc = Function<MEMPTR<ITEM ()>> () ;
		self_.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	imports void static_execute (THIS_PACK &self_) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		try {
			//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
			rax = self_.mThreadProc () ;
		} catch (const Exception &e) {
			static_rethrow (self_ ,e) ;
		} catch (...) {
			const auto r1x = Exception (_PCSTR_ ("unknown C++ exception")) ;
			static_rethrow (self_ ,r1x) ;
		}
		if (rax.exist ())
			static_push (self_ ,_MOVE_ (rax.self)) ;
		static_signal (self_) ;
	}

	imports void static_push (THIS_PACK &self_ ,const REMOVE_CONST_TYPE<ITEM> &item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_push (THIS_PACK &self_ ,REMOVE_CONST_TYPE<ITEM> &&item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_rethrow (THIS_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM> () ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	imports void static_signal (THIS_PACK &self_) {
		auto rax = self_.mThreadConditionLock.watch (PhanRef<Mutex>::make (self_.mThreadMutex)) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		self_.mThreadFlag.self = FALSE ;
		rax.notify () ;
		if switch_once (TRUE) {
			if (!self_.mItem.exist ())
				discard ;
			if (!self_.mCallbackProc.exist ())
				discard ;
			self_.mCallbackProc (self_.mItem) ;
		}
		self_.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}
} ;

template <class ITEM>
class Promise<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<THIS_PACK> mThis ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<THIS_PACK> &&this_) {
		mThis = _MOVE_ (this_) ;
	}

	void execute () override {
		_CALL_TRY_ ([&] () {
			static_execute (mThis) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
	}
} ;

template <class ITEM>
class Promise<ITEM>::Private::ThreadCounter
	:private Wrapped<THIS_PACK> {
private:
	using Wrapped<THIS_PACK>::mSelf ;

public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (mSelf.mThreadMutex) ;
		mSelf.mThreadCounter-- ;
	}
} ;

template <class ITEM>
class Future {
private:
	using THREAD_PACK = typename Promise<ITEM>::THREAD_PACK ;

private:
	StrongRef<THREAD_PACK> mThis ;

public:
	implicit Future () = delete ;

	explicit Future (const StrongRef<THREAD_PACK> &this_) {
		mThis = this_.share () ;
	}

	BOOL ready () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mThreadFlag.exist ())
			return TRUE ;
		if (r1x.mThreadFlag.self)
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mThreadFlag.self)
				break ;
			rbx.wait () ;
		}
		if (r1x.mException.exist ())
			r1x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r1x.mItem.exist ()) ;
		ITEM ret = _MOVE_ (r1x.mItem.self) ;
		r1x.mItem = AutoRef<ITEM> () ;
		return _MOVE_ (ret) ;
	}

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		auto rbx = r1x.mThreadConditionLock.watch (PhanRef<Mutex>::make (r1x.mThreadMutex)) ;
		while (TRUE) {
			if (!r1x.mThreadFlag.exist ())
				break ;
			if (!r1x.mThreadFlag.self)
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			rbx.wait (interval) ;
		}
		if (r1x.mException.exist ())
			r1x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r1x.mItem.exist ()) ;
		ITEM ret = _MOVE_ (r1x.mItem.self) ;
		r1x.mItem = AutoRef<ITEM> () ;
		return _MOVE_ (ret) ;
	}

	ITEM value (const ITEM &def) const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		if (!r1x.mThreadFlag.exist ())
			return def ;
		if (r1x.mThreadFlag.self)
			return def ;
		if (!r1x.mItem.exist ())
			return def ;
		return r1x.mItem.self ;
	}

	void then (Function<MEMPTR<void (ITEM &)>> &&proc) const {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		ScopedGuard<Mutex> ANONYMOUS (r1x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!r1x.mCallbackProc.exist ()) ;
		r1x.mCallbackProc = _MOVE_ (proc) ;
		if (r1x.mThreadFlag.self)
			return ;
		if (!r1x.mItem.exist ())
			return ;
		r1x.mCallbackProc (r1x.mItem.self) ;
		r1x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	void stop () const {
		auto rax = mThis.share () ;
		auto &r1x = rax->mThis.self ;
		static_destroy (r1x) ;
	}
} ;
} ;