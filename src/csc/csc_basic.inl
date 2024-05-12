#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
class OptionalImplement implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		BoxHolder::create (fake.mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
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

	CREF<Pointer> fetch (CREF<Pointer> item) const leftvalue override {
		if ifnot (exist ())
			return item ;
		return BoxHolder::create (fake.mValue)->self ;
	}

	void poll (VREF<BoxLayout> item) const override {
		assume (exist ()) ;
		auto &&tmp = keep[TYPE<BoxLayout>::expr] (Pointer::make (address (fake.mValue))) ;
		BoxHolder::create (item)->acquire (tmp) ;
		BoxHolder::create (tmp)->release () ;
	}

	void store (RREF<BoxLayout> item) const override {
		auto &&tmp = keep[TYPE<BoxLayout>::expr] (Pointer::make (address (fake.mValue))) ;
		BoxHolder::create (tmp)->destroy () ;
		BoxHolder::create (tmp)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::create (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplement () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::create (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplement () ,that) ;
}

struct FunctionPureLayout {
	UnknownHolder mFunctor ;
	BoxLayout mValue ;
} ;

class FunctionImplement implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (RREF<BoxLayout> item ,CREF<Unknown> functor) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<FunctionPureLayout> () ,r1x ,1) ;
		fake.mThis->mFunctor = functor ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}

	void initialize (CREF<FunctionLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (fake.mThis->mFunctor) ;
		return r1x->rank () ;
	}

	void invoke (CREF<CaptureLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (fake.mThis->mFunctor) ;
		return r1x->invoke (BoxHolder::create (fake.mThis->mValue)->self ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplement () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplement () ,that) ;
}

struct AutoRefPureLayout {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplement implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (RREF<UnknownHolder> holder) override {
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<AutoRefPureLayout> () ,holder ,1) ;
		ClazzHolder::create (fake.mThis->mClazz)->initialize (holder) ;
		BoxHolder::create (fake.mThis->mValue)->initialize (holder) ;
		fake.mPointer = address (raw ()) ;
		const auto r1x = RFat<ReflectCreate> (holder) ;
		r1x->create (self ,1) ;
	}

	void initialize (RREF<AutoRefLayout> that ,CREF<Clazz> clazz) override {
		fake.mThis = move (that.mThis) ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			if (fake.mThis->mClazz != clazz)
				discard ;
			fake.mPointer = address (raw ()) ;
		}
	}

	void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<AutoRefPureLayout> () ,r1x ,1) ;
		fake.mThis->mClazz = clazz ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (raw ()) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mPointer == ZERO)
			return FALSE ;
		return TRUE ;
	}

	Clazz clazz () const override {
		return fake.mThis->mClazz ;
	}

	VREF<Pointer> raw () leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
	}

	CREF<Pointer> raw () const leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
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
	return VFat<AutoRefHolder> (AutoRefImplement () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplement () ,that) ;
}

struct SharedRefPureLayout {
	Pin<std::mutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplement implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<SharedRefPureLayout> () ,r1x ,1) ;
		fake.mThis->mCounter = 0 ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (raw ()) ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			std::lock_guard<std::mutex> anonymous (that.mThis->mMutex) ;
			fake.mThis = that.mThis.share () ;
			fake.mThis->mCounter++ ;
			fake.mPointer = address (raw ()) ;
		}
	}

	void initialize (RREF<SharedRefLayout> that ,CREF<FLAG> offset) override {
		assert (offset == ZERO) ;
		fake.mThis = move (that.mThis) ;
		fake.mPointer = that.mPointer ;
	}

	void initialize (CREF<Unknown> holder ,VREF<Pointer> pointer) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = address (pointer) - inline_max (SIZE_OF<BoxLayout>::expr ,r1x->type_align ()) ;
		const auto r3x = UnknownHolder (inline_code (Pointer::make (r2x))) ;
		if (r3x != holder)
			return ;
		const auto r4x = r2x - address (CPTR<SharedRefPureLayout> (NULL)->mValue) ;
		const auto r5x = r4x - inline_max (SIZE_OF<BoxLayout>::expr ,ALIGN_OF<SharedRefPureLayout>::expr) ;
		const auto r6x = UnknownHolder (inline_code (Pointer::make (r5x))) ;
		if (r6x != RefUnknownBinder<SharedRefPureLayout> ())
			return ;
		auto &&tmp1 = keep[TYPE<SharedRefPureLayout>::expr] (Pointer::make (r4x)) ;
		std::lock_guard<std::mutex> anonymous (tmp1.mMutex) ;
		if (tmp1.mCounter <= 0)
			return ;
		const auto r7x = r5x - address (CPTR<RefPureLayout> (NULL)->mValue) ;
		auto &&tmp2 = keep[TYPE<RefPureLayout>::expr] (Pointer::make (r7x)) ;
		if (tmp2.mCounter <= 0)
			return ;
		auto &&tmp3 = keep[TYPE<RefLayout>::expr] (fake.mThis) ;
		tmp3.mHolder = address (tmp2) ;
		tmp3.mPointer = address (tmp1) ;
		tmp2.mCounter++ ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (pointer) ;
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
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mPointer == ZERO)
			return FALSE ;
		return TRUE ;
	}

	FLAG counter () const override {
		if (fake.mThis == NULL)
			return 0 ;
		std::lock_guard<std::mutex> anonymous (fake.mThis->mMutex) ;
		return fake.mThis->mCounter ;
	}

	VREF<Pointer> raw () leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
	}

	CREF<Pointer> raw () const leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
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
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::create (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplement () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::create (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplement () ,that) ;
}

