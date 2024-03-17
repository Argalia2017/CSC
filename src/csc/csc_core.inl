#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

#include "csc_end.h"
#include <new>
#include <atomic>
#include "csc_begin.h"

namespace CSC {
class BoxImplHolder implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		fake.mHolder = unsafe_hold (holder) ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHolder == ZERO)
				return ;
			const auto r1x = CFat<ReflectDestroy> (unknown ()) ;
			r1x->destroy (self ,1) ;
			fake.mHolder = ZERO ;
		}
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	CREF<Unknown> unknown () const leftvalue override {
		assert (exist ()) ;
		return Pointer::from (fake.mHolder) ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = CFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = operator_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = CFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = operator_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		assert (ifnot (exist ())) ;
		fake.mHolder = that.mHolder ;
		if (fake.mHolder == ZERO)
			return ;
		const auto r1x = CFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = (&self) ;
		const auto r4x = (&BoxHolder::create (that)->self) ;
		memcpy (r3x ,r4x ,r2x) ;
	}

	void release () override {
		fake.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,address (that)) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,address (that)) ;
}

struct RefImplLayout {
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class RefImplHolder implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> value) override {
		const auto r1x = CFat<ReflectSize> (BoxHolder::create (value)->unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = operator_max (r3x - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r5x = SIZE_OF<RefImplLayout>::expr + r4x + r2x ;
		fake.mHolder = Heap::instance ().alloc (r5x) ;
		ptr (fake.mHolder).mCounter = 1 ;
		ptr (fake.mHolder).mValue.mHolder = ZERO ;
		BoxHolder::create (ptr (fake.mHolder).mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
		fake.mPointer = address (BoxHolder::create (ptr (fake.mHolder).mValue)->self) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> item ,CREF<LENGTH> size_) override {
		const auto r1x = CFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r5x = CFat<ReflectSize> (item) ;
		const auto r6x = r5x->type_size () ;
		const auto r7x = r5x->type_align () ;
		const auto r4x = operator_max (operator_max (r3x ,r7x) - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r8x = operator_alignas (r2x ,r7x) ;
		const auto r9x = SIZE_OF<RefImplLayout>::expr + r4x + r8x + r6x * size_ ;
		fake.mHolder = Heap::instance ().alloc (r9x) ;
		ptr (fake.mHolder).mCounter = 1 ;
		BoxHolder::create (ptr (fake.mHolder).mValue)->initialize (holder) ;
		const auto r10x = CFat<ReflectCreate> (holder) ;
		r10x->create (BoxHolder::create (ptr (fake.mHolder).mValue)->self ,1) ;
		fake.mPointer = address (BoxHolder::create (ptr (fake.mHolder).mValue)->self) ;
	}

	imports VREF<RefImplLayout> ptr (CREF<FLAG> pointer) {
		return Pointer::make (pointer) ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHolder == ZERO)
				discard ;
			const auto r1x = --ptr (fake.mHolder).mCounter ;
			if (r1x > 0)
				discard ;
			const auto r2x = CFat<ReflectDestroy> (unknown ()) ;
			r2x->destroy (self ,1) ;
			Heap::instance ().free (fake.mHolder) ;
			fake.mHolder = ZERO ;
		}
		fake.mPointer = ZERO ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	CREF<Unknown> unknown () const leftvalue override {
		return BoxHolder::create (ptr (fake.mHolder).mValue)->unknown () ;
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
			if (fake.mHolder == ZERO)
				discard ;
			ptr (fake.mHolder).mCounter++ ;
			ret.mHolder = fake.mHolder ;
		}
		ret.mPointer = fake.mPointer ;
		return move (ret) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,address (that)) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,address (that)) ;
}

struct HeapImplLayout {
	Pin<std::atomic<LENGTH>> mLength ;
} ;

class HeapImplHolder implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		auto &&rax = memorize ([&] () {
			return TEMP<HeapImplLayout> () ;
		}) ;
		fake.mThis = Ref<HeapImplLayout>::reference (ptr (address (rax))) ;
		fake.mThis->mLength.self = 0 ;
	}

	imports VREF<HeapImplLayout> ptr (CREF<FLAG> pointer) {
		return Pointer::make (pointer) ;
	}

	LENGTH length () const override {
		return fake.mThis->mLength.self ;
	}

	FLAG stack (CREF<LENGTH> size_) const override {
		return FLAG (_alloca (size_)) ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mThis->mLength.self += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mThis->mLength.self -= r2x ;
		operator delete (r1x) ;
	}
} ;

