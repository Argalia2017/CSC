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
	struct SELF_PACK {
		Atomic mCounter ;
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<BOOL> mThreadFlag ;
		LENGTH mThreadCounter ;
		Array<Function<MEMPTR<ITEM ()>>> mThreadProc ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
	} ;

private:
	friend IntrusiveRef<SELF_PACK ,CalcThread> ;
	IntrusiveRef<SELF_PACK ,CalcThread> mThis ;

public:
 	implicit CalcThread () {
		mThis = IntrusiveRef<SELF_PACK ,CalcThread>::make () ;
	}

	LENGTH size () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->size () ;
	}

	LENGTH length () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->length () ;
	}

	void reserve (const LENGTH &post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (r2x.mItemQueue.exist ())
			if (r2x.mItemQueue->length () + post_len <= r2x.mItemQueue->size ())
				return ;
		auto tmp = AutoRef<List<ITEM ,SFIXED>>::make (post_len) ;
		tmp->appand (_MOVE_ (r2x.mItemQueue.self)) ;
		r2x.mItemQueue = _MOVE_ (tmp) ;
	}

	ITEM poll () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->empty ())
				break ;
			r3x.wait () ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		ITEM ret = _MOVE_ (r2x.mItemQueue.self[r2x.mItemQueue->head ()]) ;
		r2x.mItemQueue->take () ;
		return _MOVE_ (ret) ;
	}

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->empty ())
				break ;
			const auto r4x = predicate () ;
			_DYNAMIC_ASSERT_ (r4x) ;
			r3x.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		ITEM ret = _MOVE_ (r2x.mItemQueue.self[r2x.mItemQueue->head ()]) ;
		r2x.mItemQueue->take () ;
		return _MOVE_ (ret) ;
	}

	void start (Array<Function<MEMPTR<ITEM ()>>> &&proc) {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = Array<Function<MEMPTR<ITEM ()>>> (proc.length ()) ;
		for (auto &&i : _RANGE_ (0 ,proc.length ())) {
			_DEBUG_ASSERT_ (proc[i].exist ()) ;
			r2x.mThreadProc[i] = _MOVE_ (proc[i]) ;
		}
		if (!r2x.mItemQueue.exist ())
			r2x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (proc.length ()) ;
		r2x.mItemQueue->clear () ;
		r2x.mException = AutoRef<Exception> () ;
		r2x.mThreadPool = Array<AutoRef<Thread>> (proc.size ()) ;
		for (auto &&i : _RANGE_ (0 ,r2x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			auto tmp = StrongRef<ThreadBinder>::make (PhanRef<SELF_PACK>::make (r2x) ,i) ;
			r2x.mThreadPool[i] = AutoRef<Thread>::make (_MOVE_ (tmp)) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mItemQueue->size () > 0) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
			if (r2x.mException.exist ())
				break ;
			if (r2x.mItemQueue->full ())
				break ;
			const auto r4x = predicate () ;
			if (!r4x)
				break ;
			r3x.wait (interval) ;
		}
		const auto r5x = _MOVE_ (r2x.mException) ;
		if (!r5x.exist ())
			return ;
		r5x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		friend_destroy (r1x) ;
	}

