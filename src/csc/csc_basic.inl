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

	imports VREF<BoxLayout> ptr (CREF<FLAG> pointer) {
		return Pointer::make (pointer) ;
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
		auto &&rax = ptr (address (fake.mValue)) ;
		BoxHolder::create (item)->acquire (rax) ;
		BoxHolder::create (rax)->release () ;
	}

	void store (RREF<BoxLayout> item) const override {
		auto &&rax = ptr (address (fake.mValue)) ;
		BoxHolder::create (rax)->acquire (item) ;
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
	void initialize (CREF<Unknown> holder ,RREF<BoxLayout> value) override {
		RefHolder::create (fake.mThis)->initialize (holder ,BoxHolder::create (value)->unknown ()) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = CFat<ReflectInvoke> (fake.mThis.unknown ()) ;
		return r1x->rank () ;
	}

	void invoke (CREF<CaptureLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		auto &&rax = BoxHolder::create (fake.mThis->mValue)->self ;
		const auto r1x = CFat<ReflectInvoke> (fake.mThis.unknown ()) ;
		return r1x->invoke (rax ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

template <class A>
class BoxScopeUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		return 0 ;
	}
} ;

struct AutoRefImplLayout {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (RREF<BoxLayout> value ,CREF<Clazz> clazz) override {
		const auto r1x = BoxScopeUnknownBinder<AutoRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown ()) ;
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
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

struct SharedRefImplLayout {
	Pin<std::mutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> value) override {
		const auto r1x = BoxScopeUnknownBinder<SharedRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown ()) ;
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
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::create (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefImplLayout {
	Function<VREF<Pointer>> mDtor ;
	BoxLayout mValue ;
} ;

class UniqueRefImplHolder implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> value ,RREF<Function<VREF<Pointer>>> dtor) override {
		const auto r1x = BoxScopeUnknownBinder<UniqueRefImplLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (value)->unknown ()) ;
		fake.mThis->mDtor = move (dtor) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		fake.mThis->mDtor (BoxHolder::create (fake.mThis->mValue)->self) ;
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
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

class RefBufferImplHolder implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefHolder::create (fake.mBuffer)->initialize (holder ,size_) ;
		const auto r1x = CFat<ReflectSize> (RefHolder::create (fake.mBuffer)->unknown ()) ;
		fake.mSize = size_ ;
		fake.mStep = r1x->type_size () ;
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
		if (size_ <= size ())
			return ;
		auto rax = RefLayout () ;
		RefHolder::create (rax)->initialize (holder ,size_) ;
		const auto r1x = CFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r2x * fake.mSize ;
		if ifdo (TRUE) {
			if (r3x == 0)
				discard ;
			auto &&rbx = RefHolder::create (rax)->self ;
			auto &&rcx = self ;
			memcpy ((&rbx) ,(&rcx) ,r3x) ;
		}
		fake.mBuffer = move (rax) ;
		fake.mSize = size_ ;
		fake.mStep = r2x ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

class AllocatorImplHolder implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (holder ,size_) ;
	}

	void destroy () override {
		if ifnot (RefHolder::create (fake.mAllocator.mBuffer)->exist ())
			return ;
		const auto r1x = CFat<ReflectRemake> (RefHolder::create (fake.mAllocator.mBuffer)->unknown ()) ;
		if (r1x->trivial ())
			return ;
		const auto r2x = operator_min (fake.mSize ,size ()) ;
		for (auto &&i : iter (0 ,r2x)) {
			if (at (fake.mAllocator ,i).mNext != USED)
				continue ;
			r1x->drop (RefBufferHolder::create (fake.mAllocator)->at (i)) ;
		}
	}

	void clear () override {
		destroy () ;
		fake.mSize = 0 ;
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

	imports VREF<AllocatorNodeLayout> at (VREF<RefBufferLayout> layout ,CREF<INDEX> index) {
		const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mBuffer)->unknown ()) ;
		const auto r2x = layout.mBuffer.mPointer + index * layout.mStep + r1x->offset (0) ;
		return Pointer::make (r2x) ;
	}

	imports CREF<AllocatorNodeLayout> at (CREF<RefBufferLayout> layout ,CREF<INDEX> index) {
		const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mBuffer)->unknown ()) ;
		const auto r2x = layout.mBuffer.mPointer + index * layout.mStep + r1x->offset (0) ;
		return Pointer::make (r2x) ;
	}

	INDEX alloc (CREF<Unknown> holder) override {
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mSize < size ())
					discard ;
				const auto r1x = operator_max (fake.mSize * 2 ,256) ;
				resize (holder ,r1x) ;
			}
			fake.mFree = fake.mSize ;
			at (fake.mAllocator ,fake.mSize).mNext = NONE ;
			fake.mSize++ ;
		}
		INDEX ret = fake.mFree ;
		fake.mFree = at (fake.mAllocator ,ret).mNext ;
		at (fake.mAllocator ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		assert (used (index)) ;
		const auto r1x = CFat<ReflectRemake> (RefHolder::create (fake.mAllocator.mBuffer)->unknown ()) ;
		r1x->drop (RefBufferHolder::create (fake.mAllocator)->at (index)) ;
		at (fake.mAllocator ,index).mNext = fake.mFree ;
		fake.mFree = index ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		assert (operator_between (index ,0 ,size ())) ;
		if (index >= fake.mSize)
			return FALSE ;
		const auto r1x = at (fake.mAllocator ,index) ;
		return r1x.mNext == USED ;
	}

	void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		if (size_ <= size ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->resize (holder ,size_) ;
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::create (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::create (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;