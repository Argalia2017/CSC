#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_core.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <malloc.h>
#include <typeinfo>
#include <initializer_list>
#include <atomic>
#include <mutex>
#include <csignal>
#include <exception>

#ifdef __CSC_SYSTEM_LINUX__
#include <fcntl.h>
#endif
#include "csc_begin.h"

#ifdef __CSC_SYSTEM_LINUX__
namespace std {
inline namespace {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
} ;
} ;
#endif

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
exports Bool CoreProc::inline_debug () {
	return memorize ([&] () {
		return IsDebuggerPresent () ;
	}) ;
}
#endif

#ifdef __CSC_SYSTEM_LINUX__
exports Bool CoreProc::inline_debug () {
	return memorize ([&] () {
		auto rax = Buffer<char ,ENUM<4096>> () ;
		if ifdo (TRUE) {
			const auto r1x = std::open ("/proc/self/status" ,O_RDONLY) ;
			if (r1x < 0)
				discard ;
			const auto r2x = Length (std::read (r1x ,rax ,csc_size_t (rax.size () - 1))) ;
			std::close (r1x) ;
			if (r2x <= 0)
				discard ;
			rax[r2x] = 0 ;
			const auto r3x = std::strstr (rax ,"TracerPid:") ;
			if (r3x == NULL)
				discard ;
			const auto r4x = std::atoi (r3x + sizeof ("TracerPid:") - 1) ;
			if (r4x == 0)
				discard ;
			return TRUE ;
		}
		return FALSE ;
	}) ;
}
#endif

exports void CoreProc::inline_crash () {
	std::raise (SIGABRT) ;
	std::quick_exit (-1) ;
}

#ifdef __CSC_SYSTEM_WINDOWS__
exports void CoreProc::inline_notice (CR<Flag> name ,CR<Flag> addr) {
	if ifdo (TRUE) {
		const auto r1x = csc_string_t (name) ;
		const auto r2x = csc_handle_t (addr) ;
		const auto r3x = Index (bitwise (Pointer::make (addr))) ;
		const auto r4x = Val64 (r3x) ;
		std::printf ("%s [0X%p] : %lld\n" ,r1x ,r2x ,r4x) ;
	}
}
#endif

#ifdef __CSC_SYSTEM_LINUX__
exports void CoreProc::inline_notice (CR<Flag> name ,CR<Flag> addr) {
	if ifdo (TRUE) {
		const auto r1x = csc_string_t (name) ;
		const auto r2x = csc_handle_t (addr) ;
		const auto r3x = Index (bitwise (Pointer::make (addr))) ;
		const auto r4x = Val64 (r3x) ;
		std::printf ("%s [%p] : %lld\n" ,r1x ,r2x ,r4x) ;
	}
}
#endif

#ifdef __CSC_CXX_RTTI__
exports Flag CoreProc::inline_type_name (CR<Interface> squalor ,CR<Flag> func_) {
	return Flag (typeid (squalor).name ()) ;
}
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_NVCC__
#pragma message "NVCC would not generate type_name without rtti"
#endif

exports Flag CoreProc::inline_type_name (CR<Interface> squalor ,CR<Flag> func_) {
	return func_ ;
}
#endif