private:
	imports void static_execute (SELF_PACK &self_ ,const INDEX &tid) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			_CATCH_ ([&] () {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				rax = self_.mThreadProc[tid] () ;
			} ,[&] (const Exception &e) {
				_CALL_TRY_ ([&] () {
					static_rethrow (self_ ,e) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
			if (rax.exist ())
				static_push (self_ ,_MOVE_ (rax.self)) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	imports void static_push (SELF_PACK &self_ ,const REMOVE_CVR_TYPE<ITEM> &item) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_once (TRUE) {
			if (!self_.mItemQueue->full ())
				discard ;
			r1x.yield () ;
			if (!self_.mItemQueue->full ())
				discard ;
			self_.mItemQueue->take () ;
		}
		self_.mItemQueue->add (_MOVE_ (item)) ;
		r1x.notify () ;
	}

	imports void static_push (SELF_PACK &self_ ,REMOVE_CVR_TYPE<ITEM> &&item) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_once (TRUE) {
			if (!self_.mItemQueue->full ())
				discard ;
			r1x.yield () ;
			if (!self_.mItemQueue->full ())
				discard ;
			self_.mItemQueue->take () ;
		}
		self_.mItemQueue->add (_MOVE_ (item)) ;
		r1x.notify () ;
	}

	imports void static_rethrow (SELF_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	imports void friend_create (SELF_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Array<Function<MEMPTR<ITEM ()>>> () ;
	}

	imports void friend_destroy (SELF_PACK &self_) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		r1x.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			r1x.yield () ;
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

	imports LENGTH friend_attach (SELF_PACK &self_) side_effects {
		return ++self_.mCounter ;
	}

	imports LENGTH friend_detach (SELF_PACK &self_) side_effects {
		return --self_.mCounter ;
	}

	imports void friend_latch (SELF_PACK &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<SELF_PACK> mThis ;
	INDEX mIndex ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<SELF_PACK> &&this_ ,const INDEX &index) {
		mThis = _MOVE_ (this_) ;
		mIndex = index ;
	}

	void execute () override {
		_CALL_TRY_ ([&] () {
			static_execute (mThis ,mIndex) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
	}
} ;

template <class ITEM>
class CalcThread<ITEM>::Private::ThreadCounter
	:private Wrapped<SELF_PACK> {
public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter-- ;
	}
} ;

template <class ITEM>
class WorkThread {
private:
	struct SELF_PACK {
		Atomic mCounter ;
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		LENGTH mThreadCounter ;
		LENGTH mThreadWaitCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<MEMPTR<void (const ITEM &)>> mThreadProc ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;

		class Counter ;
	} ;

private:
	friend IntrusiveRef<SELF_PACK ,WorkThread> ;
	IntrusiveRef<SELF_PACK ,WorkThread> mThis ;

public:
 	implicit WorkThread () {
		mThis = IntrusiveRef<SELF_PACK ,WorkThread>::make () ;
	}

	LENGTH size () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->size () ;
	}

	LENGTH length () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->length () ;
	}

	void reserve (const LENGTH &post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (r2x.mItemQueue.exist ())
			if (r2x.mItemQueue->length () + post_len <= r2x.mItemQueue->size ())
				return ;
		auto tmp = AutoRef<List<ITEM ,SFIXED>>::make (post_len) ;
		tmp->appand (_MOVE_ (r2x.mItemQueue.self)) ;
		r2x.mItemQueue = _MOVE_ (tmp) ;
	}

	void post (const REMOVE_CVR_TYPE<ITEM> &item) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->full ())
				break ;
			r3x.wait () ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		r2x.mItemQueue->add (_MOVE_ (item)) ;
		r3x.notify () ;
	}

	void post (REMOVE_CVR_TYPE<ITEM> &&item) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->full ())
				break ;
			r3x.wait () ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		r2x.mItemQueue->add (_MOVE_ (item)) ;
		r3x.notify () ;
	}

	void post (const REMOVE_CVR_TYPE<ITEM> &item ,const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->full ())
				break ;
			const auto r4x = predicate () ;
			_DYNAMIC_ASSERT_ (r4x) ;
			r3x.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		r2x.mItemQueue->add (_MOVE_ (item)) ;
		r3x.notify () ;
	}

	void post (REMOVE_CVR_TYPE<ITEM> &&item ,const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mItemQueue->full ())
				break ;
			const auto r4x = predicate () ;
			_DYNAMIC_ASSERT_ (r4x) ;
			r3x.wait (interval) ;
		}
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		r2x.mItemQueue->add (_MOVE_ (item)) ;
		r3x.notify () ;
	}

	void start (const LENGTH &count ,Function<MEMPTR<void (const ITEM &)>> &&proc) {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadWaitCounter = 0 ;
		r2x.mThreadProc = _MOVE_ (proc) ;
		if (!r2x.mItemQueue.exist ())
			r2x.mItemQueue = AutoRef<List<ITEM ,SFIXED>>::make (count) ;
		r2x.mItemQueue->clear () ;
		r2x.mException = AutoRef<Exception> () ;
		r2x.mThreadPool = Array<AutoRef<Thread>> (count) ;
		for (auto &&i : _RANGE_ (0 ,r2x.mThreadPool.length ())) {
			//@warn: forward object having captured context
			auto tmp = StrongRef<ThreadBinder>::make (PhanRef<SELF_PACK>::make (r2x)) ;
			r2x.mThreadPool[i] = AutoRef<Thread>::make (_MOVE_ (tmp)) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
			if (r2x.mException.exist ())
				break ;
			if (r2x.mThreadWaitCounter >= r2x.mThreadPool.length ())
				if (r2x.mItemQueue->empty ())
					break ;
			const auto r4x = predicate () ;
			if (!r4x)
				break ;
			r3x.wait (interval) ;
		}
		const auto r5x = _MOVE_ (r2x.mException) ;
		if (!r5x.exist ())
			return ;
		r5x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		friend_destroy (r1x) ;
	}