struct UniqueRefPureLayout {
	Function<VREF<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplement implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> item ,RREF<Function<VREF<Pointer>>> dtor) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<UniqueRefPureLayout> () ,r1x ,1) ;
		fake.mThis->mOwner = move (dtor) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (raw ()) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		fake.mThis->mOwner (raw ()) ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mPointer == ZERO)
			return FALSE ;
		return TRUE ;
	}

	VREF<Pointer> raw () leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
	}

	CREF<Pointer> raw () const leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::create (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplement () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplement () ,that) ;
}

struct RefBufferPureLayout {
	LENGTH mWidth ;
	BoxLayout mValue ;
} ;

class RefBufferImplement implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mThis != NULL)
			return ;
		fake.mThis = Ref<RefBufferPureLayout>::make () ;
		BoxHolder::create (fake.mThis->mValue)->initialize (element) ;
		fake.mThis->mWidth = 0 ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferPureLayout> () ,element ,size_) ;
		BoxHolder::create (fake.mThis->mValue)->initialize (element) ;
		fake.mBuffer = address (raw ()) ;
		fake.mSize = size_ ;
		const auto r1x = RFat<ReflectSize> (element) ;
		fake.mStep = r1x->type_size () ;
		const auto r2x = RFat<ReflectCreate> (element) ;
		r2x->create (self ,size_) ;
		fake.mThis->mWidth = size_ ;
	}

	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = BoxHolder::create (item)->unknown () ;
		RefHolder::create (fake.mThis)->initialize (RefUnknownBinder<RefBufferPureLayout> () ,r1x ,1) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mThis->mWidth = 1 ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
		r1x->destroy (raw () ,fake.mThis->mWidth) ;
		BoxHolder::create (fake.mThis->mValue)->release () ;
	}

	BOOL exist () const override {
		return fake.mThis != NULL ;
	}

	UnknownHolder unknown () const override {
		if (fake.mThis == NULL)
			return UnknownHolder () ;
		return BoxHolder::create (fake.mThis->mValue)->unknown () ;
	}

	LENGTH size () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mStep ;
	}

	VREF<Pointer> raw () leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
	}

	CREF<Pointer> raw () const leftvalue override {
		return BoxHolder::create (fake.mThis->mValue)->self ;
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
		const auto r1x = size () ;
		if (size_ <= r1x)
			return ;
		auto rax = Ref<RefBufferPureLayout> () ;
		const auto r2x = unknown () ;
		RefHolder::create (rax)->initialize (RefUnknownBinder<RefBufferPureLayout> () ,r2x ,size_) ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () ;
		const auto r5x = r4x * r1x ;
		BoxHolder::create (rax->mValue)->initialize (r2x) ;
		const auto r6x = address (BoxHolder::create (rax->mValue)->self) ;
		CoreProc::inline_memcpy (Pointer::make (r6x) ,self ,r5x) ;
		const auto r7x = r6x + r5x ;
		const auto r8x = RFat<ReflectCreate> (r2x) ;
		r8x->create (Pointer::make (r7x) ,size_ - r1x) ;
		swap (fake.mThis ,rax) ;
		fake.mThis->mWidth = size_ ;
		BoxHolder::create (rax->mValue)->release () ;
		fake.mBuffer = r6x ;
		fake.mSize = size_ ;
		fake.mStep = r4x ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::create (VREF<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplement () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplement () ,that) ;
}

