﻿#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
struct HeapMutexImplLayout {
	Box<std::recursive_mutex> mMutex ;
} ;

class HeapMutexImplHead implement Pin<HeapMutexImplLayout> {
public:
	imports CREF<HeapMutexImplHead> instance () ;
} ;

exports CREF<HeapMutexImplHead> HeapMutexImplHead::instance () {
	return memorize ([&] () {
		return HeapMutexImplHead () ;
	}) ;
}

class HeapMutexImplHolder final implement Fat<HeapMutexHolder ,HeapMutexLayout> {
public:
	void initialize () override {
		ptr (fake).mMutex.remake () ;
	}

	static VREF<HeapMutexImplLayout> ptr (CREF<HeapMutexLayout> layout) {
		return HeapMutexImplHead::instance ().self ;
	}

	void enter () const override {
		return ptr (fake).mMutex->lock () ;
	}

	void leave () const override {
		return ptr (fake).mMutex->unlock () ;
	}
} ;

exports CREF<HeapMutexLayout> HeapMutexHolder::instance () {
	return memorize ([&] () {
		HeapMutexLayout ret ;
		ret.mHolder = inline_vptr (HeapMutexImplHolder ()) ;
		HeapMutexHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HeapMutexHolder> HeapMutexHolder::hold (VREF<HeapMutexLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapMutexHolder> (rax ,that) ;
}

exports CFat<HeapMutexHolder> HeapMutexHolder::hold (CREF<HeapMutexLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapMutexHolder> (rax ,that) ;
}

class OptionalImplHolder final implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		set (item) ;
	}

	void initialize (CREF<FLAG> code) override {
		fake.mCode = code ;
	}

	BOOL exist () const override {
		auto &&rax = keep[TYPE<BoxLayout>::expr] (fake.mValue.self) ;
		return BoxHolder::hold (rax)->exist () ;
	}

	FLAG code () const override {
		return fake.mCode ;
	}

	void get (VREF<BoxLayout> item) const override {
		assume (exist ()) ;
		auto &&rax = keep[TYPE<BoxLayout>::expr] (fake.mValue.self) ;
		BoxHolder::hold (item)->acquire (rax) ;
		BoxHolder::hold (rax)->release () ;
	}

	void set (VREF<BoxLayout> item) const override {
		assume (!exist ()) ;
		auto &&rax = keep[TYPE<BoxLayout>::expr] (fake.mValue.self) ;
		BoxHolder::hold (rax)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::hold (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::hold (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

struct FunctionImplLayout {
	Pin<BoxLayout> mValue ;
} ;

class FunctionImplHolder final implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<Unknown> holder) override {
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<FunctionImplLayout> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (raw ())->release () ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		BoxHolder::hold (item)->release () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return fake.mThis->mValue.self ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::hold (raw ())->unknown ()) ;
		return r1x->rank () ;
	}

	void invoke (CREF<WrapperLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (BoxHolder::hold (raw ())->unknown ()) ;
		return r1x->invoke (BoxHolder::hold (raw ())->self ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::hold (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::hold (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefImplLayout {
	Clazz mClazz ;
	Pin<BoxLayout> mValue ;
} ;

class AutoRefImplHolder final implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,holder ,1) ;
		auto &&rax = keep[TYPE<AutoRefImplLayout>::expr] (fake.mThis.pin ().self) ;
		ClazzHolder::hold (rax.mClazz)->initialize (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		const auto r1x = RFat<ReflectCreate> (holder) ;
		r1x->create (BoxHolder::hold (raw ())->self ,1) ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz_) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<AutoRefImplLayout> () ,r1x ,1) ;
		auto &&rax = keep[TYPE<AutoRefImplLayout>::expr] (fake.mThis.pin ().self) ;
		rax.mClazz = clazz_ ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
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

exports VFat<AutoRefHolder> AutoRefHolder::hold (VREF<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::hold (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

#ifdef __CSC_CONFIG_VAL32__
static constexpr auto SHADERREFIMPLLAYOUT_HEADER = FLAG (0X07654321) ;
#endif

#ifdef __CSC_CONFIG_VAL64__
static constexpr auto SHADERREFIMPLLAYOUT_HEADER = FLAG (0X0FDCEBA907654321) ;
#endif

struct SharedRefImplLayout {
	FLAG mHeader ;
	HeapMutex mMutex ;
	Pin<LENGTH> mCounter ;
	Pin<BoxLayout> mValue ;
} ;

class SharedRefImplHolder final implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<SharedRefImplLayout> () ,r1x ,1) ;
		auto &&rax = keep[TYPE<SharedRefImplLayout>::expr] (fake.mThis.pin ().self) ;
		rax.mHeader = SHADERREFIMPLLAYOUT_HEADER ;
		rax.mMutex = HeapMutex::instance () ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		increase (fake.mThis->mCounter) ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
		const auto r2x = address (rax) + SIZE_OF<SharedRefImplLayout>::expr ;
		inline_memset (Pointer::make (r2x) ,fake.mPointer - r2x) ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			Scope<HeapMutex> anonymous (that.mThis->mMutex) ;
			fake.mThis = that.mThis ;
			increase (fake.mThis->mCounter) ;
			fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
		}
	}

	void initialize (CREF<Unknown> holder ,CREF<FLAG> pointer) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = pointer - SIZE_OF<SharedRefImplLayout>::expr ;
		const auto r3x = invoke ([&] () {
			const auto r4x = r1x->type_align () / ALIGN_OF<SharedRefImplLayout>::expr ;
			for (auto &&i : iter (0 ,r4x)) {
				const auto r5x = r2x - i * ALIGN_OF<SharedRefImplLayout>::expr ;
				auto &&rax = keep[TYPE<SharedRefImplLayout>::expr] (Pointer::make (r5x)) ;
				if (rax.mHeader == SHADERREFIMPLLAYOUT_HEADER)
					return i ;
			}
			return ZERO ;
		}) ;
		const auto r6x = r2x - r3x * ALIGN_OF<SharedRefImplLayout>::expr ;
		auto &&rax = keep[TYPE<SharedRefImplLayout>::expr] (Pointer::make (r6x)) ;
		auto &&rbx = keep[TYPE<BoxLayout>::expr] (rax.mValue.self) ;
		const auto r7x = address (BoxHolder::hold (rbx)->self) ;
		assert (r7x == pointer) ;
		Scope<HeapMutex> anonymous (rax.mMutex) ;
		const auto r8x = unchange (rax.mCounter) ;
		assert (r8x > 0) ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<SharedRefImplLayout> () ,r6x) ;
		increase (fake.mThis->mCounter) ;
		fake.mPointer = pointer ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		Scope<HeapMutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = decrease (fake.mThis->mCounter) ;
		if (r1x > 0)
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	LENGTH unchange (CREF<Pin<LENGTH>> counter) const {
		LENGTH ret ;
		counter.get (ret) ;
		return move (ret) ;
	}

	LENGTH increase (CREF<Pin<LENGTH>> counter) const {
		LENGTH ret ;
		counter.get (ret) ;
		ret++ ;
		counter.set (ret) ;
		return move (ret) ;
	}

	LENGTH decrease (CREF<Pin<LENGTH>> counter) const {
		LENGTH ret ;
		counter.get (ret) ;
		ret-- ;
		counter.set (ret) ;
		return move (ret) ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	LENGTH counter () const override {
		if (!exist ())
			return 0 ;
		Scope<HeapMutex> anonymous (fake.mThis->mMutex) ;
		return unchange (fake.mThis->mCounter) ;
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

exports VFat<SharedRefHolder> SharedRefHolder::hold (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::hold (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefImplLayout {
	Function<VREF<Pointer>> mOwner ;
	Pin<UniqueRefLayout> mUpper ;
	Pin<BoxLayout> mValue ;
} ;

class UniqueRefImplHolder final implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<UniqueRefImplLayout> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mPointer = address (BoxHolder::hold (raw ())->self) ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		fake.mThis->mUpper.~Pin () ;
		if (!BoxHolder::hold (raw ())->exist ())
			return ;
		fake.mThis->mOwner (BoxHolder::hold (raw ())->self) ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	void use_owner (CREF<FunctionLayout> owner) override {
		auto &&rax = keep[TYPE<UniqueRefImplLayout>::expr] (fake.mThis.pin ().self) ;
		rax.mOwner = move (keep[TYPE<Function<VREF<Pointer>>>::expr] (owner)) ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	void depend (VREF<UniqueRefLayout> that) const override {
		auto &&rax = keep[TYPE<UniqueRefImplLayout>::expr] (that.mThis.pin ().self) ;
		auto rbx = UniqueRefLayout () ;
		rax.mUpper.get (rbx) ;
		assert (!rbx.mThis.exist ()) ;
		rbx.mThis = fake.mThis ;
		rbx.mPointer = fake.mPointer ;
		rax.mUpper.set (rbx) ;
	}

	UniqueRefLayout recast (CREF<Unknown> simple) override {
		UniqueRefLayout ret ;
		ret.mThis = move (fake.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mPointer = r1x->recast (fake.mPointer) ;
		return move (ret) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::hold (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::hold (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferImplLayout {
	LENGTH mCapacity ;
	Pin<BoxLayout> mValue ;
} ;

class RefBufferImplHolder final implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		fake.mHolder = inline_vptr (holder) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
		if (size_ <= 0)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,size_) ;
		BoxHolder::hold (raw ())->initialize (r1x) ;
		fake.mBuffer = address (BoxHolder::hold (raw ())->self) ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		fake.mStep = r2x->type_size () ;
		const auto r3x = RFat<ReflectCreate> (r1x) ;
		r3x->create (self ,size_) ;
		auto &&rax = keep[TYPE<RefBufferImplLayout>::expr] (fake.mThis.pin ().self) ;
		rax.mCapacity = size_ ;
	}

	void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
		fake.mBuffer = buffer.mBuffer ;
		fake.mSize = buffer.mSize ;
		fake.mStep = buffer.mStep ;
	}

	void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer ,RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (fake.mThis)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mBuffer = buffer.mBuffer ;
		fake.mSize = buffer.mSize ;
		fake.mStep = buffer.mStep ;
		auto &&rax = keep[TYPE<RefBufferImplLayout>::expr] (fake.mThis.pin ().self) ;
		rax.mCapacity = USED ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (fake.mThis == NULL)
			return ;
		if (fake.mThis->mCapacity == USED)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		const auto r2x = RFat<ReflectDestroy> (r1x) ;
		r2x->destroy (self ,fake.mThis->mCapacity) ;
		BoxHolder::hold (raw ())->release () ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	BOOL fixed () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mThis->mCapacity != USED)
			return FALSE ;
		return TRUE ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (fake.mHolder) ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return fake.mThis->mValue.self ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return fake.mThis->mValue.self ;
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
		if (size_ == size ())
			return ;
		assert (!fixed ()) ;
		const auto r1x = inline_min (size_ ,size ()) ;
		auto rax = Ref<RefBufferImplLayout> () ;
		const auto r2x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (rax)->initialize (RefUnknownBinder<RefBufferImplLayout> () ,r2x ,size_) ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () * r1x ;
		auto &&rbx = keep[TYPE<BoxLayout>::expr] (rax->mValue.self) ;
		BoxHolder::hold (rbx)->initialize (r2x) ;
		const auto r5x = address (BoxHolder::hold (rbx)->self) ;
		inline_memcpy (Pointer::make (r5x) ,self ,r4x) ;
		const auto r6x = r5x + r4x ;
		const auto r7x = RFat<ReflectCreate> (r2x) ;
		r7x->create (Pointer::make (r6x) ,size_ - r1x) ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			const auto r8x = address (self) + r4x ;
			const auto r9x = RFat<ReflectDestroy> (r2x) ;
			r9x->destroy (Pointer::make (r8x) ,size () - r1x) ;
			BoxHolder::hold (raw ())->release () ;
		}
		swap (fake.mThis ,rax) ;
		fake.mBuffer = r5x ;
		fake.mSize = size_ ;
		fake.mStep = r3x->type_size () ;
		auto &&rcx = keep[TYPE<RefBufferImplLayout>::expr] (fake.mThis.pin ().self) ;
		rcx.mCapacity = size_ ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::hold (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::hold (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

class FarBufferImplHolder final implement Fat<FarBufferHolder ,FarBufferLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectElement> (holder)->element () ;
		RefHolder::hold (fake.mThis)->initialize (r1x ,r1x ,0) ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		fake.mStep = r2x->type_size () ;
		fake.mIndex = NONE ;
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

	Unknown unknown () const override {
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

	VREF<Pointer> self_m () leftvalue {
		const auto r1x = address (fake.mThis.self) ;
		return Pointer::make (r1x) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return self ;
	}

	void update_sync (CREF<INDEX> index) {
		if (fake.mIndex == index)
			return ;
		refresh () ;
		fake.mGetter (index ,self) ;
		fake.mIndex = index ;
	}

	void refresh () override {
		if (fake.mIndex == NONE)
			return ;
		fake.mSetter (fake.mIndex ,self) ;
		fake.mGetter (fake.mIndex ,self) ;
	}
} ;

exports VFat<FarBufferHolder> FarBufferHolder::hold (VREF<FarBufferLayout> that) {
	return VFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

exports CFat<FarBufferHolder> FarBufferHolder::hold (CREF<FarBufferLayout> that) {
	return CFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

class AllocatorImplHolder final implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		RefBufferHolder::hold (fake.mAllocator)->prepare (holder) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mWidth = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		RefBufferHolder::hold (fake.mAllocator)->initialize (holder ,size_) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		fake.mOffset = r1x->tuple_m2nd () ;
		fake.mWidth = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		const auto r1x = inline_min (fake.mWidth ,size ()) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (ptr (fake ,i).mNext != USED)
				continue ;
			r2x->destroy (fake.mAllocator.at (i) ,1) ;
		}
	}

	BOOL exist () const override {
		return fake.mAllocator.exist () ;
	}

	Unknown unknown () const override {
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
		fake.mWidth = 0 ;
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
		return Pointer::from (ptr (fake ,index)) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return Pointer::from (ptr (fake ,index)) ;
	}

	static VREF<AllocatorNode> ptr (VREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	static CREF<AllocatorNode> ptr (CREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = address (layout.mAllocator.at (index)) + layout.mOffset ;
		return Pointer::make (r1x) ;
	}

	INDEX alloc () override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = ptr (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectCreate> (unknown ()) ;
		r1x->create (fake.mAllocator.at (ret) ,1) ;
		ptr (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = ptr (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (fake.mAllocator.at (ret) ,BoxHolder::hold (item)->self ,r1x->type_size ()) ;
		BoxHolder::hold (item)->release () ;
		ptr (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		r2x->destroy (fake.mAllocator.at (r1x) ,1) ;
		ptr (fake ,r1x).mNext = fake.mFree ;
		fake.mFree = r1x ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if (!inline_between (index ,0 ,fake.mWidth))
			return FALSE ;
		const auto r1x = ptr (fake ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::hold (fake.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		assert (exist ()) ;
		if ifdo (TRUE) {
			if (fake.mWidth != 0)
				if (fake.mFree != NONE)
					discard ;
			if ifdo (TRUE) {
				if (fake.mWidth < size ())
					discard ;
				const auto r1x = inline_max (fake.mWidth * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
				resize (r1x) ;
			}
			fake.mFree = fake.mWidth ;
			ptr (fake ,fake.mFree).mNext = NONE ;
			fake.mWidth++ ;
		}
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::hold (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::hold (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;