#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"

namespace CSC {
template <class ITEM>
class Broadcaster {
private:
	class Counter :private Wrapped<LENGTH> {
	public:
		void lock () {
			Counter::mData++ ;
			_DYNAMIC_ASSERT_ (Counter::mData < RECURSIVE_SIZE::value) ;
		}

		void unlock () {
			Counter::mData-- ;
		}
	} ;

private:
	SharedRef<std::mutex> mItemMutex ;
	AutoRef<std::condition_variable> mItemCondition ;
	AutoRef<std::condition_variable> mBackupCondition ;
	LENGTH mWaitCounter ;
	QList<ITEM ,SFIXED> mItemQueue ;
	AutoRef<Set<ITEM ,SFIXED>> mItemWaitingSet ;

public:
	Broadcaster () = delete ;

	explicit Broadcaster (LENGTH len) :mItemQueue (len) {
		mItemMutex = SharedRef<std::mutex>::make () ;
		mBackupCondition = AutoRef<std::condition_variable>::make () ;
		mWaitCounter = 0 ;
	}

	LENGTH length () const {
		ScopedGuard<std::mutex> ANONYMOUS (mItemMutex) ;
		return mItemQueue.length () ;
	}

	LENGTH length (LENGTH wc) const {
		ScopedGuard<std::mutex> ANONYMOUS (mItemMutex) ;
		return mWaitCounter >= wc ? mItemQueue.length () : VAR_NONE ;
	}

	ITEM get () popping {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.empty ())
			mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		return mItemQueue.poll () ;
	}

	ITEM get (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) popping {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.empty ()) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		return mItemQueue.poll () ;
	}

	void post (const ITEM &item) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.full ())
			mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		mItemQueue.add (std::move (item)) ;
		mItemCondition->notify_all () ;
	}

	void post (const ITEM &item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.full ()) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		mItemQueue.add (std::move (item)) ;
		mItemCondition->notify_all () ;
	}

	void post (ITEM &&item) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.full ())
			mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		mItemQueue.add (std::move (item)) ;
		mItemCondition->notify_all () ;
	}

	void post (ITEM &&item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		while (mItemCondition.exist () && mItemQueue.full ()) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		mItemQueue.add (std::move (item)) ;
		mItemCondition->notify_all () ;
	}

	void wait (const ITEM &item) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		if (!mItemWaitingSet.exist ())
			mItemWaitingSet = AutoRef<Set<ITEM ,SFIXED>>::make (mItemQueue.size ()) ;
		while (mItemCondition.exist () && (mItemWaitingSet->length () >= mItemWaitingSet->size () || mItemQueue.full ()))
			mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		if (mItemWaitingSet->find (item) == VAR_NONE) {
			mItemWaitingSet->add (item) ;
			mItemQueue.add (item) ;
			mItemCondition->notify_all () ;
		}
		while (mItemCondition.exist () && mItemWaitingSet->find (item) != VAR_NONE)
			mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
	}

	void wait (const ITEM &item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mWaitCounter)) ;
		if (!mItemWaitingSet.exist ())
			mItemWaitingSet = AutoRef<Set<ITEM ,SFIXED>>::make (mItemQueue.size ()) ;
		while (mItemCondition.exist () && (mItemWaitingSet->length () >= mItemWaitingSet->size () || mItemQueue.full ())) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		if (mItemWaitingSet->find (item) == VAR_NONE) {
			mItemWaitingSet->add (item) ;
			mItemQueue.add (item) ;
			mItemCondition->notify_all () ;
		}
		while (mItemCondition.exist () && mItemWaitingSet->find (item) != VAR_NONE) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
	}

	void notify (const ITEM &item) {
		ScopedGuard<std::mutex> ANONYMOUS (mItemMutex) ;
		_DYNAMIC_ASSERT_ (mItemCondition.exist ()) ;
		INDEX ix = mItemWaitingSet.exist () ? mItemWaitingSet->find (item) : VAR_NONE ;
		if (ix == VAR_NONE)
			return ;
		mItemWaitingSet->remove (ix) ;
		mItemCondition->notify_all () ;
	}

	BOOL alive () const {
		ScopedGuard<std::mutex> ANONYMOUS (mItemMutex) ;
		return mItemCondition.exist () ;
	}

	void start () {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		if (mItemCondition.exist ())
			return ;
		while (mWaitCounter > 0)
			mBackupCondition->wait_for (sgd ,std::chrono::milliseconds (0)) ;
		mItemCondition = std::move (mBackupCondition) ;
	}

	void stop () {
		std::unique_lock<std::mutex> sgd (mItemMutex) ;
		if (!mItemCondition.exist ())
			return ;
		mItemQueue.clear () ;
		if (mItemWaitingSet.exist ())
			mItemWaitingSet->clear () ;
		mBackupCondition = std::move (mItemCondition) ;
		mBackupCondition->notify_all () ;
		while (mWaitCounter > 0)
			mBackupCondition->wait_for (sgd ,std::chrono::milliseconds (0)) ;
	}
} ;

