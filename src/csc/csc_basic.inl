#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

#include "csc_end.h"
#include <mutex>
#include "csc_begin.h"

namespace CSC {
struct HeapMutexRoot {
	Box<std::recursive_mutex> mMutex ;
} ;

class HeapMutexImplHolder final implement Fat<HeapMutexHolder ,HeapMutexLayout> {
public:
	void initialize () override {
		root_ptr ().mMutex.remake () ;
	}

	static VREF<HeapMutexRoot> root_ptr () {
		static auto mInstance = HeapMutexRoot () ;
		return mInstance ;
	}

	void enter () const override {
		return root_ptr ().mMutex->lock () ;
	}

	void leave () const override {
		return root_ptr ().mMutex->unlock () ;
	}
} ;

exports CREF<HeapMutexLayout> HeapMutexHolder::expr_m () {
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
	void initialize (CREF<FLAG> code) override {
		self.mCode = code ;
	}

	BOOL exist () const override {
		return BoxHolder::hold (self.mValue)->exist () ;
	}

	FLAG code () const override {
		return self.mCode ;
	}

	static VREF<BoxLayout> ptr (CREF<OptionalLayout> that) {
		return Pointer::make (address (that.mValue)) ;
	}

	void get (VREF<BoxLayout> item) const override {
		assume (exist ()) ;
		BoxHolder::hold (item)->acquire (ptr (self)) ;
		BoxHolder::hold (ptr (self))->release () ;
	}

	void set (VREF<BoxLayout> item) const override {
		assume (!exist ()) ;
		BoxHolder::hold (ptr (self))->acquire (item) ;
		BoxHolder::hold (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::hold (VREF<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::hold (CREF<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

struct FunctionTree {
	FLAG mHolder ;
	BoxLayout mValue ;
} ;

class FunctionImplHolder final implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<FunctionTree> () ,holder ,1) ;
		self.mThis->mHolder = inline_vptr (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		BoxHolder::hold (raw ())->release () ;
	}

	void initialize (CREF<FunctionLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	LENGTH rank () const override {
		if (self.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (Unknown (self.mThis->mHolder)) ;
		return r1x->rank () ;
	}

	void invoke (CREF<WrapperLayout> params) const override {
		if (self.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (Unknown (self.mThis->mHolder)) ;
		return r1x->invoke (BoxHolder::hold (raw ())->ref ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::hold (VREF<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::hold (CREF<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefTree {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder final implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<AutoRefTree> () ,holder ,1) ;
		ClazzHolder::hold (self.mThis->mClazz)->initialize (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		const auto r1x = RFat<ReflectCreate> (holder) ;
		r1x->create (BoxHolder::hold (raw ())->ref ,1) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<Clazz> clazz_) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<AutoRefTree> () ,holder ,1) ;
		self.mThis->mClazz = clazz_ ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return self.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Clazz clazz () const override {
		if (!exist ())
			return Clazz () ;
		return self.mThis->mClazz ;
	}

	VREF<Pointer> ref_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	CREF<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	VREF<Pointer> rebind (CREF<Clazz> clazz_) leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (self) ;
	}

	CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (self) ;
	}

	AutoRefLayout recast (CREF<Unknown> simple) override {
		AutoRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
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

struct SharedRefTree {
	FLAG mHeader ;
	HeapMutex mMutex ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder final implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<SharedRefTree> () ,holder ,1) ;
		self.mThis->mHeader = SHADERREFIMPLLAYOUT_HEADER ;
		self.mThis->mMutex = HeapMutex::expr ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
		self.mThis->mCounter = 1 ;
		const auto r2x = address (self.mThis.ref) + SIZE_OF<SharedRefTree>::expr ;
		inline_memset (Pointer::make (r2x) ,self.mLayout - r2x) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<FLAG> layout) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = layout - SIZE_OF<SharedRefTree>::expr ;
		const auto r3x = invoke ([&] () {
			const auto r4x = r1x->type_align () / ALIGN_OF<SharedRefTree>::expr ;
			for (auto &&i : range (0 ,r4x)) {
				const auto r5x = r2x - i * ALIGN_OF<SharedRefTree>::expr ;
				auto &&rax = keep[TYPE<SharedRefTree>::expr] (Pointer::make (r5x)) ;
				if (rax.mHeader == SHADERREFIMPLLAYOUT_HEADER)
					return i ;
			}
			return ZERO ;
		}) ;
		const auto r6x = r2x - r3x * ALIGN_OF<SharedRefTree>::expr ;
		auto &&rax = keep[TYPE<SharedRefTree>::expr] (Pointer::make (r6x)) ;
		const auto r7x = address (BoxHolder::hold (rax.mValue)->ref) ;
		assert (r7x == layout) ;
		Scope<HeapMutex> anonymous (rax.mMutex) ;
		const auto r8x = rax.mCounter ;
		assert (r8x > 0) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<SharedRefTree> () ,r6x) ;
		self.mLayout = layout ;
		self.mThis->mCounter++ ;
	}

	void initialize (CREF<SharedRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			Scope<HeapMutex> anonymous (that.mThis->mMutex) ;
			self.mThis = that.mThis.share () ;
			self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
			self.mThis->mCounter++ ;
		}
	}

	void destroy () override {
		if (!exist ())
			return ;
		Scope<HeapMutex> anonymous (self.mThis->mMutex) ;
		const auto r1x = --self.mThis->mCounter ;
		if (r1x > 0)
			return ;
		BoxHolder::hold (raw ())->destroy () ;
	}

	BOOL exist () const override {
		return self.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	LENGTH counter () const override {
		if (!exist ())
			return 0 ;
		Scope<HeapMutex> anonymous (self.mThis->mMutex) ;
		return self.mThis->mCounter ;
	}

	VREF<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	SharedRefLayout recast (CREF<Unknown> simple) override {
		SharedRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::hold (VREF<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::hold (CREF<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefTree {
	Function<VREF<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplHolder final implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<Function<VREF<Pointer>>> owner) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<UniqueRefTree> () ,holder ,1) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
		self.mThis->mOwner = owner ;
	}

	void initialize (CREF<UniqueRefLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if (!self.mThis.exclusive ())
			return ;
		if ifdo (TRUE) {
			if (!BoxHolder::hold (raw ())->exist ())
				discard ;
			self.mThis->mOwner (BoxHolder::hold (raw ())->ref) ;
		}
		self.mThis->mOwner = Function<VREF<Pointer>> () ;
	}

	BOOL exist () const override {
		return self.mThis != NULL ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	UniqueRefLayout recast (CREF<Unknown> simple) override {
		UniqueRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::hold (VREF<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::hold (CREF<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferTree {
	LENGTH mCapacity ;
	BoxLayout mValue ;
} ;

class RefBufferImplHolder final implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		self.mHolder = inline_vptr (holder) ;
		self.mBuffer = ZERO ;
		self.mSize = 0 ;
		self.mStep = 0 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		if (size_ <= 0)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r1x ,size_) ;
		BoxHolder::hold (raw ())->initialize (r1x) ;
		self.mBuffer = address (BoxHolder::hold (raw ())->ref) ;
		self.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		self.mStep = r2x->type_size () ;
		const auto r3x = RFat<ReflectCreate> (r1x) ;
		r3x->create (ref ,size_) ;
		self.mThis->mCapacity = size_ ;
	}

	void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer) override {
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		self.mBuffer = buffer.mBuffer ;
		self.mSize = buffer.mSize ;
		self.mStep = buffer.mStep ;
	}

	void initialize (CREF<Unknown> holder ,CREF<SliceLayout> buffer ,RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		self.mBuffer = buffer.mBuffer ;
		self.mSize = buffer.mSize ;
		self.mStep = buffer.mStep ;
		self.mThis->mCapacity = USED ;
	}

	void destroy () override {
		if (self.mHolder == ZERO)
			return ;
		if ifdo (TRUE) {
			if (self.mThis == NULL)
				discard ;
			if (self.mThis->mCapacity <= 0)
				discard ;
			const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
			const auto r2x = RFat<ReflectDestroy> (r1x) ;
			r2x->destroy (ref ,self.mThis->mCapacity) ;
			BoxHolder::hold (raw ())->release () ;
		}
		self.mHolder = ZERO ;
	}

	BOOL exist () const override {
		return self.mHolder != ZERO ;
	}

	BOOL fixed () const override {
		if (self.mThis == NULL)
			return FALSE ;
		if (self.mThis->mCapacity != USED)
			return FALSE ;
		return TRUE ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (self.mHolder) ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return self.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return self.mStep ;
	}

	VREF<Pointer> ref_m () leftvalue override {
		return Pointer::make (self.mBuffer) ;
	}

	CREF<Pointer> ref_m () const leftvalue override {
		return Pointer::make (self.mBuffer) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CREF<LENGTH> size_) override {
		if (size_ == size ())
			return ;
		assert (!fixed ()) ;
		const auto r1x = inline_min (size_ ,size ()) ;
		auto rax = RefBufferLayout () ;
		rax.mHolder = self.mHolder ;
		const auto r2x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (rax.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r2x ,size_) ;
		BoxHolder::hold (rax.mThis->mValue)->initialize (r2x) ;
		rax.mBuffer = address (BoxHolder::hold (rax.mThis->mValue)->ref) ;
		rax.mSize = size_ ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		rax.mStep = r3x->type_size () ;
		const auto r4x = r3x->type_size () * r1x ;
		inline_memcpy (Pointer::make (rax.mBuffer) ,ref ,r4x) ;
		inline_memset (ref ,r4x) ;
		const auto r6x = rax.mBuffer + r4x ;
		const auto r7x = RFat<ReflectCreate> (r2x) ;
		r7x->create (Pointer::make (r6x) ,size_ - r1x) ;
		rax.mThis->mCapacity = size_ ;
		swap (self ,rax) ;
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
		RefHolder::hold (self.mThis)->initialize (r1x ,r1x ,0) ;
		self.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		self.mStep = r2x->type_size () ;
		self.mIndex = NONE ;
	}

	void use_getter (CREF<Function<CREF<INDEX> ,VREF<Pointer>>> getter) override {
		self.mGetter = getter ;
	}

	void use_setter (CREF<Function<CREF<INDEX> ,CREF<Pointer>>> setter) override {
		self.mSetter = setter ;
	}

	BOOL exist () const override {
		return self.mThis.exist () ;
	}

	Unknown unknown () const override {
		return self.mThis.unknown () ;
	}

	LENGTH size () const override {
		if (!exist ())
			return 0 ;
		return self.mSize ;
	}

	LENGTH step () const override {
		if (!exist ())
			return 0 ;
		return self.mStep ;
	}

	VREF<Pointer> ref_m () leftvalue {
		return self.mThis.ref ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return ref ;
	}

	void update_sync (CREF<INDEX> index) {
		if (self.mIndex == index)
			return ;
		refresh () ;
		self.mGetter (index ,ref) ;
		self.mIndex = index ;
	}

	void refresh () override {
		if (self.mIndex == NONE)
			return ;
		self.mSetter (self.mIndex ,ref) ;
		self.mIndex = NONE ;
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
		RefBufferHolder::hold (self.mAllocator)->prepare (holder) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		self.mOffset = r1x->tuple_m2nd () ;
		self.mWidth = 0 ;
		self.mLength = 0 ;
		self.mFree = NONE ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		RefBufferHolder::hold (self.mAllocator)->initialize (holder ,size_) ;
		const auto r1x = RFat<ReflectTuple> (holder) ;
		self.mOffset = r1x->tuple_m2nd () ;
		self.mWidth = 0 ;
		self.mLength = 0 ;
		self.mFree = NONE ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		const auto r1x = inline_min (self.mWidth ,size ()) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			if (ptr (self ,i).mNext != USED)
				continue ;
			r2x->destroy (self.mAllocator.at (i) ,1) ;
		}
	}

	BOOL exist () const override {
		return self.mAllocator.exist () ;
	}

	Unknown unknown () const override {
		return self.mAllocator.unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return self.mAllocator.raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return self.mAllocator.raw () ;
	}

	void clear () override {
		destroy () ;
		self.mWidth = 0 ;
		self.mLength = 0 ;
		self.mFree = NONE ;
	}

	LENGTH size () const override {
		return self.mAllocator.size () ;
	}

	LENGTH step () const override {
		return self.mAllocator.step () ;
	}

	LENGTH length () const override {
		if (!exist ())
			return 0 ;
		return self.mLength ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return self.mAllocator.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return self.mAllocator.at (index) ;
	}

	VREF<Pointer> bt (CREF<INDEX> index) leftvalue override {
		assert (used (index)) ;
		return Pointer::from (ptr (self ,index)) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue override {
		assert (used (index)) ;
		return Pointer::from (ptr (self ,index)) ;
	}

	static VREF<AllocatorNode> ptr (VREF<AllocatorLayout> that ,CREF<INDEX> index) {
		const auto r1x = address (that.mAllocator.at (index)) + that.mOffset ;
		return Pointer::make (r1x) ;
	}

	static CREF<AllocatorNode> ptr (CREF<AllocatorLayout> that ,CREF<INDEX> index) {
		const auto r1x = address (that.mAllocator.at (index)) + that.mOffset ;
		return Pointer::make (r1x) ;
	}

	INDEX alloc () override {
		check_resize () ;
		INDEX ret = self.mFree ;
		self.mFree = ptr (self ,ret).mNext ;
		const auto r1x = RFat<ReflectCreate> (unknown ()) ;
		r1x->create (self.mAllocator.at (ret) ,1) ;
		ptr (self ,ret).mNext = USED ;
		self.mLength++ ;
		return move (ret) ;
	}

	INDEX alloc (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ret = self.mFree ;
		self.mFree = ptr (self ,ret).mNext ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (self.mAllocator.at (ret) ,BoxHolder::hold (item)->ref ,r1x->type_size ()) ;
		BoxHolder::hold (item)->release () ;
		ptr (self ,ret).mNext = USED ;
		self.mLength++ ;
		return move (ret) ;
	}

	void free (CREF<INDEX> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		r2x->destroy (self.mAllocator.at (r1x) ,1) ;
		ptr (self ,r1x).mNext = self.mFree ;
		self.mFree = r1x ;
		self.mLength-- ;
	}

	BOOL used (CREF<INDEX> index) const override {
		if (!inline_between (index ,0 ,self.mWidth))
			return FALSE ;
		const auto r1x = ptr (self ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CREF<LENGTH> size_) override {
		RefBufferHolder::hold (self.mAllocator)->resize (size_) ;
	}

	void check_resize () {
		assert (exist ()) ;
		if ifdo (TRUE) {
			if (self.mWidth != 0)
				if (self.mFree != NONE)
					discard ;
			if ifdo (TRUE) {
				if (self.mWidth < size ())
					discard ;
				const auto r1x = inline_max (self.mWidth * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
				resize (r1x) ;
			}
			self.mFree = self.mWidth ;
			ptr (self ,self.mFree).mNext = NONE ;
			self.mWidth++ ;
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