#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"

namespace CSC {
struct FUNCTION_noop {
	inline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	inline void operator() (CREF<ARG1> a) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_unsafe_do {
	inline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	inline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto unsafe_ifdo = FUNCTION_unsafe_do () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__debugbreak () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_break {
	inline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

static constexpr auto unsafe_break = FUNCTION_unsafe_break () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_unsafe_abort {
	inline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_unsafe_abort {
	inline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_unsafe_abort {
	inline void operator() () const noexcept {
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
	imports VREF<Pointer> from (VREF<ARG1> that) {
		return reinterpret_cast<VREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports CREF<Pointer> from (CREF<ARG1> that) {
		return reinterpret_cast<CREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports RREF<Pointer> from (RREF<ARG1> that) {
		return reinterpret_cast<RREF<Pointer>> (that) ;
	}

	template <class ARG1>
	inline operator VREF<ARG1> () leftvalue {
		return reinterpret_cast<VREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	inline operator CREF<ARG1> () const leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	inline operator RREF<ARG1> () rightvalue {
		return reinterpret_cast<RREF<ARG1>> (thiz) ;
	}
} ;

template <class A>
struct TEMPLATE_unsafe_cast {
	template <class ARG1>
	inline VREF<A> operator() (VREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return Pointer::from (a) ;
	}

	template <class ARG1>
	inline CREF<A> operator() (CREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return Pointer::from (a) ;
	}

	template <class ARG1>
	inline CREF<A> operator() (RREF<ARG1>) const noexcept = delete ;
} ;

struct FUNCTION_unsafe_cast {
	template <class ARG1>
	inline consteval TEMPLATE_unsafe_cast<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return TEMPLATE_unsafe_cast<ARG1> () ;
	}
} ;

static constexpr auto unsafe_cast = FUNCTION_unsafe_cast () ;

struct FUNCTION_bitwise {
	template <class ARG1>
	inline BYTE_BASE<ARG1> operator() (CREF<ARG1> a) const noexcept {
		return Pointer::from (a) ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_address {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_swap {
	template <class ARG1>
	inline void operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
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
	inline void operator() (VREF<ARG1> a ,RREF<ARG2> b) const noexcept {
		thiz (a ,b) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (RREF<ARG1> a ,VREF<ARG2> b) const noexcept {
		thiz (a ,b) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

template <class A>
struct TEMPLATE_keep {
	template <class ARG1>
	inline XREF<A> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}

	template <class ARG1>
	inline XREF<A> operator() (CREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}

	template <class ARG1>
	inline XREF<A> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<XREF<A>> (a) ;
	}
} ;

struct FUNCTION_keep {
	template <class ARG1>
	inline consteval TEMPLATE_keep<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return TEMPLATE_keep<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	inline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	inline ARG1 operator() (CREF<ARG1> a) const {
		require (IS_CLONEABLE<ARG1>) ;
		return keep[TYPE<CREF<ARG1>>::expr] (a) ;
	}

	template <class ARG1>
	inline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return keep[TYPE<RREF<ARG1>>::expr] (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_invoke {
	template <class ARG1>
	inline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

struct FUNCTION_operator_min {
	inline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto operator_min = FUNCTION_operator_min () ;

struct FUNCTION_operator_max {
	inline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto operator_max = FUNCTION_operator_max () ;

struct FUNCTION_operator_between {
	inline BOOL operator() (CREF<VAL> curr ,CREF<VAL> begin_ ,CREF<VAL> end_) const noexcept {
		if (curr < begin_)
			return FALSE ;
		if (curr >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto operator_between = FUNCTION_operator_between () ;

struct FUNCTION_operator_alignas {
	inline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const noexcept {
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
	inline FLAG operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return invoke (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	FLAG invoke (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	FLAG invoke (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
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
	inline void operator() (CREF<Visitor> visitor ,CREF<ARG1> a) const noexcept {
		return invoke (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	inline void invoke (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		visitor.push (bitwise (a)) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	inline void invoke (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return a.visit (visitor) ;
	}
} ;

static constexpr auto operator_visit = FUNCTION_operator_visit () ;

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

	inline BOOL operator== (CREF<IndexIterator>) const {
		return ifnot (good ()) ;
	}

	inline BOOL operator!= (CREF<IndexIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	inline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek++ ;
	}

	inline void operator++ () {
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
		mEnd.mX = operator_min (begin_x ,end_x) ;
		mEnd.mY = operator_min (begin_y ,end_y) ;
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

	inline BOOL operator== (CREF<PixelIterator>) const {
		return ifnot (good ()) ;
	}

	inline BOOL operator!= (CREF<PixelIterator>) const {
		return good () ;
	}

	CREF<PIXEL> peek () const leftvalue {
		return mPeek ;
	}

	inline CREF<PIXEL> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek.mX++ ;
		if (mPeek.mX < mEnd.mX)
			return ;
		mPeek.mX = 0 ;
		mPeek.mY++ ;
	}

	inline void operator++ () {
		next () ;
	}
} ;

struct FUNCTION_iter {
	inline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	inline PixelIterator operator() (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) const noexcept {
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
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit VFat () = delete ;

	template <class ARG1>
	explicit VFat (CREF<A> holder ,VREF<ARG1> pointer) {
		mHolder = Pointer::from (holder) ;
		mPointer = address (pointer) ;
	}

	inline VPTR<A> operator-> () rightvalue {
		return (&unsafe_cast[TYPE<A>::expr] (mHolder)) ;
	}
} ;

template <class A>
class CFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit CFat () = delete ;

	template <class ARG1>
	explicit CFat (CREF<A> holder ,CREF<ARG1> pointer) {
		mHolder = Pointer::from (holder) ;
		mPointer = address (pointer) ;
	}

	inline CPTR<A> operator-> () rightvalue {
		return (&unsafe_cast[TYPE<A>::expr] (mHolder)) ;
	}
} ;

struct Unknown implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual void destroy (CREF<LENGTH> size_) noexcept = 0 ;
} ;

template <class A>
class UnknownBinder final implement Unknown {
protected:
	A mValue ;

public:
	template <class...ARG1>
	explicit UnknownBinder (XREF<ARG1>...initval) :mValue (keep[TYPE<XREF<ARG1>>::expr] (initval)...) {}

	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}

	void destroy (CREF<LENGTH> size_) noexcept override {
		auto &&rax = unsafe_cast[TYPE<ARR<A ,RANK1>>::expr] (mValue) ;
		for (auto &&i : iter (0 ,size_)) {
			rax[i].~A () ;
		}
	}
} ;

class BoxLayout ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> create (VREF<BoxLayout> that) ;
	imports CFat<BoxHolder> create (CREF<BoxLayout> that) ;

	virtual void initialize (CREF<Unknown> that) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual void acquire (CREF<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

class BoxLayout {
public:
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept {
		if (mHolder == ZERO)
			return ;
		BoxHolder::create (thiz)->destroy () ;
		mHolder = ZERO ;
	}

	implicit BoxLayout (CREF<BoxLayout>) = delete ;

	inline VREF<BoxLayout> operator= (CREF<BoxLayout>) = delete ;

	implicit BoxLayout (RREF<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	inline VREF<BoxLayout> operator= (RREF<BoxLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class Box implement BoxLayout {
protected:
	TEMP<A> mValue ;

public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {}

	template <class...ARG1>
	imports Box make (XREF<ARG1>...initval) {
		Box ret ;
		const auto r1x = DEF<csc_temp_t *> (&ret) ;
		new (r1x) UnknownBinder<A> (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::create (thiz)->exist () ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	inline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	inline CPTR<A> operator-> () const leftvalue {
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
	inline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		using R1X = FUNCTION_RETURN<ARG1> ;
		static R1X mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

class RefLayout ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> create (VREF<RefLayout> that) ;
	imports CFat<RefHolder> create (CREF<RefLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual RefLayout share () const = 0 ;
} ;

class RefLayout {
public:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RefLayout () noexcept {
		mHolder = ZERO ;
		mPointer = ZERO ;
	}

	implicit ~RefLayout () noexcept {
		if (mHolder == ZERO)
			return ;
		RefHolder::create (thiz)->destroy () ;
		mHolder = ZERO ;
		mPointer = ZERO ;
	}

	implicit RefLayout (CREF<RefLayout>) = delete ;

	inline VREF<RefLayout> operator= (CREF<RefLayout>) = delete ;

	implicit RefLayout (RREF<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	inline VREF<RefLayout> operator= (RREF<RefLayout> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}
} ;

template <class A>
class Ref implement RefLayout {
public:
	implicit Ref () = default ;

	implicit Ref (CREF<typeof (NULL)>) {}

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<A ,ARG1>>>
	implicit Ref (RREF<Ref<ARG1>> that) :Ref (keep[TYPE<RREF<Ref>>::expr] (that)) {}

	template <class...ARG1>
	imports Ref make (XREF<ARG1>...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		const auto r1x = 1 - IS_TRIVIAL<A>::expr ;
		RefHolder::create (ret)->initialize (rax ,r1x) ;
		rax.release () ;
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

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	inline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	Ref share () const {
		auto rax = RefHolder::create (thiz)->share () ;
		return keep[TYPE<RREF<Ref>>::expr] (rax) ;
	}
} ;

template <class A>
class RefBase {
protected:
	Ref<A> mThis ;

public:
	inline operator VREF<Ref<A>> () leftvalue {
		return mThis ;
	}

	inline operator CREF<Ref<A>> () const leftvalue {
		return mThis ;
	}
} ;

class HeapProcLayout ;

struct HeapProcHolder implement Interface {
	imports VFat<HeapProcHolder> create (VREF<Ref<HeapProcLayout>> that) ;
	imports CFat<HeapProcHolder> create (CREF<Ref<HeapProcLayout>> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
	virtual void free (CREF<FLAG> addr) const = 0 ;
} ;

class HeapProc implement RefBase<HeapProcLayout> {
public:
	imports CREF<HeapProc> instance () {
		return memorize ([&] () {
			HeapProc ret ;
			HeapProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH length () {
		return HeapProcHolder::create (instance ())->length () ;
	}

	imports FLAG alloc (CREF<LENGTH> size_) {
		return HeapProcHolder::create (instance ())->alloc (size_) ;
	}

	imports void free (CREF<FLAG> addr) {
		return HeapProcHolder::create (instance ())->free (addr) ;
	}
} ;

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

class CaptureLayout {
public:
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
	inline void operator() (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return invoke (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	inline void invoke (CREF<ARG1> func ,TYPEID<TYPE<ARG2...>>) const {
		return func (keep[TYPE<CREF<PARAMS>>::expr] (Pointer::make (mCapture.m1st[ARG2::expr]))...) ;
	}
} ;

struct FUNCTION_capture {
	template <class...ARG1>
	inline Capture<ARG1...> operator() (CREF<ARG1>...initval) const noexcept {
		return Capture<ARG1...> (address (initval)...) ;
	}
} ;

static constexpr auto capture = FUNCTION_capture () ;

struct SliceData {
	FLAG mBegin ;
	FLAG mEnd ;
	LENGTH mStep ;
} ;

class SliceLayout ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> create (VREF<Ref<SliceLayout>> that) ;
	imports CFat<SliceHolder> create (CREF<Ref<SliceLayout>> that) ;

	virtual void initialize (CREF<SliceData> data) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual STRU32 get (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<Ref<SliceLayout>> that) const = 0 ;
	virtual FLAG compr (CREF<Ref<SliceLayout>> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class Slice implement RefBase<SliceLayout> {
public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	explicit Slice (CREF<ARG1> that) {
		auto &&rax = memorize ([&] () {
			SliceData ret ;
			ret.mBegin = address (that) ;
			ret.mEnd = ret.mBegin + SIZE_OF<ARG1>::expr / ALIGN_OF<ARG1>::expr ;
			ret.mStep = ALIGN_OF<ARG1>::expr ;
			return move (ret) ;
		}) ;
		SliceHolder::create (thiz)->initialize (rax) ;
	}

	implicit Slice (CREF<Slice> that) {
		mThis = that.mThis.share () ;
	}

	implicit VREF<Slice> operator= (CREF<Slice> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Slice (RREF<Slice>) = default ;

	implicit VREF<Slice> operator= (RREF<Slice>) = default ;

	LENGTH size () const {
		return SliceHolder::create (thiz)->size () ;
	}

	A get (CREF<INDEX> index) const {
		return A (SliceHolder::create (thiz)->get (index)) ;
	}

	inline A operator[] (CREF<INDEX> index) const {
		return get (index) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Slice> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<Slice> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Slice> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return SliceHolder::create (thiz)->visit (visitor) ;
	}
} ;

struct ClazzData {
	LENGTH mTypeSize ;
	LENGTH mTypeAlign ;
	Slice<STR> mTypeName ;
} ;

class ClazzLayout ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> create (VREF<Ref<ClazzLayout>> that) ;
	imports CFat<ClazzHolder> create (CREF<Ref<ClazzLayout>> that) ;

	virtual void initialize (CREF<ClazzData> data) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_cabi () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
	virtual BOOL equal (CREF<Ref<ClazzLayout>> that) const = 0 ;
	virtual FLAG compr (CREF<Ref<ClazzLayout>> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

class Clazz implement RefBase<ClazzLayout> {
public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPEID<ARG1>) {
		const auto r1x = Slice<STR> (__FUNCSIG__) ;
		auto &&rax = memorize ([&] () {
			ClazzData ret ;
			ret.mTypeSize = SIZE_OF<ARG1>::expr ;
			ret.mTypeAlign = ALIGN_OF<ARG1>::expr ;
			ret.mTypeName = r1x ;
			return move (ret) ;
		}) ;
		ClazzHolder::create (thiz)->initialize (rax) ;
	}

	implicit Clazz (CREF<Clazz> that) {
		mThis = that.mThis.share () ;
	}

	implicit VREF<Clazz> operator= (CREF<Clazz> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Clazz (RREF<Clazz>) = default ;

	implicit VREF<Clazz> operator= (RREF<Clazz>) = default ;

	LENGTH type_size () const {
		return ClazzHolder::create (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::create (thiz)->type_align () ;
	}

	FLAG type_cabi () const {
		return ClazzHolder::create (thiz)->type_cabi () ;
	}

	Slice<STR> type_name () const {
		return ClazzHolder::create (thiz)->type_name () ;
	}

	BOOL equal (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Clazz> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return ClazzHolder::create (thiz)->visit (visitor) ;
	}
} ;

class Exception {
protected:
	Slice<STR> mWhatA ;
	Slice<STR> mWhatB ;
	Slice<STR> mWhatC ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice<STR>> a) {
		mWhatA = a ;
	}

	explicit Exception (CREF<Slice<STR>> a ,CREF<Slice<STR>> b) {
		mWhatA = a ;
		mWhatB = b ;
	}

	explicit Exception (CREF<Slice<STR>> a ,CREF<Slice<STR>> b ,CREF<Slice<STR>> c) {
		mWhatA = a ;
		mWhatB = b ;
		mWhatC = c ;
	}

	Slice<STR> what (CREF<INDEX> index) const {
		if (index == 0)
			return mWhatA ;
		if (index == 1)
			return mWhatB ;
		if (index == 2)
			return mWhatC ;
		return Slice<STR> () ;
	}

	void raise () {
		throw thiz ;
	}
} ;

struct FUNCTION_unimplemented {
	inline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

class AutoLayout {
public:
	BoxLayout mThis ;
} ;

struct AutoHolder implement Interface {
	imports VFat<AutoHolder> create (VREF<AutoLayout> that) ;
	imports CFat<AutoHolder> create (CREF<AutoLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual void poll (VREF<BoxLayout> out) const = 0 ;
} ;

class Auto implement AutoLayout {
private:
	using FAKE_SIZE = ENUM<1024> ;
	using FAKE_ALIGN = RANK8 ;

protected:
	Storage<FAKE_SIZE ,FAKE_ALIGN> mValue ;

public:
	implicit Auto () = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
	implicit Auto (RREF<ARG1> that) {
		require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,FAKE_SIZE>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,FAKE_ALIGN>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		AutoHolder::create (thiz)->initialize (rax) ;
		rax.release () ;
	}

	BOOL exist () const {
		return AutoHolder::create (thiz)->exist () ;
	}

	template <class ARG1>
	ARG1 poll (TYPEID<ARG1>) const {
		Box<ARG1> ret ;
		AutoHolder::create (thiz)->poll (ret) ;
		return move (ret.self) ;
	}
} ;
} ;