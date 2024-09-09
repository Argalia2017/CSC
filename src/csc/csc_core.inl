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
exports FLAG FUNCTION_inline_type_name::invoke (CREF<Pointer> squalor) noexcept {
	auto &&rax = keep[TYPE<std::type_info>::expr] (squalor) ;
	return FLAG (rax.name ()) ;
}

#ifdef __CSC_COMPILER_MSVC__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.end ()) ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.begin ()) + LENGTH (rax.size ()) * step_ ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
exports FLAG FUNCTION_inline_list_begin::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.begin ()) ;
}

exports FLAG FUNCTION_inline_list_end::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) noexcept {
	auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	return FLAG (rax.end ()) ;
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
		assert (!exist ()) ;
		fake.mHolder = inline_hold (holder) ;
	}

	void destroy () override {
		if (fake.mHolder == ZERO)
			return ;
		const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
		r1x->destroy (self ,1) ;
		fake.mHolder = ZERO ;
	}

	BOOL exist () const override {
		return fake.mHolder != ZERO ;
	}

	RFat<Unknown> unknown () const override {
		assert (exist ()) ;
		auto &&rax = keep[TYPE<Unknown>::expr] (Pointer::from (fake.mHolder)) ;
		return RFat<Unknown> (rax ,NULL) ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = inline_alignas (SIZE_OF<BoxLayout>::expr ,r1x->type_align ()) ;
		const auto r3x = address (fake) + r2x ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = inline_alignas (SIZE_OF<BoxLayout>::expr ,r1x->type_align ()) ;
		const auto r3x = address (fake) + r2x ;
		return Pointer::make (r3x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		assert (!exist ()) ;
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

struct RefImplLayout implement Proxy {
	Heap mHeap ;
	Pin<std::atomic<VAL>> mCounter ;
	BoxLayout mValue ;
} ;

class RefImplHolder implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = inline_alignas (SIZE_OF<RefImplLayout>::expr ,r1x->type_align ()) ;
		const auto r4x = r3x + r2x ;
		const auto r5x = Heap () ;
		fake.mHandle = r5x.alloc (r4x) ;
		inline_memset (Pointer::make (fake.mHandle) ,r4x) ;
		ptr (fake).mHeap = r5x ;
		BoxHolder::create (ptr (fake).mValue)->acquire (item) ;
		BoxHolder::create (item)->release () ;
		fake.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
		ptr (fake).mCounter.self = 1 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> extend ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = RFat<ReflectSize> (extend) ;
		const auto r4x = r3x->type_size () ;
		const auto r5x = inline_alignas (SIZE_OF<RefImplLayout>::expr ,r1x->type_align ()) ;
		const auto r6x = inline_alignas (r5x + r2x ,r3x->type_align ()) ;
		const auto r7x = r6x + r4x * size_ ;
		const auto r8x = Heap () ;
		fake.mHandle = r8x.alloc (r7x) ;
		inline_memset (Pointer::make (fake.mHandle) ,r7x) ;
		ptr (fake).mHeap = r8x ;
		BoxHolder::create (ptr (fake).mValue)->initialize (holder) ;
		fake.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
		const auto r9x = RFat<ReflectCreate> (holder) ;
		r9x->create (Pointer::make (fake.mPointer) ,1) ;
		ptr (fake).mCounter.self = 1 ;
	}

	imports VREF<RefImplLayout> ptr (CREF<RefLayout> layout) {
		return Pointer::make (layout.mHandle) ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHandle == ZERO)
				discard ;
			const auto r1x = --ptr (fake).mCounter.self ;
			if (r1x > 0)
				discard ;
			BoxHolder::create (ptr (fake).mValue)->destroy () ;
			const auto r2x = ptr (fake).mHeap ;
			r2x.free (fake.mHandle) ;
			fake.mHandle = ZERO ;
		}
		fake.mPointer = ZERO ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mHandle == ZERO)
				discard ;
			const auto r1x = ++ptr (fake).mCounter.self ;
			assert (r1x >= 2) ;
			ret.mHandle = fake.mHandle ;
			ret.mPointer = address (BoxHolder::create (ptr (fake).mValue)->self) ;
		}
		if ifdo (act) {
			ret.mPointer = fake.mPointer ;
		}
		return move (ret) ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	RFat<Unknown> unknown () const override {
		assert (exist ()) ;
		return BoxHolder::create (ptr (fake).mValue)->unknown () ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	RefLayout recycle () const override {
		RefLayout ret ;
		if ifdo (TRUE) {
			if (fake.mHandle == ZERO)
				discard ;
			const auto r1x = ptr (fake).mCounter->load () ;
			assert (r1x == IDEN) ;
		}
		ret.mPointer = fake.mPointer ;
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

struct HeapImplLayout {
	BOOL mOnce ;
	Pin<std::atomic<VAL>> mLength ;

public:
	imports CREF<HeapImplLayout> instance () {
		return memorize ([&] () {
			HeapImplLayout ret ;
			ret.mOnce = FALSE ;
			return move (ret) ;
		}) ;
	}
} ;

class HeapImplHolder implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		fake.mHolder = inline_hold (thiz) ;
		if (ptr (fake).mOnce)
			return ;
		ptr (fake).mOnce = TRUE ;
		ptr (fake).mLength.remake () ;
		ptr (fake).mLength.self = 0 ;
	}

	imports VREF<HeapImplLayout> ptr (CREF<HeapLayout> layout) {
		return Pointer::make (address (HeapImplLayout::instance ())) ;
	}

	LENGTH length () const override {
		return ptr (fake).mLength.self ;
	}

	FLAG stack (CREF<LENGTH> size_) const override {
		unimplemented () ;
		return ZERO ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = memsize (r1x) ;
		ptr (fake).mLength.self += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = memsize (r1x) ;
		ptr (fake).mLength.self -= r2x ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

exports VFat<HeapHolder> HeapHolder::create (VREF<HeapLayout> that) {
	if (that.mHolder == ZERO)
		return VFat<HeapHolder> (HeapImplHolder () ,that) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapHolder> (rax ,that) ;
}

exports CFat<HeapHolder> HeapHolder::create (CREF<HeapLayout> that) {
	if (that.mHolder == ZERO)
		return CFat<HeapHolder> (HeapImplHolder () ,that) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapHolder> (rax ,that) ;
}

class SliceImplHolder implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) override {
		fake.mBuffer = buffer ;
		fake.mSize = size_ ;
		fake.mStep = step_ ;
	}

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
			if (!r3x)
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

	void visit (VREF<Visitor> visitor) const override {
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

struct ClazzImplLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeGuid ;
	Slice mTypeName ;
} ;

class ClazzImplHolder implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		auto rax = ClazzImplLayout () ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		rax.mTypeSize = r1x->type_size () ;
		rax.mTypeAlign = r1x->type_align () ;
		const auto r2x = RFat<ReflectGuid> (holder) ;
		rax.mTypeGuid = r2x->type_guid () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		rax.mTypeName = r3x->type_name () ;
		fake.mThis = Ref<ClazzImplLayout>::make (move (rax)) ;
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
		return fake.mThis->mTypeGuid ;
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
		if (type_guid () != ZERO)
			if (type_guid () == ClazzHolder::create (that)->type_guid ())
				return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		const auto r1x = inline_compr (type_size () ,ClazzHolder::create (that)->type_size ()) ;
		if (r1x != ZERO)
			return r1x ;
		const auto r2x = inline_compr (type_align () ,ClazzHolder::create (that)->type_align ()) ;
		if (r2x != ZERO)
			return r2x ;
		if (type_guid () != ZERO)
			if (type_guid () == ClazzHolder::create (that)->type_guid ())
				return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::create (that)->type_name ()) ;
	}

	void visit (VREF<Visitor> visitor) const override {
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
} ;