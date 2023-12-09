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
	void initialize (CREF<Unknown> that) override {
		assert (FALSE) ;
	}

	void destroy () override {
		fake.destroy (1) ;
	}

	BOOL exist () const override {
		return thix.mHolder != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (thix.mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (thix.mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		thix.mHolder = that.mHolder ;
		if (thix.mHolder == ZERO)
			return ;
		const auto r1x = fake.type_size () ;
		const auto r2x = fake.type_align () ;
		const auto r3x = operator_alignas (address (thix.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		const auto r4x = operator_alignas (address (that.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		memcpy (csc_pointer_t (r3x) ,csc_pointer_t (r4x) ,r1x) ;
	}

	void release () override {
		thix.mHolder = ZERO ;
	}

	VREF<Unknown> fake_m () leftvalue {
		return Pointer::from (thix.mHolder) ;
	}

	CREF<Unknown> fake_m () const leftvalue {
		return Pointer::from (thix.mHolder) ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
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
		thix = Ref<HeapProcLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH length () const override {
		return thix->mValue ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		thix->mValue += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		thix->mValue -= r2x ;
		operator delete (r1x) ;
	}
} ;

exports VFat<HeapProcHolder> HeapProcHolder::create (VREF<Ref<HeapProcLayout>> that) {
	return VFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

exports CFat<HeapProcHolder> HeapProcHolder::create (CREF<Ref<HeapProcLayout>> that) {
	return CFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

struct RefLayoutData {
	LENGTH mCounter ;
	LENGTH mSize ;
	BoxLayout mHolder ;
} ;

class RefImplHolder implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) override {
		auto &&rax = unsafe_cast[TYPE<Unknown>::expr] (value.mHolder) ;
		const auto r1x = operator_max (rax.type_align () - ALIGN_OF<FLAG>::expr ,0) ;
		const auto r2x = SIZE_OF<RefLayoutData>::expr + r1x + size_ * rax.type_size () ;
		thix.mHolder = HeapProc::instance ().alloc (r2x) ;
		fake.mCounter = 0 ;
		fake.mSize = size_ ;
		BoxHolder::create (fake.mHolder)->acquire (value) ;
		const auto r3x = thix.mHolder + SIZE_OF<RefLayoutData>::expr ;
		thix.mPointer = operator_alignas (r3x ,rax.type_align ()) ;
	}

	void destroy () override {
		const auto r1x = --fake.mCounter ;
		if (r1x > 0)
			return ;
		auto &&rax = unsafe_cast[TYPE<Unknown>::expr] (fake.mHolder) ;
		rax.destroy (fake.mSize) ;
		const auto r2x = thix.mHolder ;
		HeapProc::instance ().free (r2x) ;
	}

	BOOL exist () const override {
		return thix.mPointer != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (thix.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (thix.mPointer) ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		if ifswitch (TRUE) {
			ret.mHolder = thix.mHolder ;
			ret.mPointer = thix.mPointer ;
			if (ret.mHolder == ZERO)
				discard ;
			fake.mCounter++ ;
		}
		return move (ret) ;
	}

	VREF<RefLayoutData> fake_m () const leftvalue {
		return Pointer::make (thix.mHolder) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

class SliceLayout {
public:
	FLAG mBegin ;
	FLAG mEnd ;
	LENGTH mStep ;
} ;

class SliceImplHolder implement Fat<SliceHolder ,Ref<SliceLayout>> {
public:
	void initialize (CREF<SliceData> data) override {
		const auto r1x = address (data) ;
		thix = Ref<SliceLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH size () const override {
		if (thix == NULL)
			return 0 ;
		return thix->mEnd - thix->mBegin ;
	}

	STRU32 at (CREF<INDEX> index) const override {
		return load (thix->mBegin + index * thix->mStep) ;
	}

	STRU32 load (CREF<FLAG> addr) const {
		if (thix->mStep == 1)
			return STRU8 (Pointer::make (addr)) ;
		if (thix->mStep == 2)
			return STRU16 (Pointer::make (addr)) ;
		if (thix->mStep == 4)
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
			const auto r3x = load (thix->mBegin + i * thix->mStep) ;
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
			const auto r4x = load (thix->mBegin + i * thix->mStep) ;
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
			const auto r2x = load (thix->mBegin + i * thix->mStep) ;
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

class ClazzLayout {
public:
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	Slice<STR> mTypeName ;
} ;

class ClazzImplHolder implement Fat<ClazzHolder ,Ref<ClazzLayout>> {
public:
	void initialize (CREF<ClazzData> data) override {
		const auto r1x = address (data) ;
		thix = Ref<ClazzLayout>::reference (Pointer::make (r1x)) ;
	}

	LENGTH type_size () const override {
		if (thix == NULL)
			return 0 ;
		return thix->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (thix == NULL)
			return 0 ;
		return thix->mTypeAlign ;
	}

	FLAG type_cabi () const override {
		if (thix == NULL)
			return 0 ;
		return address (thix.self) ;
	}

	Slice<STR> type_name () const override {
		if (thix == NULL)
			return Slice<STR> () ;
		return thix->mTypeName ;
	}

	BOOL equal (CREF<Ref<ClazzLayout>> that) const override {
		const auto r1x = type_name () ;
		const auto r2x = ClazzHolder::create (that)->type_name () ;
		return r1x == r2x ;
	}

	FLAG compr (CREF<Ref<ClazzLayout>> that) const override {
		const auto r1x = type_name () ;
		const auto r2x = ClazzHolder::create (that)->type_name () ;
		return operator_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		operator_visit (visitor ,type_size ()) ;
		operator_visit (visitor ,type_align ()) ;
		operator_visit (visitor ,type_cabi ()) ;
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
		BoxHolder::create (thix.mThis)->acquire (value) ;
	}

	BOOL exist () const override {
		return BoxHolder::create (thix.mThis)->exist () ;
	}

	void poll (VREF<BoxLayout> out) const override {
		const auto r1x = address (thix.mThis) ;
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