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

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<FLAG> code) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG code () const = 0 ;
	virtual CREF<Pointer> fetch (CREF<Pointer> item) const leftvalue = 0 ;
	virtual void poll (VREF<BoxLayout> item) const = 0 ;
	virtual void store (RREF<BoxLayout> item) const = 0 ;
} ;

template <class A>
class Optional implement OptionalLayout {
protected:
	using OptionalLayout::mCode ;
	using OptionalLayout::mValue ;
	Temp<A> mStorage ;

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

	CREF<A> fetch (CREF<A> item) const leftvalue {
		return OptionalHolder::create (thiz)->fetch (Pointer::from (item)) ;
	}

	A poll () const {
		auto rax = Box<A> () ;
		OptionalHolder::create (thiz)->poll (rax) ;
		return move (rax.self) ;
	}

	forceinline operator A () const rightvalue {
		return poll () ;
	}

	void store (CREF<A> item) const {
		store (move (item)) ;
	}

	void store (RREF<A> item) const {
		auto rax = Box<A>::make (move (item)) ;
		return OptionalHolder::create (thiz)->store (move (rax)) ;
	}
} ;

struct ReflectInvoke implement Interface {
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<Pointer> func ,CREF<CaptureLayout> params) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 200 ;
	}
} ;

template <class A>
class ReflectInvokeBinder implement ReflectInvoke {
public:
	LENGTH rank () const override {
		return RANK_OF<FUNCTION_PARAMS<A>>::expr ;
	}

	void invoke (CREF<Pointer> func ,CREF<CaptureLayout> params) const override {
		auto &&tmp = capture_from (FUNCTION_PARAMS<A>::expr ,params) ;
		return tmp (keep[TYPE<A>::expr] (func)) ;
	}

	template <class...ARG1>
	CREF<Capture<ARG1...>> capture_from (TYPE<ARG1...> ,CREF<CaptureLayout> params) const {
		return Pointer::from (params) ;
	}
} ;

struct FunctionPureLayout ;

struct FunctionLayout {
	Ref<FunctionPureLayout> mThis ;
} ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> create (VREF<FunctionLayout> that) ;
	imports CFat<FunctionHolder> create (CREF<FunctionLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item ,CREF<Unknown> functor) = 0 ;
	virtual void initialize (CREF<FunctionLayout> that) = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<CaptureLayout> params) const = 0 ;
} ;

template <class A>
struct FunctionUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectInvokeBinder<A>::expr)
			return inline_code (ReflectInvokeBinder<A> ()) ;
		return ZERO ;
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
		require (IS_VOID<R1X>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		FunctionHolder::create (thiz)->initialize (move (rax) ,FunctionUnknownBinder<ARG1> ()) ;
	}

	implicit Function (CREF<Function> that) {
		FunctionHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Function> operator= (CREF<Function> that) {
		return assign (thiz ,that) ;
	}

	implicit Function (RREF<Function> that) = default ;

	forceinline VREF<Function> operator= (RREF<Function> that) = default ;

	LENGTH rank () const {
		return FunctionHolder::create (thiz)->rank () ;
	}

	void invoke (XREF<PARAMS>...params) const {
		return FunctionHolder::create (thiz)->invoke (Capture<XREF<PARAMS>...> (params...)) ;
	}

	forceinline void operator() (XREF<PARAMS>...params) const {
		return invoke (keep[TYPE<XREF<PARAMS>>::expr] (params)...) ;
	}
} ;

struct AutoRefPureLayout ;

struct AutoRefLayout {
	Ref<AutoRefPureLayout> mThis ;
	FLAG mPointer ;

public:
	implicit AutoRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~AutoRefLayout () noexcept ;

	implicit AutoRefLayout (CREF<AutoRefLayout> that) = delete ;

	forceinline VREF<AutoRefLayout> operator= (CREF<AutoRefLayout> that) = delete ;

