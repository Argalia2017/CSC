#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_core.hpp"

#include "csc_end.h"
#include <new>
#include <atomic>
#include <cstring>
#include "csc_begin.h"

namespace CSC {
exports void CoreTool::inline_memset (VREF<Pointer> dst ,CREF<LENGTH> size_) noexcept {
	std::memset ((&dst) ,0 ,size_) ;
}

exports void CoreTool::inline_memcpy (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept {
	std::memcpy ((&dst) ,(&src) ,size_) ;
}

exports BOOL CoreTool::inline_memcmp (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept {
	return FLAG (std::memcmp ((&dst) ,(&src) ,size_)) == ZERO ;
}

exports LENGTH CoreTool::inline_memchr (CREF<Pointer> src ,CREF<LENGTH> size_) noexcept {
	const auto r1x = FLAG (std::memchr ((&src) ,0 ,size_)) ;
	if (r1x == ZERO)
		return size_ ;
	return r1x - address (src) ;
}

class BoxImplement implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		fake.mHolder = inline_code (holder) ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHolder == ZERO)
				return ;
			const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
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
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = inline_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = inline_alignas (address (fake.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		assert (ifnot (exist ())) ;
		fake.mHolder = that.mHolder ;
		if (fake.mHolder == ZERO)
			return ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_size () ;
		CoreTool::inline_memcpy (self ,BoxHolder::create (that)->self ,r2x) ;
	}

	void release () override {
		fake.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplement () ,Pointer::from (that)) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplement () ,Pointer::from (that)) ;
}

struct RefPureLayout {
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class RefImplement implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		const auto r1x = RFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = inline_max (r3x - ALIGN_OF<RefPureLayout>::expr ,0) ;
		const auto r5x = SIZE_OF<RefPureLayout>::expr + r4x + r2x ;
		fake.mHolder = HeapTool::instance ().alloc (r5x) ;
		ptr (fake).mCounter = 1 ;
		ptr (fake).mValue.mHolder = ZERO ;
		const auto r6x = BoxHolder::create (ptr (fake).mValue) ;
		r6x->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (r6x->self) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> element ,CREF<LENGTH> size_) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = RFat<ReflectSize> (element) ;
		const auto r5x = r4x->type_size () ;
		const auto r6x = r4x->type_align () ;
		const auto r7x = inline_max (inline_max (r3x ,r6x) - ALIGN_OF<RefPureLayout>::expr ,0) ;
		const auto r8x = inline_alignas (r2x ,r6x) ;
		const auto r9x = SIZE_OF<RefPureLayout>::expr + r7x + r8x + r5x * size_ ;
		fake.mHolder = HeapTool::instance ().alloc (r9x) ;
		ptr (fake).mCounter = 1 ;
		const auto r10x = BoxHolder::create (ptr (fake).mValue) ;
		r10x->initialize (holder) ;
		const auto r11x = RFat<ReflectCreate> (holder) ;
		r11x->create (r10x->self ,1) ;
		fake.mPointer = address (r10x->self) ;
	}

	imports VREF<RefPureLayout> ptr (CREF<RefLayout> layout) {
		return Pointer::make (layout.mHolder) ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHolder == ZERO)
				discard ;
			const auto r1x = --ptr (fake).mCounter ;
			if (r1x > 0)
				discard ;
			const auto r2x = RFat<ReflectDestroy> (unknown ()) ;
			r2x->destroy (self ,1) ;
			HeapTool::instance ().free (fake.mHolder) ;
			fake.mHolder = ZERO ;
		}
		fake.mPointer = ZERO ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		if ifdo (TRUE) {
			if (fake.mHolder == ZERO)
				discard ;
			ptr (fake).mCounter++ ;
			ret.mHolder = fake.mHolder ;
		}
		ret.mPointer = fake.mPointer ;
		return move (ret) ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	CREF<Unknown> unknown () const leftvalue override {
		return BoxHolder::create (ptr (fake).mValue)->unknown () ;
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

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplement () ,Pointer::from (that)) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplement () ,Pointer::from (that)) ;
}

struct HeapToolPureLayout {
	Pin<std::atomic<VAL>> mLength ;
} ;

