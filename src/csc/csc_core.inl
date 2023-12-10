#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

#include "csc_end.h"
#include <atomic>
#include "csc_begin.h"

namespace CSC {
class BoxImplHolder implement Fat<BoxHolder ,BoxLayout> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		const auto r1x = fake.mHolder ;
		assert (r1x != ZERO) ;
		return unsafe_cast[TYPE<Unknown>::expr] (r1x).unknown (uuid) ;
	}

	void initialize (CREF<Unknown> holder) override {
		fake.mHolder = unsafe_cast[TYPE<FLAG>::expr] (holder) ;
	}

	void destroy () override {
		fake.reflect (TYPE<ReflectDestroy>::expr)->destroy (self ,1) ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.reflect (TYPE<ReflectSize>::expr)->type_align () ;
		const auto r2x = operator_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.reflect (TYPE<ReflectSize>::expr)->type_align () ;
		const auto r2x = operator_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		fake.mHolder = that.mHolder ;
		if (fake.mHolder == ZERO)
			return ;
		const auto r1x = fake.reflect (TYPE<ReflectSize>::expr)->type_size () ;
		const auto r2x = fake.reflect (TYPE<ReflectSize>::expr)->type_align () ;
		const auto r3x = operator_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		const auto r4x = operator_alignas (address (that.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		memcpy (csc_pointer_t (r3x) ,csc_pointer_t (r4x) ,r1x) ;
	}

	void release () override {
		fake.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct RefLayoutData {
	LENGTH mCounter ;
	LENGTH mSize ;
	BoxLayout mThis ;
} ;

class RefImplHolder implement Fat<RefHolder ,RefLayout> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		const auto r1x = super ().mThis.mHolder ;
		assert (r1x != ZERO) ;
		return unsafe_cast[TYPE<Unknown>::expr] (r1x).unknown (uuid) ;
	}

	void initialize (CREF<BoxLayout> value) override {
		const auto r1x = value.reflect (TYPE<ReflectSize>::expr)->type_size () ;
		const auto r2x = value.reflect (TYPE<ReflectSize>::expr)->type_align () ;
		const auto r3x = operator_max (r2x - ALIGN_OF<FLAG>::expr ,0) ;
		const auto r4x = SIZE_OF<RefLayoutData>::expr + r3x + 1 * r1x ;
		fake.mHolder = HeapProc::instance ().alloc (r4x) ;
		super ().mCounter = 0 ;
		super ().mSize = 1 ;
		const auto r5x = fake.mHolder + SIZE_OF<RefLayoutData>::expr ;
		fake.mPointer = operator_alignas (r5x ,r2x) ;
		BoxHolder::create (super ().mThis)->acquire (value) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		auto &&rax = unsafe_cast[TYPE<BoxLayout>::expr] (holder) ;
		const auto r1x = rax.reflect (TYPE<ReflectSize>::expr)->type_size () ;
		const auto r2x = rax.reflect (TYPE<ReflectSize>::expr)->type_align () ;
		const auto r3x = operator_max (r2x - ALIGN_OF<FLAG>::expr ,0) ;
		const auto r4x = SIZE_OF<RefLayoutData>::expr + r3x + size_ * r1x ;
		fake.mHolder = HeapProc::instance ().alloc (r4x) ;
		super ().mCounter = 0 ;
		super ().mSize = size_ ;
		super ().mThis.mHolder = rax.mHolder ;
		const auto r5x = fake.mHolder + SIZE_OF<RefLayoutData>::expr ;
		fake.mPointer = operator_alignas (r5x ,r2x) ;
		rax.reflect (TYPE<ReflectCreate>::expr)->create (self ,super ().mSize) ;
	}

	void destroy () override {
		const auto r1x = --super ().mCounter ;
		if (r1x > 0)
			return ;
		super ().mThis.reflect (TYPE<ReflectDestroy>::expr)->destroy (self ,super ().mSize) ;
		const auto r2x = fake.mHolder ;
		HeapProc::instance ().free (r2x) ;
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

	RefLayout share () const override {
		RefLayout ret ;
		if ifdo (TRUE) {
			ret.mHolder = fake.mHolder ;
			ret.mPointer = fake.mPointer ;
			if (ret.mHolder == ZERO)
				discard ;
			super ().mCounter++ ;
		}
		return move (ret) ;
	}

	VREF<RefLayoutData> super () const leftvalue {
		return Pointer::make (fake.mHolder) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

class HeapProcLayout {
public:
	mutable std::atomic<LENGTH> mValue ;
} ;

class HeapProcImplHolder implement Fat<HeapProcHolder ,Ref<HeapProcLayout>> {
public:
	void initialize () override {
		auto &&rax = memorize ([&] () {
			return TEMP<HeapProcLayout> () ;
		}) ;
		const auto r1x = address (rax) ;
		fake = Ref<HeapProcLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH length () const override {
		return fake->mValue ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake->mValue += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake->mValue -= r2x ;
		operator delete (r1x) ;
	}
} ;

exports VFat<HeapProcHolder> HeapProcHolder::create (VREF<Ref<HeapProcLayout>> that) {
	return VFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

exports CFat<HeapProcHolder> HeapProcHolder::create (CREF<Ref<HeapProcLayout>> that) {
	return CFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

class SliceLayout implement SliceData {} ;

class SliceImplHolder implement Fat<SliceHolder ,Ref<SliceLayout>> {
public:
	void initialize (CREF<SliceData> data) override {
		const auto r1x = address (data) ;
		fake = Ref<SliceLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH size () const override {
		if (fake == NULL)
			return 0 ;
		return fake->mEnd - fake->mBegin ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		item = load (fake->mBegin + index * fake->mStep) ;
	}

	STRU32 load (CREF<FLAG> addr) const {
		if (fake->mStep == 1)
			return STRU8 (Pointer::make (addr)) ;
		if (fake->mStep == 2)
			return STRU16 (Pointer::make (addr)) ;
		if (fake->mStep == 4)
			return STRU32 (Pointer::make (addr)) ;
		assert (FALSE) ;
		return 0 ;
	}

	BOOL equal (CREF<Ref<SliceLayout>> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = load (fake->mBegin + i * fake->mStep) ;
			const auto r4x = load (that->mBegin + i * that->mStep) ;
			if (r3x != r4x)
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<Ref<SliceLayout>> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		const auto r3x = operator_min (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r4x = load (fake->mBegin + i * fake->mStep) ;
			const auto r5x = load (that->mBegin + i * that->mStep) ;
			const auto r6x = operator_compr (r4x ,r5x) ;
			if (r6x != ZERO)
				return r6x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = size () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = load (fake->mBegin + i * fake->mStep) ;
			operator_visit (visitor ,r2x) ;
		}
		visitor.end () ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::create (VREF<Ref<SliceLayout>> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::create (CREF<Ref<SliceLayout>> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

class ClazzLayout implement ClazzData {} ;

class ClazzImplHolder implement Fat<ClazzHolder ,Ref<ClazzLayout>> {
public:
	void initialize (CREF<ClazzData> data) override {
		const auto r1x = address (data) ;
		fake = Ref<ClazzLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH type_size () const override {
		if (fake == NULL)
			return 0 ;
		return fake->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (fake == NULL)
			return 0 ;
		return fake->mTypeAlign ;
	}

	FLAG type_uuid () const override {
		if (fake == NULL)
			return 0 ;
		return address (fake.self) ;
	}

	Slice<STR> type_name () const override {
		if (fake == NULL)
			return Slice<STR> () ;
		return fake->mTypeName ;
	}

	BOOL equal (CREF<Ref<ClazzLayout>> that) const override {
		if (type_uuid () == ClazzHolder::create (that)->type_uuid ())
			return TRUE ;
		return type_name () == ClazzHolder::create (that)->type_name () ;
	}

	FLAG compr (CREF<Ref<ClazzLayout>> that) const override {
		if (type_uuid () == ClazzHolder::create (that)->type_uuid ())
			return ZERO ;
		return operator_compr (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		operator_visit (visitor ,type_size ()) ;
		operator_visit (visitor ,type_align ()) ;
		operator_visit (visitor ,type_uuid ()) ;
		operator_visit (visitor ,type_name ()) ;
		visitor.end () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::create (VREF<Ref<ClazzLayout>> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

exports CFat<ClazzHolder> ClazzHolder::create (CREF<Ref<ClazzLayout>> that) {
	return CFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

class AutoImplHolder implement Fat<AutoHolder ,AutoLayout> {
public:
	void initialize (CREF<BoxLayout> value) override {
		BoxHolder::create (fake.mThis)->acquire (value) ;
	}

	BOOL exist () const override {
		return BoxHolder::create (fake.mThis)->exist () ;
	}

	void poll (VREF<BoxLayout> out) const override {
		const auto r1x = address (fake.mThis) ;
		auto rax = Ref<BoxLayout>::reference (Pointer::make (r1x)) ;
		BoxHolder::create (out)->acquire (rax.self) ;
		BoxHolder::create (rax.self)->release () ;
	}
} ;

exports VFat<AutoHolder> AutoHolder::create (VREF<AutoLayout> that) {
	return VFat<AutoHolder> (AutoImplHolder () ,that) ;
}

exports CFat<AutoHolder> AutoHolder::create (CREF<AutoLayout> that) {
	return CFat<AutoHolder> (AutoImplHolder () ,that) ;
}
} ;