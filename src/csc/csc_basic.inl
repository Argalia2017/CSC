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
		BoxHolder::create (item)->acquire (fake.mValue) ;
		BoxHolder::create (fake.mValue)->release () ;
	}

	void store (RREF<BoxLayout> item) const override {
		BoxHolder::create (fake.mValue)->destroy () ;
		BoxHolder::create (fake.mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::create (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplement () ,Pointer::from (that)) ;
}

exports CFat<OptionalHolder> OptionalHolder::create (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplement () ,Pointer::from (that)) ;
}

struct FunctionPureLayout {
	BoxLayout mValue ;
} ;

class FunctionImplement implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = RefUnknownBinder<FunctionPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (item)->unknown () ,1) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}

	void initialize (CREF<FunctionLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	LENGTH rank () const override {
		if (fake.mThis == NULL)
			return 0 ;
		const auto r1x = BoxHolder::create (fake.mThis->mValue) ;
		const auto r2x = RFat<ReflectInvoke> (r1x->unknown ()) ;
		return r2x->rank () ;
	}

	void invoke (CREF<CaptureLayout> params) const override {
		if (fake.mThis == NULL)
			return ;
		const auto r1x = BoxHolder::create (fake.mThis->mValue) ;
		const auto r2x = RFat<ReflectInvoke> (r1x->unknown ()) ;
		return r2x->invoke (r1x->self ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::create (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplement () ,Pointer::from (that)) ;
}

exports CFat<FunctionHolder> FunctionHolder::create (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplement () ,Pointer::from (that)) ;
}

struct AutoRefPureLayout {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplement implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		const auto r1x = RefUnknownBinder<AutoRefPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,holder ,1) ;
		ClazzHolder::create (fake.mThis->mClazz)->initialize (holder) ;
		BoxHolder::create (fake.mThis->mValue)->initialize (holder) ;
	}

	void initialize (RREF<AutoRefLayout> that ,CREF<Clazz> clazz) override {
		fake.mThis = move (that.mThis) ;
		if ifdo (TRUE) {
			if (fake.mThis == NULL)
				discard ;
			if (fake.mThis->mClazz != clazz)
				discard ;
			fake.mPointer = that.mPointer ;
		}
	}

	void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz) override {
		const auto r1x = RefUnknownBinder<AutoRefPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (item)->unknown () ,1) ;
		fake.mThis->mClazz = clazz ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
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
	return VFat<AutoRefHolder> (AutoRefImplement () ,Pointer::from (that)) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::create (CREF<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplement () ,Pointer::from (that)) ;
}

struct SharedRefPureLayout {
	Pin<std::mutex> mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplement implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = RefUnknownBinder<SharedRefPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (item)->unknown () ,1) ;
		fake.mThis->mCounter = 0 ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mThis->mCounter++ ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void initialize (RREF<SharedRefLayout> that ,CREF<FLAG> offset) override {
		fake.mThis = move (that.mThis) ;
		fake.mPointer = that.mPointer ;
		if ifdo (TRUE) {
			if (fake.mPointer != ZERO)
				discard ;
			fake.mPointer += offset ;
		}
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

	void initialize (CREF<Unknown> holder ,VREF<Pointer> that) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = address (that) - inline_max (SIZE_OF<BoxLayout>::expr ,r1x->type_align ()) ;
		const auto r3x = inline_code (Pointer::make (r2x)) ;
		if (r3x != inline_code (holder))
			return ;
		const auto r4x = r2x - address (CPTR<SharedRefPureLayout> (NULL)->mValue) ;
		auto &&tmp1 = keep[TYPE<SharedRefPureLayout>::expr] (Pointer::make (r4x)) ;
		std::lock_guard<std::mutex> anonymous (tmp1.mMutex) ;
		if (tmp1.mCounter <= 0)
			return ;
		const auto r5x = r4x - inline_max (SIZE_OF<BoxLayout>::expr ,ALIGN_OF<SharedRefPureLayout>::expr) ;
		const auto r6x = inline_code (Pointer::make (r5x)) ;
		if (r6x != inline_code (RefUnknownBinder<SharedRefPureLayout> ()))
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
		fake.mPointer = address (that) ;
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
		return fake.mPointer != ZERO ;
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
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::create (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplement () ,Pointer::from (that)) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::create (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplement () ,Pointer::from (that)) ;
}

