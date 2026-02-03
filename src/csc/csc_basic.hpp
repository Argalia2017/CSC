#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
struct OptionalLayout {
	Flag mCode ;
	mutable BoxLayout mValue ;

public:
	implicit OptionalLayout () noexcept {
		mCode = 0 ;
	}
} ;

struct OptionalHolder implement Interface {
	imports VFat<OptionalHolder> hold (VR<OptionalLayout> that) ;
	imports CFat<OptionalHolder> hold (CR<OptionalLayout> that) ;

	virtual void initialize (CR<Flag> code) = 0 ;
	virtual Bool exist () const = 0 ;
	virtual Flag code () const = 0 ;
	virtual void get (VR<BoxLayout> item) const = 0 ;
	virtual void set (VR<BoxLayout> item) const = 0 ;
} ;

template <class A>
class Optional implement OptionalLayout {
protected:
	using OptionalLayout::mCode ;
	using OptionalLayout::mValue ;
	Union<A> mStorage ;

public:
	implicit Optional () = default ;

	implicit Optional (CR<A> that) :Optional (move (that)) {}

	implicit Optional (RR<A> that) {
		once (move (that)) ;
	}

	static Optional error (CR<Flag> code) {
		Optional ret ;
		OptionalHolder::hold (ret)->initialize (code) ;
		return move (ret) ;
	}

	Bool exist () const {
		return OptionalHolder::hold (thiz)->exist () ;
	}

	Flag code () const {
		return OptionalHolder::hold (thiz)->code () ;
	}

	A fetch () const {
		auto rax = Box<A> () ;
		OptionalHolder::hold (thiz)->get (rax) ;
		return move (rax.ref) ;
	}

	forceinline operator A () const {
		return fetch () ;
	}

	void once (CR<A> item) const {
		once (move (item)) ;
	}

	void once (RR<A> item) const {
		if (exist ())
			return ;
		auto rax = Box<A>::make (move (item)) ;
		OptionalHolder::hold (thiz)->set (rax) ;
	}

	template <class ARG1>
	forceinline CR<Optional> operator() (RR<ARG1> item) const {
		once (move (item)) ;
		return thiz ;
	}

	template <class ARG1>
	void then (CR<ARG1> func) const {
		if (!exist ())
			return ;
		auto rax = Box<A> () ;
		OptionalHolder::hold (thiz)->get (rax) ;
		func (rax.ref) ;
		OptionalHolder::hold (thiz)->set (rax) ;
	}
} ;

struct BufferLayout {} ;

template <class A ,class B>
class Buffer implement BufferLayout {
private:
	using BUFFER = Tuple<CONDITIONAL<ENUM_GT_ZERO<B> ,ARR<A ,B> ,Storage<RANK1 ,ALIGN_OF<A>>>> ;

protected:
	BUFFER mBuffer ;

public:
	implicit Buffer () = default ;

	template <class...ARG1 ,class = REQUIRE<ENUM_EQUAL<RANK_OF<TYPE<ARG1...>> ,B>>>
	implicit Buffer (XR<ARG1>...initval) {
		mBuffer = BUFFER {A (initval)...} ;
	}

	Length size () const {
		return B::expr ;
	}

	Length step () const {
		return SIZE_OF<A>::expr ;
	}

	VR<ARR<A>> ref_m () leftvalue {
		return Pointer::from (mBuffer) ;
	}

	forceinline operator VR<ARR<A>> () leftvalue {
		return ref ;
	}

	CR<ARR<A>> ref_m () const leftvalue {
		return Pointer::from (mBuffer) ;
	}

	forceinline operator CR<ARR<A>> () const leftvalue {
		return ref ;
	}

	forceinline Bool operator== (CR<Buffer> that) = delete ;

	forceinline Bool operator!= (CR<Buffer> that) = delete ;

