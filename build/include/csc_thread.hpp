#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class ITEM>
class CalcThread {
private:
	class Holder {
	public:
		using INTRUSIVE_TYPE = CalcThread ;

	private:
		friend CalcThread ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mThreadMutex ;
		Monostate<std::condition_variable> mThreadCondition ;
		AutoRef<BOOL> mThreadFlag ;
		LENGTH mThreadCounter ;
		Array<Function<DEF<ITEM ()> NONE::*>> mThreadProc ;
		Array<AutoRef<std::thread>> mThreadPool ;
		AutoRef<QList<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

private:
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	CalcThread () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mItemQueue.exist ())
			return 0 ;
		return r1.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mItemQueue.exist ())
			return 0 ;
		return r1.mItemQueue->length () ;
	}

	void reserve (LENGTH post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (r1.mItemQueue.exist () && r1.mItemQueue->length () + post_len <= r1.mItemQueue->size ())
			return ;
		auto rax = AutoRef<QList<ITEM ,SFIXED>>::make (post_len) ;
		rax->appand (std::move (r1.mItemQueue.self)) ;
		r1.mItemQueue = std::move (rax) ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		while (r1.mThreadFlag.exist () && r1.mItemQueue->empty ())
			r1.mThreadCondition.self.wait (sgd) ;
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		ITEM ret = std::move (r1.mItemQueue.self[r1.mItemQueue->peek ()]) ;
		r1.mItemQueue->take () ;
		return std::move (ret) ;
	}

	ITEM poll (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		while (TRUE) {
			if (!r1.mThreadFlag.exist ())
				break ;
			if (!r1.mItemQueue->empty ())
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		ITEM ret = std::move (r1.mItemQueue.self[r1.mItemQueue->peek ()]) ;
		r1.mItemQueue->take () ;
		return std::move (ret) ;
	}

	void start (const Array<INDEX> &pid ,Array<Function<DEF<ITEM ()> NONE::*>> &&proc) {
		_DEBUG_ASSERT_ (pid.length () > 0) ;
		for (auto &&i : pid)
			_DEBUG_ASSERT_ (i >= 0 && i < proc.size ()) ;
		for (auto &&i : proc)
			_DEBUG_ASSERT_ (i.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r1.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1.mThreadCounter == 0) ;
		r1.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1.mThreadCounter = 0 ;
		r1.mThreadProc = std::move (proc) ;
		if (!r1.mItemQueue.exist ())
			r1.mItemQueue = AutoRef<QList<ITEM ,SFIXED>>::make (pid.length ()) ;
		r1.mItemQueue->clear () ;
		r1.mException = AutoRef<Exception> () ;
		r1.mThreadPool = Array<AutoRef<std::thread>> (pid.size ()) ;
		for (INDEX i = 0 ; i < r1.mThreadPool.length () ; i++) {
			const auto r2x = PACK<PTR<Holder> ,INDEX> {&r1 ,pid[i]} ;
			//@warn: move object having captured context
			r1.mThreadPool[i] = AutoRef<std::thread>::make ([r2x] () noexcept {
				_CALL_TRY_ ([&] () {
					compute_execute (*r2x.P1 ,r2x.P2) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
		}
	}

	void join (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mItemQueue->size () > 0) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
			if (r1.mException.exist ())
				break ;
			if (r1.mItemQueue->full ())
				break ;
			const auto r3x = predicate () ;
			if (!r3x)
				break ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		const auto r2x = std::move (r1.mException) ;
		if (!r2x.exist ())
			return ;
		r2x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		intrusive_destroy (r1x) ;
	}

private:
	static void compute_execute (Holder &_self ,INDEX pid) {
		class Finally :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter-- ;
			}
		} ;
		ScopedGuard<Finally> ANONYMOUS (_CAST_<Finally> (_self)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			_CALL_EH_ ([&] () {
				rax.template recreate<ITEM> (_self.mThreadProc[pid] ()) ;
			} ,[&] (const Exception &e) noexcept {
				_CALL_TRY_ ([&] () {
					compute_rethrow (_self ,e) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
			compute_push (_self ,std::move (rax)) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	static void compute_push (Holder &_self ,Optional<ITEM> &&item) {
		ScopedGuard<std::mutex> sgd (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.self) ;
		if (!item.exist ())
			return ;
		_DYNAMIC_ASSERT_ (_self.mItemQueue->size () > 0) ;
		for (FOR_ONCE_DO_WHILE) {
			if (!_self.mItemQueue->full ())
				continue ;
			_self.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
			if (!_self.mItemQueue->full ())
				continue ;
			_self.mItemQueue->take () ;
		}
		_self.mItemQueue->add (std::move (item.self)) ;
		_self.mThreadCondition.self.notify_all () ;
	}

	static void compute_rethrow (Holder &_self ,const Exception &e) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		if (_self.mException.exist ())
			return ;
		_self.mException = AutoRef<Exception>::make (e) ;
	}

	static void intrusive_create (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_self.mCounter.self = 0 ;
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadPool = Array<AutoRef<std::thread>> () ;
		_self.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
	}

	static void intrusive_destroy (Holder &_self) {
		std::unique_lock<std::mutex> sgd (_self.mThreadMutex) ;
		if (!_self.mThreadFlag.exist ())
			return ;
		_self.mThreadFlag.self = FALSE ;
		_self.mThreadCondition.self.notify_all () ;
		while (_self.mThreadFlag.exist () && _self.mThreadCounter > 0)
			_self.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.exist ()) ;
		for (auto &&i : _self.mThreadPool) {
			if (!i.exist ())
				continue ;
			i->join () ;
		}
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadPool = Array<AutoRef<std::thread>> () ;
		_self.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
	}

	static LENGTH intrusive_attach (Holder &_self) popping {
		return ++_self.mCounter.self ;
	}

	static LENGTH intrusive_detach (Holder &_self) popping {
		return --_self.mCounter.self ;
	}

	static void intrusive_latch (Holder &_self) {
		GlobalRuntime::thread_sleep () ;
	}
} ;

template <class ITEM>
class WorkThread {
private:
	class Counter :private Wrapped<LENGTH> {
	public:
		inline void lock () {
			Counter::mSelf++ ;
		}

		inline void unlock () {
			Counter::mSelf-- ;
		}
	} ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = WorkThread ;

	private:
		friend WorkThread ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mThreadMutex ;
		Monostate<std::condition_variable> mThreadCondition ;
		LENGTH mThreadCounter ;
		LENGTH mThreadWaitCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<DEF<void (const ITEM &)> NONE::*> mThreadProc ;
		Array<AutoRef<std::thread>> mThreadPool ;
		AutoRef<QList<ITEM ,SFIXED>> mItemQueue ;
		AutoRef<Exception> mException ;
	} ;

private:
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	WorkThread () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mItemQueue.exist ())
			return 0 ;
		return r1.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mItemQueue.exist ())
			return 0 ;
		return r1.mItemQueue->length () ;
	}

	void reserve (LENGTH post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (r1.mItemQueue.exist () && r1.mItemQueue->length () + post_len <= r1.mItemQueue->size ())
			return ;
		auto rax = AutoRef<QList<ITEM ,SFIXED>>::make (post_len) ;
		rax->appand (std::move (r1.mItemQueue.self)) ;
		r1.mItemQueue = std::move (rax) ;
	}

	void post (const ITEM &item) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mItemQueue->size () > 0) ;
		while (r1.mThreadFlag.exist () && r1.mItemQueue->full ())
			r1.mThreadCondition.self.wait (sgd) ;
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		r1.mItemQueue->add (std::move (item)) ;
		r1.mThreadCondition.self.notify_all () ;
	}

	void post (ITEM &&item) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mItemQueue->size () > 0) ;
		while (r1.mThreadFlag.exist () && r1.mItemQueue->full ())
			r1.mThreadCondition.self.wait (sgd) ;
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		r1.mItemQueue->add (std::move (item)) ;
		r1.mThreadCondition.self.notify_all () ;
	}

	void post (const ITEM &item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1.mThreadFlag.exist ())
				break ;
			if (!r1.mItemQueue->full ())
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		r1.mItemQueue->add (std::move (item)) ;
		r1.mThreadCondition.self.notify_all () ;
	}

	void post (ITEM &&item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r1.mThreadFlag.exist ())
				break ;
			if (!r1.mItemQueue->full ())
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		r1.mItemQueue->add (std::move (item)) ;
		r1.mThreadCondition.self.notify_all () ;
	}

	void start (LENGTH count ,Function<DEF<void (const ITEM &)> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r1.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1.mThreadCounter == 0) ;
		r1.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1.mThreadCounter = 0 ;
		r1.mThreadWaitCounter = 0 ;
		r1.mThreadProc = std::move (proc) ;
		if (!r1.mItemQueue.exist ())
			r1.mItemQueue = AutoRef<QList<ITEM ,SFIXED>>::make (count) ;
		r1.mItemQueue->clear () ;
		r1.mException = AutoRef<Exception> () ;
		r1.mThreadPool = Array<AutoRef<std::thread>> (count) ;
		for (INDEX i = 0 ; i < r1.mThreadPool.length () ; i++) {
			const auto r2x = &r1 ;
			//@warn: move object having captured context
			r1.mThreadPool[i] = AutoRef<std::thread>::make ([r2x] () noexcept {
				_CALL_TRY_ ([&] () {
					compute_execute (*r2x) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
		}
	}

	void join (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
			if (r1.mException.exist ())
				break ;
			if (r1.mThreadWaitCounter >= r1.mThreadPool.length () && r1.mItemQueue->empty ())
				break ;
			const auto r3x = predicate () ;
			if (!r3x)
				break ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		const auto r2x = std::move (r1.mException) ;
		if (!r2x.exist ())
			return ;
		r2x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		intrusive_destroy (r1x) ;
	}

private:
	static void compute_execute (Holder &_self) {
		class Finally :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter-- ;
			}
		} ;
		ScopedGuard<Finally> ANONYMOUS (_CAST_<Finally> (_self)) ;
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
			compute_poll (_self ,rax) ;
			_CALL_EH_ ([&] () {
				_self.mThreadProc (rax.self) ;
			} ,[&] (const Exception &e) noexcept {
				_CALL_TRY_ ([&] () {
					compute_rethrow (_self ,e) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}

	static void compute_poll (Holder &_self ,Optional<ITEM> &item) popping {
		std::unique_lock<std::mutex> sgd (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (_self.mThreadWaitCounter)) ;
		while (_self.mThreadFlag.self && _self.mItemQueue->empty ())
			_self.mThreadCondition.self.wait (sgd) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.self) ;
		item = std::move (_self.mItemQueue.self[_self.mItemQueue->peek ()]) ;
		_self.mItemQueue->take () ;
	}

	static void compute_rethrow (Holder &_self ,const Exception &e) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		if (_self.mException.exist ())
			return ;
		_self.mException = AutoRef<Exception>::make (e) ;
	}

	static void intrusive_create (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_self.mCounter.self = 0 ;
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadWaitCounter = 0 ;
		_self.mThreadPool = Array<AutoRef<std::thread>> () ;
		_self.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
	}

	static void intrusive_destroy (Holder &_self) {
		std::unique_lock<std::mutex> sgd (_self.mThreadMutex) ;
		if (!_self.mThreadFlag.exist ())
			return ;
		_self.mThreadFlag.self = FALSE ;
		_self.mThreadCondition.self.notify_all () ;
		while (_self.mThreadFlag.exist () && _self.mThreadCounter > 0)
			_self.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.exist ()) ;
		for (auto &&i : _self.mThreadPool) {
			if (!i.exist ())
				continue ;
			i->join () ;
		}
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadWaitCounter = 0 ;
		_self.mThreadPool = Array<AutoRef<std::thread>> () ;
		_self.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
	}

	static LENGTH intrusive_attach (Holder &_self) popping {
		return ++_self.mCounter.self ;
	}

	static LENGTH intrusive_detach (Holder &_self) popping {
		return --_self.mCounter.self ;
	}

	static void intrusive_latch (Holder &_self) {
		GlobalRuntime::thread_sleep () ;
	}
} ;

template <class ITEM>
class Promise {
public:
	class Future ;

private:
	class Holder {
	public:
		using INTRUSIVE_TYPE = Promise ;

	private:
		friend Promise ;
		friend Future ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mThreadMutex ;
		Monostate<std::condition_variable> mThreadCondition ;
		LENGTH mThreadCounter ;
		AutoRef<BOOL> mThreadFlag ;
		Function<DEF<ITEM ()> NONE::*> mThreadProc ;
		Function<DEF<void (ITEM &)> NONE::*> mCallbackProc ;
		AutoRef<std::thread> mThreadPool ;
		AutoRef<ITEM> mItem ;
		AutoRef<Exception> mException ;
	} ;

private:
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	Promise () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	Future future () popping ;

	void push (const ITEM &item) {
		const auto r1x = mThis.watch () ;
		compute_push (r1x) ;
	}

	void push (ITEM &&item) {
		const auto r1x = mThis.watch () ;
		compute_push (r1x) ;
	}

	void rethrow (const Exception &e) {
		const auto r1x = mThis.watch () ;
		compute_rethrow (r1x) ;
	}

	void start () {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1.mThreadCounter == 0) ;
		r1.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1.mThreadCounter = 0 ;
		r1.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		r1.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r1.mItem = AutoRef<ITEM> () ;
		r1.mException = AutoRef<Exception> () ;
		r1.mThreadPool = AutoRef<std::thread> () ;
	}

	void start (Function<DEF<ITEM ()> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r1.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r1.mThreadCounter == 0) ;
		r1.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r1.mThreadCounter = 0 ;
		r1.mThreadProc = std::move (proc) ;
		r1.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r1.mItem = AutoRef<ITEM> () ;
		r1.mException = AutoRef<Exception> () ;
		const auto r2x = &r1 ;
		//@warn: move object having captured context
		r1.mThreadPool = AutoRef<std::thread>::make ([r2x] () noexcept {
			_CALL_TRY_ ([&] () {
				compute_execute (*r2x) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}) ;
	}

	void signal () {
		const auto r1x = mThis.watch () ;
		compute_signal (r1x) ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		intrusive_destroy (r1x) ;
	}

private:
	explicit Promise (IntrusiveRef<Holder> &_this) popping : mThis (_this.copy ()) {}

private:
	static void compute_execute (Holder &_self) {
		class Finally :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (Finally::mSelf.mThreadMutex) ;
				Finally::mSelf.mThreadCounter-- ;
			}
		} ;
		ScopedGuard<Finally> ANONYMOUS (_CAST_<Finally> (_self)) ;
		_CALL_EH_ ([&] () {
			compute_push (_self ,_self.mThreadProc ()) ;
		} ,[&] (const Exception &e) noexcept {
			_CALL_TRY_ ([&] () {
				compute_rethrow (_self ,e) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}) ;
		compute_signal (_self) ;
	}

	static void compute_push (Holder &_self ,const ITEM &item) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!_self.mException.exist ()) ;
		_self.mItem = AutoRef<ITEM>::make (std::move (item)) ;
	}

	static void compute_push (Holder &_self ,ITEM &&item) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.self) ;
		_DEBUG_ASSERT_ (!_self.mException.exist ()) ;
		_self.mItem = AutoRef<ITEM>::make (std::move (item)) ;
	}

	static void compute_rethrow (Holder &_self ,const Exception &e) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!_self.mException.exist ()) ;
		_self.mItem = AutoRef<ITEM> () ;
		_self.mException = AutoRef<Exception>::make (e) ;
	}

	static void compute_signal (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_DEBUG_ASSERT_ (_self.mThreadFlag.exist ()) ;
		_self.mThreadFlag.self = FALSE ;
		_self.mThreadCondition.self.notify_all () ;
		if (_self.mItem.exist () && _self.mCallbackProc.exist ())
			_self.mCallbackProc (_self.mItem) ;
		_self.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static void intrusive_create (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mThreadMutex) ;
		_self.mCounter.self = 0 ;
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadPool = AutoRef<std::thread> () ;
		_self.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		_self.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static void intrusive_destroy (Holder &_self) {
		std::unique_lock<std::mutex> sgd (_self.mThreadMutex) ;
		if (!_self.mThreadFlag.exist ())
			return ;
		_self.mThreadFlag.self = FALSE ;
		_self.mThreadCondition.self.notify_all () ;
		while (_self.mThreadFlag.exist () && _self.mThreadCounter > 0)
			_self.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
		_DYNAMIC_ASSERT_ (_self.mThreadFlag.exist ()) ;
		if (_self.mThreadPool.exist ())
			_self.mThreadPool->join () ;
		_self.mThreadFlag = AutoRef<BOOL> () ;
		_self.mThreadCounter = 0 ;
		_self.mThreadPool = AutoRef<std::thread> () ;
		_self.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		_self.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	static LENGTH intrusive_attach (Holder &_self) popping {
		return ++_self.mCounter.self ;
	}

	static LENGTH intrusive_detach (Holder &_self) popping {
		return --_self.mCounter.self ;
	}

	static void intrusive_latch (Holder &_self) {
		GlobalRuntime::thread_sleep () ;
	}

public:
	static Future async (Function<DEF<ITEM ()> NONE::*> &&proc) ;
} ;

template <class ITEM>
class Promise<ITEM>::Future {
private:
	using Holder = typename Promise<ITEM>::Holder ;

private:
	friend Promise<ITEM> ;
	IntrusiveRef<Holder> mThis ;

public:
	Future () = delete ;

	BOOL ready () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mThreadFlag.exist ())
			return TRUE ;
		if (r1.mThreadFlag.self)
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		while (r1.mThreadFlag.exist () && r1.mThreadFlag.self)
			r1.mThreadCondition.self.wait (sgd) ;
		for (FOR_ONCE_DO_WHILE) {
			if (!r1.mException.exist ())
				continue ;
			r1.mException->raise () ;
		}
		_DYNAMIC_ASSERT_ (r1.mItem.exist ()) ;
		ITEM ret = std::move (r1.mItem.self) ;
		r1.mItem = AutoRef<ITEM> () ;
		return std::move (ret) ;
	}

	ITEM poll (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r1.mThreadMutex) ;
		while (TRUE) {
			if (!r1.mThreadFlag.exist ())
				break ;
			if (!r1.mThreadFlag.self)
				break ;
			const auto r2x = predicate () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			r1.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (!r1.mException.exist ())
				continue ;
			r1.mException->raise () ;
		}
		_DYNAMIC_ASSERT_ (r1.mItem.exist ()) ;
		ITEM ret = std::move (r1.mItem.self) ;
		r1.mItem = AutoRef<ITEM> () ;
		return std::move (ret) ;
	}

	ITEM value (const ITEM &def) popping {
		_STATIC_ASSERT_ (std::is_copy_constructible<ITEM>::value && std::is_nothrow_move_constructible<ITEM>::value) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		if (!r1.mThreadFlag.exist ())
			return def ;
		if (r1.mThreadFlag.self)
			return def ;
		if (!r1.mItem.exist ())
			return def ;
		return r1.mItem.self ;
	}

	void then (Function<DEF<void (ITEM &)> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r1 = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r1.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!r1.mCallbackProc.exist ()) ;
		r1.mCallbackProc = std::move (proc) ;
		if (r1.mThreadFlag.self)
			return ;
		if (!r1.mItem.exist ())
			return ;
		r1.mCallbackProc (r1.mItem.self) ;
		r1.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		intrusive_destroy (r1x) ;
	}

private:
	explicit Future (IntrusiveRef<Holder> &_this) popping : mThis (_this.copy ()) {}
} ;

template <class ITEM>
inline typename Promise<ITEM>::Future Promise<ITEM>::future () popping {
	return Future (mThis) ;
}

template <class ITEM>
inline typename Promise<ITEM>::Future Promise<ITEM>::async (Function<DEF<ITEM ()> NONE::*> &&proc) {
	auto rax = Promise<ITEM> () ;
	rax.start (std::move (proc)) ;
	return rax.future () ;
}
} ;