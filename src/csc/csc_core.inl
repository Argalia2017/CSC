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

#ifdef __CSC_CXX_RTTI__
struct FUNCTION_stl_type_name {
	forceinline FLAG operator() (CR<Interface> squalor ,CR<FLAG> func_) const noexcept {
		return FLAG (typeid (squalor).name ()) ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_NVCC__
#pragma message "NVCC would not generate type_name without rtti"
#endif

struct FUNCTION_stl_type_name {
	forceinline FLAG operator() (CR<Interface> squalor ,CR<FLAG> func_) const noexcept {
		return func_ ;
	}
} ;
#endif

static constexpr auto stl_type_name = FUNCTION_stl_type_name () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.begin ()) + LENGTH (squalor.size ()) * step_ ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<LENGTH> step_) const noexcept {
		Tuple<FLAG ,FLAG> ret ;
		ret.m1st = FLAG (squalor.begin ()) ;
		ret.m2nd = FLAG (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto stl_list_pair = FUNCTION_stl_list_pair () ;

class CoreProcImplHolder final implement Fat<CoreProcHolder ,Proxy> {
public:
	BOOL inline_unittest () const override {
		return memorize ([&] () {
			return has_debugger () ;
		}) ;
	}

	void inline_abort () const override {
		std::abort () ;
	}

	void inline_notice (VR<Pointer> src) const override {
		//@warn: make side effect
		inline_memcpy (src ,src ,1) ;
	}

	FLAG inline_type_name (CR<Pointer> squalor ,CR<FLAG> func_) const override {
		return stl_type_name (squalor ,func_) ;
	}

	Tuple<FLAG ,FLAG> inline_list_pair (CR<Pointer> squalor ,CR<LENGTH> step_) const override {
		return stl_list_pair (squalor ,step_) ;
	}

	void inline_memset (VR<Pointer> dst ,CR<LENGTH> size_) const override {
		std::memset ((&dst) ,0 ,size_) ;
	}

	void inline_memcpy (VR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const override {
		std::memcpy ((&dst) ,(&src) ,size_) ;
	}

	FLAG inline_memcmp (CR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const override {
		return FLAG (std::memcmp ((&dst) ,(&src) ,size_)) ;
	}
} ;

exports CR<CoreProcHolder> CoreProcHolder::expr_m () {
	return memorize ([&] () {
		return CoreProcImplHolder () ;
	}) ;
}

class BoxImplHolder final implement Fat<BoxHolder ,BoxLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memset (ref ,r1x->type_size ()) ;
	}

	void destroy () override {
		if (self.mHolder == ZERO)
			return ;
		const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
		r1x->destroy (ref ,1) ;
		self.mHolder = ZERO ;
	}

	BOOL exist () const override {
		return self.mHolder != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return Unknown (self.mHolder) ;
	}

	VR<Pointer> ref_m () leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (self) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		//@warn: different from mStorage due to alignment
		const auto r2x = address (self) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	void remake (CR<Unknown> holder) override {
		if (exist ())
			return ;
		self.mHolder = inline_vptr (holder) ;
	}

	void acquire (CR<BoxLayout> that) override {
		assert (!exist ()) ;
		if (that.mHolder == ZERO)
			return ;
		self.mHolder = that.mHolder ;
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		inline_memcpy (ref ,BoxHolder::hold (that)->ref ,r1x->type_size ()) ;
	}

	void release () override {
		self.mHolder = ZERO ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::hold (VR<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::hold (CR<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct RefTree implement Proxy {
	Heap mHeap ;
	std::atomic<VAL> mCounter ;
	BoxLayout mValue ;
} ;

static constexpr auto REFIMPLLAYOUT_MIN_HANDLE = 8 ;

class RefImplHolder final implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RR<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (BoxHolder::hold (item)->unknown ()) ;
		const auto r2x = inline_max (r1x->type_align () - ALIGN_OF<RefTree>::expr ,0) ;
		const auto r3x = SIZE_OF<RefTree>::expr + r2x + r1x->type_size () ;
		const auto r4x = Heap::expr ;
		self.mHandle = r4x.alloc (r3x) ;
		assert (self.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (self.mHandle) ,SIZE_OF<RefTree>::expr) ;
		ptr (self).mHeap = r4x ;
		BoxHolder::hold (ptr (self).mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		self.mLayout = address (BoxHolder::hold (ptr (self).mValue)->ref) ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Unknown> holder ,CR<Unknown> extend ,CR<LENGTH> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = inline_max (r1x->type_align () - ALIGN_OF<RefTree>::expr ,0) ;
		const auto r3x = SIZE_OF<RefTree>::expr + r2x + r1x->type_size () ;
		const auto r4x = RFat<ReflectSize> (extend) ;
		const auto r5x = inline_max (r4x->type_align () - r1x->type_align () ,0) ;
		const auto r6x = r3x + r5x + r4x->type_size () * size_ ;
		const auto r7x = Heap::expr ;
		self.mHandle = r7x.alloc (r6x) ;
		assert (self.mHandle >= REFIMPLLAYOUT_MIN_HANDLE) ;
		inline_memset (Pointer::make (self.mHandle) ,SIZE_OF<RefTree>::expr) ;
		ptr (self).mHeap = r7x ;
		BoxHolder::hold (ptr (self).mValue)->initialize (holder) ;
		self.mLayout = address (BoxHolder::hold (ptr (self).mValue)->ref) ;
		const auto r8x = RFat<ReflectCreate> (holder) ;
		r8x->create (ref ,1) ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Unknown> holder ,CR<FLAG> layout) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_align () ;
		noop (r2x) ;
		assert (r2x <= ALIGN_OF<RefTree>::expr) ;
		const auto r3x = layout - SIZE_OF<RefTree>::expr ;
		auto &&rax = keep[TYPE<RefTree>::expr] (Pointer::make (r3x)) ;
		assert (rax.mCounter > 0) ;
		self.mHandle = r3x ;
		self.mLayout = layout ;
		ptr (self).mCounter++ ;
	}

	void destroy () override {
		if (self.mLayout == ZERO)
			return ;
		if ifdo (TRUE) {
			if (self.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			assert (self.mLayout != ZERO) ;
			if ifdo (TRUE) {
				const auto r1x = --ptr (self).mCounter ;
				if (r1x > 0)
					discard ;
				BoxHolder::hold (ptr (self).mValue)->destroy () ;
				const auto r2x = ptr (self).mHeap ;
				r2x.free (self.mHandle) ;
			}
		}
		self.mHandle = ZERO ;
		self.mLayout = ZERO ;
	}

	static VR<RefTree> ptr (CR<RefLayout> that) {
		return Pointer::make (that.mHandle) ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ++ptr (self).mCounter ;
			noop (r1x) ;
			assert (r1x >= 2) ;
			ret.mHandle = self.mHandle ;
			ret.mLayout = address (BoxHolder::hold (ptr (self).mValue)->ref) ;
		}
		if ifdo (act) {
			ret.mHandle = self.mHandle ;
			ret.mLayout = self.mLayout ;
		}
		return move (ret) ;
	}

	BOOL exist () const override {
		return self.mLayout != ZERO ;
	}

	Unknown unknown () const override {
		assert (exist ()) ;
		return BoxHolder::hold (ptr (self).mValue)->unknown () ;
	}

	VR<Pointer> ref_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	BOOL exclusive () const override {
		assert (exist ()) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mHandle < REFIMPLLAYOUT_MIN_HANDLE)
				discard ;
			const auto r1x = ptr (self).mCounter.load () ;
			return r1x == IDEN ;
		}
		if ifdo (act) {
			//@warn: reference instance are always unchecked except cv-qualifier
			if (self.mHandle != VARIABLE::expr)
				discard ;
			return TRUE ;
		}
		return FALSE ;
	}
} ;

exports VFat<RefHolder> RefHolder::hold (VR<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::hold (CR<RefLayout> that) {
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
	forceinline LENGTH operator() (CR<csc_handle_t> a) const {
		return LENGTH (_msize (a)) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_memsize {
	forceinline LENGTH operator() (CR<csc_handle_t> a) const {
		return LENGTH (malloc_usable_size (a)) ;
	}
} ;
#endif

static constexpr auto memsize = FUNCTION_memsize () ;

struct HeapRoot {
	Box<std::atomic<VAL>> mStack ;
	Box<std::atomic<VAL>> mLength ;
} ;

class HeapImplHolder final implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		root_ptr ().mStack.remake () ;
		root_ptr ().mLength.remake () ;
		dump_memory_leaks () ;
	}

	static VR<HeapRoot> root_ptr () {
		static auto mInstance = HeapRoot () ;
		return mInstance ;
	}

	INDEX stack () const override {
		INDEX ret = root_ptr ().mStack.ref++ ;
		if ifdo (TRUE) {
			if (ret >= 0)
				discard ;
			ret = ret & VAL_MAX ;
		}
		return move (ret) ;
	}

	LENGTH length () const override {
		return root_ptr ().mLength.ref ;
	}

	FLAG alloc (CR<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_handle_t (ret) ;
		const auto r2x = memsize (r1x) ;
		root_ptr ().mLength.ref += r2x ;
		return move (ret) ;
	}

	void free (CR<FLAG> layout) const override {
		const auto r1x = csc_handle_t (layout) ;
		const auto r2x = memsize (r1x) ;
		root_ptr ().mLength.ref -= r2x ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

exports CR<HeapLayout> HeapHolder::expr_m () {
	return memorize ([&] () {
		HeapLayout ret ;
		ret.mHolder = inline_vptr (HeapImplHolder ()) ;
		HeapHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<HeapHolder> HeapHolder::hold (VR<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return VFat<HeapHolder> (rax ,that) ;
}

exports CFat<HeapHolder> HeapHolder::hold (CR<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapHolder> (rax ,that) ;
}

class SliceImplHolder final implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CR<FLAG> buffer ,CR<LENGTH> size_ ,CR<LENGTH> step_) override {
		self.mBuffer = buffer ;
		self.mSize = size_ ;
		self.mStep = step_ ;
	}

	LENGTH size () const override {
		return self.mSize ;
	}

	LENGTH step () const override {
		return self.mStep ;
	}

	void get (CR<INDEX> index ,VR<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStep != SIZE_OF<STRU8>::expr)
				discard ;
			item = bitwise[TYPE<STRU8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (self.mStep != SIZE_OF<STRU16>::expr)
				discard ;
			item = bitwise[TYPE<STRU16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (self.mStep != SIZE_OF<STRU32>::expr)
				discard ;
			item = bitwise[TYPE<STRU32>::expr] (at (index)) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CR<Pointer> at (CR<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	BOOL equal (CR<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r3x = inline_equal (rax ,rbx) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CR<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : range (0 ,r3x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			inline_visit (visitor ,rax) ;
		}
		visitor.leave () ;
	}

	SliceLayout eos () const override {
		SliceLayout ret = self ;
		INDEX ix = 0 ;
		auto rax = STRU32 () ;
		while (TRUE) {
			if (ix >= self.mSize)
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

exports VFat<SliceHolder> SliceHolder::hold (VR<SliceLayout> that) {
	return VFat<SliceHolder> (SliceImplHolder () ,that) ;
}

exports CFat<SliceHolder> SliceHolder::hold (CR<SliceLayout> that) {
	return CFat<SliceHolder> (SliceImplHolder () ,that) ;
}

class ExceptionImplHolder final implement Fat<ExceptionHolder ,ExceptionLayout> {
public:
	void initialize (CR<Slice> what_ ,CR<Slice> func_ ,CR<Slice> file_ ,CR<Slice> line_) override {
		self.mWhat = what_ ;
		self.mFunc = func_ ;
		self.mFile = file_ ;
		self.mLine = line_ ;
	}

	Slice what () const override {
		return self.mWhat ;
	}

	Slice func () const override {
		return self.mFunc ;
	}

	Slice file () const override {
		return self.mFile ;
	}

	Slice line () const override {
		return self.mLine ;
	}

	void event () const override {
		unimplemented () ;
	}

	void raise () const override {
		auto &&rax = keep[TYPE<Exception>::expr] (self) ;
		throw rax ;
	}
} ;

exports VFat<ExceptionHolder> ExceptionHolder::hold (VR<ExceptionLayout> that) {
	return VFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

exports CFat<ExceptionHolder> ExceptionHolder::hold (CR<ExceptionLayout> that) {
	return CFat<ExceptionHolder> (ExceptionImplHolder () ,that) ;
}

struct ClazzTree {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	FLAG mTypeGuid ;
	Slice mTypeName ;
} ;

class ClazzImplHolder final implement Fat<ClazzHolder ,ClazzLayout> {
public:
	void initialize (CR<Unknown> holder) override {
		self.mThis = Ref<ClazzTree>::make () ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		self.mThis->mTypeSize = r1x->type_size () ;
		self.mThis->mTypeAlign = r1x->type_align () ;
		const auto r2x = RFat<ReflectGuid> (holder) ;
		self.mThis->mTypeGuid = r2x->type_guid () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		self.mThis->mTypeName = r3x->type_name () ;
	}

	void initialize (CR<ClazzLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	LENGTH type_size () const override {
		if (self.mThis == NULL)
			return 0 ;
		return self.mThis->mTypeSize ;
	}

	LENGTH type_align () const override {
		if (self.mThis == NULL)
			return 0 ;
		return self.mThis->mTypeAlign ;
	}

	FLAG type_guid () const override {
		if (self.mThis == NULL)
			return ZERO ;
		return self.mThis->mTypeGuid ;
	}

	Slice type_name () const override {
		if (self.mThis == NULL)
			return Slice () ;
		return self.mThis->mTypeName ;
	}

	BOOL equal (CR<ClazzLayout> that) const override {
		if (type_size () != ClazzHolder::hold (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::hold (that)->type_align ())
			return FALSE ;
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	FLAG compr (CR<ClazzLayout> that) const override {
		if (type_guid () != 0)
			if (type_guid () == ClazzHolder::hold (that)->type_guid ())
				return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	void visit (VR<FriendVisitor> visitor) const override {
		visitor.enter () ;
		inline_visit (visitor ,type_size ()) ;
		inline_visit (visitor ,type_align ()) ;
		inline_visit (visitor ,type_guid ()) ;
		inline_visit (visitor ,type_name ()) ;
		visitor.leave () ;
	}
} ;

exports VFat<ClazzHolder> ClazzHolder::hold (VR<ClazzLayout> that) {
	return VFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}

exports CFat<ClazzHolder> ClazzHolder::hold (CR<ClazzLayout> that) {
	return CFat<ClazzHolder> (ClazzImplHolder () ,that) ;
}
} ;