template <class ITEM>
class CalcThread {
private:
	using THREAD_SIZE = RECURSIVE_SIZE ;

private:
	Broadcaster<ITEM> mBroadcaster ;
	SharedRef<std::mutex> mCalcThreadMutex ;
	Function<ITEM ()> mCalcThreadProc ;
	Array<AutoRef<std::thread>> mCalcThread ;

public:
	CalcThread () = delete ;

	explicit CalcThread (LENGTH len) :mBroadcaster (len) {
		mCalcThreadMutex = SharedRef<std::mutex>::make () ;
	}

	~CalcThread () noexcept {
		_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<CalcThread>::make (this) ,&CalcThread::stop) ,std::nothrow) ;
	}

	inline CalcThread (const CalcThread &) = delete ;
	inline CalcThread &operator= (const CalcThread &) = delete ;
	inline CalcThread (CalcThread &&) = delete ;
	inline CalcThread &operator= (CalcThread &&) = delete ;

	LENGTH length () const {
		return mBroadcaster.length () ;
	}

	ITEM get () popping {
		return mBroadcaster.get () ;
	}

	ITEM get (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) popping {
		return mBroadcaster.get (interval ,pred) ;
	}

	BOOL alive () const {
		return mBroadcaster.alive () ;
	}

	void start (LENGTH count ,Function<ITEM ()> &&proc) {
		ScopedGuard<std::mutex> ANONYMOUS (mCalcThreadMutex) ;
		_DEBUG_ASSERT_ (count > 0 && count < THREAD_SIZE::value) ;
		_DEBUG_ASSERT_ (!alive ()) ;
		for (auto &i : mCalcThread)
			if (i.exist ())
				i->join () ;
		mBroadcaster.start () ;
		mCalcThreadProc = std::move (proc) ;
		mCalcThread = Array<AutoRef<std::thread>> (count) ;
		for (auto &i : mCalcThread) {
			//@error:deliver object having captured this
			i = AutoRef<std::thread>::make ([this] () {
				_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<CalcThread>::make (this) ,&CalcThread::execute)) ;
			}) ;
		}
	}

	void join (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		while (TRUE) {
			const auto r1x = mBroadcaster.length (mCalcThread.length ()) ;
			if (r1x > 0)
				break ;
			const auto r2x = pred () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			std::this_thread::sleep_for (interval) ;
		}
	}

	void stop () {
		ScopedGuard<std::mutex> ANONYMOUS (mCalcThreadMutex) ;
		if (mCalcThread.size () == 0)
			return ;
		mBroadcaster.stop () ;
		for (auto &i : mCalcThread) {
			if (!i.exist ())
				continue ;
			i->join () ;
			i = AutoRef<std::thread> () ;
		}
		mCalcThread = Array<AutoRef<std::thread>> () ;
		mCalcThreadProc = Function<ITEM ()> () ;
	}

