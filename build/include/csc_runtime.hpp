#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"

namespace CSC {
template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> final :private Wrapped<void> {
private:
	struct VALUE_NODE {
		FLAG mGUID ;
		PTR<struct VALUE_NODE> mNext ;
		BOOL mReadOnly ;
		VAR mData ;
	} ;

	using GUID_TYPE = TEMP<BYTE[DEFAULT_RECURSIVE_SIZE::value]> ;

	struct CLASS_NODE {
		GUID_TYPE mGUID ;
		PTR<struct CLASS_NODE> mNext ;
		PTR<NONE> mData ;
	} ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = GlobalStatic ;

	private:
		template <class>
		friend class GlobalStatic ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mNodeMutex ;
		LENGTH mVNLength ;
		PTR<VALUE_NODE> mVNRoot ;
		LENGTH mCNLength ;
		PTR<CLASS_NODE> mCNRoot ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Holder> ;

private:
	//@warn: this function should be implemented in a 'runtime.dll'
	static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

	static Holder &unique () popping {
		return _CACHE_ ([] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Holder> () ;
			for (FOR_ONCE_DO_WHILE) {
				if (rax != NULL)
					continue ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Holder>::make () ;
				const auto r2x = rbx.watch () ;
				const auto r3x = &_XVALUE_<Holder> (r2x) ;
				const auto r4x = &_LOAD_<NONE> (r3x) ;
				rax = unique_atomic_address (NULL ,r4x) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			const auto r5x = &_LOAD_<Holder> (rax) ;
			return IntrusiveRef<Holder> (r5x).watch () ;
		}) ;
	}

	static PTR<VALUE_NODE> static_new_node (Holder &_self ,FLAG guid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<VALUE_NODE>> () ;
		ScopedHolder<VALUE_NODE> ANONYMOUS (sgd) ;
		PTR<VALUE_NODE> ret = &_LOAD_<VALUE_NODE> (_XVALUE_<PTR<TEMP<VALUE_NODE>>> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mGUID = guid ;
		ret->mNext = _self.mVNRoot ;
		_self.mVNRoot = ret ;
		_self.mVNLength++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	static PTR<CLASS_NODE> static_new_node (Holder &_self ,const GUID_TYPE &guid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<CLASS_NODE>> () ;
		ScopedHolder<CLASS_NODE> ANONYMOUS (sgd) ;
		PTR<CLASS_NODE> ret = &_LOAD_<CLASS_NODE> (_XVALUE_<PTR<TEMP<CLASS_NODE>>> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mGUID = guid ;
		ret->mNext = _self.mCNRoot ;
		_self.mCNRoot = ret ;
		_self.mCNLength++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	static PTR<VALUE_NODE> static_find_node (const Holder &_self ,FLAG guid) {
		for (PTR<VALUE_NODE> i = _self.mVNRoot ; i != NULL ; i = i->mNext)
			if (i->mGUID == guid)
				return i ;
		return NULL ;
	}

	static PTR<CLASS_NODE> static_find_node (const Holder &_self ,const GUID_TYPE &guid) {
		for (PTR<CLASS_NODE> i = _self.mCNRoot ; i != NULL ; i = i->mNext)
			if (_MEMEQUAL_ (i->mGUID.unused ,PTRTOARR[guid.unused]))
				return i ;
		return NULL ;
	}

	static void intrusive_create (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		_self.mCounter.self = 0 ;
		_self.mVNLength = 0 ;
		_self.mVNRoot = NULL ;
		_self.mCNLength = 0 ;
		_self.mCNRoot = NULL ;
	}

	static void intrusive_destroy (Holder &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		_CALL_EH_ ([&] () {
			for (PTR<VALUE_NODE> i = _self.mVNRoot ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				i->~VALUE_NODE () ;
				GlobalHeap::free (i) ;
			}
		}) ;
		_self.mVNLength = 0 ;
		_self.mVNRoot = NULL ;
		_CALL_EH_ ([&] () {
			for (PTR<CLASS_NODE> i = _self.mCNRoot ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				i->~CLASS_NODE () ;
				GlobalHeap::free (i) ;
			}
		}) ;
		_self.mCNLength = 0 ;
		_self.mCNRoot = NULL ;
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

template <FLAG GUID>
class GlobalStatic<ARGC<GUID>> final :private Wrapped<void> {
public:
	static void init (VAR data ,BOOL read_only) {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		if (r1x != NULL)
			return ;
		const auto r2x = GlobalStatic<void>::static_new_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		r2x->mReadOnly = read_only ;
		r2x->mData = data ;
	}

	static VAR load () popping {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		return r1x->mData ;
	}

	static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		_DYNAMIC_ASSERT_ (!r1x->mReadOnly) ;
		if (r1x->mData == expect)
			r1x->mData = data ;
		return r1x->mData ;
	}

	static void save (VAR data) {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		for (FOR_ONCE_DO_WHILE) {
			if (rax != NULL)
				continue ;
			rax = GlobalStatic<void>::static_new_node (r1 ,GUID) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mData = data ;
	}
} ;

template <class TYPE>
class GlobalStatic<Singleton<TYPE>> final :private Wrapped<void> {
private:
	class Holder {
	public:
		using INTRUSIVE_TYPE = GlobalStatic ;

	public:
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Singleton<TYPE> mData ;
	} ;

	using GUID_TYPE = typename GlobalStatic<void>::GUID_TYPE ;

private:
	friend IntrusiveRef<Holder> ;

private:
	static void intrusive_create (Holder &_self) {
		_self.mCounter.self = 0 ;
	}

	static void intrusive_destroy (Holder &_self) {
		_STATIC_WARNING_ ("noop") ;
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

	static GUID_TYPE guid_from_typeid_name () {
		GUID_TYPE ret ;
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (typeid (TYPE).name ()) ;
		const auto r2x = _MEMCHR_ (PTRTOARR[r1x] ,DEFAULT_HUGEBUFFER_SIZE::value ,BYTE (0X00)) ;
		_DEBUG_ASSERT_ (r2x > 0 && r2x < _SIZEOF_ (GUID_TYPE)) ;
		const auto r3x = _MIN_ (r2x ,_SIZEOF_ (GUID_TYPE)) ;
		_MEMCOPY_ (PTRTOARR[_ZERO_ (ret).unused] ,PTRTOARR[r1x] ,r3x) ;
		return std::move (ret) ;
	}

public:
	static Singleton<TYPE> &unique () popping {
		auto &r1 = _CACHE_ ([] () {
			auto &r2 = GlobalStatic<void>::unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2.mNodeMutex) ;
			const auto r2x = guid_from_typeid_name () ;
			auto rax = GlobalStatic<void>::static_find_node (r2 ,r2x) ;
			auto rbx = IntrusiveRef<Holder> () ;
			for (FOR_ONCE_DO_WHILE) {
				if (rax != NULL)
					continue ;
				rax = GlobalStatic<void>::static_new_node (r2 ,r2x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Holder>::make () ;
				const auto r3x = rbx.watch () ;
				const auto r4x = &_XVALUE_<Holder> (r3x) ;
				rax->mData = &_LOAD_<NONE> (r4x) ;
			}
			const auto r5x = &_LOAD_<Holder> (rax->mData) ;
			return IntrusiveRef<Holder> (r5x).watch () ;
		}) ;
		return _XVALUE_<Holder> (r1).mData ;
	}
} ;

#ifdef __CSC_DEPRECATED__
template <class TYPE>
class Coroutine {
public:
	class SubRef ;

private:
	static constexpr auto STATUS_CREATED = FLAG (0) ;
	static constexpr auto STATUS_RUNNING = FLAG (1) ;
	static constexpr auto STATUS_SUSPEND = FLAG (2) ;
	static constexpr auto STATUS_STOPPED = FLAG (3) ;

private:
	class Implement ;
	Monostate<FLAG> mCoStatus ;
	AutoRef<TYPE> mCoContext ;
	AnyRef<void> mCoBreakPoint ;
	Array<Function<DEF<void (SubRef &)> NONE::*>> mSubProc ;
	Array<AnyRef<void>> mSubBreakPoint ;
	Queue<INDEX> mSubQueue ;
	Priority<VAR ,INDEX> mSubAwaitQueue ;
	INDEX mSubCurr ;

public:
	Coroutine () {
		mCoStatus.self = STATUS_CREATED ;
		mSubCurr = VAR_NONE ;
	}

	BOOL ready () const {
		if (mCoStatus.self != STATUS_STOPPED)
			return FALSE ;
		return TRUE ;
	}

	TYPE &context () & {
		_DEBUG_ASSERT_ (mCoContext.exist ()) ;
		return mCoContext ;
	}

	TYPE &context () && = delete ;

	void start (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		mCoContext = AutoRef<TYPE>::make () ;
		mCoBreakPoint = AnyRef<void> () ;
		mSubProc = std::move (proc) ;
		mSubBreakPoint = Array<AnyRef<void>> (mSubProc.size ()) ;
		mSubQueue = Queue<INDEX> (mSubProc.length ()) ;
		for (INDEX i = 0 ; i < mSubProc.length () ; i++)
			mSubQueue.add (i) ;
		mSubAwaitQueue = Priority<VAR ,INDEX> (mSubProc.length ()) ;
		mSubQueue.take (mSubCurr) ;
	}

	void execute () {
		_DEBUG_ASSERT_ (!mCoBreakPoint.exist ()) ;
		init_break_point (mCoBreakPoint) ;
		for (auto &&i : mSubBreakPoint) {
			_DEBUG_ASSERT_ (!i.exist ()) ;
			init_break_point (i) ;
		}
		mCoStatus.self = STATUS_CREATED ;
		store_break_point (mCoBreakPoint) ;
		for (auto &&i : mSubBreakPoint) {
			if (mCoStatus.self != STATUS_STOPPED)
				continue ;
			if (!i.exist ())
				continue ;
			goto_break_point (i) ;
		}
		if (mCoStatus.self != STATUS_CREATED)
			return ;
		for (auto &&i : mSubBreakPoint) {
			if (mCoStatus.self != STATUS_CREATED)
				continue ;
			store_break_point (i) ;
		}
		_DEBUG_ASSERT_ (mSubCurr != VAR_NONE) ;
		const auto r1x = mSubCurr ;
		_CALL_TRY_ ([&] () {
			if (mCoStatus.self == STATUS_STOPPED)
				return ;
			mCoStatus.self = STATUS_RUNNING ;
			mSubProc[r1x] (_CAST_<SubRef> (*this)) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
		mSubBreakPoint[r1x] = AnyRef<void> () ;
		mCoStatus.self = STATUS_STOPPED ;
		goto_break_point (mCoBreakPoint) ;
	}

private:
	static void init_break_point (AnyRef<void> &bp) ;

	static void store_break_point (AnyRef<void> &bp) noexcept ;

	static void goto_break_point (AnyRef<void> &bp) noexcept ;
} ;

template <class TYPE>
class Coroutine<TYPE>::SubRef :private Wrapped<Coroutine<TYPE>> {
private:
	using Wrapped<Coroutine<TYPE>>::mSelf ;

public:
	TYPE &to () {
		_DEBUG_ASSERT_ (mSelf.mCoContext.exist ()) ;
		return mSelf.mCoContext ;
	}

	inline implicit operator TYPE & () {
		return to () ;
	}

	inline PTR<TYPE> operator-> () {
		return &to () ;
	}

	const TYPE &to () const {
		_DEBUG_ASSERT_ (mSelf.mCoContext.exist ()) ;
		return mSelf.mCoContext ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

	void sub_await () {
		sub_await (mSelf.mSubAwaitQueue.length ()) ;
	}

	void sub_await (VAR priority) {
		_DEBUG_ASSERT_ (priority >= 0) ;
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		mSelf.mCoStatus.self = STATUS_SUSPEND ;
		store_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mCoStatus.self != STATUS_STOPPED) ;
		if (mSelf.mCoStatus.self != STATUS_SUSPEND)
			return ;
		mSelf.mSubAwaitQueue.add (priority ,mSelf.mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mSubQueue.empty ()) ;
		mSelf.mSubQueue.take (mSelf.mSubCurr) ;
		mSelf.mCoStatus.self = STATUS_RUNNING ;
		goto_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
	}

	void sub_resume () {
		sub_resume (mSelf.mSubAwaitQueue.length ()) ;
	}

	void sub_resume (LENGTH count) {
		for (INDEX i = 0 ; i < count ; i++) {
			if (mSelf.mSubAwaitQueue.empty ())
				continue ;
			const auto r1x = mSelf.mSubAwaitQueue[mSelf.mSubAwaitQueue.peek ()].item ;
			mSelf.mSubAwaitQueue.take () ;
			mSelf.mSubQueue.add (r1x) ;
		}
	}

	void sub_yield () {
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		if (mSelf.mSubQueue.empty ())
			return ;
		mSelf.mCoStatus.self = STATUS_SUSPEND ;
		store_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mCoStatus.self != STATUS_STOPPED) ;
		if (mSelf.mCoStatus.self != STATUS_SUSPEND)
			return ;
		mSelf.mSubQueue.add (mSelf.mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mSubQueue.empty ()) ;
		mSelf.mSubQueue.take (mSelf.mSubCurr) ;
		mSelf.mCoStatus.self = STATUS_RUNNING ;
		goto_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
	}

	void sub_return () {
		_DEBUG_ASSERT_ (mSubCurr != VAR_NONE) ;
		mSelf.mSubQueue.clear () ;
		mSelf.mSubAwaitQueue.clear () ;
		mSelf.mSubCurr = VAR_NONE ;
		mSelf.mCoStatus.self = STATUS_STOPPED ;
		goto_break_point (mSelf.mCoBreakPoint) ;
	}
} ;
#endif

class RandomService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual VAL entropy () const = 0 ;
		virtual void reset_seed (VAR _seed) = 0 ;
		virtual VAR random_value () popping = 0 ;
		virtual void random_skip (LENGTH len) = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<RandomService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	VAL entropy () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->entropy () ;
	}

	void reset_seed (VAR _seed) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->reset_seed (_seed) ;
	}

	VAR random_value (VAR _min ,VAR _max) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (_min >= 0 && _min <= _max) ;
		const auto r1x = mThis->random_value () ;
		return r1x % (_max - _min + 1) + _min ;
	}

	Array<VAR> random_value (VAR _min ,VAR _max ,LENGTH len) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		Array<VAR> ret = Array<VAR> (len) ;
		const auto r1x = _max - _min + 1 ;
		for (INDEX i = 0 ; i < ret.length () ; i++) {
			const auto r2x = mThis->random_value () ;
			ret[i] = r2x % r1x + _min ;
		}
		return std::move (ret) ;
	}

	BitSet<> random_shuffle (LENGTH count ,LENGTH range) popping {
		return random_shuffle (count ,range ,BitSet<> (range)) ;
	}

	BitSet<> random_shuffle (LENGTH count ,LENGTH range ,BitSet<> &&res) popping {
		_DEBUG_ASSERT_ (count >= 0 && count < range) ;
		_DEBUG_ASSERT_ (res.size () == range) ;
		BitSet<> ret = std::move (res) ;
		ret.clear () ;
		while (TRUE) {
			if (ret.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range - 1)) ;
			ret[ix] = TRUE ;
		}
		return std::move (ret) ;
	}

	BitSet<> random_shuffle (LENGTH count ,const BitSet<> &range) popping {
		return random_shuffle (count ,range ,BitSet<> (range.size ())) ;
	}

	BitSet<> random_shuffle (LENGTH count ,const BitSet<> &range ,BitSet<> &&res) popping {
		_DEBUG_ASSERT_ (count >= 0 && count < range.size ()) ;
		_DEBUG_ASSERT_ (res.size () == range.size ()) ;
		BitSet<> ret = std::move (res) ;
		ret.clear () ;
		while (TRUE) {
			if (ret.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range.size () - 1)) ;
			ret[ix] = range[ix] ;
		}
		return std::move (ret) ;
	}

	String<STR> random_uuid () popping {
		String<STR> ret = String<STR> (_COUNTOF_ (decltype ("00000000-0000-0000-000000000000"))) ;
		INDEX iw = 0 ;
		const auto r5x = random_value (0 ,36 ,28) ;
		for (INDEX i = 0 ; i < 8 ; i++) {
			INDEX ix = 0 + i ;
			ret[iw++] = index_to_hex (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = 8 + i ;
			ret[iw++] = index_to_hex (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = 12 + i ;
			ret[iw++] = index_to_hex (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 12 ; i++) {
			INDEX ix = 16 + i ;
			ret[iw++] = index_to_hex (r5x[ix]) ;
		}
		if (iw < ret.size ())
			ret[iw] = 0 ;
		return std::move (ret) ;
	}

	void random_skip (LENGTH len) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->random_skip (len) ;
	}

private:
	RandomService () ;

private:
	static STRU8 index_to_hex (INDEX index) {
		const auto r1x = (index < 10) ? (STRU8 ('0')) : (STRU8 ('A') - 10) ;
		return STRU8 (r1x + index) ;
	}
} ;
} ;