#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <mutex>
#include "csc_begin.h"

namespace CSC {
class OptionalImplHolder implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (RREF<BoxLayout> value) override {
		BoxHolder::create (fake.mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
	}

	void initialize (CREF<FLAG> code) override {
		fake.mCode = code ;
	}

	BOOL exist () const override {
		return BoxHolder::create (fake.mValue)->exist () ;
	}

	FLAG code () const override {
		return fake.mCode ;
	}

	CREF<Pointer> poll (CREF<Pointer> item) const override {
		if ifnot (exist ())
			return item ;
		return BoxHolder::create (fake.mValue)->self ;
	}

	void fetch (VREF<BoxLayout> item) const override {
		BoxHolder::create (item)->acquire (fake.mValue) ;
		BoxHolder::create (fake.mValue)->release () ;
	}

	void store (RREF<BoxLayout> item) const override {
		BoxHolder::create (fake.mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::create (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,address (that)) ;
}

exports CFat<OptionalHolder> OptionalHolder::create (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,address (that)) ;
}

struct FunctionImplLayout {
	DefaultUnknown mFunctor ;
	BoxLayout mValue ;
} ;

class FunctionImplHolder implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (CREF<Unknown> holder ,RREF<BoxLayout> value) override {
		const auto r1x = RefUnknownBinder<FunctionImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown () ,1) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		auto &&rax = unsafe_cast[TYPE<BoxLayout>::expr] (fake.mThis->mFunctor) ;
		rax.mHolder = unsafe_hold (holder) ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = CFat<ReflectInvoke> (fake.mThis->mFunctor) ;
		return r1x->rank () ;
	}

	void invoke (CREF<CaptureLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		auto &&rax = BoxHolder::create (fake.mThis->mValue)->self ;
		const auto r1x = CFat<ReflectInvoke> (fake.mThis->mFunctor) ;
		return r1x->invoke (rax ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,address (that)) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,address (that)) ;
}

struct AutoRefImplLayout {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (RREF<BoxLayout> value ,CREF<Clazz> clazz) override {
		const auto r1x = RefUnknownBinder<AutoRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown () ,1) ;
		fake.mThis->mClazz = clazz ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void initialize (RREF<AutoRefLayout> that ,CREF<Clazz> clazz) override {
		fake.mThis = move (that.mThis) ;
		fake.mPointer = ZERO ;
		if ifdo (TRUE) {
			if (fake.mThis->mClazz != clazz)
				discard ;
			fake.mPointer = that.mPointer ;
		}
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		return RefHolder::create (fake.mThis)->exist () ;
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
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,address (that)) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,address (that)) ;
}

struct SharedRefImplLayout {
	Pin<std::mutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> value) override {
		const auto r1x = RefUnknownBinder<SharedRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown () ,1) ;
		fake.mThis->mCounter = 0 ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			std::lock_guard<std::mutex> anonymous (that.mThis->mMutex) ;
			fake.mThis = that.mThis.share () ;
			fake.mThis->mCounter++ ;
			fake.mPointer = that.mPointer ;
		}
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		std::lock_guard<std::mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = --fake.mThis->mCounter ;
		if (r1x > 0)
			return ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		return RefHolder::create (fake.mThis)->exist () ;
	}

	FLAG counter () const override {
		if (fake.mThis == NULL)
			return 0 ;
		std::lock_guard<std::mutex> anonymous (fake.mThis->mMutex) ;
		return fake.mThis->mCounter ;
	}

	VREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	SharedRefLayout weak () const override {
		SharedRefLayout ret ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			std::lock_guard<std::mutex> anonymous (fake.mThis->mMutex) ;
			ret.mThis = fake.mThis.share () ;
		}
		ret.mPointer = ZERO ;
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::create (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,address (that)) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::create (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,address (that)) ;
}

struct UniqueRefImplLayout {
	Function<VREF<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplHolder implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> value ,RREF<Function<VREF<Pointer>>> dtor) override {
		const auto r1x = RefUnknownBinder<UniqueRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown () ,1) ;
		fake.mThis->mOwner = move (dtor) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		fake.mThis->mOwner (BoxHolder::create (fake.mThis->mValue)->self) ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		return RefHolder::create (fake.mThis)->exist () ;
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
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,address (that)) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,address (that)) ;
}

