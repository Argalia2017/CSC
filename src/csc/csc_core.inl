#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_core.hpp"

#include "csc_end.h"
#include <new>
#include <atomic>
#include <cstring>
#include <typeinfo>
#include <initializer_list>
#include <malloc.h>
#include "csc_begin.h"

namespace CSC {
exports FLAG FUNCTION_inline_type_name::invoke (CREF<csc_type_info_t> squalor) noexcept {
	return FLAG (squalor.name ()) ;
}

#ifdef __CSC_COMPILER_MSVC__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.end ()) ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.end ()) ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept {
	return FLAG (squalor.begin ()) + LENGTH (squalor.size ()) * step_ ;
}
#endif

exports void FUNCTION_inline_memset::invoke (VREF<Pointer> dst ,CREF<LENGTH> size_) noexcept {
	std::memset ((&dst) ,0 ,size_) ;
}

exports void FUNCTION_inline_memcpy::invoke (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept {
	std::memcpy ((&dst) ,(&src) ,size_) ;
}

exports FLAG FUNCTION_inline_memcmp::invoke (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept {
	return FLAG (std::memcmp ((&dst) ,(&src) ,size_)) ;
}

class BoxImplHolder implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (fake.mHolder == ZERO) ;
		fake.mHolder = inline_hold (holder) ;
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

	UnknownHolder unknown () const override {
		return UnknownHolder (fake.mHolder) ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = address (fake) + inline_alignas (SIZE_OF<BoxLayout>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_align () ;
		const auto r3x = address (fake) + inline_alignas (SIZE_OF<BoxLayout>::expr ,r2x) ;
		return Pointer::make (r3x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		destroy () ;
		if (that.mHolder == ZERO)
			return ;
		fake.mHolder = that.mHolder ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = r1x->type_size () ;
		inline_memcpy (self ,BoxHolder::create (that)->self ,r2x) ;
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

struct RefPureLayout {
	Heap mHeap ;
	LENGTH mCounter ;
	BoxLayout mValue ;
} ;

class RefImplHolder implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (fake.mHolder == ZERO) ;
		const auto r1x = RFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = inline_alignas (SIZE_OF<RefPureLayout>::expr ,r3x) ;
		const auto r5x = r4x + r2x ;
		const auto r6x = Heap () ;
		fake.mHolder = r6x.alloc (r5x) ;
		ptr (fake).mHeap = r6x ;
		ptr (fake).mCounter = 1 ;
		ptr (fake).mValue.mHolder = ZERO ;
		BoxHolder::create (ptr (fake).mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> element ,CREF<LENGTH> size_) override {
		assert (fake.mHolder == ZERO) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = RFat<ReflectSize> (element) ;
		const auto r5x = r4x->type_size () ;
		const auto r6x = r4x->type_align () ;
		const auto r7x = inline_alignas (SIZE_OF<RefPureLayout>::expr ,r3x) ;
		const auto r8x = inline_alignas (r7x + r2x ,r6x) ;
		const auto r9x = r8x + r5x * size_ ;
		const auto r10x = Heap () ;
		fake.mHolder = r10x.alloc (r9x) ;
		ptr (fake).mHeap = r10x ;
		ptr (fake).mCounter = 1 ;
		ptr (fake).mValue.mHolder = ZERO ;
		BoxHolder::create (ptr (fake).mValue)->initialize (holder) ;
		fake.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
		const auto r11x = RFat<ReflectCreate> (holder) ;
		r11x->create (self ,1) ;
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
			const auto r3x = ptr (fake).mHeap ;
			r3x.free (fake.mHolder) ;
			fake.mHolder = ZERO ;
		}
		fake.mPointer = ZERO ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	UnknownHolder unknown () const override {
		if (fake.mHolder == ZERO)
			return UnknownHolder () ;
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

	RefLayout share () const override {
		RefLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mHolder == ZERO)
				discard ;
			ptr (fake).mCounter++ ;
			ret.mHolder = fake.mHolder ;
			ret.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
		}
		if ifdo (act) {
			ret.mPointer = fake.mPointer ;
		}
		return move (ret) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> addr) const {
		return LENGTH (_msize (addr)) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> addr) const {
		return LENGTH (malloc_usable_size (addr)) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> addr) const {
		return LENGTH (_msize (addr)) ;
	}
} ;
#endif

static constexpr auto memsize = FUNCTION_memsize () ;

class HeapImplement implement HeapHolder {
protected:
	Pin<std::atomic<VAL>> mLength ;
	Temp<Auto> mStorage ;

public:
	imports CREF<Box<HeapImplement>> instance () {
		return memorize ([&] () {
			return Box<HeapImplement>::make () ;
		}) ;
	}

	void initialize () override {
		mLength = NULL ;
		mLength.self = 0 ;
	}

	LENGTH length () const override {
		return mLength.self ;
	}

	FLAG stack (CREF<LENGTH> size_) const override {
		using R1X = typeof (mStorage) ;
		assume (size_ <= SIZE_OF<R1X>::expr) ;
		return address (mStorage) ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = memsize (r1x) ;
		mLength.self += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = memsize (r1x) ;
		mLength.self -= r2x ;
		operator delete (r1x) ;
	}
} ;

class HeapImplHolder implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		fake.mHolder = address (HeapImplement::instance ().self) ;
		ptr (fake).initialize () ;
	}

	imports VREF<HeapHolder> ptr (CREF<HeapLayout> layout) {
		return Pointer::make (layout.mHolder) ;
	}

	LENGTH length () const override {
		assert (fake.mHolder != ZERO) ;
		return ptr (fake).length () ;
	}

	FLAG stack (CREF<LENGTH> size_) const override {
		assert (fake.mHolder != ZERO) ;
		return ptr (fake).stack (size_) ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		assert (fake.mHolder != ZERO) ;
		return ptr (fake).alloc (size_) ;
	}

	void free (CREF<FLAG> addr) const override {
		assert (fake.mHolder != ZERO) ;
		ptr (fake).free (addr) ;
	}
} ;

exports VFat<HeapHolder> HeapHolder::create (VREF<HeapLayout> that) {
	return VFat<HeapHolder> (HeapImplHolder () ,that) ;
}

exports CFat<HeapHolder> HeapHolder::create (CREF<HeapLayout> that) {
	return CFat<HeapHolder> (HeapImplHolder () ,that) ;
}

class SliceImplHolder implement Fat<SliceHolder ,SliceLayout> {
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
			if (fake.mStep != SIZE_OF<STRU8>::expr)
				discard ;
			item = bitwise[TYPE<STRU8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != SIZE_OF<STRU16>::expr)
				discard ;
			item = bitwise[TYPE<STRU16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (fake.mStep != SIZE_OF<STRU32>::expr)
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
			if (!(r3x))
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

	SliceLayout eos () const override {
		SliceLayout ret = fake ;
		INDEX ix = 0 ;
		auto rax = STRU32 () ;
		while (TRUE) {
			if (ix >= fake.mSize)
				break ;
			get (ix ,rax) ;
			if (rax == STRU32 (0X00))
				break ;
			ix++ ;
		}
		ret.mSize = ix ;
		return move (ret) ;
	}
} ;

exports VFat<SliceHolder> SliceHolder::create (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::create (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

struct ClazzPureLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeCode ;
	Slice mTypeName ;
} ;

class ClazzImplHolder implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		fake.mThis = Ref<ClazzPureLayout>::make () ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		fake.mThis->mTypeSize = r1x->type_size () ;
		fake.mThis->mTypeAlign = r1x->type_align () ;
		const auto r2x = RFat<ReflectGuid> (holder) ;
		fake.mThis->mTypeCode = r2x->type_guid () ;
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

	FLAG type_guid () const override {
		if (fake.mThis == NULL)
			return ZERO ;
		return fake.mThis->mTypeCode ;
	}

	Slice type_name () const override {
		if (fake.mThis == NULL)
			return Slice () ;
		return fake.mThis->mTypeName ;
	}

	BOOL equal (CREF<ClazzLayout> that) const override {
		if (type_size () != ClazzHolder::create (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::create (that)->type_align ())
			return FALSE ;
		if (type_guid () == ClazzHolder::create (that)->type_guid ())
			return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		if (type_size () == ClazzHolder::create (that)->type_size ())
			if (type_align () == ClazzHolder::create (that)->type_align ())
				if (type_guid () == ClazzHolder::create (that)->type_guid ())
					return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		inline_visit (visitor ,type_size ()) ;
		inline_visit (visitor ,type_align ()) ;
		inline_visit (visitor ,type_guid ()) ;
		inline_visit (visitor ,type_name ()) ;
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
	void initialize (RREF<BoxLayout> item) override {
		BoxHolder::create (fake.mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
	}

	BOOL exist () const {
		return BoxHolder::create (fake.mValue)->exist () ;
	}

	void poll (CREF<Unknown> holder ,VREF<BoxLayout> item) override {
		assert (exist ()) ;
		assert (BoxHolder::create (fake.mValue)->unknown () == holder) ;
		BoxHolder::create (item)->acquire (fake.mValue) ;
		BoxHolder::create (fake.mValue)->release () ;
	}
} ;

exports VFat<AutoHolder> AutoHolder::create (VREF<AutoLayout> that) {
	return VFat<AutoHolder> (AutoImplHolder () ,that) ;
}

exports CFat<AutoHolder> AutoHolder::create (CREF<AutoLayout> that) {
	return CFat<AutoHolder> (AutoImplHolder () ,that) ;
}
} ;