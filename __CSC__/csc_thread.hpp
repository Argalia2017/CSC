#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class ITEM>
class CalcThread {
private:
	struct Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename CalcThread::Detail ;

	private:
		friend CalcThread ;
		friend IntrusiveRef<Holder> ;
		std::atomic<LENGTH> mCounter ;
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
	struct Detail ;
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	CalcThread () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mItemQueue.exist ())
			return 0 ;
		return r2y.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mItemQueue.exist ())
			return 0 ;
		return r2y.mItemQueue->length () ;
	}

	void reserve (LENGTH post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (r2y.mItemQueue.exist ())
			if (r2y.mItemQueue->length () + post_len <= r2y.mItemQueue->size ())
				return ;
		auto tmp = AutoRef<QList<ITEM ,SFIXED>>::make (post_len) ;
		tmp->appand (std::move (r2y.mItemQueue.self)) ;
		r2y.mItemQueue = std::move (tmp) ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->empty ())
				break ;
			r2y.mThreadCondition.self.wait (sgd) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		ITEM ret = std::move (r2y.mItemQueue.self[r2y.mItemQueue->head ()]) ;
		r2y.mItemQueue->take () ;
		return std::move (ret) ;
	}

	template <class _ARG1 ,class _ARG2>
	ITEM poll (const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->empty ())
				break ;
			const auto r3x = predicate () ;
			_DYNAMIC_ASSERT_ (r3x) ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		ITEM ret = std::move (r2y.mItemQueue.self[r2y.mItemQueue->head ()]) ;
		r2y.mItemQueue->take () ;
		return std::move (ret) ;
	}

	void start (Array<Function<DEF<ITEM ()> NONE::*>> &&proc) {
		for (auto &&i : proc) {
			_DEBUG_ASSERT_ (i.exist ()) ;
			(void) i ;
		}
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r2y.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2y.mThreadCounter == 0) ;
		r2y.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2y.mThreadCounter = 0 ;
		r2y.mThreadProc = std::move (proc) ;
		if (!r2y.mItemQueue.exist ())
			r2y.mItemQueue = AutoRef<QList<ITEM ,SFIXED>>::make (proc.length ()) ;
		r2y.mItemQueue->clear () ;
		r2y.mException = AutoRef<Exception> () ;
		r2y.mThreadPool = Array<AutoRef<std::thread>> (proc.size ()) ;
		for (auto &&i : _RANGE_ (0 ,r2y.mThreadPool.length ())) {
			const auto r3x = PACK<PTR<Holder> ,INDEX> {&r2y ,i} ;
			//@warn: move object having captured context
			r2y.mThreadPool[i] = AutoRef<std::thread>::make ([r3x] () noexcept {
				_CALL_TRY_ ([&] () {
					Detail::static_execute ((*r3x.P1) ,r3x.P2) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	void join (const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mItemQueue->size () > 0) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
			if (r2y.mException.exist ())
				break ;
			if (r2y.mItemQueue->full ())
				break ;
			const auto r3x = predicate () ;
			if (!r3x)
				break ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		const auto r4x = std::move (r2y.mException) ;
		if (!r4x.exist ())
			return ;
		r4x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		Detail::friend_destroy (r1x) ;
	}

private:
	struct Detail {
		class ThreadCounter :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter-- ;
			}
		} ;

		inline static void static_execute (Holder &self_ ,INDEX tid) {
			ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
			auto rax = Optional<ITEM>::nullopt () ;
			while (TRUE) {
				_CATCH_ ([&] () {
					//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
					rax = self_.mThreadProc[tid] () ;
				} ,[&] (const Exception &e) noexcept {
					_CALL_TRY_ ([&] () {
						static_rethrow (self_ ,e) ;
					} ,[&] () {
						_STATIC_WARNING_ ("noop") ;
					}) ;
				}) ;
				if (rax.exist ())
					static_push (self_ ,std::move (rax.self)) ;
				rax = Optional<ITEM>::nullopt () ;
			}
		}

		inline static void static_push (Holder &self_ ,ITEM &&item) {
			ScopedGuard<std::mutex> sgd (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
			_DYNAMIC_ASSERT_ (self_.mItemQueue->size () > 0) ;
			if SWITCH_CASE (TRUE) {
				if (!self_.mItemQueue->full ())
					discard ;
				self_.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
				if (!self_.mItemQueue->full ())
					discard ;
				self_.mItemQueue->take () ;
			}
			self_.mItemQueue->add (std::move (item)) ;
			self_.mThreadCondition.self.notify_all () ;
		}

		inline static void static_rethrow (Holder &self_ ,const Exception &e) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			if (self_.mException.exist ())
				return ;
			self_.mException = AutoRef<Exception>::make (e) ;
		}

		inline static void friend_create (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			self_.mCounter = 0 ;
			self_.mThreadFlag = AutoRef<BOOL> () ;
			self_.mThreadCounter = 0 ;
			self_.mThreadPool = Array<AutoRef<std::thread>> () ;
			self_.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
		}

		inline static void friend_destroy (Holder &self_) {
			std::unique_lock<std::mutex> sgd (self_.mThreadMutex) ;
			if (!self_.mThreadFlag.exist ())
				return ;
			self_.mThreadFlag.self = FALSE ;
			self_.mThreadCondition.self.notify_all () ;
			while (TRUE) {
				if (!self_.mThreadFlag.exist ())
					break ;
				if (self_.mThreadCounter == 0)
					break ;
				self_.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
			}
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
			for (auto &&i : self_.mThreadPool) {
				if (!i.exist ())
					continue ;
				i->join () ;
			}
			self_.mThreadFlag = AutoRef<BOOL> () ;
			self_.mThreadCounter = 0 ;
			self_.mThreadPool = Array<AutoRef<std::thread>> () ;
			self_.mThreadProc = Array<Function<DEF<ITEM ()> NONE::*>> () ;
		}

		inline static LENGTH friend_attach (Holder &self_) popping {
			return ++self_.mCounter ;
		}

		inline static LENGTH friend_detach (Holder &self_) popping {
			return --self_.mCounter ;
		}

		inline static void friend_latch (Holder &self_) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

template <class ITEM>
class WorkThread {
private:
	struct Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename WorkThread::Detail ;

	private:
		friend WorkThread ;
		friend IntrusiveRef<Holder> ;
		std::atomic<LENGTH> mCounter ;
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
	struct Detail ;
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	WorkThread () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	LENGTH size () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mItemQueue.exist ())
			return 0 ;
		return r2y.mItemQueue->size () ;
	}

	LENGTH length () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mItemQueue.exist ())
			return 0 ;
		return r2y.mItemQueue->length () ;
	}

	void reserve (LENGTH post_len) {
		_DEBUG_ASSERT_ (post_len >= 0) ;
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (r2y.mItemQueue.exist ())
			if (r2y.mItemQueue->length () + post_len <= r2y.mItemQueue->size ())
				return ;
		auto tmp = AutoRef<QList<ITEM ,SFIXED>>::make (post_len) ;
		tmp->appand (std::move (r2y.mItemQueue.self)) ;
		r2y.mItemQueue = std::move (tmp) ;
	}

	void post (const ITEM &item) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->full ())
				break ;
			r2y.mThreadCondition.self.wait (sgd) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		r2y.mItemQueue->add (std::move (item)) ;
		r2y.mThreadCondition.self.notify_all () ;
	}

	void post (ITEM &&item) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->full ())
				break ;
			r2y.mThreadCondition.self.wait (sgd) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		r2y.mItemQueue->add (std::move (item)) ;
		r2y.mThreadCondition.self.notify_all () ;
	}

	template <class _ARG1 ,class _ARG2>
	void post (const ITEM &item ,const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->full ())
				break ;
			const auto r3x = predicate () ;
			_DYNAMIC_ASSERT_ (r3x) ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		r2y.mItemQueue->add (std::move (item)) ;
		r2y.mThreadCondition.self.notify_all () ;
	}

	template <class _ARG1 ,class _ARG2>
	void post (ITEM &&item ,const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mItemQueue->size () > 0) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mItemQueue->full ())
				break ;
			const auto r3x = predicate () ;
			_DYNAMIC_ASSERT_ (r3x) ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		r2y.mItemQueue->add (std::move (item)) ;
		r2y.mThreadCondition.self.notify_all () ;
	}

	void start (LENGTH count ,Function<DEF<void (const ITEM &)> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (count > 0) ;
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DEBUG_ASSERT_ (!r2y.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2y.mThreadCounter == 0) ;
		r2y.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2y.mThreadCounter = 0 ;
		r2y.mThreadWaitCounter = 0 ;
		r2y.mThreadProc = std::move (proc) ;
		if (!r2y.mItemQueue.exist ())
			r2y.mItemQueue = AutoRef<QList<ITEM ,SFIXED>>::make (count) ;
		r2y.mItemQueue->clear () ;
		r2y.mException = AutoRef<Exception> () ;
		r2y.mThreadPool = Array<AutoRef<std::thread>> (count) ;
		for (auto &&i : _RANGE_ (0 ,r2y.mThreadPool.length ())) {
			const auto r3x = PACK<PTR<Holder>> {&r2y} ;
			//@warn: move object having captured context
			r2y.mThreadPool[i] = AutoRef<std::thread>::make ([r3x] () noexcept {
				_CALL_TRY_ ([&] () {
					Detail::static_execute ((*r3x.P1)) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	void join (const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		while (TRUE) {
			_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
			if (r2y.mException.exist ())
				break ;
			if (r2y.mThreadWaitCounter >= r2y.mThreadPool.length ())
				if (r2y.mItemQueue->empty ())
					break ;
			const auto r3x = predicate () ;
			if (!r3x)
				break ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		const auto r4x = std::move (r2y.mException) ;
		if (!r4x.exist ())
			return ;
		r4x->raise () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		Detail::friend_destroy (r1x) ;
	}

private:
	struct Detail {
		class ThreadCounter :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter-- ;
			}
		} ;

		inline static void static_execute (Holder &self_) {
			ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
			auto rax = Optional<ITEM>::nullopt () ;
			while (TRUE) {
				static_poll (self_ ,rax) ;
				_CATCH_ ([&] () {
					//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
					self_.mThreadProc (rax.self) ;
				} ,[&] (const Exception &e) noexcept {
					_CALL_TRY_ ([&] () {
						static_rethrow (self_ ,e) ;
					} ,[&] () {
						_STATIC_WARNING_ ("noop") ;
					}) ;
				}) ;
				rax = Optional<ITEM>::nullopt () ;
			}
		}

		class Counter :private Wrapped<LENGTH> {
		public:
			inline void lock () {
				Counter::mSelf++ ;
			}

			inline void unlock () {
				Counter::mSelf-- ;
			}
		} ;

		inline static void static_poll (Holder &self_ ,Optional<ITEM> &item) {
			std::unique_lock<std::mutex> sgd (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (self_.mThreadWaitCounter)) ;
			while (TRUE) {
				if (!self_.mThreadFlag.self)
					break ;
				if (!self_.mItemQueue->empty ())
					break ;
				self_.mThreadCondition.self.wait (sgd) ;
			}
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
			item = std::move (self_.mItemQueue.self[self_.mItemQueue->head ()]) ;
			self_.mItemQueue->take () ;
		}

		inline static void static_rethrow (Holder &self_ ,const Exception &e) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			if (self_.mException.exist ())
				return ;
			self_.mException = AutoRef<Exception>::make (e) ;
		}

		inline static void friend_create (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			self_.mCounter = 0 ;
			self_.mThreadFlag = AutoRef<BOOL> () ;
			self_.mThreadCounter = 0 ;
			self_.mThreadWaitCounter = 0 ;
			self_.mThreadPool = Array<AutoRef<std::thread>> () ;
			self_.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
		}

		inline static void friend_destroy (Holder &self_) {
			std::unique_lock<std::mutex> sgd (self_.mThreadMutex) ;
			if (!self_.mThreadFlag.exist ())
				return ;
			self_.mThreadFlag.self = FALSE ;
			self_.mThreadCondition.self.notify_all () ;
			while (TRUE) {
				if (!self_.mThreadFlag.exist ())
					break ;
				if (self_.mThreadCounter == 0)
					break ;
				self_.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
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
			self_.mThreadPool = Array<AutoRef<std::thread>> () ;
			self_.mThreadProc = Function<DEF<void (const ITEM &)> NONE::*> () ;
		}

		inline static LENGTH friend_attach (Holder &self_) popping {
			return ++self_.mCounter ;
		}

		inline static LENGTH friend_detach (Holder &self_) popping {
			return --self_.mCounter ;
		}

		inline static void friend_latch (Holder &self_) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

template <class ITEM>
class Promise {
public:
	class Future ;

private:
	struct Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename Promise::Detail ;

	private:
		friend Promise ;
		friend Future ;
		friend IntrusiveRef<Holder> ;
		std::atomic<LENGTH> mCounter ;
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
	struct Detail ;
	friend IntrusiveRef<Holder> ;
	IntrusiveRef<Holder> mThis ;

public:
	Promise () {
		mThis = IntrusiveRef<Holder>::make () ;
	}

	Future future () popping ;

	void push (const ITEM &item) {
		const auto r1x = mThis.watch () ;
		Detail::static_push (r1x) ;
	}

	void push (ITEM &&item) {
		const auto r1x = mThis.watch () ;
		Detail::static_push (r1x) ;
	}

	void rethrow (const Exception &e) {
		const auto r1x = mThis.watch () ;
		Detail::static_rethrow (r1x) ;
	}

	void start () {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2y.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2y.mThreadCounter == 0) ;
		r2y.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2y.mThreadCounter = 0 ;
		r2y.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
		r2y.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r2y.mItem = AutoRef<ITEM> () ;
		r2y.mException = AutoRef<Exception> () ;
		r2y.mThreadPool = AutoRef<std::thread> () ;
	}

	void start (Function<DEF<ITEM ()> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (!r2y.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (r2y.mThreadCounter == 0) ;
		r2y.mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		r2y.mThreadCounter = 0 ;
		r2y.mThreadProc = std::move (proc) ;
		r2y.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		r2y.mItem = AutoRef<ITEM> () ;
		r2y.mException = AutoRef<Exception> () ;
		const auto r3x = PACK<PTR<Holder>> {&r2y} ;
		//@warn: move object having captured context
		r2y.mThreadPool = AutoRef<std::thread>::make ([r3x] () noexcept {
			_CALL_TRY_ ([&] () {
				Detail::static_execute ((*r3x.P1)) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}) ;
	}

	void signal () {
		const auto r1x = mThis.watch () ;
		Detail::static_signal (r1x) ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		Detail::friend_destroy (r1x) ;
	}

private:
	explicit Promise (IntrusiveRef<Holder> &this_) popping : mThis (this_.copy ()) {}

public:
	static Future async (Function<DEF<ITEM ()> NONE::*> &&proc) ;

private:
	struct Detail {
		class ThreadCounter :private Wrapped<Holder> {
		public:
			inline void lock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter++ ;
			}

			inline void unlock () {
				ScopedGuard<std::mutex> ANONYMOUS (ThreadCounter::mSelf.mThreadMutex) ;
				ThreadCounter::mSelf.mThreadCounter-- ;
			}
		} ;

		inline static void static_execute (Holder &self_) {
			ScopedGuard<ThreadCounter> ANONYMOUS (_CAST_<ThreadCounter> (self_)) ;
			auto rax = Optional<ITEM>::nullopt () ;
			_CATCH_ ([&] () {
				//@warn: 'mThreadProc' is not protected by 'mThreadMutex'
				rax = self_.mThreadProc () ;
			} ,[&] (const Exception &e) noexcept {
				_CALL_TRY_ ([&] () {
					static_rethrow (self_ ,e) ;
				} ,[&] () {
					_STATIC_WARNING_ ("noop") ;
				}) ;
			}) ;
			if (rax.exist ())
				static_push (self_ ,std::move (rax.self)) ;
			static_signal (self_) ;
		}

		inline static void static_push (Holder &self_ ,const ITEM &item) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
			_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
			self_.mItem = AutoRef<ITEM>::make (std::move (item)) ;
		}

		inline static void static_push (Holder &self_ ,ITEM &&item) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.self) ;
			_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
			self_.mItem = AutoRef<ITEM>::make (std::move (item)) ;
		}

		inline static void static_rethrow (Holder &self_ ,const Exception &e) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			_DEBUG_ASSERT_ (!self_.mException.exist ()) ;
			self_.mItem = AutoRef<ITEM> () ;
			self_.mException = AutoRef<Exception>::make (e) ;
		}

		inline static void static_signal (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			_DEBUG_ASSERT_ (self_.mThreadFlag.exist ()) ;
			self_.mThreadFlag.self = FALSE ;
			self_.mThreadCondition.self.notify_all () ;
			if SWITCH_CASE (TRUE) {
				if (!self_.mItem.exist ())
					discard ;
				if (!self_.mCallbackProc.exist ())
					discard ;
				self_.mCallbackProc (self_.mItem) ;
			}
			self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		}

		inline static void friend_create (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mThreadMutex) ;
			self_.mCounter = 0 ;
			self_.mThreadFlag = AutoRef<BOOL> () ;
			self_.mThreadCounter = 0 ;
			self_.mThreadPool = AutoRef<std::thread> () ;
			self_.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
			self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		}

		inline static void friend_destroy (Holder &self_) {
			std::unique_lock<std::mutex> sgd (self_.mThreadMutex) ;
			if (!self_.mThreadFlag.exist ())
				return ;
			self_.mThreadFlag.self = FALSE ;
			self_.mThreadCondition.self.notify_all () ;
			while (TRUE) {
				if (!self_.mThreadFlag.exist ())
					break ;
				if (self_.mThreadCounter == 0)
					break ;
				self_.mThreadCondition.self.wait_for (sgd ,std::chrono::milliseconds (0)) ;
			}
			_DYNAMIC_ASSERT_ (self_.mThreadFlag.exist ()) ;
			if (self_.mThreadPool.exist ())
				self_.mThreadPool->join () ;
			self_.mThreadFlag = AutoRef<BOOL> () ;
			self_.mThreadCounter = 0 ;
			self_.mThreadPool = AutoRef<std::thread> () ;
			self_.mThreadProc = Function<DEF<ITEM ()> NONE::*> () ;
			self_.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
		}

		inline static LENGTH friend_attach (Holder &self_) popping {
			return ++self_.mCounter ;
		}

		inline static LENGTH friend_detach (Holder &self_) popping {
			return --self_.mCounter ;
		}

		inline static void friend_latch (Holder &self_) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

template <class ITEM>
class Promise<ITEM>::Future {
private:
	using Holder = typename Promise<ITEM>::Holder ;
	using Detail = typename Promise<ITEM>::Detail ;

private:
	friend Promise<ITEM> ;
	IntrusiveRef<Holder> mThis ;

public:
	Future () = delete ;

	BOOL ready () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mThreadFlag.exist ())
			return TRUE ;
		if (r2y.mThreadFlag.self)
			return FALSE ;
		return TRUE ;
	}

	ITEM poll () popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mThreadFlag.self)
				break ;
			r2y.mThreadCondition.self.wait (sgd) ;
		}
		if (r2y.mException.exist ())
			r2y.mException->raise () ;
		_DYNAMIC_ASSERT_ (r2y.mItem.exist ()) ;
		ITEM ret = std::move (r2y.mItem.self) ;
		r2y.mItem = AutoRef<ITEM> () ;
		return std::move (ret) ;
	}

	template <class _ARG1 ,class _ARG2>
	ITEM poll (const std::chrono::duration<_ARG1 ,_ARG2> &interval ,const Function<BOOL ()> &predicate) popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		std::unique_lock<std::mutex> sgd (r2y.mThreadMutex) ;
		while (TRUE) {
			if (!r2y.mThreadFlag.exist ())
				break ;
			if (!r2y.mThreadFlag.self)
				break ;
			const auto r3x = predicate () ;
			_DYNAMIC_ASSERT_ (r3x) ;
			r2y.mThreadCondition.self.wait_for (sgd ,interval) ;
		}
		if (r2y.mException.exist ())
			r2y.mException->raise () ;
		_DYNAMIC_ASSERT_ (r2y.mItem.exist ()) ;
		ITEM ret = std::move (r2y.mItem.self) ;
		r2y.mItem = AutoRef<ITEM> () ;
		return std::move (ret) ;
	}

	ITEM value (const ITEM &def) popping {
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		if (!r2y.mThreadFlag.exist ())
			return def ;
		if (r2y.mThreadFlag.self)
			return def ;
		if (!r2y.mItem.exist ())
			return def ;
		return r2y.mItem.self ;
	}

	void then (Function<DEF<void (ITEM &)> NONE::*> &&proc) {
		_DEBUG_ASSERT_ (proc.exist ()) ;
		const auto r1x = mThis.watch () ;
		auto &r2y = _XVALUE_<Holder> (r1x) ;
		ScopedGuard<std::mutex> ANONYMOUS (r2y.mThreadMutex) ;
		_DYNAMIC_ASSERT_ (r2y.mThreadFlag.exist ()) ;
		_DEBUG_ASSERT_ (!r2y.mCallbackProc.exist ()) ;
		r2y.mCallbackProc = std::move (proc) ;
		if (r2y.mThreadFlag.self)
			return ;
		if (!r2y.mItem.exist ())
			return ;
		r2y.mCallbackProc (r2y.mItem.self) ;
		r2y.mCallbackProc = Function<DEF<void (ITEM &)> NONE::*> () ;
	}

	void stop () {
		const auto r1x = mThis.watch () ;
		Detail::friend_destroy (r1x) ;
	}

private:
	explicit Future (IntrusiveRef<Holder> &this_) popping : mThis (this_.copy ()) {}
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