class FarBufferImplement implement Fat<FarBufferHolder ,FarBufferLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		noop () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefHolder::create (fake.mBuffer)->initialize (element ,RefUnknownBinder<int> () ,0) ;
		fake.mIndex = NONE ;
		fake.mSize = size_ ;
		const auto r1x = RFat<ReflectSize> (element) ;
		fake.mStep = r1x->type_size () ;
	}

	void destroy () override {
		if (fake.mBuffer == NULL)
			return ;
		if ifdo (TRUE) {
			if (fake.mIndex == NONE)
				discard ;
			fake.mSetter (fake.mIndex ,fake.mBuffer.self) ;
		}
		fake.mIndex = NONE ;
		fake.mBuffer = NULL ;
	}

	void use_getter (RREF<FunctionLayout> getter) override {
		fake.mGetter = move (keep[TYPE<Function<CREF<INDEX> ,VREF<Pointer>>>::expr] (getter)) ;
	}

	void use_setter (RREF<FunctionLayout> setter) override {
		fake.mSetter = move (keep[TYPE<Function<CREF<INDEX> ,CREF<Pointer>>>::expr] (setter)) ;
	}

	BOOL exist () const override {
		return fake.mBuffer != NULL ;
	}

	UnknownHolder unknown () const override {
		return fake.mBuffer.unknown () ;
	}

	LENGTH size () const override {
		if (fake.mBuffer == NULL)
			return 0 ;
		return fake.mSize ;
	}

	LENGTH step () const override {
		if (fake.mBuffer == NULL)
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
		if ifdo (TRUE) {
			if (fake.mIndex == NONE)
				discard ;
			fake.mSetter (fake.mIndex ,fake.mBuffer.self) ;
		}
		fake.mGetter (index ,fake.mBuffer.self) ;
		fake.mIndex = index ;
	}

	void resize (CREF<LENGTH> size_) override {
		assert (FALSE) ;
	}
} ;

exports VFat<FarBufferHolder> FarBufferHolder::create (VREF<FarBufferLayout> that) {
	return VFat<FarBufferHolder> (FarBufferImplement () ,that) ;
}

exports CFat<FarBufferHolder> FarBufferHolder::create (CREF<FarBufferLayout> that) {
	return CFat<FarBufferHolder> (FarBufferImplement () ,that) ;
}

class AllocatorImplement implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mAllocator.exist ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mAllocator)->initialize (element ,size_) ;
	}

	void destroy () override {
		if ifnot (fake.mAllocator.exist ())
			return ;
		const auto r1x = inline_min (fake.mRest ,size ()) ;
		const auto r2x = RFat<ReflectMove> (unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (ptr (fake ,i).mNext != USED)
				continue ;
			r2x->drop (fake.mAllocator.at (i)) ;
		}
		fake.mRest = 0 ;
	}

	BOOL exist () const override {
		return fake.mAllocator.exist () ;
	}

	UnknownHolder unknown () const override {
		return fake.mAllocator.unknown () ;
	}

	void clear () override {
		destroy () ;
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
		if ifnot (fake.mAllocator.exist ())
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

	INDEX alloc () override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = ptr (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectMove> (unknown ()) ;
		r1x->init (fake.mAllocator.at (ret)) ;
		ptr (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = fake.mFree ;
		fake.mFree = ptr (fake ,ret).mNext ;
		const auto r1x = RFat<ReflectMove> (unknown ()) ;
		r1x->copy (fake.mAllocator.at (ret) ,BoxHolder::create (item)->self) ;
		BoxHolder::create (item)->release () ;
		ptr (fake ,ret).mNext = USED ;
		fake.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectMove> (unknown ()) ;
		r2x->drop (fake.mAllocator.at (r1x)) ;
		ptr (fake ,r1x).mNext = fake.mFree ;
		fake.mFree = r1x ;
		fake.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if ifnot (inline_between (index ,0 ,fake.mRest))
			return FALSE ;
		const auto r1x = ptr (fake ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		if ifdo (TRUE) {
			if (fake.mRest > 0)
				if (fake.mFree != NONE)
					discard ;
			if ifdo (TRUE) {
				if (fake.mRest < size ())
					discard ;
				const auto r1x = inline_max (fake.mRest * 2 ,256) ;
				resize (r1x) ;
			}
			fake.mFree = fake.mRest ;
			ptr (fake ,fake.mFree).mNext = NONE ;
			fake.mRest++ ;
		}
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::create (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplement () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::create (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplement () ,that) ;
}
} ;