#ifdef __CSC_COMPILER_MSVC__
exports Tuple<Flag ,Flag> CoreProc::inline_list_pair (CR<Pointer> squalor ,CR<Length> step_) {
	Tuple<Flag ,Flag> ret ;
	auto rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	ret.m1st = Flag (rax.begin ()) ;
	ret.m2nd = Flag (rax.end ()) ;
	return move (ret) ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
exports Tuple<Flag ,Flag> CoreProc::inline_list_pair (CR<Pointer> squalor ,CR<Length> step_) {
	Tuple<Flag ,Flag> ret ;
	auto rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	ret.m1st = Flag (rax.begin ()) ;
	ret.m2nd = Flag (rax.begin ()) + Length (rax.size ()) * step_ ;
	return move (ret) ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
exports Tuple<Flag ,Flag> CoreProc::inline_list_pair (CR<Pointer> squalor ,CR<Length> step_) {
	Tuple<Flag ,Flag> ret ;
	auto rax = keep[TYPE<std::initializer_list<Pointer>>::expr] (squalor) ;
	ret.m1st = Flag (rax.begin ()) ;
	ret.m2nd = Flag (rax.end ()) ;
	return move (ret) ;
}
#endif

exports void CoreProc::inline_memset (VR<Pointer> dst ,CR<Length> size_) {
	std::memset ((&dst) ,0 ,size_) ;
}

exports void CoreProc::inline_memcpy (VR<Pointer> dst ,CR<Pointer> src ,CR<Length> size_) {
	std::memcpy ((&dst) ,(&src) ,size_) ;
}

exports Flag CoreProc::inline_memcmp (CR<Pointer> dst ,CR<Pointer> src ,CR<Length> size_) {
	return Flag (std::memcmp ((&dst) ,(&src) ,size_)) ;
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
		assert (!exist ()) ;
		self.mHolder = inline_vptr (holder) ;
		assert (layout == address (ref)) ;
	}

	void acquire (CR<BoxLayout> that) override {
		assert (!exist ()) ;
		if (!BoxHolder::hold (that)->exist ())
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
	Flag mMemPin ;
	std::atomic<Val> mCounter ;
	BoxLayout mValue ;
} ;

class RefImplHolder final implement Fat<RefHolder ,RefLayout> {
public:
	void initialize (RR<BoxLayout> item) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (ownership ())
				discard ;
			self.mExtend = ORDINARY::expr ;
		}
		const auto r1x = BoxHolder::hold (item)->unknown () ;
		const auto r2x = RFat<ReflectSize> (r1x) ;
		const auto r3x = inline_max (r2x->type_align () - ALIGN_OF<RefTree>::expr ,0) ;
		const auto r4x = SIZE_OF<RefTree>::expr + r3x + r2x->type_size () ;
		const auto r5x = Heap::expr ;
		const auto r6x = r5x.alloc (r4x) ;
		self.mLayout = inline_alignas (r6x + SIZE_OF<RefTree>::expr ,r2x->type_align ()) ;
		inline_memset (Pointer::make (r6x) ,self.mLayout - r6x) ;
		ptr (self).mHeap = r5x ;
		ptr (self).mMemPin = r4x * 1024 + (address (ptr (self)) - r6x) ;
		BoxHolder::hold (ptr (self).mValue)->acquire (item) ;
		BoxHolder::hold (item)->release () ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Unknown> holder ,CR<Unknown> extend ,CR<Length> size_) override {
		assert (!exist ()) ;
		if ifdo (TRUE) {
			if (ownership ())
				discard ;
			self.mExtend = ORDINARY::expr ;
		}
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
		ptr (self).mMemPin = r5x * 1024 + (address (ptr (self)) - r7x) ;
		BoxHolder::hold (ptr (self).mValue)->initialize (holder) ;
		const auto r8x = RFat<ReflectCreate> (holder) ;
		r8x->create (ref ,1) ;
		ptr (self).mCounter = 1 ;
	}

	void initialize (CR<Flag> extend ,CR<Flag> layout) override {
		assert (!exist ()) ;
		self.mLayout = layout ;
		self.mExtend = extend ;
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
			const auto r3x = ptr (self).mMemPin ;
			const auto r4x = address (ptr (self)) - r3x % 1024 ;
			const auto r5x = r3x / 1024 ;
			r2x.free (r4x ,r5x) ;
		}
		self.mLayout = ZERO ;
		self.mExtend = ZERO ;
	}

	static VR<RefTree> ptr (CR<RefLayout> that) {
		const auto r1x = that.mLayout - SIZE_OF<RefTree>::expr ;
		return Pointer::make (r1x) ;
	}

	Bool exist () const override {
		return self.mLayout != ZERO ;
	}

	Unknown unknown () const override {
		assert (ownership ()) ;
		if (self.mExtend != ORDINARY::expr)
			return Unknown (self.mExtend) ;
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

	Bool ownership () const override {
		if (self.mExtend == ORDINARY::expr)
			return TRUE ;
		if (self.mExtend == VARIABLE::expr)
			return FALSE ;
		if (self.mExtend == CONSTANT::expr)
			return FALSE ;
		if (self.mExtend == REGISTER::expr)
			return FALSE ;
		return TRUE ;
	}

	Bool exclusive () const override {
		if (!exist ())
			return TRUE ;
		if (!ownership ())
			return FALSE ;
		const auto r1x = ptr (self).mCounter.load () ;
		if (r1x != IDEN)
			return FALSE ;
		return TRUE ;
	}

	void intrusive (CR<Unknown> extend) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (!ownership ())
				discard ;
			self.mExtend = inline_vptr (extend) ;
		}
		if ifdo (act) {
			if (!exist ())
				discard ;
			if (ptr (self).mCounter <= 0)
				discard ;
			const auto r1x = ++ptr (self).mCounter ;
			noop (r1x) ;
			assert (r1x >= 1) ;
			self.mExtend = inline_vptr (extend) ;
		}
		if ifdo (act) {
			self.mLayout = ZERO ;
			self.mExtend = ZERO ;
		}
	}

	void reveal () override {
		if ifdo (TRUE) {
			if (exist ())
				if (!ownership ())
					discard ;
			self.mExtend = ORDINARY::expr ;
		}
	}
} ;