private:
	void execute () {
		auto rax = Optional<ITEM>::nullopt () ;
		while (TRUE) {
#ifdef __CSC_DEBUG__
			_CALL_TRY_ ([&] () {
				rax.template recreate<ITEM> (mCalcThreadProc ()) ;
			} ,std::nothrow) ;
#else
			_CALL_TRY_ ([&] () {
				rax.template recreate<ITEM> (mCalcThreadProc ()) ;
			}) ;
#endif
			const auto r1x = rax.exist () || mBroadcaster.alive () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			if (rax.exist ())
				mBroadcaster.post (std::move (rax.self)) ;
			rax = Optional<ITEM>::nullopt () ;
		}
	}
} ;

template <class ITEM>
class WorkThread {
private:
	using THREAD_SIZE = RECURSIVE_SIZE ;

private:
	Broadcaster<ITEM> mBroadcaster ;
	SharedRef<std::mutex> mWorkThreadMutex ;
	Function<void (const ITEM &)> mWorkThreadProc ;
	Array<AutoRef<std::thread>> mWorkThread ;

public:
	WorkThread () = delete ;

	explicit WorkThread (LENGTH len) :mBroadcaster (len) {
		mWorkThreadMutex = SharedRef<std::mutex>::make () ;
	}

	~WorkThread () noexcept {
		_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<WorkThread>::make (this) ,&WorkThread::stop) ,std::nothrow) ;
	}

	inline WorkThread (const WorkThread &) = delete ;
	inline WorkThread &operator= (const WorkThread &) = delete ;
	inline WorkThread (WorkThread &&) = delete ;
	inline WorkThread &operator= (WorkThread &&) = delete ;

	LENGTH length () const {
		return mBroadcaster.length () ;
	}

	void post (const ITEM &item) {
		mBroadcaster.post (std::move (item)) ;
	}

	void post (const ITEM &item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		mBroadcaster.post (std::move (item) ,interval ,pred) ;
	}

	void post (ITEM &&item) {
		mBroadcaster.post (std::move (item)) ;
	}

	void post (ITEM &&item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		mBroadcaster.post (std::move (item) ,interval ,pred) ;
	}

	void wait (const ITEM &item) {
		mBroadcaster.wait (item) ;
	}

	void wait (const ITEM &item ,const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		mBroadcaster.wait (item ,interval ,pred) ;
	}

	BOOL alive () const {
		return mBroadcaster.alive () ;
	}

	void start (LENGTH count ,Function<void (const ITEM &)> &&proc) {
		ScopedGuard<std::mutex> ANONYMOUS (mWorkThreadMutex) ;
		_DEBUG_ASSERT_ (count > 0 && count < THREAD_SIZE::value) ;
		_DEBUG_ASSERT_ (!alive ()) ;
		for (auto &i : mWorkThread)
			if (i.exist ())
				i->join () ;
		mBroadcaster.start () ;
		mWorkThreadProc = std::move (proc) ;
		mWorkThread = Array<AutoRef<std::thread>> (count) ;
		for (auto &i : mWorkThread) {
			//@error:deliver object having captured this
			i = AutoRef<std::thread>::make ([this] () {
				_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<WorkThread>::make (this) ,&WorkThread::execute)) ;
			}) ;
		}
	}

	void join (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) {
		while (TRUE) {
			const auto r1x = mBroadcaster.length (mWorkThread.length ()) ;
			if (r1x == 0)
				break ;
			const auto r2x = pred () ;
			_DYNAMIC_ASSERT_ (r2x) ;
			std::this_thread::sleep_for (interval) ;
		}
	}

	void stop () {
		ScopedGuard<std::mutex> ANONYMOUS (mWorkThreadMutex) ;
		if (mWorkThread.size () == 0)
			return ;
		mBroadcaster.stop () ;
		for (auto &i : mWorkThread) {
			if (!i.exist ())
				continue ;
			i->join () ;
			i = AutoRef<std::thread> () ;
		}
		mWorkThread = Array<AutoRef<std::thread>> () ;
		mWorkThreadProc = Function<void (const ITEM &)> () ;
	}

