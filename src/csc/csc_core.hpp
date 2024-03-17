#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"

namespace CSC {
struct FUNCTION_noop {
	forceinline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> a) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_unsafe_ifdo {
	forceinline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	forceinline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto unsafe_ifdo = FUNCTION_unsafe_ifdo () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_break {
	forceinline void operator() () const noexcept {
		__debugbreak () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_abort {
	forceinline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_abort {
	forceinline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_abort {
	forceinline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

static constexpr auto unsafe_abort = FUNCTION_unsafe_abort () ;

class Pointer implement Proxy {
public:
	imports VREF<Pointer> make (CREF<FLAG> that) {
		return (*reinterpret_cast<VPTR<Pointer>> (that)) ;
	}

	template <class ARG1>
	imports forceinline VREF<Pointer> from (VREF<ARG1> that) {
		return reinterpret_cast<VREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports forceinline CREF<Pointer> from (CREF<ARG1> that) {
		return reinterpret_cast<CREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports forceinline RREF<Pointer> from (RREF<ARG1>) = delete ;

	template <class ARG1>
	forceinline operator VREF<ARG1> () leftvalue {
		return reinterpret_cast<VREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () leftvalue = delete ;

	template <class ARG1>
	forceinline operator CREF<ARG1> () const leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () const leftvalue = delete ;

	template <class ARG1>
	forceinline operator RREF<ARG1> () rightvalue = delete ;
} ;

template <class A>
struct TEMPLATE_unsafe_cast {
	template <class ARG1>
	forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return Pointer::from (a) ;
	}

	template <class ARG1>
	forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return Pointer::from (a) ;
	}

	template <class ARG1>
	forceinline CREF<A> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	forceinline consteval TEMPLATE_unsafe_cast<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return TEMPLATE_unsafe_cast<ARG1> () ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_bitwise {
	template <class ARG1>
	forceinline BYTE_BASE<ARG1> operator() (CREF<ARG1> a) const noexcept {
		return Pointer::from (a) ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_address {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_unsafe_hold {
	forceinline FLAG operator() (CREF<Interface> binder) const noexcept {
		return unsafe_cast[TYPE<FLAG>::expr] (binder) ;
	}
} ;

static constexpr auto unsafe_hold = FUNCTION_unsafe_hold () ;

struct FUNCTION_swap {
	template <class ARG1>
	forceinline void operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		//@warn: no class should depend on its address
		auto &&rax = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (a) ;
		auto &&rbx = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (b) ;
		const auto r1x = rax ;
		rax = rbx ;
		rbx = r1x ;
		noop (a) ;
		noop (b) ;
	}

	template <class ARG1 ,class ARG2>
	forceinline void operator() (VREF<ARG1> a ,RREF<ARG2> b) const noexcept {
		thiz (a ,b) ;
	}

	template <class ARG1 ,class ARG2>
	forceinline void operator() (RREF<ARG1> a ,VREF<ARG2> b) const noexcept {
		thiz (a ,b) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

template <class A>
struct TEMPLATE_keep {
	template <class ARG1>
	forceinline XREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline XREF<A> operator() (CREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}

	template <class ARG1>
	forceinline XREF<A> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}
} ;

struct FUNCTION_keep {
	template <class ARG1>
	forceinline consteval TEMPLATE_keep<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return TEMPLATE_keep<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	forceinline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	forceinline ARG1 operator() (CREF<ARG1> a) const {
		require (IS_CLONEABLE<ARG1>) ;
		return keep[TYPE<CREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	forceinline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_invoke {
	template <class ARG1>
	forceinline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

struct FUNCTION_operator_min {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto operator_min = FUNCTION_operator_min () ;

struct FUNCTION_operator_max {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto operator_max = FUNCTION_operator_max () ;

struct FUNCTION_operator_between {
	forceinline BOOL operator() (CREF<VAL> curr ,CREF<VAL> begin_ ,CREF<VAL> end_) const noexcept {
		if (curr < begin_)
			return FALSE ;
		if (curr >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto operator_between = FUNCTION_operator_between () ;

struct FUNCTION_operator_alignas {
	forceinline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const noexcept {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto operator_alignas = FUNCTION_operator_alignas () ;

struct FUNCTION_operator_compr {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return template_compr (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline FLAG template_compr (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline FLAG template_compr (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.compr (b) ;
	}
} ;

static constexpr auto operator_compr = FUNCTION_operator_compr () ;

struct Visitor implement Interface {
	virtual void begin () const = 0 ;
	virtual void end () const = 0 ;
	virtual void push (CREF<BYTE> a) const = 0 ;
	virtual void push (CREF<WORD> a) const = 0 ;
	virtual void push (CREF<CHAR> a) const = 0 ;
	virtual void push (CREF<QUAD> a) const = 0 ;
} ;

struct FUNCTION_operator_visit {
	template <class ARG1>
	forceinline void operator() (CREF<Visitor> visitor ,CREF<ARG1> a) const noexcept {
		return template_visit (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline void template_visit (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		visitor.push (bitwise (a)) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline void template_visit (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return a.visit (visitor) ;
	}
} ;

static constexpr auto operator_visit = FUNCTION_operator_visit () ;

template <class...>
trait TUPLE_HELP ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<PARAMS>>>> {
	struct Tuple {} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK1>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK2>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK3>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
		TYPE_M3RD_ONE<PARAMS> m3rd ;
	} ;
} ;

template <class...A>
using Tuple = typename TUPLE_HELP<TYPE<A...> ,ALWAYS>::Tuple ;

class IndexIterator {
protected:
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit IndexIterator () = delete ;

	explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = operator_max (begin_ ,end_) ;
		mPeek = mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return mEnd - mBegin ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<IndexIterator>) const {
		return ifnot (good ()) ;
	}

	forceinline BOOL operator!= (CREF<IndexIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct PIXEL {
	INDEX mX ;
	INDEX mY ;
} ;

class PixelIterator {
protected:
	PIXEL mBegin ;
	PIXEL mEnd ;
	PIXEL mPeek ;

public:
	implicit PixelIterator () = delete ;

	explicit PixelIterator (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) {
		mBegin.mX = begin_x ;
		mBegin.mY = begin_y ;
		mEnd.mX = operator_max (begin_x ,end_x) ;
		mEnd.mY = operator_max (begin_y ,end_y) ;
		mPeek = mBegin ;
		if (rank () > 0)
			return ;
		mPeek = mEnd ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return (mEnd.mX - mBegin.mX) * (mEnd.mY - mBegin.mY) ;
	}

	BOOL good () const {
		return mPeek.mY != mEnd.mY ;
	}

	forceinline BOOL operator== (CREF<PixelIterator>) const {
		return ifnot (good ()) ;
	}

	forceinline BOOL operator!= (CREF<PixelIterator>) const {
		return good () ;
	}

	CREF<PIXEL> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<PIXEL> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek.mX++ ;
		if (mPeek.mX < mEnd.mX)
			return ;
		mPeek.mX = 0 ;
		mPeek.mY++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct FUNCTION_iter {
	forceinline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	forceinline PixelIterator operator() (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) const noexcept {
		return PixelIterator (begin_x ,end_x ,begin_y ,end_y) ;
	}
} ;

static constexpr auto iter = FUNCTION_iter () ;

template <class A ,class B>
class Fat implement A {
protected:
	FLAG mPointer ;

public:
	VREF<B> fake_m () leftvalue {
		return Pointer::make (mPointer) ;
	}

	CREF<B> fake_m () const leftvalue {
		return Pointer::make (mPointer) ;
	}
} ;

template <class A>
class VFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit VFat () = delete ;

	template <class ARG1>
	explicit VFat (CREF<ARG1> holder) {
		mHolder = holder.reflect (KILL<A ,ARG1>::expr) ;
		assert (mHolder != ZERO) ;
		mPointer = ZERO ;
	}

	explicit VFat (CREF<Interface> holder ,CREF<FLAG> addr) {
		mHolder = unsafe_hold (holder) ;
		mPointer = addr ;
	}

	forceinline VPTR<A> operator-> () const {
		return VPTR<A> (address (thiz)) ;
	}
} ;

template <class A>
class CFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit CFat () = delete ;

	template <class ARG1>
	explicit CFat (CREF<ARG1> holder) {
		mHolder = holder.reflect (KILL<A ,ARG1>::expr) ;
		assert (mHolder != ZERO) ;
		mPointer = ZERO ;
	}

	explicit CFat (CREF<Interface> holder ,CREF<FLAG> addr) {
		mHolder = unsafe_hold (holder) ;
		mPointer = addr ;
	}

	forceinline CPTR<A> operator-> () const {
		return CPTR<A> (address (thiz)) ;
	}
} ;

struct ReflectSize implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 100 ;
	}
} ;

template <class A>
class ReflectSizeBinder final implement ReflectSize {
public:
	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}
} ;

struct ReflectCreate implement Interface {
	virtual void create (VREF<Pointer> this_ ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 101 ;
	}
} ;

template <class A>
class ReflectCreateBinder final implement ReflectCreate {
public:
	void create (VREF<Pointer> this_ ,CREF<LENGTH> size_) const noexcept override {
		require (IS_DEFAULT<A>) ;
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&rax = keep[TYPE<VREF<ARR<A ,RANK1>>>::expr] (this_) ;
		for (auto &&i : iter (0 ,size_)) {
			new (VPTR<csc_temp_t> (&rax[i])) A () ;
		}
	}
} ;

struct ReflectDestroy implement Interface {
	virtual void destroy (VREF<Pointer> this_ ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 102 ;
	}
} ;

template <class A>
class ReflectDestroyBinder final implement ReflectDestroy {
public:
	void destroy (VREF<Pointer> this_ ,CREF<LENGTH> size_) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&rax = keep[TYPE<VREF<ARR<A ,RANK1>>>::expr] (this_) ;
		for (auto &&i : iter (0 ,size_)) {
			rax[i].~A () ;
		}
	}
} ;

template <class A>
class Pin {
protected:
	mutable TEMP<A> mStorage ;

public:
	implicit Pin () noexcept {
		new (VPTR<csc_temp_t> (&mStorage)) A () ;
	}

	implicit ~Pin () noexcept {
		self.~A () ;
	}

	implicit Pin (CREF<Pin>) = delete ;

	forceinline VREF<Pin> operator= (CREF<Pin>) = delete ;

	implicit Pin (RREF<Pin> that) noexcept :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (RREF<Pin> that) noexcept {
		return thiz ;
	}

	VREF<A> self_m () const leftvalue {
		return Pointer::from (mStorage) ;
	}

	forceinline operator VREF<A> () const leftvalue {
		return self ;
	}
} ;

struct BoxLayout {
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept ;

	implicit BoxLayout (CREF<BoxLayout>) = delete ;

	forceinline VREF<BoxLayout> operator= (CREF<BoxLayout>) = delete ;

	implicit BoxLayout (RREF<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<BoxLayout> operator= (RREF<BoxLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> create (VREF<BoxLayout> that) ;
	imports CFat<BoxHolder> create (CREF<BoxLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual CREF<Unknown> unknown () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual void acquire (CREF<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

inline BoxLayout::~BoxLayout () noexcept {
	BoxHolder::create (thiz)->destroy () ;
}

template <class A>
class BoxUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Box implement BoxLayout {
protected:
	using BoxLayout::mHolder ;
	TEMP<A> mStorage ;

public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	imports forceinline Box make (XREF<ARG1>...initval) {
		Box ret ;
		new (VPTR<csc_temp_t> (&ret.mStorage)) A (keep[TYPE<ARG1>::expr] (initval)...) ;
		BoxHolder::create (ret)->initialize (BoxUnknownBinder<A> ()) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::create (thiz)->exist () ;
	}

	CREF<Unknown> unknown () const leftvalue {
		return BoxHolder::create (thiz)->unknown () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	void acquire (CREF<BoxLayout> that) {
		return BoxHolder::create (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::create (thiz)->release () ;
	}
} ;

struct FUNCTION_memorize {
	template <class ARG1>
	forceinline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		using R1X = FUNCTION_RETURN<ARG1> ;
		static const auto mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;


struct RefLayout {
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RefLayout () noexcept {
		mHolder = ZERO ;
		mPointer = ZERO ;
	}

	implicit ~RefLayout () noexcept ;

	implicit RefLayout (CREF<RefLayout>) = delete ;

	forceinline VREF<RefLayout> operator= (CREF<RefLayout>) = delete ;

	implicit RefLayout (RREF<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefLayout> operator= (RREF<RefLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> create (VREF<RefLayout> that) ;
	imports CFat<RefHolder> create (CREF<RefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<Unknown> item ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual CREF<Unknown> unknown () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual RefLayout share () const = 0 ;
} ;

inline RefLayout::~RefLayout () noexcept {
	RefHolder::create (thiz)->destroy () ;
}

template <class A>
class RefUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Ref implement RefLayout {
protected:
	using RefLayout::mHolder ;
	using RefLayout::mPointer ;

public:
	implicit Ref () = default ;

	implicit Ref (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	imports forceinline Ref make (XREF<ARG1>...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<ARG1>::expr] (initval)...) ;
		RefHolder::create (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	imports Ref reference (VREF<A> that) {
		Ref ret ;
		ret.mPointer = address (that) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return RefHolder::create (thiz)->exist () ;
	}

	CREF<Unknown> unknown () const leftvalue {
		return RefHolder::create (thiz)->unknown () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	Ref share () const {
		auto rax = RefHolder::create (thiz)->share () ;
		return keep[TYPE<RREF<Ref>>::expr] (rax) ;
	}
} ;

template <class A>
class RefBase {
public:
	Ref<A> mThis ;
} ;

struct HeapImplLayout ;

struct HeapLayout implement RefBase<HeapImplLayout> {} ;

struct HeapHolder implement Interface {
	imports VFat<HeapHolder> create (VREF<HeapLayout> that) ;
	imports CFat<HeapHolder> create (CREF<HeapLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual FLAG stack (CREF<LENGTH> size_) const = 0 ;
	virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
	virtual void free (CREF<FLAG> addr) const = 0 ;
} ;

class Heap implement HeapLayout {
protected:
	using HeapLayout::mThis ;

public:
	imports CREF<Heap> instance () {
		return memorize ([&] () {
			Heap ret ;
			HeapHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH length () {
		return HeapHolder::create (instance ())->length () ;
	}

	imports FLAG stack (CREF<LENGTH> size_) {
		return HeapHolder::create (instance ())->stack (size_) ;
	}

	imports FLAG alloc (CREF<LENGTH> size_) {
		return HeapHolder::create (instance ())->alloc (size_) ;
	}

	imports void free (CREF<FLAG> addr) {
		return HeapHolder::create (instance ())->free (addr) ;
	}
} ;

struct CaptureLayout {
	LENGTH mRank ;
} ;

template <class...PARAMS>
class Capture implement CaptureLayout {
private:
	using RANK = COUNT_OF<TYPE<PARAMS...>> ;

protected:
	using CaptureLayout::mRank ;
	Tuple<ARR<FLAG ,RANK>> mCapture ;

public:
	explicit Capture (CREF<KILL<FLAG ,PARAMS>>...params) {
		mRank = RANK::expr ;
		mCapture = Tuple<ARR<FLAG ,RANK>> {params...} ;
	}

	imports CREF<Capture> from (CREF<CaptureLayout> that) {
		assert (that.mRank == RANK::expr) ;
		return Pointer::from (that) ;
	}

	imports CREF<Capture> from (RREF<CaptureLayout>) = delete ;

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return invoke (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	forceinline void invoke (CREF<ARG1> func ,TYPEID<TYPE<ARG2...>>) const {
		return func (keep[TYPE<PARAMS>::expr] (Pointer::make (mCapture.m1st[ARG2::expr]))...) ;
	}
} ;

template <>
class Capture<> implement CaptureLayout {
private:
	using RANK = ENUM_ZERO ;

protected:
	using CaptureLayout::mRank ;

public:
	explicit Capture () {
		mRank = RANK::expr ;
	}

	imports CREF<Capture> from (CREF<CaptureLayout> that) {
		assert (that.mRank == RANK::expr) ;
		return Pointer::from (that) ;
	}

	imports CREF<Capture> from (RREF<CaptureLayout>) = delete ;

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return invoke (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1>
	forceinline void invoke (CREF<ARG1> func ,TYPEID<TYPE<>>) const {
		return func () ;
	}
} ;

template <class...>
trait CAPTURE_WRAP_HELP ;

template <class...A>
trait CAPTURE_WRAP_HELP<TYPE<A...> ,ALWAYS> {
	using RET = Capture<A...> ;
} ;

template <class A>
using CAPTURE_WRAP = typename CAPTURE_WRAP_HELP<A ,ALWAYS>::RET ;

struct FUNCTION_capture {
	template <class...ARG1>
	forceinline Capture<ARG1...> operator() (XREF<ARG1>...initval) const noexcept {
		return Capture<ARG1...> (address (initval)...) ;
	}
} ;

static constexpr auto capture = FUNCTION_capture () ;

struct SliceLayout {
	FLAG mBegin ;
	FLAG mEnd ;
	LENGTH mStep ;
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> create (VREF<SliceLayout> that) ;
	imports CFat<SliceHolder> create (CREF<SliceLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<FLAG> pointer) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual BOOL equal (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CREF<SliceLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class SliceUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Slice implement SliceLayout {
protected:
	using SliceLayout::mBegin ;
	using SliceLayout::mEnd ;
	using SliceLayout::mStep ;

public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	explicit Slice (CREF<ARG1> that) {
		SliceHolder::create (thiz)->initialize (SliceUnknownBinder<ARG1> () ,address (that)) ;
	}

	explicit Slice (CREF<FLAG> begin_ ,CREF<FLAG> end_ ,CREF<FLAG> step_) {
		mBegin = begin_ ;
		mEnd = end_ ;
		mStep = step_ ;
	}

	LENGTH size () const {
		return SliceHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SliceHolder::create (thiz)->step () ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		return SliceHolder::create (thiz)->get (index ,item) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Slice> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Slice> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return SliceHolder::create (thiz)->visit (visitor) ;
	}
} ;

struct ReflectClone implement Interface {
	virtual void clone (VREF<Pointer> this_ ,CREF<Pointer> that_) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 110 ;
	}
} ;

template <class A>
class ReflectCloneBinder final implement ReflectClone {
public:
	void clone (VREF<Pointer> this_ ,CREF<Pointer> that_) const override {
		require (IS_CLONEABLE<A>) ;
		auto &&rax = keep[TYPE<VREF<A>>::expr] (this_) ;
		auto &&rbx = keep[TYPE<CREF<A>>::expr] (that_) ;
		rax = rbx ;
	}
} ;

struct ReflectEqual implement Interface {
	virtual BOOL equal (CREF<Pointer> this_ ,CREF<Pointer> that_) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 111 ;
	}
} ;

template <class A>
class ReflectEqualBinder final implement ReflectEqual {
public:
	BOOL equal (CREF<Pointer> this_ ,CREF<Pointer> that_) const override {
		auto &&rax = keep[TYPE<CREF<A>>::expr] (this_) ;
		auto &&rbx = keep[TYPE<CREF<A>>::expr] (that_) ;
		return rax == rbx ;
	}
} ;

struct ReflectCompr implement Interface {
	virtual FLAG compr (CREF<Pointer> this_ ,CREF<Pointer> that_) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 112 ;
	}
} ;

template <class A>
class ReflectComprBinder final implement ReflectCompr {
public:
	FLAG compr (CREF<Pointer> this_ ,CREF<Pointer> that_) const override {
		auto &&rax = keep[TYPE<CREF<A>>::expr] (this_) ;
		auto &&rbx = keep[TYPE<CREF<A>>::expr] (that_) ;
		return operator_compr (rax ,rbx) ;
	}
} ;

struct ReflectVisit implement Interface {
	virtual void visit (CREF<Visitor> visitor ,CREF<Pointer> this_) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 113 ;
	}
} ;

template <class A>
class ReflectVisitBinder final implement ReflectVisit {
public:
	void visit (CREF<Visitor> visitor ,CREF<Pointer> this_) const override {
		auto &&rax = keep[TYPE<CREF<A>>::expr] (this_) ;
		return operator_visit (visitor ,rax) ;
	}
} ;

struct ReflectName implement Interface {
	virtual Slice<STR> type_name () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 114 ;
	}
} ;

template <class A>
class ReflectNameBinder final implement ReflectName {
public:
#ifdef __CSC_CXX_RTTI__
	Slice<STR> type_name () const override {
		const auto r1x = typeid (A).name () ;
		return Slice<STR> (FLAG (r1x) ,FLAG (r1x) + strlen (r1x) ,1) ;
	}
#endif

#ifndef __CSC_CXX_RTTI__
	Slice<STR> type_name () const override {
		return Slice<STR> () ;
	}
#endif
} ;

struct ClazzImplLayout ;

struct ClazzLayout implement RefBase<ClazzImplLayout> {} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> create (VREF<ClazzLayout> that) ;
	imports CFat<ClazzHolder> create (CREF<ClazzLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_uuid () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
	virtual BOOL equal (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ClazzLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class ClazzUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return unsafe_hold (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

class Clazz implement ClazzLayout {
protected:
	using ClazzLayout::mThis ;

public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPEID<ARG1>) {
		ClazzHolder::create (thiz)->initialize (ClazzUnknownBinder<ARG1> ()) ;
	}

	implicit Clazz (CREF<Clazz> that) {
		mThis = that.mThis.share () ;
	}

	forceinline VREF<Clazz> operator= (CREF<Clazz> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Clazz (RREF<Clazz>) = default ;

	forceinline VREF<Clazz> operator= (RREF<Clazz>) = default ;

	LENGTH type_size () const {
		return ClazzHolder::create (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::create (thiz)->type_align () ;
	}

	FLAG type_uuid () const {
		return ClazzHolder::create (thiz)->type_uuid () ;
	}

	Slice<STR> type_name () const {
		return ClazzHolder::create (thiz)->type_name () ;
	}

	BOOL equal (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Clazz> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return ClazzHolder::create (thiz)->visit (visitor) ;
	}
} ;

class Exception {
protected:
	Tuple<ARR<Slice<STR> ,RANK3>> mWhat ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice<STR>> a) {
		mWhat.m1st[0] = a ;
	}

	explicit Exception (CREF<Slice<STR>> a ,CREF<Slice<STR>> b) {
		mWhat.m1st[0] = a ;
		mWhat.m1st[1] = b ;
	}

	explicit Exception (CREF<Slice<STR>> a ,CREF<Slice<STR>> b ,CREF<Slice<STR>> c) {
		mWhat.m1st[0] = a ;
		mWhat.m1st[1] = b ;
		mWhat.m1st[2] = c ;
	}

	Slice<STR> what (CREF<INDEX> index) const {
		assert (operator_between (index ,0 ,3)) ;
		return mWhat.m1st[index] ;
	}

	void raise () {
		throw thiz ;
	}
} ;

struct FUNCTION_unimplemented {
	forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

struct AutoLayout {
	BoxLayout mValue ;
} ;

struct AutoHolder implement Interface {
	imports VFat<AutoHolder> create (VREF<AutoLayout> that) ;
	imports CFat<AutoHolder> create (CREF<AutoLayout> that) ;

	virtual void initialize (RREF<BoxLayout> value) = 0 ;
	virtual void fetch (CREF<Unknown> holder ,VREF<BoxLayout> out) = 0 ;
} ;

class Auto implement AutoLayout {
private:
	using FAKE_SIZE = ENUM<1024> ;
	using FAKE_ALIGN = RANK8 ;

protected:
	using AutoLayout::mValue ;
	Storage<FAKE_SIZE ,FAKE_ALIGN> mStorage ;

public:
	implicit Auto () = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
	implicit Auto (RREF<ARG1> that) {
		require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,FAKE_SIZE>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,FAKE_ALIGN>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		AutoHolder::create (thiz)->initialize (move (rax)) ;
	}

	template <class ARG1>
	forceinline ARG1 fetch (TYPEID<ARG1>) {
		auto rax = Box<ARG1> () ;
		AutoHolder::create (thiz)->fetch (BoxUnknownBinder<ARG1> () ,rax) ;
		return move (rax.self) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () rightvalue {
		return fetch (TYPE<ARG1>::expr) ;
	}
} ;
} ;