struct RefBufferImplLayout {
	DefaultUnknown mSize ;
} ;

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> item ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = RefUnknownBinder<RefBufferImplLayout> () ;
		RefHolder::create (fake.mBuffer)->initialize (r1x ,item ,size_) ;
		const auto r2x = CFat<ReflectSize> (item) ;
		fake.mSize = size_ ;
		fake.mStep = r2x->type_size () ;
	}

	void destroy () override {
		//@mark
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

	void resize (CREF<Unknown> item ,CREF<LENGTH> size_) override {
		if (size_ <= size ())
			return ;
		auto rax = RefLayout () ;
		const auto r1x = RefUnknownBinder<RefBufferImplLayout> () ;
		RefHolder::create (rax)->initialize (r1x ,item ,size_) ;
		const auto r2x = CFat<ReflectSize> (item) ;
		const auto r3x = r2x->type_size () ;
		const auto r4x = r3x * fake.mSize ;
		if ifdo (TRUE) {
			if (r4x == 0)
				discard ;
			const auto r5x = (&RefHolder::create (rax)->self) ;
			const auto r6x = (&self) ;
			memcpy (r5x ,r6x ,r4x) ;
		}
		fake.mBuffer = move (rax) ;
		fake.mSize = size_ ;
		fake.mStep = r3x ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,address (that)) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,address (that)) ;
}

class AllocatorImplHolder implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> item ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (item ,size_) ;
	}

	void destroy () override {
		if ifnot (RefHolder::create (fake.mAllocator.mBuffer)->exist ())
			return ;
		const auto r1x = CFat<ReflectDestroy> (RefHolder::create (fake.mAllocator.mBuffer)->unknown ()) ;
		const auto r2x = operator_min (fake.mWidth ,size ()) ;
		for (auto &&i : iter (0 ,r2x)) {
			if (ptr (fake.mAllocator ,i).mNext != USED)
				continue ;
			r1x->destroy (RefBufferHolder::create (fake.mAllocator)->at (i) ,1) ;
		}
	}

	void clear () override {
		destroy () ;
		fake.mWidth = 0 ;
		fake.mLength = 0 ;
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

	imports VREF<AllocatorNode<int>> ptr (VREF<RefBufferLayout> layout ,CREF<INDEX> index) {
		//@mark
		//const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mBuffer)->unknown ()) ;
		const auto r1x = layout.mBuffer.mPointer + index * layout.mStep ;
		return Pointer::make (r1x) ;
	}

	imports CREF<AllocatorNode<int>> ptr (CREF<RefBufferLayout> layout ,CREF<INDEX> index) {
		//@mark
		//const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mBuffer)->unknown ()) ;
		const auto r1x = layout.mBuffer.mPointer + index * layout.mStep ;
		return Pointer::make (r1x) ;
	}

	INDEX alloc (CREF<Unknown> item) override {
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mWidth < size ())
					discard ;
				const auto r1x = operator_max (fake.mWidth * 2 ,256) ;
				resize (item ,r1x) ;
			}
			fake.mFree = fake.mWidth ;
			ptr (fake.mAllocator ,fake.mWidth).mNext = NONE ;
			fake.mWidth++ ;
		}
		INDEX ret = fake.mFree ;
		const auto r2x = CFat<ReflectCreate> (RefHolder::create (fake.mAllocator.mBuffer)->unknown ()) ;
		r2x->create (RefBufferHolder::create (fake.mAllocator)->at (ret) ,1) ;
		fake.mFree = ptr (fake.mAllocator ,ret).mNext ;
		ptr (fake.mAllocator ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		assert (used (index)) ;
		const auto r1x = CFat<ReflectDestroy> (RefHolder::create (fake.mAllocator.mBuffer)->unknown ()) ;
		r1x->destroy (RefBufferHolder::create (fake.mAllocator)->at (index) ,1) ;
		ptr (fake.mAllocator ,index).mNext = fake.mFree ;
		fake.mFree = index ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		assert (operator_between (index ,0 ,size ())) ;
		if (index >= fake.mWidth)
			return FALSE ;
		const auto r1x = ptr (fake.mAllocator ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<Unknown> item ,CREF<LENGTH> size_) override {
		if (size_ <= size ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->resize (item ,size_) ;
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::create (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,address (that)) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::create (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,address (that)) ;
}
} ;