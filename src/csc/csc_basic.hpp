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
	Pin<BoxLayout> mValue ;

public:
	implicit OptionalLayout () noexcept {
		mCode = 0 ;
	}

	implicit OptionalLayout (CREF<OptionalLayout> that) = delete ;

	forceinline VREF<OptionalLayout> operator= (CREF<OptionalLayout> that) = delete ;

	implicit OptionalLayout (RREF<OptionalLayout> that) noexcept :OptionalLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<OptionalLayout> operator= (RREF<OptionalLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct OptionalHolder implement Interface {
	imports VFat<OptionalHolder> create (VREF<OptionalLayout> that) ;
	imports CFat<OptionalHolder> create (CREF<OptionalLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<FLAG> code) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual FLAG code () const = 0 ;
	virtual void poll (VREF<BoxLayout> item) const = 0 ;
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

	A poll () const {
		auto rax = Box<A> () ;
		OptionalHolder::create (thiz)->poll (rax) ;
		return move (rax.self) ;
	}

	forceinline operator A () const rightvalue {
		return poll () ;
	}

	void once (RREF<A> item) const {
		if (exist ())
			return ;
		auto rax = Box<A>::make (move (item)) ;
		OptionalHolder::create (thiz)->poll (rax) ;
	}

	template <class ARG1>
	void then (CREF<ARG1> func) const {
		if ((!exist ()))
			return ;
		auto rax = Box<A> () ;
		OptionalHolder::create (thiz)->poll (rax) ;
		func (rax.self) ;
		OptionalHolder::create (thiz)->poll (rax) ;
	}

	template <class ARG1>
	forceinline CREF<Optional> operator() (CREF<ARG1> func) const {
		then (func) ;
		return thiz ;
	}
} ;

struct BufferLayout {} ;

template <class A ,class B>
class Buffer implement BufferLayout {
protected:
	ARR<A ,ENUM_MAX<B ,RANK1>> mBuffer ;

public:
	implicit Buffer () = default ;

	template <class...ARG1 ,class = REQUIRE<ENUM_EQUAL<RANK_OF<TYPE<ARG1...>> ,B>>>
	implicit Buffer (XREF<ARG1>...initval) :mBuffer {A (initval)...} {}

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

	forceinline BOOL operator== (CREF<Buffer> that) = delete ;

	forceinline BOOL operator!= (CREF<Buffer> that) = delete ;

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
} ;

template <class A>
using Buffer1 = Buffer<A ,RANK1> ;
template <class A>
using Buffer2 = Buffer<A ,RANK2> ;
template <class A>
using Buffer3 = Buffer<A ,RANK3> ;
template <class A>
using Buffer4 = Buffer<A ,RANK4> ;
template <class A>
using Buffer5 = Buffer<A ,RANK5> ;
template <class A>
using Buffer6 = Buffer<A ,RANK6> ;
template <class A>
using Buffer7 = Buffer<A ,RANK7> ;
template <class A>
using Buffer8 = Buffer<A ,RANK8> ;
template <class A>
using Buffer9 = Buffer<A ,RANK9> ;
template <class A>
using BufferX = Buffer<A ,RANKX> ;

struct WrapperLayout {
	LENGTH mRank ;
} ;

template <class...A>
class Wrapper implement WrapperLayout {
private:
	using RANK = RANK_OF<TYPE<A...>> ;

protected:
	using WrapperLayout::mRank ;
	Buffer<FLAG ,RANK> mWrapper ;

public:
	implicit Wrapper () = delete ;

	explicit Wrapper (CREF<Buffer<FLAG ,RANK>> that) {
		mRank = RANK::expr ;
		mWrapper = that ;
	}

	template <class ARG1>
	void invoke (CREF<ARG1> func) const {
		return invoke_impl (func ,TYPE_SENQUENCE<RANK>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	void invoke_impl (CREF<ARG1> func ,TYPE<ARG2...>) const {
		return func (keep[TYPE<XREF<A>>::expr] (Pointer::make (mWrapper[ARG2::expr]))...) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		return invoke (func) ;
	}
} ;

template <class...ARG1>
inline Wrapper<CREF<ARG1>...> MakeWrapper (CREF<ARG1>...params) {
	using R1X = RANK_OF<TYPE<ARG1...>> ;
	return Wrapper<CREF<ARG1>...> (Buffer<FLAG ,R1X> ({address (params)...})) ;
}

template <class A>
class WrapperIterator implement IndexIteratorLayout {
protected:
	using IndexIteratorLayout::mBegin ;
	using IndexIteratorLayout::mEnd ;
	using IndexIteratorLayout::mPeek ;

public:
	implicit WrapperIterator () = delete ;

	explicit WrapperIterator (CREF<WrapperLayout> that) {
		mBegin = address (that) + SIZE_OF<WrapperLayout>::expr ;
		mEnd = mBegin + that.mRank * SIZE_OF<FLAG>::expr ;
		mPeek = mBegin ;
	}

	LENGTH length () const {
		return (mEnd - mBegin) / SIZE_OF<FLAG>::expr ;
	}

	WrapperIterator begin () const {
		return thiz ;
	}

	WrapperIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<WrapperIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CREF<WrapperIterator>) const {
		return good () ;
	}

	CREF<A> peek () const leftvalue {
		const auto r1x = inline_hold (Pointer::make (mPeek)) ;
		return Pointer::make (r1x) ;
	}

	forceinline CREF<A> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek += SIZE_OF<FLAG>::expr ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct ReflectInvoke implement Interface {
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<Pointer> func ,CREF<WrapperLayout> params) const = 0 ;

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

	void invoke (CREF<Pointer> func ,CREF<WrapperLayout> params) const override {
		assert (params.mRank == rank ()) ;
		auto &&tmp = wrapper_from (FUNCTION_PARAMS<A>::expr ,params) ;
		return tmp (keep[TYPE<A>::expr] (func)) ;
	}

	template <class...ARG1>
	imports CREF<Wrapper<ARG1...>> wrapper_from (TYPE<ARG1...> ,CREF<WrapperLayout> params) {
		return Pointer::from (params) ;
	}
} ;

struct FunctionImplLayout ;

struct FunctionLayout {
	Ref<FunctionImplLayout> mThis ;
} ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> create (VREF<FunctionLayout> that) ;
	imports CFat<FunctionHolder> create (CREF<FunctionLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item ,CREF<Unknown> reflect) = 0 ;
	virtual void initialize (CREF<FunctionLayout> that) = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual void invoke (CREF<WrapperLayout> params) const = 0 ;
} ;

template <class A>
class FunctionUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectInvokeBinder<A>::expr)
			return inline_hold (ReflectInvokeBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class...A>
class Function implement FunctionLayout {
protected:
	using FunctionLayout::mThis ;

public:
	implicit Function () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_EXTEND<FunctionLayout ,ARG1>>>
	implicit Function (RREF<ARG1> that) {
		using R1X = FUNCTION_RETURN<ARG1> ;
		using R2X = FUNCTION_PARAMS<ARG1> ;
		require (IS_VOID<R1X>) ;
		require (IS_SAME<R2X ,TYPE<A...>>) ;
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

	VREF<BoxLayout> raw () leftvalue {
		return FunctionHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return FunctionHolder::create (thiz)->raw () ;
	}

	LENGTH rank () const {
		return FunctionHolder::create (thiz)->rank () ;
	}

	void invoke (XREF<A>...params) const {
		return FunctionHolder::create (thiz)->invoke (MakeWrapper (params...)) ;
	}

	forceinline void operator() (XREF<A>...params) const {
		return invoke (keep[TYPE<XREF<A>>::expr] (params)...) ;
	}
} ;

struct ReflectRecast implement Interface {
	virtual FLAG recast (CREF<FLAG> pointer) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 201 ;
	}
} ;

template <class A ,class B>
class ReflectRecastBinder implement ReflectRecast {
public:
	FLAG recast (CREF<FLAG> pointer) const override {
		auto &&tmp = keep[TYPE<B>::expr] (Pointer::make (pointer)) ;
		return offset_impl (PHX ,TYPE<A>::expr ,tmp) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_EXTEND<ARG1 ,ARG2>>>
	forceinline FLAG offset_impl (CREF<typeof (PH3)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return FLAG (PTR<CREF<ARG1>> (&b)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<HAS_SELF<ARG2>>>
	forceinline FLAG offset_impl (CREF<typeof (PH2)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return address (b.self) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_SAME<ARG1 ,Pointer>>>
	forceinline FLAG offset_impl (CREF<typeof (PH1)> ,TYPE<ARG1> ,CREF<ARG2> b) const {
		return address (b) ;
	}
} ;

struct AutoRefImplLayout ;

struct AutoRefLayout {
	Ref<AutoRefImplLayout> mThis ;
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

	virtual void initialize (RREF<BoxLayout> item ,CREF<Clazz> clazz_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> rebind (CREF<Clazz> clazz_) leftvalue = 0 ;
	virtual CREF<Pointer> rebind (CREF<Clazz> clazz_) const leftvalue = 0 ;
	virtual AutoRefLayout recast (CREF<Unknown> reflect) = 0 ;
} ;

inline AutoRefLayout::~AutoRefLayout () noexcept {
	AutoRefHolder::create (thiz)->destroy () ;
}

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis ;
	using AutoRefLayout::mPointer ;

public:
	implicit AutoRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit AutoRef (RREF<AutoRef<ARG1>> that) :AutoRef (that.recast (TYPE<A>::expr)) {}

	template <class...ARG1>
	imports AutoRef make (XREF<ARG1>...initval) {
		AutoRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		AutoRefHolder::create (ret)->initialize (move (rax) ,Clazz (TYPE<A>::expr)) ;
		return move (ret) ;
	}

	implicit AutoRef (CREF<AutoRef> that) = delete ;

	forceinline VREF<AutoRef> operator= (CREF<AutoRef> that) = delete ;

	implicit AutoRef (RREF<AutoRef> that) = default ;

	forceinline VREF<AutoRef> operator= (RREF<AutoRef> that) = default ;

	BOOL exist () const {
		return AutoRefHolder::create (thiz)->exist () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return AutoRefHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return AutoRefHolder::create (thiz)->raw () ;
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

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return AutoRefHolder::create (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<AutoRef> that) = delete ;

	forceinline BOOL operator!= (CREF<AutoRef> that) = delete ;

	template <class ARG1>
	VREF<AutoRef<ARG1>> rebind (TYPE<ARG1>) leftvalue {
		return AutoRefHolder::create (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	CREF<AutoRef<ARG1>> rebind (TYPE<ARG1>) const leftvalue {
		return AutoRefHolder::create (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	AutoRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>>::create () ;
		AutoRefLayout ret = AutoRefHolder::create (thiz)->recast (r1x) ;
		return move (keep[TYPE<AutoRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct SharedRefImplLayout ;

struct SharedRefLayout {
	Ref<SharedRefImplLayout> mThis ;
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
	virtual void initialize (CREF<Unknown> holder ,VREF<Pointer> pointer) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual FLAG counter () const = 0 ;
	virtual VREF<Pointer> self_m () const leftvalue = 0 ;
	virtual SharedRefLayout recast (CREF<Unknown> reflect) = 0 ;
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

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit SharedRef (RREF<SharedRef<ARG1>> that) :SharedRef (that.recast (TYPE<A>::expr)) {}

	template <class...ARG1>
	imports SharedRef make (XREF<ARG1>...initval) {
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

	VREF<BoxLayout> raw () leftvalue {
		return SharedRefHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return SharedRefHolder::create (thiz)->raw () ;
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

	forceinline PTR<VREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<SharedRef> that) = delete ;

	forceinline BOOL operator!= (CREF<SharedRef> that) = delete ;

	template <class ARG1>
	SharedRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>>::create () ;
		SharedRefLayout ret = SharedRefHolder::create (thiz)->recast (r1x) ;
		return move (keep[TYPE<SharedRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct UniqueRefImplLayout ;

struct UniqueRefLayout {
	Ref<UniqueRefImplLayout> mThis ;
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

	virtual void initialize (RREF<BoxLayout> item ,CREF<FunctionLayout> dtor) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual UniqueRefLayout recast (CREF<Unknown> reflect) = 0 ;
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

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit UniqueRef (RREF<UniqueRef<ARG1>> that) :UniqueRef (that.recast (TYPE<A>::expr)) {}

	template <class ARG1 ,class ARG2>
	explicit UniqueRef (RREF<ARG1> ctor ,RREF<ARG2> dtor) {
		auto rax = Box<A>::make () ;
		auto rbx = Function<VREF<A>> (move (dtor)) ;
		UniqueRefHolder::create (thiz)->initialize (move (rax) ,move (rbx)) ;
		ctor (BoxHolder::create (raw ())->self) ;
	}

	template <class...ARG1>
	imports UniqueRef make (XREF<ARG1>...initval) {
		UniqueRef ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		auto rbx = Function<VREF<A>> () ;
		UniqueRefHolder::create (ret)->initialize (move (rax) ,move (rbx)) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return UniqueRefHolder::create (thiz)->exist () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return UniqueRefHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return UniqueRefHolder::create (thiz)->raw () ;
	}

	CREF<A> self_m () const leftvalue {
		return UniqueRefHolder::create (thiz)->self ;
	}

	forceinline operator CREF<A> () const leftvalue {
		return self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	forceinline BOOL operator== (CREF<UniqueRef> that) = delete ;

	forceinline BOOL operator!= (CREF<UniqueRef> that) = delete ;

	template <class ARG1>
	UniqueRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = SimpleUnknownBinder<ReflectRecastBinder<ARG1 ,A>>::create () ;
		UniqueRefLayout ret = UniqueRefHolder::create (thiz)->recast (r1x) ;
		return move (keep[TYPE<UniqueRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct ReflectElement implement Interface {
	virtual RFat<Unknown> unknown () const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 202 ;
	}
} ;

template <class A>
class ReflectElementBinder implement ReflectElement {
public:
	RFat<Unknown> unknown () const override {
		return RFat<Unknown> (BoxUnknownBinder<A> () ,NULL) ;
	}
} ;

struct RefBufferImplLayout ;

struct RefBufferLayout {
	Ref<RefBufferImplLayout> mThis ;
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
	virtual void initialize (CREF<Unknown> element ,RREF<BoxLayout> item) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual RFat<Unknown> unknown () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
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
class BufferUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_hold (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class RefBufferRealLayout implement RefBufferLayout {} ;

template <class A>
class RefBuffer implement RefBufferRealLayout<A> {
protected:
	using RefBufferRealLayout<A>::mThis ;
	using RefBufferRealLayout<A>::mBuffer ;
	using RefBufferRealLayout<A>::mSize ;
	using RefBufferRealLayout<A>::mStep ;

public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		RefBufferHolder::create (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	imports RefBuffer reference (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<Unknown> element) {
		RefBuffer ret ;
		RefBufferHolder::create (ret)->initialize (element) ;
		const auto r1x = RFat<ReflectSize> (element) ;
		ret.mBuffer = buffer ;
		ret.mSize = size_ ;
		ret.mStep = r1x->type_size () ;
		return move (ret) ;
	}

	BOOL exist () const {
		return RefBufferHolder::create (thiz)->exist () ;
	}

	RFat<Unknown> unknown () const {
		return RefBufferHolder::create (thiz)->unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return RefBufferHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return RefBufferHolder::create (thiz)->raw () ;
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
} ;

struct FarBufferHolder implement Interface {
	imports VFat<FarBufferHolder> create (VREF<FarBufferLayout> that) ;
	imports CFat<FarBufferHolder> create (CREF<FarBufferLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual RFat<Unknown> unknown () const = 0 ;
	virtual void use_getter (CREF<FunctionLayout> getter) = 0 ;
	virtual void use_setter (CREF<FunctionLayout> setter) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual void refresh () = 0 ;
} ;

template <class A>
class FarBufferRealLayout implement FarBufferLayout {
public:
	implicit FarBufferRealLayout () noexcept {
		auto &&tmp = keep[TYPE<Ref<A>>::expr] (Pointer::from (thiz.mBuffer)) ;
		tmp = Ref<A> () ;
	}
} ;

template <class A>
class FarBuffer implement FarBufferRealLayout<A> {
protected:
	using FarBufferRealLayout<A>::mGetter ;
	using FarBufferRealLayout<A>::mSetter ;
	using FarBufferRealLayout<A>::mBuffer ;
	using FarBufferRealLayout<A>::mIndex ;
	using FarBufferRealLayout<A>::mSize ;
	using FarBufferRealLayout<A>::mStep ;

public:
	implicit FarBuffer () = default ;

	explicit FarBuffer (CREF<LENGTH> size_) {
		FarBufferHolder::create (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	void use_getter (CREF<Function<CREF<INDEX> ,VREF<A>>> getter) {
		return FarBufferHolder::create (thiz)->use_getter (getter) ;
	}

	void use_setter (CREF<Function<CREF<INDEX> ,CREF<A>>> setter) {
		return FarBufferHolder::create (thiz)->use_setter (setter) ;
	}

	BOOL exist () const {
		return FarBufferHolder::create (thiz)->exist () ;
	}

	RFat<Unknown> unknown () const {
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

	void refresh () {
		return FarBufferHolder::create (thiz)->refresh () ;
	}
} ;

template <class A ,class B>
struct MainTuple implement Tuple<Temp<A> ,B> {} ;

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
	virtual RFat<Unknown> unknown () const = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual VREF<Pointer> bt (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> bt (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX alloc (RREF<BoxLayout> item) = 0 ;
	virtual void free (CREF<INDEX> index) = 0 ;
	virtual BOOL used (CREF<INDEX> index) const = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

inline AllocatorLayout::~AllocatorLayout () noexcept {
	AllocatorHolder::create (thiz)->destroy () ;
}

template <class A ,class B>
class AllocatorUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = MainTuple<A ,B> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_hold (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_hold (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_hold (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_hold (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B>
class AllocatorRealLayout implement AllocatorLayout {
private:
	using RealType = RefBuffer<Tuple<Temp<A> ,B>> ;

public:
	implicit AllocatorRealLayout () noexcept ;
} ;

template <class A ,class B>
inline AllocatorRealLayout<A ,B>::AllocatorRealLayout () noexcept {
	auto &&tmp = keep[TYPE<RealType>::expr] (Pointer::from (thiz.mAllocator)) ;
	tmp = RealType () ;
}

using ALLOCATOR_MIN_SIZE = ENUM<256> ;

template <class A ,class B>
class Allocator implement AllocatorRealLayout<A ,B> {
protected:
	using AllocatorRealLayout<A ,B>::mAllocator ;
	using AllocatorRealLayout<A ,B>::mRest ;
	using AllocatorRealLayout<A ,B>::mLength ;
	using AllocatorRealLayout<A ,B>::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (CREF<LENGTH> size_) {
		AllocatorHolder::create (thiz)->initialize (AllocatorUnknownBinder<A ,B> () ,size_) ;
	}

	BOOL exist () const {
		return AllocatorHolder::create (thiz)->exist () ;
	}

	RFat<Unknown> unknown () const {
		return AllocatorHolder::create (thiz)->unknown () ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return AllocatorHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return AllocatorHolder::create (thiz)->raw () ;
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

	INDEX alloc (RREF<BoxLayout> item) {
		return AllocatorHolder::create (thiz)->alloc (move (item)) ;
	}

	void free (CREF<INDEX> index) {
		return AllocatorHolder::create (thiz)->free (index) ;
	}

	BOOL used (CREF<INDEX> index) const {
		return AllocatorHolder::create (thiz)->used (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return AllocatorHolder::create (thiz)->resize (size_) ;
	}
} ;
} ;