private:
	void execute () {
		while (TRUE) {
			const auto r1x = mBroadcaster.get () ;
#ifdef __CSC_DEBUG__
			_CALL_TRY_ ([&] () {
				mWorkThreadProc (r1x) ;
			} ,std::nothrow) ;
#else
			_CALL_TRY_ ([&] () {
				mWorkThreadProc (r1x) ;
			}) ;
#endif
			mBroadcaster.notify (r1x) ;
		}
	}
} ;

template <class ITEM>
class Promise {
public:
	class Future ;

	class Pack {
	private:
		using INSTRUSIVE_TYPE = Promise ;

	private:
		friend Promise ;
		friend Future ;
		friend IntrusiveRef<Pack> ;
		AutoRef<std::atomic<LENGTH>> mCounter ;
		SharedRef<std::mutex> mItemMutex ;
		AutoRef<std::condition_variable> mItemCondition ;
		AutoRef<BOOL> mThreadFlag ;
		Function<ITEM ()> mThreadProc ;
		AutoRef<std::thread> mThread ;
		AutoRef<ITEM> mItem ;
		AutoRef<std::exception_ptr> mException ;
	} ;

private:
	friend IntrusiveRef<Pack> ;
	IntrusiveRef<Pack> mThis ;

public:
	Promise () {
		mThis = IntrusiveRef<Pack>::make () ;
	}

	Future future () const ;

	void put (const ITEM &item) const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		_DYNAMIC_ASSERT_ (!mThis->mException.exist ()) ;
		_DYNAMIC_ASSERT_ (!mThis->mItem.exist ()) ;
		mThis->mItem = AutoRef<ITEM>::make (std::move (item)) ;
	}

	void put (ITEM &&item) const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		_DYNAMIC_ASSERT_ (!mThis->mException.exist ()) ;
		_DYNAMIC_ASSERT_ (!mThis->mItem.exist ()) ;
		mThis->mItem = AutoRef<ITEM>::make (std::move (item)) ;
	}

	void set_exception () const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		_DYNAMIC_ASSERT_ (!mThis->mException.exist ()) ;
		mThis->mException = AutoRef<std::exception_ptr>::make (std::current_exception ()) ;
	}

	void notify () const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		_DYNAMIC_ASSERT_ (mThis->mThreadFlag.exist ()) ;
		mThis->mThreadFlag.self = FALSE ;
		mThis->mItemCondition->notify_all () ;
	}

	void start () const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		if (mThis->mThread.exist ())
			mThis->mThread->join () ;
		mThis->mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		mThis->mThreadProc = Function<ITEM ()> () ;
		mThis->mThread = AutoRef<std::thread> () ;
	}

	void start (Function<ITEM ()> &&proc) const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		if (mThis->mThread.exist ())
			mThis->mThread->join () ;
		mThis->mThreadFlag = AutoRef<BOOL>::make (TRUE) ;
		mThis->mThreadProc = std::move (proc) ;
		const auto r1x = &mThis.self ;
		//@error:deliver object having captured this
		mThis->mThread = AutoRef<std::thread>::make ([r1x] () {
			const auto r2x = &_CAST_<Promise> (r1x) ;
			_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<const Promise>::make (r2x) ,&Promise::execute)) ;
		}) ;
	}

	void stop () {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		destroy_counter (&mThis.self) ;
	}

private:
	void execute () const {
		try {
			put (mThis->mThreadProc ()) ;
		} catch (const std::exception &) {
			//@info:forward std::exception across threads
			set_exception () ;
		} catch (...) {
			_DEBUG_ASSERT_ (FALSE) ;
			std::terminate () ;
		}
		notify () ;
	}

private:
	inline static void create_counter (Pack &me) {
		me.mCounter = AutoRef<std::atomic<LENGTH>>::make (0) ;
		me.mItemMutex = SharedRef<std::mutex>::make () ;
		me.mItemCondition = AutoRef<std::condition_variable>::make () ;
	}

	inline static void destroy_counter (Pack &me) {
		std::unique_lock<std::mutex> ANONYMOUS (me.mItemMutex) ;
		if (!me.mThread.exist ())
			return ;
		me.mThread->join () ;
		me.mThread = AutoRef<std::thread> () ;
		me.mThreadProc = Function<ITEM ()> () ;
		me.mThreadFlag = AutoRef<BOOL> () ;
	}

	inline static LENGTH increase_counter (Pack &me) popping {
		return ++me.mCounter.self ;
	}

	inline static LENGTH decrease_counter (Pack &me) popping {
		return --me.mCounter.self ;
	}

