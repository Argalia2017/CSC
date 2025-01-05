#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_core.hpp"

#include "csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#include <debugapi.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <malloc.h>
#include <typeinfo>
#include <initializer_list>
#include <atomic>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_has_debugger {
	forceinline BOOL operator() () const noexcept {
		return IsDebuggerPresent () ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_has_debugger {
	forceinline BOOL operator() () const noexcept {
		return FALSE ;
	}
} ;
#endif

static constexpr auto has_debugger = FUNCTION_has_debugger () ;

exports BOOL FUNCTION_inline_unittest::invoke () {
	return has_debugger () ;
} ;

exports void FUNCTION_inline_abort::invoke () {
	std::abort () ;
} ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_side_effect {
	forceinline void operator() (csc_pointer_t src) const noexcept {
		_ReadWriteBarrier () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_side_effect {
	forceinline void operator() (csc_pointer_t src) const noexcept {
		asm volatile ("" : : "r" (src) : "memory") ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_side_effect {
	forceinline void operator() (csc_pointer_t src) const noexcept {
		asm volatile ("" : : "r" (src) : "memory") ;
	}
} ;
#endif

static constexpr auto side_effect = FUNCTION_side_effect () ;

exports void FUNCTION_inline_watch::invoke (VREF<Pointer> src) {
	side_effect ((&src)) ;
}

#ifdef __CSC_CXX_RTTI__
struct FUNCTION_core_type_name {
	forceinline FLAG operator() (CREF<Interface> squalor) const noexcept {
		return FLAG (typeid (squalor).name ()) ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
struct FUNCTION_core_type_name {
	forceinline FLAG operator() (CREF<Interface> squalor) const noexcept {
		assert (FALSE) ;
		return ZERO ;
	}
} ;
#endif

static constexpr auto core_type_name = FUNCTION_core_type_name () ;

exports FLAG FUNCTION_inline_type_name::invoke (CREF<Interface> squalor) {
	return core_type_name (squalor) ;
}

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_core_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<Pointer> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
		ret.m1st = FLAG (rax.begin ()) ;
		ret.m2nd = FLAG (rax.end ()) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_core_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<Pointer> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
		ret.m1st = FLAG (rax.begin ()) ;
		ret.m2nd = FLAG (rax.begin ()) + LENGTH (rax.size ()) * step_ ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_core_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CREF<Pointer> squalor ,CREF<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		auto &&rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
		ret.m1st = FLAG (rax.begin ()) ;
		ret.m2nd = FLAG (rax.end ()) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto core_list_pair = FUNCTION_core_list_pair () ;

exports Tuple<FLAG ,FLAG> FUNCTION_inline_list_pair::invoke (CREF<Pointer> squalor ,CREF<LENGTH> step_) {
	return core_list_pair (squalor ,step_) ;
}

exports void FUNCTION_inline_memset::invoke (VREF<Pointer> dst ,CREF<LENGTH> size_) {
	std::memset ((&dst) ,0 ,size_) ;
}

exports void FUNCTION_inline_memcpy::invoke (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) {
	std::memcpy ((&dst) ,(&src) ,size_) ;
}

exports FLAG FUNCTION_inline_memcmp::invoke (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) {
	return FLAG (std::memcmp ((&dst) ,(&src) ,size_)) ;
}

class BoxImplHolder final implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CREF<Unknown> holder) override {
		assert (!exist ()) ;
		fake.mHolder = inline_vptr (holder) ;
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

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (fake.mHolder) ;
	}

	VREF<Pointer> self_m () leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (fake) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (fake) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	void remake (CREF<Unknown> holder) override {
		if (exist ())
			return ;
		fake.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memset (self ,r1x->type_size ()) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		assert (!exist ()) ;
		if (that.mHolder == ZERO)
			return ;
		fake.mHolder = that.mHolder ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (self ,BoxHolder::hold (that)->self ,r1x->type_size ()) ;
	}

	void release () override {
		fake.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::hold (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::hold (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct RefImplLayout implement Proxy {
	Heap mHeap ;
	std::atomic<VAL> mCounter ;
	BoxLayout mValue ;
} ;

static constexpr auto REFIMPLLAYOUT_MIN_HANDLE = FLAG (0X1000) ;

class RefImplHolder final implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RREF<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (BoxHolder::hold (item)->unknown ()) ;
		const auto r2x = inline_max (r1x->type_align () - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r3x = SIZE_OF<RefImplLayout>::expr + r2x + r1x->type_size () ;
		const auto r4x = Heap::instance () ;
		fake.mHandle = r4x.alloc (r3x) ;
		assert (fake.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (fake.mHandle) ,SIZE_OF<RefImplLayout>::expr) ;
		ptr (fake).mHeap = r4x ;
		BoxHolder::hold (ptr (fake).mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		fake.mPointer = address (BoxHolder::hold (ptr (fake).mValue)->self) ;
		ptr (fake).mCounter = 1 ;
	}

	void initialize (CREF<RefLayout> that) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (that.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ++ptr (that).mCounter ;
			noop (r1x) ;
			assert (r1x >= 2) ;
			fake.mHandle = that.mHandle ;
			fake.mPointer = address (BoxHolder::hold (ptr (that).mValue)->self) ;
		}
		if ifdo (act) {
			fake.mHandle = that.mHandle ;
			fake.mPointer = that.mPointer ;
		}
	}

	void initialize (CREF<Unknown> holder ,CREF<Unknown> extend ,CREF<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = RFat<ReflectSize> (extend) ;
		const auto r3x = inline_max (r1x->type_align () - ALIGN_OF<RefImplLayout>::expr ,0) ;
		const auto r4x = SIZE_OF<RefImplLayout>::expr + r3x + r1x->type_size () ;
		const auto r5x = inline_max (r2x->type_align () - r1x->type_align () ,0) ;
		const auto r6x = r4x + r5x + r2x->type_size () * size_ ;
		const auto r7x = Heap::instance () ;
		fake.mHandle = r7x.alloc (r6x) ;
		assert (fake.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (fake.mHandle) ,SIZE_OF<RefImplLayout>::expr) ;
		ptr (fake).mHeap = r7x ;
		BoxHolder::hold (ptr (fake).mValue)->initialize (holder) ;
		fake.mPointer = address (BoxHolder::hold (ptr (fake).mValue)->self) ;
		const auto r8x = RFat<ReflectCreate> (holder) ;
		r8x->create (self ,1) ;
		ptr (fake).mCounter = 1 ;
	}

	void initialize (CREF<Unknown> holder ,CREF<FLAG> pointer) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_align () ;
		noop (r2x) ;
		assert (r2x <= ALIGN_OF<RefImplLayout>::expr) ;
		const auto r3x = pointer - SIZE_OF<RefImplLayout>::expr ;
		auto &&rax = keep[TYPE<RefImplLayout>::expr] (Pointer::make (r3x)) ;
		assert (rax.mCounter > 0) ;
		fake.mHandle = r3x ;
		fake.mPointer = pointer ;
		ptr (fake).mCounter++ ;
	}

	void destroy () override {
		if ifdo (TRUE) {
			if (fake.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			assert (fake.mPointer != ZERO) ;
			if ifdo (TRUE) {
				const auto r1x = --ptr (fake).mCounter ;
				if (r1x > 0)
					discard ;
				BoxHolder::hold (ptr (fake).mValue)->destroy () ;
				const auto r2x = ptr (fake).mHeap ;
				r2x.free (fake.mHandle) ;
			}
			fake.mHandle = ZERO ;
		}
		fake.mHandle = ZERO ;
		fake.mPointer = ZERO ;
	}

	static VREF<RefImplLayout> ptr (CREF<RefLayout> layout) {
		return Pointer::make (layout.mHandle) ;
	}

	BOOL exist () const override {
		return fake.mPointer != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return BoxHolder::hold (ptr (fake).mValue)->unknown () ;
	}

	VREF<Pointer> self_m () leftvalue {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (fake.mPointer) ;
	}

	CREF<Pointer> pin () const leftvalue override {
		assert (exist ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ptr (fake).mCounter.load () ;
			noop (r1x) ;
			assert (r1x == IDEN) ;
		}
		if ifdo (act) {
			//@warn: reference instance are always unchecked except cv-qualifier
			assert (fake.mHandle == VARIABLE::expr) ;
		}
		return Pointer::make (fake.mPointer) ;
	}
} ;

exports VFat<RefHolder> RefHolder::hold (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::hold (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

static constexpr auto dump_memory_leaks = FUNCTION_dump_memory_leaks () ;

#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> pointer) const {
		return LENGTH (_msize (pointer)) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CREF<csc_pointer_t> pointer) const {
		return LENGTH (malloc_usable_size (pointer)) ;
	}
} ;
#endif

static constexpr auto memsize = FUNCTION_memsize () ;

struct HeapImplLayout {
	Box<std::atomic<VAL>> mStack ;
	Box<std::atomic<VAL>> mLength ;
} ;

class HeapImplRoot implement Pin<HeapImplLayout> {
public:
	imports CREF<HeapImplRoot> instance () ;
} ;

exports CREF<HeapImplRoot> HeapImplRoot::instance () {
	return memorize ([&] () {
		return HeapImplRoot () ;
	}) ;
}

class HeapImplHolder final implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		ptr (fake).mStack.remake () ;
		ptr (fake).mLength.remake () ;
		dump_memory_leaks () ;
	}

	static VREF<HeapImplLayout> ptr (CREF<HeapLayout> layout) {
		return HeapImplRoot::instance ().self ;
	}

	INDEX stack () const override {
		INDEX ret = ptr (fake).mStack.self++ ;
		if ifdo (TRUE) {
			if (ret >= 0)
				discard ;
			ret = ret & VAL_MAX ;
		}
		return move (ret) ;
	}

	LENGTH length () const override {
		return ptr (fake).mLength.self ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = memsize (r1x) ;
		ptr (fake).mLength.self += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> pointer) const override {
		const auto r1x = csc_pointer_t (pointer) ;
		const auto r2x = memsize (r1x) ;
		ptr (fake).mLength.self -= r2x ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

exports CREF<HeapLayout> HeapHolder::instance () {
	return memorize ([&] () {
		HeapLayout ret ;
		ret.mHolder = inline_vptr (HeapImplHolder ()) ;
		HeapHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HeapHolder> HeapHolder::hold (VREF<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapHolder> (rax ,that) ;
}

exports CFat<HeapHolder> HeapHolder::hold (CREF<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapHolder> (rax ,that) ;
}

struct KeyNodeImplLayout implement Proxy {
	FLAG mHead ;
	BOOL mMarked ;
	FLAG mPointer ;
	INDEX mCheck ;
} ;

struct KeyHeadImplLayout implement Proxy {
	FLAG mRoot ;
	FLAG mNext ;
	INDEX mIndex ;
	FLAG mBegin ;
	FLAG mEnd ;
} ;

struct KeyRootImplLayout implement Proxy {
	FLAG mHead ;
	FLAG mKeyNode ;
	FLAG mDefNode ;
	Heap mHeap ;
	BOOL mFinalize ;
} ;

class KeyNodeImplHolder final implement Fat<KeyNodeHolder ,KeyNodeLayout> {
private:
	using GCROOTIMPLLAYOUT_GROUP_SIZE = ENUM<1024> ;

public:
	void initialize () override {
		if (fake.mHandle != ZERO)
			return ;
		auto rax = ZERO ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			const auto r1x = Heap::instance () ;
			rax = r1x.alloc (SIZE_OF<KeyRootImplLayout>::expr) ;
			root_ptr (rax).mHeap = r1x ;
			root_ptr (rax).mFinalize = FALSE ;
			const auto r2x = SIZE_OF<KeyHeadImplLayout>::expr + 2 * SIZE_OF<KeyNodeImplLayout>::expr ;
			rbx = r1x.alloc (r2x) ;
			inline_memset (Pointer::make (rbx) ,r2x) ;
			const auto r3x = rbx + SIZE_OF<KeyHeadImplLayout>::expr ;
			const auto r4x = r3x + SIZE_OF<KeyNodeImplLayout>::expr ;
			head_ptr (rbx).mRoot = rax ;
			head_ptr (rbx).mNext = rbx ;
			head_ptr (rbx).mIndex = NONE ;
			head_ptr (rbx).mBegin = r3x ;
			head_ptr (rbx).mEnd = r3x + 2 * SIZE_OF<KeyNodeImplLayout>::expr ;
			node_ptr (r3x).mHead = rbx ;
			node_ptr (r3x).mCheck = NONE ;
			node_ptr (r4x).mHead = rbx ;
			node_ptr (r4x).mCheck = NONE ;
			root_ptr (rax).mHead = rbx ;
			root_ptr (rax).mKeyNode = r3x ;
			root_ptr (rax).mDefNode = r4x ;
		}
		fake.mHandle = root_ptr (rax).mKeyNode ;
	}

	void initialize (CREF<KeyNodeLayout> root ,CREF<FLAG> pointer) override {
		assert (fake.mHandle == ZERO) ;
		assert (address (fake) == pointer) ;
		const auto r1x = node_ptr (root.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		fake.mHandle = r3x ;
	}

	void destroy () override {
		if (fake.mHandle == ZERO)
			return ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		node_ptr (fake.mHandle).mPointer = node_ptr (r3x).mPointer ;
		node_ptr (fake.mHandle).mCheck++ ;
		if ifdo (TRUE) {
			const auto r4x = root_ptr (r2x).mKeyNode ;
			if (fake.mHandle != r4x)
				discard ;
			root_ptr (r2x).mFinalize = TRUE ;
			remove_all_head (r2x) ;
			remove_root (r2x) ;
		}
		fake.mHandle = ZERO ;
	}

	static VREF<KeyNodeImplLayout> node_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	static VREF<KeyHeadImplLayout> head_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	static VREF<KeyRootImplLayout> root_ptr (CREF<FLAG> handle) {
		return Pointer::make (handle) ;
	}

	INDEX get_index () const override {
		if (fake.mHandle == ZERO)
			return NONE ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		if (head_ptr (r1x).mIndex < 0)
			return NONE ;
		const auto r2x = (fake.mHandle - head_ptr (r1x).mBegin) / SIZE_OF<KeyNodeImplLayout>::expr ;
		return head_ptr (r1x).mIndex + r2x ;
	}

	INDEX get_check () const override {
		if (fake.mHandle == ZERO)
			return ZERO ;
		return node_ptr (fake.mHandle).mCheck ;
	}

	void set_key (CREF<INDEX> index) override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = spwan (index) ;
		node_ptr (r1x).mPointer = address (fake) ;
		fake.mHandle = r1x ;
	}

	void set_def () override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		node_ptr (r3x).mPointer = address (fake) ;
		fake.mHandle = r3x ;
	}

	VREF<KeyNodeLayout> lock (CREF<INDEX> check) leftvalue override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mPointer ;
		const auto r2x = node_ptr (fake.mHandle).mCheck ;
		if ifdo (TRUE) {
			if (r2x >= 0)
				if (r2x == check)
					discard ;
			const auto r3x = node_ptr (fake.mHandle).mHead ;
			const auto r4x = head_ptr (r3x).mRoot ;
			const auto r5x = root_ptr (r4x).mDefNode ;
			fake.mHandle = r5x ;
			const auto r6x = node_ptr (fake.mHandle).mPointer ;
			return Pointer::make (r6x) ;
		}
		return Pointer::make (r1x) ;
	}

	FLAG spwan () const override {
		assert (fake.mHandle != ZERO) ;
		return fake.mHandle ;
	}

	FLAG spwan (CREF<INDEX> index) const override {
		assert (fake.mHandle != ZERO) ;
		const auto r1x = node_ptr (fake.mHandle).mHead ;
		const auto r2x = head_ptr (r1x).mRoot ;
		const auto r3x = root_ptr (r2x).mDefNode ;
		if (index < 0)
			return r3x ;
		assert (!root_ptr (r2x).mFinalize) ;
		const auto r4x = insert_head (index ,r2x) ;
		const auto r5x = head_ptr (r4x).mBegin + index % GCROOTIMPLLAYOUT_GROUP_SIZE::expr * SIZE_OF<KeyNodeImplLayout>::expr ;
		if ifdo (TRUE) {
			if (node_ptr (r5x).mHead != ZERO)
				discard ;
			node_ptr (r5x).mHead = r4x ;
			node_ptr (r5x).mPointer = node_ptr (r3x).mPointer ;
		}
		return r5x ;
	}

	FLAG insert_head (CREF<INDEX> index ,CREF<FLAG> root) const {
		const auto r1x = index / GCROOTIMPLLAYOUT_GROUP_SIZE::expr * GCROOTIMPLLAYOUT_GROUP_SIZE::expr ;
		const auto r2x = root_ptr (root).mHead ;
		FLAG ret = r2x ;
		while (TRUE) {
			if (head_ptr (ret).mIndex == r1x)
				break ;
			ret = head_ptr (ret).mNext ;
			if (ret == r2x)
				break ;
		}
		if ifdo (TRUE) {
			if (head_ptr (ret).mIndex == r1x)
				discard ;
			const auto r3x = root_ptr (root).mHeap ;
			const auto r4x = SIZE_OF<KeyHeadImplLayout>::expr + GCROOTIMPLLAYOUT_GROUP_SIZE::expr * SIZE_OF<KeyNodeImplLayout>::expr ;
			ret = r3x.alloc (r4x) ;
			inline_memset (Pointer::make (ret) ,r4x) ;
			const auto r5x = ret + SIZE_OF<KeyHeadImplLayout>::expr ;
			head_ptr (ret).mRoot = root ;
			head_ptr (ret).mNext = head_ptr (r2x).mNext ;
			head_ptr (r2x).mNext = ret ;
			head_ptr (ret).mIndex = r1x ;
			head_ptr (ret).mBegin = r5x ;
			head_ptr (ret).mEnd = r5x + GCROOTIMPLLAYOUT_GROUP_SIZE::expr * SIZE_OF<KeyNodeImplLayout>::expr ;
		}
		return move (ret) ;
	}

	void remove_root (CREF<FLAG> root) {
		const auto r1x = root_ptr (root).mHeap ;
		r1x.free (root) ;
	}

	void remove_all_head (CREF<FLAG> root) {
		const auto r1x = root_ptr (root).mHead ;
		const auto r2x = root_ptr (root).mHeap ;
		auto rax = r1x ;
		while (TRUE) {
			const auto r3x = head_ptr (rax).mNext ;
			r2x.free (rax) ;
			rax = r3x ;
			if (rax == r1x)
				break ;
		}
	}
} ;

exports VFat<KeyNodeHolder> KeyNodeHolder::hold (VREF<KeyNodeLayout> that) {
	return VFat<KeyNodeHolder> (KeyNodeImplHolder () ,that) ;
}

exports CFat<KeyNodeHolder> KeyNodeHolder::hold (CREF<KeyNodeLayout> that) {
	return CFat<KeyNodeHolder> (KeyNodeImplHolder () ,that) ;
}

class SliceImplHolder final implement Fat<SliceHolder ,SliceLayout> {
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
		const auto r2x = SliceHolder::hold (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r3x = inline_equal (rax ,rbx) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			inline_visit (visitor ,rax) ;
		}
		visitor.leave () ;
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

exports VFat<SliceHolder> SliceHolder::hold (VREF<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::hold (CREF<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

class ExceptionImplHolder final implement Fat<ExceptionHolder ,ExceptionLayout> {
public:
	void initialize (CREF<Slice> what_ ,CREF<Slice> func_ ,CREF<Slice> file_ ,CREF<Slice> line_) override {
		fake.mWhat = what_ ;
		fake.mFunc = func_ ;
		fake.mFile = file_ ;
		fake.mLine = line_ ;
	}

	Slice what () const override {
		return fake.mWhat ;
	}

	Slice func () const override {
		return fake.mFunc ;
	}

	Slice file () const override {
		return fake.mFile ;
	}

	Slice line () const override {
		return fake.mLine ;
	}

	void event () const override {
		unimplemented () ;
	}

	void raise () const override {
		auto &&rax = keep[TYPE<Exception>::expr] (fake) ;
		throw rax ;
	}
} ;

exports VFat<ExceptionHolder> ExceptionHolder::hold (VREF<ExceptionLayout> that) {
	return VFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

exports CFat<ExceptionHolder> ExceptionHolder::hold (CREF<ExceptionLayout> that) {
	return CFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

struct ClazzImplLayout {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeGuid ;
	Slice mTypeName ;
} ;

class ClazzImplHolder final implement Fat<ClazzHolder ,ClazzLayout> {
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
		if (type_size () != ClazzHolder::hold (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::hold (that)->type_align ())
			return FALSE ;
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	FLAG compr (CREF<ClazzLayout> that) const override {
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		inline_visit (visitor ,type_size ()) ;
		inline_visit (visitor ,type_align ()) ;
		inline_visit (visitor ,type_guid ()) ;
		inline_visit (visitor ,type_name ()) ;
		visitor.leave () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::hold (VREF<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

exports CFat<ClazzHolder> ClazzHolder::hold (CREF<ClazzLayout> that) {
	return CFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}
} ;