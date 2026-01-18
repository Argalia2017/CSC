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
	forceinline Bool operator() () const noexcept {
		return IsDebuggerPresent () ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_has_debugger {
	forceinline Bool operator() () const noexcept {
		return FALSE ;
	}
} ;
#endif

static constexpr auto has_debugger = FUNCTION_has_debugger () ;

#ifdef __CSC_CXX_RTTI__
struct FUNCTION_stl_type_name {
	forceinline Flag operator() (CR<Interface> squalor ,CR<Flag> func_) const noexcept {
		return Flag (typeid (squalor).name ()) ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_NVCC__
#pragma message "NVCC would not generate type_name without rtti"
#endif

struct FUNCTION_stl_type_name {
	forceinline Flag operator() (CR<Interface> squalor ,CR<Flag> func_) const noexcept {
		return func_ ;
	}
} ;
#endif

static constexpr auto stl_type_name = FUNCTION_stl_type_name () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<Flag ,Flag> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<Length> step_) const noexcept {
		Tuple<Flag ,Flag> ret ;
		ret.m1st = Flag (squalor.begin ()) ;
		ret.m2nd = Flag (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<Flag ,Flag> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<Length> step_) const noexcept {
		Tuple<Flag ,Flag> ret ;
		ret.m1st = Flag (squalor.begin ()) ;
		ret.m2nd = Flag (squalor.begin ()) + Length (squalor.size ()) * step_ ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_stl_list_pair {
	forceinline Tuple<Flag ,Flag> operator() (CR<csc_initializer_list_t<Pointer>> squalor ,CR<Length> step_) const noexcept {
		Tuple<Flag ,Flag> ret ;
		ret.m1st = Flag (squalor.begin ()) ;
		ret.m2nd = Flag (squalor.end ()) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto stl_list_pair = FUNCTION_stl_list_pair () ;

class CoreProcImplHolder final implement Fat<CoreProcHolder ,Proxy> {
public:
	Bool inline_debug () const override {
		return memorize ([&] () {
			return has_debugger () ;
		}) ;
	}

	void inline_abort () const override {
		std::abort () ;
	}

	void inline_notice (CR<Flag> name ,CR<Flag> addr) const override {
		const auto r1x = csc_string_t (name) ;
		const auto r2x = csc_handle_t (addr) ;
		std::printf ("%s : [0x%p] \n" ,r1x ,r2x) ;
	}

	Flag inline_type_name (CR<Pointer> squalor ,CR<Flag> func_) const override {
		return stl_type_name (squalor ,func_) ;
	}

	Tuple<Flag ,Flag> inline_list_pair (CR<Pointer> squalor ,CR<Length> step_) const override {
		return stl_list_pair (squalor ,step_) ;
	}

	void inline_memset (VR<Pointer> dst ,CR<Length> size_) const override {
		std::memset ((&dst) ,0 ,size_) ;
	}

	void inline_memcpy (VR<Pointer> dst ,CR<Pointer> src ,CR<Length> size_) const override {
		std::memcpy ((&dst) ,(&src) ,size_) ;
	}

	Flag inline_memcmp (CR<Pointer> dst ,CR<Pointer> src ,CR<Length> size_) const override {
		return Flag (std::memcmp ((&dst) ,(&src) ,size_)) ;
	}

	Index inline_guid () const override {
		static auto mInstance = std::atomic<Val64> (IDEN) ;
		return Index (mInstance++) ;
	}
} ;

exports CR<CoreProcHolder> CoreProcHolder::expr_m () {
	static CoreProcImplHolder mInstance ;
	return mInstance ;
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
		if (!exist ())
			return ;
		const auto r1x = RFat<ReflectDestroy> (unknown ()) ;
		r1x->destroy (ref ,1) ;
		self.mHolder = ZERO ;
	}

	Bool exist () const override {
		return self.mHolder != ZERO ;
	}

	Unknown unknown () const override {
		return Unknown (self.mHolder) ;
	}

	VR<Pointer> ref_m () leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = address (self) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		const auto r1x = RFat<ReflectSize> (unknown ()) ;
		const auto r2x = address (self) + SIZE_OF<BoxLayout>::expr ;
		const auto r3x = inline_alignas (r2x ,r1x->type_align ()) ;
		return Pointer::make (r3x) ;
	}

	void remake (CR<Unknown> holder ,CR<Flag> layout) override {
		if (exist ())
			return ;
		self.mHolder = inline_vptr (holder) ;
		assert (layout == address (ref)) ;
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

struct RefTree {
	Heap mHeap ;
	Flag mOrigin ;
	std::atomic<Val> mCounter ;
	BoxLayout mValue ;
} ;

class RefImplHolder final implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RR<BoxLayout> item) override {
		assert (!exist ()) ;
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		const auto r3x = inline_max (r2x->type_align () - ALIGN_OF<RefTree>::expr ,0) ;
		const auto r4x = SIZE_OF<RefTree>::expr + r3x + r2x->type_size () ;
		const auto r5x = Heap::expr ;
		const auto r6x = r5x.alloc (r4x) ;
		self.mLayout = inline_alignas (r6x + SIZE_OF<RefTree>::expr ,r2x->type_align ()) ;
		inline_memset (Pointer::make (r6x) ,self.mLayout - r6x) ;
		ptr (self).mHeap = r5x ;
		ptr (self).mOrigin = r6x ;
		BoxHolder::hold (ptr (self).mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Unknown> holder ,CR<Unknown> extend ,CR<Length> size_) override {
		assert (!exist ()) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = RFat<ReflectSize> (extend) ;
		const auto r3x = inline_max (r1x->type_align () - ALIGN_OF<RefTree>::expr ,0) ;
		const auto r4x = inline_max (r2x->type_align () - r1x->type_align () ,0) ;
		const auto r5x = SIZE_OF<RefTree>::expr + r3x + r1x->type_size () + r4x + r2x->type_size () * size_ ;
		const auto r6x = Heap::expr ;
		const auto r7x = r6x.alloc (r5x) ;
		self.mLayout = inline_alignas (r7x + SIZE_OF<RefTree>::expr ,r1x->type_align ()) ;
		inline_memset (Pointer::make (r7x) ,self.mLayout - r7x) ;
		ptr (self).mHeap = r6x ;
		ptr (self).mOrigin = r7x ;
		BoxHolder::hold (ptr (self).mValue)->initialize (holder) ;
		const auto r8x = RFat<ReflectCreate> (holder) ;
		r8x->create (ref ,1) ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Unknown> holder ,CR<Flag> extend ,CR<Flag> layout) override {
		assert (!exist ()) ;
		self.mLayout = layout ;
		self.mExtend = extend ;
		auto act = TRUE ;
		if ifdo (act) {
			if (ownership ())
				discard ;
			noop () ;
		}
		if ifdo (act) {
			const auto r1x = RFat<ReflectSize> (holder) ;
			const auto r2x = r1x->type_align () ;
			assert (r2x <= ALIGN_OF<RefTree>::expr) ;
			if (ptr (self).mCounter <= 0)
				discard ;
			ptr (self).mCounter++ ;
		}
		if ifdo (act) {
			self.mLayout = ZERO ;
			self.mExtend = ZERO ;
		}
	}

	void destroy () override {
		if (!exist ())
			return ;
		if ifdo (TRUE) {
			if (!ownership ())
				discard ;
			const auto r1x = --ptr (self).mCounter ;
			if (r1x > 0)
				discard ;
			BoxHolder::hold (ptr (self).mValue)->destroy () ;
			const auto r2x = ptr (self).mHeap ;
			const auto r3x = ptr (self).mOrigin ;
			r2x.free (r3x) ;
		}
		self.mLayout = ZERO ;
		self.mExtend = ZERO ;
	}

	static VR<RefTree> ptr (CR<RefLayout> that) {
		assert (that.mLayout != ZERO) ;
		const auto r1x = that.mLayout - SIZE_OF<RefTree>::expr ;
		return Pointer::make (r1x) ;
	}

	RefLayout share () const override {
		RefLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			if (!ownership ())
				discard ;
			const auto r1x = ++ptr (self).mCounter ;
			noop (r1x) ;
			assert (r1x >= 2) ;
		}
		ret.mLayout = self.mLayout ;
		ret.mExtend = self.mExtend ;
		return move (ret) ;
	}

	Bool exist () const override {
		return self.mLayout != ZERO ;
	}

	Unknown unknown () const override {
		assert (ownership ()) ;
		if (self.mExtend == ZERO)
			return BoxHolder::hold (ptr (self).mValue)->unknown () ;
		return Unknown (self.mExtend) ;
	}

	VR<Pointer> ref_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (self.mLayout) ;
	}

	void prepare (CR<Unknown> extend) override {
		assert (ownership ()) ;
		self.mExtend = inline_vptr (extend) ;
	}

	Bool exclusive () const override {
		assert (exist ()) ;
		if (!ownership ())
			return FALSE ;
		const auto r1x = ptr (self).mCounter.load () ;
		if (r1x != IDEN)
			return FALSE ;
		return TRUE ;
	}

	Bool ownership () const override {
		if (self.mExtend == VARIABLE::expr)
			return FALSE ;
		if (self.mExtend == CONSTANT::expr)
			return FALSE ;
		if (self.mExtend == REGISTER::expr)
			return FALSE ;
		return TRUE ;
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
	forceinline Length operator() (CR<csc_handle_t> a) const {
		return Length (_msize (a)) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_memsize {
	forceinline Length operator() (CR<csc_handle_t> a) const {
		return Length (malloc_usable_size (a)) ;
	}
} ;
#endif

static constexpr auto memsize = FUNCTION_memsize () ;

struct HeapRoot {
	Box<std::atomic<Val>> mStack ;
	Box<std::atomic<Val>> mWidth ;
	Box<std::atomic<Val>> mLength ;
} ;

class HeapImplHolder final implement Fat<HeapHolder ,HeapLayout> {
public:
	void initialize () override {
		root_ptr ().mStack.remake () ;
		root_ptr ().mWidth.remake () ;
		root_ptr ().mLength.remake () ;
		dump_memory_leaks () ;
	}

	static VR<HeapRoot> root_ptr () {
		static auto mInstance = HeapRoot () ;
		return mInstance ;
	}

	Flag stack (CR<Length> size_) const override {
		unimplemented () ;
		return ZERO ;
	}

	Length size () const override {
		return root_ptr ().mWidth.ref ;
	}

	Length length () const override {
		return root_ptr ().mLength.ref ;
	}

	Flag alloc (CR<Length> size_) const override {
		Flag ret = Flag (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_handle_t (ret) ;
		const auto r2x = memsize (r1x) ;
		root_ptr ().mLength.ref += r2x ;
		return move (ret) ;
	}

	Flag alloc (CR<Length> size_ ,CR<Length> align_) const override {
		Flag ret = Flag (operator new (size_ ,std::align_val_t (align_) ,std::nothrow)) ;
		assume (ret != ZERO) ;
		const auto r1x = csc_handle_t (ret) ;
		const auto r2x = memsize (r1x) ;
		root_ptr ().mLength.ref += r2x ;
		return move (ret) ;
	}

	void free (CR<Flag> layout) const override {
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
	void initialize (CR<Flag> buffer ,CR<Length> size_ ,CR<Length> step_) override {
		self.mBuffer = buffer ;
		self.mSize = size_ ;
		self.mStep = step_ ;
	}

	Length size () const override {
		return self.mSize ;
	}

	Length step () const override {
		return self.mStep ;
	}

	void get (CR<Index> index ,VR<Stru32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStep != SIZE_OF<Stru8>::expr)
				discard ;
			item = bitwise[TYPE<Stru8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (self.mStep != SIZE_OF<Stru16>::expr)
				discard ;
			item = bitwise[TYPE<Stru16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (self.mStep != SIZE_OF<Stru32>::expr)
				discard ;
			item = bitwise[TYPE<Stru32>::expr] (at (index)) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CR<Pointer> at (CR<Index> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		const auto r1x = self.mBuffer + index * self.mStep ;
		return Pointer::make (r1x) ;
	}

	Bool equal (CR<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = Stru32 () ;
		auto rbx = Stru32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r3x = inline_equal (rax ,rbx) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	Flag compr (CR<SliceLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = SliceHolder::hold (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = Stru32 () ;
		auto rbx = Stru32 () ;
		for (auto &&i : range (0 ,r3x)) {
			get (i ,rax) ;
			SliceHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			inline_visit (visitor ,rax) ;
		}
		visitor.leave () ;
	}

	SliceLayout eos () const override {
		SliceLayout ret = self ;
		Index ix = 0 ;
		auto rax = Stru32 () ;
		while (TRUE) {
			if (ix >= self.mSize)
				break ;
			get (ix ,rax) ;
			if (rax == Stru32 (0X00))
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
	Length mTypeSize ;
	Length mTypeAlign ;
	Flag mTypeMeta ;
	Flag mTypeGuid ;
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
		self.mThis->mTypeMeta = r2x->type_meta () ;
		self.mThis->mTypeGuid = r2x->type_guid () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		self.mThis->mTypeName = r3x->type_name () ;
	}

	void initialize (CR<ClazzLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	Length type_size () const override {
		if (self.mThis == NULL)
			return 0 ;
		return self.mThis->mTypeSize ;
	}

	Length type_align () const override {
		if (self.mThis == NULL)
			return 0 ;
		return self.mThis->mTypeAlign ;
	}

	Flag type_meta () const override {
		if (self.mThis == NULL)
			return ZERO ;
		return self.mThis->mTypeMeta ;
	}

	Flag type_guid () const override {
		if (self.mThis == NULL)
			return ZERO ;
		return self.mThis->mTypeGuid ;
	}

	Slice type_name () const override {
		if (self.mThis == NULL)
			return Slice () ;
		return self.mThis->mTypeName ;
	}

	Bool equal (CR<ClazzLayout> that) const override {
		if (type_size () != ClazzHolder::hold (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::hold (that)->type_align ())
			return FALSE ;
		if (type_meta () == ClazzHolder::hold (that)->type_meta ())
			return TRUE ;
		return inline_equal (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	Flag compr (CR<ClazzLayout> that) const override {
		if (type_meta () == ClazzHolder::hold (that)->type_meta ())
			return ZERO ;
		return inline_compr (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	void visit (CR<Visitor> visitor) const override {
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

class ScopeImplHolder final implement Fat<ScopeHolder ,ScopeLayout> {
public:
	void initialize (CR<Unknown> holder ,CR<Flag> layout) override {
		self.mHolder = inline_vptr (holder) ;
		const auto r1x = RFat<ReflectScope> (unknown ()) ;
		r1x->enter (Pointer::make (layout)) ;
		self.mLayout = layout ;
	}

	void destroy () override {
		if (self.mLayout == ZERO)
			return ;
		const auto r1x = RFat<ReflectScope> (unknown ()) ;
		r1x->leave (Pointer::make (self.mLayout)) ;
		self.mLayout = ZERO ;
	}

	Bool exist () const override {
		return self.mLayout != ZERO ;
	}

	Unknown unknown () const {
		return Unknown (self.mHolder) ;
	}
} ;

exports VFat<ScopeHolder> ScopeHolder::hold (VR<ScopeLayout> that) {
	return VFat<ScopeHolder> (ScopeImplHolder () ,that) ;
}

exports CFat<ScopeHolder> ScopeHolder::hold (CR<ScopeLayout> that) {
	return CFat<ScopeHolder> (ScopeImplHolder () ,that) ;
}
} ;