	implicit AutoRefLayout (RREF<AutoRefLayout> that) noexcept :AutoRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AutoRefLayout> operator= (RREF<AutoRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AutoRefHolder implement Interface {
	imports VFat<AutoRefHolder> create (VREF<AutoRefLayout> that) ;
	imports CFat<AutoRefHolder> create (CREF<AutoRefLayout> that) ;

	virtual void initialize (RREF<UnknownHolder> that) = 0 ;
	virtual void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz) = 0 ;
	virtual void initialize (RREF<AutoRefLayout> that ,CREF<Clazz> clazz) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VREF<Pointer> raw () leftvalue = 0 ;
	virtual CREF<Pointer> raw () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

inline AutoRefLayout::~AutoRefLayout () noexcept {
	AutoRefHolder::create (thiz)->destroy () ;
}

template <class A>
struct AutoRefUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_code (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectCodeBinder<A>::expr)
			return inline_code (ReflectCodeBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_code (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis ;
	using AutoRefLayout::mPointer ;

public:
	implicit AutoRef () = default ;

	implicit AutoRef (RREF<UnknownHolder> that) {
		AutoRefHolder::create (thiz)->initialize (move (that)) ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,A>>>>
	implicit AutoRef (RREF<AutoRef<ARG1>> that) {
		AutoRefHolder::create (thiz)->initialize (move (that) ,Clazz (TYPE<A>::expr)) ;
	}

	template <class...ARG1>
	imports AutoRef make (ARG1 &&...initval) {
		AutoRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		AutoRefHolder::create (ret)->initialize (move (rax) ,Clazz (TYPE<A>::expr)) ;
		return move (ret) ;
	}

	imports UnknownHolder expression () {
		//@mark
		return AutoRefUnknownBinder<A> () ;
	}

	implicit AutoRef (CREF<AutoRef> that) = delete ;

	forceinline VREF<AutoRef> operator= (CREF<AutoRef> that) = delete ;

	implicit AutoRef (RREF<AutoRef> that) = default ;

	forceinline VREF<AutoRef> operator= (RREF<AutoRef> that) = default ;

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

	forceinline BOOL operator== (CREF<AutoRef> that) = delete ;

	forceinline BOOL operator!= (CREF<AutoRef> that) = delete ;
} ;

struct SharedRefPureLayout ;

struct SharedRefLayout {
	Ref<SharedRefPureLayout> mThis ;
	FLAG mPointer ;

public:
	implicit SharedRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~SharedRefLayout () noexcept ;

	implicit SharedRefLayout (CREF<SharedRefLayout> that) = delete ;

	forceinline VREF<SharedRefLayout> operator= (CREF<SharedRefLayout> that) = delete ;

	implicit SharedRefLayout (RREF<SharedRefLayout> that) noexcept :SharedRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<SharedRefLayout> operator= (RREF<SharedRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct SharedRefHolder implement Interface {
	imports VFat<SharedRefHolder> create (VREF<SharedRefLayout> that) ;
	imports CFat<SharedRefHolder> create (CREF<SharedRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<SharedRefLayout> that) = 0 ;
	virtual void initialize (RREF<SharedRefLayout> that ,CREF<FLAG> offset) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,VREF<Pointer> pointer) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG counter () const = 0 ;
	virtual VREF<Pointer> raw () leftvalue = 0 ;
	virtual CREF<Pointer> raw () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () const leftvalue = 0 ;
	virtual SharedRefLayout weak () const = 0 ;
} ;

inline SharedRefLayout::~SharedRefLayout () noexcept {
	SharedRefHolder::create (thiz)->destroy () ;
}

template <class A>
class SharedRef implement SharedRefLayout {
protected:
	using SharedRefLayout::mThis ;
	using SharedRefLayout::mPointer ;

public:
	implicit SharedRef () = default ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,A>>>>
	implicit SharedRef (RREF<SharedRef<ARG1>> that) {
		require (IS_EXTEND<A ,ARG1>) ;
		const auto r1x = SIZE_OF<A>::expr ;
		const auto r2x = FLAG (CPTR<A> (CPTR<ARG1> (r1x))) - r1x ;
		SharedRefHolder::create (thiz)->initialize (move (that) ,r2x) ;
	}

	template <class...ARG1>
	imports SharedRef make (ARG1 &&...initval) {
		SharedRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		SharedRefHolder::create (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	imports SharedRef intrusive (VREF<A> that) {
		SharedRef ret ;
		SharedRefHolder::create (ret)->initialize (BoxUnknownBinder<A> () ,Pointer::from (that)) ;
		return move (ret) ;
	}

	implicit SharedRef (CREF<SharedRef> that) {
		SharedRefHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<SharedRef> operator= (CREF<SharedRef> that) {
		return assign (thiz ,that) ;
	}

	implicit SharedRef (RREF<SharedRef> that) = default ;

	forceinline VREF<SharedRef> operator= (RREF<SharedRef> that) = default ;

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

	forceinline BOOL operator== (CREF<SharedRef> that) = delete ;

	forceinline BOOL operator!= (CREF<SharedRef> that) = delete ;

	SharedRef weak () const {
		SharedRefLayout ret = SharedRefHolder::create (thiz)->weak () ;
		return move (keep[TYPE<SharedRef>::expr] (ret)) ;
	}
} ;

struct UniqueRefPureLayout ;

struct UniqueRefLayout {
	Ref<UniqueRefPureLayout> mThis ;
	FLAG mPointer ;

public:
	implicit UniqueRefLayout () noexcept {
		mPointer = ZERO ;
	}

	implicit ~UniqueRefLayout () noexcept ;

	implicit UniqueRefLayout (CREF<UniqueRefLayout> that) = delete ;

	forceinline VREF<UniqueRefLayout> operator= (CREF<UniqueRefLayout> that) = delete ;

	implicit UniqueRefLayout (RREF<UniqueRefLayout> that) noexcept :UniqueRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<UniqueRefLayout> operator= (RREF<UniqueRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct UniqueRefHolder implement Interface {
	imports VFat<UniqueRefHolder> create (VREF<UniqueRefLayout> that) ;
	imports CFat<UniqueRefHolder> create (CREF<UniqueRefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item ,RREF<Function<VREF<Pointer>>> dtor) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<Pointer> raw () leftvalue = 0 ;
	virtual CREF<Pointer> raw () const leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

inline UniqueRefLayout::~UniqueRefLayout () noexcept {
	UniqueRefHolder::create (thiz)->destroy () ;
}

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
		ctor (UniqueRefHolder::create (thiz)->raw ()) ;
	}

	template <class...ARG1>
	imports UniqueRef make (ARG1 &&...initval) {
		UniqueRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
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

	forceinline BOOL operator== (CREF<UniqueRef> that) = delete ;

	forceinline BOOL operator!= (CREF<UniqueRef> that) = delete ;
} ;

struct BoxBufferLayout {} ;

template <class A ,class B>
class BoxBuffer implement BoxBufferLayout {
protected:
	Tuple<ARR<A ,B>> mBuffer ;

public:
	implicit BoxBuffer () = default ;

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

	VREF<Pointer> raw () leftvalue {
		return Pointer::from (mBuffer) ;
	}

	CREF<Pointer> raw () const leftvalue {
		return Pointer::from (mBuffer) ;
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

	forceinline BOOL operator== (CREF<BoxBuffer> that) = delete ;

	forceinline BOOL operator!= (CREF<BoxBuffer> that) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return self[index] ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return self[index] ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		assert (FALSE) ;
	}
} ;

struct RefBufferPureLayout ;

struct RefBufferLayout {
	Ref<RefBufferPureLayout> mThis ;
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;

public:
	implicit RefBufferLayout () noexcept {
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}

	implicit ~RefBufferLayout () noexcept ;

	implicit RefBufferLayout (CREF<RefBufferLayout> that) = delete ;

	forceinline VREF<RefBufferLayout> operator= (CREF<RefBufferLayout> that) = delete ;

	implicit RefBufferLayout (RREF<RefBufferLayout> that) noexcept :RefBufferLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefBufferLayout> operator= (RREF<RefBufferLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> create (VREF<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> create (CREF<RefBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual UnknownHolder unknown () const = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> raw () leftvalue = 0 ;
	virtual CREF<Pointer> raw () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual VREF<Pointer> bt (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> bt (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline RefBufferLayout::~RefBufferLayout () noexcept {
	RefBufferHolder::create (thiz)->destroy () ;
}

template <class A>
class RefBufferWrap implement RefBufferLayout {} ;

template <class A>
class RefBuffer implement RefBufferWrap<A> {
protected:
	using RefBufferLayout::mThis ;
	using RefBufferLayout::mBuffer ;
	using RefBufferLayout::mSize ;
	using RefBufferLayout::mStep ;

public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,size_) ;
	}

	BOOL exist () const {
		return RefBufferHolder::create (thiz)->exist () ;
	}

	UnknownHolder unknown () const {
		return RefBufferHolder::create (thiz)->unknown () ;
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

	forceinline BOOL operator== (CREF<RefBuffer> that) = delete ;

	forceinline BOOL operator!= (CREF<RefBuffer> that) = delete ;

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

	VREF<Pointer> bt (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::create (thiz)->bt (index) ;
	}

	CREF<Pointer> bt (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::create (thiz)->bt (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->initialize (RefUnknownBinder<A> ()) ;
		return RefBufferHolder::create (thiz)->resize (size_) ;
	}
} ;

struct FarBufferLayout {
	Function<CREF<INDEX> ,VREF<Pointer>> mGetter ;
	Function<CREF<INDEX> ,CREF<Pointer>> mSetter ;
	Ref<Pointer> mBuffer ;
	INDEX mIndex ;
	LENGTH mSize ;
	LENGTH mStep ;

public:
	implicit FarBufferLayout () = default ;

	implicit ~FarBufferLayout () noexcept ;

	implicit FarBufferLayout (CREF<FarBufferLayout> that) = delete ;

	forceinline VREF<FarBufferLayout> operator= (CREF<FarBufferLayout> that) = delete ;

	implicit FarBufferLayout (RREF<FarBufferLayout> that) noexcept {
		swap (thiz ,that) ;
	}

	forceinline VREF<FarBufferLayout> operator= (RREF<FarBufferLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct FarBufferHolder implement Interface {
	imports VFat<FarBufferHolder> create (VREF<FarBufferLayout> that) ;
	imports CFat<FarBufferHolder> create (CREF<FarBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual UnknownHolder unknown () const = 0 ;
	virtual void use_getter (RREF<FunctionLayout> getter) = 0 ;
	virtual void use_setter (RREF<FunctionLayout> setter) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline FarBufferLayout::~FarBufferLayout () noexcept {
	FarBufferHolder::create (thiz)->destroy () ;
}

template <class A>
class FarBuffer implement FarBufferLayout {
protected:
	using FarBufferLayout::mGetter ;
	using FarBufferLayout::mSetter ;
	using FarBufferLayout::mBuffer as (Ref<A>) ;
	using FarBufferLayout::mIndex ;
	using FarBufferLayout::mSize ;
	using FarBufferLayout::mStep ;

public:
	implicit FarBuffer () = default ;

	explicit FarBuffer (CREF<LENGTH> size_) {
		FarBufferHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,size_) ;
	}

	void use_getter (RREF<Function<CREF<INDEX> ,VREF<A>>> getter) {
		FarBufferHolder::create (thiz)->use_getter (move (getter)) ;
	}

	void use_setter (RREF<Function<CREF<INDEX> ,CREF<A>>> setter) {
		FarBufferHolder::create (thiz)->use_setter (move (setter)) ;
	}

	BOOL exist () const {
		return FarBufferHolder::create (thiz)->exist () ;
	}

	UnknownHolder unknown () const {
		return FarBufferHolder::create (thiz)->unknown () ;
	}

	LENGTH size () const {
		return FarBufferHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return FarBufferHolder::create (thiz)->step () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return FarBufferHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		FarBufferHolder::create (thiz)->initialize (RefUnknownBinder<A> ()) ;
		return FarBufferHolder::create (thiz)->resize (size_) ;
	}
} ;

struct AllocatorNode {
	INDEX mNext ;
} ;

struct AllocatorLayout {
	RefBuffer<Pointer> mAllocator ;
	LENGTH mRest ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	implicit AllocatorLayout () noexcept {
		mRest = 0 ;
		mLength = 0 ;
		mFree = NONE ;
	}

	implicit ~AllocatorLayout () noexcept ;

	implicit AllocatorLayout (CREF<AllocatorLayout> that) = delete ;

	forceinline VREF<AllocatorLayout> operator= (CREF<AllocatorLayout> that) = delete ;

	implicit AllocatorLayout (RREF<AllocatorLayout> that) noexcept :AllocatorLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<AllocatorLayout> operator= (RREF<AllocatorLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AllocatorHolder implement Interface {
	imports VFat<AllocatorHolder> create (VREF<AllocatorLayout> that) ;
	imports CFat<AllocatorHolder> create (CREF<AllocatorLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual UnknownHolder unknown () const = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual VREF<Pointer> bt (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> bt (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX alloc () = 0 ;
	virtual INDEX alloc (RREF<BoxLayout> item) = 0 ;
	virtual void free (CREF<INDEX> index) = 0 ;
	virtual BOOL used (CREF<INDEX> index) const = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline AllocatorLayout::~AllocatorLayout () noexcept {
	AllocatorHolder::create (thiz)->destroy () ;
}

template <class A ,class B>
struct AllocatorUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = Tuple<Temp<A> ,B> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_code (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B>
class AllocatorWrap implement AllocatorLayout {} ;

template <class A ,class B>
class Allocator implement AllocatorWrap<A ,B> {
protected:
	using AllocatorLayout::mAllocator as (RefBuffer<Tuple<Temp<A> ,B>>) ;
	using AllocatorLayout::mRest ;
	using AllocatorLayout::mLength ;
	using AllocatorLayout::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (CREF<LENGTH> size_) {
		AllocatorHolder::create (thiz)->initialize (AllocatorUnknownBinder<A ,B> () ,size_) ;
	}

	BOOL exist () const {
		return AllocatorHolder::create (thiz)->exist () ;
	}

	UnknownHolder unknown () const {
		return AllocatorHolder::create (thiz)->unknown () ;
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

	VREF<B> bt (CREF<INDEX> index) leftvalue {
		return AllocatorHolder::create (thiz)->bt (index) ;
	}

	CREF<B> bt (CREF<INDEX> index) const leftvalue {
		return AllocatorHolder::create (thiz)->bt (index) ;
	}

	INDEX alloc () {
		AllocatorHolder::create (thiz)->initialize (AllocatorUnknownBinder<A ,B> ()) ;
		return AllocatorHolder::create (thiz)->alloc () ;
	}

	void free (CREF<INDEX> index) {
		return AllocatorHolder::create (thiz)->free (index) ;
	}

	BOOL used (CREF<INDEX> index) const {
		return AllocatorHolder::create (thiz)->used (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		AllocatorHolder::create (thiz)->initialize (AllocatorUnknownBinder<A ,B> ()) ;
		return AllocatorHolder::create (thiz)->resize (size_) ;
	}
} ;
} ;