private:
	imports void static_execute (SELF_PACK &self_) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			static_poll (self_ ,rax) ;
			_CATCH_ ([&] () {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				self_.mThreadProc (rax.self) ;
			} ,[&] (const Exception &e) {
				_CALL_TRY_ ([&] () {
					static_rethrow (self_ ,e) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	imports void static_poll (SELF_PACK &self_ ,Optional<ITEM> &item) {
		struct Dependent ;
		using Counter = typename DEPENDENT_TYPE<Private ,Dependent>::Counter ;
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_ (ARGV<Counter>::null ,self_.mThreadWaitCounter)) ;
		while (TRUE) {
			if (!self_.mThreadFlag.self)
				break ;
			if (!self_.mItemQueue->empty ())
				break ;
			r1x.wait () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		item = _MOVE_ (self_.mItemQueue.self[self_.mItemQueue->head ()]) ;
		self_.mItemQueue->take () ;
	}

	imports void static_rethrow (SELF_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	imports void friend_create (SELF_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadWaitCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Function<MEMPTR<void (const ITEM &)>> () ;
	}

	imports void friend_destroy (SELF_PACK &self_) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		r1x.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			r1x.yield () ;
		}
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
		for (auto &&i : self_.mThreadPool) {
			if (!i.exist ())
				continue ;
			i->join () ;
		}
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadWaitCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Function<MEMPTR<void (const ITEM &)>> () ;
	}

	imports LENGTH friend_attach (SELF_PACK &self_) side_effects {
		return ++self_.mCounter ;
	}

	imports LENGTH friend_detach (SELF_PACK &self_) side_effects {
		return --self_.mCounter ;
	}

	imports void friend_latch (SELF_PACK &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<SELF_PACK> mThis ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<SELF_PACK> &&this_) {
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
class WorkThread<ITEM>::Private::ThreadCounter
	:private Wrapped<SELF_PACK> {
public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter-- ;
	}
} ;

template <class ITEM>
class WorkThread<ITEM>::Private::Counter
	:private Wrapped<LENGTH> {
public:
	void lock () {
		Counter::mSelf++ ;
	}

	void unlock () {
		Counter::mSelf-- ;
	}
} ;

template <class>
class Future ;

template <class ITEM>
class Promise {
private:
	struct SELF_PACK {
		Atomic mCounter ;
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

	struct Private {
		class ThreadBinder ;

		class ThreadCounter ;
	} ;

private:
	friend IntrusiveRef<SELF_PACK ,Promise> ;
	IntrusiveRef<SELF_PACK ,Promise> mThis ;

public:
 	implicit Promise () {
		mThis = IntrusiveRef<SELF_PACK ,Promise>::make () ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Future<ITEM>>>
	_RET future () side_effects {
		struct Dependent ;
		using Future_ITEM = DEPENDENT_TYPE<Future<ITEM> ,Dependent> ;
		return Future_ITEM (mThis.share ()) ;
	}

	void push (const REMOVE_CVR_TYPE<ITEM> &item) {
		const auto r1x = mThis.watch () ;
		static_push (r1x) ;
	}

	void push (REMOVE_CVR_TYPE<ITEM> &&item) {
		const auto r1x = mThis.watch () ;
		static_push (r1x) ;
	}

	void rethrow (const Exception &e) {
		const auto r1x = mThis.watch () ;
		static_rethrow (r1x) ;
	}

	void start () {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = Function<MEMPTR<ITEM ()>> () ;
		r2x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
		r2x.mItem = AutoRef<ITEM> () ;
		r2x.mException = AutoRef<Exception> () ;
		r2x.mThreadPool = AutoRef<Thread> () ;
	}

	void start (Function<MEMPTR<ITEM ()>> &&proc) {
		struct Dependent ;
		using ThreadBinder = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadBinder ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = _MOVE_ (proc) ;
		r2x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
		r2x.mItem = AutoRef<ITEM> () ;
		r2x.mException = AutoRef<Exception> () ;
		//@warn: forward object having captured context
		auto tmp = StrongRef<ThreadBinder>::make (PhanRef<SELF_PACK>::make (r2x)) ;
		r2x.mThreadPool = AutoRef<Thread>::make (_MOVE_ (tmp)) ;
	}

	void signal () {
		const auto r1x = mThis.watch () ;
		static_signal (r1x) ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		friend_destroy (r1x) ;
	}

public:
	template <class _RET = REMOVE_CVR_TYPE<Future<ITEM>>>
	imports _RET async (Function<MEMPTR<ITEM ()>> &&proc) {
		auto rax = Promise<ITEM> () ;
		rax.start (_MOVE_ (proc)) ;
		return rax.future () ;
	}

private:
	imports void static_execute (SELF_PACK &self_) {
		struct Dependent ;
		using ThreadCounter = typename DEPENDENT_TYPE<Private ,Dependent>::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_ (ARGV<ThreadCounter>::null ,self_)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		_CATCH_ ([&] () {
			//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
			rax = self_.mThreadProc () ;
		} ,[&] (const Exception &e) {
			_CALL_TRY_ ([&] () {
				static_rethrow (self_ ,e) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}) ;
		if (rax.exist ())
			static_push (self_ ,_MOVE_ (rax.self)) ;
		static_signal (self_) ;
	}

	imports void static_push (SELF_PACK &self_ ,const REMOVE_CVR_TYPE<ITEM> &item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_push (SELF_PACK &self_ ,REMOVE_CVR_TYPE<ITEM> &&item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	imports void static_rethrow (SELF_PACK &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM> () ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	imports void static_signal (SELF_PACK &self_) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		self_.mThreadFlag.self = FALSE ;
		r1x.notify () ;
		if switch_once (TRUE) {
			if (!self_.mItem.exist ())
				discard ;
			if (!self_.mCallbackProc.exist ())
				discard ;
			self_.mCallbackProc (self_.mItem) ;
		}
		self_.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	imports void friend_create (SELF_PACK &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = AutoRef<Thread> () ;
		self_.mThreadProc = Function<MEMPTR<ITEM ()>> () ;
		self_.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	imports void friend_destroy (SELF_PACK &self_) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		if (!self_.mThreadFlag.exist ())
			return ;
		self_.mThreadFlag.self = FALSE ;
		r1x.notify () ;
		while (TRUE) {
			if (!self_.mThreadFlag.exist ())
				break ;
			if (self_.mThreadCounter == 0)
				break ;
			r1x.yield () ;
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

	imports LENGTH friend_attach (SELF_PACK &self_) side_effects {
		return ++self_.mCounter ;
	}

	imports LENGTH friend_detach (SELF_PACK &self_) side_effects {
		return --self_.mCounter ;
	}

	imports void friend_latch (SELF_PACK &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
class Promise<ITEM>::Private::ThreadBinder
	:public Thread::Binder {
private:
	PhanRef<SELF_PACK> mThis ;

public:
	implicit ThreadBinder () = delete ;

	explicit ThreadBinder (PhanRef<SELF_PACK> &&this_) {
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
	:private Wrapped<SELF_PACK> {
public:
	void lock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter++ ;
	}

	void unlock () {
		ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
		ThreadCounter::mSelf.mThreadCounter-- ;
	}
} ;

template <class ITEM>
class Future {
private:
	using SELF_PACK = typename Promise<ITEM>::SELF_PACK ;

private:
	IntrusiveRef<SELF_PACK ,Promise<ITEM>> mThis ;

public:
	implicit Future () = delete ;

	explicit Future (IntrusiveRef<SELF_PACK ,Promise<ITEM>> &&this_)
		: mThis (_MOVE_ (this_)) {}

	BOOL ready () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mThreadFlag.exist ())
			return TRUE ;
		if (r2x.mThreadFlag.self)
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mThreadFlag.self)
				break ;
			r3x.wait () ;
		}
		if (r2x.mException.exist ())
			r2x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r2x.mItem.exist ()) ;
		ITEM ret = _MOVE_ (r2x.mItem.self) ;
		r2x.mItem = AutoRef<ITEM> () ;
		return _MOVE_ (ret) ;
	}

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		const auto r3x = r2x.mThreadConditionLock.watch (r2x.mThreadMutex) ;
		while (TRUE) {
			if (!r2x.mThreadFlag.exist ())
				break ;
			if (!r2x.mThreadFlag.self)
				break ;
			const auto r4x = predicate () ;
			_DYNAMIC_ASSERT_ (r4x) ;
			r3x.wait (interval) ;
		}
		if (r2x.mException.exist ())
			r2x.mException->raise () ;
		_DYNAMIC_ASSERT_ (r2x.mItem.exist ()) ;
		ITEM ret = _MOVE_ (r2x.mItem.self) ;
		r2x.mItem = AutoRef<ITEM> () ;
		return _MOVE_ (ret) ;
	}

	ITEM value (const ITEM &def) side_effects {
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mThreadFlag.exist ())
			return def ;
		if (r2x.mThreadFlag.self)
			return def ;
		if (!r2x.mItem.exist ())
			return def ;
		return r2x.mItem.self ;
	}

	void then (Function<MEMPTR<void (ITEM &)>> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _XVALUE_ (ARGV<SELF_PACK>::null ,r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!r2x.mCallbackProc.exist ()) ;
		r2x.mCallbackProc = _MOVE_ (proc) ;
		if (r2x.mThreadFlag.self)
			return ;
		if (!r2x.mItem.exist ())
			return ;
		r2x.mCallbackProc (r2x.mItem.self) ;
		r2x.mCallbackProc = Function<MEMPTR<void (ITEM &)>> () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		friend_destroy (r1x) ;
	}
} ;
} ;