public:
	static Future async (Function<ITEM ()> &&proc) ;
} ;

template <class ITEM>
class Promise<ITEM>::Future {
private:
	using Pack = typename Promise<ITEM>::Pack ;

private:
	friend Promise<ITEM> ;
	IntrusiveRef<Pack> mThis ;

public:
	Future () = delete ;

	BOOL ready () const {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		_DYNAMIC_ASSERT_ (mThis->mThreadFlag.exist ()) ;
		return !mThis->mThreadFlag.self ;
	}

	ITEM get () const popping {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		std::unique_lock<std::mutex> sgd (mThis->mItemMutex) ;
		while (mThis->mItemCondition.exist () && mThis->mThreadFlag.exist () && mThis->mThreadFlag.self)
			mThis->mItemCondition->wait (sgd) ;
		_DYNAMIC_ASSERT_ (mThis->mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (!mThis->mThreadFlag.self) ;
		if (mThis->mException.exist ())
			std::rethrow_exception (mThis->mException.self) ;
		_DYNAMIC_ASSERT_ (mThis->mItem.exist ()) ;
		ITEM ret = std::move (mThis->mItem.self) ;
		mThis->mThreadFlag = AutoRef<BOOL> () ;
		return std::move (ret) ;
	}

	ITEM get (const std::chrono::milliseconds &interval ,const Function<BOOL ()> &pred) const popping {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		std::unique_lock<std::mutex> sgd (mThis->mItemMutex) ;
		while (mThis->mItemCondition.exist () && mThis->mThreadFlag.exist () && mThis->mThreadFlag.self) {
			const auto r1x = pred () ;
			_DYNAMIC_ASSERT_ (r1x) ;
			mThis->mItemCondition->wait_for (sgd ,interval) ;
		}
		_DYNAMIC_ASSERT_ (mThis->mThreadFlag.exist ()) ;
		_DYNAMIC_ASSERT_ (!mThis->mThreadFlag.self) ;
		if (mThis->mException.exist ())
			std::rethrow_exception (mThis->mException.self) ;
		_DYNAMIC_ASSERT_ (mThis->mItem.exist ()) ;
		ITEM ret = std::move (mThis->mItem.self) ;
		mThis->mThreadFlag = AutoRef<BOOL> () ;
		return std::move (ret) ;
	}

	Optional<ITEM> peek () const {
		_STATIC_ASSERT_ (std::is_copy_constructible<ITEM>::value && std::is_nothrow_move_constructible<ITEM>::value) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		ScopedGuard<std::mutex> ANONYMOUS (mThis->mItemMutex) ;
		Optional<ITEM> ret = Optional<ITEM>::nullopt () ;
		if (mThis->mThreadFlag.exist () && !mThis->mThreadFlag.self && !mThis->mException.exist () && mThis->mItem.exist ())
			ret.template recreate<ITEM> (mThis->mItem.self) ;
		return std::move (ret) ;
	}

	void stop () {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		Promise<ITEM>::destroy_counter (&mThis.self) ;
	}

private:
	explicit Future (const IntrusiveRef<Pack> &_this) :mThis (_this) {}
} ;

template <class ITEM>
using Future = typename Promise<ITEM>::Future ;

template <class ITEM>
inline typename Promise<ITEM>::Future Promise<ITEM>::future () const {
	_DEBUG_ASSERT_ (mThis.exist ()) ;
	return Future (mThis) ;
}

template <class ITEM>
inline typename Promise<ITEM>::Future Promise<ITEM>::async (Function<ITEM ()> &&proc) {
	const auto r1x = Promise<ITEM> () ;
	r1x.start (std::move (proc)) ;
	return r1x.future () ;
}
} ;