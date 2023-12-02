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

struct FUNCTION_unsafe_switch {
	inline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	inline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto unsafe_switch = FUNCTION_unsafe_switch () ;

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

template <class A>
struct TEMPLATE_unsafe_cast {
	template <class ARG1>
	inline VREF<A> operator() (VREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return reinterpret_cast<VREF<R1X>> (a) ;
	}

	template <class ARG1>
	inline CREF<A> operator() (CREF<ARG1> a) const noexcept {
		using R1X = A ;
		using R2X = ARG1 ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<R1X> ,ALIGN_OF<R2X>>) ;
		return reinterpret_cast<CREF<R1X>> (a) ;
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

struct FUNCTION_address {
	template <class ARG1>
	inline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&reinterpret_cast<CREF<int>> (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_swap {
	template <class ARG1>
	inline void operator() (VREF<ARG1> obj1 ,VREF<ARG1> obj2) const noexcept {
		//@warn: no class should depend on its address
		auto &&tmp1 = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (obj1) ;
		auto &&tmp2 = unsafe_cast[TYPE<TEMP<ARG1>>::expr] (obj2) ;
		const auto r1x = tmp1 ;
		tmp1 = tmp2 ;
		tmp2 = r1x ;
		noop (obj1) ;
		noop (obj2) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (VREF<ARG1> obj1 ,RREF<ARG2> obj2) const noexcept {
		thiz (obj1 ,obj2) ;
	}

	template <class ARG1 ,class ARG2>
	inline void operator() (RREF<ARG1> obj1 ,VREF<ARG2> obj2) const noexcept {
		thiz (obj1 ,obj2) ;
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

struct FUNCTION_init {
	template <class ARG1>
	inline void operator() (VREF<ARG1> a) const noexcept {
		require (IS_TRIVIAL<ARG1>) ;
		using R1X = TEMP<ARG1> ;
		auto &&tmp1 = unsafe_cast[TYPE<R1X>::expr] (a) ;
		tmp1 = {0} ;
	}
} ;

static constexpr auto init = FUNCTION_init () ;

struct FUNCTION_drop {
	template <class ARG1>
	inline void operator() (VREF<ARG1> a) const noexcept {
		require (IS_TEMP<ARG1>) ;
		using R1X = TEMP_BASE<ARG1> ;
		auto &&tmp1 = unsafe_cast[TYPE<R1X>::expr] (a) ;
		tmp1.~R1X () ;
	}
} ;

static constexpr auto drop = FUNCTION_drop () ;

struct FUNCTION_invoke {
	template <class ARG1>
	inline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

struct FUNCTION_operator_min {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const noexcept {
		if (obj1 <= obj2)
			return obj1 ;
		return obj2 ;
	}
} ;

static constexpr auto operator_min = FUNCTION_operator_min () ;

struct FUNCTION_operator_max {
	inline VAL operator() (CREF<VAL> obj1 ,CREF<VAL> obj2) const noexcept {
		if (obj1 >= obj2)
			return obj1 ;
		return obj2 ;
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

class IndexIterator {
protected:
	LENGTH mRank ;
	INDEX mBegin ;
	INDEX mEnd ;

public:
	implicit IndexIterator () = delete ;

	explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = operator_max (begin_ ,end_) ;
		mRank = mEnd - mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return mRank ;
	}

	BOOL good () const {
		return mBegin != mEnd ;
	}

	inline BOOL operator== (CREF<IndexIterator>) const {
		return ifnot (good ()) ;
	}

	inline BOOL operator!= (CREF<IndexIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mBegin ;
	}

	inline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mBegin++ ;
	}

	inline void operator++ () {
		next () ;
	}
} ;

struct PIXEL {
	INDEX x ;
	INDEX y ;
} ;

class PixelIterator {
protected:
	LENGTH mRank ;
	PIXEL mBegin ;
	PIXEL mEnd ;

public:
	implicit PixelIterator () = delete ;

	explicit PixelIterator (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) {
		mBegin.x = begin_x ;
		mBegin.y = begin_y ;
		mEnd.x = operator_min (begin_x ,end_x) ;
		mEnd.y = operator_min (begin_y ,end_y) ;
		mRank = (mEnd.x - mBegin.x) * (mEnd.y - mBegin.y) ;
		if (mRank > 0)
			return ;
		mBegin = mEnd ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return mRank ;
	}

	BOOL good () const {
		return mBegin.y != mEnd.y ;
	}

	inline BOOL operator== (CREF<PixelIterator>) const {
		return ifnot (good ()) ;
	}

	inline BOOL operator!= (CREF<PixelIterator>) const {
		return good () ;
	}

	CREF<PIXEL> peek () const leftvalue {
		return mBegin ;
	}

	inline CREF<PIXEL> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mBegin.x++ ;
		if (mBegin.x < mEnd.x)
			return ;
		mBegin.x = 0 ;
		mBegin.y++ ;
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

struct Visitor implement Interface {
	virtual void begin () const = 0 ;
	virtual void end () const = 0 ;
	virtual FLAG push (CREF<BYTE> a) const = 0 ;
	virtual FLAG push (CREF<WORD> a) const = 0 ;
	virtual FLAG push (CREF<CHAR> a) const = 0 ;
	virtual FLAG push (CREF<QUAD> a) const = 0 ;
} ;

class Pointer implement Proxy {
public:
	imports VREF<Pointer> make (CREF<FLAG> that) {
		return *reinterpret_cast<VPTR<Pointer>> (that) ;
	}

	template <class ARG1>
	imports VREF<Pointer> from (CREF<ARG1> that) {
		return *reinterpret_cast<VREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports CREF<Pointer> from (CREF<ARG1> that) {
		return *reinterpret_cast<CPTR<Pointer>> (that) ;
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
class VFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit VFat () = delete ;

	inline VPTR<A> operator-> () rightvalue {
		return (&reinterpret_cast<VREF<A>> (thiz)) ;
	}
} ;

template <class A>
class CFat {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit CFat () = delete ;

	inline CPTR<A> operator-> () rightvalue {
		return (&reinterpret_cast<VREF<A>> (thiz)) ;
	}
} ;

class BoxLayout {
public:
	implicit BoxLayout () noexcept {
		//@mark
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

struct BoxHolder implement Interface {
	virtual void initialize () = 0 ;
} ;

template <class A>
class Box implement BoxLayout {
public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {}

	template <class...ARG1>
	imports Box make (XREF<ARG1>...initval) {
		Box ret ;
		//@mark
		return move (ret) ;
	}

	BOOL exist () const {
		return TRUE ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		//@mark
		return Pointer::make (0) ;
	}

	inline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		//@mark
		return Pointer::make (0) ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	void acquire (CREF<BoxLayout> a) {
		//@mark
	}

	template <class ARG1 ,class...ARG2>
	void remake (TYPEID<ARG1> ,XREF<ARG2>...initval) {
		//@mark
	}

	void release () {
		//@mark
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

class HeapProcLayout {
public:
	FLAG mData ;
} ;

struct HeapProcHolder implement Interface {
	imports VFat<HeapProcHolder> create () ;

	virtual LENGTH length () const = 0 ;
	virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
	virtual void free (CREF<FLAG> addr) const = 0 ;
} ;

class HeapProc implement HeapProcLayout {
public:
	imports LENGTH length () {
		return HeapProcHolder::create ()->length () ;
	}

	imports FLAG alloc (CREF<LENGTH> size_) {
		return HeapProcHolder::create ()->alloc (size_) ;
	}

	imports void free (CREF<FLAG> addr) {
		return HeapProcHolder::create ()->free (addr) ;
	}
} ;

class RefLayout {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RefLayout () noexcept {
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

struct RefHolder implement Interface {
	imports VFat<RefHolder> create (VREF<RefLayout> that) ;
	imports CFat<RefHolder> create (CREF<RefLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefLayout clone () const = 0 ;
	virtual void destroy () = 0 ;
	virtual void acquire (CREF<BoxLayout> a) = 0 ;
	virtual void release (VREF<BoxLayout> a) = 0 ;
} ;

template <class A>
class VRef implement RefLayout {
public:
	implicit VRef () = default ;

	implicit VRef (CREF<typeof (NULL)>) {}

	implicit VRef (RREF<RefLayout> that) :VRef (keep[TYPE<RREF<VRef>>::expr] (that)) {}

	template <class...ARG1>
	imports VRef make (XREF<ARG1>...initval) {
		VRef ret ;
		auto rax = Box<A>::make (forward[TYPE<ARG1>::expr] (initval)...) ;
		RefHolder::create (ret)->acquire (rax) ;
		rax.release () ;
		return move (ret) ;
	}

	imports VRef reference (VREF<A> that) {
		VRef ret ;
		ret.mPointer = address (that) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return mPointer != ZERO ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return Pointer::make (mPointer) ;
	}

	inline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return Pointer::make (mPointer) ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}
} ;

template <class A>
class CRef implement RefLayout {
public:
	implicit CRef () = default ;

	implicit CRef (CREF<typeof (NULL)>) {}

	implicit CRef (RREF<RefLayout> that) :CRef (keep[TYPE<RREF<CRef>>::expr] (that)) {}

	template <class...ARG1>
	imports CRef make (XREF<ARG1>...initval) {
		return VRef<A>::make (keep[TYPE<ARG1>::expr] (initval)...) ;
	}

	imports CRef reference (CREF<A> that) {
		CRef ret ;
		ret.mPointer = address (that) ;
		return move (ret) ;
	}

	imports CRef reference (RREF<A>) = delete ;

	implicit CRef (CREF<CRef> that) :CRef (RefHolder::create (that)->clone ()) {}

	inline VREF<CRef> operator= (CREF<CRef> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit CRef (RREF<CRef>) = default ;

	inline VREF<CRef> operator= (RREF<CRef>) = default ;

	BOOL exist () const {
		return mPointer != ZERO ;
	}

	inline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	inline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	CREF<A> self_m () const leftvalue {
		return Pointer::make (mPointer) ;
	}

	inline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}
} ;

class CaptureLayout {
protected:
	LENGTH mRank ;

public:
	implicit CaptureLayout () noexcept {
		mRank = 0 ;
	}
} ;

template <class...PARAMS>
class Capture implement CaptureLayout {
private:
	using RANK = COUNT_OF<TYPE<PARAMS...>> ;

	struct NODE {
		ARR<FLAG ,RANK> mValue ;
	} ;

protected:
	NODE mCapture ;

public:
	implicit Capture () = delete ;

	explicit Capture (CREF<KILL<FLAG ,PARAMS>>...params) {
		mCapture = NODE {params...} ;
	}

	template <class ARG1>
	inline void operator() (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return thiz (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	inline void operator() (CREF<ARG1> func ,TYPEID<TYPE<ARG2...>>) const {
		return func (keep[TYPE<CREF<PARAMS>>::expr] (Pointer::make (mCapture.mValue[ARG2::expr]))...) ;
	}
} ;

struct FUNCTION_capture {
	template <class...ARG1>
	inline Capture<ARG1...> operator() (CREF<ARG1>...initval) const noexcept {
		return Capture<ARG1...> (address (initval)...) ;
	}
} ;

static constexpr auto capture = FUNCTION_capture () ;

class SliceLayout {
public:
	FLAG mBegin ;
	FLAG mEnd ;
	LENGTH mStep ;

public:
	implicit SliceLayout () noexcept {
		mBegin = ZERO ;
		mEnd = ZERO ;
		mStep = 0 ;
	}
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> create (VREF<SliceLayout> that) ;
	imports CFat<SliceHolder> create (CREF<SliceLayout> that) ;

	virtual void initialize (CREF<Pointer> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual RREF<Pointer> at (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class Slice implement SliceLayout {
public:
	implicit Slice () = default ;

	template <class ARG1>
	explicit Slice (CREF<ARG1> text) {
		//@mark
	}

	template <class ARG1>
	explicit Slice (RREF<Slice> prefix ,CREF<ARG1> text) {
		//@mark
	}

	LENGTH size () const {
		return SliceHolder::create (thiz)->size () ;
	}

	A at (CREF<INDEX> index) const {
		return mThis->at (index) ;
	}

	inline A operator[] (CREF<INDEX> index) const {
		return at (index) ;
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

	FLAG visit (CREF<Visitor> visitor) const {
		return SliceHolder::create (thiz)->visit (visitor) ;
	}
} ;

class ClazzLayout {
public:
	FLAG mData ;
} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> create (VREF<ClazzLayout> that) ;
	imports CFat<ClazzHolder> create (CREF<ClazzLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_cabi () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
	virtual BOOL equal (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG visit (CREF<Visitor> visitor) const = 0 ;
} ;

class Clazz implement ClazzLayout {
public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPEID<ARG1>) {
		const auto r1x = SIZE_OF<ARG1>::expr ;
		const auto r2x = ALIGN_OF<ARG1>::expr ;
		const auto r3x = Slice<STR> ("") ;
		ClazzHolder::create (thiz)->initialize (r1x ,r2x ,r3x) ;
	}

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

	FLAG visit (CREF<Visitor> visitor) const {
		return ClazzHolder::create (thiz)->visit (visitor) ;
	}
} ;

class Exception {
protected:
	Slice<STR> mWhat ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice<STR>> what_) {
		mWhat = what_ ;
	}

	Slice<STR> what () const {
		return mWhat ;
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

class AutoLayout {} ;

struct AutoHolder implement Interface {
	imports VFat<AutoHolder> create (VREF<AutoLayout> that) ;
	imports CFat<AutoHolder> create (CREF<AutoLayout> that) ;

	virtual void initialize (VREF<BoxLayout> value) = 0 ;
	virtual VREF<Pointer> poll () const leftvalue = 0 ;
} ;

class Auto implement AutoLayout {
private:
	Box<AutoHolder> mThis ;
	Storage<ENUM<1024> ,RANK8> mValue ;

public:
	implicit Auto () = delete ;

	template <class ARG1>
	explicit Auto (RREF<ARG1> that) {
		using R1X = ARG1 ;
		auto rax = Box<R1X>::make (keep[TYPE<ARG1>::expr] (that)) ;
		AutoHolder::create (thiz)->initialize (rax) ;
		rax.release () ;
	}

	BOOL exist () const {
		return mThis.exist () ;
	}

	template <class ARG1>
	ARG1 poll (TYPEID<ARG1>) const {
		ARG1 ret = move (mThis->poll ()) ;
		mThis = NULL ;
		return move (ret) ;
	}
} ;
} ;