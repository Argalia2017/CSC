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

struct FUNCTION_inline_ifdo {
	forceinline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	forceinline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto inline_ifdo = FUNCTION_inline_ifdo () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__debugbreak () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

static constexpr auto inline_break = FUNCTION_inline_break () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		throw ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		__builtin_abort () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		__builtin_abort () ;
	}
} ;
#endif

static constexpr auto inline_abort = FUNCTION_inline_abort () ;

struct FUNCTION_inline_barrier {
	template <class ARG1>
	forceinline void operator() (TYPE<ARG1> ,CREF<csc_const_pointer_t> a) const noexcept {
		static volatile auto mInstance = PTR<void (csc_const_pointer_t)> (NULL) ;
		if (mInstance == NULL)
			return ;
		mInstance (a) ;
	}
} ;

static constexpr auto inline_barrier = FUNCTION_inline_barrier () ;

class Pointer implement Proxy {
public:
	imports VREF<Pointer> make (CREF<FLAG> that) {
		return (*reinterpret_cast<PTR<VREF<Pointer>>> (that)) ;
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
	imports RREF<Pointer> from (RREF<ARG1> that) = delete ;

	template <class ARG1>
	forceinline operator VREF<ARG1> () leftvalue {
		return reinterpret_cast<VREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator CREF<ARG1> () leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () leftvalue = delete ;

	template <class ARG1>
	forceinline operator ARG1 () leftvalue = delete ;

	template <class ARG1>
	forceinline operator VREF<ARG1> () const leftvalue = delete ;

	template <class ARG1>
	forceinline operator CREF<ARG1> () const leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () const leftvalue = delete ;

	template <class ARG1>
	forceinline operator ARG1 () const leftvalue = delete ;
} ;

struct FUNCTION_inline_type_name {
	//@fatal: fuck cpp
	imports FLAG invoke (CREF<csc_type_info_t> squalor) noexcept ;

	forceinline FLAG operator() (CREF<csc_type_info_t> squalor) const noexcept {
		return invoke (squalor) ;
	}
} ;

static constexpr auto inline_type_name = FUNCTION_inline_type_name () ;

struct FUNCTION_inline_list_begin {
	//@fatal: fuck cpp
	imports FLAG invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept ;

	forceinline FLAG operator() (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) const noexcept {
		return invoke (squalor ,step_) ;
	}
} ;

static constexpr auto inline_list_begin = FUNCTION_inline_list_begin () ;

struct FUNCTION_inline_list_end {
	//@fatal: fuck cpp
	imports FLAG invoke (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) noexcept ;

	forceinline FLAG operator() (CREF<csc_initializer_list_t<Pointer>> squalor ,CREF<LENGTH> step_) const noexcept {
		return invoke (squalor ,step_) ;
	}
} ;

static constexpr auto inline_list_end = FUNCTION_inline_list_end () ;

struct FUNCTION_inline_memset {
	imports void invoke (VREF<Pointer> dst ,CREF<LENGTH> size_) noexcept ;

	forceinline void operator() (VREF<Pointer> dst ,CREF<LENGTH> size_) const noexcept {
		return invoke (dst ,size_) ;
	}

	template <class ARG1>
	forceinline void operator() (VREF<ARG1> dst) const noexcept {
		require (IS_TRIVIAL<ARG1>) ;
		return invoke (Pointer::from (dst) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memset = FUNCTION_inline_memset () ;

struct FUNCTION_inline_memcpy {
	imports void invoke (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept ;

	forceinline void operator() (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) const noexcept {
		return invoke (dst ,src ,size_) ;
	}

	template <class ARG1>
	forceinline void operator() (VREF<ARG1> dst ,CREF<ARG1> src) {
		require (IS_TRIVIAL<ARG1>) ;
		return invoke (dst ,src ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcpy = FUNCTION_inline_memcpy () ;

struct FUNCTION_inline_memcmp {
	imports FLAG invoke (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept ;

	forceinline FLAG operator() (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) const noexcept {
		return invoke (dst ,src ,size_) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> dst ,CREF<ARG1> src) {
		require (IS_TRIVIAL<ARG1>) ;
		return invoke (dst ,src ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcmp = FUNCTION_inline_memcmp () ;

struct FUNCTION_address {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,ORDINARY>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<VREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<VREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline VREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

		template <class ARG1>
		forceinline VREF<A> operator() (RREF<ARG1> a) const noexcept = delete ;
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline CREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,REGISTER>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline RREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline RREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

		template <class ARG1>
		forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}
	} ;
} ;

template <class A>
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<REMOVE_REF<A> ,REFLECT_REF<A> ,ALWAYS>::FUNCTION_keep_impl ;

struct FUNCTION_keep {
	template <class ARG1>
	forceinline consteval FUNCTION_keep_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_keep_impl<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	forceinline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline ARG1 operator() (CREF<ARG1> a) const {
		return static_cast<CREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_swap {
	template <class ARG1>
	forceinline void operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		//@warn: no class should depend on its address
		auto &&tmp1 = keep[TYPE<Temp<ARG1>>::expr] (Pointer::from (a)) ;
		auto &&tmp2 = keep[TYPE<Temp<ARG1>>::expr] (Pointer::from (b)) ;
		const auto r1x = tmp1 ;
		tmp1 = tmp2 ;
		tmp2 = r1x ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

struct FUNCTION_assign {
	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		a.~ARG1 () ;
		new (Pointer::from (a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		a.~ARG1 () ;
		new (Pointer::from (a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,RREF<ARG1> b) const noexcept = delete ;
} ;

static constexpr auto assign = FUNCTION_assign () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (VREF<ARG1> a ,CREF<ARG1> expect ,CREF<ARG2> item) const noexcept {
		if (a != expect)
			return ;
		a = ARG1 (item) ;
	}
} ;

static constexpr auto replace = FUNCTION_replace () ;

struct FUNCTION_invoke {
	template <class ARG1>
	forceinline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

template <class A>
struct FUNCTION_bitwise_impl {
	template <class ARG1>
	forceinline A operator() (CREF<ARG1> a) const noexcept {
		require (IS_TRIVIAL<A>) ;
		using R1X = A ;
		using R2X = CONDITIONAL<IS_SAME<ARG1 ,Pointer> ,A ,ARG1> ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		A ret ;
		inline_memcpy (Pointer::from (ret) ,Pointer::from (a) ,SIZE_OF<R1X>::expr) ;
		return move (ret) ;
	}
} ;

struct FUNCTION_bitwise {
	template <class ARG1>
	forceinline consteval FUNCTION_bitwise_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_bitwise_impl<ARG1> () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_inline_hold {
	forceinline FLAG operator() (CREF<Interface> binder) const noexcept {
		return bitwise[TYPE<FLAG>::expr] (binder) ;
	}
} ;

static constexpr auto inline_hold = FUNCTION_inline_hold () ;

struct FUNCTION_inline_min {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_min = FUNCTION_inline_min () ;

struct FUNCTION_inline_max {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_max = FUNCTION_inline_max () ;

struct FUNCTION_inline_between {
	forceinline BOOL operator() (CREF<VAL> curr ,CREF<VAL> lb ,CREF<VAL> rb) const noexcept {
		if (curr < lb)
			return FALSE ;
		if (curr >= rb)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto inline_between = FUNCTION_inline_between () ;

struct FUNCTION_inline_alignas {
	forceinline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const noexcept {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto inline_alignas = FUNCTION_inline_alignas () ;

struct FUNCTION_inline_equal {
	template <class ARG1>
	forceinline BOOL operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return equal_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH3)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a == b ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.equal (b) ;
	}
} ;

static constexpr auto inline_equal = FUNCTION_inline_equal () ;

struct FUNCTION_inline_compr {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return compr_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.compr (b) ;
	}
} ;

static constexpr auto inline_compr = FUNCTION_inline_compr () ;

struct Visitor implement Interface {
	virtual void initialize () = 0 ;
	virtual void begin () const = 0 ;
	virtual void end () const = 0 ;
	virtual void push (CREF<BYTE> a) const = 0 ;
	virtual void push (CREF<WORD> a) const = 0 ;
	virtual void push (CREF<CHAR> a) const = 0 ;
	virtual void push (CREF<QUAD> a) const = 0 ;
	virtual FLAG peek () const = 0 ;
} ;

struct FUNCTION_inline_visit {
	template <class ARG1>
	forceinline void operator() (CREF<Visitor> visitor ,CREF<ARG1> a) const noexcept {
		return visit_impl (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		const auto r1x = bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		visitor.push (r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return a.visit (visitor) ;
	}
} ;

static constexpr auto inline_visit = FUNCTION_inline_visit () ;

class IndexIterator {
protected:
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit IndexIterator () = delete ;

	explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = inline_max (begin_ ,end_) ;
		mPeek = mBegin ;
	}

	LENGTH length () const {
		return mEnd - mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<IndexIterator>) const {
		return !(good ()) ;
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

struct Pixel {
	INDEX mX ;
	INDEX mY ;
} ;

class PixelIterator {
protected:
	Pixel mBegin ;
	Pixel mEnd ;
	Pixel mPeek ;

public:
	implicit PixelIterator () = delete ;

	explicit PixelIterator (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) {
		mBegin.mX = begin_x ;
		mBegin.mY = begin_y ;
		mEnd.mX = inline_max (begin_x ,end_x) ;
		mEnd.mY = inline_max (begin_y ,end_y) ;
		mPeek = mBegin ;
		if (length () > 0)
			return ;
		mPeek = mEnd ;
	}

	LENGTH length () const {
		return (mEnd.mX - mBegin.mX) * (mEnd.mY - mBegin.mY) ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek.mY != mEnd.mY ;
	}

	forceinline BOOL operator== (CREF<PixelIterator>) const {
		return !(good ()) ;
	}

	forceinline BOOL operator!= (CREF<PixelIterator>) const {
		return good () ;
	}

	CREF<Pixel> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<Pixel> operator* () const leftvalue {
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
private:
	require (IS_INTERFACE<A>) ;

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

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_EXTEND<Fat<A ,ARG2> ,ARG1>>>
	explicit VFat (CREF<ARG1> holder ,VREF<ARG2> pointer) {
		mHolder = inline_hold (holder) ;
		mPointer = address (pointer) ;
	}

	VREF<A> self_m () const {
		return Pointer::from (const_cast<VREF<VFat>> (thiz)) ;
	}

	forceinline PTR<VREF<A>> operator-> () const {
		return (&self) ;
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

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_EXTEND<Fat<A ,ARG2> ,ARG1>>>
	explicit CFat (CREF<ARG1> holder ,CREF<ARG2> pointer) {
		mHolder = inline_hold (holder) ;
		mPointer = address (pointer) ;
	}

	CREF<A> self_m () const {
		return Pointer::from (const_cast<CREF<CFat>> (thiz)) ;
	}

	forceinline PTR<CREF<A>> operator-> () const {
		return (&self) ;
	}
} ;

template <class A>
class RFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RFat () = default ;

	explicit RFat (CREF<Unknown> holder) {
		mHolder = holder.reflect (A::expr) ;
		assert (mHolder != ZERO) ;
		mPointer = ZERO ;
	}

	CREF<A> self_m () const {
		return Pointer::from (const_cast<CREF<RFat>> (thiz)) ;
	}

	forceinline PTR<CREF<A>> operator-> () const {
		return (&self) ;
	}
} ;

struct ReflectSize implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 100 ;
	}
} ;

template <class A>
class ReflectSizeBinder implement ReflectSize {
public:
	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}
} ;

struct ReflectCreate implement Interface {
	virtual void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 101 ;
	}
} ;

template <class A>
class ReflectCreateBinder implement ReflectCreate {
public:
	void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		require (IS_DEFAULT<A>) ;
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			new (Pointer::from (tmp[i])) A () ;
		}
	}
} ;

struct ReflectDestroy implement Interface {
	virtual void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 102 ;
	}
} ;

template <class A>
class ReflectDestroyBinder implement ReflectDestroy {
public:
	void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			tmp[i].~A () ;
		}
	}
} ;

struct ReflectMove implement Interface {
	virtual void init (VREF<Pointer> a) const noexcept = 0 ;
	virtual void copy (VREF<Pointer> a ,CREF<Pointer> b) const noexcept = 0 ;
	virtual void drop (VREF<Pointer> a) const noexcept = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 103 ;
	}
} ;

template <class A>
class ReflectMoveBinder implement ReflectMove {
public:
	void init (VREF<Pointer> a) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		inline_memset (a ,SIZE_OF<A>::expr) ;
	}

	void copy (VREF<Pointer> a ,CREF<Pointer> b) const noexcept override {
		inline_memcpy (a ,b ,SIZE_OF<A>::expr) ;
	}

	void drop (VREF<Pointer> a) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<A>::expr] (a) ;
		tmp.~A () ;
	}
} ;

struct ReflectTuple implement Interface {
	virtual LENGTH offset_m1st () const = 0 ;
	virtual LENGTH offset_m2nd () const = 0 ;
	virtual LENGTH offset_m3rd () const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 104 ;
	}
} ;

template <class A>
class ReflectTupleBinder implement ReflectTuple {
public:
	LENGTH offset_m1st () const override {
		return offset_m1st_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<KILL<ENUM_TRUE ,typeof (PTR<CREF<ARG1>> (NULL)->m1st)>>>
	LENGTH offset_m1st_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (PTR<CREF<A>> (NULL)->m1st) ;
	}

	template <class ARG1>
	LENGTH offset_m1st_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH offset_m2nd () const override {
		return offset_m2nd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<KILL<ENUM_TRUE ,typeof (PTR<CREF<ARG1>> (NULL)->m2nd)>>>
	LENGTH offset_m2nd_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (PTR<CREF<A>> (NULL)->m2nd) ;
	}

	template <class ARG1>
	LENGTH offset_m2nd_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH offset_m3rd () const override {
		return offset_m3rd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<KILL<ENUM_TRUE ,typeof (PTR<CREF<ARG1>> (NULL)->m3rd)>>>
	LENGTH offset_m3rd_impl (CREF<typeof (PH2)> ,TYPE<ARG1>) const {
		return address (PTR<CREF<A>> (NULL)->m3rd) ;
	}

	template <class ARG1>
	LENGTH offset_m3rd_impl (CREF<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}
} ;

class UnknownHolder {
protected:
	FLAG mHolder ;

public:
	implicit UnknownHolder () noexcept {
		mHolder = ZERO ;
	}

	implicit UnknownHolder (CREF<Unknown> holder) {
		mHolder = inline_hold (holder) ;
	}

	explicit UnknownHolder (CREF<FLAG> holder) {
		mHolder = holder ;
	}

	BOOL equal (CREF<UnknownHolder> that) const {
		return mHolder == that.mHolder ;
	}

	forceinline BOOL operator== (CREF<UnknownHolder> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<UnknownHolder> that) const {
		return !(equal (that)) ;
	}

	CREF<Unknown> self_m () const leftvalue {
		assert (mHolder != ZERO) ;
		return Pointer::from (mHolder) ;
	}

	forceinline operator CREF<Unknown> () const leftvalue {
		return self ;
	}
} ;

template <class A>
class Pin {
protected:
	mutable Temp<A> mStorage ;

public:
	implicit Pin () noexcept {
		inline_memset (mStorage) ;
	}

	implicit ~Pin () noexcept {
		self.~A () ;
	}

	implicit Pin (CREF<Pin> that) :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (CREF<Pin> that) {
		return thiz ;
	}

	implicit Pin (RREF<Pin> that) noexcept :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (RREF<Pin> that) noexcept {
		return thiz ;
	}

	VREF<A> self_m () const leftvalue {
		return Pointer::from (mStorage) ;
	}

	forceinline PTR<VREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	void assign (CREF<typeof (NULL)>) {
		//@warn: may cause leaks on duplicated call
		new (Pointer::from (mStorage)) A () ;
	}

	forceinline void operator= (CREF<typeof (NULL)>) {
		assign (NULL) ;
	}
} ;

struct FUNCTION_memorize {
	template <class ARG1>
	forceinline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		static const auto mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

template <class A ,class B>
class External {
protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	imports CREF<Pin<External>> instance () {
		return memorize ([&] () {
			return Pin<External> () ;
		}) ;
	}

	imports CREF<Fat<A ,B>> create () {
		return Pointer::from (instance ()) ;
	}

	template <class ARG1>
	imports BOOL declare (RREF<ARG1> func) {
		instance ()->mHolder = func () ;
		return TRUE ;
	}
} ;

struct BoxLayout {
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept ;

	implicit BoxLayout (CREF<BoxLayout> that) = delete ;

	forceinline VREF<BoxLayout> operator= (CREF<BoxLayout> that) = delete ;

	implicit BoxLayout (RREF<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<BoxLayout> operator= (RREF<BoxLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> create (VREF<BoxLayout> that) ;
	imports CFat<BoxHolder> create (CREF<BoxLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual UnknownHolder unknown () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual void acquire (CREF<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

inline BoxLayout::~BoxLayout () noexcept {
	BoxHolder::create (thiz)->destroy () ;
}

template <class A>
class BoxUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Box implement BoxLayout {
protected:
	using BoxLayout::mHolder ;
	Temp<A> mStorage ;

public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	imports Box make (ARG1 &&...initval) {
		Box ret ;
		BoxHolder::create (ret)->initialize (BoxUnknownBinder<A> ()) ;
		new (Pointer::from (ret.mStorage)) A (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::create (thiz)->exist () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return !(exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	UnknownHolder unknown () const {
		return BoxHolder::create (thiz)->unknown () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	void acquire (CREF<BoxLayout> that) {
		return BoxHolder::create (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::create (thiz)->release () ;
	}
} ;

struct RefLayout {
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RefLayout () noexcept {
		mHolder = ZERO ;
		mPointer = ZERO ;
	}

	implicit ~RefLayout () noexcept ;

	implicit RefLayout (CREF<RefLayout> that) = delete ;

	forceinline VREF<RefLayout> operator= (CREF<RefLayout> that) = delete ;

	implicit RefLayout (RREF<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefLayout> operator= (RREF<RefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> create (VREF<RefLayout> that) ;
	imports CFat<RefHolder> create (CREF<RefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual UnknownHolder unknown () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual RefLayout share () const = 0 ;
} ;

inline RefLayout::~RefLayout () noexcept {
	RefHolder::create (thiz)->destroy () ;
}

template <class A>
class RefUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
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
	imports Ref make (ARG1 &&...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
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

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return !(exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	UnknownHolder unknown () const {
		return RefHolder::create (thiz)->unknown () ;
	}

	VREF<A> self_m () leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline PTR<VREF<A>> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline PTR<CREF<A>> operator-> () const leftvalue {
		return (&self) ;
	}

	Ref share () const {
		RefLayout ret = RefHolder::create (thiz)->share () ;
		return move (keep[TYPE<Ref>::expr] (ret)) ;
	}
} ;

struct HeapPureLayout ;

struct HeapLayout {
	FLAG mHolder ;

public:
	implicit HeapLayout () noexcept {
		mHolder = ZERO ;
	}
} ;

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
	using HeapLayout::mHolder ;

public:
	explicit Heap () {
		HeapHolder::create (thiz)->initialize () ;
	}

	LENGTH length () const {
		return HeapHolder::create (thiz)->length () ;
	}

	FLAG stack (CREF<LENGTH> size_) const {
		return HeapHolder::create (thiz)->stack (size_) ;
	}

	FLAG alloc (CREF<LENGTH> size_) const {
		return HeapHolder::create (thiz)->alloc (size_) ;
	}

	void free (CREF<FLAG> addr) const {
		return HeapHolder::create (thiz)->free (addr) ;
	}
} ;
struct WrapperLayout {
	LENGTH mRank ;
} ;

template <class...PARAMS>
class Wrapper implement WrapperLayout {
private:
	using RANK = RANK_OF<TYPE<PARAMS...>> ;
	using CAPTURE = Tuple<ARR<FLAG ,ENUM_MAX<RANK ,RANK1>>> ;

protected:
	using WrapperLayout::mRank ;
	CAPTURE mStorage ;

public:
	explicit Wrapper (CREF<KILL<FLAG ,PARAMS>>...params) {
		mRank = RANK::expr ;
		mStorage = {params...} ;
	}

	template <class ARG1>
	void invoke (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return invoke_impl (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	void invoke_impl (CREF<ARG1> func ,TYPE<TYPE<ARG2...>>) const {
		return func (keep[TYPE<XREF<PARAMS>>::expr] (Pointer::make (mStorage.m1st[ARG2::expr]))...) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		return invoke (func) ;
	}
} ;

template <class...ARG1>
inline Wrapper<CREF<ARG1>...> MakeWrapper (CREF<ARG1>...params) {
	return Wrapper<CREF<ARG1>...> (address (params)...) ;
}

template <class A>
class WrapperIterator {
protected:
	FLAG mBegin ;
	FLAG mEnd ;
	FLAG mPeek ;

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
		return !(good ()) ;
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

struct SliceLayout {
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> create (VREF<SliceLayout> that) ;
	imports CFat<SliceHolder> create (CREF<SliceLayout> that) ;

	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual BOOL equal (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CREF<SliceLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual SliceLayout eos () const = 0 ;
} ;

class Slice implement SliceLayout {
protected:
	using SliceLayout::mBuffer ;
	using SliceLayout::mSize ;
	using SliceLayout::mStep ;

public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	implicit Slice (CREF<ARG1> that) {
		mBuffer = address (that) ;
		mSize = ENUM_DEC<ARRAY_SIZE<ARG1>>::expr ;
		mStep = ALIGN_OF<ARRAY_ITEM<ARG1>>::expr ;
	}

	explicit Slice (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) {
		mBuffer = buffer ;
		mSize = size_ ;
		mStep = step_ ;
	}

	LENGTH size () const {
		return SliceHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SliceHolder::create (thiz)->step () ;
	}

	STRU32 at (CREF<INDEX> index) const {
		STRU32 ret ;
		SliceHolder::create (thiz)->get (index ,ret) ;
		return move (ret) ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		return at (index) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return !(equal (that)) ;
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

	Slice eos () const {
		SliceLayout ret = SliceHolder::create (thiz)->eos () ;
		return move (keep[TYPE<Slice>::expr] (ret)) ;
	}
} ;

class Exception {
protected:
	Slice mWhat ;
	Slice mFunc ;
	Slice mFile ;
	Slice mLine ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice> what_) {
		mWhat = what_ ;
	}

	explicit Exception (CREF<Slice> what_ ,CREF<Slice> func_) {
		mWhat = what_ ;
		mFunc = func_ ;
	}

	explicit Exception (CREF<Slice> what_ ,CREF<Slice> func_ ,CREF<Slice> file_ ,CREF<Slice> line_) {
		mWhat = what_ ;
		mFunc = func_ ;
		mFile = file_ ;
		mLine = line_ ;
	}

	Slice what () const {
		return mWhat ;
	}

	Slice func () const {
		return mFunc ;
	}

	Slice file () const {
		return mFile ;
	}

	Slice line () const {
		return mLine ;
	}

	declspec (noreturn) void raise () {
		throw thiz ;
	}
} ;

struct FUNCTION_unimplemented {
	forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

struct ReflectClone implement Interface {
	virtual void clone (VREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 110 ;
	}
} ;

template <class A>
class ReflectCloneBinder implement ReflectClone {
public:
	void clone (VREF<Pointer> a ,CREF<Pointer> b) const override {
		return clone_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_CLONE<ARG1>>>
	forceinline void clone_impl (CREF<typeof (PH2)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assign (a ,b) ;
	}

	template <class ARG1>
	forceinline void clone_impl (CREF<typeof (PH1)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
	}
} ;

struct ReflectEqual implement Interface {
	virtual BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 111 ;
	}
} ;

template <class A>
class ReflectEqualBinder implement ReflectEqual {
public:
	BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return equal_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_EQUAL<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_equal (a ,b) ;
	}

	template <class ARG1>
	forceinline BOOL equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
		return BOOL () ;
	}
} ;

struct ReflectCompr implement Interface {
	virtual FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 112 ;
	}
} ;

template <class A>
class ReflectComprBinder implement ReflectCompr {
public:
	FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return equal_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_COMPR<ARG1>>>
	forceinline FLAG equal_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_compr (a ,b) ;
	}

	template <class ARG1>
	forceinline FLAG equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assert (FALSE) ;
		return FLAG () ;
	}
} ;

struct ReflectVisit implement Interface {
	virtual void visit (CREF<Visitor> visitor ,CREF<Pointer> a) const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 113 ;
	}
} ;

template <class A>
class ReflectVisitBinder implement ReflectVisit {
public:
	void visit (CREF<Visitor> visitor ,CREF<Pointer> a) const override {
		return visit_impl (PHX ,visitor ,keep[TYPE<A>::expr] (a)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_VISIT<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return inline_visit (visitor ,a) ;
	}

	template <class ARG1>
	forceinline void visit_impl (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		assert (FALSE) ;
	}
} ;

struct ReflectGuid implement Interface {
	virtual FLAG type_guid () const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 114 ;
	}
} ;

template <class...>
trait REFLECT_GUID_HELP ;

template <class A ,class OTHERWISE>
trait REFLECT_GUID_HELP<A ,OTHERWISE> {
	using RET = ENUM<0> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_BOOL<A>>> {
	using RET = ENUM<1> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_VALUE<A>>> {
	using RET = ENUM<2> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_FLOAT<A>>> {
	using RET = ENUM<3> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_TEXT<A>>> {
	using RET = ENUM<4> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_BYTE<A>>> {
	using RET = ENUM<5> ;
} ;

template <class A>
trait REFLECT_GUID_HELP<A ,REQUIRE<IS_NULL<A>>> {
	using RET = ENUM<6> ;
} ;

template <class A>
using REFLECT_GUID = typename REFLECT_GUID_HELP<A ,ALWAYS>::RET ;

template <class A>
class ReflectGuidBinder implement ReflectGuid {
public:
	FLAG type_guid () const override {
		const auto r1x = REFLECT_GUID<A>::expr ;
		if (r1x != 0)
			return r1x ;
		return inline_hold (thiz) ;
	}
} ;

struct ReflectName implement Interface {
	virtual Slice type_name () const = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 115 ;
	}
} ;

using DEFAULT_SLICE_SIZE = ENUM<4096> ;

#ifdef __CSC_CXX_RTTI__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice type_name () const override {
		const auto r1x = inline_type_name (typeid (A)) ;
		return Slice (r1x ,DEFAULT_SLICE_SIZE::expr ,1).eos () ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice type_name () const override {
		return Slice (__macro_funcion) ;
}
} ;
#endif

struct ClazzPureLayout ;

struct ClazzLayout {
	Ref<ClazzPureLayout> mThis ;
} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> create (VREF<ClazzLayout> that) ;
	imports CFat<ClazzHolder> create (CREF<ClazzLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<ClazzLayout> that) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_guid () const = 0 ;
	virtual Slice type_name () const = 0 ;
	virtual BOOL equal (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ClazzLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class ClazzUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectGuidBinder<A>::expr)
			return inline_hold (ReflectGuidBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_hold (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

class Clazz implement ClazzLayout {
protected:
	using ClazzLayout::mThis ;

public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPE<ARG1>) {
		auto &&tmp = memorize ([&] () {
			Clazz ret ;
			ClazzHolder::create (ret)->initialize (ClazzUnknownBinder<ARG1> ()) ;
			return move (ret) ;
		}) ;
		mThis = Ref<ClazzPureLayout>::reference (Pointer::make (address (tmp.mThis.self))) ;
	}

	implicit Clazz (CREF<Clazz> that) {
		ClazzHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Clazz> operator= (CREF<Clazz> that) {
		return assign (thiz ,that) ;
	}

	implicit Clazz (RREF<Clazz> that) = default ;

	forceinline VREF<Clazz> operator= (RREF<Clazz> that) = default ;

	LENGTH type_size () const {
		return ClazzHolder::create (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::create (thiz)->type_align () ;
	}

	FLAG type_guid () const {
		return ClazzHolder::create (thiz)->type_guid () ;
	}

	Slice type_name () const {
		return ClazzHolder::create (thiz)->type_name () ;
	}

	BOOL equal (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Clazz> that) const {
		return !(equal (that)) ;
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

struct AutoLayout {
	BoxLayout mValue ;
} ;

struct AutoHolder implement Interface {
	imports VFat<AutoHolder> create (VREF<AutoLayout> that) ;
	imports CFat<AutoHolder> create (CREF<AutoLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void poll (CREF<Unknown> holder ,VREF<BoxLayout> item) = 0 ;
} ;

class Auto implement AutoLayout {
private:
	using FAKE_SIZE = ENUM<1024 - 8> ;
	using FAKE_ALIGN = RANK8 ;

protected:
	using AutoLayout::mValue ;
	Temp<Storage<FAKE_SIZE ,FAKE_ALIGN>> mStorage ;

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
	ARG1 poll (TYPE<ARG1>) {
		auto rax = Box<ARG1> () ;
		AutoHolder::create (thiz)->poll (BoxUnknownBinder<ARG1> () ,rax) ;
		return move (rax.self) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () rightvalue {
		return poll (TYPE<ARG1>::expr) ;
	}
} ;

template <class A>
class Scope {
protected:
	PTR<A> mThat ;
	BOOL mOwner ;

public:
	implicit Scope () = delete ;

	explicit Scope (XREF<A> that) :mThat ((&that)) {
		mThat->enter () ;
		mOwner = TRUE ;
	}

	implicit ~Scope () noexcept {
		if (!(mOwner))
			return ;
		mThat->leave () ;
		mOwner = FALSE ;
	}

	implicit Scope (CREF<Scope> that) = delete ;

	forceinline VREF<Scope> operator= (CREF<Scope> that) = delete ;

	implicit Scope (RREF<Scope> that) = delete ;

	forceinline VREF<Scope> operator= (RREF<Scope> that) = delete ;
} ;
} ;