struct UniqueRefPureLayout {
	Function<VREF<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplement implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (RREF<BoxLayout> item ,RREF<Function<VREF<Pointer>>> dtor) override {
		const auto r1x = RefUnknownBinder<UniqueRefPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (item)->unknown () ,1) ;
		fake.mThis->mOwner = move (dtor) ;
		BoxHolder::create (fake.mThis->mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (fake.mThis->mValue)->self) ;
	}

	void destroy () override {
		if (fake.mThis == NULL)
			return ;
		fake.mThis->mOwner (BoxHolder::create (fake.mThis->mValue)->self) ;
		BoxHolder::create (fake.mThis->mValue)->destroy () ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
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
	return VFat<UniqueRefHolder> (UniqueRefImplement () ,Pointer::from (that)) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::create (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplement () ,Pointer::from (that)) ;
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
		const auto r1x = RefUnknownBinder<RefBufferPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,element ,size_) ;
		BoxHolder::create (fake.mThis->mValue)->initialize (element) ;
		fake.mBuffer = address (raw ()) ;
		fake.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (element) ;
		fake.mStep = r2x->type_size () ;
		const auto r3x = RFat<ReflectCreate> (element) ;
		r3x->create (self ,size_) ;
		fake.mThis->mWidth = size_ ;
	}

	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = RefUnknownBinder<RefBufferPureLayout> () ;
		RefHolder::create (fake.mThis)->initialize (r1x ,BoxHolder::create (item)->unknown () ,1) ;
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

	CREF<Unknown> unknown () const leftvalue override {
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
		const auto r1x = RFat<ReflectPair> (unknown ()) ;
		const auto r2x = fake.mBuffer + index * fake.mStep + r1x->offset () ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = RFat<ReflectPair> (unknown ()) ;
		const auto r2x = fake.mBuffer + index * fake.mStep + r1x->offset () ;
		return Pointer::make (r2x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		const auto r1x = size () ;
		if (size_ <= r1x)
			return ;
		auto rax = Ref<RefBufferPureLayout>::make () ;
		const auto r2x = RefUnknownBinder<RefBufferPureLayout> () ;
		RefHolder::create (rax)->initialize (r2x ,unknown () ,size_) ;
		const auto r3x = RFat<ReflectSize> (unknown ()) ;
		const auto r4x = r3x->type_size () ;
		const auto r5x = r4x * r1x ;
		BoxHolder::create (rax->mValue)->initialize (unknown ()) ;
		const auto r6x = address (BoxHolder::create (rax->mValue)->self) ;
		CoreTool::inline_memcpy (Pointer::make (r6x) ,self ,r5x) ;
		const auto r7x = r6x + r5x ;
		const auto r8x = RFat<ReflectCreate> (unknown ()) ;
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
	return VFat<RefBufferHolder> (RefBufferImplement () ,Pointer::from (that)) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::create (CREF<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplement () ,Pointer::from (that)) ;
}

class AllocatorImplement implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mAllocator.exist ())
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefBufferHolder::create (fake.mAllocator)->initialize (element ,size_) ;
	}

	void destroy () override {
		if ifnot (fake.mAllocator.exist ())
			return ;
		const auto r1x = inline_min (fake.mWidth ,size ()) ;
		const auto r2x = RFat<ReflectMove> (unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (ptr (fake ,i).mNext != USED)
				continue ;
			r2x->drop (fake.mAllocator.at (i)) ;
		}
	}

	BOOL exist () const override {
		return fake.mAllocator.exist () ;
	}

	CREF<Unknown> unknown () const leftvalue override {
		return fake.mAllocator.unknown () ;
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
		if ifnot (inline_between (index ,0 ,fake.mWidth))
			return FALSE ;
		const auto r1x = ptr (fake ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		if ifdo (TRUE) {
			if (fake.mFree != NONE)
				discard ;
			if ifdo (TRUE) {
				if (fake.mWidth < size ())
					discard ;
				const auto r1x = inline_max (fake.mWidth * 2 ,256) ;
				resize (r1x) ;
			}
			fake.mFree = fake.mWidth ;
			ptr (fake ,fake.mFree).mNext = NONE ;
			fake.mWidth++ ;
		}
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::create (VREF<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplement () ,Pointer::from (that)) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::create (CREF<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplement () ,Pointer::from (that)) ;
}
} ;