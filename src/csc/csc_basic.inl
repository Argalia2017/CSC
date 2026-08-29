#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_basic.hpp"

namespace CSC {
class OptionalImplHolder final implement Fat<OptionalHolder ,OptionalLayout> {
public:
	void initialize (CR<Flag> code ,VR<BoxLayout> item) override {
		self.mPin.pinned (item) ;
		self.mCode = code ;
	}

	Bool exist () const override {
		return BoxHolder::hold (self.mPin.ref)->exist () ;
	}

	Flag code () const override {
		return self.mCode ;
	}

	void pull (VR<BoxLayout> item) const override {
		assume (exist ()) ;
		BoxHolder::hold (item)->acquire (self.mPin.ref) ;
		BoxHolder::hold (self.mPin.ref)->release () ;
	}

	void push (RR<BoxLayout> item) const override {
		if (exist ())
			return ;
		BoxHolder::hold (self.mPin.ref)->acquire (item) ;
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
	Ref<Length> mAlive ;
	BoxLayout mValue ;

public:
	void enter () {
		if ifdo (TRUE) {
			if (mAlive != NULL)
				discard ;
			mAlive = Ref<Length>::make (ZERO) ;
		}
		mAlive.ref++ ;
	}

	void leave () {
		if (mAlive == NULL)
			return ;
		mAlive.ref-- ;
	}
} ;

class FunctionImplHolder final implement Fat<FunctionHolder ,FunctionLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		self.mThis.intrusive (holder) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<FunctionTree> () ,holder ,1) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		BoxHolder::hold (raw ())->release () ;
	}

	void initialize (CR<FunctionLayout> that) override {
		if (that.mThis == NULL)
			return ;
		self.mThis = Ref<FunctionTree>::reference (that.mThis.ref) ;
		self.mThis.intrusive (that.mThis.unknown ()) ;
	}

	Unknown unknown () const {
		return self.mThis.unknown () ;
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
		if (self.mThis->mAlive != NULL)
			if (self.mThis->mAlive.ref == 0)
				return ;
		const auto r1x = RFat<ReflectInvoke> (unknown ()) ;
		return r1x->invoke (BoxHolder::hold (raw ())->ref ,params) ;
	}

	Scope until () const override {
		if (self.mThis == NULL)
			return Scope () ;
		return Scope (self.mThis.ref) ;
	}
} ;

exports VFat<FunctionHolder> FunctionHolder::hold (VR<FunctionLayout> that) {
	return VFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

exports CFat<FunctionHolder> FunctionHolder::hold (CR<FunctionLayout> that) {
	return CFat<FunctionHolder> (FunctionImplHolder () ,that) ;
}

struct AutoRefTree {
	Flag mExtend ;
	Clazz mClazz ;
	BoxLayout mValue ;
} ;

class AutoRefImplHolder final implement Fat<AutoRefHolder ,AutoRefLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		assert (!exist ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<AutoRefTree> () ,holder ,1) ;
		self.mThis->mExtend = inline_vptr (holder) ;
		self.mThis->mClazz = Clazz (holder) ;
		BoxHolder::hold (raw ())->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (raw ())->ref) ;
		BoxHolder::hold (raw ())->release () ;
	}
	
	void initialize (CR<AutoRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			const auto r1x = Unknown (that.mThis->mExtend) ;
			const auto r2x = r1x.reflect (ReflectClone::expr) ;
			assume (r2x != ZERO) ;
			initialize (r1x) ;
			const auto r3x = RFat<ReflectClone> (r1x) ;
			assume (r3x->is_noexcept ()) ;
			r3x->clone (ref ,BoxHolder::hold (that.mThis->mValue)->ref) ;
			BoxHolder::hold (raw ())->remake (r1x ,self.mLayout) ;
		}
	}

	void destroy () override {
		if (!exist ())
			return ;
		BoxHolder::hold (raw ())->destroy () ;
		self.mLayout = ZERO ;
	}

	Bool exist () const override {
		if (self.mThis == NULL)
			return FALSE ;
		return TRUE ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	VR<Pointer> ref_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	AutoRefLayout recast (CR<Unknown> extend) override {
		AutoRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (extend) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}

	Clazz clazz () const override {
		if (!exist ())
			return Clazz () ;
		return self.mThis->mClazz ;
	}

	VR<Pointer> rebind (CR<Clazz> clazz_) leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		const auto r1x = address (BoxHolder::hold (raw ())->ref) ;
		const auto r2x = address (ref) ;
		assume (r1x == r2x) ;
		return Pointer::from (self) ;
	}

	CR<Pointer> rebind (CR<Clazz> clazz_) const leftvalue override {
		assume (exist ()) ;
		assume (clazz () == clazz_) ;
		const auto r1x = address (BoxHolder::hold (raw ())->ref) ;
		const auto r2x = address (ref) ;
		assume (r1x == r2x) ;
		return Pointer::from (self) ;
	}
} ;

