#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
struct OptionalLayout {
	FLAG mCode ;
	BoxLayout mValue ;

public:
	implicit OptionalLayout () noexcept {
		mCode = 0 ;
	}
} ;

struct OptionalHolder implement Interface {
	imports VFat<OptionalHolder> create (VREF<OptionalLayout> that) ;
	imports CFat<OptionalHolder> create (CREF<OptionalLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value) = 0 ;
	virtual void initialize (CREF<FLAG> code) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG code () const = 0 ;
	virtual CREF<Pointer> poll (CREF<Pointer> item) const = 0 ;
	virtual void fetch (VREF<BoxLayout> item) const = 0 ;
	virtual void store (RREF<BoxLayout> item) const = 0 ;
} ;

template <class A>
class Optional implement OptionalLayout {
protected:
	using OptionalLayout::mThis ;
	TEMP<A> mStorage ;

public:
	implicit Optional () = default ;

	implicit Optional (RREF<A> that) {
		auto rax = Box<A>::make (move (that)) ;
		OptionalHolder::create (thiz)->initialize (move (rax)) ;
	}

	imports Optional error (CREF<FLAG> code) {
		Optional ret ;
		OptionalHolder::create (ret)->initialize (code) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return OptionalHolder::create (thiz)->exist () ;
	}

	FLAG code () const {
		return OptionalHolder::create (thiz)->code () ;
	}

	A fetch () const {
		auto rax = Box<A> () ;
		OptionalHolder::create (thiz)->fetch (rax) ;
		return move (rax.self) ;
	}

	CREF<A> poll (CREF<A> item) const {
		return OptionalHolder::create (thiz)->poll (Pointer::from (item)) ;
	}

	void store (CREF<A> item) const {
		store (move (item)) ;
	}

	void store (RREF<A> item) const {
		auto rax = Box<A>::make (move (item)) ;
		return OptionalHolder::create (thiz)->set (move (rax)) ;
	}
} ;

struct ReflectInvoke implement Interface {
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<Pointer> this_ ,CREF<CaptureLayout> params) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 200 ;
	}
} ;

template <class A>
class ReflectInvokeBinder final implement ReflectInvoke {
public:
	LENGTH rank () const override {
		return COUNT_OF<FUNCTION_PARAMS<A>>::expr ;
	}

	void invoke (CREF<Pointer> this_ ,CREF<CaptureLayout> params) const override {
		auto &&rax = keep[TYPE<CREF<A>>::expr] (this_) ;
		return CAPTURE_WRAP<FUNCTION_PARAMS<A>>::from (params) (rax) ;
	}
} ;

struct FunctionImplLayout ;

struct FunctionLayout implement RefBase<FunctionImplLayout> {} ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> create (VREF<FunctionLayout> that) ;
	imports CFat<FunctionHolder> create (CREF<FunctionLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,RREF<BoxLayout> value) = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<CaptureLayout> params) const = 0 ;
} ;

