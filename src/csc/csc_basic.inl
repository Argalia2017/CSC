#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

#include "csc_end.h"
#include <cstdlib>
#include "csc_begin.h"

namespace CSC {
struct FunctionImplLayout {
	BoxLayout mHandle ;
	RefLayout mValue ;
} ;

class FunctionImplHolder implement Fat<FunctionHolder ,Ref<FunctionImplLayout>> {
public:
	void initialize (CREF<BoxLayout> value ,CREF<Unknown> holder) override {
		fake = Ref<FunctionImplLayout>::make () ;
		RefHolder::create (fake->mValue)->initialize (value) ;
		BoxHolder::create (fake->mHandle)->initialize (holder) ;
	}

	LENGTH rank () const override {
		return fake->mHandle.reflect (TYPE<ReflectInvoke>::expr)->rank () ;
	}

	void invoke (CREF<CaptureLayout> params) const override {
		auto &&rax = RefHolder::create (fake->mValue)->self ;
		return fake->mHandle.reflect (TYPE<ReflectInvoke>::expr)->invoke (rax ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<Ref<FunctionImplLayout>> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<Ref<FunctionImplLayout>> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefImplLayout {
	Clazz mClazz ;
	RefLayout mValue ;
} ;

class AutoRefImplHolder implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CREF<BoxLayout> value ,CREF<Clazz> clazz) override {
		//@mark
		fake.mThis = Ref<AutoRefImplLayout>::make () ;
		RefHolder::create (fake.mThis->mValue)->initialize (value) ;
		fake.mThis->mClazz = clazz ;
		fake.mPointer = fake.mThis->mValue.mPointer ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			fake.mThis->mValue = RefLayout () ;
		}
		fake.mPointer = ZERO ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	Clazz clazz () const override {
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
} ;

exports VFat<AutoRefHolder> AutoRefHolder::create (VREF<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

struct SharedRefImplLayout {
	LENGTH mCounter ;
	RefLayout mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (CREF<BoxLayout> value) override {
		//@mark
		fake.mThis = Ref<SharedRefImplLayout>::make () ;
		RefHolder::create (fake.mThis->mValue)->initialize (value) ;
		fake.mThis->mCounter = 0 ;
		fake.mPointer = fake.mThis->mValue.mPointer ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mPointer = that.mPointer ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			const auto r1x = --fake.mThis->mCounter ;
			if (r1x > 0)
				discard ;
			fake.mThis->mValue = RefLayout () ;
		}
		fake.mPointer = ZERO ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	FLAG counter () const override {
		return fake.mThis->mCounter ;
	}

	VREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	SharedRefLayout weak () const override {
		SharedRefLayout ret ;
		ret.mThis = fake.mThis.share () ;
		ret.mPointer = ZERO ;
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
	Function<VREF<Pointer>> mHandle ;
	RefLayout mValue ;
} ;

class UniqueRefImplHolder implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (CREF<BoxLayout> value) override {
		//@mark
		fake.mThis = Ref<UniqueRefImplLayout>::make () ;
		RefHolder::create (fake.mThis->mValue)->initialize (value) ;
		fake.mPointer = fake.mThis->mValue.mPointer ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			fake.mThis->mHandle (self) ;
			fake.mThis->mValue = RefLayout () ;
		}
		fake.mPointer = ZERO ;
	}

	void ownership (RREF<Function<VREF<Pointer>>> dtor) override {
		fake.mThis->mHandle = move (dtor) ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::create (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefHolder::create (fake.mBuffer)->initialize (holder ,size_) ;
		fake.mSize = size_ ;
		fake.mStep = fake.mBuffer.reflect (TYPE<ReflectSize>::expr)->type_size () ;
	}

	LENGTH size () const override {
		if ifnot (RefHolder::create (fake.mBuffer)->exist ())
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if ifnot (RefHolder::create (fake.mBuffer)->exist ())
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return RefHolder::create (fake.mBuffer)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefHolder::create (fake.mBuffer)->self ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer.mPointer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer.mPointer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		check (holder) ;
		if (size_ <= size ())
			return ;
		auto rax = RefLayout () ;
		RefHolder::create (rax)->initialize (holder ,size_) ;
		for (auto &&i : iter (0 ,fake.mSize)) {
			const auto r1x = fake.mBuffer.mPointer + i * fake.mStep ;
			const auto r2x = rax.mPointer + i * fake.mStep ;
			memcpy (csc_pointer_t (r2x) ,csc_pointer_t (r1x) ,fake.mStep) ;
		}
		swap (rax ,fake.mBuffer) ;
		fake.mSize = size_ ;
	}

	void check (CREF<Unknown> holder) {
		if (RefHolder::create (fake.mBuffer)->exist ())
			return ;
		auto &&rax = unsafe_cast[TYPE<BoxLayout>::expr] (holder) ;
		fake.mSize = 0 ;
		fake.mStep = rax.reflect (TYPE<ReflectSize>::expr)->type_size () ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

class RefAllocatorImplHolder implement Fat<RefAllocatorHolder ,RefAllocatorLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mAllocator)->initialize (holder ,size_) ;
		fake.mSize = 0 ;
		fake.mLength = 0 ;
		const auto r1x = fake.mAllocator.mBuffer.reflect (TYPE<ReflectSize>::expr) ;
		const auto r2x = fake.mAllocator.mStep - operator_max (r1x->type_align () ,SIZE_OF<FLAG>::expr) ;
		fake.mNextOffset = r2x ;
		fake.mFree = NONE ;
	}

	LENGTH size () const override {
		return RefBufferHolder::create (fake.mAllocator)->size () ;
	}

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mAllocator)->step () ;
	}

