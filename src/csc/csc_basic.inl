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

	void poll (VREF<BoxLayout> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (BoxHolder::create (item)->exist ())
				discard ;
			assume (exist ()) ;
			BoxHolder::create (item)->acquire (fake.mValue.self) ;
			BoxHolder::create (fake.mValue.self)->release () ;
		}
		if ifdo (act) {
			assume ((!exist ())) ;
			BoxHolder::create (fake.mValue.self)->acquire (item) ;
			BoxHolder::create (item)->release () ;
		}
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
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,r1x ,1) ;
		fake.mThis->mClazz = clazz_ ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (raw ())->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
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
		if (fake.mThis == NULL)
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
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			assert (clazz () == clazz_) ;
		}
		return Pointer::from (fake) ;
	}

	CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue override {
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			assert (clazz () == clazz_) ;
		}
		return Pointer::from (fake) ;
	}

	AutoRefLayout recast (CREF<Unknown> reflect) override {
		AutoRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (reflect) ;
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
		ptr (fake).mMutex = NULL ;
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
	auto &&tmp = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<SharedRefMutexHolder> (tmp ,that) ;
}

exports CFat<SharedRefMutexHolder> SharedRefMutexHolder::create (CREF<SharedRefMutexLayout> that) {
	if (that.mHolder == ZERO)
		return CFat<SharedRefMutexHolder> (SharedRefMutexImplHolder () ,that) ;
	auto &&tmp = keep[TYPE<SharedRefMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<SharedRefMutexHolder> (tmp ,that) ;
}

struct SharedRefImplLayout {
	Pin<SharedRefMutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
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
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = address (pointer) - inline_alignas (SIZE_OF<SharedRefImplLayout>::expr ,r1x->type_align ()) ;
		auto &&tmp1 = keep[TYPE<SharedRefImplLayout>::expr] (Pointer::make (r2x)) ;
		const auto r3x = BoxHolder::create (tmp1.mValue)->unknown () ;
		if (inline_hold (r3x) != inline_hold (holder))
			return ;
		Scope<SharedRefMutex> anonymous (tmp1.mMutex.self) ;
		if (tmp1.mCounter <= 0)
			return ;
		const auto r4x = r2x - inline_alignas (SIZE_OF<RefImplLayout>::expr ,ALIGN_OF<SharedRefImplLayout>::expr) ;
		auto &&tmp2 = keep[TYPE<RefImplLayout>::expr] (Pointer::make (r4x)) ;
		if (tmp2.mCounter.self <= 0)
			return ;
		auto &&tmp3 = keep[TYPE<RefLayout>::expr] (fake.mThis) ;
		tmp3.mHolder = address (tmp2) ;
		tmp3.mPointer = address (tmp1) ;
		tmp2.mCounter.self++ ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (pointer) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
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

	FLAG counter () const override {
		if (fake.mThis == NULL)
			return 0 ;
		Scope<SharedRefMutex> anonymous (fake.mThis->mMutex.self) ;
		return fake.mThis->mCounter ;
	}

	VREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	SharedRefLayout recast (CREF<Unknown> reflect) override {
		SharedRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (reflect) ;
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
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<UniqueRefImplLayout> () ,r1x ,1) ;
		fake.mThis->mOwner = move (keep[TYPE<Function<VREF<Pointer>>>::expr] (dtor)) ;
		BoxHolder::create (raw ())->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (raw ())->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
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

	UniqueRefLayout recast (CREF<Unknown> reflect) override {
		UniqueRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (reflect) ;
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

struct RefBufferType {
	enum {
		TRIVIAL ,
		DESTROY ,
		CARRIER ,
		ETC
	} ;
} ;

struct RefBufferImplLayout {
	JustInt<RefBufferType> mType ;
	BoxLayout mValue ;
} ;

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mThis != NULL)
			return ;
		fake = RefBufferLayout () ;
		fake.mThis = Ref<RefBufferImplLayout>::make () ;
		BoxHolder::create (raw ())->initialize (element) ;
		fake.mThis->mType = RefBufferType::TRIVIAL ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,element ,size_) ;
		BoxHolder::create (raw ())->initialize (element) ;
		fake.mBuffer = address (BoxHolder::create (raw ())->self) ;
		fake.mSize = size_ ;
		const auto r1x = RFat<ReflectSize> (element) ;
		fake.mStep = r1x->type_size () ;
		const auto r2x = RFat<ReflectCreate> (element) ;
		r2x->create (self ,size_) ;
		fake.mThis->mType = RefBufferType::DESTROY ;
	}

	void initialize (CREF<Unknown> element ,RREF<BoxLayout> item) override {
		const auto r1x = RFat<ReflectElement> (element)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,1) ;
		BoxHolder::create (raw ())->initialize (element) ;
		const auto r2x = RFat<ReflectAssign> (element) ;
		r2x->xinit (BoxHolder::create (raw ())->self) ;
		r2x->xmove (BoxHolder::create (raw ())->self ,BoxHolder::create (item)->self) ;
		fake.mThis->mType = RefBufferType::CARRIER ;
	}

	void destroy () override {
		if ((!exist ()))
			return ;
		if (raw ().mHolder == ZERO)
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mType != RefBufferType::DESTROY)
				discard ;
			const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
			r1x->destroy (self ,fake.mSize) ;
		}
		if ifdo (act) {
			if (fake.mThis->mType != RefBufferType::CARRIER)
				discard ;
			const auto r2x = RFat<ReflectElement> (unknown ())->unknown () ;
			const auto r3x = RFat<ReflectDestroy> (r2x) ;
			r3x->destroy (BoxHolder::create (raw ())->self ,1) ;
		}
		BoxHolder::create (raw ())->release () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	RFat<Unknown> unknown () const override {
		return BoxHolder::create (raw ())->unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue ;
	}

	LENGTH size () const override {
		if ((!exist ()))
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if ((!exist ()))
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

	VREF<Pointer> bt (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = RFat<ReflectTuple> (unknown ()) ;
		const auto r2x = fake.mBuffer + index * fake.mStep + r1x->offset_m2nd () ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = RFat<ReflectTuple> (unknown ()) ;
		const auto r2x = fake.mBuffer + index * fake.mStep + r1x->offset_m2nd () ;
		return Pointer::make (r2x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		assert (exist ()) ;
		const auto r1x = size () ;
		if (size_ <= r1x)
			return ;
		auto rax = Ref<RefBufferImplLayout> () ;
		const auto r2x = unknown () ;
		RefHolder::create (rax)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r2x ,size_) ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () ;
		const auto r5x = r4x * r1x ;
		BoxHolder::create (rax->mValue)->initialize (r2x) ;
		const auto r6x = address (BoxHolder::create (rax->mValue)->self) ;
		inline_memcpy (Pointer::make (r6x) ,self ,r5x) ;
		const auto r7x = r6x + r5x ;
		const auto r8x = RFat<ReflectCreate> (r2x) ;
		r8x->create (Pointer::make (r7x) ,size_ - r1x) ;
		swap (fake.mThis ,rax) ;
		fake.mThis->mType = RefBufferType::DESTROY ;
		BoxHolder::create (rax->mValue)->release () ;
		fake.mBuffer = r6x ;
		fake.mSize = size_ ;
		fake.mStep = r4x ;
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
	void initialize (CREF<Unknown> element) override {
		if (exist ())
			return ;
		fake = FarBufferLayout () ;
		RefHolder::create (fake.mBuffer)->initialize (element ,element ,0) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefHolder::create (fake.mBuffer)->initialize (element ,element ,0) ;
		fake.mIndex = NONE ;
		fake.mSize = size_ ;
		const auto r1x = RFat<ReflectSize> (element) ;
		fake.mStep = r1x->type_size () ;
	}

	void use_getter (CREF<FunctionLayout> getter) override {
		fake.mGetter = move (keep[TYPE<Function<CREF<INDEX> ,VREF<Pointer>>>::expr] (getter)) ;
	}

	void use_setter (CREF<FunctionLayout> setter) override {
		fake.mSetter = move (keep[TYPE<Function<CREF<INDEX> ,CREF<Pointer>>>::expr] (setter)) ;
	}

	BOOL exist () const override {
		return fake.mBuffer.exist () ;
	}

	RFat<Unknown> unknown () const override {
		return fake.mBuffer.unknown () ;
	}

	LENGTH size () const override {
		if ((!exist ()))
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if ((!exist ()))
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return fake.mBuffer.self ;
	}

	void update_sync (CREF<INDEX> index) {
		if (fake.mIndex == index)
			return ;
		refresh () ;
		fake.mGetter (index ,fake.mBuffer.self) ;
		fake.mIndex = index ;
	}

	void refresh () override {
		if (fake.mIndex == NONE)
			return ;
		fake.mSetter (fake.mIndex ,fake.mBuffer.self) ;
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
	void initialize (CREF<Unknown> element) override {
		if (exist ())
			return ;
		fake = AllocatorLayout () ;
		RefBufferHolder::create (fake.mAllocator)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		fake = AllocatorLayout () ;
		RefBufferHolder::create (fake.mAllocator)->initialize (element ,size_) ;
	}

	void destroy () override {
		if ((!exist ()))
			return ;
		if (raw ().mHolder == ZERO)
			return ;
		const auto r1x = inline_min (fake.mRest ,size ()) ;
		const auto r2x = RFat<ReflectElement> (unknown ())->unknown () ;
		const auto r3x = RFat<ReflectDestroy> (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (ptr (fake ,i).mNext != USED)
				continue ;
			r3x->destroy (fake.mAllocator.at (i) ,1) ;
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
		if ((!exist ()))
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
		return fake.mAllocator.bt (index) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return fake.mAllocator.bt (index) ;
	}

	imports VREF<AllocatorNode> ptr (VREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		return layout.mAllocator.bt (index) ;
	}

	imports CREF<AllocatorNode> ptr (CREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		return layout.mAllocator.bt (index) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = ptr (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectAssign> (unknown ()) ;
		r1x->xinit (fake.mAllocator.at (ret)) ;
		r1x->xmove (fake.mAllocator.at (ret) ,BoxHolder::create (item)->self) ;
		ptr (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectElement> (unknown ())->unknown () ;
		const auto r3x = RFat<ReflectDestroy> (r2x) ;
		r3x->destroy (fake.mAllocator.at (r1x) ,1) ;
		ptr (fake ,r1x).mNext = fake.mFree ;
		fake.mFree = r1x ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if ((!inline_between (index ,0 ,fake.mRest)))
			return FALSE ;
		const auto r1x = ptr (fake ,index).mNext ;
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
			ptr (fake ,fake.mFree).mNext = NONE ;
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