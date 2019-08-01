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
		BOOL mReadOnly ;
		VAR mData ;
	} ;

	using GUID_TYPE = TEMP<BYTE[DEFAULT_RECURSIVE_SIZE::value]> ;

	struct CLASS_NODE {
		GUID_TYPE mGUID ;
		PTR<NONE> mData ;
	} ;

	class Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename GlobalStatic::Detail ;

	private:
		template <class>
		friend class GlobalStatic ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mNodeMutex ;
		LinkedRef<VALUE_NODE> mValueNode ;
		LinkedRef<CLASS_NODE> mClassNode ;
	} ;

private:
	class Detail ;
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Holder> ;

private:
	static Holder &static_unique () popping {
		return _CACHE_ ([] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Holder> () ;
			for (FOR_ONCE_DO_WHILE) {
				if (rax != NULL)
					discard ;
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
		_self.mValueNode.push () ;
		_self.mValueNode->mGUID = guid ;
		return &_self.mValueNode.self ;
	}

	static PTR<CLASS_NODE> static_new_node (Holder &_self ,const GUID_TYPE &guid) popping {
		_self.mClassNode.push () ;
		_self.mClassNode->mGUID = guid ;
		return &_self.mClassNode.self ;
	}

	static PTR<VALUE_NODE> static_find_node (Holder &_self ,FLAG guid) {
		PTR<VALUE_NODE> ret = NULL ;
		for (FOR_ONCE_DO_WHILE) {
			if (!_self.mValueNode.exist ())
				discard ;
			const auto r1x = &_self.mValueNode.self ;
			while (TRUE) {
				if (ret != NULL)
					break ;
				if (_self.mValueNode->mGUID == guid)
					ret = &_self.mValueNode.self ;
				if (&_self.mValueNode.self == r1x)
					break ;
				_self.mValueNode.cycle () ;
			}
		}
		return std::move (ret) ;
	}

	static PTR<CLASS_NODE> static_find_node (Holder &_self ,const GUID_TYPE &guid) {
		PTR<CLASS_NODE> ret = NULL ;
		for (FOR_ONCE_DO_WHILE) {
			if (!_self.mClassNode.exist ())
				discard ;
			const auto r1x = &_self.mClassNode.self ;
			while (TRUE) {
				if (ret != NULL)
					break ;
				if (_MEMEQUAL_ (_self.mClassNode->mGUID.unused ,PTRTOARR[guid.unused]))
					ret = &_self.mClassNode.self ;
				if (&_self.mClassNode.self == r1x)
					break ;
				_self.mClassNode.cycle () ;
			}
		}
		return std::move (ret) ;
	}

public:
	//@warn: this function should be implemented in a 'runtime.dll'
	static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

private:
	class Detail :private Wrapped<void> {
	public:
		inline static void friend_create (Holder &_self) {
			ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
			_self.mCounter.self = 0 ;
			_self.mValueNode = LinkedRef<VALUE_NODE> () ;
			_self.mClassNode = LinkedRef<CLASS_NODE> () ;
		}

		inline static void friend_destroy (Holder &_self) {
			ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
			_self.mValueNode = LinkedRef<VALUE_NODE> () ;
			_self.mClassNode = LinkedRef<CLASS_NODE> () ;
		}

		inline static LENGTH friend_attach (Holder &_self) popping {
			return ++_self.mCounter.self ;
		}

		inline static LENGTH friend_detach (Holder &_self) popping {
			return --_self.mCounter.self ;
		}

		inline static void friend_latch (Holder &_self) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

template <FLAG GUID>
class GlobalStatic<ARGC<GUID>> final :private Wrapped<void> {
public:
	static void init (VAR data ,BOOL read_only) {
		auto &r1 = GlobalStatic<void>::static_unique () ;
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
		auto &r1 = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		return r1x->mData ;
	}

	static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1 = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		_DYNAMIC_ASSERT_ (!r1x->mReadOnly) ;
		if (r1x->mData == expect)
			r1x->mData = data ;
		return r1x->mData ;
	}

	static void save (VAR data) {
		auto &r1 = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		for (FOR_ONCE_DO_WHILE) {
			if (rax != NULL)
				discard ;
			rax = GlobalStatic<void>::static_new_node (r1 ,GUID) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mData = data ;
	}
} ;

template <class UNIT>
class GlobalStatic<Singleton<UNIT>> final :private Wrapped<void> {
private:
	class Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename GlobalStatic::Detail ;

	public:
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Singleton<UNIT> mData ;
	} ;

	using GUID_TYPE = typename GlobalStatic<void>::GUID_TYPE ;

private:
	class Detail ;
	friend IntrusiveRef<Holder> ;

public:
	static Singleton<UNIT> &unique () popping {
		auto &r1 = _CACHE_ ([] () {
			auto &r2 = GlobalStatic<void>::static_unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2.mNodeMutex) ;
			const auto r2x = Detail::guid_from_typeid_name () ;
			auto rax = GlobalStatic<void>::static_find_node (r2 ,r2x) ;
			auto rbx = IntrusiveRef<Holder> () ;
			for (FOR_ONCE_DO_WHILE) {
				if (rax != NULL)
					discard ;
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

private:
	class Detail :private Wrapped<void> {
	public:
		inline static void friend_create (Holder &_self) {
			_self.mCounter.self = 0 ;
		}

		inline static void friend_destroy (Holder &_self) {
			_STATIC_WARNING_ ("noop") ;
		}

		inline static LENGTH friend_attach (Holder &_self) popping {
			return ++_self.mCounter.self ;
		}

		inline static LENGTH friend_detach (Holder &_self) popping {
			return --_self.mCounter.self ;
		}

		inline static void friend_latch (Holder &_self) {
			GlobalRuntime::thread_sleep () ;
		}

		inline static GUID_TYPE guid_from_typeid_name () {
			GUID_TYPE ret ;
			_STATIC_WARNING_ ("mark") ;
			const auto r1x = typeid (UNIT).name () ;
			auto &r1 = _LOAD_<ARR<BYTE>> (&PTRTOARR[r1x]) ;
			const auto r2x = _MEMCHR_ (r1 ,DEFAULT_HUGEBUFFER_SIZE::value ,BYTE (0X00)) ;
			_DEBUG_ASSERT_ (BOOL (r2x > 0 && r2x < _SIZEOF_ (GUID_TYPE))) ;
			const auto r3x = _MIN_ (r2x ,_SIZEOF_ (GUID_TYPE)) ;
			_ZERO_ (ret) ;
			_MEMCOPY_ (PTRTOARR[ret.unused] ,r1 ,r3x) ;
			return std::move (ret) ;
		}
	} ;
} ;

#ifdef __CSC_DEPRECATED__
template <class UNIT>
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
	AutoRef<UNIT> mCoContext ;
	AnyRef<void> mCoBreakPoint ;
	Array<Function<DEF<void (SubRef &)> NONE::*>> mSubProc ;
	Array<AnyRef<void>> mSubBreakPoint ;
	Deque<INDEX> mSubQueue ;
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

	UNIT &context () & {
		_DEBUG_ASSERT_ (mCoContext.exist ()) ;
		return mCoContext ;
	}

	UNIT &context () && = delete ;

	void start (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		mCoContext = AutoRef<UNIT>::make () ;
		mCoBreakPoint = AnyRef<void> () ;
		mSubProc = std::move (proc) ;
		mSubBreakPoint = Array<AnyRef<void>> (mSubProc.size ()) ;
		mSubQueue = Deque<INDEX> (mSubProc.length ()) ;
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
	void init_break_point (AnyRef<void> &bp) ;

	void store_break_point (AnyRef<void> &bp) noexcept ;

	void goto_break_point (AnyRef<void> &bp) noexcept ;

public:
	static void csync (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) ;
} ;

template <class UNIT>
class Coroutine<UNIT>::SubRef :private Wrapped<Coroutine<UNIT>> {
private:
	using Wrapped<Coroutine<UNIT>>::mSelf ;

public:
	UNIT &to () {
		_DEBUG_ASSERT_ (mSelf.mCoContext.exist ()) ;
		return mSelf.mCoContext ;
	}

	inline implicit operator UNIT & () {
		return to () ;
	}

	inline PTR<UNIT> operator-> () {
		return &to () ;
	}

	const UNIT &to () const {
		_DEBUG_ASSERT_ (mSelf.mCoContext.exist ()) ;
		return mSelf.mCoContext ;
	}

	inline implicit operator const UNIT & () const {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const {
		return &to () ;
	}

	void sub_await () {
		sub_await (mSelf.mSubAwaitQueue.length ()) ;
	}

	void sub_await (VAR priority) {
		_DEBUG_ASSERT_ (priority >= 0) ;
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		_DYNAMIC_ASSERT_ (mSelf.mCoStatus.self == STATUS_RUNNING) ;
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
			const auto r1x = mSelf.mSubAwaitQueue[mSelf.mSubAwaitQueue.head ()].item ;
			mSelf.mSubAwaitQueue.take () ;
			mSelf.mSubQueue.add (r1x) ;
		}
	}

	void sub_yield () {
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		if (mSelf.mSubQueue.empty ())
			return ;
		_DYNAMIC_ASSERT_ (mSelf.mCoStatus.self == STATUS_RUNNING) ;
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
		_DYNAMIC_ASSERT_ (mSelf.mCoStatus.self == STATUS_RUNNING) ;
		mSelf.mSubQueue.clear () ;
		mSelf.mSubAwaitQueue.clear () ;
		mSelf.mSubCurr = VAR_NONE ;
		mSelf.mCoStatus.self = STATUS_STOPPED ;
		goto_break_point (mSelf.mCoBreakPoint) ;
	}
} ;
#endif

template <class UNIT>
inline void Coroutine<UNIT>::csync (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
	auto rax = Coroutine<UNIT> (std::move (proc)) ;
	rax.execute () ;
}

class RandomService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual VAL entropy () const = 0 ;
		virtual void reset_seed (VAR _seed) = 0 ;
		virtual VAR random_value () popping = 0 ;
		virtual void random_skip (LENGTH len) = 0 ;
	} ;

private:
	class Detail ;
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
		_DEBUG_ASSERT_ (BOOL (_min >= 0 && _min <= _max)) ;
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
		_DEBUG_ASSERT_ (BOOL (count >= 0 && count < range)) ;
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
		BitSet<> ret = BitSet<> (range.size ()) ;
		compute_random_shuffle (count ,range ,ret) ;
		return std::move (ret) ;
	}

	void compute_random_shuffle (LENGTH count ,const BitSet<> &range ,BitSet<> &chosen) popping {
		_DEBUG_ASSERT_ (BOOL (count >= 0 && count < range.size ())) ;
		_DEBUG_ASSERT_ (chosen.size () == range.size ()) ;
		chosen.clear () ;
		while (TRUE) {
			if (chosen.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range.size () - 1)) ;
			chosen[ix] = range[ix] ;
		}
	}

	String<STR> random_uuid () popping {
		static constexpr auto M_UUID = _PCSTR_ ("00000000-0000-0000-000000000000") ;
		String<STR> ret = String<STR> (M_UUID.size ()) ;
		INDEX iw = 0 ;
		const auto r5x = random_value (0 ,36 ,28) ;
		for (INDEX i = 0 ; i < 8 ; i++) {
			INDEX ix = 0 + i ;
			ret[iw++] = Detail::index_to_hex_str (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = 8 + i ;
			ret[iw++] = Detail::index_to_hex_str (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = 12 + i ;
			ret[iw++] = Detail::index_to_hex_str (r5x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ; i < 12 ; i++) {
			INDEX ix = 16 + i ;
			ret[iw++] = Detail::index_to_hex_str (r5x[ix]) ;
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
	class Detail :private Wrapped<void> {
	public:
		inline static STRU8 index_to_hex_str (INDEX index) {
			const auto r1x = (index < 10) ? (STRU8 ('0')) : (STRU8 ('A') - 10) ;
			return STRU8 (r1x + index) ;
		}
	} ;
} ;
} ;