	LENGTH length () const override {
		if ifnot (RefHolder::create (fake.mAllocator.mBuffer)->exist ())
			return 0 ;
		return fake.mLength ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return RefBufferHolder::create (fake.mAllocator)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return RefBufferHolder::create (fake.mAllocator)->at (index) ;
	}

	void clear () override {
		const auto r1x = fake.mAllocator.mBuffer.reflect (TYPE<ReflectDrop>::expr) ;
		const auto r2x = operator_min (fake.mSize ,size ()) ;
		for (auto &&i : iter (0 ,fake.mSize)) {
			r1x->drop (RefBufferHolder::create (fake.mAllocator)->at (i)) ;
		}
		fake.mSize = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}

	INDEX alloc (CREF<Unknown> holder) override {
		check (holder) ;
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mSize < size ())
					discard ;
				const auto r1x = operator_max (size () * 2 ,256) ;
				resize (holder ,r1x) ;
			}
			fake.mFree = fake.mSize ;
			at_next (fake.mSize) = NONE ;
			fake.mSize++ ;
		}
		INDEX ret = fake.mFree ;
		fake.mFree = at_next (ret) ;
		at_next (ret) = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		assert (used (index)) ;
		const auto r1x = fake.mAllocator.mBuffer.reflect (TYPE<ReflectDrop>::expr) ;
		r1x->drop (RefBufferHolder::create (fake.mAllocator)->at (index)) ;
		at_next (index) = fake.mFree ;
		fake.mFree = index ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		assert (operator_between (index ,0 ,size ())) ;
		if (index >= fake.mSize)
			return FALSE ;
		const auto r1x = at_next (index) ;
		return r1x == USED ;
	}

	VREF<INDEX> at_next (CREF<INDEX> index) leftvalue {
		const auto r1x = fake.mAllocator.mBuffer.mPointer + index * fake.mAllocator.mStep ;
		const auto r2x = r1x + fake.mNextOffset ;
		return Pointer::make (r2x) ;
	}

	CREF<INDEX> at_next (CREF<INDEX> index) const leftvalue {
		const auto r1x = fake.mAllocator.mBuffer.mPointer + index * fake.mAllocator.mStep ;
		const auto r2x = r1x + fake.mNextOffset ;
		return Pointer::make (r2x) ;
	}

	void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		check (holder) ;
		if (size_ <= size ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->resize (holder ,size_) ;
	}

	void check (CREF<Unknown> holder) {
		if (RefHolder::create (fake.mAllocator.mBuffer)->exist ())
			return ;
		fake.mSize = 0 ;
		fake.mLength = 0 ;
		fake.mFree = NONE ;
	}
} ;

exports VFat<RefAllocatorHolder> RefAllocatorHolder::create (VREF<RefAllocatorLayout> that) {
	return VFat<RefAllocatorHolder> (RefAllocatorImplHolder () ,that) ;
}

exports CFat<RefAllocatorHolder> RefAllocatorHolder::create (CREF<RefAllocatorLayout> that) {
	return CFat<RefAllocatorHolder> (RefAllocatorImplHolder () ,that) ;
}
} ;