exports VFat<HeapHolder> HeapHolder::create (VREF<HeapLayout> that) {
	return VFat<HeapHolder> (HeapImplHolder () ,address (that)) ;
}

exports CFat<HeapHolder> HeapHolder::create (CREF<HeapLayout> that) {
	return CFat<HeapHolder> (HeapImplHolder () ,address (that)) ;
}

class SliceImplHolder implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<FLAG> pointer) override {
		const auto r1x = CFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		fake.mBegin = pointer ;
		fake.mEnd = fake.mBegin + r2x - r3x ;
		fake.mStep = r3x ;
	}

	LENGTH size () const override {
		return (fake.mEnd - fake.mBegin) / fake.mStep ;
	}

	LENGTH step () const override {
		return fake.mStep ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		if ifdo (TRUE) {
			if (fake.mStep != 1)
				discard ;
			item = STRU8 (at (index)) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mStep != 2)
				discard ;
			item = STRU16 (at (index)) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mStep != 4)
				discard ;
			item = STRU32 (at (index)) ;
			return ;
		}
		assert (FALSE) ;
	}

	imports STRU32 get (CREF<SliceLayout> layout ,CREF<INDEX> index) {
		STRU32 ret ;
		SliceHolder::create (layout)->get (index ,ret) ;
		return move (ret) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBegin + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	BOOL equal (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = get (fake ,i) ;
			const auto r4x = get (that ,i) ;
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
			const auto r4x = get (fake ,i) ;
			const auto r5x = get (that ,i) ;
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
			const auto r2x = get (fake ,i) ;
			operator_visit (visitor ,r2x) ;
		}
		visitor.end () ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::create (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,address (that)) ;
}

exports CFat<SliceHolder> SliceHolder::create (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,address (that)) ;
}

struct ClazzImplLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeUUID ;
	Slice<STR> mTypeName ;
} ;

class ClazzImplHolder implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		const auto r1x = CFat<ReflectSize> (holder) ;
		const auto r2x = CFat<ReflectName> (holder) ;
		fake.mThis = Ref<ClazzImplLayout>::make () ;
		fake.mThis->mTypeSize = r1x->type_size () ;
		fake.mThis->mTypeAlign = r1x->type_align () ;
		fake.mThis->mTypeUUID = unsafe_hold (holder) ;
		fake.mThis->mTypeName = r2x->type_name () ;
	}

	LENGTH type_size () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mTypeAlign ;
	}

	FLAG type_uuid () const override {
		if (fake.mThis == NULL)
			return ZERO ;
		return fake.mThis->mTypeUUID ;
	}

	Slice<STR> type_name () const override {
		if (fake.mThis == NULL)
			return Slice<STR> () ;
		return fake.mThis->mTypeName ;
	}

	BOOL equal (CREF<ClazzLayout> that) const override {
		if (type_uuid () != ZERO)
			if (type_uuid () == ClazzHolder::create (that)->type_uuid ())
				return TRUE ;
		return type_name () == ClazzHolder::create (that)->type_name () ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		if (type_uuid () != ZERO)
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

exports VFat<ClazzHolder> ClazzHolder::create (VREF<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,address (that)) ;
}

exports CFat<ClazzHolder> ClazzHolder::create (CREF<ClazzLayout> that) {
	return CFat<ClazzHolder> (ClazzImplHolder () ,address (that)) ;
}

class AutoImplHolder implement Fat<AutoHolder ,AutoLayout> {
public:
	void initialize (RREF<BoxLayout> value) override {
		BoxHolder::create (fake.mValue)->acquire (value) ;
		BoxHolder::create (value)->release () ;
	}

	void fetch (CREF<Unknown> holder ,VREF<BoxLayout> out) override {
		assert (fake.mValue.mHolder == unsafe_hold (holder)) ;
		BoxHolder::create (out)->acquire (fake.mValue) ;
		BoxHolder::create (fake.mValue)->release () ;
	}
} ;

exports VFat<AutoHolder> AutoHolder::create (VREF<AutoLayout> that) {
	return VFat<AutoHolder> (AutoImplHolder () ,address (that)) ;
}

exports CFat<AutoHolder> AutoHolder::create (CREF<AutoLayout> that) {
	return CFat<AutoHolder> (AutoImplHolder () ,address (that)) ;
}
} ;