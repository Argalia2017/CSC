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

struct HeapProcLayoutData {
	std::atomic<LENGTH> mValue ;
} ;

class HeapProcImplHolder implement Fat<HeapProcHolder ,HeapProcLayout> {
public:
	void initialize () override {
		auto &&rax = memorize ([&] () {
			return TEMP<HeapProcLayoutData> () ;
		}) ;
		thix.mHolder = address (rax) ;
	}

	LENGTH length () const override {
		return fake.mValue ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mValue += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mValue -= r2x ;
		operator delete (r1x) ;
	}

	VREF<HeapProcLayoutData> fake_m () const leftvalue {
		return Pointer::make (thix.mHolder) ;
	}
} ;

exports VFat<HeapProcHolder> HeapProcHolder::create (VREF<HeapProcLayout> that) {
	return VFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

exports CFat<HeapProcHolder> HeapProcHolder::create (CREF<HeapProcLayout> that) {
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
		ret.mHolder = thix.mHolder ;
		ret.mPointer = thix.mPointer ;
		fake.mCounter++ ;
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

class SliceImplHolder implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CREF<SliceData> data) override {
		const auto r1x = address (data) ;
		thix.mThis = Ref<SliceData>::reference (Pointer::make (r1x)) ;
	}

	LENGTH size () const override {
		if (thix.mThis == NULL)
			return 0 ;
		return thix.mThis->mEnd - thix.mThis->mBegin ;
	}

	RREF<Pointer> at (CREF<INDEX> index ,RREF<STRU32> ret) const override {
		ret = load (thix.mThis->mBegin + index * thix.mThis->mStep) ;
		return Pointer::from (move (ret)) ;
	}

	STRU32 load (CREF<FLAG> addr) const {
		if (thix.mThis->mStep == 1)
			return STRU8 (Pointer::make (addr)) ;
		if (thix.mThis->mStep == 2)
			return STRU16 (Pointer::make (addr)) ;
		if (thix.mThis->mStep == 4)
			return STRU32 (Pointer::make (addr)) ;
		assert (FALSE) ;
		return 0 ;
	}

	BOOL equal (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = load (thix.mThis->mBegin + i * thix.mThis->mStep) ;
			const auto r4x = load (that.mThis->mBegin + i * that.mThis->mStep) ;
			if (r3x != r4x)
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		const auto r3x = operator_min (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r4x = load (thix.mThis->mBegin + i * thix.mThis->mStep) ;
			const auto r5x = load (that.mThis->mBegin + i * that.mThis->mStep) ;
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
			const auto r2x = load (thix.mThis->mBegin + i * thix.mThis->mStep) ;
			visitor.push (bitwise (r2x)) ;
		}
		visitor.end () ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::create (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::create (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

class ClazzImplHolder implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<ClazzData> data) override {
		const auto r1x = address (data) ;
		thix.mThis = Ref<ClazzData>::reference (Pointer::make (r1x)) ;
	}

	LENGTH type_size () const override {
		if (thix.mThis == NULL)
			return 0 ;
		return thix.mThis->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (thix.mThis == NULL)
			return 0 ;
		return thix.mThis->mTypeAlign ;
	}

	FLAG type_cabi () const override {
		if (thix.mThis == NULL)
			return 0 ;
		return address (thix.mThis.self) ;
	}

	Slice<STR> type_name () const override {
		if (thix.mThis == NULL)
			return Slice<STR> () ;
		return thix.mThis->mTypeName ;
	}

	BOOL equal (CREF<ClazzLayout> that) const override {
		const auto r1x = type_name () ;
		const auto r2x = ClazzHolder::create (that)->type_name () ;
		return r1x == r2x ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		const auto r1x = type_name () ;
		const auto r2x = ClazzHolder::create (that)->type_name () ;
		return operator_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		visitor.push (bitwise (type_size ())) ;
		visitor.push (bitwise (type_align ())) ;
		visitor.push (bitwise (type_cabi ())) ;
		type_name ().visit (visitor) ;
		visitor.end () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::create (VREF<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

exports CFat<ClazzHolder> ClazzHolder::create (CREF<ClazzLayout> that) {
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