exports VFat<AutoRefHolder> AutoRefHolder::hold (VR<AutoRefLayout> that) {
	return VFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

exports CFat<AutoRefHolder> AutoRefHolder::hold (CR<AutoRefLayout> that) {
	return CFat<AutoRefHolder> (AutoRefImplHolder () ,that) ;
}

struct SharedRefTree {
	Flag mHeader ;
	Heap mMutex ;
	Length mCounter ;
	BoxLayout mValue ;
} ;

static constexpr auto SHAREDREF_HEADER = Flag (QUAD_ENDIAN) ;

class SharedRefImplHolder final implement Fat<SharedRefHolder ,SharedRefLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		assert (!exist ()) ;
		self.mThis.intrusive (RefUnknownBinder<SharedRefTree> ()) ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<SharedRefTree> () ,holder ,1) ;
		self.mThis->mHeader = SHAREDREF_HEADER ;
		self.mThis->mMutex = Heap::expr ;
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
				if (rax.mHeader == SHAREDREF_HEADER)
					return i ;
			}
			return ZERO ;
		}) ;
		const auto r6x = r2x - r3x * ALIGN_OF<SharedRefTree>::expr ;
		if ifdo (TRUE) {
			auto &&rax = keep[TYPE<SharedRefTree>::expr] (Pointer::make (r6x)) ;
			if (rax.mHeader != SHAREDREF_HEADER)
				discard ;
			const auto r7x = address (BoxHolder::hold (rax.mValue)->ref) ;
			if (layout < r7x)
				discard ;
			if (layout >= r7x + r1x->type_size ())
				discard ;
			Scope anonymous (rax.mMutex) ;
			RefHolder::hold (self.mThis)->initialize (REGISTER::expr ,r6x) ;
			self.mThis.intrusive (RefUnknownBinder<SharedRefTree> ()) ;
			if (self.mThis == NULL)
				discard ;
			self.mThis->mCounter++ ;
		}
	}

	void initialize (CR<SharedRefLayout> that) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (that.mThis == NULL)
				discard ;
			Scope anonymous (that.mThis->mMutex) ;
			self.mThis = Ref<SharedRefTree>::reference (that.mThis.ref) ;
			self.mThis.intrusive (RefUnknownBinder<SharedRefTree> ()) ;
			self.mLayout = that.mLayout ;
			self.mThis->mCounter++ ;
		}
	}

	void destroy () override {
		if (!exist ())
			return ;
		if ifdo (TRUE) {
			Scope anonymous (self.mThis->mMutex) ;
			if (is_weak ())
				discard ;
			const auto r1x = --self.mThis->mCounter ;
			if (r1x > 0)
				discard ;
			BoxHolder::hold (raw ())->destroy () ;
			self.mThis->mHeader = ZERO ;
		}
		self.mLayout = ZERO ;
	}

	Bool exist () const override {
		if (self.mThis == NULL)
			return FALSE ;
		if (self.mThis->mHeader != SHAREDREF_HEADER)
			return FALSE ;
		return TRUE ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return self.mThis->mValue ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return self.mThis->mValue ;
	}

	VR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	SharedRefLayout recast (CR<Unknown> extend) override {
		SharedRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (extend) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}

	Length counter () const override {
		if (!exist ())
			return 0 ;
		Scope anonymous (self.mThis->mMutex) ;
		return self.mThis->mCounter ;
	}

	Bool is_weak () const override {
		auto &&rax = keep[TYPE<RefLayout>::expr] (self.mThis) ;
		return rax.mExtend == ORDINARY::expr ;
	}

	SharedRefLayout weak () const override {
		SharedRefLayout ret ;
		if ifdo (TRUE) {
			if (self.mThis == NULL)
				discard ;
			ret.mThis = Ref<SharedRefTree>::reference (self.mThis.ref) ;
			ret.mThis.intrusive (RefUnknownBinder<SharedRefTree> ()) ;
			ret.mThis.reveal () ;
			ret.mLayout = self.mLayout ;
		}
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
	Pin<UniqueRefTree> mPin ;
	Bool mUnique ;
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
		self.mThis->mUnique = FALSE ;
		self.mThis->mOwner = owner ;
	}

	void initialize (CR<UniqueRefLayout> that) override {
		if (that.mThis == NULL)
			return ;
		self.mThis = Ref<UniqueRefTree>::reference (that.mThis.ref) ;
		self.mThis.intrusive (that.mThis.unknown ()) ;
		self.mLayout = that.mLayout ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if ifdo (TRUE) {
			if (!self.mThis.exclusive ())
				discard ;
			if (!BoxHolder::hold (raw ())->exist ())
				discard ;
			self.mThis->mOwner (BoxHolder::hold (raw ())->ref) ;
		}
		self.mLayout = ZERO ;
	}

	Bool exist () const override {
		if (self.mThis == NULL)
			return FALSE ;
		return TRUE ;
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

	UniqueRefLayout recast (CR<Unknown> extend) override {
		UniqueRefLayout ret ;
		ret.mThis = move (self.mThis) ;
		const auto r1x = RFat<ReflectRecast> (extend) ;
		ret.mLayout = r1x->recast (self.mLayout) ;
		return move (ret) ;
	}

	RefLayout borrow () const leftvalue override {
		assert (exist ()) ;
		assume (!self.mThis->mUnique) ;
		self.mThis->mPin->mUnique = TRUE ;
		return Ref<Pointer>::reference (Pointer::make (self.mLayout)) ;
	}
} ;

