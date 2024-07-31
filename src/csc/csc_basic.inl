#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
class OptionalImplHolder implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		BoxHolder::create (fake.mValue.self)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}

	void initialize (CREF<FLAG> code) override {
		fake.mCode = code ;
	}

	BOOL exist () const override {
		return BoxHolder::create (fake.mValue.self)->exist () ;
	}

	FLAG code () const override {
		return fake.mCode ;
	}

	void fetch (VREF<BoxLayout> item) const override {
		assume (exist ()) ;
		BoxHolder::create (item)->acquire (fake.mValue.self) ;
		BoxHolder::create (fake.mValue.self)->release () ;
	}

	void store (VREF<BoxLayout> item) const override {
		assume (!exist ()) ;
		BoxHolder::create (fake.mValue.self)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::create (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::create (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

struct FunctionImplLayout {
	BoxLayout mValue ;
} ;

class FunctionImplHolder implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<Unknown> holder) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<FunctionImplLayout> () ,r1x ,1) ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (raw ())->release () ;
		BoxHolder::create (raw ())->initialize (holder) ;
		BoxHolder::create (item)->release () ;
	}

	void initialize (CREF<FunctionLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::create (raw ())->unknown ()) ;
		return r1x->rank () ;
	}

	void invoke (CREF<WrapperLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::create (raw ())->unknown ()) ;
		return r1x->invoke (BoxHolder::create (raw ())->self ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefImplLayout {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz_) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,r1x ,1) ;
		fake.mThis->mClazz = clazz_ ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (raw ().mHolder == ZERO)
			return ;
		BoxHolder::create (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	Clazz clazz () const override {
		if (!exist ())
			return Clazz () ;
		return fake.mThis->mClazz ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	VREF<Pointer> rebind (CREF<Clazz> clazz_) leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (fake) ;
	}

	CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (fake) ;
	}

	AutoRefLayout recast (CREF<Unknown> simple) override {
		AutoRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mPointer = r1x->recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<AutoRefHolder> AutoRefHolder::create (VREF<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

struct SharedRefMutexLayout {
	FLAG mHolder ;

public:
	implicit SharedRefMutexLayout () noexcept {
		mHolder = ZERO ;
	}
} ;

struct SharedRefMutexHolder implement Interface {
	imports VFat<SharedRefMutexHolder> create (VREF<SharedRefMutexLayout> that) ;
	imports CFat<SharedRefMutexHolder> create (CREF<SharedRefMutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class SharedRefMutex implement SharedRefMutexLayout {
protected:
	using SharedRefMutexLayout::mHolder ;

public:
	explicit SharedRefMutex () {
		SharedRefMutexHolder::create (thiz)->initialize () ;
	}

	void enter () {
		return SharedRefMutexHolder::create (thiz)->enter () ;
	}

	void leave () {
		return SharedRefMutexHolder::create (thiz)->leave () ;
	}
} ;

struct SharedRefMutexImplLayout {
	BOOL mOnce ;
	Pin<std::recursive_mutex> mMutex ;

public:
	imports CREF<SharedRefMutexImplLayout> instance () {
		return memorize ([&] () {
			SharedRefMutexImplLayout ret ;
			ret.mOnce = FALSE ;
			return move (ret) ;
		}) ;
	}
} ;

class SharedRefMutexImplHolder implement Fat<SharedRefMutexHolder ,SharedRefMutexLayout> {
public:
	void initialize () override {
		fake.mHolder = inline_hold (thiz) ;
		if (ptr (fake).mOnce)
			return ;
		ptr (fake).mOnce = TRUE ;
		ptr (fake).mMutex.remake () ;
	}

	imports VREF<SharedRefMutexImplLayout> ptr (CREF<SharedRefMutexLayout> layout) {
		return Pointer::make (address (SharedRefMutexImplLayout::instance ())) ;
	}

	void enter () override {
		return ptr (fake).mMutex->lock () ;
	}

	void leave () override {
		return ptr (fake).mMutex->unlock () ;
	}
} ;

exports VFat<SharedRefMutexHolder> SharedRefMutexHolder::create (VREF<SharedRefMutexLayout> that) {
	if (that.mHolder == ZERO)
		return VFat<SharedRefMutexHolder> (SharedRefMutexImplHolder () ,that) ;
	auto &&rax = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<SharedRefMutexHolder> (rax ,that) ;
}

exports CFat<SharedRefMutexHolder> SharedRefMutexHolder::create (CREF<SharedRefMutexLayout> that) {
	if (that.mHolder == ZERO)
		return CFat<SharedRefMutexHolder> (SharedRefMutexImplHolder () ,that) ;
	auto &&rax = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<SharedRefMutexHolder> (rax ,that) ;
}

struct RefImplLayout ;

struct SharedRefImplLayout {
	Pin<SharedRefMutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<SharedRefImplLayout> () ,r1x ,1) ;
		fake.mThis->mMutex.self = SharedRefMutex () ;
		fake.mThis->mCounter = 0 ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (BoxHolder::create (raw ())->self) ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			Scope<SharedRefMutex> anonymous (that.mThis->mMutex.self) ;
			fake.mThis = that.mThis.share () ;
			fake.mThis->mCounter++ ;
			fake.mPointer = address (BoxHolder::create (raw ())->self) ;
		}
	}

	void initialize (CREF<Unknown> holder ,VREF<Pointer> pointer) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = address (pointer) - inline_alignas (SIZE_OF<SharedRefImplLayout>::expr ,r1x->type_align ()) ;
		auto &&rax = keep[TYPE<SharedRefImplLayout>::expr] (Pointer::make (r2x)) ;
		const auto r3x = BoxHolder::create (rax.mValue)->unknown () ;
		if (inline_hold (r3x) != inline_hold (holder))
			return ;
		Scope<SharedRefMutex> anonymous (rax.mMutex.self) ;
		if (rax.mCounter <= 0)
			return ;
		const auto r4x = r2x - inline_alignas (SIZE_OF<RefImplLayout>::expr ,ALIGN_OF<SharedRefImplLayout>::expr) ;
		auto &&rbx = keep[TYPE<RefImplLayout>::expr] (Pointer::make (r4x)) ;
		if (rbx.mCounter.self <= 0)
			return ;
		auto &&rcx = keep[TYPE<RefLayout>::expr] (fake.mThis) ;
		rcx.mHandle = address (rbx) ;
		rcx.mPointer = address (rax) ;
		rbx.mCounter.self++ ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (pointer) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (raw ().mHolder == ZERO)
			return ;
		Scope<SharedRefMutex> anonymous (fake.mThis->mMutex.self) ;
		const auto r1x = --fake.mThis->mCounter ;
		if (r1x > 0)
			return ;
		BoxHolder::create (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	LENGTH counter () const override {
		if (!exist ())
			return 0 ;
		Scope<SharedRefMutex> anonymous (fake.mThis->mMutex.self) ;
		return fake.mThis->mCounter ;
	}

	VREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	SharedRefLayout recast (CREF<Unknown> simple) override {
		SharedRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mPointer = r1x->recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::create (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::create (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefImplLayout {
	Function<VREF<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplHolder implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<FunctionLayout> dtor) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<UniqueRefImplLayout> () ,r1x ,1) ;
		fake.mThis->mOwner = move (keep[TYPE<Function<VREF<Pointer>>>::expr] (dtor)) ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (raw ().mHolder == ZERO)
			return ;
		fake.mThis->mOwner (BoxHolder::create (raw ())->self) ;
		BoxHolder::create (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	UniqueRefLayout recast (CREF<Unknown> simple) override {
		UniqueRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mPointer = r1x->recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::create (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferImplLayout {
	LENGTH mWidth ;
	BoxLayout mValue ;
} ;

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		fake.mHolder = inline_hold (holder) ;
		fake.mBuffer = ZERO ;
		fake.mSize = 0 ;
		fake.mStep = 0 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		fake.mHolder = inline_hold (holder) ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,size_) ;
		BoxHolder::create (raw ())->initialize (r1x) ;
		fake.mBuffer = address (BoxHolder::create (raw ())->self) ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		fake.mStep = r2x->type_size () ;
		const auto r3x = RFat<ReflectCreate> (r1x) ;
		r3x->create (self ,size_) ;
		fake.mThis->mWidth = size_ ;
	}

	void initialize (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		fake.mHolder = inline_hold (holder) ;
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,1) ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mThis->mWidth = USED ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (fake.mThis == NULL)
			return ;
		if (fake.mThis->mWidth == USED)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		const auto r2x = RFat<ReflectDestroy> (r1x) ;
		r2x->destroy (self ,fake.mThis->mWidth) ;
		BoxHolder::create (raw ())->release () ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	RFat<Unknown> unknown () const override {
		assert (exist ()) ;
		auto &&rax = keep[TYPE<Unknown>::expr] (Pointer::from (fake.mHolder)) ;
		return RFat<Unknown> (rax ,NULL) ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return Pointer::make (fake.mBuffer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return Pointer::make (fake.mBuffer) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		const auto r1x = size () ;
		if (size_ <= r1x)
			return ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			assert (fake.mThis->mWidth != USED) ;
		}
		auto rax = Ref<RefBufferImplLayout> () ;
		const auto r2x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::create (rax)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r2x ,size_) ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () * r1x ;
		BoxHolder::create (rax->mValue)->initialize (r2x) ;
		const auto r5x = address (BoxHolder::create (rax->mValue)->self) ;
		inline_memcpy (Pointer::make (r5x) ,self ,r4x) ;
		const auto r6x = r5x + r4x ;
		const auto r7x = RFat<ReflectCreate> (r2x) ;
		r7x->create (Pointer::make (r6x) ,size_ - r1x) ;
		swap (fake.mThis ,rax) ;
		if ifdo (TRUE) {
			if (rax == NULL)
				discard ;
			BoxHolder::create (rax->mValue)->release () ;
		}
		fake.mThis->mWidth = size_ ;
		fake.mBuffer = r5x ;
		fake.mSize = size_ ;
		fake.mStep = r3x->type_size () ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

class FarBufferImplHolder implement Fat<FarBufferHolder ,FarBufferLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectElement> (holder)->element () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,r1x ,0) ;
		fake.mIndex = NONE ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (holder) ;
		fake.mStep = r2x->type_size () ;
	}

	void use_getter (CREF<FunctionLayout> getter) override {
		fake.mGetter = move (keep[TYPE<Function<CREF<INDEX> ,VREF<Pointer>>>::expr] (getter)) ;
	}

	void use_setter (CREF<FunctionLayout> setter) override {
		fake.mSetter = move (keep[TYPE<Function<CREF<INDEX> ,CREF<Pointer>>>::expr] (setter)) ;
	}

	BOOL exist () const override {
		return fake.mThis.exist () ;
	}

	RFat<Unknown> unknown () const override {
		return fake.mThis.unknown () ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return fake.mThis.self ;
	}

	void update_sync (CREF<INDEX> index) {
		if (fake.mIndex == index)
			return ;
		refresh () ;
		fake.mGetter (index ,fake.mThis.self) ;
		fake.mIndex = index ;
	}

	void refresh () override {
		if (fake.mIndex == NONE)
			return ;
		fake.mSetter (fake.mIndex ,fake.mThis.self) ;
		fake.mIndex = NONE ;
	}
} ;

exports VFat<FarBufferHolder> FarBufferHolder::create (VREF<FarBufferLayout> that) {
	return VFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

exports CFat<FarBufferHolder> FarBufferHolder::create (CREF<FarBufferLayout> that) {
	return CFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

class AllocatorImplHolder implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (holder) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		RefBufferHolder::create (fake.mAllocator)->initialize (holder ,size_) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		const auto r1x = inline_min (fake.mRest ,size ()) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (xbt (fake ,i).mNext != USED)
				continue ;
			r2x->destroy (fake.mAllocator.at (i) ,1) ;
		}
	}

	BOOL exist () const override {
		return fake.mAllocator.exist () ;
	}

	RFat<Unknown> unknown () const override {
		return fake.mAllocator.unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mAllocator.raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mAllocator.raw () ;
	}

	void clear () override {
		destroy () ;
		fake.mRest = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	LENGTH size () const override {
		return fake.mAllocator.size () ;
	}

	LENGTH step () const override {
		return fake.mAllocator.step () ;
	}

	LENGTH length () const override {
		if (!exist ())
			return 0 ;
		return fake.mLength ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return fake.mAllocator.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return fake.mAllocator.at (index) ;
	}

	VREF<Pointer> bt (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return Pointer::from (xbt (fake ,index)) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return Pointer::from (xbt (fake ,index)) ;
	}

	imports VREF<AllocatorNode> xbt (VREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	imports CREF<AllocatorNode> xbt (CREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	INDEX alloc () override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = xbt (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectCreate> (unknown ()) ;
		r1x->create (fake.mAllocator.at (ret) ,1) ;
		xbt (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = xbt (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_size () ;
		inline_memcpy (fake.mAllocator.at (ret) ,BoxHolder::create (item)->self ,r2x) ;
		inline_memset (BoxHolder::create (item)->self ,r2x) ;
		xbt (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		r2x->destroy (fake.mAllocator.at (r1x) ,1) ;
		xbt (fake ,r1x).mNext = fake.mFree ;
		fake.mFree = r1x ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if (!inline_between (index ,0 ,fake.mRest))
			return FALSE ;
		const auto r1x = xbt (fake ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		assert (exist ()) ;
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mRest < size ())
					discard ;
				const auto r1x = inline_max (fake.mRest * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
				resize (r1x) ;
			}
			fake.mFree = fake.mRest ;
			xbt (fake ,fake.mFree).mNext = NONE ;
			fake.mRest++ ;
		}
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::create (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::create (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;