exports VFat<RefHolder> RefHolder::hold (VR<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::hold (CR<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}

struct HeapNode ;
using HeapNodePtr = DEF<HeapNode *> ;

struct HeapNode {
	Flag mHeader ;
	Flag mStackPtr ;
	HeapNodePtr mPrev ;
	HeapNodePtr mNext ;
} ;

struct HeapImplLayout {
	Pin<HeapImplLayout> mPin ;
	Box<std::recursive_mutex> mMutex ;
	Box<std::atomic<Val>> mWidth ;
	Box<std::atomic<Val>> mLength ;
	Flag mStackRoot ;
	Flag mStackRest ;
	HeapNodePtr mStackTop ;

public:
	static VR<HeapImplLayout> expr_m () ;
} ;

inline VR<HeapImplLayout> HeapImplLayout::expr_m () {
	static auto mInstance = HeapImplLayout () ;
	return mInstance ;
}

static constexpr auto HEAP_HEADER = Flag (0XF0F0F0F0CCCCCCCC) ;

class HeapImplHolder final implement Fat<HeapHolder ,HeapImplLayout> {
public:
	void initialize () override {
		self.mMutex.remake () ;
		self.mWidth.remake () ;
		self.mLength.remake () ;
		self.mStackRoot = 0 ;
		self.mStackRest = 0 ;
		self.mStackTop = NULL ;
		dump_memory_leaks () ;
	}

#ifdef __CSC_COMPILER_MSVC__
	void dump_memory_leaks () const {
		_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
	}
#endif

#ifdef __CSC_COMPILER_GNUC__
	void dump_memory_leaks () const {
		noop () ;
	}
#endif

#ifdef __CSC_COMPILER_CLANG__
	void dump_memory_leaks () const {
		noop () ;
	}
#endif

	void enter () const override {
		return self.mPin->mMutex->lock () ;
	}

	void leave () const override {
		return self.mPin->mMutex->unlock () ;
	}

	Length size () const override {
		return self.mWidth.ref ;
	}

	Length length () const override {
		return self.mLength.ref ;
	}

	Flag stack (CR<Length> size_) const override {
		assert (size_ > 0) ;
		Scope anonymous (thiz) ;
		auto rax = HeapNodePtr (NULL) ;
		const auto r1x = address (rax) ;
		if ifdo (TRUE) {
			if (self.mStackRoot != ZERO)
				discard ;
			self.mPin->mStackRest = 4 * 1024 * 1024 ;
			self.mPin->mStackRoot = alloc (self.mStackRest) ;
			self.mPin->mStackTop = NULL ;
		}
		const auto r3x = SIZE_OF<HeapNode>::expr + size_ ;
		assume (self.mStackRest >= r3x) ;
		rax = self.mStackTop ;
		while (TRUE) {
			if (rax == NULL)
				break ;
			if (r1x <= rax->mStackPtr)
				break ;
			assert (rax->mHeader == HEAP_HEADER) ;
			self.mPin->mStackRest += Flag (rax->mNext) - Flag (rax) ;
			rax = rax->mPrev ;
		}
		self.mPin->mStackTop = rax ;
		if ifdo (TRUE) {
			if (rax != NULL)
				discard ;
			rax = HeapNodePtr (self.mStackRoot)  ;
			rax->mPrev = NULL ;
			rax->mNext = rax ;
		}
		rax = rax->mNext ;
		rax->mHeader = HEAP_HEADER ;
		rax->mStackPtr = r1x ;
		rax->mPrev = self.mStackTop ;
		rax->mNext = HeapNodePtr (Flag (rax) + r3x) ;
		self.mPin->mStackTop = rax ;
		self.mPin->mStackRest -= r3x ;
		return Flag (rax) + SIZE_OF<HeapNode>::expr ;
	}

	Flag alloc (CR<Length> size_) const override {
		assert (size_ > 0) ;
		Flag ret = Flag (operator new (size_ ,std::nothrow)) ;
		assume (ret != ZERO) ;
		self.mPin->mLength.ref += size_ ;
		self.mPin->mWidth.ref = inline_max (self.mWidth.ref ,self.mLength.ref) ;
		return move (ret) ;
	}

#ifdef __CSC_CXX_LATEST__
	Flag alloc (CR<Length> size_ ,CR<Length> align_) const override {
		assert (size_ > 0) ;
		Flag ret = Flag (operator new (size_ ,std::align_val_t (align_) ,std::nothrow)) ;
		assume (ret != ZERO) ;
		self.mPin->mLength.ref += size_ ;
		self.mPin->mWidth.ref = inline_max (self.mWidth.ref ,self.mLength.ref) ;
		return move (ret) ;
	}
#endif

#ifndef __CSC_CXX_LATEST__
	Flag alloc (CR<Length> size_ ,CR<Length> align_) const override {
		assume (FALSE) ;
		return ZERO ;
	}
#endif

	void free (CR<Flag> layout ,CR<Length> size_) const override {
		assert (size_ > 0) ;
		const auto r1x = csc_handle_t (layout) ;
		self.mPin->mLength.ref -= size_ ;
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
	return VFat<HeapHolder> (rax ,HeapImplLayout::expr) ;
}

exports CFat<HeapHolder> HeapHolder::hold (CR<HeapLayout> that) {
	assert (that.mHolder != ZERO) ;
	auto &&rax = keep[TYPE<HeapImplHolder>::expr] (Pointer::from (that.mHolder)) ;
	return CFat<HeapHolder> (rax ,HeapImplLayout::expr) ;
}

class SliceImplHolder final implement Fat<SliceHolder ,SliceLayout> {
public:
	void initialize (CR<Flag> buffer ,CR<Length> size_ ,CR<Length> step_) override {
		self.mBuffer = buffer ;
		self.mSize = size_ ;
		self.mStep = step_ ;
	}

	Flag offset (CR<Index> index) const override {
		return self.mBuffer + index * self.mStep ;
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
			if (self.mStep != 1)
				discard ;
			item = Stru (bitwise (at (index))) ;
		}
		if ifdo (act) {
			if (self.mStep != 2)
				discard ;
			item = Stru16 (bitwise (at (index))) ;
		}
		if ifdo (act) {
			if (self.mStep != 4)
				discard ;
			item = Stru32 (bitwise (at (index))) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CR<Pointer> at (CR<Index> index) const leftvalue {
		assert (self.mBuffer != ZERO) ;
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
		const auto r1x = self.mBuffer != ZERO ? self.mSize : ZERO ;
		while (TRUE) {
			if (ix >= r1x)
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
	void initialize (CR<Slice> what_ ,CR<Slice> func_) override {
		self.mWhat = what_ ;
		self.mFunc = func_ ;
		self.mFile = slice ("???") ;
		self.mLine = slice ("0") ;
	}

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
		std::terminate () ;
	}

	void raise () const override {
		if ifdo (TRUE) {
			const auto r1x = std::current_exception () ;
			if (!Bool (r1x))
				discard ;
			std::rethrow_exception (r1x) ;
		}
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
		self.mThis->mTypeGuid = r2x->type_guid () ;
		const auto r3x = RFat<ReflectName> (holder) ;
		self.mThis->mTypeName = r3x->type_name () ;
	}

	void initialize (CR<ClazzLayout> that) override {
		if (that.mThis == NULL)
			return ;
		self.mThis = Ref<ClazzTree>::reference (that.mThis.ref) ;
		self.mThis.intrusive (that.mThis.unknown ()) ;
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

	Flag type_guid () const override {
		if (self.mThis == NULL)
			return ZERO ;
		return self.mThis->mTypeGuid ;
	}

	Flag type_expr () const override {
		if (self.mThis == NULL)
			return ZERO ;
		const auto r1x = Unknown (type_guid ()) ;
		const auto r2x = RFat<ReflectGuid> (r1x) ;
		return r2x->type_expr () ;
	}

	Slice type_name () const override {
		if (self.mThis == NULL)
			return Slice () ;
		return self.mThis->mTypeName ;
	}

	Bool equal (CR<ClazzLayout> that) const override {
		if (type_guid () == ClazzHolder::hold (that)->type_guid ())
			return TRUE ;
		if (type_size () != ClazzHolder::hold (that)->type_size ())
			return FALSE ;
		if (type_align () != ClazzHolder::hold (that)->type_align ())
			return FALSE ;
		return inline_equal (type_name () ,ClazzHolder::hold (that)->type_name ()) ;
	}

	Flag compr (CR<ClazzLayout> that) const override {
		if (type_guid () == ClazzHolder::hold (that)->type_guid ())
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
		if (!exist ())
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