class HeapProcImplement implement Fat<HeapToolHolder ,HeapToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<HeapToolPureLayout>::reference (instance ()) ;
		fake.mThis->mLength.self = 0 ;
	}

	imports VREF<HeapToolPureLayout> instance () {
		return memorize ([&] () {
			return Pin<HeapToolPureLayout> () ;
		}) ;
	}

	LENGTH length () const override {
		return fake.mThis->mLength.self ;
	}

	void stack (CREF<FLAG> buffer ,CREF<LENGTH> size_) const override {
		unimplemented () ;
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

exports VFat<HeapToolHolder> HeapToolHolder::create (VREF<HeapToolLayout> that) {
	return VFat<HeapToolHolder> (HeapProcImplement () ,Pointer::from (that)) ;
}

exports CFat<HeapToolHolder> HeapToolHolder::create (CREF<HeapToolLayout> that) {
	return CFat<HeapToolHolder> (HeapProcImplement () ,Pointer::from (that)) ;
}

class SliceImplement implement Fat<SliceHolder ,SliceLayout> {
public:
	LENGTH size () const override {
		return fake.mSize ;
	}

	LENGTH step () const override {
		return fake.mStep ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStep != ALIGN_OF<STRU8>::expr)
				discard ;
			item = bitwise[TYPE<STRU8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != ALIGN_OF<STRU16>::expr)
				discard ;
			item = bitwise[TYPE<STRU16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != ALIGN_OF<STRU32>::expr)
				discard ;
			item = bitwise[TYPE<STRU32>::expr] (at (index)) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = fake.mBuffer + index * fake.mStep ;
		return Pointer::make (r1x) ;
	}

	BOOL equal (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			SliceHolder::create (that)->get (i ,rbx) ;
			const auto r3x = inline_equal (rax ,rbx) ;
			if ifnot (r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::create (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			SliceHolder::create (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			inline_visit (visitor ,rax) ;
		}
		visitor.end () ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::create (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplement () ,Pointer::from (that)) ;
}

exports CFat<SliceHolder> SliceHolder::create (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplement () ,Pointer::from (that)) ;
}

struct ClazzPureLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeSpec ;
	Slice<STR> mTypeName ;
} ;

class ClazzImplement implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		fake.mThis = Ref<ClazzPureLayout>::make () ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		fake.mThis->mTypeSize = r1x->type_size () ;
		fake.mThis->mTypeAlign = r1x->type_align () ;
		const auto r2x = RFat<ReflectCode> (holder) ;
		fake.mThis->mTypeSpec = r2x->type_code () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		fake.mThis->mTypeName = r3x->type_name () ;
	}

	void initialize (CREF<ClazzLayout> that) override {
		fake.mThis = that.mThis.share () ;
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

	FLAG type_code () const override {
		if (fake.mThis == NULL)
			return ZERO ;
		return fake.mThis->mTypeSpec ;
	}

	Slice<STR> type_name () const override {
		if (fake.mThis == NULL)
			return Slice<STR> () ;
		return fake.mThis->mTypeName ;
	}

	BOOL equal (CREF<ClazzLayout> that) const override {
		if (type_size () != ClazzHolder::create (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::create (that)->type_align ())
			return FALSE ;
		if (type_code () == ClazzHolder::create (that)->type_code ())
			return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		if (type_code () == ClazzHolder::create (that)->type_code ())
			return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		inline_visit (visitor ,type_size ()) ;
		inline_visit (visitor ,type_align ()) ;
		inline_visit (visitor ,type_code ()) ;
		inline_visit (visitor ,type_name ()) ;
		visitor.end () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::create (VREF<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplement () ,Pointer::from (that)) ;
}

exports CFat<ClazzHolder> ClazzHolder::create (CREF<ClazzLayout> that) {
	return CFat<ClazzHolder> (ClazzImplement () ,Pointer::from (that)) ;
}

class AutoImplement implement Fat<AutoHolder ,AutoLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		BoxHolder::create (fake.mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}

	BOOL exist () const {
		return BoxHolder::create (fake.mValue)->exist () ;
	}

	void poll (CREF<Unknown> holder ,VREF<BoxLayout> item) override {
		assert (exist ()) ;
		assert (inline_code (BoxHolder::create (fake.mValue)->unknown ()) == inline_code (holder)) ;
		BoxHolder::create (item)->acquire (fake.mValue) ;
		BoxHolder::create (fake.mValue)->release () ;
	}
} ;

exports VFat<AutoHolder> AutoHolder::create (VREF<AutoLayout> that) {
	return VFat<AutoHolder> (AutoImplement () ,Pointer::from (that)) ;
}

exports CFat<AutoHolder> AutoHolder::create (CREF<AutoLayout> that) {
	return CFat<AutoHolder> (AutoImplement () ,Pointer::from (that)) ;
}
} ;