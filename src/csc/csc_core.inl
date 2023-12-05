#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

#include "csc_end.h"
#include <atomic>
#include "csc_begin.h"

namespace CSC {
class BoxImplHolder implement BoxHolder {
protected:
	XPTR<BoxLayout> mThis ;

public:
	void initialize (CREF<Unknown> that) override {
		mThis->mHolder = Pointer::from (that) ;
	}

	void destroy () override {
		fake.destroy (1) ;
	}

	BOOL exist () const override {
		return mThis->mHolder != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		mThis->mHolder = that.mHolder ;
		const auto r1x = fake.type_size () ;
		const auto r2x = fake.type_align () ;
		const auto r3x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		const auto r4x = operator_alignas (address (that.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		memcpy (csc_pointer_t (r3x) ,csc_pointer_t (r4x) ,r1x) ;
	}

	void release () override {
		mThis->mHolder = ZERO ;
	}

	VREF<Unknown> fake_m () const leftvalue {
		return Pointer::from (mThis->mHolder) ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct HeapProcData {
	mutable std::atomic<LENGTH> mValue ;
} ;

class HeapProcImplHolder implement HeapProcHolder {
protected:
	XPTR<HeapProcLayout> mThis ;

public:
	void initialize () override {
		static HeapProcData mInstance ;
		mThis->mPointer = address (mInstance) ;
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

	VREF<HeapProcData> fake_m () const leftvalue {
		return Pointer::make (mThis->mPointer) ;
	}
} ;

exports VFat<HeapProcHolder> HeapProcHolder::create (VREF<HeapProcLayout> that) {
	return VFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

exports CFat<HeapProcHolder> HeapProcHolder::create (CREF<HeapProcLayout> that) {
	return CFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

struct RefUnknownBinderLayout {
	LENGTH mCounter ;
	LENGTH mSize ;
	FLAG mHolder ;
} ;

class RefImplHolder implement RefHolder {
protected:
	XPTR<RefLayout> mThis ;

public:
	void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) override {
		auto &&rax = unsafe_cast[TYPE<Unknown>::expr] (value.mHolder) ;
		const auto r1x = operator_max (rax.type_align () - ALIGN_OF<FLAG>::expr ,0) ;
		const auto r2x = SIZE_OF<RefUnknownBinderLayout>::expr + r1x + rax.type_size () ;
		mThis->mHolder = HeapProc::instance ().alloc (r2x) ;
		fake.mCounter = 0 ;
		fake.mSize = size_ ;
		auto &&rbx = unsafe_cast[TYPE<BoxLayout>::expr] (fake.mHolder) ;
		BoxHolder::create (rbx)->acquire (value) ;
		const auto r3x = mThis->mHolder + SIZE_OF<RefUnknownBinderLayout>::expr ;
		mThis->mPointer = operator_alignas (r3x ,rax.type_align ()) ;
	}

	void destroy () override {
		const auto r1x = --fake.mCounter ;
		if (r1x > 0)
			return ;
		auto &&rax = unsafe_cast[TYPE<Unknown>::expr] (fake.mHolder) ;
		rax.destroy (fake.mSize) ;
	}

	BOOL exist () const override {
		return mThis->mPointer != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (mThis->mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (mThis->mPointer) ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		ret.mHolder = mThis->mHolder ;
		ret.mPointer = mThis->mPointer ;
		fake.mCounter++ ;
		return move (ret) ;
	}

	VREF<RefUnknownBinderLayout> fake_m () const leftvalue {
		return Pointer::make (mThis->mHolder) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

class SliceImplHolder implement SliceHolder {
protected:
	XPTR<SliceLayout> mThis ;

public:
	void initialize (CREF<SliceData> data) override {
		const auto r1x = address (data) ;
		mThis->mThis = Ref<SliceData>::reference (Pointer::make (r1x)) ;
	}

	LENGTH size () const override {
		if (mThis->mThis == NULL)
			return 0 ;
		return mThis->mThis->mEnd - mThis->mThis->mBegin ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const override {
		unimplemented () ;
		return Pointer::make (0) ;
	}

	STRU32 load (CREF<FLAG> addr) const {
		if (mThis->mThis->mStep == 1)
			return STRU8 (Pointer::make (addr)) ;
		if (mThis->mThis->mStep == 2)
			return STRU16 (Pointer::make (addr)) ;
		if (mThis->mThis->mStep == 4)
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
			const auto r3x = load (mThis->mThis->mBegin + i * mThis->mThis->mStep) ;
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
			const auto r4x = load (mThis->mThis->mBegin + i * mThis->mThis->mStep) ;
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
			const auto r2x = load (mThis->mThis->mBegin + i * mThis->mThis->mStep) ;
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

class ClazzImplHolder implement ClazzHolder {
protected:
	XPTR<ClazzLayout> mThis ;

public:
	void initialize (CREF<ClazzData> data) override {
		const auto r1x = address (data) ;
		mThis->mThis = Ref<ClazzData>::reference (Pointer::make (r1x)) ;
	}

	LENGTH type_size () const override {
		if (mThis->mThis == NULL)
			return 0 ;
		return mThis->mThis->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (mThis->mThis == NULL)
			return 0 ;
		return mThis->mThis->mTypeAlign ;
	}

	FLAG type_cabi () const override {
		if (mThis->mThis == NULL)
			return 0 ;
		return address (mThis->mThis.self) ;
	}

	Slice<STR> type_name () const override {
		if (mThis->mThis == NULL)
			return Slice<STR> () ;
		return mThis->mThis->mTypeName ;
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

class AutoImplHolder implement AutoHolder {
protected:
	XPTR<AutoLayout> mThis ;

public:
	void initialize (CREF<BoxLayout> value) override {
		BoxHolder::create (mThis->mThis)->acquire (value) ;
	}

	BOOL exist () const override {
		return BoxHolder::create (mThis->mThis)->exist () ;
	}

	void poll (VREF<BoxLayout> out) const override {
		BoxHolder::create (out)->acquire (mThis->mThis) ;
		BoxHolder::create (mThis->mThis)->release () ;
	}
} ;

exports VFat<AutoHolder> AutoHolder::create (VREF<AutoLayout> that) {
	return VFat<AutoHolder> (AutoImplHolder () ,that) ;
}

exports CFat<AutoHolder> AutoHolder::create (CREF<AutoLayout> that) {
	return CFat<AutoHolder> (AutoImplHolder () ,that) ;
}
} ;