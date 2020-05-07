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
	class Pack {
	private:
		friend CalcThread ;
		Atomic mCounter ;
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		AutoRef<BOOL> mThreadFlag ;
		LENGTH mThreadCounter ;
		Array<Function<DEF<ITEM ()> NONE::*>> mThreadProc ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

private:
	struct Detail ;
	friend IntrusiveRef<Pack ,CalcThread> ;
	IntrusiveRef<Pack ,CalcThread> mThis ;

public:
	CalcThread () {
		mThis = IntrusiveRef<Pack ,CalcThread>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->length () ;
	}

	void reserve (const LENGTH &post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (r2x.mItemQueue.exist ())
			if (r2x.mItemQueue->length () + post_len <= r2x.mItemQueue->size ())
				return ;
		auto tmp = AutoRef<List<ITEM ,SFIXED>>::make (post_len) ;
		tmp->appand (_MOVE_ (r2x.mItemQueue.self)) ;
		r2x.mItemQueue = _MOVE_ (tmp) ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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

	void start (Array<Function<DEF<ITEM ()> NONE::*>> &&proc) {
		using LocalProc = typename Detail::LocalProc ;
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> (proc.length ()) ;
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
			auto tmp = StrongRef<LocalProc>::make (PhanRef<Pack>::make (r2x) ,i) ;
			r2x.mThreadPool[i] = AutoRef<Thread>::make (_MOVE_ (tmp)) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
	static void static_execute (Pack &self_ ,const INDEX &tid) {
		using ThreadCounter = typename Detail::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
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

	static void static_push (Pack &self_ ,const REMOVE_CVR_TYPE<ITEM> &item) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_case (TRUE) {
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

	static void static_push (Pack &self_ ,REMOVE_CVR_TYPE<ITEM> &&item) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
		if switch_case (TRUE) {
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

	static void static_rethrow (Pack &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	static void friend_create (Pack &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
	}

	static void friend_destroy (Pack &self_) {
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
		self_.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
struct CalcThread<ITEM>::Detail {
	class LocalProc
		:public Thread::Binder {
	private:
		PhanRef<Pack> mThis ;
		INDEX mIndex ;

	public:
		inline LocalProc () = delete ;

		inline explicit LocalProc (const PhanRef<Pack> &this_ ,const INDEX &index) {
			mThis = PhanRef<Pack>::make (this_) ;
			mIndex = index ;
		}

		inline void execute () override {
			_CALL_TRY_ ([&] () {
				static_execute (mThis ,mIndex) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}
	} ;

	class ThreadCounter
		:private Wrapped<Pack> {
	public:
		inline void lock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter++ ;
		}

		inline void unlock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter-- ;
		}
	} ;
} ;

template <class ITEM>
class WorkThread {
private:
	class Pack {
	private:
		friend WorkThread ;
		Atomic mCounter ;
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		LENGTH mThreadCounter ;
		LENGTH mThreadWaitCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<DEF<void (const ITEM &)> NONE::*> mThreadProc ;
		Array<AutoRef<Thread>> mThreadPool ;
		AutoRef<List<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

private:
	struct Detail ;
	friend IntrusiveRef<Pack ,WorkThread> ;
	IntrusiveRef<Pack ,WorkThread> mThis ;

public:
	WorkThread () {
		mThis = IntrusiveRef<Pack ,WorkThread>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mItemQueue.exist ())
			return 0 ;
		return r2x.mItemQueue->length () ;
	}

	void reserve (const LENGTH &post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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

	void start (const LENGTH &count ,Function<DEF<void (const ITEM &)> NONE::*> &&proc) {
		using LocalProc = typename Detail::LocalProc ;
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
			auto tmp = StrongRef<LocalProc>::make (PhanRef<Pack>::make (r2x)) ;
			r2x.mThreadPool[i] = AutoRef<Thread>::make (_MOVE_ (tmp)) ;
		}
	}

	void join (const Duration &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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
	static void static_execute (Pack &self_) {
		using ThreadCounter = typename Detail::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
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

	static void static_poll (Pack &self_ ,Optional<ITEM> &item) {
		using Counter = typename Detail::Counter ;
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (self_.mThreadWaitCounter)) ;
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

	static void static_rethrow (Pack &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		if (self_.mException.exist ())
			return ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	static void friend_create (Pack &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadWaitCounter = 0 ;
		self_.mThreadPool = Array<AutoRef<Thread>> () ;
		self_.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
	}

	static void friend_destroy (Pack &self_) {
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
		self_.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
struct WorkThread<ITEM>::Detail {
	class LocalProc
		:public Thread::Binder {
	private:
		PhanRef<Pack> mThis ;

	public:
		inline LocalProc () = delete ;

		inline explicit LocalProc (const PhanRef<Pack> &this_) {
			mThis = PhanRef<Pack>::make (this_) ;
		}

		inline void execute () override {
			_CALL_TRY_ ([&] () {
				static_execute (mThis) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}
	} ;

	class ThreadCounter
		:private Wrapped<Pack> {
	public:
		inline void lock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter++ ;
		}

		inline void unlock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter-- ;
		}
	} ;

	class Counter
		:private Wrapped<LENGTH> {
	public:
		inline void lock () {
			Counter::mSelf++ ;
		}

		inline void unlock () {
			Counter::mSelf-- ;
		}
	} ;
} ;

template <class>
class Future ;

template <class ITEM>
class Promise {
private:
	class Pack {
	private:
		friend Promise ;
		friend Future<ITEM> ;
		Atomic mCounter ;
		Mutex mThreadMutex ;
		ConditionLock mThreadConditionLock ;
		LENGTH mThreadCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<DEF<ITEM ()> NONE::*> mThreadProc ;
		Function<DEF<void (ITEM &)> NONE::*> mCallbackProc ;
		AutoRef<Thread> mThreadPool ;
		AutoRef<ITEM> mItem ;
		AutoRef<Exception> mException ;
	} ;

private:
	struct Detail ;
	friend Future<ITEM> ;
	friend IntrusiveRef<Pack ,Promise> ;
	IntrusiveRef<Pack ,Promise> mThis ;

public:
	Promise () {
		mThis = IntrusiveRef<Pack ,Promise>::make () ;
	}

	DEPENDENT_TYPE<Future<ITEM> ,Promise> future () popping {
		return DEPENDENT_TYPE<Future<ITEM> ,Promise> (mThis) ;
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
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		r2x.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r2x.mItem = AutoRef<ITEM> () ;
		r2x.mException = AutoRef<Exception> () ;
		r2x.mThreadPool = AutoRef<Thread> () ;
	}

	void start (Function<DEF<ITEM ()> NONE::*> &&proc) {
		using LocalProc = typename Detail::LocalProc ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2x.mThreadCounter == 0) ;
		r2x.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2x.mThreadCounter = 0 ;
		r2x.mThreadProc = _MOVE_ (proc) ;
		r2x.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r2x.mItem = AutoRef<ITEM> () ;
		r2x.mException = AutoRef<Exception> () ;
		//@warn: forward object having captured context
		auto tmp = StrongRef<LocalProc>::make (PhanRef<Pack>::make (r2x)) ;
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
	imports_static DEPENDENT_TYPE<Future<ITEM> ,Promise> async (Function<DEF<ITEM ()> NONE::*> &&proc) {
		auto rax = Promise<ITEM> () ;
		rax.start (_MOVE_ (proc)) ;
		return rax.future () ;
	}

private:
	explicit Promise (IntrusiveRef<Pack ,Promise> &this_)
		: mThis (this_.share ()) {}

private:
	static void static_execute (Pack &self_) {
		using ThreadCounter = typename Detail::ThreadCounter ;
		ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
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

	static void static_push (Pack &self_ ,const REMOVE_CVR_TYPE<ITEM> &item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	static void static_push (Pack &self_ ,REMOVE_CVR_TYPE<ITEM> &&item) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM>::make (_MOVE_ (item)) ;
	}

	static void static_rethrow (Pack &self_ ,const Exception &e) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
		self_.mItem = AutoRef<ITEM> () ;
		self_.mException = AutoRef<Exception>::make (e) ;
	}

	static void static_signal (Pack &self_) {
		const auto r1x = self_.mThreadConditionLock.watch (self_.mThreadMutex) ;
		_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
		self_.mThreadFlag.self = FALSE ;
		r1x.notify () ;
		if switch_case (TRUE) {
			if (!self_.mItem.exist ())
				discard ;
			if (!self_.mCallbackProc.exist ())
				discard ;
			self_.mCallbackProc (self_.mItem) ;
		}
		self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static void friend_create (Pack &self_) {
		ScopedGuard<Mutex> ANONYMOUS (self_.mThreadMutex) ;
		self_.mCounter = 0 ;
		self_.mThreadFlag = AutoRef<BOOL> () ;
		self_.mThreadCounter = 0 ;
		self_.mThreadPool = AutoRef<Thread> () ;
		self_.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static void friend_destroy (Pack &self_) {
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
		self_.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_yield () ;
	}
} ;

template <class ITEM>
struct Promise<ITEM>::Detail {
	class LocalProc
		:public Thread::Binder {
	private:
		PhanRef<Pack> mThis ;

	public:
		inline LocalProc () = delete ;

		inline explicit LocalProc (const PhanRef<Pack> &this_) {
			mThis = PhanRef<Pack>::make (this_) ;
		}

		inline void execute () override {
			_CALL_TRY_ ([&] () {
				static_execute (mThis) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}
	} ;

	class ThreadCounter
		:private Wrapped<Pack> {
	public:
		inline void lock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter++ ;
		}

		inline void unlock () {
			ScopedGuard<Mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
			ThreadCounter::mSelf.mThreadCounter-- ;
		}
	} ;
} ;

template <class ITEM>
class Future {
private:
	using Pack = typename Promise<ITEM>::Pack ;

private:
	friend Promise<ITEM> ;
	IntrusiveRef<Pack ,Promise<ITEM>> mThis ;

public:
	Future () = delete ;

	BOOL ready () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mThreadFlag.exist ())
			return TRUE ;
		if (r2x.mThreadFlag.self)
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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

	ITEM poll (const Duration &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
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

	ITEM value (const ITEM &def) popping {
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		if (!r2x.mThreadFlag.exist ())
			return def ;
		if (r2x.mThreadFlag.self)
			return def ;
		if (!r2x.mItem.exist ())
			return def ;
		return r2x.mItem.self ;
	}

	void then (Function<DEF<void (ITEM &)> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2x = _FORWARD_<Pack &> (r1x) ;
		ScopedGuard<Mutex> ANONYMOUS (r2x.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2x.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!r2x.mCallbackProc.exist ()) ;
		r2x.mCallbackProc = _MOVE_ (proc) ;
		if (r2x.mThreadFlag.self)
			return ;
		if (!r2x.mItem.exist ())
			return ;
		r2x.mCallbackProc (r2x.mItem.self) ;
		r2x.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		friend_destroy (r1x) ;
	}

private:
	explicit Future (IntrusiveRef<Pack ,Promise<ITEM>> &this_)
		: mThis (this_.share ()) {}
} ;
} ;