	VR<A> at (CR<Index> index) leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return ref[index] ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		assert (inline_between (index ,0 ,size ())) ;
		return ref[index] ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
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

template <class A ,class B = RANK1>
class Wrapper implement Proxy {
protected:
	Length mRank ;
	Buffer<Flag ,B> mWrapper ;

public:
	implicit Wrapper () = delete ;

	explicit Wrapper (CR<Buffer<Flag ,B>> that) {
		mRank = B::expr ;
		mWrapper = that ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_ANY<IS_SAME<ARG1 ,A> ,ENUM_EQ_ZERO<B>>>>
	implicit operator CR<Wrapper<ARG1 ,ARG2>> () const leftvalue {
		return Pointer::from (thiz) ;
	}

	Length rank () const {
		return mRank ;
	}

	CR<A> at (CR<Index> index) const {
		assert (inline_between (index ,0 ,mRank)) ;
		return Pointer::make (mWrapper.ref[index]) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const {
		return at (index) ;
	}
} ;

inline Wrapper<Pointer ,RANK0> MakeWrapper () {
	return Wrapper<Pointer ,RANK0> (Buffer<Flag ,RANK0> ()) ;
}

template <class ARG1 ,class...ARG2>
inline Wrapper<ARG1 ,RANK_OF<TYPE<ARG1 ,ARG2...>>> MakeWrapper (CR<ARG1> params1 ,CR<ARG2>...params2) {
	using R1X = RANK_OF<TYPE<ARG1 ,ARG2...>> ;
	return Wrapper<ARG1 ,R1X> (Buffer<Flag ,R1X> ({address (params1) ,address (params2)...})) ;
}

struct ReflectInvoke implement Interface {
	virtual Length rank () const = 0 ;
	virtual void invoke (CR<Pointer> func ,CR<Wrapper<Pointer>> params) const = 0 ;

	forceinline static consteval Flag expr_m () noexcept {
		return 200 ;
	}
} ;

template <class A>
class ReflectInvokeBinder final implement Fat<ReflectInvoke ,void> {
public:
	Length rank () const override {
		return RANK_OF<FUNCTION_PARAMS<A>>::expr ;
	}

	void invoke (CR<Pointer> func ,CR<Wrapper<Pointer>> params) const override {
		assert (params.rank () == rank ()) ;
		using R1X = FUNCTION_PARAMS<A> ;
		using R2X = TYPE_SENQUENCE<RANK_OF<R1X>> ;
		return invoke_impl (func ,params ,R1X::expr ,R2X::expr) ;
	}

	template <class...ARG1 ,class...ARG2>
	forceinline void invoke_impl (CR<A> func ,CR<Wrapper<Pointer>> params ,TYPE<ARG1...> ,TYPE<ARG2...>) const {
		//@fatal: GCC is so bad
		return func (keep[TYPE<XR<ARG1>>::expr] (Pointer::make (address (params[ARG2::expr])))...) ;
	}
} ;

struct FunctionTree ;

struct FunctionLayout {
	Ref<FunctionTree> mThis ;
} ;

struct FunctionHolder implement Interface {
	imports VFat<FunctionHolder> hold (VR<FunctionLayout> that) ;
	imports CFat<FunctionHolder> hold (CR<FunctionLayout> that) ;

	virtual void initialize (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<FunctionLayout> that) = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual Length rank () const = 0 ;
	virtual void invoke (CR<Wrapper<Pointer>> params) const = 0 ;
} ;

template <class A>
class FunctionUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectInvokeBinder<A>::expr)
			return inline_vptr (ReflectInvokeBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class...A>
class Function implement FunctionLayout {
protected:
	using FunctionLayout::mThis ;

public:
	implicit Function () = default ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_EXTEND<FunctionLayout ,ARG1>>>>
	implicit Function (RR<ARG1> that) {
		using R1X = FUNCTION_RETURN<ARG1> ;
		using R2X = FUNCTION_PARAMS<ARG1> ;
		require (IS_VOID<R1X>) ;
		require (IS_SAME<R2X ,TYPE<A...>>) ;
		FunctionHolder::hold (thiz)->initialize (FunctionUnknownBinder<ARG1> ()) ;
		auto &&rax = keep[TYPE<Box<ARG1>>::expr] (raw ()) ;
		rax.remake (move (that)) ;
	}

	implicit Function (CR<Function> that) {
		FunctionHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Function> operator= (CR<Function> that) {
		return assign (thiz ,that) ;
	}

	implicit Function (RR<Function> that) = default ;

	forceinline VR<Function> operator= (RR<Function> that) = default ;

	VR<BoxLayout> raw () leftvalue {
		return FunctionHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return FunctionHolder::hold (thiz)->raw () ;
	}

	Length rank () const {
		return FunctionHolder::hold (thiz)->rank () ;
	}

	void invoke (XR<A>...params) const {
		return FunctionHolder::hold (thiz)->invoke (MakeWrapper (Pointer::from (params)...)) ;
	}

	forceinline void operator() (XR<A>...params) const {
		return invoke (keep[TYPE<XR<A>>::expr] (params)...) ;
	}
} ;

struct ReflectRecast implement Interface {
	virtual Flag recast (CR<Flag> layout) const = 0 ;

	forceinline static consteval Flag expr_m () noexcept {
		return 201 ;
	}
} ;

template <class A ,class B>
class ReflectRecastBinder final implement Fat<ReflectRecast ,void> {
public:
	Flag recast (CR<Flag> layout) const override {
		auto &&rax = keep[TYPE<B>::expr] (Pointer::make (layout)) ;
		return recast_impl (PHX ,TYPE<A>::expr ,rax) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_EXTEND<ARG1 ,ARG2>>>
	forceinline Flag recast_impl (CR<typeof (PH3)> ,TYPE<ARG1> ,CR<ARG2> b) const {
		return address (keep[TYPE<ARG1>::expr] (b)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_VIRTUAL<ARG1 ,ARG2>>>
	forceinline Flag recast_impl (CR<typeof (PH2)> ,TYPE<ARG1> ,CR<ARG2> b) const {
		return address (b.ref) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_SAME<ARG1 ,Pointer>>>
	forceinline Flag recast_impl (CR<typeof (PH1)> ,TYPE<ARG1> ,CR<ARG2> b) const {
		return address (b) ;
	}
} ;

template <class A ,class B>
class RecastUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectRecastBinder<A ,B>::expr)
			return inline_vptr (ReflectRecastBinder<A ,B> ()) ;
		return ZERO ;
	}
} ;

struct AutoRefTree ;

struct AutoRefLayout {
	Ref<AutoRefTree> mThis ;
	Flag mLayout ;

public:
	implicit AutoRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~AutoRefLayout () noexcept ;

	implicit AutoRefLayout (CR<AutoRefLayout> that) = delete ;

	forceinline VR<AutoRefLayout> operator= (CR<AutoRefLayout> that) = delete ;

	implicit AutoRefLayout (RR<AutoRefLayout> that) noexcept :AutoRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<AutoRefLayout> operator= (RR<AutoRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AutoRefHolder implement Interface {
	imports VFat<AutoRefHolder> hold (VR<AutoRefLayout> that) ;
	imports CFat<AutoRefHolder> hold (CR<AutoRefLayout> that) ;

	virtual void initialize (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Clazz> clazz_) = 0 ;
	virtual void destroy () = 0 ;
	virtual Bool exist () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual VR<Pointer> rebind (CR<Clazz> clazz_) leftvalue = 0 ;
	virtual CR<Pointer> rebind (CR<Clazz> clazz_) const leftvalue = 0 ;
	virtual AutoRefLayout recast (CR<Unknown> extend) = 0 ;
} ;

inline AutoRefLayout::~AutoRefLayout () noexcept {
	AutoRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class AutoRef implement AutoRefLayout {
protected:
	using AutoRefLayout::mThis ;
	using AutoRefLayout::mLayout ;

public:
	implicit AutoRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit AutoRef (RR<AutoRef<ARG1>> that) :AutoRef (that.recast (TYPE<A>::expr)) {}

	explicit AutoRef (CR<Unknown> holder) {
		AutoRefHolder::hold (thiz)->initialize (holder) ;
	}

	template <class...ARG1>
	static AutoRef make (XR<ARG1>...initval) {
		AutoRef ret ;
		AutoRefHolder::hold (ret)->initialize (BoxUnknownBinder<A> () ,Clazz (TYPE<A>::expr)) ;
		auto &&rax = keep[TYPE<Box<A>>::expr] (ret.raw ()) ;
		rax.remake (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	Bool exist () const {
		return AutoRefHolder::hold (thiz)->exist () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return AutoRefHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return AutoRefHolder::hold (thiz)->raw () ;
	}

	Clazz clazz () const {
		return AutoRefHolder::hold (thiz)->clazz () ;
	}

	VR<A> ref_m () leftvalue {
		return AutoRefHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<A> () leftvalue {
		return ref ;
	}

	forceinline PTR<VR<A>> operator-> () leftvalue {
		return (&ref) ;
	}

	CR<A> ref_m () const leftvalue {
		return AutoRefHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<A> () const leftvalue {
		return ref ;
	}

	forceinline PTR<CR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}

	forceinline Bool operator== (CR<AutoRef> that) = delete ;

	forceinline Bool operator!= (CR<AutoRef> that) = delete ;

	template <class ARG1>
	VR<AutoRef<ARG1>> rebind (TYPE<ARG1>) leftvalue {
		return AutoRefHolder::hold (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	CR<AutoRef<ARG1>> rebind (TYPE<ARG1>) const leftvalue {
		return AutoRefHolder::hold (thiz)->rebind (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	AutoRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = Unknown (RecastUnknownBinder<ARG1 ,A> ()) ;
		AutoRefLayout ret = AutoRefHolder::hold (thiz)->recast (r1x) ;
		return move (keep[TYPE<AutoRef<ARG1>>::expr] (ret)) ;
	}
} ;

struct SharedRefTree ;

struct SharedRefLayout {
	Ref<SharedRefTree> mThis ;
	Flag mLayout ;

public:
	implicit SharedRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~SharedRefLayout () noexcept ;

	implicit SharedRefLayout (CR<SharedRefLayout> that) = delete ;

	forceinline VR<SharedRefLayout> operator= (CR<SharedRefLayout> that) = delete ;

	implicit SharedRefLayout (RR<SharedRefLayout> that) noexcept :SharedRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<SharedRefLayout> operator= (RR<SharedRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct SharedRefHolder implement Interface {
	imports VFat<SharedRefHolder> hold (VR<SharedRefLayout> that) ;
	imports CFat<SharedRefHolder> hold (CR<SharedRefLayout> that) ;

	virtual void initialize (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Flag> layout) = 0 ;
	virtual void initialize (CR<SharedRefLayout> that) = 0 ;
	virtual void destroy () = 0 ;
	virtual Bool exist () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual Length counter () const = 0 ;
	virtual VR<Pointer> ref_m () const leftvalue = 0 ;
	virtual SharedRefLayout recast (CR<Unknown> extend) = 0 ;
	virtual SharedRefLayout weak () const = 0 ;
} ;

inline SharedRefLayout::~SharedRefLayout () noexcept {
	SharedRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class SharedRef implement SharedRefLayout {
protected:
	using SharedRefLayout::mThis ;
	using SharedRefLayout::mLayout ;

public:
	implicit SharedRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit SharedRef (RR<SharedRef<ARG1>> that) :SharedRef (that.recast (TYPE<A>::expr)) {}

	explicit SharedRef (VR<A> item) {
		SharedRefHolder::hold (thiz)->initialize (BoxUnknownBinder<A> () ,address (item)) ;
	}

	template <class...ARG1>
	static SharedRef make (XR<ARG1>...initval) {
		SharedRef ret ;
		SharedRefHolder::hold (ret)->initialize (BoxUnknownBinder<A> ()) ;
		auto &&rax = keep[TYPE<Box<A>>::expr] (ret.raw ()) ;
		rax.remake (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	implicit SharedRef (CR<SharedRef> that) {
		SharedRefHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<SharedRef> operator= (CR<SharedRef> that) {
		return assign (thiz ,that) ;
	}

	implicit SharedRef (RR<SharedRef> that) = default ;

	forceinline VR<SharedRef> operator= (RR<SharedRef> that) = default ;

	Bool exist () const {
		return SharedRefHolder::hold (thiz)->exist () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return SharedRefHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return SharedRefHolder::hold (thiz)->raw () ;
	}

	Length counter () const {
		return SharedRefHolder::hold (thiz)->counter () ;
	}

	VR<A> ref_m () const leftvalue {
		return SharedRefHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<A> () const leftvalue {
		return ref ;
	}

	forceinline PTR<VR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}

	forceinline Bool operator== (CR<SharedRef> that) = delete ;

	forceinline Bool operator!= (CR<SharedRef> that) = delete ;

	template <class ARG1>
	SharedRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = Unknown (RecastUnknownBinder<ARG1 ,A> ()) ;
		SharedRefLayout ret = SharedRefHolder::hold (thiz)->recast (r1x) ;
		return move (keep[TYPE<SharedRef<ARG1>>::expr] (ret)) ;
	}

	SharedRef weak () const {
		SharedRefLayout ret = SharedRefHolder::hold (thiz)->weak () ;
		return move (keep[TYPE<SharedRef>::expr] (ret)) ;
	}
} ;

struct UniqueRefTree ;

struct UniqueRefLayout {
	Ref<UniqueRefTree> mThis ;
	Flag mLayout ;

public:
	implicit UniqueRefLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit ~UniqueRefLayout () noexcept ;

	implicit UniqueRefLayout (CR<UniqueRefLayout> that) = delete ;

	forceinline VR<UniqueRefLayout> operator= (CR<UniqueRefLayout> that) = delete ;

	implicit UniqueRefLayout (RR<UniqueRefLayout> that) noexcept :UniqueRefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<UniqueRefLayout> operator= (RR<UniqueRefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct UniqueRefHolder implement Interface {
	imports VFat<UniqueRefHolder> hold (VR<UniqueRefLayout> that) ;
	imports CFat<UniqueRefHolder> hold (CR<UniqueRefLayout> that) ;

	virtual void initialize (CR<Unknown> holder ,CR<Function<VR<Pointer>>> owner) = 0 ;
	virtual void initialize (CR<UniqueRefLayout> that) = 0 ;
	virtual void destroy () = 0 ;
	virtual Bool exist () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual UniqueRefLayout recast (CR<Unknown> extend) = 0 ;
	virtual void recycle () = 0 ;
} ;

inline UniqueRefLayout::~UniqueRefLayout () noexcept {
	UniqueRefHolder::hold (thiz)->destroy () ;
}

template <class A>
class UniqueRef implement UniqueRefLayout {
protected:
	using UniqueRefLayout::mThis ;
	using UniqueRefLayout::mLayout ;

public:
	implicit UniqueRef () = default ;

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,A>>>
	implicit UniqueRef (RR<UniqueRef<ARG1>> that) :UniqueRef (that.recast (TYPE<A>::expr)) {}

	template <class ARG1 ,class ARG2>
	explicit UniqueRef (RR<ARG1> ctor ,RR<ARG2> dtor) {
		auto rax = Function<VR<A>> (move (dtor)) ;
		UniqueRefHolder::hold (thiz)->initialize (BoxUnknownBinder<A> () ,Pointer::from (rax)) ;
		auto &&rbx = keep[TYPE<Box<A>>::expr] (raw ()) ;
		rbx.remake () ;
		ctor (Pointer::from (rbx.ref)) ;
	}

	template <class...ARG1>
	static UniqueRef make (XR<ARG1>...initval) {
		UniqueRef ret ;
		auto rax = Function<VR<A>> () ;
		UniqueRefHolder::hold (ret)->initialize (BoxUnknownBinder<A> () ,Pointer::from (rax)) ;
		auto &&rbx = keep[TYPE<Box<A>>::expr] (ret.raw ()) ;
		rbx.remake (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	implicit UniqueRef (CR<UniqueRef> that) {
		UniqueRefHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<UniqueRef> operator= (CR<UniqueRef> that) {
		return assign (thiz ,that) ;
	}

	implicit UniqueRef (RR<UniqueRef> that) = default ;

	forceinline VR<UniqueRef> operator= (RR<UniqueRef> that) = default ;

	Bool exist () const {
		return UniqueRefHolder::hold (thiz)->exist () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return UniqueRefHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return UniqueRefHolder::hold (thiz)->raw () ;
	}

	CR<A> ref_m () const leftvalue {
		return UniqueRefHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<A> () const leftvalue {
		return ref ;
	}

	forceinline PTR<CR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}

	forceinline Bool operator== (CR<UniqueRef> that) = delete ;

	forceinline Bool operator!= (CR<UniqueRef> that) = delete ;

	template <class ARG1>
	UniqueRef<ARG1> recast (TYPE<ARG1>) {
		const auto r1x = Unknown (RecastUnknownBinder<ARG1 ,A> ()) ;
		UniqueRefLayout ret = UniqueRefHolder::hold (thiz)->recast (r1x) ;
		return move (keep[TYPE<UniqueRef<ARG1>>::expr] (ret)) ;
	}

	void recycle () {
		return UniqueRefHolder::hold (thiz)->recycle () ;
	}
} ;

struct ReflectElement implement Interface {
	virtual Unknown element () const = 0 ;

	forceinline static consteval Flag expr_m () noexcept {
		return 202 ;
	}
} ;

template <class A>
class ReflectElementBinder final implement Fat<ReflectElement ,void> {
public:
	Unknown element () const override {
		return RefUnknownBinder<A> () ;
	}
} ;

struct RefBufferTree ;

struct RefBufferLayout {
	Ref<RefBufferTree> mThis ;
	Flag mBuffer ;
	Length mSize ;
	Length mStep ;

public:
	implicit RefBufferLayout () noexcept {
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}

	implicit ~RefBufferLayout () noexcept ;

	implicit RefBufferLayout (CR<RefBufferLayout> that) = delete ;

	forceinline VR<RefBufferLayout> operator= (CR<RefBufferLayout> that) = delete ;

	implicit RefBufferLayout (RR<RefBufferLayout> that) noexcept :RefBufferLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<RefBufferLayout> operator= (RR<RefBufferLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> hold (VR<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> hold (CR<RefBufferLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<SliceLayout> buffer ,RR<BoxLayout> item) = 0 ;
	virtual void destroy () = 0 ;
	virtual Bool exist () const = 0 ;
	virtual Bool fixed () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual void resize (CR<Length> size_) = 0 ;
} ;

inline RefBufferLayout::~RefBufferLayout () noexcept {
	RefBufferHolder::hold (thiz)->destroy () ;
}

template <class A>
class BufferUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct RefBufferImplLayout implement RefBufferLayout {
public:
	implicit RefBufferImplLayout () noexcept {
		RefBufferHolder::hold (thiz)->prepare (BufferUnknownBinder<A> ()) ;
	}
} ;

template <>
struct RefBufferImplLayout<Pointer> implement RefBufferLayout {} ;

template <class A>
class RefBuffer implement RefBufferImplLayout<A> {
protected:
	using RefBufferImplLayout<A>::mThis ;
	using RefBufferImplLayout<A>::mBuffer ;
	using RefBufferImplLayout<A>::mSize ;
	using RefBufferImplLayout<A>::mStep ;

public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CR<Length> size_) {
		RefBufferHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	static RefBuffer reference (CR<Flag> buffer ,CR<Length> size_) {
		RefBuffer ret ;
		const auto r1x = Slice (buffer ,size_ ,SIZE_OF<A>::expr) ;
		RefBufferHolder::hold (ret)->initialize (BufferUnknownBinder<A> () ,r1x ,Box<int>::make ()) ;
		return move (ret) ;
	}

	Bool exist () const {
		return RefBufferHolder::hold (thiz)->exist () ;
	}

	Bool fixed () const {
		return RefBufferHolder::hold (thiz)->fixed () ;
	}

	Unknown unknown () const {
		return RefBufferHolder::hold (thiz)->unknown () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return RefBufferHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return RefBufferHolder::hold (thiz)->raw () ;
	}

	Length size () const {
		return RefBufferHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return RefBufferHolder::hold (thiz)->step () ;
	}

	VR<ARR<A>> ref_m () leftvalue {
		return RefBufferHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<ARR<A>> () leftvalue {
		return ref ;
	}

	CR<ARR<A>> ref_m () const leftvalue {
		return RefBufferHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<ARR<A>> () const leftvalue {
		return ref ;
	}

	forceinline Bool operator== (CR<RefBuffer> that) = delete ;

	forceinline Bool operator!= (CR<RefBuffer> that) = delete ;

	VR<A> at (CR<Index> index) leftvalue {
		return RefBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return RefBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	void resize (CR<Length> size_) {
		return RefBufferHolder::hold (thiz)->resize (size_) ;
	}
} ;

struct FarBufferTree ;

struct FarBufferLayout {
	Ref<FarBufferTree> mThis ;
	Flag mBuffer ;
	Length mSize ;
	Length mStep ;

public:
	implicit FarBufferLayout () noexcept {
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}
} ;

struct FarBufferHolder implement Interface {
	imports VFat<FarBufferHolder> hold (VR<FarBufferLayout> that) ;
	imports CFat<FarBufferHolder> hold (CR<FarBufferLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual Bool exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual void use_getter (CR<Function<CR<Index> ,VR<Pointer>>> getter) = 0 ;
	virtual void use_setter (CR<Function<CR<Index> ,CR<Pointer>>> setter) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual void refresh () const = 0 ;
} ;

template <class A>
struct FarBufferImplLayout implement FarBufferLayout {
public:
	implicit FarBufferImplLayout () noexcept {
		FarBufferHolder::hold (thiz)->prepare (BufferUnknownBinder<A> ()) ;
	}
} ;

template <>
struct FarBufferImplLayout<Pointer> implement FarBufferLayout {} ;

template <class A>
class FarBuffer implement FarBufferLayout {
protected:
	using FarBufferLayout::mThis ;
	using FarBufferLayout::mBuffer ;
	using FarBufferLayout::mSize ;
	using FarBufferLayout::mStep ;

public:
	implicit FarBuffer () = default ;

	explicit FarBuffer (CR<Length> size_) {
		FarBufferHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,size_) ;
	}

	void use_getter (CR<Function<CR<Index> ,VR<A>>> getter) {
		return FarBufferHolder::hold (thiz)->use_getter (Pointer::from (getter)) ;
	}

	void use_setter (CR<Function<CR<Index> ,CR<A>>> setter) {
		return FarBufferHolder::hold (thiz)->use_setter (Pointer::from (setter)) ;
	}

	Bool exist () const {
		return FarBufferHolder::hold (thiz)->exist () ;
	}

	Unknown unknown () const {
		return FarBufferHolder::hold (thiz)->unknown () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return FarBufferHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return FarBufferHolder::hold (thiz)->raw () ;
	}

	Length size () const {
		return FarBufferHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return FarBufferHolder::hold (thiz)->step () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return FarBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return FarBufferHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	void refresh () const {
		return FarBufferHolder::hold (thiz)->refresh () ;
	}
} ;

template <class A ,class B>
struct UnionPair implement Tuple<Union<A> ,B> {} ;

struct AllocatorNode {
	Index mNext ;
} ;

struct AllocatorLayout {
	RefBuffer<Pointer> mAllocator ;
	Flag mOffset ;
	Length mLength ;
	Index mFree ;

public:
	implicit AllocatorLayout () noexcept {
		mOffset = 0 ;
		mLength = 0 ;
		mFree = 0 ;
	}

	implicit ~AllocatorLayout () noexcept ;

	implicit AllocatorLayout (CR<AllocatorLayout> that) = delete ;

	forceinline VR<AllocatorLayout> operator= (CR<AllocatorLayout> that) = delete ;

	implicit AllocatorLayout (RR<AllocatorLayout> that) noexcept :AllocatorLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<AllocatorLayout> operator= (RR<AllocatorLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct AllocatorHolder implement Interface {
	imports VFat<AllocatorHolder> hold (VR<AllocatorLayout> that) ;
	imports CFat<AllocatorHolder> hold (CR<AllocatorLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual Bool exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual VR<Pointer> bt (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> bt (CR<Index> index) const leftvalue = 0 ;
	virtual Index alloc () = 0 ;
	virtual Index alloc (RR<BoxLayout> item) = 0 ;
	virtual void free (CR<Index> index) = 0 ;
	virtual Bool used (CR<Index> index) const = 0 ;
	virtual void resize (CR<Length> size_) = 0 ;
} ;

inline AllocatorLayout::~AllocatorLayout () noexcept {
	AllocatorHolder::hold (thiz)->destroy () ;
}

template <class A ,class B>
class AllocatorUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		using R1X = UnionPair<A ,B> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B>
struct AllocatorImplLayout implement AllocatorLayout {
public:
	implicit AllocatorImplLayout () noexcept {
		noop (RefBuffer<UnionPair<A ,B>> ()) ;
		AllocatorHolder::hold (thiz)->prepare (AllocatorUnknownBinder<A ,B> ()) ;
	}
} ;

template <class B>
struct AllocatorImplLayout<Pointer ,B> implement AllocatorLayout {} ;

using ALLOCATOR_MIN_SIZE = ENUM<256> ;

template <class A ,class B>
class Allocator implement AllocatorImplLayout<A ,B> {
private:
	require (IS_TRIVIAL<B>) ;

protected:
	using AllocatorImplLayout<A ,B>::mAllocator ;
	using AllocatorImplLayout<A ,B>::mOffset ;
	using AllocatorImplLayout<A ,B>::mLength ;
	using AllocatorImplLayout<A ,B>::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (CR<Length> size_) {
		AllocatorHolder::hold (thiz)->initialize (AllocatorUnknownBinder<A ,B> () ,size_) ;
	}

	Bool exist () const {
		return AllocatorHolder::hold (thiz)->exist () ;
	}

	Unknown unknown () const {
		return AllocatorHolder::hold (thiz)->unknown () ;
	}

	VR<BoxLayout> raw () leftvalue {
		return AllocatorHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return AllocatorHolder::hold (thiz)->raw () ;
	}

	void clear () {
		return AllocatorHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return AllocatorHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return AllocatorHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return AllocatorHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return AllocatorHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return AllocatorHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	VR<B> bt (CR<Index> index) leftvalue {
		return AllocatorHolder::hold (thiz)->bt (index) ;
	}

	CR<B> bt (CR<Index> index) const leftvalue {
		return AllocatorHolder::hold (thiz)->bt (index) ;
	}

	Index alloc () {
		return AllocatorHolder::hold (thiz)->alloc () ;
	}

	Index alloc (RR<BoxLayout> item) {
		return AllocatorHolder::hold (thiz)->alloc (move (item)) ;
	}

	void free (CR<Index> index) {
		return AllocatorHolder::hold (thiz)->free (index) ;
	}

	Bool used (CR<Index> index) const {
		return AllocatorHolder::hold (thiz)->used (index) ;
	}

	void resize (CR<Length> size_) {
		return AllocatorHolder::hold (thiz)->resize (size_) ;
	}
} ;
} ;
