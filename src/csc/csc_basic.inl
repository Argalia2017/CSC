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

	static VR<HeapMutexRoot> root_ptr () {
		static auto mInstance = HeapMutexRoot () ;
		return mInstance ;
	}

	void enter () override {
		return root_ptr ().mMutex->lock () ;
	}

	void leave () override {
		return root_ptr ().mMutex->unlock () ;
	}
} ;

exports CR<HeapMutexLayout> HeapMutexHolder::expr_m () {
	return memorize ([&] () {
		HeapMutexLayout ret ;
		ret.mHolder = inline_vptr (HeapMutexImplHolder ()) ;
		HeapMutexHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HeapMutexHolder> HeapMutexHolder::hold (VR<HeapMutexLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapMutexHolder> (rax ,that) ;
}

exports CFat<HeapMutexHolder> HeapMutexHolder::hold (CR<HeapMutexLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapMutexImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapMutexHolder> (rax ,that) ;
}

class OptionalImplHolder final implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (CR<Flag> code) override {
		self.mCode = code ;
	}

	Bool exist () const override {
		return BoxHolder::hold (self.mValue)->exist () ;
	}

	Flag code () const override {
		return self.mCode ;
	}

	void get (VR<BoxLayout> item) const override {
		assume (exist ()) ;
		BoxHolder::hold (item)->acquire (self.mValue) ;
		BoxHolder::hold (self.mValue)->release () ;
	}

	void set (VR<BoxLayout> item) const override {
		assume (!exist ()) ;
		BoxHolder::hold (self.mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
	}
} ;

exports VFat<OptionalHolder> OptionalHolder::hold (VR<OptionalLayout> that) {
	return VFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

exports CFat<OptionalHolder> OptionalHolder::hold (CR<OptionalLayout> that) {
	return CFat<OptionalHolder> (OptionalImplHolder () ,that) ;
}

struct FunctionTree {
	Flag mHolder ;
	BoxLayout mValue ;
} ;

class FunctionImplHolder final implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<FunctionTree> () ,holder ,1) ;
		self.mThis->mHolder = inline_vptr (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		BoxHolder::hold (raw ())->release () ;
	}

	void initialize (CR<FunctionLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	Unknown unknown () const {
		return Unknown (self.mThis->mHolder) ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Length rank () const override {
		if (self.mThis == NULL)
			return 0 ;
		const auto r1x = RFat<ReflectInvoke> (unknown ()) ;
		return r1x->rank () ;
	}

	void invoke (CR<Wrapper<Pointer>> params) const override {
		if (self.mThis == NULL)
			return ;
		const auto r1x = RFat<ReflectInvoke> (unknown ()) ;
		return r1x->invoke (BoxHolder::hold (raw ())->ref ,params) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::hold (VR<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::hold (CR<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefTree {
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder final implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<AutoRefTree> () ,holder ,1) ;
		ClazzHolder::hold (self.mThis->mClazz)->initialize (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		const auto r1x = RFat<ReflectCreate> (holder) ;
		r1x->create (BoxHolder::hold (raw ())->ref ,1) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
	}

	void initialize (CR<Unknown> holder ,CR<Clazz> clazz_) override {
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

	Bool exist () const override {
		return self.mThis.exist () ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Clazz clazz () const override {
		if (!exist ())
			return Clazz () ;
		return self.mThis->mClazz ;
	}

	VR<Pointer> ref_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	VR<Pointer> rebind (CR<Clazz> clazz_) leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (self) ;
	}

	CR<Pointer> rebind (CR<Clazz> clazz_) const leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		return Pointer::from (self) ;
	}

	AutoRefLayout recast (CR<Unknown> simple) override {
		AutoRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}
} ;

exports VFat<AutoRefHolder> AutoRefHolder::hold (VR<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::hold (CR<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

#ifdef __CSC_CONFIG_VAL32__
static constexpr auto SHADERREFIMPLLAYOUT_HEADER = Flag (CHAR_ENDIAN) ;
#endif

#ifdef __CSC_CONFIG_VAL64__
static constexpr auto SHADERREFIMPLLAYOUT_HEADER = Flag (QUAD_ENDIAN) ;
#endif

struct SharedRefTree {
	Flag mHeader ;
	HeapMutex mMutex ;
	Length mCounter ;
	BoxLayout mValue ;
} ;

class SharedRefImplHolder final implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<SharedRefTree> () ,holder ,1) ;
		self.mThis->mHeader = SHADERREFIMPLLAYOUT_HEADER ;
		self.mThis->mMutex = HeapMutex::expr ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
		self.mThis->mCounter = 1 ;
		const auto r1x = address (self.mThis.ref) + SIZE_OF<SharedRefTree>::expr ;
		inline_memset (Pointer::make (r1x) ,self.mLayout - r1x) ;
	}

	void initialize (CR<Unknown> holder ,CR<Flag> layout) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = inline_alignas (layout ,ALIGN_OF<SharedRefTree>::expr) - SIZE_OF<SharedRefTree>::expr ;
		const auto r3x = invoke ([&] () {
			const auto r4x = (r1x->type_size () + r1x->type_align ()) / ALIGN_OF<SharedRefTree>::expr ;
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
		if (rax.mHeader != SHADERREFIMPLLAYOUT_HEADER)
			return ;
		const auto r7x = address (BoxHolder::hold (rax.mValue)->ref) ;
		if (layout < r7x)
			return ;
		if (layout >= r7x + r1x->type_size ())
			return ;
		Scope<HeapMutex> anonymous (rax.mMutex) ;
		const auto r8x = rax.mCounter ;
		if (r8x <= 0)
			return ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<SharedRefTree> () ,r6x) ;
		self.mLayout = layout ;
		self.mThis->mCounter++ ;
	}

	void initialize (CR<SharedRefLayout> that) override {
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
		self.mThis->mHeader = ZERO ;
	}

	Bool exist () const override {
		return self.mThis.exist () ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Length counter () const override {
		if (!exist ())
			return 0 ;
		Scope<HeapMutex> anonymous (self.mThis->mMutex) ;
		return self.mThis->mCounter ;
	}

	VR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	SharedRefLayout recast (CR<Unknown> simple) override {
		SharedRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}

	RefLayout weak () const override {
		RefLayout ret = self.mThis.share () ;
		ret.mLayout = self.mLayout ;
		return move (ret) ;
	}
} ;

exports VFat<SharedRefHolder> SharedRefHolder::hold (VR<SharedRefLayout> that) {
	return VFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

exports CFat<SharedRefHolder> SharedRefHolder::hold (CR<SharedRefLayout> that) {
	return CFat<SharedRefHolder> (SharedRefImplHolder () ,that) ;
}

struct UniqueRefTree {
	Function<VR<Pointer>> mOwner ;
	BoxLayout mValue ;
} ;

class UniqueRefImplHolder final implement Fat<UniqueRefHolder ,UniqueRefLayout> {
public:
	void initialize (CR<Unknown> holder ,CR<Function<VR<Pointer>>> owner) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<UniqueRefTree> () ,holder ,1) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
		self.mThis->mOwner = owner ;
	}

	void initialize (CR<UniqueRefLayout> that) override {
		self.mThis = that.mThis.share () ;
		self.mLayout = that.mLayout ;
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
	}

	Bool exist () const override {
		return self.mThis.exist () ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	UniqueRefLayout recast (CR<Unknown> simple) override {
		UniqueRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (simple) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::hold (VR<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::hold (CR<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferTree {
	Length mCapacity ;
	BoxLayout mValue ;
} ;

class RefBufferImplHolder final implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		self.mHolder = inline_vptr (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		assert (!exist ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (size_ <= 0)
				discard ;
			self.mHolder = inline_vptr (holder) ;
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
		if ifdo (act) {
			self.mHolder = inline_vptr (holder) ;
			self.mBuffer = ZERO ;
			self.mSize = 0 ;
			self.mStep = 0 ;
		}
	}

	void initialize (CR<Unknown> holder ,CR<SliceLayout> buffer ,RR<BoxLayout> item) override {
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
		if (!exist ())
			return ;
		if (self.mThis->mCapacity <= 0)
			return ;
		const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
		const auto r2x = RFat<ReflectDestroy> (r1x) ;
		r2x->destroy (ref ,self.mThis->mCapacity) ;
		BoxHolder::hold (raw ())->release () ;
	}

	Bool exist () const override {
		return self.mThis.exist () ;
	}

	Bool fixed () const override {
		if (!exist ())
			return FALSE ;
		if (self.mThis->mCapacity != USED)
			return FALSE ;
		return TRUE ;
	}

	Unknown unknown () const override {
		return Unknown (self.mHolder) ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Length size () const override {
		if (!exist ())
			return 0 ;
		return self.mSize ;
	}

	Length step () const override {
		if (!exist ())
			return 0 ;
		return self.mStep ;
	}

	VR<Pointer> ref_m () leftvalue override {
		return Pointer::make (self.mBuffer) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		return Pointer::make (self.mBuffer) ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	void resize (CR<Length> size_) override {
		check_exist () ;
		const auto r1x = inline_max (size_ ,0) ;
		if (r1x == size ())
			return ;
		assert (!fixed ()) ;
		const auto r2x = inline_min (r1x ,size ()) ;
		auto rax = RefBufferLayout () ;
		rax.mHolder = self.mHolder ;
		const auto r3x = RFat<ReflectElement> (unknown ())->element () ;
		RefHolder::hold (rax.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r3x ,r1x) ;
		BoxHolder::hold (rax.mThis->mValue)->initialize (r3x) ;
		rax.mBuffer = address (BoxHolder::hold (rax.mThis->mValue)->ref) ;
		rax.mSize = r1x ;
		const auto r4x = RFat<ReflectSize> (r3x) ;
		rax.mStep = r4x->type_size () ;
		const auto r5x = r4x->type_size () * r2x ;
		inline_memcpy (Pointer::make (rax.mBuffer) ,ref ,r5x) ;
		inline_memset (ref ,r5x) ;
		const auto r6x = rax.mBuffer + r5x ;
		const auto r7x = RFat<ReflectCreate> (r3x) ;
		r7x->create (Pointer::make (r6x) ,r1x - r2x) ;
		rax.mThis->mCapacity = r1x ;
		swap (self ,rax) ;
	}

	void check_exist () {
		if (exist ())
			return ;
		initialize (unknown () ,0) ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::hold (VR<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::hold (CR<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

struct FarBufferTree {
	Index mIndex ;
	Function<CR<Index> ,VR<Pointer>> mGetter ;
	Function<CR<Index> ,CR<Pointer>> mSetter ;
	BoxLayout mValue ;
} ;

class FarBufferImplHolder final implement Fat<FarBufferHolder ,FarBufferLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		self.mHolder = inline_vptr (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectElement> (holder)->element () ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<FarBufferTree> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->initialize (r1x) ;
		self.mBuffer = address (BoxHolder::hold (raw ())->ref) ;
		self.mSize = size_ ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		self.mStep = r2x->type_size () ;
		self.mThis->mIndex = NONE ;
	}

	void use_getter (CR<Function<CR<Index> ,VR<Pointer>>> getter) override {
		self.mThis->mGetter = getter ;
	}

	void use_setter (CR<Function<CR<Index> ,CR<Pointer>>> setter) override {
		self.mThis->mSetter = setter ;
	}

	Bool exist () const override {
		return self.mThis.exist () ;
	}

	Unknown unknown () const override {
		return Unknown (self.mHolder) ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	Length size () const override {
		if (!exist ())
			return 0 ;
		return self.mSize ;
	}

	Length step () const override {
		if (!exist ())
			return 0 ;
		return self.mStep ;
	}

	VR<Pointer> ref_m () const leftvalue {
		return Pointer::make (self.mBuffer) ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return ref ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (inline_between (index ,0 ,size ())) ;
		update_sync (index) ;
		return ref ;
	}

	void update_sync (CR<Index> index) const {
		if (self.mThis->mIndex == index)
			return ;
		refresh () ;
		self.mThis->mGetter (index ,ref) ;
		self.mThis->mIndex = index ;
	}

	void refresh () const override {
		if (self.mThis->mIndex == NONE)
			return ;
		self.mThis->mSetter (self.mThis->mIndex ,ref) ;
		self.mThis->mIndex = NONE ;
	}
} ;

exports VFat<FarBufferHolder> FarBufferHolder::hold (VR<FarBufferLayout> that) {
	return VFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

exports CFat<FarBufferHolder> FarBufferHolder::hold (CR<FarBufferLayout> that) {
	return CFat<FarBufferHolder> (FarBufferImplHolder () ,that) ;
}

class AllocatorImplHolder final implement Fat<AllocatorHolder ,AllocatorLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mAllocator)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
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

	Bool exist () const override {
		return self.mAllocator.exist () ;
	}

	Unknown unknown () const override {
		return self.mAllocator.unknown () ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mAllocator.raw () ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mAllocator.raw () ;
	}

	void clear () override {
		destroy () ;
		self.mWidth = 0 ;
		self.mLength = 0 ;
		self.mFree = NONE ;
	}

	Length size () const override {
		return self.mAllocator.size () ;
	}

	Length step () const override {
		return self.mAllocator.step () ;
	}

	Length length () const override {
		if (!exist ())
			return 0 ;
		return self.mLength ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		assert (used (index)) ;
		return self.mAllocator.at (index) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (used (index)) ;
		return self.mAllocator.at (index) ;
	}

	VR<Pointer> bt (CR<Index> index) leftvalue override {
		assert (used (index)) ;
		return Pointer::from (ptr (self ,index)) ;
	}

	CR<Pointer> bt (CR<Index> index) const leftvalue override {
		assert (used (index)) ;
		return Pointer::from (ptr (self ,index)) ;
	}

	static VR<AllocatorNode> ptr (VR<AllocatorLayout> that ,CR<Index> index) {
		const auto r1x = address (that.mAllocator.at (index)) + that.mOffset ;
		return Pointer::make (r1x) ;
	}

	static CR<AllocatorNode> ptr (CR<AllocatorLayout> that ,CR<Index> index) {
		const auto r1x = address (that.mAllocator.at (index)) + that.mOffset ;
		return Pointer::make (r1x) ;
	}

	Index alloc () override {
		check_resize () ;
		Index ret = self.mFree ;
		self.mFree = ptr (self ,ret).mNext ;
		const auto r1x = RFat<ReflectCreate> (unknown ()) ;
		r1x->create (self.mAllocator.at (ret) ,1) ;
		ptr (self ,ret).mNext = USED ;
		self.mLength++ ;
		return move (ret) ;
	}

	Index alloc (RR<BoxLayout> item) override {
		check_resize () ;
		Index ret = self.mFree ;
		self.mFree = ptr (self ,ret).mNext ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (self.mAllocator.at (ret) ,BoxHolder::hold (item)->ref ,r1x->type_size ()) ;
		BoxHolder::hold (item)->release () ;
		ptr (self ,ret).mNext = USED ;
		self.mLength++ ;
		return move (ret) ;
	}

	void free (CR<Index> index) override {
		const auto r1x = index ;
		assert (used (r1x)) ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		r2x->destroy (self.mAllocator.at (r1x) ,1) ;
		ptr (self ,r1x).mNext = self.mFree ;
		self.mFree = r1x ;
		self.mLength-- ;
	}

	Bool used (CR<Index> index) const override {
		if (!inline_between (index ,0 ,self.mWidth))
			return FALSE ;
		const auto r1x = ptr (self ,index).mNext ;
		return r1x == USED ;
	}

	void resize (CR<Length> size_) override {
		check_exist () ;
		RefBufferHolder::hold (self.mAllocator)->resize (size_) ;
	}

	void check_exist () {
		if (exist ())
			return ;
		initialize (unknown () ,0) ;
	}

	void check_resize () {
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

exports VFat<AllocatorHolder> AllocatorHolder::hold (VR<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::hold (CR<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;