template <class A>
class FunctionUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectInvokeBinder<A>::expr)
			return unsafe_hold (ReflectInvokeBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class...PARAMS>
class Function implement FunctionLayout {
protected:
	using FunctionLayout::mThis ;

public:
	implicit Function () = default ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Function>>>>
	implicit Function (RREF<ARG1> that) {
		using R1X = FUNCTION_RETURN<ARG1> ;
		using R2X = FUNCTION_PARAMS<ARG1> ;
		require (IS_VOID<R1X>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		FunctionHolder::create (thiz)->initialize (FunctionUnknownBinder<ARG1> () ,move (rax)) ;
	}

	implicit Function (CREF<Function> that) {
		mThis = that.mThis.share () ;
	}

	forceinline VREF<Function> operator= (CREF<Function> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Function (RREF<Function>) = default ;

	forceinline VREF<Function> operator= (RREF<Function>) = default ;

	LENGTH rank () const {
		return FunctionHolder::create (thiz)->rank () ;
	}

	void invoke (XREF<PARAMS>...params) const {
		return FunctionHolder::create (thiz)->invoke (capture (keep[TYPE<PARAMS>::expr] (params)...)) ;
	}

	forceinline void operator() (XREF<PARAMS>...params) const {
		return invoke (keep[TYPE<PARAMS>::expr] (params)...) ;
	}
} ;

template <class...>
trait FUNCTION_WRAP_HELP ;

template <class...A>
trait FUNCTION_WRAP_HELP<TYPE<A...> ,ALWAYS> {
	using RET = Function<A...> ;
} ;

template <class A>
using FUNCTION_WRAP = typename FUNCTION_WRAP_HELP<A ,ALWAYS>::RET ;

struct AutoRefImplLayout ;
struct AutoRefLayout ;

struct AutoRefHolder implement Interface {
	imports VFat<AutoRefHolder> create (VREF<AutoRefLayout> that) ;
	imports CFat<AutoRefHolder> create (CREF<AutoRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value ,CREF<Clazz> clazz) = 0 ;
	virtual void initialize (RREF<AutoRefLayout> that ,CREF<Clazz> clazz) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

struct AutoRefLayout implement RefBase<AutoRefImplLayout> {
	FLAG mPointer ;

public:
	implicit AutoRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~AutoRefLayout () noexcept {
		AutoRefHolder::create (thiz)->destroy () ;
	}

	implicit AutoRefLayout (CREF<AutoRefLayout>) = delete ;

	forceinline VREF<AutoRefLayout> operator= (CREF<AutoRefLayout>) = delete ;

	implicit AutoRefLayout (RREF<AutoRefLayout> that) noexcept :AutoRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AutoRefLayout> operator= (RREF<AutoRefLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis ;
	using AutoRefLayout::mPointer ;

public:
	implicit AutoRef () = default ;

	implicit AutoRef (RREF<AutoRefLayout> that) {
		AutoRefHolder::create (thiz)->initialize (move (that) ,Clazz (TYPE<A>::expr)) ;
	}

	template <class...ARG1>
	imports forceinline AutoRef make (XREF<ARG1>...initval) {
		AutoRef ret ;
		auto rax = Box<A>::make (keep[TYPE<ARG1>::expr] (initval)...) ;
		AutoRefHolder::create (ret)->initialize (move (rax) ,Clazz (TYPE<A>::expr)) ;
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

	forceinline operator VREF<A> () leftvalue {
		return self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return AutoRefHolder::create (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<AutoRef>) = delete ;

	forceinline BOOL operator!= (CREF<AutoRef>) = delete ;
} ;

struct SharedRefImplLayout ;
struct SharedRefLayout ;

struct SharedRefHolder implement Interface {
	imports VFat<SharedRefHolder> create (VREF<SharedRefLayout> that) ;
	imports CFat<SharedRefHolder> create (CREF<SharedRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value) = 0 ;
	virtual void initialize (CREF<SharedRefLayout> that) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG counter () const = 0 ;
	virtual VREF<Pointer> self_m () const leftvalue = 0 ;
	virtual SharedRefLayout weak () const = 0 ;
} ;

struct SharedRefLayout implement RefBase<SharedRefImplLayout> {
	FLAG mPointer ;

public:
	implicit SharedRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~SharedRefLayout () noexcept {
		SharedRefHolder::create (thiz)->destroy () ;
	}

	implicit SharedRefLayout (CREF<SharedRefLayout>) = delete ;

	forceinline VREF<SharedRefLayout> operator= (CREF<SharedRefLayout>) = delete ;

	implicit SharedRefLayout (RREF<SharedRefLayout> that) noexcept :SharedRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<SharedRefLayout> operator= (RREF<SharedRefLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class SharedRef implement SharedRefLayout {
protected:
	using SharedRefLayout::mThis ;
	using SharedRefLayout::mPointer ;

public:
	implicit SharedRef () = default ;

	template <class...ARG1>
	imports forceinline SharedRef make (XREF<ARG1>...initval) {
		SharedRef ret ;
		auto rax = Box<A>::make (keep[TYPE<ARG1>::expr] (initval)...) ;
		SharedRefHolder::create (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	implicit SharedRef (CREF<SharedRef> that) {
		SharedRefHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<SharedRef> operator= (CREF<SharedRef> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit SharedRef (RREF<SharedRef>) = default ;

	forceinline VREF<SharedRef> operator= (RREF<SharedRef>) = default ;

	BOOL exist () const {
		return SharedRefHolder::create (thiz)->exist () ;
	}

	FLAG counter () const {
		return SharedRefHolder::create (thiz)->counter () ;
	}

	VREF<A> self_m () const leftvalue {
		return SharedRefHolder::create (thiz)->self ;
	}

	forceinline operator VREF<A> () const leftvalue {
		return self ;
	}

	forceinline VPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<SharedRef>) = delete ;

	forceinline BOOL operator!= (CREF<SharedRef>) = delete ;

	SharedRef weak () const {
		SharedRef ret ;
		keep[TYPE<VREF<SharedRefLayout>>::expr] (ret) = SharedRefHolder::create (thiz)->weak () ;
		return move (ret) ;
	}
} ;

struct UniqueRefImplLayout ;
struct UniqueRefLayout ;

struct UniqueRefHolder implement Interface {
	imports VFat<UniqueRefHolder> create (VREF<UniqueRefLayout> that) ;
	imports CFat<UniqueRefHolder> create (CREF<UniqueRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value ,RREF<Function<VREF<Pointer>>> dtor) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

struct UniqueRefLayout implement RefBase<UniqueRefImplLayout> {
	FLAG mPointer ;

public:
	implicit UniqueRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~UniqueRefLayout () noexcept {
		UniqueRefHolder::create (thiz)->destroy () ;
	}

	implicit UniqueRefLayout (CREF<UniqueRefLayout>) = delete ;

	forceinline VREF<UniqueRefLayout> operator= (CREF<UniqueRefLayout>) = delete ;

	implicit UniqueRefLayout (RREF<UniqueRefLayout> that) noexcept :UniqueRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<UniqueRefLayout> operator= (RREF<UniqueRefLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class UniqueRef implement UniqueRefLayout {
protected:
	using UniqueRefLayout::mThis ;
	using UniqueRefLayout::mPointer ;

public:
	implicit UniqueRef () = default ;

	template <class ARG1 ,class ARG2>
	explicit UniqueRef (RREF<ARG1> ctor ,RREF<ARG2> dtor) {
		auto rax = Box<A>::make () ;
		auto rbx = Function<VREF<Pointer>> (move (dtor)) ;
		UniqueRefHolder::create (thiz)->initialize (move (rax) ,move (rbx)) ;
		ctor (UniqueRefHolder::create (thiz)->self) ;
	}

	template <class...ARG1>
	imports forceinline UniqueRef make (XREF<ARG1>...initval) {
		UniqueRef ret ;
		auto rax = Box<A>::make () ;
		auto rbx = Function<VREF<Pointer>> () ;
		UniqueRefHolder::create (ret)->initialize (move (rax) ,move (rbx)) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return UniqueRefHolder::create (thiz)->exist () ;
	}

	CREF<A> self_m () const leftvalue {
		return UniqueRefHolder::create (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<UniqueRef>) = delete ;

	forceinline BOOL operator!= (CREF<UniqueRef>) = delete ;
} ;

struct BoxBufferLayout {} ;

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

	forceinline operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return Pointer::from (mBuffer) ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	forceinline BOOL operator== (CREF<BoxBuffer>) = delete ;

	forceinline BOOL operator!= (CREF<BoxBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		assert (operator_between (index ,0 ,size ())) ;
		return mBuffer.m1st[index] ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
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

public:
	implicit RefBufferLayout () noexcept {
		mSize = 0 ;
		mStep = 0 ;
	}
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
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class RefBufferBase implement RefBufferLayout {} ;

template <class A>
class RefBuffer implement RefBufferBase<A> {
protected:
	require (CHECK<A>) ;
	using RefBufferLayout::mBuffer ;
	using RefBufferLayout::mSize ;
	using RefBufferLayout::mStep ;

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

	forceinline operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	forceinline BOOL operator== (CREF<RefBuffer>) = delete ;

	forceinline BOOL operator!= (CREF<RefBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return RefBufferHolder::create (thiz)->resize (RefBufferUnknownBinder<A> () ,size_) ;
	}
} ;

struct AllocatorLayout ;

struct AllocatorNodeLayout {
	INDEX mNext ;
} ;

template <class A>
struct AllocatorNode {
	TEMP<A> mItem ;
	AllocatorNodeLayout mNode ;
} ;

struct AllocatorHolder implement Interface {
	imports VFat<AllocatorHolder> create (VREF<AllocatorLayout> that) ;
	imports CFat<AllocatorHolder> create (CREF<AllocatorLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX alloc (CREF<Unknown> holder) = 0 ;
	virtual void free (CREF<INDEX> index) = 0 ;
	virtual BOOL used (CREF<INDEX> index) const = 0 ;
	virtual void resize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
} ;

struct AllocatorLayout {
	RefBufferLayout mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	implicit AllocatorLayout () noexcept {
		mSize = 0 ;
		mLength = 0 ;
		mFree = NONE ;
	}

	implicit ~AllocatorLayout () noexcept {
		AllocatorHolder::create (thiz)->destroy () ;
	}

	implicit AllocatorLayout (CREF<AllocatorLayout>) = delete ;

	forceinline VREF<AllocatorLayout> operator= (CREF<AllocatorLayout>) = delete ;

	implicit AllocatorLayout (RREF<AllocatorLayout> that) noexcept :AllocatorLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AllocatorLayout> operator= (RREF<AllocatorLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class AllocatorUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectSizeBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectCreateBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectCreateBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectDestroyBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectDestroyBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectNodeBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectNodeBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectRemakeBinder<A>::expr)
			return unsafe_hold (ReflectRemakeBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class Allocator implement AllocatorLayout {
protected:
	require (CHECK<RefBuffer<AllocatorNode<A>>>) ;
	using AllocatorLayout::mAllocator ;
	using AllocatorLayout::mSize ;
	using AllocatorLayout::mLength ;
	using AllocatorLayout::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (CREF<LENGTH> size_) {
		AllocatorHolder::create (thiz)->initialize (AllocatorUnknownBinder<A> () ,size_) ;
	}

	void clear () {
		return AllocatorHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return AllocatorHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return AllocatorHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return AllocatorHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return AllocatorHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return AllocatorHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX alloc () {
		return AllocatorHolder::create (thiz)->alloc (AllocatorUnknownBinder<A> ()) ;
	}

	void free (CREF<INDEX> index) {
		return AllocatorHolder::create (thiz)->free (index) ;
	}

	BOOL used (CREF<INDEX> index) const {
		return AllocatorHolder::create (thiz)->used (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return AllocatorHolder::create (thiz)->resize (AllocatorUnknownBinder<A> () ,size_) ;
	}
} ;
} ;