exports VFat<UniqueRefHolder> UniqueRefHolder::hold (VR<UniqueRefLayout> that) {
	return VFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

exports CFat<UniqueRefHolder> UniqueRefHolder::hold (CR<UniqueRefLayout> that) {
	return CFat<UniqueRefHolder> (UniqueRefImplHolder () ,that) ;
}

struct RefBufferTree {
	Length mMinCapacity ;
	Length mMaxCapacity ;
	BoxLayout mValue ;
} ;

class RefBufferImplHolder final implement Fat<RefBufferHolder ,RefBufferLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		self.mThis.intrusive (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		assert (!exist ()) ;
		self.mThis.intrusive (holder) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (size_ <= 0)
				discard ;
			const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
			RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r1x ,size_) ;
			BoxHolder::hold (raw ())->initialize (r1x) ;
			self.mBuffer = address (BoxHolder::hold (raw ())->ref) ;
			self.mSize = size_ ;
			const auto r2x = RFat<ReflectSize> (r1x) ;
			self.mStep = r2x->type_size () ;
			const auto r3x = RFat<ReflectCreate> (r1x) ;
			r3x->create (ref ,size_) ;
			self.mThis->mMinCapacity = 0 ;
			self.mThis->mMaxCapacity = size_ ;
		}
		if ifdo (act) {
			self.mBuffer = ZERO ;
			self.mSize = 0 ;
			self.mStep = 0 ;
		}
	}

	void initialize (CR<Unknown> holder ,CR<Slice> buffer ,RR<BoxLayout> item) override {
		assert (!exist ()) ;
		self.mThis.intrusive (holder) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r1x ,1) ;
		BoxHolder::hold (raw ())->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		self.mBuffer = buffer.offset (0) ;
		self.mSize = buffer.size () ;
		self.mStep = buffer.step () ;
		self.mThis->mMinCapacity = 0 ;
		self.mThis->mMaxCapacity = 0 ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		if ifdo (TRUE) {
			if (self.mThis->mMaxCapacity <= 0)
				discard ;
			const auto r1x = RFat<ReflectElement> (unknown ())->element () ;
			const auto r2x = RFat<ReflectDestroy> (r1x) ;
			r2x->destroy (ref ,self.mThis->mMaxCapacity) ;
			BoxHolder::hold (raw ())->release () ;
		}
		self.mBuffer = ZERO ;
		self.mSize = 0 ;
		self.mStep = 0 ;
	}

	Bool exist () const override {
		if (self.mThis == NULL)
			return FALSE ;
		return TRUE ;
	}

	Bool fixed () const override {
		if (!exist ())
			return FALSE ;
		if (self.mThis->mMinCapacity != 0)
			return FALSE ;
		if (self.mThis->mMaxCapacity != 0)
			return FALSE ;
		return TRUE ;
	}

	Unknown unknown () const override {
		return self.mThis.unknown () ;
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
		assert (inline_mid (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (inline_mid (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	Length min_resize () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mMinCapacity ;
	}

	Length max_resize () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mMaxCapacity ;
	}

	void resize (CR<Length> size_) override {
		const auto r1x = inline_max (size_ ,0) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (exist ())
				discard ;
			initialize (unknown () ,r1x) ;
		}
		if (r1x == size ())
			return ;
		if ifdo (act) {
			if (r1x >= min_resize ())
				discard ;
			assert (FALSE) ;
		}
		if ifdo (act) {
			if (r1x <= max_resize ())
				discard ;
			assume (!fixed ()) ;
			assume (self.mThis.exclusive ()) ;
			resize_real (r1x) ;
		}
		if ifdo (act) {
			self.mSize = r1x ;
		}
	}

	void resize_real (CR<Length> size_) {
		const auto r1x = size_ ;
		auto rax = RefBufferLayout () ;
		rax.mThis.intrusive (unknown ()) ;
		const auto r2x = RFat<ReflectElement> (unknown ())->element () ;
		const auto r3x = RFat<ReflectSize> (r2x) ;
		const auto r4x = r3x->type_size () ;
		const auto r5x = size () ;
		const auto r6x = step () ;
		const auto r7x = r5x * r6x / r4x ;
		const auto r8x = r1x * r6x / r4x ;
		assert (r7x * r4x == r5x * r6x) ;
		assert (r8x * r4x == r1x * r6x) ;
		RefHolder::hold (rax.mThis)->initialize (RefUnknownBinder<RefBufferTree> () ,r2x ,r8x) ;
		BoxHolder::hold (rax.mThis->mValue)->initialize (r2x) ;
		rax.mBuffer = address (BoxHolder::hold (rax.mThis->mValue)->ref) ;
		rax.mSize = r1x ;
		rax.mStep = r6x ;
		const auto r9x = r7x * r4x ;
		inline_memcpy (Pointer::make (rax.mBuffer) ,ref ,r9x) ;
		inline_memset (ref ,r9x) ;
		const auto r10x = RFat<ReflectCreate> (r2x) ;
		const auto r11x = rax.mBuffer + r9x ;
		r10x->create (Pointer::make (r11x) ,r8x - r7x) ;
		rax.mThis->mMinCapacity = min_resize () ;
		rax.mThis->mMaxCapacity = r8x ;
		swap (self ,rax) ;
	}
} ;

exports VFat<RefBufferHolder> RefBufferHolder::hold (VR<RefBufferLayout> that) {
	return VFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

exports CFat<RefBufferHolder> RefBufferHolder::hold (CR<RefBufferLayout> that) {
	return CFat<RefBufferHolder> (RefBufferImplHolder () ,that) ;
}

struct FarBufferTree {
	Pin<FarBufferTree> mPin ;
	Index mIndex ;
	Index mCheck ;
	Function<CR<Index> ,VR<Pointer>> mGetter ;
	Function<CR<Index> ,CR<Pointer>> mSetter ;
	BoxLayout mValue ;
} ;

class FarBufferImplHolder final implement Fat<FarBufferHolder ,FarBufferLayout> {
private:
	using FARBUFF_MAX_CHECK = ENUM<3> ;

public:
	void prepare (CR<Unknown> holder) override {
		self.mThis.intrusive (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		assert (!exist ()) ;
		self.mThis.intrusive (holder) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (size_ <= 0)
				discard ;
			const auto r1x = RFat<ReflectElement> (holder)->element () ;
			RefHolder::hold (self.mThis)->initialize (RefUnknownBinder<FarBufferTree> () ,r1x ,FARBUFF_MAX_CHECK::expr) ;
			BoxHolder::hold (raw ())->initialize (r1x) ;
			self.mBuffer = address (BoxHolder::hold (raw ())->ref) ;
			self.mSize = size_ ;
			const auto r2x = RFat<ReflectSize> (r1x) ;
			self.mStep = r2x->type_size () ;
			self.mThis->mIndex = NONE ;
			self.mThis->mCheck = 0 ;
			const auto r3x = RFat<ReflectCreate> (r1x) ;
			r3x->create (ref ,FARBUFF_MAX_CHECK::expr) ;
		}
		if ifdo (act) {
			self.mBuffer = ZERO ;
			self.mSize = 0 ;
			self.mStep = 0 ;
		}
	}

	void use_getter (CR<Function<CR<Index> ,VR<Pointer>>> getter) override {
		if (self.mThis == NULL)
			return ;
		self.mThis->mGetter = getter ;
	}

	void use_setter (CR<Function<CR<Index> ,CR<Pointer>>> setter) override {
		if (self.mThis == NULL)
			return ;
		self.mThis->mSetter = setter ;
	}

	Bool exist () const override {
		if (self.mThis == NULL)
			return FALSE ;
		return TRUE ;
	}

	Unknown unknown () const override {
		return self.mThis.unknown () ;
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
		const auto r1x = self.mBuffer + self.mThis->mCheck * self.mStep ;
		return Pointer::make (r1x) ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		assert (inline_mid (index ,0 ,size ())) ;
		update_sync (index) ;
		return ref ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (inline_mid (index ,0 ,size ())) ;
		update_sync (index) ;
		return ref ;
	}

	void update_sync (CR<Index> index) const {
		if (self.mThis->mIndex == index)
			return ;
		refresh () ;
		self.mThis->mPin->mIndex = index ;
		self.mThis->mPin->mCheck++ ;
		replace (self.mThis->mPin->mCheck ,FARBUFF_MAX_CHECK::expr ,0) ;
		self.mThis->mGetter (index ,ref) ;
	}

	void refresh () const override {
		if (self.mThis->mIndex == NONE)
			return ;
		self.mThis->mSetter (self.mThis->mIndex ,ref) ;
		self.mThis->mPin->mIndex = NONE ;
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
		self.mLength = 0 ;
		self.mFree = NONE ;
	}

	void destroy () override {
		if (!exist ())
			return ;
		const auto r1x = min_resize () ;
		const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			if (ptr (self ,i).mNext != USED)
				continue ;
			r2x->destroy (self.mAllocator.at (i) ,1) ;
		}
		set_min_resize (0) ;
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
		check_exist () ;
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
		check_exist () ;
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
		const auto r1x = ptr (self ,index).mNext ;
		return r1x == USED ;
	}

	Length min_resize () const override {
		return self.mAllocator.min_resize () ;
	}

	Length max_resize () const override {
		return self.mAllocator.max_resize () ;
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
		const auto r1x = min_resize () ;
		if ifdo (TRUE) {
			if (r1x > 0)
				discard ;
			self.mFree = NONE ;
		}
		if (self.mFree != NONE)
			return ;
		if ifdo (TRUE) {
			if (r1x < size ())
				discard ;
			const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
			resize (r2x) ;
		}
		const auto r3x = inline_alignas (r1x + 1 ,ALLOCATOR_MIN_SIZE::expr) ;
		const auto r4x = inline_min (r3x ,size ()) ;
		Index ix = self.mFree ;
		for (auto &&i : range (r1x ,r4x)) {
			Index iy = r4x - 1 - i + r1x ;
			ptr (self ,iy).mNext = ix ;
			ix = iy ;
		}
		set_min_resize (r4x) ;
		self.mFree = ix ;
	}

	void set_min_resize (CR<Length> size_) {
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mAllocator) ;
		rax.mThis->mMinCapacity = size_ ;
	}
} ;

exports VFat<AllocatorHolder> AllocatorHolder::hold (VR<AllocatorLayout> that) {
	return VFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}

exports CFat<AllocatorHolder> AllocatorHolder::hold (CR<AllocatorLayout> that) {
	return CFat<AllocatorHolder> (AllocatorImplHolder () ,that) ;
}
} ;