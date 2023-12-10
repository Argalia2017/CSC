#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
template <class A>
class Optional {
protected:
	mutable Box<A> mValue ;
	mutable BOOL mExist ;
	FLAG mCode ;

public:
	implicit Optional () = default ;

	implicit Optional (RREF<A> that) {
		mValue = Box<A>::make (move (that)) ;
		mExist = TRUE ;
		mCode = 0 ;
	}

	imports Optional error (CREF<FLAG> code) {
		Optional ret ;
		ret.mCode = code ;
		return move (ret) ;
	}

	BOOL exist () const {
		if ifnot (mValue.exist ())
			return FALSE ;
		return mExist ;
	}

	FLAG code () const {
		return mCode ;
	}

	A poll () const {
		assert (exist ()) ;
		A ret = move (mValue.self) ;
		mExist = FALSE ;
		return move (ret) ;
	}

	CREF<A> fetch (CREF<A> def) const {
		if ifnot (exist ())
			return def ;
		return mValue.self ;
	}
} ;

struct ReflectInvoke implement Interface {
	virtual FLAG rank () const = 0 ;
	virtual void invoke (CREF<Pointer> this_ ,CREF<CaptureLayout> params) const = 0 ;

	imports FLAG uuid () {
		return 200 ;
	}
} ;

template <class...>
trait CAPTURE_FUNCTION_HELP ;

template <class...A>
trait CAPTURE_FUNCTION_HELP<TYPE<A...> ,ALWAYS> {
	using RET = Capture<A...> ;
} ;

template <class A>
using CAPTURE_FUNCTION = typename CAPTURE_FUNCTION_HELP<A ,ALWAYS>::RET ;

template <class A>
class ReflectInvokeBinder final implement ReflectInvoke {
public:
	FLAG rank () const override {
		return COUNT_OF<FUNCTION_PARAMS<A>>::expr ;
	}

	void invoke (CREF<Pointer> this_ ,CREF<CaptureLayout> params) const override {
		auto &&rax = unsafe_cast[TYPE<A>::expr] (this_) ;
		return CAPTURE_FUNCTION<FUNCTION_PARAMS<A>>::from (params) (rax) ;
	}
} ;

struct FunctionImplLayout ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> create (VREF<Ref<FunctionImplLayout>> that) ;
	imports CFat<FunctionHolder> create (CREF<Ref<FunctionImplLayout>> that) ;

	virtual void initialize (CREF<BoxLayout> value ,CREF<Unknown> holder) = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<CaptureLayout> params) const = 0 ;
} ;

template <class A>
class FunctionUnknownBinder final implement Unknown {
public:
	FLAG unknown (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSize::uuid ())
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroy::uuid ())
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectInvoke::uuid ())
			return unsafe_hold (ReflectInvokeBinder<A> ()) ;
		assert (FALSE) ;
		return 0 ;
	}
} ;

template <class...PARAMS>
class Function implement RefBase<FunctionImplLayout> {
public:
	implicit Function () = default ;

	template <class ARG1>
	implicit Function (RREF<ARG1> that) {
		using R1X = FUNCTION_RETURN<ARG1> ;
		using R2X = FUNCTION_PARAMS<ARG1> ;
		require (IS_VOID<R1X>) ;
		require (ENUM_COMPR_LT<COUNT_OF<R2X> ,RANKX>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		FunctionHolder::create (thiz)->initialize (rax ,FunctionUnknownBinder<ARG1> ()) ;
		rax.release () ;
	}

	implicit Function (CREF<Function> that) {
		mThis = that.mThis.share () ;
	}

	implicit VREF<Function> operator= (CREF<Function> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Function (RREF<Function>) = default ;

	implicit VREF<Function> operator= (RREF<Function>) = default ;

	LENGTH rank () const {
		return FunctionHolder::create (thiz)->rank () ;
	}

	void invoke (XREF<PARAMS>...params) const {
		return FunctionHolder::create (thiz)->invoke (capture (keep[TYPE<XREF<PARAMS>>::expr] (params)...)) ;
	}

	inline void operator() (XREF<PARAMS>...params) const {
		return invoke (keep[TYPE<XREF<PARAMS>>::expr] (params)...) ;
	}
} ;

struct AutoRefImplLayout ;

struct AutoRefLayout {
	Ref<AutoRefImplLayout> mThis ;
	FLAG mPointer ;
} ;

struct AutoRefHolder implement Interface {
	imports VFat<AutoRefHolder> create (VREF<AutoRefLayout> that) ;
	imports CFat<AutoRefHolder> create (CREF<AutoRefLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value ,CREF<Clazz> clazz) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis as (Ref<AutoRefLayoutData>) ;

public:
	implicit AutoRef () = default ;

	implicit AutoRef (RREF<AutoRefLayout> that) :AutoRef (keep[TYPE<RREF<AutoRef>>::expr] (that)) {}

	template <class...ARG1>
	imports AutoRef make (XREF<ARG1>...initval) {
		AutoRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		AutoRefHolder::create (ret)->initialize (rax ,Clazz (TYPE<A>::expr)) ;
		rax.release () ;
		return move (ret) ;
	}

	BOOL exist () const {
		return AutoRefHolder::create (thiz)->exist () ;
	}

	Clazz clazz () const {
		return AutoRefHolder::create (thiz)->clazz () ;
	}

	VREF<A> self_m () leftvalue {
		return AutoRefHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<A> () leftvalue {
		return self ;
	}

	inline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return AutoRefHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<A> () const leftvalue {
		return self ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	inline BOOL operator== (CREF<AutoRef>) = delete ;

	inline BOOL operator!= (CREF<AutoRef>) = delete ;
} ;

struct SharedRefImplLayout ;

struct SharedRefLayout {
	Ref<SharedRefImplLayout> mThis ;
	FLAG mPointer ;
} ;

struct SharedRefHolder implement Interface {
	imports VFat<SharedRefHolder> create (VREF<SharedRefLayout> that) ;
	imports CFat<SharedRefHolder> create (CREF<SharedRefLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value) = 0 ;
	virtual void initialize (CREF<SharedRefLayout> that) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG counter () const = 0 ;
	virtual VREF<Pointer> self_m () const leftvalue = 0 ;
	virtual SharedRefLayout weak () const = 0 ;
} ;

template <class A>
class SharedRef implement SharedRefLayout {
protected:
	using SharedRefLayout::mThis as (Ref<SharedRefLayoutData>) ;

public:
	implicit SharedRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<A ,ARG1>>>
	implicit SharedRef (RREF<SharedRef<ARG1>> that) :SharedRef (keep[TYPE<RREF<SharedRef>>::expr] (that)) {}

	implicit SharedRef (CREF<SharedRef> that) {
		SharedRefHolder::create (thiz)->initialize (that) ;
	}

	implicit VREF<SharedRef> operator= (CREF<SharedRef> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit SharedRef (RREF<SharedRef>) = default ;

	implicit VREF<SharedRef> operator= (RREF<SharedRef>) = default ;

	template <class...ARG1>
	imports SharedRef make (XREF<ARG1>...initval) {
		SharedRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		SharedRefHolder::create (ret)->initialize (rax) ;
		rax.release () ;
		return move (ret) ;
	}

	BOOL exist () const {
		return SharedRefHolder::create (thiz)->exist () ;
	}

	FLAG counter () const {
		return SharedRefHolder::create (thiz)->counter () ;
	}

	VREF<A> self_m () const leftvalue {
		return SharedRefHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<A> () const leftvalue {
		return self ;
	}

	inline VPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	inline BOOL operator== (CREF<SharedRef>) = delete ;

	inline BOOL operator!= (CREF<SharedRef>) = delete ;

	SharedRef weak () const {
		SharedRef ret ;
		ret = SharedRefHolder::create (thiz)->weak () ;
		return move (ret) ;
	}
} ;

struct UniqueRefImplLayout ;

struct UniqueRefLayout {
	Ref<UniqueRefImplLayout> mThis ;
	FLAG mPointer ;
} ;

struct UniqueRefHolder implement Interface {
	imports VFat<UniqueRefHolder> create (VREF<UniqueRefLayout> that) ;
	imports CFat<UniqueRefHolder> create (CREF<UniqueRefLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value) = 0 ;
	virtual void destroy () = 0 ;
	virtual void ownership (RREF<Function<VREF<Pointer>>> dtor) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

template <class A>
class UniqueRef implement UniqueRefLayout {
protected:
	using UniqueRefLayout::mThis as (Ref<UniqueRefLayoutData>) ;

public:
	implicit UniqueRef () = default ;

	template <class ARG1 ,class ARG2>
	explicit UniqueRef (RREF<ARG1> ctor ,RREF<ARG2> dtor) {
		auto rax = Box<A>::make () ;
		UniqueRefHolder::create (thiz)->initialize (rax) ;
		rax.release () ;
		ctor (UniqueRefHolder::create (thiz)->self) ;
		UniqueRefHolder::create (thiz)->ownership (Function<VREF<Pointer>> (move (dtor))) ;
	}

	template <class...ARG1>
	imports UniqueRef make (XREF<ARG1>...initval) {
		UniqueRef ret ;
		auto rax = Box<A>::make () ;
		UniqueRefHolder::create (ret)->initialize (rax) ;
		rax.release () ;
		return move (ret) ;
	}

	BOOL exist () const {
		return UniqueRefHolder::create (thiz)->exist () ;
	}

	CREF<A> self_m () const leftvalue {
		return UniqueRefHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<A> () const leftvalue {
		return self ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	inline BOOL operator== (CREF<UniqueRef>) = delete ;

	inline BOOL operator!= (CREF<UniqueRef>) = delete ;
} ;

class BoxBufferLayout {} ;

template <class A ,class B>
class BoxBuffer implement BoxBufferLayout {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	Tuple<ARR<A ,B>> mBuffer ;

public:
	implicit BoxBuffer () = default ;

	implicit BoxBuffer (CREF<ARR<A ,B>> that) {
		mBuffer = unsafe_cast[TYPE<Tuple<ARR<A ,B>>>::expr] (that) ;
	}

	explicit BoxBuffer (CREF<LENGTH> size_) {
		assert (size_ >= 0) ;
		assert (size_ < B::expr) ;
	}

	LENGTH size () const {
		return B::expr ;
	}

	LENGTH step () const {
		return SIZE_OF<A>::expr ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return Pointer::from (mBuffer) ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return Pointer::from (mBuffer) ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<BoxBuffer>) = delete ;

	inline BOOL operator!= (CREF<BoxBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		assert (FALSE) ;
	}
} ;

struct RefBufferLayout {
	RefLayout mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> create (VREF<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> create (CREF<RefBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
} ;

template <class A>
class RefBufferUnknownBinder final implement Unknown {
public:
	FLAG unknown (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSize::uuid ())
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreate::uuid ())
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroy::uuid ())
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		assert (FALSE) ;
		return 0 ;
	}
} ;

template <class A>
class RefBufferBase implement RefBufferLayout {} ;

template <class A>
class RefBuffer implement RefBufferBase<A> {
protected:
	using RefBufferLayout::mBuffer as (Ref<A>) ;

public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->initialize (RefBufferUnknownBinder<A> () ,size_) ;
	}

	LENGTH size () const {
		return RefBufferHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return RefBufferHolder::create (thiz)->step () ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<RefBuffer>) = delete ;

	inline BOOL operator!= (CREF<RefBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->resize (RefBufferUnknownBinder<A> () ,size_) ;
	}
} ;

struct RefAllocatorLayout {
	RefBufferLayout mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	LENGTH mNextOffset ;
	INDEX mFree ;
} ;

struct RefAllocatorHolder implement Interface {
	imports VFat<RefAllocatorHolder> create (VREF<RefAllocatorLayout> that) ;
	imports CFat<RefAllocatorHolder> create (CREF<RefAllocatorLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void clear () = 0 ;
	virtual INDEX alloc (CREF<Unknown> holder) = 0 ;
	virtual void free (CREF<INDEX> index) = 0 ;
	virtual BOOL used (CREF<INDEX> index) const = 0 ;
	virtual void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
} ;

template <class A>
class RefAllocatorUnknownBinder final implement Unknown {
public:
	FLAG unknown (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSize::uuid ())
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreate::uuid ())
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroy::uuid ())
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectDrop::uuid ())
			return unsafe_hold (ReflectDropBinder<A> ()) ;
		assert (FALSE) ;
		return 0 ;
	}
} ;

template <class A>
class RefAllocatorBase implement RefAllocatorLayout {
public:
	struct NODE {
		A mValue ;
		INDEX mNext ;
	} ;
} ;

template <class A>
class RefAllocator implement RefAllocatorBase<A> {
protected:
	using RefAllocatorLayout::mAllocator as (RefBuffer<NODE>) ;

public:
	implicit RefAllocator () = default ;

	explicit RefAllocator (CREF<LENGTH> size_) {
		RefAllocatorHolder::create (thiz)->initialize (RefAllocatorUnknownBinder<NODE> () ,size_) ;
	}

	LENGTH size () const {
		return RefAllocatorHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return RefAllocatorHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return RefAllocatorHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefAllocatorHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefAllocatorHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void clear () {
		return RefAllocatorHolder::create (thiz)->clear () ;
	}

	INDEX alloc () {
		return RefAllocatorHolder::create (thiz)->alloc (RefAllocatorUnknownBinder<NODE> ()) ;
	}

	void free (CREF<INDEX> index) {
		return RefAllocatorHolder::create (thiz)->free (index) ;
	}

	BOOL used (CREF<INDEX> index) const {
		return RefAllocatorHolder::create (thiz)->used (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return RefAllocatorHolder::create (thiz)->resize (RefAllocatorUnknownBinder<NODE> () ,size_) ;
	}
} ;
} ;
